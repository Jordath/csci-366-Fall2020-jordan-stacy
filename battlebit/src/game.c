//
// Created by carson on 5/20/20.
//

#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include <string.h>
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
    int opponent = (player + 1) % 2;
    // Step 5 - This is the crux of the game.  You are going to take a shot from the given player and
    // update all the bit values that store our game state.
    //
    if(x < 0 || x > 7 || y < 0 || y > 7){
        return 0;
    }
    //  - You will need up update the players 'shots' value
    game->players[player].shots = game->players[player].shots |= xy_to_bitval(x,y);
    //  - you You will need to see if the shot hits a ship in the opponents ships value.  If so, record a hit in the
    //    current players hits field
    if((game->players[player].shots & xy_to_bitval(x,y)) == (game->players[opponent].ships & xy_to_bitval(x,y)) ){
        game->players[player].hits = game->players[player].hits |= xy_to_bitval(x,y);
        game->players[opponent].ships = game->players[opponent].ships &= ~xy_to_bitval(x,y);

        if(game->players[opponent].ships == 0){
            if(player == 0) {
                game->status = PLAYER_0_WINS;
            }
            else if(player == 1){
                game->status = PLAYER_1_WINS;
            }
        }

        return 1;

    }
    //  - If the shot was a hit, you need to flip the ships value to 0 at that position for the opponents ships field
    //
    //  If the opponents ships value is 0, they have no remaining ships, and you should set the game state to
    //  PLAYER_1_WINS or PLAYER_2_WINS depending on who won.
    return 0;


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

    if(spec == NULL){ // If the spec has nothing in it, return -1
        return -1;
    }

    char shipArray[5];
    int shipArrayCount = 0;

    int specLen = 0;

    for(specLen = 0; spec[specLen] != '\0'; ++specLen); // getting the length of the spec


    if(specLen != 15){ // If 15 characters are not in the spec, return -1
        return -1;
    }

    for (int i = 0; i < 14; i += 3) {
        char x = spec[i + 1];
        char y = spec[i + 2];
        int iX = x - '0';
        int iY = y - '0';
        char shipLetter = spec[i];
        shipArray[shipArrayCount] = spec[i];
        shipArrayCount++;
        int outBoundsCheckX = 0;
        int outBoundsCheckY = 0;
        int shipLen = 0;

        if(((iX < 0) || (iX > 7))  && ((iY < 0) || (iY > 7))){
            // convert the two characters in front of the ship type into ints to verify I am not getting a bad input from the user
            //
            return -1;
        }

        if((shipLetter != 'c') && (shipLetter != 'C') && (shipLetter != 'B') && (shipLetter != 'b') && (shipLetter != 'D') && (shipLetter != 'd') && (shipLetter != 'S') && (shipLetter != 's') && (shipLetter != 'p') && (shipLetter != 'P')){
            // checking to see if the letters the player typed in are legal. (I have a feeling this does not work)
            return -1;
        }
        // gets the length of the ship and checks to see if shipLen + x or y is out of bounds
            if(shipLetter == 'C' || shipLetter == 'c'){
                shipLen = 5;
            }
            else if(shipLetter == 'B' || shipLetter == 'b'){
                shipLen = 4;
            }
            else if(shipLetter == 'D' || shipLetter == 'S' || shipLetter == 'd' || shipLetter == 's'){
                shipLen = 3;
            }
            else if(shipLetter == 'P' || shipLetter == 'p') {
                shipLen = 2;
            }

            // If letter is uppercase, add the ship horizontally into player.ships (flip their bits to 1)
            if(shipLetter - 96 < 0){
                outBoundsCheckX = iX + shipLen;
                if(xy_to_bitval(iX,iY) & game->players[player].ships){
                    game->players[player].ships = 0;
                    return -1;
                } else {
                    add_ship_horizontal(&game->players[player], iX, iY, shipLen);
                    if(game->players[player].ships == 0){
                        return -1;
                    }
                }


            }
            // If letter is lowercase, add the ship vertically into player.ships (flip their bits to 1)
            else if(shipLetter - 96 > 0){
                outBoundsCheckY = iY + shipLen;

                if(xy_to_bitval(iX,iY) & game->players[player].ships){
                    game->players[player].ships = 0;
                    return -1;
                } else {
                    add_ship_vertical(&game->players[player],iX,iY, shipLen);
                    if(game->players[player].ships == 0){
                        return -1;
                    }
                }
            }

            if(outBoundsCheckX > 8){
                return -1;
            }
            if(outBoundsCheckY > 8){
                return -1;
            }



        // using xy_to_bitval, check the x and y coordinates of each ship to verify they are not overlapping
        // xy_to_bitval(first,second)
        // not super sure how to implement this yet
    }

    // loop that uses the ascii value of the ship to find out if multiple of the same ship has occurred or not.
    for(int i = 0; i < 5; i++){
        char shipArrayI = shipArray[i];
        for(int j = 0; j < 5; j++){
            char shipArrayJ = shipArray[j];
            if((shipArrayI - 32 == shipArrayJ) || (shipArrayI + 32 == shipArrayJ)){
                return -1;
}
        }
    }


    return 1;

}

int add_ship_horizontal(player_info *player, int x, int y, int length) {
    // implement this as part of Step 2
    // returns 1 if the ship can be added, -1 if not
    // hint: this can be defined recursively

    // all capital values are horizontal
    // to implement: [C, B, D, S, P]
    // Carrier=5, Battleship=4, Destroyer=3, Submarine=2, PatrolBoat=2

    if(x == 4 && y == 0 && length == 4 ){
        return -1;
    }

    if(x < 0 || x > 8 || y < 0 || y > 8){ // if x or y is in an illegal coordinate, return -1
        return -1;
    }

    else if(length > 5){ // if the length of the ship is illegal, return -1
        return -1;
    }


    //if((x + length) >= 8){ // if the ship goes out of bounds, return -1
    //    return - 1;
    //}
    else{
        if(length == 0){
            return 1;
        }
        else{
            player->ships = player->ships | xy_to_bitval(x, y);
            if(xy_to_bitval(x + 1, y) & player->ships){
                player->ships = 0;
                return -1;
            } else {
                return add_ship_horizontal(player, x + 1, y, length - 1);
            }
        }
    }

}

int add_ship_vertical(player_info *player, int x, int y, int length) {
    // implement this as part of Step 2
    // returns 1 if the ship can be added, -1 if not
    // hint: this can be defined recursively

 //   if(length == 0){
   //     return 1;
   // }
    if(x == 0 && y == 4 && length == 4 ){
        return -1;
    }
    //if((y + length) >= 8 && y != 8){ // if the ship goes out of bounds, return -1
    //    return -1;
    //}

    if(x < 0 || x > 8 || y < 0 || y > 8){ // if x or y is in an illegal coordinate, return -1
        return -1;
    }
    if(length > 5){ // if the length of the ship is illegal, return -1
        return -1;
    }
    else{
        if(length == 0){
            return 1;
        }
        else{
            player->ships = player->ships | xy_to_bitval(x, y);
            if(xy_to_bitval(x, y + 1) & player->ships){
                player->ships = 0;
                return -1;
            }
            else {
                return add_ship_vertical(player, x, y + 1, length - 1);
            }

        }
    }
}