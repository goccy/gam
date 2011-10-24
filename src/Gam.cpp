#include <gam.hpp>

//============================= GamObject ==============================//
GamObject::GamObject(void)
{
	//x = 0;
	//y = 0;
	//width = 0.0f;
	//height = 0.0f;
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
	body_userdata = new GamObject();
}

void GamRigidBody::setBodyUserData(void *userdata)
{
	body_userdata->userdata = userdata;
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
	setTag(GamRectTag);
	se = NULL;
	body = NULL;
	isStatic = true;
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	body_userdata->i = i;
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
}

void GamRect::setTexture(GamTexture *t)
{
	QPixmap p = t->pixmap();
	QBrush b(p);
	setBrush(b);
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
	/* reset position for synchronization with box2d [anchor:center] */
	setRect(-width/2, -height/2, width, height);
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	/* box2d's anchor point is center position */
	bodyDef.position.Set((x+width/2)/PTM_RATIO, (y+height/2)/PTM_RATIO);
	//bodyDef.position.Set(0, 0);
	bodyDef.angle = -(rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);

	b2FixtureDef shapeDef;
	b2PolygonShape shape;
	//shape.SetAsBox(width/2/PTM_RATIO, height/2/PTM_RATIO,
	//		   b2Vec2((x + width/2)/PTM_RATIO, (y + height/2)/PTM_RATIO), 0.0);
	//b2Vec2((x + width/2)/PTM_RATIO, (-y - height/2)/PTM_RATIO), 0.0);
	/* box size is half size of original size */
	shape.SetAsBox(width/2/PTM_RATIO, height/2/PTM_RATIO);
	shapeDef.shape = &shape;
	shapeDef.density = density;
	shapeDef.friction = friction;
	shapeDef.restitution = restitution;
	body->CreateFixture(&shapeDef);
	body->SetBullet(bullet);
	body->SetUserData(body_userdata);
}

void GamRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit dragBeginSignal(event);
}

void GamRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	emit dragMoveSignal(event);
}

void GamRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	emit dragEndSignal(event);
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
	setTag(GamEllipseTag);
	glow_center_color = new QColor("white");
	isStatic = true;
	glow = false;
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	body_userdata->i = i;
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
}

void GamEllipse::setPosition(int x_, int y_)
{
	x = x_;
	y = y_;
	setPos(x, y);
}

void GamEllipse::setGlow(void)
{
	glow = true;
	setPen(Qt::NoPen);
	QColor c = brush().color();
	color = new QColor(c);
	QRadialGradient radial(x+width/2, y+height/2, width/2);
	QColor c1(c);
	c1.setAlpha(0xcc);
	QColor c2(c);
	c2.setAlpha(0x44);
	QColor c3(c);
	c3.setAlpha(0x11);
	radial.setColorAt(0.4, *glow_center_color);
	radial.setColorAt(0.8, c1);
	radial.setColorAt(1.0, c2);
	setBrush(QBrush(radial));
}

void GamEllipse::setGlowCenterColor(QColor *c)
{
	glow_center_color = c;
}

void GamEllipse::addToWorld(GamWorld *w)
{
	/* reset position for synchronization with box2d [anchor:center] */
	setRect(-width/2, -height/2, width, height);
	if (glow) {
		QColor c = *color;
		QRadialGradient radial(0, 0, width/2);
		QColor c1(c);
		c1.setAlpha(0xcc);
		QColor c2(c);
		c2.setAlpha(0x44);
		QColor c3(c);
		c3.setAlpha(0x11);
		radial.setColorAt(0.4, *glow_center_color);
		radial.setColorAt(0.8, c1);
		radial.setColorAt(1.0, c2);
		setBrush(radial);
	}
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	//bodyDef.position.Set(0, 0);
	/* box2d's anchor point is center position */
	bodyDef.position.Set(x/PTM_RATIO, y/PTM_RATIO);
	bodyDef.angle = -(rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);

	b2FixtureDef shapeDef;
	b2CircleShape shape;
	//shape.m_p = b2Vec2(x + width / 2, -y - height / 2);
	//shape.m_p = b2Vec2(x/PTM_RATIO, y/PTM_RATIO);
	shape.m_radius = width/2/PTM_RATIO;
	shapeDef.shape = &shape;
	shapeDef.density = density;
	shapeDef.friction = friction;
	shapeDef.restitution = restitution;
	body->CreateFixture(&shapeDef);
	body->SetUserData(body_userdata);
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
	setTag(GamTextTag);
	isStatic = true;
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	body_userdata->i = i;
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
}

void GamText::setPosition(int x_, int y_)
{
	x = x_;
	y = y_;
}

void GamText::addToWorld(GamWorld *w)
{
	/* reset position for synchronization with box2d [anchor:center] */
	//setRect(-width/2, -height/2, width, height);
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	/* box2d's anchor point is center position */
	//bodyDef.position.Set(x/PTM_RATIO, y/PTM_RATIO);
	bodyDef.position.Set((x+width/2)/PTM_RATIO, (y+height/2)/PTM_RATIO);
	//bodyDef.position.Set(x, -y);
	bodyDef.angle = -(rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);

	b2FixtureDef shapeDef;
	b2PolygonShape shape;
	//shape.SetAsBox(width/2, height/2, b2Vec2(3 + width/2, -height/2 - 5), 0.0);
	shape.SetAsBox((width/2+3)/PTM_RATIO, (height/2+5)/PTM_RATIO);
	shapeDef.shape = &shape;
	shapeDef.density = density;
	shapeDef.friction = friction;
	shapeDef.restitution = restitution;
	body->CreateFixture(&shapeDef);
	body->SetUserData(body_userdata);
}

//================================== ComplexItem =============================================//
GamComplexItem::GamComplexItem(const GamVector &pts)
{
	x = 0;
	y = 0;
	int size = pts.size();
	if (size < 3) return;
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
	setTag(GamComplexItemTag);
	setObjectName("GamComplexItem");
	isStatic = true;
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	body_userdata->i = i;
	body_userdata->setTag(GamComplexItemTag);
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
}

GamComplexItem::GamComplexItem(void)
{
	x = 0;
	y = 0;
	isDrag = false;
	setTag(GamComplexItemTag);
	setObjectName("GamComplexItem");
	isStatic = true;
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	body_userdata->i = i;
	body_userdata->setTag(GamComplexItemTag);
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
}

void GamComplexItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit dragBeginSignal(event);
}

void GamComplexItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	emit dragMoveSignal(event);
}

void GamComplexItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	emit dragEndSignal(event);
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
	bodyDef.position.Set(x/PTM_RATIO, y/PTM_RATIO);
	//bodyDef.position.Set(x, -y);
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
		//fprintf(stderr, "p0 = [%f, %f]\n", p0.x(), p0.y());
		//fprintf(stderr, "p1 = [%f, %f]\n", p1.x(), p1.y());
		//fprintf(stderr, "p2 = [%f, %f]\n", p2.x(), p2.y());
		vers[0].Set(p0.x()/PTM_RATIO, p0.y()/PTM_RATIO);
		vers[1].Set(p1.x()/PTM_RATIO, p1.y()/PTM_RATIO);
		vers[2].Set(p2.x()/PTM_RATIO, p2.y()/PTM_RATIO);

		//vers[2].Set((p0.x())/PTM_RATIO, (p0.y())/PTM_RATIO);
		//vers[1].Set((p1.x())/PTM_RATIO, (p1.y())/PTM_RATIO);
		//vers[0].Set((p2.x())/PTM_RATIO, (p2.y())/PTM_RATIO);
		b2PolygonShape shape;
		shape.Set(vers, 3);
		shapeDef.shape = &shape;
		body->CreateFixture(&shapeDef);
	}
	body->SetBullet(bullet);
	body->SetUserData(body_userdata);
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
//================================== GamScene ====================================

void GamScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	emit dragBeginSignal(event);
	QGraphicsScene::mousePressEvent(event);
}

void GamScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	emit dragMoveSignal(event);
	QGraphicsScene::mousePressEvent(event);
}

void GamScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	emit dragEndSignal(event);
	QGraphicsScene::mousePressEvent(event);
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
	ipl = cvLoadImage(filepath_, CV_LOAD_IMAGE_COLOR);
	setTag(GamTextureTag);
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	body_userdata->i = i;
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
	//setTrackData(filepath_);
}

GamTexture::GamTexture(QImage *image)
{
	p = NULL;
	QPixmap pixmap = QPixmap::fromImage(*image);
	setPixmap(pixmap);
	isDrag = false;
	setObjectName("GamTexture");
	ce = new QGraphicsColorizeEffect();
	setConnect();
	isStatic = true;
	ipl = NULL;
	setTag(GamTextureTag);
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	body_userdata->i = i;
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
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
	setTag(GamTextureTag);
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	body_userdata->i = i;
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
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
	/* reset position for synchronization with box2d [anchor:center] */
	//setRect(-width/2, -height/2, width, height);
	setOffset(-width/2, -height/2);
	b2World *world = w->world;
	b2BodyDef bodyDef;
	if (!isStatic) {
		bodyDef.type = b2_dynamicBody;
	}
	/* box2d's anchor point is center position */
	bodyDef.position.Set((x)/PTM_RATIO, (y)/PTM_RATIO);
	//bodyDef.position.Set((x+width/2)/PTM_RATIO, (y+height/2)/PTM_RATIO);
	//bodyDef.position.Set(x, -y);
	bodyDef.angle = -(rotation() * (2 * M_PI)) / 360.0;
	body = world->CreateBody(&bodyDef);

	b2FixtureDef shapeDef;
	b2PolygonShape shape;
	shape.SetAsBox(width/2/PTM_RATIO, height/2/PTM_RATIO);
	//shape.SetAsBox(width/2, height/2, b2Vec2(width/2, -height/2), 0.0);
	shapeDef.shape = &shape;
	shapeDef.density = density;
	shapeDef.friction = friction;
	shapeDef.restitution = restitution;
	body->CreateFixture(&shapeDef);
	body->SetUserData(body_userdata);
}

QList<GamVector> *GamTexture::detectHuman(GamTexture *background_)
{
	IplImage *src = ipl;
	IplImage *background = background_->ipl;
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* find_contour = NULL;
	const int w = src->width;
    const int h = src->height;
	IplImage *imgResult = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
	IplImage *hsvImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 3);
	IplImage *hueImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
	IplImage *saturationImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
	IplImage *valueImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
	IplImage *thresholdImage1 = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
	IplImage *thresholdImage2 = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
	IplImage *thresholdImage3 = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U, 1);
	IplImage *grayImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U,1);
	IplImage *differenceImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U,1);
    IplImage *backgroundImage = cvCreateImage(cvSize(w, h), IPL_DEPTH_8U,1);
	cvCvtColor(background, backgroundImage, CV_BGR2GRAY);
	cvCvtColor(src, grayImage, CV_BGR2GRAY);
	cvAbsDiff(grayImage, backgroundImage, differenceImage);
	cvCvtColor(src, hsvImage, CV_BGR2HSV);
	cvSplit(hsvImage, hueImage, saturationImage, valueImage, NULL);
	cvThreshold(hueImage, thresholdImage1, 0, 255, CV_THRESH_BINARY);
	cvThreshold(hueImage, thresholdImage2, 96, 255, CV_THRESH_BINARY_INV);
	cvAnd(thresholdImage1, thresholdImage2, thresholdImage3, NULL);
	cvAnd(differenceImage, thresholdImage3, imgResult, NULL);
	cvErode(imgResult, imgResult, NULL, 1);
	cvDilate(imgResult, imgResult, NULL, 1);

	//=====================================================================//
	IplImage *tmp_img = cvCreateImage(cvGetSize(imgResult), IPL_DEPTH_8U, 1);
	cvThreshold(imgResult, tmp_img, 40, 255, CV_THRESH_BINARY);
	cvDilate(tmp_img, tmp_img, NULL, 20);
	cvFindContours(tmp_img, storage, &find_contour, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    cvDrawContours(src, find_contour, CV_RGB(255, 0, 0), CV_RGB(255, 0, 0), 1, 2, CV_AA, cvPoint(0, 0));
	//=====================================================================//

	QList<GamVector> *objs = new QList<GamVector>();
	while (1) {
		if (find_contour == NULL) break;
		//fprintf(stderr, "=============START=============\n");
		GamVector obj;
		for (int i = 0; i < find_contour->total; i++) {
			CvPoint *point = CV_GET_SEQ_ELEM(CvPoint, find_contour, i);
			obj.push_back(Vec2f(point->x, point->y));
		}
		//fprintf(stderr, "=============END=============\n");
		objs->append(obj);
		if (find_contour->h_next == NULL) break;
		find_contour = find_contour->h_next;
	}
	cvReleaseImage(&imgResult);
	cvReleaseImage(&hsvImage);
	cvReleaseImage(&hueImage);
	cvReleaseImage(&saturationImage);
	cvReleaseImage(&valueImage);
	cvReleaseImage(&thresholdImage1);
	cvReleaseImage(&thresholdImage2);
	cvReleaseImage(&thresholdImage3);
	cvReleaseImage(&grayImage);
	cvReleaseImage(&differenceImage);
    cvReleaseImage(&backgroundImage);
	cvReleaseImage(&tmp_img);
	cvReleaseMemStorage(&storage);
	return objs;
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
	setTag(GamLineTag);
	isStatic = true;
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	body_userdata->i = i;
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
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
	shape.SetAsBox(width / 2, 1, b2Vec2(x1 + width/2, -y1), 0.0);
	shapeDef.shape = &shape;
	shapeDef.density = density;
	shapeDef.friction = friction;
	shapeDef.restitution = restitution;
	body->CreateFixture(&shapeDef);
	body->SetUserData(body_userdata);
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

GamQueryCallback::GamQueryCallback(const b2Vec2& point)
{
	m_point = point;
	m_fixture = NULL;
}

bool GamQueryCallback::ReportFixture(b2Fixture* fixture)
{
	b2Body* body = fixture->GetBody();
	b2Vec2 pos = body->GetPosition();
	//fprintf(stderr, "body = [%p]\n", body);
	//fprintf(stderr, "pos = (%f, %f)\n", pos.x, pos.y);
	if (body->GetType() == b2_dynamicBody) {
		//fprintf(stderr, "m_point = [%f, %f]\n", m_point.x, m_point.y);
		bool inside = fixture->TestPoint(m_point);
		fprintf(stderr, "inside = [%d]\n", inside);
		if (inside) {
			m_fixture = fixture;
			// We are done, terminate the query.
			return false;
		}
	}
	// Continue the query.
	return true;
}

//========================================== GamWorld ===========================================//
GamWorld::GamWorld(GamScene *scene_)
{
	world = new b2World(b2Vec2(0.0f, 10.0f), true);
	world->SetContinuousPhysics(true);
	iteration = 10;
	timestep = 1.0f / 30.0f;
	timer_id = 0;

	contact = new GamContact();
	world->SetContactListener(contact);
	b2BodyDef bodyDef;
	mouse_joint_body = world->CreateBody(&bodyDef);
	mouse_joint = NULL;
	scene = scene_;
	connect(contact, SIGNAL(emitBeginContactSignal(GamObject *, GamObject *)),
			this, SLOT(beginContactSlot(GamObject *, GamObject *)));
	connect(contact, SIGNAL(emitEndContactSignal(GamObject *, GamObject *)),
			this, SLOT(endContactSlot(GamObject *, GamObject *)));
	//========================== for debug =============================//
	debugDraw = new GamGL(PTM_RATIO);
	world->SetDebugDraw(debugDraw);
	debugDraw->resize(600, 600);

	uint32 flags = 0;
	flags += b2DebugDraw::e_shapeBit;
	flags += b2DebugDraw::e_jointBit;
	flags += b2DebugDraw::e_aabbBit;
	flags += b2DebugDraw::e_pairBit;
	flags += b2DebugDraw::e_centerOfMassBit;
	debugDraw->SetFlags(flags);
	debugDraw->world = world;
	//connect(scene, SIGNAL(dragBeginSignal(QGraphicsSceneMouseEvent *)),
//			this, SLOT(dragBeginSlot(QGraphicsSceneMouseEvent *)));
	//connect(scene, SIGNAL(dragMoveSignal(QGraphicsSceneMouseEvent *)),
//			this, SLOT(dragMoveSlot(QGraphicsSceneMouseEvent *)));
	//connect(scene, SIGNAL(dragEndSignal(QGraphicsSceneMouseEvent *)),
	//	this, SLOT(dragEndSlot(QGraphicsSceneMouseEvent *)));
	//=================================================================//
}

void GamWorld::dragBeginSlot(QGraphicsSceneMouseEvent *event)
{
	//fprintf(stderr, "drag begin\n");
	QPointF pos = event->scenePos();
	//width/2, height/2, b2Vec2(x + width/2, -y - height/2), 0.0);
	b2Vec2 p(pos.x()/PTM_RATIO, pos.y()/PTM_RATIO);
	b2AABB aabb;
	b2Vec2 d;
	d.Set(0.001f, 0.001f);
	aabb.lowerBound = p - d;
	aabb.upperBound = p + d;
	//fprintf(stderr, "p -d = (%f, %f)\n", p.x - d.x, p.y - d.y);
	//fprintf(stderr, "p +d = (%f, %f)\n", p.x + d.x, p.y + d.y);
	// Query the world for overlapping shapes.
	GamQueryCallback callback(p);
	world->QueryAABB(&callback, aabb);
	//fprintf(stderr, "callback.m_fixture = [%p]\n", callback.m_fixture);
	if (callback.m_fixture) {
		//fprintf(stderr, "create joint\n");
		b2Body* body = callback.m_fixture->GetBody();
		b2MouseJointDef md;
		md.bodyA = mouse_joint_body;
		md.bodyB = body;
		md.target = p;
		md.maxForce = 1000.0f * body->GetMass();
		mouse_joint = (b2MouseJoint*)world->CreateJoint(&md);
		body->SetAwake(true);
	}
}

void GamWorld::dragMoveSlot(QGraphicsSceneMouseEvent *event)
{
	if (mouse_joint) {
		QPointF pos = event->scenePos();
		b2Vec2 p(pos.x()/PTM_RATIO, pos.y()/PTM_RATIO);
		mouse_joint->SetTarget(p);
	}
}

void GamWorld::dragEndSlot(QGraphicsSceneMouseEvent *event)
{
	if (mouse_joint) {
		world->DestroyJoint(mouse_joint);
		mouse_joint = NULL;
	}
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

template<class T>
inline void removeWorld(GamWorld *world, T o)
{
	b2Body *body = o->body;
	world->world->DestroyBody(body);
}

#define addWorld(T, o) ((T)o)->addToWorld(this)

template<class T>
inline void dragConnect(GamWorld *world, T o)
{
	QObject::connect(o, SIGNAL(dragBeginSignal(QGraphicsSceneMouseEvent *)),
					 world, SLOT(dragBeginSlot(QGraphicsSceneMouseEvent *)));
	QObject::connect(o, SIGNAL(dragMoveSignal(QGraphicsSceneMouseEvent *)),
					 world, SLOT(dragMoveSlot(QGraphicsSceneMouseEvent *)));
	QObject::connect(o, SIGNAL(dragEndSignal(QGraphicsSceneMouseEvent *)),
					 world, SLOT(dragEndSlot(QGraphicsSceneMouseEvent *)));
}

void GamWorld::add(GamObject *o)
{
	//fprintf(stderr, "tag = [%d]\n", o->tag());
	switch (o->tag()) {
	case GamRectTag:
		addWorld(GamRect *, o);
		dragConnect(this, (GamRect *)o);
		break;
	case GamEllipseTag:
		addWorld(GamEllipse *, o);
		break;
	case GamTextureTag:
		addWorld(GamTexture *, o);
		break;
	case GamTextTag:
		addWorld(GamText *, o);
		break;
	case GamLineTag:
		addWorld(GamLine *, o);
		break;
	case GamComplexItemTag:
		addWorld(GamComplexItem *, o);
		dragConnect(this, (GamComplexItem *)o);
		break;
	case GamDistanceJointTag:
		addWorld(GamDistanceJoint *, o);
		break;
	case GamRevoluteJointTag:
		addWorld(GamRevoluteJoint *, o);
		break;
	case GamPrismaticJointTag:
		addWorld(GamPrismaticJoint *, o);
		break;
	case GamPulleyJointTag:
		addWorld(GamPulleyJoint *, o);
		break;
	case GamGearJointTag:
		addWorld(GamGearJoint *, o);
		break;
	default:
		fprintf(stderr, "World: [WARNING] UNNOWN OBJECT\n");
		break;
	}
}

void GamWorld::remove(GamObject *o)
{
	switch (o->tag()) {
	case GamRectTag:
		removeWorld(this, (GamRect *)o);
	case GamEllipseTag:
		removeWorld(this, (GamEllipse *)o);
		break;
	case GamTextureTag:
		removeWorld(this, (GamTexture *)o);
		break;
	case GamTextTag:
		removeWorld(this, (GamText *)o);
		break;
	case GamLineTag:
		removeWorld(this, (GamLine *)o);
		break;
	case GamComplexItemTag:
		removeWorld(this, (GamComplexItem *)o);
		break;
	default:
		fprintf(stderr, "World: [WARNING] UNNOWN OBJECT\n");
	}

}

void GamWorld::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == timer_id) {
		world->Step(timestep, 8, 1);
		for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {
			GamObject *data = (GamObject *)b->GetUserData();
			if (data) {
				b2Vec2 posA = b->GetPosition();
				QGraphicsItem *i = (QGraphicsItem *)data->i;
				i->setPos(posA.x * PTM_RATIO, posA.y * PTM_RATIO);
				if (data->tag() == GamComplexItemTag) {
					i->setRotation(b->GetAngle() * 360.0 / (2 * M_PI));
				} else {
					i->setRotation(-1 * b->GetAngle() * 360.0 / (2 * M_PI));
				}
				b2JointEdge *jointList = b->GetJointList();
				if (jointList) {
					GamObject *d = (GamObject *)jointList->joint->GetUserData();
					b2Body *b2 = jointList->other;
					if (d) {
						if (jointList->joint->GetType() == e_pulleyJoint) {
							b2PulleyJoint *pulley = (b2PulleyJoint *)jointList->joint;
							b2Vec2 anchorA = pulley->GetAnchorA();
							b2Vec2 anchorB = pulley->GetAnchorB();
							b2Vec2 groundAnchorA = pulley->GetGroundAnchorA();
							b2Vec2 groundAnchorB = pulley->GetGroundAnchorB();
							QGraphicsPathItem *p = dynamic_cast<QGraphicsPathItem *>(d->i);
							QPainterPath path;
							path.moveTo(anchorA.x * PTM_RATIO, anchorA.y * PTM_RATIO);
							path.lineTo(groundAnchorA.x * PTM_RATIO, groundAnchorA.y * PTM_RATIO);
							path.lineTo(groundAnchorB.x * PTM_RATIO, groundAnchorB.y * PTM_RATIO);
							path.lineTo(anchorB.x * PTM_RATIO, anchorB.y * PTM_RATIO);
							p->setPath(path);
						} else {
							QGraphicsLineItem *line = dynamic_cast<QGraphicsLineItem *>(d->i);
							b2Vec2 posB = b2->GetPosition();
							line->setLine(posA.x * PTM_RATIO, posA.y * PTM_RATIO,
										  posB.x * PTM_RATIO, posB.y * PTM_RATIO);
						}
					}
				}
			}
		}
	}
	scene->update(scene->sceneRect());
	QObject::timerEvent(event);
}


//===================================== GamCapture =============================================//
GamCapture::GamCapture(void)
{
	capture = NULL;
}

QImage *GamCapture::convertFromIplImageToQImage(const IplImage * iplImage, double mini, double maxi)
{
	int width = iplImage->width;
	int widthStep = iplImage->widthStep;
	int height = iplImage->height;
	uchar *qImageBuffer = NULL;
	switch (iplImage->depth) {
	case IPL_DEPTH_8U:
		if (iplImage->nChannels == 1) {
			qImageBuffer = (uchar *)malloc(width * height * sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			const uchar *iplImagePtr = (const uchar *)iplImage->imageData;
			for (int y = 0; y < height; y++) {
				memcpy(QImagePtr, iplImagePtr, width);
				QImagePtr += width;
				iplImagePtr += widthStep;
			}
		} else if (iplImage->nChannels == 3) {
			qImageBuffer = (uchar *)malloc(width * height * 4 * sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			const uchar *iplImagePtr = (const uchar *)iplImage->imageData;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					QImagePtr[0] = iplImagePtr[0];
					QImagePtr[1] = iplImagePtr[1];
					QImagePtr[2] = iplImagePtr[2];
					QImagePtr[3] = 0;
					QImagePtr += 4;
					iplImagePtr += 3;
				}
				iplImagePtr += widthStep-3*width;
			}
		} else {
			qDebug("IplImageToQImage: image format is not supported:\
						depth=8U and %d channels\n", iplImage->nChannels);
		}
		break;
	case IPL_DEPTH_16U:
		if (iplImage->nChannels == 1) {
			qImageBuffer = (uchar *)malloc(width * height * sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			const uint16_t *iplImagePtr = (const uint16_t *)iplImage->imageData;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					*QImagePtr++ = ((*iplImagePtr++) >> 8);
				}
				iplImagePtr += widthStep/sizeof(uint16_t)-width;
			}
		} else {
			qDebug("IplImageToQImage: image format is not supported:\
						depth=16U and %d channels\n", iplImage->nChannels);
		}
		break;
	case IPL_DEPTH_32F:
		if (iplImage->nChannels == 1) {
			qImageBuffer = (uchar *)malloc(width * height * sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			const float *iplImagePtr = (const float *)iplImage->imageData;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					uchar p;
					float pf = 255 * ((*iplImagePtr++) - mini) / (maxi - mini);
					if (pf < 0) p = 0;
					else if (pf > 255) p = 255;
					else p = (uchar) pf;
					*QImagePtr++ = p;
				}
				iplImagePtr += widthStep/sizeof(float)-width;
			}
		} else {
			qDebug("IplImageToQImage: image format is not supported:\
						depth=32F and %d channels\n", iplImage->nChannels);
		}
		break;
	case IPL_DEPTH_64F:
		if (iplImage->nChannels == 1) {
			qImageBuffer = (uchar *)malloc(width * height * sizeof(uchar));
			uchar *QImagePtr = qImageBuffer;
			const double *iplImagePtr = (const double *)iplImage->imageData;
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					uchar p;
					double pf = 255 * ((*iplImagePtr++) - mini) / (maxi - mini);
					if (pf < 0) p = 0;
					else if (pf > 255) p = 255;
					else p = (uchar) pf;
					*QImagePtr++ = p;
				}
				iplImagePtr += widthStep/sizeof(double)-width;
			}
		} else {
			qDebug("IplImageToQImage: image format is not supported:\
						depth=64F and %d channels\n", iplImage->nChannels);
		}
		break;
	default:
		qDebug("IplImageToQImage: image format is not supported: depth=%d\
					and %d channels\n", iplImage->depth, iplImage->nChannels);
		break;
	}
	QImage *qImage;
	if (iplImage->nChannels == 1) {
		QVector<QRgb> colorTable;
		for (int i = 0; i < 256; i++) {
			colorTable.push_back(qRgb(i, i, i));
		}
		qImage = new QImage(qImageBuffer, width, height, QImage::Format_Indexed8);
		qImage->setColorTable(colorTable);
	} else {
		qImage = new QImage(qImageBuffer, width, height, QImage::Format_RGB32);
	}
	free(qImageBuffer);
	return qImage;
}

GamTexture *GamCapture::queryFrame(void)
{
	IplImage *frame = cvQueryFrame(capture);//not allocated
	QImage *image = convertFromIplImageToQImage(frame, 0.0, 0.0);
	GamTexture *texture = new GamTexture(image);
	texture->ipl = cvCloneImage(frame);//save image of iplimage version
	delete image;
	return texture;
}

GamCapture::~GamCapture(void)
{
	cvReleaseCapture(&capture);
}

//========================================= GamCamera ==============================================//
GamCamera::GamCamera(int n)
{
	capture = cvCaptureFromCAM(n);
}

//========================================== GamVideo ==============================================//
GamVideo::GamVideo(const char *filename)
{
	capture = cvCaptureFromAVI(filename);
}

GamJoint::GamJoint(void)
{
	body_userdata = new GamObject();
}

void GamJoint::setBodyUserData(void *userdata)
{
	body_userdata->userdata = userdata;
}

#define GET_BODY(T, o) ((T)o)->body
b2Body *GamJoint::getBody(GamObject *o)
{
	b2Body *body = NULL;
	switch (o->tag()) {
	case GamRectTag:
		body = GET_BODY(GamRect *, o);
		break;
	case GamEllipseTag:
		body = GET_BODY(GamEllipse *, o);
		break;
	case GamTextureTag:
		body = GET_BODY(GamTexture *, o);
		break;
	case GamTextTag:
		body = GET_BODY(GamText *, o);
		break;
	case GamLineTag:
		body = GET_BODY(GamLine *, o);
		break;
	case GamComplexItemTag:
		body = GET_BODY(GamComplexItem *, o);
		break;
	default:
		fprintf(stderr, "World: [WARNING] UNNOWN OBJECT\n");
		break;
	}
	return body;
}

#define GET_DISTANCE_(x, y) sqrt((x) * (x) + (y) * (y))
#define GET_DISTANCE(posA, posB) GET_DISTANCE_(posA.x - posA.y, posB.x - posB.y)

//============================================ GamDistanceJoint ==============================================//
GamDistanceJoint::GamDistanceJoint(GamObject *o1, GamObject *o2)
{
	b2Body *bodyA = getBody(o1);
	b2Body *bodyB = getBody(o2);
	if (bodyA == NULL || bodyB == NULL) {
		fprintf(stderr, "GamDistanceJoint: ERROR!!, Please call GamWorld.add(GamObject o) previously.\n");
		return;
	}
	b2Vec2 posA = bodyA->GetPosition();
	b2Vec2 posB = bodyB->GetPosition();
	fprintf(stderr, "GamDistanceJoint: bodyA = (%f, %f) : bodyB = (%f, %f)\n", posA.x, posA.y, posB.x, posB.y);
	Initialize(bodyA, bodyB, posA, posB);
	setLine(posA.x * PTM_RATIO, posA.y * PTM_RATIO, posB.x * PTM_RATIO, posB.y * PTM_RATIO);
	collideConnected = true;
	frequencyHz = 4.0f;
	dampingRatio = 0.5f;
	float distance = GET_DISTANCE(posA, posB);
	length = distance;
	QGraphicsLineItem *i = dynamic_cast<QGraphicsLineItem *>(this);
	body_userdata->i = i;
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
	setTag(GamDistanceJointTag);
}

GamDistanceJoint::GamDistanceJoint(GamObject *o1, const GamPoint &p1, GamObject *o2, const GamPoint &p2)
{
	b2Body *bodyA = getBody(o1);
	b2Body *bodyB = getBody(o2);
	if (bodyA == NULL || bodyB == NULL) {
		fprintf(stderr, "GamDistanceJoint: ERROR!!, Please call GamWorld.add(GamObject o) previously.\n");
		return;
	}
	b2Vec2 posA(p1.x/PTM_RATIO, p1.y/PTM_RATIO);
	b2Vec2 posB(p2.x/PTM_RATIO, p2.y/PTM_RATIO);
	fprintf(stderr, "GamDistanceJoint: bodyA = (%f, %f) : bodyB = (%f, %f)\n", posA.x, posA.y, posB.x, posB.y);
	Initialize(bodyA, bodyB, posA, posB);
	setLine(p1.x, p1.y, p2.x, p2.y);
	collideConnected = true;
	frequencyHz = 4.0f;
	dampingRatio = 0.5f;
	float distance = GET_DISTANCE(posA, posB);
	length = distance;
	QGraphicsLineItem *i = dynamic_cast<QGraphicsLineItem *>(this);
	body_userdata->i = i;
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
	setTag(GamDistanceJointTag);
}

void GamDistanceJoint::setLocalAnchorA(const GamPoint &p)
{
	localAnchorA = b2Vec2(p.x/PTM_RATIO, p.y/PTM_RATIO);
}

void GamDistanceJoint::setLocalAnchorB(const GamPoint &p)
{
	localAnchorB = b2Vec2(p.x/PTM_RATIO, p.y/PTM_RATIO);
}

void GamDistanceJoint::setFrequencyHz(float frequency)
{
	frequencyHz = frequency;
}

void GamDistanceJoint::setDampingRatio(float ratio)
{
	dampingRatio = ratio;
}

void GamDistanceJoint::setLength(float length_)
{
	length = length_ / PTM_RATIO;
}

void GamDistanceJoint::setCollideConnected(bool b)
{
	collideConnected = b;
}

void GamDistanceJoint::addToWorld(GamWorld *w)
{
	b2World *world = w->world;
	b2DistanceJoint *j = (b2DistanceJoint *)world->CreateJoint(this);
	joint = j;
	joint->SetUserData(body_userdata);
}

#define DEGREE_TO_RADIAN(deg) deg * 2 * M_PI / 360.0f

//=========================================== GamRevoluteJoint ===============================================//
GamRevoluteJoint::GamRevoluteJoint(GamObject *o1, GamObject *o2)
{
	b2Body *bodyA = getBody(o1);
	b2Body *bodyB = getBody(o2);
	if (bodyA == NULL || bodyB == NULL) {
		fprintf(stderr, "GamRevoluteJoint: ERROR!!, Please call GamWorld.add(GamObject o) previously.\n");
		return;
	}
	b2Vec2 posA = bodyA->GetPosition();
	b2Vec2 posB = bodyB->GetPosition();
	fprintf(stderr, "GamRevoluteJoint: bodyA = (%f, %f) : bodyB = (%f, %f)\n", posA.x, posA.y, posB.x, posB.y);
	Initialize(bodyA, bodyB, posA);
	setLine(posA.x * PTM_RATIO, posA.y * PTM_RATIO, posB.x * PTM_RATIO, posB.y * PTM_RATIO);
	lowerAngle = DEGREE_TO_RADIAN(-90);
	upperAngle = DEGREE_TO_RADIAN(45);
	enableLimit = true;
	maxMotorTorque = 10.0f;
	motorSpeed = 0.0f;
	enableMotor = true;
	QGraphicsLineItem *i = dynamic_cast<QGraphicsLineItem *>(this);
	body_userdata->i = i;
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
	setTag(GamRevoluteJointTag);
}

GamRevoluteJoint::GamRevoluteJoint(GamObject *o1, GamObject *o2, const GamPoint &anchor)
{
	b2Body *bodyA = getBody(o1);
	b2Body *bodyB = getBody(o2);
	if (bodyA == NULL || bodyB == NULL) {
		fprintf(stderr, "GamRevoluteJoint: ERROR!!, Please call GamWorld.add(GamObject o) previously.\n");
		return;
	}
	b2Vec2 posA = bodyA->GetPosition();
	b2Vec2 posB = bodyB->GetPosition();
	fprintf(stderr, "GamRevoluteJoint: bodyA = (%f, %f) : bodyB = (%f, %f)\n", posA.x, posA.y, posB.x, posB.y);
	Initialize(bodyA, bodyB, b2Vec2(anchor.x/PTM_RATIO, anchor.y/PTM_RATIO));
	setLine(posA.x * PTM_RATIO, posA.y * PTM_RATIO, posB.x * PTM_RATIO, posB.y * PTM_RATIO);
	lowerAngle = DEGREE_TO_RADIAN(-90);
	upperAngle = DEGREE_TO_RADIAN(45);
	enableLimit = true;
	maxMotorTorque = 10.0f;
	motorSpeed = 0.0f;
	enableMotor = true;
	QGraphicsLineItem *i = dynamic_cast<QGraphicsLineItem *>(this);
	body_userdata->i = i;
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
	setTag(GamRevoluteJointTag);
}

void GamRevoluteJoint::setLocalAnchorA(const GamPoint &p)
{
	localAnchorA = b2Vec2(p.x/PTM_RATIO, p.y/PTM_RATIO);
}

void GamRevoluteJoint::setLocalAnchorB(const GamPoint &p)
{
	localAnchorB = b2Vec2(p.x/PTM_RATIO, p.y/PTM_RATIO);
}

void GamRevoluteJoint::setLowerAngle(float degree_angle)
{
	lowerAngle = DEGREE_TO_RADIAN(degree_angle);
}

void GamRevoluteJoint::setUpperAngle(float degree_angle)
{
	upperAngle = DEGREE_TO_RADIAN(degree_angle);
}

void GamRevoluteJoint::setEnableLimit(bool b)
{
	enableLimit = b;
}

void GamRevoluteJoint::setMaxMotorTorque(float torque)
{
	maxMotorTorque = torque;
}

void GamRevoluteJoint::setMotorSpeed(float speed)
{
	motorSpeed = speed;
}

void GamRevoluteJoint::setEnableMotor(bool b)
{
	enableMotor = b;
}

void GamRevoluteJoint::addToWorld(GamWorld *w)
{
	b2World *world = w->world;
	b2RevoluteJoint *j = (b2RevoluteJoint *)world->CreateJoint(this);
	joint = j;
	joint->SetUserData(body_userdata);
}

//========================================== GamPrismaticJoint ===============================================//
GamPrismaticJoint::GamPrismaticJoint(GamObject *o1, GamObject *o2)
{
	b2Body *bodyA = getBody(o1);
	b2Body *bodyB = getBody(o2);
	if (bodyA == NULL || bodyB == NULL) {
		fprintf(stderr, "GamPrismaticJoint: ERROR!!, Please call GamWorld.add(GamObject o) previously.\n");
		return;
	}
	b2Vec2 posA = bodyA->GetPosition();
	b2Vec2 posB = bodyB->GetPosition();
	fprintf(stderr, "GamPrismaticJoint: bodyA = (%f, %f) : bodyB = (%f, %f)\n", posA.x, posA.y, posB.x, posB.y);
	b2Vec2 worldAxis(1.0f, 0.0f);
	Initialize(bodyA, bodyB, posA, worldAxis);
	lowerTranslation = -5.0f;
	upperTranslation = 2.5f;
	enableLimit = true;
	maxMotorForce = 1.0f;
	motorSpeed = 0.0f;
	enableMotor = true;
	setLine(posA.x * PTM_RATIO, posA.y * PTM_RATIO, posB.x * PTM_RATIO, posB.y * PTM_RATIO);
	QGraphicsLineItem *i = dynamic_cast<QGraphicsLineItem *>(this);
	body_userdata->i = i;
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
	setTag(GamPrismaticJointTag);
}

void GamPrismaticJoint::setLowerTranslation(float translation)
{
	lowerTranslation = translation;
}

void GamPrismaticJoint::setUpperTranslation(float translation)
{
	upperTranslation = translation;
}

void GamPrismaticJoint::setEnableLimit(bool b)
{
	enableLimit = b;
}

void GamPrismaticJoint::setMaxMotorForce(float force)
{
	maxMotorForce = force;
}

void GamPrismaticJoint::setMotorSpeed(float speed)
{
	motorSpeed = speed;
}

void GamPrismaticJoint::setEnableMotor(bool b)
{
	enableMotor = b;
}

void GamPrismaticJoint::addToWorld(GamWorld *w)
{
	b2World *world = w->world;
	b2PrismaticJoint *j = (b2PrismaticJoint *)world->CreateJoint(this);
	joint = j;
	joint->SetUserData(body_userdata);
}

//========================================== GamPulleyJoint ===============================================//
GamPulleyJoint::GamPulleyJoint(GamObject *o1, const GamPoint &p1, GamObject *o2, const GamPoint &p2)
{
	b2Body *bodyA = getBody(o1);
	b2Body *bodyB = getBody(o2);
	if (bodyA == NULL || bodyB == NULL) {
		fprintf(stderr, "GamPulleyJoint: ERROR!!, Please call GamWorld.add(GamObject o) previously.\n");
		return;
	}
	b2Vec2 posA = bodyA->GetPosition();
	b2Vec2 posB = bodyB->GetPosition();
	fprintf(stderr, "GamPulleyJoint: bodyA = (%f, %f) : bodyB = (%f, %f)\n", posA.x, posA.y, posB.x, posB.y);
	b2Vec2 anchorA = posA;
	b2Vec2 anchorB = posB;
	b2Vec2 groundAnchorA(p1.x/PTM_RATIO, p1.y/PTM_RATIO);
	b2Vec2 groundAnchorB(p2.x/PTM_RATIO, p2.y/PTM_RATIO);
	float32 ratio = 1.0f;
	Initialize(bodyA, bodyB, groundAnchorA, groundAnchorB, anchorA, anchorB, ratio);
	QGraphicsPathItem *i = dynamic_cast<QGraphicsPathItem *>(this);
	body_userdata->i = i;
	GamObject *o = (GamObject *)this;
	setBodyUserData(o);
	setTag(GamPulleyJointTag);
	QPainterPath path;
	path.moveTo(posA.x * PTM_RATIO, posA.y * PTM_RATIO);
	path.lineTo(p1.x, p1.y);
	path.lineTo(p2.x, p2.y);
	path.lineTo(posB.x * PTM_RATIO, posB.y * PTM_RATIO);
	setPath(path);
}

void GamPulleyJoint::addToWorld(GamWorld *w)
{
	b2World *world = w->world;
	b2PulleyJoint *j = (b2PulleyJoint *)world->CreateJoint(this);
	joint = j;
	joint->SetUserData(body_userdata);
}

//========================================== GamGearJoint ===============================================//
GamGearJoint::GamGearJoint(GamObject *o1, GamJoint *j1, GamObject *o2, GamJoint *j2)
{
	b2Body *bodyA = getBody(o1);
	b2Body *bodyB = getBody(o2);
	fprintf(stderr, "GamGearJoint: bodyA = (%f, %f) : bodyB = (%f, %f)\n",
			bodyA->GetPosition().x, bodyA->GetPosition().y,
			bodyB->GetPosition().x, bodyB->GetPosition().y);
	this->bodyA = bodyA;
	this->bodyB = bodyB;
	joint1 = j1->joint;
	joint2 = j2->joint;
	float length = 100 / PTM_RATIO;
	ratio = 2 * M_PI / length;
	setTag(GamGearJointTag);
}

void GamGearJoint::addToWorld(GamWorld *w)
{
	b2World *world = w->world;
	b2GearJoint *j = (b2GearJoint *)world->CreateJoint(this);
	joint = j;
}

//===================================== Written by Takuma Wakamori ===========================================//
#include <del_interface.hpp>

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
