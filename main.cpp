#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> 
#include <time.h>
#include <iostream>

using namespace sf;

//          VARIABLES

const int row = 35;
const int col = 70;
int score=0;

bool won=false;

int grid[row][col] = {0};
int ts = 18; // tile size

float totaltime = 90; 
Font font;
Text text;


//          BACKGROUND MUSIC

class SoundManager {

private:
  
  Music bgMusic;
  Sound collisionSound;
  SoundBuffer collisionBuffer;

public:

  void init() 
  {
    // Load background music 
    bgMusic.openFromFile("audio/a1.ogg");
    bgMusic.setLoop(true);
    bgMusic.setVolume(70);
    bgMusic.play();
  }

};

SoundManager sounds;

void musicplay() 
{
  sounds.init();  
}



struct Enemy
{
  int x, y, dx, dy;

  Enemy()
  {
    x = 500;
    y = 300;
    dx = 4 - rand() % 8;
    dy = 4 - rand() % 8;
  }

  void EnemyMovement()
  {
    x =x+ dx;

    if(grid[y/ts][x/ts]==1)  // enemy got hit the walls
    {
      dx = -dx;
      x = x+dx;
    }

    y= y+dy;
     
    if (grid[y/ts][x/ts]==1)  
    {
      dy = -dy;
      y = y+dy;
    }
  }
};


//          FLOOD FILL (CONVERT TILES TO WALLS)

void Tiles_to_Walls(int y, int x)
{
  if (y < 0 || y >= row || x < 0 || x >= col) return;
  if (grid[y][x] == 0)   grid[y][x]= -1;
  if (grid[y-1][x] == 0) Tiles_to_Walls(y - 1, x);
  if (grid[y+1][x] == 0) Tiles_to_Walls(y + 1, x);
  if (grid[y][x-1] == 0) Tiles_to_Walls(y, x - 1);
  if (grid[y][x+1] == 0) Tiles_to_Walls(y, x + 1);
}



int main()
{
  srand(time(0));

  bool Game = false; //  game off
  bool menu = true;  //  menu on

  musicplay();
  
  int enemyCount = 0;

  Enemy e1[10];    // object of enemy struct

  int x = 0, y = 0;
  int dx = 0, dy = 0;

  float timer = 0, delay = 0.07;
  Clock clock;


  //           GAME WINDOW

  RenderWindow window(VideoMode(col * ts, row * ts), "Muanna Hamid");
  window.setFramerateLimit(60);

  
  //           IMPORT TEXT
  
  font.loadFromFile("fonts/Roboto-Regular.ttf");

  text.setFont(font);
  text.setCharacterSize(20);
  text.setFillColor(Color::White);
  text.setStyle(Text::Bold);
  text.setPosition(20, 20); // top-left corner


  //           IMPORT IMAGES

  Texture t1, t2, t3, t4, t5, t6, t7, t8;
  t1.loadFromFile("images/tiles.png");
  t2.loadFromFile("images/gameover.png"); 
  t3.loadFromFile("images/enemy.png") ;
  t4.loadFromFile("images/menu.png") ;
  t5.loadFromFile("images/easy.png") ;
  t6.loadFromFile("images/medium.png") ;
  t7.loadFromFile("images/hard.png");  
  t8.loadFromFile("images/win.png");   

  Sprite sTile(t1), sGameover(t2), sEnemy(t3), sMenu(t4), sEasy(t5), sMedium(t6), sDifficult(t7), sWin(t8);

  //           POSITION

  sGameover.setPosition(400, 100);
  sEnemy.setOrigin(20, 20);
  sMenu.setPosition(450, 49);
  sEasy.setPosition(477, 250);
  sMedium.setPosition(477, 330);
  sDifficult.setPosition(477, 405);
  sWin.setPosition(450, 100);


  //           RESIZE

  sMenu.setScale(0.4f, 0.4f);
  sEasy.setScale(0.1f, 0.1f);       
  sMedium.setScale(0.1f, 0.1f);
  sDifficult.setScale(0.1f, 0.1f);
  sWin.setScale(0.4f, 0.4f);

  //           GRID WALLS 

  for(int i=0; i<row; i++)
  {
    for(int j=0; j<col; j++)
    {
      if(i==0 || j==0 || i==row-1 || j==col-1)
      {
        grid[i][j] = 1;
      } 
    }
  }


  //          GAME LOOP

  while (window.isOpen())
  {
      float time = clock.getElapsedTime().asSeconds();
      clock.restart();
      timer += time;

      Event e;

      while(window.pollEvent(e))
      {
        if(e.type == Event::Closed) window.close();

        if(e.type == Event::KeyPressed)
        {

          //       ESCAPE KEY HANDLER

          if(e.key.code == Keyboard::Escape)
          {
            if(!menu) 
            {
              //        GRID INITIALLIZE

              for(int i=1; i<row-1; i++)
              {
                for(int j=1; j<col-1; j++)
                {
                  grid[i][j] = 0;
                }
                    
              }

              x = 10;
              y = 0;
              Game = false;
              menu = true;
            }

          }

          //        DIFFICULTY SELECTION

          else if (menu)
          {

            if (e.key.code == Keyboard::Num1 || e.key.code == Keyboard::Numpad1)
            {
              enemyCount = 4; // Easy
              menu = false;
              Game = true;
            }

            else if (e.key.code == Keyboard::Num2 || e.key.code == Keyboard::Numpad2)
            {
              enemyCount = 7; // Medium
              menu = false;
              Game = true;
            }

            else if (e.key.code == Keyboard::Num3 || e.key.code == Keyboard::Numpad3)
            {
              enemyCount = 9; // Hard
              menu = false;
              Game = true;
            }

          }

        }
      }

      //          PLAYER'S MOVEMENTS

      if (Game && !menu)
      {
        if (Keyboard::isKeyPressed(Keyboard::Left))  
        { 
          dx = -1;
          dy = 0; 
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) 
        { 
          dx = 1; 
          dy = 0; 
        }

        if (Keyboard::isKeyPressed(Keyboard::Up))    
        { 
          dx = 0;
          dy = -1; 
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))  
        { 
          dx = 0;
          dy = 1; 
        }

        if(timer > delay)
        {
          x += dx;
          y += dy;

          if(x < 0)
          {
            x = 0;
          } 

          if(x > col-1)
          {
            x = col-1;
          } 

          if(y < 0) 
          {
            y = 0;
          }

          if(y > row-1)
          {
            y = row-1;
          }

          if(grid[y][x] == 0)
          {
            grid[y][x] = 2;

            score=score+1; 

              if(score >= 100) // 🏆 
              {
                Game = false;
                won = true;
              }
          } 

          else if(grid[y][x] == 2) 
          {
            Game = false; // 💀GAME OVER 1 (PLAYER TO ENEMY COLLISION)
          }

          timer = 0;
        }

        //     ENEMY MOVEMENT

        for (int i = 0; i < enemyCount; i++) 
        {
          e1[i].EnemyMovement();  
        }


        if(grid[y][x] == 1) // if reach wall
        {
          dx = dy = 0;

          for (int i=0; i<enemyCount; i++)
          {
            Tiles_to_Walls(e1[i].y / ts, e1[i].x / ts);// block for all enemy
          }
            
          for (int i = 0; i < row; i++)
          {
            for (int j = 0; j < col; j++)
            {
              if(grid[i][j] == -1) 
              {
                grid[i][j] = 0;
              }

              else grid[i][j] = 1;
            }
          }
                
        }

        for (int i =0; i<enemyCount; i++)
        {
          if (grid[e1[i].y / ts][e1[i].x / ts] == 2) Game = false;  // 💀GAME OVER 2 (ENEMY TO PLAYER COLLISION)
        }
              
      }

      //        DRAWING TO WINDOW

      window.clear();

      if(menu)
      {
        window.draw(sMenu);
        window.draw(sEasy);
        window.draw(sMedium);
        window.draw(sDifficult);
      }
      else if(Game)
      {
        for(int i=0; i<row; i++)
        {
          for(int j=0; j<col; j++)
          {
            if (grid[i][j] == 0) continue;
            if (grid[i][j] == 1) sTile.setTextureRect(IntRect(0, 0, ts, ts));
            if (grid[i][j] == 2) sTile.setTextureRect(IntRect(54, 0, ts, ts));
            sTile.setPosition(j * ts, i * ts);
            window.draw(sTile);
          }
        }
            

        sTile.setTextureRect(IntRect(36, 0, ts, ts));
        sTile.setPosition(x * ts, y * ts);
        window.draw(sTile);

        sEnemy.rotate(10);

        for (int i = 0; i < enemyCount; i++)
        {
          sEnemy.setPosition(e1[i].x, e1[i].y);
          window.draw(sEnemy);
        }

        text.setString("Score: " + std::to_string(score));
        window.draw(text);
      }

      //         WIN / LOSE DISPLAY

      else if(!Game && !menu)
      {
        if(won)
        {
          window.draw(sWin);
        }

        else
        {
          window.draw(sGameover);
        }
        
      }

      window.display();
  }

  return 0;
}
