// Near miss, differ 20 (28 before statements reordered 2026-10-26) of 91 (2026-10-18; 40 before), Func_080dbf94's mirror:
// moves the object from 16 pixels above the effect's main object to (x, y,
// z), growing it from 0.25. The products written (x - start) * i and scaleX
// stored first took it from 40. Left: the frame opens after mov r9, r3 in
// the native; at the loop the native builds the zero for the scale in r2
// and loads the spilled x into r1 before the divisor; the y and z loads
// come one slot later. A one-pass block after the y store, or the scale
// zeroed before z, measured the same; five minutes of permuter found nothing.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

struct MoveScene {
  u16 mode;
  u8 unk02[14];
  struct Actor *mainObject;
};

struct MapCell;
u8 Func_080dbde8(s32 x, s32 z, s32 layer);
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);
void Actor_SetSpritePriorityDirectFar(struct Actor *object,
                                         s32 priority);
void Actor_StartScriptFar(struct Actor *object,
                                  const s32 *script);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);
void WaitFrames(s32 frames);
u32 Random16(void);
extern const s32 gData_080f0e78[];

void Func_080dbed0(struct Actor *object, s32 x, s32 y, s32 z) {
  struct MoveScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *mainObject = scene->mainObject;
  s32 start[3];
  s32 i = 0;
  s32 scale;

  if (scene->mode != 0xc000)
    Actor_SetSpritePriorityDirectFar(object, 0);
  scale = 0;
  start[0] = mainObject->x;
  start[1] = mainObject->y + 0x100000;
  start[2] = mainObject->z;
  do {
    object->x = start[0] + (x - start[0]) * i / 10;
    object->y = start[1] + (y - start[1]) * i / 10;
    object->z = start[2] + (z - start[2]) * i / 10;
    object->scaleY = object->scaleX = scale / 10 + 0x4000;
    WaitFrames(1);
    i++;
    scale += 0xc000;
  } while (i <= 10);
}
