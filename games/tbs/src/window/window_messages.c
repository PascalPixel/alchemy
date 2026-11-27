/* Message strings and arguments, symbol decoding and portrait windows. Part of
   window.c. */

/* A capacity no message reaches: the largest Sint32. */
#define MESSAGE_CAPACITY_UNLIMITED 0x7fffffff

/* The previous code's low byte, which picks a symbol's tree in its bank. */
#define SYMBOL_TREE_MASK 0xff
/* A byte of the bit stream as it is read from: the first bit taken, the seven
   left, and a stop bit above them that reads as the last 1 with nothing left,
   when the next byte is loaded. */
#define BIT_STREAM_STOP	 (1 << 7)

#include "window/message_decode_raw.c"

/* Defined in the German, Spanish, French and Italian editions, whose raw
   decoding takes no delta off the operands. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH || \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#define DECODE_EUROPEAN
#endif

/* Decodes message into text as a zero-terminated string of 16-bit codes, with
   at most capacity entries, keeping style codes with their operands. */
void mnMessageDecodeRaw(Sint32 msg, Uint16 *text, Sint32 capacity)
{
	MESSAGE_READER reader;
	void		  *original;
	Sint32 (*decode)(MESSAGE_READER *);
	Uint32 value;
#if !defined(DECODE_EUROPEAN)
	Uint16 delta;
#endif

	/* The decoder may already be in HEAP_SLOT_MESSAGE_DECODER, as it is while
	   mnMessageDecode decodes the names this is called for; then it is
	   borrowed, and freed only if loaded here. */
	original = ((HEAP_STATE *)&HeapSlots)->slots[HEAP_SLOT_MESSAGE_DECODER];
	if(original == NULL)
		RAM_CODE_LOAD_SLOT(HEAP_SLOT_MESSAGE_DECODER, TextDecodeSymbol,
			TextDecodeSymbolCodeSize);
	decode = (Sint32 (*)(MESSAGE_READER *))((HEAP_STATE *)&HeapSlots)
				 ->slots[HEAP_SLOT_MESSAGE_DECODER];
	mnMessageFind(&reader, msg);
#if !defined(DECODE_EUROPEAN)
	delta = (Uint16)-1;
#endif
	/* Operands are stored one above their value; adding (Uint16)-1 takes the
	   one off in 16 bits. */
	while((value = decode(&reader)) != 0) {
		switch(value) {
		case MESSAGE_UNUSED_14:
			capacity -= 3;
			if(capacity <= 0)
				goto done;
			*text++ = value;
#if defined(DECODE_EUROPEAN)
			value = decode(&reader) + (Uint16)-1;
			*text++ = value;
			value = decode(&reader) + (Uint16)-1;
#else
			*text++ = decode(&reader) + delta;
			value = decode(&reader) + delta;
#endif
			break;
		case MESSAGE_TEXT_COLOR:
		case MESSAGE_TEXT_EFFECT:
		case MESSAGE_LINE_SPACING:
		case MESSAGE_UNUSED_11:
		case MESSAGE_UNUSED_12:
		case MESSAGE_WINDOW_EFFECT:
#if defined(DECODE_EUROPEAN)
		case MESSAGE_ARTICLE_CLASS:
#endif
			capacity--;
			if(capacity <= 0)
				goto done;
			*text++ = value;
			value = decode(&reader) + (Uint16)-1;
			break;
		default:
			capacity--;
			if(capacity <= 0)
				goto done;
			break;
		}
		*text++ = value;
	}
done:
	if(original == NULL)
		syHeapSlotRelease(HEAP_SLOT_MESSAGE_DECODER);
	*text = 0;
}

/* Unused: zeroes the word at p unless it is already 0, and returns p. */
UNUSED Sint32 *mnMemoryClearWordIfSet(Sint32 *p)
{
	if(*p != 0) {
		*p = 0;
	}
	return p;
}

/* Closing the messages' windows, and the message argument table. */

/* Closes every window: first the messages' windows, waiting until they have
   closed, then the windows opened directly, which it does not wait for. */
void mnWindowCloseAll(Sint32 unused)
{
	WIN_STATE *state;
	MESSAGE	  *msg;
	WINDOW	  *directWin;
	Uint32	   done;
	WINDOW	  *win;
	WINDOW	  *pollWin;
	Sint32	   i;
	Uint16	   flg;

	state = WINDOW_STATE;
	msg = state->msgs;
	directWin = state->wins;
	i = 0;
	do {
		win = msg->win;
		if(win != 0 && win->flg != 0)
			mnWindowClose(win, 0);
		i++;
		msg++;
	} while(i != ARRAY_COUNT(state->msgs));

	/* Free each message whose window has closed; while one is still closing,
	   wait a tick. */
	for(;;) {
		done = 1;
		msg = state->msgs;
		i = 0;
		do {
			pollWin = msg->win;
			if(pollWin != 0) {
				if(pollWin->zoomStage == 0 && pollWin->duration == 0) {
					flg = pollWin->flg;
					/* flag is 0 here: the message is set free. */
					if(flg == 0)
						msg->win = (WINDOW *)(Uint32)flg;
					else
						done = 0;
				} else {
					done = 0;
				}
			}
			i++;
			msg++;
		} while(i != ARRAY_COUNT(state->msgs));
		i = 0;
		if(done)
			break;
		sySynch(1);
	}

	/* Then the windows opened directly. */
	for(;;) {
		if(i == WINDOW_COUNT)
			break;
		if(directWin->flg != 0)
			mnWindowClose(directWin, 0);
		directWin++;
		i++;
	}
}

/* Steps a message's window animation (WINDOW_ANIMATION_SWELL): the tick is
   drawn a tile bigger on every side until the countdown ends, then back to
   normal. */
void mnWindowStepSwellEffect(MESSAGE *msg)
{
	WINDOW *win = msg->win;
	Sint32	effect = win->anm;
	Sint32	x = win->x;
	Sint32	y = win->y;
	Sint32	width = win->width;
	Sint32	height = win->height;

	if(effect != WINDOW_ANIMATION_SWELL)
		return;

	mnWindowDrawBorder(x - 1, y - 1, width + 2, height + 2);
	msg->countdown--;
	if(msg->countdown != 0)
		return;

	msg->win->anm = 0;
	mnWindowEraseBorderRect(x - 1, y - 1, width + 2, height + 2);
	mnWindowDrawBorder(x, y, width, height);
}

/* Empties the table of message arguments. */
void mnMessageArgumentClearAll(void)
{
	Sint32	   i;
	WIN_STATE *state;

	state = WINDOW_STATE;

	for(i = 0; i != MESSAGE_ARGUMENT_COUNT; i++) {
		state->argumentValues[i] = 0;
		state->argumentKinds[i] = MESSAGE_ARGUMENT_NONE;
	}
}

/* Adds a message argument of kind (MESSAGE_ARGUMENT_) with value to the first
   free place of the argument table; when all eight are in use it is dropped. */
void mnMessageArgumentPush(Uint32 value, Uint32 kind)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint32	   i = 0;
	Uint32	   limit = MESSAGE_ARGUMENT_COUNT;

	do {
		if(state->argumentKinds[i] == MESSAGE_ARGUMENT_NONE) {
			state->argumentValues[i] = value;
			state->argumentKinds[i] = kind;
			break;
		}
		i++;
	} while(i != limit);
}

#include "window/message_argument_lookup.c"

/* Ends a message's pause early, returning 1: when the message waits on a sound
   that has finished, or while A, B, L or R is held. */
Bool mnTextCheckPauseSkipped(MESSAGE *msg)
{
	WIN_STATE *state = WINDOW_STATE;
	Bool	   cancelKeys = FALSE;

	if(state->waitingForSnd != 0 && sySndGetBGMRestoreDelayStub() == 0)
		cancelKeys = TRUE;
	if(PerData & (KEY_A | KEY_B | KEYS_SHOULDERS))
		cancelKeys = TRUE;
	if(cancelKeys != 0) {
		msg->countdown = 0;
		return TRUE;
	}
	return FALSE;
}

/* As mnTextCheckPauseSkipped, but on a newly pressed button: the latched
   presses while WIN_STATE.mindRead is set. */
Bool mnTextCheckTextDismissed(MESSAGE *msg)
{
	WIN_STATE *state = WINDOW_STATE;
	Sint32	   pressedKeys;
	Bool	   cancelKeys = FALSE;

	if(state->waitingForSnd != 0 && sySndGetBGMRestoreDelayStub() == 0)
		cancelKeys = TRUE;
	pressedKeys = PerPush;
	if(state->mindRead != 0)
		pressedKeys = PerPushLatch;
	if(pressedKeys & (KEY_A | KEY_B | KEYS_SHOULDERS))
		cancelKeys = TRUE;
	if(cancelKeys != 0) {
		msg->countdown = 0;
		return TRUE;
	}
	return FALSE;
}

/* Closes each message's window whose text has ended, then waits ten ticks. */
void mnMessageCloseFinishedWindows(void)
{
	Sint32	 i;
	MESSAGE *msg;
	WINDOW	*win;

	msg = WINDOW_STATE->msgs;
	i = 0;
	do {
		win = msg->win;
		if(win != NULL && win->zoomStage == 0 && win->duration == 0 &&
			win->flg != 0 && win->state != WINDOW_TYPING) {
			mnWindowClose(win, WINDOW_FLAG_INSTANT & win->flg);
		}
		i++;
		msg++;
	} while(i != ARRAY_COUNT(WINDOW_STATE->msgs));
	sySynch(10);
}

/* Shows a message centred across the screen and in its top 15 rows (yOfs
   tiles lower), waits until it is typed out and closes it, in the window mode
   (CENTERED_MESSAGE_MODE) asks for. */
void mnFieldMessageShowCentered(Sint32 msg, Sint32 mode, Sint32 yOfs)
{
	WIN_STATE *state;
	WINDOW	  *msgWin;
	Sint32	   x;
	Sint32	   y;
	Sint32	   width;
	Sint32	   height;
	Sint32	   entryIdx;

	state = WINDOW_STATE;
	x = 8;
	y = 8;
	msgWin = NULL;
	entryIdx = mnMessageDecode(msg, 1);
	if(state->textBuf[entryIdx] != 0) {
		mnTextGetMessageSize(msg, &x, &y, (Uint32 *)&width, (Uint32 *)&height);
		x = (DISPLAY_TILE_WIDTH - width) >> 1;
		y = ((15 - height) >> 1) + yOfs;
		if(mode != CENTERED_MESSAGE_BARE)
			msgWin = mnWindowCreate(x, y, width, height, 0);
		else {
			/* mode, CENTERED_MESSAGE_BARE, is 0: the window has no size. */
			msgWin = mnWindowCreate(x, y, 0, 0, WINDOW_FLAG_INSTANT);
			msgWin->width = mode;
			msgWin->height = mode;
		}
		if(mnMessageStart(msgWin, entryIdx, 0, 0, 0, 0) == 0)
			mnWindowClose(msgWin, 1);
		else {
			while(mnMessageAllTyped() == 0)
				sySynch(1);
			if(mode != CENTERED_MESSAGE_BARE) {
				mnWindowClose(msgWin, 0);
				while(mnWindowIsClosed(msgWin) == 0)
					sySynch(1);
			} else
				mnWindowClose(msgWin, 1);
			state->voiceSnd = 0;
			state->voiceCountdown = 0;
		}
	}
}

#include "window/message_decode_and_clear_arguments.c"

/* Points reader at the start of message (its compressed bits and first
   context), running the ARM lookup function from a RAM copy, which it frees
   before returning. */
void mnMessageFind(MESSAGE_READER *reader, Sint32 msg)
{
	void (*routine)(MESSAGE_READER *, Sint32);

	RAM_CODE_LOAD(routine, mnMessageFindArm, mnMessageFindCodeSize);
	routine(reader, msg);
	syHeapRelease(routine);
}

/* Unused: decodes the next symbol of a message, as TextDecodeSymbol, the copy
   the game runs from RAM, does. */
UNUSED Sint32 mnTextDecodeSymbolInC(MESSAGE_READER *reader)
{
	Uint32		 code;
	Uint32		 contextBank;
	const Uint8 *base;
	const Uint8 *tree;
	const Uint8 *leaves;
	const Uint8 *text;
	Sint32		 textBits;
	Sint32		 treeBits;
	Sint32		 rank;
	Sint32		 depth;
	Sint32		 bit;
	Uint32		 idx;
	Uint32		 leafNibble;

	code = reader->prevCode;
	text = (const Uint8 *)reader->words;
	textBits = reader->bits;
	treeBits = 1;
	/* The previous code's high bits pick the bank, its low byte the tree. */
	contextBank = code >> 8;
	code &= SYMBOL_TREE_MASK;
	base = TextMessageContexts[contextBank][0];
	base += ((const Uint16 *)TextMessageContexts[contextBank][1])[code];
	tree = base;
	leaves = base - 1;
	rank = 0;
	/* The tree is its shape in preorder, a bit a node: 0 a branch, 1 a leaf. */
	for(;;) {
		bit = treeBits & 1;
		treeBits >>= 1;
		if(bit != 0 && treeBits == 0) {
			treeBits = *tree++;
			bit = treeBits & 1;
			treeBits = (treeBits >> 1) | BIT_STREAM_STOP;
		}
		if(bit != 0)
			break;
		bit = textBits & 1;
		textBits >>= 1;
		if(bit != 0 && textBits == 0) {
			textBits = *text++;
			bit = textBits & 1;
			textBits = (textBits >> 1) | BIT_STREAM_STOP;
		}
		if(bit == 0)
			continue;
		depth = 0;
		do {
			bit = treeBits & 1;
			treeBits >>= 1;
			if(bit != 0 && treeBits == 0) {
				treeBits = *tree++;
				bit = treeBits & 1;
				treeBits = (treeBits >> 1) | BIT_STREAM_STOP;
			}
			if(bit == 0)
				depth++;
			else {
				rank++;
				depth--;
			}
		} while(depth >= 0);
	}
	/* The leaf's symbol is the rank-th 12-bit entry, three nibbles each, packed
	   backwards from base - 1: an even rank starts on a byte, an odd one in the
	   middle of one. */
	idx = rank * 3;
	if(((idx * 4) & 7) == 0) {
		bit = leaves[-(idx >> 1)];
		code = bit << 4;
		bit = leaves[-(idx >> 1) - 1];
		code |= (Uint32)bit >> 4;
	} else {
		leafNibble = leaves[-(idx >> 1)];
		leafNibble &= 0xf;
		code = leaves[-(idx >> 1) - 1] | (leafNibble << 8);
	}
	reader->bits = textBits;
	reader->prevCode = code;
	reader->words = (const Uint32 *)text;
	return code;
}

#include "window/portrait_clear_slots.c"

/* Returns the portrait of a character or another speaker's sprite, or -1 when
   it has none: speakers up to Garet's child sprite take theirs from
   PortraitCharacterMap, the others from PortraitSpriteMap, numbered from
   PORTRAIT_SPRITE_MAP_FIRST (mnIconLoadWithPalette takes it as the row
   PORTRAIT_JUPITER_DJINNI). */
Sint32 mnPortraitGetForSpeaker(Uint32 speaker)
{
	Sint32 result = -1;
	Sint32 portraitIdx = 0;

	/* Both maps are {speaker, portrait} pairs ending in -1
	   (window/scales.c). */
	if(speaker <= SPRITE_GARET_CHILD) {
		for(;;) {
			Sint32 speakerId = PortraitCharacterMap[portraitIdx];

			if(speakerId == -1)
				break;
			if(speakerId == speaker) {
				portraitIdx++;
				result = PortraitCharacterMap[portraitIdx];
				break;
			}
			portraitIdx += 2;
		}
	} else {
		for(;;) {
			Sint32 speakerId = PortraitSpriteMap[portraitIdx];

			if(speakerId == -1)
				break;
			if(speakerId == speaker) {
				portraitIdx++;
				result = PortraitSpriteMap[portraitIdx];
				result += PORTRAIT_SPRITE_MAP_FIRST;
				break;
			}
			portraitIdx += 2;
		}
	}
	return result;
}

/* Opens a portrait window at tile (x, y) showing the speaker's portrait.
   portraitOption is passed on through mnPortraitCreate to
   mnIconLoadWithPalette, which ignores it. */
WINDOW *mnPortraitOpenWindow(
	Sint32 speaker, Sint32 portraitOption, Sint32 x, Sint32 y)
{
	WINDOW *win;
	void   *state;

	state = WindowStateWork[0];
	if(mnPortraitGetForSpeaker(speaker) == -1) {
		return 0;
	}
	{
		Sint32 portraitX = -4;
		Sint32 portraitY = -4;

		if(((WIN_STATE *)state)->mindRead != 0) {
			win = mnWindowCreate(x, y, 6, 5, WINDOW_FLAG_INSTANT);
			portraitX = 0;
		} else {
			win = mnWindowCreate(x, y, 5, 5, WINDOW_FLAG_INSTANT);
		}
		if(win != 0) {
			mnPortraitCreate(
				speaker, portraitOption, -1, win, portraitX, portraitY);
		}
	}
	return win;
}

/* Closes the window showing a speaker's portrait, if one is open, at once: the
   portrait slot showing it names its window sprite, the first sprite of its
   window. */
void mnPortraitCloseWindow(Uint32 speaker)
{
	WIN_STATE  *state;
	WINDOW	   *win;
	Sint32		resourceId;
	Sint32		portraitSlot;
	Sint32		winIdx;
	WINDOW_SPR *portraitSpr;

	state = WINDOW_STATE;
	win = state->wins;
	resourceId = mnPortraitGetForSpeaker(speaker);
	if(resourceId == -1)
		return;

	if(state->portraitPictures[1] == resourceId) {
		portraitSlot = 1;
	} else if(state->portraitPictures[0] == resourceId) {
		portraitSlot = 0;
	} else {
		return;
	}

	/* From here resourceId is the slot's window sprite. */
	resourceId = state->portraitSprs[portraitSlot];

	for(winIdx = 0; winIdx != WINDOW_COUNT; winIdx++, win++) {
		portraitSpr = win->sprs.head;
#if FIX_NULL_DEREFERENCE
		/* mnWindowCreate and the closing update leave empty heads NULL. Skip
		   them before reading kind: dereferencing that NULL pointer is
		   undefined. */
		if(portraitSpr == NULL)
			continue;
#endif
		if(portraitSpr->kind == WINDOW_SPRITE_PORTRAIT &&
			portraitSpr->idx == resourceId) {
			mnWindowClose(win, WINDOW_FLAG_INSTANT);
			return;
		}
	}
}
