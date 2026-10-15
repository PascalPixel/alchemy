/* Draft, differ 16 of 120 (no device): summon 25's change of backdrop
 * behind the wipe (Func_08191958's midpoint). The colour loop takes The
 * Broken Seal's matched channel form (BattlePres_RunRisingWall's darkening:
 * blue and green through (u16)color shifts, which agscc does in halfword
 * mode, so the 31 they are masked with comes from the pool; red as
 * red = 31; red &= color).
 * Left: the native loads REG_BG1HOFS's pooled zero before it shifts the
 * register address (one reordered row); the colour loop's literal pool sits
 * after the green clamp in the native, after the blue clamp here, which
 * moves the pool's offsets and the clamps' branches (about 12 rows).
 * Tried: u16 color with plain (color >> n) & 31 channels (45: red is then
 * also taken from the halfword shift), s32 or u32 color with plain shifts
 * (38), (u16) casts on the masked channels (35-45), other channel orders
 * (37-58). alchemy permute 5 minutes: best 8, by computing red << 5 into a
 * temporary before the blue clamp (a forced temporary; not kept). */
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "motobj.h"
#include "bg_scroll.h"
#include "battle_display.h"
#include "battle_effect_work.h"

extern char ResourceId_VioletSkyBackdrop;
extern s32 gCameraSceneParameters[];

struct BattleObj *GetBattleObjectSlotFar(s32 objectId);
u32 Random16(void);
void BattleActor_SpawnObjectsForListFar(s16 *targets, s32 mode);
void BattleBackground_LoadFar(s32 mode, s32 background, s32 level);

// Summon 25's backdrop change: clears the targets' ground motion and drops
// each a little above the ground at a random height, empties the actor
// list, loads the violet sky and recolours its 128 BG palette entries from
// 96 on: each channel is darkened and the three are rotated (red to green,
// green to blue, blue to red).
void Func_0819253c(void) {
  struct BattlePresentationTransition *transition =
      Ram_HeapSlots->transitionWork;
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  s16 list[14];
  u16 *palette;
  s32 i;

  REG_BG1HOFS = 0;
  gCameraSceneParameters[4] = 240;

  // The targets fall in from just above the ground, halfway closer in depth.
  for (i = 0; i != work->effect->count; i++) {
    struct MotionObject *object =
        GetBattleObjectSlotFar(work->effect->actors[i])->object;

    object->x = 0;
    object->y = (16 - (Random16() & 15)) << 16;
    object->z /= 2;
    object->verticalMotionStrength = 0;
  }

  // No actor objects over the new backdrop.
  list[0] = 0xff;
  BattleActor_SpawnObjectsForListFar(list, 0);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);

  palette = (u16 *)BG_PLTT + 96;
  for (i = 0; i != 128; i++) {
    s32 color = *palette;
    s32 blue = ((u16)color >> 10) & 31;
    s32 green = ((u16)color >> 5) & 31;
    s32 red = 31;

    red &= color;
    red -= 16;
    green -= 24;
    blue -= 20;

    if (red < 0)
      red = 0;
    if (green < 0)
      green = 0;
    if (blue < 0)
      blue = 0;
    *palette++ = green << 10 | red << 5 | blue;
  }
  transition->active = 1;
  gBgScroll[1].x = 0;
}
