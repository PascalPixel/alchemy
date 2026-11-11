#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

extern const u8 gData_020017d8[];
extern const u8 gData_020017a4[];
extern const u8 gData_0200171c[];
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_020013f0(s32 a, s32 b, s32 c);
void Func_02001440(s32 a, s32 b);
void Func_020012e0(void);
void Func_02000f38(void);

void Func_02000ffc(void) {
  s32 back;
  s32 id;
  s32 x;

  Func_020012e0();
  Event_ResolvePendingActions(0);
  back = 14;
  id = 4;
  x = 0;
  /* FAKEMATCH(barrier): the empty asm on back, id and x, so the other
   * arguments are set before the step is negated; as constant arguments. */
  asm("" : "+l"(back), "+l"(id), "+l"(x));
  Actor_CommitPositionAndActivate(id, x, -back);
  Actor_CommitPositionAndActivate(4, 16, 0);
  Event_Wait(10);
  Func_02000f38();
}

void Func_0200102c(void) {
  s32 back;
  s32 id;
  s32 x;

  Func_020012e0();
  Event_ResolvePendingActions(0);
  back = 14;
  id = 4;
  x = 0;
  /* FAKEMATCH(barrier): the empty asm on back, id and x, so the other
   * arguments are set before the step is negated; as constant arguments. */
  asm("" : "+l"(back), "+l"(id), "+l"(x));
  Actor_CommitPositionAndActivate(id, x, -back);
  Actor_CommitPositionAndActivate(4, -16, 0);
  Event_Wait(10);
  Func_02000f38();
}

void Func_0200105c(void) {
  Func_020012e0();
  Event_ResolvePendingActions(0);
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  Func_02000f38();
}

void Func_02001080(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  Actor_SetSingleChildFlags(Actor_GetById(20), 0);
}

void Func_020010a0(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 521;
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
}
