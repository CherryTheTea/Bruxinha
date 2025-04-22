#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include "basics.h"
#include "vec2d.h"
#include "transform.h"
#include "tilemap.h"




typedef struct entity_struct{

    int id;
    int ti, tj;//coordenadas na spritesheet;

    vec2d pos;
    vec2d corners [4];

} Entity;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~O~~~~~~~~~~| M A I N |~~~~~~~~~~~O~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main(int argc, char *argv[]){

    SDL_Window *window;
    SDL_Renderer *R;
    int width = 600;
    int height = 400;
    int cx, cy;
    int loop = 1;


    if( !SDL_Init(SDL_INIT_VIDEO) ){
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError() );
        return 3;
    }
    if( !SDL_CreateWindowAndRenderer( "Bruxa", width, height, 
                                      SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED, 
                                      &window, &R ) ){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }
    SDL_GetWindowSize( window, &width, &height );
    cx = width / 2;
    cy = height / 2;

    // prime the random number generator
    SDL_srand(0);

    Transform T = (Transform){256,256,cx,cy,1,1};
    int scaleI = 0;
    
    Tilemap MAP;
    int E = 32;
    int hE = 16;
    MAP.L = E;
    load_tilemap( "Assets/o mapa.tmx", &MAP );;
    MAP.spritesheet_pitch = 17;

    SDL_Texture *spritesheet = IMG_LoadTexture( R, "Assets/tiles.png" );
    if( spritesheet != NULL ) SDL_Log( "Loaded spritesheet." );
    else SDL_Log( "Failed to load spritesheet, : %s", SDL_GetError() );

    Entity player;
    player.pos = v2d( 256, 256 );
    player.id = 46;
    player.ti = player.id % MAP.spritesheet_pitch;
    player.tj = player.id / MAP.spritesheet_pitch;
    bool p1u = 0, p1d = 0, p1l = 0, p1r = 0; // up down left right
    
    
    //our desired frame period
    int frame_period = SDL_roundf( 1000 / 60.0 );

    SDL_Log("<<Entering Loop>>");
    while ( loop ) { //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> L O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
        
        SDL_Event event;
        while( SDL_PollEvent(&event) ){
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    loop = 0;
                    break;
                case SDL_EVENT_KEY_DOWN:
                         if( event.key.key == 'w' ) p1u = 1;
                    else if( event.key.key == 's' ) p1d = 1;
                    else if( event.key.key == 'a' ) p1l = 1;
                    else if( event.key.key == 'd' ) p1r = 1;
                    break;
                case SDL_EVENT_KEY_UP:
                         if( event.key.key == 'w' ) p1u = 0;
                    else if( event.key.key == 's' ) p1d = 0;
                    else if( event.key.key == 'a' ) p1l = 0;
                    else if( event.key.key == 'd' ) p1r = 0;
                    break;
            }
        }

        if( p1u ){
            player.pos.y -= 3;
        }
        if( p1d ){
            player.pos.y += 3;
        }
        if( p1l ){
            player.pos.x -= 3;
        }
        if( p1r  ){
            player.pos.x += 3;
        }


        //if( world[currentMap].map[I[i]][y].solid ){

        //if( solid[ newcorner[i].x ][ newcorner[i].y ] ){

        render_layer( R, spritesheet, &MAP, MAP.background, &T );

        SDL_FRect src = (SDL_FRect){ player.ti * E, player.tj * E, E, E };
        SDL_FRect dst = (SDL_FRect){ atfX( player.pos.x - hE, &T ),
                                     atfY( player.pos.y - hE, &T ), E, E };
        SDL_RenderTexture( R, spritesheet, &src, &dst );

        render_layer( R, spritesheet, &MAP, MAP.midground, &T );



        // throw things up onscreen
        SDL_RenderPresent(R);

        // try to maintain constant framerate
        SDL_framerateDelay( frame_period );

    }//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> / L O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    SDL_DestroyRenderer(R);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

