#include "types.h"
#include "iwram_call.h"

struct OrbitEffect {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[15];
  u8 priorityFlags;
  u8 unk24[4];
  s32 scale;
  u8 unk2c[28];
  s32 speed;
  u8 unk4c[24];
  u16 timer;
  s16 pause;
  struct OrbitEffect *anchor;
  void (*update)(struct OrbitEffect *);
};

u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02000e2c(struct OrbitEffect *effect, const u32 *script);
void Func_02000e5c(struct OrbitEffect *effect, s32 priority);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);
struct OrbitEffect *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);

extern u32 gData_020015c0[];

void Func_02000474(struct OrbitEffect *effect) {
  s32 radius;
  s32 angle;
  s32 dx;
  s32 dz;

  radius = Random16Far() + 0x20000;
  angle = effect->angle;
  dx = Iwram_MulQ16(radius, Math_Cosine(angle));
  dz = Iwram_MulQ16(radius, Math_Sine(angle));
  effect->x += dx;
  effect->z += dz;
  effect->angle += 0xfff0;
  /* FAKEMATCH: the empty one-pass block forms the pause address after the angle
   * store (differ 2 without it), as the battle twin
   * BattleEffect_WanderAroundAnchor. */
  do {
  } while (0);
  if (effect->pause != 0) {
    effect->pause--;
    effect->angle += 0x800;
  } else if ((Random16Far() << 5) >> 16 == 0) {
    effect->pause = ((Random16Far() << 4) >> 16) + 8;
  }
  if (++effect->timer == 71)
    Func_02000e2c(effect, gData_020015c0);
}
