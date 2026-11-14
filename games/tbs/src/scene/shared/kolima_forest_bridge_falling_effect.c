#include "macros.h"
#include "field/map_grid_scroll.h"

// Kolima and Kolima Forest (SceneId_Kolima and SceneId_KolimaForestEntrance):
// the sparkling dust that falls over the party when Tret's curse strikes
// ("Watch out! It's happening again!").

s32 gFallingEffectOffset BSS_DATA;
s32 gFallingEffectWidth BSS_DATA;
u32 gFallingEffectState BSS_DATA;

/// A task: every eighth frame a speck (its script
/// gFallingEffectScript) starts 160 pixels up at a random point near the point
/// the map view follows, with SE_GLITTER every 64 frames. gFallingEffectState,
/// an enum KolimaCurseDustState, shapes the fall.
void Kolima_UpdateCurseDust(void) {
  struct Actor *actor = NULL;
  s32 *origin;
  struct Sprite *sprite;
  s32 x, z;

  switch (gFallingEffectState) {
  case KOLIMA_DUST_CLOSING:
    if (gFallingEffectWidth < 15000)
      gFallingEffectWidth += 50;
    if (gFallingEffectOffset > px(60))
      gFallingEffectOffset -= px(1) / 4;
    break;
  case KOLIMA_DUST_CLOSING_WIDE:
    if (gFallingEffectWidth < 30000)
      gFallingEffectWidth += 50;
    if (gFallingEffectOffset > px(24))
      gFallingEffectOffset -= px(1) / 4;
    break;
  case KOLIMA_DUST_DRIFTING:
    if (gFallingEffectOffset < -px(128)) {
      gFallingEffectState = KOLIMA_DUST_CENTRED;
    } else {
      gFallingEffectWidth += 50;
      gFallingEffectOffset -= px(1) / 4;
    }
    break;
  }
  if ((gFrameCount & 7) == 0 &&
      (actor = FieldActor_Create(SPRITE_SPARKLE, 0, 0, 0)) != 0) {
    origin = ((struct MapGridScrollWork *)gMapGridWork[0])->origin;
    if ((gFrameCount & 63) == 0)
      Sound_Play(SE_GLITTER);
    if (gFallingEffectState != KOLIMA_DUST_CENTRED)
      x = origin[0] + ((Random_Below(gFallingEffectWidth)) << 8) +
          gFallingEffectOffset;
    else
      x = origin[0] + (Random_Next() << 8) - px(128);
    z = origin[2] + (Random_Next() << 8) - px(128);
    actor->motionFlags = 0;
    actor->y.fixed = px(160);
    sprite = actor->sprite;
    actor->scaleX = 9 * Q16_ONE / 10;
    actor->scaleY = 9 * Q16_ONE / 10;
    actor->x.fixed = x;
    actor->z.fixed = z;
    sprite->flags = 0;
    actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    sprite->image.priority = 1;
    Actor_SelectAnimation(actor, 1);
    Actor_StartScript(actor, gFallingEffectScript);
  }
}
