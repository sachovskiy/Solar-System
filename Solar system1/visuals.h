#pragma once

struct point
{
    float x;
    float y;
    float z;
};

struct face
{
    int vtx[3];
};

extern int globalW, globalH;

//-------- Functions --------------------------------
void Render();
void Resize(int w, int h);
void Setup();
void Idle();
void Keyboard(unsigned char key, int x, int y);
void MouseWheel(int button, int dir, int x, int y);
void RandomCoordinates(point*);
void DrawStars();
void keimeno(const char* str, float size);
