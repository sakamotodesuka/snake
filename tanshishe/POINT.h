#pragma once

class Point{
public:
	int x, y;
	Point();
	Point(int, int);
	bool operator==(Point& point);
	Point& operator=(Point& point);
};