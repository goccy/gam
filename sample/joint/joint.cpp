#include <joint.hpp>

PhysicsWorld::PhysicsWorld(void)
{
	scene = new GamScene();
	world = new GamWorld(scene);
	scene->setSceneRect(0, 0, 600, 600);
	//scene.setBackgroundColor(new Color("black"));
}

void PhysicsWorld::addJointObject(void)
{
	GamRect *r1 = new GamRect(200, 150, 40, 40);
	r1->setDensity(1);
	r1->setFriction(0.8);
	r1->setBrush(QColor("#90ee90"));
	GamRect *r2 = new GamRect(200, 300, 50, 100);
	r2->setDensity(1);
	r2->setFriction(0.8);
	r2->setBrush(QColor("#87ceeb"));
	world->add(r1);
	world->add(r2);
	scene->addItem(r1);
	scene->addItem(r2);
	GamDistanceJoint *j = new GamDistanceJoint(r1, r2);
	world->add(j);
	GamTexture *texture = new GamTexture("qt.jpg");
	texture->setRectShape(new GamRect(200, 200, 50, 50));
	texture->setFriction(0.8);
	texture->setDensity(1);
	world->add(texture);
	scene->addItem(texture);
	GamRect *r3 = new GamRect(0, 500, 600, 20);
	r3->setBrush(QColor("#87ceeb"));
	r3->setFriction(0.8);
	world->add(r3);
	scene->addItem(r3);
}

void PhysicsWorld::show(void)
{
	view = new QGraphicsView(scene);
	view->setRenderHint(QPainter::Antialiasing);
	view->show();
	world->start();
	//world->debugDraw->show();
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	PhysicsWorld world;
	world.addJointObject();
	world.show();
	return app.exec();
}
