////////////////////////////
// PAlib project template //
////////////////////////////

// Lines starting with two slashes are ignored by the compiler
// Basically you can use them to comment what are you doing
// In fact, this kind of lines are called comments :P

// Include PAlib so that you can use it
#include <PA9.h>
#include "all_gfx.h"

<<<<<<< HEAD
void initScreen();
void readyScreen();
=======
void screen();
>>>>>>> 28c48b7f82845319441174504265659accd63390

int main(){
	// Initialize PAlib
	PA_Init();

	// Put your initialization code here

	// Infinite loop to keep the program running
	while(1){
		// Put your game logic here

<<<<<<< HEAD
		initScreen();
		readyScreen();
=======
		screen();
>>>>>>> 28c48b7f82845319441174504265659accd63390
		// Wait until the next frame.
		// The DS runs at 60 frames per second.
		PA_WaitForVBL();
	}
}
