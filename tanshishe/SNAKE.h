#pragma once
#include"POINT.h"

class SNAKE{
public:
	int direction, Nnode, score;
	char op_dir; //w for down ,s for up, a for right, d for left

	Point head, body[40 * 40], tail;
	SNAKE();
};