#include <SDL2/SDL.h>
#include <time.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define Y_VEL_MIN -3
#define Y_VEL_MAX 3

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Rect player_paddle;
SDL_Rect computer_paddle;
SDL_Rect ball;
SDL_Rect powerup;
SDL_Rect line;

int x_vel;
int y_vel;
int chance;

void reset();
void update_computer();
void update_ball();
void update_powerup();
void draw();

int main(int argc, char* argv[])
{
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Initialize game
	srand(time(NULL));
	reset();

	// Main loop
	int running = 1;
	while (running)
	{
		// Handle events
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
				case SDL_QUIT:
					running = 0;
					break;
				
				case SDL_MOUSEWHEEL:
					if (event.wheel.y > 0 && player_paddle.y > 0)
					{
						player_paddle.y -= 20;
					}
					else if (event.wheel.y < 0 && player_paddle.y + player_paddle.h < window_height)
					{
						player_paddle.y += 20;
					}
	
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_SPACE)
					{
						reset();
					}
	
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
	player_paddle.y = (window_height / 2) - (player_paddle.h / 2);

	// Computer paddle
	computer_paddle.w = 20;
	computer_paddle.h = 100;
	computer_paddle.x = window_width - 40;
	computer_paddle.y = (window_height / 2) - (computer_paddle.h / 2);

	// Ball
	ball.w = 20;
	ball.h = 20;
	ball.x = (window_width / 2) - (ball.w / 2);
	ball.y = (window_height / 2) - (ball.h / 2);

	// Line
	line.w = 2;
	line.h = 5;
	line.x = window_width / 2;

	// Randomize initial x direction
	chance = rand() % 2;
	if (chance == 0)
	{
		x_vel = 5;
	}
	else if (chance == 1)
	{
		x_vel = -5;
	}

	// Randomize initial y direction
	chance = rand() % 2;
	if (chance == 0)
	{
		y_vel = 1;
	}
	else if (chance == 1)
	{
		y_vel = -1;
	}
}

void update_computer()
{
	// Paddle is below or above ball
	if (computer_paddle.y >= ball.y && computer_paddle.y >= 0)
	{
		// Randomize paddle's speed
		chance = rand() % 2;
		if (chance == 0)
		{
			computer_paddle.y += Y_VEL_MIN - 1;
		}
		else if (chance == 1)
		{
			computer_paddle.y += Y_VEL_MIN;
		}
	}
	else if (computer_paddle.y + computer_paddle.h <= ball.y + ball.h && computer_paddle.y + computer_paddle.h <= window_height)
	{
		// Randomize paddle's speed
		chance = rand() % 2;
		if (chance == 0)
		{
			computer_paddle.y += Y_VEL_MAX + 1;
		}
		else if (chance == 1)
		{
			computer_paddle.y += Y_VEL_MAX;
		}
	}
}

void update_ball()
{
	// Ball hits left or right of window
	if (ball.x <= 0 || ball.x + ball.w >= window_width)
	{
		SDL_Delay(300);
		reset();
	}

	// Ball hits bottom or top of window
	if (ball.y + ball.h >= window_height || ball.y <= 0)
	{
		y_vel *= -1;
	}

	// Ball hits right or left of a paddle
	if (ball.x <= player_paddle.x + player_paddle.w && ball.y >= player_paddle.y && ball.y + ball.h <= player_paddle.y + player_paddle.h)
	{
		// Ball hits bottom or top half of paddle
		if (ball.y > player_paddle.y + (player_paddle.h / 2) && y_vel <= Y_VEL_MAX)
		{
			if (y_vel == -1)
			{
				y_vel *= -1;
			}
			else
			{
				y_vel++;
			}
		}
		else if (ball.y + ball.h < player_paddle.y + (player_paddle.h / 2) && y_vel >= Y_VEL_MIN)
		{
			if (y_vel == 1)
			{
				y_vel *= -1;
			}
			else
			{
				y_vel--;
			}
		}

		x_vel *= -1;
	}
	else if (ball.x + ball.w >= computer_paddle.x && ball.y >= computer_paddle.y && ball.y + ball.h <= computer_paddle.y + computer_paddle.h)
	{
		// Ball hits bottom or top half of paddle
		if (ball.y > computer_paddle.y + (computer_paddle.h / 2) && y_vel <= Y_VEL_MAX)
		{
			if (y_vel == -1)
			{
				y_vel *= -1;
			}
			else
			{
				y_vel++;
			}
		}
		else if (ball.y + ball.h < computer_paddle.y + (computer_paddle.h / 2) && y_vel >= Y_VEL_MIN)
		{
			if (y_vel == 1)
			{
				y_vel *= -1;
			}
			else
			{
				y_vel--;
			}
		}

		x_vel *= -1;
	}

	// Increment ball
	ball.x += x_vel;
	ball.y += y_vel;
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
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	for (line.y = 0; line.y < window_height; line.y += 10)
	{
		SDL_RenderFillRect(renderer, &line);
	}

	// Draw background
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
}