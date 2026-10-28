// Draft, differ 115 of 216 (was 121): overlay 6a2's copy of the function
// drafted in resource_698_c_020003cc.c (overlay 698's Func_020003cc), whose
// header says what it is and what was tried. The camera call forwarded through
// an inline wrapper gained 6. Left: the native keeps the event work in sl and
// the motion-flags address in r9 (here r9 and fp); it compares the rise with
// 0x3ffff from the pool (bgt) where the source's form gives 0x40000 (bge); it
// stores the two halfword 1s from a word constant, where these plain stores
// load a halfword from the pool.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "frame.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02001be8(void);
void Func_02001c70(s32 value);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void WaitFrames(s32 frames);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct ObjectRuntime *Func_02001c48(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02001b98(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02001ca0(s32 sound);
void Func_02001ba0(struct ObjectRuntime *object);
void Camera_FollowActor(s32 id, s32 flag);
void Func_02001bf0(void);

static __inline__ void Camera_MoveWithinMapBounds_(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_02000538(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *actor = Object_GetById(gAdventure.currentOwner);
  struct ObjectRuntime *linked = actor->linkedObject;
  struct ObjectRuntime *shadow;
  s32 facing;
  s32 i;

  Func_02001be8();
  Func_02001c70(0);
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
    WaitFrames(1);
    if (*((u8 *)linked + 0x63) != 0)
      goto rise;
  }
  *(u16 *)(work + 0xd44) = 1;
  *(u16 *)(work + 0xd46) = 1;
  facing = linked->angle >> 13;
  actor->x = (((linked->x >> 19) & ~1) + facing - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - (facing & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  actor->priorityFlags = 1;
  actor->motionFlags = 3;
  ObjectDispatch_SetSingleChildFlags(actor, 1);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  shadow = Func_02001c48();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  shadow->motionFlags = 0;
  Func_02001b98(shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z),
                actor->z);
  i = 0;
  if (actor->y > actor->terrainHeight + 0x100000) {
  fall:
    i++;
    WaitFrames(1);
    if (i <= 59 && actor->y > actor->terrainHeight + 0x100000)
      goto fall;
  }
  Func_02001ca0(127);
  i = 0;
  if (actor->velocityY != 0) {
  settle:
    i++;
    WaitFrames(1);
    if (i <= 59 && actor->velocityY != 0)
      goto settle;
  }
  Func_02001ba0(shadow);
  Object_GetById(gAdventure.currentOwner)->priorityFlags |= 1;
  Camera_FollowActor(gAdventure.currentOwner, 1);
  *(u16 *)(work + 0xd46) = 1;
  *(u16 *)(work + 0x154) = 0;
  Func_02001bf0();
}
