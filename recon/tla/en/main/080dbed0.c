// Matches (tla-en, 2026-11-07).
// Written as its matched mirror Func_080dbf94 (games/tla/src/field/map/
// f0dbf94.c): the scale is i * (Q16_ONE * 3 / 4) / 10 + Q16_ONE / 4, which
// loop.c reduces to the native's running sum, in place of a scale variable.
// Moves the object from 16 pixels above the effect's main object to (x, y,
// z), growing it from 0.25 to 1.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "fixed_math.h"

struct MoveScene {
  u16 mode;
  u8 unk02[14];
  struct Actor *mainObject;
};

void Actor_SetSpritePriorityDirectFar(struct Actor *object, s32 priority);
void wait(s32 frames);

void Func_080dbed0(struct Actor *object, s32 x, s32 y, s32 z) {
  struct MoveScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *mainObject = scene->mainObject;
  s32 start[3];
  s32 i = 0;

  if (scene->mode != 0xc000)
    Actor_SetSpritePriorityDirectFar(object, 0);
  start[0] = mainObject->x;
  start[1] = mainObject->y + 0x100000;
  start[2] = mainObject->z;
  do {
    object->x = start[0] + (x - start[0]) * i / 10;
    object->y = start[1] + (y - start[1]) * i / 10;
    object->z = start[2] + (z - start[2]) * i / 10;
    object->scaleY = object->scaleX = i * (Q16_ONE * 3 / 4) / 10 + Q16_ONE / 4;
    wait(1);
    i++;
  } while (i <= 10);
}
