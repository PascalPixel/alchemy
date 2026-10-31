#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "fixed_math.h"
#include "scene_tables.h"

struct MoveScene {
  u16 mode;
  u8 unk02[14];
  struct Actor *mainObject;
};

struct MapCell;
u8 Func_080dbde8(s32 x, s32 z, s32 layer);
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);
void Actor_SetSpritePriorityDirectFar(struct Actor *object, s32 priority);
void Actor_StartScriptFar(struct Actor *object, const s32 *script);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);
void wait(s32 frames);
u32 Random16(void);
extern const s32 gData_080f0e78[];

void Func_080dbf94(struct Actor *object, s32 x, s32 y, s32 z) {
  struct Actor *mainObject =
      ((struct MoveScene *)Ram_HeapSlots->effectWork)->mainObject;
  s32 start[3];
  s32 i;

  i = 0;
  start[0] = mainObject->x;
  start[1] = mainObject->y + PIXELS(16);
  start[2] = mainObject->z;
  do {
    object->x = x + (start[0] - x) * i / 10;
    object->y = y + (start[1] - y) * i / 10;
    object->z = z + (start[2] - z) * i / 10;
    object->scaleY = object->scaleX = i * -(Q16_ONE * 3 / 4) / 10 + Q16_ONE;
    wait(1);
    i++;
  } while (i <= 10);
}
