#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include "basics.h"
#include "vec2d.h"
#include "transform.h"
#include "tilemap.h"

int E = 32;
int rad;


typedef struct entity_struct{

    int id;
    int ti, tj;//coordenadas na spritesheet;
    float vel;

    vec2d pos;
    vec2d corners [4];

} Entity;

void refresh_corners( Entity *E ){
    E->corners[0] = v2d( E->pos.x - rad, E->pos.y - rad );
    E->corners[1] = v2d( E->pos.x + rad, E->pos.y - rad );
    E->corners[2] = v2d( E->pos.x + rad, E->pos.y + rad );
    E->corners[3] = v2d( E->pos.x - rad, E->pos.y + rad );
}
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

    int hE = E/2;
    rad = hE-2;
    
    Tilemap MAP;
    
    
    MAP.L = E;
    load_tilemap( "Assets/o mapa.tmx", &MAP );;
    MAP.spritesheet_pitch = 17;

    SDL_Texture *spritesheet = IMG_LoadTexture( R, "Assets/tiles.png" );
    if( spritesheet != NULL ) SDL_Log( "Loaded spritesheet." );
    else SDL_Log( "Failed to load spritesheet, : %s", SDL_GetError() );

    Entity player;
    player.pos = v2d( 256, 256 );
    player.vel = 3;
    refresh_corners( &player );
    player.id = 46;
    player.ti = player.id % MAP.spritesheet_pitch;
    player.tj = player.id / MAP.spritesheet_pitch;
    bool p1u = 0, p1d = 0, p1l = 0, p1r = 0; // up down left right
    
    
    //our desired frame period
    int frame_period = SDL_roundf( 1000 / 60.0 );

    SDL_Log("<<Entering Loop>>");
    while ( loop ) { //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> E O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
        
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

        vec2d desloc = {0,0};
        if( p1u ){
            desloc.y = -1;
        }
        if( p1d ){
            desloc.y = 1;
        }
        if( p1l ){
            desloc.x = -1;
        }
        if( p1r  ){
            desloc.x = 1;
        }
        if( desloc.x != 0 || desloc.y != 0 ){
            desloc = v2d_setlen(desloc, player.vel);
        }
        
        vec2d desloc_corners [4];// coordenadas em tiles
        for (int i = 0; i < 4; ++i){
            desloc_corners[i] = v2d_sum( player.corners[i], desloc );
            desloc_corners[i].x = floor( desloc_corners[i].x / E );
            desloc_corners[i].y = floor( desloc_corners[i].y / E );
        }


        bool blocked = false;//x pra y
        int blocks = 0;
        for(int i = 0; i < 4; i++){
          int y = floor(player.corners[i].y / E);
          if( desloc_corners[i].x >= 0 && desloc_corners[i].x < MAP.chunk_cols ){// checar se o passo fugiu do mapa
            if( MAP.solid[ (int)(desloc_corners[i].x) + (MAP.chunk_cols * y) ] ){
              blocked = true;
              break;
            }
          }
          else blocked = true;
        }
        if( blocked ) ++blocks;
        else player.pos.x += desloc.x;
        
        blocked = false;
        for(int i = 0; i < 4; i++){
          int x = floor(player.corners[i].x / E);
          if( desloc_corners[i].y >= 0 && desloc_corners[i].y < MAP.chunk_rows){
            if( MAP.solid[ x + (MAP.chunk_cols * (int)(desloc_corners[i].y)) ] ){
              blocked = true;
              break;
            }
          }
          else blocked = true;
        }
        if( blocked ) ++blocks;
        else player.pos.y += desloc.y;

        refresh_corners( &player );




        //if( world[currentMap].map[desloc_corners[i].x][y].solid ){

        //if( solid[ newcorner[i].x ][ newcorner[i].y ] ){

        render_layer( R, spritesheet, &MAP, MAP.background, &T );
        render_layer( R, spritesheet, &MAP, MAP.midground, &T );

        // desenhar player
        SDL_FRect src = (SDL_FRect){ player.ti * E, player.tj * E, E, E };
        SDL_FRect dst = (SDL_FRect){ atfX( player.pos.x - hE, &T ),
                                     atfY( player.pos.y - hE, &T ), E, E };
        SDL_RenderTexture( R, spritesheet, &src, &dst );




        // throw things up onscreen
        SDL_RenderPresent(R);

        // try to maintain constant framerate
        SDL_framerateDelay( frame_period );

    }//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> / E O O P <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    SDL_DestroyRenderer(R);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

