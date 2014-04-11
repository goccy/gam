#include <water.hpp>

WaterWorld::WaterWorld(void)
{
	scene = new GamScene();
	scene->setSceneRect(0, 0, 600, 600);
	GamTexture *t = new GamTexture("flower.jpg");
	t->setRectShape(&GamRect(0, 0, 600, 600));
	t->setZValue(-1);
	scene->addItem(t);
	view = new GamView(scene);
	world = new GamWorld(scene);
}

void WaterWorld::addGround(void)
{
	GamRect *ground = new GamRect(200, 400, 200, 40);
	ground->setBrush(QColor("#00ff00"));
	GamRect *left_block = new GamRect(200, 350, 10, 50);
	left_block->setBrush(QColor("#00ff00"));
	GamRect *right_block = new GamRect(400 - 10, 350, 10, 50);
	right_block->setBrush(QColor("#00ff00"));
	ground->setRestitution(0);
	scene->addItem(ground);
	scene->addItem(left_block);
	scene->addItem(right_block);
	world->add(left_block);
	world->add(right_block);
	world->add(ground);
}

void WaterWorld::addWater(void)
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 400; i++) {
		int r = rand() % 100;
		GamEllipse *e = new GamEllipse();
		e->setRectShape(&GamRect(350 - r, 300 - r, 5, 5));
		e->setBrush(QColor("#0000ff"));
		e->setPen(Qt::NoPen);
		e->setDensity(1);
		e->convertToWater();
		world->add(e);
		scene->addItem(e);
	}
}

void WaterWorld::show(void)
{
	view->setRenderHint(QPainter::Antialiasing);
	view->setGLRendering(true);
	view->setWaterRendering(true);
	view->show();
	world->start();
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	WaterWorld ww;
	ww.addGround();
	ww.addWater();
	ww.show();
	return app.exec();
}
