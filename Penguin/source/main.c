////////////////////////////
// PAlib project template //
////////////////////////////

// Lines starting with two slashes are ignored by the compiler
// Basically you can use them to comment what are you doing
// In fact, this kind of lines are called comments :P

// Include PAlib so that you can use it
#include <PA9.h>
<<<<<<< HEAD
#include "all_gfx.h"

void screen();
=======
>>>>>>> 0e1544b17e619df6c38fa42a88278128bac4671a

int main(){
	// Initialize PAlib
	PA_Init();

	// Put your initialization code here

	// Infinite loop to keep the program running
	while(1){
		// Put your game logic here

<<<<<<< HEAD
		screen();
=======
>>>>>>> 0e1544b17e619df6c38fa42a88278128bac4671a
		// Wait until the next frame.
		// The DS runs at 60 frames per second.
		PA_WaitForVBL();
	}
}
