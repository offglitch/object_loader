#include <stdio.h>
#include <GL/freeglut.h>

#include "model.h"
#include "arcball.h"

// SCREEN parameters
int SCR_WIDTH = 600, SCR_HEIGHT = 400;

// Object
unsigned int obj_id = 0;
glm::vec3 obj_scale(1.0f), obj_scale_factor(0.04f);

//float position[4] = { 20.0f, 20.0f, 20.0f, 1.0f };
float globalAmbient[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// Arcball
float arcballSpeed = 0.2f;
Arcball modelArcBall(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
glm::mat4 arcballMatrix;

void display(void) {
    // Clear color buffer, depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Prepare the perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01, 1000.0f);
    gluLookAt(0.0f, 0.0f, 4.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw Objects
    glPushMatrix();
        // Load Rotate matrix
        arcballMatrix = modelArcBall.createRotationMatrix();
        glLoadMatrixf(glm::value_ptr(arcballMatrix));
        // Scale object
        glScalef(obj_scale.x, obj_scale.y, obj_scale.z);
        glCallList(obj_id);
    glPopMatrix();

    //printf("display flush\n");

    glFlush();
}

void init() {
    glClearColor(0.03125f, 0.046875f, 0.207031f, 1.0f);
    // Enable depth
    glEnable(GL_DEPTH_TEST);
    
    // Enable gpu nomaization of surface normals
    glEnable(GL_NORMALIZE);
    
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Light ing parameters
    //glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

}

void onMouseClick(int button, int state, int x, int y) {
    //printf("(%d %d %d, %d)\n", button, state, x, y);
    // Init or end mouse drag operation
    modelArcBall.mouseButtonCallback(button, state, x, y);
}

void onMouseMotion(int x, int y) {
    // Process drag operation
    modelArcBall.cursorCallback(x, y);
    glutPostRedisplay();
}

void onMouseScroll(int button, int dir, int x, int y) {
    if (dir > 0) {
        // Scale up
        obj_scale.x += obj_scale_factor.x;
        obj_scale.y += obj_scale_factor.y;
        obj_scale.z += obj_scale_factor.z;
        glutPostRedisplay();
    }
    else if (obj_scale.x > obj_scale_factor.x) {
        // Scale down
        obj_scale.x -= obj_scale_factor.x;
        obj_scale.y -= obj_scale_factor.y;
        obj_scale.z -= obj_scale_factor.z;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    std::string path;

    if (argc == 2) {
        // file path given as command line argument
        path.copy(argv[1], strlen(argv[1]));
    } else {
        // default path
        path.clear();
        path.append("res/models/square/square-normalize-normals.obj");
    }

    // Init glut Window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(SCR_WIDTH, SCR_HEIGHT);
    glutCreateWindow("Convex Hull");

    // Init OpenGL parameters
    init();

    // Load the Model
    obj_id = loadModel(path);

    if (obj_id == 0) {
        printf("Error loading object!\n");
        return 0;
    }
    
    // Register callbacks
    glutDisplayFunc(display);
    glutMouseFunc(onMouseClick);
    glutMotionFunc(onMouseMotion);
    glutMouseWheelFunc(onMouseScroll);

    // Enter into render loop
    glutMainLoop();
    return 0;
}