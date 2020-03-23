#include <chrono>
#include "main.h"
std::vector<std::string> split (const std::string &s, char delim);
Color_t colorAdd(Color_t a, Color_t b);

#ifndef MUSEUMVERLICHTING_TOOLS_H
#define MUSEUMVERLICHTING_TOOLS_H

class Pulse {
    int delay;
    int size;
    bool direction;
    Color_t pcolor;
    unsigned long lastDrawn; //or, calls since last redraw
    int phase;
    int location;
    float* profile;
    Color_t state[NUM_COLS];
    
  public:
    bool finished;
    Pulse(int, int, bool, Color_t, float*);
    Color_t* draw();
};

//TODO: look into defining it using a template
struct node {
    Pulse* pulse;
    int row;
    
    node* next;
    node* prev;
};

#endif
