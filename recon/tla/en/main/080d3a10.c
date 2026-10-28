// Under -mthumb-slow-loads (2026-10-26): differ 8 of 132 in tla-en, instruction
// order only. Empty asms before the angle's rounding, the first
// SetMapCellCollisionFar and the priority flags lower it to 2: the native then
// stores x between movs r3, #128 and its shift, where this stores it first.
// Near miss, differ 14 of 132: turns an object to one of the four facings
// (angle rounded to the nearest quarter turn), optionally setting its mode,
// marks its cell and the cell ahead of it blocked, and nudges its position and
// sprite offsets for that facing. No ☀️ twin found. Left: instruction order
// only: the native stores x to the vector between the two halves of 0x2000 and
// starts 34 before the stack store of z, and four constants and an address copy
// sit one slot apart. The cases are in the native's block order (0, 0x8000,
// 0xc000, 0x4000) and the sprite offsets are signed bytes (subs #4).
#include "types.h"
#include "field_sprite.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
void SetMapCellCollisionFar(s32 layer, s32 x, s32 z, s32 value);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);

void Func_080d3a10(s32 id, u32 angle, s32 mode) {
  struct Actor *object = Actor_GetById(id);
  struct FieldSprite *sprite = object->animation;
  s32 position[3];

  if (object == NULL || sprite == NULL)
    return;
  if (mode != -1)
    Actor_SetMode(object, mode);
  position[0] = object->x;
  angle = (angle + 0x2000) & 0xc000;
  position[2] = object->z;
  SetMapCellCollisionFar(object->terrainId, position[0], position[2], 255);
  Vector_AddPolarOffset(0x100000, angle, position);
  SetMapCellCollisionFar(object->terrainId, position[0], position[2], 255);
  switch (angle) {
  case 0:
    object->x += 0x80000;
    object->z += 0x10000;
    ((s8 *)sprite->unk14)[2] -= 4;
    ((s8 *)sprite->unk14)[3] += 15;
    break;
  case 0x8000:
    object->x -= 0x80000;
    object->z += 0x10000;
    ((s8 *)sprite->unk14)[2] += 3;
    ((s8 *)sprite->unk14)[3] += 16;
    break;
  case 0xc000:
    ((s8 *)sprite->unk14)[3] += 1;
    break;
  case 0x4000:
    object->z += 0x80000;
    ((s8 *)sprite->unk14)[3] += 13;
    break;
  }
  object->collisionFlags = 0;
  object->priorityFlags |= 2;
  sprite->nextFrame = 0;
  sprite->rotation = angle + 0x4000;
}
