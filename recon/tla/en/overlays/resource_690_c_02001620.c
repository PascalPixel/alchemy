// Draft, differ 50 of 58, registers: nine effects in a ring (angles 0 to 16 <<
// 12, x stretched by a third), each spawned through Effect_Spawn with
// Effect_AdvanceMotion. The native forms the velocity's address (sp + 56) anew
// inside the loop and keeps the object in r7, the index in r10 and the sine in
// r8; the draft keeps the address in r7 across the loop. Tried: both
// declaration orders, the vector declared in the loop, a pointer local, an
// angle local, a goto loop, a 1-minute permute.
#include "../../../../games/tla/src/scene/settlements/local.h"

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

void Effect_AdvanceMotion(struct ObjectRuntime *object);
void Func_020000b8(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, s32 flags,
                   struct SettlementsSpawn *spawn);
s32 Math_Sine(s32 angle);

void Func_02001620(s32 id) {
  struct SettlementsVector velocity;
  struct SettlementsSpawn spawn;
  struct ObjectRuntime *object = Object_GetById(id);
  u32 i;

  spawn.update = Effect_AdvanceMotion;
  for (i = 0; i <= 16; i += 2) {
    velocity.x = Math_Cosine(i << 12);
    velocity.y = 0;
    velocity.z = Math_Sine(i << 12);
    velocity.x += velocity.x / 3;
    Func_020000b8(object->x, object->y, object->z, velocity.x, velocity.y,
                  velocity.z, 0x1000001, &spawn);
  }
}
