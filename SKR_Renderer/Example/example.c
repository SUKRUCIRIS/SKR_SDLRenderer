#include "C:\Users\kurt1\Desktop\c projeleri\SKR_Renderer\SKR_Renderer.h"
#include <time.h>

//you should also look at the header file to see all functions
//you should not use inner functions which arent on the header file 

int main(int argc, char* argv[]) {
	int attack = 0;
	int walk = 1;
	SKR_RendererInit();
	SKR_Rect bg;
	bg.x = 0;
	bg.y = 0;
	bg.w = 1920;
	bg.h = 1080;
	SKR_CreateObjectbyColor(100, 10, 100, 255, &bg, -1, -1, NULL);//this object wont be animated thats why animation parameter must be -1, milliseconds parameter would be any random number
	//if condition parameter is 1 the texture will be rendered, otherwise it wont. if it is null it will always be rendered
	SKR_Rect square;
	square.x = 50;
	square.y = 50;
	square.w = 100;
	square.h = 100;
	SKR_CreateObjectbyColor(10, 100, 10, 150, &square, -1, -1, NULL);
	SKR_Rect text;
	SKR_CreateObjectbyTTF("data/x.ttf", 50, "IT WORKS", &text, NULL, 0, 255, 0, 50, -1, -1, NULL);
	text.x = (1920 - text.w) / 2;
	text.y = (1080 - text.h) / 2;
	SKR_Rect res;
	res.x = 500;
	res.y = 500;
	res.w = 100;
	res.h = 100;
	SKR_CreateObjectbyPNG("data/1.png", &res, NULL, -1, -1, NULL);
	//As you can see below, making sprite animation is very easy with SKR_Renderer. You just create the objects and the library handle the rest
	//milliseconds parameter is how much time you want that sprite to show, animation parameter should start from "spritenumber-1" and go to 0. -1 means no animation
	//if the condition parameter is 1, the animation will be played. otherwise it wont. if it is null it will always be rendered
	SKR_Rect character;
	character.x = 1000;
	character.y = 800;
	character.w = 250;
	character.h = 250;
	SKR_Rect src1;
	src1.x = 0;
	src1.y = 0;
	src1.w = 250;
	src1.h = 250;
	SKR_RenderObject* attacko = SKR_CreateObjectbyPNG("data/Sprites/Attack1.png", &character, &src1, 7, 75, &attack);
	SKR_Rect src2;
	src2.x = 250;
	src2.y = 0;
	src2.w = 250;
	src2.h = 250;
	SKR_CreateObjectbyTexture(SKR_GetTexture(attacko), &character, &src2, 6, 75, &attack);
	SKR_Rect src3;
	src3.x = 500;
	src3.y = 0;
	src3.w = 250;
	src3.h = 250;
	SKR_CreateObjectbyTexture(SKR_GetTexture(attacko), &character, &src3, 5, 75, &attack);
	SKR_Rect src4;
	src4.x = 750;
	src4.y = 0;
	src4.w = 250;
	src4.h = 250;
	SKR_CreateObjectbyTexture(SKR_GetTexture(attacko), &character, &src4, 4, 75, &attack);
	SKR_Rect src5;
	src5.x = 1000;
	src5.y = 0;
	src5.w = 250;
	src5.h = 250;
	SKR_CreateObjectbyTexture(SKR_GetTexture(attacko), &character, &src5, 3, 75, &attack);
	SKR_Rect src6;
	src6.x = 1250;
	src6.y = 0;
	src6.w = 250;
	src6.h = 250;
	SKR_CreateObjectbyTexture(SKR_GetTexture(attacko), &character, &src6, 2, 75, &attack);
	SKR_Rect src7;
	src7.x = 1500;
	src7.y = 0;
	src7.w = 250;
	src7.h = 250;
	SKR_CreateObjectbyTexture(SKR_GetTexture(attacko), &character, &src7, 1, 75, &attack);
	SKR_Rect src8;
	src8.x = 1750;
	src8.y = 0;
	src8.w = 250;
	src8.h = 250;
	SKR_CreateObjectbyTexture(SKR_GetTexture(attacko), &character, &src8, 0, 75, &attack);

	//i used same src rects because the sprites are in same order
	SKR_RenderObject* walko = SKR_CreateObjectbyPNG("data/Sprites/Run.png", &character, &src1, 7, 75, &walk);
	SKR_CreateObjectbyTexture(SKR_GetTexture(walko), &character, &src2, 6, 75, &walk);
	SKR_CreateObjectbyTexture(SKR_GetTexture(walko), &character, &src3, 5, 75, &walk);
	SKR_CreateObjectbyTexture(SKR_GetTexture(walko), &character, &src4, 4, 75, &walk);
	SKR_CreateObjectbyTexture(SKR_GetTexture(walko), &character, &src5, 3, 75, &walk);
	SKR_CreateObjectbyTexture(SKR_GetTexture(walko), &character, &src6, 2, 75, &walk);
	SKR_CreateObjectbyTexture(SKR_GetTexture(walko), &character, &src7, 1, 75, &walk);
	SKR_CreateObjectbyTexture(SKR_GetTexture(walko), &character, &src8, 0, 75, &walk);

	SDL_Event e;
	clock_t time;

	int fade = 1;
	SDL_Color fadecolor = { 231,31,31 };

	SDL_Color bordercolor = { 255,255,255,100 };
	SDL_Color insidecolor1 = { 100,11,11,150 };
	SDL_Color insidecolor2 = { 50,50,50,255 };
	SDL_Color textcolor = { 89,99,31,200 };
	SKR_Rect quitbutonrect;
	quitbutonrect.x = 1700;
	quitbutonrect.y = 100;
	quitbutonrect.w = 200;
	quitbutonrect.h = 100;
	int isquit;
	SKR_CreateButton(&bordercolor, &insidecolor1, &insidecolor2, &textcolor, "data/x.ttf", 80, "QUIT", &quitbutonrect, 2.5, &isquit, &e);//voila the button is created

	SKR_Rect bar;
	bar.x = 800;
	bar.y = 200;
	bar.w = 500;
	bar.h = 70;
	float maxbar = 100;
	float currentbar = 25.87f;
	SKR_CreateProgressBar(&bordercolor, &insidecolor1, &insidecolor2, &bar, 5.5, &maxbar, &currentbar);//voila the progress bar is created

	while (1) {
		time = clock();
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT || isquit == 2) {
				fade = 2;
				SKR_RenderAll(&fade, &fadecolor, 10);
				SKR_RendererQuit();//quit all sdl libraries, and free all memory which is retained by sdl libraries and skr
				return 0;
			}
			if (e.button.button == SDL_BUTTON_LEFT && e.button.type == SDL_MOUSEBUTTONDOWN) {
				if (attack == 1) {
					attack = 0;
					walk = 1;
				}
				else {
					attack = 1;
					walk = 0;
				}
			}
			if (e.key.keysym.sym == SDLK_t) {
				SDL_Texture* screen = SKR_ScreenShot();//you can take screenshot as texture by this function
				SKR_SaveTexture(screen, "screenshot.png");//you can save a texture as png
				SDL_DestroyTexture(screen);
			}
			SKR_RunButtonInput();//you must call this function in input taking part of your game loop for the buttons to work
		}
		SKR_RenderAll(&fade, &fadecolor, 10);//render all created objects, animations, buttons, bars etc. Also applies fading, if it is requested.
		while ((clock() - time) < 15);
	}
}