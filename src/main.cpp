#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>

#include "main.h"
#include "ws2812/ws2812-rpi.h"
#include <tgbot.h>
#include "tools.h"

#include <pthread.h>

using namespace std;
using namespace TgBot;

pthread_t tmp_thread;

int main() {
    /*  Set up bot with token   */
    string token(getenv("TOKEN"));
    printf("Token: %s\n", token.c_str());
    Bot bot(token);
    
    
    /*  Message callbacks   */
    
    //start
    bot.getEvents().onCommand("start", [&bot](Message::Ptr message){
        bot.getApi().sendMessage(message->chat->id, "Hi!");
    });
    
    //stop
    bot.getEvents().onCommand("stop", [&bot](Message::Ptr message){
        pthread_cancel(tmp_thread);
    });
    
    //demo
    bot.getEvents().onCommand("demo", [&bot](Message::Ptr message){

        pthread_t t;
        pthread_create(&t, NULL, demoleds, NULL);
        
        bot.getApi().sendMessage(message->chat->id, "demo leds");
    });
    
    //colour
    bot.getEvents().onCommand("colour", [&bot](Message::Ptr message){
        //TODO: stop demo thread if it's running
        vector<string> v = split (message->text, ' ');
        
        vector<int> L(3);
        try{
            for (int i = 0; i < 3; i++){
                L.data()[i] = stoi(v[1+i]);
            }
        }
        catch (const invalid_argument& ia) {
            cout << "Invalid input. Please try again!\n";
        }
        
        /*cout << L.data()[0] << endl;
        cout << L.data()[1] << endl;
        cout << L.data()[2] << endl;*/
        
        pthread_t t;
        //pthread_create(&t, NULL, colour, NULL);
        pthread_create(&t, NULL, colour, (void *) &L);
        
        bot.getApi().sendMessage(message->chat->id, "set colour");
    });
    
    //anything
    bot.getEvents().onAnyMessage([&bot](Message::Ptr message) 
    {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
    });

    
    /*  Catch getting killed   */
    
    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });
    
    /*  Main loop   */

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (exception& e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}

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
