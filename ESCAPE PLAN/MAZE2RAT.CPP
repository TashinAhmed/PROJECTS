//a circular rat finds its path in the maze by itself.

#include<iostream.h>
#include<conio.h>
#include<stdlib.h>
#include<graphics.h>
#include<dos.h>

#define w 15

void maze(int,int);
void ratmove(int,int,int);
void win();
void title();

void main()
	{
		int gd=DETECT,gm,x,y;
		initgraph(&gd,&gm,"..\\bgi");
		title();

		x=90,y=50;
		maze(x,y);
		int s=3;
		ratmove(x,y,s);
		//program will end in ratmove() function
	}





void maze(int x,int y)
	{
		rectangle(0,0,639,479);
		circle(x,y,3);
		line(50,50,50,300);
		line(100,50,290,50);
		line(50,50,80,50);
		line(50,300,290,300);
		line(290,50,290,300);
		line(70,70,270,70);
		line(270,70,270,280);
		line(250,280,70,280);
		line(70,280,70,70);
		line(230,90,90,90);
		line(90,90,90,260);
		line(90,260,250,260);
		line(250,260,250,90);
		line(110,240,110,110);
		line(110,110,230,110);
		line(230,110,230,240);
		line(230,240,130,240);
		line(190,220,130,220);
		line(130,220,130,130);
		line(130,130,210,130);
		line(210,130,210,220);
		line(150,200,150,150);
		line(150,150,190,150);
		line(190,150,190,200);
		line(190,200,170,200);
		//now cross lines
		line(50,300,70,280);
		line(70,70,90,90);
		line(130,130,150,150);
		line(210,130,230,110);
		line(230,240,250,260);

		setcolor(YELLOW);
		circle(170,180,10);
	}

void ratmove(int x,int y,int s)
	{
		setcolor(15);
		circle(x,y,3);
		if(x==160&&y==180)
			{

				delay(100);
				setcolor(0);
				circle(x,y,3);
				win();
			}
		delay(20);
		switch(s)
			{
				case 1:
					if(getpixel(x-10,y)!=w)
						{ setcolor(0);   circle(x,y,3);   ratmove(x-10,y,4);   }
					setcolor(15);   circle(x,y,3);   delay(20);
					if(getpixel(x,y-10)!=w)
						{ setcolor(0);   circle(x,y,3);   ratmove(x,y-10,1);   }
					setcolor(15);   circle(x,y,3);   delay(20);
					if(getpixel(x+10,y)!=w)
						{ setcolor(0);   circle(x,y,3);   ratmove(x+10,y,2);   }
					setcolor(15);   circle(x,y,3);   delay(20);
					setcolor(0);   circle(x,y,3);
					break;

				case 2:
					if(getpixel(x,y-10)!=w)
						{ setcolor(0);   circle(x,y,3);   ratmove(x,y-10,1);   }
					setcolor(15);   circle(x,y,3);   delay(20);
					if(getpixel(x+10,y)!=w)
						{ setcolor(0);   circle(x,y,3);   ratmove(x+10,y,2);   }
					setcolor(15);   circle(x,y,3);   delay(20);
					if(getpixel(x,y+10)!=w)
						{ setcolor(0);   circle(x,y,3);   ratmove(x,y+10,3);   }
					setcolor(15);   circle(x,y,3);   delay(20);
					setcolor(0);   circle(x,y,3);
					break;

				case 3:
					if(getpixel(x+10,y)!=w)
						{ setcolor(0);   circle(x,y,3);   ratmove(x+10,y,2);   }
					setcolor(15);   circle(x,y,3);   delay(20);
					if(getpixel(x,y+10)!=w)
						{ setcolor(0);   circle(x,y,3);   ratmove(x,y+10,3);   }
					setcolor(15);   circle(x,y,3);   delay(20);
					if(getpixel(x-10,y)!=w)
						{ setcolor(0);   circle(x,y,3);   ratmove(x-10,y,4);   }
					setcolor(15);   circle(x,y,3);   delay(20);
					setcolor(0);   circle(x,y,3);
					break;

				case 4:
					if(getpixel(x,y+10)!=w)
						{ setcolor(0);   circle(x,y,3);   ratmove(x,y+10,3);   }
					setcolor(15);   circle(x,y,3);   delay(20);
					if(getpixel(x-10,y)!=w)
						{ setcolor(0);   circle(x,y,3);   ratmove(x-10,y,4);   }
					setcolor(15);   circle(x,y,3);   delay(20);
					if(getpixel(x,y-10)!=w)
						{ setcolor(0);   circle(x,y,3);   ratmove(x,y-10,1);   }
					setcolor(15);   circle(x,y,3);   delay(20);
					setcolor(0);   circle(x,y,3);
					break;
			}
	}


void win()
	{
		setcolor(15);
		circle(170,180,3);
		settextstyle(3,0,6);
		setcolor(YELLOW);
		outtextxy(0,160,"Yupiee...rat cracked the");
		settextstyle(3,0,8);
		outtextxy(300,210," Big Oh!!!!!!!!");
		delay(1000);
		settextstyle(1,0,1);
		outtextxy(100,450,"Press any key to exit.");
		getch();
		exit(0);
	}

void title()
	{
		setcolor(11);
		settextstyle(3,0,7);
		for(int i=479;i>=0;i-=5)
			{
				cleardevice();
				outtextxy(i,100,"Circular Rat's Maze");
				delay(10);
			}

		delay(500);
		cleardevice();
		delay(500);
		outtextxy(0,100,"Circular Rat's Maze");
		delay(500);
		cleardevice();
		delay(500);
		outtextxy(0,100,"Circular Rat's Maze");
		delay(500);
		cleardevice();
		outtextxy(0,100,"Circular Rat's Maze");

		settextstyle(4,0,9);
		delay(1000);
		cleardevice();
		outtextxy(300,200,"3");
		delay(1000);
		cleardevice();
		outtextxy(300,200,"2");
		delay(1000);
		cleardevice();
		outtextxy(300,200,"1");
		delay(1000);
		cleardevice();
		setcolor(15);
	}