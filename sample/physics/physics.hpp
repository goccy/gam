#include <gam.hpp>

class PhysicsWorld : public QObject {
	Q_OBJECT;
public:
	GamWorld *world;
	GamScene *scene;
	QGraphicsView *view;

	PhysicsWorld(void);
	void addStaticObject(void);
	void addDynamicObject(void);
	void show(void);
public slots:
	void beginContactEvent(GamObject *o1, GamObject *o2);
	void endContactEvent(GamObject *o1, GamObject *o2);
};

