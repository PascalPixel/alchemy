// Sol Sanctum's corridor and maze: the scenes' initialization.
#include "types.h"
#include "field_obj.h"
#include "songs.h"

// The scene opens in the window transition over 16 frames. Once Mt. Aleph has
// erupted (flag 0x814), the rooms rumble (SE_141), the map shakes without end
// and sparkles start.
s32 Scene_InitSceneRequestAndCameraZoom(void) {
  struct FieldWork *work;

  s32 shakeX = 0x10000;
  s32 shakeY = 0x10000;
  s32 shakeDecay = 0x10000;

  work = (struct FieldWork *)gEventWork;
  work->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  work->transitionFrames = 16;
  if (Engine_GameFlagIsSet(0x814) != 0) {
    QueueSceneSound(SE_141);
    Engine_WorkSetValuesIfNonNegative(shakeX, shakeY, shakeDecay);
    InitializeSceneRecordBuffer();
  }
  return 0;
}
