#define ROW 100
#define COLLUM 128
#define TRUE 1
#define FALSE 0
#define MAXEF1 20   //maximum numbers of enemy fighters of type 1


#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<dos.h>
#include<stdlib.h>


//enum BOOLEAN {TRUE,FALSE};
//BOOLEAN  bcheck[ROW][COLLUM],mcheck[ROW][COLLUM];


typedef struct object{
		int x1;
		int y1;
		int x2;
		int y2;
		int speed;
		int destroy;
		void  *storecraftimage;
		}aircraft;

 aircraft  c={280,400,360,460,0,1};
 aircraft ef1={300,50,350,90,0,1};

 aircraft bullet1;
 aircraft missile1;
union REGS in,out;



void load_mouse_driver();
void unload_mouse_driver();
void menu();
void menugraphics();
void highlightremover();



void newGame();
int keycheck() ;
int craft();

int bullet();
int missile();

int arraychecker();

void bulletcheck(int i,int j);
void missilecheck(int i,int j);

int  craftmovement();
int clash(aircraft ob,int x,int y);
int enemyFighters1();
aircraft  enemyFighters1movement(aircraft ef);
void boom(int x,int y);
void craftimage();

void loadallimages();



char key;
int brk=1;
int a[ROW][COLLUM];


void newGame()
{

    loadallimages();
  craft();
  enemyFighters1();
  while(brk==1)
  {
	  if(kbhit())
	  keycheck();

	   delay(5);
	   enemyFighters1movement( ef1);
	   arraychecker();

  }
  exit(1);

}    //newGame ends

int craft()
{

  craftimage();
  c.storecraftimage=malloc (imagesize(c.x1,c.y1,c.x2,c.y2));//memory allocation
  getimage(c.x1,c.y1,c.x2,c.y2,c.storecraftimage);//save image of craft


}//craft ends


int enemyFighters1()//creats the enemy fighter
{
   setfillstyle(1,13);
   bar(ef1.x1,ef1.y1,ef1.x2,ef1.y2);
   ef1.storecraftimage=malloc(imagesize(ef1.x1,ef1.y1,ef1.x2,ef1.y2));
   getimage(ef1.x1,ef1.y1,ef1.x2,ef1.y2,ef1.storecraftimage);

} // enemyfighters  ends



aircraft  enemyFighters1movement(aircraft ef) // moves enemy plane
{    static short int flag=1;

  if(ef.destroy==1)       //destroy value 1 means it's not destroyed 0 means the object is destroyed
  {
       putimage(ef.x1,ef.y1,ef.storecraftimage,XOR_PUT);  //deleting previous position

      if(flag==1)
	 {
	  ef.x1-=1;
	  ef.x2-=1;
	  ef.y1+=1;
	  ef.y2+=1;
		 if(ef.y2==200)
		 flag=2;
	 }
  else if(flag==2)
     {

	  ef.x1+=1;
	  ef.x2+=1;
	  ef.y1-=1;
	  ef.y2-=1;

      if(ef.y2==50)
	 flag=1;
     }
     putimage(ef.x1,ef.y1,ef.storecraftimage,XOR_PUT);
ef1 =ef;  //assigning change values to the main structure
   }

}  //enemymove ends








int bullet() //current bullet
{                                  //10 pixels=1pixel
   static short int  flag=1; //to prevent unnecessary image save code

//creating the image of bullet1
setfillstyle(1,RED);
bar(c.x1+39,c.y1-10,c.x1+41,c.y1-5);
//we dont need to save image everytime this function is called.
//so we use a static flag that will be initialized once and save the image only once when the program runs

     if(flag==1)
     {           //this block will be excecuted only once
    bullet1.storecraftimage=malloc(imagesize(c.x1+39,c.y1-10,c.x1+41,c.y1-5));
    getimage(c.x1+39,c.y1-10,c.x1+41,c.y1-5,bullet1.storecraftimage);
      flag=0;
      }
  a[(c.y1/10)-1][c.x1/10]=1; //assign 1 in the array's in crafts firing position

}  //bullet ends



int arraychecker()
{      int i,j;
     for(i=0;i<ROW;i++)
   for(j=0;j<COLLUM;j++)
    if(a[i][j]==1)
    bulletcheck(i,j);
    else if(a[i][j]==2)
    missilecheck(i,j);
}
void bulletcheck(int i ,int j)
{


     short int hit=0;      //j*10=c.x1, i*10=c.y1
   //deleting previous position of the bullet
setfillstyle(1,BLACK);
   bar((j*10)+39,(i*10),(j*10)+41,(i*10)+5);

   hit= clash(ef1,j*10+40,i*10);
if(hit!=1)
 {

//place the bullet image 10 pixel upwords
	 putimage((j*10)+39,(i*10)-10,bullet1.storecraftimage,XOR_PUT);
	a[i-1][j]=1 ;
 } //if hit ends
	a[i][j]=0;//neutralize the current position
  //assign bullet or 1 to the 10 pixel upward position i-1 represent 10 pixel=1
}//bulletcheck end



int missile()
{
   static short int  flag=1;

setfillstyle(1,5);
bar(c.x1+39,c.y1-30,c.x1+50,c.y1-5);

     if(flag==1)
     {
    missile1.storecraftimage=malloc(imagesize(c.x1+39,c.y1-30,c.x1+50,c.y1-5));
    getimage(c.x1+39,c.y1-30,c.x1+50,c.y1-5,missile1.storecraftimage);
      flag=0;
      }
  a[(c.y1/10)-1][c.x1/10]=2;

}


void missilecheck(int i,int j)
{


     short int hit=0;
  setfillstyle(1,BLACK);
   bar((j*10)+39,(i*10)-20,(j*10)+50,(i*10)+5);

   hit= clash(ef1,j*10,i*10);
if(hit!=2)
{


    putimage((j*10)+39,(i*10)-30,missile1.storecraftimage,XOR_PUT);
a[i-1][j]=2 ;
 }
a[i][j]=0;
}

int clash(aircraft ob,int x,int y)
{ //so we chek if this point finds an enemy craft if it does then a[i][j]=0 so there will be no bullet instead there will be a booom



  if(x>=ob.x1 && x<=ob.x2 && y<=ob.y2 && y>ob.y1)
  {
 boom(x,y);
  ob.destroy=0;
  ef1=ob;
   return 1;
   }
 else return 0;
}//clash ends



void  boom(int x,int y)
{

 static short int tim=1,radius=10; //working


setcolor(RED);
setfillstyle(1,RED);
pieslice(x,y-20,0,360,radius);



 a[y/10][(x-40)/10]=0;

}


int  craftmovement()
{

      putimage(c.x1,c.y1,c.storecraftimage,XOR_PUT);   //deleting current position


   if (key==77)     //move right
   {
      c.x1+=10;
      c.x2+=10;

    }
   if(key==75)  //move left
   {
      c.x1-=10;
      c.x2-=10;
    }
   if(key==72)
    {
      c.y1-=10;
      c.y2-=10;
    }
  if(key==80)
   {
      c.y1+=10;
      c.y2+=10;
   }
   putimage(c.x1,c.y1,c.storecraftimage,XOR_PUT); //new craft position
} //craftmovements end



int keycheck()
{
 key=getch();

 //checking which key is pressed
 switch(key)
	 {
	    case 77:  if(c.x2<630)
		     craftmovement();
		     break;

	    case 75:  if(c.x1>0)
		     craftmovement();
		     break;

	    case 80: if(c.y2<480)
		     craftmovement();
		     break;

	    case 72: if(c.y1>0)
		     craftmovement();
		     break;

	    case 27:  brk=0;
		     break;

	    case 'b': bullet();
		     break;

	    case 'm' :missile();
		     break;

	   default:
		     break;
	 }//switch ends
}  //keycheck ends





void load_mouse_driver()
{
	out.x.cx=0;

	out.x.bx=0;

	in.x.ax=1;

	int86(0x33,&in,&out);

	in.x.ax=7;

	in.x.cx=0;

	in.x.dx=639;

	int86(0x33,&in,&out);

	in.x.ax=8;

	in.x.cx=0;

	in.x.dx=479;

	int86(0x33,&in,&out);
}








void unload_mouse_driver()

{

	in.x.ax=2;

	int86(0x33,&in,&out);
}






void menugraphics()
{

	settextstyle(4,0,0);
	setcolor(12);
	outtextxy(220,80,"NEW GAME");
	setcolor(13);
	outtextxy(210,140,"LOAD GAME");
	setcolor(14);
	outtextxy(225,200,"SETTINGS");
	setcolor(20);
	outtextxy(240,260,"ABOUT");
	setcolor(17);
	outtextxy(250,320,"HELP");
	setcolor(18);
	outtextxy(250,380,"EXIT");
}



void highlightremover()
{
	 setcolor(0);

	      rectangle(80,70,560,130);
	      rectangle(80,130,560,190);
	      rectangle(80,190,560,250);
	      rectangle(80,250,560,310);
	      rectangle(80,310,560,370);
	      rectangle(80,370,560,430);


}

 void craftimage()
{
	int left[14]={332,460,308,460,315,457,313,455,280,456,284,448,305,440};
      int right[12]={332,460,325,457,327,455,360,456,356,448,335,440};
    setcolor(11);

   // right
    drawpoly(6,right);
      line(332,425,325,418);
     line(332,425,335,450);
     line(335,450,320,460);
    //left
     drawpoly(7,left);
   line(320,460,305,450);
    line(305,450,308,425);
   line(308,425,315,418);
    ellipse(320,418,0,360,5,12);

    line(318,406,320,400);
    line(320,400,322,406);

}

void loadallimages()
{



}



void menu()
 {
short  int flag=0;
    load_mouse_driver();
	menugraphics();

	while(1)

			{

int x,y,cl,a,flag;

in.x.ax=3;

int86(0x33,&in,&out);

x=out.x.cx;      // To get x coordinate

y=out.x.dx;      // To get y coordinate

cl=out.x.bx;     // 1/0 ; 1 for mouse click


if(x>=220 && y>=70 && x<=360&& y<=137)
{
   if(flag!=1)
   highlightremover();
   setcolor(21);
   rectangle(80,70,560,130);
   flag=1;

      if(cl==1)



			{
			unload_mouse_driver();

			cleardevice();

			gotoxy(39,13);
			setcolor(12);
			outtextxy(200,210,"Loading.....");

			delay(500);
			cleardevice();
			newGame();
			   }
			   }

 if(x>=220 && y>=140 && x<=360&& y<=199)
{
    if(flag!=2)
    highlightremover() ;
    setcolor(22);
    rectangle(80,130,560,190);
    flag=2;
  if(cl==1)

			{
			unload_mouse_driver();

			cleardevice();

			gotoxy(39,13);
			setcolor(13);
			outtextxy(200,210,"Load game.....");

			delay(1500);

			exit(1);
			   }
			   }

 if(x>=220 && y>=200 && x<=360&& y<=255 )
 {

       if(flag!=3)
    highlightremover() ;
    setcolor(4);
    rectangle(80,190,560,250);
    flag=3;
  if(cl==1)



			{

			 unload_mouse_driver();

			 cleardevice();

			gotoxy(39,13);
			setcolor(14);
			outtextxy(200,210,"Setting.....");

			delay(1500);

			 exit(1);
			   }
			   }


if(x>=220 && y>=260 && x<=360&& y<=318 )
{
    if(flag!=4)
    highlightremover() ;
    setcolor(13);
    rectangle(80,250,560,310);
    flag=4;
  if(cl==1)


			{

			 unload_mouse_driver();

			 cleardevice();

			gotoxy(39,13);
			setcolor(20);
			outtextxy(200,210,"About.....");

			delay(1500);

			 exit(1);
			   }
			   }
if(x>=220 && y>=320 && x<=360&& y<=376 )
{
   if(flag!=5)
    highlightremover() ;
    setcolor(10);
    rectangle(80,310,560,370);
    flag=5;
  if(cl==1)


			{

			 unload_mouse_driver();

			 cleardevice();

			gotoxy(39,13);
			setcolor(17);
			outtextxy(200,210,"Help.....");

			delay(1500);

			 exit(1);
			   }
			   }


if(x>=220 && y>=380 && x<=360&& y<=480 )
{
    if(flag!=6)
    highlightremover() ;
    setcolor(9);
    rectangle(80,370,560,430);
    flag=6;
  if(cl==1)

			{

			 unload_mouse_driver();

			 cleardevice();

			gotoxy(39,13);
			setcolor(13);
			outtextxy(200,210,"Thank You");


			delay(1500);

			 exit(1);
			   }
				     }


			}
  load_mouse_driver();
	menugraphics();

	while(1)

			{

int x,y,cl,a,flag;

in.x.ax=3;

int86(0x33,&in,&out);

x=out.x.cx;      // To get x coordinate

y=out.x.dx;      // To get y coordinate

cl=out.x.bx;     // 1/0 ; 1 for mouse click


if(x>=220 && y>=70 && x<=360&& y<=137)
{
   if(flag!=1)
   highlightremover();
   setcolor(21);
   rectangle(80,70,560,130);
   flag=1;

      if(cl==1)



			{
			unload_mouse_driver();

			cleardevice();

			gotoxy(39,13);
			setcolor(12);
			outtextxy(200,210,"Loading.....");

			delay(500);
			cleardevice();
			newGame();
			   }
			   }

 if(x>=220 && y>=140 && x<=360&& y<=199)
{
    if(flag!=2)
    highlightremover() ;
    setcolor(22);
    rectangle(80,130,560,190);
    flag=2;
  if(cl==1)

			{
			unload_mouse_driver();

			cleardevice();

			gotoxy(39,13);
			setcolor(13);
			outtextxy(200,210,"Load game.....");

			delay(1500);

			exit(1);
			   }
			   }

 if(x>=220 && y>=200 && x<=360&& y<=255 )
 {

       if(flag!=3)
    highlightremover() ;
    setcolor(4);
    rectangle(80,190,560,250);
    flag=3;
  if(cl==1)



			{

			 unload_mouse_driver();

			 cleardevice();

			gotoxy(39,13);
			setcolor(14);
			outtextxy(200,210,"Setting.....");

			delay(1500);

			 exit(1);
			   }
			   }


if(x>=220 && y>=260 && x<=360&& y<=318 )
{
    if(flag!=4)
    highlightremover() ;
    setcolor(13);
    rectangle(80,250,560,310);
    flag=4;
  if(cl==1)


			{

			 unload_mouse_driver();

			 cleardevice();

			gotoxy(39,13);
			setcolor(20);
			outtextxy(200,210,"About.....");

			delay(1500);

			 exit(1);
			   }
			   }
if(x>=220 && y>=320 && x<=360&& y<=376 )
{
   if(flag!=5)
    highlightremover() ;
    setcolor(10);
    rectangle(80,310,560,370);
    flag=5;
  if(cl==1)


			{

			 unload_mouse_driver();

			 cleardevice();

			gotoxy(39,13);
			setcolor(17);
			outtextxy(200,210,"Help.....");

			delay(1500);

			 exit(1);
			   }
			   }


if(x>=220 && y>=380 && x<=360&& y<=480 )
	{
    if(flag!=6)
    highlightremover() ;
    setcolor(9);
    rectangle(80,370,560,430);
    flag=6;
  if(cl==1)

		{
		     unload_mouse_driver();
		     cleardevice();
		     gotoxy(39,13);
		     setcolor(13);
		     outtextxy(200,210,"Thank You");
		     delay(1500);
		     exit(1);
		 } //click if ends
	}//exit if ends
  } //while ends
}//menu ends

void main()
{
	int gd=DETECT,gm;

	initgraph(&gd,&gm,"c:\\tc\\bgi");
	     //  menu();
		newGame();

getch();
closegraph();
}//main ends

