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

bool Pong::OpenWindow() {
    bool success = true;
     //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        this->g_window = SDL_CreateWindow("Simple Pong Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)this->window_size.x, (int)this->window_size.y, SDL_WINDOW_SHOWN );
        if(this->g_window == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else{
            this->g_renderer = SDL_CreateRenderer(this->g_window, -1, SDL_RENDERER_ACCELERATED );
            if(this->g_renderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else {
                SDL_SetRenderDrawColor(this->g_renderer, 0x12, 0x12, 0x12, 0xFF);
            }
        }
        
    }

    return success;
}

void Pong::DrawFrame() {
    SDL_SetRenderDrawColor(this->g_renderer, 0x12, 0x12, 0x12, 0xFF);
    SDL_RenderClear(this->g_renderer);

    //Desenha raquete esquerda
    SDL_Rect left_paddle_rect =  {(int) this->leftPaddle_pos.x,(int) this->leftPaddle_pos.y, (int) this->paddles_wid,(int) this->paddles_len};
    SDL_SetRenderDrawColor(this->g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(g_renderer, &left_paddle_rect);

    //Desenha raquete direita
    SDL_Rect right_paddle_rect =  {(int) this->rightPaddle_pos.x,(int) this->rightPaddle_pos.y, (int) this->paddles_wid,(int) this->paddles_len};
    SDL_SetRenderDrawColor(this->g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(this->g_renderer, &right_paddle_rect);

    SDL_Rect ball_rect =  {(int) ball_pos.x, (int) ball_pos.y, (int) field_size.x/20, (int)field_size.x/20};
    SDL_SetRenderDrawColor(this->g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(this->g_renderer, &ball_rect);

    SDL_RenderPresent(this->g_renderer);
}

bool loadMedia(){
    bool success = true;
    return success;
}

void Pong::DrawWinner() {
    // mostra o vencedor na janela (jogador da esquerda se winner = -1 e da direita se winner = 1)
}

void Pong::CloseWindow() {
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    g_renderer = NULL;
    g_window = NULL;

    SDL_Quit();
}



void Pong::HandleLeftPaddle() {
    // checa input e mexe na leftPaddle_dir (lembrando de pegar o semaforo pra isso)

    SDL_Event Levent;

    while (SDL_PollEvent(&Levent)) {
        if (Levent.type == SDL_KEYDOWN) {
            if (Levent.key.keysym.sym == SDLK_w) {
                m_lPad_dir.lock();
                leftPaddle_dir.y = -1;
                m_lPad_dir.unlock();
            } else if (Levent.key.keysym.sym == SDLK_s) {
                m_lPad_dir.lock();
                leftPaddle_dir.y = 1;
                m_lPad_dir.unlock();
            } 
        }else{
            m_lPad_dir.lock();
            leftPaddle_dir.y = 0;
            m_lPad_dir.unlock();
        }
    }
}

void Pong::HandleRightPaddle() {
    // checa input e mexe na rightPaddle_dir (lembrando de pegar o semaforo pra isso)
    SDL_Event Revent;

    while (SDL_PollEvent(&Revent)) {
        if (Revent.type == SDL_KEYDOWN) {
            if (Revent.key.keysym.sym == SDLK_DOWN) {
                m_rPad_dir.lock();
                rightPaddle_dir.y = -1;
                m_rPad_dir.unlock();
            } else if (Revent.key.keysym.sym == SDLK_UP) {
                m_rPad_dir.lock();
                rightPaddle_dir.y = 1;
                m_rPad_dir.unlock();
            } 
        }else{
            m_rPad_dir.lock();
            rightPaddle_dir.y = 0;
            m_rPad_dir.unlock();
        }
    }
}



Pong::Pong(int fps, int window_w, int window_h) {
    this->fps = fps;
    this->window_size = Vector2(window_w, window_h);

    double aspect_ratio = this->window_size.x / this->window_size.y;
    this->field_size = Vector2(640, 600/aspect_ratio);    // se quiser trocar por algum motivo pode

    this->leftPaddle_pos = Vector2(this->field_size.x * 1 / 20, this->field_size.y / 2 - field_size.y/8);     // se quiser trocar por algum motivo pode
    this->rightPaddle_pos = Vector2(this->field_size.x * 18 / 20, this->field_size.y / 2-field_size.y/8);     // se quiser trocar por algum motivo pode
    this->paddles_speed = this->field_size.y / 2;       // se quiser trocar por algum motivo pode
    this->paddles_len = this->field_size.y / 4;     // se quiser trocar por algum motivo pode
    this->paddles_wid = this->field_size.x /20;

    this->ball_pos = Vector2(field_size.x/2 - field_size.x/20, field_size.y/2 - field_size.x/20);
    this->ball_pos_old = Vector2(0, 0);
    this->ball_speed = this->field_size.x / 3;       // se quiser trocar por algum motivo pode

    this->winner = 0;

    this->OpenWindow();
    this->DrawFrame();
    this->Play();
}

void Pong::Play() {
    thread leftPaddle_thread([] (Pong *pong) {pong->HandleLeftPaddle();}, this);	// cria thread para cuidar dos inputs da raquete esquerda
    leftPaddle_thread.detach();
    thread rightPaddle_thread([] (Pong *pong) {pong->HandleRightPaddle();}, this);	// cria thread para cuidar dos inputs da raquete direita
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

        //double fps_estimate = 1000 / chrono::duration_cast<chrono::milliseconds>(clock::now() - lastFrame).count();
        //cout << "FPS : " << fps_estimate << endl;
    }

    this->DrawWinner();
}

Pong::~Pong() {
    this->CloseWindow();
}