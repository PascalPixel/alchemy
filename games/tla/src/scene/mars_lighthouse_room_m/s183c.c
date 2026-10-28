#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sequence_parent_work.h"

struct MarsLighthouseRoomMEventWork1ac {
  u8 unk000[0x1ac];
  s32 value1ac;
};

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200166c(void);
void Func_0200168c(void);
void Func_020017b4(void);
void Func_02003248(s32 a, s32 b);
void Func_02003258(s32 a);
void Func_020032a0(void);

static __inline__ void Func_02003248_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003248(a, b);
}

void Func_0200183c(void) {
  struct MarsLighthouseRoomMEventWork1ac *work = Ram_HeapSlots->eventWork;
  s32 saved;

  /* FAKEMATCH: a one-pass block loads the event work before its offset is built
   * (one reordered row). */
  do {
  } while (0);
  saved = work->value1ac;
  Func_02003248_(0x202108, 1);
  Func_02003248_(0x10000, 2);
  ((struct MarsLighthouseRoomMEventWork1ac *)Ram_HeapSlots->eventWork)->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_0200168c();
  Func_020032a0();
  wait(40);
  Func_0200166c();
  Func_02003248_(0x10000, 1);
  Func_02003258(16);
  wait(16);
  {
    struct SequenceParentWork *party = &gSequenceParentWork;

    /* FAKEMATCH: a one-pass block loads gSequenceParentWork before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}
