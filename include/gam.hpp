/*=============== QT ====================*/
#include <QtGui>
/*============== Box2D ==================*/
#include <Box2D.h>
/*============== OpenCV =================*/
#include <cv.h>
#include <highgui.h>
/*========== OpenNI & NITE ==============*/
#define warning(msg) //for escape OpenNI's warning
#include <XnOpenNI.h>
#include <XnVPointControl.h>
#include <XnVSessionManager.h>
#include <XnVPushDetector.h>
#include <XnVSwipeDetector.h>
/*========================================*/
#include <iostream>
#define PTM_RATIO 32.0f

class Vec2f {
public:
	float x;
	float y;
	Vec2f() {
		this->x = 0;
		this->y = 0;
	};
	Vec2f(float x, float y) {
		this->x = x;
		this->y = y;
	}
};

typedef enum {
	GamRectTag,
	GamEllipseTag,
	GamTextureTag,
	GamTextTag,
	GamLineTag,
	GamComplexItemTag,
	GamDistanceJointTag,
	GamRevoluteJointTag,
	GamPrismaticJointTag,
	GamPulleyJointTag,
	GamGearJointTag,
	//GamMouseJointTag,
	//GamWheelJointTag,
	//GamWeldJointTag,
	//GamRopeJointTag,
	//GamFrictionJointTag,
} GamClassDef;

class GamObject {
private:
	int _tag;
public:
	QGraphicsItem *i;
	void *userdata;

	GamObject(void);
	int tag(void);
	void setTag(int id);
};

class GamPoint {
public:
	int x;
	int y;
	GamPoint(int x_, int y_) {
		x = x_;
		y = y_;
	}
};

class GamScene;
class GamContact : public QObject, public b2ContactListener {
	Q_OBJECT;
public:
	GamContact(void);
	void BeginContact(b2Contact *contact);
	void EndContact(b2Contact *contact);
	void PreSolve(b2Contact *contact, const b2Manifold *oldManifold);
	void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);
signals:
	void emitBeginContactSignal(GamObject *o1, GamObject *o2);
	void emitEndContactSignal(GamObject *o1, GamObject *o2);
};

class GamQueryCallback : public b2QueryCallback {
public:
	b2Vec2 m_point;
	b2Fixture* m_fixture;
	GamQueryCallback(const b2Vec2& point);
	bool ReportFixture(b2Fixture* fixture);
};

#include <QtOpenGL>

class GamGL : public QGLWidget, public b2DebugDraw {
	Q_OBJECT;
public:
	float32 mRatio;
	b2World *world;

	GamGL(void);
	GamGL(float32 ratio);
	void show(void);
	void timerEvent(QTimerEvent *event);
	void initializeGL(void);
	void paintGL(void);
	void resizeGL(int width, int height);
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	void DrawTransform(const b2Transform& xf);
    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
    void DrawString(int x, int y, const char* string, ...);
    void DrawAABB(b2AABB* aabb, const b2Color& color);
};

class GamWorld : public QObject, public GamObject {
	Q_OBJECT;
public:
	int timer_id;
	int iteration;
	float timestep;
	b2World *world;
	GamContact *contact;
	GamScene *scene;
	b2Body *mouse_joint_body;
	b2MouseJoint *mouse_joint;
	GamGL *debugDraw;

	GamWorld(GamScene *scene);
	void add(GamObject *o);
	void remove(GamObject *o);
	void start(void);
	void joint(void);
	void timerEvent(QTimerEvent *event);
signals:
	void beginContact(GamObject *o1, GamObject *o2);
	void endContact(GamObject *o1, GamObject *o2);
public slots:
	void beginContactSlot(GamObject *o1, GamObject *o2);
	void endContactSlot(GamObject *o1, GamObject *o2);
	void dragBeginSlot(QGraphicsSceneMouseEvent *event);
	void dragMoveSlot(QGraphicsSceneMouseEvent *event);
	void dragEndSlot(QGraphicsSceneMouseEvent *event);
};

class GamRigidBody {
public:
	bool isStatic;
	qreal _rotation;
	qreal friction;
	qreal density;
	qreal restitution;
	bool bullet;
	GamObject *body_userdata;

	GamRigidBody(void);
	void setBodyUserData(void *userdata);
	void setRot(qreal rotation_);
	void setDensity(qreal density_);
	void setFriction(qreal friction_);
	void setRestitution(qreal restitution_);
	void setBullet(bool bullet_);
};
class GamTexture;

class GamRect : public QObject, public GamObject, public QGraphicsRectItem, public GamRigidBody {
	Q_OBJECT;
public:
	QRect *r;
	bool isDrag;
	int x;
	int y;
	qreal width;
	qreal height;
	QGraphicsDropShadowEffect *se;
	b2Body *body;

	GamRect(int x, int y, int width, int height);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void addToWorld(GamWorld *w);
	void setTexture(GamTexture *t);
	GamPoint *getCenter(void);
	void setSize(float width_, float height_);
	~GamRect(void);
signals:
	void dragBeginSignal(QGraphicsSceneMouseEvent *event);
	void dragMoveSignal(QGraphicsSceneMouseEvent *event);
	void dragEndSignal(QGraphicsSceneMouseEvent *event);
};

class GamEllipse : public QObject, public GamObject, public QGraphicsEllipseItem, public GamRigidBody {
	Q_OBJECT;
public:
	bool isDrag;
	int x;
	int y;
	int width;
	int height;
	QColor *glow_center_color;
	QColor *color;
	b2Body *body;
	bool glow;

	GamEllipse();
	void setRectShape(GamRect *r);
	void setPosition(int x_, int y_);
	void setGlow(void);
	void setGlowCenterColor(QColor *color);
	void addToWorld(GamWorld *w);
};

typedef std::vector<Vec2f> GamVector;

class GamTexture : public QObject, public GamObject, public QGraphicsPixmapItem, public GamRigidBody {
	Q_OBJECT;
public:
	QPixmap *p;
	bool isDrag;
	int x;
	int y;
	qreal width;
	qreal height;
	QGraphicsColorizeEffect *ce;
	b2Body *body;
	IplImage *ipl;

	GamTexture(const char *filepath);
	GamTexture(QImage *image);
	GamTexture(QPixmap *image);
	void setTrackData(const char *filepath);
	void setConnect(void);
	QList<GamVector> *detectHuman(GamTexture *background);
	QList<GamTexture*> *split(int row, int col);
	void setRectShape(GamRect *r);
	void setColor(QColor *c);
	GamPoint *getCenter(void);
	void setSize(float width, float height);
	~GamTexture(void);
	void addToWorld(GamWorld *w);
};

class GamLine : public QObject, public GamObject, public QGraphicsLineItem, public GamRigidBody {
	Q_OBJECT;
public:
	QLine *l;
	QPen *_pen;
	bool isDrag;
	int x1;
	int y1;
	int x2;
	int y2;
	int width;
	int height;
	QGraphicsDropShadowEffect *se;
	b2Body *body;

	GamLine(int x1, int y1, int x2, int y2);
	void addToWorld(GamWorld *w);
};

class GamText : public QGraphicsTextItem, public GamObject, public GamRigidBody {
	Q_OBJECT;
public:
	bool isDrag;
	int x;
	int y;
	int width;
	int height;
	b2Body *body;

	GamText(QString text);
	void setPosition(int x, int y);
	void addToWorld(GamWorld *w);
};

class GamScene : public QGraphicsScene {
	Q_OBJECT;
public:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:
	void dragBeginSignal(QGraphicsSceneMouseEvent *event);
	void dragMoveSignal(QGraphicsSceneMouseEvent *event);
	void dragEndSignal(QGraphicsSceneMouseEvent *event);
};

static inline int match(const char *base, const char *target)
{
	int ret = 0;
	if (strlen(base) == strlen(target) &&
		!strncmp(base, target, strlen(target))) {
		ret = 1;
	}
	return ret;
}

class GamTimer : public QObject, public GamObject {
};


class GamGroup : public QObject, public QGraphicsItemGroup {
public:
	int x;
	int y;
	qreal width;
	qreal height;

	GamGroup();
	GamPoint *getCenter(void);
	void setSize(float width_, float height_);
};

typedef struct {
	Vec2f a;
	Vec2f b;
	Vec2f c;
} Triangle;


class GamComplexItem : public QObject, public GamObject, public QGraphicsItemGroup, public GamRigidBody {
	Q_OBJECT;
public:
	QList<QGraphicsPolygonItem*> *gp_list;
	bool isDrag;
	int x;
	int y;
	int width;
	int height;
	b2Body *body;

	GamComplexItem(const GamVector &pts);
	GamComplexItem(void);
	void setPosition(int x, int y);
	void setColor(QColor *c);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	~GamComplexItem(void);
	void addToWorld(GamWorld *w);
signals:
	void dragBeginSignal(QGraphicsSceneMouseEvent *event);
	void dragMoveSignal(QGraphicsSceneMouseEvent *event);
	void dragEndSignal(QGraphicsSceneMouseEvent *event);
};

class GamCapture {
public:
	CvCapture *capture;

	GamCapture(void);
	QImage *convertFromIplImageToQImage(const IplImage *ipl, double min, double max);
	GamTexture *queryFrame(void);
	~GamCapture(void);
};

class GamCamera : public GamCapture {
public:
	GamCamera(int n);
};

class GamVideo : public GamCapture {
public:
	GamVideo(const char *filename);
};

class GamJoint {
public:
	b2Joint *joint;
	GamObject *body_userdata;

	GamJoint();
	void setBodyUserData(void *userdata);
	b2Body *getBody(GamObject *o);
};

class GamDistanceJoint : public b2DistanceJointDef, public QGraphicsLineItem,
						 public GamObject, public GamJoint {
public:

	GamDistanceJoint(GamObject *o1, GamObject *o2);
	GamDistanceJoint(GamObject *o1, const GamPoint &p1, GamObject *o2, const GamPoint &p2);
	void setLocalAnchorA(const GamPoint &p);
	void setLocalAnchorB(const GamPoint &p);
	void setFrequencyHz(float frequency);
	void setDampingRatio(float ratio);
	void setLength(float length);
	void setCollideConnected(bool b);
	void addToWorld(GamWorld *world);
};

class GamRevoluteJoint : public b2RevoluteJointDef, public QGraphicsLineItem,
						 public GamObject, public GamJoint {
public:

	GamRevoluteJoint(GamObject *o1, GamObject *o2);
	GamRevoluteJoint(GamObject *o1, GamObject *o2, const GamPoint &anchor);
	void setLocalAnchorA(const GamPoint &p);
	void setLocalAnchorB(const GamPoint &p);
	void setLowerAngle(float angle);
	void setUpperAngle(float angle);
	void setEnableLimit(bool b);
	void setMaxMotorTorque(float torque);
	void setMotorSpeed(float speed);
	void setEnableMotor(bool b);
	void addToWorld(GamWorld *world);
};

class GamPrismaticJoint : public b2PrismaticJointDef, public QGraphicsLineItem,
						  public GamObject, public GamJoint {
public:

	GamPrismaticJoint(GamObject *o1, GamObject *o2);
	void setLowerTranslation(float translation);
	void setUpperTranslation(float translation);
	void setEnableLimit(bool b);
	void setMaxMotorForce(float force);
	void setMotorSpeed(float speed);
	void setEnableMotor(bool b);
	void addToWorld(GamWorld *world);
};

class GamPulleyJoint : public b2PulleyJointDef, public QGraphicsPathItem,
					   public GamObject, public GamJoint {
public:

	GamPulleyJoint(GamObject *o1, const GamPoint &p1, GamObject *o2, const GamPoint &p2);
	void addToWorld(GamWorld *world);
};

class GamGearJoint : public b2GearJointDef, public GamObject, public GamJoint {
public:

	GamGearJoint(GamObject *o1, GamJoint *j1, GamObject *o2, GamJoint *j2);
	void addToWorld(GamWorld *world);
};

class XnVPointDrawer : public QObject, public XnVPointControl {
	Q_OBJECT;
public:
	XnVPointDrawer(XnUInt32 nHistorySize, xn::DepthGenerator depthGenerator);
	virtual ~XnVPointDrawer();
	void Update(XnVMessage* pMessage);
	void OnPointCreate(const XnVHandPointContext* cxt);
	void OnPointUpdate(const XnVHandPointContext* cxt);
	void OnPointDestroy(XnUInt32 nID);
	void SetDepthMap(XnBool bDrawDM);
	void SetFrameID(XnBool bFrameID);
	void SetTouchingFOVEdge(XnUInt32 nID);
	//void Draw() const;
protected:
	XnBool IsTouching(XnUInt32 nID) const;
	XnUInt32 m_nHistorySize;
	std::map<XnUInt32, std::list<XnPoint3D> > m_History;
	std::list<XnUInt32> m_TouchingFOVEdge;
	xn::DepthGenerator m_DepthGenerator;
	XnFloat* m_pfPositionBuffer;
	XnBool m_bDrawDM;
	XnBool m_bFrameID;
signals:
	void updateHandPositionSignal(GamPoint p, float z);
};

class GamPerson : public GamComplexItem {//public QGraphicsItemGroup {
private:
	unsigned int label;
public:

	GamPerson(unsigned int label_, const GamVector &pts);
	void addEdgePoint(const GamPoint &p);
	void createShape(const GamVector &pts);
	void strokePath(void);
	QList<GamPoint> *points(void);
	~GamPerson(void);
};

class GamPeople {
private:
	unsigned int capacity;
	unsigned int num;
	QList<GamPerson *> *persons;
public:
	GamPeople(int max_num);
	void addPerson(GamPerson *p);
	unsigned int length(void);
	GamPerson *getPerson(int n);
	void clear(void);
	~GamPeople(void);
};

class GamKinect : public QObject, public GamCapture {
	Q_OBJECT;
private:
	XnVSessionManager *sessionManager;
	XnVPushDetector *pushDetector;
	XnVSwipeDetector *swipeDetector;
	xn::Context *context;
	xn::UserGenerator *userGenerator;
	xn::ImageGenerator *imageGenerator;
	xn::DepthGenerator *depthGenerator;
	xn::HandsGenerator *handsGenerator;
	xn::GestureGenerator *gestureGenerator;
	xn::ImageMetaData *imageMD;
	IplImage *iplImage;
	XnVPointDrawer *drawer;
	GamPeople *people;
	IplImage *background_texture;
	IplImage *blend_texture;
	//XnUInt64 focalLength;
	//XnDouble pixelSize;
	//int uSize;
	//int vSize;
	//int uCenter;
	//int vCenter;

public:
	GamKinect(const char *xmlpath);
	GamTexture *queryFrame(void);
	GamTexture *queryBlendFrame(void);
	void setEdgePointToPeople(IplImage *image);
	void startSessionHandler(const XnPoint3D &pFocus, void *ctx);
	void endSessionHandler(void *ctx);
	void pushHandler(XnFloat fVelocity, XnFloat fAngle, void *ctx);
	void swipeHandler(XnFloat fVelocity, XnFloat fAngle, void *ctx);
	void update(void);
	void setBackgroundTexture(GamTexture *texture);
	GamPeople *getPeople(void);
	~GamKinect(void);
signals:
	void startSessionSignal(const XnPoint3D pFocus, void *ctx);
	void endSessionSignal(void *ctx);
	void pushSignal(XnFloat fVelocity, XnFloat fAngle, void *ctx);
	void swipeSignal(XnFloat fVelocity, XnFloat fAngle, void *ctx);
	void updateHandPositionSignal(GamPoint p, float z);
	void foundPeopleSignal(GamPeople *people);
public slots:
	void updateHandPositionHandler(GamPoint p, float z);
};

extern "C" std::vector<Triangle> triangulate(const std::vector<Vec2f> & points, float resolution);
