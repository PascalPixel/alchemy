// Draft of overlay 69b's Func_020000d0, differ 58 of 145: the leader sinks 36
// frames, a kind-280 object spawns at it, both go down to height 32 while the
// leader spins, then the leader spins 15 frames more and is restored. Written
// from the listing (the ☀️ twin's shape was 0.49). Left: (1) the native stores 0
// into terrainId from a pool word loaded just before the store; here the pool
// word is loaded inside the last spin loop and kept in r8, which pushes the
// owner to r10; (2) the native rotates the descent loop (its test at the top
// and again at the bottom) where this jumps to one test at the bottom; (3) the
// native places a literal pool after the first if/else; (4) the scale and
// priority stores of the spawned object are scheduled differently. The last
// spin is a goto loop (as a for loop loop.c keeps 0x2000 in r8, differ 63).
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
void Task_Add(void (*callback)(void), s32 priority);
void Task_RemoveFar(void (*callback)(void));
void WaitFrames(s32 frames);
void Func_02000054(void);
void Func_020000b4(void);
void Func_020021b4(struct Actor *object, s32 a);
void Func_020021d4(struct Actor *object);
void Func_0200224c(void);
void Func_02002254(void);
void Func_020022dc(s32 a);

void Func_020000d0(s32 kind) {
  s32 owner = gAdventure.currentOwner;
  struct Actor *leader = Actor_GetById(owner);
  struct Actor *effect;
  s32 i;
  s32 *target;

  Func_0200224c();
  Func_020022dc(0);
  target = gData_02002b88;
  if (kind == 32) {
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
  for (i = 35; i >= 0; i--) {
    leader->y += -0x10000;
    WaitFrames(1);
  }
  leader->scaleX = 0x10000;
  leader->scaleY = 0x10000;
  effect = Actor_Spawn(280, leader->x, leader->y, leader->z);
  Func_020021b4(effect, 2);
  effect->scaleX = 0x18000;
  effect->scaleY = 0x10000;
  ((struct FieldSprite *)effect->animation)->priority = 1;
  while (leader->y > 0x200000) {
    leader->y += -0x10000;
    leader->angle += 0x2000;
    effect->x = leader->x;
    effect->y = leader->y + -0x60000;
    effect->z = leader->z;
    WaitFrames(1);
  }
  Func_020021b4(effect, 6);
  i = 14;
spin:
  leader->angle += 0x2000;
  i--;
  WaitFrames(1);
  if (i >= 0)
    goto spin;
  Func_020021d4(effect);
  Actor_SetSingleChildFlags(Actor_GetById(owner), 1);
  leader->angle = 0x4000;
  leader->terrainId = 0;
  leader->motionFlags = 3;
  Task_RemoveFar(Func_020000b4);
  Func_02000054();
  Func_02002254();
}
