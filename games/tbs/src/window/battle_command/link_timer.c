/// @file
/// The link battle timer and the no-escape check.
/// Part of battle_command.c, the menu object, compiled only through it.

#include "../battle_command_internal.h"

#define COUNTDOWN_START_FRAMES 900
#define FRAMES_PER_SECOND 60

struct LinkSignature {
  u8 padding000[8];
  u16 e;
  u16 d;
  u16 v;
  u16 s;
  u8 padding010[8];
};

/// Advance the link command-entry timer, slide the battle panels, and redraw
/// the remaining seconds. Pause flags and peer signatures stop the timer.
void UpdateLinkSessionCountdown(void) {
  struct LinkRoundState *state = COUNTDOWN;
  struct BattleWork *runtime;
  struct MenuSprite *entry;
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
      step = difference / 3;
      if (step == 0) {
        step--;
        if (difference >= 0)
          step = 1;
      }
      newOffset = current + step;
      state->currentOffset = newOffset;
      Camera_ConfigureSceneFar(newOffset);
    }

    entry = state->entries;
    active = state->entryActive;
    i = 2;
    do {
      if (*active++ != 0)
        Runtime_PushSlotEntry(entry, 240);
      i--;
      entry++;
    } while (i >= 0);
    Link_DrawShiftedTilePair(0x06006680);

    // Local battles still animate the UI, but do not run the link timer.
    if (state->enabled == 0)
      return;

    runtime = gBattleWork;
    if (runtime->linkPaused == 0)
      goto load_timer;
    state->timer = 0;
    return;

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
    signature =
        (struct LinkSignature *)gLinkPeerSignatures[runtime->linkSide ^ 1];
    if (signature->e != 'E')
      goto signature_done;
    if (signature->d == 'D')
      goto signature_tail;

  signature_done:
    if (timer < 0)
      return;

  timer_ready:
    if (state->displayHandle == 0 && state->secondaryHandle == 0) {
      state->displayHandle = (s32)Link_CreateCountdownLabelWindow();
      timer = state->timer;
    }

    if (timer > 0) {
      nextTimer = timer - 1;
      state->timer = nextTimer;
      timer = nextTimer;
    }
    if (timer < 0)
      return;

    // Round remaining frames upward so zero appears only after time expires.
    seconds = (timer + FRAMES_PER_SECOND - 1) / FRAMES_PER_SECOND;
    if (seconds != 0 && seconds * FRAMES_PER_SECOND == timer)
      AudioCommand_PlayFar(SE_TRIPLE_TONE_LOW);

    if (state->displayHandle != 0)
      UiText_DrawNumberInWindow(seconds, 2,
                                (struct UiWindow *)state->displayHandle, 16, 8);
  }
}

/// Test whether this battle forbids escape. Show the reason until A/B or the
/// link timer expires, returning 1 when escape is forbidden and 0 otherwise.
s32 UiText_ShowLocalizedMessageAndWait(void) {
  s16 buffer[64];
  struct UiWindow *work;
  s32 result;
  struct BattleWork *battle;
  s32 mode;

  battle = gBattleWork;
  mode = gGameState.battleStart;
  if (gGameState.battleStart == 2 || mode <= 2 || (result = 1, mode > 4)) {
    result = 0;
  }
  if (result == 0) {
    if ((s8)battle->challengeFlag != 0) {
      result = 1;
    }
  }
  if (result != 0) {
    work = UiWindow_Create(0, 7, 30, 4, 42);
    Ui_FillVramBlockPattern();
    BATTLE_MENU_MESSAGE_COPY(MsgNoTimeToRun, buffer,
                             BATTLE_MENU_MESSAGE_CAPACITY);
    UiText_RenderWideStringAtOffset(buffer, work, 0, 4);
    do {
      WaitFrames(1);
    } while ((gKeysPressed & (KEY_A | KEY_B)) == 0 && COUNTDOWN->timer != 0);
    UiWork_Finalize(work, 1);
  }
  return result;
}
