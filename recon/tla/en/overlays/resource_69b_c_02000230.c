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
#include "actor.h"
#include "field_sprite.h"
#include "adventure.h"

extern s32 gData_02002b88[3];

struct Actor *Actor_GetById(s32 id);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Camera_FollowActor(s32 id, s32 a);
void Task_Add(void (*func)(void), s32 priority);
void Task_RemoveFar(void (*func)(void));
void wait(s32 frames);
void Func_02000054(void);
void Func_020000b4(void);
void Func_020021b4(struct Actor *object, s32 a);
void Func_020021d4(struct Actor *object);
void Func_0200224c(void);
void Func_02002254(void);
void Func_020022dc(s32 a);

void Func_02000230(s32 kind) {
  s32 owner = gAdventure.currentOwner;
  struct Actor *leader = Actor_GetById(owner);
  struct Actor *effect;
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
  Task_Add(Func_020000b4, 0x480);
  Camera_FollowActor(owner, 1);
  leader->scaleX = 0x10000;
  leader->scaleY = 0x10000;
  effect = Actor_Spawn(280, leader->x, leader->y, leader->z);
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
    wait(1);
  }
  leader->scaleX = 0;
  effect->scaleX = 0;
  while (leader->y < 0x900000) {
    leader->y += 0x10000;
    effect->y += 0x10000;
    wait(1);
  }
  effect->scaleX = 0x8000;
  effect->scaleY = 0xc000;
  for (i = 31; i >= 0; i--) {
    effect->scaleX += 0x800;
    effect->scaleY += 0x200;
    wait(1);
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
  wait(1);
  if (i >= 0)
    goto spin;
  Func_020021d4(effect);
  Actor_SetSingleChildFlags(Actor_GetById(owner), 1);
  leader->angle = 0x4000;
  leader->terrainId = 1;
  leader->motionFlags = 3;
  leader->y = 0x900000;
  leader->terrainHeight = 0x900000;
  Task_RemoveFar(Func_020000b4);
  Func_02000054();
  Func_02002254();
}
