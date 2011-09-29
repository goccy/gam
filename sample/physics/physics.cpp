#include <physics.hpp>

PhysicsWorld::PhysicsWorld(void)
{
	scene = new GamScene();
	world = new GamWorld(scene);
	connect(world, SIGNAL(beginContact(GamObject *, GamObject *)),
			this, SLOT(beginContactEvent(GamObject *, GamObject *)));
	connect(world, SIGNAL(endContact(GamObject *, GamObject *)),
			this, SLOT(endContactEvent(GamObject *, GamObject *)));
	scene->setSceneRect(0, 0, 600, 600);
	//scene.setBackgroundColor(new Color("black"));
}

void PhysicsWorld::addStaticObject(void)
{
	QColor *static_block_color = new QColor("#90ee90");
	GamRect *ground = new GamRect(200, 400, 200, 40);
	ground->setBrush(*static_block_color);//TODO => add setColor
	ground->setGraphicsEffect(new QGraphicsDropShadowEffect());
	GamRect *left_block = new GamRect(200, 350, 10, 50);
	left_block->setBrush(*static_block_color);
	GamRect *right_block = new GamRect(400 - 10, 350, 10, 50);
	right_block->setBrush(*static_block_color);
	right_block->setGraphicsEffect(new QGraphicsDropShadowEffect());
	GamEllipse *e = new GamEllipse();
	e->setRectShape(new GamRect(300, 300, 20, 20));
	e->setBrush(QColor("#999900"));
	//e.setGlow();
	ground->setRestitution(0);
	scene->addItem(ground);
	scene->addItem(left_block);
	scene->addItem(right_block);
	scene->addItem(e);
	world->add(left_block);
	world->add(right_block);
	world->add(ground);
	world->add(e);
}

void PhysicsWorld::addDynamicObject(void)
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < 100; i++) {
		//int r = Int.random(100);
		int r = rand() % 100;
		GamRect *rect = new GamRect(350 - r, 300 - r, 10, 10);
		rect->setDensity(1);
		//rect.setFriction(0.3);
		rect->setRestitution(0.1);
		rect->setBrush(QColor("#87ceeb"));//155 - r, 200 - r, 255 - r));
		world->add(rect);
		scene->addItem(rect);
	}
	GamEllipse *e = new GamEllipse();
	e->setRectShape(new GamRect(250, 300, 30, 30));
	e->setBrush(QColor("#eeee00"));
	e->setDensity(1.0);
	//e->setGlow();
	GamText *text = new GamText("Hello World");
	text->setPosition(300, 100);
	text->setDensity(1.0);
	//Texture texture = new Texture("sample/qt.jpg");
	//texture.setRectShape(new Rect(200, 200, 50, 50));
	//texture.setDensity(1);
	world->add(e);
	scene->addItem(e);
	world->add(text);
	scene->addItem(text);
	//world.add(texture);
	//scene.addItem(texture);
}

void PhysicsWorld::show(void)
{
	view = new QGraphicsView(scene);
	view->setRenderHint(QPainter::Antialiasing);
	view->show();
	world->start();
}

void PhysicsWorld::beginContactEvent(GamObject *o1, GamObject *o2)
{
	fprintf(stderr, "beginContactEvent!!\n");
}

void PhysicsWorld::endContactEvent(GamObject *o1, GamObject *o2)
{
	fprintf(stderr, "endContactEvent!!\n");
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	PhysicsWorld world;
	world.addStaticObject();
	world.addDynamicObject();
	world.show();
	return app.exec();
}
