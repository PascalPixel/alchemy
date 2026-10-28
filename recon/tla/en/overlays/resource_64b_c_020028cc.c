// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 4 of
// 91 with nextFrame's store in a one-pass block, found by a greedy device
// search (6 without it).
// Draft of Func_020028cc (overlay 64B, 91 instructions), differ 8: the puff
// that trails actor 8 every sixteenth frame, ported from ☀️'s
// WorldMap_SpawnActorEightPuff (⚓️ creates object 285 with Func_02004b30).
// Object creation through a forwarding inline took it from differ 34 to 8: the
// arguments then build in r0 as the native. Three swaps remain, each where the
// native loads earlier: obj->x after the * 48 multiply (here after its first
// shift) and after the << 5 random (here after the shift); and the sprite's
// byte 9 right after nextFrame's store, before the leader's sprite pointer
// (then the native fills the load stall with the first call's adds r0, r5).
// sched2 gives the shifts and the leader's chain the higher priority. Measured:
// the random value apart from its shifts in the else branch (no change).
// alchemy permute, 5 minutes (25,361 candidates): nothing closer.
#include "types.h"
#include "actor.h"
#include "field_sprite.h"

extern u32 gFrameCount;
extern const u8 gData_02004f10[];

struct Actor *Actor_GetById(s32 id);
struct Actor *Func_02004b30(s32 kind, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_02004b18(struct Actor *object, s32 mode);
void Func_02004b20(struct Actor *object, const u8 *script);

static inline struct Actor *CreateObject(s32 kind, s32 x, s32 y,
                                                 s32 z) {
  return Func_02004b30(kind, x, y, z);
}

void Func_020028cc(void) {
  struct Actor *leader;
  struct Actor *obj;
  struct FieldSprite *spr;
  u32 value;

  if ((gFrameCount & 15) != 0)
    return;
  leader = Actor_GetById(8);
  obj =
      CreateObject(285, leader->x - 0x200000, leader->y, leader->z - 0x100000);
  if (obj == NULL)
    return;
  obj->scaleX = 0x8000;
  obj->scaleY = 0x8000;
  spr = obj->animation;
  if ((Random16Far() * 2) >> 16) {
    s32 back = ((Random16Far() * 48) >> 16) << 16;

    obj->x -= back >> 1;
    obj->z -= back;
  } else {
    value = Random16Far();
    obj->x += ((value << 5) >> 16) << 16;
    obj->z += (s32)(((value << 5) >> 16) << 16) >> 1;
  }
  do {
  spr->nextFrame = 0;
  } while (0);
  spr->priority = ((struct FieldSprite *)leader->animation)->priority;
  obj->priorityFlags |= 2;
  obj->motionFlags = leader->motionFlags;
  Actor_SetPartAttribute(obj, 9);
  Func_02004b18(obj, 2);
  Func_02004b20(obj, gData_02004f10);
}
