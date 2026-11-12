/// @file
/// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
/// the large lettering a visit opens with, `Lend a Hand!` for the friends'
/// turns, and for the race `The 1st Match` (or 2nd or 3rd), 3, 2, 1 and `GO`.
#include "types.h"
#include "task.h"

/// Starts the lettering task on the picture last loaded, laid out in shape
/// (enum ColossoLetteringShape) and moved by that shape's script;
/// secondHalf picks the second half of the picture: 2 rather than 3, or
/// LOSE rather than WIN.
static void Colosso_StartLettering(u32 shape, u32 secondHalf) {
  const s16 *script;

  gColosso_LetteringMode = shape;
  gColosso_LetteringParameter = (secondHalf << 4);

  Task_AddFrame(Colosso_UpdateLettering, TASK_PASS_FRAME);

  script = gColosso_LetteringTitle;
  if (shape == COLOSSO_SHAPE_DIGIT) {
    script = gColosso_LetteringDigit;
  }
  if (shape == COLOSSO_SHAPE_GO) {
    script = gColosso_LetteringGo;
  }
  if (shape == COLOSSO_SHAPE_RESULT) {
    if (secondHalf != 0) {
      script = gColosso_LetteringLose;
    } else {
      script = gColosso_LetteringWin;
    }
  }

  gColosso_LetteringTimer = 0;
  gColosso_LetteringScript = (s16 *)script;
  gColosso_ModeMoveTarget = 0;
  gColosso_ModeMoveDuration = 0;
  gColosso_LetteringPosition = 0;
}

/// Shows the lettering a visit opens with: `Lend a Hand!` when match is 0, or
/// else `The 1st Match` to `The 3rd Match`, held for match seconds, and the
/// count of 3, 2, 1 and `GO`, after which the race music starts.
static void Colosso_ShowOpeningLettering(s32 match) {
  if (match == 0) {
    Event_Start();
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(30);
    Sound_Play(MUS_LEVEL_UP);
    Colosso_LoadLettering(COLOSSO_PICTURE_LEND_A_HAND);
    Colosso_StartLettering(COLOSSO_SHAPE_TITLE, 0);
    Event_Wait(120);
    Event_End();
    return;
  }

  Sound_Play(SE_COLOSSO_LETTERING);
  Event_Start();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  // The wait while the title stands still in the middle.
  gColosso_LetteringTitle[15] = match * 60;
  Event_Wait(30);
  // The 1st to 3rd Match play MUS_JINGLE_SMALL, MUS_JINGLE_MEDIUM and
  // MUS_JINGLE_LARGE.
  Sound_Play(match + (MUS_JINGLE_SMALL - COLOSSO_PICTURE_MATCH_1));
  Colosso_LoadLettering(match);
  Colosso_StartLettering(COLOSSO_SHAPE_TITLE, 0);
  Event_Wait(120);
  while (Sound_GetBgmRestoreDelay() != 0) {
    wait(1);
  }
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Colosso_LoadLettering(COLOSSO_PICTURE_COUNT_3_2);
  Colosso_StartLettering(COLOSSO_SHAPE_DIGIT, 0);
  Sound_Play(SE_COLOSSO_COUNTDOWN);
  Event_Wait(60);
  Colosso_StartLettering(COLOSSO_SHAPE_DIGIT, 1);
  Sound_Play(SE_COLOSSO_COUNTDOWN);
  Event_Wait(60);
  Colosso_LoadLettering(COLOSSO_PICTURE_COUNT_1);
  Colosso_StartLettering(COLOSSO_SHAPE_DIGIT, 0);
  Sound_Play(SE_COLOSSO_COUNTDOWN);
  Event_Wait(60);
  Colosso_LoadLettering(COLOSSO_PICTURE_GO);
  Colosso_StartLettering(COLOSSO_SHAPE_GO, 0);
  Sound_Play(SE_COLOSSO_GO);
  Scene_PlayMusic();
  Event_End();
  GameFlag_Set(FLAG_IN_TOWN);
}
