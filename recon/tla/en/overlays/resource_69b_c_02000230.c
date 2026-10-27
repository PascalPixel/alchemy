// Draft of overlay 69b's Func_02000230, differ 112 of 196: the leader rises
// with a kind-280 object under it, and Func_020000d0 is the descent. Written
// from the listing (the ☀️ twin's shape was 0.45). The last spin is a goto loop:
// as a for loop loop.c keeps 0x2000 in r8 and the owner moves to r10 (differ
// 147). Left: global.c gives the leader r6 and the spawned object r7, the
// native the other way round, and with them most rows differ; the two while
// loops as explicit goto loops are worse (133). permute 5 minutes reached 45
// only with changes that are not the same program (an extra store, a duplicated
// if).
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "party_state.h"

extern s32 gData_02002b88[3];

struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Camera_FollowActor(s32 id, s32 a);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void WaitFrames(s32 frames);
void Func_02000054(void);
void Func_020000b4(void);
void Func_020021b4(struct ObjectRuntime *object, s32 a);
void Func_020021d4(struct ObjectRuntime *object);
void Func_0200224c(void);
void Func_02002254(void);
void Func_020022dc(s32 a);

void Func_02000230(s32 kind) {
  s32 owner = gPartyState.currentOwner;
  struct ObjectRuntime *leader = Object_GetById(owner);
  struct ObjectRuntime *effect;
  s32 *target;
  s32 i;

  Func_0200224c();
  Func_020022dc(0);
  target = gData_02002b88;
  if (kind == 30) {
    target[0] = 136 << 16;
    target[1] = 144 << 16;
    target[2] = 148 << 18;
  } else {
    target[0] = 248 << 16;
    target[1] = 144 << 16;
    target[2] = 144 << 18;
  }
  Scheduler_AddOrUpdateCallback(Func_020000b4, 0x480);
  Camera_FollowActor(owner, 1);
  leader->scaleX = 0x10000;
  leader->scaleY = 0x10000;
  effect = Object_Spawn(280, leader->x, leader->y, leader->z);
  Func_020021b4(effect, 2);
  effect->scaleX = 0x18000;
  effect->scaleY = 0x10000;
  ((struct FieldSprite *)effect->animation)->priority = 1;
  while (leader->y < 0x6c0000) {
    leader->y += 0x10000;
    leader->angle += 0x2000;
    effect->x = leader->x;
    effect->y = leader->y + -0x60000;
    effect->z = leader->z;
    WaitFrames(1);
  }
  leader->scaleX = 0;
  effect->scaleX = 0;
  while (leader->y < 0x900000) {
    leader->y += 0x10000;
    effect->y += 0x10000;
    WaitFrames(1);
  }
  effect->scaleX = 0x8000;
  effect->scaleY = 0xc000;
  for (i = 31; i >= 0; i--) {
    effect->scaleX += 0x800;
    effect->scaleY += 0x200;
    WaitFrames(1);
  }
  effect->scaleX = 0x18000;
  effect->scaleY = 0x10000;
  Func_020021b4(effect, 6);
  leader->scaleX = 0x6000;
  leader->scaleY = 0x6000;
  i = 19;
spin:
  leader->angle += 0x2000;
  leader->scaleX += 0x800;
  leader->scaleY = leader->scaleX;
  i--;
  WaitFrames(1);
  if (i >= 0)
    goto spin;
  Func_020021d4(effect);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(owner), 1);
  leader->angle = 0x4000;
  leader->terrainId = 1;
  leader->motionFlags = 3;
  leader->y = 0x900000;
  leader->terrainHeight = 0x900000;
  Scheduler_RemoveCallbackFar(Func_020000b4);
  Func_02000054();
  Func_02002254();
}
