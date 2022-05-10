#include <iostream>
#include <SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Rect player_paddle;
SDL_Rect computer_paddle;
SDL_Rect ball;
SDL_Rect line;

int chance;
int xVel, yVel;

void reset();
void update_computer();
void update_ball();
void draw();

int main(int argc, char* argv[])
{
    // Main loop flag
    bool running = false;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (window == NULL)
            std::cout << "Could not create window. Error: " << SDL_GetError();
        else if (renderer == NULL)
            std::cout << "Could not create renderer. Error: " << SDL_GetError();
        else
            running = true;
    }
    else
        std::cout << "Could not initialize SDL. Error: " << SDL_GetError();

    // Initialize game
    srand(time(NULL));
    reset();

    // Main loop
    while (running)
    {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEWHEEL:
                if (event.wheel.y > 0 && player_paddle.y > 0)
                    player_paddle.y -= 20;
                else if (event.wheel.y < 0 && player_paddle.y + player_paddle.h < WINDOW_HEIGHT)
                    player_paddle.y += 20;
            default:
                break;
            }
        }

        // Update
        update_computer();
        update_ball();
        draw();

        // Delay for smoother movements
        SDL_Delay(10);
    }

    // Clean up
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}

void reset()
{
    // Player Paddle
    player_paddle.w = 20;
    player_paddle.h = 100;
    player_paddle.x = 20;
    player_paddle.y = (WINDOW_HEIGHT / 2) - (player_paddle.h / 2);

    // Computer paddle
    computer_paddle.w = 20;
    computer_paddle.h = 100;
    computer_paddle.x = WINDOW_WIDTH - 40;
    computer_paddle.y = (WINDOW_HEIGHT / 2) - (computer_paddle.h / 2);

    // Ball
    ball.w = 20;
    ball.h = 20;
    ball.x = (WINDOW_WIDTH / 2) - (ball.w / 2);
    ball.y = (WINDOW_HEIGHT / 2) - (ball.h / 2);

    // Line
    line.w = 2;
    line.h = 5;
    line.x = WINDOW_WIDTH / 2;

    // Randomize initial x direction
    chance = rand() % 2;
    if (chance == 0)
        xVel = 8;
    else if (chance == 1)
        xVel = -8;

    // Randomize initial y direction
    chance = rand() % 2;
    if (chance == 0)
        yVel = 1;
    else if (chance == 1)
        yVel = -1;
}

void update_computer()
{
    // Paddle is below or above ball
    if (computer_paddle.y > ball.y - (ball.h * 2))
    {
        // Randomize paddle's speed
        chance = rand() % 2;
        if (chance == 0)
            computer_paddle.y -= 1;
        else if (chance == 1)
            computer_paddle.y -= 4;
    }
    else if (computer_paddle.y < ball.y - (ball.h * 2))
    {
        // Randomize paddle's speed
        chance = rand() % 2;
        if (chance == 0)
            computer_paddle.y += 1;
        else if (chance == 1)
            computer_paddle.y += 4;
    }
}

void update_ball()
{
    // Ball hits left or right of window
    if (ball.x <= 0 || ball.x + ball.w >= WINDOW_WIDTH)
        reset();

    // Ball hits bottom or top of window
    if (ball.y + ball.h >= WINDOW_HEIGHT || ball.y <= 0)
        yVel *= -1;

    // Ball hits right or left side of a paddle
    if (ball.x <= player_paddle.x + player_paddle.w && ball.y >= player_paddle.y && ball.y + ball.h <= player_paddle.y + player_paddle.h)
    {
        // Ball hits bottom or top part of paddle
        if (ball.y > player_paddle.y + (player_paddle.h / 2) && yVel < 4)
        {
            if (yVel == -1)
                yVel += 2;
            else
                yVel += 1;
        }
        else if (ball.y < player_paddle.y + (player_paddle.h / 2) && yVel > -4)
        {
            if (yVel == 1)
                yVel -= 2;
            else
                yVel -= 1;
        }

        xVel *= -1;
    }
    else if (ball.x + ball.w >= computer_paddle.x && ball.y >= computer_paddle.y && ball.y + ball.h <= computer_paddle.y + computer_paddle.h)
    {
        // Ball hits bottom or top part of paddle
        if (ball.y > computer_paddle.y + (computer_paddle.h / 2) && yVel < 4)
        {
            if (yVel == -1)
                yVel += 2;
            else
                yVel += 1;
        }
        else if (ball.y < computer_paddle.y + (computer_paddle.h / 2) && yVel > -4)
        {
            if (yVel == 1)
                yVel -= 2;
            else
                yVel -= 1;
        }

        xVel *= -1;
    }

    // Increment ball
    ball.x += xVel;
    ball.y += yVel;
}

void draw()
{
    // Clear screen
    SDL_RenderClear(renderer);

    // Draw objects
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &player_paddle);
    SDL_RenderFillRect(renderer, &computer_paddle);
    SDL_RenderFillRect(renderer, &ball);

    // Draw line
    SDL_SetRenderDrawColor(renderer, 155, 155, 155, 255);
    for (line.y = 0; line.y < WINDOW_HEIGHT; line.y += 10)
        SDL_RenderFillRect(renderer, &line);

    // Draw background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
}
