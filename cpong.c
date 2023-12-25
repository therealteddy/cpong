/* 
Author: Ted Josh Issac Jerin 
Date: 24-Dec-23 
Desc: A pong implementation in c and raylib 
*/

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h> 
#include <stdbool.h>

// Window Settings
#define WINDOW_H 1000 
#define WINDOW_W 1000 
#define WINDOW_T "CPong!" 
#define WINDOW_FPS 120

// Ball configurations
#define BALL_SIZE 10 
#define BALL_COLOR WHITE
#define BALL_VELOCITY_VERTICAL 200
#define BALL_VELOCITY_HORIZONTAL 200

// Panel Settings
#define LEFT_PANEL_H 250 
#define LEFT_PANEL_W 25 
#define LEFT_PANEL_COLOR WHITE 
#define LEFT_PANEL_XPOS 25 
#define LEFT_PANEL_YPOS (WINDOW_H/2)-(LEFT_PANEL_H/2)

#define RIGHT_PANEL_H 250 
#define RIGHT_PANEL_W 25 
#define RIGHT_PANEL_COLOR WHITE 
#define RIGHT_PANEL_XPOS (WINDOW_W-50)
#define RIGHT_PANEL_YPOS (WINDOW_H/2)-(RIGHT_PANEL_H/2)

// Score board settings
#define SCORE_FONT_SIZE 50
#define SCORE_LEFT_POS_X (WINDOW_W/4)-50
#define SCORE_LEFT_POS_Y 0 
#define SCORE_RIGHT_POS_X ((3*WINDOW_W)/4)-50
#define SCORE_RIGHT_POS_Y 0
#define SCORE_FONT_COLOR WHITE
#define SCORE_INCREMENT 1L

// Game-state: idle text settings 
#define GAMESTATE_IDLE_START_TEXT "Press the Space Bar to Start!"
#define GAMESTATE_IDLE_TEXT_XPOS (WINDOW_W/10)
#define GAMESTATE_IDLE_TEXT_YPOS (WINDOW_H/02)
#define GAMESTATE_IDLE_TEXT_FONTSIZE 50 
#define GAMESTATE_IDLE_TEXT_COLOR WHITE

// Endscreen score settings
#define GAMESTATE_IDLE_PLAYER1_SCORE_XPOS (WINDOW_W/10)
#define GAMESTATE_IDLE_PLAYER1_SCORE_YPOS ((3*WINDOW_H)/4)
#define GAMESTATE_IDLE_PLAYER2_SCORE_XPOS (625)
#define GAMESTATE_IDLE_PLAYER2_SCORE_YPOS ((3*WINDOW_H)/4)
#define GAMESTATE_IDLE_PLAYER1_SCORE_TEXT_FONTSIZE 50
#define GAMESTATE_IDLE_PLAYER2_SCORE_TEXT_FONTSIZE 50 
#define GAMESTATE_IDLE_PLAYER1_SCORE_TEXT_COLOR WHITE
#define GAMESTATE_IDLE_PLAYER2_SCORE_TEXT_COLOR WHITE

// End Game states 
#define ENDGAME_TEXT_FONTSIZE 50 
#define ENDGAME_TEXT_COLOR WHITE

#define ENDGAME_TIE_TEXT "Its a tie!"
#define ENDGAME_TIE_TEXT_XPOS 400 
#define ENDGAME_TIE_TEXT_YPOS (WINDOW_H/4)

#define ENDGAME_PLAYER1_WIN_TEXT "Player A is Victorious!"
#define ENDGAME_PLAYER2_WIN_TEXT "Player B is Victorious!"
#define ENDGAME_PLAYER_WIN_TEXT_XPOS 250 
#define ENDGAME_PLAYER_WIN_TEXT_YPOS (WINDOW_H/4)

typedef struct {
    unsigned int radius;
    Color color; 
    Vector2 position; 
} circle_t; 

typedef struct {
    circle_t circle;
    Vector2 velocity; 
} ball_t;

// Return a rectangle structure
Rectangle rec(unsigned int width, unsigned int height, int xpos, int ypos); 

// Update panels on keypress 
void move_panels_on_keypress(Rectangle* left_panel, Rectangle* right_panel);

// Return a ball structure from args 
ball_t ball(unsigned int radius, Color color, int velocity_x, int velocity_y, int xpos, int ypos);

// Updates balls position based on collision in the window bounds and panels 
void move_ball(ball_t* ball, Rectangle left_panel, Rectangle right_panel, unsigned int* score_one, unsigned int* score_two, bool* game_state); 

// Return a vector2 structure
Vector2 vec2(int x, int y); 

// Gamestates
enum game_states {IDLE, RUNNING};

int main(int argc, char** argv) {
    // Define the panels 
    Rectangle left_panel = rec(LEFT_PANEL_W, LEFT_PANEL_H, LEFT_PANEL_XPOS, LEFT_PANEL_YPOS); 
    Rectangle right_panel = rec(RIGHT_PANEL_W, RIGHT_PANEL_H, RIGHT_PANEL_XPOS, RIGHT_PANEL_YPOS); 
    
    // Create a pong ball 
    ball_t pong_ball = ball(BALL_SIZE, BALL_COLOR, BALL_VELOCITY_HORIZONTAL, BALL_VELOCITY_VERTICAL, WINDOW_W/2, WINDOW_H/2);

    // Score counter 
    unsigned int player1_score, player2_score; 
    player1_score = player2_score = 0U; 

    // Manage Gamestate 
    bool game_state = IDLE;

    InitAudioDevice();
    InitWindow(WINDOW_W, WINDOW_H, WINDOW_T); 
    SetTargetFPS(WINDOW_FPS); 

    // Game sounds
    Sound main_sound = LoadSound("./audio/base.mp3");

    while (!WindowShouldClose()) {
        ClearBackground(BLACK); 

        // Play main song 
        PlaySound(main_sound);

        // Move panels
        if (game_state) {
            move_panels_on_keypress(&left_panel, &right_panel);
            move_ball(&pong_ball, left_panel, right_panel, &player1_score, &player2_score, &game_state);
        }
        
        // Quit on Esc 
        if (IsKeyPressed(KEY_Q)) game_state = IDLE;

        BeginDrawing();
        {
            if (!game_state) {
                if ((player1_score > 0) || (player2_score > 0))
                {   
                    if (player1_score > player2_score) {
                        DrawText(ENDGAME_PLAYER1_WIN_TEXT, ENDGAME_PLAYER_WIN_TEXT_XPOS, ENDGAME_PLAYER_WIN_TEXT_YPOS, ENDGAME_TEXT_FONTSIZE, ENDGAME_TEXT_COLOR);
                    } else if (player2_score > player1_score) {
                        DrawText(ENDGAME_PLAYER2_WIN_TEXT, ENDGAME_PLAYER_WIN_TEXT_XPOS, ENDGAME_PLAYER_WIN_TEXT_YPOS, ENDGAME_TEXT_FONTSIZE, ENDGAME_TEXT_COLOR);
                    } else if (player1_score == player2_score) {
                        DrawText(ENDGAME_TIE_TEXT, ENDGAME_TIE_TEXT_XPOS, ENDGAME_TIE_TEXT_YPOS, ENDGAME_TEXT_FONTSIZE, ENDGAME_TEXT_COLOR);
                    }
                    DrawText(TextFormat("Player A: %u", player1_score), GAMESTATE_IDLE_PLAYER1_SCORE_XPOS, GAMESTATE_IDLE_PLAYER1_SCORE_YPOS, GAMESTATE_IDLE_PLAYER1_SCORE_TEXT_FONTSIZE, GAMESTATE_IDLE_PLAYER1_SCORE_TEXT_COLOR);
                    DrawText(TextFormat("Player B: %u", player2_score), GAMESTATE_IDLE_PLAYER2_SCORE_XPOS, GAMESTATE_IDLE_PLAYER2_SCORE_YPOS, GAMESTATE_IDLE_PLAYER2_SCORE_TEXT_FONTSIZE, GAMESTATE_IDLE_PLAYER2_SCORE_TEXT_COLOR);
                } else {
                    DrawText(GAMESTATE_IDLE_START_TEXT, GAMESTATE_IDLE_TEXT_XPOS, GAMESTATE_IDLE_TEXT_YPOS, GAMESTATE_IDLE_TEXT_FONTSIZE, GAMESTATE_IDLE_TEXT_COLOR);
                    if (IsKeyPressed(KEY_SPACE)) game_state = RUNNING;
                }
            }
            else {
                DrawText(TextFormat("A - %u", player1_score), SCORE_LEFT_POS_X, SCORE_LEFT_POS_Y, SCORE_FONT_SIZE, SCORE_FONT_COLOR);
                DrawText(TextFormat("B - %u", player2_score), SCORE_RIGHT_POS_X, SCORE_RIGHT_POS_Y, SCORE_FONT_SIZE, SCORE_FONT_COLOR);
                DrawLine(WINDOW_W/2, 0, WINDOW_W/2, WINDOW_H, WHITE);
                DrawLine(0, SCORE_FONT_SIZE, WINDOW_W, SCORE_FONT_SIZE, WHITE);
                DrawCircle(pong_ball.circle.position.x, pong_ball.circle.position.y, pong_ball.circle.radius, WHITE);
                DrawRectangleRec(left_panel, LEFT_PANEL_COLOR); 
                DrawRectangleRec(right_panel, RIGHT_PANEL_COLOR);
            }
        }
        EndDrawing(); 
    }

    UnloadSound(main_sound);
    CloseAudioDevice();
    CloseWindow();
    exit(EXIT_SUCCESS);
}

Rectangle rec(unsigned int width, unsigned int height, int xpos, int ypos) {
    return (Rectangle) {
        xpos,
        ypos,
        width, 
        height
    };
}

void move_panels_on_keypress(Rectangle* left_panel, Rectangle* right_panel) {
    // Move left panel 
    if (IsKeyDown(KEY_W) && (left_panel->y >= 0)) left_panel->y -= 10.0f; 
    if (IsKeyDown(KEY_S) && (left_panel->y <= (WINDOW_H-LEFT_PANEL_H))) left_panel->y += 10.0f; 

    // Move right panel
    if (IsKeyDown(KEY_UP) && (right_panel->y >= 0)) right_panel->y -= 10.0f; 
    if (IsKeyDown(KEY_DOWN) && (right_panel->y <= (WINDOW_H-RIGHT_PANEL_H))) right_panel->y += 10.0f; 
}   

ball_t ball(unsigned int radius, Color color, int velocity_x, int velocity_y, int xpos, int ypos) {
    return (ball_t) {
        // circle_t
        {
            radius, 
            color, 
            // Vector2
            {
                xpos, 
                ypos
            }
        },
        // Vector2
        {
            velocity_x, 
            velocity_y
        }
    };
}

Vector2 vec2(int x, int y) {
    return (Vector2) {x, y};
}

void move_ball(ball_t* ball, Rectangle left_panel, Rectangle right_panel, unsigned int* score_one, unsigned int* score_two, bool* game_state) {
        if (CheckCollisionCircleRec(vec2(ball->circle.position.x, ball->circle.position.y), ball->circle.radius, left_panel) 
            || 
            CheckCollisionCircleRec(vec2(ball->circle.position.x, ball->circle.position.y), ball->circle.radius, right_panel)) 
        {
            ball->velocity.x *= -1; 
        }
        else if (ball->circle.position.y >= (WINDOW_H-ball->circle.radius) || ball->circle.position.y <= (0+ball->circle.radius)) {
            ball->velocity.y *= -1;
        }
        else if (ball->circle.position.x < 0) {
            *score_two += SCORE_INCREMENT; // increase score for the opponent
            ball->circle.position = vec2(WINDOW_W/2, WINDOW_H/2);
            ball->velocity.x *= -1;
            ball->velocity.y *= -1;
        }
        else if (ball->circle.position.x > WINDOW_W) {
            *score_one += SCORE_INCREMENT; // increase score for the opponent
            ball->circle.position = vec2(WINDOW_W/2, WINDOW_H/2);
            ball->velocity.x *= -1;
            ball->velocity.y *= -1;
        }
        ball->circle.position.x += ball->velocity.x * GetFrameTime(); 
        ball->circle.position.y += ball->velocity.y * GetFrameTime(); 
}