#include "types.h"
#include "field_event.h"
#include "ram_buffer.h"
#include "party_state.h"
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
  /* FAKEMATCH: a one-pass block keeps the position load ahead of the two
   * halvings; as one statement the first halving comes before the load (2
   * differ). */
  do {
  } while (0);
  second >>= 1;
  first >>= 1;
  object->x = x + (first - second);
}

#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
/// Spanish and Italian builds use a different name for this far call, as in
/// objlayer.c.
s32 BattleFx_WaitForArcGroupsFar(s32 arg);
#else
s32 Func_080c8828(s32 arg);
#endif

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
  {
    struct PartyState *party = &gPartyState;
    /* FAKEMATCH: the pressed keys are read again for each of the three tests;
     * through a plain pointer one read serves all three (differ 38). */
    volatile struct InputState *input = (volatile struct InputState *)&gInput;

    /* FAKEMATCH: the native loads both addresses before it builds the first
     * key offset; without the barrier the input's load comes after (differ
     * 2). */
    asm volatile("" : "+l"(party), "+l"(input));
    if ((input->pressed & party->keys234) ||
        (input->pressed & party->keys230) || (input->pressed & party->keys236))
      pressed = 1;
  }
  if (pressed) {
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
    if (BattleFx_WaitForArcGroupsFar(arg)) {
#else
    if (Func_080c8828(arg)) {
#endif
      Audio_PlayCue(113);
    } else if (!GameFlag_Test(0x146)) {
      work->unk166 = 0x208b;
      return 1;
    }
  }
  return 0;
}
