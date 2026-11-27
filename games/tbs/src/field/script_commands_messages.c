/* Messages shown at an actor, and the speaker's voice. Part of
   script_commands.c. */

#include "script_commands_internal.h"

/* fdMessageSetVoiceFromActor's actor id for no voice: it clears the voice. */
#define MESSAGE_VOICE_NONE 0x80000000

/* mnPortraitOpenWindow's window is 5 tiles square. */
#define PORTRAIT_WINDOW_TILES 5

/* Opens the script's current message in a speech window next to the speaking
   actor, with the speaker's portrait beside it when it has one, and moves on to
   the next message. speaker is the actor id with SPEAKER_ bits above it; the
   script's flags argument is not read. */
WINDOW *fdMessageShowAtActor(Sint32 speaker, Sint32 unusedFlg)
{
	WIN_STATE	*winState;
	FIELD_STATE *state;
	Sint32		 spr;
	Sint32		 extra;
	Sint32		 portraitTop;
	Sint32		 portraitReach;
	Sint32		 height;
	Sint32		 width;
	Sint32		 top;
	Sint32		 left;
	Sint32		 pos[3];
	WINDOW		*win;
	Sint32		 portrait;
	Sint32		 flg;
	Sint32		 msg;
	ACTOR		*actor;
	Sint32		 x;
	Sint32		 y;
	Sint32		 found;
	Sint32		 column;
#if !SPEECH_WINDOW_ANCHOR_FOLLOWS_PORTRAIT
	Sint32 portraitId;
#endif

	winState = WINDOW_STATE;
	state = FieldStateWork;
	win = 0;
	portrait = 0;
	spr = fdActorGetSprById(speaker);
	extra = 0;
	portraitTop = 0;
	portraitReach = PORTRAIT_WINDOW_TILES - 1;
	flg = speaker & SPEAKER_FLAGS;
	msg = state->msg;
	speaker &= SPEAKER_ACTOR_MASK;
	x = 0;
	actor = fdActorLookup(speaker);
	state->speaker = speaker;
	y = 0;
	found = 0;
	if(state->fastForward == 0) {
		/* Find the speaker on screen, in 8-pixel tiles. */
		if(actor != 0) {
			if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
				syConvert3Dto2D(&actor->x.fixed, pos);
				x = pos[0] >> 3;
				y = pos[1] >> 3;
				found = 1;
				y -= 2;
			} else {
				if(fdActorGetScreenPos(speaker, pos) != -1)
					found = 1;
				else
					found = 0;
				x = pos[0] >> 3;
				y = pos[1] >> 3;
			}
		} else if(speaker < ACTOR_FIRST_PLACED) {
			spr = speaker;
			actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
			if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
				syConvert3Dto2D(&actor->x.fixed, pos);
				x = pos[0] >> 3;
				y = pos[1] >> 3;
				found = 1;
			} else {
				if(fdActorGetScreenPos(
					   MainRoutineStateWork.leaderActorId, pos) != -1)
					found = 1;
				else
					found = 0;
				x = pos[0] >> 3;
				y = pos[1] >> 3;
			}
		}

		/* Place the message above or below the speaker, or mid-screen when the
		   speaker is not on screen. */
		if(!found) {
			left = DISPLAY_TILE_WIDTH / 2;
			top = DISPLAY_TILE_HEIGHT / 2;
		} else {
			left = 0;
			top = 0;
			mnTextGetMessageSizeAllPagesStub(
				msg, &left, &top, (Uint32 *)&width, (Uint32 *)&height);
			left = x - width / 2;
			if(flg & SPEAKER_WINDOW_ABOVE)
				top = y - height - 1;
			else if(!(flg & SPEAKER_WINDOW_BELOW) && y <= 8)
				top = y - height - 1;
			else
				top = y + 4;
		}

		/* Place the portrait beside the speaker, on the side with more room,
		   portraitReach tiles kept clear from its column to the screen's
		   edge. */
		/* XXX: why Mind Read keeps a tile more; a Mind Read photo of a speaker
		   at the screen's right edge would show what that tile holds. */
		if(winState->mindRead != 0)
			portraitReach = PORTRAIT_WINDOW_TILES;
		column = x;
		if(flg & SPEAKER_PORTRAIT_LEFT) {
			column = x - portraitReach - 2;
			if(column < 0)
				column = 0;
		} else if(flg & SPEAKER_PORTRAIT_RIGHT) {
			column += 2;
			if(column + portraitReach > DISPLAY_TILE_WIDTH - 1)
				column = DISPLAY_TILE_WIDTH - 1 - portraitReach;
		} else if(column <= DISPLAY_TILE_WIDTH / 2) {
			column = column - portraitReach - 2;
			if(column < 0)
				column = x + 2;
		} else {
			column += 2;
			if(column + portraitReach > DISPLAY_TILE_WIDTH - 1)
				column = x - portraitReach - 2;
		}

		/* A speaker with a portrait: put the portrait just above or below the
		   message, within the screen's rows, and grow the message to the
		   portrait's height. */
#if SPEECH_WINDOW_ANCHOR_FOLLOWS_PORTRAIT
		found = mnPortraitGetForSpeakerStub(spr);
		if(found != -1) {
			mnTextGetMessageSizeAllPagesStub(
				msg, &left, &top, (Uint32 *)&width, (Uint32 *)&height);
			msg = -1;
			if(top > y)
				portraitTop = top - PORTRAIT_WINDOW_TILES;
			else
				portraitTop = top + height;
			if(portraitTop < 0)
				portraitTop = top + height;
			else if(portraitTop + PORTRAIT_WINDOW_TILES >
				DISPLAY_TILE_HEIGHT - 1)
				portraitTop = top - PORTRAIT_WINDOW_TILES;
			if(top < portraitTop) {
				Sint32 lines = height;

				mnTextGetMessageSizeStub(
					msg, &left, &top, (Uint32 *)&width, (Uint32 *)&height);
				msg = -1;
				extra = lines - height + 1;
			}
		} else if(top < y) {
			Sint32 lines = height;

			mnTextGetMessageSizeStub(
				msg, &left, &top, (Uint32 *)&width, (Uint32 *)&height);
			extra = lines - height + 1;
			msg = found;
		}
#else
		portraitId = mnPortraitGetForSpeakerStub(spr);
		found = -1;
		if(portraitId != found) {
			mnTextGetMessageSizeAllPagesStub(
				msg, &left, &top, (Uint32 *)&width, (Uint32 *)&height);
			msg = found;
			if(top > y)
				portraitTop = top - PORTRAIT_WINDOW_TILES;
			else
				portraitTop = top + height;
			if(portraitTop < 0)
				portraitTop = top + height;
			else if(portraitTop + PORTRAIT_WINDOW_TILES >
				DISPLAY_TILE_HEIGHT - 1)
				portraitTop = top - PORTRAIT_WINDOW_TILES;
		}
		if(top < y) {
			Sint32 lines = height;

			mnTextGetMessageSizeStub(
				msg, &left, &top, (Uint32 *)&width, (Uint32 *)&height);
			msg = -1;
			extra = lines - height + 1;
		}
#endif
		if(column < 0)
			column = 0;
		else if(column + portraitReach > DISPLAY_TILE_WIDTH - 1)
			column = DISPLAY_TILE_WIDTH - 1 - portraitReach;

		/* Open the message and the portrait, and wait until the text is typed
		   out. */
		if(winState->mindRead != 0) {
			sySynch(8);
			if(extra != 0)
				win = mnMessageOpenWindowStub(msg, left, top + extra - 1,
					MESSAGE_WINDOW_ANCHOR_BOTTOM | MESSAGE_WINDOW_UNREAD);
			else
				win = mnMessageOpenWindowStub(
					msg, left, top, MESSAGE_WINDOW_UNREAD);
		} else {
			Sint32 voice = fdSpeakerGetVoice(spr);

			if(extra != 0)
				win = mnMessageOpenWindowStub(msg, left, top + extra - 1,
					MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_ANCHOR_BOTTOM |
						MESSAGE_WINDOW_ZOOM);
			else
				win = mnMessageOpenWindowStub(msg, left, top,
					MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_ZOOM);
		}
		/* DEAD: both arms open the same portrait behind a test of
		   winState->mindRead; the ROM keeps the test's load of mindRead. */
		if(winState->mindRead != 0)
			portrait =
				(Sint32)mnPortraitOpenWindowStub(spr, 0, column, portraitTop);
		else
			portrait =
				(Sint32)mnPortraitOpenWindowStub(spr, 0, column, portraitTop);
		while(!mnMessageAllTypedStub())
			sySynch(1);
	}
	state->msgWin = (Sint32)win;
	state->sideWin = portrait;
	state->msg++;
	return win;
}

/* Opens the current message at actorId (fdMessageShowAtActor) and waits until
   it has been read and its window has closed; the speaker's portrait is closed
   once the text is typed out. */
void fdScriptShowMessageAndWait(Sint32 actorId, Sint32 flg)
{
	Sint32		 member;
	FIELD_STATE *fieldState = FieldStateWork;
	WINDOW		*win = fdMessageShowAtActor(actorId, flg);
	Sint32		 spr;
	Uint32		 ticks;

	ticks = 0;
	sySynch(1);
	/* A party member the scene does not place has its portrait under its
	   character id, as fdMessageShowAtActor opened it. */
	spr = fdActorGetSprById(actorId);
	if(actorId < ACTOR_FIRST_PLACED) {
		member = actorId & SPEAKER_ACTOR_MASK;

		if(fdFindActorPlacement(member) == 0) {
			spr = member;
		}
	}
	mnPortraitCloseWindowStub(spr);

	if(fieldState->fastForward == 0) {
		while(mnWindowIsClosedStub(win) == 0) {
			sySynch(1);
			ticks++;
			/* After 600 ticks, or at once with Select, L, R and A held, the
			   windows whose text is typed out close. */
			if(ticks > 600 ||
				((PerData & KEY_SELECT) && (PerData & KEY_R) &&
					(PerData & KEY_L) && (PerData & KEY_A))) {
				mnMessageCloseFinishedWindowsStub();
			}
		}
	}

	sySynch(1);
}

/* Shows the message, then waits the given number of ticks. */
void fdScriptShowMessageThenWait(Sint32 actorId, Sint32 flg, Sint32 ticks)
{
	fdScriptShowMessageAndWait(actorId, flg);
	fdScriptWait(ticks);
}

/* Opens the question at actorId, lets the leader answer, then shows the next
   message for yes or the one after it for no. */
Sint32 fdScriptAskYesNoAndShowResponse(Sint32 actorId, Sint32 flg)
{
	Sint32				answer;
	FIELD_STATE		   *fieldState;
	MAIN_ROUTINE_STATE *mainState;

	fdMessageShowAtActor(actorId, flg);
	mainState = &MainRoutineStateWork;
	answer = fdScriptAskYesNoWithActorAnimation(mainState->leaderActorId, 0);
	if(answer == 0) {
		fdScriptShowMessageAndWait(actorId, flg);
		fieldState = FieldStateWork;
		fieldState->msg += 1;
	} else {
		fieldState = FieldStateWork;
		fieldState->msg += 1;
		fdScriptShowMessageAndWait(actorId, flg);
	}
	return answer;
}

void fdScriptNoOperationCallback(void)
{
}

/* fdScriptShowSpeakerMessage and fdScriptShowMessageAtPos move a position
   MESSAGE_POSITION_SHIFT away from the middle row of a MESSAGE_POSITION_WIDTH
   by MESSAGE_POSITION_HEIGHT area, larger than the 240 by 160 screen, and hold
   it MESSAGE_POSITION_MARGIN_X and MESSAGE_POSITION_MARGIN_Y inside the
   area. */
#define MESSAGE_POSITION_WIDTH	  320
#define MESSAGE_POSITION_HEIGHT	  240
#define MESSAGE_POSITION_MARGIN_X 8
#define MESSAGE_POSITION_MARGIN_Y 20
#define MESSAGE_POSITION_SHIFT	  32

/* Unused: reached only through a bare stub in field/mode_stubs.s, which has no
   symbol, so no code can call it. It would record the speaker, open the message
   near it and wait unless fast-forwarding, then advance the message index. It
   places the message as fdScriptShowMessageAtPos does, before passing the
   position to a window function that takes tiles; with no caller, the units it
   expected can't be told. */
void fdScriptShowSpeakerMessage(Sint32 speaker)
{
	/* BUG: the lookup's result is discarded, and pos is left unset. */
#if FIX_UNINITIALIZED_READ
	Sint32 pos[3];
	ACTOR *actor;
#else
	struct {
		Sint32 x, y;
	} pos;
#endif
	FIELD_STATE *fieldState = FieldStateWork;

	speaker &= SPEAKER_ACTOR_MASK;
#if FIX_UNINITIALIZED_READ
	/* Only the actor slots hold actor pointers; fdActorLookup's legacy range
	   also exposes the script fields after them. */
	if((Uint32)speaker <
		sizeof(fieldState->actors) / sizeof(fieldState->actors[0]))
		actor = fdActorLookup(speaker);
	else
		actor = NULL;
#else
	fdActorLookup(speaker);
#endif
	fieldState->speaker = speaker;
	if(fieldState->fastForward == 0) {
#if FIX_UNINITIALIZED_READ
		Sint32 positionFound = 0;

		/* Supply the actual speaker position as fdMessageShowAtActor does. */
		if(actor != NULL) {
			if(fieldState->mode == ACTOR_SYSTEM_WORLD_MAP)
				positionFound =
					syConvert3Dto2D(
						(const Uint8 *)actor + offsetof(ACTOR, x), pos) != 0;
			else
				positionFound = fdActorGetScreenPos(speaker, pos) != -1;
		}
		if(positionFound) {
			Sint32 winX = pos[0];
			Sint32 winY = pos[1];
			Sint32 win;
#else
		Sint32 winX = pos.x;
		Sint32 winY = pos.y;
		Sint32 win;
#endif

			if(winY > MESSAGE_POSITION_HEIGHT / 2 - 1)
				winY += MESSAGE_POSITION_SHIFT;
			else
				winY -= MESSAGE_POSITION_SHIFT;
			winX = winX < MESSAGE_POSITION_MARGIN_X ? MESSAGE_POSITION_MARGIN_X
													: winX;
			if(winX > MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X)
				winX = MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X;
			winY = winY < MESSAGE_POSITION_MARGIN_Y ? MESSAGE_POSITION_MARGIN_Y
													: winY;
			if(winY > MESSAGE_POSITION_HEIGHT - MESSAGE_POSITION_MARGIN_Y)
				winY = MESSAGE_POSITION_HEIGHT - MESSAGE_POSITION_MARGIN_Y;
			win = (Sint32)mnMessageOpenWindowStub(
				fieldState->msg, winX, winY, MESSAGE_WINDOW_ZOOM);
			fieldState->msgWin = win;
			while(mnWindowIsClosedStub((WINDOW *)win) == 0)
				sySynch(1);
#if FIX_UNINITIALIZED_READ
		}
#endif
	}
	fieldState->msg++;
}

/* Unused: reached only through a bare stub in field_mode.s; forwards its
   arguments to fdScriptShowMessageAtPos. */
void fdScriptForwardShowMessageAtPos(
	Sint32 unusedA, Sint32 unusedB, Sint32 x, Sint32 y)
{
	fdScriptShowMessageAtPos(unusedA, unusedB, x, y);
}

/* Unused: reached only through a bare stub in field_mode.s. It would open the
   script's message at (x, y), moved and held as MESSAGE_POSITION_WIDTH says,
   wait for it and advance the message index; its units are as unknown as
   fdScriptShowSpeakerMessage's. */
void fdScriptShowMessageAtPos(
	Sint32 unusedA, Sint32 unusedB, Sint32 x, Sint32 y)
{
	FIELD_STATE *fieldState = FieldStateWork;
	Sint32		 winY = y;
	Sint32		 winX = x;
	Sint32		 minX = MESSAGE_POSITION_MARGIN_X;
	Sint32		 minY = MESSAGE_POSITION_MARGIN_Y;
	Sint32		 win;

	if(winY > MESSAGE_POSITION_HEIGHT / 2 - 1)
		winY += MESSAGE_POSITION_SHIFT;
	else
		winY -= MESSAGE_POSITION_SHIFT;

	if(x < minX)
		winX = minX;
	if(winX > MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X)
		winX = MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X;
	CLAMP(winY, minY, MESSAGE_POSITION_HEIGHT - MESSAGE_POSITION_MARGIN_Y);

	win = (Sint32)mnMessageOpenWindowStub(
		fieldState->msg, winX, winY, MESSAGE_WINDOW_ZOOM);
	while(mnWindowIsClosedStub((WINDOW *)win) == 0)
		sySynch(1);
	fieldState->msg++;
}

/* Closes the portrait of an actor's sprite, if it has one. */
void fdActorClosePortrait(Sint32 actorId)
{
	Sint32 spr;

	spr = fdActorGetSprById(actorId);
	if(spr != -1)
		mnPortraitCloseWindowStub(spr);
}

/* Two actors speak at once, each message at its own (x, y) with its portrait at
   (portraitX, portraitY), all in tiles. Waits for A, B, L or R, then closes
   both. */
void fdScriptShowTwoMessagesAndWait(Sint32 first, Sint32 firstX, Sint32 firstY,
	Sint32 firstPortraitX, Sint32 firstPortraitY, Sint32 second, Sint32 secondX,
	Sint32 secondY, Sint32 secondPortraitX, Sint32 secondPortraitY)
{
	FIELD_STATE *state = FieldStateWork;
	Sint32		 firstSpr = fdActorGetSprById(first);
	Sint32		 secondSpr = fdActorGetSprById(second);
	Sint32		 firstWin;
	Sint32		 secondWin;

	firstWin = (Sint32)mnMessageOpenWindowStub(state->msg++, firstX, firstY,
		MESSAGE_WINDOW_VOICE(fdSpeakerGetVoice(firstSpr)));
	mnPortraitOpenWindowStub(firstSpr, 0, firstPortraitX, firstPortraitY);

	secondWin = (Sint32)mnMessageOpenWindowStub(state->msg++, secondX, secondY,
		MESSAGE_WINDOW_VOICE(fdSpeakerGetVoice(secondSpr)));
	mnPortraitOpenWindowStub(secondSpr, 0, secondPortraitX, secondPortraitY);

	while(mnMessageAllTypedStub() == 0)
		sySynch(1);

	sySynch(1);
	while((PerPush & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0)
		sySynch(1);

	sySynch(1);
	mnPortraitCloseWindowStub(firstSpr);
	mnPortraitCloseWindowStub(secondSpr);
	mnMessageCloseFinishedWindowsStub();
	sySynch(1);

	while(mnWindowIsClosedStub((WINDOW *)firstWin) == 0)
		sySynch(1);
	while(mnWindowIsClosedStub((WINDOW *)secondWin) == 0)
		sySynch(1);

	sySynch(1);
}

/* Sets the voice of the messages that follow to that of actorId's sprite, at
   the rate the message speed option picks (MessageSpeedVoiceRates);
   MESSAGE_VOICE_NONE clears it. */
void fdMessageSetVoiceFromActor(Sint32 actorId)
{
	WIN_STATE *windowState = WINDOW_STATE;
	Uint8	   rate;
	Sint32	   voice;

	if(actorId == MESSAGE_VOICE_NONE) {
		windowState->voiceSnd = 0;
		windowState->voiceCountdown = 0;
	} else {
		voice = fdSpeakerGetVoice(fdActorGetSprById(actorId));
		rate = MessageSpeedVoiceRates[MainRoutineStateWork.msgSpeed];
		windowState->voiceSnd = voice;
		windowState->voiceCountdown = rate;
	}
}

#undef MESSAGE_VOICE_NONE
#undef PORTRAIT_WINDOW_TILES
#undef MESSAGE_POSITION_WIDTH
#undef MESSAGE_POSITION_HEIGHT
#undef MESSAGE_POSITION_MARGIN_X
#undef MESSAGE_POSITION_MARGIN_Y
#undef MESSAGE_POSITION_SHIFT
