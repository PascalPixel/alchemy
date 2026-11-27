#ifndef ALCHEMY_MAIN_ROUTINE_H
#define ALCHEMY_MAIN_ROUTINE_H

/* The main routine, which runs the game's modes one after another, and the
   scene changes between them. */

#include "types.h"

/* gmMainRoutineRun's mode: where the game starts. */
typedef enum GAME_START {
	GAME_START_TITLE = 0,			 /* the Title, with the state cleared */
	GAME_START_DEBUG_VALE = 1,		 /* in debug mode, SceneId_ValePlaza */
	GAME_START_DEBUG_FILE_SCREEN = 2 /* in debug mode, the File Screen */
} GAME_START;

/* system/main_routine.c documents each. */
void   gmTakeExit(Sint32 exitId);
void   gmReturnFromMode(Sint32 result);
Sint32 gmGetArea(Sint32 sceneId);
void   gmLoadOverlay(Sint32 unused);
void   gmArrive(Sint32 unused, Sint32 keep);

#endif
