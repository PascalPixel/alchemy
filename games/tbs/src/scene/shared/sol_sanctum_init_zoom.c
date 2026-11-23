/// @file
/// The initialization that two rooms of Sol Sanctum share,
/// SceneId_SolSanctumMaze and SceneId_SolSanctumCorridor.

#include "flags.h"

/// The scene opens in the window transition over 16 frames; after the
/// eruption the ground rumbles and shakes, and the weather's sparkles start
/// (FieldEffect_InitializeSparkles).
s32 Scene_Initialize(void) {
  struct FieldWork *work;

  FIXED shakeX = toFIXED(1);
  FIXED shakeY = toFIXED(1);
  FIXED shakeDecay = FIXED_ONE;

  work = gFieldWork;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  work->transitionFrames = MAP_TRANSITION_FRAMES;
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Scene_QueueMusic(SE_RUMBLE);
    MapGrid_SetScreenShake(shakeX, shakeY, shakeDecay);
    FieldEffect_InitializeSparkles();
  }
  return 0;
}
