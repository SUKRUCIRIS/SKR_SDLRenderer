#pragma once
#include <SDL.h>

typedef struct {
	float x;
	float y;
	float w;
	float h;
}SKR_Rect;

//you should also look at the example to see how you can use these functions

typedef struct SKR_RenderObject SKR_RenderObject;

typedef struct SKR_Button SKR_Button;

typedef struct SKR_ProgressBar SKR_ProgressBar;

void SKR_RendererInit();//this function starts sdl libraries and skr, you must call this function before all of the other functions

void SKR_RendererQuit();//this function quits sdl libraries and skr, also frees all the memory which is retained by these libraries

SKR_RenderObject* SKR_CreateObjectbyPNG(char* PNGfilepath, SKR_Rect* desrect, SKR_Rect* srcrect, int animation, int milliseconds, int* condition);

SKR_RenderObject* SKR_CreateObjectbyColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SKR_Rect* desrect, int animation, int milliseconds, int* condition);

SKR_RenderObject* SKR_CreateObjectbyTTF(char* fontpath, int size, char* text, SKR_Rect* desrect, SKR_Rect* srcrect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int animation, int milliseconds, int* condition);

SKR_RenderObject* SKR_CreateObjectbyTexture(SDL_Texture* texture, SKR_Rect* desrect, SKR_Rect* srcrect, int animation, int milliseconds, int* condition);

void SKR_ChangeObjectAlpha(SKR_RenderObject* RenderObject, Uint8 a);

void SKR_ChangeObjectColor(SKR_RenderObject* RenderObject, Uint8 r, Uint8 g, Uint8 b);

SDL_Texture* SKR_GetTexture(SKR_RenderObject* RenderObject);//returns texture of the SKR_RenderObject

void SKR_DestroyObject(SKR_RenderObject* RenderObject);//free the memory which is retained by one SKR_RenderObject

void SKR_DestroyAllObjects();//frees all the memory which is retained by SKR_RenderObjects

void SKR_RenderAll(int* fade, SDL_Color* fadingcolor, int faderate);//renders all things. fade=NULL or 0 means no fade. fade=1 means fade in. fade=2 means fade out. fade rate is the speed of fading. after fading is done, the fade variable will be set to 0 by this function.

SDL_Texture* SKR_ScreenShot();//takes a screenshot and returns as a texture.

void SKR_SaveTexture(SDL_Texture* texture, char* savepath);//this function saves the texture in given file path.

SKR_Button* SKR_CreateButton(SDL_Color* bordercolor, SDL_Color* inside1color, SDL_Color* inside2color, SDL_Color* textcolor, char* fontpath, int textsize, char* text, SKR_Rect* buttonrect, float borderthickness, int* isPressed, SDL_Event* input);//ispressed will be 0 when mouse isn't on the button, will be 1 when mouse is on the button, will be 2 when the button is pressed

void SKR_RunButtonInput();//you must put this function in input taking part of your game loop for the buttons to work

void SKR_DestroyButton(SKR_Button* button);//use this function to remove the button and free the memory which is retained by the button

void SKR_DestroyAllButtons();//frees all the memory which is retained by SKR_Buttons

SKR_ProgressBar* SKR_CreateProgressBar(SDL_Color* bordercolor, SDL_Color* inside1color, SDL_Color* inside2color, SKR_Rect* barrect, float borderthickness, float* maxvalue, float* currentvalue);//create a progress bar

void SKR_DestroyProgressBar(SKR_ProgressBar* bar);//use this function to remove the bar and free the memory which is retained by the bar

void SKR_DestroyAllBars();//frees all the memory which is retained by bars
