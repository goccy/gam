#include <gam.hpp>

class WobbleTexture {
public:
	GamScene *scene;
	GamWorld *world;
	QList<GamEllipse*> *pts;
	WobbleTexture(void);
	void makeCage(int width, int height);
	void show(void);
};
