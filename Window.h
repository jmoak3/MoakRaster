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
		format = malloc(sizeof(SDL_PixelFormat));
		format->format = SDL_PIXELFORMAT_RGB888;
		format->palette = NULL;
		format->BitsPerPixel = 24;
		format->BytesPerPixel = 3;
		format->Rmask = 0xFF0000;
		format->Gmask = 0x00FF00;
		format->Bmask = 0x0000FF;
		format->Amask = 0;
		SDL_ConvertSurface(screenSurface, format, 0);
		SDL_UpdateWindowSurface( window ); 
	}	
}

void UpdateWindow(char * pixels, int width, int height)
{
	SDL_LockSurface(screenSurface);
	screenSurface->pixels = (void*)pixels;	
	SDL_UnlockSurface(screenSurface);
	SDL_UpdateWindowSurface( window ); 
}

#endif