//
// Created by carson on 5/20/20.
//

#include <stdlib.h>
#include <stdio.h>
#include "game.h"

// STEP 10 - Synchronization: the GAME structure will be accessed by both players interacting
// asynchronously with the server.  Therefore the data must be protected to avoid race conditions.
// Add the appropriate synchronization needed to ensure a clean battle.

static game * GAME = NULL;

void game_init() {
    if (GAME) {
        free(GAME);
    }
    GAME = malloc(sizeof(game));
    GAME->status = CREATED;
    game_init_player_info(&GAME->players[0]);
    game_init_player_info(&GAME->players[1]);
}

void game_init_player_info(player_info *player_info) {
    player_info->ships = 0;
    player_info->hits = 0;
    player_info->shots = 0;
}

int game_fire(game *game, int player, int x, int y) {
    // Step 5 - This is the crux of the game.  You are going to take a shot from the given player and
    // update all the bit values that store our game state.
    //
    //  - You will need up update the players 'shots' value
    //  - you You will need to see if the shot hits a ship in the opponents ships value.  If so, record a hit in the
    //    current players hits field
    //  - If the shot was a hit, you need to flip the ships value to 0 at that position for the opponents ships field
    //
    //  If the opponents ships value is 0, they have no remaining ships, and you should set the game state to
    //  PLAYER_1_WINS or PLAYER_2_WINS depending on who won.
}

unsigned long long int xy_to_bitval(int x, int y) {
    // Step 1 - implement this function.  We are taking an x, y position
    // and using bitwise operators, converting that to an unsigned long long
    // with a 1 in the position corresponding to that x, y
    //
    // x:0, y:0 == 0b1 (the one is in the first position)
    // x:1, y: 0 == 0b10 (the one is in the second position)
    // ....
    // x:0, y: 1 == 0b100000000 (the one is in the eighth position)
    //
    // you will need to use bitwise operators and some math to produce the right
    // value.
    unsigned long long bitVal;
    unsigned long long shift = 8*y + x;

    if(x < 0 || x > 7 || y < 0 || y > 7){
        bitVal = 0;
    }
    else {
        bitVal = 1ull << shift;
    }
    return bitVal;

}

struct game * game_get_current() {
    return GAME;
}

int game_load_board(struct game *game, int player, char * spec) {
    // Step 2 - implement this function.  Here you are taking a C
    // string that represents a layout of ships, then testing
    // to see if it is a valid layout (no off-the-board positions
    // and no overlapping ships)
    //
    // if it is valid, you should write the corresponding unsigned
    // long long value into the Game->players[player].ships data
    // slot and return 1
    //
    // if it is invalid, you should return -1

    // Ex. Spec: C00b02D23S47p71
    // Using xy_to_bitval(int x, int y)

    int specLen = 0;

    for(specLen = 0; spec[specLen] != '\0'; ++specLen); // getting the length of the spec

    if(spec == NULL){ // If the spec has nothing in it, return -1
        return -1;
    }
    if(specLen != 15){ // If 15 characters are not in the spec, return -1
        return -1;
    }

    for (int i = 0; i < 14; i += 3) {
        int first = (int)spec[i+1];
        int second = (int)spec[i+2];
        if((spec[i] != 'c') && (spec[i] != 'C') && (spec[i] != 'B') && (spec[i] != 'b') && (spec[i] != 'D') && (spec[i] != 'd') && (spec[i] != 'S') && (spec[i] != 's') && (spec[i] != 'p') && (spec[i] != 'P')){
            // checking to see if the letters the player typed in are legal. (I have a feeling this does not work)
            return -1;
        }

        else if(((first < 0) || (first > 7))  && ((second < 0) || (second > 7))){
            // convert the two characters in front of the ship type into ints to verify I am not getting a bad input from the user
            //
            return -1;
        }

        // My last idea is to loop through all of the ship characters and take the character of the ship
        // If there is a 'c' and a 'C', return -1
        // not super sure how to implement this yet

        // using xy_to_bitval, check the x and y coordinates of each ship to verify they are not overlapping
        // xy_to_bitval(first,second)
        // not super sure how to implement this yet

    }


}

int add_ship_horizontal(player_info *player, int x, int y, int length) {
    // implement this as part of Step 2
    // returns 1 if the ship can be added, -1 if not
    // hint: this can be defined recursively

    // all capital values are horizontal
    // to implement: [C, B, D, S, P]
    // Carrier=5, Battleship=4, Destroyer=3, Submarine=2, PatrolBoat=2
    if(x < 0 || x > 7 || y < 0 || y > 7){ // if x or y is in an illegal coordinate, return -1
        return -1;
    }
    if(length == 0 && x <= 7 && x >= 0 && y <= 7 && y >= 0){
        return 1;
    }
    else if(length < 2 || length > 5){ // if the length of the ship is illegal, return -1
        return -1;
    }
    if((x + length) > 7){ // if the ship goes out of bounds, return -1
        return - 1;
    }
    else{
        return 1;
    }

}

int add_ship_vertical(player_info *player, int x, int y, int length) {
    // implement this as part of Step 2
    // returns 1 if the ship can be added, -1 if not
    // hint: this can be defined recursively

    if(x < 0 || x > 7 || y < 0 || y > 7){ // if x or y is in an illegal coordinate, return -1
        return -1;
    }
    if(length < 2 || length > 5){ // if the length of the ship is illegal, return -1
        return -1;
    }
    if((y + length) > 7){ // if the ship goes out of bounds, return -1
        return - 1;
    }
    else{
        return 1;
    }
}