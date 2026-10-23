/// @file
/// The initialization that two rooms of Sol Sanctum share,
/// MapId_SolSanctumMaze and MapId_SolSanctumCorridor.
#include "types.h"
#include "object_event.h"
#include "songs.h"
#include "flags.h"

/// The scene opens in the window transition over 16 frames. Once Mt. Aleph has
/// erupted (FLAG_SOL_SANCTUM_ERUPTED), the rooms rumble (SE_141), the map
/// shakes without end and sparkles start.
s32 SolSanctumRooms_Initialize(void) {
  struct FieldWork *work;

  s32 shakeX = 0x10000;
  s32 shakeY = 0x10000;
  s32 shakeDecay = 0x10000;

  work = (struct FieldWork *)gEventWork;
  work->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  work->transitionFrames = 16;
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    QueueMapSound(SE_141);
    Engine_WorkSetValuesIfNonNegative(shakeX, shakeY, shakeDecay);
    InitializeSparkleRecordBuffer();
  }
  return 0;
}
