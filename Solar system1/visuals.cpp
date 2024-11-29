#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <GL/freeglut.h>
#include "visuals.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

bool one = false;
static float tx = 0.0;
static float rotx = 0.0;
static float worldX = 15.0;
static float worldY = 0.0;
static float scaleFactor = 0.6;
static bool animate = false;
point stars[500];

int globalW, globalH;
GLuint moonTexture; // Текстура для Місяця

// Масив для швидкості обертання планет навколо Сонця
static float orbitSpeeds[] = { 2.0, 0.8, 0.5, 0.4, 0.1, 0.05, 0.025, 0.015, 10.0 }; 

// Масив для швидкості обертання планет навколо своєї осі
static float axisSpeeds[] = { 10.0, 8.0, 15.0, 12.0, 5.0, 4.0, 3.0, 2.0 };

// Кути орбіт і осей
static float orbitAngles[8] = { 0.0 };
static float axisAngles[8] = { 0.0 };

// Текстури
GLuint sunTexture, mercuryTexture, venusTexture, earthTexture, marsTexture, jupiterTexture, saturnTexture, uranusTexture, neptuneTexture;

GLuint LoadTexture(const char* filename) {
    GLuint texture;
    int width, height;
    unsigned char* data;
    FILE* file;
    if (fopen_s(&file, filename, "rb") != 0 || file == NULL) {
        return 0;
    }

    fseek(file, 18, SEEK_SET);
    fread(&width, 4, 1, file);
    fread(&height, 4, 1, file);
    fseek(file, 54, SEEK_SET);

    data = (unsigned char*)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);
    fclose(file);

    for (int i = 0; i < width * height * 3; i += 3) {
        unsigned char tmp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = tmp;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    free(data);
    return texture;
}

void DrawOrbit(float radius) {
    glDisable(GL_LIGHTING);
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float theta = i * M_PI / 180.0f;
        glVertex3f(radius * cos(theta), 0.0, radius * sin(theta));
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void keimeno(const char* str, float size) {
    glPushMatrix();
    glScalef(size, size, size);
    for (int i = 0; i < strlen(str); i++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    glPopMatrix();
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0, 0, -100);

    if (!animate) {
        glPushMatrix();
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glColor3f(1, 1, 1);
        glTranslatef(-8, 50, 0.0);
        keimeno("Pause", 0.05f);
        glPopMatrix();
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricDrawStyle(quadric, GLU_FILL);
    glScalef(scaleFactor, scaleFactor, scaleFactor);
    glRotatef(worldX, 1, 0, 0);
    glRotatef(worldY, 0, 1, 0);

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sunTexture);
    glColor3f(1, 1, 1);
    gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, 15, 36, 18);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // Відстані орбіт та розміри планет
    float distances[] = { 18, 30, 45, 60, 80, 100, 120, 140 };
    float radii[] = { 1.5, 3.5, 4.0, 3.0, 10.0, 9.0, 8.0, 7.0 };
    GLuint textures[] = { mercuryTexture, venusTexture, earthTexture, marsTexture, jupiterTexture, saturnTexture, uranusTexture, neptuneTexture };

    // Орбіти та планети
    for (int i = 0; i < 8; i++) {
        DrawOrbit(distances[i]); 

        glPushMatrix();
        glRotatef(orbitAngles[i], 0, 1, 0); // Обертання навколо Сонця
        glTranslatef(distances[i], 0, 0);  

        glPushMatrix();
        glRotatef(axisAngles[i], 0, 1, 0); // Обертання планети навколо своєї осі
        glRotatef(270, 1, 0, 0);           

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        gluQuadricTexture(quadric, GL_TRUE);
        glColor3f(1, 1, 1);
        gluSphere(quadric, radii[i], 36, 18); // Малюємо планету
        glDisable(GL_TEXTURE_2D);

        glPopMatrix(); // Вихід з локального обертання планети
        glPopMatrix(); // Вихід з обертання навколо Сонця
    }
    glPushMatrix();
    glRotatef(orbitAngles[2], 0, 1, 0);  // Земля рухається навколо Сонця
    glTranslatef(45, 0, 0);             // Переміщення на орбіту Землі

    glPushMatrix();
    glRotatef(axisAngles[2], 0, 1, 0);  // Обертання Землі навколо осі
    glRotatef(270, 1, 0, 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, 4.0, 36, 18);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    DrawOrbit(7.0f);  // Радіус орбіти Місяця
    glPushMatrix();
    glRotatef(-orbitAngles[8], 0, 1, 0); 
    glTranslatef(7.0, 0, 0);    

    glRotatef(270, 1, 0, 0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, moonTexture);
    gluQuadricTexture(quadric, GL_TRUE);
    gluSphere(quadric, 1.0, 36, 18);  
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glPopMatrix(); 
    gluDeleteQuadric(quadric);
    DrawStars();
    glutSwapBuffers();
}

void Resize(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    globalH = h;
    globalW = w;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, (float)w / (float)h, 1.0, 1000.0);
}

void Idle() {
    if (animate) {
        for (int i = 0; i < 8; i++) {
            orbitAngles[i] += orbitSpeeds[i];
            axisAngles[i] += axisSpeeds[i];
            if (orbitAngles[i] >= 360.0) orbitAngles[i] -= 360.0;
            if (axisAngles[i] >= 360.0) axisAngles[i] -= 360.0;
        }
    }
    glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'q': exit(0); break;
    case 'w': if (animate) worldX -= 1.0f; break;
    case 's': if (animate) worldX += 1.0f; break;
    case 'a': if (animate) worldY -= 1.0f; break;
    case 'd': if (animate) worldY += 1.0f; break;
    case ' ': animate = !animate; break;
    default: break;
    }
    glutPostRedisplay();
}

void MouseWheel(int button, int dir, int x, int y) {
    if (animate) {
        if (dir > 0) {
            scaleFactor += 0.01;
        }
        else {
            if (scaleFactor > 0.08) {
                scaleFactor -= 0.01;
            }
        }
        glutPostRedisplay();
    }
}

void Setup() {
    sunTexture = LoadTexture("sun.bmp");
    mercuryTexture = LoadTexture("mercurio.bmp");
    venusTexture = LoadTexture("venus.bmp");
    earthTexture = LoadTexture("terra.bmp");
    marsTexture = LoadTexture("marte.bmp");
    jupiterTexture = LoadTexture("jupiter.bmp");
    saturnTexture = LoadTexture("saturno.bmp");
    uranusTexture = LoadTexture("urano.bmp");
    neptuneTexture = LoadTexture("neptuno.bmp");
    moonTexture = LoadTexture("lua.bmp");


    for (int i = 0; i < 500; i++) {
        RandomCoordinates(&stars[i]);
    }
    srand(time(0));

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void RandomCoordinates(point* star) {
    int lowest = -1000, highest = 1000;
    int range = (highest - lowest) + 1;
    star->x = lowest + int(range * rand() / (RAND_MAX + 1.0));
    star->y = lowest + int(range * rand() / (RAND_MAX + 1.0));
    star->z = lowest + int(range * rand() / (RAND_MAX + 1.0));
}

void DrawStars() {
    glDisable(GL_LIGHTING);
    glColor3f(1, 1, 1);
    for (int i = 0; i < 500; i++) {
        glPushMatrix();
        glTranslatef(stars[i].x, stars[i].y, stars[i].z);
        glutSolidSphere(0.5, 10, 10);
        glPopMatrix();
    }
    glEnable(GL_LIGHTING);
}
