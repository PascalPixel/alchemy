#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern const u8 gData_02003dba[];
void Func_02002e30(const u8 *data);
void Func_02003b3c(s32 id);
void Func_020036f8(s32 a, s32 b, s32 owner);

void Func_02003784(s32 a, s32 b) {
  Func_020036f8(a, b, gMainRoutineWork.currentOwner);
}
