#include <gam.hpp>

class PhysicsWorld : public QObject {
	Q_OBJECT;
public:
	GamWorld *world;
	GamScene *scene;
	QGraphicsView *view;

	PhysicsWorld(void);
	void addJointObject(void);
	void show(void);
};

