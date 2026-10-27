// Draft, differ 115 of 216: overlay 69a's copy of the function drafted in
// resource_698_c_020003cc.c (overlay 698's Func_020003cc, differ 115 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "frame.h"
#include "call.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02001b24(void);
void Func_02001ba4(s32 value);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void WaitTicks(s32 ticks);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct ObjectRuntime *Func_02001b7c(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02001ac4(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02001bd4(s32 sound);
void Func_02001acc(struct ObjectRuntime *object);
void Camera_FollowActor(s32 id, s32 flag);
void Func_02001b2c(void);

void Func_0200045c(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *actor = Object_GetById(gPartyState.currentOwner);
  struct ObjectRuntime *linked = actor->linkedObject;
  struct ObjectRuntime *shadow;
  s32 facing;
  s32 i;

  Func_02001b24();
  Func_02001ba4(0);
  i = 0;
  *(u16 *)(work + 0xd46) = 0;
  actor->motionFlags = 4;
  ObjectDispatch_SetSingleChildFlags(actor, 0);
  if (*((u8 *)linked + 0x63) != 0) {
  rise:
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->angle = (u8)gFrameCount << 12;
    actor->priorityFlags ^= 2 << (gFrameCount & 1);
    WaitTicks(1);
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
  ObjectDispatch_SetSingleChildFlags(actor, 1);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  shadow = Func_02001b7c();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  shadow->motionFlags = 0;
  Func_02001ac4(shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z),
                actor->z);
  i = 0;
  if (actor->y > actor->terrainHeight + 0x100000) {
  fall:
    i++;
    WaitTicks(1);
    if (i <= 59 && actor->y > actor->terrainHeight + 0x100000)
      goto fall;
  }
  Func_02001bd4(127);
  i = 0;
  if (actor->velocityY != 0) {
  settle:
    i++;
    WaitTicks(1);
    if (i <= 59 && actor->velocityY != 0)
      goto settle;
  }
  Func_02001acc(shadow);
  Object_GetById(gPartyState.currentOwner)->priorityFlags |= 1;
  Camera_FollowActor(gPartyState.currentOwner, 1);
  *(u16 *)(work + 0xd46) = 1;
  *(u16 *)(work + 0x154) = 0;
  Func_02001b2c();
}
