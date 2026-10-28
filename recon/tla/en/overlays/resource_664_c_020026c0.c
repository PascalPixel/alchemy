// Draft, differ 38 of 70: once the controlled character stands still on
// another cell, puts back the cell attributes Func_02002898 saved at
// (73, 72) and forgets the cell; in scene 0x59 it also records the kind of
// the cell it left (Func_02000690, kind 2) plus 200 in the event work's
// counter (0x16a). The same listing as overlay 692's Func_020024ac. Left: order
// and registers; the native tests the velocities before reading the last cell.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
extern char SceneId_Ov664Room1, SceneId_Ov664Room2, SceneId_Ov664Room3,
    SceneId_Ov664Room5, SceneId_Ov664Room6, SceneId_Ov664Room7,
    SceneId_Ov664Room8, SceneId_Ov664Room9;
#include "ram_buffer.h"

extern s32 gData_02006c0c;
extern s32 gData_020073dc;
extern s32 gData_020073e0;
extern s32 gData_020073e4;
extern s32 gData_020073e8;

s32 Func_02005f84(void);
struct Actor *Actor_GetById(u32 id);
void Func_02005db4(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
s32 Func_02000690(s32 kind, s32 x, s32 z);

void Func_020026c0(void) {
  struct Actor *actor = Actor_GetById(Func_02005f84());
  s32 x = actor->x >> 20;
  s32 z = actor->z >> 20;
  s32 cell = (x << 16) + z;
  s32 last;
  u8 *work;

  if (actor->velocityX != 0 || actor->velocityZ != 0)
    return;
  last = gData_02006c0c;
  if (last == -1 || cell == last)
    return;
  gData_02006c0c = -1;
  x = last >> 16;
  z = last & 0xffff;
  Func_02005db4(73, 72, gData_020073dc, gData_020073e0, gData_020073e4,
                gData_020073e8);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room5) {
    work = Ram_HeapSlots->eventWork;
    cell = Func_02000690(2, x, z) >> 8;
    if (cell != 0)
      *(s16 *)(work + 0x16a) = cell + 200;
  }
}
