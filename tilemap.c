#include "tilemap.h"


void load_layer( FILE *f, char *layer_name, Chunk *layer, int SX, int SY, int CO, int RO, int CC, int CR ){

	char buf [512];
	rewind(f);

	while( 1 ){
		if( fseek_string( f, "<layer" ) ){
			sprintf( buf, "name=\"%s", layer_name );
			if( fseek_string_before( f, buf, ">" ) ){
				break;
			}
		}
		else{
			printf( "Couldn't find layer \"%s\"!!!\n", layer_name );
			return;
		}
	}

	int CN = CO * RO;
	int Clen = CC * CR;

	long int layer_start = ftell( f );

	int X = 0; int Y = 0;

	for (int C = 0; C < CN; ++C ){

		fseek( f, layer_start, SEEK_SET );
		
		sprintf( buf, "<chunk x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\">", (X+SX) * CC, (Y+SY) * CR, CC, CR );
		if( fseek_string_before( f, buf, "</layer>" ) ){

			layer[C].IDs = calloc( Clen, sizeof(int32_t) );
			layer[C].unimatrix = calloc( Clen, sizeof(uint8_t) );
			layer[C].empty = 0;

			uint32_t raw = 0;
			int t = 0;
			while( fscanf(f, "%u,", &raw) > 0 ){
				if( raw > 0x0FFFFFFF ){
					layer[C].IDs[ t ] = (raw & 0x0FFFFFFF) - 1;
					layer[C].unimatrix[ t ] = raw >> 29;
				}
				else{
					layer[C].IDs[ t ] = raw-1;
					layer[C].unimatrix[ t ] = 0;
				}
				++t;
				if( t >= Clen ) break;
			}
		}
		else{
			//printf( "%s Appears to be missing! [X:%d, Y:%d]\n", buf, X, Y );
			layer[C].IDs = NULL;
			layer[C].unimatrix = NULL;
			layer[C].empty = 1;
		}

		X += 1;
		if( X >= CO ){
			X = 0;
			Y += 1;
			if( Y >= RO ){
				break;
			}
		}
	}
}


void load_tilemap( char *filename, Tilemap *TM ){

	FILE *f = fopen( filename, "rb" );

	if( f != NULL ){

		fseek_string( f, "<chunk ");
		fseek_string( f, "width=\"");
		fscanf( f, "%d\" height=\"%d\"", &(TM->chunk_cols), &(TM->chunk_rows) );
		//printf("chunk_cols= %d, chunk_rows= %d\n", TM->chunk_cols, TM->chunk_rows );

		rewind( f );

		int minx = 999999, maxx = -999999, miny = 999999, maxy = -999999;
		while( fseek_string( f, "<chunk ") ){
			int x, y;
			fscanf( f, "x=\"%d\" y=\"%d\"", &x, &y );//printf("x= %d, y= %d\n", x, y );
			if( x < minx ) minx = x;
			else if( x > maxx ) maxx = x;
			if( y < miny ) miny = y;
			else if( y > maxy ) maxy = y;
		}

		TM->SX = minx;
		TM->SY = miny;//printf("TM->SX: %d, TM->SY: %d\n", TM->SX, TM->SY );
		TM->cols = ((maxx - minx) / TM->chunk_cols)+1;
		TM->rows = ((maxy - miny) / TM->chunk_rows)+1;
		TM->chunks_count = TM->chunk_cols * TM->chunk_rows;
		printf("cols: %d * rows: %d = %d\n", TM->cols, TM->rows, TM->chunks_count );

		TM->background = malloc( TM->chunks_count * sizeof(Chunk) );
		load_layer( f, "background", TM->background, TM->SX, TM->SY, TM->cols, TM->rows, 
									 TM->chunk_cols, TM->chunk_rows );
		//for (int i = 0; i < 256; ++i ){	printf("%d, ", TM->background[0].IDs[i] );	if( i % 16 == 0 ) puts("");}

		TM->midground = malloc( TM->chunks_count * sizeof(Chunk) );
		load_layer( f, "midground", TM->midground, TM->SX, TM->SY, TM->cols, TM->rows, 
									 TM->chunk_cols, TM->chunk_rows );

		Chunk temp;
		load_layer( f, "solid", &temp, TM->SX, TM->SY, TM->cols, TM->rows, 
									   TM->chunk_cols, TM->chunk_rows );

		TM->solid = malloc( TM->chunk_cols * TM->chunk_rows * sizeof(bool) );
		int t = 0;
		for (int j = 0; j < TM->chunk_rows; ++j){
			for (int i = 0; i < TM->chunk_cols; ++i){
				if( temp.IDs[t] == 1 ){
					TM->solid[t] = true;
				}
				else TM->solid[t] = false;
				t++;
			}
		}

		free( temp.IDs );
		free( temp.unimatrix );
		fclose( f );
		puts("loaded map!");
	}
	else printf( "Could not open \"%s\"!!\n", filename );
}

void render_layer( SDL_Renderer *R, SDL_Texture *spritesheet, Tilemap *TM, Chunk *layer, Transform *T ){
	float E = TM->L * T->s;
	int C = 0;
	for (int CJ = 0; CJ < TM->rows; ++CJ ){
		for (int CI = 0; CI < TM->cols; ++CI ){
			if( layer[C].empty || layer[C].IDs == NULL ){
				C++;
				continue;
			}
			int t = 0;
			float IOFF = (CI + TM->SX) * TM->chunk_cols * TM->L;
			float JOFF = (CJ + TM->SY) * TM->chunk_rows * TM->L;
			for (int j = 0; j < TM->chunk_rows; ++j){ 
				for (int i = 0; i < TM->chunk_cols; ++i){
					//if( layer[C].IDs == NULL ) printf( "skip CI:%d, CJ:%d\n", CI, CJ );
					//printf( ">>> %p, %p\n", layer, layer[C].IDs );
					if( layer[C].IDs[t] >= 0 ){
						int ti = layer[C].IDs[t] % TM->spritesheet_pitch;
						int tj = layer[C].IDs[t] / TM->spritesheet_pitch;
						SDL_FRect src = (SDL_FRect){ ti * TM->L, tj * TM->L, TM->L, TM->L };
						SDL_FRect dst = (SDL_FRect){ atfX( IOFF + i * TM->L, T ),
													 atfY( JOFF + j * TM->L, T ), E, E };
						if( layer[C].unimatrix[ t ] ){
							SDL_RenderTextureRotated( R, spritesheet, &src, &dst, 
											   angles[ layer[C].unimatrix[ t ] ], NULL, 
											   flips[ layer[C].unimatrix[ t ] ] );
						}
						else{
							SDL_RenderTexture( R, spritesheet, &src, &dst );
						}
					}
					t++;
				}
			}
			C++;
			//printf("C: %d, ", C );
		}
	}
}