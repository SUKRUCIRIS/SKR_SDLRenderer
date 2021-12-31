#include "SKR_Renderer.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <time.h>

struct SKR_RenderObject {
	SDL_Texture* texture;
	SKR_Rect* desrect;
	SKR_Rect* srcrect;
	SKR_RenderObject* sonraki;
	int animation;
	int played;
	int milliseconds;
	int* condition;
	clock_t time;
};

SKR_RenderObject* objects = NULL;
SDL_Renderer* gamerenderer = NULL;
SDL_Window* gamewindow = NULL;

SDL_Rect sdli;
SDL_FRect sdlf;

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
	yeni->sonraki = NULL;
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
		while (tmp->sonraki != NULL) {
			tmp = tmp->sonraki;
		}
		tmp->sonraki = yeni;
	}
	return yeni;
}

SKR_RenderObject* SKR_CreateObjectbyColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a, SKR_Rect* desrect, int animation, int milliseconds, int* condition) {
	SKR_RenderObject* yeni = malloc(sizeof(SKR_RenderObject));
	yeni->desrect = desrect;
	yeni->srcrect = NULL;
	yeni->sonraki = NULL;
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
		while (tmp->sonraki != NULL) {
			tmp = tmp->sonraki;
		}
		tmp->sonraki = yeni;
	}
	return yeni;
}

SKR_RenderObject* SKR_CreateObjectbyTTF(char* fontpath, int size, char* text, SKR_Rect* desrect, SKR_Rect* srcrect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, int animation, int milliseconds, int* condition) {
	SKR_RenderObject* yeni = malloc(sizeof(SKR_RenderObject));
	yeni->desrect = desrect;
	yeni->srcrect = srcrect;
	yeni->sonraki = NULL;
	yeni->animation = animation;
	yeni->milliseconds = milliseconds;
	yeni->played = 0;
	yeni->time = 0;
	yeni->condition = condition;
	TTF_Font* font = TTF_OpenFont(fontpath, size);
	SDL_Color color = { r, g, b, a };
	int w, h;
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
		while (tmp->sonraki != NULL) {
			tmp = tmp->sonraki;
		}
		tmp->sonraki = yeni;
	}
	return yeni;
}

SKR_RenderObject* SKR_CreateObjectbyTexture(SDL_Texture* texture, SKR_Rect* desrect, SKR_Rect* srcrect, int animation, int milliseconds, int* condition) {
	SKR_RenderObject* yeni = malloc(sizeof(SKR_RenderObject));
	yeni->desrect = desrect;
	yeni->srcrect = srcrect;
	yeni->sonraki = NULL;
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
		while (tmp->sonraki != NULL) {
			tmp = tmp->sonraki;
		}
		tmp->sonraki = yeni;
	}
	return yeni;
}

void SKR_DestroyObject(SKR_RenderObject* RenderObject) {
	SDL_DestroyTexture(RenderObject->texture);
	RenderObject->texture = NULL;
	if (objects == RenderObject) {
		objects = objects->sonraki;
		free(RenderObject);
		RenderObject = NULL;
		return;
	}
	SKR_RenderObject* tmp = objects;
	SKR_RenderObject* tmp2 = NULL;
	while (tmp != NULL) {
		if (tmp == RenderObject) {
			tmp2->sonraki = tmp->sonraki;
			SDL_DestroyTexture(RenderObject->texture);
			free(RenderObject);
			RenderObject = NULL;
			return;
		}
		tmp2 = tmp;
		tmp = tmp->sonraki;
	}
	return;
}

void SKR_ChangeObjectAlpha(SKR_RenderObject* RenderObject, Uint8 a) {
	SDL_SetTextureAlphaMod(RenderObject->texture, a);
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
	SKR_RenderObject* bastaki = *tmp;
	while ((*tmp) != NULL && (*tmp)->animation != -1) {
		if ((*tmp)->played == 1) {
			if ((clock() - (*tmp)->time) >= (*tmp)->milliseconds) {
				(*tmp)->played = 0;
				if ((*tmp)->animation != 0) {
					(*tmp) = (*tmp)->sonraki;
				}
				else {
					(*tmp) = bastaki;
				}
				(*tmp)->played = 1;
				(*tmp)->time = clock();
			}
			SDL_RenderCopyF(gamerenderer, (*tmp)->texture, SKRtoSDLi((*tmp)->srcrect, &sdli), SKRtoSDLf((*tmp)->desrect, &sdlf));
			while ((*tmp)->animation != 0) {
				(*tmp) = (*tmp)->sonraki;
			}
			return;
		}
		(*tmp) = (*tmp)->sonraki;
	}
	(*tmp) = bastaki;
	(*tmp)->played = 1;
	(*tmp)->time = clock();
	SDL_RenderCopyF(gamerenderer, (*tmp)->texture, SKRtoSDLi((*tmp)->srcrect, &sdli), SKRtoSDLf((*tmp)->desrect, &sdlf));
	while ((*tmp)->animation != 0) {
		(*tmp) = (*tmp)->sonraki;
	}
	return;
}

void SKR_RenderAll() {
	SKR_RenderObject* tmp = objects;
	SDL_RenderClear(gamerenderer);
	while (tmp != NULL) {
		if (tmp->animation == -1) {
			SDL_RenderCopyF(gamerenderer, tmp->texture, SKRtoSDLi((tmp->srcrect), &sdli), SKRtoSDLf(tmp->desrect, &sdlf));
		}
		else if (*(tmp->condition) == 1) {
			SKR_HandleAnimation(&tmp);
		}
		tmp = tmp->sonraki;
	}
	SDL_RenderPresent(gamerenderer);
	return;
}

void SKR_DestroyAllObjects() {
	SKR_RenderObject* tmp = objects;
	SKR_RenderObject* tmp2 = NULL;
	while (tmp != NULL) {
		tmp2 = tmp;
		tmp = tmp->sonraki;
		SDL_DestroyTexture(tmp2->texture);
		free(tmp2);
		tmp2 = NULL;
	}
	return;
}

SDL_Texture* SKR_GetTexture(SKR_RenderObject* RenderObject) {
	return RenderObject->texture;
}