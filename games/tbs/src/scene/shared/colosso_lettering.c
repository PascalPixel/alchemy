/// @file
/// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
/// the large lettering a visit opens with, `Lend a Hand!` for the friends'
/// turns, and for the race `The 1st Match` (or 2nd or 3rd), 3, 2, 1 and `GO`;
/// and `WIN` or `LOSE` once the match's battle is over.

/// The wait in gColosso_LetteringTitle while the title stands still in the
/// middle, and the first in gColosso_LetteringWin and gColosso_LetteringLose,
/// while `WIN` or `LOSE` stands: the script entries each match rewrites.
#define LETTERING_TITLE_HOLD 15
#define LETTERING_RESULT_HOLD 13

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
  gColosso_LetteringTitle[LETTERING_TITLE_HOLD] = match * FRAMES_PER_SECOND;
  Event_Wait(30);
  // The 1st to 3rd Match play MUS_LUCKY, MUS_VERY_LUCKY and
  // MUS_SUPER_LUCKY.
  Sound_Play(match + (MUS_LUCKY - COLOSSO_PICTURE_MATCH_TITLE_FIRST));
  Colosso_LoadLettering(match);
  Colosso_StartLettering(COLOSSO_SHAPE_TITLE, 0);
  Event_Wait(120);
  while (Sound_GetBgmRestoreDelay() != 0) {
    wait(1);
  }
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Colosso_LoadLettering(COLOSSO_PICTURE_COUNTDOWN_START);
  Colosso_StartLettering(COLOSSO_SHAPE_DIGIT, 0);
  Sound_Play(SE_COLOSSO_COUNTDOWN);
  Event_Wait(60);
  Colosso_StartLettering(COLOSSO_SHAPE_DIGIT, 1);
  Sound_Play(SE_COLOSSO_COUNTDOWN);
  Event_Wait(60);
  Colosso_LoadLettering(COLOSSO_PICTURE_COUNTDOWN_END);
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

// UB: the result calls Colosso_StartLettering through an s32 function type,
// the call the bytes show; UBFIX calls it as declared.
#ifdef UBFIX
#define COLOSSO_START_LETTERING Colosso_StartLettering
#else
#define COLOSSO_START_LETTERING ((s32 (*)())Colosso_StartLettering)
#endif

/// Shows how a match ended, back in the arena after its battle: `WIN`, or
/// `LOSE` in blue, swells over the arena with its jingle, the loser shows an
/// ellipsis, and the screen closes. result is the match's number, 1 to 3,
/// negated when Isaac lost it; the lettering stands a second for each.
static void Colosso_ShowMatchResult(s32 result) {
  s32 loser;

  Sound_Play(SE_COLOSSO_LETTERING);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  gColosso_LetteringWin[LETTERING_RESULT_HOLD] = result * FRAMES_PER_SECOND;
  STORE_CONST(gColosso_LetteringLose[LETTERING_RESULT_HOLD]) =
      (result < 0 ? -result : result) * FRAMES_PER_SECOND;
  if (result < 0) {
    Event_Wait(30);
    Sound_Play(MUS_OVERNIGHT_AT_THE_INN);
    Colosso_LoadLettering(COLOSSO_PICTURE_WIN_LOSE_FOR_LOSS);
    COLOSSO_START_LETTERING(COLOSSO_SHAPE_RESULT, 1);
    Event_Wait(-result * FRAMES_PER_SECOND + FRAMES_PER_SECOND);
    loser = ACTOR_PARTY_LEADER;
  } else {
    Event_Wait(30);
    // The 1st to 3rd match play MUS_LUCKY, MUS_VERY_LUCKY and
    // MUS_SUPER_LUCKY.
    Sound_Play(result + (MUS_LUCKY - 1));
    Colosso_LoadLettering(COLOSSO_PICTURE_WIN_LOSE);
    COLOSSO_START_LETTERING(COLOSSO_SHAPE_RESULT, 0);
    Event_Wait(result * FRAMES_PER_SECOND + FRAMES_PER_SECOND);
    loser = ACTOR_OPPONENT;
  }
  Actor_ShowEmote(loser, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  while (Sound_GetBgmRestoreDelay() != 0) {
    wait(1);
  }
  Sound_Play(MUS_SILENCE);
  Event_Wait(30);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

#undef LETTERING_TITLE_HOLD
#undef LETTERING_RESULT_HOLD
