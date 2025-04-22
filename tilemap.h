#ifndef TILEMAP_H_INCLUDED
#define TILEMAP_H_INCLUDED

#include "basics.h"
#include "transform.h"


typedef struct chunk_struct{
	
	int32_t *IDs;
	uint8_t *unimatrix;
	bool empty;

} Chunk;

typedef struct tilemap_struct{

	int L; // length (width and height) of the tile in pixels

	int SX, SY;					// smallest (i.e. first, i.e. top-left-most) chunk position
	int cols, rows;             // chunks in the map
	int chunks_count;           // cols * rows
	int chunk_cols, chunk_rows; // tiles in the chunk

	Chunk *background;
	Chunk *midground;
	Chunk *foreground;

	bool *solid;

	int spritesheet_pitch;

} Tilemap;

static const float angles [] =           { 0,             -90,                 0,                 -90,           0,                   90,            180,           90                  };
static const SDL_FlipMode flips [] = { SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_VERTICAL, SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, SDL_FLIP_NONE, SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL };

void load_tilemap( char *filename, Tilemap *TM );
void render_layer( SDL_Renderer *R, SDL_Texture *spritesheet, Tilemap *TM, Chunk *layer, Transform *T );


#endif