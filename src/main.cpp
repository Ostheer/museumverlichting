#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>

#include "main.h"
#include "ws2812/ws2812-rpi.h"
#include <tgbot.h>

#include <pthread.h>

using namespace std;
using namespace TgBot;

pthread_t tmp_thread;

int main() {
    string token(getenv("TOKEN"));
    printf("Token: %s\n", token.c_str());
    Bot bot(token);
    
    bot.getEvents().onCommand("start", [&bot](Message::Ptr message)
    {
        bot.getApi().sendMessage(message->chat->id, "Hi!");
    });
    
    bot.getEvents().onCommand("stop", [&bot](Message::Ptr message)
    {
        pthread_cancel(tmp_thread);
    });
    
    bot.getEvents().onCommand("demo", [&bot](Message::Ptr message)
    {
        
        
        pthread_t thread_one;
        pthread_create(&thread_one, NULL, threadfunc, NULL);
        
        bot.getApi().sendMessage(message->chat->id, "demo leds");
    });
    
    bot.getEvents().onAnyMessage([&bot](Message::Ptr message) 
    {
        printf("User wrote %s\n", message->text.c_str());
        if (StringTools::startsWith(message->text, "/start")) {
            return;
        }
        bot.getApi().sendMessage(message->chat->id, "Your message is: " + message->text);
    });

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

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

void* threadfunc(void* p)  
{ 
    // store thread_two id to tmp_thread 
    tmp_thread = pthread_self();  
  
    demoleds();
    
    return 0;
} 

int demoleds(){
    NeoPixel *n=new NeoPixel(180);

    while(true) n->effectsDemo();
    delete n;

    return 0;
}
