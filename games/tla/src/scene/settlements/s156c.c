#include "local.h"

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

extern const u8 gData_02002310[];

void Func_020000b8(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, s32 flags,
                   struct SettlementsSpawn *spawn);

void Func_0200156c(s32 id) {
  /* FAKEMATCH: unused; its twelve bytes are the gap the native frame keeps
   * above the spawn record, where Func_02001620 holds its velocity. */
  struct SettlementsVector velocity;
  struct SettlementsSpawn spawn;
  struct ObjectRuntime *object = Object_GetById(id);

  spawn.kind = 0x115;
  spawn.scaleX = 0xcccc;
  spawn.scaleY = 0xcccc;
  spawn.speed = 0x10000;
  spawn.acceleration = 0x10000;
  spawn.script = gData_02002310;
  Func_020021b0(154);
  Func_020000b8(object->x, object->y, object->z, 0, 0, 0, 0x1c0000, &spawn);
}
