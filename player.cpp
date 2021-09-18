//=================================================================
// Implementation for the player module
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================
#include "player_private.h"

PLAYER player; // structure of player

PLAYER player_get_info(void){
    return player;
}



void player_init(void) {    
    player.y = 0;
    player.height = 10;
    player.score = 0;
    player.knife_length = 0;
    player.knifex = 0;
    player.player_has_knife = true;
    player.lives = 3;
    player.strikes = 7;
    player.y_pos = 0;
    player.knife_hits = 0;
}

void player_moveUp(void) { 
    if (player.height > 10) {
        player.height -= 5;
    }
    
    return; 
    
}

void player_moveDown(void) { 
    if (player.height < 110 - (get_compost_tallest_height())) {
        player.height += 5;
    }
    
    return;
    
}

void player_moveLeft(void) { 
    if (player.y_pos > 0) {
        player.y_pos -= 5;
    }
    return;
}

void player_moveRight(void) { 
    if (player.y_pos < 115) {
        player.y_pos += 5;
    }
    return;
}

void player_throw(void) { 
    player.player_has_knife = false;
    
    return;
    
}

void player_knife_draw(void) {
    
    if (player.player_has_knife == true) {
        player.knifex = player.height;
        player.y = player.y_pos;
        return;
    }
    
    else {
        if (player.y < 128) {
            player.y += 7;
            uLCD.filled_rectangle(player.y-7, player.knifex-3, player.y, player.knifex+6, 000000);
            uLCD.filled_rectangle(player.y, player.knifex, player.y+12, player.knifex+2, WHITE);
            uLCD.filled_rectangle(player.y, player.knifex+1, player.y+15, player.knifex+1, WHITE);
            uLCD.filled_rectangle(player.y+3, player.knifex-3, player.y+5, player.knifex+5, RED);
        }
        else {
            player_knife_return();
        }
        return;
    }
}

void player_knife_return(void) {
    uLCD.filled_rectangle(player.y, player.knifex-3, player.y+15, player.knifex+5, 000000);
    player.player_has_knife = true;
    player.y = player.y_pos;
    player.knife_hits = 0;
    return;
    
}

// ==== player_private.h implementation ====
void player_draw(int color) {
    uLCD.filled_rectangle(player.y_pos-10, player.height-10, player.y_pos + 20, player.height+20, 000000);
    
    
    uLCD.filled_rectangle(player.y_pos, player.height, player.y_pos + 10, player.height+1, color);
    uLCD.filled_rectangle(player.y_pos, player.height+2, player.y_pos + 12, player.height+4, RED);
    uLCD.filled_rectangle(player.y_pos, player.height+5, player.y_pos + 10, player.height+10, color);
    uLCD.filled_rectangle(player.y_pos+ 8, player.height+7, player.y_pos + 9, player.height+8, BLACK);
}

void player_score_inc(int points) {
    player.score += points;
}

void player_printScore(void) {
    uLCD.printf("score:%d  lives:%d\nstr: %d\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n", player.score, player.lives, player.strikes);
    
    return;
}

int player_get_score() {
    return player.score;
}

void game_over(void) {
    uLCD.filled_rectangle(0, 0, 127, 127, RED);
    uLCD.printf("\n\n\n\n    Game Over \n\n   score: %d\n\n\n\n\n\n\n\n\n\n", player.score);
}

void game_won(void) {
    uLCD.filled_rectangle(0, 0, 127, 127, GREEN);
    uLCD.printf("\n\n\n\n    Game WON! \n\n   score: %d\n\n\n\n\n\n\n\n\n\n", player.score);
}

void pause(void) {
    uLCD.filled_rectangle(0, 0, 127, 127, 0);
    uLCD.printf("\n\n\n\n    Paused! \n\n   score: %d\n\n\n\n\n\n\n\n\n\n", player.score);
    
    return;
}

void clearscrn(void) {
    uLCD.filled_rectangle(0, 0, 127, 127, 0);
    return;
}

int get_lives() {
    return player.lives;
}

void sub_life() {
    player.lives--;
    return;
}

int get_strikes() {
    return player.strikes;
}

void sub_strikes() {
    player.strikes--;
    return;
}

void knife_hits_inc() {
    player.knife_hits++;
}
int get_knife_hits() {
    return player.knife_hits;
}

bool player_has_knife_val() {
    return player.player_has_knife;
}

void printtime(int time) { // prints a time progress bar at the bottom of the screen
    
    uLCD.filled_rectangle(120, 3, 125, 124, WHITE);
    uLCD.filled_rectangle(121, 4, 124, 123, 000000);
    uLCD.filled_rectangle(121, 4 + (time * 2), 124, 123, GREEN);
    
    //uLCD.printf("\n\n\n\n %d \n\n\n\n\n\n\n\n\n\n\n\n", time);
    
}