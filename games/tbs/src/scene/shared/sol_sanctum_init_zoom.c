/// @file
/// The initialization that two rooms of Sol Sanctum share,
/// SceneId_SolSanctumMaze and SceneId_SolSanctumCorridor.

#include "flags.h"

/// The scene opens in the window transition over 16 frames.
s32 Scene_Initialize(void) {
  struct FieldWork *work;

  s32 shakeX = px(1);
  s32 shakeY = px(1);
  s32 shakeDecay = Q16_ONE;

  work = gFieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  work->transitionFrames = 16;
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Scene_QueueMusic(SE_RUMBLE);
    MapGrid_SetScreenShake(shakeX, shakeY, shakeDecay);
    FieldEffect_InitializeSparkles();
  }
  return 0;
}
