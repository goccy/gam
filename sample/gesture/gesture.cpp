#include <gesture.hpp>

GestureWidget::GestureWidget(void)
{
	scene = new GamScene();
	world = new GamWorld(scene);
	scene->setSceneRect(0, 0, 640, 480);
	handPoint = new GamEllipse();
	handPoint->setRectShape(&GamRect(0, 0, 30, 30));
	handPoint->setBrush(QColor("#00eeee"));
	handPoint->setOpacity(0);
	handPoint->setZValue(10);
	scene->addItem(handPoint);
	added_persons = new QList<GamPerson *>();
	prev_texture = NULL;
	prev_people = NULL;
	srand((unsigned)time(NULL));
	for (int i = 0; i < 100; i++) {
		int r = rand() % 100;
		GamRect *rect = new GamRect(300 - r, 100 - r, 30, 30);
		rect->setDensity(1);
		rect->setZValue(12);
		//rect.setFriction(0.3);
		rect->setRestitution(0.1);
		rect->setBrush(QColor("#87ceeb"));//155 - r, 200 - r, 255 - r));
		world->add(rect);
		scene->addItem(rect);
	}
	GamRect *ground = new GamRect(0, 470, 640, 10);
	ground->setBrush(QColor("#ffff00"));
	world->add(ground);
	scene->addItem(ground);
	GamRect *left_wall = new GamRect(0, 0, 10, 480);
	GamRect *right_wall = new GamRect(630, 10, 10, 480);
	left_wall->setBrush(QColor("#ffff00"));
	right_wall->setBrush(QColor("#ffff00"));
	world->add(left_wall);
	world->add(right_wall);
	scene->addItem(left_wall);
	scene->addItem(right_wall);
	kinect = new GamKinect("Sample-Tracking.xml");
	GamTexture *background = new GamTexture("sky.jpg");
	kinect->setBackgroundTexture(background);
	connect(kinect, SIGNAL(startSessionSignal(const XnPoint3D, void *)),
			this, SLOT(startSession(const XnPoint3D, void *)));
	connect(kinect, SIGNAL(endSessionSignal(void *)),
			this, SLOT(endSession(void *)));
	connect(kinect, SIGNAL(pushSignal(XnFloat, XnFloat, void *)),
			this, SLOT(push(XnFloat, XnFloat, void *)));
	connect(kinect, SIGNAL(swipeSignal(XnFloat, XnFloat, void *)),
			this, SLOT(swipe(XnFloat, XnFloat, void *)));
	connect(kinect, SIGNAL(updateHandPositionSignal(GamPoint, float)),
			this, SLOT(updateHandPosition(GamPoint, float)));
}

void GestureWidget::updateHandPosition(GamPoint p, float z)
{
	fprintf(stderr, "update hand position (%d, %d, %f)\n", p.x, p.y, z);
	handPoint->setOpacity(1);
	handPoint->setPos(p.x, p.y);
}

void GestureWidget::timerEvent(QTimerEvent *event)
{
	if (prev_texture) {
		scene->removeItem(prev_texture);
		delete prev_texture;
		prev_texture = NULL;
	}
	int size = added_persons->length();
	for (int i = 0; i < size; i++) {
		GamPerson *person = added_persons->at(i);
		//scene->removeItem(person);
		world->remove(person);
	}
	if (prev_people) {
		delete prev_people;
		prev_people = NULL;
	}
	added_persons->clear();
	//GamTexture *t = kinect->queryFrame();
	GamTexture *t = kinect->queryBlendFrame();
	//t->setRectShape(&GamRect(0, 0, 1920, 1200));
	scene->addItem(t);
	GamPeople *people = kinect->getPeople();
	if (people) {
		//fprintf(stderr, "found people\n");
		int num = people->length();
		for (int i = 0; i < num; i++) {
			GamPerson *person = people->getPerson(i);
			person->setOpacity(0.7);
			added_persons->append(person);
			//scene->addItem(person);
			world->add(person);
		}
	}
	prev_texture = t;
	prev_people = people;
	kinect->update();
	scene->update();
}

void GestureWidget::startSession(const XnPoint3D pFocus, void *ctx)
{
	fprintf(stderr, "startSession!!!!\n");
}

void GestureWidget::endSession(void *ctx)
{
	fprintf(stderr, "endSession!!!!\n");
}

void GestureWidget::push(XnFloat fVelocity, XnFloat fAngle, void *ctx)
{
	fprintf(stderr, "push!!!!\n");
}

void GestureWidget::swipe(XnFloat fVelocity, XnFloat fAngle, void *ctx)
{
	fprintf(stderr, "swipe!!!!\n");
}

void GestureWidget::show(void)
{
	QGraphicsView *view = new QGraphicsView(scene);
	view->setRenderHint(QPainter::Antialiasing);
	view->showFullScreen();
	//view->show();
	//scene->setSceneRect(0, 0, view->width(), view->height());
	view->scale(view->width()/640, view->height()/480);
	startTimer(1000 / 30);
	world->start();
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	GestureWidget gesture;
	gesture.show();
	return app.exec();
}
