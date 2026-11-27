/* @file */
/* Message strings and arguments, symbol decoding and portrait windows. */
/* Part of window.c. */

/* A capacity no message reaches: the largest s32. */
#define MESSAGE_CAPACITY_UNLIMITED 0x7fffffff

/* The previous code's low byte, which picks a symbol's tree in its bank. */
#define SYMBOL_TREE_MASK 0xff
/* A byte of the bit stream as it is read from: the first bit taken, the seven
 */
/* left, and a stop bit above them that reads as the last 1 with nothing left,
 */
/* when the next byte is loaded. */
#define BIT_STREAM_STOP (1 << 7)

/* Decodes message into text (a u16 string) with no length limit: text must */
/* have room for the whole message and its terminator. */
void mnMessageDecodeRawUnbounded(s32 message, u16 *text) {
	mnMessageDecodeRaw(message, text, MESSAGE_CAPACITY_UNLIMITED);
}

/* Decodes message into destination as a zero-terminated string of at most */
/* capacity entries, and returns its length. capacity counts halfwords */
/* including the terminator and must be at least 1. */
s32 mnMessageCopyExpanded(s32 message, u16 *destination, u32 capacity) {
	WIN_STATE *state;
	u32 count;

	state = WINDOW_STATE;
	state->textBufEnd = 0;
	mnMessageDecode(message, 1);
	for(count = 0; count < capacity - 1 &&
				   (destination[count] = state->textBuf[count]) != 0;
		count++) {
	}
	destination[count] = 0;
	return count;
}

/* Defined in the German, Spanish, French and Italian editions, whose raw */
/* decoding takes no delta off the operands. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#define DECODE_EUROPEAN
#endif

/* Decodes message into text as a zero-terminated string of 16-bit codes, with
 */
/* at most capacity entries, keeping style codes with their operands. */
void mnMessageDecodeRaw(s32 message, u16 *text, s32 capacity) {
	MESSAGE_READER reader;
	void *original;
	s32 (*decode)(MESSAGE_READER *);
	u32 value;
#if !defined(DECODE_EUROPEAN)
	u16 delta;
#endif

	/* The decoder may already be in HEAP_SLOT_MESSAGE_DECODER, as it is while
	 */
	/* mnMessageDecode decodes the names this is called for; then it is */
	/* borrowed, and freed only if loaded here. */
	original = ((HEAP_STATE *)&HeapSlots)->slots[HEAP_SLOT_MESSAGE_DECODER];
	if(original == NULL)
		syRamCodeLoadSlot(HEAP_SLOT_MESSAGE_DECODER, Text_DecodeSymbol,
						  Text_DecodeSymbolCodeSize);
	decode = (s32 (*)(MESSAGE_READER *))((HEAP_STATE *)&HeapSlots)
				 ->slots[HEAP_SLOT_MESSAGE_DECODER];
	mnMessageFind(&reader, message);
#if !defined(DECODE_EUROPEAN)
	delta = (u16)-1;
#endif
	/* Operands are stored one above their value; adding (u16)-1 takes the one
	 * off */
	/* in 16 bits. */
	while((value = decode(&reader)) != 0) {
		switch(value) {
		case MESSAGE_UNUSED_14:
			capacity -= 3;
			if(capacity <= 0)
				goto done;
			*text++ = value;
#if defined(DECODE_EUROPEAN)
			value = decode(&reader) + (u16)-1;
			*text++ = value;
			value = decode(&reader) + (u16)-1;
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
			value = decode(&reader) + (u16)-1;
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
UNUSED s32 *mnMemoryClearWordIfSet(s32 *p) {
	if(*p != 0) {
		*p = 0;
	}
	return p;
}

/* Closing the messages' windows, and the message argument table. */

/* Closes every window: first the messages' windows, waiting until they have */
/* closed, then the windows opened directly, which it does not wait for. */
void mnWindowCloseAll(s32 unused) {
	WIN_STATE *state;
	MESSAGE *message;
	WINDOW *directWindow;
	u32 done;
	WINDOW *window;
	WINDOW *pollWindow;
	s32 i;
	u16 flag;

	state = WINDOW_STATE;
	message = state->msgs;
	directWindow = state->wins;
	i = 0;
	do {
		window = message->win;
		if(window != 0 && window->flg != 0)
			mnWindowClose(window, 0);
		i++;
		message++;
	} while(i != ARRAY_COUNT(state->msgs));

	/* Free each message whose window has closed; while one is still closing, */
	/* wait a frame. */
	for(;;) {
		done = 1;
		message = state->msgs;
		i = 0;
		do {
			pollWindow = message->win;
			if(pollWindow != 0) {
				if(pollWindow->zoomStage == 0 && pollWindow->duration == 0) {
					flag = pollWindow->flg;
					/* flag is 0 here: the message is set free. */
					if(flag == 0)
						message->win = (WINDOW *)(u32)flag;
					else
						done = 0;
				}
				else {
					done = 0;
				}
			}
			i++;
			message++;
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
		if(directWindow->flg != 0)
			mnWindowClose(directWindow, 0);
		directWindow++;
		i++;
	}
}

/* Steps a message's window animation (WINDOW_ANIMATION_SWELL): the frame is */
/* drawn a tile bigger on every side until the countdown ends, then back to */
/* normal. */
void mnWindowStepSwellEffect(MESSAGE *message) {
	WINDOW *window = message->win;
	s32 effect = window->anm;
	s32 x = window->x;
	s32 y = window->y;
	s32 width = window->width;
	s32 height = window->height;

	if(effect != WINDOW_ANIMATION_SWELL)
		return;

	mnWindowDrawBorder(x - 1, y - 1, width + 2, height + 2);
	message->countdown--;
	if(message->countdown != 0)
		return;

	message->win->anm = 0;
	mnWindowEraseBorderRect(x - 1, y - 1, width + 2, height + 2);
	mnWindowDrawBorder(x, y, width, height);
}

/* Empties the table of message arguments. */
void mnMessageArgumentClearAll(void) {
	s32 i;
	WIN_STATE *state;

	state = WINDOW_STATE;

	for(i = 0; i != MESSAGE_ARGUMENT_COUNT; i++) {
		state->argumentValues[i] = 0;
		state->argumentKinds[i] = MESSAGE_ARGUMENT_NONE;
	}
}

/* Adds a message argument of kind (MESSAGE_ARGUMENT_) with value to the first
 */
/* free place of the argument table; when all eight are in use it is dropped. */
void mnMessageArgumentPush(u32 value, u32 kind) {
	WIN_STATE *state = WINDOW_STATE;
	u32 i = 0;
	u32 limit = MESSAGE_ARGUMENT_COUNT;

	do {
		if(state->argumentKinds[i] == MESSAGE_ARGUMENT_NONE) {
			state->argumentValues[i] = value;
			state->argumentKinds[i] = kind;
			break;
		}
		i++;
	} while(i != limit);
}

/* Returns the value of the first message argument of kind (MESSAGE_ARGUMENT_),
 */
/* or 0 when there is none. */
u32 mnMessageArgumentLookup(u32 kind, u32 clear) {
	WIN_STATE *state = WINDOW_STATE;
	u32 i;
	u32 value = 0;

	for(i = 0; i < MESSAGE_ARGUMENT_COUNT; i++) {
		if(state->argumentKinds[i] == kind) {
			value = state->argumentValues[i];
			if(clear != 0) {
				state->argumentValues[i] = 0;
				state->argumentKinds[i] = MESSAGE_ARGUMENT_NONE;
			}
			break;
		}
	}
	return value;
}

/* Ends a message's pause early, returning 1: when the message waits on a */
/* sound that has finished, or while A, B, L or R is held. */
s32 mnTextCheckPauseSkipped(MESSAGE *message) {
	WIN_STATE *state = WINDOW_STATE;
	s32 cancelKeys = 0;

	if(state->waitingForSnd != 0 && sySndGetBgmRestoreDelayStub() == 0)
		cancelKeys = 1;
	if(KeysHeld & (KEY_A | KEY_B | KEYS_SHOULDERS))
		cancelKeys = 1;
	if(cancelKeys != 0) {
		message->countdown = 0;
		return 1;
	}
	return 0;
}

/* As mnTextCheckPauseSkipped, but on a newly pressed button: the latched */
/* presses while WIN_STATE.mindRead is set. */
s32 mnTextCheckTextDismissed(MESSAGE *message) {
	WIN_STATE *state = WINDOW_STATE;
	s32 pressedKeys;
	s32 cancelKeys = 0;

	if(state->waitingForSnd != 0 && sySndGetBgmRestoreDelayStub() == 0)
		cancelKeys = 1;
	pressedKeys = KeysPressed;
	if(state->mindRead != 0)
		pressedKeys = KeysPressedLatch;
	if(pressedKeys & (KEY_A | KEY_B | KEYS_SHOULDERS))
		cancelKeys = 1;
	if(cancelKeys != 0) {
		message->countdown = 0;
		return 1;
	}
	return 0;
}

/* Closes each message's window whose text has ended, then waits ten frames. */
void mnMessageCloseFinishedWindows(void) {
	s32 i;
	MESSAGE *message;
	WINDOW *window;

	message = WINDOW_STATE->msgs;
	i = 0;
	do {
		window = message->win;
		if(window != NULL && window->zoomStage == 0 && window->duration == 0 &&
		   window->flg != 0 && window->state != WINDOW_TYPING) {
			mnWindowClose(window, WINDOW_FLAG_INSTANT & window->flg);
		}
		i++;
		message++;
	} while(i != ARRAY_COUNT(WINDOW_STATE->msgs));
	sySynch(10);
}

/* Shows a message centred across the screen and in its top 15 rows (yOffset */
/* tiles lower), waits until it is typed out and closes it, in the window mode
 */
/* (CENTERED_MESSAGE_MODE) asks for. */
void mnFieldMessageShowCentered(s32 message, s32 mode, s32 yOffset) {
	WIN_STATE *state;
	WINDOW *messageWindow;
	s32 x;
	s32 y;
	s32 width;
	s32 height;
	s32 entryIndex;

	state = WINDOW_STATE;
	x = 8;
	y = 8;
	messageWindow = NULL;
	entryIndex = mnMessageDecode(message, 1);
	if(state->textBuf[entryIndex] != 0) {
		mnTextGetMessageSize(message, &x, &y, (u32 *)&width, (u32 *)&height);
		x = (DISPLAY_TILE_WIDTH - width) >> 1;
		y = ((15 - height) >> 1) + yOffset;
		if(mode != CENTERED_MESSAGE_BARE)
			messageWindow = mnWindowCreate(x, y, width, height, 0);
		else {
			/* mode, CENTERED_MESSAGE_BARE, is 0: the window has no size. */
			messageWindow = mnWindowCreate(x, y, 0, 0, WINDOW_FLAG_INSTANT);
			messageWindow->width = mode;
			messageWindow->height = mode;
		}
		if(mnMessageStart(messageWindow, entryIndex, 0, 0, 0, 0) == 0)
			mnWindowClose(messageWindow, 1);
		else {
			while(mnMessageAllTyped() == 0)
				sySynch(1);
			if(mode != CENTERED_MESSAGE_BARE) {
				mnWindowClose(messageWindow, 0);
				while(mnWindowIsClosed(messageWindow) == 0)
					sySynch(1);
			}
			else
				mnWindowClose(messageWindow, 1);
			state->voiceSnd = 0;
			state->voiceCountdown = 0;
		}
	}
}

/* Decodes message and empties the argument table; returns where its */
/* decoded text starts. The Djinn and party join messages */
/* (screens_joined.c) use it. */
s32 mnMessageDecodeAndClearArguments(s32 message) {
	return mnMessageDecode(message, 1);
}

/* Points reader at the start of message (its compressed bits and first */
/* context), running the ARM lookup function from a RAM copy, which it frees */
/* before returning. */
void mnMessageFind(MESSAGE_READER *reader, s32 message) {
	void (*routine)(MESSAGE_READER *, s32);

	syRamCodeLoad(routine, mnMessageFindArm, mnMessageFindCodeSize);
	routine(reader, message);
	syHeapRelease(routine);
}

/* Unused: decodes the next symbol of a message, as Text_DecodeSymbol, the */
/* copy the game runs from RAM, does. */
UNUSED s32 mnTextDecodeSymbolInC(MESSAGE_READER *reader) {
	u32 code;
	u32 contextBank;
	const u8 *base;
	const u8 *tree;
	const u8 *leaves;
	const u8 *text;
	s32 textBits;
	s32 treeBits;
	s32 rank;
	s32 depth;
	s32 bit;
	u32 index;
	u32 leafNibble;

	code = reader->prevCode;
	text = (const u8 *)reader->words;
	textBits = reader->bits;
	treeBits = 1;
	/* The previous code's high bits pick the bank, its low byte the tree. */
	contextBank = code >> 8;
	code &= SYMBOL_TREE_MASK;
	base = Text_MessageContexts[contextBank][0];
	base += ((const u16 *)Text_MessageContexts[contextBank][1])[code];
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
	/* The leaf's symbol is the rank-th 12-bit entry, three nibbles each, */
	/* packed backwards from base - 1: an even rank starts on a byte, an odd one
	 */
	/* in the middle of one. */
	index = rank * 3;
	if(((index * 4) & 7) == 0) {
		bit = leaves[-(index >> 1)];
		code = bit << 4;
		bit = leaves[-(index >> 1) - 1];
		code |= (u32)bit >> 4;
	}
	else {
		leafNibble = leaves[-(index >> 1)];
		leafNibble &= 0xf;
		code = leaves[-(index >> 1) - 1] | (leafNibble << 8);
	}
	reader->bits = textBits;
	reader->prevCode = code;
	reader->words = (const u32 *)text;
	return code;
}

/* Marks both portrait slots free (PORTRAIT_SLOT_EMPTY). Only the field */
/* engine's setup calls it. */
void mnPortraitClearSlots(void) {
	WIN_STATE *state = WINDOW_STATE;
	s32 index;

	for(index = 0; index != ARRAY_COUNT(state->portraitPictures); index++) {
		state->portraitPictures[index] = PORTRAIT_SLOT_EMPTY;
	}
}

/* Returns the portrait of a character or another speaker's sprite, or -1 when
 */
/* it has none: speakers up to Garet's child sprite take theirs from */
/* Portrait_CharacterMap, the others from Portrait_SpriteMap, numbered from */
/* PORTRAIT_SPRITE_MAP_FIRST (mnIconLoadWithPalette takes it as the row */
/* PORTRAIT_JUPITER_DJINNI). */
s32 mnPortraitGetForSpeaker(u32 speaker) {
	s32 result = -1;
	s32 portraitIndex = 0;

	/* Both maps are {speaker, portrait} pairs ending in -1 (window/scales.c).
	 */
	if(speaker <= SPRITE_GARET_CHILD) {
		for(;;) {
			s32 speakerId = Portrait_CharacterMap[portraitIndex];

			if(speakerId == -1)
				break;
			if(speakerId == speaker) {
				portraitIndex++;
				result = Portrait_CharacterMap[portraitIndex];
				break;
			}
			portraitIndex += 2;
		}
	}
	else {
		for(;;) {
			s32 speakerId = Portrait_SpriteMap[portraitIndex];

			if(speakerId == -1)
				break;
			if(speakerId == speaker) {
				portraitIndex++;
				result = Portrait_SpriteMap[portraitIndex];
				result += PORTRAIT_SPRITE_MAP_FIRST;
				break;
			}
			portraitIndex += 2;
		}
	}
	return result;
}

/* Opens a portrait window at tile (x, y) showing the speaker's portrait. */
/* portraitOption is passed on through mnPortraitCreate to */
/* mnIconLoadWithPalette, which ignores it. */
WINDOW *mnPortraitOpenWindow(s32 speaker, s32 portraitOption, s32 x, s32 y) {
	WINDOW *window;
	void *state;

	state = WindowStateWork[0];
	if(mnPortraitGetForSpeaker(speaker) == -1) {
		return 0;
	}
	{
		s32 portraitX = -4;
		s32 portraitY = -4;

		if(((WIN_STATE *)state)->mindRead != 0) {
			window = mnWindowCreate(x, y, 6, 5, WINDOW_FLAG_INSTANT);
			portraitX = 0;
		}
		else {
			window = mnWindowCreate(x, y, 5, 5, WINDOW_FLAG_INSTANT);
		}
		if(window != 0) {
			mnPortraitCreate(speaker, portraitOption, -1, window, portraitX,
							 portraitY);
		}
	}
	return window;
}

/* Closes the window showing a speaker's portrait, if one is open, at once: the
 */
/* portrait slot showing it names its window sprite, the first sprite of its */
/* window. */
void mnPortraitCloseWindow(u32 speaker) {
	WIN_STATE *state;
	WINDOW *window;
	s32 resourceId;
	s32 portraitSlot;
	s32 windowIndex;
	WINDOW_SPR *portraitSprite;

	state = WINDOW_STATE;
	window = state->wins;
	resourceId = mnPortraitGetForSpeaker(speaker);
	if(resourceId == -1)
		return;

	if(state->portraitPictures[1] == resourceId) {
		portraitSlot = 1;
	}
	else if(state->portraitPictures[0] == resourceId) {
		portraitSlot = 0;
	}
	else {
		return;
	}

	/* From here resourceId is the slot's window sprite. */
	resourceId = state->portraitSprs[portraitSlot];

	for(windowIndex = 0; windowIndex != WINDOW_COUNT; windowIndex++, window++) {
		portraitSprite = window->sprs.head;
#ifdef UBFIX
		/* mnWindowCreate and the closing update leave empty heads NULL. Skip
		 * them */
		/* before reading kind: dereferencing that NULL pointer is undefined. */
		if(portraitSprite == NULL)
			continue;
#endif
		if(portraitSprite->kind == WINDOW_SPRITE_PORTRAIT &&
		   portraitSprite->idx == resourceId) {
			mnWindowClose(window, WINDOW_FLAG_INSTANT);
			return;
		}
	}
}
