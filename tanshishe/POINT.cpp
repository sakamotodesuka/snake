#include"POINT.h"

Point::Point(){
	x = 27;
	y = 27;
}

Point::Point(int x, int y){
	this->x = x;
	this->y = y;
}

bool Point::operator==(Point& point){
	if (this->x == point.x && this->y == point.y)
		return 1;
	else return 0;
}

Point& Point::operator=(Point& point){
	x = point.x;
	y = point.y;
	return *this;
}