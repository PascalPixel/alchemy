#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct AnemosInnerSanctumEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000158(void);
void Func_02000178(void);
void Func_020017b4(void);
void Func_020034a8(void);

static __inline__ void Func_02003458_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  BattleEffect_ApplyColorToTargetBuffer(a, b);
}

void Func_02000328(void) {
  struct AnemosInnerSanctumEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;


  saved = work->value1ac;
  BattleEffect_ApplyColorToTargetBuffer(0x202108, 1);
  Func_02003458_(0x10000, 2);
  ((struct AnemosInnerSanctumEventWork *)Ram_HeapSlots->eventWork)->value1ac =
      0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02000178();
  Func_020034a8();
  wait(40);
  Func_02000158();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  BattleEffect_StartBufferInterpolation(16);
  wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;


    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}
