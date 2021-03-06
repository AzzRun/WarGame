#ifndef ENGINE_H
#define ENGINE_H
#ifdef _WIN32 || _WIN64
/* si vous êtes sous Windows */
#include <pthread_VC.h>
#elif defined linux || defined __linux || defined __linux__
/* si vous êtes sous linux */
#include <pthread.h>
#else
/* sinon vous êtes sur une plateforme non supportée */
#error not defined for this platform
#endif

#define MAX_PLAYERS 4
#define MAX_BULLETS 250

#include "ft_map.h"
#include "ft_player.h"
#include "unionproto.pb.h"
#include "ft_explode.h"
typedef struct Engine {

	SDL_Event event;
	const uint8_t *keystate;
	pthread_cond_t condition; 
	pthread_mutex_t mutex; 
    int WIDTH;
    int HEIGHT;
    bool fullscreen;
	uint32_t cooldownFullscreen;
	uint16_t playersCount;
	uint32_t cooldownDeath;
	uint32_t lastCooldownDeath;
    Player players[MAX_PLAYERS];
    Player mainPlayer;
    SDL_Renderer* screenRenderer;
    SDL_Window* window;
	Map *map;
    
    TTF_Font *font;

    SDL_Color colorWhite;
	SDL_Color colorRed;
    SDL_Color colorWarGame;

	SDL_Rect camera;
	SDL_Rect bullets[MAX_BULLETS];
	SDL_Rect viewRect;
	SDL_Rect healthRect;
	Explode AnimKillEx;
	SDL_Rect healthPos;
	SDL_Rect AmmoRect;
	SDL_Rect ammoPos;
	#pragma region  SDL_Surface
	double viewDegrees;

	SDL_Surface *mapSurface;
	SDL_Texture *viewSurface;
	SDL_Texture *redSurface;
	SDL_Texture *AmmoSurface;
	SDL_Texture *healthSurface;
	SDL_Texture *bulletSurface;
	SDL_Texture *explodeSurface;
	SDL_Texture *bloodSurface;
	SDL_Texture *AnimKill;
	SDL_Texture *bombSurface;
	SDL_Texture *characterSurface;
	SDL_Texture *characterEnnemiSurface;
	SDL_Texture *menuSurface;
	SDL_Texture *selectionSurface;
	SDL_Texture *gameoverBackground;

	#pragma endregion

    SDL_Rect spriteRect;
    SDL_Rect mousePos;
}Engine;
extern Engine _engine;

Item *headItemList2;

void Engine_init();

#endif // ENGINE_H
