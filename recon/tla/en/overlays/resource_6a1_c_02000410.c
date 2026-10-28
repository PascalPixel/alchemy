// Draft, differ 115 of 216: overlay 6a1's copy of the function drafted in
// resource_698_c_020003cc.c (overlay 698's Func_020003cc, differ 115 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "frame.h"
#include "call.h"

struct Actor *Actor_GetById(s32 id);
void Func_020021f0(void);
void Func_02002290(s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct Actor *Func_02002268(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02002190(struct Actor *object, s32 x, s32 y, s32 z);
void Func_020022c8(s32 sound);
void Func_02002198(struct Actor *object);
void Camera_FollowActor(s32 id, s32 flag);
void Func_020021f8(void);

void Func_02000410(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_GetById(gAdventure.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  s32 facing;
  s32 i;

  Func_020021f0();
  Func_02002290(0);
  i = 0;
  *(u16 *)(work + 0xd46) = 0;
  actor->motionFlags = 4;
  Actor_SetSingleChildFlags(actor, 0);
  if (*((u8 *)linked + 0x63) != 0) {
  rise:
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->angle = (u8)gFrameCount << 12;
    actor->priorityFlags ^= 2 << (gFrameCount & 1);
    wait(1);
    if (*((u8 *)linked + 0x63) != 0)
      goto rise;
  }
  *(u16 *)(work + 0xd44) = 1;
  *(u16 *)(work + 0xd46) = 1;
  facing = linked->angle / 0x2000;
  actor->x = (((linked->x >> 19) & ~1) + facing - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - (facing & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  actor->priorityFlags = 1;
  actor->motionFlags = 3;
  Actor_SetSingleChildFlags(actor, 1);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  shadow = Func_02002268();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  shadow->motionFlags = 0;
  Func_02002190(shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z),
                actor->z);
  i = 0;
  if (actor->y > actor->terrainHeight + 0x100000) {
  fall:
    i++;
    wait(1);
    if (i <= 59 && actor->y > actor->terrainHeight + 0x100000)
      goto fall;
  }
  Func_020022c8(127);
  i = 0;
  if (actor->velocityY != 0) {
  settle:
    i++;
    wait(1);
    if (i <= 59 && actor->velocityY != 0)
      goto settle;
  }
  Func_02002198(shadow);
  Actor_GetById(gAdventure.currentOwner)->priorityFlags |= 1;
  Camera_FollowActor(gAdventure.currentOwner, 1);
  *(u16 *)(work + 0xd46) = 1;
  *(u16 *)(work + 0x154) = 0;
  Func_020021f8();
}
