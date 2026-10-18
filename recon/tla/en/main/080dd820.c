// Near miss, differ 8 of 137 (2026-10-15), The Broken Seal's
// RunBattleEffect08: Reveal. ⚓️
// centres the transition on the caster from the effect scene, clears the
// active flag as ☀️'s non-English editions do, finds the Reveal event
// through Func_080ce31c/Func_080ce458 and schedules the watch only while the
// scene's mode byte is clear.
// Left: order only, four pairs. Three times the native loads a field
// between the halves of a split offset (the caster's x after movs r0, #160,
// its z before movs r1, #166), one slot earlier than this; and it stores
// active = 1 before it reloads the centre, where this loads first.
#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "fx_scene.h"
#include "object_runtime.h"
#include "display_transition.h"

// The field event work's Reveal state (eventWork + 0xca4).
struct RevealEvent {
  u8 unk000[0xca4];
  s8 active;   // Reveal is showing
  u8 markers;  // the nearby markers are up
  s16 frames;  // how long it lasts
  s16 centreX; // pixels
  s16 centreY;
};

void BattleFx_RunBusyWaitDelay(s32 value);
void DisplayTransition_InitializeBattleEffectState(s32 mode);
void BattleFx_ApplyColorToSourceBuffer(u32 value, s32 enabled);
void BattleFx_ApplyColorToTargetBuffer(u32 value, s32 enabled);
void BattleFx_StartBufferInterpolation(s32 frames);
s32 Func_080ce31c(s32 kind);
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
void EventRuntime_RunPendingEvent(void *event, s32 a, s32 b);
void Func_080cf554(void);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Func_080dd754(void);

#define FIELD_FX_REVEAL 8
#define REVEAL_EVENT_START 0x50000005 // the Reveal event, stage 5
#define REVEAL_FRAMES 600
#define SE_REVEAL 131

// Reveal: open a circle of the transition on the caster, wash the screen
// plain, run the Reveal event that matches, then show the markers nearby
// and watch how far the leader walks.
void Func_080dd820(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct RevealEvent *event;
  struct DisplayTransitionState *transition;
  struct ObjectRuntime *caster;
  void *pending;
  s32 frame;

  BattleFx_RunBusyWaitDelay(6);
  event = Ram_HeapSlots->eventWork;
  DisplayTransition_InitializeBattleEffectState(FIELD_FX_REVEAL);
  transition = Ram_HeapSlots->displayTransition;
  caster = scene->mainObject;
  transition->centreX = caster->x;
  transition->centreY = caster->z - caster->y;
  BattleFx_ApplyColorToSourceBuffer(0x10000, 0);
  BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
  BattleFx_StartBufferInterpolation(1);
  event->active = 0;
  WaitFrames(1);

  pending = Func_080ce458(REVEAL_EVENT_START, FIELD_FX_REVEAL,
                          Func_080ce31c(FIELD_FX_REVEAL));
  if (pending != 0)
    EventRuntime_RunPendingEvent(pending, scene->firstObjectId,
                                 scene->secondObjectId);
  Audio_PlayCue(SE_REVEAL);

  // Leave the centre (in pixels) and the time in the event work, and put
  // up the markers nearby.
  event->active = 1;
  event->centreX = transition->centreX / 0x10000;
  event->centreY = transition->centreY / 0x10000;
  event->frames = REVEAL_FRAMES;
  event->markers = 1;
  Func_080cf554();
  frame = 0;
  do {
    WaitFrames(1);
    transition->value = frame;
    frame++;
  } while (frame <= 18);
  if ((s8)scene->mode == 0)
    Scheduler_AddOrUpdateCallback(Func_080dd754, 0x480);
}
