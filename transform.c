#include "transform.h"

void set_scale( Transform *T, double s ){
	T->s = s;
	T->invs = 1 / s;
}

double atfX( double x, Transform *T ){
	return T->cx + ( T->s * (x - T->tx) );
}
double atfY( double y, Transform *T ){
	return T->cy + ( T->s * (y - T->ty) );
}
double rtfX( double x, Transform *T ){
	return ((x - T->cx) * T->invs) + T->tx;
}
double rtfY( double y, Transform *T ){
	return ((y - T->cy) * T->invs) + T->ty;
}

vec2d apply_transform_v2d( vec2d *vec, Transform *T ){
	return (vec2d){ T->cx + (T->s * (vec->x - T->tx)), T->cy + (T->s * (vec->y - T->ty)) };
}

vec2d reverse_transform_v2d( vec2d *vec, Transform *T ){
	return (vec2d){ ((vec->x - T->cx) * T->invs) + T->tx, ((vec->y - T->cy) * T->invs) + T->ty };
}

//cpVect apply_transform_cpv( cpVect vec, Transform *T ){
//	return (cpVect){ T->cx + (T->s * (vec.x - T->tx)), T->cy + (T->s * (vec.y - T->ty)) };
//}
//cpVect reverse_transform_cpv( cpVect vec, Transform *T ){
//	return (cpVect){ ((vec.x - T->cx) * T->invs) + T->tx, ((vec.y - T->cy) * T->invs) + T->ty };
//}

SDL_Rect apply_transform_rect( SDL_Rect *rct, Transform *T ){
	return (SDL_Rect){ T->cx + (T->s * (rct->x - T->tx)), T->cy + (T->s * (rct->y - T->ty)), 
					   rct->w * T->s, rct->h * T->s };
}

SDL_Rect reverse_transform_rect( SDL_Rect *rct, Transform *T ){
	return (SDL_Rect){ ((rct->x - T->cx) * T->invs) + T->tx, ((rct->y - T->cy) * T->invs) + T->ty,
						rct->w * T->invs, rct->h * T->invs };
}

SDL_FRect apply_transform_frect( SDL_FRect *rct, Transform *T ){
	return (SDL_FRect){ T->cx + (T->s * (rct->x - T->tx)), T->cy + (T->s * (rct->y - T->ty)), 
						rct->w * T->s, rct->h * T->s };
}

SDL_FRect reverse_transform_frect( SDL_FRect *rct, Transform *T ){
	return (SDL_FRect){ ((rct->x - T->cx) * T->invs) + T->tx, ((rct->y - T->cy) * T->invs) + T->ty,
						rct->w * T->invs, rct->h * T->invs };
}

/*
Poly apply_transform_Poly( Poly *P, Transform *T ){
	Poly out;
	malloc_Poly( &out, P->N );
	for (int i = 0; i < P->N; ++i){
		out.V[i] = apply_transform_v2d( P->V + i, T );
	}
	return out;
}

Poly reverse_transform_Poly( Poly *P, Transform *T ){
	Poly out;
	malloc_Poly( &out, P->N );
	for (int i = 0; i < P->N; ++i){
		out.V[i] = reverse_transform_v2d( P->V + i, T );
	}
	return out;
}*/