//=================================================================
// The file is for module "compost pile"
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================

#include "compost_pile_private.h"

COMPOST compost_record[NUM_PILE];
int tallest_pile_height;


// See the comments in compost_pile_public.h
void compost_pile_init() {
    for (int i = 0; i < NUM_PILE; i ++) {
        compost_record[i].x = (127);
        compost_record[i].y = 11 * i;
        compost_record[i].height = 1;
        compost_record[i].width = 11;
    }
    tallest_pile_height = 1;
}

COMPOST compost_get_info(int index){
    return compost_record[index];
}

void compost_add(int index) {
    compost_record[index].height += 11;
    if (compost_record[index].height > tallest_pile_height) {
        tallest_pile_height = compost_record[index].height;
    }
}

void draw_compost(void){
    for (int i = 0; i < NUM_PILE; i++) {
        uLCD.filled_rectangle(compost_record[i].y, compost_record[i].x, compost_record[i].y + compost_record[i].width, compost_record[i].x - compost_record[i].height, GREEN);
    }
}

int get_compost_tallest_height() {
    return tallest_pile_height;
}

int get_compost_height(int index) {
    return compost_record[index].height;
}

int get_compost_position(int index) {
    return compost_record[index].y;
}

void check_tetris(void) {
    int count = 0;
    for (int i = 0; i < NUM_PILE; i++) {
        if (get_compost_height(i) > 11) {
            count += 1;
        }
        else {
            break;
        }
    }
    
    if (count >= 11) {
        tallest_pile_height -= 11;
        for (int i = 0; i < NUM_PILE; i++) {
            compost_record[i].height -= 11;
        }
    }
}

void compost_animation(int index) {
    for (int i = 0; i < 11; i++) {
        uLCD.filled_rectangle(compost_record[index].y, compost_record[index].x, compost_record[index].y + compost_record[index].width, compost_record[index].x - (compost_record[index].height + i), GREEN);
        wait(.05);
    }
    
}