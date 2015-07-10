//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// define height and width of rectangle

#define paddleHeight 20
#define paddleWidth 80

// prototypes
void removeGWindow(GWindow gw, GObject gobj);
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
double velocity_x;
double velocity_y;
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{


    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
     velocity_x = 1.5;
     velocity_y = 3.0;
     
     waitForClick();

    // keep playing until game over
    while (lives > 0 && bricks > 0)
     
    {
        updateScoreboard(window, label, points);
        // TODO
        move(ball, velocity_x, velocity_y);   

  
        pause(10);
        
        
        GEvent event = getNextEvent(MOUSE_EVENT);
         if (event != NULL)
        {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
            {
                // ensure paddle follows top cursor
                double x = getX(event) - getWidth(paddle) / 2;
                double y = HEIGHT-paddleHeight;
                setLocation(paddle, x, y);
            }
        }
        
        GObject object = detectCollision(window, ball);
        if (object == paddle)
        {
            velocity_y = -velocity_y;
            }
        else if (strcmp(getType(object), "GRect")  == 0)
        {
            removeGWindow(window, object);
            velocity_y = -velocity_y;
            bricks--;
            points++;
            
       
         }
        
       
        
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        
        {
            velocity_x = -velocity_x;
        }

        else if (getX(ball) <= 0)
        {
            velocity_x = -velocity_x;
        }
        
         else if(getY(ball) + getHeight(ball) >= getHeight(window))
         {
            lives = lives - 1;
            setLocation(ball, (WIDTH/2), (HEIGHT/2));
            setLocation(paddle, ((WIDTH-paddleWidth)/2), ((HEIGHT-paddleHeight)));
            waitForClick();
            
            
        }
        
        else if(getY(ball) + getHeight(ball) <= 0)
        {
        velocity_y= -velocity_y;
        }
        
          
    }
    
    
    
        
        
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // TODO
   int ybrick = 5;
    for(int i = 0; i < ROWS; i++)
     {
     
    
     int xbrick = 5;    
        
    for (int j = 0; j<COLS; j++) {
       GRect brick = newGRect(xbrick+10, ybrick, 32, 8);
        
     if (i == 0)
     {
        
        setColor(brick, "BLUE");
        setFilled(brick, true);
      add(window, brick);
        }
        
      if (i == 1)
     {
        
        setColor(brick, "GREEN");
        setFilled(brick, true);
      add(window, brick);
        }
        
       if (i == 2)
     {
        
        setColor(brick, "YELLOW");
        setFilled(brick, true);
      add(window, brick);
        }
        
       if (i == 3)
        {
        setColor(brick, "RED");
        setFilled(brick, true);
      add(window, brick);
        }
        
       if (i == 4)
     {
        
        setColor(brick, "ORANGE");
        setFilled(brick, true);
      add(window, brick);
        }
        
        xbrick += (WIDTH/11);
        
        
     }
    
      ybrick = ybrick + 20;
     }
     
}


/**
 * Instantiates 
 
ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // TODO
    GOval ball = newGOval((WIDTH/2), (HEIGHT/2), 30, 30);
    setFilled(ball, true);
    setColor(ball, "BLACK");
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    
    GRect paddle = newGRect((WIDTH-paddleWidth)/2, (HEIGHT-paddleHeight), paddleWidth, paddleHeight);
    setFilled(paddle, true);
    setColor(paddle, "BLUE");
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // TODO
    
    GLabel label = newGLabel(" ");
    setFont(label, "Sanserif-36");
    setColor(label, "RED");
    setLocation(label, 150, 300);
  add(window, label);
    
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
