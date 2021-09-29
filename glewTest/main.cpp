/* STL Headers */
#include <iostream>
#include <vector>
#include <math.h>


#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

/* OpenGL Headers */
#include <glew.h>
#pragma comment(lib, "glew32.lib") 

#include <freeglut.h>

using namespace std;

struct Point2D
{
    GLfloat x;
    GLfloat y;
    Point2D() { x = 0; y = 0; }
    Point2D(GLfloat xc, GLfloat yc)
    {
        x = xc;
        y = yc;
    }
};

/* Geometrical/Numerical Constants */
const GLint64 WIDTH = 640-1;
const GLint64 HEIGHT = 480-1;
const GLfloat PI = 3.14159265;
const GLfloat VELOCITY_DAMP = 100; 
const GLfloat ANGULAR_DAMP = 2800;

/* Animation Parameters */
GLfloat theta;
Point2D directionVector; 
Point2D originalDirection; 
GLfloat directionSpeed;
GLint rotationSpeed;
GLboolean showPoly;
GLboolean animating;
GLint inputState;
Point2D dirVecBufInit;
Point2D dirVecBufFinal;
Point2D lastMousePos;

/* Polygon Storage */
std::vector<Point2D> poly; // Initialize our polygon
std::vector<Point2D> clipping;
std::vector<Point2D> clipped;
std::vector<std::vector<Point2D>> clippedFinal;
std::vector<Point2D> intersections;
std::vector<int> interDirections;
std::vector<Point2D> rotatedPoly;
std::vector<Point2D> midpoints; // Initialize our polygon's midpoints
Point2D centroid; // Polygon centroid

/* GUI Controls */
static bool controlsActive = true;
ImGuiWindowFlags windowFlags = 
    ImGuiWindowFlags_NoBackground 
    | ImGuiWindowFlags_NoTitleBar 
    | ImGuiWindowFlags_NoResize
    | ImGuiWindowFlags_NoCollapse
    | ImGuiWindowFlags_NoDecoration
    | ImGuiWindowFlags_AlwaysAutoResize
    | ImGuiWindowFlags_NoMove;


/* Initialize ImGui Context */
void imGuiInit(void) 
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();
}

/* Initialize OpenGL and Polygon */
void init(void) 
{
    imGuiInit();

    /* Setup Frame and Drawing */
    glClearColor(1, 1, 1, 1);
    glLineWidth(3.0);
    glPointSize(10.0f);
    glColor3f(0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH+1, 0, HEIGHT+1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    /* Load Polygon Initial Points */
    showPoly = false;
    animating = false;
    originalDirection = Point2D();
    directionVector = Point2D();
    directionSpeed = 1;
    rotationSpeed = 0;
    theta = 0;
    inputState = -1;
    Point2D dirVecBufInit = Point2D();
    Point2D dirVecBufFinal = Point2D();

    centroid = Point2D(WIDTH / 2, HEIGHT / 2);
    GLfloat unit = HEIGHT / 15;
    poly.push_back(Point2D(-1 * unit, 2 * unit));
    poly.push_back(Point2D(-3 * unit, 2 * unit));
    poly.push_back(Point2D(-3 * unit, -3 * unit));
    poly.push_back(Point2D(3 * unit, -3 * unit));
    poly.push_back(Point2D(3 * unit, 2 * unit));
    poly.push_back(Point2D(1 * unit, 2 * unit));
    poly.push_back(Point2D(1 * unit, -1 * unit));
    poly.push_back(Point2D(-1 * unit, -1 * unit));
    
}

/* Field midpoint buffer with midpoints from specified polygon */
void calcMidpoints(std::vector<Point2D> polygon) 
{
    midpoints.clear();
    for (GLuint i = 0; i < polygon.size(); i++) {
        GLfloat midX = (polygon.at(i).x + polygon.at((i+1)%polygon.size()).x) / 2;
        GLfloat midY = (polygon.at(i).y + polygon.at((i+1)%polygon.size()).y) / 2;
        midpoints.push_back(Point2D(midX, midY));
    }
}

/* Calculate a degree two Bezier curve */
Point2D bezierCurveD2(Point2D A, Point2D B, Point2D C, GLfloat t) 
{
    GLfloat px; GLfloat py;
    px = std::pow(1-t, 2)*A.x + 2*t*(1-t)*B.x + std::pow(t, 2)*C.x;
    py = std::pow(1-t, 2)*A.y + 2*t*(1-t)*B.y + std::pow(t, 2)*C.y;
    return Point2D(px, py);
}

Point2D vectorDirection(Point2D A, Point2D B) 
{
    Point2D mA = Point2D(A.x / ((WIDTH/2) - 1.0), -(A.y / ((HEIGHT/2) - 1.0)));
    Point2D mB = Point2D(B.x / ((WIDTH / 2) - 1.0), -(B.y / ((HEIGHT / 2) - 1.0)));
    return Point2D(A.x - B.x, B.y - A.y);
}

Point2D normalizeVector(Point2D v) 
{
    GLint mag = std::sqrt(v.x * v.x + v.y * v.y);
    return Point2D(v.x / mag, v.y / mag);
}

int distance(Point2D A, Point2D B) 
{
    return std::sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
}

/* Calculate the intersection between Line (A, B) with the viewing bound */
Point2D findIntersection(Point2D A, Point2D B) 
{
    Point2D C, D;
    GLfloat t = -1;
    if ((B.x < 0 && A.x >= 0) || (A.x < 0 && B.x >= 0))//Left clipping
    {
        C = Point2D(0, 0);
        D = Point2D(0, HEIGHT);
    }
    else if ((B.x > WIDTH && A.x <= WIDTH)
        || (A.x > WIDTH && B.x <= WIDTH))//Right clipping
    {
        C = Point2D(WIDTH, HEIGHT);
        D = Point2D(WIDTH, 0);
    }
    else if ((B.y < 0 && A.y >= 0) || (A.y < 0 && B.y >= 0))//Bottom clipping
    {
        C = Point2D(WIDTH, 0);
        D = Point2D(0, 0);
    }
    else if ((B.y > (HEIGHT) && A.y <= (HEIGHT))
        || (A.y > (HEIGHT) && B.y <= (HEIGHT)))//Top clipping
    {
        C = Point2D(0, HEIGHT);
        D = Point2D(WIDTH, HEIGHT);
    }
    else
        return Point2D(99999, 99999);//No intersection

    GLfloat denom = (D.y - C.y) * (B.x - A.x) - (D.x - C.x) * (B.y - A.y);
    if (denom != 0)
        t = ((D.x - C.x) * (A.y - C.y) - (D.y - C.y) * (A.x - C.x)) / denom;

    GLfloat interX = 99999, interY = 99999;
    if (t >= 0 && t <= 1)
    {
        interX = (1 - t) * A.x + t * B.x;
        interY = (1 - t) * A.y + t * B.y;
        //std::cout << " Inter:" << interX << "," << interY << std::endl;
    }
    return Point2D((int)interX, (int)interY);
}


/* Find Intersection Direction (Entering or Exiting) */
int findDirection(Point2D A, Point2D B) {
    GLboolean isAInside = false; GLboolean isBInside = false;

    if (A.x <= WIDTH && A.y <= HEIGHT && A.x >= 0 && A.y >= 0) {
        isAInside = true;
    }

    if (B.x <= WIDTH && B.y <= HEIGHT && B.x >= 0 && B.y >= 0) {
        isBInside = true;
    }

    if (isAInside && !isBInside)
    {
        return 0;
    }

    if (!isAInside && isBInside)
    {
        return 1;
    }


    return -1;
}

/* Implements Weiler-Atherton Polygon Clipping Algorithm */
void clip() 
{
    clipping.clear();
    clipped.clear();
    clippedFinal.clear();
    intersections.clear();
    interDirections.clear();

    std::vector<Point2D> interA;
    std::vector<int> interADir;
    std::vector<Point2D> interB;
    std::vector<int> interBDir;
    std::vector<Point2D> interC;
    std::vector<int> interCDir;
    std::vector<Point2D> interD;
    std::vector<int> interDDir;

    GLboolean interFound = false;

    for (int i = 0; i <= rotatedPoly.size(); i++) {
        Point2D A = rotatedPoly.at(i % rotatedPoly.size()); Point2D B =  rotatedPoly.at((i + 1) % rotatedPoly.size());
        Point2D inter = findIntersection(A, B);
        int interDir = -1;
        if (inter.x != 99999 && inter.y != 99999) {
            interDir = findDirection(A, B);

            if (inter.y == HEIGHT) {
                interA.push_back(inter);
                interADir.push_back(interDir);
            }
            else if (inter.x == WIDTH ) {
                interB.push_back(inter);
                interBDir.push_back(interDir);
            }
            else if (inter.y <= 2 && inter.y >= -2) {
                interC.push_back(inter);
                interCDir.push_back(interDir);
            }
            else if (inter.x <= 2 && inter.x >= -2) {
                interD.push_back(inter);
                interDDir.push_back(interDir);
            }

            // PLOT INTERSECTIONS DEBUG
            /*glPointSize(25.0f);
            if (interDir == 1)
            {
                glColor3f(0, 1, 0);
            }
            else
            {
                glColor3f(0, 0, 1);
            }

            glBegin(GL_POINTS);
            glVertex2f(inter.x, inter.y);
            glEnd();

            glPointSize(10.0f);
            glColor3f(1, 0, 0);*/
        }
        intersections.push_back(findIntersection(A, B));
        interDirections.push_back(interDir);
    }

    std::vector<int> clippingInterDir;
    std::vector<int> clippedInterDir;

    for (int i = 0; i <= rotatedPoly.size(); i++) {
        Point2D inter = intersections.at(i % intersections.size());
        GLboolean interDir = interDirections.at(i % interDirections.size());

        if (interDir == 1 || interDir == 0) // Entering or Exiting
        {
            clipped.push_back(rotatedPoly.at(i % rotatedPoly.size()));
            clippedInterDir.push_back(-1);
            clipped.push_back(inter);
            clippedInterDir.push_back(interDir);
            interFound = true;
        }
        else
        {
            clipped.push_back(rotatedPoly.at(i % rotatedPoly.size()));
            clippedInterDir.push_back(-1);
        }

    }

    if (interFound)
    {
        // A
        clipping.push_back(Point2D(0, HEIGHT - 1));
        clippingInterDir.push_back(-1);
        for (int i = 0; i < interA.size(); i++)
        {
            clipping.push_back(interA.at(i));
            clippingInterDir.push_back(interADir.at(i));
        }

        // B
        clipping.push_back(Point2D(WIDTH - 1, HEIGHT - 1));
        clippingInterDir.push_back(-1);
        for (int i = 0; i < interB.size(); i++)
        {
            clipping.push_back(interB.at(i));
            clippingInterDir.push_back(interBDir.at(i));
        }

        // C
        clipping.push_back(Point2D(WIDTH - 1, 0));
        clippingInterDir.push_back(-1);
        for (int i = 0; i < interC.size(); i++)
        {
            clipping.push_back(interC.at(i));
            clippingInterDir.push_back(interCDir.at(i));
        }

        // D
        clipping.push_back(Point2D(0, 0));
        clippingInterDir.push_back(-1);
        for (int i = 0; i < interD.size(); i++)
        {
            clipping.push_back(interD.at(i));
            clippingInterDir.push_back(interDDir.at(i));
        }

        for (int i = 0; i <= clipped.size(); i++) {
            if (clippedInterDir.at(i % clippedInterDir.size()) == 1)
            {
                clippedFinal.push_back(std::vector<Point2D>());

                Point2D entering = clipped.at(i % clipped.size());
                int j = i;
                while (clippedInterDir.at(j % clippedInterDir.size()) != 0) {
                    clippedFinal.back().push_back(clipped.at(j % clipped.size()));
                    j += 1;
                }
                clippedFinal.back().push_back(clipped.at(j % clipped.size()));

                Point2D exiting = clipped.at(j % clipped.size());
                j = 0;
                while (clipping.at(j % clipping.size()).x != exiting.x && clipping.at(j % clipping.size()).y != exiting.y)
                {
                    j += 1;
                }


                while (clipping.at(j % clipping.size()).x != entering.x && clipping.at(j % clipping.size()).y != entering.y)
                {
                    Point2D thisPoint = clipping.at(j % clipping.size());
                    GLboolean duplicate = false;
                    for (auto polygon : clippedFinal)
                    {
                        for (auto point : clippedFinal.back())
                        {
                            if (point.x == thisPoint.x && point.y == thisPoint.y)
                            {
                                duplicate = true;
                                break;
                            }
                        }
                    }

                    if (!duplicate && clippingInterDir[j % clippingInterDir.size()] != -1)
                    {
                        clippedFinal.back().push_back(thisPoint);
                    }
                    else if (!duplicate && clippingInterDir[j % clippingInterDir.size()] == -1)
                    {
                        if (distance(thisPoint, centroid) < 100)
                        {
                            clippedFinal.back().push_back(thisPoint);
                        }
                    }
                    j += 1;
                }

            }
        }
    }
    else
    {
        clippedFinal.push_back(rotatedPoly);
    }
    
}

/* Draw a polygon from a list of points */
void drawPolygon()
{
    // Bounce back if outside of bounds
    if (centroid.x <= 0 || centroid.x >= WIDTH) {
        directionVector.x *= -1;
    }
    if (centroid.y <= 0 || centroid.y >= HEIGHT) {
        directionVector.y *= -1;
    }

    // Rotate poly by theta around centroid
    GLfloat newTheta = theta * (rotationSpeed / (ANGULAR_DAMP));
    rotatedPoly.clear();
    for (GLuint n = 0; n < poly.size(); n++) {
        Point2D thisVertex = poly.at(n);
        Point2D rotated = Point2D(
            thisVertex.x * std::cos(newTheta) + thisVertex.y * std::sin(newTheta),
            thisVertex.x * -std::sin(newTheta) + thisVertex.y * std::cos(newTheta)
        );

        rotatedPoly.push_back(Point2D(rotated.x + centroid.x, rotated.y + centroid.y));
    }

    clip();
    
    // DRAW BOUNDS DEBUG
    //for (auto thisPoly : clippedFinal) {
    //    glBegin(GL_LINE_STRIP);
    //    for (GLint i = 0; i <= thisPoly.size(); i++) {
    //        GLfloat newX = thisPoly.at(i % thisPoly.size()).x;
    //        GLfloat newY = thisPoly.at(i % thisPoly.size()).y;
    //        glVertex2f(newX, newY);
    //    }
    //    glEnd();
    //}

    for (auto thisPoly : clippedFinal) {
        calcMidpoints(thisPoly);

        glBegin(GL_LINE_STRIP);
        for (GLuint k = 0; k < midpoints.size(); k++)
        {
            Point2D thisVertex = Point2D(midpoints.at(k % midpoints.size()).x, midpoints.at(k % midpoints.size()).y);

            Point2D A = thisVertex;

            for (GLfloat t = 0; t <= 1; t += 0.01)
            {
                Point2D B = Point2D(thisPoly.at((k + 1) % thisPoly.size()).x, thisPoly.at((k + 1) % thisPoly.size()).y);
                Point2D C = Point2D(midpoints.at((k + 1) % midpoints.size()).x, midpoints.at((k + 1) % midpoints.size()).y);
                Point2D newVertex = bezierCurveD2(A, B, C, t);
                glColor3f(0, 0, 0);
                glVertex2f(newVertex.x, newVertex.y);
                thisVertex = newVertex;
            }
        }
        glEnd();
    }

    // DRAW CENTROID DEBUG
   /* glBegin(GL_POINTS);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(centroid.x, centroid.y);
    glEnd();*/
}

/* accepts animation input */
void acceptMouseInput(int button, int state, int x, int y) {
    //std::cout << button << " " << state << " (" << x << "," << y << ")" << std::endl;

    if (state && dirVecBufInit.x == Point2D().x && dirVecBufInit.y == Point2D().y) {
        //std::cout << "set 0" << std::endl;
        dirVecBufInit = Point2D(x, y);
    }
    else if (state) {
        dirVecBufFinal = Point2D(x, y);

        //std::cout << "(" << dirVecBufInit.x << "," << dirVecBufInit.y << ")";
        //std::cout << "(" << dirVecBufFinal.x << "," << dirVecBufFinal.y << ")";
        //std::cout << std::endl;

        switch (inputState) {
        case 1:
            // Calculate vector for direction and distance for speed
            //std::cout << "Setting direction and speed" << std::endl;
            directionVector = normalizeVector(vectorDirection(dirVecBufFinal, dirVecBufInit));
            directionSpeed = distance(dirVecBufFinal, dirVecBufInit);
            inputState++;
            break;
        case 2:
            // Calculate magnitutde for rotation speed
            //std::cout << "Setting rotation speed" << std::endl;
            rotationSpeed = distance(dirVecBufFinal, dirVecBufInit);
            inputState = 0;
            glutMouseFunc(ImGui_ImplGLUT_MouseFunc);
            glutPassiveMotionFunc(ImGui_ImplGLUT_MotionFunc);
            break;
        default:
            break;
        }
        dirVecBufFinal = Point2D(); dirVecBufInit = Point2D();
    }
}

void passiveMotion(int x, int y) {
    lastMousePos = Point2D(x, y);
}

/* Handles ImGui Drawing Code */
/**********************
 INPUT STATE MACHINE

 State Status
 -1    Uninitialized (Needs Input)
 0     Input Accepted, Ready to Animation
 1     Speed and Direction Accepted (Needs Rotation Speed)
 2     Speed, Direction, and Rotation Speed Accepted
 3     Animating
***********************/
void drawGui(void) 
{
    ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT / 6));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::Begin("Toolbar", &controlsActive, windowFlags);
    ImGui::SetNextItemWidth(0.25 * ImGui::GetWindowWidth());
    if (ImGui::Button("Draw"))  // Draw Pressed
    {
        showPoly = true;
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(600);

    if (ImGui::Button("Input Velocity / Rotation Speed")) // Input Pressed
    {
        if (inputState == -1 && showPoly) {
            inputState = 1;
            glutMouseFunc(acceptMouseInput);
            glutPassiveMotionFunc(passiveMotion);
        }
    } 
    ImGui::SameLine();

    if (ImGui::Button("Start")) // Start Animation
    {
        if (inputState == 0 && showPoly) {
            inputState = 3;
        } // If directionVector is at initilization state then wait for input
    }
    ImGui::SameLine();

    if (ImGui::Button("Stop")) // Stop Animation
    {
        if (showPoly && inputState == 3) {
            inputState = -1;
            directionVector = originalDirection;
            directionSpeed = 1;
            rotationSpeed = 0;
            theta = 0;
            centroid = Point2D(WIDTH / 2, HEIGHT / 2);
        }
    }

    ImGui::Separator();
    ImGui::End();
}

/* Draws text on screen if necessary */
void drawAlertText() {
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos2f(WIDTH / 4, HEIGHT - (HEIGHT / 4));
    switch (inputState) {
    case 1:
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Please input a vector for direction and speed.");
        break;
    case 2:
        glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)"Please input a vector for rotation speed.");
        break;
    default:
        break;
    }

}

/* Display Loop */
void displayMe(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    drawGui();
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    // Draw Polygon
    if (showPoly) drawPolygon();
    if (inputState == 1 && dirVecBufInit.x != Point2D().x && dirVecBufInit.y != Point2D().y) {
        glBegin(GL_LINE_STRIP);
        glVertex2f(dirVecBufInit.x, HEIGHT - dirVecBufInit.y);
        glVertex2f(lastMousePos.x, HEIGHT - lastMousePos.y);
        glEnd();
    } else if (inputState == 2 && dirVecBufInit.x != Point2D().x && dirVecBufInit.y != Point2D().y) {
        glBegin(GL_LINE_STRIP);
        glVertex2f(dirVecBufInit.x, HEIGHT - dirVecBufInit.y);
        glVertex2f(lastMousePos.x, HEIGHT - lastMousePos.y);
        glEnd();
    }
    drawAlertText();

    glutSwapBuffers();
}

/* Animation Frame Step */
void animation(void) 
{
    if (inputState == 3) 
    {
        theta += 1 / (2 * PI);
        centroid.x += directionSpeed * directionVector.x / VELOCITY_DAMP;
        centroid.y += directionSpeed * directionVector.y / VELOCITY_DAMP;
    }


    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH+1, HEIGHT+1);
    glutInitWindowPosition(WIDTH/2, HEIGHT/2);
    glutCreateWindow("Project 1 - Brennen Green");
    init();

    glutIdleFunc(animation);
    glutDisplayFunc(displayMe);

    glutMainLoop();

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
    return 0;
}