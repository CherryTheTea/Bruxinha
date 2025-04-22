import processing.javafx.*;


Map[] world;
Entity Player;

float transX, transY;
boolean dynamic_tx, dynamic_ty;
float cx, cy;
int currentMap = 0;
int N;
float L;
boolean w, a, s, d;

void setup(){
  size( 600, 600, FX2D );  //1200, 700, FX2D ); // fullScreen(FX2D); //
  textSize(8);
  frameRate(60);
  //surface.setResizable(true);
  cx = width/2.0f;
  cy = height/2.0f;
  
  N = 20;
  L = width/N;

  String it = "01";
  
  File file = new File(sketchPath()+"\\data\\worlds\\"+it);
  String[] l = file.list();
  PImage[] rooms = new PImage[0];
  String[] conns = new String[0];
  for(int i = 0; i < l.length; i++){
    String[] q = split( l[i], '.' );
    if( q[q.length-1].equals("png") ){
      rooms = (PImage[]) append( rooms, loadImage( "\\worlds\\"+it+"\\"+l[i] ));
    }
    else if( q[0].equals("connections") ){
      conns = loadStrings( "\\worlds\\"+it+"\\"+l[i] );
    }
  }
  
  world = new Map[rooms.length];
  for(int o = 0; o < world.length; o++){
    world[o] = new Map( rooms[o].width, rooms[o].height);
    for(int i = 0; i < rooms[o].width; i++){
      for(int j = 0; j < rooms[o].height; j++){
        switch( rooms[o].get(i, j) ){
          case 0xff000000:
            world[o].map[i][j] = new Tile( color(0), true );
            break;
          case 0xffFFFFFF:
            world[o].map[i][j] = new Tile( color(255), false );
            break;
          case 0xff7F7F7F:
            world[o].map[i][j] = new Tile( color(127), true );
            break;
          case #786446:
            world[o].map[i][j] = new Tile( #786446, true );
            break;
          case 0xffFF7F00:
            float[] d = find_connection( o, i, j, conns );
            if( d.length == 3 ) world[o].map[i][j] = new Path( round(d[0]), d[1], d[2] );
            else {
              world[o].map[i][j] = new Tile( 0xffFF7F00, false );
              println( "unconnected path: " + o + ": " + i + "x" + j );
            }
            break;
          default:
            world[o].map[i][j] = new Tile( rooms[o].get(i, j), false );
            break;
        }
        
      }
    }
  }
  configure_translation();
  
  Player = new Entity();
  Player.load_sprites( loadImage("walk.png"), loadImage("idle.png"), 3*L, 0.5, 0.85 );
  int[] x = { 11, 10, 40, 20 };
  int[] y = { 25, 123, 48, 20 };
  Player.pos = new PVector( x[int(it)-1] * L, y[int(it)-1] * L );
  
  /*
  boolean[][] solids = new boolean[N][N];
  for(int i = 0; i < N; i++) for(int j = 0; j < N; j++) solids[i][j] = false;
  for(int i = 0; i < 50; i++) solids[round(random( -0.499, N-0.501 ))][round(random( -0.499, N-0.501 ))] = true;
  solids[0][0] = false;
  world[currentMap] = new Map( N, N, solids );
  */
}

public void draw(){
  background(0);
  
  stroke( 180 ); 
  translate( transX, transY );
  
  world[currentMap].display();
  
  PVector dir = new PVector();
  if( Player.pathing ){
    dir = Player.path_dir();
  }
  else{
    if(w) dir.add(0, -1);
    if(a) dir.add(-1, 0);
    if(s) dir.add(0, 1);
    if(d) dir.add(1, 0);
    dir.normalize();
  }
  PVector[] newPos = Player.movement( dir );
  int[] I = new int[4]; for(int i = 0; i < 4; i++) I[i] = floor( newPos[i].x / L );
  int[] J = new int[4]; for(int i = 0; i < 4; i++) J[i] = floor( newPos[i].y / L );
  
  boolean blocked = false;
  int blocks = 0;
  for(int i = 0; i < 4; i++){
    int y = floor(Player.corners[i].y / L);
    if( I[i] >= 0 && I[i] < world[currentMap].map.length){
      if( world[currentMap].map[I[i]][y].solid ){
        blocked = true;
        break;
      }
    }
    else blocked = true;
  }
  if( blocked ) ++blocks;
  else  Player.moveX( dir );
  
  blocked = false;
  for(int i = 0; i < 4; i++){
    int x = floor(Player.corners[i].x / L);
    if( J[i] >= 0 && J[i] < world[currentMap].map[0].length){
      if( world[currentMap].map[x][J[i]].solid ){
        blocked = true;
        break;
      }
    }
    else blocked = true;
  }
  if( blocked ) ++blocks;
  else Player.moveY( dir );
  
  if( blocks == 2 && Player.pathing ) Player.recalculate_path();

  
  stroke(0);
  Player.display();
  
  //println( Player.current_I, Player.current_J );
  Tile current_tile = world[currentMap].map[Player.current_I][Player.current_J];
  if( current_tile instanceof Path ){
    float[] n = current_tile.destination();
    currentMap = PApplet.parseInt( n[0] );
    Player.pos = new PVector( n[1]*L, n[2]*L );
    Player.refresh_corners();
    configure_translation();
  }
  
  update_translation();
  
  //println( frameRate );
  //println( transX, transY, Player.pos.x, Player.pos.y );
}
