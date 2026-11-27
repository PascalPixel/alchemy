#ifndef ALCHEMY_MAIN_ROUTINE_H
#define ALCHEMY_MAIN_ROUTINE_H

/* @file */
/* The main routine, which runs the game's sequences one after another, and */
/* the scene changes between them. */

#include "types.h"

/* gmMainRoutineRun's mode: where the game starts. */
typedef enum GAME_START {
	GAME_START_TITLE = 0,			 /* the Title, with the state cleared */
	GAME_START_DEBUG_VALE = 1,		 /* in debug mode, SceneId_ValePlaza */
	GAME_START_DEBUG_FILE_SCREEN = 2 /* in debug mode, the File Screen */
} GAME_START;

/* system/main_routine.c documents each. */
void scTakeExit(s32 exitId);
void scReturnFromSequence(s32 result);
s32 scGetArea(s32 sceneId);
void scLoadOverlay(s32 unused);
void scArrive(s32 unused, s32 keep);

#endif
