#include "vec2d.h"

double v2d_rough_heading8(vec2d v){
    if( v.x > 0 ){
        if( v.y > 0 ){
            if( v.x < v.y ) return 1.1780972450961724;// 3/8 PI
            else            return 0.3926990816987241;// 1/8 PI;
        }
        else{//v.y < 0
            if( v.x < -v.y ) return -1.1780972450961724;// -3/8 PI
            else             return -0.3926990816987241;// -1/8 PI
        }
    }
    else{//v.x < 0
        if( v.y > 0 ){
            if( -v.x < v.y ) return 1.9634954084936207;// 5/8 PI
            else             return 2.7488935718910690;// 7/8 PI
        }
        else{//v.y < 0
            if( v.x > v.y ) return -1.9634954084936207;// 5/8 PI
            else            return -2.7488935718910690;// 7/8 PI
        }
    }
}

double v2d_rough_heading16(vec2d v){
    if( v.x > 0 ){
        if( v.y > 0 ){
            if( v.x < v.y ){
                //  tan(3/8 PI)
                if( 2.414213 * v.x < v.y ) return 1.3744467859455345;// 7/16 PI
                                      else return 0.9817477042468103;// 5/16 PI
            }
            else{// tan(1/8 PI)
                if( 0.414213 * v.x < v.y ) return 0.5890486225480862;// 3/16 PI
                                      else return 0.1963495408493620;// 1/16 PI
            }
        }
        else{//v.y < 0
            if( v.x < -v.y ){
                //  tan(3/8 PI)
                if( 2.414213 * v.x < -v.y ) return -1.3744467859455345;// -7/16 PI
                                       else return -0.9817477042468103;// -5/16 PI
            }
            else{// tan(1/8 PI)
                if( 0.414213 * v.x < -v.y ) return -0.5890486225480862;// -3/16 PI
                                       else return -0.1963495408493620;// -1/16 PI
            }
        }
    }
    else{//v.x < 0
        if( v.y > 0 ){
            if( -v.x < v.y ){
                //  -tan(3/8 PI)
                if( -2.414213 * v.x < v.y ) return 1.7671458676442586;// 9/16 PI
                                       else return 2.1598449493429828;// 11/16 PI
            }
            else{// -tan(1/8 PI)
                if( -0.414213 * v.x < v.y ) return 2.5525440310417070;// 13/16 PI
                                       else return 2.9452431127404311;// 15/16 PI
            }
        }
        else{//v.y < 0
            if( v.x > v.y ){
                //  tan(3/8 PI)
                if( 2.414213 * v.x > v.y ) return -1.7671458676442586;// -9/16 PI
                                      else return -2.1598449493429828;// -11/16 PI
            }
            else{// tan(1/8 PI)
                if( 0.414213 * v.x > v.y ) return -2.5525440310417070;// -13/16 PI
                                      else return -2.9452431127404311;// -15/16 PI
            }
        }
    }
}

double v2d_rough_heading32(vec2d v){
    if( v.x > 0 ){
        if( v.y > 0 ){
            if( v.x < v.y ){
                // tan(3/8 PI)
                if( 2.414213 * v.x < v.y ){
                    // tan(7/16 PI)
                    if( 5.027339 * v.x < v.y ) return 1.4726215563702154;//15/32 PI
                                          else return 1.2762720155208536;//13/32 PI
                }
                else{
                    // tan(5/16 PI)
                    if( 1.496605 * v.x < v.y ) return 1.0799224746714913;//11/32 PI
                                          else return 0.8835729338221293;// 9/32 PI
                }
            }
            else{// tan(1/8 PI)
                if( 0.414213 * v.x < v.y ){
                    // tan(3/16 PI)
                    if( 0.668178 * v.x < v.y ) return 0.6872233929727672;// 7/32 PI
                                          else return 0.4908738521234052;// 5/32 PI
                }
                else{
                    // tan(1/16 PI)
                    if( 0.198912 * v.x < v.y ) return 0.2945243112740431;// 3/32 PI
                                          else return 0.0981747704246810;// 1/32 PI
                }
            }
        }
        else{//v.y < 0
           if( v.x < -v.y ){
                // tan(3/8 PI)
                if( 2.414213 * v.x < -v.y ){
                    // tan(7/16 PI)
                    if( 5.027339 * v.x < -v.y ) return -1.4726215563702154;//-15/32 PI
                                           else return -1.2762720155208536;//-13/32 PI
                }
                else{
                    // tan(5/16 PI)
                    if( 1.496605 * v.x < -v.y ) return -1.0799224746714913;//-11/32 PI
                                           else return -0.8835729338221293;// -9/32 PI
                }
            }
            else{// tan(1/8 PI)
                if( 0.414213 * v.x < -v.y ){
                    // tan(3/16 PI)
                    if( 0.668178 * v.x < -v.y ) return -0.6872233929727672;// -7/32 PI
                                           else return -0.4908738521234052;// -5/32 PI
                }
                else{
                    // tan(1/16 PI)
                    if( 0.198912 * v.x < -v.y ) return -0.2945243112740431;// -3/32 PI
                                           else return -0.0981747704246810;// -1/32 PI
                }
            } 
        }
    }
    else{//v.x < 0
        if( v.y > 0 ){
            if( -v.x < v.y ){
                //  -tan(3/8 PI)
                if( -2.414213 * v.x < v.y ){
                    // -tan(7/16 PI) 
                    if( -5.027339 * v.x < v.y ) return 1.6689710972195777;//17/32 PI
                                           else return 1.8653206380689396;//19/32 PI
                }
                else{
                    // -tan(5/16 PI)
                    if( -1.496605 * v.x < v.y ) return 2.0616701789183018;//21/32 PI
                                           else return 2.2580197197676637;//23/32 PI
                }
            }
            else{// -tan(1/8 PI)
                if( -0.414213 * v.x < v.y ){
                    //  -tan(3/16 PI)
                    if( -0.668178 * v.x < v.y ) return 2.4543692606170260;//25/32 PI
                                           else return 2.6507188014663878;//27/32 PI
                }
                else{
                    //  -tan(1/16 PI)
                    if( -0.198912 * v.x < v.y ) return 2.8470683423157501;//29/32 PI
                                           else return 3.0434178831651120;//31/32 PI
                }
            }
        }
        else{//v.y < 0
            if( v.x > v.y ){
                //  tan(3/8 PI)
                if( 2.414213 * v.x > v.y ){
                    // tan(7/16 PI)
                    if( 5.027339 * v.x > v.y ) return -1.6689710972195777;//-17/32 PI
                                          else return -1.8653206380689396;//-19/32 PI
                }
                else{
                    // tan(5/16 PI)
                    if( 1.496605 * v.x > v.y ) return -2.0616701789183018;//-21/32 PI
                                          else return -2.2580197197676637;//-23/32 PI
                }
            }
            else{// tan(1/8 PI)
                if( 0.414213 * v.x > v.y ){
                    //  tan(3/16 PI)
                    if( 0.668178 * v.x > v.y ) return -2.4543692606170260;//-25/32 PI
                                          else return -2.6507188014663878;//-27/32 PI
                }
                else{
                    //  tan(1/16 PI) 
                    if( 0.198912 * v.x > v.y ) return -2.8470683423157501;//-29/32 PI
                                          else return -3.0434178831651120;//-31/32 PI
                }
            }
        }
    }
}

double v2d_inside_angle( vec2d *V ){
    double d0 = v2d_distsq( V[1], V[0] );
    double d2 = v2d_distsq( V[1], V[2] );
    return acos( ( d0 + d2 - v2d_distsq(V[0], V[2]) ) / (2 * sqrt(d0) * sqrt(d2)) );
}

void v2d_rotate( vec2d *a, float theta ){
    double px = a->x;
    double co = cos(theta);
    double si = sin(theta);
    a->x = a->x * co - a->y * si;
    a->y = px * si + a->y * co;
}

vec2d v2d_rotation( vec2d a, float theta ){
    double px = a.x;
    double co = cos(theta);
    double si = sin(theta);
    a.x = a.x * co - a.y * si;
    a.y = px * si + a.y * co;
    return a;
}

vec2d v2d_medicenter( vec2d *list, int len ){
    vec2d min = v2d(  9999999,  9999999 );
    vec2d max = v2d( -9999999, -9999999 );
    for (int i = 0; i < len; ++i ){
        if( list[i].x < min.x ) min.x = list[i].x;
        if( list[i].y < min.y ) min.y = list[i].y;
        if( list[i].x > max.x ) max.x = list[i].x;
        if( list[i].y > max.y ) max.y = list[i].y;
    }
    return v2d_lerp( min, max, 0.5 );
}