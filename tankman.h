
#include <stdint.h>

typedef struct {
	int16_t y;
	int16_t x;
} Point;

typedef struct {
	double y;
	double x;
} PointD;

Point point_add(Point a, Point b) { return (Point) {a.y + b.y, a.x + b.x}; }

Point point_substract(Point sub, Point src) { return (Point) {src.y - sub.y, src.x - sub.x}; }

typedef struct {
	Point anchor;		// Center point.
	Point size;			// Dimensions of the box (Y, X), Must be larget than 0.
} Box;

Point box_topLeft(Box b) { 
	return (Point) {b.anchor.y - (b.size.y - 1) / 2, b.anchor.x - (b.size.x - 1) / 2}; 
}

Point box_bottomRight(Box b) { 
	return point_add(box_topLeft(b), point_substract((Point) {1, 1}, b.size)); 
}

typedef enum  {SOLID, PLAYER, FOE, BULLET} ENTITY_TYPE;
typedef enum  {SUCCESS, FAILURE, UNKNOWN} STATE;

/***
		Things to be done:
			- Collision detection
			- Moving things around
***/

typedef struct Entity {
	char 			name[16];	// Name of the entity.	
	ENTITY_TYPE 	type;		// Type of the entity. 
	double 			lastSec;	// Time of last refreshing.
	Box				bBox;		// Bounding box of the entity.
	PointD			lastPoz;	// Last position of the entity.
	PointD			poz;		// Position of the entity. (Center)
	PointD			vel;		// Velocity of the entity. Pixel per second.
	Point 			rot;		// Rotation vector of the entity. Must have a length of 1.
	void*			data;		// Custom data of this specific entity.
	STATE			(*refresh)(struct Entity* self);
	STATE			(*draw)(struct Entity* self);
	STATE			(*deallocate)(struct Entity* self);
} Entity;

STATE entity_update_position(Entity* self);

typedef struct SOLID_Data {
	int 			placeholder;
} SOLID_Data;

STATE SOLID_refresh(Entity* self) { 
	entity_update_position(self);
	return SUCCESS; 
}

STATE SOLID_draw(Entity* self);

typedef struct {
	Point canvasSize;
	double thisSec;
	double lastSec;
	Entity* test;
} Game;

