#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "task.h"
#include "field_psynergy_work.h"
#include "display_transition.h"

// The field event work's Reveal state (eventWork + 0xca4).
struct RevealEvent {
  u8 unk000[0xca4];
  s8 active;  // Reveal is showing
  u8 markers; // the nearby markers are up
  s16 frames; // how long it lasts
};

void Sound_PlayFar(s32 no);
void BattleEffect_RunBusyWaitDelay(s32 value);
void BattleEffect_ApplyColorToTargetBuffer(u32 value, s32 enabled);
void BattleEffect_ApplyColorToSourceBuffer(u32 value, s32 enabled);
void BattleEffect_StartBufferInterpolation(s32 frames);
s32 Func_080ce31c(s32 kind);
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
void Field_RunPsynergyEvent(void *event, s32 a, s32 b);
void Func_080dd754(void);

#define FIELD_FX_REVEAL 8

void Func_080dd950(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct ScreenTransitionState *transition = Ram_HeapSlots->screenTransition;
  struct RevealEvent *event = Ram_HeapSlots->eventWork;
  void *pending;

  if (event->active != 0) {
    Sound_PlayFar(167);
    Task_Remove(Func_080dd754);
    event->active = 0;
    event->frames = 0;
    BattleEffect_RunBusyWaitDelay(0);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
    BattleEffect_StartBufferInterpolation(1);
    BattleEffect_ApplyColorToSourceBuffer(0, 0);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
    BattleEffect_StartBufferInterpolation(30);
    wait(1);
    pending = Func_080ce458(0x40000005, FIELD_FX_REVEAL,
                            Func_080ce31c(FIELD_FX_REVEAL));
    if (pending != NULL)
      Field_RunPsynergyEvent(pending, scene->firstObjectId,
                             scene->secondObjectId);
    if (scene->childOption == 0) {
      transition->nextMode = 0;
      transition->duration = 1;
      transition->step = 1;
      wait(10);
    }
  }
}
