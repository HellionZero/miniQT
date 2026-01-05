/* Direct SDL2 test */
#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	
	fprintf(stderr, "=== DIRECT SDL2 TEST ===\n");
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	}
	fprintf(stderr, "SDL_Init OK\n");
	
	window = SDL_CreateWindow("DIRECT TEST - Can you see me?",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640, 480, SDL_WINDOW_SHOWN);
	
	if (!window)
	{
		fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	fprintf(stderr, "SDL_CreateWindow OK\n");
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (!renderer)
	{
		fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	fprintf(stderr, "SDL_CreateRenderer OK\n");
	
	/* Show window explicitly */
	SDL_ShowWindow(window);
	SDL_RaiseWindow(window);
	
	/* Draw something */
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	
	fprintf(stderr, "\n*** WINDOW SHOULD BE VISIBLE NOW! ***\n");
	fprintf(stderr, "Red screen, 640x480\n");
	fprintf(stderr, "Sleeping 10 seconds...\n\n");
	
	sleep(10);
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	fprintf(stderr, "Done!\n");
	return 0;
}
