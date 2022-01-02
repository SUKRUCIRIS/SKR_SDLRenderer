#pragma once
#include <SDL.h>

typedef struct {
	float x;
	float y;
	float w;
	float h;
}SKR_Rect;

//you should look at the example to see how you can use these functions

typedef struct SKR_RenderObject SKR_RenderObject;

void SKR_RendererInit();//this function starts sdl libraries and skr, you must call this function before all of the other functions

void SKR_RendererQuit();//this function quits sdl libraries and skr, also frees all the memory which is retained by these libraries

SKR_RenderObject* SKR_CreateObjectbyPNG(char* PNGfilepath, SKR_Rect* desrect, SKR_Rect* srcrect, int animation, int milliseconds, int* condition);

SKR_RenderObject* SKR_CreateObjectbyColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SKR_Rect* desrect, int animation, int milliseconds, int* condition);

SKR_RenderObject* SKR_CreateObjectbyTTF(char* fontpath, int size, char* text, SKR_Rect* desrect, SKR_Rect* srcrect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int animation, int milliseconds, int* condition);

SKR_RenderObject* SKR_CreateObjectbyTexture(SDL_Texture* texture, SKR_Rect* desrect, SKR_Rect* srcrect, int animation, int milliseconds, int* condition);

void SKR_ChangeObjectAlpha(SKR_RenderObject* RenderObject, Uint8 a);

SDL_Texture* SKR_GetTexture(SKR_RenderObject* RenderObject);

void SKR_DestroyObject(SKR_RenderObject* RenderObject);//free the memory which is retained by one SKR_RenderObject

void SKR_DestroyAllObjects();//frees all the memory which is retained by SKR_RenderObjects

void SKR_RenderAll();//renders all SKR_RenderObjects
