#include <mapping.hpp>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
	scene = NULL;
	view = NULL;
	startTimer(1000 / 30);
}

void GLWidget::initializeGL(void)
{
	glClearColor(0xff, 0xff, 0xff, 0x00);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(0, 600, 600, 0, 2, 10);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	const GLfloat lightPos[] = {100, 100, -5, 0};
	const GLfloat lightCol[] = {1, 1, 1, 1};
	//glLightfv(GL_LIGHT0 , GL_POSITION , lightPos);
	//glLightfv(GL_LIGHT0 , GL_DIFFUSE , lightCol);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	//gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
}

static int CalculateNormal(const float p1[3], const float p2[3], const float p3[3], float n[3])
{
	float v1[3];
	float v2[3];
	float cross[3];
	int i;
	for (i = 0; i < 3; i++) {
		v1[i] = p1[i] - p2[i];
	}
	for (i = 0; i < 3; i++) {
		v2[i] = p3[i] - p2[i];
	}
	for (i = 0; i < 3; i++) {
		cross[i] = v2[(i+1)%3] * v1[(i+2)%3] - v2[(i+2)%3] * v1[(i+1)%3];
	}
	float length = sqrtf(cross[0] * cross[0] + cross[1] * cross[1] + cross[2] * cross[2]);
	if (length == 0.0f) {
		return 0;
	}
	for (i = 0; i < 3; i++) {
		n[i] = cross[i] / length;
	}
	return 1;
}

void GLWidget::timerEvent(QTimerEvent *event)
{
	glDraw();
}

void GLWidget::paintGL(void)
{
	QPainter p(this);
	p.beginNativePainting();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	GLuint flower = bindTexture(QPixmap("flower.jpg"), GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, flower);
	glBegin(GL_TRIANGLES);
	//glEdgeFlag(GL_TRUE);
	float normal[3] = {0};
	float p0[3][3] = {{10, 10, -7}, {100, 100, -2}, {10, 200, -2}};
	float p1[3][3] = {{100, 100, -2}, {200, 10, -3}, {100, 100, -2}};
	float p2[3][3] = {{10, 200, -2}, {200, 200, -2}, {200, 200, -2}};

	CalculateNormal(p0[0], p1[0], p2[0], normal);
	//fprintf(stderr, "normal : (x, y, z) = (%f, %f, %f)\n", normal[0], normal[1], normal[2]);
	glNormal3f(normal[0], normal[1], normal[2]);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(p0[0][0], p0[0][1], p0[0][2]);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(p1[0][0], p1[0][1], p1[0][2]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(p2[0][0], p2[0][1], p2[0][2]);

	CalculateNormal(p0[1], p1[1], p2[1], normal);
	//fprintf(stderr, "normal : (x, y, z) = (%f, %f, %f)\n", normal[0], normal[1], normal[2]);
	glNormal3f(normal[0], normal[1], normal[2]);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(p0[1][0], p0[1][1], p0[1][2]);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(p1[1][0] , p1[1][1], p1[1][2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(p2[1][0] , p2[1][1], p2[1][2]);

	CalculateNormal(p0[2], p1[2], p2[2], normal);
	//fprintf(stderr, "normal : (x, y, z) = (%f, %f, %f)\n", normal[0], normal[1], normal[2]);
	glNormal3f(normal[0], normal[1], normal[2]);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(p0[2][0] , p0[2][1], p0[2][2]);
	glTexCoord2f(0.5f, 1.0f);
	glVertex3f(p1[2][0] , p1[2][1], p1[2][2]);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(p2[2][0] , p2[2][1], p2[2][2]);

	glEnd();

	//glBindTexture(GL_TEXTURE_2D, 0);
	//deleteTexture(flower);
	//glDisable(GL_TEXTURE_2D);
	//glDisable(GL_NORMALIZE);
	//glFlush();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();
	p.endNativePainting();
	if (view) {
		view->render(&p);
	}
}

void GLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	this->width = width;
	this->height = height;
}

void GLWidget::setScene(QGraphicsScene *scene)
{
	this->scene = scene;
}

void GLWidget::setView(MyView *view)
{
	this->view = view;
}

void MyView::paintEvent(QPaintEvent *event)
{
	fprintf(stderr, "CALLED PAINT EVENT\n");
	QGraphicsView::paintEvent(event);
}

//void MyView::timerEvent(QTimerEvent *event)
//{
//
//}

MyView::MyView(QGraphicsScene *scene) : QGraphicsView(scene)
{
	//startTimer(1000 / 30);
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QGraphicsRectItem *r = new QGraphicsRectItem();
	r->setBrush(QColor("#ff0000"));
	r->setRect(100, 100, 50, 50);
	QGraphicsScene *scene = new QGraphicsScene();
	scene->addItem(r);
	scene->setSceneRect(0, 0, 600, 600);
	MyView *view = new MyView(scene);
	GLWidget *widget = new GLWidget(0);
	view->setViewport(widget);
	widget->setView(view);
	view->show();
	return app.exec();
}
