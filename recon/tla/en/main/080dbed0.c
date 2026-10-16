// Near miss: listing 91 instructions, draft 91, common 71, differ 40; the rest
// differs in instruction order.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct MoveScene {
  u16 mode;
  u8 unk02[14];
  struct ObjectRuntime *mainObject;
};

struct MapCell;
u8 Func_080dbde8(s32 x, s32 z, s32 layer);
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);
void ObjectDispatch_SetSpritePriorityFar(struct ObjectRuntime *object,
                                         s32 priority);
void ObjectDispatch_InitializeFar(struct ObjectRuntime *object,
                                  const s32 *script);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct ObjectRuntime *object,
                                                s32 magnitude, s32 angle);
void WaitFrames(s32 frames);
u32 Random16(void);
extern const s32 gData_080f0e78[];

void Func_080dbed0(struct ObjectRuntime *object, s32 x, s32 y, s32 z) {
  struct MoveScene *scene = Ram_HeapSlots->effectWork;
  struct ObjectRuntime *mainObject = scene->mainObject;
  s32 start[3];
  s32 i = 0;
  s32 scale;

  if (scene->mode != 0xc000)
    ObjectDispatch_SetSpritePriorityFar(object, 0);
  start[0] = mainObject->x;
  start[1] = mainObject->y + 0x100000;
  start[2] = mainObject->z;
  scale = 0;
  do {
    object->x = start[0] + i * (x - start[0]) / 10;
    object->y = start[1] + i * (y - start[1]) / 10;
    object->z = start[2] + i * (z - start[2]) / 10;
    object->scaleX = object->scaleY = scale / 10 + 0x4000;
    WaitFrames(1);
    i++;
    scale += 0xc000;
  } while (i <= 10);
}
