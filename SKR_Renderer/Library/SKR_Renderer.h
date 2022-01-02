#pragma once
#include <SDL.h>

typedef struct {
	float x;
	float y;
	float w;
	float h;
}SKR_Rect;

typedef struct SKR_RenderObject SKR_RenderObject;

void SKR_RendererInit();

void SKR_RendererQuit();

SKR_RenderObject* SKR_CreateObjectbyPNG(char* PNGfilepath, SKR_Rect* desrect, SKR_Rect* srcrect, int animation, int milliseconds, int* condition);

SKR_RenderObject* SKR_CreateObjectbyColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SKR_Rect* desrect, int animation, int milliseconds, int* condition);

SKR_RenderObject* SKR_CreateObjectbyTTF(char* fontpath, int size, char* text, SKR_Rect* desrect, SKR_Rect* srcrect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int animation, int milliseconds, int* condition);

SKR_RenderObject* SKR_CreateObjectbyTexture(SDL_Texture* texture, SKR_Rect* desrect, SKR_Rect* srcrect, int animation, int milliseconds, int* condition);

void SKR_ChangeObjectAlpha(SKR_RenderObject* RenderObject, Uint8 a);

SDL_Texture* SKR_GetTexture(SKR_RenderObject* RenderObject);

void SKR_DestroyObject(SKR_RenderObject* RenderObject);

void SKR_DestroyAllObjects();

void SKR_RenderAll();
