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

struct SKR_Button {
	SDL_Color* bordercolor;
	SDL_Color* inside1color;
	SDL_Color* inside2color;
	SDL_Texture* text;
	SKR_Rect borderrect1;
	SKR_Rect borderrect2;
	SKR_Rect borderrect3;
	SKR_Rect borderrect4;
	SKR_Rect textrect;
	SKR_Rect insiderect;
	SKR_Rect* butonrect;
	SDL_Event* input;
	int* ispressed;
	SKR_Button* next;
};

struct SKR_ProgressBar {
	SDL_Color* bordercolor;
	SDL_Color* inside1color;
	SDL_Color* inside2color;
	SKR_Rect borderrect1;
	SKR_Rect borderrect2;
	SKR_Rect borderrect3;
	SKR_Rect borderrect4;
	SKR_Rect insiderect;
	SKR_Rect paintrect;
	float* maxvalue;
	float* currentvalue;
	SKR_ProgressBar* next;
};

SKR_ProgressBar* bars = NULL;
SKR_Button* buttons = NULL;
SKR_RenderObject* objects = NULL;
SDL_Renderer* gamerenderer = NULL;
SDL_Window* gamewindow = NULL;

SDL_Texture* fadetexture = NULL;
SKR_RenderObject* tmpR = NULL;
SKR_Button* tmpbR = NULL;
SKR_ProgressBar* tmppR = NULL;
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
	SKR_DestroyAllBars();
	SKR_DestroyAllButtons();
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
	tmpbR = buttons;
	while (tmpbR != NULL) {
		SDL_SetRenderDrawColor(gamerenderer, tmpbR->bordercolor->r, tmpbR->bordercolor->g, tmpbR->bordercolor->b, tmpbR->bordercolor->a);
		SDL_RenderFillRectF(gamerenderer, SKRtoSDLf(&(tmpbR->borderrect1), &sdlf));
		SDL_RenderFillRectF(gamerenderer, SKRtoSDLf(&(tmpbR->borderrect2), &sdlf));
		SDL_RenderFillRectF(gamerenderer, SKRtoSDLf(&(tmpbR->borderrect3), &sdlf));
		SDL_RenderFillRectF(gamerenderer, SKRtoSDLf(&(tmpbR->borderrect4), &sdlf));
		if (*(tmpbR->ispressed) != 0) {
			SDL_SetRenderDrawColor(gamerenderer, tmpbR->inside2color->r, tmpbR->inside2color->g, tmpbR->inside2color->b, tmpbR->inside2color->a);
			SDL_RenderFillRectF(gamerenderer, SKRtoSDLf(&(tmpbR->insiderect), &sdlf));
		}
		else {
			SDL_SetRenderDrawColor(gamerenderer, tmpbR->inside1color->r, tmpbR->inside1color->g, tmpbR->inside1color->b, tmpbR->inside1color->a);
			SDL_RenderFillRectF(gamerenderer, SKRtoSDLf(&(tmpbR->insiderect), &sdlf));
		}
		SDL_RenderCopyF(gamerenderer, tmpbR->text, NULL, SKRtoSDLf(&(tmpbR->textrect), &sdlf));
		tmpbR = tmpbR->next;
	}
	tmppR = bars;
	while (tmppR != NULL) {
		SDL_SetRenderDrawColor(gamerenderer, tmppR->bordercolor->r, tmppR->bordercolor->g, tmppR->bordercolor->b, tmppR->bordercolor->a);
		SDL_RenderFillRectF(gamerenderer, SKRtoSDLf(&(tmppR->borderrect1), &sdlf));
		SDL_RenderFillRectF(gamerenderer, SKRtoSDLf(&(tmppR->borderrect2), &sdlf));
		SDL_RenderFillRectF(gamerenderer, SKRtoSDLf(&(tmppR->borderrect3), &sdlf));
		SDL_RenderFillRectF(gamerenderer, SKRtoSDLf(&(tmppR->borderrect4), &sdlf));
		SDL_SetRenderDrawColor(gamerenderer, tmppR->inside1color->r, tmppR->inside1color->g, tmppR->inside1color->b, tmppR->inside1color->a);
		SDL_RenderFillRectF(gamerenderer, SKRtoSDLf(&(tmppR->insiderect), &sdlf));
		if (*(tmppR->currentvalue) > 0) {
			if (*(tmppR->currentvalue) >= *(tmppR->maxvalue)) {
				tmppR->paintrect.w = (tmppR->insiderect.w);
			}
			else {
				tmppR->paintrect.w = (tmppR->insiderect.w) * (*(tmppR->currentvalue) / (*(tmppR->maxvalue)));
			}
			SDL_SetRenderDrawColor(gamerenderer, tmppR->inside2color->r, tmppR->inside2color->g, tmppR->inside2color->b, tmppR->inside2color->a);
			SDL_RenderFillRectF(gamerenderer, SKRtoSDLf(&(tmppR->paintrect), &sdlf));
		}
		tmppR = tmppR->next;
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

SKR_Button* SKR_CreateButton(SDL_Color* bordercolor, SDL_Color* inside1color, SDL_Color* inside2color, SDL_Color* textcolor, char* fontpath, int textsize, char* text, SKR_Rect* buttonrect, float borderthickness, int* isPressed, SDL_Event* input) {
	SKR_Button* yeni = malloc(sizeof(SKR_Button));
	yeni->butonrect = buttonrect;
	yeni->bordercolor = bordercolor;
	yeni->inside1color = inside1color;
	yeni->inside2color = inside2color;
	yeni->borderrect1.x = buttonrect->x;
	yeni->borderrect1.y = buttonrect->y;
	yeni->borderrect1.w = borderthickness;
	yeni->borderrect1.h = buttonrect->h;
	yeni->borderrect2.x = buttonrect->x + borderthickness;
	yeni->borderrect2.y = buttonrect->y;
	yeni->borderrect2.w = buttonrect->w - (2 * borderthickness);
	yeni->borderrect2.h = borderthickness;
	yeni->borderrect3.x = buttonrect->x + buttonrect->w - borderthickness;
	yeni->borderrect3.y = buttonrect->y;
	yeni->borderrect3.w = borderthickness;
	yeni->borderrect3.h = buttonrect->h;
	yeni->borderrect4.x = buttonrect->x + borderthickness;
	yeni->borderrect4.y = buttonrect->y + buttonrect->h - borderthickness;
	yeni->borderrect4.w = buttonrect->w - (2 * borderthickness);
	yeni->borderrect4.h = borderthickness;
	yeni->insiderect.x = buttonrect->x + borderthickness;
	yeni->insiderect.y = buttonrect->y + borderthickness;
	yeni->insiderect.w = buttonrect->w - (2 * borderthickness);
	yeni->insiderect.h = buttonrect->h - (2 * borderthickness);
	TTF_Font* font = TTF_OpenFont(fontpath, textsize);
	TTF_SizeText(font, text, &w, &h);
	yeni->textrect.w = (float)w;
	yeni->textrect.h = (float)h;
	yeni->textrect.x = buttonrect->x + ((buttonrect->w - w) / 2);
	yeni->textrect.y = buttonrect->y + ((buttonrect->h - h) / 2) + 4;
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, *textcolor);
	yeni->text = SDL_CreateTextureFromSurface(gamerenderer, surface);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
	yeni->input = input;
	yeni->ispressed = isPressed;
	yeni->next = NULL;
	if (buttons == NULL) {
		buttons = yeni;
	}
	else {
		SKR_Button* tmp = buttons;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = yeni;
	}
	return yeni;
}

void SKR_RunButtonInput() {
	tmpbR = buttons;
	while (tmpbR != NULL) {
		if (tmpbR->input->motion.x >= tmpbR->butonrect->x && tmpbR->input->motion.y >= tmpbR->butonrect->y && tmpbR->input->motion.x <= (tmpbR->butonrect->x + tmpbR->butonrect->w) && tmpbR->input->motion.y <= (tmpbR->butonrect->y + tmpbR->butonrect->h)) {
			*(tmpbR->ispressed) = 1;
			if (tmpbR->input->button.button == SDL_BUTTON_LEFT && tmpbR->input->button.type == SDL_MOUSEBUTTONDOWN) {
				*(tmpbR->ispressed) = 2;
			}
		}
		else {
			*(tmpbR->ispressed) = 0;
		}
		tmpbR = tmpbR->next;
	}
}

void SKR_DestroyButton(SKR_Button* button) {
	SDL_DestroyTexture(button->text);
	button->text = NULL;
	if (buttons == button) {
		buttons = buttons->next;
		free(button);
		button = NULL;
		return;
	}
	SKR_Button* tmp = buttons;
	SKR_Button* tmp2 = NULL;
	while (tmp != NULL) {
		if (tmp == button) {
			tmp2->next = tmp->next;
			free(button);
			button = NULL;
			return;
		}
		tmp2 = tmp;
		tmp = tmp->next;
	}
	return;
}

void SKR_DestroyAllButtons() {
	SKR_Button* tmp = buttons;
	SKR_Button* tmp2 = NULL;
	while (tmp != NULL) {
		tmp2 = tmp;
		tmp = tmp->next;
		SDL_DestroyTexture(tmp2->text);
		free(tmp2);
		tmp2 = NULL;
	}
	return;
}

SKR_ProgressBar* SKR_CreateProgressBar(SDL_Color* bordercolor, SDL_Color* inside1color, SDL_Color* inside2color, SKR_Rect* barrect, float borderthickness, float* maxvalue, float* currentvalue) {
	SKR_ProgressBar* yeni = malloc(sizeof(SKR_ProgressBar));
	yeni->bordercolor = bordercolor;
	yeni->inside1color = inside1color;
	yeni->inside2color = inside2color;
	yeni->borderrect1.x = barrect->x;
	yeni->borderrect1.y = barrect->y;
	yeni->borderrect1.w = borderthickness;
	yeni->borderrect1.h = barrect->h;
	yeni->borderrect2.x = barrect->x + borderthickness;
	yeni->borderrect2.y = barrect->y;
	yeni->borderrect2.w = barrect->w - (2 * borderthickness);
	yeni->borderrect2.h = borderthickness;
	yeni->borderrect3.x = barrect->x + barrect->w - borderthickness;
	yeni->borderrect3.y = barrect->y;
	yeni->borderrect3.w = borderthickness;
	yeni->borderrect3.h = barrect->h;
	yeni->borderrect4.x = barrect->x + borderthickness;
	yeni->borderrect4.y = barrect->y + barrect->h - borderthickness;
	yeni->borderrect4.w = barrect->w - (2 * borderthickness);
	yeni->borderrect4.h = borderthickness;
	yeni->insiderect.x = barrect->x + borderthickness;
	yeni->insiderect.y = barrect->y + borderthickness;
	yeni->insiderect.w = barrect->w - (2 * borderthickness);
	yeni->insiderect.h = barrect->h - (2 * borderthickness);
	yeni->maxvalue = maxvalue;
	yeni->currentvalue = currentvalue;
	yeni->paintrect.x = yeni->insiderect.x;
	yeni->paintrect.y = yeni->insiderect.y;
	yeni->paintrect.h = yeni->insiderect.h;
	yeni->next = NULL;
	if (bars == NULL) {
		bars = yeni;
	}
	else {
		SKR_ProgressBar* tmp = bars;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = yeni;
	}
	return yeni;
}

void SKR_DestroyProgressBar(SKR_ProgressBar* bar) {
	if (bars == bar) {
		bars = bars->next;
		free(bar);
		bar = NULL;
		return;
	}
	SKR_ProgressBar* tmp = bars;
	SKR_ProgressBar* tmp2 = NULL;
	while (tmp != NULL) {
		if (tmp == bar) {
			tmp2->next = tmp->next;
			free(bar);
			bar = NULL;
			return;
		}
		tmp2 = tmp;
		tmp = tmp->next;
	}
	return;
}

void SKR_DestroyAllBars() {
	SKR_ProgressBar* tmp = bars;
	SKR_ProgressBar* tmp2 = NULL;
	while (tmp != NULL) {
		tmp2 = tmp;
		tmp = tmp->next;
		free(tmp2);
		tmp2 = NULL;
	}
	return;
}