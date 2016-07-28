//
//  main.cpp
//  Project
//
//  Created by Supriya Narayan: supriyn@g.clemson.edu
//  Copyright © 2015 Supriya Narayan. All rights reserved.
//

#include<iostream>
#include<stdlib.h>
#include <cstdlib>
#include<iostream>
#include<stdlib.h>
#include "Vector.h"
#include "Camera.h"
#include <cstdlib>
#include <cstdio>
#ifdef __APPLE__    /* works on mac */
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
#define WIN_HT 400
#define WIN_WT 400
#define ALPHA 0.05f
using namespace std;
/*defining the initial position of ball and paddles*/
Vector3d pos1(5,0.5,0); 
Vector3d pos2(5,9.5,0);
Vector3d pos3(0.5,5,0);
Vector3d pos4(9.5,5,0);
Vector3d ballpos(5,5,0);
/* defining the velocity and accelerator */
Vector3d newballpos,newvelocity;
Vector3d Velocity(1,1,0);
Vector3d accel(2,-10,0);
void drawScene();
int co=0;
Camera *camera;

void initRendering()
{
    camera = new Camera(Vector3d(0, 0, 5), Vector3d(0, 0, 0), Vector3d(0, 1, 0));
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glDepthRange(0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void mouseEventHandler(int button, int state, int x, int y) {
    camera->HandleMouseEvent(button, state, x, y);
    glutPostRedisplay();
}

void motionEventHandler(int x, int y) {
    camera->HandleMouseMotion(x, y);
    glutPostRedisplay();
}

void handleResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

/*Function that decides the ball position */
void drawBall(){
    glTranslatef( 0, 0, 0 );
    glColor3f(0.0,0.5,0.0);
    glPushMatrix();
    glTranslatef(ballpos.x,ballpos.y,ballpos.z);
    glutWireSphere(0.5,20,20);
    glPopMatrix();
}

/*to change the ball position*/
void a(){
    if(newballpos.y<0.5)
    {
        if((pos1.x< newballpos.x) && (newballpos.x < pos1.x +2 ))
            
        {
            newvelocity = Velocity * -0.8;
            newballpos= ballpos + newvelocity;
        }
        
    }
}
void b(){
    if(newballpos.y>9.5)
    {
        if((pos2.x< newballpos.x) && (newballpos.x < pos2.x +2 ))
            
        {
            newvelocity = Velocity * -1;
            newballpos= ballpos + newvelocity;
        }
        
    }
}
void c(){
    if(newballpos.x<0.5)
    {
        if((pos3.y< newballpos.y) && (newballpos.y < pos3.y +2 ))
            
        {
            newvelocity = Velocity * -1.5;
            newballpos= ballpos + Velocity;
        }
        
    }
}

void d(){
    if(newballpos.x>9.5)
    {
        if((pos4.y< newballpos.y) && (newballpos.y < pos4.y +2 ))
            
        {
            newvelocity = Velocity * -1.5;
            newballpos= ballpos + Velocity;
        }
        
    }
}

void simulate(){
    newvelocity = Velocity + accel * 0.05;
    newballpos= ballpos + Velocity * 0.05;
    a();
    b();
    c();
    d();
    if (newballpos.x > 11.5 || newballpos.x < -1 || newballpos.y >11.5 || newballpos.y < -1)
    {
        newvelocity = (0,0,0);
        newballpos=ballpos+newvelocity;
        //initRendering();
    }
    ballpos=newballpos;
    Velocity = newvelocity;
    drawBall();
}

/*defining initial paddle position*/
void paddle3(){
    glTranslatef( 0, 0, 0 );
    glColor3f(0.0,0.0,0.5);
    glBegin(GL_QUADS);
    glVertex3f(pos4.x,pos4.y,pos4.z);
    glVertex3f(pos4.x+1,pos4.y,pos4.z);
    glVertex3f(pos4.x+1,pos4.y+2,pos4.z);
    glVertex3f(pos4.x,pos4.y+2,pos4.z);
    glEnd();
}
void paddle2(){
    glTranslatef( 0, 0, 0 );
    glColor3f(0.0,0.0,0.5);
    glBegin(GL_QUADS);
    glVertex3f(pos3.x,pos3.y,pos3.z);
    glVertex3f(pos3.x-1,pos3.y,pos3.z);
    glVertex3f(pos3.x-1,pos3.y+2,pos3.z);
    glVertex3f(pos3.x,pos3.y+2,pos3.z);
    glEnd();
}
void paddle1(){
    glTranslatef( 0, 0, 0 );
    glColor3f(0.0,0.0,0.5);
    glBegin(GL_QUADS);
    glVertex3f(pos2.x,pos2.y,pos2.z);
    glVertex3f(pos2.x+2,pos2.y,pos2.z);
    glVertex3f(pos2.x+2,pos2.y+1,pos2.z);
    glVertex3f(pos2.x,pos2.y+1,pos2.z);
    glEnd();
}
void paddle(){
    glTranslatef( 0, 0, 0 );
    glColor3f(0.0,0.0,0.5);
    glBegin(GL_QUADS);
    glVertex3f(pos1.x,pos1.y,pos1.z);
    glVertex3f(pos1.x+2,pos1.y,pos1.z);
    glVertex3f(pos1.x+2,pos1.y-1,pos1.z);
    glVertex3f(pos1.x,pos1.y-1,pos1.z);
    glEnd();
}
/*function to call both drawBall and all paddle*/
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->PerspectiveDisplay(WIN_HT, WIN_WT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef( 0, 0, 0 );
    drawBall();
    glColor3f( 0.5, 0, 0.0 );
    glBegin(GL_LINE_LOOP);
    glVertex3f( 0, 0, 0 );
    glVertex3f( 0, 10, 0 );
    glVertex3f( 10, 10, 0 );
    glVertex3f( 10, 0, 0 );
    glEnd();
    paddle();
    paddle1();
    paddle2();
    paddle3();
    //simulate();
    glutSwapBuffers();
}
void update(int value) {
    simulate();
    glutPostRedisplay();
    glutTimerFunc(175, update, 0);
}
/*function defining keys */
void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        /*a and s handle paddle on x axis to move left and right*/
        case 'a' :
            if(co==0){
            if(pos1.x<8){
                pos1.x=pos1.x+1;
                pos2.x=pos2.x+1;
                paddle();
                paddle1();
            }
            }
            else{
                if(pos1.x<8){
                    pos1.x=pos1.x+1;
                    paddle();
                }
            }
            break;
        case 's' :
            if(co==0){
            if(pos1.x>0){
                pos1.x=pos1.x-1;
                pos2.x=pos2.x-1;
                paddle();
                paddle1();
            }
            }
            else{
                if(pos1.x>0){
                    pos1.x=pos1.x-1;
                    paddle();
                }

            }
            break;
            /*k and l handle paddle on y axis to move up and down*/
        case 'k' :
            if(pos2.x<8){
                pos2.x=pos2.x+1;
                paddle1();
            }
            break;
        case 'l' :
            if(pos2.x>0){
                pos2.x=pos2.x-1;
                paddle1();
            }
            break;
            
        case 'r': case 'R':
            // reset the camera to its initial position
            camera->Reset();
            break;
        case 27: //Escape key
            exit(0);
    }
    glutPostRedisplay();
}
void idle(){
    
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIN_HT, WIN_WT);
    glutCreateWindow("Project");
    initRendering();
    glutDisplayFunc(drawScene);
    glutKeyboardFunc(handleKeypress);
    glutReshapeFunc(handleResize);
    glutMouseFunc(mouseEventHandler);
    glutMotionFunc(motionEventHandler);
    glutTimerFunc(25, update, 0);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
