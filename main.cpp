#include "Window.h"

//The Window Instance
Window window;

int main(int argc, char* args[])
{
	window.show();

	window.~Window();
	return 0;
}