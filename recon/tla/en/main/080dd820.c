// Near miss, differ 4 of 137 (2026-10-18; 8 on 2026-10-15), The Broken Seal's
// FieldFx_RunReveal: Reveal. ⚓️
// centres the transition on the caster from the effect scene, clears the
// active flag as ☀️'s non-English editions do, finds the Reveal event
// through Func_080ce31c/Func_080ce458 and schedules the watch only while the
// scene's mode byte is clear.
// Left: order only, two pairs: the native loads the caster's x between the
// halves of a split offset (after movs r0, #160), one slot earlier than
// this, and its z after movs r1, #166, one slot later. The one-pass block
// round the cue stores active = 1 before the centre is reloaded, as the
// native does (found by five minutes of permuter).
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

void BattleEffect_RunBusyWaitDelay(s32 value);
void DisplayTransition_InitializeBattleEffectState(s32 mode);
void BattleEffect_ApplyColorToSourceBuffer(u32 value, s32 enabled);
void BattleEffect_ApplyColorToTargetBuffer(u32 value, s32 enabled);
void BattleEffect_StartBufferInterpolation(s32 frames);
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

  BattleEffect_RunBusyWaitDelay(6);
  event = Ram_HeapSlots->eventWork;
  DisplayTransition_InitializeBattleEffectState(FIELD_FX_REVEAL);
  transition = Ram_HeapSlots->displayTransition;
  caster = scene->mainObject;
  transition->centreX = caster->x;
  transition->centreY = caster->z - caster->y;
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10001, 1);
  BattleEffect_StartBufferInterpolation(1);
  event->active = 0;
  WaitFrames(1);

  pending = Func_080ce458(REVEAL_EVENT_START, FIELD_FX_REVEAL,
                          Func_080ce31c(FIELD_FX_REVEAL));
  if (pending != 0)
    EventRuntime_RunPendingEvent(pending, scene->firstObjectId,
                                 scene->secondObjectId);
  // Leave the centre (in pixels) and the time in the event work, and put
  // up the markers nearby.
  /* FAKEMATCH: the one-pass block stores active before the centre is
   * reloaded (one reordered pair without it). */
  do {
    Audio_PlayCue(SE_REVEAL);
    event->active = 1;
  } while (0);
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
