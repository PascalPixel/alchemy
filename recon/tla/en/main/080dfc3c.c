// Near miss, differ 2 of 69 (2026-10-15), ported from ☀️'s BattleFx_CircleAnchor
// onto radial.c's OrbitEffect. One pair: the listing loads the angle before it
// starts the 0x800 step (ldrh r3, [r7, #6]; movs r0, #128); this starts the
// step first. A forced temporary with a one-pass block (as overlay 69e's
// s0810.c) gives differ 4, the step held in an empty asm differ 12; no single
// barrier helped. Belongs after radial.c's BattleFx_WanderAroundAnchor.
#include "iwram_call.h"
#include "system.h"
#include "fixed_math.h"
#include "types.h"

struct OrbitEffect {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[20];
  s32 scale;
  u8 unk2c[28];
  s32 speed;
  u8 unk4c[24];
  u16 timer;
  s16 pause;
  struct OrbitEffect *anchor;
  void (*update)(struct OrbitEffect *);
};

void BattleFx_WanderAroundAnchor(struct OrbitEffect *effect);

void Func_080dfc3c(struct OrbitEffect *effect) {
  struct OrbitEffect *anchor;
  s32 angle;
  s32 r;
  s32 dx;
  s32 dz;

  anchor = effect->anchor;
  r = 0x80000;
  angle = effect->angle;
  dx = Iwram_MulQ16(r, Trig_Cos(angle));
  dz = Iwram_MulQ16(r, Trig_Sin(angle));
  effect->x = anchor->x + dx;
  effect->z = anchor->z + dz;
  effect->angle += 0x800;
  if (++effect->timer == 121) {
    effect->update = BattleFx_WanderAroundAnchor;
    effect->timer = 0;
    effect->pause = 0;
    effect->speed = 0x1999;
    effect->scale = 0x30000;
    effect->angle = Random16();
  }
}
