#include <net.hpp>

WobbleTexture::WobbleTexture()
{
	scene = new GamScene();
	scene->setSceneRect(0, 0, 600, 600);
	world = new GamWorld(scene);
	//world->setGravity(0.0f, 0.0f);
	pts = new QList<GamEllipse*>();
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			GamEllipse *e = new GamEllipse();
			e->body_userdata->setTag(GamWobbleItemTag);
			e->body_userdata->idx = y * 8 + x;
			//fprintf(stderr, "tag = [%d]\n", e->tag());
			e->setBrush(QColor("#ff00ff"));
			e->setRectShape(&GamRect(100 + x * 30, 100 + y * 30, 10, 10));
			e->setDensity(1);
			//e->setOpacity(0.0);
			e->setOpacity(0.01);
			scene->addItem(e);
			world->add(e);
			b2MassData *data = new b2MassData();
			e->body->GetMassData(data);
			data->mass = 0;
			e->body->SetMassData(data);
			pts->append(e);
		}
	}
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 7; x++) {
			GamEllipse *prev = pts->at(8 * y + x);
			GamEllipse *next = pts->at(8 * y + x + 1);
			GamDistanceJoint *j = new GamDistanceJoint(prev, next);
			j->setLength(30);
			//scene->addItem(j);
			world->add(j);
		}
	}
	for (int y = 0; y < 7; y++) {
		for (int x = 0; x < 8; x++) {
			GamEllipse *prev = pts->at(8 * y + x);
			GamEllipse *next = pts->at(8 * (y+1) + x);
			GamDistanceJoint *j = new GamDistanceJoint(prev, next);
			j->setLength(30);
			//scene->addItem(j);
			world->add(j);
		}
	}
	for (int y = 0; y < 7; y++) {
		for (int x = 0; x < 7; x++) {
			GamEllipse *prev = pts->at(8 * y + x);
			GamEllipse *next = pts->at(8 * (y+1) + (x+1));
			GamDistanceJoint *j = new GamDistanceJoint(prev, next);
			j->setLength(40);
			//scene->addItem(j);
			world->add(j);
		}
	}
	for (int y = 0; y < 7; y++) {
		for (int x = 0; x < 7; x++) {
			GamEllipse *prev = pts->at(8 * y + x+1);
			GamEllipse *next = pts->at(8 * (y+1) + x);
			GamDistanceJoint *j = new GamDistanceJoint(prev, next);
			j->setLength(40);
			//scene->addItem(j);
			world->add(j);
		}
	}
	GamRect *rect = new GamRect(250, 50, 50, 50);
	rect->setBrush(QColor("#00ffff"));
	//rect->setDensity(1);
	scene->addItem(rect);
	world->add(rect);
	GamDistanceJoint *d = new GamDistanceJoint(rect, pts->at(4));
	d->setLength(100);
	d->setDampingRatio(0.7);
	scene->addItem(d);
	world->add(d);
}

void WobbleTexture::makeCage(int width, int height)
{
	GamRect *left = new GamRect(0, 0, 5, height);
	GamRect *right = new GamRect(width - 5, 0, 5, height);
	GamRect *top = new GamRect(0, 0, width, 5);
	GamRect *bottom = new GamRect(0, height - 5, width, 5);
	left->setBrush(QColor("#00ff00"));
	right->setBrush(QColor("#00ff00"));
	top->setBrush(QColor("#00ff00"));
	bottom->setBrush(QColor("#00ff00"));
	scene->addItem(left);
	scene->addItem(right);
	scene->addItem(top);
	scene->addItem(bottom);
	world->add(left);
	world->add(right);
	world->add(top);
	world->add(bottom);
}

void WobbleTexture::show(void)
{
	GamView *view = new GamView(scene);
	view->setGLRendering(true);
	view->show();
	world->start();
	//world->debugDraw->show();
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	WobbleTexture tex;
	tex.makeCage(600, 600);
	tex.show();
	return app.exec();
}
