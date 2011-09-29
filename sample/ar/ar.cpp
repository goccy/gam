#include <ar.hpp>

AR::AR(void)
{
	scene = new GamScene();
	world = new GamWorld(scene);
	camera = new GamCamera(0);
	background = camera->queryFrame();
	scene->addItem(background);
	srand((unsigned)time(NULL));
	for (int i = 0; i < 10; i++) {
		int rand_x = rand() % 1200;
		int rand_y = rand() % 200;
		GamTexture *logo = new GamTexture("qt.jpg");
		logo->setRectShape(&GamRect(rand_x, rand_y, 50, 50));
		logo->setOpacity(0.9);
		logo->setDensity(1);
		logo->setRestitution(0.7);
		logo->setZValue(4);
		world->add(logo);
		scene->addItem(logo);
	}
	prev_list = new QList<GamComplexItem *>();
	startTimer(1000 / 40);
}

void AR::show(void)
{
	QGraphicsView *view = new QGraphicsView(scene);
	view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	view->setRenderHint(QPainter::Antialiasing);
	view->show();
	world->start();
}

void AR::timerEvent(QTimerEvent *event)
{
	if (prev_texture != NULL) {
		scene->removeItem(prev_texture);
	}
	GamTexture *t = camera->queryFrame();
	QList<GamVector> *objs = t->detectHuman(background);
	int objs_size = objs->length();
	int list_size = prev_list->length();
	for (int i = 0; i < list_size; i++) {
		world->remove(prev_list->at(i));
	}
	prev_list->clear();
	scene->addItem(t);
	for (int i = 0; i < objs_size; i++) {
		std::vector<Vec2f> obj = objs->at(i);
		GamComplexItem *item = new GamComplexItem(obj, obj.size());
		item->setColor(&QColor("#00ff00"));
		item->setBullet(true);
		world->add(item);
		prev_list->append(item);
	}
	prev_texture = t;
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	AR ar;
	ar.show();
	return app.exec();
}
