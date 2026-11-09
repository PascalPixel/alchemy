#include "types.h"
#include "field_event.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "input.h"
#include "flag_call.h"

#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))

u32 Random16(void);

void Func_08029810(struct FieldActor *object) {
  u32 first;
  u32 second;
  s32 x;

  HALF_AT(object->sprite, 18) += 0x400;
  first = Random16();
  second = Random16();
  x = object->x;

  second >>= 1;
  first >>= 1;
  object->x = x + (first - second);
}

s32 Func_080c8828(s32 arg);

/// When one of three party key sets is pressed: plays cue 113 if the far call
/// accepts the argument, else posts event request 0x208b and returns 1 while
/// flag 0x146 is clear.
s32 Func_08029838(s32 arg) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  /* FAKEMATCH: the flag lives in r0, where the native also returns the early
   * zero; as a free local it takes r6 and the argument r0 (differ 14). */
  register s32 pressed asm("r0") = 0;

  if (work == NULL)
    return 0;
  if ((gInput.pressed & gMainRoutineWork.keys234) ||
      (gInput.pressed & gMainRoutineWork.keys230) ||
      (gInput.pressed & gMainRoutineWork.keys236))
    pressed = 1;
  if (pressed) {
    if (Func_080c8828(arg)) {
      Sound_PlayFar(113);
    } else if (!GameFlag_Test(0x146)) {
      work->unk166 = 0x208b;
      return 1;
    }
  }
  return 0;
}
