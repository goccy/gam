#include <gam.hpp>

GamWobble::GamWobble(QWidget *parent) : QGLWidget(parent)
{
	pts = NULL;
	view = NULL;
	scene = NULL;
	ripple_count = 0;
}

void GamWobble::initializeGL(void)
{
	glClearColor(0xff, 0xff, 0xff, 0x00);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(0, 600, 600, 0, 2, 10);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void GamWobble::updatePoint(QList<GamVectorIdx> *pts)
{
	this->pts = pts;
	//glDraw();
	//QPixmap pixmap = renderPixmap(600, 600);
	//QImage image = pixmap.toImage();
	//QImage image = grabFrameBuffer();
	//image.save("hoge.jpg");
}

void GamWobble::draw(void)
{
	glDraw();
}

void GamWobble::rippleEffect(QPainter *p)
{
	if (ripple_count > 100) {
		ripple_count = 0;
	}
	//====================== for water effect =========================//
	QImage image = grabFrameBuffer(true);//current frame buffer
	image = image.convertToFormat(QImage::Format_ARGB32);
	int height = image.height();
	int width = image.width();
	for (int y = 0; y < height; ++y) {
		QRgb *row = (QRgb*)image.scanLine(y);
		for (int x = 0; x < width; ++x) {
			unsigned char *r = (unsigned char*)&row[x];
			if (r[0] == 0xff && r[1] == 0xff && r[2] == 0xff) {
				r[3] = 0x00;
			}
		}
	}
	QPixmap pixmap = QPixmap::fromImage(image, Qt::DiffuseAlphaDither);
	int original_height = pixmap.height();
	int original_width = pixmap.width();
	pixmap = pixmap.scaled(600 * (1 + 5/100.0f), 600 * (1 + 5/100.0f));//scale up
	p->setCompositionMode(QPainter::CompositionMode_SourceOver);
	QPainterPath path;
	int x = 100;
	int y = 100;
	int w = 100;
	int h = 100;
	float ripple_delta = 1 + ripple_count * 0.1;
	int center_x = x + w/2;
	int center_y = y + h/2;
	int after_width = w * ripple_delta;
	int after_height = h * ripple_delta;
	int after_center_x = x + after_width/2;
	int after_center_y = y + after_height/2;
	int div_x = after_center_x - center_x;
	int div_y = after_center_y - center_y;
	path.addEllipse(x - div_x, y - div_y, after_width, after_height);
	int small_circle_width = after_width * 0.9;
	int small_circle_height = after_height * 0.9;
	int dx = (after_width - small_circle_width) / 2;
	int dy = (after_height - small_circle_height) / 2;
	//fprintf(stderr, "(dx, dy) = (%d, %d)\n", dx, dy);
	path.addEllipse(x - div_x + dx, y - div_y + dy, small_circle_width, small_circle_height);
	p->setClipPath(path);
	//fprintf(stderr, "(width, height) = (%d, %d)\n", pixmap.width(), pixmap.height());
	//fprintf(stderr, "(dx, dy) = (%d, %d)\n", (original_width - pixmap.width())/2, (original_height - pixmap.height())/2);
	p->drawPixmap((original_width - pixmap.width())/4, (original_height - pixmap.height())/4, pixmap);
	ripple_count++;
}

void GamWobble::wobbleEffect(QPainter *p)
{
	p->beginNativePainting();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	GLuint flower = bindTexture(QPixmap("qt.jpg"), GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, flower);

	int size = pts->length();
	glBegin(GL_QUADS);
	//fprintf(stderr, "================\n");
	for (int i = 0; i < size; i++) {
		GamVectorIdx vec_idx = pts->at(i);
		int idx = vec_idx.idx;
		//fprintf(stderr, "idx = [%d]\n", idx);
		GamVector pt = vec_idx.vec;
		int mod_idx = idx % 8;
		int div_idx = idx / 8;
		//fprintf(stderr, "mod_idx = [%d]\n", mod_idx);
		//fprintf(stderr, "div_idx = [%d]\n", div_idx);
		glTexCoord2f(mod_idx * 1/7.0f, 1.0f - div_idx/7.0f + 6/7.0f);
		//fprintf(stderr, "pt1: (x, y) = (%f, %f)\n", pt.at(2).x, pt.at(2).y);
		glVertex3f(pt.at(2).x, pt.at(2).y, 0.0f);
		glTexCoord2f(mod_idx * 1/7.0f + 1/7.0f, 1.0f - div_idx/7.0f + 6/7.0f);
		//fprintf(stderr, "pt2: (x, y) = (%f, %f)\n", pt.at(1).x, pt.at(1).y);
		glVertex3f(pt.at(1).x, pt.at(1).y, 0.0f);
		glTexCoord2f(mod_idx * 1/7.0f + 1/7.0f, 1.0f - div_idx/7.0f + 7/7.0f);
		//fprintf(stderr, "pt3: (x, y) = (%f, %f)\n", pt.at(3).x, pt.at(3).y);
		glVertex3f(pt.at(3).x, pt.at(3).y, 0.0f);
		glTexCoord2f(mod_idx * 1/7.0f, 1.0f - div_idx/7.0f + 7/7.0f);
		//fprintf(stderr, "pt4: (x, y) = (%f, %f)\n", pt.at(0).x, pt.at(0).y);
		glVertex3f(pt.at(0).x, pt.at(0).y, 0.0f);
	}
	glEnd();
	pts = NULL;
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();
	p->endNativePainting();
}

QPainterPath *g_path = NULL;

void GamWobble::paintGL(void)
{
	QPainter p;
	p.begin(this);
	if (pts) {
		wobbleEffect(&p);
	}
	if (scene) {
		int original_height = 0;
		int original_width = 0;
		QPixmap pixmap;
		QList<QGraphicsItem*> items = scene->items(Qt::AscendingOrder);
		g_path = new QPainterPath();
		foreach (QGraphicsItem *item, items) {
			item->paint(&p, NULL);
		}
		delete g_path;
		g_path = new QPainterPath();
		if (view && view->waterRendering) {
			QImage image = grabFrameBuffer(true);//current frame buffer
			image = image.convertToFormat(QImage::Format_ARGB32);
			int height = image.height();
			int width = image.width();
			//fprintf(stderr, "=====================\n");
			for (int y = 0; y < height; ++y) {
				QRgb *row = (QRgb*)image.scanLine(y);
				for (int x = 0; x < width; ++x) {
					unsigned char *r = (unsigned char*)&row[x];
					if (r[0] == 0xff && r[1] == 0xff && r[2] == 0xff) {
						r[3] = 0x00;
					}
					//r[0] = 0;
					//r[1] = 0;
					//r[2] = 0;
				}
			}
			pixmap = QPixmap::fromImage(image, Qt::DiffuseAlphaDither);
			original_height = pixmap.height();
			original_width = pixmap.width();
			pixmap = pixmap.scaled(600 * (1 + 20/100.0f), 600 * (1 + 20/100.0f));//scale up
			//pixmap = pixmap.scaled(600 * (1 + 5/100.0f), 600 * (1 + 5/100.0f));//scale up
			p.setCompositionMode(QPainter::CompositionMode_SourceOver);
		}
		//QList<QGraphicsItem*> items = scene->items(Qt::AscendingOrder);
		//foreach (QGraphicsItem *item, items) {
		//item->paint(&p, NULL);
		//}
		if (view && view->waterRendering) {
			//QPainterPath path = p.clipPath();
			p.setClipPath(*g_path);
			p.drawPixmap((original_width - pixmap.width())/2, (original_height - pixmap.height())/2, pixmap);
			delete g_path;
			g_path = NULL;
		}
	}
	//rippleEffect(&p);
	p.end();
	swapBuffers();//MOST IMPORTANT CODE
}

void GamWobble::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	this->width = width;
	this->height = height;
}
