/// @file
/// The link battle timer and the no-escape check.
// Part of battle_command.c.

#include "../battle_command_internal.h"

#define COUNTDOWN_START_FRAMES 900 ///< 15 seconds
#define FRAMES_PER_SECOND 60

/// The partner console's link signature (gLinkPeerSignatures) as the timer
/// reads it: halfwords 4 to 7 read "EDVS" once the partner has chosen its
/// commands (Battle_CollectPartyCommands), which starts this side's
/// countdown.
struct LinkSignature {
  u8 padding000[8]; ///< halfwords 0 to 3, "EXEC" while the link holds
  u16 e;
  u16 d;
  u16 v;
  u16 s;
  u8 padding010[8];
};

/// A scheduler callback while the party's commands are chosen
/// (Battle_CollectPartyCommands): eases the battle scene a third of the way
/// to BattleMenuWork.targetSceneOffset each frame (a pixel at least), draws
/// the sprites of BattleMenuWork.icons that are shown (the chosen command's
/// and ability's icons, the member's portrait) and slides the page arrows.
/// In a link battle it also runs the timer: once the partner has chosen its
/// commands, 15 seconds, shown in a window at the top left with a tone at
/// each whole second; at 0 the menus end as if cancelled, or take their
/// first command. A paused link sets the timer to 0 at once.
void UpdateLinkSessionCountdown(void) {
  struct BattleMenuWork *state = gLinkCountdownWork;
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
    target = state->targetSceneOffset;
    current = state->sceneOffset;
    if (target != current) {
      difference = target - current;
      step = difference / 3;
      if (step == 0) {
        step--;
        if (difference >= 0)
          step = 1;
      }
      newOffset = current + step;
      state->sceneOffset = newOffset;
      Camera_ConfigureSceneFar(newOffset);
    }

    entry = state->icons;
    active = state->iconShown;
    i = 2;
    do {
      if (*active++ != 0)
        Runtime_PushSlotEntry(entry, 240);
      i--;
      entry++;
    } while (i >= 0);
    Link_DrawShiftedTilePair(0x06006680);

    // Local battles still animate the UI, but do not run the link timer.
    if (state->linkTimer == 0)
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

    // -1 until the partner's signature reads "EDVS", then the countdown.
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

    // The window opens the first frame the status screen is not up.
  timer_ready:
    if (state->timerWindow == 0 && state->timerHidden == 0) {
      state->timerWindow = (s32)Link_CreateCountdownLabelWindow();
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

    if (state->timerWindow != 0)
      UiText_DrawNumberInWindow(seconds, 2,
                                (struct UiWindow *)state->timerWindow, 16, 8);
  }
}

/// Tests whether the party may not run from this battle: battle starts 3 and
/// 4 (the battles a scene starts, and link battles; see
/// BATTLE_START_NO_ESCAPE in battle/battle/round.c) and a party once
/// challenged (BattleWork.challengeFlag). When it may not, shows "This is no
/// time to be running away!" until A or B is pressed or the link battle's
/// time runs out, and returns 1; else returns 0. The Flee command asks it
/// (Battle_CollectPartyCommands).
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
    } while ((gKeysPressed & (KEY_A | KEY_B)) == 0 &&
             gLinkCountdownWork->timer != 0);
    UiWork_Finalize(work, 1);
  }
  return result;
}
