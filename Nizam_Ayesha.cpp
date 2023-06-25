/*********
   CTIS164 - Template Source Program
----------
STUDENT : Ayesha Nizam
SECTION :002
HOMEWORK: 03
----------
PROBLEMS: 
----------
ADDITIONAL FEATURES:
Created a Flower using polar and vector form of equation
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "vec.h"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON       1// 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define PI 3.1415


/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

bool move[3] = { true, true, true }, animation = true;
char status[4][10] = { {"ON"},{"ON"}, {"ON"}, {"ON"} };


typedef struct {
    float r, g, b;
} color_t;

typedef struct {
    vec_t   pos;
    color_t color;   
} light_t;

typedef struct {
    vec_t pos;
    vec_t N;
} vertex_t;

light_t light[3] =
{
   { {    0, 0 }, { 1, 0, 0 } },
   { {  200, 0 }, { 0, 1, 0 } },
   { { -200, 0 }, { 0, 0, 1 } }
};

double IPangle[3];
int radius[3] = { 180,215,250 };

polar_t Ipolar;
vec_t Ivec;
double LightsVelocity[3];


color_t mulColor(float k, color_t c) {
    color_t tmp = { k * c.r, k * c.g, k * c.b };
    return tmp;
}

color_t addColor(color_t c1, color_t c2) {
    color_t tmp = { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b };
    return tmp;
}


color_t lightSimulation(vec_t outerPoint)
{
    color_t temp = { 0,0,0 };
    double diff;
    color_t effectColor;

    for (int i = 0; i < 3; i++)
    {
        if (move[i]) {

            diff = angleBetween2V(light[i].pos, outerPoint);

            if (diff < 0)
               diff = diff * (-1);

            if (diff < 60)
                effectColor = mulColor(cos(diff*D2R)/1.05, light[i].color);
            else 
                effectColor = mulColor(0, light[i].color);

            temp = addColor(temp, effectColor);
        }
    }
    return temp;
}


void InititalPos()
{
    int direction[3];
    int min = -1, n;
    srand(time(0));
    IPangle[0] = (double)(rand() % (364 - 1 + 1)) + 1;
    IPangle[1] = (double)(rand() % (364 - 1 + 1)) + 1;
    IPangle[2] = (double)(rand() % (364 - 1 + 1)) + 1;
    
    for (int i = 0; i < 3; i++)
    {
        n = (rand() % (2 - min)) + min;
        while ( n == 0 ) 
            n = (rand() % (2 - min)) + min;
        direction[i] = n;   
    }

    //direction[0] = (rand() % (2 - min)) + min;
    //direction[1] = (rand() % (2 - min)) + min; 
    //direction[2] = (rand() % (2 - min)) + min;    

    LightsVelocity[0] = direction[0] * (double(rand() % (10 - 5)) + 5) / 10.0;
    LightsVelocity[1] = direction[1] * (double(rand() % (10 - 5)) + 5) / 10.0;
    LightsVelocity[2] = direction[2] * (double(rand() % (10 - 5)) + 5) / 10.0;

    for (int i = 0; i < 3; i++)
    {
        Ipolar.magnitude = radius[i];
        Ipolar.angle = IPangle[i];
        Ivec = pol2rec(Ipolar);
        light[i].pos.x = Ivec.x;
        light[i].pos.y = Ivec.y;  
    }
 
}

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}


void InformativeMessage()
{
    glColor3ub(192, 192, 192);
    glRectf(-400, -300, 400, -270);

    //writing instructions for the players
    glColor3ub(0, 0,0);
    vprint(-300, -285, GLUT_BITMAP_8_BY_13, "F1(Red):%s F2(Green):%s F3(BLUE):%s   F4(Pause/Start Animation):%s  F5(RESTART)",status[0], status[1],status[2],status[3]);
    glColor3ub(255, 255, 255);
    vprint(-330, 280, GLUT_BITMAP_8_BY_13, "HOMEWORK #4");
    glColor3ub(255, 255, 255);
    vprint(-330, 265, GLUT_BITMAP_8_BY_13, "Ayesha Nizam");
}

void drawPath()
{
   
    glColor3ub(250, 250, 250);
    circle_wire(0, 0, 180);

    glColor3ub(250, 250, 250);
    circle_wire(0, 0, 250);

    glColor3ub(250, 250, 250);
    circle_wire(0, 0, 215);   
     
}

void drawPlanet()
{
    polar_t Polarplanet;
    vec_t Planet;
    color_t res;
    glColor3f(0, 0, 0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0,0);
    for (int i = 0; i <= 360; i++)
    {
        Polarplanet.magnitude = 50;
        Polarplanet.angle = i;
        Planet = pol2rec(Polarplanet);
        res = lightSimulation(Planet);
        glColor3f(res.r,res.g,res.b);
        glVertex2f(Planet.x, Planet.y);
        
    }
    glEnd();
   
}

void drawFlower()
{
    double r1;
    double theta1=0;
    polar_t Pflower;
    vec_t Pvec;

    glColor3ub(255,0,127);
    glBegin(GL_LINE_LOOP);
    for (theta1 = 0; theta1 <= 360; theta1++)
    {
        r1 = 150*cos(4 * theta1 * D2R);
        Pflower.magnitude = r1;
        Pflower.angle = theta1;
        Pvec = pol2rec(Pflower);
        glVertex2f(Pvec.x, Pvec.y);
    }
    glEnd();
}



//
// To display onto window using OpenGL commands
//
void display() 
{
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    InformativeMessage();

    drawPath();
    drawFlower();

    for (int i = 0; i < 3; i++)
    {
        if (move[i])
        {

            glColor3f(light[i].color.r, light[i].color.g, light[i].color.b);
            circle(light[i].pos.x, light[i].pos.y, 15);
        }
    }

    drawPlanet();

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;

    case GLUT_KEY_F1: move[0] = !move[0];
        if (move[0] == false) {
            strcpy(status[0], "OFF"); 
        }
        else
            strcpy(status[0], "ON");
        break;

    case GLUT_KEY_F2: move[1] = !move[1];
        if (move[1] == false)
            strcpy(status[1], "OFF");
        else
            strcpy(status[1], "ON");
        break;

    case GLUT_KEY_F3: move[2] = !move[2];
        if (move[2] == false)
            strcpy(status[2], "OFF");
        else
            strcpy(status[2], "ON");
        break;

    case GLUT_KEY_F4: animation = !animation;
        if (animation == false)
            strcpy(status[3], "OFF");
        else
            strcpy(status[3], "ON");
        break;

    case GLUT_KEY_F5: 
        InititalPos();
        for (int i = 0; i < 3; i++)
            move[i] = true;
        animation = true;
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (animation)
    {
        for (int i = 0; i < 3; i++)
        {
            //if (IPangle[i] > 360)
              // IPangle[i] = double(int(IPangle[i]) % 360);
            Ipolar.magnitude = radius[i];
            Ipolar.angle = IPangle[i];
            Ivec = pol2rec(Ipolar);
            light[i].pos.x = Ivec.x;
            light[i].pos.y = Ivec.y;
           // if(move[i])
               IPangle[i] += LightsVelocity[i];
            
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    InititalPos();


}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 0);
    glutCreateWindow("Planetary System");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}