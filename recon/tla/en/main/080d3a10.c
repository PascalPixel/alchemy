// Near miss, differ 8 of 132 (tla-en, 2026-10-31): instruction order only,
// three pairs sched2 ranks one priority apart. The native issues mov r5, sp
// before movs r3, #128 (ranked 15 against 16 here, the constant's chain
// running through the ands' anti-dependence on r2, the z load), the store of
// z (str r2, [r5, #8]) before movs r3, #34 (9 against 10), and the strb of
// the zero into collisionFlags before adds r1, r6, #0 (9 against 10). Each
// store ranked low is followed by a load whose wait after a store
// -mthumb-slow-loads does not count: the ldrb of terrainId is an extended
// load, and the priorityFlags |= 2 load opens a read-modify-write. Scheduler
// order the C does not move: a struct Vector3 for the position, the rounding
// before x is read, the angle in a separate local, terrainId read through a
// pointer and either flag store moved all measured 8 or worse. Empty asms
// before the angle's rounding, after the read of z and before the priority
// flags lower it to 2 (only the x store left), so the plain form stays.
// Func_080d3940 (080d3940.c) keeps the same pattern.
// Turns an object to one of the four facings (angle rounded to the nearest
// quarter turn), optionally setting its mode, marks its cell and the cell
// ahead of it blocked, and nudges its position and sprite offsets for that
// facing. No ☀️ twin found. The cases are in the native's block order (0,
// 0x8000, 0xc000, 0x4000) and the sprite offsets are signed bytes (subs #4).
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
