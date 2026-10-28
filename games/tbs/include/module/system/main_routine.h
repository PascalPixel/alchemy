#ifndef ALCHEMY_MODULE_SYSTEM_MAIN_ROUTINE_H
#define ALCHEMY_MODULE_SYSTEM_MAIN_ROUTINE_H

#include "types.h"

/// MainRoutine_Run's mode: where the game starts.
enum GameStart {
  GAME_START_TITLE = 0,            ///< the Title, with the work cleared
  GAME_START_DEBUG_VALE = 1,       ///< in debug mode, SceneId_ValePlaza
  GAME_START_DEBUG_FILE_SCREEN = 2 ///< in debug mode, the File Screen
};

void Scene_TakeExit(s32 exitId);
void Scene_ReturnFromSequence(s32 result);
s32 Scene_GetArea(s32 sceneId);
void MainRoutine_Run(s32 mode);
void Scene_LoadOverlay(s32 unused);
void Scene_Arrive(s32 unused, s32 keep);
void Scene_PlayMusic(void);

#endif
