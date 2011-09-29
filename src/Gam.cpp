#include <gam.hpp>

//============================= GamObject ==============================//
GamObject::GamObject(void)
{
	_tag = 0;
}

void GamObject::setTag(int tag)
{
	_tag = tag;
}

int GamObject::tag(void)
{
	return _tag;
}

//============================ GamRigidBody =============================//
GamRigidBody::GamRigidBody()
{
	_rotation = 0.0f;
	density = 0.0f;
	friction = 0.0f;
	restitution = 0.0f;
	bullet = false;
}

void GamRigidBody::setRot(qreal rotation)
{
	_rotation = rotation;
}

void GamRigidBody::setDensity(qreal density_)
{
	density = density_;
	if (density_ > 0) {
		isStatic = false;
	} else {
		isStatic = true;
	}
}
void GamRigidBody::setFriction(qreal friction_)
{
	friction = friction_;
}

void GamRigidBody::setRestitution(qreal restitution_)
{
	restitution = restitution_;
}

void GamRigidBody::setBullet(bool bullet_)
{
	bullet = bullet_;
}
//================================ GamRect ===================================//
GamRect::GamRect(int x_, int y_, int width_, int height_)
{
	r = new QRect(x_, y_, width_, height_);
	x = x_;
	y = y_;
	width = width_;
	height = height_;
	isDrag = false;
	setRect(*r);
	setObjectName("GamRect");
	setTag(GRect);
	se = NULL;
	body = NULL;
	isStatic = true;
}


GamPoint *GamRect::getCenter(void)
{
	QPointF p = pos();
	x = p.x();
	y = p.y();
	return new GamPoint(x + width / 2, y + height / 2);
}

void GamRect::setSize(float width_, float height_)
{
	width = width_;
	height = height_;
}

void GamRect::addToWorld(GamWorld *w)
{
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(0, 0);
	bodyDef.angle = -(rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);

	b2FixtureDef shapeDef;
	b2PolygonShape shape;
	shape.SetAsBox(width/2, height/2, b2Vec2(x + width/2, -y - height/2), 0.0);
	shapeDef.shape = &shape;
	shapeDef.density = density;
	shapeDef.friction = friction;
	shapeDef.restitution = restitution;
	body->CreateFixture(&shapeDef);
	body->SetBullet(bullet);
	//QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	QGraphicsItem *i = (QGraphicsItem *)this;
	GamObject *data = new GamObject();
	data->i = i;
	data->userdata = this;
	//knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	//memset(data, 0, sizeof(knh_GraphicsUserData_t));
	//CTX lctx = knh_getCurrentContext();
	//data->ct = getClassTBL(Rect);
	//data->o = i;
	//data->self = this;
	body->SetUserData(data);
}

GamRect::~GamRect(void)
{
	delete r;
	if (se != NULL)	delete se;
	if (body != NULL) {
		body->GetWorld()->DestroyBody(body);
	}
}

//========================================== GamEllipse =============================================//
GamEllipse::GamEllipse()
{
	setObjectName("GamEllipse");
	isDrag = false;
	setTag(GEllipse);
	glow_center_color = new QColor("white");
	isStatic = true;
}

void GamEllipse::setPosition(int x_, int y_)
{
	x = x_;
	y = y_;
	setPos(x, y);
}

void GamEllipse::setGlowCenterColor(QColor *c)
{
	glow_center_color = c;
}

void GamEllipse::addToWorld(GamWorld *w)
{
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(0, 0);
	bodyDef.angle = -(rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);

	b2FixtureDef shapeDef;
	b2CircleShape shape;
	shape.m_p = b2Vec2(x + width / 2, -y - height / 2);
	shape.m_radius = width / 2;
	shapeDef.shape = &shape;
	shapeDef.density = density;
	shapeDef.friction = friction;
	shapeDef.restitution = restitution;
	body->CreateFixture(&shapeDef);

	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	//knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	//memset(data, 0, sizeof(knh_GraphicsUserData_t));
	//CTX lctx = knh_getCurrentContext();
	//data->ct = getClassTBL(Ellipse);
	//data->o = i;
	//data->self = this;
	GamObject *data = new GamObject();
	data->i = i;
	data->userdata = this;
	body->SetUserData(data);
}

void GamEllipse::setRectShape(GamRect *r)
{
	setRect(*r->r);
	x = r->x;
	y = r->y;
	width = r->width;
	height = r->height;
}

//=================================== GamText ==================================================//
GamText::GamText(QString text)
{
	setPlainText(text);
	setObjectName("GamText");
	isDrag = false;
	//TODO
	width = text.size() * 7;
	height = 10;
	setTag(GText);
	isStatic = true;
}

void GamText::setPosition(int x_, int y_)
{
	x = x_;
	y = y_;
}

void GamText::addToWorld(GamWorld *w)
{
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(x, -y);
	bodyDef.angle = -(rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);

	b2FixtureDef shapeDef;
	b2PolygonShape shape;
	shape.SetAsBox(width / 2, height / 2, b2Vec2(3 + width / 2, -height / 2 - 5), 0.0);
	shapeDef.shape = &shape;
	shapeDef.density = density;
	shapeDef.friction = friction;
	shapeDef.restitution = restitution;
	body->CreateFixture(&shapeDef);

	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	//knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	//memset(data, 0, sizeof(knh_GraphicsUserData_t));
	//CTX lctx = knh_getCurrentContext();
	//data->ct = getClassTBL(Text);
	//data->o = i;
	//data->self = this;
	GamObject *data = new GamObject();
	data->i = i;
	data->userdata = this;
	body->SetUserData(data);
}

//================================== ComplexItem =============================================//
GamComplexItem::GamComplexItem(const std::vector<Vec2f> &pts, int size)
{
	x = 0;
	y = 0;
	/*
	int asize = knh_Array_size(a);
	std::vector<Vec2f> pts;
	for (int i = 0; i < asize; i++) {
		knh_RawPtr_t *o = (knh_RawPtr_t *)a->list[i];
		GamPoint *p = (GamPoint *)o->rawptr;
		pts.push_back(Vec2f(p->x, p->y));
		//delete p;
		//fprintf(stderr, "(x, y) = (%d, %d)\n", p->x, p->y);
	}
	*/
	std::vector<Triangle> tris = triangulate(pts, (float)size);
	gp_list = new QList<QGraphicsPolygonItem *>();
	for (std::vector<Triangle>::iterator triIt = tris.begin(); triIt != tris.end(); ++triIt) {
		QPolygonF p;
		p << QPoint(triIt->a.x, triIt->a.y);
		p << QPoint(triIt->b.x, triIt->b.y);
		p << QPoint(triIt->c.x, triIt->c.y);
		QGraphicsPolygonItem *gp = new QGraphicsPolygonItem();
		gp->setPolygon(p);
		gp->setPen(Qt::NoPen);
		//gp->setBrush(QColor((int)triIt->a.x, (int)triIt->b.x, (int)triIt->c.x));
		gp_list->append(gp);
		addToGroup(gp);
		//fprintf(stderr, "(%f, %f), (%f, %f), (%f, %f)\n", triIt->a.x, triIt->a.y, triIt->b.x, triIt->b.y, triIt->c.x, triIt->c.y);
	}
	isDrag = false;
	setTag(GComplexItem);
	setObjectName("GamComplexItem");
	isStatic = true;
}

GamComplexItem::~GamComplexItem(void)
{
	//int gp_length = gp_list->size();
	//for (int i = 0; i < gp_length; i++) {
	//QGraphicsPolygonItem *gp = gp_list->at(i);
	//delete gp;
	//}
}

void GamComplexItem::addToWorld(GamWorld *w)
{
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(x, -y);
	//bodyDef.angle = -(gp_list->at(0)->rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);
	int gp_length = gp_list->size();
	//fprintf(stderr, "length ==%d\n", gp_length);
	b2FixtureDef shapeDef;
	shapeDef.density = density;
	shapeDef.friction = friction;
	shapeDef.restitution = restitution;

	for (int i = 0; i < gp_length; i++) {
		QPolygonF poly = gp_list->at(i)->polygon();
		b2Vec2 vers[3];
		const QPointF p0 = poly.at(0);
		const QPointF p1 = poly.at(1);
		const QPointF p2 = poly.at(2);
		vers[2].Set(p0.x(), -p0.y());
		vers[1].Set(p1.x(), -p1.y());
		vers[0].Set(p2.x(), -p2.y());
		b2PolygonShape shape;
		shape.Set(vers, 3);
		shapeDef.shape = &shape;
		body->CreateFixture(&shapeDef);
	}
	body->SetBullet(bullet);
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	//knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	//memset(data, 0, sizeof(knh_GraphicsUserData_t));
	//CTX lctx = knh_getCurrentContext();
	//data->ct = getClassTBL(Texture);
	//data->o = i;
	//data->self = this;
	GamObject *data = new GamObject();
	data->i = i;
	data->userdata = this;
	body->SetUserData(data);
}

void GamComplexItem::setColor(QColor *c)
{
	int gp_length = gp_list->size();
	for (int i = 0; i < gp_length; i++) {
		QGraphicsPolygonItem *gp = gp_list->at(i);
		gp->setPen(QPen(*c));
		gp->setBrush(*c);
	}
}

void GamComplexItem::setPosition(int x_, int y_)
{
	x = x_;
	y = y_;
}


//=================================== GamTexture ==============================================//
GamTexture::GamTexture(const char *filepath_)
{
	QString filepath(filepath_);
	p = new QPixmap(filepath);
	setPixmap(*p);
	isDrag = false;
	setObjectName("GamTexture");
	ce = new QGraphicsColorizeEffect();
	setConnect();
	isStatic = true;
	ipl = NULL;
	//setTrackData(filepath_);
}

GamTexture::GamTexture(QImage *image)
{
	p = new QPixmap();
	QPixmap pixmap = p->fromImage(*image);
	setPixmap(pixmap);
	isDrag = false;
	setObjectName("GamTexture");
	ce = new QGraphicsColorizeEffect();
	setConnect();
	isStatic = true;
	ipl = NULL;
	//setTrackData();
}

GamTexture::GamTexture(QPixmap *image)
{
	p = NULL;
	setPixmap(*image);
	isDrag = false;
	setObjectName("GamTexture");
	ce = new QGraphicsColorizeEffect();
	setConnect();
	isStatic = true;
	ipl = NULL;
	//setTrackData();
}

GamTexture::~GamTexture(void)
{
	if (p != NULL) {
		delete p;
	}
	if (ce != NULL) {
		delete ce;
	}
	if (ipl != NULL) {
		cvReleaseImage(&ipl);
	}
}

void GamTexture::setConnect(void)
{

}

QList<GamTexture*> *GamTexture::split(int row, int col)
{
	QList<GamTexture*> *list = new QList<GamTexture*>();
	QPixmap p = pixmap();
	int width = p.width();
	int height = p.height();
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			QPixmap pixmap = p.copy(width / row * i, height / col * j, width / row, height / col);
			GamTexture *t = new GamTexture(&pixmap);
			list->append(t);
		}
	}
	return list;
}

void GamTexture::setRectShape(GamRect *r)
{
	x = r->x;
	y = r->y;
	width = r->width;
	height = r->height;
	QPixmap p_;
	if (p == NULL) {
		QPixmap p = pixmap();
		p_ = p.scaled(width, height);
	} else {
		p_ = p->scaled(width, height);
	}
	setPixmap(p_);
	setPos(x, y);
}

GamPoint *GamTexture::getCenter(void)
{
	QPointF p = pos();
	x = p.x();
	y = p.y();
	return new GamPoint(x + width / 2, y + height / 2);
}

void GamTexture::setSize(float width_, float height_)
{
	width = width_;
	height = height_;
}

void GamTexture::setColor(QColor *c)
{
	ce->setColor(*c);
	setGraphicsEffect(ce);
}

void GamTexture::addToWorld(GamWorld *w)
{
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(x, -y);
	bodyDef.angle = -(rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);

	b2FixtureDef shapeDef;
	b2PolygonShape shape;
	shape.SetAsBox(width / 2, height / 2, b2Vec2(width / 2, -height / 2), 0.0);
	shapeDef.shape = &shape;
	shapeDef.density = density;
	shapeDef.friction = friction;
	shapeDef.restitution = restitution;
	body->CreateFixture(&shapeDef);

	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	//knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	//memset(data, 0, sizeof(knh_GraphicsUserData_t));
	//CTX lctx = knh_getCurrentContext();
	//data->ct = getClassTBL(Texture);
	//data->o = i;
	//data->self = this;
	GamObject *data = new GamObject();
	data->i = i;
	data->userdata = this;
	body->SetUserData(data);
}

/*
#include <legacy/legacy.hpp>
typedef struct parameter Parameter;
struct parameter {
	float alpha;
	float beta;
	float gamma;
};

void GamTexture::setTrackData(const char *filepath)
{
	Parameter snake_param = {0.45, 0.35, 0.2};
	IplImage *src = cvLoadImage(filepath, CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *dst = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 3);
	//cvInitFont(&font, CV_FONT_HERSHEY_DUPLEX, 0.7, 0.7);
	CvPoint center;
	center.x = src->width / 2;
	center.y = src->height / 2;
	int length = 50;
	CvPoint *contour = (CvPoint *)cvAlloc(sizeof(CvPoint) * length);
	for (int i = 0; i < length; i++) {
		contour[i].x = (int)(center.x * cos(2 * CV_PI * i / length) + center.x);
		contour[i].y = (int)(center.y * sin(2 * CV_PI * i / length) + center.y);
	}
	cvCvtColor(src, dst, CV_GRAY2RGB);
	for (int i = 0; i < length - 1; i++) {
		cvLine(dst, contour[i], contour[i + 1], CV_RGB(255, 0, 0), 2, 8, 0);
	}
	for (int i = 0; i < 50; i++) {
		cvSnakeImage(src, contour, length, &snake_param.alpha, &snake_param.beta, &snake_param.gamma,
					 CV_VALUE, cvSize(15, 15), cvTermCriteria (CV_TERMCRIT_ITER, 1, 0.0), 1);
		cvCvtColor(src, dst, CV_GRAY2RGB);
		for (int j = 0; j < length - 1; j++) {
			cvLine(dst, contour[j], contour[j + 1], CV_RGB(255, 0, 0), 2);
		}
		cvLine(dst, contour[length - 1], contour[0], CV_RGB(255, 0, 0), 2);
		//cvPutText (dst_img, iter, cvPoint (15, 30), &font, CV_RGB (0, 0, 255));
	}
	for (int i = 0; i < length; i++) {
		fprintf(stderr, "(x, y) = (%d, %d)\n", contour[i].x, contour[i].y);
	}
	//cvNamedWindow ("Snakes", CV_WINDOW_AUTOSIZE);
	//cvShowImage("Snakes", dst);
	//int c = cvWaitGamey (0);
}
*/

//================================== GamLine ================================================//
GamLine::GamLine(int x1_, int y1_, int x2_, int y2_)
{
	x1 = x1_;
	y1 = y1_;
	x2 = x2_;
	y2 = y2_;
	width = ((x1_ - x2_) > 0) ? (x1_ - x2_) : (x2_ - x1_);
	height = ((y1_ - y2_) > 0) ? (y1_ - y2_) : (y2_ - y1_);
	isDrag = false;
	l = new QLine(x1_, y1_, x2_, y2_);
	setLine(*l);
	setObjectName("GamLine");
	setTag(GLine);
	isStatic = true;
}

void GamLine::addToWorld(GamWorld *w)
{
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(0, 0);
	bodyDef.angle = -(rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);

	b2FixtureDef shapeDef;
	b2PolygonShape shape;
	shape.SetAsBox(width / 2, 1, *(new b2Vec2(x1 + width/2, -y1)), 0.0);
	shapeDef.shape = &shape;
	shapeDef.density = density;
	shapeDef.friction = friction;
	shapeDef.restitution = restitution;
	body->CreateFixture(&shapeDef);

	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	//knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	//memset(data, 0, sizeof(knh_GraphicsUserData_t));
	//CTX lctx = knh_getCurrentContext();
	//data->ct = getClassTBL(Texture);
	//data->o = i;
	//data->self = this;
	GamObject *data = new GamObject();
	data->i = i;
	data->userdata = this;
	body->SetUserData(data);
}

//=================================== GamContact ==============================================//
GamContact::GamContact(void)
{


}

void GamContact::BeginContact(b2Contact *contact)
{
	//fprintf(stderr, "CONTACT BEGIN\n");
	b2Fixture *a = contact->GetFixtureA();
	b2Fixture *b = contact->GetFixtureB();
	b2Body *abody = a->GetBody();
	b2Body *bbody = b->GetBody();
	GamObject *data1 = (GamObject *)abody->GetUserData();
	GamObject *data2 = (GamObject *)bbody->GetUserData();
	if (data1 != NULL && data2 != NULL) {
		GamObject *o1 = (GamObject *)data1->userdata;
		GamObject *o2 = (GamObject *)data2->userdata;
		emit emitBeginContactSignal(o1, o2);
	}
}

void GamContact::EndContact(b2Contact *contact)
{
	//fprintf(stderr, "CONTACT END\n");
	b2Fixture *a = contact->GetFixtureA();
	b2Fixture *b = contact->GetFixtureB();
	b2Body *abody = a->GetBody();
	b2Body *bbody = b->GetBody();
	GamObject *data1 = (GamObject *)abody->GetUserData();
	GamObject *data2 = (GamObject *)bbody->GetUserData();
	if (data1 != NULL && data2 != NULL) {
		GamObject *o1 = (GamObject *)data1->userdata;
		GamObject *o2 = (GamObject *)data2->userdata;
		emit emitEndContactSignal(o1, o2);
	}
}

void GamContact::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
	//fprintf(stderr, "CONTACT PRE SOLVE\n");
	(void)contact;
	(void)oldManifold;
}

void GamContact::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
	//fprintf(stderr, "CONTACT POST SOLVE\n");
	(void)contact;
	(void)impulse;
}

//========================================== GamWorld ===========================================//
GamWorld::GamWorld(GamScene *scene_)
{
	world = new b2World(b2Vec2(0.0f, -10.0f), true);
	iteration = 10;
	timestep = 1.0f / 30.0f;
	timer_id = 0;
	contact = new GamContact();
	world->SetContactListener(contact);
	scene = scene_;
	connect(contact, SIGNAL(emitBeginContactSignal(GamObject *, GamObject *)),
			this, SLOT(beginContactSlot(GamObject *, GamObject *)));
	connect(contact, SIGNAL(emitEndContactSignal(GamObject *, GamObject *)),
			this, SLOT(endContactSlot(GamObject *, GamObject *)));
}

void GamWorld::beginContactSlot(GamObject *o1, GamObject *o2)
{
	emit beginContact(o1, o2);
}

void GamWorld::endContactSlot(GamObject *o1, GamObject *o2)
{
	emit endContact(o1, o2);
}

void GamWorld::start()
{
	if (!timer_id) {
		timer_id = startTimer(1000 / 60.0);
	}
}

#define addWorld(T, o) ((T)o)->addToWorld(this)

void GamWorld::add(GamObject *o)
{
	switch (o->tag()) {
	case GRect:
		addWorld(GamRect *, o);
		break;
	case GEllipse:
		addWorld(GamEllipse *, o);
		break;
	case GTexture:
		addWorld(GamTexture *, o);
		break;
	case GText:
		addWorld(GamText *, o);
		break;
	case GLine:
		addWorld(GamLine *, o);
		break;
	case GComplexItem:
		addWorld(GamComplexItem *, o);
		break;
	default:
		fprintf(stderr, "World: [WARNING] UNNOWN OBJECT\n");
		break;
	}
}

void GamWorld::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == timer_id) {
		world->Step(timestep, 8, 1);
		for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())	{
			if (b->GetUserData() != NULL) {
				GamObject *data = (GamObject *)b->GetUserData();
				QGraphicsItem *i = (QGraphicsItem *)data->i;
				i->setPos(b->GetPosition().x, -b->GetPosition().y);
				i->setRotation(-1 * b->GetAngle() * 360.0 / (2 * M_PI));
			}
		}
	}
	scene->update(scene->sceneRect());
	QObject::timerEvent(event);
}

#include <del_interface.hpp>
//====================== Written by Takuma Wakamori =========================//

#ifdef __cplusplus
extern "C" {
#endif

//static vector<Triangle> triangulate(const vector<Vec2f> & points, float resolution = 50.0f);

#define fMIN(x, y) (((x) < (y)) ? x : y)
#define fMAX(x, y) (((x) < (y)) ? y : x)

std::vector<Triangle> triangulate(const std::vector<Vec2f> & points, float resolution = 50.0f)
{
	std::vector<Triangle> mTriangles;
	std::vector<Vec2f> mPoints = points;
	float mSize = mPoints.size();
	float mCount = fMIN(resolution, mSize);
	tpp::Delaunay::Point mPoint;
	std::vector<tpp::Delaunay::Point> mVertices;
	for (int32_t i = 0; i < mCount; i++) {
		int32_t mId = (int32_t)((float)i / mCount * mSize);
		mPoint[0] = mPoints[mId].x;
		mPoint[1] = mPoints[mId].y;
		mVertices.push_back(mPoint);
	}
	tpp::Delaunay mDelaunay(mVertices);
	mDelaunay.Triangulate();
	for (tpp::Delaunay::fIterator mTriIt = mDelaunay.fbegin(); mTriIt != mDelaunay.fend(); ++mTriIt) {
		int32_t mA = mDelaunay.Org(mTriIt);
		int32_t mB = mDelaunay.Dest(mTriIt);
		int32_t mC = mDelaunay.Apex(mTriIt);
		int32_t mAId = (int32_t)(((float)mA / resolution) * mSize);
		int32_t mBId = (int32_t)(((float)mB / resolution) * mSize);
		int32_t mCId = (int32_t)(((float)mC / resolution) * mSize);
		Vec2f mTriangle[3];
		mTriangle[0] = Vec2f(mPoints[mAId].x, mPoints[mAId].y);
		mTriangle[1] = Vec2f(mPoints[mBId].x, mPoints[mBId].y);
		mTriangle[2] = Vec2f(mPoints[mCId].x, mPoints[mCId].y);
		Vec2f mCentroid = Vec2f(
				(mTriangle[0].x + mTriangle[1].x + mTriangle[2].x) / 3.0f,
				(mTriangle[0].y + mTriangle[1].y + mTriangle[2].y) / 3.0f
			);
		int32_t mCounter = 0;
		Vec2f mPointA = mPoints[0];
		Vec2f mPointB;
		for (int32_t i = 1; i < (int32_t)mSize; i++) {
			mPointB = mPoints[i];
			if (mCentroid.y > fMIN(mPointA.y, mPointB.y) &&
					mCentroid.y <= fMAX(mPointA.y, mPointB.y) &&
					mCentroid.x <= fMAX(mPointA.x, mPointB.x) &&
					mPointA.y != mPointB.y &&
					(mPointA.x == mPointB.x || mCentroid.x <= (mCentroid.y - mPointA.y) * (mPointB.x - mPointA.x) / (mPointB.y - mPointA.y) + mPointA.x))
				mCounter++;
			mPointA = mPointB;
		}
		if (mCounter % 2 != 0) {
			Triangle mTriData;
			float x0 = mTriangle[0].x;
			float y0 = mTriangle[0].y;
			float x1 = mTriangle[1].x;
			float y1 = mTriangle[1].y;
			float x2 = mTriangle[2].x;
			float y2 = mTriangle[2].y;
			if (((x1 - x0) * (y2 - y0) - (x2 -x0) * (y1 - y0)) > 0) {
				mTriData.a = Vec2f(x0, y0);
				mTriData.b = Vec2f(x1, y1);
				mTriData.c = Vec2f(x2, y2);
			} else {
				mTriData.a = Vec2f(x0, y0);
				mTriData.b = Vec2f(x2, y2);
				mTriData.c = Vec2f(x1, y1);
			}
			mTriangles.push_back(mTriData);
		}
	}
	return mTriangles;
}

#ifdef __cplusplus
}
#endif
