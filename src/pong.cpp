#include "../headers/pong.h"



void Pong::MovePaddles() {
    this->m_lPad_dir.lock();        // down no semaforo da direcao da raquete esquerda
    this->leftPaddle_pos += this->leftPaddle_dir * this->paddles_speed / this->fps;
    this->leftPaddle_pos.y = max(min(this->leftPaddle_pos.y, (this->field_size.y - this->paddles_len) / 2), -(this->field_size.y - this->paddles_len) / 2);
    this->m_lPad_dir.unlock();      // up no semaforo da direcao da raquete esquerda

    this->m_rPad_dir.lock();        // down no semaforo da direcao da raquete direita
    this->rightPaddle_pos += this->rightPaddle_dir * this->paddles_speed / this->fps;
    this->rightPaddle_pos.y = max(min(this->rightPaddle_pos.y, (this->field_size.y - this->paddles_len) / 2), -(this->field_size.y - this->paddles_len) / 2);
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
    if(this->ball_pos.x + this->ball_size / 2 > this->rightPaddle_pos.x - this->paddles_wid / 2
       && this->ball_pos_old.x + this->ball_size / 2 <= this->rightPaddle_pos.x - this->paddles_wid / 2
       && abs(this->ball_pos.y - this->rightPaddle_pos.y) <= (this->paddles_len + this->ball_size) / 2) {
        this->ball_pos.x = 2 * (this->rightPaddle_pos.x - this->paddles_wid / 2) - (this->ball_pos.x + this->ball_size / 2);
        this->ball_dir.x *= -1;
    }
    else if(this->ball_pos.x - this->ball_size / 2 < this->leftPaddle_pos.x + this->paddles_wid / 2
            && this->ball_pos_old.x - this->ball_size / 2 >= this->leftPaddle_pos.x + this->paddles_wid / 2
            && abs(this->ball_pos.y - this->leftPaddle_pos.y) <= (this->paddles_len + this->ball_size) / 2) {
        this->ball_pos.x = 2 * (this->leftPaddle_pos.x + this->paddles_wid / 2) - (this->ball_pos.x - this->ball_size / 2);
        this->ball_dir.x *= -1;
    }
}

void Pong::CheckGoalCollisions() {
    if(this->ball_pos.x > this->field_size.x / 2) {
        this->winner = 1;
    }
    else if(this->ball_pos.x < -this->field_size.x / 2) {
        this->winner = 2;
    }
}



Vector2 Pong::Field2Window(Vector2 pos) {
    Vector2 window_pos;
    window_pos.x = pos.x + this->field_size.x / 2;
    window_pos.y = -pos.y + this->field_size.y / 2;
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
    Vector2 leftPaddle_pos_win = this->Field2Window(this->leftPaddle_pos) - Vector2(this->paddles_wid / 2, this->paddles_len / 2);
    SDL_Rect left_paddle_rect =  {(int) leftPaddle_pos_win.x,(int) leftPaddle_pos_win.y, (int) this->paddles_wid,(int) this->paddles_len};
    SDL_SetRenderDrawColor(this->g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(g_renderer, &left_paddle_rect);

    //Desenha raquete direita
    Vector2 rightPaddle_pos_win = this->Field2Window(this->rightPaddle_pos) - Vector2(this->paddles_wid / 2, this->paddles_len / 2);
    SDL_Rect right_paddle_rect =  {(int) rightPaddle_pos_win.x,(int) rightPaddle_pos_win.y, (int) this->paddles_wid,(int) this->paddles_len};
    SDL_SetRenderDrawColor(this->g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(this->g_renderer, &right_paddle_rect);

    Vector2 ball_pos_win = this->Field2Window(this->ball_pos) - Vector2(this->ball_size / 2, this->ball_size / 2);
    SDL_Rect ball_rect =  {(int) ball_pos_win.x, (int) ball_pos_win.y, (int) this->ball_size, (int) this->ball_size};
    SDL_SetRenderDrawColor(this->g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderFillRect(this->g_renderer, &ball_rect);

    SDL_RenderPresent(this->g_renderer);
}

void Pong::CloseWindow() {
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    g_renderer = NULL;
    g_window = NULL;

    SDL_Quit();
}

void Pong::HandleInput() {
    SDL_Event event;
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

    bool gameFinished = false;

    while(!gameFinished) {
        while(SDL_PollEvent(&event) && !gameFinished) {
            if(event.type == SDL_QUIT) {
                m_quit.lock();  // down no semaforo de quit
                this->quit = true;
                m_quit.unlock();    // up no semaforo de quit
                gameFinished = true;
            }
            else {
                m_quit.lock();  // down no semaforo de quit
                if(this->quit) gameFinished = true;
                m_quit.unlock();    // up no semaforo de quit
            }

            bool movedRight = false;
            if(currentKeyStates[SDL_SCANCODE_UP]) {
                m_rPad_dir.lock();
                rightPaddle_dir.y = 1;
                m_rPad_dir.unlock();
                movedRight = true;
            }
            else if(currentKeyStates[SDL_SCANCODE_DOWN]) {
                m_rPad_dir.lock();
                rightPaddle_dir.y = -1;
                m_rPad_dir.unlock();
                movedRight = true;
            }

            bool movedLeft = false;
            if(currentKeyStates[SDL_SCANCODE_W]) {
                m_lPad_dir.lock();
                leftPaddle_dir.y = 1;
                m_lPad_dir.unlock();
                movedLeft = true;
            }
            else if(currentKeyStates[SDL_SCANCODE_S]) {
                m_lPad_dir.lock();
                leftPaddle_dir.y = -1;
                m_lPad_dir.unlock();
                movedLeft = true;
            }

            if(!movedRight) {
                m_rPad_dir.lock();
                rightPaddle_dir.y = 0;
                m_rPad_dir.unlock();
            }

            if(!movedLeft) {
                m_lPad_dir.lock();
                leftPaddle_dir.y = 0;
                m_lPad_dir.unlock();
            }
        }
    }
}



Pong::Pong(int fps, int window_w, int window_h) {
    this->fps = fps;
    this->window_size = Vector2(window_w, window_h);

    double aspect_ratio = this->window_size.x / this->window_size.y;
    this->field_size = Vector2(640, 640/aspect_ratio);

    this->leftPaddle_pos = Vector2(-this->field_size.x / 2 * 0.9, 0);
    this->rightPaddle_pos = Vector2(this->field_size.x / 2 * 0.9, 0);
    this->paddles_speed = this->field_size.y / 2;
    this->paddles_len = this->field_size.y / 4;
    this->paddles_wid = this->field_size.x /20;

    this->ball_pos = Vector2(0, 0);
    this->ball_pos_old = Vector2(0, 0);
    this->ball_speed = this->field_size.x / 3;
    this->ball_size = this->field_size.x / 20;

    this->winner = 0;
    this->quit = false;

    this->OpenWindow();
    this->DrawFrame();
}

void Pong::Play() {
    this->leftPaddle_dir = Vector2(0, 0);
    this->rightPaddle_dir = Vector2(0, 0);

    srand(time(NULL));
    double randomDir = 6.28 * ((double)rand() / (double)RAND_MAX);
    this->ball_dir = Vector2(cos(randomDir), sin(randomDir));

    thread paddles_thread([] (Pong *pong) {pong->HandleInput();}, this);	// cria thread para cuidar dos inputs das raquetes

    using clock = chrono::steady_clock;
    clock::time_point lastFrame;
    clock::time_point nextFrame = clock::now();

    bool gameClosed = false;

    while(this->winner == 0 && !gameClosed) {
        m_quit.lock();  // down no semaforo de quit
        if(this->quit) gameClosed = true;
        m_quit.unlock();    // up no semaforo de quit

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

    if(this->winner != 0) {
        cout << "O jogador " << this->winner << " venceu!" << endl;
    }
    else {
        cout << "Ja vai embora? Jogue um pouco mais ;-;]" << endl;
    }

    m_quit.lock();  // down no semaforo de quit
    this->quit = true;
    m_quit.unlock();    // up no semaforo de quit
    paddles_thread.join();
}

Pong::~Pong() {
    this->CloseWindow();
}