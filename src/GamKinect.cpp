#include <gam.hpp>

GamPerson::GamPerson(unsigned int label_, const GamVector &pts) : GamComplexItem()
{
	label = label_;
	int size = pts.size();
	gp_list = new QList<QGraphicsPolygonItem *>();
	if (size < 3) return;
	//fprintf(stderr, "pts_length = [%d]\n", size);
	std::vector<Triangle> tris = triangulate(pts, (float)size);
	for (std::vector<Triangle>::iterator triIt = tris.begin(); triIt != tris.end(); ++triIt) {
		QPolygonF p;
		p << QPoint(triIt->a.x, triIt->a.y);
		p << QPoint(triIt->b.x, triIt->b.y);
		p << QPoint(triIt->c.x, triIt->c.y);
		QGraphicsPolygonItem *gp = new QGraphicsPolygonItem();
		gp->setPolygon(p);
		gp->setPen(Qt::NoPen);
		gp->setBrush(QColor("#00ff00"));
		addToGroup(gp);
		gp_list->append(gp);
	}
}

//void GamPerson::strokePath(void)
void GamPerson::createShape(const GamVector &pts)
{
	int size = pts.size();
	if (size < 3) return;
	//fprintf(stderr, "pts_length = [%d]\n", size);
	std::vector<Triangle> tris = triangulate(pts, (float)size);
	for (std::vector<Triangle>::iterator triIt = tris.begin(); triIt != tris.end(); ++triIt) {
		QPolygonF p;
		p << QPoint(triIt->a.x, triIt->a.y);
		p << QPoint(triIt->b.x, triIt->b.y);
		p << QPoint(triIt->c.x, triIt->c.y);
		QGraphicsPolygonItem *gp = new QGraphicsPolygonItem();
		gp->setPolygon(p);
		gp->setPen(Qt::NoPen);
		//gp->setBrush(QColor((int)triIt->a.x, (int)triIt->b.x, (int)triIt->c.x));
		gp->setBrush(QColor("#00ff00"));
		//poly_list->append(gp);
		addToGroup(gp);
		//fprintf(stderr, "(%f, %f), (%f, %f), (%f, %f)\n", triIt->a.x, triIt->a.y, triIt->b.x, triIt->b.y, triIt->c.x, triIt->c.y);
	}
	//QPainterPath path;
	//GamPoint p = pts->at(0);
	//path.moveTo(p.x, p.y);
	//for (int i = 1; i < size; i++) {
	//p = pts->at(i);
	//path.lineTo(p.x, p.y);
	//}
	//setPath(path);
	//setBrush(QColor("#00ff00"));
}

GamPerson::~GamPerson(void)
{
	if (gp_list) {
		int size = gp_list->length();
		for (int i = 0; i < size; i++) {
			delete gp_list->at(i);
		}
		delete gp_list;
	}
}

void GamPerson::addEdgePoint(const GamPoint &p)
{
	//pts->append(p);
}

GamPeople::GamPeople(int max_num)
{
	num = 0;
	capacity = max_num;
	persons = new QList<GamPerson *>();
}

void GamPeople::addPerson(GamPerson *person)
{
	num++;
	persons->append(person);
}

unsigned int GamPeople::length(void)
{
	return num;
}

GamPerson *GamPeople::getPerson(int n)
{
	return persons->at(n);
}

void GamPeople::clear(void)
{
	num = 0;
	for (int i = 0; i < num; i++) {
		GamPerson *p = persons->at(i);
	}
}

GamPeople::~GamPeople(void)
{
	if (persons) {
		int size = persons->length();
		for (int i = 0; i < size; i++) {
			GamPerson *p = persons->at(i);
			delete p;
		}
		delete persons;
	}
}

static GamKinect *kinect = NULL;

static void focusProgress(const XnChar* strFocus, const XnPoint3D &ptPosition, XnFloat fProgress, void* UserCxt)
{
	fprintf(stderr, "Focus progress: %s @(%f,%f,%f): %f\n", strFocus, ptPosition.X, ptPosition.Y, ptPosition.Z, fProgress);
}

static void startSession(const XnPoint3D &ptPosition, void *ctx)
{
	fprintf(stderr, "Session start: (%f,%f,%f)\n", ptPosition.X, ptPosition.Y, ptPosition.Z);
	if (kinect) {
		kinect->startSessionHandler(ptPosition, ctx);
	}
}

static void endSession(void *ctx)
{
	fprintf(stderr, "Session end\n");
	if (kinect) {
		kinect->endSessionHandler(ctx);
	}
}

static void noHands(void *ctx)
{
	//if (sessionState != NOT_IN_SESSION) {
	//fprintf(stderr, "Quick refocus\n");
	//sessionState = QUICK_REFOCUS;
	//}
}

static void push(XnFloat fVelocity, XnFloat fAngle, void *ctx)
{
	if (kinect) {
		kinect->pushHandler(fVelocity, fAngle, ctx);
	}
}

static void swipe(XnFloat fVelocity, XnFloat fAngle, void *ctx)
{
	if (kinect) {
		kinect->swipeHandler(fVelocity, fAngle, ctx);
	}
}

static void touching(xn::HandTouchingFOVEdgeCapability &generator, XnUserID id,
					 const XnPoint3D* pPosition, XnFloat fTime, XnDirection eDir, void* pCookie)
{
	fprintf(stderr, "TouchingCallback\n");
	//g_pDrawer->SetTouchingFOVEdge(id);
}

static void gestureIntermediateStageCompleted(xn::GestureGenerator& generator, const XnChar* strGesture,
											  const XnPoint3D* pPosition, void* pCookie)
{
	fprintf(stderr, "Gesture %s: Intermediate stage complete (%f,%f,%f)\n", strGesture, pPosition->X, pPosition->Y, pPosition->Z);
}

static void gestureReadyForNextIntermediateStage(xn::GestureGenerator& generator, const XnChar* strGesture,
												 const XnPoint3D* pPosition, void* pCookie)
{
	fprintf(stderr, "Gesture %s: Ready for next intermediate stage (%f,%f,%f)\n", strGesture, pPosition->X, pPosition->Y, pPosition->Z);
}

static void gestureProgress(xn::GestureGenerator& generator, const XnChar* strGesture,
							const XnPoint3D* pPosition, XnFloat fProgress, void* pCookie)
{
	fprintf(stderr, "Gesture %s progress: %f (%f,%f,%f)\n", strGesture, fProgress, pPosition->X, pPosition->Y, pPosition->Z);
}

using namespace xn;

XnVPointDrawer::XnVPointDrawer(XnUInt32 nHistory, xn::DepthGenerator depthGenerator) :
	XnVPointControl("XnVPointDrawer"),
	m_nHistorySize(nHistory), m_DepthGenerator(depthGenerator), m_bDrawDM(false), m_bFrameID(false)
{
	m_pfPositionBuffer = new XnFloat[nHistory*3];
}

XnVPointDrawer::~XnVPointDrawer()
{
	std::map<XnUInt32, std::list<XnPoint3D> >::iterator iter;
	for (iter = m_History.begin(); iter != m_History.end(); ++iter)
	{
		iter->second.clear();
	}
	m_History.clear();
	delete []m_pfPositionBuffer;
}

void XnVPointDrawer::OnPointCreate(const XnVHandPointContext* cxt)
{
	fprintf(stderr, "** %d\n", cxt->nID);
	// Create entry for the hand
	m_History[cxt->nID].clear();
	OnPointUpdate(cxt);
}

void XnVPointDrawer::OnPointUpdate(const XnVHandPointContext* cxt)
{
	XnPoint3D ptProjective(cxt->ptPosition);
	fprintf(stderr, "Point (%f,%f,%f)", ptProjective.X, ptProjective.Y, ptProjective.Z);
	m_DepthGenerator.ConvertRealWorldToProjective(1, &ptProjective, &ptProjective);
	fprintf(stderr, " -> (%f,%f,%f)\n", ptProjective.X, ptProjective.Y, ptProjective.Z);
	GamPoint p(ptProjective.X, ptProjective.Y);
	float z = ptProjective.Z;
	emit updateHandPositionSignal(p, z);
	m_History[cxt->nID].push_front(ptProjective);
	if (m_History[cxt->nID].size() > m_nHistorySize)
		m_History[cxt->nID].pop_back();
}

void XnVPointDrawer::OnPointDestroy(XnUInt32 nID)
{
	m_History.erase(nID);
}

void XnVPointDrawer::Update(XnVMessage* pMessage)
{
	XnVPointControl::Update(pMessage);
	//if (m_bDrawDM) {
	//xn::DepthMetaData depthMD;
	//m_DepthGenerator.GetMetaData(depthMD);
	//DrawDepthMap(depthMD);
	//}
	/*
	std::map<XnUInt32, std::list<XnPoint3D> >::const_iterator PointIterator;
	for (PointIterator = m_History.begin(); PointIterator != m_History.end(); ++PointIterator) {
		// Clear buffer
		XnUInt32 nPoints = 0;
		XnUInt32 i = 0;
		XnUInt32 Id = PointIterator->first;
		std::list<XnPoint3D>::const_iterator PositionIterator;
		fprintf(stderr, "===================================\n");
		for (PositionIterator = PointIterator->second.begin(); PositionIterator != PointIterator->second.end(); ++PositionIterator, ++i) {
			// Add position to buffer
			XnPoint3D pt(*PositionIterator);
			m_pfPositionBuffer[3*i] = pt.X;
			m_pfPositionBuffer[3*i + 1] = pt.Y;
			m_pfPositionBuffer[3*i + 2] = 0;//pt.Z();
			fprintf(stderr, "(x, y) = (%f, %f)\n", pt.X, pt.Y);
		}
	}
	*/
	m_TouchingFOVEdge.clear();
}

void XnVPointDrawer::SetDepthMap(XnBool bDrawDM)
{
	m_bDrawDM = bDrawDM;
}

void XnVPointDrawer::SetFrameID(XnBool bFrameID)
{
	m_bFrameID = bFrameID;
}

void XnVPointDrawer::SetTouchingFOVEdge(XnUInt32 nID)
{
	m_TouchingFOVEdge.push_front(nID);
}

XnBool XnVPointDrawer::IsTouching(XnUInt32 id) const
{
	for (std::list<XnUInt32>::const_iterator iter = m_TouchingFOVEdge.begin(); iter != m_TouchingFOVEdge.end(); ++iter)
	{
		if (*iter == id)
			return TRUE;
	}
	return FALSE;
}

GamKinect::GamKinect(const char *xml)
{
	people = NULL;
	background_texture = NULL;
	context = new Context();
	XnStatus rc = context->InitFromXmlFile(xml);
	userGenerator = new UserGenerator();
	imageGenerator = new ImageGenerator();
	depthGenerator = new DepthGenerator();
	handsGenerator = new HandsGenerator();
	gestureGenerator = new GestureGenerator();
	XnStatus val = context->FindExistingNode(XN_NODE_TYPE_USER, *userGenerator);
	context->FindExistingNode(XN_NODE_TYPE_IMAGE, *imageGenerator);
	context->FindExistingNode (XN_NODE_TYPE_DEPTH, *depthGenerator);
	context->FindExistingNode (XN_NODE_TYPE_HANDS, *handsGenerator);
	context->FindExistingNode (XN_NODE_TYPE_GESTURE, *gestureGenerator);
	XnCallbackHandle h;
	if (handsGenerator->IsCapabilitySupported(XN_CAPABILITY_HAND_TOUCHING_FOV_EDGE)) {
		handsGenerator->GetHandTouchingFOVEdgeCap().RegisterToHandTouchingFOVEdge(&touching, NULL, h);
	}
	if (val != XN_STATUS_OK) {
		userGenerator->Create(*context);
	}
	XnCallbackHandle intermediateStageCompleted, progress, readyForNextIntermediateStage;
	gestureGenerator->RegisterToGestureIntermediateStageCompleted(gestureIntermediateStageCompleted,
															  NULL, intermediateStageCompleted);
	gestureGenerator->RegisterToGestureReadyForNextIntermediateStage(gestureReadyForNextIntermediateStage,
																 NULL, readyForNextIntermediateStage);
	gestureGenerator->RegisterGestureCallbacks(NULL, gestureProgress, NULL, progress);

	sessionManager = new XnVSessionManager();
	rc = sessionManager->Initialize(context, "Wave,Click", "RaiseHand");
	sessionManager->RegisterSession(NULL, startSession, endSession, focusProgress);

	pushDetector = new XnVPushDetector();
	pushDetector->RegisterPush(NULL, &push);
	sessionManager->AddListener(pushDetector);
	swipeDetector = new XnVSwipeDetector();
	swipeDetector->RegisterSwipeUp(NULL, &swipe);
	sessionManager->AddListener(swipeDetector);
	drawer = new XnVPointDrawer(20, *depthGenerator);
	//flowRouter = new XnVFlowRouter();
	//flowRouter->SetActive(drawer);
	sessionManager->AddListener(drawer);
	drawer->RegisterNoPoints(NULL, noHands);
	drawer->SetDepthMap(false);
	connect(drawer, SIGNAL(updateHandPositionSignal(GamPoint, float)),
			this, SLOT(updateHandPositionHandler(GamPoint, float)));
	depthGenerator->GetAlternativeViewPointCap().SetViewPoint(*imageGenerator);
	rc = context->StartGeneratingAll();
	imageMD = new ImageMetaData();
	iplImage = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
	blend_texture = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
	context->WaitAndUpdateAll();
	kinect = this;
}

void GamKinect::updateHandPositionHandler(GamPoint p, float z)
{
	emit updateHandPositionSignal(p, z);
}

void GamKinect::startSessionHandler(const XnPoint3D &pFocus, void *ctx)
{
	emit startSessionSignal(pFocus, ctx);
}

void GamKinect::endSessionHandler(void *ctx)
{
	emit endSessionSignal(ctx);
}

void GamKinect::pushHandler(XnFloat fVelocity, XnFloat fAngle, void *ctx)
{
	emit pushSignal(fVelocity, fAngle, ctx);
}

void GamKinect::swipeHandler(XnFloat fVelocity, XnFloat fAngle, void *ctx)
{
	emit swipeSignal(fVelocity, fAngle, ctx);
}

GamTexture *GamKinect::queryFrame(void)
{
	imageGenerator->GetMetaData(*imageMD);
	//int width = imageMD->XRes();
	//int height = imageMD->YRes();
	//fprintf(stderr, "width = [%d], height = [%d]\n", width, height);
	//iplImage = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	iplImage->imageData = (char *)imageMD->WritableData();
	cvCvtColor(iplImage, iplImage, CV_RGB2BGR);
	QImage *image = convertFromIplImageToQImage(iplImage, 0.0, 0.0);
	GamTexture *texture = new GamTexture(image);
	delete image;
	//cvReleaseImage(&iplImage);
	return texture;
}

GamTexture *GamKinect::queryBlendFrame(void)
{
	cvCvtColor(blend_texture, blend_texture, CV_RGB2BGR);
	//cvCvtColor(background_texture, background_texture, CV_RGB2BGR);
	QImage *image = convertFromIplImageToQImage(blend_texture, 0.0, 0.0);
	//QImage *image = convertFromIplImageToQImage(background_texture, 0.0, 0.0);
	GamTexture *texture = new GamTexture(image);
	delete image;
	return texture;
}

void GamKinect::update(void)
{
	sessionManager->Update(context);
	context->WaitAndUpdateAll();
	xn::DepthMetaData dmd;
	depthGenerator->GetMetaData(dmd);
	context->WaitOneUpdateAll(*depthGenerator);
	imageGenerator->GetMetaData(*imageMD);
	xn::SceneMetaData smd;
	userGenerator->GetUserPixels(0, smd);
	const XnLabel* pLabels = smd.Data();//user labels' pointer
	XnUInt16 width = dmd.XRes();
	XnUInt16 height = dmd.YRes();
	bool user_found_flag = false;
	char *imagePtr = (char *)imageMD->WritableData();
	char *headPtr = imagePtr;
	char *background = NULL;
	if (background_texture) {
		background = background_texture->imageData;
	}
	char *blend = blend_texture->imageData;
	for (int y = 0; y < width; y++) {
		for (int x = 0; x < height; x++) {
			if (*pLabels != 0) {
				user_found_flag = true;
				blend[0] = imagePtr[0];
				blend[1] = imagePtr[1];
				blend[2] = imagePtr[2];
				imagePtr[0] = 255;
				imagePtr[1] = 255;
				imagePtr[2] = 255;
			} else {
				imagePtr[0] = 0;
				imagePtr[1] = 0;
				imagePtr[2] = 0;
				if (background) {
					blend[0] = background[0];
					blend[1] = background[1];
					blend[2] = background[2];
				}
			}
			pLabels++;
			imagePtr+=3;
			blend+=3;
			if (background) {
				background+=3;
			}
		}
	}
	//delete people; ==> please delete application side
	people = NULL;
	if (user_found_flag) {
		iplImage->imageData = headPtr;//(char *)imageMD->WritableData();
		cvCvtColor(iplImage, iplImage, CV_RGB2BGR);
		setEdgePointToPeople(iplImage);
		//emit foundPeopleSignal(people);
	}
}

GamPeople *GamKinect::getPeople(void)
{
	return people;
}

void GamKinect::setEdgePointToPeople(IplImage *src_img)
{
	people = new GamPeople(0);
	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq *contours = NULL;
	IplImage *src_img_gray = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
	cvCvtColor(src_img, src_img_gray, CV_BGR2GRAY);
	IplImage *tmp_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
	cvThreshold(src_img_gray, tmp_img, 120, 255, CV_THRESH_BINARY);
	cvFindContours(tmp_img, storage, &contours, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	int label = 0;
	people->clear();
	while (1) {
		if (contours == NULL) break;
		GamVector obj;
		for (int i = 0; i < contours->total; i++) {
			CvPoint *point = CV_GET_SEQ_ELEM(CvPoint, contours, i);
			obj.push_back(Vec2f(point->x, point->y));
			//person->addEdgePoint(GamPoint(point->x, point->y));
		}
		GamPerson *person = new GamPerson(label, obj);//people->getPerson(label);
		//person->createShape(obj);
		//person->strokePath();
		people->addPerson(person);
		if (contours->h_next == NULL) break;
		contours = contours->h_next;
		label++;
	}
	//cvReleaseImage(&src_img);
	cvReleaseImage(&src_img_gray);
	cvReleaseImage(&tmp_img);
	cvReleaseMemStorage(&storage);
}

void GamKinect::setBackgroundTexture(GamTexture *texture)
{
	if (!texture->ipl) {
		fprintf(stderr, "WARNING: cannot set background texture\n");
		return;
	}
	background_texture = texture->ipl;
	cvResize(texture->ipl, background_texture, CV_INTER_NN);
	cvCvtColor(background_texture, background_texture, CV_RGB2BGR);
}

GamKinect::~GamKinect(void)
{
	context->Shutdown();
}
