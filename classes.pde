class Entity{
  PVector pos, ppos, facing, sprite;
  PVector[] corners;
  float speed;
  int current_I, current_J, stuck;//, next_I, next_J;
  ArrayList<PVector> path;
  boolean pathing, nexttime;
  
  float rad, dia;
  PImage walking[][];
  PImage idleing[][];
  int frame, frame_step, nMillis;
  float sprite_scale;
  
  boolean fire_at_will; // as opposed to hold_fire, or 'passive mode'
  boolean hold_position; // as opposed to auto-retreat
  Entity(){
    pos = new PVector(L/2f, L/2f);
    facing = new PVector();
    speed = 3;
    dia = 0.7*L;
    rad = dia/2f;
    corners = new PVector[4];
    refresh_corners();
  }
  void load_sprites( PImage walk, PImage idle, float actual_size, float xt, float yt ){
    walking = new PImage[8][8];
    idleing = new PImage[4][8];
    float w = walk.width / 8.0;
    float h = walk.height / 8.0;
    for(int i = 0; i < 8; i++){
      for(int j = 0; j < 8; j++){
        walking[i][j] = walk.get( round(i*w), round(j*h), round(w), round(h) );
      }
    }
    w = idle.width / 4.0;
    h = idle.height / 8.0;
    for(int i = 0; i < 4; i++){
      for(int j = 0; j < 8; j++){
        idleing[i][j] = idle.get( round(i*w), round(j*h), round(w), round(h) );
      }
    }
    frame = 0;
    frame_step = 95;// 5-> 46;
    sprite_scale = (actual_size)/idleing[0][0].height;
    w *= sprite_scale;
    h *= sprite_scale;
    for(int i = 0; i < 8; i++) for(int j = 0; j < 8; j++) walking[i][j].resize( floor(w), floor(h) );
    for(int i = 0; i < 4; i++) for(int j = 0; j < 8; j++) idleing[i][j].resize( floor(w), floor(h) );
    Player.sprite = new PVector( w * xt, h * yt );
  }
  void refresh_corners(){
    corners[0] = new PVector( pos.x - rad, pos.y - rad );
    corners[1] = new PVector( pos.x + rad, pos.y - rad );
    corners[2] = new PVector( pos.x + rad, pos.y + rad );
    corners[3] = new PVector( pos.x - rad, pos.y + rad );
  }
  PVector[] movement( PVector dir ){
    if( dir.mag() > 0 ) facing = dir.get();
    else facing.setMag( 0.4 );
    dir.setMag( speed );
    PVector[] out = new PVector[4];
    out[0] = new PVector( corners[0].x + dir.x, corners[0].y + dir.y );
    out[1] = new PVector( corners[1].x + dir.x, corners[1].y + dir.y );
    out[2] = new PVector( corners[2].x + dir.x, corners[2].y + dir.y );
    out[3] = new PVector( corners[3].x + dir.x, corners[3].y + dir.y );
    return out;
  }
  void moveX( PVector dir ){
    pos.x += (dir.x);
    current_I = floor( pos.x / L );
    /* snapping
    if( pathing ){
      if( abs(dir.y) > abs(dir.x) ){
        if( abs( pos.x - ( (current_I+0.5)*L ) ) < speed ) pos.x = (current_I+0.5)*L;
      }
    }
    */
  }
  void moveY( PVector dir ){
    pos.y += (dir.y);
    current_J = floor( pos.y / L );
    /* snapping
    if( pathing ){
      if( abs(dir.x) > abs(dir.y) ){
        if( abs( pos.y - ( (current_J+0.5)*L ) ) < speed ) pos.y = (current_J+0.5)*L;
      }
    }
    */
  }
  void receive_path( float X, float Y ){
    
    ArrayList<Index> Ipath = A_Star( new Index( current_I, current_J ),
                                     new Index( floor( X / L ), floor( Y / L ) ),
                                     world[currentMap] );
    if( Ipath != null ){
      path = new ArrayList();
      
      for( int u = 0; u < Ipath.size(); ++u ) path.add( new PVector( (Ipath.get(u).i + 0.5)*L, (Ipath.get(u).j + 0.5)*L ) );
      
      //print( PVpath.size() );
      //*
      for( int u = 0; u < Ipath.size()-1; ++u ){
        for( int v = Ipath.size()-1; v >= u+1 ; --v ){
          ArrayList<PVector> line = Bresenham_plus( Ipath.get(u).i, Ipath.get(u).j, Ipath.get(v).i, Ipath.get(v).j );
          boolean clear = true;
          for( int z = 0; z < line.size(); ++z ){
            line.get(z).mult( L );
            if( world[currentMap].map[ floor( line.get(z).x / L ) ][ floor( line.get(z).y / L ) ].solid ){
              clear = false;
              break;              
            }
          }
          if( clear ){
            for( int z = v-1; z > u; --z ){
              Ipath.remove( z );
              path.remove( z );
              --v;
            }
          }
        }
      }
      //*/
      //println(  " : "+ PVpath.size() );
      pathing = true;
      ppos = pos.get();
    }
  }
  void recalculate_path(){
    this.receive_path( path.get(0).x, path.get(0).y );
    println( "recalculating... " + frameCount );
  }
  PVector path_dir(){
    if( dist( pos.x, pos.y, path.get(path.size()-1).x, path.get(path.size()-1).y ) <= speed ) nexttime = true;
    if( nexttime ){
      path.remove( path.size()-1 );
      if( path.size() == 0 ){
        pathing = false;
      }
      nexttime = false;
    }
    
    if( PVector.sub( pos, ppos ).mag() < speed*0.2 ) stuck++;
    else stuck = 0;
    
    if( stuck >= 6 ){
      recalculate_path();
      stuck = 0;
    }
    //if( stuck > 0 ) print( stuck +", " );
    ppos = pos.get();
    /*
    if( current_I == next_I && current_J == next_J ){
      path.remove( path.size()-1 );
      if( path.size() == 0 ){
        pathing = false;
      }
      else{
        next_I = floor( path.get(path.size()-1).x / L );
        next_J = floor( path.get(path.size()-1).y / L );
      }
    }
    */
    if( pathing ){
      return PVector.sub( path.get(path.size()-1), pos ).normalize();
      /*
      PVector dir = new PVector();
      if( path.get(path.size()-1).x > pos.x ) dir.add( 1, 0 );
      else if( path.get(path.size()-1).x < pos.x ) dir.add( -1, 0 );

      if( path.get(path.size()-1).y > pos.y ) dir.add( 0, 1 );
      else if( path.get(path.size()-1).y < pos.y ) dir.add( 0, -1 );
      
      return dir.normalize();
      */
    }
    else return new PVector();
  }
  void display(){
    refresh_corners();
    /*
    fill(#02A7F7);
    ellipse(pos.x, pos.y, dia, dia );
    */
    int j = round(facing.heading() / QUARTER_PI) + 4;
    if( j == 8 ) j = 0;
    if( facing.mag() > 0.5 ){
      image( walking[frame][j], pos.x - sprite.x, pos.y - sprite.y );
    }
    else{
      if( frame > 3 ) frame -= 4;
      image( idleing[frame][j], pos.x - sprite.x, pos.y - sprite.y );
    }
    if( millis() >= nMillis ){
      if( frame == 7 ) frame = 0;
      else frame++;
      nMillis = millis() + frame_step;
    }
  }
}

//0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•|
//0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•0•|

class Map{
  Tile[][] map;
  Map( int I, int J ){
    map = new Tile[I][J];
  }
  /*
  Map( int I, int J, boolean[][] s){
    map = new Tile[I][J];
    for(int i = 0; i < map.length; i++) for(int j = 0; j < map[0].length; j++) map[i][j] = new Tile( s[i][j] );
  }
  */
  int rows(){ return map.length; }
  int columns(){ return map[0].length; }
  float width(){ return map.length * L; }
  float height(){ return map[0].length * L; }
  void display(){
    int startX = constrain( floor(-transX/L), 0, map.length-1 );
    int stopX = constrain(startX + N + 2, 0, map.length );
    int startY = constrain( floor(-transY/L), 0, map[0].length-1 );
    int stopY = constrain(startY + N + 2, 0, map[0].length );
    //println( transX, transY, startX, stopX, startY, stopY );
    for(int i = startX; i < stopX; i++){
      for(int j = startY; j < stopY; j++){
        fill(map[i][j].c);
        rect( i*L, j*L, L, L );
        //fill(0);
        //text( i+", "+j, i*L, j*L );
      }
    }
  }
  void display( PGraphics pg, float l ){
    for(int i = 0; i < map.length; i++){
      for(int j = 0; j < map[0].length; j++){
        pg.fill(map[i][j].c);
        pg.rect( i*l, j*l, l, l );
      }
    }
  }
}

class Tile{
  PImage sprite;
  boolean solid;
  color c;
  Tile( color c, boolean s ){
    this.c = c;
    solid = s; 
  }
  float[] destination() { return null; }
}

class Path extends Tile{
  int dest;
  float x, y;
  Path( int d, float x, float y ){
    super( color(255, 127, 0 ), false );
    dest = d;
    this.x = x;
    this.y = y;
  }
  float[] destination(){
    float[] out = { float(dest), x, y };
    return out;
  }
}