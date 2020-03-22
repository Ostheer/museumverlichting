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
  

    NeoPixel *n=new NeoPixel(180);
    while(true) n->effectsDemo();
    delete n;

    
    return 0;
}

void* colour(void* p)  
{ 
    tmp_thread = pthread_self();  

    vector<int> L = *((vector<int> *) p);

    NeoPixel *n=new NeoPixel(180);

    for (int i = 0; i < 180; i++){
        Color_t color(L.data()[0], L.data()[1], L.data()[2]);
        n->setPixelColor(i, color);
    }
    n->show();

    delete n;

    return 0;
}

void* off(void* p)  
{ 
    tmp_thread = pthread_self();  

    NeoPixel *n=new NeoPixel(180);

    for (int i = 0; i < 180; i++){
        Color_t color(0,0,0);
        n->setPixelColor(i, color);
    }
    n->show();

    delete n;

    return 0;
}
