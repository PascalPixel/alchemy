// Draft of Func_02001620 (overlay 690, 132 bytes), differ 50 of 58: the native
// takes the address of the velocity inside the loop, after the cosine call
// (add r5, sp, #56), with the object in r7 and the angle step in r10; this
// form takes the address once before the loop and keeps it in r7, with the
// object in r10. Measured: a for loop, a goto loop, the cosine in a local and
// the velocity through a pointer set inside the loop all give the same code.
// Func_0200156c, which spawns through the same record, matches (s156c.c).
#include "../../../../../games/tla/src/scene/settlements/local.h"

struct SettlementsVector {
  s32 x;
  s32 y;
  s32 z;
};

struct SettlementsSpawn {
  s32 unk00;
  s32 unk04;
  s32 scaleX;
  s32 scaleY;
  s32 speed;
  s32 acceleration;
  u16 kind;
  const u8 *script;
  s32 unk20;
  void *update;
};

s32 Math_Sine(s32 angle);
void Effect_AdvanceMotion(void);
void Func_020000b8(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, s32 flags,
                   struct SettlementsSpawn *spawn);

void Func_02001620(s32 id) {
  struct SettlementsVector velocity;
  struct SettlementsSpawn spawn;
  struct Actor *object = Actor_GetById(id);
  u32 i;

  spawn.update = Effect_AdvanceMotion;
  i = 0;
loop:
  {
    s32 c = Math_Cosine(i << 12);
    struct SettlementsVector *v = &velocity;

    v->x = c;
    v->y = 0;
    v->z = Math_Sine(i << 12);
    v->x += v->x / 3;
    Func_020000b8(object->x, object->y, object->z, v->x, v->y, v->z, 0x1000001,
                  &spawn);
  }
  i += 2;
  if (i <= 16)
    goto loop;
}
