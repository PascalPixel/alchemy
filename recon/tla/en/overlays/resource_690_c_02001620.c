// Draft, differ 38 of 58, skeleton gap 7 (tla-en, 2026-11-01)
// Twin of CrossboneIsle_SpawnPillarMovedBurst: nine effects in a ring (angles 0
// to 16 << 12, x stretched by a third), each spawned with Effect_AdvanceMotion,
// the same calls, divisor, flags and frame. One local holds the angle and then
// the stretched x, as the native's r6 does; that took differ from 50 to 38.
// Left: the native forms the velocity's address (sp + 56) inside the loop,
// after the first call, so it sits in r5, the object in r7, the sine in r8 and
// the index in r10. Here the address leaves the loop, as it does in the twin's
// own build, which moves those registers and the store of z. In a for loop
// loop.c moves it (insn 43, life 19); in a goto loop GCSE does. CSE merges the
// separate address pseudos first, so only a shorter life would keep it inside.
// Tried, no gain: the twin's separate angle and x locals, for, while, do and
// goto loops, a pointer local set once, twice or after the first call, an
// array, an inline setter, declaration orders, block-scope locals, z stored
// directly, the y read into a local. A one-pass block between the stores made
// it worse and would be a new device.
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

void Effect_AdvanceMotion(struct Actor *object);
void Func_020000b8(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, s32 flags,
                   struct SettlementsSpawn *spawn);
s32 Math_Sine(s32 angle);

void Func_02001620(s32 id) {
  struct Actor *object;
  struct SettlementsVector velocity;
  struct SettlementsSpawn spawn;
  u32 i;
  s32 value;
  s32 velocityZ;

  object = Actor_GetById(id);
  spawn.update = Effect_AdvanceMotion;
  for (i = 0; i <= 16; i += 2) {
    value = i << 12;
    velocity.x = Math_Cosine(value);
    velocity.y = 0;
    velocityZ = Math_Sine(value);
    value = velocity.x;
    velocity.z = velocityZ;
    value += value / 3;
    velocity.x = value;
    Func_020000b8(object->x, object->y, object->z, value, velocity.y, velocityZ,
                  0x1000001, &spawn);
  }
}
