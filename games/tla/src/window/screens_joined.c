/* @file */
/* The joined-party and found-Djinni messages. */
/* Part of screens.c. */

/* Returns 0; only the stub table refers to it. */
s32 mnScreensReturnZero(void) { return 0; }

/* The window that announces a Djinni, wider in French and Italian. */
#if (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define DJINN_JOIN_X	 1
#define DJINN_JOIN_WIDTH 28
#else
#define DJINN_JOIN_X	 2
#define DJINN_JOIN_WIDTH 26
#endif

/* UB: mnMessageStart is called with five of its six arguments, so the flags it
 */
/* reads are whatever the stack word after the fifth holds (the 0 left there as
 */
/* mnIconLoadWithPalette's last argument), as a call with no declaration in */
/* scope compiles; UBFIX declares it whole and passes 0. */
#ifdef UBFIX
MESSAGE *mnMessageStart(WINDOW *window, s32 entry, s32 x, s32 y, u16 *spacing,
						s32 flags);
#endif

/* Shows that a Djinni joined a party member while the join jingle */
/* (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed. */
void mnDjinnShowJoinedMessage(s32 member, s32 element, s32 djinn) {
	WIN_STATE *state = (WIN_STATE *)Ram_HeapSlots->winState;
	s32 djinniSprite;
	WINDOW *window;
	s32 sprite[3];
	s32 *entry;
	s32 tile, slot;
	s32 id;
	s32 text;
	s32 *p;
	u16 *spacing = NULL;

	window = NULL;
	djinniSprite = Djinn_ElementSprites[element & 3];
	entry = sprite;
	window = mnWindowCreate(DJINN_JOIN_X, 1, DJINN_JOIN_WIDTH, 5, 0);

	if(window != NULL) {
		mnWindowDrawDividerLine(window, 4, 0, 4, 4);

		state->dirty = 1;

		id = mnPortraitGetForSpeaker(djinniSprite);
		mnIconLoadWithPalette(id, 0, &slot, &tile, PORTRAIT_PALETTE, 0);

		/* The portrait as a bare OAM entry, pushed each frame: a link word, */
		/* attributes 0 and 1 (32x32 at y 12) and attribute 2, its tile in */
		/* PORTRAIT_PALETTE. */
		p = entry;
		*p++ = 0;
		*p++ = OBJ_SIZE_32X32 | OBJ_X(DJINN_JOIN_X * 8 + 4) | 12;
		*p = tile | OBJ_PALETTE(PORTRAIT_PALETTE);

		state->speakerVoiceSnd = 0;
		state->speakerVoiceCountdown = 0;

		mnMessageArgumentPush(member, MESSAGE_ARGUMENT_ACTOR);
		mnMessageArgumentPush(element * DJINN_PER_ELEMENT + djinn +
								  ABILITY_DJINNI_FIRST,
							  MESSAGE_ARGUMENT_ABILITY);

		text = mnMessageDecodeAndClearArguments(element +
												(s32)MsgFoundVenusDjinni);

#ifdef UBFIX
		mnMessageStart(window, text, 36, 2, spacing, 0);
#else
		mnMessageStart(window, text, 36, 2, spacing);
#endif

		sySndPlayStub(MUS_A_NEW_ALLY);

		do {
			syOamAddSprAtDepth(entry, OAM_DEPTH_FRONT - 5);
			sySynch(1);
		} while(sySndGetBgmRestoreDelayStub() != 0 &&
				(InputWork.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

		mnWindowClose(window, 2);
		sySynch(1);
		syVramCacheFreeEntry(slot);
	}
}

/* Returns the sprite of party member `index` (ISAAC to PIERS), from */
/* Character_SpriteTable, by which mnPortraitGetForSpeaker finds its portrait;
 */
/* 0 past 8. Index 8 reads the halfword after the table's eight. */
s32 mnPartyGetMemberPortraitSpr(u32 index) {
	if(index > 8)
		return 0;
	return Character_SpriteTable[index];
}

/* Shows that a party member joined the party while the join jingle */
/* (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed. */
void mnPartyShowJoinedMessage(s32 member) {
	WIN_STATE *state = (WIN_STATE *)Ram_HeapSlots->winState;
	WINDOW *window = NULL;
	s32 sprite[3];
	s32 *entry = sprite;
	s32 tile, slot;
	s32 *p;

	window = mnWindowCreate(2, 1, 26, 5, 0);
	if(window != NULL) {
		mnWindowDrawDividerLine(window, 4, 0, 4, 4);
		state->dirty = 1;
		mnIconLoadWithPalette(
			mnPortraitGetForSpeaker(mnPartyGetMemberPortraitSpr(member)), 0,
			&slot, &tile, PORTRAIT_PALETTE, 0);
		/* The portrait as a bare OAM entry, pushed each frame: a link word, */
		/* attributes 0 and 1 (32x32 at x 20, y 12) and attribute 2, its tile in
		 */
		/* PORTRAIT_PALETTE. */
		p = entry;
		*p++ = 0;
		*p++ = OBJ_SIZE_32X32 | OBJ_X(2 * 8 + 4) | 12;
		*p = tile | OBJ_PALETTE(PORTRAIT_PALETTE);
		state->speakerVoiceSnd = 0;
		state->speakerVoiceCountdown = 0;
		mnMessageArgumentPush(member, MESSAGE_ARGUMENT_ACTOR);
#ifdef UBFIX
		mnMessageStart(window,
					   mnMessageDecodeAndClearArguments((s32)MsgJoinedParty),
					   36, 2, NULL, 0);
#else
		mnMessageStart(window,
					   mnMessageDecodeAndClearArguments((s32)MsgJoinedParty),
					   36, 2, 0);
#endif
		sySndPlayStub(MUS_A_NEW_ALLY);
		do {
			syOamAddSprAtDepth(entry, OAM_DEPTH_FRONT - 5);
			sySynch(1);
		} while(sySndGetBgmRestoreDelayStub() != 0 &&
				(InputWork.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
		mnWindowClose(window, 2);
		sySynch(1);
		syVramCacheFreeEntry(slot);
	}
}

/* Shows that two party members joined the party while the join jingle */
/* (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed. */
void mnPartyShowPairJoinedMessage(s32 first, s32 second) {
	WIN_STATE *state = (WIN_STATE *)Ram_HeapSlots->winState;
	WINDOW *window;
	s32 secondSprite[3];
	s32 firstSprite[3];
	s32 *firstEntry;
	s32 *secondEntry;
	s32 firstSlot, palette, secondSlot;
	s32 *p;

	window = NULL;
	firstEntry = firstSprite;
	window = mnWindowCreate(1, 1, 28, 5, 0);

	if(window != NULL) {
		mnWindowDrawDividerLine(window, 8, 0, 4, 4);

		state->dirty = 1;

		mnIconLoadWithPalette(
			mnPortraitGetForSpeaker(mnPartyGetMemberPortraitSpr(first)), 0,
			&firstSlot, &palette, PORTRAIT_PALETTE, 0);
		/* The portraits as bare OAM entries, pushed each frame: a link word, */
		/* attributes 0 and 1 (32x32 at y 12, x 12 and 44) and attribute 2, the
		 */
		/* tile in PORTRAIT_PALETTE or the palette after it. */
		p = firstEntry;
		*p++ = 0;
		*p++ = OBJ_SIZE_32X32 | OBJ_X(12) | 12;
		*p = palette | OBJ_PALETTE(PORTRAIT_PALETTE);

		secondEntry = secondSprite;
		mnIconLoadWithPalette(
			mnPortraitGetForSpeaker(mnPartyGetMemberPortraitSpr(second)), 0,
			&secondSlot, &palette, PORTRAIT_PALETTE + 1, 0);
		p = secondEntry;
		*p++ = 0;
		*p++ = OBJ_SIZE_32X32 | OBJ_X(44) | 12;
		*p = palette | OBJ_PALETTE(PORTRAIT_PALETTE + 1);

		state->speakerVoiceSnd = 0;
		state->speakerVoiceCountdown = 0;

		mnMessageArgumentPush(first, MESSAGE_ARGUMENT_ACTOR);
		mnMessageArgumentPush(second, MESSAGE_ARGUMENT_ACTOR);

#ifdef UBFIX
		mnMessageStart(
			window, mnMessageDecodeAndClearArguments((s32)MsgPairJoinedParty),
			68, 2, NULL, 0);
#else
		mnMessageStart(
			window, mnMessageDecodeAndClearArguments((s32)MsgPairJoinedParty),
			68, 2, 0);
#endif

		sySndPlayStub(MUS_A_NEW_ALLY);

		do {
			syOamAddSprAtDepth(firstEntry, OAM_DEPTH_FRONT - 5);
			syOamAddSprAtDepth(secondEntry, OAM_DEPTH_FRONT - 5);
			sySynch(1);
		} while(sySndGetBgmRestoreDelayStub() != 0 &&
				(InputWork.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

		mnWindowClose(window, 2);
		sySynch(1);
		syVramCacheFreeEntry(firstSlot);
		syVramCacheFreeEntry(secondSlot);
	}
}

/* The window that lists the Djinn a summon needs is wider and taller in */
/* Spanish and French, whose text gives each Djinn count a line of its own. */
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
#define TO_SUMMON_WINDOW_X		2
#define TO_SUMMON_WINDOW_WIDTH	25
#define TO_SUMMON_WINDOW_HEIGHT 9
#else
#define TO_SUMMON_WINDOW_X		3
#define TO_SUMMON_WINDOW_WIDTH	24
#define TO_SUMMON_WINDOW_HEIGHT 7
#endif

/* Announces a summon the party can now use: "... can now summon ..." */
/* (MsgCanNowSummon) until the jingle ends or a button is pressed, then */
/* "To summon" (MsgToSummon) with its two largest Djinn counts, until a button
 */
/* is pressed. */
void mnSummonShowLearnedMessage(s32 summon) {
	WIN_STATE *state = Ram_HeapSlots->winState;
	WINDOW *window;
	s32 entries;
	u32 resource;
	/* LEFTOVER(frame): an unused local the frame reserves and no instruction
	 * touches. */
	UNUSED s32 unused[7];
	s32 counts[4];
	const u8 *djinn;
	s32 i;
	s32 n;

	window = NULL;
	window = mnWindowCreate(4, 1, 22, 5, 0);
	if(window == NULL)
		return;
	state->dirty = 1;
	state->speakerVoiceSnd = 0;
	state->speakerVoiceCountdown = 0;
	mnMessageArgumentPush(Summon_Abilities[summon & (SUMMON_END - 1)],
						  MESSAGE_ARGUMENT_ABILITY);
	entries = mnMessageDecodeAndClearArguments((s32)MsgCanNowSummon);
	/* UB: five of mnMessageStart's six arguments, as above; the flags are the
	 */
	/* stack word resource takes, which nothing sets. UBFIX passes 0. */
#ifdef UBFIX
	mnMessageStart(window, entries, 0, 2, NULL, 0);
#else
	mnMessageStart(window, entries, 0, 2, 0);
#endif
	sySndPlayStub(MUS_A_NEW_ALLY);
	do
		sySynch(1);
	while(sySndGetBgmRestoreDelayStub() != 0 &&
		  (InputWork.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
	mnWindowClose(window, 2);
	sySynch(1);

	window = mnWindowCreate(TO_SUMMON_WINDOW_X, 1, TO_SUMMON_WINDOW_WIDTH,
							TO_SUMMON_WINDOW_HEIGHT, 0);
	if(window == NULL)
		return;
	mnMessageArgumentPush(Summon_Abilities[summon & (SUMMON_END - 1)],
						  MESSAGE_ARGUMENT_ABILITY);
	entries = mnMessageDecodeAndClearArguments((s32)MsgToSummon);
	djinn = gmSummonDefinitionGet(summon)->djinnRequired;
	for(i = 0; i < 4; i++)
		counts[i] = djinn[i];
	for(n = 0; n < 2; n++) {
		s32 best = 0;
		s32 most = 0;

		for(i = 0; i < 4; i++) {
			if(most < counts[i]) {
				best = i;
				most = counts[i];
			}
		}
		/* MsgSummonVenus is the first of four Djinn counts (Venus, Mercury,
		 * Mars, */
		/* Jupiter); the message four past it ends their line. */
		mnMessageArgumentPush(most, MESSAGE_ARGUMENT_NUMBER);
		mnMessageDecodeAppend((s32)MsgSummonVenus + best);
		counts[best] = 0;
	}
	mnMessageDecodeAppend((s32)MsgSummonVenus + 4);
#ifdef UBFIX
	mnMessageStart(window, entries, 4, 2, NULL, 0);
#else
	mnMessageStart(window, entries, 4, 2, 0);
#endif
	do
		sySynch(1);
	while((InputWork.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
	mnWindowClose(window, 2);
	sySynch(1);
	/* UB: resource is never set, so this frees whichever VRAM cache entry its
	 */
	/* stack word names; UBFIX frees nothing, as nothing was loaded. */
#ifndef UBFIX
	syVramCacheFreeEntry(resource);
#endif
}

#undef DJINN_JOIN_X
#undef DJINN_JOIN_WIDTH
#undef TO_SUMMON_WINDOW_X
#undef TO_SUMMON_WINDOW_WIDTH
#undef TO_SUMMON_WINDOW_HEIGHT
