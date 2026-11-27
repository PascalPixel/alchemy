/* The joined-party and found-Djinni messages. Part of screens.c. */

/* Returns 0; only the stub table refers to it. */
Sint32 mnScreensReturnZero(void)
{
	return 0;
}

/* The window that announces a Djinni, wider in French and Italian. */
#define DJINN_JOIN_X	 BY_LANGUAGE(2, 2, 2, 2, 1, 1)
#define DJINN_JOIN_WIDTH BY_LANGUAGE(26, 26, 26, 26, 28, 28)

/* UB: mnMessageStart is called with five of its six arguments, so the flags it
   reads are whatever the stack word after the fifth holds (the 0 left there as
   mnIconLoadWithPalette's last argument), as a call with no declaration in
   scope compiles; UBFIX declares it whole and passes 0. */
#if FIX_FUNCTION_SIGNATURES
MESSAGE *mnMessageStart(WINDOW *window, Sint32 entry, Sint32 x, Sint32 y,
	Uint16 *spacing, Sint32 flags);
#endif

/* Shows that a Djinni joined a party member while the join jingle
   (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed. */
void mnDjinnShowJoinedMessage(Sint32 member, Sint32 element, Sint32 djinn)
{
	WIN_STATE *state = (WIN_STATE *)RamHeapSlots->windowState;
	Sint32	   djinniSprite;
	WINDOW	  *window;
	Sint32	   sprite[3];
	Sint32	  *entry;
	Sint32	   tile, slot;
	Sint32	   id;
	Sint32	   text;
	Sint32	  *p;
	Uint16	  *spacing = NULL;

	window = NULL;
	djinniSprite = DjinnElementSprites[element & 3];
	entry = sprite;
	window = mnWindowCreate(DJINN_JOIN_X, 1, DJINN_JOIN_WIDTH, 5, 0);

	if(window != NULL) {
		mnWindowDrawDividerLine(window, 4, 0, 4, 4);

		state->dirty = 1;

		id = mnPortraitGetForSpeaker(djinniSprite);
		mnIconLoadWithPalette(id, 0, &slot, &tile, PORTRAIT_PALETTE, 0);

		/* The portrait as a bare OAM entry, pushed each tick: a link word,
		   attributes 0 and 1 (32x32 at y 12) and attribute 2, its tile in
		   PORTRAIT_PALETTE. */
		p = entry;
		*p++ = 0;
		*p++ = OBJ_SIZE_32X32 | OBJ_X(DJINN_JOIN_X * 8 + 4) | 12;
		*p = tile | OBJ_PALETTE(PORTRAIT_PALETTE);

		state->speakerVoiceSound = 0;
		state->speakerVoiceCountdown = 0;

		mnMessageArgumentPush(member, MESSAGE_ARGUMENT_ACTOR);
		mnMessageArgumentPush(
			element * DJINN_PER_ELEMENT + djinn + ABILITY_DJINNI_FIRST,
			MESSAGE_ARGUMENT_ABILITY);

		text = mnMessageDecodeAndClearArguments(
			element + (Sint32)MsgFoundVenusDjinni);

#if FIX_FUNCTION_SIGNATURES
		mnMessageStart(window, text, 36, 2, spacing, 0);
#else
		mnMessageStart(window, text, 36, 2, spacing);
#endif

		sySoundPlayStub(MUS_A_NEW_ALLY);

		do {
			syOAMAddSpriteAtDepth(entry, OAM_DEPTH_FRONT - 5);
			sySynch(1);
		} while(sySoundGetBGMRestoreDelayStub() != 0 &&
			(PerWork.push & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

		mnWindowClose(window, 2);
		sySynch(1);
		syVRAMCacheFreeEntry(slot);
	}
}

/* Returns the sprite of party member `index` (ISAAC to PIERS), from
   CharacterSpriteTable, by which mnPortraitGetForSpeaker finds its portrait; 0
   past 8. Index 8 reads the halfword after the table's eight. */
Sint32 mnPartyGetMemberPortraitSprite(Uint32 index)
{
	if(index > 8)
		return 0;
	return CharacterSpriteTable[index];
}

/* Shows that a party member joined the party while the join jingle
   (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed. */
void mnPartyShowJoinedMessage(Sint32 member)
{
	WIN_STATE *state = (WIN_STATE *)RamHeapSlots->windowState;
	WINDOW	  *window = NULL;
	Sint32	   sprite[3];
	Sint32	  *entry = sprite;
	Sint32	   tile, slot;
	Sint32	  *p;

	window = mnWindowCreate(2, 1, 26, 5, 0);
	if(window != NULL) {
		mnWindowDrawDividerLine(window, 4, 0, 4, 4);
		state->dirty = 1;
		mnIconLoadWithPalette(
			mnPortraitGetForSpeaker(mnPartyGetMemberPortraitSprite(member)), 0,
			&slot, &tile, PORTRAIT_PALETTE, 0);
		/* The portrait as a bare OAM entry, pushed each tick: a link word,
		   attributes 0 and 1 (32x32 at x 20, y 12) and attribute 2, its tile in
		   PORTRAIT_PALETTE. */
		p = entry;
		*p++ = 0;
		*p++ = OBJ_SIZE_32X32 | OBJ_X(2 * 8 + 4) | 12;
		*p = tile | OBJ_PALETTE(PORTRAIT_PALETTE);
		state->speakerVoiceSound = 0;
		state->speakerVoiceCountdown = 0;
		mnMessageArgumentPush(member, MESSAGE_ARGUMENT_ACTOR);
#if FIX_FUNCTION_SIGNATURES
		mnMessageStart(window,
			mnMessageDecodeAndClearArguments((Sint32)MsgJoinedParty), 36, 2,
			NULL, 0);
#else
		mnMessageStart(window,
			mnMessageDecodeAndClearArguments((Sint32)MsgJoinedParty), 36, 2, 0);
#endif
		sySoundPlayStub(MUS_A_NEW_ALLY);
		do {
			syOAMAddSpriteAtDepth(entry, OAM_DEPTH_FRONT - 5);
			sySynch(1);
		} while(sySoundGetBGMRestoreDelayStub() != 0 &&
			(PerWork.push & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
		mnWindowClose(window, 2);
		sySynch(1);
		syVRAMCacheFreeEntry(slot);
	}
}

/* Shows that two party members joined the party while the join jingle
   (MUS_A_NEW_ALLY) plays, until it ends or A, B, L or R is pressed. */
void mnPartyShowPairJoinedMessage(Sint32 first, Sint32 second)
{
	WIN_STATE *state = (WIN_STATE *)RamHeapSlots->windowState;
	WINDOW	  *window;
	Sint32	   secondSprite[3];
	Sint32	   firstSprite[3];
	Sint32	  *firstEntry;
	Sint32	  *secondEntry;
	Sint32	   firstSlot, palette, secondSlot;
	Sint32	  *p;

	window = NULL;
	firstEntry = firstSprite;
	window = mnWindowCreate(1, 1, 28, 5, 0);

	if(window != NULL) {
		mnWindowDrawDividerLine(window, 8, 0, 4, 4);

		state->dirty = 1;

		mnIconLoadWithPalette(
			mnPortraitGetForSpeaker(mnPartyGetMemberPortraitSprite(first)), 0,
			&firstSlot, &palette, PORTRAIT_PALETTE, 0);
		/* The portraits as bare OAM entries, pushed each tick: a link word,
		   attributes 0 and 1 (32x32 at y 12, x 12 and 44) and attribute 2, the
		   tile in PORTRAIT_PALETTE or the palette after it. */
		p = firstEntry;
		*p++ = 0;
		*p++ = OBJ_SIZE_32X32 | OBJ_X(12) | 12;
		*p = palette | OBJ_PALETTE(PORTRAIT_PALETTE);

		secondEntry = secondSprite;
		mnIconLoadWithPalette(
			mnPortraitGetForSpeaker(mnPartyGetMemberPortraitSprite(second)), 0,
			&secondSlot, &palette, PORTRAIT_PALETTE + 1, 0);
		p = secondEntry;
		*p++ = 0;
		*p++ = OBJ_SIZE_32X32 | OBJ_X(44) | 12;
		*p = palette | OBJ_PALETTE(PORTRAIT_PALETTE + 1);

		state->speakerVoiceSound = 0;
		state->speakerVoiceCountdown = 0;

		mnMessageArgumentPush(first, MESSAGE_ARGUMENT_ACTOR);
		mnMessageArgumentPush(second, MESSAGE_ARGUMENT_ACTOR);

#if FIX_FUNCTION_SIGNATURES
		mnMessageStart(window,
			mnMessageDecodeAndClearArguments((Sint32)MsgPairJoinedParty), 68, 2,
			NULL, 0);
#else
		mnMessageStart(window,
			mnMessageDecodeAndClearArguments((Sint32)MsgPairJoinedParty), 68, 2,
			0);
#endif

		sySoundPlayStub(MUS_A_NEW_ALLY);

		do {
			syOAMAddSpriteAtDepth(firstEntry, OAM_DEPTH_FRONT - 5);
			syOAMAddSpriteAtDepth(secondEntry, OAM_DEPTH_FRONT - 5);
			sySynch(1);
		} while(sySoundGetBGMRestoreDelayStub() != 0 &&
			(PerWork.push & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);

		mnWindowClose(window, 2);
		sySynch(1);
		syVRAMCacheFreeEntry(firstSlot);
		syVRAMCacheFreeEntry(secondSlot);
	}
}

/* The window that lists the Djinn a summon needs is wider and taller in Spanish
   and French, whose text gives each Djinn count a line of its own. */
#define TO_SUMMON_WINDOW_X		BY_LANGUAGE(3, 3, 3, 2, 2, 3)
#define TO_SUMMON_WINDOW_WIDTH	BY_LANGUAGE(24, 24, 24, 25, 25, 24)
#define TO_SUMMON_WINDOW_HEIGHT BY_LANGUAGE(7, 7, 7, 9, 9, 7)

/* Announces a summon the party can now use: "... can now summon ..."
   (MsgCanNowSummon) until the jingle ends or a button is pressed, then "To
   summon" (MsgToSummon) with its two largest Djinn counts, until a button is
   pressed. */
void mnSummonShowLearnedMessage(Sint32 summon)
{
	WIN_STATE *state = RamHeapSlots->windowState;
	WINDOW	  *window;
	Sint32	   entries;
	Uint32	   resource;
	/* LEFTOVER(frame): an unused local the tick reserves and no instruction
	 * touches. */
	UNUSED Sint32 unused[7];
	Sint32		  counts[4];
	const Uint8	 *djinn;
	Sint32		  i;
	Sint32		  n;

	window = NULL;
	window = mnWindowCreate(4, 1, 22, 5, 0);
	if(window == NULL)
		return;
	state->dirty = 1;
	state->speakerVoiceSound = 0;
	state->speakerVoiceCountdown = 0;
	mnMessageArgumentPush(
		SummonAbilities[summon & (SUMMON_END - 1)], MESSAGE_ARGUMENT_ABILITY);
	entries = mnMessageDecodeAndClearArguments((Sint32)MsgCanNowSummon);
	/* UB: five of mnMessageStart's six arguments, as above; the flags are the
	   stack word resource takes, which nothing sets. UBFIX passes 0. */
#if FIX_FUNCTION_SIGNATURES
	mnMessageStart(window, entries, 0, 2, NULL, 0);
#else
	mnMessageStart(window, entries, 0, 2, 0);
#endif
	sySoundPlayStub(MUS_A_NEW_ALLY);
	do
		sySynch(1);
	while(sySoundGetBGMRestoreDelayStub() != 0 &&
		(PerWork.push & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
	mnWindowClose(window, 2);
	sySynch(1);

	window = mnWindowCreate(TO_SUMMON_WINDOW_X, 1, TO_SUMMON_WINDOW_WIDTH,
		TO_SUMMON_WINDOW_HEIGHT, 0);
	if(window == NULL)
		return;
	mnMessageArgumentPush(
		SummonAbilities[summon & (SUMMON_END - 1)], MESSAGE_ARGUMENT_ABILITY);
	entries = mnMessageDecodeAndClearArguments((Sint32)MsgToSummon);
	djinn = gmSummonDefinitionGetNearStub(summon)->djinnRequired;
	for(i = 0; i < 4; i++)
		counts[i] = djinn[i];
	for(n = 0; n < 2; n++) {
		Sint32 best = 0;
		Sint32 most = 0;

		for(i = 0; i < 4; i++) {
			if(most < counts[i]) {
				best = i;
				most = counts[i];
			}
		}
		/* MsgSummonVenus is the first of four Djinn counts (Venus, Mercury,
		   Mars, Jupiter); the message four past it ends their line. */
		mnMessageArgumentPush(most, MESSAGE_ARGUMENT_NUMBER);
		mnMessageDecodeAppend((Sint32)MsgSummonVenus + best);
		counts[best] = 0;
	}
	mnMessageDecodeAppend((Sint32)MsgSummonVenus + 4);
#if FIX_FUNCTION_SIGNATURES
	mnMessageStart(window, entries, 4, 2, NULL, 0);
#else
	mnMessageStart(window, entries, 4, 2, 0);
#endif
	do
		sySynch(1);
	while((PerWork.push & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0);
	mnWindowClose(window, 2);
	sySynch(1);
	/* UB: resource is never set, so this frees whichever VRAM cache entry its
	   stack word names; UBFIX frees nothing, as nothing was loaded. */
#if !FIX_UNINITIALIZED_READ
	syVRAMCacheFreeEntry(resource);
#endif
}

#undef DJINN_JOIN_X
#undef DJINN_JOIN_WIDTH
#undef TO_SUMMON_WINDOW_X
#undef TO_SUMMON_WINDOW_WIDTH
#undef TO_SUMMON_WINDOW_HEIGHT
