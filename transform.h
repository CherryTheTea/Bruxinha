#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include "basics.h"
#include "vec2d.h"
//#include <cpVect.h>

typedef struct transform_struct{
	double tx, ty; // translate
    int cx, cy;    // center, a secondary translate on top of the scale
    double s, invs;// scale, inverse scale
} Transform;

void set_scale( Transform *T, double s );

double atfX( double x, Transform *T );
double atfY( double y, Transform *T );

double rtfX( double x, Transform *T );
double rtfY( double y, Transform *T );

vec2d apply_transform_v2d( vec2d *vec, Transform *T );
vec2d reverse_transform_v2d( vec2d *vec, Transform *T );

//cpVect apply_transform_cpv( cpVect vec, Transform *T );
//cpVect reverse_transform_cpv( cpVect vec, Transform *T );

SDL_Rect apply_transform_rect( SDL_Rect *rct, Transform *T );
SDL_Rect reverse_transform_rect( SDL_Rect *rct, Transform *T );

SDL_FRect apply_transform_frect( SDL_FRect *rct, Transform *T );
SDL_FRect reverse_transform_frect( SDL_FRect *rct, Transform *T );
#endif