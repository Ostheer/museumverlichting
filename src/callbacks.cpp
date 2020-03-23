#include <iostream>
#include <string>
#include <pthread.h>
#include <chrono>
#include <thread>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "ws2812/ws2812-rpi.h"

#include "main.h"
#include "tools.h"
#include "callbacks.h"

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

void* randomPulses(void* p)
{
    tmp_thread = pthread_self();  
    
    int N = 0;
    const int frame_delay = 30;
    const int pulseLength = 13;
    float gauss[pulseLength] = {0,0.043937,0.135335,0.324652,0.606531,0.882497,1.000000,0.882497,0.606531,0.324652,0.135335,0.043937,0};
    
    Color_t buffer[NUM_COLS][NUM_ROWS];
    for (int i = 0; i < NUM_ROWS; i++){
        for (int j = 0; j < NUM_COLS; j++){
            Color_t color(0,0,0);;
            buffer[i][j] = color;
        }
    }
    Color_t* rowbuffer;
    
    node* head = nullptr;
    node* current = head;
    node* temp;
    
    srand (time(NULL));
    
    while (true){
        /* Remove dead pulses */
        current = head;
        while (current != nullptr){
            //cout << "Looking for dead pulses" << endl;
            if (current->pulse->finished){
                //cout << "Checking if head was finished" << endl;
                if (current == head) head = head->next;
                
                //cout << "Removing finished pulse from chain" << endl;
                if (current->next != nullptr) current->next->prev = current->prev;
                if (current->prev != nullptr) current->prev->next = current->next;
                
                //cout << "Freeing memory" << endl;
                temp = current->next;
                delete current->pulse;
                delete current;
                current = temp;
                //cout << "Pulse deleted." << endl;
                
                N--;
            }
            if (current != nullptr) current = current->next;
        }
        
        /* Draw all pulses */
        current = head;
        while (current != nullptr){
            //cout << "Drawing pulse" << endl;
            rowbuffer = current->pulse->draw();
            for (int i = 0; i < NUM_COLS; i++) buffer[i][current->row] = colorAdd(rowbuffer[i], buffer[i][current->row]);
            current = current->next;
        }
        

        /* Write buffers to ledstrip */
        //cout << "write buffers" << endl;
        for (int i = 0; i < NUM_ROWS; i++){
            for (int j = 0; j < NUM_COLS; j++){
                n->setPixelColor(j + i*NUM_COLS, buffer[j][i]);
            }
        }
        
        /* render the changes */
        //cout << "render leds" << endl;
        n->show();
        
        /* Create new pulse at random */
        if (rand() % 50 == 1){
            //cout << "making pulse" << endl;
            N++;
            //cout << "Number of pulses: " << N << endl;
            Color_t color(rand()%256, rand()%256, rand()%256);
            
            //cout << "Looking for last element" << endl;
            current = head;
            temp = head;
            while (current != nullptr){
                temp = current;
                current = current->next;
            }
            current = temp;
            
            //cout << "making node" << endl;
            temp = new node;
            if (current != nullptr){
                current->next = temp;
                temp->prev = current;
            }
            else{
                temp->prev = nullptr;
                head = temp;
            }
            temp->next = nullptr;
            
            //cout << "making new pulse object" << endl;
            temp->pulse = new Pulse(rand()%60, pulseLength, rand()%2 ,color, gauss);
            temp->row = rand()%4;
            
        }
        
        /* clear buffer */
        //cout << "clear buffer" << endl;
        for (int i = 0; i < NUM_ROWS; i++){
            for (int j = 0; j < NUM_COLS; j++){
                Color_t color(0,0,0);;
                buffer[j][i] = color;
            }
        }
        
        /* Wait */
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/100));
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

