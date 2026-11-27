// The preview of what setting or removing a Djinni changes. Part of screens.c.

// How many bytes of NumberBuffer are widened: the eleven characters syStringFormatDecimal writes,
// its terminator and the two bytes after.
#define NUMBER_TEXT_WIDE_LENGTH   14

// A changed stat's arrow, right of its value on window tile row `row` (HP on row 1 to Luck on row
// 6).
#define PREVIEW_STAT_ARROW_X      80
#define PREVIEW_STAT_ARROW_Y(row) ((row) * 8 + 6)

// Writes `value` into `out` as syStringFormatDecimal's eleven-character, right-aligned decimal
// text, widened to halfwords for mnTextDrawWideStringInWindow. `out` needs room for
// NUMBER_TEXT_WIDE_LENGTH halfwords.
UNREAD_RESULT mnStringFormatNumberToHalfwords(Sint16 *out, Sint32 value)
{
	Sint16 *dest;
	Sint32  i;
	Uint8  *src;

	dest = out;
	syStringFormatDecimal(value);
	src = NumberBuffer;
	for (i = 0; i < NUMBER_TEXT_WIDE_LENGTH; i++)
	{
		*dest++ = *src++;
	}
}

// The marks mnBattleDjinnMenuListChangedAbilities gives an ability in its list, above its id: the
// preview draws a gained one in TEXT_COLOR_YELLOW and a lost one in TEXT_COLOR_RED.
#define DJINN_PREVIEW_ABILITY_GAINED     0x8000 // new with the change
#define DJINN_PREVIEW_ABILITY_LOST       0x4000 // gone with the change
// Entries of the change list: the new list's 32 and the lost ones after.
#define DJINN_PREVIEW_ABILITY_CAPACITY   48
// The abilities a page of the preview lists.
#define DJINN_PREVIEW_ABILITIES_PER_PAGE 5

// Lists into `out` the abilities usable in battle (ABILITY_USABLE_IN_BATTLE) of the action list
// `newGrid`, in its order, marking with DJINN_PREVIEW_ABILITY_GAINED those `oldGrid` lacks; then
// appends those of `oldGrid` that `newGrid` lacks, marked DJINN_PREVIEW_ABILITY_LOST. Returns the
// entries written, and the gained and lost counts through `gained` and `lost`.
Sint32 mnBattleDjinnMenuListChangedAbilities(const PSYNERGY_ENTRY *oldGrid,
                                             const PSYNERGY_ENTRY *newGrid,
                                             Uint16               *out,
                                             Sint32               *gained,
                                             Sint32               *lost)
{
	Sint32 i;
	Sint32 j;
	Sint32 count;
	Sint32 gainedCount;
	Sint32 lostCount;

	gainedCount = 0;
	lostCount   = 0;
	count       = 0;
	for (i = 0; i < PSYNERGY_SLOTS && newGrid[i].encodedAction != 0; i++)
	{
		if (gmAbilityGetDirectStub(newGrid[i].encodedAction)->flags & ABILITY_USABLE_IN_BATTLE)
		{
			out[count] = newGrid[i].encodedAction & ACTION_ID_MASK;
			count++;
			for (j = 0; j < PSYNERGY_SLOTS; j++)
			{
				if (((newGrid[i].encodedAction ^ oldGrid[j].encodedAction) & ACTION_ID_MASK) == 0)
					break;
			}
			if (j == PSYNERGY_SLOTS)
			{
				gainedCount++;
				out[count - 1] |= DJINN_PREVIEW_ABILITY_GAINED;
			}
		}
	}
	for (i = 0; i < PSYNERGY_SLOTS && oldGrid[i].encodedAction != 0; i++)
	{
		if (gmAbilityGetDirectStub(oldGrid[i].encodedAction)->flags & ABILITY_USABLE_IN_BATTLE)
		{
			for (j = 0; j < PSYNERGY_SLOTS; j++)
			{
				if (((oldGrid[i].encodedAction ^ newGrid[j].encodedAction) & ACTION_ID_MASK) == 0)
					break;
			}
			if (j == PSYNERGY_SLOTS)
			{
				lostCount++;
				out[count] =
					(oldGrid[i].encodedAction & ACTION_ID_MASK) | DJINN_PREVIEW_ABILITY_LOST;
				count++;
			}
		}
	}
	*gained = gainedCount;
	*lost   = lostCount;
	return count;
}

// Loads the icon of ability `abilityId` into the first free VRAM cache slot and shows it as a 16x16
// sprite at pixel (x, y) inside `window`, whose sprite list then owns the slot. Does nothing when
// the cache is full.
UNREAD_RESULT mnWindowSpriteCreateAbilityIcon(WINDOW *window, Sint32 x, Sint32 y, Sint32 abilityId)
{
	Sint32 iconSlot;

	iconSlot = syVRAMCacheFindFreeEntry();
	if (iconSlot != VRAM_CACHE_ENTRY_COUNT)
	{
		mnAbilityIconLoadIntoSlot(abilityId, iconSlot);
		// If the sprite pool is full, the creator releases iconSlot itself.
		mnWindowSpriteCreate(iconSlot, WINDOW_SPRITE_SIZE_16X16, window, x, y);
	}
}

// Shows what setting or removing Djinni `encodedDjinn` (element << 8 | index) would change for
// `member`: closes the preview window `window`, opens a new one and returns it, or NULL when none
// could be opened. Page 0 compares the class and stats, old against new, with an arrow by each stat
// that rises or falls; pages 1 on list the unit's battle abilities after the change and then those
// it would lose, and *pageCount is set to their number (at least 1). The Djinni is toggled and the
// unit recalculated only to measure: the unit is restored from a copy before it returns.
WINDOW *mnBattleDjinnMenuShowChangePreview(
	WINDOW *window, Sint32 member, Sint32 encodedDjinn, Sint32 page, Sint32 *pageCount)
{
	UNIT               *unit;
	UNIT               *savedUnit;
	ABILITY_DEFINITION *ability;
	Sint16             *numberText;
	Sint32              oldHp;
	Sint32              oldPp;
	Sint32              oldAtk;
	Sint32              oldDefense;
	Sint32              oldAgi;
	Uint16             *changedAbilities;
	Sint32              abilityCount;
	Sint32              visibleRow;
	Sint32              summaryRows;
	Sint32              visibleRows;
	Sint32              element;
	Sint32              djinnIndex;
	Sint32              pages;
	Sint32              gained;
	Sint32              lost;
	Sint32              rangePattern;
	Sint32              rising;
	Sint32              i;

	// Draws a rising or falling stat arrow at pixel (x, y) of the preview window: a 16x16 sprite in
	// a VRAM cache entry of its own, which the window's sprite list frees with it
	// (WINDOW_SPRITE_ICON).
	void                mnDjinnMenuDrawStatArrow(Sint32 x, Sint32 y, Sint32 rising)
	{
		WINDOW_SPR  *sprite = mnWindowSpriteAcquireFree();
		PREVIEW_SPR *entry;
		if (sprite)
		{
			sprite->motion                = WINDOW_SPRITE_MOTION_STILL;
			sprite->kind                  = WINDOW_SPRITE_ICON;
			sprite->index                 = syVRAMCacheAllocateEntry(2 * 2 * TILE_SIZE_4BPP);
			entry                         = (PREVIEW_SPR *)&sprite->sprite;
			sprite->depth                 = OAM_DEPTH_WINDOW;
			sprite->x                     = 120;
			sprite->y                     = 120;
			*(Uint32 *)&entry->attributes = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
			IO_WRITE32(0, &entry->tile.value);
			// FAKEMATCH(dead): advancing and restoring the captured window.
			window++;
			window--;
			entry->attributes.bits.x = window->x * 8 + x;
			entry->attributes.bits.y = window->y * 8 + y;
			entry->tile.bits.index   = syVRAMCacheReload(
                sprite->index, (Sint32)(rising ? DjinnMenuArrowUpTiles : DjinnMenuArrowDownTiles));
			mnWindowSpriteAppendToList(&window->sprites, sprite);
		}
	}

	unit = gmUnitGetStub(member);
	if (window != NULL)
		mnWindowClose(window, 1);

	if (page == 0)
		window = mnWindowCreate(0, 8, 21, 11, MENU_WINDOW_FLAGS);
	else
		window = mnWindowCreate(0, 5, 21, 14, MENU_WINDOW_FLAGS);
	if (window == NULL)
		return NULL;

	numberText = syHeapAllocate(64 * sizeof(*numberText));
	savedUnit  = syHeapAllocate(sizeof(*savedUnit));
	changedAbilities =
		syHeapAllocateEWRAM(DJINN_PREVIEW_ABILITY_CAPACITY * sizeof(*changedAbilities));
	syWordsCopy(savedUnit, unit, sizeof(*savedUnit));

	element    = DJINN_COMMAND_ELEMENT(encodedDjinn);
	djinnIndex = DJINN_COMMAND_INDEX(encodedDjinn);
	if (gmDjinnIsSetStub(member, element, djinnIndex) != 0)
		gmDjinnPutOnStandbyStub(member, element, djinnIndex);
	else
		gmDjinnSetStub(member, element, djinnIndex);
	gmUnitCalculateStub(member);

	abilityCount = mnBattleDjinnMenuListChangedAbilities(
		savedUnit->psynergy, unit->psynergy, changedAbilities, &gained, &lost);
	// A page past the list shows the last.
	pages      = (abilityCount - 1) / DJINN_PREVIEW_ABILITIES_PER_PAGE + 1;
	*pageCount = pages;
	if ((page - 1) * DJINN_PREVIEW_ABILITIES_PER_PAGE >= abilityCount)
		page = pages;

	// The first page compares stats; the remaining pages compare Psynergy.
	if (page == 0)
	{
		Sint32 label = MsgPreviewStatLabel;

		mnTextDrawMessageAsTiles(label, window, 0, 8);
		mnTextDrawMessageAsTiles(label + 1, window, 0, 16);
		mnTextDrawMessageAsTiles(label + 2, window, 0, 24);
		mnTextDrawMessageAsTiles(label + 3, window, 0, 32);
		mnTextDrawMessageAsTiles(label + 4, window, 0, 40);
		mnTextDrawMessageAsTiles(label + 5, window, 0, 48);

		// The old values, by the labels; the new ones go at column 11 below.
		oldHp = savedUnit->hp;
		mnStringFormatNumberToHalfwords(numberText, oldHp);
		mnTextDrawWideStringInWindow(
			numberText + NUMBER_TEXT_FOUR_PLACES, window, DJINN_PREVIEW_VITALS_X, 1);
		oldPp = savedUnit->pp;
		mnStringFormatNumberToHalfwords(numberText, oldPp);
		mnTextDrawWideStringInWindow(
			numberText + NUMBER_TEXT_FOUR_PLACES, window, DJINN_PREVIEW_VITALS_X, 2);
		oldAtk = savedUnit->attack;
		mnStringFormatNumberToHalfwords(numberText, oldAtk);
		mnTextDrawWideStringInWindow(
			numberText + DJINN_PREVIEW_STAT_DIGIT_OFFSET, window, DJINN_PREVIEW_STAT_X, 3);
		oldDefense = savedUnit->defense;
		mnStringFormatNumberToHalfwords(numberText, oldDefense);
		mnTextDrawWideStringInWindow(
			numberText + DJINN_PREVIEW_STAT_DIGIT_OFFSET, window, DJINN_PREVIEW_STAT_X, 4);
		oldAgi = savedUnit->agility;
		mnStringFormatNumberToHalfwords(numberText, oldAgi);
		mnTextDrawWideStringInWindow(
			numberText + DJINN_PREVIEW_STAT_DIGIT_OFFSET, window, DJINN_PREVIEW_STAT_X, 5);
		mnStringFormatNumberToHalfwords(numberText, savedUnit->luck);
		mnTextDrawWideStringInWindow(numberText + NUMBER_TEXT_THREE_PLACES, window, 5, 6);

		mnWindowDrawDividerLine(window, 0, 8, 19, 8);
		// The prompt for the Psynergy pages, its button and ":Psynergy Gained", stands out in
		// TEXT_COLOR_RED when the Psynergy changes.
		if (gained != 0 || lost != 0)
			mnTextSetColor(TEXT_COLOR_RED);
		mnTextDrawMessageAsTiles(MsgBattleAdditionalPsynergyHelp, window, DJINN_PREVIEW_HELP_X, 64);
		mnTextSetColor(TEXT_COLOR_WHITE);
	}

	if (page > 0)
	{
		summaryRows = 0;
		visibleRows = abilityCount;
		if (abilityCount >= DJINN_PREVIEW_ABILITIES_PER_PAGE)
			visibleRows = DJINN_PREVIEW_ABILITIES_PER_PAGE;
		i = (page - 1) * DJINN_PREVIEW_ABILITIES_PER_PAGE;
		for (visibleRow = 0; visibleRow < visibleRows && i < abilityCount; visibleRow++, i++)
		{
			ability = gmAbilityGetDirectStub(changedAbilities[i]);
			if (ability->element != ELEMENT_NONE)
				mnWindowSetTilemapEntry(
					window, LIST_TILE_SUMMON_ELEMENT + ability->element, 15, visibleRow * 2, 0);

			// Its element icon, range icon, icon, name and PP cost.
			rangePattern = ability->range;
			if (rangePattern == ABILITY_RANGE_ALL)
				rangePattern = RANGE_ICON_ALL;
			else
				rangePattern--;
			mnRangeIconDraw(window, 16, visibleRow * 2, rangePattern, 0);

			mnWindowSpriteCreateAbilityIcon(
				window, 0, visibleRow * 16 - 4, changedAbilities[i] & ACTION_ID_MASK);
			if ((changedAbilities[i] & DJINN_PREVIEW_ABILITY_GAINED) != 0)
				mnTextSetColor(TEXT_COLOR_YELLOW);
			else if ((changedAbilities[i] & DJINN_PREVIEW_ABILITY_LOST) != 0)
				mnTextSetColor(TEXT_COLOR_RED);
			else
				mnTextSetColor(TEXT_COLOR_WHITE);

			mnTextDrawMessageAsTiles((changedAbilities[i] & ACTION_ID_MASK) + MsgAbilityName,
			                         window,
			                         16,
			                         visibleRow * 16);
			mnWindowSetTilemapEntry(window, PSYNERGY_COST_FIRST_TILE, 11, visibleRow * 2, 0);
			mnWindowSetTilemapEntry(window, PSYNERGY_COST_SECOND_TILE, 12, visibleRow * 2, 0);
			mnTextDrawNumberInWindow(gmAbilityGetDirectStub(changedAbilities[i])->ppCost,
			                         2,
			                         window,
			                         104,
			                         visibleRow * 16);
		}

		if (gained != 0)
		{
			mnTextSetColor(TEXT_COLOR_YELLOW);
			mnTextDrawMessageAsTiles(MsgPsynergyGained, window, 32, 80);
			summaryRows = 1;
		}
		if (lost != 0)
		{
			mnTextSetColor(TEXT_COLOR_RED);
			mnTextDrawMessageAsTiles(MsgPsynergyLost, window, 32, summaryRows * 8 + 80);
			summaryRows++;
		}
		if (summaryRows == 0)
			mnTextDrawMessageAsTiles(MsgPsynergyUnchanged, window, 32, 80);
		mnTextSetColor(TEXT_COLOR_WHITE);
		mnTextSetColor(TEXT_COLOR_WHITE);
		mnWindowDrawDividerLine(window, 0, 10, 19, 10);
	}

	if (page == 0)
	{
		mnTextDrawMessageAsTiles(MsgClassName + savedUnit->classId, window, 0, 0);
		mnTextDrawMessageAsTiles(MsgClassName + unit->classId, window, 80, 0);
		// The arrow between the classes, flipped: DJINN_MENU_ARROW_TILE blinks when the class
		// changes, the tile after it stays (DjinnMenuBlinkTile).
		if (savedUnit->classId != unit->classId)
			mnWindowSetTilemapEntry(
				window, WINDOW_TILE(DJINN_MENU_ARROW_TILE) | TILE_HFLIP, 9, 0, page);
		else
			mnWindowSetTilemapEntry(
				window, WINDOW_TILE(DJINN_MENU_ARROW_TILE + 1) | TILE_HFLIP, 9, 0, page);

		mnStringFormatNumberToHalfwords(numberText, unit->hp);
		mnTextDrawWideStringInWindow(numberText + NUMBER_TEXT_FOUR_PLACES, window, 11, 1);
		if (unit->hp != oldHp)
		{
			rising = 0;
			if (unit->hp > oldHp)
				rising = 1;
			mnDjinnMenuDrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(1), rising);
		}

		mnStringFormatNumberToHalfwords(numberText, unit->pp);
		mnTextDrawWideStringInWindow(numberText + NUMBER_TEXT_FOUR_PLACES, window, 11, 2);
		if (unit->pp != oldPp)
		{
			rising = 0;
			if (unit->pp > oldPp)
				rising = 1;
			mnDjinnMenuDrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(2), rising);
		}

		mnStringFormatNumberToHalfwords(numberText, unit->attack);
		mnTextDrawWideStringInWindow(numberText + NUMBER_TEXT_FOUR_PLACES, window, 11, 3);
		if (unit->attack != oldAtk)
		{
			rising = 0;
			if (unit->attack > oldAtk)
				rising = 1;
			mnDjinnMenuDrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(3), rising);
		}

		mnStringFormatNumberToHalfwords(numberText, unit->defense);
		mnTextDrawWideStringInWindow(numberText + NUMBER_TEXT_FOUR_PLACES, window, 11, 4);
		if (unit->defense != oldDefense)
		{
			rising = 0;
			if (unit->defense > oldDefense)
				rising = 1;
			mnDjinnMenuDrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(4), rising);
		}

		mnStringFormatNumberToHalfwords(numberText, unit->agility);
		mnTextDrawWideStringInWindow(numberText + NUMBER_TEXT_FOUR_PLACES, window, 11, 5);
		if (unit->agility != oldAgi)
		{
			rising = 0;
			if (unit->agility > oldAgi)
				rising = 1;
			mnDjinnMenuDrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(5), rising);
		}

		mnStringFormatNumberToHalfwords(numberText, unit->luck);
		mnTextDrawWideStringInWindow(numberText + NUMBER_TEXT_THREE_PLACES, window, 12, 6);
		if (unit->luck != savedUnit->luck)
		{
			rising = 0;
			if (unit->luck > savedUnit->luck)
				rising = 1;
			mnDjinnMenuDrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(6), rising);
		}
	}

	// Previewing must not change the party member's Djinn, class or battle stats.
	syWordsCopy(unit, savedUnit, sizeof(*savedUnit));
	syHeapRelease(changedAbilities);
	syHeapRelease(savedUnit);
	syHeapRelease(numberText);
	return window;
}
