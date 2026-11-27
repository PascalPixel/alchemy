/* @file */
/* Messages shown at an actor, and the speaker's voice. */
/* Part of event_commands.c. */

#include "window/stubs.h"
#include "field/field_sequence.h"
#include "field/object.h"
#include "field/event.h"

/* fdMessageSetVoiceFromActor's key for no voice: it clears the voice. */
#define MESSAGE_VOICE_NONE 0x80000000

/* The event command "open message": opens the event's current message in a */
/* speech window next to the speaking actor, with the speaker's portrait beside
 */
/* it when it has one, and moves on to the next message. */
s32 fdMessageShowAtActor(s32 speaker, s32 unused) {
	WIN_STATE *win;
	FIELD_STATE *state;
	s32 sprite;
	s32 extra;
	s32 portraitTop;
	s32 margin;
	s32 height;
	s32 width;
	s32 top;
	s32 left;
	s32 pos[3];
	WINDOW *handle;
	s32 portrait;
	s32 flags;
	s32 message;
	ACTOR *actor;
	s32 x;
	s32 y;
	s32 found;
	s32 column;

	win = (WIN_STATE *)Ram_HeapSlots->winState;
	state = FieldStateWork;
	handle = 0;
	portrait = 0;
	sprite = fdActorGetSprById(speaker);
	extra = 0;
	portraitTop = 0;
	margin = 4;
	flags = speaker & SPEAKER_FLAGS;
	message = state->msg;
	speaker &= SPEAKER_ACTOR_MASK;
	x = 0;
	actor = fdActorLookup(speaker);
	state->speaker = speaker;
	y = 0;
	found = 0;
	if(state->fastForward == 0) {
		if(actor != 0) {
			if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
				syConvert3Dto2D(&actor->x.fixed, pos);
				x = pos[0] >> 3;
				y = pos[1] >> 3;
				found = 1;
				y -= 2;
			}
			else {
				if(fdActorGetScrPos(speaker, pos) != -1)
					found = 1;
				else
					found = 0;
				x = pos[0] >> 3;
				y = pos[1] >> 3;
			}
		}
		else if(speaker <= ACTOR_PIERS) {
			sprite = speaker;
			actor = fdActorLookup(fdPartyGetLeaderActor());
			if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
				syConvert3Dto2D(&actor->x.fixed, pos);
				x = pos[0] >> 3;
				y = pos[1] >> 3;
				found = 1;
			}
			else {
				if(fdActorGetScrPos(fdPartyGetLeaderActor(), pos) != -1)
					found = 1;
				else
					found = 0;
				x = pos[0] >> 3;
				y = pos[1] >> 3;
			}
		}

		if(!found) {
			left = DISPLAY_TILE_WIDTH / 2;
			top = DISPLAY_TILE_HEIGHT / 2;
		}
		else {
			left = 0;
			top = 0;
			mnTextGetMessageSizeAllPagesStub(message, &left, &top, &width,
											 &height);
			left = x - (s32)width / 2;
			if(flags & SPEAKER_WINDOW_ABOVE)
				top = y - height - 1;
			else if(!(flags & SPEAKER_WINDOW_BELOW) && y <= 8)
				top = y - height - 1;
			else
				top = y + 4;
		}

		/* Place the portrait beside the speaker, on the side with more room. */
		if(win->mindRead != 0)
			margin = 5;
		column = x;
		if(flags & SPEAKER_PORTRAIT_LEFT) {
			column = x - margin - 2;
			if(column < 0)
				column = 0;
		}
		else if(flags & SPEAKER_PORTRAIT_RIGHT) {
			column += 2;
			if(column + margin > DISPLAY_TILE_WIDTH - 1)
				column = DISPLAY_TILE_WIDTH - 1 - margin;
		}
		else if(column <= DISPLAY_TILE_WIDTH / 2) {
			column = column - margin - 2;
			if(column < 0)
				column = x + 2;
		}
		else {
			column += 2;
			if(column + margin > DISPLAY_TILE_WIDTH - 1)
				column = x - margin - 2;
		}

		/* A speaker with a portrait: put the portrait just above or below the
		 */
		/* message, within the screen's rows, and grow the message to the */
		/* portrait's height. */
		found = mnPortraitGetForSpeakerStub(sprite);
		if(found != -1) {
			mnTextGetMessageSizeAllPagesStub(message, &left, &top, &width,
											 &height);
			message = -1;
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
				s32 lines = height;

				mnTextGetMessageSizeStub(message, &left, &top, &width, &height);
				message = -1;
				extra = lines - height + 1;
			}
		}
		else if(top < y) {
			s32 lines = height;

			mnTextGetMessageSizeStub(message, &left, &top, &width, &height);
			extra = lines - height + 1;
			message = found;
		}
		if(column < 0)
			column = 0;
		else if(column + margin > DISPLAY_TILE_WIDTH - 1)
			column = DISPLAY_TILE_WIDTH - 1 - margin;

		/* Open the message and the portrait, and wait until the text is typed
		 * out. */
		if(win->mindRead != 0) {
			sySynch(8);
			if(extra != 0)
				handle = mnMessageOpenWindowStub(message, left, top + extra - 1,
												 MESSAGE_WINDOW_ANCHOR_BOTTOM |
													 MESSAGE_WINDOW_UNREAD);
			else
				handle = mnMessageOpenWindowStub(message, left, top,
												 MESSAGE_WINDOW_UNREAD);
		}
		else {
			s32 voice = fdSpeakerGetVoice(sprite);

			if(extra != 0)
				handle = mnMessageOpenWindowStub(
					message, left, top + extra - 1,
					MESSAGE_WINDOW_VOICE(voice) | MESSAGE_WINDOW_ANCHOR_BOTTOM |
						MESSAGE_WINDOW_ZOOM);
			else
				handle = mnMessageOpenWindowStub(message, left, top,
												 MESSAGE_WINDOW_VOICE(voice) |
													 MESSAGE_WINDOW_ZOOM);
		}
		if(win->mindRead != 0)
			portrait =
				(s32)mnPortraitOpenWindowStub(sprite, 0, column, portraitTop);
		else
			portrait =
				(s32)mnPortraitOpenWindowStub(sprite, 0, column, portraitTop);
		while(!mnMessageAllTypedStub())
			sySynch(1);
	}
	state->msgWin = (s32)handle;
	state->sideWin = (WINDOW *)portrait;
	state->msg++;
	return handle;
}

/* Opens the event's current message at the speaker and waits for its window; */
/* after ten seconds, or with Select, R, L and A held, the text is finished at
 */
/* once. */
void fdMessageShowAtActorAndWait(s32 speaker, s32 flags) {
	s32 masked;
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	void *window = fdMessageShowAtActor(speaker, flags);
	s32 id;
	u32 frames;

	frames = 0;
	sySynch(1);
	id = fdActorGetSprById(speaker);
	if(speaker <= ACTOR_PIERS) {
		masked = speaker & SPEAKER_ACTOR_MASK;
		if(scFindActorPlacement(masked) == 0)
			id = masked;
	}
	mnPortraitCloseWindowStub(id);

	if(state->fastForward == 0) {
		while(mnWindowIsClosedStub((s32)window) == 0) {
			sySynch(1);
			frames++;
			if(frames > 600 ||
			   ((InputWork.held & KEY_SELECT) && (InputWork.held & KEY_R) &&
				(InputWork.held & KEY_L) && (InputWork.held & KEY_A)))
				mnMessageCloseFinishedWindowsStub();
		}
	}

	fdMessageSetVoiceFromActor(MESSAGE_VOICE_NONE);
	sySynch(1);
}

/* The event command "show message and wait": shows the message, then waits the
 */
/* given number of frames. */
void fdEventShowMessageThenWait(s32 speaker, s32 flags, s32 frames) {
	fdMessageShowAtActorAndWait(speaker, flags);
	fdEventWait(frames);
}

/* The event command "show message": opens the current message at the speaker */
/* (fdMessageShowAtActor) and waits until it has been read and its window */
/* has closed; the speaker's portrait is closed once the text is typed out. */
void fdEventShowMessageAndWait(s32 speaker, s32 flags) {
	fdMessageShowAtActorAndWait(speaker, flags);
}

/* The event command "ask yes or no": opens the question at the speaker, lets */
/* the leader answer, then shows the next message for yes or the one after it */
/* for no. */
s32 fdEventAskYesNoAndShowResponse(s32 speaker, s32 flags) {
	s32 result;

	fdMessageShowAtActor(speaker, flags);
	result = fdEventAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0);
	if(result == 0) {
		fdMessageShowAtActorAndWait(speaker, flags);
		FieldStateWork->msg++;
	}
	else {
		FieldStateWork->msg++;
		fdMessageShowAtActorAndWait(speaker, flags);
	}
	return result;
}

/* An empty hook; it does nothing in the released game. */
void fdEventNoOpUpdate(void) {}

/* fdEventShowSpeakerMessage and fdEventShowMessageAtPos move a position */
/* MESSAGE_POSITION_SHIFT away from the middle row of a MESSAGE_POSITION_WIDTH
 */
/* by MESSAGE_POSITION_HEIGHT area, larger than the 240 by 160 screen, and hold
 */
/* it MESSAGE_POSITION_MARGIN_X and MESSAGE_POSITION_MARGIN_Y inside the area.
 */
#define MESSAGE_POSITION_WIDTH	  320
#define MESSAGE_POSITION_HEIGHT	  240
#define MESSAGE_POSITION_MARGIN_X 8
#define MESSAGE_POSITION_MARGIN_Y 20
#define MESSAGE_POSITION_SHIFT	  32

/* Unused: nothing calls it. It would record the speaker, open the message near
 */
/* it and wait unless fast-forwarding, then advance the message index. It */
/* places the message as fdEventShowMessageAtPos does, before passing the */
/* position to a window function that takes tiles; with no caller, the units it
 */
/* expected can't be told. */
void fdEventShowSpeakerMessage(s32 speaker) {
	/* BUG: the lookup's result is discarded, and pos is left unset. */
	struct {
		s32 x, y;
	} pos;
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	speaker &= SPEAKER_ACTOR_MASK;
	fdActorLookup(speaker);
	state->speaker = speaker;
	if(state->fastForward == 0) {
		s32 windowX = pos.x;
		s32 windowY = pos.y;
		s32 window;

		if(windowY > MESSAGE_POSITION_HEIGHT / 2 - 1)
			windowY += MESSAGE_POSITION_SHIFT;
		else
			windowY -= MESSAGE_POSITION_SHIFT;
		windowX = windowX < MESSAGE_POSITION_MARGIN_X
					  ? MESSAGE_POSITION_MARGIN_X
					  : windowX;
		if(windowX > MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X)
			windowX = MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X;
		windowY = windowY < MESSAGE_POSITION_MARGIN_Y
					  ? MESSAGE_POSITION_MARGIN_Y
					  : windowY;
		if(windowY > MESSAGE_POSITION_HEIGHT - MESSAGE_POSITION_MARGIN_Y)
			windowY = MESSAGE_POSITION_HEIGHT - MESSAGE_POSITION_MARGIN_Y;
		window = mnMessageOpenWindowStub((s16)state->msg, windowX, windowY,
										 MESSAGE_WINDOW_ZOOM);
		state->msgWin = window;
		while(mnWindowIsClosedStub(window) == 0)
			sySynch(1);
	}
	state->msg++;
}

/* Unused: nothing calls it; forwards its arguments to */
/* fdEventShowMessageAtPos. */
void fdEventForwardShowMessageAtPos(s32 unusedA, s32 unusedB, s32 x, s32 y) {
	fdEventShowMessageAtPos(unusedA, unusedB, x, y);
}

/* Unused: nothing calls it. It would open the event's message at (x, y), wait
 */
/* for it and advance the message index; its units are as unknown as */
/* fdEventShowSpeakerMessage's. */
void fdEventShowMessageAtPos(s32 unusedA, s32 unusedB, s32 x, s32 y) {
	FIELD_STATE *state = FieldStateWork;
	s32 windowY = y;
	s32 windowX = x;
	s32 minX = MESSAGE_POSITION_MARGIN_X;
	s32 minY = MESSAGE_POSITION_MARGIN_Y;
	s32 window;

	if(windowY > MESSAGE_POSITION_HEIGHT / 2 - 1)
		windowY += MESSAGE_POSITION_SHIFT;
	else
		windowY -= MESSAGE_POSITION_SHIFT;

	if(x < minX)
		windowX = minX;
	if(windowX > MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X)
		windowX = MESSAGE_POSITION_WIDTH - MESSAGE_POSITION_MARGIN_X;
	CLAMP(windowY, minY, MESSAGE_POSITION_HEIGHT - MESSAGE_POSITION_MARGIN_Y);

	window = (s32)mnMessageOpenWindowStub(state->msg, windowX, windowY,
										  MESSAGE_WINDOW_ZOOM);
	while(mnWindowIsClosedStub((WINDOW *)window) == 0)
		sySynch(1);
	state->msg++;
}

/* Closes the portrait of an actor's sprite, if it has one. */
void fdActorClosePortrait(s32 actorId) {
	s32 sprite;

	sprite = fdActorGetSprById(actorId);
	if(sprite != -1)
		mnPortraitCloseWindowStub(sprite);
}

/* The event command "show two messages and wait": two actors speak at once, */
/* each message at its own (x, y) with its portrait at (portraitX, */
/* portraitY), all in tiles. When delay is nonzero, the second message waits */
/* for the first to finish, checking every delay frames. Waits for A, B, L or */
/* R, then closes both. */
void fdEventShowTwoMessagesAndWait(s32 first, s32 firstX, s32 firstY,
								   s32 firstPortraitX, s32 firstPortraitY,
								   s32 second, s32 secondX, s32 secondY,
								   s32 secondPortraitX, s32 secondPortraitY,
								   s32 delay) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	s32 firstSprite = fdActorGetSprById(first);
	s32 secondSprite = fdActorGetSprById(second);
	s32 firstWindow;
	s32 secondWindow;

	firstWindow = mnMessageOpenWindowStub(
		(s16)event->msg++, firstX, firstY,
		MESSAGE_WINDOW_VOICE(fdSpeakerGetVoice(firstSprite)));
	mnPortraitOpenWindowStub(firstSprite, 0, firstPortraitX, firstPortraitY);
	if(delay != 0) {
		while(!mnMessageAllTypedStub())
			sySynch(delay);
	}

	secondWindow = mnMessageOpenWindowStub(
		(s16)event->msg++, secondX, secondY,
		MESSAGE_WINDOW_VOICE(fdSpeakerGetVoice(secondSprite)));
	mnPortraitOpenWindowStub(secondSprite, 0, secondPortraitX, secondPortraitY);

	while(mnMessageAllTypedStub() == 0)
		sySynch(1);

	sySynch(1);
	while((InputWork.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0)
		sySynch(1);

	sySynch(1);
	mnPortraitCloseWindowStub(firstSprite);
	mnPortraitCloseWindowStub(secondSprite);
	mnMessageCloseFinishedWindowsStub();
	sySynch(1);

	while(mnWindowIsClosedStub((WINDOW *)firstWindow) == 0)
		sySynch(1);
	while(mnWindowIsClosedStub((WINDOW *)secondWindow) == 0)
		sySynch(1);

	sySynch(1);
}

extern const u8 MessageSpeed_VoiceRates[];

/* Sets the voice of the messages that follow to that of actor key's sprite, at
 */
/* the rate the message speed option picks (MessageSpeed_VoiceRates); */
/* MESSAGE_VOICE_NONE clears it. */
void fdMessageSetVoiceFromActor(s32 key) {
	WIN_STATE *state = (WIN_STATE *)Ram_HeapSlots->winState;
	u8 value;
	s32 id;

	if(key == MESSAGE_VOICE_NONE) {
		*(s16 *)&state->speakerVoiceSnd = (value = 0);
		*(s16 *)&state->speakerVoiceCountdown = value;
	}
	else {
		id = fdSpeakerGetVoice(fdActorGetSprById(key));
		value = MessageSpeed_VoiceRates[MainRoutineStateWork.msgSpeed];
		*(s16 *)&state->speakerVoiceSnd = id;
		*(s16 *)&state->speakerVoiceCountdown = value;
	}
}

/* The event command "camera follow actor": the camera follows the actor, put */
/* on it at once unless keepPosition leaves it where it is to set out from. */
void fdCameraFollowActor(s32 actorId, s32 keepPosition) {
	ACTOR *actor;
	ACTOR *target;
	FIELD_STATE *state;
	MAP_STATE *map;

	actor = fdActorLookup(actorId);
	state = Ram_HeapSlots->fieldState;
	target = state->camera;
	map = Ram_HeapSlots->mapGrid;
	if(actor != 0) {
		map->origin = &target->x.fixed;
		fdActorStartCameraStub(target, actor);
		if(keepPosition == 0) {
			SET_ACTOR_POSITION(target, actor->x.fixed, actor->y.fixed,
							   actor->z.fixed);
			sySynch(1);
			if(state->mode != ACTOR_SYSTEM_WORLD_MAP)
				fdMapGridCenterOnFollowedPointStub();
		}
	}
}

/* The event command "camera set speed": the camera's top speed and */
/* acceleration, 16.16 pixels per frame. */
void fdCameraSetSpeedParms(FIXED speed, FIXED acceleration) {
	FIELD_STATE *state =
		syHeapSlotAllocEwram(HEAP_SLOT_FIELD, sizeof(FIELD_STATE));

	state->camera->speedLimit = speed;
	state->camera->acceleration = acceleration;
}
#undef MESSAGE_VOICE_NONE
#undef MESSAGE_POSITION_WIDTH
#undef MESSAGE_POSITION_HEIGHT
#undef MESSAGE_POSITION_MARGIN_X
#undef MESSAGE_POSITION_MARGIN_Y
#undef MESSAGE_POSITION_SHIFT
