#ifndef PONG_H
#define PONG_H

#include "vector2.h"
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include <mutex>

using namespace std;

class Pong {

    private:
        int fps;
        
        Vector2 window_size;
        Vector2 field_size;

        Vector2 leftPaddle_pos, rightPaddle_pos;
        Vector2 leftPaddle_dir, rightPaddle_dir;
        double paddles_speed, paddles_len;

        Vector2 ball_pos, ball_pos_old;
        Vector2 ball_dir;
        double ball_speed;

        int winner;

        mutex m_lPad_dir, m_rPad_dir;
        
        void MovePaddles();
        void MoveBall();
        void CheckWallCollisions();
        void CheckPaddleCollisions();
        void CheckGoalCollisions();

        Vector2 Field2Window(Vector2 pos);
        void OpenWindow();
        void DrawFrame();
        void DrawWinner();
        void CloseWindow();

    protected:
        void HandleLeftPaddle();
        void HandleRightPaddle();

    public:
        Pong(int fps, int window_w, int window_h);

        void Play();

        ~Pong();
};

#endif