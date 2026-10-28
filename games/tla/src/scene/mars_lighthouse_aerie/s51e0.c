#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

struct MarsLighthouseAerieEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

struct Actor *Actor_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005010(void);
void Func_02005030(void);
void Func_0200575c(s32 a, s32 b);
void Func_0200576c(s32 a);
void Func_020057c4(void);

static __inline__ void Func_0200575c_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200575c(a, b);
}

void Func_020051e0(void) {
  struct MarsLighthouseAerieEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  /* FAKEMATCH: a one-pass block loads the event work before its offset is built
   * (one reordered row). */
  do {
  } while (0);
  saved = work->value1ac;
  Func_0200575c_(0x202108, 1);
  Func_0200575c_(0x10000, 2);
  ((struct MarsLighthouseAerieEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005030();
  Func_020057c4();
  wait(40);
  Func_02005010();
  Func_0200575c_(0x10000, 1);
  Func_0200576c(16);
  wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    /* FAKEMATCH: a one-pass block loads gMainRoutineWork before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}
