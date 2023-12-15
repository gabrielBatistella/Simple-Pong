#ifndef PONG_H
#define PONG_H

#include "vector2.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <mutex>
#include <SDL2/SDL.h>

using namespace std;

class Pong {

    private:
        int fps;
        
        SDL_Window* g_window;
        SDL_Surface* g_screen_surface;
        SDL_Renderer* g_renderer;

        Vector2 window_size;
        Vector2 field_size;

        Vector2 leftPaddle_pos, rightPaddle_pos;
        Vector2 leftPaddle_dir, rightPaddle_dir;
        double paddles_speed, paddles_len, paddles_wid;

        Vector2 ball_pos, ball_pos_old;
        Vector2 ball_dir;
        double ball_speed, ball_size;

        int winner;

        bool quit;

        mutex m_lPad_dir, m_rPad_dir;
        mutex m_quit;
        
        void MovePaddles();
        void MoveBall();
        void CheckWallCollisions();
        void CheckPaddleCollisions();
        void CheckGoalCollisions();

        Vector2 Field2Window(Vector2 pos);
        bool OpenWindow();
        void DrawFrame();
        void CloseWindow();

    protected:
        void HandlePaddles();

    public:
        Pong(int fps, int window_w, int window_h);

        void Play();

        ~Pong();
};

#endif