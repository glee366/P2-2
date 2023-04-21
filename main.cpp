//=================================================================
// The main program file.
//
// Copyright 2023 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


// Projet includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
#include <math.h>

#define CITY_HIT_MARGIN 1
#define CITY_UPPER_BOUND (SIZE_Y-(LANDSCAPE_HEIGHT+MAX_BUILDING_HEIGHT))
static int active_map = 0;
static int before_cavex = 0;
static int before_cavey = 0;

// Helper function declarations
void playSound(char* wav);


/////////////////////////
// Struct of Player 
/////////////////////////

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;            // Current locations
    int px, py;         // Previous locations
    bool has_key;       // flag when obtained the key
    bool game_solved;   // flag when game is complete
    bool talked_to_npc; // flag when you've talked to npc
    bool ramblin;       // flag when ramblin has been activated/de

    //You may add more flags as needed

} Player;



/////////////////////////
// Get Action
/////////////////////////

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possible return values are defined below.
 * Get Actions from User (pushbuttons, and nav_switch)
 * Based on push button and navigation switch inputs, determine which action
 * needs to be performed (may be no action).
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define RAMBLIN 7

int get_action(GameInputs inputs)
{
    //******************
    // TODO: Implement
    //******************
    
    // 1. Check your action and menu button inputs and return the corresponding action value
    
    // 2. Check for your navigation switch inputs and return the corresponding action value
    
    // If no button is pressed, just return no action value
    MapItem* getN = get_north(Player.x, Player.y);
    MapItem* getS = get_south(Player.x, Player.y);
    MapItem* getE = get_east(Player.x, Player.y);
    MapItem* getW = get_west(Player.x, Player.y);


    if (!inputs.b1) {
        return ACTION_BUTTON;
    } 
    if (!inputs.b2) {
        return MENU_BUTTON;
    } 
    if (!inputs.b3) {
        return RAMBLIN;
    } 
    if (inputs.ns_up && getN->walkable || Player.ramblin && inputs.ns_up) {
        return GO_UP;
    }
    if (inputs.ns_down && getS->walkable || Player.ramblin && inputs.ns_down) {
        return GO_DOWN;
    }
    if (inputs.ns_right && getE->walkable || Player.ramblin && inputs.ns_right) {
        return GO_RIGHT;
    }
    if (inputs.ns_left && getW->walkable || Player.ramblin && inputs.ns_left) {
        return GO_LEFT;
    }
    
    return NO_ACTION;
}




/////////////////////////
// Helper Functions:
/////////////////////////
// Feel free to define any helper functions here for update game





/////////////////////////
// Update Game
/////////////////////////

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
int update_game(int action)

{
    
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    MapItem* item = NULL;
    //******************
    // TODO: Implement
    //******************
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        case GO_UP:
            //TODO: Implement
            //1. Check the item north of the player
            //2. Make sure to not walk through walls
            //3. If it is not a wall, the walk up by updating player's coordinates
            Player.y = Player.y - 1;
            break;
            
        case GO_LEFT:
            //TODO: Implement
            Player.x = Player.x - 1;
            break;
            
        case GO_DOWN:
            Player.y = Player.y + 1;
            break;
            
        case GO_RIGHT:
            Player.x = Player.x + 1; 
            break;

        case ACTION_BUTTON:

            // 1. Check if near NPC, 

            if (Player.x == 11 && Player.y == 12 || Player.x == 13 && Player.y == 12 ||
                Player.x == 12 && Player.y == 11 || Player.x == 12 && Player.y == 13 && active_map == 0) {
                if (Player.talked_to_npc == false) {
                    speech("You found me...  ", "I was stuck here ");
                    speech("for many years to", "come, and I am   ");
                    speech("happy to see you.", "");
                    speech("Go into the cave ", "and slay the     ");
                    speech("almighty BUZZ    ", "");
                    speech("Oh and one more  ", "thing.. I forgot ");
                    speech("which spell was  ", "effective........");
                    speech("Good luck LMAO...", "");
                    Player.talked_to_npc = true;
                } else if (Player.game_solved == true) {
                    speech("You did it...   ", "You slayed Buzz.. ");
                    speech("You deserve this", "the key to exit...");
                    speech("You are him/her.", "");
                    Player.has_key = true;
                } else {
                    speech("WYD????????????", "GO SLAY BUZZ");
                }
                return FULL_DRAW;
            }

            // 2. Check if near a door

            if (active_map == 0 && (abs(Player.x - 33) == 1 || abs(Player.y - 10) == 1)) {
                if (Player.has_key == false) {
                    speech("You need a key  ", "to enter the door ");
                    return FULL_DRAW;
                } else {
                    speech("You are entering", "END GAME         ");
                    speech("If you want to  ", "Reset the MBED to");
                    speech("Play again.", "TY for playing");
                    Player.x = 25;
                    Player.y = 25;
                    draw_game_over();
                }
            }


            // 3. Check if on Buzz's cave

            if (active_map == 0 && (Player.x == 4  && (Player.y == 20 || Player.y == 21)) 
            || (Player.x == 7 && (Player.y == 20 || Player.y == 21)) || (Player.y == 19 &&
                (Player.x == 5 || Player.x == 6)) || Player.y == 22 && (Player.x == 5 || Player.x == 6)) {
                if (Player.talked_to_npc == true) {
                    speech("You are entering", "a dangerous place ");
                    before_cavex = Player.x;
                    before_cavey = Player.y;
                    Player.x = 2;
                    Player.y = 1;
                    active_map = 1;
                    set_active_map(1);
                    return FULL_DRAW;
                } else if (active_map == 0 && (Player.x == 4  && (Player.y == 20 || Player.y == 21)) 
            || (Player.x == 7 && (Player.y == 20 || Player.y == 21)) || (Player.y == 19 &&
                (Player.x == 5 || Player.x == 6)) || Player.y == 22 && (Player.x == 5 || Player.x == 6)) {
                    speech("You can't enter.", "Talk to NPC first.");
                    return FULL_DRAW;
                }
            }

            // 4. Check if on a stairs
            if (Player.x == 2 && Player.y == 1 || Player.x == 1 && Player.y == 2
            && active_map == 1) {
                Player.x = before_cavex;
                Player.y = before_cavey;
                active_map = 0;
                set_active_map(0);
                return FULL_DRAW;
            }

            // 5. Check if at the right spell to end the game
            if (active_map == 1 && (Player.x == 6 && Player.y == 12 || Player.x == 4 && Player.y == 12 ||
            Player.x == 5 && Player.y == 11 || Player.x == 5 && Player.y == 13)) {
                speech("WATER has been ", "casted!");
                speech("It is super    ", "effective!");
                speech("BUZZ has been  ", "Defeated.");
                add_plant(7, 7);
                draw_plant(10, 10);
                Player.game_solved = true;
                return FULL_DRAW;
            }
            if (active_map == 1 && (Player.x == 8 && Player.y == 12 || Player.x == 10 && Player.y == 12 ||
            Player.x == 9 && Player.y == 13 || Player.x == 9 && Player.y == 11)) {
                speech("FIRE has been ", "casted!");
                speech("It is NOT     ", "effective!");
                return FULL_DRAW;
            }

            if (active_map == 1 && (Player.x == 12 && Player.y == 12 ||
            Player.x == 13 && Player.y == 13 || Player.x == 13 && Player.y == 11)) {
                speech("EARTH has been ", "casted!");
                speech("It is NOT     ", "effective!");
                return FULL_DRAW;
            }
            break;
        case RAMBLIN:
                if (Player.ramblin == false) {
                    Player.ramblin = true;
                    speech("RAMBLIN: ON", "");
                } else {
                    Player.ramblin = false;
                    speech("RAMBLIN: OFF", "");
                }
                return FULL_DRAW;
            break;
    }
    
    return NO_RESULT;
}




/////////////////////////
// Draw Game
/////////////////////////

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if (init) {
        draw_border();
    }
    
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) // draw player if init
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
                else if (curr_item && curr_item->type == CLEAR)
                {
                    // This is a special case for erasing things like doors.
                    draw = curr_item->draw; // i.e. draw_nothing
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) {
                draw(u, v);
            }
        }
    }

    // Draw status bars    
    draw_upper_status();
    draw_lower_status();
}







/////////////////////////
// Map Intialization
/////////////////////////

// Important locations for all maps
int cb_loc[2] = {5,20}; //Location of the center of the cave


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    //Initialize and sets current map to the first map
    Map* map = set_active_map(0);

    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        if(!(i % map_width() > 16 && i % map_width() < 35 && i / map_width() > 27 && i / map_width() < 40))
            add_plant(i % map_width(), i / map_width());
    }
    draw_plant(10, 10);
    /////////////////////////
    //Initial Environmnet
    /////////////////////////

    //Adding wall borders 
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    
    //Adding extra chamber borders 
    pc.printf("Add extra chamber\r\n");
    add_wall(30, 0, VERTICAL, 10);
    add_wall(30, 10, HORIZONTAL, 10);
    add_wall(39, 0, VERTICAL, 10);
    add_door(33, 10, HORIZONTAL, 4);
    draw_door(10, 10);


    //Adding extra cave to Buzz's evil lair
    pc.printf("Add cave\r\n");
    add_cave(cb_loc[0],cb_loc[1],1,1,5,5);      //Cave is set as a 4x4 block to be bigger
    add_cave(cb_loc[0]+1,cb_loc[1],2,1,5,5);
    add_cave(cb_loc[0],cb_loc[1]+1,3,1,5,5);
    add_cave(cb_loc[0]+1,cb_loc[1]+1,4,1,5,5);
    draw_cave1(10, 10);
    draw_cave2(10, 10);
    draw_cave3(10, 10);
    draw_cave4(10, 10);

    pc.printf("Initial environment completed\r\n");

    /////////////////////////////////
    // Characters and Items for the map
    /////////////////////////////////

    // Add NPC
    add_npc(12, 12);  //NPC is initialized to (x,y) = 10, 5. Feel free to move him around

    //Prints out map
    print_map();
}







void init_small_map()
{
    Map* map = set_active_map(1);

    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              14, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(14,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");


    add_water(5, 12);
    draw_water(10, 10);
    add_fire(9, 12);
    draw_fire(10, 10);
    add_earth(13, 12);
    draw_earth(10, 10);

    //
    // 3. Add Evil Buzz at the center of the map
    add_buzz(7, 7);
    draw_buzz(15, 15);


    // You may add any extra characters/items here for your project


    // Add stairs back to main (map 0)
    add_stairs(1, 1, 0, cb_loc[0], cb_loc[1]);
    draw_stairs(1, 1);
    
}


/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");



    // Initialize the maps
    maps_init();
    init_main_map();
    init_small_map();
    
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;
    Player.has_key = false;
    Player.game_solved = false;
    Player.talked_to_npc = false;

    // Initial drawing
    draw_game(true);

    // Main game loop
    while(1)
    {

        ////////////////////////////////
        // TODO: Implement 
        ////////////////////////////////

        // Timer to measure game update speed
        Timer t; t.start();
        
        // Actually do the game update:
        // 1. Read inputs        
        GameInputs in = read_inputs();
        // 2. Determine action (get_action)       
        int detAction = get_action(in);
        // 3. Update game (update_game)
        int result = update_game(detAction);  // Set this variable "result" for the resulting state after update game

        // 3b. Check for game over based on update game result
        if (result == GAME_OVER) {
            break;
        }
        // 4. Draw screen to uLCD
        bool full_draw = false;
        if (result == FULL_DRAW) full_draw = true;
        draw_game(full_draw);
        
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }

}




/////////////////////////////
//Advanced Features
/////////////////////////////

// Plays a wavfile
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
