//=================================================================
// The main program file.
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

// External libs
#include <stdlib.h>

// Project includes
#include "globals.h"
#include "hardware.h"
#include "compost_pile_public.h"
#include "fruit_public.h"
#include "player_public.h"
#include "Speaker.h"

//For sound components
////AnalogOut DACout(p18);
//PwmOut pwmout(p26);
////PwmOut speaker(p25);

////wave_player waver(&DACout);
//SDFileSystem sd(p5, p6, p7, p8, "sd"); // mosi, miso, sck, cs

DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);

Speaker mySpeaker1(p25); 

// ===User implementations start===
int fruit_contact(void); // iterate through fruits and see if any collided
void compost_pile_update(void);
void fruitCheck(void);

void playSound(char * wav);
void playNotes(void);
void set_random_seed(Timer);

void throw_noise(void);
void playSoundEnter();
void playSoundHighScore();
void playSoundLost();

bool did_win = false;

int main()
{
    uLCD.baudrate(300000);
    int high_score = 0;
    GameInputs inputs; 
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    pc.printf("Program Starting\n");

    // ===User implementations start===
    // Game state variables
    
    // Timer to measure game update speed (secondarily used to generate random seed)
    Timer t;
    int dt; // delta time
    set_random_seed(t);
 
    //pwmout.period(1.0/4000.0);
    //playSound("/sd/wavfiles/BUZZER.wav");//test the sound convert to thread
    //wait(0.1);
    //playNotes();
    
    //initialize functions
    compost_pile_init();
    fruit_init();
    player_init();        
    pc.printf("Initialization complete\n");
    
    
    
    while(1)
    {
        
        bool timed_mode = false;
        bool max_score_mode = false;
        wait(1);
        uLCD.printf("Choose Game Mode:\n b1. top Score 5\n b2. timed(60s)");
        while(1){
            
            inputs = read_inputs();
            if (inputs.b1) {
                max_score_mode = true;
                break;
            }
            else if (inputs.b2) {
                timed_mode = true;
                break;
            }
        }
        
        uLCD.cls();
        
        compost_pile_init();
        fruit_init();
        player_init();  
        playSoundEnter();
        did_win = false;
        int time = 0; // timer 
         
        while(1) {
            t.start();
            time++;
            if(timed_mode == true) {
                printtime(time/6);
            }
            
    
            // Draw fruits first
            //pc.printf("fruit_generator\n");
            fruit_generator();
            
            //Put score on screen
            // NOTE: Text is 8 pixels tall
            player_printScore();
    
            //pc.printf("player_fruit_draw\n");
            player_draw(0x0000FF);
            player_knife_draw();
            
            
            
            //pc.printf("Reading inputs\n");
            inputs = read_inputs();
    
            //pc.printf("player motions\n");
            if (inputs.ax>= -0.2) player_moveUp();            
            if (inputs.ax<= -0.4) player_moveDown();   
            if (inputs.ay<= -0.2) player_moveRight();
            if (inputs.ay>= 0.2) player_moveLeft();             
            if (inputs.b3) {
                player_throw();
                if(player_has_knife_val() == false) {
                    throw_noise();
                }
            }
            
            //pc.printf("compost_pile_update\n");
            compost_pile_update();
            fruitCheck();
            
            //pc.printf("fruit_contact\n");
            int is_bomb = fruit_contact();
            
            if (get_strikes() == 0 || get_compost_tallest_height() > 80 || get_lives() == 0) {
                break;
            }
            
            if (inputs.b1 && inputs.b2) { // IN GAME MENU
                uLCD.cls();
                uLCD.printf("Menu \nHigh Score: %d\nYour Score: %d \nLives Left: %d\nStrikes: %d\n\n  b1. pause\n  b2. change mode\n\n\n\n\n\n\n\n\n", high_score, player_get_score(), get_lives(), get_strikes());
                //pause();
                wait(1);
                while(1){
                    inputs = read_inputs();
                    if (inputs.b1) {
                        pause();
                        while(1) {
                            wait(1);
                            inputs = read_inputs();
                            if (inputs.b1) {
                                break;
                            }
                            else {
                                continue;
                            }
                        }
                        break;
                    }
                    else if (inputs.b2) {
                        if (max_score_mode == true) {
                            max_score_mode = false;
                            timed_mode = true;
                            break;
                        }
                        else if (timed_mode == true) {
                            timed_mode = false;
                            max_score_mode = true;
                            break;
                        }
                    }
                }
                clearscrn();
            }
            
            if (max_score_mode == true && player_get_score() >= 5) {
                did_win = true;
                break;
            }
            
            if (timed_mode == true && (time/6) > 60) {
                did_win = true;
                break;
            }
            check_tetris();
            
    
            // Compute update time
            t.stop();
            dt = t.read_ms();
            
            if (dt < 100) wait_ms(100 - dt);
            
        } // end of inner while loop
        
        
        
        if (did_win == false) {
            playSoundLost();
            game_over();
            wait(2);
            if (player_get_score() > high_score) {
                uLCD.cls();
                high_score = player_get_score();
                uLCD.printf("\n\n\nNew high \nScore: %d", high_score);
                playSoundHighScore();
            }
            
            wait(2);
        }
        else {
            game_won();
            wait(2);
            if (player_get_score() > high_score) {
                uLCD.cls();
                high_score = player_get_score();
                uLCD.printf("\n\n\nNew high \nScore: %d", high_score);
                playSoundHighScore();
            }
            wait(2);
        }
        
        uLCD.cls();
        
        
        
        uLCD.printf("\n\n\nCurrent high \nScore: %d \n--------\nPress any button \nto continue", high_score);
        
        
        while(1){
            inputs = read_inputs();
            if (inputs.b1 || inputs.b2 || inputs.b3) break;
        }
        uLCD.cls();
        
        destroyList(get_fruit_list());
        
    } // end of outter while loop
    
    return 0;
    // ===User implementations end===
}

// ===User implementations start===

/* This function iterate through the fruit list, checking if anyone one of them is contacting 
*  with the player's knife. Don't forget to check if the "fruit" is the bomb, since we 
*  generated bombs using the fruit_generator. 
*/ 
int fruit_contact(void) {
    int return_val = 0;
    DLinkedList* fruitDLL = get_fruit_list();
    LLNode* current = fruitDLL->head;
    FRUIT* M;
    PLAYER player1 = player_get_info();
    while (current){
        M = (FRUIT*) current->data;
        int left_side = M->box.topLeft.x;
        int top_side = M->box.topLeft.y;
        int right_side = M->box.bottomRight.x;
        int bottom_side = M->box.bottomRight.y;
        if ((player1.y+10 >= left_side) && (player1.y+10 <= left_side + 10) && (player1.knifex <= (bottom_side+2)) && (player1.knifex >= (top_side-2)) ) {
            if (M->type == 0) {
                sub_life();
            }
            else {
                knife_hits_inc();
                if (get_knife_hits() == 1) {
                    player_score_inc(1);
                }
                else if (get_knife_hits() == 2) {
                    player_score_inc(2);
                }
                else {
                    player_score_inc(3);
                }
            }
            
            explode_fruit(M);
            draw_nothing(M->box);
            deleteNode(fruitDLL, current);
            
        }
        
        current = current->next;
    }
    return return_val;
}

/** Call compost_pile_update() repeatedly in your game-loop. ex: main()
    This function iterate through the fruit list. Add fruits that reach the bottom of the screen
    to the compost pile.
    @return  Number of remaining cities. You might end the game when all cities are demolished.
*/
void compost_pile_update(void){
    draw_compost();
    bool collision = false;
    DLinkedList* fruitDLL = get_fruit_list();
    LLNode* current = fruitDLL->head;
    FRUIT* M;
    PLAYER player1 = player_get_info();
    while (current){
        M = (FRUIT*) current->data;
        int left_side = M->box.topLeft.x;
        int top_side = M->box.topLeft.y;
        int right_side = M->box.bottomRight.x;
        int bottom_side = M->box.bottomRight.y;
        for (int i = 0; i < 11; i++) {
            if ((bottom_side  >= (127 - get_compost_height(i))) && ((right_side) > get_compost_position(i)) && ((right_side) < (get_compost_position(i)+11))) {
                draw_nothing(M->box);
                compost_animation(i);
                compost_add(i);
                deleteNode(fruitDLL, current);
                collision = true;
                break;
            }
        }
        if (collision == false) {
            current = current->next;
        }
        else {
            
            break;
        }
    }
    
}

void fruitCheck(void) { 

    DLinkedList* fruitDLL = get_fruit_list();
    LLNode* current = fruitDLL->head;
    FRUIT* M;
    while (current){
        M = (FRUIT*) current->data;
        int left_side = M->box.topLeft.x;
        int top_side = M->box.topLeft.y;
        int right_side = M->box.bottomRight.x;
        int bottom_side = M->box.bottomRight.y;
        if (M->type != 0 && (bottom_side <= 0 || left_side >= 128 || right_side <= 0)) {
            sub_strikes();
        }
        
        current = current->next;
    }
    return;

}

//fcn to play a wav
void playSound(char* wav)
{
    //open wav file
    FILE *wave_file;
    wave_file=fopen(wav,"r");
    
    if(wave_file != NULL) 
    {
        printf("File opened successfully\n");

        //play wav file
        printf("Sound playing...\n");
        waver.play(wave_file);
    
        //close wav file
        printf("Sound stopped...\n");
        fclose(wave_file);
        return;
    }
    
    printf("Could not open file for reading - %s\n", wav);
        return;
}

/* Wanna hear some annoying sounds?*/
void playNotes(void)
{
    int i;
// generate a 500Hz tone using PWM hardware output
    speaker.period(1.0/500.0); // 500hz period
    speaker =0.5; //50% duty cycle - max volume
    wait(3);
    speaker=0.0; // turn off audio
    wait(2);
// generate a short 150Hz tone using PWM hardware output
// something like this can be used for a button click effect for feedback
    for (i=0; i<10; i++) {
        speaker.period(1.0/150.0); // 500hz period
        speaker =0.25; //25% duty cycle - mid range volume
        wait(.02);
        speaker=0.0; // turn off audio
        wait(0.5);
    }

// sweep up in frequency by changing the PWM period
    for (i=0; i<8000; i=i+100) {
        speaker.period(1.0/float(i));
        speaker=0.25;
        wait(.1);
    }
    wait(2);

// two tone police siren effect -  two periods or two frequencies
// increase volume - by changing the PWM duty cycle
    for (i=0; i<26; i=i+2) {
        speaker.period(1.0/969.0);
        speaker = float(i)/50.0;
        wait(.5);
        speaker.period(1.0/800.0);
        wait(.5);
    }
    // decrease volume
    for (i=25; i>=0; i=i-2) {
        speaker.period(1.0/969.0);
        speaker = float(i)/50.0;
        wait(.5);
        speaker.period(1.0/800.0);
        wait(.5);
    }
    speaker =0.0;
    wait(2);
}

void set_random_seed(Timer t) {
    GameInputs inputs; 
    t.start();
    uLCD.printf("Push any button to start.\n");
    while(1){
      inputs = read_inputs();
      if (inputs.b1 || inputs.b2 || inputs.b3) break;
      }
    uLCD.cls();
    t.stop();
    int seed = t.read_ms();    
    srand(seed);
}

void throw_noise() {
    for (int i = 0; i < 10; i++){
        mySpeaker1.PlayNote(500-(i*i),0.02,0.1);
    }   
}

void playSoundEnter() { // game entry noise
    mySpeaker1.PlayNote(262.0,0.25,0.1);
    wait(.25);
    mySpeaker1.PlayNote(294.0,0.12,0.1);
    wait(.12);
    mySpeaker1.PlayNote(329.0,0.12,0.1);
    wait(.12);
    mySpeaker1.PlayNote(262.0,0.12,0.1);
    wait(.12);
    mySpeaker1.PlayNote(392.0,0.5,0.1);
    return;
}

void playSoundLost() { // game lost music
    mySpeaker1.PlayNote(523.0,0.1,0.1);
    wait(.1);
    mySpeaker1.PlayNote(587.0,0.1,0.1);
    wait(.25);
    
    mySpeaker1.PlayNote(494.0,0.15,0.1);
    wait(.15);
    
    mySpeaker1.PlayNote(699.0,0.15,0.1);
    wait(.15);
    mySpeaker1.PlayNote(699.0,0.15,0.1);
    wait(.15);
    mySpeaker1.PlayNote(699.0,0.15,0.1);
    wait(.15);
    
    mySpeaker1.PlayNote(659.0,0.15,0.1);
    wait(.15);
    mySpeaker1.PlayNote(587.0,0.15,0.1);
    wait(.15);
    mySpeaker1.PlayNote(523.0,0.15,0.1);
    wait(.15);
    
    mySpeaker1.PlayNote(330.0,0.15,0.1);
    wait(.15);
    mySpeaker1.PlayNote(330.0,0.1,0.1);
    wait(.1);
    
    mySpeaker1.PlayNote(261.0,0.25,0.1);
    wait(.25);
    
    return;
}

void playSoundHighScore() { // game won music
    mySpeaker1.PlayNote(261.0,0.25,0.1);
    mySpeaker1.PlayNote(330.0,0.25,0.1);
    mySpeaker1.PlayNote(392.0,0.25,0.1);
    wait(.25);
    
    mySpeaker1.PlayNote(311.0,0.25,0.1);
    mySpeaker1.PlayNote(392.0,0.25,0.1);
    mySpeaker1.PlayNote(466.0,0.25,0.1);
    wait(.25);
    
    mySpeaker1.PlayNote(466.0,0.25,0.1);
    mySpeaker1.PlayNote(587.0,0.25,0.1);
    mySpeaker1.PlayNote(698.0,0.25,0.1);
    wait(.25);
    
    mySpeaker1.PlayNote(370.0,0.25,0.1);
    mySpeaker1.PlayNote(466.0,0.25,0.1);
    mySpeaker1.PlayNote(554.0,0.25,0.1);
    wait(.25);
    
    mySpeaker1.PlayNote(277.0,0.25,0.1);
    mySpeaker1.PlayNote(349.0,0.25,0.1);
    mySpeaker1.PlayNote(415.0,0.25,0.1);
    wait(.25);
    
    mySpeaker1.PlayNote(523.0,0.25,0.1);
    mySpeaker1.PlayNote(659.0,0.25,0.1);
    mySpeaker1.PlayNote(784.0,0.25,0.1);
    wait(.25);
    
    
    return;
}
// ===User implementations end===
