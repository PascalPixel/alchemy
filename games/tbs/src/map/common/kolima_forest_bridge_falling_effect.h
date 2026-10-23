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
#include "script_layer.h"
#include "map_grid_scroll.h"
#include "songs.h"

// Kolima and Kolima Forest (MapId_Kolima and MapId_KolimaForestEntrance):
// the sparkling dust that falls over the party when Tret's curse strikes
// ("Watch out! It's happening again!").

s32 gFallingEffectOffset __attribute__((section(".bss")));
s32 gFallingEffectWidth __attribute__((section(".bss")));
u32 gFallingEffectState __attribute__((section(".bss")));

/// A map callback: every eighth frame a speck (its script
/// gFallingEffectScript) starts 160 pixels up at a random point near the point
/// the map view follows, with SE_246 every 64 frames. gFallingEffectState, an
/// enum KolimaCurseDustState, shapes the fall.
void Kolima_UpdateCurseDust(void) {
  struct ObjectEvent *objectEvent = NULL;
  s32 *origin;
  struct AnimationObject *sprite;
  s32 x, z;

  switch (gFallingEffectState) {
  case KOLIMA_DUST_CLOSING:
    if (gFallingEffectWidth < 15000)
      gFallingEffectWidth += 50;
    if (gFallingEffectOffset > PIXELS(60))
      gFallingEffectOffset -= PIXELS(1) / 4;
    break;
  case KOLIMA_DUST_CLOSING_WIDE:
    if (gFallingEffectWidth < 30000)
      gFallingEffectWidth += 50;
    if (gFallingEffectOffset > PIXELS(24))
      gFallingEffectOffset -= PIXELS(1) / 4;
    break;
  case KOLIMA_DUST_DRIFTING:
    if (gFallingEffectOffset < -PIXELS(128)) {
      gFallingEffectState = KOLIMA_DUST_CENTRED;
    } else {
      gFallingEffectWidth += 50;
      gFallingEffectOffset -= PIXELS(1) / 4;
    }
    break;
  }
  if ((gFrameCount & 7) == 0 &&
      (objectEvent = Engine_ObjectCreate(SPRITE_SPARKLE, 0, 0, 0)) != 0) {
    origin = ((struct MapGridScrollWork *)gMapGridWork[0])->origin;
    if ((gFrameCount & 63) == 0)
      Engine_AudioPlayCue(SE_246);
    if (gFallingEffectState != KOLIMA_DUST_CENTRED)
      x = origin[0] +
          (((u32)Engine_RandomNext() * gFallingEffectWidth >> 16) << 8) +
          gFallingEffectOffset;
    else
      x = origin[0] + (Engine_RandomNext() << 8) - PIXELS(128);
    z = origin[2] + (Engine_RandomNext() << 8) - PIXELS(128);
    objectEvent->motionFlags = 0;
    objectEvent->y.fixed = PIXELS(160);
    sprite = objectEvent->sprite;
    objectEvent->scaleX = 9 * Q16_ONE / 10;
    objectEvent->scaleY = 9 * Q16_ONE / 10;
    objectEvent->x.fixed = x;
    objectEvent->z.fixed = z;
    sprite->flags = 0;
    objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
    sprite->image.priority = 1;
    ObjectEvent_SetMode(objectEvent, 1);
    Engine_ObjectSetScript(objectEvent, gFallingEffectScript);
  }
}
