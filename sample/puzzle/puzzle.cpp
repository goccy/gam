#include <puzzle.hpp>

#define MAX_PANEL_NUM  36
#define HALF_PANEL_NUM sqrt(MAX_PANEL_NUM)
#define PANEL_SIZE  50
#define PANEL_OFFSET 100

Puzzle::Puzzle()
{
	scene = new GamScene();
	scene->setSceneRect(0, 0, 1200, 700);
	camera = new GamCamera(0);
	panels = new QList<GamRect *>();
	//v = new Video("sample/sample.avi");
	for (int i = 0; i < MAX_PANEL_NUM; i++) {
		Panel *panel = new Panel(0, 0, PANEL_SIZE, PANEL_SIZE);
		panels->append(panel);
		scene->addItem(panel);
	}
	int panel_num = 0;
	for (int i = 0; i < HALF_PANEL_NUM; i++) {
		for (int j = 0; j < HALF_PANEL_NUM; j++) {
			if (panel_num > MAX_PANEL_NUM) break;
			panels->at(panel_num)->translate(PANEL_OFFSET * i + PANEL_SIZE,
											 PANEL_OFFSET * j + PANEL_SIZE);
			panel_num++;
		}
	}
	startTimer(1000 / 40);
}

void Puzzle::show(void)
{
	QGraphicsView *view = new QGraphicsView(scene);
	view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	view->setRenderHint(QPainter::Antialiasing);
	view->show();
}

void Puzzle::timerEvent(QTimerEvent *event)
{
	//Texture t = v.queryFrame();
	GamTexture *t = camera->queryFrame();
	QList<GamTexture *> *textures = t->split(HALF_PANEL_NUM, HALF_PANEL_NUM);
	int size = textures->length();
	for (int i = 0; i < size; i++) {
		textures->at(i)->setRectShape(&GamRect(0, 0, PANEL_SIZE, PANEL_SIZE));
		panels->at(i)->setTexture(textures->at(i));
	}
	//TODO textures ==> needs to delete
}

Panel::Panel(int x, int y, int width, int height) : GamRect(x, y, width, height)
{

}

void Panel::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPoint p = event->lastScreenPos();
	prev_point = p;
}

void Panel::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPoint p = event->lastScreenPos();
	int dx = p.x() - prev_point.x();
	int dy = p.y() - prev_point.y();
	translate(dx, dy);
	prev_point = p;
}

void Panel::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	//Point p = event.lastScreenPos();
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	Puzzle p;
	p.show();
	return app.exec();
}
