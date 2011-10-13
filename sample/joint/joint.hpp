#include <gam.hpp>

class PhysicsWorld : public QObject {
	Q_OBJECT;
public:
	GamWorld *world;
	GamScene *scene;
	QGraphicsView *view;

	PhysicsWorld(void);
	void addDistanceJointObject(void);
	void addRevoluteJointObject(void);
	void addPrismaticJointObject(void);
	void addPulleyJointObject(void);
	void show(void);
};

