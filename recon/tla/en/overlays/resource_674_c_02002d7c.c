// Draft, differ 83 of 182: the Islets' marching guards (objects 11 to 18):
// while the event work is busy or flag 0x218 is set they stand still;
// once the controlled character drops below height -12 they stand and the
// event work's counter 0x16a becomes 200; otherwise, for each frame owed
// (gSceneState's halfword 2) while no event runs, a 260-frame routine sets
// them walking with their facing steps (Func_02002d48) at 0, still at 100
// and 220, and walking in place at 130. Left: the native's loops are goto loops (the first stand loop is entered from both exits), keeps the owed counter's address in r8, and stores the wrapped step from a pool zero.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

extern u8 gSceneState[];
s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(u32 id);
void Actor_SetModeById(u32 id, s32 mode);
void Func_02002d48(s32 id, s32 dx, s32 dz);

void Func_02002d7c(void) {
  u16 *step = (u16 *)gSceneState;
  u8 *work = Ram_HeapSlots->eventWork;
  u16 *owed = (u16 *)(gSceneState + 2);
  s32 i;
  u16 count;

  if (*(s16 *)(work + 0x166) != 0 || GameFlag_Test(0x218)) {
    for (i = 11; i <= 18; i++)
      Actor_GetById(i)->movementState = 1;
    return;
  }
  for (i = 11; i <= 18; i++)
    Actor_GetById(i)->movementState = 0;
  if (Actor_GetById(gMainRoutineWork.currentOwner)->y < -0xc0000) {
    work = Ram_HeapSlots->eventWork;
    for (i = 11; i <= 18; i++)
      Actor_GetById(i)->movementState = 1;
    *(s16 *)(work + 0x16a) = 200;
    return;
  }
  count = ++*owed;
  if (*(s16 *)(work + 0x156) != 0 || count == 0)
    return;
  do {
    switch (*step) {
    case 0:
      for (i = 11; i <= 18; i++)
        Actor_SetModeById(i, 2);
      Func_02002d48(11, 0, 1);
      Func_02002d48(12, 0, -1);
      Func_02002d48(13, 0, 1);
      Func_02002d48(14, 0, 1);
      Func_02002d48(15, -1, 0);
      Func_02002d48(16, 0, 1);
      Func_02002d48(17, 1, 0);
      Func_02002d48(18, 0, -1);
      break;
    case 100:
    case 220:
      for (i = 11; i <= 18; i++)
        Actor_SetModeById(i, 1);
      break;
    case 130:
      for (i = 11; i <= 18; i++) {
        Actor_SetModeById(i, 2);
        Func_02002d48(i, 0, 0);
      }
      break;
    }
    if (++*step == 260)
      *step = 0;
  } while (--*owed != 0);
}
