/// @file
/// The initialization that two rooms of Sol Sanctum share,
/// SceneId_SolSanctumMaze and SceneId_SolSanctumCorridor.
#include "types.h"
#include "actor.h"
#include "songs.h"
#include "flags.h"

/// The scene opens in the window transition over 16 frames. Once Mt. Aleph has
/// erupted (FLAG_SOL_SANCTUM_ERUPTED), the rooms rumble (SE_RUMBLE), the map
/// shakes without end and sparkles start.
s32 SolSanctumRooms_Initialize(void) {
  struct FieldWork *work;

  s32 shakeX = 0x10000;
  s32 shakeY = 0x10000;
  s32 shakeDecay = 0x10000;

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
