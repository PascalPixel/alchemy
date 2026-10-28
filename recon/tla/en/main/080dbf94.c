// Under -mthumb-slow-loads (2026-10-26): differ 8 of 81 in tla-en, instruction
// order only. Empty asms before the z start and the z step lower it to 4.
// Near miss, differ 8 of 81 (2026-10-18; 38 before): order only. Moves the
// object over 11 frames from (x, y, z) to 16 pixels above the effect's main
// object, shrinking it from 1.0 by 0.075 a frame. The products are
// written (start - x) * i, scaleX is stored first, z is read after the
// counters are set and the one-pass block after the y store holds z's load
// back (differ 10 without). Left: the native copies the object to r5 after
// the x store and loads y after it; and it copies the stack pointer to r9
// before the z store and the zero to fp after it.
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

void Func_080dbf94(struct Actor *object, s32 x, s32 y, s32 z) {
  struct Actor *mainObject =
      ((struct MoveScene *)Ram_HeapSlots->effectWork)->mainObject;
  s32 start[3];
  s32 i;
  s32 scale;

  start[0] = mainObject->x;
  start[1] = mainObject->y + 0x100000;
  /* FAKEMATCH: the one-pass block loads z after the y store. */
  do {
  } while (0);
  i = 0;
  scale = 0;
  start[2] = mainObject->z;
  do {
    object->x = x + (start[0] - x) * i / 10;
    object->y = y + (start[1] - y) * i / 10;
    object->z = z + (start[2] - z) * i / 10;
    object->scaleY = object->scaleX = scale / 10 + 0x10000;
    WaitFrames(1);
    i++;
    scale -= 0xc000;
  } while (i <= 10);
}
