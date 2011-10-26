#include <complex.hpp>

const int xarray[] = {100, 0, 200, 100, 100, 0, 0, 100, 0, 100, 100, 0, 60, 140, 150};
const int yarray[] = {0, 173, 173, 0, 173, 200, 0, 173, 0, 0, 100, 100, 200, 150, 100};

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	GamScene scene;
	scene.setSceneRect(0, 0, 500, 500);
	GamWorld world(&scene);
	std::vector<Vec2f> pts;
	QList<GamPoint> list;
	int size = sizeof(xarray) / sizeof(int);
	for (int i = 0; i < size; i++) {
		pts.push_back(Vec2f(xarray[i], yarray[i]));
	}
	GamComplexItem c(pts);
	c.setColor(QColor("blue"));
	c.setDensity(1);
	world.add(&c);
	scene.addItem(&c);
	GamRect g(0, 300, 500, 50);
	g.setBrush(QColor("lightblue"));
	g.setRestitution(0.3);
	world.add(&g);
	scene.addItem(&g);

	QGraphicsView view(&scene);
	view.setRenderHint(QPainter::Antialiasing);
	view.show();
	world.start();
	//world.debugDraw->show();
	return app.exec();
}
