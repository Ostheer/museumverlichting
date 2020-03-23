#include <iostream>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <pthread.h>

#include "ws2812/ws2812-rpi.h"
#include <tgbot.h>

#include "main.h"
#include "tools.h"
#include "callbacks.h"

using namespace std;
using namespace TgBot;

pthread_t tmp_thread;
NeoPixel *n = new NeoPixel(NUM_LEDS);

int main(int argc, char* argv[]) {
    /*  Set up bot with token   */
    if (argc > 1)
        printf("Token: %s\n", argv[1]);
    else {
        cout << "You must supply the token as the first argument." << endl;
        return 1;
    }
    Bot bot(argv[1]);
    
    /*  Message callbacks   */
    
    //start
    bot.getEvents().onCommand("start", [&bot](Message::Ptr message){
        bot.getApi().sendMessage(message->chat->id, "Hi!");
    });
    
    //stop
    bot.getEvents().onCommand("stop", [&bot](Message::Ptr message){
        pthread_cancel(tmp_thread);
    });

    //off
    bot.getEvents().onCommand("off", [&bot](Message::Ptr message){
        //stop any currently running thread
        pthread_cancel(tmp_thread);
        
        pthread_t t;
        pthread_create(&t, NULL, off, NULL);
        
        bot.getApi().sendMessage(message->chat->id, "Turning everything off");
    });
    
    //demo
    bot.getEvents().onCommand("demo", [&bot](Message::Ptr message){
        //stop any currently running thread
        pthread_cancel(tmp_thread);
        
        pthread_t t;
        pthread_create(&t, NULL, demoleds, NULL);
        
        bot.getApi().sendMessage(message->chat->id, "demo leds");
    });

    //random
    bot.getEvents().onCommand("random", [&bot](Message::Ptr message){
        /* Function expects a message shaped "/random t" */
        //stop any currently running thread
        pthread_cancel(tmp_thread);
        //split the message
        vector<string> v = split (message->text, ' ');

        if (v.size() > 1){
            
            //allocate an int vector to store the parameters
            vector<int> L(1);
            try{
                //convert strings to int
                L.data()[0] = stoi(v[1]);
                
                //call the ledstrip function in another thread
                pthread_t t;
                pthread_create(&t, NULL, randomPulses, (void *) &L);
                
                //reply to the user
                bot.getApi().sendMessage(message->chat->id, "starting random pulses");
            }
            catch (const invalid_argument& ia) {
                bot.getApi().sendMessage(message->chat->id, "Invalid creation ");
                cout << "Invalid arguments." << endl;
            }
            catch(const out_of_range& e){
                bot.getApi().sendMessage(message->chat->id, "Invalid creation ");
                cout << "Invalid arguments." << endl;
            }
        }
        else{
            bot.getApi().sendMessage(message->chat->id, "Not enough arguments");
            cout << "Not enough arguments" << endl;
        }
    });
    
    //colour
    bot.getEvents().onCommand("colour", [&bot](Message::Ptr message){
        /* Function expects a message shaped "/colour r g b", with 0 < r, g, b < 255 */
        //stop any currently running thread
        pthread_cancel(tmp_thread);
        //split the message
        vector<string> v = split (message->text, ' ');

        
        if (v.size() > 3){
            //allocate an int vector to store the parameters
            vector<int> L(3);
            try{
                //convert strings to int
                for (int i = 0; i < 3; i++){
                    L.data()[i] = stoi(v[1+i]);
                }
                //call the ledstrip function in another thread
                pthread_t t;
                pthread_create(&t, NULL, colour, (void *) &L);
                //reply to the user
                bot.getApi().sendMessage(message->chat->id, "set colour");
            }
            catch (const invalid_argument& ia) {
                bot.getApi().sendMessage(message->chat->id, "Invalid colour");
                cout << "Invalid user message\n";
            }
            catch(const out_of_range& e){
                bot.getApi().sendMessage(message->chat->id, "Invalid creation ");
                cout << "Invalid arguments." << endl;
            }
        }
        else{
            bot.getApi().sendMessage(message->chat->id, "Not enough arguments");
            cout << "Not enough arguments" << endl;
        }
    });
    
    //horizontalPulse
    bot.getEvents().onCommand("pulse", [&bot](Message::Ptr message){
        /* Function expects a message shaped "/pulse r g b", with 0 < r, g, b < 255 */
        //stop any currently running thread
        pthread_cancel(tmp_thread);
        //split the message
        vector<string> v = split (message->text, ' ');

        if (v.size() > 3){
            //allocate an int vector to store the parameters
            vector<int> L(3);
            try{
                //convert strings to int
                for (int i = 0; i < 3; i++){
                    L.data()[i] = stoi(v[1+i]);
                }
                
                //call the ledstrip function in another thread
                pthread_t t;
                pthread_create(&t, NULL, horizontalPulse, (void *) &L);
                
                //reply to the user
                bot.getApi().sendMessage(message->chat->id, "starting Horizontal Pulse");
            }
            catch (const invalid_argument& ia) {
                bot.getApi().sendMessage(message->chat->id, "Invalid colour");
                cout << "Invalid arguments." << endl;
            }
        }
        else{
            bot.getApi().sendMessage(message->chat->id, "Not enough arguments");
            cout << "Not enough arguments" << endl;
        }
    });
    
    //Rainbow
    bot.getEvents().onCommand("rainbow", [&bot](Message::Ptr message){
        /* Function expects a message shaped "/rainbow t" */
        //stop any currently running thread
        pthread_cancel(tmp_thread);
        //split the message
        vector<string> v = split (message->text, ' ');

        if (v.size() > 1){
            //allocate an int vector to store the parameters
            vector<int> L(1);
            try{
                //convert strings to int
                L.data()[0] = stoi(v[1]);
                
                //call the ledstrip function in another thread
                pthread_t t;
                pthread_create(&t, NULL, rainbow, (void *) &L);
                
                //reply to the user
                bot.getApi().sendMessage(message->chat->id, "starting Rainbow");
            }
            catch (const invalid_argument& ia) {
                bot.getApi().sendMessage(message->chat->id, "Invalid time");
                cout << "Invalid arguments." << endl;
            }
            catch(const out_of_range& e){
                bot.getApi().sendMessage(message->chat->id, "Invalid creation ");
                cout << "Invalid arguments." << endl;
            }
        }
        else{
            bot.getApi().sendMessage(message->chat->id, "Not enough arguments");
            cout << "Not enough arguments" << endl;
        }
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

