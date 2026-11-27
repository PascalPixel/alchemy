// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3): the large lettering a
// visit opens with, `Lend a Hand!` for the friends' turns, and for the race `The 1st Match` (or 2nd
// or 3rd), 3, 2, 1 and `GO`; and `WIN` or `LOSE` once the match's battle is over.

// The wait in ColossoLetteringTitle while the title stands still in the middle, and the first in
// ColossoLetteringWin and ColossoLetteringLose, while `WIN` or `LOSE` stands: the script entries
// each match rewrites.
#define LETTERING_TITLE_HOLD  15
#define LETTERING_RESULT_HOLD 13

// Starts the lettering event on the picture last loaded, laid out in shape
// (COLOSSO_LETTERING_SHAPE) and moved by that shape's script; secondHalf picks the second half of
// the picture: 2 rather than 3, or LOSE rather than WIN.
static void scColossoStartLettering(Uint32 shape, Uint32 secondHalf)
{
	const Sint16 *script;

	ColossoLetteringMode      = shape;
	ColossoLetteringParameter = (secondHalf << 4);

	sySetTickEvent(scColossoUpdateLettering, EVENT_PASS_TICK);

	script = ColossoLetteringTitle;
	if (shape == COLOSSO_SHAPE_DIGIT)
	{
		script = ColossoLetteringDigit;
	}
	if (shape == COLOSSO_SHAPE_GO)
	{
		script = ColossoLetteringGo;
	}
	if (shape == COLOSSO_SHAPE_RESULT)
	{
		if (secondHalf != 0)
		{
			script = ColossoLetteringLose;
		}
		else
		{
			script = ColossoLetteringWin;
		}
	}

	ColossoLetteringTimer        = 0;
	ColossoLetteringScript       = (Sint16 *)script;
	ColossoLetteringMoveTarget   = 0;
	ColossoLetteringMoveDuration = 0;
	ColossoLetteringPosition     = 0;
}

// Shows the lettering a visit opens with: `Lend a Hand!` when match is LETTERING_LEND_A_HAND, or
// else `The 1st Match` to `The 3rd Match`, held for match seconds, and the count of 3, 2, 1 and
// `GO`, after which the race music starts.
static void scColossoShowOpeningLettering(Sint32 match)
{
	if (match == LETTERING_LEND_A_HAND)
	{
		fdScriptStart();
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		fdScriptWait(30);
		sySoundPlay(MUS_LEVEL_UP);
		scColossoLoadLettering(COLOSSO_PICTURE_LEND_A_HAND);
		scColossoStartLettering(COLOSSO_SHAPE_TITLE, 0);
		fdScriptWait(120);
		fdScriptEnd();
		return;
	}

	sySoundPlay(SE_COLOSSO_LETTERING);
	fdScriptStart();
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	ColossoLetteringTitle[LETTERING_TITLE_HOLD] = match * TICKS_PER_SECOND;
	fdScriptWait(30);
	// The 1st to 3rd Match play MUS_LUCKY, MUS_VERY_LUCKY and MUS_SUPER_LUCKY.
	sySoundPlay(match + (MUS_LUCKY - COLOSSO_PICTURE_MATCH_TITLE_FIRST));
	scColossoLoadLettering(match);
	scColossoStartLettering(COLOSSO_SHAPE_TITLE, 0);
	fdScriptWait(120);
	while (sySoundGetBGMRestoreDelay() != 0)
	{
		sySynch(1);
	}
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	scColossoLoadLettering(COLOSSO_PICTURE_COUNTDOWN_START);
	scColossoStartLettering(COLOSSO_SHAPE_DIGIT, 0);
	sySoundPlay(SE_COLOSSO_COUNTDOWN);
	fdScriptWait(60);
	scColossoStartLettering(COLOSSO_SHAPE_DIGIT, 1);
	sySoundPlay(SE_COLOSSO_COUNTDOWN);
	fdScriptWait(60);
	scColossoLoadLettering(COLOSSO_PICTURE_COUNTDOWN_END);
	scColossoStartLettering(COLOSSO_SHAPE_DIGIT, 0);
	sySoundPlay(SE_COLOSSO_COUNTDOWN);
	fdScriptWait(60);
	scColossoLoadLettering(COLOSSO_PICTURE_GO);
	scColossoStartLettering(COLOSSO_SHAPE_GO, 0);
	sySoundPlay(SE_COLOSSO_GO);
	gmPlayMusic();
	fdScriptEnd();
	gmFlagSet(FLAG_IN_TOWN);
}

// UB: the result calls scColossoStartLettering through an Sint32 function type, the call the bytes
// show; UBFIX calls it as declared.
#define COLOSSO_START_LETTERING                                                                    \
	WHEN(FIX_FUNCTION_SIGNATURES, scColossoStartLettering, ((Sint32 (*)())scColossoStartLettering))

// Shows how a match ended, back in the arena after its battle: `WIN`, or `LOSE` in blue, swells
// over the arena with its jingle, the loser shows an ellipsis, and the screen closes. result is the
// match's number, 1 to 3, negated when Isaac lost it; the lettering stands a second for each.
static void scColossoShowMatchResult(Sint32 result)
{
	Sint32 loser;

	sySoundPlay(SE_COLOSSO_LETTERING);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	ColossoLetteringWin[LETTERING_RESULT_HOLD] = result * TICKS_PER_SECOND;
	STORE_CONST(ColossoLetteringLose[LETTERING_RESULT_HOLD]) =
		(result < 0 ? -result : result) * TICKS_PER_SECOND;
	if (result < 0)
	{
		fdScriptWait(30);
		sySoundPlay(MUS_OVERNIGHT_AT_THE_INN);
		scColossoLoadLettering(COLOSSO_PICTURE_WIN_LOSE_FOR_LOSS);
		COLOSSO_START_LETTERING(COLOSSO_SHAPE_RESULT, 1);
		fdScriptWait(-result * TICKS_PER_SECOND + TICKS_PER_SECOND);
		loser = ACTOR_PARTY_LEADER;
	}
	else
	{
		fdScriptWait(30);
		// The 1st to 3rd match play MUS_LUCKY, MUS_VERY_LUCKY and MUS_SUPER_LUCKY.
		sySoundPlay(result + (MUS_LUCKY - 1));
		scColossoLoadLettering(COLOSSO_PICTURE_WIN_LOSE);
		COLOSSO_START_LETTERING(COLOSSO_SHAPE_RESULT, 0);
		fdScriptWait(result * TICKS_PER_SECOND + TICKS_PER_SECOND);
		loser = ACTOR_OPPONENT;
	}
	fdActorShowEmote(loser, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	while (sySoundGetBGMRestoreDelay() != 0)
	{
		sySynch(1);
	}
	sySoundPlay(MUS_SILENCE);
	fdScriptWait(30);
	sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
}

#undef LETTERING_TITLE_HOLD
#undef LETTERING_RESULT_HOLD
