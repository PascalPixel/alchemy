/// @file
/// The link battle timer and the no-flee check.
// Part of screens.c.

#include "screens_internal.h"

#define COUNTDOWN_START_FRAMES 900 ///< 15 seconds

/// The partner console's link signature (gLinkPeerSignatures) as the timer
/// reads it: halfwords 4 to 7 read "EDVS" once the partner has chosen its
/// commands (Battle_CollectPartyCommands), which starts this side's
/// countdown.
struct LinkSignature {
  u8 exec[8]; ///< halfwords 0 to 3, "EXEC" while the link holds
  u16 e;
  u16 d;
  u16 v;
  u16 s;
  u8 paddingAfterSignature[8];
};

/// The battle menus' task while the party's commands are chosen
/// (Battle_CollectPartyCommands): eases the battle view a third of the way to
/// BattleMenuState.targetViewOffset each frame (a pixel at least), draws the
/// sprites of BattleMenuState.icons that are shown (the chosen command's and
/// ability's icons, the party member's portrait) and slides the page arrows.
/// In a link battle it also runs the timer: once the partner has chosen, 15
/// seconds counted down in a window at the top left, SE_TRIPLE_TONE_LOW playing
/// at each second.
void BattleMenu_Update(void) {
  struct BattleMenuState *state = gBattleMenuState;
  struct BattleState *runtime;
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
    target = state->targetViewOffset;
    current = state->viewOffset;
    if (target != current) {
      difference = target - current;
      step = difference / 3;
      if (step == 0) {
        step--;
        if (difference >= 0)
          step = 1;
      }
      newOffset = current + step;
      state->viewOffset = newOffset;
      BattleCamera_SetCommandViewFar(newOffset);
    }

    for (i = 0; i < BATTLE_MENU_ICON_COUNT; i++) {
      if (state->iconShown[i] != 0)
        Oam_AddSpriteAtDepth(&state->icons[i], OAM_DEPTH_WINDOW);
    }
    PageArrows_Draw(LIST_PAGE_ARROW_VRAM);

    // Local battles still animate the UI, but do not run the link timer.
    if (state->linkTimer == 0)
      return;

    runtime = gBattleState;
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

    // The window opens the first frame the Status screen is not up.
  timer_ready:
    if (state->timerWindow == 0 && state->timerHidden == 0) {
      state->timerWindow = Link_CreateCountdownLabelWindow();
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
      Sound_PlayFar(SE_TRIPLE_TONE_LOW);

    if (state->timerWindow != 0)
      Text_DrawNumberInWindow(seconds, 2, state->timerWindow, 16, 8);
  }
}

/// Tests whether the party may not run from this battle, and if so says "This
/// is no time to be running away!": a battle an event starts
/// (BATTLE_TYPE_SCRIPTED), a link battle, and a party once challenged
/// (BattleState.challengeFlag). In a BATTLE_TYPE_NO_ESCAPE battle the party may
/// try, and always fails (battle/battle_turn.c).
s32 Battle_RefuseFlee(void) {
  s16 buffer[64];
  struct Window *window;
  s32 result;
  struct BattleState *battle;
  s32 mode;

  battle = gBattleState;
  mode = gMainRoutineState.battleType;
  if (gMainRoutineState.battleType == BATTLE_TYPE_NO_ESCAPE ||
      mode <= BATTLE_TYPE_NO_ESCAPE || (result = 1, mode > BATTLE_TYPE_LINK)) {
    result = 0;
  }
  if (result == 0) {
    if ((s8)battle->challengeFlag != 0) {
      result = 1;
    }
  }
  if (result != 0) {
    window = Window_Create(0, 7, 30, 4,
                           WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
                               WINDOW_FLAG_BORDERED_CANVAS);
    Text_FillCanvasBackground();
    BATTLE_MENU_MESSAGE_COPY(MsgNoTimeToRun, buffer,
                             BATTLE_MENU_MESSAGE_CAPACITY);
    Text_DrawWideStringAtOffset(buffer, window, 0, 4);
    do {
      wait(1);
    } while ((gKeysPressed & (KEY_A | KEY_B)) == 0 &&
             gBattleMenuState->timer != 0);
    Window_Close(window, 1);
  }
  return result;
}
