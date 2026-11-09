#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "io_reg.h"
#include "io_write.h"
#include "call.h"
#include "event_inline.h"

extern const u8 gData_02002b60[];

void Func_02000054(void);
void Func_020007d0(void);
void Func_02000b28(s32 id);
void Func_02001498(void);
void Func_020015a0(s32 id, s32 value);
void Func_02002314(s32 a);
void Func_02002324(const void *script, s32 a, s32 b, s32 c);
s32 Scene_SetArrivalFlags();
s32 Func_02000aa4();

/// The scene's set-up entry: screen effect 0x204, actors 13 and 14 at action
/// variant 3, the blend registers, the particles, and the map and actors by
/// flags 0xa02, 0xa00 and 0xa03 and the entrance.
s32 Func_02000854(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x204;
  FieldActor_SetSpritePriority(13, 3);
  FieldActor_SetSpritePriority(14, 3);
  Io_Write16(0x3f06, &REG_BLDCNT);
  Io_Write16(0x1000, &REG_BLDALPHA);
  Func_02002314(0);
  Call4((void (*)())Func_02002324, (s32)gData_02002b60, 0, 0, 0x80000000);
  Func_02000054();
  Func_020007d0();
  if (Event_GetFlag(0xa02)) {
    Actor_SetPosition(17, 0, 0);
    MapGrid_CopyCellAttributes(28, 39, 1, 1, 20, 39);
    MapGrid_CopyCellsLayered(28, 39, 1, 1, 20, 39);
    MapGrid_CopyCellsLayered(92, 39, 2, 3, 84, 39);
    MapGrid_SetCellBehavior(168 << 17, 164 << 18, 2, 236);
  } else {
    Func_02001498();
    Func_020015a0(17, 0);
  }
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 6 ||
      gMainRoutineWork.entrance == 7)
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
  if (Event_GetFlag(0xa00)) {
    Func_02000b28(8);
    Func_02000b28(9);
    Func_02000b28(10);
    Func_02000b28(11);
  }
  if (Event_GetFlag(0xa03)) {
    Actor_SetPosition(12, 148 << 17, 208 << 15);
    Func_02000b28(12);
  }
  return 0;
}

s32 Func_0200099c(void) {
  Scene_SetArrivalFlags(0);
  Func_02000aa4(0);
  return 0;
}
