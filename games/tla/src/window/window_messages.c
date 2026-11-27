/* Messages copied out as strings, the message arguments, closing windows, a
   message's flash, the story crawl's text, the symbol decoder and the portrait
   windows. Part of window.c. */

/* A capacity no message reaches: the largest Sint32. */
#define MESSAGE_CAPACITY_UNLIMITED ((Uint32)-1 >> 1)

/* The previous code's low byte, which picks a symbol's tree in its bank. */
#define SYMBOL_TREE_MASK 0xff
/* A byte of the bit stream as it is read from: the first bit taken, the seven
   left, and a stop bit above them that reads as the last 1 with nothing left,
   when the next byte is loaded. */
#define BIT_STREAM_STOP	 (1 << 7)

/* Decodes message into text (a Uint16 string) with no length limit: text must
   have room for the whole message and its terminator. */
void mnMessageDecodeRawUnbounded(Sint32 msg, Uint16 *text)
{
	mnMessageDecodeRaw(msg, text, MESSAGE_CAPACITY_UNLIMITED);
}

/* Decodes message into destination as a zero-terminated string of at most
   capacity entries, and returns its length. capacity counts halfwords including
   the terminator and must be at least 1. */
Sint32 mnMessageCopyExpanded(Sint32 msg, Uint16 *dst, Uint32 capacity)
{
	WIN_STATE *state;
	Uint32	   cnt;

	state = WINDOW_STATE;
	state->textBufEnd = 0;
	mnMessageDecode(msg, 1);
	for(cnt = 0; cnt < capacity - 1 && (dst[cnt] = state->textBuf[cnt]) != 0;
		cnt++) {
	}
	dst[cnt] = 0;
	return cnt;
}

/* The German, Spanish, French and Italian editions' text: its operands are
   stored biased by OPERAND_UNBIAS, and it marks article classes. */
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) || \
	(LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define DECODE_EUROPEAN
#endif

extern const Uint8 Text_DecodeSymbolCode[];
extern Uint8	   Text_DecodeSymbolCodeSize[];

/* Decodes message into text as a zero-terminated string of 16-bit codes, with
   at most capacity entries, keeping style codes with their operands. */
void mnMessageDecodeRaw(Sint32 msg, Uint16 *text, Sint32 capacity)
{
	TEXT_READER reader;
	void	   *original;
	Sint32 (*decode)(TEXT_READER *);
	Uint32 value;
#if !defined(DECODE_EUROPEAN)
	Uint16 delta;
#endif

	original = Ram_HeapSlots->msgDecoder;
	if(original == NULL)
		RAM_CODE_LOAD_SLOT(HEAP_SLOT_MESSAGE_DECODER, Text_DecodeSymbolCode,
			Text_DecodeSymbolCodeSize);
	decode = (Sint32 (*)(TEXT_READER *))Ram_HeapSlots->msgDecoder;
	mnMessageFind(&reader, msg);
#if !defined(DECODE_EUROPEAN)
	delta = -1;
#endif
	while((value = decode(&reader)) != 0) {
		switch(value) {
		case 14:
			capacity -= 3;
			if(capacity <= 0)
				goto done;
			*text++ = value;
#if defined(DECODE_EUROPEAN)
			value = decode(&reader) + OPERAND_UNBIAS;
			*text++ = value;
			value = decode(&reader) + OPERAND_UNBIAS;
#else
			*text++ = decode(&reader) + delta;
			value = decode(&reader) + delta;
#endif
			break;
		case MESSAGE_TEXT_COLOR:
		case MESSAGE_TEXT_EFFECT:
		case MESSAGE_LINE_SPACING:
		case 11:
		case 12:
		case MESSAGE_WINDOW_EFFECT:
#if defined(DECODE_EUROPEAN)
		case MESSAGE_ARTICLE_CLASS:
#endif
			capacity--;
			if(capacity <= 0)
				goto done;
			*text++ = value;
			value = decode(&reader) + OPERAND_UNBIAS;
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

/* Clears *p if nonzero and returns p. */
Sint32 *mnMemoryClearWordIfSet(Sint32 *p)
{
	if(*p != 0) {
		*p = 0;
	}
	return p;
}

/* Closes every window: first the messages' windows, waiting until they have
   closed, then the windows opened directly, which it does not wait for. */
void mnWindowCloseAll(void)
{
	WIN_STATE *state;
	MESSAGE	  *msg;
	WINDOW	  *win;
	Uint32	   done;
	WINDOW	  *window;
	WINDOW	  *pollWin;
	Sint32	   i;

	state = WINDOW_STATE;
	msg = state->msgs;
	win = state->wins;
	i = 0;
	do {
		window = msg->win;
		if(window != 0 && window->flg != 0)
			mnWindowClose(window, 0);
		i++;
		msg++;
	} while(i != ARRAY_COUNT(state->msgs));

	/* Free each message whose window has closed; while one is still closing,
	   wait a tick. The zoom's stage and duration are read as one word. */
	for(;;) {
		done = 1;
		msg = state->msgs;
		i = 0;
		do {
			pollWin = msg->win;
			if(pollWin != 0) {
				if((*(Sint32 *)&pollWin->zoomStage) == 0) {
					if(pollWin->flg == 0)
						msg->win = NULL;
					else
						done = 0;
				} else {
					done = 0;
				}
			}
			i++;
			msg++;
		} while(i != ARRAY_COUNT(state->msgs));
		if(done)
			break;
		sySynch(1);
	}

	/* Then the windows opened directly. */
	for(i = 0; i != WINDOW_COUNT; i++) {
		if(win->flg != 0)
			mnWindowClose(win, 0);
		win++;
	}
}

/* The flash border's ticks: eight tiles each, shown in tiles 8 to 15. */
#define FLASH_FRAME_BYTES (8 * TILE_SIZE_4BPP)
#define FLASH_FRAME_TILES (BG_CHAR_BLOCK(0) + 8 * TILE_SIZE_4BPP)

/* Loads the flash border frame `ofs` bytes into its picture
   (ResourceId_WindowBorderFlash) into tiles 8 to 15 and draws the window's
   rectangle with it. */
void mnWindowShowFlashFrame(WINDOW **win, Sint32 ofs)
{
	Uint8 *src =
		(Uint8 *)syResourceGetTableEntry(RESOURCE_WINDOW_BORDER_FLASH) + ofs;
	WINDOW *rect = *win;
	Sint32	x = rect->x;
	Sint32	y = rect->y;
	Sint32	width = rect->width;
	Sint32	height = rect->height;

	syDMAXCopy(src, (void *)FLASH_FRAME_TILES,
		DMA_COPY_WORDS(FLASH_FRAME_BYTES / 4), REG_DMA3);
	mnWindowDrawFlashBorder(x, y, width, height);
}

/* Steps a message's window animation (WINDOW_ANIMATION_SWELL): the tick
   flashes through the flash border's pictures as the countdown runs, the plain
   border returns two ticks before its end, and the flash tiles are restored at
   the end. */
void mnWindowStepSwellEffect(MESSAGE *msg)
{
	if(msg->win->anm == WINDOW_ANIMATION_SWELL) {
		if(msg->countdown == 10)
			mnWindowShowFlashFrame(&msg->win, 3 * FLASH_FRAME_BYTES);
		else if(msg->countdown == 9)
			mnWindowShowFlashFrame(&msg->win, 2 * FLASH_FRAME_BYTES);
		else if(msg->countdown == 8)
			mnWindowShowFlashFrame(&msg->win, 1 * FLASH_FRAME_BYTES);
		else if(msg->countdown == 6)
			mnWindowShowFlashFrame(&msg->win, 2 * FLASH_FRAME_BYTES);
		else if(msg->countdown == 4)
			mnWindowShowFlashFrame(&msg->win, 3 * FLASH_FRAME_BYTES);
		if(msg->countdown == 2) {
			WINDOW *win = msg->win;

			mnWindowDrawBorder(win->x, win->y, win->width, win->height);
		}
		msg->countdown--;
		if(msg->countdown == 0) {
			syDMAXCopy(syResourceGetTableEntry(RESOURCE_WINDOW_BORDER_FLASH),
				(void *)FLASH_FRAME_TILES,
				DMA_COPY_WORDS(FLASH_FRAME_BYTES / 4), REG_DMA3);
			msg->win->anm = 0;
		}
	}
}

/* Empties the table of message arguments. */
void mnMessageArgumentClearAll(void)
{
	Sint32	   no;
	WIN_STATE *state;

	state = WINDOW_STATE;

	for(no = 0; no != MESSAGE_ARGUMENT_COUNT; no++) {
		state->argumentValues[no] = 0;
		state->argumentKinds[no] = MESSAGE_ARGUMENT_NONE;
	}
}

/* Adds a message argument of kind (MESSAGE_ARGUMENT_) with value to the first
   free place of the argument table; when all eight are in use it is dropped. */
void mnMessageArgumentPush(Uint32 value, Uint32 kind)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint32	   no = 0;
	Uint32	   limit = MESSAGE_ARGUMENT_COUNT;

	do {
		if(state->argumentKinds[no] == MESSAGE_ARGUMENT_NONE) {
			state->argumentValues[no] = value;
			state->argumentKinds[no] = kind;
			break;
		}
		no++;
	} while(no != limit);
}

/* Returns the value of the first message argument of kind (MESSAGE_ARGUMENT_),
   or 0 when there is none. */
Uint32 mnMessageArgumentLookup(Uint32 kind, Uint32 clear)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint32	   i;
	Uint32	   value = 0;

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

/* Ends a message's pause early: when the message waits on a sound that has
   finished, or while A, B, L or R is held. */
Bool mnTextCheckPauseSkipped(MESSAGE *msg)
{
	WIN_STATE *state = WINDOW_STATE;
	Bool	   cancelKeys = FALSE;

	if(state->waitingForSnd != 0 && sySndGetBGMRestoreDelayStub() == 0)
		cancelKeys = TRUE;
	if(PerWork.data & (KEY_A | KEY_B | KEYS_SHOULDERS))
		cancelKeys = TRUE;
	if(PerWork.data & KEY_B)
		cancelKeys = TRUE;
	if(cancelKeys) {
		msg->countdown = 0;
		return TRUE;
	}
	return FALSE;
}

/* As mnTextCheckPauseSkipped, but on a newly pressed button (the latched
   presses while WIN_STATE.mindRead is set). While B is held the text also goes
   on by itself once the wait's countdown runs down
   (MessageSpeedHeldPageDelays); while it is up the countdown stays at 2. */
Bool mnTextCheckTextDismissed(MESSAGE *msg)
{
	WIN_STATE *state = WINDOW_STATE;
	Bool	   cancelKeys = FALSE;
	Uint32	   pressedKeys;

	if(state->waitingForSnd != 0 && sySndGetBGMRestoreDelayStub() == 0)
		cancelKeys = TRUE;
	pressedKeys = PerWork.push;
	if(state->mindRead != 0)
		pressedKeys = PerWork.pendingPush;
	if(pressedKeys & (KEY_A | KEY_B | KEYS_SHOULDERS))
		cancelKeys = TRUE;
	if(PerWork.data & KEY_B) {
		if(msg->countdown == 1)
			cancelKeys = TRUE;
	} else {
		msg->countdown = 2;
	}
	if(cancelKeys) {
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
		/* The zoom's stage and duration are read as one word: both zero. */
		if(win != NULL && *(Sint32 *)&win->zoomStage == 0 && win->flg != 0 &&
			win->state != 0) {
			mnWindowClose(win, WINDOW_FLAG_INSTANT & win->flg);
		}
		i++;
		msg++;
	} while(i != ARRAY_COUNT(WINDOW_STATE->msgs));
	sySynch(10);
}

/* Shows a message centred across the screen and in its top 15 rows (yOfs
   tiles lower) and waits until it is typed out, then closes it, in the window
   mode (CENTERED_MESSAGE_) gives. */
void mnFieldMessageShowCentered(Sint32 msg, Sint32 mode, Sint32 yOfs)
{
	WIN_STATE *state;
	WINDOW	  *win;
	Sint32	   x;
	Sint32	   y;
	Sint32	   width;
	Sint32	   height;
	Sint32	   entryIdx;

	state = WINDOW_STATE;
	x = 8;
	y = 8;
	win = NULL;
	entryIdx = mnMessageDecode(msg, 1);
	if(state->textBuf[entryIdx] != 0) {
		mnTextGetMessageSize(msg, &x, &y, (Uint32 *)&width, (Uint32 *)&height);
		x = (DISPLAY_TILE_WIDTH - width) >> 1;
		y = ((15 - height) >> 1) + yOfs;
		if(mode != CENTERED_MESSAGE_BARE)
			win = mnWindowCreate(x, y, width, height, 0);
		else {
			win = mnWindowCreate(x, y, 0, 0, WINDOW_FLAG_INSTANT);
			win->width = mode;
			win->height = mode;
		}
		if(mnMessageStart(win, entryIdx, 0, 0, 0, 0) == 0)
			mnWindowClose(win, 1);
		else {
			while(mnMessageAllTyped() == 0)
				sySynch(1);
			if(mode != CENTERED_MESSAGE_BARE) {
				mnWindowClose(win, 0);
				while(mnWindowIsClosed(win) == 0)
					sySynch(1);
			} else
				mnWindowClose(win, 1);
			state->speakerVoiceSnd = 0;
			state->speakerVoiceCountdown = 0;
		}
	}
}

/* Decodes message and empties the argument table; returns where its decoded
   text starts. The Djinn and party join messages (screens_joined.c) use it. */
Sint32 mnMessageDecodeAndClearArguments(Sint32 msg)
{
	return mnMessageDecode(msg, 1);
}

/* Decodes message after the text decoded last, over its terminator. */
void mnMessageDecodeAppend(Sint32 msg)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint32	   cnt = state->textBufEnd;

	cnt = (Uint16)(cnt - 1) & TEXT_BUFFER_MASK;
	state->textBufEnd = (Uint16)(cnt - 1) & TEXT_BUFFER_MASK;
	mnMessageDecode(msg, 1);
}

/* The story crawl's canvas, one byte a pixel: a line of 24 tiles of the
   credits' tile ring. */
#define CRAWL_CANVAS_WIDTH 192

/* The rows of a glyph drawn: all twelve of the Japanese font's, and fourteen of
   the European fonts' fifteen. */
#define PLOT_GLYPH_ROWS JAPANESE_OR(12, 14)

/* Plots a glyph's rows of bits onto the story crawl's canvas at dst, one byte a
   pixel in color, leftmost pixel from the high bit; clear bits are left. */
void mnTextPlotGlyphRows(Uint16 *rows, Uint8 *dst, Sint32 col)
{
	Sint32 y;
	Uint8 *p;
	Sint32 bits;

	for(y = 0; y != PLOT_GLYPH_ROWS; y++) {
		bits = *rows++ & ~3;
		p = dst;
		bits <<= GLYPH_ROW_BITS;
		while(bits != 0) {
			if(bits < 0)
				*p = col;
			bits <<= 1;
			p++;
		}
		dst += CRAWL_CANVAS_WIDTH;
	}
}

#if (LANGUAGE == LANGUAGE_JAPANESE)

/* Draws a character on the canvas at (x, row) with its shadow and returns its
   width: a font glyph, or a kanji from KANJI_FIRST_CODE on, with the mark its
   code carries above TEXT_MARK_SHIFT drawn over it one pixel wider. */
Sint32 mnTextDrawGlyphToCanvas(Uint8 *canvas, Uint32 ch, Sint32 x, Sint32 row)
{
	const Uint16 *mark = NULL;
	const Uint16 *rows;
	Sint32		  width;
	Sint32		  markCode = FontMarkGlyphCodes[ch >> TEXT_MARK_SHIFT];

	if(markCode != 0) {
		markCode -= TEXT_SPACE;
		mark = Font_Glyphs[markCode].rows;
	}
	ch &= (1 << TEXT_MARK_SHIFT) - 1;
	if(ch < KANJI_FIRST_CODE) {
		Sint32 idx = ch - TEXT_SPACE;

		/* The width, then the rows after it. */
		rows = &Font_Glyphs[idx].width;
		width = *rows++;
	} else {
		Sint32 idx = ch - KANJI_FIRST_CODE;

		width = KANJI_WIDTH;
		rows = (const Uint16 *)(Font_KanjiGlyphs + idx * (FONT_GLYPH_ROWS * 2));
	}
	canvas += x + row * CRAWL_CANVAS_WIDTH;
	mnTextPlotGlyphRows((Uint16 *)rows, canvas + 3 * CRAWL_CANVAS_WIDTH + 1, 1);
	if(mark != NULL)
		mnTextPlotGlyphRows((Uint16 *)mark, canvas + CRAWL_CANVAS_WIDTH + 8, 1);
	mnTextPlotGlyphRows(
		(Uint16 *)rows, canvas + 2 * CRAWL_CANVAS_WIDTH, TEXT_COLOR_WHITE);
	if(mark != NULL) {
		mnTextPlotGlyphRows((Uint16 *)mark, canvas + 7, TEXT_COLOR_WHITE);
		width++;
	}
	return width;
}
#else
/* Draws a character on the canvas at (x, row) with its shadow and returns its
   width. */
Sint32 mnTextDrawGlyphToCanvas(Uint8 *canvas, Sint32 ch, Sint32 x, Sint32 row)
{
	/* ch's glyph: a GLYPH is 32 bytes, one a character from TEXT_SPACE. */
	Uint16 *glyph = (Uint16 *)((Uint8 *)Font_Glyphs +
		(((ch & TEXT_LAST_FONT_CODE) - TEXT_SPACE) << 5));
	Sint32	width = *glyph;
	Uint8  *dst = canvas + (x + row * CRAWL_CANVAS_WIDTH);
	Uint8  *rows;

	glyph++;
	rows = dst + 3 * CRAWL_CANVAS_WIDTH + 1;
	mnTextPlotGlyphRows(glyph, rows, 1);
	mnTextPlotGlyphRows(
		glyph, rows = dst + 2 * CRAWL_CANVAS_WIDTH, TEXT_COLOR_WHITE);
	return width;
}
#endif

/* Draws message centred in a 192-pixel canvas of one byte a pixel, as the story
   crawl draws its lines. A line of dashes draws nothing and returns how many
   dashes past the first it has (the crawl waits on it); any other line returns
   0. */
Sint32 mnTextDrawMessageToCanvas(Sint32 msg, Uint8 *canvas)
{
	WIN_STATE *state = WINDOW_STATE;
	Sint32	   x = 0;
	Sint32	   y = 0;
	Sint32	   entry;
	Sint32	   width;
	Sint32	   height;
	Uint16	  *text;
	Uint32	   code;

	state->textBufEnd = x;
	entry = mnMessageDecode(msg, 0);
	mnTextMeasureDecoded(entry, (Uint32 *)&width, (Uint32 *)&height, 0);
	text = &state->textBuf[entry];
	if(text[0] == '-') {
#if !CRAWL_PAUSE_WITHOUT_SLIDE
		if(text[1] == '-')
			return text[2] == '-' ? 2 : 1;
		return 0;
#else
		if(text[1] != '-')
			return 0;
		if(text[2] == '-')
			return text[3] == '-' ? 3 : 2;
		return 1;
#endif
	}
	x = (CRAWL_CANVAS_WIDTH - width) / 2;
	code = *text++;
	while(code != 0) {
		if(code < TEXT_COMMANDS) {
			switch(code) {
			case TEXT_COLOR:
				state->colour = *text;
				text++;
				break;
			case TEXT_OUTLINE:
				state->outline = *text;
				text++;
				break;
			case TEXT_LINE_SPACING:
				state->lineSpacing = *text;
				text++;
				break;
			case 11:
			case 12:
			case TEXT_ACTOR_NAME:
			skip:
				text++;
				break;
			case TEXT_COLORS_RESET:
				mnTextResetStyle();
				break;
			case TEXT_NEWLINE:
				x = 0;
				y += TEXT_LINE_HEIGHT;
				break;
			case 14:
			case TEXT_WINDOW_ANIMATION:
			case TEXT_ACTOR_NAME_START:
				text++;
				goto skip;
			}
		} else {
			/* Japanese text folds a mark after a kana into the kana's code,
			   which mnTextDrawGlyphToCanvas draws. */
#if (LANGUAGE == LANGUAGE_JAPANESE)
			if(*text == TEXT_MARK_VOICED) {
				code |= 1 << TEXT_MARK_SHIFT;
				text++;
			} else if(*text == TEXT_MARK_SEMIVOICED) {
				code |= 2 << TEXT_MARK_SHIFT;
				text++;
			}
#endif
			x += mnTextDrawGlyphToCanvas(canvas, code, x, y);
		}
		code = *text++;
	}
	return 0;
}

/* Points reader at the start of message (its compressed bits and first
   context), running the ARM lookup function from a RAM copy, which it frees
   before returning. */
void mnMessageFind(TEXT_READER *reader, Sint32 msg)
{
	void (*routine)(TEXT_READER *, Sint32);

	RAM_CODE_LOAD(routine, Text_LookupSymbolCode, Text_LookupSymbolCodeSize);
	routine(reader, msg);
	syHeapRelease(routine);
}

/* Each decoding context's Huffman tree and its symbols, two pointers a row: a
   message's previous symbol picks the row. */
extern const void *const Text_MessageContexts[][2];

/* Unused: the symbol decoder in C, as the copy run from RAM
   (Text_DecodeSymbolCode) decodes. Messages are Huffman coded with one tree per
   previous symbol: the previous code picks the tree, the message's bits walk it
   to a leaf, and the leaf's 12-bit symbol is returned and becomes the next
   context. reader->bytes keeps the byte the bits are read from between
   calls. */
UNUSED Sint32 mnTextDecodeSymbolInC(TEXT_READER *reader)
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
	text = reader->bytes;
	textBits = reader->bits;
	treeBits = 1;
	/* The previous code's high bits pick the bank, its low byte the tree. The
	   tree runs forwards from base; its leaves' symbols run backwards from base
	   - 1. */
	contextBank = code >> 8;
	code &= SYMBOL_TREE_MASK;
	base = Text_MessageContexts[contextBank][0];
	base += ((const Uint16 *)Text_MessageContexts[contextBank][1])[code];
	tree = base;
	leaves = base - 1;
	rank = 0;
	/* The tree is its shape in preorder, a bit a node: 0 a branch, 1 a leaf.
	   Bits are read from each byte's lowest up; a buffer that has shifted down
	   to its stop bit (BIT_STREAM_STOP) is empty. A text bit of 0 takes the
	   left branch; 1 skips the left subtree, counting its leaves in rank, and
	   takes the right. */
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
	reader->bytes = text;
	return code;
}

extern const Sint16 PortraitCharacterMap[];
extern const Sint16 PortraitSpriteMap[];

/* Marks both portrait slots free (PORTRAIT_SLOT_EMPTY), as the window system's
   field setup does. */
void mnPortraitClearSlots(void)
{
	WIN_STATE *state = WINDOW_STATE;
	Sint32	   idx;

	for(idx = 0; idx != ARRAY_COUNT(state->portraitPictures); idx++) {
		state->portraitPictures[idx] = PORTRAIT_SLOT_EMPTY;
	}
}

/* A speaker below this is a character, found in PortraitCharacterMap; one from
   it on is a sprite, found in PortraitSpriteMap. */
#define PORTRAIT_FIRST_SPRITE_SPEAKER 16

/* Returns the portrait of a character (ids below PORTRAIT_FIRST_SPRITE_SPEAKER)
   or another speaker's sprite (from PORTRAIT_FIRST_SPRITE_PORTRAIT for those),
   or -1 when it has none. */
Sint32 mnPortraitGetForSpeaker(Uint32 value)
{
	Sint32 result = -1;
	Sint32 portraitIdx = 0;

	/* The party members' small sprites speak as the party members. */
	switch(value) {
	case SPRITE_FELIX_SMALL:
		value = FELIX;
		break;
	case SPRITE_SHEBA_SMALL:
		value = SHEBA;
		break;
	case SPRITE_JENNA_SMALL:
		value = JENNA;
		break;
	case SPRITE_PIERS_SMALL:
		value = PIERS;
		break;
	case SPRITE_ISAAC_SMALL:
		value = ISAAC;
		break;
	case SPRITE_GARET_SMALL:
		value = GARET;
		break;
	case SPRITE_IVAN_SMALL:
		value = IVAN;
		break;
	case SPRITE_MIA_SMALL:
		value = MIA;
		break;
	}
	/* Both maps are {speaker, portrait} pairs ending in -1. */
	if(value < PORTRAIT_FIRST_SPRITE_SPEAKER) {
		for(;;) {
			Sint32 speakerId = PortraitCharacterMap[portraitIdx];

			if(speakerId == -1)
				break;
			if(speakerId == value) {
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
			if(speakerId == value) {
				portraitIdx++;
				result = PortraitSpriteMap[portraitIdx];
				result += PORTRAIT_FIRST_SPRITE_PORTRAIT;
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
	WINDOW	  *win;
	Sint32	   portraitY;
	Sint32	   portraitX;
	WIN_STATE *state;

	state = WINDOW_STATE;
	if(mnPortraitGetForSpeaker(speaker) == -1) {
		return 0;
	}
	portraitY = -4;
	portraitX = portraitY;
	if(state->mindRead != 0) {
		win = mnWindowCreate(x, y, PORTRAIT_WINDOW_TILES + 1,
			PORTRAIT_WINDOW_TILES, WINDOW_FLAG_INSTANT);
		portraitX = 0;
	} else {
		win = mnWindowCreate(x, y, PORTRAIT_WINDOW_TILES, PORTRAIT_WINDOW_TILES,
			WINDOW_FLAG_INSTANT);
	}
	if(win != 0) {
		mnPortraitCreate(
			speaker, portraitOption, -1, win, portraitX, portraitY);
	}
	return win;
}

/* Closes at once the portrait window showing the speaker's portrait, if one is
   open: the window whose first sprite is that portrait slot's. */
void mnPortraitCloseWindow(Sint32 speaker)
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
		if((Uint8)portraitSpr->kind == WINDOW_SPRITE_PORTRAIT &&
			(Uint8)portraitSpr->idx == resourceId) {
			mnWindowClose(win, WINDOW_FLAG_INSTANT);
			return;
		}
	}
}

#undef DECODE_EUROPEAN
#undef PLOT_GLYPH_ROWS
#undef SYMBOL_TREE_MASK
#undef BIT_STREAM_STOP
#undef MESSAGE_CAPACITY_UNLIMITED
