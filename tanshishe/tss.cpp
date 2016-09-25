#include"graphics.h"
#include"SNAKE.h"
#include"PLAYER.h"
#include<dos.h>
#include<iostream>
#include<algorithm>
#include<string>
#include<fstream>

#define LOW_SPEED 10
#define MID_SPEED 30
#define HIGH_SPEED 60
using namespace std;

const int g_BAR_X = 27, g_BAR_Y = 27, N = 2000;
int mission;
fstream score;
SNAKE snake;

void GameOver(int, PLAYER& player);
int scores(SNAKE& snake, PLAYER& player, int a);
void food(int& x, int& y);
void runningSnake(int, PLAYER& player); 
void Menu(PLAYER&);
void login(PLAYER& player);
void stage1(PLAYER& player);
void stage2(PLAYER& player);
void ranklist(PLAYER& player);
inline bool button(int x, int y, int left, int up, int right, int down, PIMAGE after = 0);
inline bool cir_button(int x, int y, int left, int up, int radius, PIMAGE after = 0);

int main(){
	setinitmode(INIT_NOBORDER);
	initgraph(1386, 1026);
	PLAYER player;
	PIMAGE menu = newimage();
	getimage(menu, "D:/snake/menu.png");
	putimage(0, 0, menu);
	//runningSnake(1, player);
	login(player);
	randomize();
	Menu(player);
	getch();
}
int scores(SNAKE& snake, PLAYER& player, int a){
	PLAYER rank[11], cp_rank[11];
	score.open("D:/snake/scores.txt", ios::in);
	int i = 0;
	int temp[11];

	//初始化数组
	for (int j = 0; j < 10; j++){
		temp[j] = 0;
	}
	
	//读取文件中的用户名，分数
	for (; !score.eof() && i < 10; i++){
		string name;
		score >> name;
		if (name == "") break;
		rank[i].playername.swap(name);
		score >> rank[i].score;
		temp[i] = rank[i].score;
	}

	//将当前用户信息和数组合并
	rank[i].playername = player.playername;
	temp[i] = rank[i].score = player.score;

	//排序
	sort(temp, temp + i + 1);

	//搜索匹配用户名和分数
	for (int j = i; j >= 0; j--){
		for (int k = 0; k <= i; k++){
			if (temp[j] == rank[k].score && rank[k].score != -1) {
				cp_rank[j] = rank[k];
				rank[k].score = -1;
				break;
			}
		}
	}
	int ret = i;
	
	//信息输出到文件
	if (a == 1){
		score.close();
		score.open("D:/snake/scores.txt", ios::out);
		for (; i >= 0; i--){
			rank[i] = cp_rank[i];
			score << rank[i].playername.data() << ' ' << rank[i].score << endl;
		}
	}

	//信息输出到屏幕
	else {
		int j = 0;
		for (; i >= 0; i--){
			rank[i] = cp_rank[i];
			setfont(30, 20, "幼圆");
			xyprintf(500, 150 + j * 40, "%s", rank[i].playername.data());
			xyprintf(850, 150 + j * 40, "%d", rank[i].score);
			j++;
		}	
	}
	score.close();
	return ret;
}	

//取随机数并定位至方格左上角
void food(int &x, int &y){
	int rda, rdb;
	x = 27 + rda - (rda = random(getwidth() - 360 - 54)) % g_BAR_X;
	y = 27 + rdb - (rdb = random(getheight() - 54)) % g_BAR_Y;
}

//获取用户名，储存
void login(PLAYER& player){	
	bool isAt = false;
	char buf[32];
	inputbox_getline("","ENTER YOUR NAME",buf , sizeof(buf));
	player.playername = buf;
}

//蛇运动主要函数
void runningSnake(int m, PLAYER& player){
	color_t bkcolor = BLACK;
	setbkcolor(bkcolor);
	char op_dir = 'a', dir = 'd';
	snake.head = Point(27, 27);
	Point fd_point(-1, -1), br_point(-1, -1), pmenubutton(1139, 447);
	bool judge = true;
	bool isBarrier = true;
	Point barrier[N];
	int j = -1;
	int speed_ = 60;
	int addition = 0;
	int n;
	snake.Nnode = 0;
	PIMAGE bianlan = newimage(), stage2 = newimage(), stage3 = newimage(), stage4 = newimage(),bkground=newimage();
	PIMAGE menubutton = newimage(), menu_down = newimage(), stage1 = newimage(); 
	PIMAGE fruit = newimage(),  body = newimage(), head = newimage();
	PIMAGE W = newimage(), S = newimage(), A = newimage(), D = newimage(), slow = newimage();
	PIMAGE smile = newimage(), stone = newimage(), heng = newimage(), shu = newimage(), doublescores = newimage(), speed = newimage();
	getimage(bianlan, "D:/snake/bianlan.png");
	getimage(stage1, "D:/snake/stage1.png");
	getimage(bkground, "D:/snake/bkground.png");
	getimage(stage2, "D:/snake/stage2.png");
	getimage(stage3, "D:/snake/stage3.png");
	getimage(stage4, "D:/snake/stage4.png");
	getimage(fruit, "D:/snake/fruit.png");
	getimage(slow, "D:/snake/slow.png");
	getimage(speed, "D:/snake/speed.png");
	getimage(doublescores,"D:/snake/2dragonball.png");
	getimage(body, "D:/snake/body.png");
	getimage(W, "D:/snake/shang.png");
	getimage(A, "D:/snake/zuo.png");
	getimage(S, "D:/snake/xia.png");
	getimage(D, "D:/snake/you.png");
	getimage(stone, "D:/snake/barrier.png");
	putimage(0, 0, bkground);
	putimage(1026, 0, bianlan);
	
	//播放ready go
	PlaySound(TEXT("D:/snake/readygo.wav"), NULL, SND_FILENAME | SND_ASYNC); delay_ms(1800);

	while (true){
		int x, y;
		mouse_msg mouse;
		mousepos(&x, &y);
		putimage(1026, 1026, bianlan);
		if (cir_button(x, y, pmenubutton.x, pmenubutton.y, 27, menu_down)){
			mouse = getmouse();
			if (mouse.is_down())
				Menu(player);
		}
		else putimage_transparent(0, menubutton, pmenubutton.x, pmenubutton.y, 0);
		//int a = 0;

		//获取键盘，判断方向
		char key;
		if (kbhit()){
			key = getch() | 0x20;
			if (key != op_dir){
				if (key == 's'){
					dir = 's';
					op_dir = 'w';
				}
				else if (key == 'w'){
					dir = 'w';
					op_dir = 's';
				}
				else if (key == 'a'){
					dir = 'a';
					op_dir = 'd';
				}
				else if (key == 'd'){
					dir = 'd';
					op_dir = 'a';
				}
			}
		}
		
		//依次给下一节身体赋值
			int i = 0;
			snake.body[0] = snake.head;
			for (i = snake.Nnode; i >= 0; i--){
				snake.body[i + 1] = snake.body[i];
			}
			
			//移动身体，改变头方向的贴图
			switch (dir){
			case 'w':{
						 snake.head.y -= g_BAR_Y; 
						 head = W; 
			}break;
			case 's':{
						 snake.head.y += g_BAR_Y; 
						 head = S; 
			}break;
			case 'a':{
						 snake.head.x -= g_BAR_X; 
						 head = A; 
			}break;
			case 'd':{
						 snake.head.x += g_BAR_X; 
						 head = D; 
			}break;
			}
			
			//食物出现*******************
			if (m == 1 || m == 2){
				if (judge){
					food(fd_point.x, fd_point.y);
					for (i = snake.Nnode + 1; i >= 0; i--){
						while (snake.body[i] == fd_point){
							food(fd_point.x, fd_point.y);
							i = snake.Nnode + 1;
						}
					}

					if (!((snake.Nnode + 1) % 5)){

						n = random(3);
						if (n == 0){
							putimage_transparent(0, doublescores, fd_point.x, fd_point.y,0);

						}
						else if (n == 1){
							putimage_transparent(0, speed, fd_point.x, fd_point.y,0);

						}
						else if (n == 2){
							putimage_transparent(0, slow, fd_point.x, fd_point.y,0);

						}
					}
					else  putimage_transparent(0, fruit, fd_point.x, fd_point.y,0);
					


					//*****************************

					if (mission == 1){
						while (1){
							if ((fd_point.x >= 378 && fd_point.x < 405 && fd_point.y >= 378 && fd_point.y < 405) ||
								(snake.head.x >= 621 && snake.head.x < 648 && snake.head.y >= 378 && snake.head.y < 648)){
								food(fd_point.x, fd_point.y);
							}
							else break;
						}
					}

					if (mission == 2){
						while (1){
							if ((fd_point.x >= 162 && fd_point.x < 432 && fd_point.y >= 162 && fd_point.y < 189) ||
								(fd_point.x >= 162 && fd_point.x < 189 && fd_point.y >= 162 && fd_point.y < 432) ||
								(fd_point.x >= 162 && fd_point.x < 189 && fd_point.y >= 594 && fd_point.y < 864) ||
								(fd_point.x >= 162 && fd_point.x < 432 && fd_point.y >= 837 && fd_point.y < 864) ||
								(fd_point.x >= 837 && fd_point.x < 864 && fd_point.y >= 594 && fd_point.y < 864) ||
								(fd_point.x >= 594 && fd_point.x < 864 && fd_point.y >= 162 && fd_point.y < 189) ||
								(fd_point.x >= 594 && fd_point.x < 864 && fd_point.y >= 837 && fd_point.y < 864) ||
								(fd_point.x >= 837 && fd_point.x < 864 && fd_point.y >= 162 && fd_point.y < 432)){
								food(fd_point.x, fd_point.y);
							}
							else break;
						}
					}
						if (mission == 3){
							while (1){
								if ((fd_point.x >= 27 && fd_point.x < 891 && fd_point.y >= 189 && fd_point.y < 216) ||
									(fd_point.x >= 27 && fd_point.x < 891 && fd_point.y >= 513 && fd_point.y < 540) ||
									(fd_point.x >= 27 && fd_point.x < 891 && fd_point.y >= 837 && fd_point.y < 864) ||
									(fd_point.x >= 135 && fd_point.x < 972 && fd_point.y >= 351 && fd_point.y < 378) ||
									(fd_point.x >= 135 && fd_point.x < 972 && fd_point.y >= 675 && fd_point.y < 702)){

									food(fd_point.x, fd_point.y);

								}
								else break;
							}
					}
						if (mission == 4){
							while (1){
						if ((fd_point.x >= 378 && fd_point.x < 648 && fd_point.y >= 378 && fd_point.y < 648) ||
							(fd_point.x >= 27 && fd_point.x < 297 && fd_point.y >= 243 && fd_point.y < 270) ||
							(fd_point.x >= 27 && fd_point.x < 297 && fd_point.y >= 756 && fd_point.y < 783) ||
							(fd_point.x >= 405 && fd_point.x < 432 && fd_point.y >= 27 && fd_point.y < 297) ||
							(fd_point.x >= 405 && fd_point.x < 432 && fd_point.y >= 729 && fd_point.y < 999) ||
							(fd_point.x >= 729 && fd_point.x < 999 && fd_point.y >= 243 && fd_point.y < 270) ||
							(fd_point.x >= 729 && fd_point.x < 999 && fd_point.y >= 756 && fd_point.y < 783) ||
							(fd_point.x >= 594 && fd_point.x < 621 && fd_point.y >= 27 && fd_point.y < 297) ||
							(fd_point.x >= 594 && fd_point.x < 621 && fd_point.y >= 729 && fd_point.y < 999)){
							food(fd_point.x, fd_point.y);
						}
						else break;
						
							}
					}

					judge = false;
				};


				//putimage(0, 0, menu);

				if (mission == 1)
				    putimage(0, 0, stage1);
				if (mission == 2)
					putimage(0, 0, stage2);
				if (mission == 3)
					putimage(0, 0, stage3);
				if (mission == 4)
					putimage(0, 0, stage4);
				if (m == 2||m==3)
					putimage(0, 0, bkground);
				if (!(snake.head.x<27 || snake.head.x >= 999 || snake.head.y<27 || snake.head.y >= 999))
					putimage_transparent(0, head, snake.head.x, snake.head.y, 0);

				for (int i = 1; i <= snake.Nnode; i++){
					putimage_transparent(0, body, snake.body[i].x, snake.body[i].y, 0);
				}

				if (n == 0){
					putimage_transparent(0, doublescores, fd_point.x, fd_point.y, 0);

				}
				else if (n == 1){
					putimage_transparent(0, speed, fd_point.x, fd_point.y, 0);

				}
				else if (n == 2){
					putimage_transparent(0, slow, fd_point.x, fd_point.y, 0);

				}
			else  putimage_transparent(0, fruit, fd_point.x, fd_point.y, 0);


				if (fd_point == snake.head){
					judge = true;
					PlaySound(TEXT("D:/snake/score.wav"), NULL, SND_FILENAME | SND_ASYNC);
					if (n == 0){
						addition++;
						snake.Nnode++;
					}
					else if (n == 1){
						speed_ += 2;
					}
					else if (n == 2){
						if (speed_ > 5)speed_ -= 2;
					}
					else snake.Nnode++;
					n = -1;

				}
				//闯关模式*********************



				if (m == 1){
					//撞墙*************************
					if (snake.head.x<27 || snake.head.x>999 - g_BAR_X || snake.head.y<27 || snake.head.y>999 - g_BAR_Y){
						putimage_transparent(0, head, snake.head.x, snake.head.y, 0);
						PlaySound(TEXT("D:/snake/gameover.wav"), NULL, SND_FILENAME | SND_ASYNC);
						delay_ms(1000);
						GameOver(m, player);

					}
					//mission 1********************
					if (mission == 1){
						
						if ((snake.head.x >= 378 && snake.head.x < 405 && snake.head.y >= 378 && snake.head.y < 648) ||
							(snake.head.x >= 621 && snake.head.x < 648 && snake.head.y >= 378 && snake.head.y < 648)){
							putimage_transparent(0, head, snake.head.x, snake.head.y, 0);
							PlaySound(TEXT("D:/snake/gameover.wav"), NULL, SND_FILENAME | SND_ASYNC);
							delay_ms(1000);
							GameOver(m, player);
						}
						if (snake.Nnode == 30){
							putimage(0, 0, stage2);
							mission = 2;
							snake.Nnode = 0;
							dir = 'd';
							snake.head.x = 27;
							snake.head.y = 27;
						}
					}
					//mission 2*********************
					if (mission == 2){
					
						if ((snake.head.x >= 162 && snake.head.x < 432 && snake.head.y >= 162 && snake.head.y < 189) ||
							(snake.head.x >= 162 && snake.head.x < 189 && snake.head.y >= 162 && snake.head.y < 432) ||
							(snake.head.x >= 162 && snake.head.x < 189 && snake.head.y >= 594 && snake.head.y < 864) ||
							(snake.head.x >= 162 && snake.head.x < 432 && snake.head.y >= 837 && snake.head.y < 864) ||
							(snake.head.x >= 837 && snake.head.x < 864 && snake.head.y >= 594 && snake.head.y < 864) ||
							(snake.head.x >= 594 && snake.head.x < 864 && snake.head.y >= 837 && snake.head.y < 864) ||
							(snake.head.x >= 594 && snake.head.x < 864 && snake.head.y >= 162 && snake.head.y < 189) ||
							(snake.head.x >= 837 && snake.head.x < 864 && snake.head.y >= 162 && snake.head.y < 432)){
							putimage_transparent(0, head, snake.head.x, snake.head.y, 0);
							PlaySound(TEXT("D:/snake/ameover.wav"), NULL, SND_FILENAME | SND_ASYNC);
							delay_ms(1000);
							GameOver(m, player);
						}
						if (snake.Nnode == 30){
							putimage(0, 0, stage3);
							mission = 3;
							dir = 'd';
							snake.head.x = 27;
							snake.head.y = 27;
						}
					}
					//mission 3*********************
					if (mission == 3){
						
						if ((snake.head.x >= 27 && snake.head.x < 891 && snake.head.y >= 189 && snake.head.y < 216) ||
							(snake.head.x >= 27 && snake.head.x < 891 && snake.head.y >= 513 && snake.head.y < 540) ||
							(snake.head.x >= 27 && snake.head.x < 891 && snake.head.y >= 837 && snake.head.y < 864) ||
							(snake.head.x >= 135 && snake.head.x < 972 && snake.head.y >= 351 && snake.head.y < 378) ||
							(snake.head.x >= 135 && snake.head.x < 972 && snake.head.y >= 675 && snake.head.y < 702)){
							putimage_transparent(0, head, snake.head.x, snake.head.y, 0);
							PlaySound(TEXT("D:/snake/gameover.wav"), NULL, SND_FILENAME | SND_ASYNC);
							delay_ms(1000);
							GameOver(m, player);
						}


						if (snake.Nnode == 30){
							putimage(0, 0, stage4);
							mission = 4;
							dir = 'd';
							snake.head.x = 27;
							snake.head.y = 27;
						}
					}
					//mission 4***************
					if (mission == 4){
					
						if ((snake.head.x >= 378 && snake.head.x < 648 && snake.head.y >= 378 && snake.head.y < 648) ||
							(snake.head.x >= 27 && snake.head.x < 297 && snake.head.y >= 243 && snake.head.y < 270) ||
							(snake.head.x >= 27 && snake.head.x < 297 && snake.head.y >= 756 && snake.head.y < 783) ||
							(snake.head.x >= 405 && snake.head.x < 432 && snake.head.y >= 27 && snake.head.y < 297) ||
							(snake.head.x >= 405 && snake.head.x < 432 && snake.head.y >= 729 && snake.head.y < 999) ||
							(snake.head.x >= 729 && snake.head.x < 999 && snake.head.y >= 243 && snake.head.y < 270) ||
							(snake.head.x >= 729 && snake.head.x < 999 && snake.head.y >= 756 && snake.head.y < 783) ||
							(snake.head.x >= 594 && snake.head.x < 621 && snake.head.y >= 27 && snake.head.y < 297) ||
							(snake.head.x >= 594 && snake.head.x < 621 && snake.head.y >= 729 && snake.head.y < 999)){
							putimage_transparent(0, head, snake.head.x, snake.head.y, 0);
							PlaySound(TEXT("D:/snake/gameover.wav"), NULL, SND_FILENAME | SND_ASYNC);
							delay_ms(1000);
							GameOver(m, player);
						}
						if (snake.Nnode == 30){
							putimage(0, 0, stage1);
							mission = 1;
							dir = 'd';
							snake.head.x = 27;
							snake.head.y = 27;
						}
					}
				}
			}

			//根本停不下来*********
			if (m == 2){
				
				if (snake.head.x < 27){
					snake.head.x = 972;
					dir = 'a';
				}
				else if (snake.head.x > 972){
					snake.head.x = 27;
					dir = 'd';
				}
				else if (snake.head.y < 27){
					snake.head.y = 972;
					dir = 'w';
				}
				else if (snake.head.y > 972){
					snake.head.y = 27;
					dir = 's';
				}
				
				
			}


			//死亡空间*********************
			if (m == 3){
				snake.Nnode = 10;
				putimage(0, 0, bkground);
				if (!(snake.head.x<27 || snake.head.x >= 999 || snake.head.y<27 || snake.head.y >= 999))
					putimage_transparent(0, head, snake.head.x, snake.head.y, 0);
				
				for (int i = 1; i <= snake.Nnode; i++){
					putimage_transparent(0, body, snake.body[i].x, snake.body[i].y, 0);
				}
				for (int d = 0; d <= 2000; d++){
					putimage_transparent(0, stone, barrier[d].x, barrier[d].y, 0);
				}
				
				
				//撞墙*************************
				if (snake.head.x<27 || snake.head.x>999 - g_BAR_X || snake.head.y<27 || snake.head.y>999 - g_BAR_Y){
					PlaySound(TEXT("D:/snake/gameover.wav"), NULL, SND_FILENAME | SND_ASYNC);
					delay_ms(1000);
					GameOver(m, player);

				}
				snake.Nnode = 10;
				if (isBarrier){
					food(br_point.x, br_point.y);
					j++;
					for (i = snake.Nnode + 1; i > 0; i--){
						while (snake.body[i] == br_point){
							food(br_point.x, br_point.y);
						}
					}
					
					barrier[j] = br_point;
					isBarrier = false;

				}
				for (int a = 0; a < N; a++){
					if (barrier[a] == snake.head){
						PlaySound(TEXT("D:/snake/gameover.wav"), NULL, SND_FILENAME | SND_ASYNC);
						delay_ms(1000);
						GameOver(m, player);
					}
				}
			}




			//游戏结束***************
			for (i = snake.Nnode; i > 0; i--){
				if (snake.body[i] == snake.head){
					PlaySound(TEXT("D:/snake/gameover.wav"), NULL, SND_FILENAME | SND_ASYNC);
					delay_ms(1000);
					GameOver(m, player);
				}
			}
			player.score = (snake.Nnode + addition) * 10 + j + 1;
			setfont(50, 0, "幼圆");
			xyprintf(1026, 0, "Score:%d", player.score);



			//闯关模式***********
			if (m == 1){ delay_fps(speed_);
			delay_fps(speed_);
			delay_fps(speed_);
			delay_fps(speed_);
			delay_fps(speed_);
			delay_fps(speed_);
			}

			//生存模式--末日逃亡模式****
			if (m == 2) {
				delay_fps(snake.Nnode/2 + 10);
			}
			//生存模式--死亡空间模式****
			if (m == 3){ delay_fps(10);	isBarrier = true; }

			


	}
}

void GameOver(int n, PLAYER& player){
	scores(snake, player, 1);
	const int lx = 245;
	cleardevice();
	PIMAGE gameover = newimage(), menu_down = newimage(), menubutton = newimage(), exitbutton = newimage(), exit_down = newimage();
	getimage(gameover, "D:/snake/gameover.png");
	getimage(menu_down, "D:/snake/menu_down.png");
	getimage(menubutton, "D:/snake/menubutton.png");
	getimage(exitbutton, "D:/snake/exitbutton.png");
	getimage(exit_down, "D:/snake/exit_down.png");
	putimage(0, 0, gameover);
	putimage_transparent(0, exitbutton,450, 550, 0);
	mouse_msg mouse;

	while (true){
		int x, y;
		mousepos(&x, &y);
		mouse = getmouse();

		//获取鼠标，主页按钮
		if (sqrt(double((x - 377)*(x - 377) + (y - 577)*(y - 577))) <= 27){
			putimage_transparent(0, menu_down, 350, 550, 0);
			if (mouse.is_down()){
				Menu(player);
			}
		}

		//获取鼠标，退出按钮
		else if (sqrt(double((x - 477)*(x - 477) + (y - 577)*(y - 577))) <= 27){
			putimage_transparent(0, exit_down, 450, 550, 0);
			if (mouse.is_down()){
				exit(0);
			}
		}
		else{
			putimage_transparent(0, menubutton, 350, 550, 0);
			putimage_transparent(0, exitbutton, 450, 550, 0);
		}

	}
}


void Menu(PLAYER& player){
	PIMAGE menu = newimage(), helpbutton = newimage(), infobutton = newimage(), menubutton = newimage(), menu_down = newimage(), help_down = newimage(), info_down = newimage(), exitbutton = newimage(), exit_down = newimage();
	PIMAGE challenge = newimage(), challenge_down = newimage(), choosestage_down = newimage(), list = newimage(), list_down = newimage(), choosestage = newimage();
	PIMAGE help = newimage(), info = newimage();
	getimage(menu, "D:/snake/menu.png");
	getimage(helpbutton, "D:/snake/helpbutton.png");
	getimage(menubutton, "D:/snake/menubutton.png");
	getimage(infobutton, "D:/snake/infobutton.png");
	getimage(challenge, "D:/snake/challenge.png");
	getimage(challenge_down, "D:/snake/challenge_down.png");
	getimage(choosestage, "D:/snake/choosestage.png");
	getimage(choosestage_down, "D:/snake/choosestage_down.png");
	getimage(list, "D:/snake/list.png");
	getimage(list_down, "D:/snake/list_down.png");
	getimage(help_down, "D:/snake/help_down.png");
	getimage(menu_down, "D:/snake/menu_down.png");
	getimage(info_down, "D:/snake/info_down.png");
	getimage(exitbutton, "D:/snake/exitbutton.png");
	getimage(exit_down, "D:/snake/exit_down.png");
	getimage(help, "D:/snake/help.jpg");
	getimage(info, "D:/snake/info.png");
	
	Point pinfobutton(1019, 254), phelpbutton(1015, 354), pchoosestage(223, 595), plist(536, 770), pchallenge(869, 647), pmenubutton(1316, 15), pexitbutton(1304, 947);
	putimage(0, 0, menu);
	putimage_transparent(0, infobutton, pinfobutton.x, pinfobutton.y, 0);
	//putimage_transparent(0, infobutton, 1070, 368, 0);
	putimage_transparent(0, helpbutton, phelpbutton.x, phelpbutton.y, 0);
	putimage_transparent(0, choosestage, pchoosestage.x, pchoosestage.y, 0);
	putimage_transparent(0, list, plist.x, plist.y, 0);
	putimage_transparent(0, challenge, pchallenge.x, pchallenge.y, 0);
	putimage_transparent(0, exitbutton, pexitbutton.x, pexitbutton.y, 0);

	while (true){
		mouse_msg mouse;
		int x, y;
		mouse = getmouse();
		mousepos(&x, &y);

		//获取鼠标，进入关卡模式选关
		if (button(x, y, 223, 595, 519, 740, choosestage_down)){
			if (mouse.is_down()){
				stage1(player);
			}
		}

		//获取鼠标，进入生存模式选关
		else if (button(x, y, 869, 647, 1167, 791, challenge_down)){
			if (mouse.is_down()){
				stage2(player);
			}
		}

		//获取鼠标，进入排行榜
		else if (button(x, y, 536, 770, 835, 914, list_down)){
			if (mouse.is_down()){
				ranklist(player);
			}		
		}

		//获取鼠标，查看帮助
		else if (cir_button(x, y, phelpbutton.x, phelpbutton.y, 27, help_down)){
			if (mouse.is_down()){
				putimage(0, 0, help);
				putimage_transparent(0, menubutton, pmenubutton.x, pmenubutton.y, 0);
				while (true){
					mouse = getmouse();
					mousepos(&x, &y);
					if (cir_button(x, y, pmenubutton.x, pmenubutton.y, 27, menu_down)){
						if (mouse.is_down()) 
							break;
					}
					else putimage_transparent(0, menubutton, pmenubutton.x, pmenubutton.y, 0);
				}
			}
		}

		//获取鼠标，查看制作成员信息
		else if (cir_button(x, y, pinfobutton.x, pinfobutton.y, 27, info_down)){
			if (mouse.is_down()){
				putimage(0, 0, info);
				putimage_transparent(0, menubutton, pmenubutton.x, pmenubutton.y, 0);
				while (true){
					mouse = getmouse();
					mousepos(&x, &y);
					if (cir_button(x, y, pmenubutton.x, pmenubutton.y, 27, menu_down)){
						if (mouse.is_down())
							break;
					}
					else putimage_transparent(0, menubutton, pmenubutton.x, pmenubutton.y, 0);
				}
			}
		}

		//获取鼠标，退出游戏
		else if (cir_button(x, y, pexitbutton.x, pexitbutton.y, 27, exit_down)){
			if (mouse.is_down()){
				exit(0);
			}
		}
		else {
			putimage(0, 0, menu);
			putimage_transparent(0, infobutton, pinfobutton.x, pinfobutton.y, 0);
			//putimage_transparent(0, infobutton, 1070, 368, 0);
			putimage_transparent(0, helpbutton, phelpbutton.x, phelpbutton.y, 0);
			putimage_transparent(0, choosestage, pchoosestage.x, pchoosestage.y, 0);
			putimage_transparent(0, list, plist.x, plist.y, 0);
			putimage_transparent(0, challenge, pchallenge.x, pchallenge.y, 0);
			putimage_transparent(0, exitbutton, pexitbutton.x, pexitbutton.y, 0);
		}
	}
}

//关卡模式选关
void stage1(PLAYER& player){
	int x, y;
	PIMAGE mission1 = newimage(), mission2 = newimage(), mission3 = newimage(), mission4 = newimage(), bkg = newimage(),
		pre_stage1 = newimage(), pre_stage2 = newimage(), pre_stage3 = newimage(), pre_stage4 = newimage(), decision = newimage();
	getimage(bkg, "D:/snake/bkg.png");
	putimage(0, 0, bkg);
	getimage(mission1, "D:/snake/mission1.png");
	getimage(mission2, "D:/snake/mission2.png");
	getimage(mission3, "D:/snake/mission3.png");
	getimage(mission4, "D:/snake/mission4.png");
	getimage(pre_stage1, "D:/snake/pre-stage1.png");
	getimage(pre_stage2, "D:/snake/pre-stage2.png");
	getimage(pre_stage3, "D:/snake/pre-stage3.png");
	getimage(pre_stage4, "D:/snake/pre-stage4.png");
	putimage_transparent(0, mission1, 100, 150, 0);
	putimage_transparent(0, mission2, 100, 350, 0);
	putimage_transparent(0, mission3, 100, 550, 0);
	putimage_transparent(0, mission4, 100, 750, 0);
	mouse_msg mouse;

	while(true){
		mouse = getmouse();
		mousepos(&x, &y);
		if (button(x, y, 100, 200, 300, 300)){
			putimage(500, 250, pre_stage1);
			if (mouse.is_down()){
				mission = 1;
				break;
			}
		}
		else if (button(x, y, 100, 400, 300, 500)){
			putimage(500, 250, pre_stage2);
			if (mouse.is_down()){
				mission = 2;
				break;
			}
		}
		else if (button(x, y, 100, 600, 300, 700)){
			putimage(500, 250, pre_stage3);
			if (mouse.is_down()){
				mission = 3;
				break;
			}
		}
		else if (button(x, y, 100, 800, 300, 900)){
			putimage(500, 250, pre_stage4);
			if (mouse.is_down()){
				mission = 4;
				break;
			}
		}
	}
	runningSnake(1, player);
}

//生存模式选关
void stage2(PLAYER& player){
	int x, y;
	mouse_msg mouse;
	PIMAGE mission5 = newimage(), mission6 = newimage(), bkg = newimage();
	getimage(mission5, "D:/snake/mission5.png");
	getimage(mission6, "D:/snake/mission6.png");
	getimage(bkg, "D:/snake/bkg.png");
	putimage(0, 0, bkg);
	putimage_transparent(0, mission5, 200, 300, 0);
	putimage_transparent(0, mission6, 200, 600, 0);
	while (true){
		mouse = getmouse();
		mousepos(&x, &y);
		if (button(x, y, 200, 350, 450, 450)){
			if (mouse.is_down())
				runningSnake(2, player);
		}
		else if (button(x, y, 200, 650, 450, 750)){
			if (mouse.is_down())
				runningSnake(3, player);
		}
	}
}

//排行榜
void ranklist(PLAYER& player){
	Point pmenubutton(1316, 15);
	PIMAGE rank_list = newimage(), menubutton = newimage(), menu_down = newimage();
	getimage(rank_list, "D:/snake/ranklist.png");
	getimage(menubutton, "D:/snake/menubutton.png");
	getimage(menu_down, "D:/snake/menu_down.png");
	putimage(0, 0, rank_list);
	putimage_transparent(0, menubutton, pmenubutton.x, pmenubutton.y, 0);
	scores(snake, player, 0);
	mouse_msg mouse;
	int x, y;
	while (true){
		mouse = getmouse();
		mousepos(&x, &y);
		if (cir_button(x, y, pmenubutton.x, pmenubutton.y, 27, menu_down)){
			if (mouse.is_down())
				break;
		}
		else putimage_transparent(0, menubutton, pmenubutton.x, pmenubutton.y, 0);
	}
}

//判断鼠标（矩形框），放置图片
inline bool button(int x, int y, int left, int up, int right, int down, PIMAGE after){
	if (x >= left && x <= right && y >= up && y <= down){
		if (after) putimage_transparent(0, after, left, up, 0);
		return true;
	}
	else return false;
}

//判断鼠标（圆形框），放置图片
inline bool cir_button(int x, int y, int left, int up, int radius, PIMAGE after){
	if ((x - left - radius) * (x - left - radius) + (y - up - radius) * (y - up - radius) <= radius * radius){
		if (after) putimage_transparent(0, after, left, up, 0);
		return true;
	}
	else return false;
}