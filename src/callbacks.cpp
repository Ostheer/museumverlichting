#include <iostream>
#include <string>
#include <pthread.h>
#include <chrono>
#include <thread>
#include <vector>

#include "ws2812/ws2812-rpi.h"

#include "main.h"

using namespace std;

void* demoleds(void* p)  
{ 
    tmp_thread = pthread_self();  
  
    while(true) n->effectsDemo();
    
    return 0;
}


void* horizontalPulse(void* p)  
{ 
    tmp_thread = pthread_self();  

    vector<int> L = *((vector<int> *) p);
    Color_t color(L.data()[0], L.data()[1], L.data()[2]);
    
    const int pulseLength = 13;
    float gauss[pulseLength] = {0,0.043937,0.135335,0.324652,0.606531,0.882497,1.000000,0.882497,0.606531,0.324652,0.135335,0.043937,0};
    
    int i = 0;
    while (true){
        for (int phase = 0; phase < NUM_COLS + pulseLength; phase++){
            for (int p = 0; p < pulseLength; p++){
                
                i = phase - pulseLength + p;
                if (i >= 0 && i < NUM_COLS){
                    Color_t color(gauss[p]*L.data()[0], gauss[p]*L.data()[1], gauss[p]*L.data()[2]);
                    
                    // row 0
                    n->setPixelColor(i, color);
                    // row 1
                    n->setPixelColor(2*NUM_COLS-1 - i, color);
                    // row 2
                    n->setPixelColor(2*NUM_COLS + i, color);
                    // row 3
                    n->setPixelColor(4*NUM_COLS-1 - i, color);
                }
                
            }
            n->show();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000/30));
        }
    }

    return 0;
}

void* colour(void* p)  
{ 
    tmp_thread = pthread_self();  

    vector<int> L = *((vector<int> *) p);

    for (int i = 0; i < NUM_LEDS; i++){
        Color_t color(L.data()[0], L.data()[1], L.data()[2]);
        n->setPixelColor(i, color);
    }
    n->show();


    return 0;
}

void* off(void* p)  
{ 
    tmp_thread = pthread_self();  

    for (int i = 0; i < NUM_LEDS; i++){
        Color_t color(0,0,0);
        n->setPixelColor(i, color);
    }
    n->show();


    return 0;
}

void* rainbow(void* p)
{ 
    tmp_thread = pthread_self();  

    int L = *((int *) p);

    while(true) n->rainbowCycle(L);

    return 0;
}
