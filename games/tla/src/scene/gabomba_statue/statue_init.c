#include "types.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

extern const u8 gData_02005944[];
extern const u8 gData_02005d7c[];
u8 *Unit_Get(s32 owner);
void Func_0200533c(void);
void Func_02005364(void);
void Func_02005724(s32 id, s32 target);
void Func_02001b38(void);
void Func_02001b68(void);
void Func_02002394(void);
void Func_020053bc(const u8 *data);
void Func_02005824(void);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);
void Func_0200582c(s32 index, s32 a, s32 b);

void Func_020034ec(void) { Func_02001b38(); }

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Exit_Set(s32 index, s32 flag, s32 a, s32 b) {
  Func_0200583c(index, flag, a, b);
}

void Func_020034f4(void) {
  if ((u16)(gMainRoutineWork.entrance - 3) <= 1) {
    Actor_SetMovementTypeById(27, gData_02005944);
    Actor_SetMovementTypeById(28, gData_02005944);
    MapGrid_CopyCellAttributes(33, 24, 3, 3, 27, 29);
  }
  Func_020053bc(gData_02005d7c);
  if (!GameFlag_Test(0x8ce) || !GameFlag_Test(0x8cf))
    Actor_GetById(26)->y = PIXELS(16);
  Func_02005824();
  Exit_Set(0, 513, 8, 9);
  Exit_Set(1, 514, 10, 11);
  Func_02001b68();
}
