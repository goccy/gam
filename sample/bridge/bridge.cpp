#include <bridge.hpp>

Bridge::Bridge()
{
	scene = new GamScene();
	world = new GamWorld(scene);
	scene->setSceneRect(0, 0, 600, 600);
	int i = 0;
	GamRect *prev = new GamRect(10, 200, 20, 10);
	prev->setBrush(QColor("#00eeee"));
	scene->addItem(prev);
	world->add(prev);
	for (i = 1; i < 20; i++) {
		GamRect *next = new GamRect(10 + i * 20, 200, 20, 10);
		next->setBrush(QColor("#00eeee"));
		next->setDensity(1);
		scene->addItem(next);
		world->add(next);
		GamRevoluteJoint *j = new GamRevoluteJoint(prev, next);
		//GamPoint anchorA(10 + i * 20, 200);
		//GamPoint anchorB(10 + i * 20, 200);
		//GamDistanceJoint *j = new GamDistanceJoint(prev, anchorA, next, anchorB);
		//j->setLength(20);
		//j->setDampingRatio(0);
		//j->setFrequencyHz(0);
		scene->addItem(j);
		world->add(j);
		prev = next;
	}
	GamRect *next = new GamRect(10 + i * 20, 200, 20, 10);
	next->setBrush(QColor("#00eeee"));
	scene->addItem(next);
	world->add(next);
	GamRevoluteJoint *j = new GamRevoluteJoint(prev, next);
	scene->addItem(j);
	world->add(j);
}

void Bridge::show(void)
{
	QGraphicsView *view = new QGraphicsView(scene);
	view->setRenderHint(QPainter::Antialiasing);
	view->show();
	world->start();
	//world->debugDraw->show();
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	Bridge b;
	b.show();
	return app.exec();
}
