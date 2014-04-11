#include <gam.hpp>


class WaterWorld {
public:
	GamScene *scene;
	GamView *view;
	GamWorld *world;

	WaterWorld(void);
	void addGround(void);
	void addWater(void);
	void show(void);
};
