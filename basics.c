#include "basics.h"


Uint8   red( Uint32 color ){
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		return (color & rmask)>>24;
	#else //          == SDL_LIL_ENDIAN
		return (color & rmask);
	#endif
}
Uint8 green( Uint32 color ){
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		return (color & gmask)>>16;
	#else //          == SDL_LIL_ENDIAN
		return (color & gmask)>>8;
	#endif
}
Uint8  blue( Uint32 color ){
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		return (color & bmask)>>8;
	#else //          == SDL_LIL_ENDIAN
		return (color & bmask)>>16;
	#endif
}
Uint8 alpha( Uint32 color ){
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		return (color & amask);
	#else //          == SDL_LIL_ENDIAN
		return (color & amask)>>24;
	#endif
}

Uint8 brightness( Uint32 color ){

	return (Uint8)(0.2126 * red(color) + 0.7152 * green(color) + 0.0722 * blue(color) );
	// variant: (0.299*R + 0.587*G + 0.114*B)
}

Uint32 rgba_to_Uint32( Uint8 r, Uint8 g, Uint8 b, Uint8 a ){
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		return a << 24 | b << 16 | g << 8 | r;
	#else
		return r << 24 | g << 16 | b << 8 | a;
	#endif
}

Uint32 SDL_Color_to_Uint32( SDL_Color C ){
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		return C.a << 24 | C.b << 16 | C.g << 8 | C.r;
	#else
		return C.r << 24 | C.g << 16 | C.b << 8 | C.a;
	#endif
}

SDL_Color Uint32_to_SDL_Color( Uint32 C ){
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		return (SDL_Color){ C & 0xff, (C >> 8) & 0xff, (C >> 16) & 0xff, (C >> 24) & 0xff };
	#else
		return (SDL_Color){ (C >> 24) & 0xff, (C >> 16) & 0xff, (C >> 8) & 0xff, C & 0xff };
	#endif
}

int SDL_SetRenderDraw_SDL_Color( SDL_Renderer *renderer, SDL_Color *C ){
	return SDL_SetRenderDrawColor( renderer, C->r, C->g, C->b, C->a);
}

int SDL_SetRenderDraw_Uint32( SDL_Renderer *renderer, Uint32 C ){
	Uint8 *p = (Uint8*) &C;
	#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		return SDL_SetRenderDrawColor( renderer, p[3], p[2], p[1], p[0] );
	#else
		return SDL_SetRenderDrawColor( renderer, p[0], p[1], p[2], p[3] );
	#endif
}
SDL_Color SDL_GetRender_SDL_Color( SDL_Renderer *R ){
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor( R, &r, &g, &b, &a );
	return(SDL_Color){ r, g, b, a };
}

Uint8 bytelerp( Uint8 start, Uint8 stop, float amt ){
	return start + (Uint8)round((stop-start) * amt);
}

Uint32 lerp_color( Uint32 CA, Uint32 CB, float amt ){

	Uint8 rA = red( CA );
	Uint8 gA = green( CA );
	Uint8 bA = blue( CA );
	Uint8 aA = alpha( CA );
	Uint8 rB = red( CB );
	Uint8 gB = green( CB );
	Uint8 bB = blue( CB );
	Uint8 aB = alpha( CB );
	Uint8 R = bytelerp( rA, rB, amt );
	Uint8 G = bytelerp( gA, gB, amt );
	Uint8 B = bytelerp( bA, bB, amt );
	Uint8 A = bytelerp( aA, aB, amt );

	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		return R>>24 | G>>16 | B>>8 | A;
	#else //             SDL_LIL_ENDIAN
		return R | G>>8 | B>>16 | A>>24;
	#endif
}

SDL_Color lerp_SDL_Color( SDL_Color CA, SDL_Color CB, float amt ){
	Uint8 R = bytelerp( CA.r, CB.r, amt );
	Uint8 G = bytelerp( CA.g, CB.g, amt );
	Uint8 B = bytelerp( CA.b, CB.b, amt );
	Uint8 A = bytelerp( CA.a, CB.a, amt );
	return (SDL_Color){ R, G, B, A };
}



void save_screenshot( SDL_Renderer *renderer, char *format, SDL_Rect *rct ){

	char str [128];

	if( strcchr( format, '%' ) > 0 ){
		time_t rawtime;
		struct tm * timeinfo;
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		strftime( str, 127, format, timeinfo );
	}
	else{
		strcpy( str, format );
	}

	int w = 0;
	int h = 0;
	if( rct == NULL ){
		SDL_GetCurrentRenderOutputSize( renderer, &w, &h );
	}
	else{
		w = rct->w;
		h = rct->h;
	}

	SDL_Surface *surf = SDL_RenderReadPixels( renderer, rct );
	
	if( IMG_SavePNG( surf, str ) ){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't save file: %s", SDL_GetError());
	}
	SDL_UnlockSurface(surf);
	SDL_DestroySurface(surf);
}
void save_texture(const char* file_name, SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_Texture* otarget = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, texture);
    //int width, height;
    //SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    float fw, fh;
	SDL_GetTextureSize( texture, &fw, &fh );
    SDL_Surface* surf = SDL_RenderReadPixels( renderer, NULL );
    IMG_SavePNG(surf, file_name);
    SDL_DestroySurface(surf);
    SDL_SetRenderTarget(renderer, otarget);
}




















double sq( double a ){
	return a * a;
}

double logarithm( double base, double x ){
	return log2( x ) / log2( base );
}

int get_divisors( int *list, int N ){
	int c = 0;
	int SQRT = floor(sqrt(N));
	if(N % 2 == 0) {
        for(int i = 2; i <= SQRT; i++) {
            if(N % i == 0) {
            	list[c++] = i;
            }
        }
    }
    else {
        for(int i = 3; i <= SQRT; i+=2) {
            if(N % i == 0) {
            	list[c++] = i;
            }
        }
    }
    //list[c] = -1;
    return c;
}


int randomI( int min, int max ){
    return min + SDL_rand(max-min);
}
float randomF( float min, float max ){
    return min + SDL_randf() * (max-min);
}


int random( int min, int max ){
    return (rand() % (max-min)) + min;
}

double random_angle(){
	// RAND_MAX :    32767
	//return 6.283185 * ( rand() / RAND_MAX );
    return rand() * 0.0001917534503;
}

int random_from_list(int n, ...){
	int R = random( 0, n );
	va_list vl;
	va_start(vl,n);
	for (int i = 0; i < R; i++) va_arg( vl, int );
	R = va_arg( vl, int );
	va_end(vl);
	return R;
}

//method discussed in Knuth and due originally to Marsaglia
//https://c-faq.com/lib/gaussian.html
double random_gaussian(){

	static double V1, V2, S;
	static int phase = 0;
	double X;

	if(phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;
			S = V1 * V1 + V2 * V2;
		} while(S >= 1 || S == 0);

		X = V1 * sqrt(-2 * log(S) / S);
	}
	else{
		X = V2 * sqrt(-2 * log(S) / S);
	}
	phase = 1 - phase;

	return X;
}

void shuffle( int *deck, int len ){
	for (int i = 0; i < len-2; ++i){
		int ni = random( i+1, len );
		int temp = deck[i];
		deck[i] = deck[ni];
		deck[ni] = temp;
	}
}

double lerp(double start, double stop, double amt) {
    return start + (stop-start) * amt;
}

double map(double value, double source_lo, double source_hi,  double dest_lo, double dest_hi) {
	return dest_lo + (dest_hi - dest_lo) * ((value - source_lo) / (source_hi - source_lo));
}

double ellipticalMap(double value, double source_lo, double source_hi, double dest_lo, double dest_hi){
  return dest_hi +((dest_lo-dest_hi)/abs(dest_lo-dest_hi))*sqrt((1-(sq(value-source_lo)/sq(source_hi-source_lo)))*sq(dest_hi-dest_lo));
}
double sigmoidMap(double value, double source_lo, double source_hi, double dest_lo, double dest_hi){
  return ( (dest_hi-dest_lo) * ( 1 / (1 + exp( -map( value, source_lo, source_hi, -6, 6 ) ) ) ) ) + dest_lo;
}
double advSigmoidMap(double value, double source_lo, double source_hi, double Slo, double Shi, double dest_lo, double dest_hi){
  return ( (dest_hi-dest_lo) * ( 1 / (1 + exp( -map( value, source_lo, source_hi, Slo, Shi ) ) ) ) ) + dest_lo;
}

int cycle( int a, int min, int max ){
	if( a < min ) return max;
	else if( a > max ) return min;
	else return a;
}

int constrain( int a, int min, int max ){
	if( a < min ) return min;
	else if( a > max ) return max;
	else return a;
}
float constrainF( float a, float min, float max ){
	if( a < min ) return min;
	else if( a > max ) return max;
	else return a;
}
double constrainD( double a, double min, double max ){
	if( a < min ) return min;
	else if( a > max ) return max;
	else return a;
}

int count_set_bits( unsigned int v ){
	unsigned int c; // c accumulates the total bits set in v
	for (c = 0; v; c++){
	  v &= v - 1; // clear the least significant bit set
	}
	return c;
}

double minD( double a, double b ){
	return (a < b)? a : b;
}
double maxD( double a, double b ){
	return (a > b)? a : b;
}


double degrees( double radians ){
	return radians * (double)57.29577951308232087679815481410517033240547246656432154916;//ONE_OVER_PI * 180;
}
double radians( double degrees ){
    return degrees * (double)0.017453292519943295769236907684886127134428718885417254560;// PI over 180
}


double rectify_angle( double a ){
	return fmod( a, TWO_PI );
	/*LOL
	if( a < 0 ){
		//printf("++ %f, %f, %f, %f.\n", a, abs(a), abs(a)/TWO_PI, ceil( abs(a) / TWO_PI ) );
		if( a >= -TWO_PI ) return TWO_PI + a;
		else return (ceil( abs(a) / TWO_PI ) * TWO_PI) + a;
	}
	else{
		if( a < TWO_PI ) return a;
		else{
			return a - (floor( a / TWO_PI ) * TWO_PI);
		}
	}*/
}
double angle_diff( double a, double b ){
	//return fmod(((a - b) + PI), TWO_PI ) - PI;

	double o = fmod( a-b, TWO_PI );
	o += (o>PI) ? -TWO_PI : (o<-PI) ? TWO_PI : 0;
	return o;

	//return min( TWO_PI - fabs(a - b), fabs(a - b));
}











void split( char *string, char *separator, char ***list, int *list_len ){
	int len = strlen( string );
	int seplen = strlen( separator );
	*list_len = ceil( len / 6.0 );//heuristic
	*list = malloc( (*list_len) * sizeof(char*) );
	int L = 0;
	int i = 0;
	while( i < len ){

		if( L >= *list_len ){
			*list_len *= 2;
			*list = realloc( *list, (*list_len) * sizeof(char*) );
		}
		(*list)[ L++ ] = string + i;


		while( i < len ){
			if( string[i] == separator[0] ){
				bool sepping = 1;
				for(int s = 1; s < seplen; s++){
					if( string[i+s] != separator[s] ){
						sepping = 0;
						break;
					}
				}
				if( sepping ){
					for(int s = 0; s < seplen; s++){
						string[i+s] = '\0';
					}
					i += seplen;
					break;
				}
				else i++;
			}
			else i++;
		}
		//++i;
	}
	*list_len = L;
	*list = realloc( *list, (*list_len) * sizeof(char*) );
}
	//apparently for strtok the string MUST be declared as "char string[]" in the calling function
	// it can't be a literal and it can't be "char *string"...
	/* TEST:
	char **list;
	int size = 0;
	char string[] = "split me baby one more time";
	split_string( string, " ", &list, &size );
	for (int i = 0; i < size; ++i){
	    printf("%s\n", list[i] );
	}
	*/
	// char * p = strtok (string, delimiters);
	// int i = 0;
	// while (p != NULL){
	// 	(*list)[i] = p;
	// 	p = strtok (NULL, delimiters);
	// 	++i;
	// }

int strcchr( char *string, char C ){ // String Count character
	
	int count = 0;
	for( int i = 0; string[i] != '\0'; i++ ){
		if( string[i] == C ) ++count;
	}
	return count;
}

// sub-string
char * substr( char *string, int start, int stop ){
	char *sub = (char*) calloc( stop-start +1, sizeof(char) );
	for (int i = start; i < stop; ++i){
		sub[i-start] = string[i];
	}
	sub[ stop-start ] = '\0';
	return sub;
}

// returns the index of the first char that does not match, or the length of the shortest string.
int str_match( char *A, char *B ){
	int i = 0;
	while(1){
		if( A[i] == '\0' ) return i;
		if( B[i] == '\0' ) return i;
		if( A[i] != B[i] ) return i;
		i++;
	}
}

bool str_insert_char( char *string, char C, int pos, int size ){
	char tmpA = string[pos];
	string[pos] = C;
	char tmpB;
	for (int i = pos+1; i < size; ++i){
		tmpB = string[i];
		string[i] = tmpA;
		if( string[i] == '\0' ) return 1;
		++i;
		if( i >= size ) return 0;
		tmpA = string[i];
		string[i] = tmpB;
		if( string[i] == '\0' ) return 1;
	}
	//string[size-1] = '\0';
	return 0;
}
void str_insert_str( char *string, char *in, int pos ){
	
	int str_len = strlen(string)+1;
	int in_len = strlen(in);
	memmove ( string + pos + in_len, string + pos, str_len - pos );
	memcpy ( string + pos, in, in_len );
}
void str_delete_char( char *string, int pos, int len ){
	for (int i = pos; i < len; ++i){
	   string[ i ] = string[ i+1 ];
	}
}





void STRB_init( STRB *S, int sz ){
	S->cap = sz;
	if( sz > 0 ){
		S->str = calloc( sz, sizeof(char) );
	}else{
		S->cap = 0;
		S->str = NULL;
	}
	S->len = 0;
}
void STRB_ensure( STRB *S, int len ){
	if( len >= S->cap ){
		if( S->cap == 0 ){
			S->cap = (ceil( len / 8.0 ) + 1 ) * 8;
		}
		else S->cap *= 2;
		S->str = realloc( S->str, S->cap * sizeof(char) );
	}
}
void STRB_reset( STRB *S, int sz ){
	if( sz > 0 ){
		S->cap = sz;
		S->str = realloc( S->str, S->cap * sizeof(char) );
		memset( S->str, 0, S->cap );
	}
	else if( S->str != NULL ){
		S->cap = 0;
		free( S->str );
		S->str = NULL;
	}	
	S->len = 0;
}
void STRB_justify( STRB *S ){
	S->cap = S->len + 1;
	S->str = realloc( S->str, S->cap * sizeof(char) );
	S->str[ S->len ] = '\0';
}
void STRB_copy( STRB *S, char *str ){
	if( str == NULL || str[0] == '\0' ){
		STRB_reset( S, 0 );
	}
	S->len = strlen(str);
	STRB_ensure( S, S->len );
	strcpy( S->str, str );
}

void STRB_append_char( STRB *S, char c ){
	STRB_ensure( S, S->len + 1 );
	S->str[(S->len)++] = c;
	S->str[S->len] = '\0';
}
void STRB_append_utf8( STRB *S, uint32_t c, int endianness ){
	char buf [4];
	int bytes = UINT32_to_UTF8( buf, c, endianness );
	if( bytes <= 0 ) return;
	STRB_ensure( S, S->len + bytes );
	memcpy( S->str + S->len, buf, bytes );
	S->len += bytes;
	S->str[S->len] = '\0';
}
void STRB_append_str( STRB *S, char *str ){
	int sl = strlen(str);
	STRB_ensure( S, S->len + sl );
	memcpy( S->str + S->len, str, sl+1 );
	S->len += sl;
	S->str[S->len] = '\0';
}

void STRB_insert_char( STRB *S, char c, int pos ){
	if( pos < 0 ) pos += S->len;
	if( pos < 0 || pos >= S->len ){
		return STRB_append_char( S, c );
	}
	STRB_ensure( S, S->len + 1 );
	memmove( S->str + pos + 1, S->str + pos, (S->len+1) - pos );
	S->str[pos] = c;
	S->len += 1;
}
void STRB_insert_utf8( STRB *S, uint32_t c, int endianness, int pos ){
	if( pos < 0 ) pos += S->len;
	if( pos < 0 || pos >= S->len ){
		return STRB_append_utf8( S, c, endianness );
	}
	char buf [4];
	int bytes = UINT32_to_UTF8( buf, c, endianness );
	if( bytes <= 0 ) return;
	STRB_ensure( S, S->len + bytes );
	memmove( S->str + pos + bytes, S->str + pos, (S->len+1) - pos );
	memcpy( S->str + pos, buf, bytes );
	S->len += bytes;
}
void STRB_insert_str( STRB *S, char *str, int pos ){
	if( pos < 0 ) pos += S->len;
	if( pos < 0 || pos >= S->len ){
		return STRB_append_str( S, str );
	}
	int sl = strlen(str);
	STRB_ensure( S, S->len + sl );
	memmove( S->str + pos + sl, S->str + pos, (S->len+1) - pos );
	memcpy( S->str + pos, str, sl );
	S->len += sl;
}

void STRB_delete( STRB *S, int pos ){
	if( pos < 0 ) pos += S->len;
	if( pos < 0 || pos >= S->len ) return;
	memmove( S->str + pos, S->str + pos + 1, S->len - pos );
	S->len -= 1;
}
void STRB_delete_range( STRB *S, int start, int stop ){
	start = constrain( start, 0, S->len-1 );
	stop = constrain( stop, 1, S->len );
	if( stop - start <= 1 ){
		return STRB_delete( S, start );
	}
	memmove( S->str + start, S->str + stop, (S->len+1) - stop );
	S->len -= stop - start;
}


char STRB_event_handler( STRB *S, int *cursor, SDL_Event *event ){

	if( event->type == SDL_EVENT_KEY_DOWN ){

		if( *cursor < 0 || *cursor > S->len ) *cursor = S->len;

		//printf( "%c (%d)\n", sym, sym );
		SDL_Keycode sym = event->key.key;

		if( sym == SDLK_LEFT ){
			*cursor = constrain( *cursor - 1, 0, S->len );
		}
		else if( sym == SDLK_RIGHT ){
			*cursor = constrain( *cursor + 1, 0, S->len );
		}
		else if( sym == SDLK_BACKSPACE && *cursor > 0 ){
			STRB_delete( S, *cursor-1 );
			*cursor -= 1;
		}
		else if( sym == SDLK_DELETE && *cursor < S->len ){
			STRB_delete( S, *cursor );
		}
		else if( sym == SDLK_HOME ){
			*cursor = 0;
		}
		else if( sym == SDLK_END ){
			*cursor = S->len;
		}
		else if( sym == SDLK_RETURN ){
			STRB_insert_char( S, '\n', *cursor );
			*cursor += 1;
		}
		else{
			const bool *state = SDL_GetKeyboardState(NULL);
			if( state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL] ){
				if( sym == 'c' ){
					SDL_SetClipboardText( S->str );
				}
				else if( sym == 'v' ){
					char *cb =  SDL_GetClipboardText();
					int tl = strlen( cb );
					STRB_insert_str( S, cb, *cursor );
					*cursor += tl;
					SDL_free( cb );
				}
			}
		}
		return 1;
	}
	if( event->type == SDL_EVENT_MOUSE_BUTTON_DOWN ){
		*cursor = -1;
		return 1;
	}
	if( event->type == SDL_EVENT_TEXT_INPUT ){
		int tl = strlen( event->text.text );
		STRB_insert_str( S, event->text.text, *cursor );
		*cursor += tl;
		return 1;
	}
	return 0;
}




void insert_sorted( int* list, int *len, int N ){
	for(int i = 0; i < (*len); i++){
		if( N == list[i] ) return;//NO DUPLICATES!
		if( N < list[i] ){
			*len += 1;
			int tmpA = list[i];
			list[i] = N;
			int tmpB;
			for (int j = i+1; j < (*len); ++j){
				tmpB = list[j];
				list[j] = tmpA;
				++j;
				if( j >= (*len) ) return;
				tmpA = list[j];
				list[j] = tmpB;
			}
			return;
		}
	}
	list[ (*len)++ ] = N;
}
void delete_repack( int* list, int *len, int i ){
	*len -= 1;
	for(int j = i; j < (*len); j++){
		list[j] = list[j+1];
	}
	list[(*len)] = 999999;//this is to ensure new numbers can be inserted later with insert_sorted
}

void Lshift_str( char *str, int n ){
	int i = 0;
	while(1){
		str[i] = str[i+n];
		if( str[i] == '\0' ) break;
		i++;
	}
}

void strtrim( char *string ){
	int len = strlen( string );
	for(int i = len-1; i >= 0; i--){
		if( !isspace( string[i] ) ){
			string[i+1] = '\0';
			break;
		}
	}
	for (int i = 0; i < len; ++i){
		if( !isspace( string[i] ) ){
			//out = string + i;
			if( i > 0 ) Lshift_str( string, i );
			break;
		}
	}
}

void strtrim_fgetsd_str( char *string ){
	int len = strlen( string );
	if( string[len-2] == '\r' ){
		string[len-2] = '\0';
		return;
	}
	else if( string[len-1] == '\n' ){
		string[len-1] = '\0';
		return;
	}
}

// get me a GOOD fucking character
char getgc( FILE *f ){
	char c;
	do{
		c = fgetc( f ); 
	} while( c == '\r' );//What the fuck is a "carriage" anyways
	return c;
}

int lines_in_a_file( FILE* f ){
	rewind(f);
    char c = fgetc(f);
    int lines = 1;
    while( c != EOF ){
        if (c == '\n') lines++;
        c = fgetc(f);
    }
    rewind(f);
    return lines;
}

bool fseek_lines( FILE* f, int N ){
	char c = fgetc( f );
	while( c != EOF ){
		if( c == '\n' ){
			N -= 1;
			if( N <= 0 ) return 1;
		}
		c = fgetc( f );
	}
	puts("EOF!!!");
	return 0;
}

bool fseek_string( FILE *f, char *str ){
	char c = getgc( f );
	int i = 0;
	while( c != EOF ){
		if( c == str[i] ){
			i++;
			if( str[i] == '\0' ) return 1;
		}
		else{
			i = 0;
		}
		c = getgc( f );
	}
	//puts("EOF!!!");
	return 0;
}


bool fseek_string_before( FILE *f, char *str, char *terminator ){
	
	long int original_pos = ftell( f );
	int s = 0;
	int t = 0;
	char c = getgc( f );
	while( c != EOF ){

		if( c == terminator[t] ){
			t++;
			if( terminator[t] == '\0' ){
				fseek( f, original_pos, SEEK_SET );
				return 0;
			}
		}
		else{
			t = 0;
		}

		if( c == str[s] ){
			s++;
			if( str[s] == '\0' ) return 1;
		}
		else{
			s = 0;
		}
		
		c = getgc( f );
	}
	fseek( f, original_pos, SEEK_SET );
	return 0;
}

void fseek_category( FILE *f, bool(*cateorize)(char c) ){
	char c = getc( f );
	while( c != EOF ){
		if( cateorize( c ) ){
			ungetc( (int)c, f );
			return;// 1;
		}
		c = getc( f );
	}
	return;// 0;
}

void fskip_whitespace( FILE *f ){
	char c;
	do{
		c = getc( f );
	} while( c == ' ' );
	ungetc( (int)c, f );
}

void fscan_str_until( FILE *f, char *dest, char *terminator, int size ){

	char c = getc( f );
	int s = 0;
	
	while( c != EOF ){

		if( c == terminator[0] ){
			long int pos = ftell( f );
			int t = 1;
			while( terminator[t] != '\0' ){
				c = getc( f );
				if( c == EOF || c != terminator[t] ){
					fseek( f, pos, SEEK_SET );
					c = terminator[0];
					goto nvm;
				}
				t++;
			}
			goto end;
		}
		else{
			nvm:
			dest[ s++ ] = c;
		}
		if( s >= size-1 ) break;
		c = getc( f );
	}
	end:
	dest[ s ] = '\0';
}


char **fscan_cslist( FILE *f, int *n, char separator ){//fscan a 'comma' separated list of strings
	long int original_pos = ftell( f );
	int commas = 1;
	int len = 0;
	char c;
	do{
		c = getc(f);
		if( c == EOF ) break;
		if( c == separator ) commas++;
		len++;	
	} while( c != '\n' && c != '\r' );

	char **list = malloc( commas * sizeof(char*) );
	if( n != NULL ) *n = commas;
	list[0] = malloc( len );
	fseek( f, original_pos, SEEK_SET );
	int i = 0;
	int j = 0;
	do{
		c = getc(f);
		if( c == separator || c == '\n' || c == '\r' || c == EOF ){	
			list[0][j++] = '\0';
			if( c == '\n' || c == '\r' || c == EOF ) break;
			i++;
			list[i] = list[0] + j;
		}
		else list[0][j++] = c;
	} while( c != EOF );

	//remove whitespace from the beginning of strings
	for (int i = 1; i < commas; ++i ){
		while( isspace( list[i][0] ) ){
			list[i] += 1;
		}
	}

	return list;
}

int f_count_char_until( FILE *f, char it, char *terminator ){

	int count = 0;
	char c = getc( f );
	
	while( c != EOF ){

		if( c == terminator[0] ){
			long int pos = ftell( f );
			int t = 1;
			while( terminator[t] != '\0' ){
				c = getc( f );
				if( c == EOF || c != terminator[t] ){
					fseek( f, pos, SEEK_SET );
					c = terminator[0];
					goto nvm;
				}
				t++;
			}
			goto end;
		}
		else{
			nvm:
			if( c == it ) count++;
		}
		c = getc( f );
	}
	end:
	return count;
}

void fscan_str_until_any( FILE *f, char *dest, char *terminators, int size ){
	
	char c = getc( f );
	int s = 0;
	while( c != EOF ){
		int t = 0;
		while( terminators[t] != '\0' ){
			if( c == terminators[t] ){
				goto end;
			}
			t++;
		}
		
		dest[ s++ ] = c;
		
		if( s >= size-1 ) break;
		c = getc( f );
	}
	end:
	dest[ s ] = '\0';
}


void fgets_but_good( FILE *f, char *dest, int size ){

	char c = getc( f );
	int s = 0;
	bool skiplwp = 1;//skip leading whitespace
	
	while( c != EOF ){

		if( skiplwp ){
			if( isspace(c) ){
				c = getc( f );
				continue;
			}
			else skiplwp = 0;
		}

		if( c == '\n' || c == '\r' ){
			break;
		}
		else{
			dest[ s++ ] = c;
		}
		if( s >= size-1 ) break;
		c = getc( f );
	}
	dest[ s ] = '\0';
}


void load_file_as_str( char *filename, char **out ){
	FILE *f = fopen( filename, "rb" );
	if( f == NULL ){
		printf("cannot open \"%s\"!\n", filename );
		return;
	}
	int len = 0;
	do{
		fgetc(f);
		len++;
	} while( !feof(f) );

	*out = malloc( len );
	rewind(f);
	int i = 0;
	do{
		(*out)[i++] = getgc(f);
	} while( !feof(f) );
	
	fclose(f);
	(*out)[i-1] = '\0';
}
/*
void get_filenames( char *directory, char ***list, int *length ){
	setlocale (LC_ALL, "");
    DIR *dir;
    struct dirent *ent;
    dir = opendir( directory );
    *list = NULL;
    *length = 0;
    readdir(dir);// getting rid of ./
    //readdir(dir);// getting rid of ../
    if (dir != NULL) {
        while ((ent = readdir (dir)) != NULL) {
        	//printf("%s\n", ent->d_name );
            (*length) += 1;
            (*list) = realloc( (*list), (*length) * sizeof(char*) );           
            size_t len = strlen( ent->d_name );
            (*list)[ (*length)-1 ] = calloc( len + 2, 1 );
            memcpy( (*list)[ (*length)-1 ], ent->d_name, len );
            if( ent->d_type == DT_DIR ){
            	(*list)[ (*length)-1 ][ len ] = '/';
            }
            (*list)[ (*length)-1 ][ len+1 ] = '\0';
        }
        closedir (dir);
    }
    else { 
        printf("ERROR: could not open directory: %s\n", directory );
    }
}
*/
void up_one_folder( char *path ){
	int L = strlen( path );
	for (int i = L-1; i >= 0; --i ){
		if( path[i] == '/' || path[i] == '\\' ){
			path[i] = '\0';
			return;
		}
	}
}



Uint16 *ascii_to_unicode( char *str ){
	int len = strlen( str );
	Uint16 *out = malloc( len * sizeof(Uint16) );
	int c = 0;
	for( int i = 0; str[i] != '\0'; ++i ){
		if( (str[i] & 0x80) == 0 ){
			out[c] = (Uint16) str[i];
			++c;
		}
		else{
			if( (str[i] & 0xE0 ) == 0xC0 ){
				out[c] = ( ( str[i] & 0x1F ) << 6 ) | (str[i+1] & 0x3F);
				++c;
				++i;
				--len;
			}
			if( (str[i] & 0xF0 ) == 0xE0 ){
				out[c] = ( ( str[i] & 0x0F ) << 12 ) | ( (str[i+1] & 0x3F) << 6 ) | (str[i+2] & 0x3F);
				++c;
				i += 2;
				len -= 2;
			}
		}
	}
	out = realloc( out, (len+1) * sizeof(Uint16) );
	out[len] = '\0';
	return out;
}

bool cursor_in_rect( SDL_Event *event, SDL_Rect *R ){
	switch (event->type) {
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP:
			return ( event->button.x > R->x && event->button.x < R->x + R->w ) && ( event->button.y > R->y && event->button.y < R->y + R->h );
		case SDL_EVENT_MOUSE_MOTION:
			return ( event->motion.x > R->x && event->motion.x < R->x + R->w ) && ( event->motion.y > R->y && event->motion.y < R->y + R->h );
		default:
			return 0;
	}
}

bool coordinates_in_Rect( float x, float y, SDL_Rect *R ){
	return ( x > R->x && x < R->x + R->w ) && ( y > R->y && y < R->y + R->h );
}
bool coordinates_in_FRect( float x, float y, SDL_FRect *R ){
	return ( x > R->x && x < R->x + R->w ) && ( y > R->y && y < R->y + R->h );
}
bool SDL_Rect_overlap( SDL_Rect *A, SDL_Rect *B ){
	return ( ( A->x + A->w > B->x ) && ( B->x + B->w > A->x ) ) && ( ( A->y + A->h > B->y ) && ( B->y + B->h > A->y ) );
}
bool SDL_FRect_overlap( SDL_FRect *A, SDL_FRect *B ){
	return ( ( A->x + A->w > B->x ) && ( B->x + B->w > A->x ) ) && ( ( A->y + A->h > B->y ) && ( B->y + B->h > A->y ) );
}
bool rect_overlap( int Ax, int Ay, int Aw, int Ah, int Bx, int By, int Bw, int Bh ){
	return ( ( Ax + Aw > Bx ) && ( Bx + Bw > Ax ) ) && ( ( Ay + Ah > By ) && ( By + Bh > Ay ) );
}
bool intersecting_or_touching( SDL_Rect *A, SDL_Rect *B){
	return ( ( A->x + A->w >= B->x ) && ( B->x + B->w >= A->x ) ) && ( ( A->y + A->h >= B->y ) && ( B->y + B->h >= A->y ) );
}

SDL_Rect add_rects( SDL_Rect *A, SDL_Rect *B){
	SDL_Rect out = *A;
	if( B->x < A->x ) out.x = B->x;
	if( B->y < A->y ) out.y = B->y;
	if( B->x + B->w > A->x + A->w  ) out.w = (B->x + B->w) - A->x;
	if( B->y + B->h > A->y + A->h  ) out.h = (B->y + B->h) - A->y;
	return out;
}

void fit_rect( SDL_Rect *A, SDL_Rect *B ){
	float Ar = A->w / (float) A->h;
	float Br = B->w / (float) B->h;
	if( Ar > Br ){
		int h = (int)( A->h * (B->w / (float) A->w) );
		A->x = B->x;
		A->y = B->y + ((B->h - h) / 2);
		A->w = B->w;
		A->h = h;
	}
	else {
		int w = (int)( A->w * (B->h / (float) A->h) );
		A->x = B->x + ((B->w - w) / 2);
		A->y = B->y;
		A->w = w;
		A->h = B->h;
	}
}





/*
char str [4] = "abcd";
printf("%x%x%x%x\n", str[0], str[1], str[2], str[3] );
int n = char4_to_int( str );
int_to_char4( n, str );
printf("%x, %x%x%x%x\n", n, str[0], str[1], str[2], str[3] );
*/
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	Uint32 char4_to_int( char str [4] ){	
		return (str[3] << 24) | (str[2] << 16) | (str[1] << 8) | str[0];
	}
	void int_to_char4( Uint32 N, char str [4] ){
		str[0] = N & 0xFF;
		str[1] = (N>>8) & 0xFF;
		str[2] = (N>>16) & 0xFF;
		str[3] = (N>>24) & 0xFF;
	}
#else
	Uint32 char4_to_int( char str [4] ){
		return (str[0] << 24) | (str[1] << 16) | (str[2] << 8) | str[3];
	}
	void int_to_char4( Uint32 N, char str [4] ){
		str[3] = N & 0xFF;
		str[2] = (N>>8) & 0xFF;
		str[1] = (N>>16) & 0xFF;
		str[0] = (N>>24) & 0xFF;
	}
#endif


int count_digits( int n ){
    if (n < 0) n = -n;//(n == INT_MIN) ? INT_MAX : -n;
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    if (n < 10000000) return 7;
    if (n < 100000000) return 8;
    if (n < 1000000000) return 9;
    /*      2147483647 is 2^31-1 - add more ifs as needed
       and adjust this final return as well. */
    return 10;
}


bool str_contains( char *str, bool(*cateorize)(char c) ){
    for (int i = 0; str[i] != '\0'; ++i ){
        if( cateorize( str[i] ) ) return 1;
    }
    return 0;
}
bool str_contains_only( char *str, bool(*cateorize)(char c) ){
    for (int i = 0; str[i] != '\0'; ++i ){
        if( !cateorize( str[i] ) ) return 0;
    }
    return 1;
}

bool list_contains( int *list, int len, int x ){
	for(int i = 0; i < len; i++){
		if( x == list[i] ){
			return 1;
		}
	}
	return 0;
}
int find_in_list( int *list, int len, int x ){
	for(int i = 0; i < len; i++){
		if( list[i] == x ){
			return i;
		}
	}
	return -1;
}


char shifted_keys( char c ){
	//            ! "#$%& '()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
	char S [] = " !\"#$%&\"()*+<_>?)!@#$%^&*(::<+>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ{|}^_`ABCDEFGHIJKLMNOPQRSTUVWXYZ{|}~";
	return S[ c - 32 ];
}


bool i_check( char c ){
	return (( c >= '0' && c <= '9' ) || ( c >= 'a' && c <= 'f' ) || c == 'x' || c == '-' || c == '+' );
}
bool d_check( char c ){
	return (( c >= '0' && c <= '9' ) || c == '-' || c == '+' );
}
bool x_check( char c ){
	return (( c >= '0' && c <= '9' ) || ( c >= 'a' && c <= 'f' ) || ( c >= 'A' && c <= 'F' ) || c == 'x' || c == 'X' || c == '-' || c == '+' );
}
bool f_check( char c ){
	return (( c >= '0' && c <= '9' ) || c == 'e' || c == 'E' || c == '.' || c == '-' || c == '+' );
}



int bytes_in_a_utf_codepoint( uint8_t ch ){
	return 1 + (ch >= 0xC0) + (ch >= 0xE0) + (ch >= 0xF0);
}
int retrobytes_in_a_utf_codepoint( const char *str ){
	//if( str == NULL ) return 0; 
	uint8_t v = (uint8_t)(*str);
	if( v >= 0x80 && v < 0xC0 ){
		return retrobytes_in_a_utf_codepoint( str-1 );
	}
	return 1 + (v >= 0xC0) + (v >= 0xE0) + (v >= 0xF0);
}
uint32_t binary_code_point( int bytes, uint32_t key ){
	switch( bytes ){
		case 2:
			return ((key & 0x1F00) >> 2) | (key & 0x3F);
		case 3:
			return ((key & 0xF0000) >> 4) | ((key & 0x3F00) >> 2) | (key & 0x3F);
		case 4:
			return ((key & 0x7000000)>>6) | ((key & 0x3F0000) >> 4) | ((key & 0x3F00) >> 2) | (key & 0x3F);
		default:
			return key;
	}
}
size_t utf8_strlen(const char *s) {
    size_t count = 0;
    while (*s) {
        count += (*s++ & 0xC0) != 0x80;
    }
    return count;
}

uint32_t UTF8_to_UINT32( char *str, int *bytes, int endianness ){

	*bytes = bytes_in_a_utf_codepoint( (uint8_t)(str[0]) );
	if( *bytes == 1 ) return (uint8_t)(str[0]);

	if( endianness == SDL_BIG_ENDIAN ){
		switch( *bytes ){
			case 2: 
				return ((uint8_t)(str[0]) <<  8) | ((uint8_t)(str[1]));
			case 3: 
				return ((uint8_t)(str[0]) << 16) | ((uint8_t)(str[1]) <<  8) | ((uint8_t)(str[2]));
			case 4: 
				return ((uint8_t)(str[0]) << 24) | ((uint8_t)(str[1]) << 16) | ((uint8_t)(str[2])) <<  8 | ((uint8_t)(str[3]));
		}
	}
	else if( endianness == SDL_LIL_ENDIAN ){
		switch( *bytes ){
			case 2:
				return ((uint8_t)(str[1]) <<  8) |  (uint8_t)(str[0]);
			case 3:
				return ((uint8_t)(str[2]) << 16) | ((uint8_t)(str[1]) <<  8) |  (uint8_t)(str[0]);
			case 4:
				return ((uint8_t)(str[3]) << 24) | ((uint8_t)(str[2]) << 16) | ((uint8_t)(str[1]) << 8) | (uint8_t)(str[0]);	
		}
	}
	return (uint8_t)(str[0]);
}

int UINT32_to_UTF8( char *str, uint32_t num, int endianness ){
	uint8_t *arr = (uint8_t *)(&num);

	if( endianness == SDL_BIG_ENDIAN ){
		//printf("[%08X] arr: %02X, %02X, %02X, %02X\n", num, arr[0], arr[1], arr[2], arr[3] );
		if( arr[3] > 0 ){
			str[0] = arr[3];
			str[1] = arr[2];
			str[2] = arr[1];
			str[3] = arr[0];
			return 4;
		}
		else if( arr[2] > 0 ){
			str[0] = arr[2];
			str[1] = arr[1];
			str[2] = arr[0];
			str[3] = 0;
			return 3;
		}
		else if( arr[1] > 0 ){
			str[0] = arr[1];
			str[1] = arr[0];
			str[2] = 0;
			str[3] = 0;
			return 2;
		}
		else {
			str[0] = arr[0];
			str[1] = 0;
			str[2] = 0;
			str[3] = 0;
			return 1;
		}
	}
	else if( endianness == SDL_LIL_ENDIAN ){
		str[0] = arr[0];
		str[1] = arr[1];
		str[2] = arr[2];
		str[3] = arr[3];
		return bytes_in_a_utf_codepoint( arr[0] );
	}
	return 0;
}


void print_string_with_escape_chars_visible( char *str ){
	
	for( int i = 0; str[i] != '\0'; ++i ){
		switch( str[i] ){
			case '\'': // 	single quote 	byte 0x27 in ASCII encoding
				printf( "\\'" );
				break;
			case '\"':// 	double quote 	byte 0x22 in ASCII encoding
				printf( "\\\"" );
				break;
			case '\?':// 	question mark 	byte 0x3f in ASCII encoding
				printf( "\\?" );
				break;
			case '\\':// 	backslash 	byte 0x5c in ASCII encoding
				printf( "\\\\" );
				break;
			case '\a'://	audible bell 	byte 0x07 in ASCII encoding
				printf( "\\a" );
				break;
			case '\b'://	backspace 	byte 0x08 in ASCII encoding
				printf( "\\b" );
				break;
			case '\f'://	form feed - new page 	byte 0x0c in ASCII encoding
				printf( "\\f" );
				break;
			case '\n'://	line feed - new line 	byte 0x0a in ASCII encoding
				printf( "\\n" );
				break;
			case '\r'://	carriage return 	byte 0x0d in ASCII encoding
				printf( "\\r" );
				break;
			case '\t'://	horizontal tab 	byte 0x09 in ASCII encoding
				printf( "\\t" );
				break;
			case '\v'://	vertical tab 	byte 0x0b in ASCII encoding 
				printf( "\\v" );
				break;
			default:
				putchar( str[i] );
		}
	}
	puts("");
}

void hard_print( char *str, int N ){
	
	putchar( '\"' );
	for( int i = 0; i < N; ++i ){
		switch( str[i] ){
			case '\0': // 	terminator
				printf( "\\0" );
				break;
			case '\'': // 	single quote 	byte 0x27 in ASCII encoding
				printf( "\\'" );
				break;
			case '\"':// 	double quote 	byte 0x22 in ASCII encoding
				printf( "\\\"" );
				break;
			case '\?':// 	question mark 	byte 0x3f in ASCII encoding
				printf( "\\?" );
				break;
			case '\\':// 	backslash 	byte 0x5c in ASCII encoding
				printf( "\\\\" );
				break;
			case '\a'://	audible bell 	byte 0x07 in ASCII encoding
				printf( "\\a" );
				break;
			case '\b'://	backspace 	byte 0x08 in ASCII encoding
				printf( "\\b" );
				break;
			case '\f'://	form feed - new page 	byte 0x0c in ASCII encoding
				printf( "\\f" );
				break;
			case '\n'://	line feed - new line 	byte 0x0a in ASCII encoding
				printf( "\\n" );
				break;
			case '\r'://	carriage return 	byte 0x0d in ASCII encoding
				printf( "\\r" );
				break;
			case '\t'://	horizontal tab 	byte 0x09 in ASCII encoding
				printf( "\\t" );
				break;
			case '\v'://	vertical tab 	byte 0x0b in ASCII encoding 
				printf( "\\v" );
				break;
			default:
				putchar( str[i] );
		}
	}
	puts("\"");
}

void hard_print_f( FILE *f, int N ){
	
	long int original_pos = ftell( f );
	char c = getc(f);
	putchar( '\"' );
	for( int i = 0; i < N; ++i ){
		switch( c ){
			case EOF:
				printf( "EOF\"\n" );
				fseek( f, original_pos, SEEK_SET );
				return;
			case '\0': // 	terminator
				printf( "\\0" );
				break;
			case '\"':// 	double quote 	byte 0x22 in ASCII encoding
				printf( "\\\"" );
				break;
			case '\?':// 	question mark 	byte 0x3f in ASCII encoding
				printf( "\\?" );
				break;
			case '\\':// 	backslash 	byte 0x5c in ASCII encoding
				printf( "\\\\" );
				break;
			case '\a'://	audible bell 	byte 0x07 in ASCII encoding
				printf( "\\a" );
				break;
			case '\b'://	backspace 	byte 0x08 in ASCII encoding
				printf( "\\b" );
				break;
			case '\f'://	form feed - new page 	byte 0x0c in ASCII encoding
				printf( "\\f" );
				break;
			case '\n'://	line feed - new line 	byte 0x0a in ASCII encoding
				printf( "\\n" );
				break;
			case '\r'://	carriage return 	byte 0x0d in ASCII encoding
				printf( "\\r" );
				break;
			case '\t'://	horizontal tab 	byte 0x09 in ASCII encoding
				printf( "\\t" );
				break;
			case '\v'://	vertical tab 	byte 0x0b in ASCII encoding 
				printf( "\\v" );
				break;
			default:
				putchar( c );
		}
		c = getc(f);
	}

	puts("\"");
	fseek( f, original_pos, SEEK_SET );
}


int SDL_framerateDelay( int frame_period ){
    static Uint64 then = 0;
    Uint64 now = SDL_GetTicks();
    int elapsed = now - then;
    int delay = frame_period - elapsed;
    //printf("%d - (%d - %d) = %d\n", frame_period, now, then, delay );
    if( delay > 0 ){
        SDL_Delay( delay );
        elapsed += delay;
    }
    then = SDL_GetTicks();
    return elapsed;
}


bool i2d_equals( index2d A, index2d B ){
   return (A.i == B.i) && (A.j == B.j);
}

int i2d_manhattan( index2d A, index2d B ){
   return abs(A.i - B.i) + abs(A.j - B.j);
}


int rect_area( SDL_Rect *r ){
	return r->w * r->h;
}

void rectCluster_init( rectCluster *rC, int x, int y, int w, int h ){
	rC->len = 1;
	rC->size = 4;
	rC->original = (SDL_Rect){x,y,w,h};
	rC->rcts = malloc( rC->size * sizeof(SDL_Rect) );
	rC->rcts[0] = rC->original;
}

static void rectCluster_append( rectCluster *rC, int x, int y, int w, int h ){

	if( rC->len >= rC->size ){
		rC->size *= 2;
		rC->rcts = realloc( rC->rcts, rC->size * sizeof(SDL_Rect) );
	}
	rC->rcts[ rC->len ] = (SDL_Rect){x,y,w,h};
	rC->len += 1;
}

void clip_rectCluster( rectCluster *rC, SDL_Rect cut ){

	int cut_r = cut.x + cut.w;
 	int cut_b = cut.y + cut.h;

	int len_so_far = rC->len;

	for ( int i = 0; i < len_so_far; ++i ){

		if( rect_area( rC->rcts + i ) <= 0 ) continue;

		int rC_rcts_i_r = rC->rcts[i].x + rC->rcts[i].w;
 		int rC_rcts_i_b = rC->rcts[i].y + rC->rcts[i].h;

 		if( cut.x >= rC_rcts_i_r  ||
 			cut.y >= rC_rcts_i_b  ||
 			cut_r <= rC->rcts[i].x ||
			cut_b <= rC->rcts[i].y ){

			continue;
		}

		bool top_in = cut.y > rC->rcts[i].y  &&  cut.y < rC_rcts_i_b;
		bool bot_in = cut_b > rC->rcts[i].y  &&  cut_b < rC_rcts_i_b;
		bool lef_in = cut.x > rC->rcts[i].x  &&  cut.x < rC_rcts_i_r;
		bool rig_in = cut_r > rC->rcts[i].x  &&  cut_r < rC_rcts_i_r;

		int total = top_in + bot_in + lef_in + rig_in;

		//printf("\n%d: %d%d%d%d = %d. ", i, top_in, bot_in, lef_in, rig_in, total );

		switch( total ){

			case 0:
				//FULL CLIP
				rC->rcts[i].w = 0;
				break;

			case 1:
				if( top_in ){
					rC->rcts[i].h = cut.y - rC->rcts[i].y;
				}
				else if( bot_in ){
					rC->rcts[i].y = cut_b;
					rC->rcts[i].h = rC_rcts_i_b - cut_b;
				}
				else if( lef_in ){
					rC->rcts[i].w = cut.x - rC->rcts[i].x;
				}
				else if( rig_in ){
					rC->rcts[i].x = cut_r;
					rC->rcts[i].w = rC_rcts_i_r - cut_r;
				}
				break;

			case 2:
				if( rig_in && bot_in ){//top left corner clipped
					rectCluster_append( rC, rC->rcts[i].x, cut_b, cut_r - rC->rcts[i].x, rC_rcts_i_b - cut_b );
					rC->rcts[i].x = cut_r;
					rC->rcts[i].w = rC_rcts_i_r - cut_r;
				}
				else if( lef_in && bot_in ){//top right corner clipped
					rectCluster_append( rC, cut.x, cut_b, rC_rcts_i_r - cut.x, rC_rcts_i_b - cut_b );
					rC->rcts[i].w = cut.x - rC->rcts[i].x;
				}
				else if( lef_in && top_in ){//bottom right corner clipped
					rectCluster_append( rC, cut.x, rC->rcts[i].y, rC_rcts_i_r - cut.x, cut.y - rC->rcts[i].y );
					rC->rcts[i].w = cut.x - rC->rcts[i].x;
				}
				else if( rig_in && top_in ){// bottom left corner clipped
					rectCluster_append( rC, rC->rcts[i].x, rC->rcts[i].y, cut_r - rC->rcts[i].x, cut.y - rC->rcts[i].y );
					rC->rcts[i].x = cut_r;
					rC->rcts[i].w = rC_rcts_i_r - cut_r;
				}
				else if( lef_in && rig_in ){// vertical slice
					rectCluster_append( rC, cut_r, rC->rcts[i].y, rC_rcts_i_r - cut_r, rC->rcts[i].h );
					rC->rcts[i].w = cut.x - rC->rcts[i].x;
				}
				else if( top_in && bot_in ){// horizontal slice
					rectCluster_append( rC, rC->rcts[i].x, cut_b, rC->rcts[i].w, rC_rcts_i_b - cut_b );
					rC->rcts[i].h = cut.y - rC->rcts[i].y;
				}		
				break;

			case 3:
				if( rig_in && bot_in && top_in ){// Left bite
					rectCluster_append( rC, rC->rcts[i].x, rC->rcts[i].y, cut_r - rC->rcts[i].x, cut.y - rC->rcts[i].y );
					rectCluster_append( rC, rC->rcts[i].x, cut_b, cut_r - rC->rcts[i].x, rC_rcts_i_b - cut_b );
					rC->rcts[i].x = cut_r;
					rC->rcts[i].w = rC_rcts_i_r - cut_r;
				}
				else if( lef_in && bot_in && rig_in ){//top bite
					rectCluster_append( rC, cut.x, cut_b, cut.w, rC_rcts_i_b - cut_b );
					rectCluster_append( rC, cut_r, rC->rcts[i].y, rC_rcts_i_r - cut_r, rC->rcts[i].h );
					rC->rcts[i].w = cut.x - rC->rcts[i].x;
				}
				else if( lef_in && top_in && bot_in ){//right bite
					rectCluster_append( rC, cut.x, rC->rcts[i].y, rC_rcts_i_r - cut.x, cut.y - rC->rcts[i].y );
					rectCluster_append( rC, cut.x, cut_b,  rC_rcts_i_r - cut.x, rC_rcts_i_b - cut_b );
					rC->rcts[i].w = cut.x - rC->rcts[i].x;
				}
				else if( rig_in && top_in && lef_in ){//bottom bite
					rectCluster_append( rC, cut.x, rC->rcts[i].y, cut.w, cut.y - rC->rcts[i].y );
					rectCluster_append( rC, cut_r, rC->rcts[i].y, rC_rcts_i_r - cut_r, rC->rcts[i].h );
					rC->rcts[i].w = cut.x - rC->rcts[i].x;
				}
				break;

			case 4:
				// HOLE
				rectCluster_append( rC, cut.x, rC->rcts[i].y, cut.w, cut.y - rC->rcts[i].y );
				rectCluster_append( rC, cut.x, cut_b,  cut.w, rC_rcts_i_b - cut_b  );
				rectCluster_append( rC, cut_r, rC->rcts[i].y, rC_rcts_i_r - cut_r, rC->rcts[i].h );
				rC->rcts[i].w = cut.x - rC->rcts[i].x;
		
				break;
		}

		//if( rC->rcts[i].w < 0 ) printf("Largura Negativa!\n");
	}
}

int rectCluster_area( rectCluster *rC ){
	int A = 0;
	for (int i = 0; i < rC->len; ++i ){
		A += rect_area( rC->rcts + i );
	}
	return A;
}