#include <joint.hpp>

PhysicsWorld::PhysicsWorld(void)
{
	scene = new GamScene();
	world = new GamWorld(scene);
	scene->setSceneRect(0, 0, 600, 600);
	//scene.setBackgroundColor(new Color("black"));
	GamRect *r = new GamRect(0, 500, 600, 20);
	r->setBrush(QColor("#87ceeb"));
	r->setFriction(0.8);
	world->add(r);
	scene->addItem(r);
}

void PhysicsWorld::addDistanceJointObject(void)
{
	GamRect *r1 = new GamRect(100, 100, 40, 40);
	//r1->setDensity(1);
	r1->setFriction(0.8);
	r1->setBrush(QColor("#90ee90"));
	GamRect *r2 = new GamRect(100, 200, 50, 100);
	r2->setDensity(1);
	r2->setFriction(0.8);
	r2->setBrush(QColor("#87ceeb"));
	world->add(r1);
	world->add(r2);
	scene->addItem(r1);
	scene->addItem(r2);
	GamDistanceJoint *j = new GamDistanceJoint(r1, r2);
	world->add(j);
	scene->addItem(j);
}

void PhysicsWorld::addRevoluteJointObject(void)
{
	GamRect *r = new GamRect(300, 100, 40, 40);
	r->setBrush(QColor("#90ee90"));
	GamEllipse *e = new GamEllipse();
	e->setRectShape(&GamRect(300, 200, 30, 30));
	e->setDensity(1);
	e->setBrush(QColor("#87ceeb"));
	world->add(r);
	world->add(e);
	scene->addItem(r);
	scene->addItem(e);
	GamRevoluteJoint *j = new GamRevoluteJoint(r, e);
	world->add(j);
	scene->addItem(j);
}

void PhysicsWorld::addPrismaticJointObject(void)
{
	GamRect *r = new GamRect(100, 300, 40, 40);
	r->setBrush(QColor("#90ee90"));
	GamEllipse *e = new GamEllipse();
	e->setRectShape(&GamRect(100, 400, 30, 30));
	e->setDensity(1);
	e->setBrush(QColor("#87ceeb"));
	world->add(r);
	world->add(e);
	scene->addItem(r);
	scene->addItem(e);
	GamPrismaticJoint *j = new GamPrismaticJoint(r, e);
	world->add(j);
	scene->addItem(j);
}

void PhysicsWorld::addPulleyJointObject(void)
{
	GamRect *r = new GamRect(300, 400, 40, 40);
	r->setDensity(1);
	r->setBrush(QColor("#90ee90"));
	GamRect *r2 = new GamRect(400, 400, 40, 40);
	r2->setDensity(1);
	r2->setBrush(QColor("#87ceeb"));
	world->add(r);
	world->add(r2);
	scene->addItem(r);
	scene->addItem(r2);
	GamPulleyJoint *j = new GamPulleyJoint(r, GamPoint(300, 300), r2, GamPoint(400, 300));
	world->add(j);
	QPen pen;
	pen.setColor(QColor("#ee82ee"));
	j->setPen(pen);
	scene->addItem(j);
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
	world.addDistanceJointObject();
	world.addRevoluteJointObject();
	world.addPrismaticJointObject();
	world.addPulleyJointObject();
	world.show();
	return app.exec();
}
