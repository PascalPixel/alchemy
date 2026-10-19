#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "types.h"
#include "field_event.h"
#include "songs.h"

// Kolima and its forest bridge: the sparkling dust that falls over the party
// when Tret's curse strikes ("Watch out! It's happening again!").

struct SceneEffectOrigin {
  s32 x;
  s32 y;
  s32 z;
};
struct SceneEffectWork {
  struct SceneEffectOrigin *origin;
};

s32 gFallingEffectOffset __attribute__((section(".bss")));
s32 gFallingEffectWidth __attribute__((section(".bss")));
u32 gFallingEffectState __attribute__((section(".bss")));

/// A scene callback: every eighth frame a speck (object 285, its script
/// gFallingEffectScript) starts 160 pixels up at a random point near the map
/// work's origin, with SE_246 every 64 frames. gFallingEffectState shapes the
/// fall: 0 within 128 pixels of the origin; 1 and 2 over a band that widens
/// (by 50 a frame, to 15000 and 30000) as it moves towards the origin; 3
/// drifting on until it is far past, then back to 0.
void Kolima_UpdateCurseDust(void) {
  struct FieldActor *object = NULL;
  struct SceneEffectOrigin *origin;
  struct AnimationObject *sprite;
  s32 x, z;

  switch (gFallingEffectState) {
  case 1:
    if (gFallingEffectWidth < 15000)
      gFallingEffectWidth += 50;
    if (gFallingEffectOffset > 0x3c0000)
      gFallingEffectOffset -= 0x4000;
    break;
  case 2:
    if (gFallingEffectWidth < 30000)
      gFallingEffectWidth += 50;
    if (gFallingEffectOffset > 0x180000)
      gFallingEffectOffset -= 0x4000;
    break;
  case 3:
    if (gFallingEffectOffset < -0x800000) {
      gFallingEffectState = 0;
    } else {
      gFallingEffectWidth += 50;
      gFallingEffectOffset -= 0x4000;
    }
    break;
  }
  if ((gFrameCount & 7) == 0 &&
      (object = Engine_ObjectCreate(285, 0, 0, 0)) != 0) {
    origin = ((struct SceneEffectWork *)gMapWork[0])->origin;
    if ((gFrameCount & 63) == 0)
      Engine_AudioPlayCue(SE_246);
    if (gFallingEffectState != 0)
      x = origin->x +
          (((u32)Engine_RandomNext() * gFallingEffectWidth >> 16) << 8) +
          gFallingEffectOffset;
    else
      x = origin->x + (Engine_RandomNext() << 8) - 0x800000;
    z = origin->z + (Engine_RandomNext() << 8) - 0x800000;
    object->motionFlags = 0;
    object->y.fixed = 0xa00000;
    sprite = object->sprite;
    object->scaleX = 0xe666;
    object->scaleY = 0xe666;
    object->x.fixed = x;
    object->z.fixed = z;
    sprite->flags = 0;
    object->priorityFlags &= ~1;
    sprite->image.priority = 1;
    Object_SetMode(object, 1);
    Engine_ObjectSetScript(object, gFallingEffectScript);
  }
}
