#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include "vec.h"


const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1000;

const int TILE_SIZE = 40;

typedef struct {
    float x,y;
} Position_f;

const Vec2_f garbage_vel = { 99999, 99999 };
const Position_f garbage_pos = { 99999,99999 };

int main() {

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *player_tx;
    SDL_Texture *bulltet_tx;

    SDL_bool left_mouse_down = SDL_FALSE;
    SDL_bool left_mouse_released = SDL_FALSE;

    Vec2_f player_world_pos = { SCREEN_WIDTH / 2 + TILE_SIZE/2 , SCREEN_HEIGHT - TILE_SIZE*2 + TILE_SIZE/2 };

    // can fire five bullets at once
    Vec2_f bullet_world_position = player_world_pos;
    SDL_Point bullet_current_tile = {bullet_world_position.x / TILE_SIZE, bullet_world_position.y / TILE_SIZE};

    Vec2_f bullet_vel = {0, 0} ;
    if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf( stderr, "Error %s\n", SDL_GetError() );
        exit( EXIT_FAILURE );
    }
    if (! ( IMG_Init( IMG_INIT_PNG ) & IMG_INIT_PNG ) ) {
        fprintf( stderr, "Error %s\n ", IMG_GetError() );
        exit( EXIT_FAILURE );
    }

    window = SDL_CreateWindow( "RAILS", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
    if ( window == NULL ) {
        fprintf( stderr, "Error %s\n ", SDL_GetError() );
        exit( EXIT_FAILURE );
    }

    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL ) {
        fprintf( stderr, "Error %s\n ", SDL_GetError() );
        exit( EXIT_FAILURE );
    }

    // LOAD IMAGE as texture
    SDL_Surface *bullet_surface = IMG_Load( "res/img/player_bullet_round.png" );
    if( bullet_surface == NULL ) {
        fprintf( stderr, "Error %s\n ", IMG_GetError() );
        exit( EXIT_FAILURE );
    }

    bulltet_tx = SDL_CreateTextureFromSurface( renderer, bullet_surface );
    if( bulltet_tx == NULL ) {
        fprintf( stderr, "Error %s\n ", SDL_GetError() );
        exit( EXIT_FAILURE );
    }

    SDL_FreeSurface( bullet_surface );




    // PREPARE VARIABLES FOR LOOP
    SDL_Event event;
    int quit = 0;

    // delta time - frame rate independent movement
    float max_delta_time = 1 / 60.0;
    float previous_frame_ticks = SDL_GetTicks() / 1000.0;

    while (!quit) {

        // semi-fixed timestep
        float current_frame_ticks = SDL_GetTicks() / 1000.0;
        float delta_time = current_frame_ticks - previous_frame_ticks;
        previous_frame_ticks = current_frame_ticks;
        // adjust for any pauses, debugging breaks, etc
        delta_time = delta_time < max_delta_time ?  delta_time : max_delta_time;

        // EVENTS
        while (SDL_PollEvent( &event ) != 0 ) {
            if( event.type == SDL_QUIT ) {
                quit = 1;
            }

        }
        if(quit) break;

        SDL_Point mouse_position;
        Uint32 mouse_state = SDL_GetMouseState(&mouse_position.x, &mouse_position.y );
        
        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);

        if ( mouse_state & SDL_BUTTON_LEFT ) {
            left_mouse_released = SDL_FALSE;
            left_mouse_down = SDL_TRUE;
        }
        else if( left_mouse_down && ! ( mouse_state & SDL_BUTTON_LEFT ) ) {
            left_mouse_released = SDL_TRUE;
            left_mouse_down = SDL_FALSE;
        }
        Vec2_f mouse_world_pos;
        if( left_mouse_released && ( bullet_vel.x == 0 && bullet_vel.y == 0 ) ) {
            // fire
           
            mouse_world_pos.x = ( float ) mouse_position.x;
            mouse_world_pos.y = ( float ) mouse_position.y;
            Vec2_f vel = sub_vec2_f( mouse_world_pos, player_world_pos );
            Vec2_f d = unit_vec2_f( vel );
            float speed = 500;
            bullet_vel.x = d.x * speed * delta_time;
            bullet_vel.y =     d.y * speed * delta_time;
            
            Vec2_f x = add_vec2_f( player_world_pos, bullet_vel );

            
            left_mouse_released = SDL_FALSE;
        }

        // move bullet
        bullet_world_position.x += bullet_vel.x;
        bullet_world_position.y += bullet_vel.y;

        // bullet goes out of bounds
        // set to player position again
        if( bullet_world_position.x > SCREEN_WIDTH || bullet_world_position.x < 0 || bullet_world_position.y > SCREEN_HEIGHT || bullet_world_position.y < 0 ) {
            bullet_world_position.x = player_world_pos.x;
            bullet_world_position.y = player_world_pos.y;
            bullet_vel.x = 0;
            bullet_vel.y = 0;
        }
        
        bullet_current_tile.x = bullet_world_position.x / TILE_SIZE;
        bullet_current_tile.y = bullet_world_position.y / TILE_SIZE;
        // draw grid
        SDL_SetRenderDrawColor( renderer, 50,50,50,255);
        for ( int y = 0; y < SCREEN_HEIGHT; y+= TILE_SIZE ) {
            SDL_RenderDrawLine( renderer, 0, y, SCREEN_WIDTH, y);
        }
        for ( int x = 0; x < SCREEN_WIDTH; x+= TILE_SIZE) {
            SDL_RenderDrawLine( renderer, x, 0, x, SCREEN_HEIGHT );
        }

        SDL_Rect current_tile_rect = { bullet_current_tile.x * TILE_SIZE, bullet_current_tile.y * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        SDL_SetRenderDrawColor(renderer, 0,255,255,255);
        SDL_RenderDrawRect( renderer, &current_tile_rect );

        SDL_SetRenderDrawColor( renderer, 255,0,0,255);
        SDL_RenderDrawLine( renderer, player_world_pos.x , player_world_pos.y, mouse_world_pos.x, mouse_world_pos.y );

        SDL_Rect bullet_dest_rect = { bullet_world_position.x - TILE_SIZE/2, bullet_world_position.y - TILE_SIZE/2, TILE_SIZE, TILE_SIZE };
        SDL_Rect src_rect = {0,0, TILE_SIZE, TILE_SIZE};
        SDL_RenderCopy( renderer, bulltet_tx, &src_rect, &bullet_dest_rect );
        

        // RENDER
        //SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
       // SDL_RenderClear( renderer ); // set bg color 









       


        SDL_RenderPresent( renderer ); 
        

        SDL_Delay( 5 );
    }
}

