#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct ContigoEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

void WaitFrames(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000158(void);
void Func_02000178(void);
void Func_020041cc(s32 a, s32 b);
void Func_020041dc(s32 a);
void Func_02004284(void);

static __inline__ void Func_020041cc_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020041cc(a, b);
}

void Func_02000328(void) {
  struct ContigoEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  /* FAKEMATCH: a one-pass block loads the event work before its offset is built
   * (one reordered row). */
  do {
  } while (0);
  saved = work->value1ac;
  Func_020041cc_(0x202108, 1);
  Func_020041cc_(0x10000, 2);
  ((struct ContigoEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02000178();
  Func_02004284();
  WaitFrames(40);
  Func_02000158();
  Func_020041cc_(0x10000, 1);
  Func_020041dc(16);
  WaitFrames(16);
  {
    struct PartyState *party = &gPartyState;

    /* FAKEMATCH: a one-pass block loads the party state before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}
