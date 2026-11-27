/* The link battle timer and the no-flee check. Part of screens.c. */

#define COUNTDOWN_START_TICKS 900 /* 15 seconds */

/* The partner console's link signature (its payload in SerialPeerPayloads) as
   the timer reads it: halfwords 4 to 7 read "EDVS" once the partner has chosen
   its commands (mnBattleCollectPartyCommands), which starts this side's
   countdown. */
typedef struct LINK_SIGNATURE {
	Uint8  exec[8]; /* halfwords 0 to 3, "EXEC" while the link holds */
	Uint16 e;
	Uint16 d;
	Uint16 v;
	Uint16 s;
	Uint8  paddingAfterSignature[8];
} LINK_SIGNATURE;

/* The battle menus' event while the party's commands are chosen
   (mnBattleCollectPartyCommands): eases the battle view a third of the way to
   BATTLE_MENU_STATE.tgtViewOfs each tick (a pixel at least), draws the sprites
   of BATTLE_MENU_STATE.icons that are shown (the chosen command's and ability's
   icons, the party member's portrait) and slides the page arrows. In a link
   battle it also runs the timer: once the partner has chosen, 15 seconds
   counted down in a window at the top left, SE_TRIPLE_TONE_LOW playing at each
   second. */
void mnBattleMenuUpdate(void)
{
	BATTLE_MENU_STATE *state = BattleMenuStateWork;
	BATTLE_STATE	  *runtime;
	LINK_SIGNATURE	  *signature;
	Sint32			   tgt;
	Sint32			   cur;
	Sint32			   difference;
	Sint32			   step;
	Sint32			   newOfs;
	Sint32			   tmr;
	Sint32			   nextTmr;
	Sint32			   seconds;
	Sint32			   i;

	if(state != 0) {
		tgt = state->tgtViewOfs;
		cur = state->viewOfs;
		if(tgt != cur) {
			difference = tgt - cur;
			step = difference / 3;
			if(step == 0) {
				step--;
				if(difference >= 0)
					step = 1;
			}
			newOfs = cur + step;
			state->viewOfs = newOfs;
			btCameraSetCommandViewStub(newOfs);
		}

		for(i = 0; i < BATTLE_MENU_ICON_COUNT; i++) {
			if(state->iconShown[i] != 0)
				syOAMAddSprAtDepth(&state->icons[i], OAM_DEPTH_WINDOW);
		}
		mnPageArrowsDraw(LIST_PAGE_ARROW_VRAM);

		/* Local battles still animate the UI, but do not run the link timer. */
		if(state->linkTmr == 0)
			return;

		runtime = BattleStateWork;
		if(runtime->linkPaused == 0)
			goto load_timer;
		state->tmr = 0;
		return;

	signature_tail:
		if(signature->v != 'V')
			goto signature_done;
		if(signature->s != 'S')
			goto signature_done;
		state->tmr = COUNTDOWN_START_TICKS;
		tmr = state->tmr;
		goto signature_done;

		/* -1 until the partner's signature reads "EDVS", then the countdown. */
	load_timer:
		tmr = state->tmr;
		if(tmr >= 0)
			goto timer_ready;
		signature =
			(LINK_SIGNATURE *)SerialPeerPayloads[runtime->linkSide ^ 1].payload;
		if(signature->e != 'E')
			goto signature_done;
		if(signature->d == 'D')
			goto signature_tail;

	signature_done:
		if(tmr < 0)
			return;

		/* The window opens the first tick the Status screen is not up. */
	timer_ready:
		if(state->tmrWin == 0 && state->tmrHidden == 0) {
			state->tmrWin = mnLinkCreateCountdownLabelWindow();
			tmr = state->tmr;
		}

		if(tmr > 0) {
			nextTmr = tmr - 1;
			state->tmr = nextTmr;
			tmr = nextTmr;
		}
		if(tmr < 0)
			return;

		/* Round remaining ticks upward so zero appears only after time
		   expires. */
		seconds = (tmr + TICKS_PER_SECOND - 1) / TICKS_PER_SECOND;
		if(seconds != 0 && seconds * TICKS_PER_SECOND == tmr)
			sySndPlayStub(SE_TRIPLE_TONE_LOW);

		if(state->tmrWin != 0)
			mnTextDrawNumInWindow(seconds, 2, state->tmrWin, 16, 8);
	}
}

/* Tests whether the party may not run from this battle, and if so says "This is
   no time to be running away!": a battle a script starts
   (BATTLE_TYPE_SCRIPTED), a link battle, and a party once challenged
   (BATTLE_STATE.challengeFlg). In a BATTLE_TYPE_NO_ESCAPE battle the party may
   try, and always fails (battle/battle_turn.c). */
Sint32 mnBattleRefuseFlee(void)
{
	Sint16		  buf[64];
	WINDOW		 *win;
	Bool		  result;
	BATTLE_STATE *battle;
	Sint32		  mode;

	battle = BattleStateWork;
	mode = MainRoutineStateWork.battleType;
	if(MainRoutineStateWork.battleType == BATTLE_TYPE_NO_ESCAPE ||
		mode <= BATTLE_TYPE_NO_ESCAPE ||
		(result = TRUE, mode > BATTLE_TYPE_LINK)) {
		result = FALSE;
	}
	if(result == 0) {
		if((Sint8)battle->challengeFlg != 0) {
			result = TRUE;
		}
	}
	if(result != 0) {
		win = mnWindowCreate(0, 7, 30, 4,
			WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
				WINDOW_FLAG_BORDERED_CANVAS);
		mnTextFillCanvasBackground();
		BATTLE_MENU_MESSAGE_COPY(
			MsgNoTimeToRun, buf, BATTLE_MENU_MESSAGE_CAPACITY);
		mnTextDrawWideStringAtOffset(buf, win, 0, 4);
		do {
			sySynch(1);
		} while(
			(PerPush & (KEY_A | KEY_B)) == 0 && BattleMenuStateWork->tmr != 0);
		mnWindowClose(win, 1);
	}
	return result;
}
