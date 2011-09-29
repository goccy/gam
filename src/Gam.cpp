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
	setTag(GamRectTag);
	se = NULL;
	body = NULL;
	isStatic = true;
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
	GamObject *data = new GamObject();
	QGraphicsItem *i = (QGraphicsItem *)this;
	data->i = i;
	GamObject *o = (GamObject *)this;
	data->userdata = o;
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
	setTag(GamEllipseTag);
	glow_center_color = new QColor("white");
	isStatic = true;
}

void GamEllipse::setPosition(int x_, int y_)
{
	x = x_;
	y = y_;
	setPos(x, y);
}

void GamEllipse::setGlow(void)
{
	setPen(Qt::NoPen);
	QColor c = brush().color();
	int orig_center_x = x + width / 2;
	int orig_center_y = y + height / 2;
	int spread = width * 4;
	double default_size = 1 / 4.0f;
	int center_x = x + spread / 2;
	int center_y = y + spread / 2;
	int div_x = orig_center_x - center_x;
	int div_y = orig_center_y - center_y;
	setRect(QRect(x + div_x, y + div_y, spread, spread));
	QRadialGradient radial(orig_center_x, orig_center_y, spread/2);
	QColor c1(c);
	c1.setAlpha(0x99);
	QColor c2(c);
	c2.setAlpha(0x44);
	QColor c3(c);
	c3.setAlpha(0x33);
	QColor c4(c);
	c4.setAlpha(0x00);
	//c4.setAlpha(0x11);
	radial.setColorAt(default_size * 0.4, *glow_center_color);
	radial.setColorAt(default_size * 0.8, c1);
	radial.setColorAt(default_size, c2);
	radial.setColorAt(default_size * 1.0, c3);
	//radial.setColorAt(default_size * 1.5, c3);
	radial.setColorAt(1.0, c4);
	setBrush(QBrush(radial));
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

	//knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	//memset(data, 0, sizeof(knh_GraphicsUserData_t));
	//CTX lctx = knh_getCurrentContext();
	//data->ct = getClassTBL(Ellipse);
	//data->o = i;
	//data->self = this;
	GamObject *data = new GamObject();
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	GamObject *o = (GamObject *)this;
	data->i = i;
	data->userdata = o;
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
	setTag(GamTextTag);
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

	//knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	//memset(data, 0, sizeof(knh_GraphicsUserData_t));
	//CTX lctx = knh_getCurrentContext();
	//data->ct = getClassTBL(Text);
	//data->o = i;
	//data->self = this;
	GamObject *data = new GamObject();
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	GamObject *o = (GamObject *)this;
	data->i = i;
	data->userdata = o;
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
	setTag(GamComplexItemTag);
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
	//knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	//memset(data, 0, sizeof(knh_GraphicsUserData_t));
	//CTX lctx = knh_getCurrentContext();
	//data->ct = getClassTBL(Texture);
	//data->o = i;
	//data->self = this;
	GamObject *data = new GamObject();
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	GamObject *o = (GamObject *)this;
	data->i = i;
	data->userdata = o;
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
	setTag(GamTextureTag);
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
	setTag(GamTextureTag);
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

	//knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	//memset(data, 0, sizeof(knh_GraphicsUserData_t));
	//CTX lctx = knh_getCurrentContext();
	//data->ct = getClassTBL(Texture);
	//data->o = i;
	//data->self = this;
	GamObject *data = new GamObject();
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	GamObject *o = (GamObject *)this;
	data->i = i;
	data->userdata = o;
	body->SetUserData(data);
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
	//knh_GraphicsUserData_t *data = (knh_GraphicsUserData_t *)malloc(sizeof(knh_GraphicsUserData_t));
	//memset(data, 0, sizeof(knh_GraphicsUserData_t));
	//CTX lctx = knh_getCurrentContext();
	//data->ct = getClassTBL(Texture);
	//data->o = i;
	//data->self = this;
	GamObject *data = new GamObject();
	QGraphicsItem *i = dynamic_cast<QGraphicsItem *>(this);
	GamObject *o = (GamObject *)this;
	data->i = i;
	data->userdata = o;
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

template<class T>
inline void removeWorld(GamWorld *world, T o)
{
	b2Body *body = o->body;
	world->world->DestroyBody(body);
}

#define addWorld(T, o) ((T)o)->addToWorld(this)

void GamWorld::add(GamObject *o)
{
	switch (o->tag()) {
	case GamRectTag:
		addWorld(GamRect *, o);
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


//===================================== GamCapture =============================================//
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
