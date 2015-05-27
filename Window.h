#ifndef WINDOW_H
#define WINDOW_H

#include "SDL2/SDL.h"

void makeWindow(int width, int height)
{
	SDL_Window *window = NULL;
	SDL_Surface *screenSurface = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) >= 0)
	{
		window = SDL_CreateWindow("MoakRaster", SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, width, height , SDL_WINDOW_SHOWN);
		
		screenSurface = SDL_GetWindowSurface( window ); 
		SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) ); 
		SDL_UpdateWindowSurface( window ); 
		SDL_Delay( 2000 );
	    SDL_DestroyWindow( window );
	    
	    SDL_Quit();
	}	
}

#endif