#include <gam.hpp>

class GestureWidget : public QObject {
	Q_OBJECT;
public:
	GamKinect *kinect;
	GamScene *scene;
	GamTexture *prev_texture;
	GamEllipse *handPoint;
	QList<GamEllipse*> *user_particles;
	QList<GamPerson *> *added_persons;
	GamPeople *prev_people;
	GamWorld *world;

	GestureWidget(void);
	bool containsAddedPersons(GamPerson *p);
	void clearUserParticles(void);
	void timerEvent(QTimerEvent *event);
	void show(void);
public slots:
	void startSession(const XnPoint3D pFocus, void *ctx);
	void endSession(void *ctx);
	void push(XnFloat velocity, XnFloat angle, void *ctx);
	void swipe(XnFloat velocity, XnFloat angle, void *ctx);
	void updateHandPosition(GamPoint p, float z);
};
