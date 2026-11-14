/// @file
/// The link battle timer, and a localized message shown until the player reads
/// it.
// Part of window.c.

#include "../window_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "module/window/screens.h"
#include "module/window/window.h"

#define COUNTDOWN_START_FRAMES 900
#define FRAMES_PER_SECOND 60

/* VIEW(menus): variable gLinkPeerSignatures as struct LinkSignature
 * gLinkPeerSignatures[]; heap_slots.h has u16 gLinkPeerSignatures[][12]; tried
 * as the rest of the object declares it, it does not compile in tla-ja:
 * games/tla/src/window/window/link_timer.c:82: `gLinkPeerSignatures__menus'
 * undeclared (first use in this function). */
extern struct LinkSignature
    gLinkPeerSignatures__menus[] __asm__("gLinkPeerSignatures");

void UpdateLinkSessionCountdown(void) {
  struct LinkCountdownState *state =
      ((struct LinkCountdownState **)Ram_HeapSlots)[57];
  struct LinkRuntimeState *runtime;
  struct CountdownDisplayEntry *entry;
  u8 *active;
  struct LinkSignature *signature;
  s32 target;
  s32 current;
  s32 difference;
  s32 step;
  s32 newOffset;
  s32 timer;
  s32 nextTimer;
  s32 seconds;
  s32 i;

  if (state != 0) {
    target = state->targetOffset;
    current = state->currentOffset;
    if (target != current) {
      difference = target - current;
      step = __divsi3(difference, 3);
      if (step == 0) {
        step--;
        if (difference >= 0)
          step = 1;
      }
      newOffset = current + step;
      state->currentOffset = newOffset;
      BattleCamera_SetCommandViewFar(newOffset);
    }

    entry = state->entries;
    active = state->entryActive;
    i = 2;
    do {
      if (*active++ != 0)
        Oam_AddSpriteAtDepth(entry, 240);
      i--;
      entry++;
    } while (i >= 0);
    Link_DrawPageArrows(0x06006680);

    if (state->enabled == 0)
      goto done;

    runtime = (struct LinkRuntimeState *)Ram_HeapSlots->battleWork;
    if (runtime->paused == 0)
      goto load_timer;
    state->timer = 0;
    goto done;

  signature_tail:
    if (signature->v != 'V')
      goto signature_done;
    if (signature->s != 'S')
      goto signature_done;
    state->timer = COUNTDOWN_START_FRAMES;
    timer = state->timer;
    goto signature_done;

  load_timer:
    timer = state->timer;
    if (timer >= 0)
      goto timer_ready;
    signature = &gLinkPeerSignatures__menus[runtime->side ^ 1];
    if (signature->e != 'E')
      goto signature_done;
    if (signature->d == 'D')
      goto signature_tail;

  signature_done:
    if (timer < 0)
      goto done;

  timer_ready:
    if (state->displayHandle == 0 && state->secondaryHandle == 0) {
      state->displayHandle = Link_CreateCountdownLabelWindow();
      timer = state->timer;
    }

    if (timer > 0) {
      nextTimer = timer - 1;
      state->timer = nextTimer;
      timer = nextTimer;
    }
    if (timer < 0)
      goto done;

    seconds = __divsi3(timer + FRAMES_PER_SECOND - 1, FRAMES_PER_SECOND);
    if (seconds != 0 && seconds * FRAMES_PER_SECOND == timer)
      Sound_PlayFar(SE_TRIPLE_TONE_LOW);

    if (state->displayHandle != 0)
      Text_DrawNumberInWindow(seconds, 2, state->displayHandle, 16, 8);
  }

done:
  return;
}

// The Japanese edition decodes the message (Message_DecodeRaw) into at most 32
// halfwords; the others copy its text (Message_CopyExpanded) into at most 52.
#if LANGUAGE != LANGUAGE_JAPANESE
#define BATTLE_MENU_MESSAGE_CAPACITY 52
#define BATTLE_MENU_MESSAGE_COPY Message_CopyExpanded
#else
#define BATTLE_MENU_MESSAGE_CAPACITY 32
#define BATTLE_MENU_MESSAGE_COPY Message_DecodeRaw
#endif

/// Tests whether the party may not run from this battle: battle types 3 and
/// 4 (the battles an event starts, and link battles; see
/// BATTLE_TYPE_NO_ESCAPE in battle/battle/turn.c) and a party once
/// challenged (BattleWork.challengeFlag). When it may not, shows "This is no
/// time to be running away!" until A or B is pressed or the link battle's
/// time runs out, and returns 1; else returns 0. The Flee command asks it
/// (Battle_CollectPartyCommands).
s32 Battle_RefuseFlee(void) {
  s16 buffer[64];
  s32 work;
  s32 result;
  void *state;
  s32 mode;
  s32 attrs;

  state = Ram_HeapSlots->battleWork;
  mode = gMainRoutineWork.battleRule24b;
  if (gMainRoutineWork.battleRule24b == 2 || mode <= 2 ||
      (result = 1, mode > 4)) {
    result = 0;
  }
  if (result == 0) {
    if (*(s8 *)((u8 *)state + 0x43) != 0) {
      result = 1;
    }
  }
  if (result != 0) {
    attrs = 42;
    work = Window_Create(0, 7, 30, 4, attrs);
    Text_FillCanvasBackground();
    BATTLE_MENU_MESSAGE_COPY((s32)&MsgNoTimeToRun, buffer,
                             BATTLE_MENU_MESSAGE_CAPACITY);
    Text_DrawWideStringAtOffset(buffer, work, 0, 4);
    do {
      wait(1);
    } while ((gInput.pressed & (KEY_A | KEY_B)) == 0 &&
             *(s32 *)((u8 *)((void **)Ram_HeapSlots)[57] + 0x4C) != 0);
    Window_Close(work, 1);
  }
  return result;
}
