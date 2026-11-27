/* The preview of what setting or removing a Djinni changes. Part of
   screens.c. */

#include "window/djinn_preview.h"

/* How many bytes of NumberTextBuffer are widened: the eleven characters
   syStringFormatDecimal writes, its terminator and the two bytes after. */
#define NUMBER_TEXT_WIDE_LENGTH			14
/* Where a stat's last four characters start in its widened text. */
#define DJINN_PREVIEW_STAT_DIGIT_OFFSET 7

/* Writes `value` into `out` as syStringFormatDecimal's eleven-character,
   right-aligned decimal text, widened to halfwords for
   mnTextDrawWideStringInWindow. `out` needs room for NUMBER_TEXT_WIDE_LENGTH
   halfwords. */
Sint32 mnStringFormatNumToHalfwords(Sint16 *out, Sint32 value)
{
	Sint16 *dst;
	Sint32	i;
	Uint8  *src;

	dst = out;
	syStringFormatDecimal(value);
	src = NumberTextBuffer;
	for(i = 0; i < NUMBER_TEXT_WIDE_LENGTH; i++) {
		*dst++ = *src++;
	}
}

/* A changed stat's arrow, right of its value on window tile row `row` (HP on
   row 1 to Luck on row 6). */
#define PREVIEW_STAT_ARROW_X	  80
#define PREVIEW_STAT_ARROW_Y(row) ((row) * 8 + 6)

/* Entries of the change list: the new list's 32 and the lost ones after. */
#define DJINN_PREVIEW_ABILITY_CAPACITY 48

/* The marks mnBattleDjinnMenuListChangedPsynergy gives an ability in its
   list. */
#define DJINN_PREVIEW_ABILITY_GAINED 0x8000 /* new with the change */
#define DJINN_PREVIEW_ABILITY_LOST	 0x4000 /* gone with the change */

/* Lists into `out` the abilities usable in battle (ABILITY_USABLE_IN_BATTLE) of
   the action list `newGrid`, in its order, marking with
   DJINN_PREVIEW_ABILITY_GAINED those `oldGrid` lacks; then appends those of
   `oldGrid` that `newGrid` lacks, marked DJINN_PREVIEW_ABILITY_LOST. Both lists
   are 32 slots, ended early by a 0 action. Returns the entries written, and the
   gained and lost counts through `gained` and `lost`. */
Sint32 mnBattleDjinnMenuListChangedPsynergy(const PSYNERGY_ENTRY *oldGrid,
	const PSYNERGY_ENTRY *newGrid, Uint16 *out, Sint32 *gained, Sint32 *lost)
{
	Sint32 i;
	Sint32 j;
	Sint32 cnt;
	Sint32 gainedCnt;
	Sint32 lostCnt;

	gainedCnt = 0;
	lostCnt = 0;
	cnt = 0;
	for(i = 0; i < PSYNERGY_SLOTS && newGrid[i].encodedAction != 0; i++) {
		if(gmAbilityGetDirectStub(newGrid[i].encodedAction)->flg &
			ABILITY_USABLE_IN_BATTLE) {
			out[cnt] = newGrid[i].encodedAction & PSYNERGY_ABILITY_MASK;
			cnt++;
			for(j = 0; j < PSYNERGY_SLOTS; j++) {
				if(((newGrid[i].encodedAction ^ oldGrid[j].encodedAction) &
					   PSYNERGY_ABILITY_MASK) == 0)
					break;
			}
			if(j == PSYNERGY_SLOTS) {
				gainedCnt++;
				out[cnt - 1] |= DJINN_PREVIEW_ABILITY_GAINED;
			}
		}
	}
	for(i = 0; i < PSYNERGY_SLOTS && oldGrid[i].encodedAction != 0; i++) {
		if(gmAbilityGetDirectStub(oldGrid[i].encodedAction)->flg &
			ABILITY_USABLE_IN_BATTLE) {
			for(j = 0; j < PSYNERGY_SLOTS; j++) {
				if(((oldGrid[i].encodedAction ^ newGrid[j].encodedAction) &
					   PSYNERGY_ABILITY_MASK) == 0)
					break;
			}
			if(j == PSYNERGY_SLOTS) {
				lostCnt++;
				out[cnt] = (oldGrid[i].encodedAction & PSYNERGY_ABILITY_MASK) |
					DJINN_PREVIEW_ABILITY_LOST;
				cnt++;
			}
		}
	}
	*gained = gainedCnt;
	*lost = lostCnt;
	return cnt;
}

/* Loads ability `abilityId`'s icon into a free VRAM cache slot and draws it as
   a 16x16 sprite at (x, y) of `win`, in pixels; does nothing when no slot is
   free. */
Sint32 mnWindowSprCreateAbilityIcon(
	WINDOW *win, Sint32 x, Sint32 y, Sint32 abilityId)
{
	Sint32 iconSlot;

	iconSlot = syVRAMCacheFindFreeEntry();
	if(iconSlot != VRAM_CACHE_ENTRY_COUNT) {
		mnAbilityIconLoadIntoSlot(abilityId, iconSlot);
		/* If the sprite pool is full, the creator releases iconSlot itself. */
		mnWindowSprCreate(iconSlot, WINDOW_SPRITE_SIZE_16X16, win, x, y);
	}
}

/* Where the line at the bottom starts: further left in French. */
#define TOTAL_LABEL_X FRENCH_OR(24, 32)

/* The stats page: Japanese makes its window a tile wider, draws the old values
   a column further left, Defense and Agility in four places as HP, PP and
   Attack (the other editions give those two three places from the next column),
   and starts the prompt under them (MsgBattleAdditionalPsynergyHelp) 16 pixels
   further right. */
#define STAT_WINDOW_WIDTH		 JAPANESE_OR(21, 20)
#define OLD_VALUE_X				 JAPANESE_OR(4, 5)
#define OLD_DEFENSE_AGILITY_TEXT JAPANESE_OR(7, 8)
#define OLD_DEFENSE_AGILITY_X	 JAPANESE_OR(4, 6)
#define PSYNERGY_PROMPT_X		 JAPANESE_OR(40, 24)

/* Shows what setting or removing Djinni `encodedDjinn` (element << 8 | index)
   would change for `member`: closes the preview window `win`, opens a new one
   and returns it, or NULL when none could be opened. Page 0 compares the class
   and stats, old against new, with an arrow by each stat that rises or falls;
   pages 1 on list, five a page, the unit's battle abilities after the change
   (unchanged ones in TEXT_COLOR_WHITE, new ones in TEXT_COLOR_YELLOW) and then
   those it would lose (TEXT_COLOR_RED), and *pageCnt is set to their number
   (at least 1). The Djinni is toggled and the unit recalculated
   (gmUnitCalcStub) only to measure: the unit is restored from a copy before it
   returns. mnBattleDjinnMenuRun calls it as the cursor moves. */
WINDOW *mnBattleDjinnMenuShowChangePreview(WINDOW *win, Sint32 member,
	Sint32 encodedDjinn, Sint32 page, Sint32 *pageCnt)
{
	UNIT			   *unit;
	UNIT			   *savedUnit;
	ABILITY_DEFINITION *ability;
	Sint16			   *numText;
	Sint32				oldHp;
	Sint32				oldPp;
	Sint32				oldAtk;
	Sint32				oldDef;
	Sint32				oldAgi;
	Uint16			   *changedAbilities;
	Sint32				abilityCnt;
	Sint32				visibleRow;
	Sint32				summaryRows;
	Sint32				visibleRows;
	Sint32				element;
	Sint32				djinnIdx;
	Sint32				pages;
	Sint32				gained;
	Sint32				lost;
	Sint32				rangePattern;
	Sint32				rising;
	Sint32				i;

	void mnDjinnMenuDrawStatArrow(Sint32 x, Sint32 y, Sint32 up)
	{
		WINDOW_SPR	*output = mnWindowSprAcquireFree();
		PREVIEW_SPR *entry;

		if(output) {
			output->motion = WINDOW_SPRITE_MOTION_STILL;
			output->kind = WINDOW_SPRITE_ICON;
			output->idx = syVRAMCacheAllocEntry(128);
			entry = (PREVIEW_SPR *)&output->spr;
			output->depth = OAM_DEPTH_WINDOW;
			output->x = 120;
			output->y = 120;
			entry->attributes.word =
				OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
			entry->tile.value = 0;
			entry->attributes.bits.x = win->x * 8 + x;
			entry->attributes.bits.y = win->y * 8 + y;
			/* FAKEMATCH(dead): advancing and restoring the captured pointer. */
			win++;
			win--;
			entry->tile.bits.idx = syVRAMCacheReload((Uint8)output->idx,
				up ? DjinnMenuArrowUpTiles : DjinnMenuArrowDownTiles);
			mnWindowSprAppendToList(&win->sprs, output);
		}
	}

	unit = syUnitGet(member);
	if(win != NULL)
		mnWindowClose(win, 1);
	if(page == 0)
		win = mnWindowCreate(0, 8, STAT_WINDOW_WIDTH, 11,
			WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
	else
		win = mnWindowCreate(
			0, 5, 21, 14, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
	if(win == NULL)
		return NULL;

	numText = syHeapAlloc(64 * sizeof(*numText));
	savedUnit = syHeapAlloc(sizeof(*savedUnit));
	changedAbilities = syHeapAllocEWRAM(
		DJINN_PREVIEW_ABILITY_CAPACITY * sizeof(*changedAbilities));
	syWordsCopyEntry(savedUnit, unit, sizeof(*savedUnit));

	element = DJINN_COMMAND_ELEMENT(encodedDjinn);
	djinnIdx = DJINN_COMMAND_INDEX(encodedDjinn);
	if(gmDjinnIsSetStub(member, element, djinnIdx) != 0)
		gmDjinnPutOnStandbyStub(member, element, djinnIdx);
	else
		gmDjinnSetStub(member, element, djinnIdx);
	gmUnitCalcStub(member);

	abilityCnt = mnBattleDjinnMenuListChangedPsynergy(
		savedUnit->psynergy, unit->psynergy, changedAbilities, &gained, &lost);
	pages = ((abilityCnt - 1) / 5) + 1;
	*pageCnt = pages;
	if(page * 5 - 5 >= abilityCnt)
		page = pages;

	if(page == 0) {
		Sint32 label = (Sint32)MsgPreviewStatLabel;

		mnTextDrawMessageAsTiles(label, win, 0, 8);
		mnTextDrawMessageAsTiles(label + 1, win, 0, 16);
		mnTextDrawMessageAsTiles(label + 2, win, 0, 24);
		mnTextDrawMessageAsTiles(label + 3, win, 0, 32);
		mnTextDrawMessageAsTiles(label + 4, win, 0, 40);
		mnTextDrawMessageAsTiles(label + 5, win, 0, 48);

		oldHp = savedUnit->hp;
		mnStringFormatNumToHalfwords(numText, oldHp);
		mnTextDrawWideStringInWindow(
			numText + DJINN_PREVIEW_STAT_DIGIT_OFFSET, win, OLD_VALUE_X, 1);
		oldPp = savedUnit->pp;
		mnStringFormatNumToHalfwords(numText, oldPp);
		mnTextDrawWideStringInWindow(
			numText + DJINN_PREVIEW_STAT_DIGIT_OFFSET, win, OLD_VALUE_X, 2);
		oldAtk = savedUnit->attack;
		mnStringFormatNumToHalfwords(numText, oldAtk);
		mnTextDrawWideStringInWindow(
			numText + DJINN_PREVIEW_STAT_DIGIT_OFFSET, win, OLD_VALUE_X, 3);
		oldDef = savedUnit->defense;
		mnStringFormatNumToHalfwords(numText, oldDef);
		mnTextDrawWideStringInWindow(
			numText + OLD_DEFENSE_AGILITY_TEXT, win, OLD_DEFENSE_AGILITY_X, 4);
		oldAgi = savedUnit->agility;
		mnStringFormatNumToHalfwords(numText, oldAgi);
		mnTextDrawWideStringInWindow(
			numText + OLD_DEFENSE_AGILITY_TEXT, win, OLD_DEFENSE_AGILITY_X, 5);
		mnStringFormatNumToHalfwords(numText, savedUnit->luck);
		mnTextDrawWideStringInWindow(numText + 8, win, OLD_VALUE_X + 1, 6);

		mnWindowDrawDividerLine(win, 0, 8, 19, 8);
		if(gained != 0 || lost != 0)
			mnTextSetCol(TEXT_COLOR_RED);
		mnTextDrawMessageAsTiles((Sint32)MsgBattleAdditionalPsynergyHelp, win,
			PSYNERGY_PROMPT_X, 64);
		mnTextSetCol(TEXT_COLOR_WHITE);
	}

	if(page > 0) {
		summaryRows = 0;
		visibleRows = abilityCnt;
		if(abilityCnt > 4)
			visibleRows = 5;
		i = page * 5 - 5;
		for(visibleRow = 0; visibleRow < visibleRows && i < abilityCnt;
			visibleRow++, i++) {
			ability = gmAbilityGetDirectStub(changedAbilities[i]);
			if(ability->element != ELEMENT_NONE)
				mnWindowSetTilemapEntry(win,
					ELEMENT_ICON_TILE + ability->element, 15, visibleRow * 2,
					0);
			rangePattern = ability->range;
			if(rangePattern == ABILITY_RANGE_ALL)
				rangePattern = RANGE_ICON_ALL;
			else
				rangePattern--;
			mnRangeIconDraw(win, 16, visibleRow * 2, rangePattern, 0);
			mnWindowSprCreateAbilityIcon((WINDOW *)win, 0, visibleRow * 16 - 4,
				changedAbilities[i] & ACTION_ID_MASK);
			if(changedAbilities[i] & DJINN_PREVIEW_ABILITY_GAINED)
				mnTextSetCol(TEXT_COLOR_YELLOW);
			else if(changedAbilities[i] & DJINN_PREVIEW_ABILITY_LOST)
				mnTextSetCol(TEXT_COLOR_RED);
			else
				mnTextSetCol(TEXT_COLOR_WHITE);
			mnTextDrawMessageAsTiles(
				(changedAbilities[i] & ACTION_ID_MASK) + (Sint32)MsgAbilityName,
				win, 16, visibleRow * 16);
			mnWindowSetTilemapEntry(
				win, WINDOW_TILE_PP_LABEL, 11, visibleRow * 2, 0);
			mnWindowSetTilemapEntry(
				win, WINDOW_TILE_PP_LABEL_END, 12, visibleRow * 2, 0);
			mnTextDrawNumInWindow(
				gmAbilityGetDirectStub(changedAbilities[i])->ppCost, 2, win,
				104, visibleRow * 16);
		}
		if(gained != 0) {
			mnTextSetCol(TEXT_COLOR_YELLOW);
			mnTextDrawMessageAsTiles(
				(Sint32)MsgPsynergyGained, win, TOTAL_LABEL_X, 80);
			summaryRows = 1;
		}
		if(lost != 0) {
			mnTextSetCol(TEXT_COLOR_RED);
			mnTextDrawMessageAsTiles((Sint32)MsgPsynergyLost, win,
				TOTAL_LABEL_X, summaryRows * 8 + 80);
			summaryRows++;
		}
		if(summaryRows == 0)
			mnTextDrawMessageAsTiles(
				(Sint32)MsgPsynergyUnchanged, win, TOTAL_LABEL_X, 80);
		mnTextSetCol(TEXT_COLOR_WHITE);
		mnTextSetCol(TEXT_COLOR_WHITE);
		mnWindowDrawDividerLine(win, 0, 10, 19, 10);
	}

	if(page == 0) {
		mnTextDrawMessageAsTiles(
			(Sint32)MsgClassName + savedUnit->classId, win, 0, 0);
		mnTextDrawMessageAsTiles(
			(Sint32)MsgClassName + unit->classId, win, 80, 0);
		/* The arrow between the classes, flipped: DJINN_MENU_ARROW_TILE blinks
		   when the class changes, the tile after it stays. */
		if(savedUnit->classId != unit->classId)
			mnWindowSetTilemapEntry(win,
				WINDOW_TILE(DJINN_MENU_ARROW_TILE) | TILE_HFLIP, 9, 0, page);
		else
			mnWindowSetTilemapEntry(win,
				WINDOW_TILE(DJINN_MENU_ARROW_TILE + 1) | TILE_HFLIP, 9, 0,
				page);

		mnStringFormatNumToHalfwords(numText, unit->hp);
		mnTextDrawWideStringInWindow(
			numText + DJINN_PREVIEW_STAT_DIGIT_OFFSET, win, 11, 1);
		if(unit->hp != oldHp) {
			rising = 0;
			if(unit->hp > oldHp)
				rising = 1;
			mnDjinnMenuDrawStatArrow(
				PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(1), rising);
		}
		mnStringFormatNumToHalfwords(numText, unit->pp);
		mnTextDrawWideStringInWindow(
			numText + DJINN_PREVIEW_STAT_DIGIT_OFFSET, win, 11, 2);
		if(unit->pp != oldPp) {
			rising = 0;
			if(unit->pp > oldPp)
				rising = 1;
			mnDjinnMenuDrawStatArrow(
				PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(2), rising);
		}
		mnStringFormatNumToHalfwords(numText, unit->attack);
		mnTextDrawWideStringInWindow(
			numText + DJINN_PREVIEW_STAT_DIGIT_OFFSET, win, 11, 3);
		if(unit->attack != oldAtk) {
			rising = 0;
			if(unit->attack > oldAtk)
				rising = 1;
			mnDjinnMenuDrawStatArrow(
				PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(3), rising);
		}
		mnStringFormatNumToHalfwords(numText, unit->defense);
		mnTextDrawWideStringInWindow(
			numText + DJINN_PREVIEW_STAT_DIGIT_OFFSET, win, 11, 4);
		if(unit->defense != oldDef) {
			rising = 0;
			if(unit->defense > oldDef)
				rising = 1;
			mnDjinnMenuDrawStatArrow(
				PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(4), rising);
		}
		mnStringFormatNumToHalfwords(numText, unit->agility);
		mnTextDrawWideStringInWindow(
			numText + DJINN_PREVIEW_STAT_DIGIT_OFFSET, win, 11, 5);
		if(unit->agility != oldAgi) {
			rising = 0;
			if(unit->agility > oldAgi)
				rising = 1;
			mnDjinnMenuDrawStatArrow(
				PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(5), rising);
		}
		mnStringFormatNumToHalfwords(numText, unit->luck);
		mnTextDrawWideStringInWindow(numText + 8, win, 12, 6);
		if(unit->luck != savedUnit->luck) {
			rising = 0;
			if(unit->luck > savedUnit->luck)
				rising = 1;
			mnDjinnMenuDrawStatArrow(
				PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(6), rising);
		}
	}

	syWordsCopyEntry(unit, savedUnit, sizeof(*savedUnit));
	syHeapRelease(changedAbilities);
	syHeapRelease(savedUnit);
	syHeapRelease(numText);
	return win;
}

#undef TOTAL_LABEL_X
#undef STAT_WINDOW_WIDTH
#undef OLD_VALUE_X
#undef OLD_DEFENSE_AGILITY_TEXT
#undef OLD_DEFENSE_AGILITY_X
#undef PSYNERGY_PROMPT_X
