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
#include "map_scroll.h"
#include "songs.h"

// Kolima and Kolima Forest (SceneId_Kolima and SceneId_KolimaForestEntrance):
// the sparkling dust that falls over the party when Tret's curse strikes
// ("Watch out! It's happening again!").

s32 gFallingEffectOffset __attribute__((section(".bss")));
s32 gFallingEffectWidth __attribute__((section(".bss")));
u32 gFallingEffectState __attribute__((section(".bss")));

/// A scene callback: every eighth frame a speck (its script
/// gFallingEffectScript) starts 160 pixels up at a random point near the point
/// the map view follows, with SE_246 every 64 frames. gFallingEffectState, an
/// enum KolimaCurseDustState, shapes the fall.
void Kolima_UpdateCurseDust(void) {
  struct FieldActor *object = NULL;
  s32 *origin;
  struct AnimationObject *sprite;
  s32 x, z;

  switch (gFallingEffectState) {
  case KOLIMA_DUST_CLOSING:
    if (gFallingEffectWidth < 15000)
      gFallingEffectWidth += 50;
    if (gFallingEffectOffset > 0x3c0000)
      gFallingEffectOffset -= 0x4000;
    break;
  case KOLIMA_DUST_CLOSING_WIDE:
    if (gFallingEffectWidth < 30000)
      gFallingEffectWidth += 50;
    if (gFallingEffectOffset > 0x180000)
      gFallingEffectOffset -= 0x4000;
    break;
  case KOLIMA_DUST_DRIFTING:
    if (gFallingEffectOffset < -0x800000) {
      gFallingEffectState = KOLIMA_DUST_CENTRED;
    } else {
      gFallingEffectWidth += 50;
      gFallingEffectOffset -= 0x4000;
    }
    break;
  }
  if ((gFrameCount & 7) == 0 &&
      (object = Engine_ObjectCreate(285, 0, 0, 0)) != 0) {
    origin = ((struct MapScrollWork *)gMapWork[0])->origin;
    if ((gFrameCount & 63) == 0)
      Engine_AudioPlayCue(SE_246);
    if (gFallingEffectState != KOLIMA_DUST_CENTRED)
      x = origin[0] +
          (((u32)Engine_RandomNext() * gFallingEffectWidth >> 16) << 8) +
          gFallingEffectOffset;
    else
      x = origin[0] + (Engine_RandomNext() << 8) - 0x800000;
    z = origin[2] + (Engine_RandomNext() << 8) - 0x800000;
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
