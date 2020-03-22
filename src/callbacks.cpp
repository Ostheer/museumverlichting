#include <string>
#include <pthread.h>
#include <chrono>
#include <thread>
#include <vector>

//#include "ws2812/ws2812-rpi.h"
#include <ws2811.h>

#include "main.h"

using namespace std;

void* demoleds(void* p)  
{ 
    tmp_thread = pthread_self();  
  
    /*
    NeoPixel *n=new NeoPixel(180);
    while(true) n->effectsDemo();
    delete n;
    */
    
    return 0;
}

void* colour(void* p)  
{ 
    tmp_thread = pthread_self();  
  
    vector<int> L = *((vector<int> *) p);
    
    
    for (int i = 0; i < 180; i++){
        ledstring.channel[0].leds[i] = 0x00990033;
    }
    
    ws2811_return_t ret;
    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
    {
        fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
    }

    return 0;
}

void* off(void* p)  
{ 
    tmp_thread = pthread_self();  
  
    for (int i = 0; i < 180; i++){
        ledstring.channel[0].leds[i] = 0x00000000;
    }

    ws2811_return_t ret;
    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS)
    {
        fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
    }
    
    return 0;
}
