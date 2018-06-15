/*Cmpe 163 Computer Graph and Argument Reality Course
File name: diffuse.c
Date Created: 
Description: diffuse reflection assignment
Author: Zihao Chen, Delong He, Guanghan Sha. Email: zihaokylechen@gmail.com
Professor: Harry Li, Ph.D, College of Computer Engineering
run on terminal:	gcc -o reflection reflection.c -lGL -lGLU -lglut -lm -std=gnu11

				
*/
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define ARRAY_LEN 30000 //define array length 
#define GRIDARRAY_LEN 512 //define grid square 512X512  

float focal_D=20;
float light_arrow[3]={0,100,101};

struct vec3{
	float x;
	float y;
	float z;
};
int pixelCount = 0;
typedef struct 
{
int x_start;
int y_start;
int x_end;
int y_end;
int i_accumulator;  
int x[ARRAY_LEN];
int y[ARRAY_LEN];
} MY_STRUCT; 

typedef struct 
{
int x;
int y;
int odd;
int even;
} Table; 



struct vec2{
	float xdp;
	float ydp;
};

struct vecInt2{
	int x;
	int y;
};

struct vec3 pixels[100000000];
float Kr = 0.5;
float Kb = 0.8;
float Kg = 0.1;
float PI = 3.141592653;
float scaler_alpha=200;
float arrow[7][3]={
	{75,30,50},
	{75,70,50},
	{25,70,50},
	{25,85,50},
	{0,50,50},
	{25,15,50},
	{25,30,50}
	};
float arrowTemplate[7][3]={
	{75,30,50},
	{75,70,50},
	{25,70,50},
	{25,85,50},
	{0,50,50},
	{25,15,50},
	{25,30,50}
	};
void getDDAline( MY_STRUCT * );

struct vec2 worldToView(struct vec3 vertex){
	struct vec3 eye_cam;
	eye_cam.x = 3000.0f;
	eye_cam.y = 3000.0f;
	eye_cam.z = 3000.0f;

	float sin_theta=0,cos_theta=0,sin_phi=0,cos_phi=0,rho=0;
	
	sin_theta = eye_cam.y/sqrt(eye_cam.x*eye_cam.x + eye_cam.y*eye_cam.y);
	cos_theta = eye_cam.x/sqrt(eye_cam.x*eye_cam.x + eye_cam.y*eye_cam.y);
	rho = sqrt(eye_cam.x*eye_cam.x+eye_cam.y*eye_cam.y+eye_cam.z*eye_cam.z);
	sin_phi = sqrt(eye_cam.x*eye_cam.x+eye_cam.y*eye_cam.y)/rho;
	cos_phi = eye_cam.z/rho;
	
	float xp = -sin_theta*vertex.x+cos_theta*vertex.y;
	float yp = -cos_phi*cos_theta*vertex.x-cos_phi*sin_theta*vertex.y+sin_phi*vertex.z;
	float zp = -sin_phi*cos_theta*vertex.x-sin_phi*cos_theta*vertex.y-cos_phi*vertex.z+rho;
		//projection 
	struct vec2 result;	
	result.xdp = focal_D*xp/zp;
	result.ydp = focal_D*yp/zp;
	return result;
}

float radius(struct vec3 a){
	float result,xx,yy,zz;
	xx = light_arrow[0] - a.x;
	yy = light_arrow[1] - a.y;
	zz = light_arrow[2] - a.z;
	result = xx*xx+yy*yy+zz*zz;
	return result;	
}

void createPixelArray(struct vec3 start_vertex,struct vec3 end_vertex){
	for(int j = start_vertex.y; j<=end_vertex.y;j=j+1){
			for(int i = start_vertex.x; i<=end_vertex.x;i=i+1){
			struct vec3 pixel;
			pixels[pixelCount].x=i;
			pixels[pixelCount].y=j;
			pixels[pixelCount].z=start_vertex.z;
			pixelCount=pixelCount+1;
			printf("%d\n",pixelCount);
		}
	}
}

void drawColor(struct vec3 offset){
	float i;
	float j;
	
	int linedone=0;
	glLineWidth(5.0);
	glBegin(GL_POINTS);
	struct vec2 p;
	struct vec3 temp;
	for(int i = 0;i<pixelCount;i++){
		
		temp.x=pixels[i].x+offset.x;
		temp.y=pixels[i].y+offset.y;
		temp.z=pixels[i].z+offset.z;
		p=worldToView(temp);
		float Ir,Ib,Ig;
		float r = radius(temp);
		Ir = scaler_alpha*Kr  *(1/r)* (((light_arrow[2] - temp.z)*(light_arrow[2] - temp.z))/sqrt((r)));
		
		Ib = scaler_alpha*Kb  *(1/r)* (((light_arrow[2] - temp.z)*(light_arrow[2] - temp.z))/sqrt((r)));

		Ig = scaler_alpha*Kg  *(1/r)* (((light_arrow[2] - temp.z)*(light_arrow[2] - temp.z))/sqrt((r)));
		//printf("Ir: %0.8f,Ib: %0.8f,Ig: %0.8f\n",Ir,Ib,Ig);
		glColor3f(Ir,Ib,Ig);
		glVertex2f(p.xdp,p.ydp);	
		
		
	}
	glEnd();
}


void DrawArrow(struct vec3 shift,struct vec3 eye_cam){
	for(int i =0;i<7;i++){
	arrow[i][0] = arrowTemplate[i][0]+shift.x;
	arrow[i][1] = arrowTemplate[i][1]+shift.y;
	arrow[i][2] = arrowTemplate[i][2]+shift.z;
	} 
	float normal_vector[3]={0,0,1};
	float lamda_top[8];
	float lamda_bot[8];
	float shade_arrow_top[8][3];
	float shade_arrow_bot[8][3];
	float arrow_below[7][3];
	for(int i=0;i<7;i++){
		lamda_top[i]=normal_vector[0]*light_arrow[0]+normal_vector[1]*light_arrow[1]+normal_vector[2]*light_arrow[2];
		lamda_top[i]=lamda_top[i]/(normal_vector[0]*(light_arrow[0]-arrow[i][0])+normal_vector[1]*(light_arrow[1]-arrow[i][1])+normal_vector[2]*(light_arrow[2]-arrow[i][2]));
		lamda_top[i]=lamda_top[i]*(-1);
		lamda_bot[i]=normal_vector[0]*light_arrow[0]+normal_vector[1]*light_arrow[1]+normal_vector[2]*light_arrow[2];
		lamda_bot[i]=lamda_bot[i]/(normal_vector[0]*(light_arrow[0]-arrow_below[i][0])+normal_vector[1]*(light_arrow[1]-arrow_below[i][1])+normal_vector[2]*(light_arrow[2]-arrow_below[i][2]));
		lamda_bot[i]=lamda_bot[i]*(-1);
	}

	for(int i=0;i<7;i++){
		shade_arrow_top[i][0]=light_arrow[0]+lamda_top[i]*(light_arrow[0]-arrow[i][0]);
		shade_arrow_top[i][1]=light_arrow[1]+lamda_top[i]*(light_arrow[1]-arrow[i][1]);
		shade_arrow_top[i][2]=light_arrow[2]+lamda_top[i]*(light_arrow[2]-arrow[i][2]);
		shade_arrow_bot[i][0]=light_arrow[0]+lamda_bot[i]*(light_arrow[0]-arrow_below[i][0]);
		shade_arrow_bot[i][1]=light_arrow[1]+lamda_bot[i]*(light_arrow[1]-arrow_below[i][1]);
		shade_arrow_bot[i][2]=light_arrow[2]+lamda_bot[i]*(light_arrow[2]-arrow_below[i][2]);
	}
	float sin_theta = eye_cam.y/sqrt(eye_cam.x*eye_cam.x + eye_cam.y*eye_cam.y);
	float cos_theta = eye_cam.x/sqrt(eye_cam.x*eye_cam.x + eye_cam.y*eye_cam.y);
	float rho = sqrt(eye_cam.x*eye_cam.x+eye_cam.y*eye_cam.y+eye_cam.z*eye_cam.z);
	float sin_phi = sqrt(eye_cam.x*eye_cam.x+eye_cam.y*eye_cam.y)/rho;
	float cos_phi = eye_cam.z/rho;
	float arrow_xprime[7],arrow_yprime[7],arrow_zprime[7],arrow_xdprime[7],arrow_ydprime[7];

	
	
	

	for(int i=0;i<7;i++){
		arrow_xprime[i] = -sin_theta*arrow[i][0]+cos_theta*arrow[i][1];
		arrow_yprime[i] = -cos_phi*cos_theta*arrow[i][0]-cos_phi*sin_theta*arrow[i][1]+sin_phi*arrow[i][2];
		arrow_zprime[i] = -sin_phi*cos_theta*arrow[i][0]-sin_phi*cos_theta*arrow[i][1]-cos_phi*arrow[i][2]+rho;

		arrow_xdprime[i]=focal_D*arrow_xprime[i]/arrow_zprime[i];
		arrow_ydprime[i]=focal_D*arrow_yprime[i]/arrow_zprime[i];
	}
	float arrowb_xprime[7],arrowb_yprime[7],arrowb_zprime[7],arrowb_xdprime[7],arrowb_ydprime[7];
	for(int i=0;i<7;i++){
		arrowb_xprime[i] = -sin_theta*arrow_below[i][0]+cos_theta*arrow_below[i][1];
		arrowb_yprime[i] = -cos_phi*cos_theta*arrow_below[i][0]-cos_phi*sin_theta*arrow_below[i][1]+sin_phi*arrow_below[i][2];
		arrowb_zprime[i] = -sin_phi*cos_theta*arrow_below[i][0]-sin_phi*cos_theta*arrow_below[i][1]-cos_phi*arrow_below[i][2]+rho;

		arrowb_xdprime[i]=focal_D*arrowb_xprime[i]/arrowb_zprime[i];
		arrowb_ydprime[i]=focal_D*arrowb_yprime[i]/arrowb_zprime[i];
	}
	float shadow_top_xprime[7],shadow_top_yprime[7],shadow_top_zprime[7],shadow_top_xdprime[7],shadow_top_ydprime[7];
	for(int i=0;i<7;i++){
		shadow_top_xprime[i] = -sin_theta*shade_arrow_top[i][0]+cos_theta*shade_arrow_top[i][1];
		shadow_top_yprime[i] = -cos_phi*cos_theta*shade_arrow_top[i][0]-cos_phi*sin_theta*shade_arrow_top[i][1]+sin_phi*shade_arrow_top[i][2];
		shadow_top_zprime[i] = -sin_phi*cos_theta*shade_arrow_top[i][0]-sin_phi*cos_theta*shade_arrow_top[i][1]-cos_phi*shade_arrow_top[i][2]+rho;

		shadow_top_xdprime[i]=focal_D*shadow_top_xprime[i]/shadow_top_zprime[i];
		shadow_top_ydprime[i]=focal_D*shadow_top_yprime[i]/shadow_top_zprime[i];
	}
	float shadow_bot_xprime[7],shadow_bot_yprime[7],shadow_bot_zprime[7],shadow_bot_xdprime[7],shadow_bot_ydprime[7];
	for(int i=0;i<7;i++){
		shadow_bot_xprime[i] = -sin_theta*shade_arrow_bot[i][0]+cos_theta*shade_arrow_bot[i][1];
		shadow_bot_yprime[i] = -cos_phi*cos_theta*shade_arrow_bot[i][0]-cos_phi*sin_theta*shade_arrow_bot[i][1]+sin_phi*shade_arrow_bot[i][2];
		shadow_bot_zprime[i] = -sin_phi*cos_theta*shade_arrow_bot[i][0]-sin_phi*cos_theta*shade_arrow_bot[i][1]-cos_phi*shade_arrow_bot[i][2]+rho;

		shadow_bot_xdprime[i]=focal_D*shadow_bot_xprime[i]/shadow_bot_zprime[i];
		shadow_bot_ydprime[i]=focal_D*shadow_bot_yprime[i]/shadow_bot_zprime[i];
	}
	glBegin(GL_LINES);
	glColor3f(0.5,0.5,0.5);

	glVertex2f(shadow_top_xdprime[0],shadow_top_ydprime[0]);
	for(int i=0;i<7;i++){
	glVertex2f(shadow_top_xdprime[i],shadow_top_ydprime[i]);
	glVertex2f(shadow_top_xdprime[i],shadow_top_ydprime[i]);
	}
	glVertex2f(shadow_top_xdprime[0],shadow_top_ydprime[0]);
	glEnd();
}


void Display(){
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		     // Clear Screen and Depth Buffer	
	MY_STRUCT  my_struct;  
	my_struct.i_accumulator = 0; //initializtion
	struct vec3 eye_cam;
	eye_cam.x = 3000.0f;
	eye_cam.y = 3000.0f;
	eye_cam.z = 3000.0f;
	
	float shade_arrow_top[8][3];
	float shade_arrow_bot[8][3];

	float cube[8][3]={{100.0,100.0,100.0}, //p1
	{0.0,100.0,100.0}, //p2
	{0.0,0.0,100.0},//p3
	{100.0,00.0,100.0},//p4
	{100.0,100.0,0.0},//p5
	{0.0,100.0,0.0},//p6
	{0.0,0.0,0.0},//p7
	{100.0,0.0,0.0}};//p8
	
	float arrow_below[7][3];
	
	float thickness=10.0;
	for(int i=0;i<7;i++){
		arrow_below[i][0]=arrow[i][0];
		arrow_below[i][1]=arrow[i][1];
		arrow_below[i][2]=arrow[i][2]+thickness;
	}
	float xw[3] = {500,0,0};
	float yw[3] = {0,500,0};
	float zw[3] = {0,0,500};
	
	float xp[3],yp[3],zp[3];
	float sin_theta=0,cos_theta=0,sin_phi=0,cos_phi=0,rho=0;
	



	sin_theta = eye_cam.y/sqrt(eye_cam.x*eye_cam.x + eye_cam.y*eye_cam.y);
	cos_theta = eye_cam.x/sqrt(eye_cam.x*eye_cam.x + eye_cam.y*eye_cam.y);
	rho = sqrt(eye_cam.x*eye_cam.x+eye_cam.y*eye_cam.y+eye_cam.z*eye_cam.z);
	sin_phi = sqrt(eye_cam.x*eye_cam.x+eye_cam.y*eye_cam.y)/rho;
	cos_phi = eye_cam.z/rho;

	//transformation
	float xdp[3],ydp[3];
	
for(int i=0;i<3;i++){	
	xp[i] = -sin_theta*xw[i]+cos_theta*yw[i];
	yp[i] = -cos_phi*cos_theta*xw[i]-cos_phi*sin_theta*yw[i]+sin_phi*zw[i];
	zp[i] = -sin_phi*cos_theta*xw[i]-sin_phi*cos_theta*yw[i]-cos_phi*zw[i]+rho;
		//projection 
	xdp[i] = focal_D*xp[i]/zp[i];
	ydp[i] = focal_D*yp[i]/zp[i];
	
	}
		//calculation for cube
float cube_xprime[8],cube_yprime[8],cube_zprime[8],cube_xdprime[8],cube_ydprime[8];
for(int i=0;i<8;i++){
	cube_xprime[i] = -sin_theta*cube[i][0]+cos_theta*cube[i][1];
	cube_yprime[i] = -cos_phi*cos_theta*cube[i][0]-cos_phi*sin_theta*cube[i][1]+sin_phi*cube[i][2];
	cube_zprime[i] = -sin_phi*cos_theta*cube[i][0]-sin_phi*cos_theta*cube[i][1]-cos_phi*cube[i][2]+rho;

	cube_xdprime[i]=focal_D*cube_xprime[i]/cube_zprime[i];
	cube_ydprime[i]=focal_D*cube_yprime[i]/cube_zprime[i];
}



	for(int i=0;i<7;i++){
	int z;
	z=i+1;
	z=z%7;
	
	my_struct.x_start=arrow[i][0];
	my_struct.y_start=arrow[i][1];
	my_struct.x_end=arrow[z][0];
	my_struct.y_end=arrow[z][1];
	printf("i:%i, z:%i, x_start: %i, arrow_start: %0.4f\n",i,z,my_struct.x_start,arrow[i][0]);
	
	my_struct.x[my_struct.i_accumulator] = my_struct.x_start ; //initializtion
   	my_struct.y[my_struct.i_accumulator] = my_struct.y_start ; //initializtion
	
	getDDAline( &my_struct );
	
	printf("one line\n");
	printf("i inside : %i \n",i);
	}

printf("i: %i\n",my_struct.i_accumulator);
	
	
	Table table[30000];
	struct vecInt2 pairList[10000];
	struct vec2 pixel[100000];
	
	int counter=0;
	int boundary=0;
	
	int sizeOfpairList=0;
	printf("list of boundary\n");
	for(int i=255;i>=-255;i--){
		for(int j=-255;j<=255;j++){
			for(int k=0;k<my_struct.i_accumulator;k++){
				if(my_struct.y[k]==i && my_struct.x[k]==j) {
					printf("x = %i, y = %i \n",my_struct.x[k],my_struct.y[k]);
					pairList[sizeOfpairList].x=my_struct.x[k];
					pairList[sizeOfpairList].y=my_struct.y[k];
					sizeOfpairList++;
				}
			}
		}
	}
	for(int k=0;k<my_struct.i_accumulator;k++){
		if( (k+1) < my_struct.i_accumulator){
			if( (pairList[k+1].y==pairList[k].y) && ((pairList[k+1].x-pairList[k].x) > 1))
			{
				table[counter].x=pairList[k].x;
				table[counter].y=pairList[k].y;
				counter++;
				int v = k;
				while ((pairList[v+1].y==pairList[v].y) && ((pairList[v+1].x-pairList[v].x)>=1)){
					v++;
				}
				table[counter].x=pairList[v].x;
				table[counter].y=pairList[v].y;
				counter++;
			}
		}
	}

	struct vec3 temp;
	struct vec3 temp1;
	struct vec2 temp2;
	//int flag = 0;
	temp.z=50;
	//float xfloat,yfloat;
	int countP=0;

	while (countP<counter){

		temp.x=table[countP].x;
		temp.y=table[countP].y;
		countP++;
		temp1.x=table[countP].x;
		temp1.y=table[countP].y;
		countP++;
		createPixelArray(temp,temp1);
		
	}

	my_struct.i_accumulator=0;
glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,0.0f);
	glFlush();
	int switch1=0;
	struct vec3 shift = {0,0,0};
	float sx = 0,sy = 0,sz=0;
	
while(1){
	FILE *pos;
	pos = fopen("Position","r");
	float x,y,z;
	
		fscanf(pos, "%f", &sx);	
		fscanf(pos, "%f", &sy);
		fscanf(pos, "%f", &sz);	
	shift.x = sx*10;
	shift.y = sy*10;	
	shift.z = sz*10;
	fclose(pos);
	int deltaLight = 1;
	
	if(switch1==0) {light_arrow[1]=light_arrow[1]-deltaLight;}
	else if(switch1==1){light_arrow[1]=light_arrow[1]+deltaLight;}
	if(light_arrow[1]<-200) {switch1=1;}
	else if(light_arrow[1]>200) {switch1=0;}

			printf("%f\n",light_arrow[1]);
	
	glLineWidth(2.5);
 	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glBegin(GL_LINES);

	glColor3f(1.0,0.0,0.0);
	glVertex2f(0,0);	
	glVertex2f(xp[0],yp[0]);
	glColor3f(0.0,1.0,0.0);
	glVertex2f(0,0);	
	glVertex2f(xp[1],yp[1]);
	glColor3f(0.0,0.0,1.0);
	glVertex2f(0,0);
	glVertex2f(xp[2],yp[2]);
	DrawArrow(shift,eye_cam);
	glColor3f(0.8,0.0,0.0);
	drawColor(shift);

	
	
	
		//arrow shadow
	
	usleep(200);
	glEnd();
	glFlush();
	
	/*
glColor3f(0.0,1.0,1.0);
glBegin(GL_LINES);
	glVertex2f(arrow_xdprime[0],arrow_ydprime[0]);
	for(int i=0;i<7;i++){
	glVertex2f(arrow_xdprime[i],arrow_ydprime[i]);
	glVertex2f(arrow_xdprime[i],arrow_ydprime[i]);
	}
	glVertex2f(arrow_xdprime[0],arrow_ydprime[0]);
	
	//glVertex2f(arrowb_xdprime[0],arrowb_ydprime[0]);
	//for(int i=0;i<7;i++){
	//glVertex2f(arrowb_xdprime[i],arrowb_ydprime[i]);
	//glVertex2f(arrowb_xdprime[i],arrowb_ydprime[i]);
	//}
	//glVertex2f(arrowb_xdprime[0],arrowb_ydprime[0]);
//
	//for(int i=0;i<7;i++){
	//glVertex2f(arrow_xdprime[i],arrow_ydprime[i]);
	//glVertex2f(arrowb_xdprime[i],arrowb_ydprime[i]);
	//}
	
glEnd();
*/
} //end while

	
}

int main(int argc,char **argv)
{
        glutInit(&argc,argv);

		//glEnable(GL_DEPTH_TEST);
		glutInitWindowSize(700,600);
        //glutInitWindowPosition(200,200);
        glutCreateWindow("ArrowInCube");
        
        glutDisplayFunc(Display);     
	glutMainLoop();
        return 0;
}

void getDDAline( MY_STRUCT *xx )   
{
#include <math.h>
#include <stdio.h>

int   i; 
float fslop1; 
float fx_buf[ARRAY_LEN];    // intermediate data 
float fy_buf[ARRAY_LEN];    // intermediate data 
float fslop2;
/*
printf("Sub 0, 348: i_accumulator = %4d \n",xx->i_accumulator );  
printf("Sub 0, 348: x_start = %4d y_start = %4d\n",xx->x_start, xx->y_start);  
printf("Sub 0, 348: x_end = %4d y_end = %4d\n",xx->x_end, xx->y_end);  
printf("Sub 0, 348: x = %4d y = %4d\n",xx->x[xx->i_accumulator], xx->y[xx->i_accumulator]);  
*/

//-------compute slop----------------------//



//--------------------------------------------
// for DDA line, if slop1 <= 1; 
// y_(k+1) = y_k + slop1;                   
// x_(k+1) = x_k; 
//-------------------------------------------- 



fx_buf[xx->i_accumulator] = xx->x_start;  // convert to floating point  
fy_buf[xx->i_accumulator] = xx->y_start;  // convert to floating point  

int steps;
int dx;
int dy;
dx=xx->x_end - xx->x_start;
dy=xx->y_end - xx->y_start;
if ((xx->x_end - xx->x_start) != 0) {
	fslop1 = (float) (xx->y_end - xx->y_start);  
	fslop1 = fslop1 / (float) (xx->x_end - xx->x_start); 
	fslop2 = (float) (xx->x_end - xx->x_start);  
	fslop2 = fslop2 /(float) (xx->y_end - xx->y_start); 
}

else fslop1 = 2;



steps=(abs(dx)>abs(dy))?(abs(dx)):(abs(dy));


for(int k=0; k<steps;k++){

printf("Sub 0, 374: x = %4d y = %4d\n",xx->x[xx->i_accumulator], xx->y[xx->i_accumulator]);  

if ( fslop1 <= 1 && fslop1 > 0 && dy > 0 && dx > 0) {    //dy>dx +dy q3 to q1
xx->x[xx->i_accumulator+1] = xx->x[xx->i_accumulator] + 1; 
fy_buf[xx->i_accumulator+1] = fy_buf[xx->i_accumulator] + fslop1; 
xx->y[xx->i_accumulator+1] = fy_buf[xx->i_accumulator+1];  // convert back to integer   
printf("Sub 1: x = %4d y = %4d\n",xx->x[xx->i_accumulator+1], xx->y[xx->i_accumulator+1]);  
} //end if 

else if ( fslop1 >= -1 && fslop1 < 0 && dy < 0 && dx > 0) {  //dy>dx -dy q4 to q2
xx->x[xx->i_accumulator+1] = xx->x[xx->i_accumulator] + 1; 
fy_buf[xx->i_accumulator+1] = fy_buf[xx->i_accumulator] + fslop1; 
xx->y[xx->i_accumulator+1] = fy_buf[xx->i_accumulator+1];  // convert back to integer   
printf("Sub 1: x = %4d y = %4d\n",xx->x[xx->i_accumulator+1], xx->y[xx->i_accumulator+1]);  
} //end if 

else if ( fslop1 <= 1 && fslop1 > 0 && dy < 0 && dx < 0) {  //dy>dx +dy q1 to q3
xx->x[xx->i_accumulator+1] = xx->x[xx->i_accumulator] - 1; 
fy_buf[xx->i_accumulator+1] = fy_buf[xx->i_accumulator] + fslop1; 
xx->y[xx->i_accumulator+1] = fy_buf[xx->i_accumulator+1];  // convert back to integer   
printf("Sub 1: x = %4d y = %4d\n",xx->x[xx->i_accumulator+1], xx->y[xx->i_accumulator+1]);  
} //end if 

else if ( fslop1 >= -1 && fslop1 < 0 && dy > 0 && dx < 0) {  //dy>dx -dy q2 to q4
xx->x[xx->i_accumulator+1] = xx->x[xx->i_accumulator] - 1; 
fy_buf[xx->i_accumulator+1] = fy_buf[xx->i_accumulator] - fslop1; 
xx->y[xx->i_accumulator+1] = fy_buf[xx->i_accumulator+1];  // convert back to integer   
printf("Sub 1: x = %4d y = %4d\n",xx->x[xx->i_accumulator+1], xx->y[xx->i_accumulator+1]);  
} //end if 

else if (fslop1 == 0 && dx < 0 && dy == 0){
xx->x[xx->i_accumulator+1] = xx->x[xx->i_accumulator] - 1; 
fy_buf[xx->i_accumulator+1] = fy_buf[xx->i_accumulator]; 
xx->y[xx->i_accumulator+1] = fy_buf[xx->i_accumulator+1];  // convert back to integer   
printf("Sub 1: x = %4d y = %4d\n",xx->x[xx->i_accumulator+1], xx->y[xx->i_accumulator+1]);  
}

else if (fslop1 == 0 && dx > 0 && dy == 0){
xx->x[xx->i_accumulator+1] = xx->x[xx->i_accumulator] + 1; 
fy_buf[xx->i_accumulator+1] = fy_buf[xx->i_accumulator]; 
xx->y[xx->i_accumulator+1] = fy_buf[xx->i_accumulator+1];  // convert back to integer   
printf("Sub 1: x = %4d y = %4d\n",xx->x[xx->i_accumulator+1], xx->y[xx->i_accumulator+1]);  
}

else if ( dx != 0 && abs(dx)<abs(dy)) { 
	
	if ( fslop1 > 1 && dy > 0 && dx > 0 ) {   //dx>dy +dx q3 to q1
	xx->y[xx->i_accumulator+1] = xx->y[xx->i_accumulator] + 1; 
	fx_buf[xx->i_accumulator+1] = fx_buf[xx->i_accumulator] + fslop2; 
	xx->x[xx->i_accumulator+1] = fx_buf[xx->i_accumulator+1];  
	printf("Sub 2: x = %4d y = %4d\n",xx->x[xx->i_accumulator+1], xx->y[xx->i_accumulator+1]);   
	} //end if 

	else if ( fslop1 < -1 && dy < 0 && dx > 0) {   //dx>dy +dx q4 to q2
	xx->y[xx->i_accumulator+1] = xx->y[xx->i_accumulator] - 1; 
	fx_buf[xx->i_accumulator+1] = fx_buf[xx->i_accumulator] - fslop2; 
	xx->x[xx->i_accumulator+1] = fx_buf[xx->i_accumulator+1];  
	printf("Sub 2: x = %4d y = %4d\n",xx->x[xx->i_accumulator+1], xx->y[xx->i_accumulator+1]);   
	} //end if 

	else if ( fslop1 > 1 && dy < 0 && dx < 0) {   //dx>dy -dx q1 to q3
	xx->y[xx->i_accumulator+1] = xx->y[xx->i_accumulator] - 1; 
	fx_buf[xx->i_accumulator+1] = fx_buf[xx->i_accumulator] - fslop2; 
	xx->x[xx->i_accumulator+1] = fx_buf[xx->i_accumulator+1];  
	printf("Sub 2: x = %4d y = %4d\n",xx->x[xx->i_accumulator+1], xx->y[xx->i_accumulator+1]);   
	} //end if 

	else if ( fslop1 < -1 && dy > 0 && dx < 0) {    //dx>dy -dy q2 to q4
	xx->y[xx->i_accumulator+1] = xx->y[xx->i_accumulator] + 1; 
	fx_buf[xx->i_accumulator+1] = fx_buf[xx->i_accumulator] + fslop2; 
	xx->x[xx->i_accumulator+1] = fx_buf[xx->i_accumulator+1];  
	printf("Sub 2: x = %4d y = %4d\n",xx->x[xx->i_accumulator+1], xx->y[xx->i_accumulator+1]);   
	} //end if 
}
else if ( dx == 0 && dy > 0){
xx->y[xx->i_accumulator+1] = xx->y[xx->i_accumulator] + 1; 
fx_buf[xx->i_accumulator+1] = fx_buf[xx->i_accumulator]; 
xx->x[xx->i_accumulator+1] = fx_buf[xx->i_accumulator+1];  
printf("Sub 2: x = %4d y = %4d\n",xx->x[xx->i_accumulator+1], xx->y[xx->i_accumulator+1]);  
} //end else if  

else if ( dx == 0 && dy < 0){
xx->y[xx->i_accumulator+1] = xx->y[xx->i_accumulator] - 1; 
fx_buf[xx->i_accumulator+1] = fx_buf[xx->i_accumulator]; 
xx->x[xx->i_accumulator+1] = fx_buf[xx->i_accumulator+1];  
printf("Sub 2: x = %4d y = %4d\n",xx->x[xx->i_accumulator+1], xx->y[xx->i_accumulator+1]);  
} //end else if 

xx->i_accumulator++;
} //end while loop 
printf("finish a line");
} //end getDDAline(.) 

