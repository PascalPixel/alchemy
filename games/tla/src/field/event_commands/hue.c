#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

struct WaveFxScroll {
  u16 lines[2][162];
  u8 unk288[2];
  u8 page;
  u8 unk28b[9];
  u8 delay;
};

/// Applies BG0 line scrolling with HBlank DMA.
void BattleEffect_ArmBg0HBlankDma(void) {
  struct WaveFxScroll *work = Ram_HeapSlots->unk4c[3];

  if (!work->delay) {
    u16 *lines = work->lines[work->page];
    volatile u16 *dma = REG_DMA0;

    dma[5] &= 0xc5ff; // Clear start timing and repeat.
    dma[5] &= 0x7fff; // Stop.
    (void)dma[5];
    // One halfword per HBlank, repeating with destination reload.
    Dma_Set(lines, REG_BG0HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
            (volatile u32 *)dma);
  }
}

/// One colour channel's level (0 to 31) at a hue in degrees: rising from 0
/// to 60, full from 60 to 180, falling from 180 to 240, else 0.
s32 BattleEffect_HueChannelRamp(s16 hue) {
  s32 level = 0;
  s16 angle = hue;

  angle = (angle + 360) % 360;
  if (angle < 60) {
    level = angle * 31 / 60;
  } else if (angle < 180) {
    level = 31;
  } else if (angle < 240) {
    level = (240 - angle) * 31 / 60;
  }
  return level;
}

struct WaveFxWork {
  u8 unk000[0x28b];
  u8 red;
  u8 green;
  u8 blue;
  u16 hue; ///< Degrees.
};

/// Advances the hue by four degrees and calculates its colour.
void BattleEffect_AdvanceHueCycle(void) {
  struct WaveFxWork *work = Ram_HeapSlots->unk4c[3];
  s16 r;
  s16 g;
  s16 b;
  u32 limit;

  r = BattleEffect_HueChannelRamp(work->hue);
  g = BattleEffect_HueChannelRamp(work->hue + 120);
  b = BattleEffect_HueChannelRamp(work->hue + 240);

  // Scale 0-31 down to 4-11.
  work->red = (r / 4 + 4) & 31;
  work->green = (g / 4 + 4) & 31;
  work->blue = (b / 4 + 4) & 31;
  limit = 359 << 16;
  if ((u32)((work->hue += 4) << 16) > limit)
    work->hue = 0;
}

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "objdisp.h"
#include "actor.h"
#include "frame.h"

void BattleEffect_SetCallbackWhenTargetUnset(struct Actor *target) {
  s32 tx;
  s32 ty;

  tx = target->targetX;
  if (tx == 0x80000000) {
    ty = target->targetY;
    if ((ty == tx) && (target->targetZ == ty)) {
      Actor_StartScriptFar((struct Actor *)target,
                           (u32)gBattleEffect_UntargetedObjectScript);
    }
  }
}

extern u8 gData_080f0e9c[];
void Animation_ApplyChildValuesFar(void *object, s32 value);

void BattleEffect_ApplyFlickerValue(void *object, s32 value) {
  s32 child = value - 1;

  if (value == 0)
    child = gData_080f0e9c[(gFrameCount >> 1) & 7];
  Animation_ApplyChildValuesFar(object, child);
}

#include "input.h"

u16 BattleEffect_GetCycledTableWord(void) {
  struct InputState *input = &gInput;
  u16 *words = gBattleEffect_CyclePatternWords;

  return *(u16 *)((u8 *)words + (((input->held >> 4) & 15) << 1));
}

u16 BattleEffect_GetEightWayHeadingWord(void) {
  struct InputState *input = &gInput;
  u16 *words = gBattleEffect_EightWayHeadingWords;

  return *(u16 *)((u8 *)words + (((input->held >> 4) & 15) << 1));
}

// Effects: motion of the main object in an item-breaking effect.

#include "edition.h"
#include "fixed_math.h"
#include "object_effect_scripts.h"
#include "field_event.h"

struct ItemBreakFragmentPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct ItemBreakFragmentSource {
  u8 reserved00[6];
  u16 angle; ///< Facing angle (0x10000 is one turn).
  s32 x;
  s32 y;
  s32 z;
  u8 reserved14[70];
  u8 flag5a;
  u8 reserved5b[13];
  struct ItemBreakFragmentSource *target; ///< Facing target.
};

struct ItemBreakFragmentObject {
  u8 reserved00[72];
  s32 field48; ///< Gravity (same offset as gravity).
  u8 reserved4c[9];
  u8 mode55; ///< Same offset as motionFlags.
  u8 reserved56[8];
  u16 field5e;
};

extern s32 ArcTan2(s32, s32);

extern void Vector_AddPolarOffset(s32, s32, struct ItemBreakFragmentPosition *);
extern struct ItemBreakFragmentObject *Actor_Spawn(s32, s32, s32, s32);

/// Updates the broken item's main object: with a facing target it turns toward
/// it by at most 0x1000 (22.5 degrees), then emits one fragment particle
/// nearby.
/// src: main object.
void BattleEffect_UpdateItemBreakFragment(struct ItemBreakFragmentSource *src) {
  struct ItemBreakFragmentSource *target;
  struct ItemBreakFragmentPosition pos;
  struct ItemBreakFragmentObject *obj;
  s32 turn;
  s32 dist;

  target = src->target;
  if (target != 0) {
    s32 dx = target->x - src->x;
    s32 dz = target->z - src->z;

    if (dx != 0 || dz != 0) {
      s32 dir = ArcTan2(dz, dx);
      s32 cur = src->angle;
      s32 lim = 128;

      turn = (s16)(dir - cur);
      lim <<= 5;
      if (turn > lim)
        turn = lim;
      if (turn < -0x1000)
        turn = -0x1000;
      src->angle += turn;
    }
    src->flag5a = 0;
  }

  // Emit the particle with a y offset of 8-24 and a random direction
  // at radius 0-48 from the main object.
  pos.x = src->x;
  pos.y = src->y - (Random16() << 4) - 0x80000;
  pos.z = src->z;
  dist = Random16() * 3;
  dist <<= 4;
  Vector_AddPolarOffset(dist, Random16(), &pos);

  obj = Actor_Spawn(0x2a1, pos.x, pos.y, pos.z);
  if (obj != 0) {
    obj->mode55 = 2;
    obj->field48 = 0x1999;
    Actor_SetMode(obj, 0);
    obj->field5e = 12;
    Actor_StartScriptFar((struct Actor *)obj,
                         gBattleEffect_SharedParticleScript);
  }
}
