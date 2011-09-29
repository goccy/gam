#include <gam.hpp>

class AR : public QObject {
	Q_OBJECT;
public:
	GamScene *scene;
	GamCamera *camera;
	int timer_id;
	GamTexture *background;
	QList<GamComplexItem *> *prev_list;
	GamWorld *world;
	GamTexture *prev_texture;

	AR(void);
	void show(void);
	void timerEvent(QTimerEvent *event);
};
