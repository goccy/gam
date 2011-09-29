#include <gam.hpp>

class Puzzle : public QObject {
	Q_OBJECT;
public:
	GamScene *scene;
	GamCamera *camera;
	GamVideo *v;
	QList<GamRect *> *panels;

	Puzzle(void);
	void show(void);
	void timerEvent(QTimerEvent *event);
};

class Panel : public GamRect {
	Q_OBJECT;
public:
	QPoint prev_point;

	Panel(int x, int y, int width, int height);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};
