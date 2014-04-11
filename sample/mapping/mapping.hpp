#include <QtGui>
#include <QtOpenGL>

class MyView : public QGraphicsView {
	Q_OBJECT;
public:
	MyView(QGraphicsScene *s);
	void paintEvent(QPaintEvent *event);
};

class GLWidget : public QGLWidget {
	Q_OBJECT;
public:
	int width;
	int height;
	QGraphicsScene *scene;
	MyView *view;

	GLWidget(QWidget *parent);
	void setScene(QGraphicsScene *scene);
	void setView(MyView *view);
	void timerEvent(QTimerEvent *event);
	void initializeGL(void);
	void paintGL(void);
	void resizeGL(int width, int height);
};
