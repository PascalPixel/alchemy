/* The Colosso choice and the joined-party messages. Part of screens.c. */

/* UB: the joined messages pass five of mnMessageStart's six arguments, so its
   flags are whatever the sixth argument would have held; UBFIX passes 0, as the
   window object's own messages do. */
#if FIX_FUNCTION_SIGNATURES
#define JOINED_QUEUE_TEXT(window, entry, x, y, spacing) \
	mnMessageStart(window, entry, x, y, (Uint16 *)(spacing), 0)
#else
#define JOINED_QUEUE_TEXT(window, entry, x, y, spacing) \
	mnMessageStart(window, entry, x, y, spacing)
#endif

/* The window of the talk choices ("Descriptions", "Cheer", "Nothing"), in
   tiles. */
#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_GERMAN || \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#define TALK_WINDOW_X	  7
#define TALK_WINDOW_WIDTH 18
#elif LANGUAGE == LANGUAGE_SPANISH
#define TALK_WINDOW_X	  9
#define TALK_WINDOW_WIDTH 12
#else
#define TALK_WINDOW_X	  8
#define TALK_WINDOW_WIDTH 16
#endif

/* The Colosso choice of what to do: "Descriptions", "Cheer" or "Nothing",
   chosen with a pointing hand. */
Sint32 mnColossoMenuChoose(Sint32 unused)
{
	WINDOW	   *win;
	MENU_CURSOR cursor;
	Sint32		slot;
	Sint32		row;
	Bool		moved;
	Uint16	   *palette;

	moved = TRUE;
	win = mnWindowCreate(
		TALK_WINDOW_X, 13, TALK_WINDOW_WIDTH, 7, WINDOW_FLAG_INSTANT);
	mnTextDrawMessageAsTiles(MsgTalkChoice, win, 8, 0);
	mnTextDrawMessageAsTiles(MsgTalkChoice + 1, win, 8, 16);
	mnTextDrawMessageAsTiles(MsgTalkChoice + 2, win, 8, 32);
	slot = syVRAMCacheFindFreeEntry();
	row = 0;
	if(slot < VRAM_CACHE_ENTRY_COUNT) {
		syVRAMCacheLoad(slot, 128, PointingHandTiles);
		cursor.anchor =
			mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_16X16, win, 0, 0);
		mnCursorSetPosImmediateStub(&cursor, win->x * 8 - 3, win->y * 8 + 9);
	}
	/* The choice under the cursor flashes between WINDOW_PALETTE and
	   HIGHLIGHT_PALETTE, a copy of it with WINDOW_BACKGROUND_COLOR grey. */
	palette = (Uint16 *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0);
	syDMAXCopy((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
		DMA_COPY_WORDS(8), REG_DMA3);
	palette[WINDOW_BACKGROUND_COLOR] = RGB(24, 24, 24);
	while(1) {
		mnWindowSetInteriorPalette(
			win, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1, HIGHLIGHT_PALETTE);
		sySynch(1);
		mnWindowSetInteriorPalette(
			win, 1, row * 2, TALK_WINDOW_WIDTH - 4, 1, WINDOW_PALETTE);
		if(moved) {
			moved = FALSE;
			mnCursorSetStub(
				&cursor, win->x * 8 - 3, (win->y + row * 2) * 8 + 9, 3);
		}
		mnCursorAdvanceStub(&cursor);
		if(PerRepeat & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			row--;
			moved = TRUE;
			if(row == -1)
				row = 2;
		}
		if(PerRepeat & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			row++;
			moved = TRUE;
			if(row == 3)
				row = 0;
		}
		if(PerRepeat & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			row = -1;
			break;
		}
		if(PerRepeat & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			break;
		}
	}
	mnWindowClose(win, 2);
	sySynch(1);
	syVRAMCacheFreeEntry(slot);
	return row;
}

/* Shows that a Djinni joined a party member while the join jingle
   (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed. */
void mnDjinnShowJoinedMessage(Sint32 member, Sint32 element, Sint32 djinn)
{
	WIN_STATE *state = WINDOW_STATE;
	Sint32	   speaker;
	WINDOW	  *win;
	Sint32	   portrait[3];
	Sint32	  *entry;
	Sint32	   palette, handle;
	Sint32	  *word;
	Uint16	  *spacing = NULL;

	win = NULL;
	speaker = DjinnElementSprites[element & 3];
	entry = portrait;
	win = mnWindowCreate(DJINN_JOIN_X, 1, DJINN_JOIN_WIDTH, 5, 0);
	if(win == NULL)
		return;
	mnWindowDrawDividerLine(win, 4, 0, 4, 4);
	state->dirty = 1;
	mnIconLoadWithPalette(mnPortraitGetForSpeaker(speaker), 0, &handle,
		&palette, PORTRAIT_PALETTE, 0);
	/* The portrait as a bare OAM entry, drawn each tick at y 12: a link word,
	   attributes 0 and 1 and attribute 2. */
	word = entry;
	*word++ = 0;
	*word++ = OBJ_SIZE_32X32 | OBJ_X(DJINN_JOIN_X * 8 + 4) | 12;
	*word = palette | OBJ_PALETTE(PORTRAIT_PALETTE);
	/* No voice blips while the message types. */
	state->voiceSnd = 0;
	state->voiceCountdown = 0;
	mnMessageArgumentPush(member, MESSAGE_ARGUMENT_ACTOR);
	mnMessageArgumentPush(
		element * DJINN_PER_ELEMENT + djinn + ABILITY_DJINNI_FIRST,
		MESSAGE_ARGUMENT_ABILITY);
	JOINED_QUEUE_TEXT(win,
		mnMessageDecodeAndClearArguments(element + MsgVenusDjinnJoined), 36, 2,
		spacing);
	sySndPlayStub(MUS_A_NEW_ALLY);
	do {
		syOAMAddSprAtDepth(entry, OAM_DEPTH_FRONT - 5);
		sySynch(1);
	} while(sySndGetBGMRestoreDelayStub() != 0 &&
		(PerPush & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
	mnWindowClose(win, 2);
	sySynch(1);
	syVRAMCacheFreeEntry(handle);
}

/* Returns the sprite whose portrait shows party member `member` (0 to 8): from
   CharacterChildSpriteTable during the prologue, from CharacterSpriteTable
   otherwise; 0 for any other party member. */
Sint32 mnPartyGetMemberPortraitSpr(Uint32 member)
{
	if(member > 8) {
		return 0;
	}
	if(gmFlagGetStub(FLAG_CHILDHOOD) == 0) {
		return CharacterSpriteTable[member];
	}
	return CharacterChildSpriteTable[member];
}

/* Shows that a party member joined the party while the join jingle
   (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed. */
void mnPartyShowJoinedMessage(Sint32 member)
{
	WIN_STATE *state = WINDOW_STATE;
	WINDOW	  *window = NULL;
	Sint32	   spr[3];
	Sint32	  *entry = spr;
	Sint32	   palette, handle;
	Sint32	  *p;

	window = mnWindowCreate(2, 1, 26, 5, 0);
	if(window != NULL) {
		mnWindowDrawDividerLine(window, 4, 0, 4, 4);
		state->dirty = 1;
		mnIconLoadWithPalette(
			mnPortraitGetForSpeaker(mnPartyGetMemberPortraitSpr(member)), 0,
			&handle, &palette, PORTRAIT_PALETTE, 0);
		/* The portrait as a bare OAM entry, drawn each tick: a link word,
		   attributes 0 and 1 and attribute 2. */
		p = entry;
		*p++ = 0;
		*p++ = OBJ_SIZE_32X32 | OBJ_X(2 * 8 + 4) | 12;
		*p = palette | OBJ_PALETTE(PORTRAIT_PALETTE);
		state->voiceSnd = 0;
		state->voiceCountdown = 0;
		mnMessageArgumentPush(member, MESSAGE_ARGUMENT_ACTOR);
		JOINED_QUEUE_TEXT(
			window, mnMessageDecodeAndClearArguments(MsgJoinedParty), 36, 2, 0);
		sySndPlayStub(MUS_A_NEW_ALLY);
		do {
			syOAMAddSprAtDepth(entry, OAM_DEPTH_FRONT - 5);
			sySynch(1);
		} while(sySndGetBGMRestoreDelayStub() != 0 &&
			(PerPush & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
		mnWindowClose(window, 2);
		sySynch(1);
		syVRAMCacheFreeEntry(handle);
	}
}

/* Shows that two party members joined the party while the join jingle
   (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed. */
void mnPartyShowPairJoinedMessage(Sint32 first, Sint32 second)
{
	WIN_STATE *state = WINDOW_STATE;
	WINDOW	  *win;
	Sint32	   secondOam[3];
	Sint32	   firstOam[3];
	Sint32	  *firstEntry;
	Sint32	  *secondEntry;
	Sint32	   firstHandle, palette, secondHandle;
	Sint32	  *p;

	win = NULL;
	firstEntry = firstOam;
	win = mnWindowCreate(1, 1, 28, 5, 0);

	if(win != NULL) {
		/* BUG: from (8, 0) to (4, 4) is neither across nor down, so
		   mnWindowDrawDividerLine draws nothing and the window has no divider.
		   The single joins draw (4, 0, 4, 4), half a tile before their text at
		   x 36; column 8 is the same place before this one's text at x 68. */
#if FIX_PAIR_JOINED_DIVIDER
		mnWindowDrawDividerLine(win, 8, 0, 8, 4);
#else
		mnWindowDrawDividerLine(win, 8, 0, 4, 4);
#endif

		state->dirty = 1;

		mnIconLoadWithPalette(
			mnPortraitGetForSpeaker(mnPartyGetMemberPortraitSpr(first)), 0,
			&firstHandle, &palette, PORTRAIT_PALETTE, 0);
		/* The portrait as a bare OAM entry, drawn each tick: a link word,
		   attributes 0 and 1 and attribute 2. */
		p = firstEntry;
		*p++ = 0;
		*p++ = OBJ_SIZE_32X32 | OBJ_X(12) | 12;
		*p = palette | OBJ_PALETTE(PORTRAIT_PALETTE);

		secondEntry = secondOam;
		mnIconLoadWithPalette(
			mnPortraitGetForSpeaker(mnPartyGetMemberPortraitSpr(second)), 0,
			&secondHandle, &palette, PORTRAIT_PALETTE + 1, 0);
		p = secondEntry;
		*p++ = 0;
		*p++ = OBJ_SIZE_32X32 | OBJ_X(44) | 12;
		*p = palette | OBJ_PALETTE(PORTRAIT_PALETTE + 1);

		state->voiceSnd = 0;
		state->voiceCountdown = 0;

		mnMessageArgumentPush(first, MESSAGE_ARGUMENT_ACTOR);
		mnMessageArgumentPush(second, MESSAGE_ARGUMENT_ACTOR);

		JOINED_QUEUE_TEXT(win,
			mnMessageDecodeAndClearArguments(MsgPairJoinedParty), 68, 2, 0);

		sySndPlayStub(MUS_A_NEW_ALLY);

		do {
			syOAMAddSprAtDepth(firstEntry, OAM_DEPTH_FRONT - 5);
			syOAMAddSprAtDepth(secondEntry, OAM_DEPTH_FRONT - 5);
			sySynch(1);
		} while(sySndGetBGMRestoreDelayStub() != 0 &&
			(PerPush & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

		mnWindowClose(win, 2);
		sySynch(1);
		syVRAMCacheFreeEntry(firstHandle);
		syVRAMCacheFreeEntry(secondHandle);
	}
}

#undef JOINED_QUEUE_TEXT
