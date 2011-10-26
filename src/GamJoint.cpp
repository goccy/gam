#include <gam.hpp>

//========================================== GamJoint ===============================================//
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
	GamObject *o = dynamic_cast<GamObject *>(this);
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
	GamObject *o = dynamic_cast<GamObject *>(this);
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
	GamObject *o = dynamic_cast<GamObject *>(this);
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
	GamObject *o = dynamic_cast<GamObject *>(this);
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
	GamObject *o = dynamic_cast<GamObject *>(this);
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
	GamObject *o = dynamic_cast<GamObject *>(this);
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
