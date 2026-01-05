/* Minimal SDL2 window test */
#include <SDL2/SDL.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) {
    fprintf(stderr, "\nReceived signal %d\n", signum);
    exit(signum);
}

int main(void)
{
	SDL_Window *window;
	SDL_Event event;
	int running = 1;
	
	/* Catch signals */
	signal(SIGTERM, signal_handler);
	signal(SIGKILL, signal_handler);
	
	fprintf(stderr, "Init SDL...\n");
	SDL_Init(SDL_INIT_VIDEO);
	
	fprintf(stderr, "Creating window...\n");
	window = SDL_CreateWindow("Test", 100, 100, 320, 240, 
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	
	fprintf(stderr, "Window created! Entering event loop...\n");
	fprintf(stderr, "Press any key in terminal to check if window exists\n");
	
	/* Simple event loop */
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				fprintf(stderr, "Got QUIT event\n");
				running = 0;
			}
		}
		SDL_Delay(100);
		
		/* Check if still alive */
		static int counter = 0;
		if (++counter % 10 == 0)
		{
			fprintf(stderr, "Still running... (%d seconds)\n", counter/10);
		}
		
		if (counter >= 100) /* 10 seconds */
			break;
	}
	
	fprintf(stderr, "Cleanup...\n");
	SDL_DestroyWindow(window);
	SDL_Quit();
	fprintf(stderr, "Done!\n");
	return 0;
}
