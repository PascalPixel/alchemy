// Under -mthumb-slow-loads (2026-10-26): differ 4 of 94 in tla-en, instruction
// order and register choice. No empty asm before any one statement lowers it.
// Near miss, differ 6 of 94 (2026-10-18; plainly 24): The Broken Seal's
// ResetSceneTransitionEffect with this game's offsets. The opening matches
// behind three tagged devices: a barrier after the two heap slot loads, the
// flag offset pinned to r2 and the transition work loaded into r1. Left: the
// two halfword arguments index through r2 where this uses r3 (temporaries for
// either argument, the same), and the pooled zero for the phase byte is loaded
// one row early in the native (a volatile barrier, the s16 zero, or a zero
// pinned to r3 behind a barrier holding r2: 6, 6, 11). Permute (4 minutes)
// found nothing better.
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"

void Sound_PlayFar(s32 no);
void FieldEffect_WatchLeaderDistance(void);
void BattleEffect_RunBusyWaitDelay(s32 value);
void BattleEffect_ApplyColorToTargetBuffer(u32 value, s32 enabled);
void BattleEffect_ApplyColorToSourceBuffer(u32 value, s32 enabled);
void BattleEffect_StartBufferInterpolation(s32 frames);
s32 Func_080ce31c(s32 kind);
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
void EventRuntime_RunPendingEvent(void *event, s32 a, s32 b);

struct EffectWork_080dd950 {
  u8 unk00[24];
  s16 value18;
  s16 value1a;
  u8 unk1c[24];
  s8 childOption;
};

void Func_080dd950(void) {
  u8 *state = Ram_HeapSlots->eventWork;
  struct EffectWork_080dd950 *work = Ram_HeapSlots->effectWork;
  s8 *flag;
  u8 *scene;
  void *ev;
  s16 zero;

  /* FAKEMATCH: the native loads both heap slots before it builds the flag
   * offset, in r2, and loads the transition work into r1, copying it after
   * the flag read (differ 24 without the three devices). */
  asm("");
  {
    register s32 offset asm("r2") = 0xca4;
    asm("" : "+r"(offset));
    flag = (s8 *)(state + offset);
  }
  {
    register u8 *loaded asm("r1") = (u8 *)Ram_HeapSlots->displayTransition;
    asm("" : "+r"(loaded));
    scene = loaded;
  }
  if (*flag != 0) {
    Sound_PlayFar(167);
    Scheduler_RemoveCallback(FieldEffect_WatchLeaderDistance);
    zero = 0;
    *flag = zero;
    *(s16 *)(state + 0xca6) = zero;
    BattleEffect_RunBusyWaitDelay(0);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
    BattleEffect_StartBufferInterpolation(1);
    BattleEffect_ApplyColorToSourceBuffer(0, 0);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
    BattleEffect_StartBufferInterpolation(30);
    WaitFrames(1);
    ev = Func_080ce458(0x40000005, 8, Func_080ce31c(8));
    if (ev != NULL)
      EventRuntime_RunPendingEvent(ev, work->value18, work->value1a);
    if (work->childOption == 0) {
      scene[0x53e] = 0;
      scene[0x53c] = 1;
      scene[0x53d] = 1;
      WaitFrames(10);
    }
  }
}
