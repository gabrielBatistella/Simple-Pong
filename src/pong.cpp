#include "../headers/pong.h"



void Pong::MovePaddles() {
    this->m_lPad_dir.lock();        // down no semaforo da direcao da raquete esquerda
    this->leftPaddle_pos += this->leftPaddle_dir * this->paddles_speed / this->fps;
    this->m_lPad_dir.unlock();      // up no semaforo da direcao da raquete esquerda

    this->m_rPad_dir.lock();        // down no semaforo da direcao da raquete direita
    this->rightPaddle_pos += this->rightPaddle_dir * this->paddles_speed / this->fps;
    this->m_rPad_dir.unlock();      // up no semaforo da direcao da raquete direita
}

void Pong::MoveBall() {
    this->ball_pos_old = this->ball_pos;
    this->ball_pos += this->ball_dir * this->ball_speed / this->fps;
}

void Pong::CheckWallCollisions() {
    if(this->ball_pos.y > this->field_size.y / 2) {
        this->ball_pos.y = this->field_size.y - this->ball_pos.y;
        this->ball_dir.y *= -1;
    }
    else if(this->ball_pos.y < -this->field_size.y / 2) {
        this->ball_pos.y = -this->field_size.y - this->ball_pos.y;
        this->ball_dir.y *= -1;
    }
}

void Pong::CheckPaddleCollisions() {
    if(this->ball_pos.x > this->rightPaddle_pos.x && this->ball_pos_old.x <= this->rightPaddle_pos.x
       && abs(this->ball_pos.y - this->rightPaddle_pos.y) <= this->paddles_len) {
        this->ball_pos.x = 2 * this->rightPaddle_pos.x - this->ball_pos.x;
        this->ball_dir.x *= -1;
    }
    else if(this->ball_pos.x < this->leftPaddle_pos.x && this->ball_pos_old.x >= this->leftPaddle_pos.x
            && abs(this->ball_pos.y - this->leftPaddle_pos.y) <= this->paddles_len) {
        this->ball_pos.x = 2 * this->leftPaddle_pos.x - this->ball_pos.x;
        this->ball_dir.x *= -1;
    }
}

void Pong::CheckGoalCollisions() {
    if(this->ball_pos.x > this->field_size.x / 2) {
        this->winner = 1;
    }
    else if(this->ball_pos.x < -this->field_size.x / 2) {
        this->winner = -1;
    }
}



Vector2 Pong::Field2Window(Vector2 pos) {
    Vector2 fieldAbsolute_pos = pos + this->field_size / 2;     // considerando a origem da window em baixo na esquerda, se for diferente vai mudar aqui

    double unit2pixel_ratio = this->window_size.x / this->field_size.x;
    Vector2 window_pos = fieldAbsolute_pos * unit2pixel_ratio;
    return window_pos;
}

void Pong::OpenWindow() {
    // abre a janela do jogo
}

void Pong::DrawFrame() {
    // pega as posicoes das raquetes e bola, converte pra pixels (field2window), e desenha
}

void Pong::DrawWinner() {
    // mostra o vencedor na janela (jogador da esquerda se winner = -1 e da direita se winner = 1)
}

void Pong::CloseWindow() {
    // fecha a janela
}



void Pong::HandleLeftPaddle() {
    // checa input e mexe na leftPaddle_dir (lembrando de pegar o semaforo pra isso)
}

void Pong::HandleRightPaddle() {
    // checa input e mexe na rightPaddle_dir (lembrando de pegar o semaforo pra isso)
}



Pong::Pong(int fps, int window_w, int window_h) {
    this->fps = fps;
    this->window_size = Vector2(window_w, window_h);

    double aspect_ratio = this->window_size.x / this->window_size.y;
    this->field_size = Vector2(100, 100 / aspect_ratio);    // se quiser trocar por algum motivo pode

    this->leftPaddle_pos = Vector2(-this->field_size.x * 4 / 10, 0);     // se quiser trocar por algum motivo pode
    this->rightPaddle_pos = Vector2(this->field_size.x * 4 / 10, 0);     // se quiser trocar por algum motivo pode
    this->paddles_speed = this->field_size.y / 2;       // se quiser trocar por algum motivo pode
    this->paddles_len = this->field_size.y / 5;     // se quiser trocar por algum motivo pode

    this->ball_pos = Vector2(0, 0);
    this->ball_pos_old = Vector2(0, 0);
    this->ball_speed = this->field_size.x / 3;       // se quiser trocar por algum motivo pode

    this->winner = 0;

    this->OpenWindow();
    this->DrawFrame();
}

void Pong::Play() {
    thread leftPaddle_thread([] (Pong *pong) {pong->HandleLeftPaddle();}, this);
    leftPaddle_thread.detach();
    thread rightPaddle_thread([] (Pong *pong) {pong->HandleRightPaddle();}, this);
    rightPaddle_thread.detach();

    using clock = chrono::steady_clock;
    clock::time_point lastFrame;
    clock::time_point nextFrame = clock::now();

    while(this->winner == 0) {
        lastFrame = nextFrame;
        nextFrame += chrono::milliseconds(1000 / this->fps);

        this->MovePaddles();
        this->MoveBall();

        this->CheckWallCollisions();
        this->CheckPaddleCollisions();
        this->CheckGoalCollisions();

        this->DrawFrame();

        this_thread::sleep_until(nextFrame);

        double fps_estimate = 1000 / chrono::duration_cast<chrono::milliseconds>(clock::now() - lastFrame).count();
        cout << "FPS : " << fps_estimate << endl;
    }

    this->DrawWinner();
}

Pong::~Pong() {
    this->CloseWindow();
}