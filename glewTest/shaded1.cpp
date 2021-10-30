/******************************************
BRENNEN GREEN
CS535 FALL 2021
PROJECT 2
DESCRIPTION:
GENERATES AN INTERACTIVE "BEDROOM" SCENE
FEATURING:
- A BED
- A DRESSER
- A DESK
- AN ANIMATED FAN
- A LAMP
- A RUG
- A BUNNY (EXTRA CREDIT)

TO INTERACT WITH THE PROGRAM:
- RIGHT MOUSE BUTTON: ZOOM IN
- LEFT MOUSE BUTTON: EXIT
- MIDDLE MOUSE BUTTON: ZOOM OUT
******************************************/

/* STL Headers */

#include <iostream>
#include <vector>
#include <math.h>



/* OpenGL Headers */
#include <glew.h>
#pragma comment(lib, "glew32.lib") 

#include <freeglut.h>

#include "model_obj.h"
#include "cube.h"
#include "cone.h"
#include "cylinder.h"
#include "sphere.h"

using namespace std;

double ss = 1.0; // zoom factor

/* Primitives */
SolidCube cube(1.0); // Unit Cube
SolidCone cone(1.0, 1.0, 20, 10);
SolidCylinder cylinder(1.0, 1.0, 20, 10);
SolidSphere sphere(1.0, 100, 100);
Model_OBJ obj;


/* Draw the walls around the scene */
GLfloat walls_ambient[] = { 0.82f, 0.85f, 0.63f, 1.0f };
GLfloat walls_diffuse[] = { 0.82f, 0.85f, 0.63f, 1.0f };
GLfloat walls_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat walls_shininess[] = { 5.0f };
GLfloat floors_ambient[] = { 0.35f, 0.16f, 0.14f, 1.0f };
GLfloat floors_diffuse[] = { 0.35f, 0.16f, 0.14f, 1.0f };
GLfloat floors_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat floors_shininess[] = { 25.0f };
void draw_walls()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, walls_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, walls_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, walls_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, walls_shininess);

    glPushMatrix();
        glTranslated(0.25, 0.5, -1.0);
        glScaled(2.5, 3.0, 0.03);
        cube.render();
    glPopMatrix();
    glPushMatrix();
        glTranslated(-1, 0.5, 0.25);
        glRotated(90, 0, 1, 0);
        glScaled(2.5, 3.0, 0.03);
        cube.render();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, floors_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, floors_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, floors_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, floors_shininess);
    glPushMatrix();
        glTranslated(.25, -1.00, .25);
        glScaled(2.5, 0.03, 2.5);
        cube.render();
    glPopMatrix();
}

// Draw wooden bedside desk
GLfloat wood_ambient[] = { 0.95f, 0.55f, 0.34f, 1.0f };
GLfloat wood_diffuse[] = { 0.7f, 0.55f, 0.28f, 1.0f };
GLfloat wood_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat wood_shininess[] = { 10.0f };
void draw_desk()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, wood_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wood_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wood_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, wood_shininess);

    glPushMatrix();
        glTranslated(0.0, 1.0, 0.0);
        glScaled(0.5, 0.1, 0.5);
        cube.render();
    glPopMatrix();
    
    glPushMatrix();
        glTranslated(0.0, 0.65, 0.0);
        glScaled(0.5, 0.1, 0.5);
        cube.render();
    glPopMatrix();

    glPushMatrix();
        glTranslated(0.2, 0.7, 0.2);
        glScaled(0.05, 0.7, 0.05);
        cube.render();
    glPopMatrix();

    glPushMatrix();
        glTranslated(0.2, 0.7, -0.2);
        glScaled(0.05, 0.7, 0.05);
        cube.render();
    glPopMatrix();

    glPushMatrix();
        glTranslated(-0.2, 0.7, 0.2);
        glScaled(0.05, 0.7, 0.05);
        cube.render();
    glPopMatrix();

    glPushMatrix();
        glTranslated(-0.2, 0.7, -0.2);
        glScaled(0.05, 0.7, 0.05);
        cube.render();
    glPopMatrix();
}

// Draw wooden dresser
void draw_dresser()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, wood_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, wood_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wood_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, wood_shininess);

    glPushMatrix();
        glTranslated(0.0, 0.75, 0.0);
        glScaled(0.85, 0.7, 0.5);
        cube.render();
    glPopMatrix();

    glPushMatrix();
        glTranslated(0.0, 0.9, 0.025);
        glScaled(0.8, 0.25, 0.55);
        cube.render();
    glPopMatrix();

    glPushMatrix();
        glTranslated(0.0, 0.62, 0.025);
        glScaled(0.8, 0.25, 0.55);
        cube.render();
    glPopMatrix();
}

// Draw bed
GLfloat bed_frame_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat bed_frame_diffuse[] = { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat bed_frame_specular[] = { 1.00f, 1.00f, 1.00f, 1.0f };
GLfloat bed_frame_shininess[] = { 30.0f };

GLfloat bed_sheets_ambient[] = { 0.137f, 0.137f, 0.556f, 1.0f };
GLfloat bed_sheets_diffuse[] = { 0.137f, 0.137f, 0.556f, 1.0f };
GLfloat bed_sheets_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat bed_sheets_shininess[] = { 10.0f };

GLfloat bed_pillows_ambient[] = { 0.95f, 0.95f, 0.95f, 1.0f };
GLfloat bed_pillows_diffuse[] = { 0.95f, 0.95f, 0.95f, 1.0f };
GLfloat bed_pillows_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat bed_pillows_shininess[] = { 0.0f };
void draw_bed()
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, bed_frame_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, bed_frame_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, bed_frame_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, bed_frame_shininess);

    glPushMatrix();
        glTranslated(0.0, 0.75, 0.0);
        glScaled(1.15, 0.2, 1.5);
        cube.render();
    glPopMatrix();

    glPushMatrix();
        glTranslated(0.0, 0.85, 0.0);
        glScaled(1.25, 0.05, 1.65);
        cube.render();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, bed_sheets_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, bed_sheets_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, bed_sheets_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, bed_sheets_shininess);
    glPushMatrix();
        glTranslated(0.0, 0.85, 0.15);
        glScaled(1.1, 0.2, 1.);
        cube.render();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, bed_pillows_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, bed_pillows_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, bed_pillows_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, bed_pillows_shininess);
    glPushMatrix();
        glTranslated(-.275, .85, -0.55);
        glScaled(0.53, 0.2, 0.4);
        cube.render();
    glPopMatrix();

    glPushMatrix();
    glTranslated(.275, .85, -0.55);
    glScaled(0.53, 0.2, 0.4);
    cube.render();
    glPopMatrix();
}

// Draw lamp
GLfloat metal_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat metal_diffuse[] = { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat metal_specular[] = { 1.00f, 1.00f, 1.00f, 1.0f };
GLfloat metal_shininess[] = { 30.0f };

GLfloat lamp_shade_diffuse[] = { 1.00f, 1.0f, 1.0f, 1.0f };
GLfloat lamp_shade_specular[] = { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat lamp_shade_ambient[] = { 0.65f, 0.55f, 0.55f, 1.0f };
GLfloat lamp_shade_shininess[] = { 10.0f };
void draw_lamp(void)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, metal_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, metal_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metal_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metal_shininess);

    glPushMatrix();
        glScaled(0.025, 0.25, 0.025);
        glRotated(90, 1, 0, 0);
        cylinder.render();
    glPopMatrix();

    glPushMatrix();
        glScaled(0.125, 0.025, 0.125);
        glRotated(90, 1, 0, 0);
        glTranslated(0.0, 0.0, 10.0);
        cylinder.render();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, lamp_shade_diffuse);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, lamp_shade_specular);
    glMaterialfv(GL_FRONT, GL_SPECULAR, lamp_shade_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, lamp_shade_shininess);

    glPushMatrix();
        glScaled(0.15, 0.5, 0.15);
        glRotated(-90, 1, 0, 0);
        cone.render();
    glPopMatrix();
}

// Draw AND animate fan
float fanAngle = 140.0;
const float TIME_STEP = 30;
const float ANGLE_STEP = 2 * 3.14 / 30;

void draw_fan(void)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, metal_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, metal_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, metal_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, metal_shininess);

    glPushMatrix();
    glScaled(0.05, 1.5, 0.05);
    glRotated(90, 1, 0, 0);
    glTranslated(0.0, 0.0, -0.5);

    cylinder.render();
    glPopMatrix();

    glPushMatrix();
    glScaled(0.125, 0.025, 0.125);
    glRotated(90, 1, 0, 0);
    glTranslated(0.0, 0.0, 10.0);
    cylinder.render();
    glPopMatrix();

    glPushMatrix();
        glScaled(0.05, 0.05, 0.05);
        glRotated(-90, 1, 0, 0);
        glTranslated(0.0, 0.0, 15.0);
        sphere.render();
    glPopMatrix();

    glPushMatrix();
    glScaled(0.075, 0.075, 0.075);
    glRotated(-90, 1, 0, 0);
    glTranslated(1., 0.0, 10.0);
    sphere.render();
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, floors_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, floors_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, floors_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, floors_shininess);

    glPushMatrix();
        glPushMatrix();
        glTranslated(.1, 0.75, 0);
        glRotated(fanAngle += 5, 1, 0, 0);
        glScaled(0.05, 0.15, 0.85);
        cube.render();
    glPopMatrix();

    glPushMatrix();
        glTranslated(.1, 0.75, 0);
        glRotated(fanAngle++ + 90, 1, 0, 0);
        glScaled(0.05, 0.15, 0.85);
        cube.render();
    glPopMatrix();

}

// Draw rug
GLfloat rug_ambient[] = { 0.65f, 0.65f, 0.65f, 1.0f };
GLfloat rug_diffuse[] = { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat rug_specular[] = { 1.00f, 1.00f, 1.00f, 1.0f };
GLfloat rug_shininess[] = { 30.0f };
void draw_rug(void)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, rug_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, rug_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, rug_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, rug_shininess);

    glPushMatrix();
        glRotated(90, 1, 0, 0);
        glScaled(1.15, 1, .025);
        glTranslated(.15, 0, 39.38);
        cylinder.render();
    glPopMatrix();
}

// DRAWS THE CURRENT SCENEGRAPH
void draw_scene(void)
{
    draw_walls();

    glPushMatrix();
        glTranslated(0.5, -1.325, -0.7);
        glScaled(.8, 0.8, 0.8);
        draw_desk();
    glPopMatrix();

    glPushMatrix();
        glTranslated(1.15, -1.325, -0.70);
        glScaled(.8, 0.8, 0.8);
        draw_dresser();
    glPopMatrix();

    glPushMatrix();
        glTranslated(-.45, -1.5, -0.30);
        glScaled(.8, 0.8, 0.8);
        draw_bed();
    glPopMatrix();

    glPushMatrix();
        glTranslated(1.3, -0.15, -.65);
        draw_lamp();
    glPopMatrix();

    glPushMatrix();
        glTranslated(-0.85, -0.725, 1.0);
        draw_fan();
    glPopMatrix();

    glPushMatrix();
        draw_rug();
    glPopMatrix();

    // Draw bunny obj
    glMaterialfv(GL_FRONT, GL_AMBIENT, floors_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, floors_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, floors_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, floors_shininess);
    glPushMatrix();
        glTranslated(0.55, -.45, -0.7);
        glScaled(.125, .125, .125);
        obj.Draw();
    glPopMatrix();
}

// GLUT DISPLAY FUNCTION
void display_solid(void)
{
    // set the light source properties
    GLfloat lightIntensity[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat light_position[] = { 0.0f, 0.8f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);

    GLfloat ceilingIntensity[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat ceiling_light_position[] = { 0.0f, 6.0f, 0.0f, 0.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, ceiling_light_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, ceilingIntensity);

    glMatrixMode(GL_MODELVIEW);      // position and aim the camera
    glLoadIdentity();

    gluLookAt(4, 2, 4, 0.0, 0.3, 0.0, 0.0, 1.0, 0.0);

    //
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double winHt = 1.0;  //half-height of the window
    gluPerspective(ss*45, 640 / 480, 1.0, 500.0);

    // start drawing
    glMatrixMode(GL_MODELVIEW);      // position and aim the camera
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // clear the screen
    //

    glPushMatrix();
        glScaled(0.85, 0.85, 0.85);
        draw_scene();
    glPopMatrix();  

    //
    glutPostRedisplay();
    glFlush();
    glutSwapBuffers(); // display the scene just made
}

// Handle mouse input for zooming
void my_mouse(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            ss = ss * 0.9;
            glutPostRedisplay(); // draw it again
        }
        break;
    case GLUT_MIDDLE_BUTTON:
        if (state == GLUT_DOWN) {
            ss = ss * 1.1;
            glutPostRedisplay(); // draw it again
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
            exit(-1);
        break;
    default:
        break;
    }
}

GLint W = 640;
GLint H = 480;

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    W = w;
    H = h;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(W, H);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Brennen Green - Project 2");
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glutDisplayFunc(display_solid);
    glutMouseFunc(my_mouse);
    glutReshapeFunc(myReshape);
    glViewport(0, 0, W, H);
    obj.Load((char*)"bunny.obj");
    glutMainLoop();
}