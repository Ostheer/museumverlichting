#include <sstream>
#include <vector>
#include <iostream>

#include "ws2812/ws2812-rpi.h"
#include "tools.h"

using namespace std;

vector<string> split (const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

Color_t colorAdd(Color_t a, Color_t b){
    Color_t r;
    
    int sr = a.r + b.r;
    int sg = a.g + b.g;
    int sb = a.b + b.b;
    
    int max = sr;
    if (sg > max) max = sg;
    if (sb > max) max = sb;
    
    if (max > 255){
        sr = sr*255/max;
        sg = sg*255/max;
        sb = sb*255/max;
    }
    
    r.r = sr;
    r.g = sg;
    r.b = sb;
    
    return r;
}

Pulse::Pulse (int a, int b, bool c, Color_t d, float* e) {
  delay = a;
  size = b;
  direction = c;
  pcolor = d;
  profile = e;
  lastDrawn = 0;
  phase = 0;
  location = 0;
  finished = false;
}

Color_t* Pulse::draw() {
    unsigned long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    
    if (time - lastDrawn > delay){
        for (int p = 0; p < size; p++){
            location = phase - size + p;
            if (location >= 0 && location < NUM_COLS){
                Color_t color(profile[p]*pcolor.r, profile[p]*pcolor.g, profile[p]*pcolor.b);
                switch (direction){
                    case true: state[NUM_COLS-1 - location] = color; break;
                    case false: state[location] = color; break;
                }
            }
            
        }
        phase++;
        if (phase > NUM_COLS + size) finished = true;
        lastDrawn = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
    
    return state;
}
