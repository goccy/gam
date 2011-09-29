#include <QtGui>
#include <Box2D.h>
#include <cv.h>
#include <highgui.h>

#include <iostream>
using namespace std;

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

class GamWorld : public QObject, public GamObject {
	Q_OBJECT;
public:
	int timer_id;
	int iteration;
	float timestep;
	b2World *world;
	GamContact *contact;
	GamScene *scene;

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
};

class GamRigidBody {
public:
	bool isStatic;
	qreal _rotation;
	qreal friction;
	qreal density;
	qreal restitution;
	bool bullet;

	GamRigidBody(void);
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
	void addToWorld(GamWorld *w);
	void setTexture(GamTexture *t);
	GamPoint *getCenter(void);
	void setSize(float width_, float height_);
	~GamRect(void);
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
	b2Body *body;

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

	GamComplexItem(const std::vector<Vec2f> &pts, int size);
	void setPosition(int x, int y);
	void setColor(QColor *c);
	~GamComplexItem(void);
	void addToWorld(GamWorld *w);
};

class GamCapture {
public:
	CvCapture *capture;

	QImage *convertFromIplImageToQImage(const IplImage *ipl, double min, double max);
	GamTexture *queryFrame(void);
};

class GamCamera : public GamCapture {
public:
	GamCamera(int n);
};

class GamVideo : public GamCapture {
public:
	GamVideo(const char *filename);
};

extern "C" std::vector<Triangle> triangulate(const std::vector<Vec2f> & points, float resolution);
