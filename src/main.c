#include "init.h"


int main(int argc, char** argv) {
	// set seed
	srand((unsigned int)time(NULL));

	// initialize
	init(argc, argv);
	
	glutMainLoop();
}
