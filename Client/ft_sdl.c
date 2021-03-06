#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/ft_sdl.h"


uint16_t lastPlayerCount = 1;

void ft_SDL_init(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de SDL_INIT_VIDEO : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	if (TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}
	sdl.font = TTF_OpenFont("res/verdanab.ttf", 16);
}
void ft_SDL_Close(void)
{
	TTF_CloseFont(sdl.font);
	SDL_FreeSurface(sdl.pseudoSurface);
	SDL_DestroyTexture(sdl.texturePseudoSurface);
}


int ft_SDL_DrawPlayers(void)
{
	for (uint8_t i = 0; i < _engine.playersCount; i++)
	{
		if (_engine.players[i].playerBase.id == -1 || _engine.players[i].playerBase.id == _engine.mainPlayer.playerBase.id)
			continue;
		_engine.players[i].sprite.h = _engine.players[i].sprite.w = 32;
		_engine.players[i].playerBase.pos.w = _engine.players[i].playerBase.pos.h = 32;
		SDL_Rect rect = _engine.players[i].playerBase.pos;
		rect.x = _engine.players[i].playerBase.pos.x - _engine.camera.x;
		rect.y = _engine.players[i].playerBase.pos.y - _engine.camera.y;
		ft_getCharactSprite(&_engine.players[i]);
		if (_engine.players[i].playerBase.health > 0)
		{
			if (strlen(_engine.players[i].playerBase.name) > 0)
			{
				sdl.pseudoSurface = TTF_RenderText_Blended(sdl.font, _engine.players[i].playerBase.name, _engine.colorWhite);
				sdl.texturePseudoSurface = SDL_CreateTextureFromSurface(_engine.screenRenderer, sdl.pseudoSurface);
				sdl.posPseudo = (SDL_Rect) { rect.x - sdl.pseudoSurface->w / 4, rect.y - 30, sdl.pseudoSurface->w, sdl.pseudoSurface->h };
				SDL_RenderCopy(_engine.screenRenderer, sdl.texturePseudoSurface, NULL, &sdl.posPseudo);
				SDL_FreeSurface(sdl.pseudoSurface);
				SDL_DestroyTexture(sdl.texturePseudoSurface);
			}
			SDL_RenderCopy(_engine.screenRenderer, _engine.characterEnnemiSurface, &_engine.players[i].sprite, &rect);
		}
		else if (_engine.players[i].deathAnimationStep != 10)
		{
			if (ft_delay(&_engine.players[i].lastAnim, 100))
			{
				_engine.players[i].sprite.x = 56 * (_engine.players[i].deathAnimationStep % 5) + (_engine.players[i].deathAnimationStep % 5 + 1);
				_engine.players[i].sprite.y = 41 * (_engine.players[i].deathAnimationStep / 5) + (_engine.players[i].deathAnimationStep / 5 + 1);
				_engine.players[i].deathAnimationStep += 1;
			}
			_engine.players[i].sprite.h = 41;
			_engine.players[i].sprite.w = 56;
			rect.x -= 12;
			rect.y -= 6;
			rect.h = 41;
			rect.w = 56;
			SDL_RenderCopy(_engine.screenRenderer, _engine.AnimKill, &_engine.players[i].sprite, &rect);		
		}
		if (_engine.players[i].playerBase.state == FIRE)
			sound_Play_Fire(&soundChannelEnemies);
	}
	return true;
}


void ft_SDL_DrawGame(void)
{
	SDL_RenderClear(_engine.screenRenderer);
	SDL_RenderCopy(_engine.screenRenderer, _engine.mapSurface, &_engine.camera, NULL);
	
	ft_SDL_DrawPlayers();
	ft_SDL_checkPlayerHit();
	if (_engine.mainPlayer.playerBase.state != DEAD)
	{
		ft_getCharactSprite(&_engine.mainPlayer);
		_engine.mainPlayer.deathAnimationStep = 0;
		SDL_RenderCopyEx(_engine.screenRenderer, _engine.viewSurface, NULL, &_engine.viewRect, _engine.viewDegrees, NULL, SDL_FLIP_NONE);
		if (_engine.mainPlayer.playerBase.state == HIT)
			SDL_RenderCopy(_engine.screenRenderer, _engine.characterEnnemiSurface, &_engine.mainPlayer.sprite, &_engine.mainPlayer.relativePos);
		else
			SDL_RenderCopy(_engine.screenRenderer, _engine.characterSurface, &_engine.mainPlayer.sprite, &_engine.mainPlayer.relativePos);
		if (_engine.mainPlayer.playerBase.health < 100)
			SDL_RenderCopy(_engine.screenRenderer, _engine.bloodSurface, NULL, NULL);
	}
	else if (_engine.mainPlayer.deathAnimationStep != 10)
	{
		if (ft_delay(&_engine.mainPlayer.lastAnim, 100))
		{
			_engine.mainPlayer.sprite.x = 56 * (_engine.mainPlayer.deathAnimationStep % 5) + (_engine.mainPlayer.deathAnimationStep % 5 + 1);
			_engine.mainPlayer.sprite.y = 41 * (_engine.mainPlayer.deathAnimationStep / 5) + (_engine.mainPlayer.deathAnimationStep / 5 + 1);
			_engine.mainPlayer.sprite.h = 41;
			_engine.mainPlayer.sprite.w = 56;
			_engine.mainPlayer.deathAnimationStep += 1;
		}
		SDL_Rect rect = _engine.mainPlayer.relativePos;
		rect.x -= 12;
		rect.y -= 6;
		rect.h = 41;
		rect.w = 56;
		SDL_RenderCopy(_engine.screenRenderer, _engine.AnimKill, &_engine.mainPlayer.sprite, &rect);
	}
	if (_engine.mainPlayer.playerBase.state == DEAD)
		menuDeath();

	browserBullets(headBullets, &drawBullet);	
	SDL_RenderCopy(_engine.screenRenderer, _engine.AmmoSurface, &_engine.AmmoRect, &_engine.ammoPos);
	SDL_RenderCopy(_engine.screenRenderer, _engine.healthSurface, &_engine.healthRect, &_engine.healthPos);

	if (_engine.playersCount > lastPlayerCount)
	{
		if (strlen(_engine.players[_engine.playersCount - 1].playerBase.name) > 0)
		{
			ft_chat_Add(PLAYERCONNECTION, &_engine.players[_engine.playersCount - 1].playerBase.name);
			lastPlayerCount = _engine.playersCount;
		}
	}
	ft_chat_Display();
	SDL_RenderPresent(_engine.screenRenderer);
}

void ft_getCharactSprite(Player *player)
{
		player->sprite.h = player->sprite.w = 32;
		switch (player->playerBase.state)
		{
		case IDLE:
			if (ft_delay(&player->lastAnim, 200))
			{
				player->sprite.y = 298;
				player->sprite.x = 32 * player->playerBase.orientation + player->playerBase.orientation + 1;
			}
			break;
		case FIRE:
			player->sprite.y = 265;
			player->sprite.x = 32 * player->playerBase.orientation + player->playerBase.orientation + 1;
			break;
		case WALK:
			if (ft_delay(&player->lastAnim, 100))
			{
				player->sprite.x = 32 * player->walkAnimationStep + player->walkAnimationStep + 1;
				player->sprite.y = 32 * player->playerBase.orientation + player->playerBase.orientation + 1;
				if (player->walkAnimationStep < 3)
					player->walkAnimationStep++;
				else
					player->walkAnimationStep = 0;
			}
			break;
		default:
			break;
		}
}

uint16_t playerLastHealth = 100;
uint16_t opacity = 0;
uint32_t lastHit = 0;
uint8_t grunt = 0;

void ft_SDL_checkPlayerHit(void)
{
	
	if (_engine.mainPlayer.playerBase.state != HIT && _engine.mainPlayer.playerBase.health == 100)
	{
		opacity = 0;
		playerLastHealth = _engine.mainPlayer.playerBase.health;
	}

	if (_engine.mainPlayer.playerBase.state != DEAD && _engine.mainPlayer.playerBase.health < playerLastHealth && !ft_delay(&lastHit, 1000))
	{
		_engine.mainPlayer.playerBase.state = HIT;
#ifdef _DEBUG
		printf("[DEBUG] grunt: %d\n", grunt);
#endif
		/*sound_Play_Grunt(&soundChannelGrunt, grunt); /!\ FIXME /!\ */
		if (grunt >= 5)
			grunt = 0;
		else
			grunt++;
		
		if (opacity + 25 < 255)
			opacity += 22;
		playerLastHealth = _engine.mainPlayer.playerBase.health;
		
	}

	SDL_SetTextureAlphaMod(_engine.bloodSurface, opacity);
}
