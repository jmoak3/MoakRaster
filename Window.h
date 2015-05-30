#ifndef WINDOW_H
#define WINDOW_H

#include "SDL2/SDL.h"

static SDL_Window *window = NULL;
static SDL_Surface *screenSurface = NULL;
static SDL_PixelFormat * format;

void MakeWindow(int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO) >= 0)
	{
		window = SDL_CreateWindow("MoakRaster", SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, width, height , SDL_WINDOW_SHOWN);
		
		screenSurface = SDL_GetWindowSurface( window ); 
		//SDL_FillRect( screenSurface, NULL, 
		//	SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) ); 
		SDL_UpdateWindowSurface( window ); 
	}	
}

void UpdateWindow(char * pixels, int width, int height)
{
	SDL_Surface *imageBuffer = 
					SDL_CreateRGBSurfaceFrom((void*)pixels, width, height,
								//24, width*3, 0x0000FF, 0x00FF00, 0xFF0000, 0);
								24, width*3, 0x0000FF, 0x00FF00, 0xFF0000, 0);
	SDL_BlitSurface(imageBuffer, NULL, screenSurface, NULL);
	SDL_UpdateWindowSurface( window ); 
}

#endif