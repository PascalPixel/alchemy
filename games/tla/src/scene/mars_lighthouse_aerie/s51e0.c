#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct MarsLighthouseAerieEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005010(void);
void Func_02005030(void);
void Func_020057c4(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_0200575c_(s32 a, s32 b) {
  BattleEffect_ApplyColorToTargetBuffer(a, b);
}

void Func_020051e0(void) {
  struct MarsLighthouseAerieEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  
  saved = work->value1ac;
  PaletteFade_ApplyTarget(0x202108, 1);
  Func_0200575c_(0x10000, 2);
  ((struct MarsLighthouseAerieEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005030();
  Func_020057c4();
  System_Wait(40);
  Func_02005010();
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    
    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}
