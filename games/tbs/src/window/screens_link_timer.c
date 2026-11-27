// The link battle timer and the no-flee check. Part of screens.c.

#define COUNTDOWN_START_TICKS 900 // 15 seconds

// The partner console's link signature (its payload in SerialPeerPayloads) as the timer reads it:
// halfwords 4 to 7 read "EDVS" once the partner has chosen its commands
// (mnBattleCollectPartyCommands), which starts this side's countdown.
typedef struct LINK_SIGNATURE
{
	Uint8  exec[8]; // halfwords 0 to 3, "EXEC" while the link holds
	Uint16 e;
	Uint16 d;
	Uint16 v;
	Uint16 s;
	Uint8  paddingAfterSignature[8];
} LINK_SIGNATURE;

// The battle menus' event while the party's commands are chosen (mnBattleCollectPartyCommands):
// eases the battle view a third of the way to BATTLE_MENU_STATE.targetViewOffset each tick (a pixel
// at least), draws the sprites of BATTLE_MENU_STATE.icons that are shown (the chosen command's and
// ability's icons, the party member's portrait) and slides the page arrows. In a link battle it
// also runs the timer: once the partner has chosen, 15 seconds counted down in a window at the top
// left, SE_TRIPLE_TONE_LOW playing at each second.
void mnBattleMenuUpdate(void)
{
	BATTLE_MENU_STATE *state = BattleMenuStateWork;
	BATTLE_STATE      *runtime;
	LINK_SIGNATURE    *signature;
	Sint32             target;
	Sint32             current;
	Sint32             difference;
	Sint32             step;
	Sint32             newOffset;
	Sint32             timer;
	Sint32             nextTimer;
	Sint32             seconds;
	Sint32             i;

	if (state != 0)
	{
		target  = state->targetViewOffset;
		current = state->viewOffset;
		if (target != current)
		{
			difference = target - current;
			step       = difference / 3;
			if (step == 0)
			{
				step--;
				if (difference >= 0)
					step = 1;
			}
			newOffset         = current + step;
			state->viewOffset = newOffset;
			btCameraSetCommandViewStub(newOffset);
		}

		for (i = 0; i < BATTLE_MENU_ICON_COUNT; i++)
		{
			if (state->iconShown[i] != 0)
				syOAMAddSpriteAtDepth(&state->icons[i], OAM_DEPTH_WINDOW);
		}
		mnPageArrowsDraw(LIST_PAGE_ARROW_VRAM);

		// Local battles still animate the UI, but do not run the link timer.
		if (state->linkTimer == 0)
			return;

		runtime = BattleStateWork;
		if (runtime->linkPaused == 0)
			goto load_timer;
		state->timer = 0;
		return;

	signature_tail:
		if (signature->v != 'V')
			goto signature_done;
		if (signature->s != 'S')
			goto signature_done;
		state->timer = COUNTDOWN_START_TICKS;
		timer        = state->timer;
		goto signature_done;

		// -1 until the partner's signature reads "EDVS", then the countdown.
	load_timer:
		timer = state->timer;
		if (timer >= 0)
			goto timer_ready;
		signature = (LINK_SIGNATURE *)SerialPeerPayloads[runtime->linkSide ^ 1].payload;
		if (signature->e != 'E')
			goto signature_done;
		if (signature->d == 'D')
			goto signature_tail;

	signature_done:
		if (timer < 0)
			return;

		// The window opens the first tick the Status screen is not up.
	timer_ready:
		if (state->timerWindow == 0 && state->timerHidden == 0)
		{
			state->timerWindow = mnLinkCreateCountdownLabelWindow();
			timer              = state->timer;
		}

		if (timer > 0)
		{
			nextTimer    = timer - 1;
			state->timer = nextTimer;
			timer        = nextTimer;
		}
		if (timer < 0)
			return;

		// Round remaining ticks upward so zero appears only after time expires.
		seconds = (timer + TICKS_PER_SECOND - 1) / TICKS_PER_SECOND;
		if (seconds != 0 && seconds * TICKS_PER_SECOND == timer)
			sySoundPlayStub(SE_TRIPLE_TONE_LOW);

		if (state->timerWindow != 0)
			mnTextDrawNumberInWindow(seconds, 2, state->timerWindow, 16, 8);
	}
}

// Tests whether the party may not run from this battle, and if so says "This is no time to be
// running away!": a battle a script starts (BATTLE_TYPE_SCRIPTED), a link battle, and a party once
// challenged (BATTLE_STATE.challengeFlag). In a BATTLE_TYPE_NO_ESCAPE battle the party may try, and
// always fails (battle/battle_turn.c).
Sint32 mnBattleRefuseFlee(void)
{
	Sint16        buf[64];
	WINDOW       *window;
	Bool          result;
	BATTLE_STATE *battle;
	Sint32        mode;

	battle = BattleStateWork;
	mode   = MainRoutineStateWork.battleType;
	if (MainRoutineStateWork.battleType == BATTLE_TYPE_NO_ESCAPE || mode <= BATTLE_TYPE_NO_ESCAPE
	    || (result = TRUE, mode > BATTLE_TYPE_LINK))
	{
		result = FALSE;
	}
	if (result == 0)
	{
		if ((Sint8)battle->challengeFlag != 0)
		{
			result = TRUE;
		}
	}
	if (result != 0)
	{
		window = mnWindowCreate(0,
		                        7,
		                        30,
		                        4,
		                        WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS
		                            | WINDOW_FLAG_BORDERED_CANVAS);
		mnTextFillCanvasBackground();
		BATTLE_MENU_MESSAGE_COPY(MsgNoTimeToRun, buf, BATTLE_MENU_MESSAGE_CAPACITY);
		mnTextDrawWideStringAtOffset(buf, window, 0, 4);
		do
		{
			sySynch(1);
		} while ((PerPush & (KEY_A | KEY_B)) == 0 && BattleMenuStateWork->timer != 0);
		mnWindowClose(window, 1);
	}
	return result;
}
