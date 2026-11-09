#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern const u8 gData_02003dba[];
void Func_02002e30(const u8 *data);
void Func_02003b3c(s32 id);
void Func_020036f8(s32 a, s32 b, s32 owner);

void Func_02001ba4(void) { Event_ResolvePendingActions(0); }

void Func_02001bb0(void) { Func_02002e30(gData_02003dba); }

void Func_02001bc0(void) {
  Field_SetCellBehaviorAt(0, 0x1080000, 0x2280000, 30);
  Event_SetFlag(0x212);
}

void Func_02001be0(void) {
  Field_SetCellBehaviorAt(0, 0x1080000, 0x2280000, 0);
  Event_ClearFlag(0x212);
}
