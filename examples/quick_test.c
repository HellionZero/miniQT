/* Quick test to see if SDL2 window appears */
#include "../lib/miniqt_backend/backend_factory.h"
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	t_backend_context *ctx;
	
	printf("Creating window...\n");
	fflush(stdout);
	
	ctx = mqt_create_auto_backend(640, 480, "TEST WINDOW - Should be visible!");
	
	if (!ctx)
	{
		printf("Failed!\n");
		return 1;
	}
	
	printf("Window created! Sleeping for 10 seconds so you can see it...\n");
	printf("Check if you see a window titled 'TEST WINDOW - Should be visible!'\n");
	fflush(stdout);
	
	/* Just sleep - don't poll events */
	sleep(10);
	
	printf("Destroying window...\n");
	mqt_backend_destroy(ctx);
	printf("Done!\n");
	
	return 0;
}
