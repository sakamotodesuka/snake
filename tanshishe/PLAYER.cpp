#include"PLAYER.h"

int PLAYER::nplayer = 0;

PLAYER::PLAYER(){
	playername = "ZHUDAKANG";
	score = 0;
	nplayer++;
}

PLAYER::PLAYER(const string& st){
	playername = st;
	score = 0;
	nplayer++;
}
