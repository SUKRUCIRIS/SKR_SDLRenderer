#include "SKR_Renderer.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>

struct SKR_RenderObject {
	SDL_Texture* texture;
	SKR_Rect* desrect;
	SKR_Rect* srcrect;
	int animation;
	int played;
	int milliseconds;
	int* condition;
	clock_t time;
	SKR_RenderObject* next;
};

SKR_RenderObject* objects = NULL;
SDL_Renderer* gamerenderer = NULL;
SDL_Window* gamewindow = NULL;

SDL_Texture* fadetexture = NULL;
SKR_RenderObject* tmpR = NULL;
SKR_RenderObject* bastaki = NULL;
clock_t timeR;

SDL_Rect sdli;
SDL_FRect sdlf;

int w, h;

void SKR_RendererInit() {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_DisplayMode screen;
	SDL_GetDesktopDisplayMode(0, &screen);
	gamewindow = SDL_CreateWindow(u8"ÞÜKRÜ ÇÝRÝÞ ENGINE", 0, 0, screen.w, screen.h, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_BORDERLESS);
	gamerenderer = SDL_CreateRenderer(gamewindow, -1, render_flags);
	SDL_RenderSetLogicalSize(gamerenderer, 1920, 1080);
	SDL_SetRenderDrawBlendMode(gamerenderer, SDL_BLENDMODE_BLEND);
	return;
}

void SKR_RendererQuit() {
	SKR_DestroyAllObjects();
	SDL_DestroyRenderer(gamerenderer);
	SDL_DestroyWindow(gamewindow);
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
	return;
}

SKR_RenderObject* SKR_CreateObjectbyPNG(char* PNGfilepath, SKR_Rect* desrect, SKR_Rect* srcrect, int animation, int milliseconds, int* condition) {
	SKR_RenderObject* yeni = malloc(sizeof(SKR_RenderObject));
	yeni->desrect = desrect;
	yeni->srcrect = srcrect;
	yeni->next = NULL;
	yeni->animation = animation;
	yeni->milliseconds = milliseconds;
	yeni->played = 0;
	yeni->time = 0;
	yeni->condition = condition;
	yeni->texture = IMG_LoadTexture(gamerenderer, PNGfilepath);
	if (objects == NULL) {
		objects = yeni;
	}
	else {
		SKR_RenderObject* tmp = objects;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = yeni;
	}
	return yeni;
}

SKR_RenderObject* SKR_CreateObjectbyColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SKR_Rect* desrect, int animation, int milliseconds, int* condition) {
	SKR_RenderObject* yeni = malloc(sizeof(SKR_RenderObject));
	yeni->desrect = desrect;
	yeni->srcrect = NULL;
	yeni->next = NULL;
	yeni->animation = animation;
	yeni->milliseconds = milliseconds;
	yeni->played = 0;
	yeni->time = 0;
	yeni->condition = condition;
	SDL_Surface* surface = SDL_CreateRGBSurface(0, 1, 1, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, r, g, b, a));
	yeni->texture = SDL_CreateTextureFromSurface(gamerenderer, surface);
	SDL_FreeSurface(surface);
	if (objects == NULL) {
		objects = yeni;
	}
	else {
		SKR_RenderObject* tmp = objects;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = yeni;
	}
	return yeni;
}

SKR_RenderObject* SKR_CreateObjectbyTTF(char* fontpath, int size, char* text, SKR_Rect* desrect, SKR_Rect* srcrect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int animation, int milliseconds, int* condition) {
	SKR_RenderObject* yeni = malloc(sizeof(SKR_RenderObject));
	yeni->desrect = desrect;
	yeni->srcrect = srcrect;
	yeni->next = NULL;
	yeni->animation = animation;
	yeni->milliseconds = milliseconds;
	yeni->played = 0;
	yeni->time = 0;
	yeni->condition = condition;
	TTF_Font* font = TTF_OpenFont(fontpath, size);
	SDL_Color color = { r, g, b, a };
	TTF_SizeText(font, text, &w, &h);
	desrect->w = (float)w;
	desrect->h = (float)h;
	if (srcrect != NULL) {
		srcrect->w = (float)w;
		srcrect->h = (float)h;
	}
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
	yeni->texture = SDL_CreateTextureFromSurface(gamerenderer, surface);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
	if (objects == NULL) {
		objects = yeni;
	}
	else {
		SKR_RenderObject* tmp = objects;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = yeni;
	}
	return yeni;
}

SKR_RenderObject* SKR_CreateObjectbyTexture(SDL_Texture* texture, SKR_Rect* desrect, SKR_Rect* srcrect, int animation, int milliseconds, int* condition) {
	SKR_RenderObject* yeni = malloc(sizeof(SKR_RenderObject));
	yeni->desrect = desrect;
	yeni->srcrect = srcrect;
	yeni->next = NULL;
	yeni->animation = animation;
	yeni->milliseconds = milliseconds;
	yeni->played = 0;
	yeni->time = 0;
	yeni->condition = condition;
	yeni->texture = texture;
	if (objects == NULL) {
		objects = yeni;
	}
	else {
		SKR_RenderObject* tmp = objects;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = yeni;
	}
	return yeni;
}

void SKR_DestroyObject(SKR_RenderObject* RenderObject) {
	SDL_DestroyTexture(RenderObject->texture);
	RenderObject->texture = NULL;
	if (objects == RenderObject) {
		objects = objects->next;
		free(RenderObject);
		RenderObject = NULL;
		return;
	}
	SKR_RenderObject* tmp = objects;
	SKR_RenderObject* tmp2 = NULL;
	while (tmp != NULL) {
		if (tmp == RenderObject) {
			tmp2->next = tmp->next;
			SDL_DestroyTexture(RenderObject->texture);
			free(RenderObject);
			RenderObject = NULL;
			return;
		}
		tmp2 = tmp;
		tmp = tmp->next;
	}
	return;
}

void SKR_ChangeObjectAlpha(SKR_RenderObject* RenderObject, Uint8 a) {
	SDL_SetTextureAlphaMod(RenderObject->texture, a);
	return;
}

void SKR_ChangeObjectColor(SKR_RenderObject* RenderObject, Uint8 r, Uint8 g, Uint8 b) {
	SDL_SetTextureColorMod(RenderObject->texture, r, g, b);
	return;
}

SDL_Rect* SKRtoSDLi(SKR_Rect* src, SDL_Rect* des) {
	if (src == NULL) {
		return NULL;
	}
	des->x = (int)src->x;
	des->y = (int)src->y;
	des->w = (int)src->w;
	des->h = (int)src->h;
	return des;
}

SDL_FRect* SKRtoSDLf(SKR_Rect* src, SDL_FRect* des) {
	if (src == NULL) {
		return NULL;
	}
	des->x = src->x;
	des->y = src->y;
	des->w = src->w;
	des->h = src->h;
	return des;
}

void SKR_HandleAnimation(SKR_RenderObject** tmp) {
	bastaki = *tmp;
	while ((*tmp) != NULL && (*tmp)->animation != -1) {
		if ((*tmp)->played == 1) {
			if ((clock() - (*tmp)->time) >= (*tmp)->milliseconds) {
				(*tmp)->played = 0;
				if ((*tmp)->animation != 0) {
					(*tmp) = (*tmp)->next;
				}
				else {
					(*tmp) = bastaki;
				}
				(*tmp)->played = 1;
				(*tmp)->time = clock();
			}
			SDL_RenderCopyF(gamerenderer, (*tmp)->texture, SKRtoSDLi((*tmp)->srcrect, &sdli), SKRtoSDLf((*tmp)->desrect, &sdlf));
			while ((*tmp)->animation != 0) {
				(*tmp) = (*tmp)->next;
			}
			return;
		}
		(*tmp) = (*tmp)->next;
	}
	(*tmp) = bastaki;
	(*tmp)->played = 1;
	(*tmp)->time = clock();
	SDL_RenderCopyF(gamerenderer, (*tmp)->texture, SKRtoSDLi((*tmp)->srcrect, &sdli), SKRtoSDLf((*tmp)->desrect, &sdlf));
	while ((*tmp)->animation != 0) {
		(*tmp) = (*tmp)->next;
	}
	return;
}

void SKR_RenderAll(int* fade, SDL_Color* fadingcolor, int faderate) {
	if (fade != NULL && *fade != 0) {
		SDL_GetRendererOutputSize(gamerenderer, &w, &h);
		fadetexture = SDL_CreateTexture(gamerenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
		SDL_SetRenderTarget(gamerenderer, fadetexture);
	}
	tmpR = objects;
	SDL_RenderClear(gamerenderer);
	while (tmpR != NULL) {
		if (tmpR->condition == NULL || *(tmpR->condition) == 1) {
			if (tmpR->animation == -1) {
				SDL_RenderCopyF(gamerenderer, tmpR->texture, SKRtoSDLi((tmpR->srcrect), &sdli), SKRtoSDLf(tmpR->desrect, &sdlf));
			}
			else {
				SKR_HandleAnimation(&tmpR);
			}
		}
		tmpR = tmpR->next;
	}
	SDL_RenderPresent(gamerenderer);
	if (fade != NULL && *fade == 1) {
		SDL_SetRenderTarget(gamerenderer, NULL);
		sdlf.x = 0;
		sdlf.y = 0;
		sdlf.w = (float)w;
		sdlf.h = (float)h;
		for (w = 255; w >= 0; w = w - faderate) {
			timeR = clock();
			SDL_RenderClear(gamerenderer);
			SDL_RenderCopyF(gamerenderer, fadetexture, NULL, &sdlf);
			SDL_SetRenderDrawColor(gamerenderer, fadingcolor->r, fadingcolor->g, fadingcolor->b, w);
			SDL_RenderFillRectF(gamerenderer, &sdlf);
			SDL_RenderPresent(gamerenderer);
			while ((clock() - timeR) < 15);
		}
		SDL_DestroyTexture(fadetexture);
		fadetexture = NULL;
		*fade = 0;
	}
	else if (fade != NULL && *fade == 2) {
		SDL_SetRenderTarget(gamerenderer, NULL);
		sdlf.x = 0;
		sdlf.y = 0;
		sdlf.w = (float)w;
		sdlf.h = (float)h;
		for (w = 0; w <= 255; w = w + faderate) {
			timeR = clock();
			SDL_RenderClear(gamerenderer);
			SDL_RenderCopyF(gamerenderer, fadetexture, NULL, &sdlf);
			SDL_SetRenderDrawColor(gamerenderer, fadingcolor->r, fadingcolor->g, fadingcolor->b, w);
			SDL_RenderFillRectF(gamerenderer, &sdlf);
			SDL_RenderPresent(gamerenderer);
			while ((clock() - timeR) < 15);
		}
		SDL_DestroyTexture(fadetexture);
		fadetexture = NULL;
		*fade = 0;
	}
	return;
}

void SKR_DestroyAllObjects() {
	SKR_RenderObject* tmp = objects;
	SKR_RenderObject* tmp2 = NULL;
	while (tmp != NULL) {
		tmp2 = tmp;
		tmp = tmp->next;
		SDL_DestroyTexture(tmp2->texture);
		free(tmp2);
		tmp2 = NULL;
	}
	return;
}

SDL_Texture* SKR_GetTexture(SKR_RenderObject* RenderObject) {
	return RenderObject->texture;
}

SDL_Texture* SKR_ScreenShot() {
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_GetRendererOutputSize(gamerenderer, &w, &h);
	surface = SDL_CreateRGBSurface(0, w, h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	SDL_RenderReadPixels(gamerenderer, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
	texture = SDL_CreateTextureFromSurface(gamerenderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void SKR_SaveTexture(SDL_Texture* texture, char* savepath) {
	SDL_SetRenderTarget(gamerenderer, texture);
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Surface* surface = SDL_CreateRGBSurface(0, w, h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	SDL_RenderReadPixels(gamerenderer, NULL, surface->format->format, surface->pixels, surface->pitch);
	IMG_SavePNG(surface, savepath);
	SDL_FreeSurface(surface);
	SDL_SetRenderTarget(gamerenderer, NULL);
}