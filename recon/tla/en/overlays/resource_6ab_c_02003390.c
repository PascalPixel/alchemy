// Draft, differ 276 of 246, skeleton gap 46 (tla-en, 2026-11-01)
// The same room's code as games/tla/src/scene/mars_lighthouse_star_room
// (Func_02002d04 and Func_02002d90 are matched there): takes the current
// owner's object, snaps it to the middle of its 16-pixel cell and walks it one
// cell at a time along its angle (snapped to a quarter turn), stopping at the
// value 255 from Func_02004224, a rise over 8 pixels, or a solid object.
// Control flow, calls, loads and widths are the listing's: the do with an
// entry jump gives its loop layout (a plain while lets jump.c rotate the height
// test to the bottom, gap 56). Left: the listing builds &position and
// &object->terrainId inside the retry cycle, at first use after the label; PRE
// hoists both over the entry edge to the retry label, 6 extra instructions
// before it, and that shifts the literal pool's branch and takes r8 and a stack
// slot from the allocation (the native keeps object r5, terrainId r11, i r9,
// flags r8, other r6, and re-adds sp+20 in the loop). The movs/adds form of the
// native's +34 and +89 comes from reload when the set's destination is a high
// register, so it follows allocation and is no source difference.
// Tried, code unchanged: pointer locals for position, a named 0x80000, a local
// for &terrainId in one or two steps, inner scopes, flags set in the for, do
// loop for i, x and z as chained assignments, nested ifs. Worse: u32 i (gap 47),
// u8 values (54), lastX and lastZ declared first (differ 282).
// Nothing plain in C stops the hoist; this is the compiler question recorded
// in other drafts (their notes on PRE), so stop here.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

void wait(s32 frames);
void Func_020041cc(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_02002d90(void);
void Func_02002d04();
s32 Func_02004224(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
s32 Func_0200428c(s32 *position, s32 radius, s32 *other, s32 otherRadius);
void Func_02004204(struct Actor *object, s32 x, s32 y, s32 z);
void Func_0200420c(struct Actor *object);

void Func_02003390(void) {
  struct Actor *object;
  s32 position[3];
  s32 angle;
  s32 x;
  s32 z;
  s32 lastX;
  s32 lastZ;
  s32 value;
  s32 startValue;
  s32 radius;
  s32 i;
  struct Actor *other;
  u8 *flags;

  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  angle = (s16)((object->angle + 0x2000) & 0xc000);
retry:
  Func_02002d90();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  x = (object->x & 0xfff00000) + 0x80000;
  position[0] = x;
  position[1] = object->y;
  z = (object->z & 0xfff00000) + 0x80000;
  position[2] = z;
  lastX = x;
  lastZ = z;
  startValue = Func_02004224(object->terrainId, x, z);
  Vector_AddPolarOffsetFar(0x100000, angle, position);
  value = Func_02004224(object->terrainId, position[0], position[2]);
  if (value != 255 &&
      Map_GetTerrainHeight(object->terrainId, position[0], position[2]) -
              object->y <=
          0x80000) {
    position[0] = lastX;
    position[2] = lastZ;
    object->speedLimit = 0x20000;
    object->acceleration = 0x1999;
    object->action = 0;
    Func_020041cc(object, 2);
    Actor_SetAnimationSpeed(object, 48);
    object->update = (void (*)(void))Func_02002d04;
  } else {
    object->angle = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  goto next;
  do {
    if (Map_GetTerrainHeight(object->terrainId, position[0], position[2]) - object->y > 0x80000)
      goto blocked;
    radius = object->radius - 2;
    other = Ram_HeapSlots->scriptObjects;
    flags = &other->collisionFlags;
    for (i = 0; i <= 63; i++) {
      if (other->script != NULL && (*flags & 1) && other != object &&
          Func_0200428c(&other->x, radius, position, other->radius - 2) >= 0)
        goto blocked;
      flags += sizeof(struct Actor);
      other++;
    }
    lastX = position[0];
    lastZ = position[2];
    object->speedLimit = 0x20000;
    object->acceleration = 0x1999;
    Func_02004204(object, position[0], position[1], position[2]);
    Func_0200420c(object);
    if (value != startValue) {
      wait(10);
      goto end;
    }
  next:
    Vector_AddPolarOffsetFar(0x100000, angle, position);
    value = Func_02004224(object->terrainId, position[0], position[2]);
  } while (value != 255);
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  Func_02004204(object, lastX, object->y, lastZ);
  Func_0200420c(object);
  wait(2);
  goto retry;
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_020041cc(object, 1);
}
