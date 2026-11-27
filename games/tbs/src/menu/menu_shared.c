/* The pieces the menu screens share: windows, the status panel, numbers, icons,
   the party figures, the hand cursor, paged lists, palettes and the Start
   watch. Part of menu.c. */

#include "menu_internal.h"

/* The status panel's name and status column starts further left in the
   international editions, whose status labels are longer, and so does "Can't
   equip" (MsgCannotEquip). */
#define STATUS_PANEL_TEXT_X			JAPANESE_OR(40, 32)
#define STATUS_PANEL_CANNOT_EQUIP_X JAPANESE_OR(16, 0)

/* A Psynergy item's panel: Spanish puts "Learned" or "Will learn" above the
   Psynergy's name, the other editions below it. */
#define STATUS_PANEL_LEARN_NAME_Y	SPANISH_OR(56, 48)
#define STATUS_PANEL_LEARN_STATUS_Y SPANISH_OR(48, 56)

/* mnWindowUpdateOrCreate's flags: its own bit, which keeps an existing window
   as it is without redrawing its saved rectangle, over the low byte of
   WINDOW_FLAG_ flags a new window opens with. */
#define WINDOW_KEEP_EXISTING 0x100
#define WINDOW_ATTRS_MASK	 0xff

/* Resumes the overlay events and tileset animations when a menu closes, and
   clears FLAG_WEATHER_HELD and FLAG_PALETTE_BLEND_HELD: undoes
   mnSuspendFieldEvents. */
Sint32 mnResumeFieldEvents(void)
{
	syResumeOverlayEvents();
	fdTilesetAnimResumeAllStub();
	/* FLAG_WEATHER_HELD freezes the field weather: its particles hold their
	   tick and the storm's lightning waits (script_commands_weather.c). */
	gmFlagClearStub(FLAG_WEATHER_HELD);
	/* FLAG_PALETTE_BLEND_HELD stops fdPaletteFadeUpdate. */
	gmFlagClearStub(FLAG_PALETTE_BLEND_HELD);
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

/* Suspends the overlay events and tileset animations while a menu is open, and
   sets FLAG_PALETTE_BLEND_HELD and FLAG_WEATHER_HELD, so that the field's
   palette blend and weather stand still behind it (see mnResumeFieldEvents,
   which undoes it). */
Sint32 mnSuspendFieldEvents(void)
{
	gmFlagSetStub(FLAG_PALETTE_BLEND_HELD);
	gmFlagSetStub(FLAG_WEATHER_HELD);
	fdTilesetAnimSuspendAllStub();
	return sySuspendOverlayEvents();
}

/* Clears the whole menu state and sets its defaults: no side selected, one
   entry in each side, and a 1x1 cursor grid. */
void mnInitState(Sint32 unused)
{
	ITEM_MENU_STATE *menu = MenuState;

	DMA_FILL32(0, menu, MENU_STATE_SIZE);
	menu->sideIdx[0] = -1;
	menu->sideCnt[0] = 1;
	menu->sideCnt[1] = 1;
	menu->columnCnt = 1;
	menu->rowCnt = 1;
}

/* Opens the window kept in *handle at tile (x, y), w by h tiles, or, if it is
   already open, clears it to its saved rectangle (unless flags has
   WINDOW_KEEP_EXISTING). Returns 1 when it opened a new window. */
Sint32 mnWindowUpdateOrCreate(
	WINDOW **handle, Sint32 x, Sint32 y, Sint32 w, Sint32 h, Sint32 flg)
{
	WINDOW *win;
	Sint32	attrs;

	win = *handle;
	if(win != 0) {
		if(WINDOW_KEEP_EXISTING & flg) {
			return 0;
		}
		mnWindowSprRedrawSavedRectStub(win);
		return 0;
	}
	attrs = flg & WINDOW_ATTRS_MASK;
	*handle = mnWindowCreateStub(x, y, w, h, attrs);
	return 1;
}

/* Closes the window kept in *handle, if any, and clears the handle; nonzero
   instant closes it at once rather than shrinking it. */
void mnWindowCloseIfOpen(WINDOW **handle, Sint32 instant)
{
	if(*handle != NULL) {
		mnWindowCloseStub(*handle, instant);
		*handle = NULL;
	}
}

/* Draws the status panel of the Item and Psynergy screens for party member
   `member` (window at tile 0,5, 13 by 12): the name, up to two statuses or the
   level, the party member's portrait, and below them what mode's panel kind
   asks for. slot is the inventory slot of the item the panel is about, for the
   equipment and Psynergy item panels. */
void mnStatPanelDraw(WINDOW *unused, Sint32 member, Sint32 slot, Sint32 mode)
{
	Sint32			 created;
	ITEM_DEFINITION *def;
	Uint32			 item;
	ITEM_MENU_STATE *menu;
	UNIT			*unit;
	WINDOW			*win;
	Sint32			 ailmentRows;
	Sint32			 value;
	Uint8			 ailments[8];

	created = 0;
	menu = MenuState;
	unit = gmUnitGetStub(member);
	item = unit->inventory[slot];
	def = gmItemGetDirectStub(item & INVENTORY_ITEM_ID_MASK);
	if(!(mode & STATUS_PANEL_LOWER_ONLY))
		created = mnWindowUpdateOrCreate(&menu->statWin, 0, 5, 13, 12,
			WINDOW_KEEP_EXISTING | WINDOW_FLAG_INSTANT);
	win = menu->statWin;

	/* The name, then each status the party member has, one row each; the level
	   takes the third row when no more than one status is shown. */
	if(!(mode & STATUS_PANEL_LOWER_ONLY)) {
		/* A window that already existed still shows the last party member:
		   clear its top 88 by 32 pixels a tick later. A new window is
		   blank. */
		if(!created) {
			sySynch(1);
			mnWindowClearInteriorTilesStub(menu->statWin, 0, 0, 88, 32);
		}
		mnTextDrawStringAtOffsetStub(unit->name, win, STATUS_PANEL_TEXT_X, 0);
		mnStatScreenListStatuses(ailments, 1, member);
		ailmentRows = 0;
		if(ailments[CHARACTER_POISON]) {
			mnTextDrawMessageAsTilesStub(
				MsgPoisonLabel, win, STATUS_PANEL_TEXT_X, ailmentRows * 8 + 8);
			ailmentRows++;
		}
		if(ailments[CHARACTER_VENOM]) {
			mnTextDrawMessageAsTilesStub(
				MsgVenomLabel, win, STATUS_PANEL_TEXT_X, ailmentRows * 8 + 8);
			ailmentRows++;
		}
		if(ailments[CHARACTER_CURSE]) {
			mnTextDrawMessageAsTilesStub(
				MsgCurseLabel, win, STATUS_PANEL_TEXT_X, ailmentRows * 8 + 8);
			ailmentRows++;
		}
		if(ailments[CHARACTER_HAUNT]) {
			mnTextDrawMessageAsTilesStub(
				MsgHauntLabel, win, STATUS_PANEL_TEXT_X, ailmentRows * 8 + 8);
			ailmentRows++;
		}
		if(ailmentRows < 2) {
			value = unit->level;
			MENU_DRAW_STATUS_LABEL(StatusPanelLevelString, win, 40, 16);
			mnTextDrawNumInWindowStub(value, 4, win, 56, 16);
		}
	}

	/* Clear the lower part and redraw the party member's portrait. */
	if(!created) {
		sySynch(1);
		mnWindowClearInteriorTilesStub(menu->statWin, 0, 32, 88, 80);
	}
	mnWindowSprClearListStub(win);
	if(!(mode & STATUS_PANEL_LOWER_ONLY))
		menu->cursor = mnPortraitCreateStub(member, 0, 0, win, 0, 0);

	switch(mode & STATUS_PANEL_KIND_MASK) {
	case STATUS_PANEL_EXPERIENCE:
		value = unit->classId + MsgClassName;
		mnTextDrawMessageAsTilesStub(value, win, 0, 32);
		mnStatPanelDrawHpPp(unit, win);
		value = unit->experience;
		mnTextDrawMessageAsTilesStub(MsgExpLabel, win, 0, 64);
		mnTextDrawNumInWindowStub(value, 8, win, 24, 72);
		break;
	case STATUS_PANEL_CLASS:
		value = unit->classId + MsgClassName;
		mnTextDrawMessageAsTilesStub(value, win, 0, 32);
		mnStatPanelDrawHpPp(unit, win);
		break;
	case STATUS_PANEL_EQUIP:
	case STATUS_PANEL_EQUIP + 1: { /* draws the same panel */
		UNIT *backup;

		if(!gmItemCanUnitEquipDirectStub(member, item)) {
			mnTextDrawMessageAsTilesStub(
				MsgCannotEquip, win, STATUS_PANEL_CANNOT_EQUIP_X, 48);
			break;
		}
		/* Equip (or, previewing a removal, unequip) the item on the real unit,
		   recalculate its stats, show them against the saved copy, then put the
		   unit back as it was. */
		backup = syHeapAlloc(sizeof(UNIT));
		syWordsCopy(backup, unit, sizeof(UNIT));
		if(menu->equipPreview)
			unit->inventory[slot] &= ~INVENTORY_EQUIPPED;
		else
			gmInventoryEquipStub(member, slot);
		gmUnitCalcStub(member);
		mnTextDrawStatComparison(unit, backup, win);
		syWordsCopy(unit, backup, sizeof(UNIT));
		syHeapRelease(backup);
		break;
	}
	case STATUS_PANEL_LEARN: {
		Sint32 psynergy = def->abilityId;
		Sint32 known = 0;
		Sint32 i;

		/* Whether the party member already knows it, from any of the 32 action
		   slots. */
		for(i = 0; i < PSYNERGY_SLOTS; i++) {
			if((unit->psynergy[i].encodedAction & PSYNERGY_ABILITY_MASK) ==
				psynergy) {
				known = 1;
				break;
			}
		}
		/* The Psynergy's name and "Learned" or "Will learn"; Spanish puts the
		   label above the name. */
		if(known) {
			mnTextDrawMessageAsTilesStub(
				psynergy + MsgAbilityName, win, 0, STATUS_PANEL_LEARN_NAME_Y);
			mnTextDrawMessageAsTilesStub(
				MsgLearned, win, 0, STATUS_PANEL_LEARN_STATUS_Y);
		} else {
			mnTextDrawMessageAsTilesStub(
				psynergy + MsgAbilityName, win, 0, STATUS_PANEL_LEARN_NAME_Y);
			mnTextDrawMessageAsTilesStub(
				MsgWillLearn, win, 0, STATUS_PANEL_LEARN_STATUS_Y);
		}
		break;
	}
	case STATUS_PANEL_STATS: {
		/* The labels are consecutive messages: Attack, Defense, Status, Luck
		   and Agility. */
		Sint32 labels = MsgPanelAttackLabel;

		mnTextDrawMessageAsTilesStub(labels, win, 0, 40); /* "Attack" */
		value = unit->attack;
		mnTextDrawNumInWindowStub(value, 3, win, 64, 40);
		mnTextDrawMessageAsTilesStub(labels + 1, win, 0, 48); /* "Defense" */
		value = unit->defense;
		mnTextDrawNumInWindowStub(value, 3, win, 64, 48);
		mnTextDrawMessageAsTilesStub(labels + 4, win, 0, 56); /* "Agility" */
		value = unit->agility;
		mnTextDrawNumInWindowStub(value, 3, win, 64, 56);
		mnTextDrawMessageAsTilesStub(labels + 3, win, 0, 64); /* "Luck" */
		value = unit->luck;
		mnTextDrawNumInWindowStub(value, 3, win, 64, 64);
		break;
	}
	}
}

#if LANGUAGE != LANGUAGE_JAPANESE
/* The pixels the field allows each digit. The Latin font's digits are 7 pixels
   wide, so the digits themselves take a pixel less each. */
#define NUMBER_PIXELS_PER_DIGIT 8

/* Returns how many decimal digits number has, but at least 2: 0-99 give 2,
   100-999 give 3, 1000-9999 give 4. */
static inline Sint32 mnCountDigits(Sint32 num)
{
	Sint32 cnt;

	/* 16 only bounds the loop: an Sint32 has at most 10 digits, so the count
	   always stops first. */
	for(cnt = 1; cnt < 16; cnt++) {
		num /= 10;
		if(num < 10) {
			break;
		}
	}
	/* The leading digit. */
	cnt++;
	return cnt;
}

/* Draws number right-aligned in a field that ends at pixel column x and allows
   8 pixels for each digit, at least two digits wide. */
void mnTextDrawNumRightAligned(Sint32 num, WINDOW *win, Sint32 x, Sint32 y)
{
	Sint32 width = mnCountDigits(num);

	/* Step x back to the field's left edge; the number writer right-aligns the
	   number in a field of width characters. */
	x -= width * NUMBER_PIXELS_PER_DIGIT;
	mnTextDrawNumAtOffsetStub(num, width, win, x, y);
}

#endif

/* Draws a character's HP and PP into window as two rows, "HP current/maximum"
   at pixel row 40 and "PP current/maximum" at row 48. */
void mnStatPanelDrawHpPp(UNIT *unit, WINDOW *win)
{
	/* Each figure in turn, as an Sint16. */
	Sint16 value;

#if LANGUAGE != LANGUAGE_JAPANESE
	/* The label at the left and a slash at 48. */
	mnTextDrawStringAtOffsetStub(StatusPanelHpString, win, 0, 40);
	mnTextDrawStringInWindowStub((Uint8 *)StatusPanelSlashString, win, 48, 40);
	value = unit->hpMax;
	mnTextDrawNumRightAligned(value, win, 88, 40);
	value = unit->hp;
	if(value < unit->hpMax >> 2) { /* below a quarter of maximum HP */
		mnTextSetColStub(TEXT_COLOR_LOW_HP);
	}
	if(value == 0) {
		mnTextSetColStub(TEXT_COLOR_NO_HP);
	}
	mnTextDrawNumRightAligned(value, win, 48, 40);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	mnTextDrawStringAtOffsetStub(StatusPanelPpString, win, 0, 48);
	mnTextDrawStringInWindowStub((Uint8 *)StatusPanelSlashString, win, 48, 48);
	value = unit->pp;
	mnTextDrawNumRightAligned(value, win, 48, 48);
	value = unit->ppMax;
	mnTextDrawNumRightAligned(value, win, 88, 48);
#else
	/* Each row string ("HP    /" and "EP    /") holds the label and the slash,
	   one character to an 8-pixel cell, the slash in the cell at 48. */
	mnTextDrawStringInWindowStub((Uint8 *)StatusPanelHpRowString, win, 0, 40);
	value = unit->hpMax;
	mnTextDrawNumInWindowStub(value, 4, win, 56, 40);
	value = unit->hp;
	if(value < unit->hpMax >> 2) { /* below a quarter of maximum HP */
		mnTextSetColStub(TEXT_COLOR_LOW_HP);
	}
	if(value == 0) {
		mnTextSetColStub(TEXT_COLOR_NO_HP);
	}
	mnTextDrawNumAtOffsetStub(value, 4, win, 16, 40);
	mnTextSetColStub(TEXT_COLOR_NORMAL);
	mnTextDrawStringInWindowStub((Uint8 *)StatusPanelPpRowString, win, 0, 48);
	value = unit->pp;
	mnTextDrawNumAtOffsetStub(value, 4, win, 16, 48);
	value = unit->ppMax;
	mnTextDrawNumInWindowStub(value, 4, win, 56, 48);
#endif
}

/* The stat arrows: the up and down arrow sprites the menu loads into its two
   sprite picture cache slots (mnLoadStatArrows). */
typedef enum STAT_ARROW { STAT_ARROW_UP, STAT_ARROW_DOWN } STAT_ARROW;

/* Writes Attack, Defense and Agility into a status window as they are now
   (before), and beside each one that changes its new value (after) with an up
   or down arrow: the equipment preview. */
void mnTextDrawStatComparison(
	const UNIT *after, const UNIT *before, WINDOW *win)
{
	mnTextDrawMessageAsTilesStub(MsgPanelAttackLabel, win, 0, 32);
	mnTextDrawNumAtOffsetStub(before->attack, 3, win, 16, 40);
	if(after->attack != before->attack) {
		mnTextDrawNumAtOffsetStub(after->attack, 3, win, 64, 40);
		if(after->attack > before->attack) {
			mnIconCreateStatChangeArrow(win, 44, 36, STAT_ARROW_UP);
		} else {
			mnIconCreateStatChangeArrow(win, 44, 36, STAT_ARROW_DOWN);
		}
	}
	mnTextDrawMessageAsTilesStub(MsgPanelDefenseLabel, win, 0, 48);
	mnTextDrawNumAtOffsetStub(before->defense, 3, win, 16, 56);
	if(after->defense != before->defense) {
		mnTextDrawNumAtOffsetStub(after->defense, 3, win, 64, 56);
		if(after->defense > before->defense) {
			mnIconCreateStatChangeArrow(win, 44, 52, STAT_ARROW_UP);
		} else {
			mnIconCreateStatChangeArrow(win, 44, 52, STAT_ARROW_DOWN);
		}
	}
	mnTextDrawMessageAsTilesStub(MsgAgilityLabel, win, 0, 64);
	mnTextDrawNumAtOffsetStub(before->agility, 3, win, 16, 72);
	if(after->agility != before->agility) {
		mnTextDrawNumAtOffsetStub(after->agility, 3, win, 64, 72);
		if(after->agility > before->agility) {
			mnIconCreateStatChangeArrow(win, 44, 68, STAT_ARROW_UP);
		} else {
			mnIconCreateStatChangeArrow(win, 44, 68, STAT_ARROW_DOWN);
		}
	}
}

/* Unused: mnIconCreateHandCursor with a second argument it ignores. */
UNUSED Sint32 mnIconCreateHandCursorUnusedArgument(
	WINDOW *win, Sint32 unused, Sint32 x, Sint32 y)
{
	Sint32 block;
	Sint32 icon;
	icon = 0;
	block = syVRAMCacheFindFreeEntry();
	if(block != 0) {
		syVRAMCacheLoad(block, ICON_TILE_BYTES, IconResourceTiles);
		icon = (Sint32)mnWindowSprCreateStub(
			block, WINDOW_SPRITE_SIZE_16X16, win, x, y);
	}
	return icon;
}

/* Creates the menu's hand cursor sprite in window at (x, y), loading the hand's
   tiles into a free VRAM block. */
Sint32 mnIconCreateHandCursor(WINDOW *win, Sint32 x, Sint32 y)
{
	Sint32 block;
	Sint32 icon;

	icon = 0;
	block = syVRAMCacheFindFreeEntry();
	/* BUG: block 0 is a valid block, and syVRAMCacheFindFreeEntry says none is
	   free with VRAM_CACHE_ENTRY_COUNT (96), which mnItemMenuSelectGiveQuantity
	   tests. With none free, the sprite is created on block 96, which
	   syVRAMCacheLoad refuses; with block 0 free, no hand is made. */
#if FIX_HAND_CURSOR_FREE_BLOCK_TEST
	if(block != VRAM_CACHE_ENTRY_COUNT) {
#else
	if(block != 0) {
#endif
		syVRAMCacheLoad(block, ICON_TILE_BYTES, IconResourceTiles);
		icon = (Sint32)mnWindowSprCreateStub(
			block, WINDOW_SPRITE_SIZE_16X16, win, x, y);
	}
	return icon;
}

/* Shows a menu icon at its stored position, without rotation or scaling. */
void mnIconShow(WINDOW_SPR *object)
{
	if(object != NULL) {
		object->motion = ICON_SHOWN;
		object->spr.oam.x = object->x;
		object->spr.oam.y = object->y;
		object->spr.oam.matrix = 0;
		object->spr.oam.affineMode = 0;
	}
}

/* Loads the graphics of character unit into menu entry slot 0. The calling menu
   argument is not used. */
void mnReloadMemberPortrait(const void *unused, Sint32 unit)
{
	mnPortraitReloadStub(0, unit, 0);
}

/* Opens the menu's top-left window (13 by 5 tiles) and puts the hand cursor in
   it, hidden for now, as sideIcon[0]. */
WINDOW *mnCursorCreate(void *src)
{
	PSYNERGY_MENU_STATE *menu = src;
	WINDOW				*win;
	Sint32				 depth;
	WINDOW_SPR			*cursor;

	menu->auxiliaryWin = NULL;
	mnWindowUpdateOrCreate(
		&menu->auxiliaryWin, 0, 0, 13, 5, WINDOW_FLAG_INSTANT);
	win = menu->auxiliaryWin;
	cursor = (WINDOW_SPR *)mnIconCreateHandCursor(win, -8, 11);
	cursor->motion = ICON_HIDDEN;
	menu->tabIdx[0] = -1;
	menu->tabIdx[1] = 0;
	menu->sideIcon[0] = cursor;
	cursor->depth = OAM_DEPTH_FRONT - 1;
	/* No screen ever makes a second marker: sideIcon[1] stays NULL, as the menu
	   state was cleared, and this store and the screens' others through it land
	   in the BIOS, which ignores them. */
	menu->sideIcon[1]->depth = OAM_DEPTH_FRONT;
	return win;
}

/* Creates a standing figure for each active party member, side by side under
   the window source: origin (originX, originY) in tiles from the window's
   corner, each figure 16 + spacing pixels after the last, at full scale. */
void mnCreatePartyFigures(
	void *src, Sint32 originX, Sint32 originY, Sint32 spacing, Sint32 unused)
{
	Uint16				 memberIds[14];
	PSYNERGY_MENU_STATE *menu = MenuState;
	WINDOW				*win = src;
	Sint32				 memberCnt = (Uint16)gmPartyListMembersStub(memberIds);
	Sint32				 i;

	menu->tabCnts[0] = memberCnt;
	/* Create and position the figures before scheduling their updates. */
	for(i = 0; i < memberCnt; i++) {
		SPR *figure = fdSprCreateStub(fdPartyGetCharacterSprStub(memberIds[i]));
		if(figure != 0) {
			Sint32 ofsX;
			Sint32 winX;
			Sint32 tileX;

			menu->tabSprs[i] = figure;
			winX = win->x;
			ofsX = spacing + 16;
			ofsX *= i;
			tileX = originX + winX;
			menu->tabX[i] = tileX * 8 + ofsX;
			menu->rowPositions[i] = (originY + win->y) * 8 + 16;
			menu->memberScale[i] = toFIXED(1);
			figure->image.pri = 0;
			figure->flg = 0;
			fdSprStartAnimationStub(figure, ANIM_STAND);
		}
	}
	/* Clear the unused party slots so the event can skip them. */
	for(; i < 8; i++) {
		menu->tabSprs[i] = 0;
	}
	sySetEvent(mnPlacePartyFigures, EVENT_PASS_TICK);
}

/* Releases the party figures mnCreatePartyFigures created and stops their
   placement event. */
void mnReleasePartyFigures(void)
{
	Uint16				 memberIds[14];
	PSYNERGY_MENU_STATE *menu = MenuState;
	Sint32				 cnt;
	SPR				   **figures;
	Sint32				 i;

	cnt = (Uint16)gmPartyListMembersStub(memberIds);
	if(cnt != 0) {
		figures = menu->tabSprs;
		i = cnt;
		do {
			SPR *figure = *figures++;

			if(figure != 0) {
				fdSprDestroyStub(figure);
			}
		} while(--i != 0);
	}
	syCloseEvent(mnPlacePartyFigures);
}

/* Event: each tick, places every party member sprite created by
   mnCreatePartyFigures at its menu position and scale, through the field's
   projected placement. */
void mnPlacePartyFigures(void)
{
	PSYNERGY_MENU_STATE *menu = MenuState;
	SPR_SCALE			 scale;
	SPR_PLACEMENT		 pos;
	Sint32				 i;
	Sint32				 cnt;

	cnt = (Uint16)gmPartyCountMembersStub();
	for(i = 0; i < cnt; i++) {
		SPR *figure = menu->tabSprs[i];
		/* 482 less the row's screen y, in 16.16. */
		FIXED placeY = toFIXED(482) - toFIXED(menu->rowPositions[i]);

		if(figure != 0) {
			figure->image.pri = 0;
			scale.x = menu->memberScale[i];
			scale.y = menu->memberScale[i];
			pos.x = toFIXED(menu->tabX[i]);
			pos.height = placeY;
			/* always 482 */
			pos.y = toFIXED(menu->rowPositions[i]) + placeY;
			pos.groundHeight = 0;
			fdRenderApplyProjectedPlacementStub(
				figure, &pos, &scale, FACING_SOUTH);
		}
	}
}

/* The cursor bobs through an 8-step table, one step every two ticks. */
#define CURSOR_BOB_STEP() ((TickCount >> 1) & 7)

/* Places the hand cursor at (xOfs, yOfs) inside the menu's top-left
   window, bobbing. */
void mnCursorPos(Sint32 xOfs, Sint32 yOfs)
{
	PSYNERGY_MENU_STATE *menu = MenuState;
	WINDOW_SPR			*cursor = menu->sideIcon[0];

	cursor->spr.oam.x = cursor->x = MenuCursorBobX[CURSOR_BOB_STEP()] + xOfs +
		(menu->auxiliaryWin)->x * 8 + 8;
	cursor->spr.oam.y = cursor->y = MenuCursorBobY[CURSOR_BOB_STEP()] + yOfs +
		(menu->auxiliaryWin)->y * 8 + 8;
}

/* Slides the hand cursor from where it is to (x, y) inside the menu's top-left
   window over two ticks, in 1/16 pixel steps. */
void mnCursorSlide(Sint32 x, Sint32 y)
{

	PSYNERGY_MENU_STATE *menu = MenuState;
	WINDOW_SPR			*cursor;
	Sint32				 steps;
	Sint32				 startX;
	Sint32				 startY;
	Sint32				 fineX;
	Sint32				 fineY;
	Sint32				 stepX;
	Sint32				 stepY;

	steps = 2;
	if(menu->skipSlide != 0) {
		menu->skipSlide = 0;
		return;
	}
	cursor = menu->sideIcon[0];
	{
		Sint32 biasedX = cursor->spr.oam.x + 64;
		Sint32 biasedY = cursor->spr.oam.y + 64;

		cursor->x = biasedX;
		cursor->y = biasedY;
	}
	x += 64;
	y += 64;
	/* Take the 8-pixel window border off the start, which is in screen pixels,
	   so that it compares with the target inside the window. */
	if(cursor->x - 8 > 0)
		cursor->x -= 8;
	fineY = cursor->y;
	if(fineY - 8 > 0) {
		cursor->y -= 8;
		fineY = cursor->y;
	}
	startX = cursor->x << 4;
	stepX = ((x << 4) - startX + 1) / steps;
	startY = fineY << 4;
	stepY = ((y << 4) - startY + 1) / steps;
	fineX = startX;
	fineY = startY;
	do {
		/* 64 - 8 = 56: the bias less the window border. */
		fineX += stepX;
		cursor->spr.oam.x = cursor->x =
			(fineX >> 4) + ((menu->auxiliaryWin)->x << 3) - 56;
		fineY += stepY;
		cursor->spr.oam.y = cursor->y =
			(fineY >> 4) + ((menu->auxiliaryWin)->y << 3) - 56;
		steps--;
		if(steps != 0)
			sySynch(1);
	} while(steps != 0);
}

/* Unused: does nothing. */
UNUSED void mnCursorNoOperation(void)
{
}

/* Lays the entry icons out eight to a row from (108, 40). Nothing calls it. */
UNUSED void mnPsynergyMenuPlaceEntryIconsDefault(void)
{
	mnPsynergyMenuPlaceEntryIcons(108, 40, 8);
}

/* Lays the menu's entry icons (up to 32) out in a grid of 16x16 cells, columns
   wide, from (originX, originY); empty slots keep their place. */
void mnPsynergyMenuPlaceEntryIcons(
	Sint32 originX, Sint32 originY, Sint32 columns)
{
	WINDOW_SPR **slot;
	WINDOW_SPR **scan;
	Sint32		 idx;

	idx = 0;
	slot = ((PSYNERGY_MENU_STATE *)MenuState)->entryIcons;
	scan = slot;
	do {
		if(*scan++ != NULL) {
			mnPsynergyMenuPlaceEntryIcon(slot, idx, originX, originY, columns);
		}
		idx++;
		slot++;
	} while(idx < MENU_ENTRY_ICONS);
}

/* Places the icon in *slot at grid cell index (0 above 31) of a grid of 16x16
   cells, columns wide, from (originX, originY), and shows it. */
void mnPsynergyMenuPlaceEntryIcon(WINDOW_SPR **slot, Sint32 idx, Sint32 originX,
	Sint32 originY, Sint32 columns)
{
	WINDOW_SPR *icon;

	if(idx > MENU_ENTRY_ICONS - 1) {
		idx = 0;
	}
	icon = *slot;
	icon->y = (idx / columns) * 16 + originY;
	icon->x = (idx % columns) * 16 + originX;
	mnIconShow(icon);
}

/* Places the icon at slotIdx in a columns-wide grid of 24x16 cells starting
   at (originX, originY), and shows it. A slotIdx above INVENTORY_SLOTS, which
   the item menu never passes, is taken as 0. */
void mnItemMenuPlaceIcon(WINDOW_SPR **slot, Sint32 slotIdx, Sint32 originX,
	Sint32 originY, Sint32 columns)
{
	WINDOW_SPR *icon;

	if(slotIdx > INVENTORY_SLOTS)
		slotIdx = 0;
	icon = *slot;
	icon->y = slotIdx / columns * 16 + originY;
	icon->x = slotIdx % columns * 24 + originX;
	mnIconShow(icon);
}

/* Lays the inventory's INVENTORY_SLOTS item icons out five to a row from (116,
   y): y is 56 for layout 1 and 40 otherwise. */
void mnItemMenuPlaceIcons(Sint32 layout)
{
	WINDOW_SPR	   **slot;
	WINDOW_SPR	   **scan;
	WINDOW_SPR		*icon;
	Sint32			 idx;
	Sint32			 originY;
	ITEM_MENU_STATE *menu;

	menu = MenuState;
	originY = 56;
	if(layout != 1) {
		originY = 40;
	}
	slot = menu->entryIcons;
	idx = 0;
	scan = slot;
	do {
		icon = *scan++;
		if(icon != NULL) {
			mnItemMenuPlaceIcon(
				slot, idx, 116, originY, MENU_ITEM_GRID_COLUMNS);
		}
		idx++;
		slot++;
	} while(idx < INVENTORY_SLOTS);
}

/* Shows message in the item menu, with the hand cursor hidden meanwhile: in a
   small window at tile (x, y) fitted to it, or with y -1 in the description
   window along the bottom. With x -1 it stays there for the next screen
   (FLAG_MENU_INFO_KEPT); otherwise it waits for A, B or Start. */
void mnItemMenuShowMessage(Sint32 msg, Sint32 x, Sint32 y)
{
	ITEM_MENU_STATE *menu = MenuState;
	WINDOW			*win;
	Sint32			 height;
	Sint32			 width;
	Sint32			 top;
	Sint32			 left;

	menu->sideIcons[0]->motion = ICON_HIDDEN;
	if(y != -1) {
		mnTextGetMessageSizeStub(msg, &left, &top, &width, &height);
		if(mnWindowUpdateOrCreate(&menu->modalWin, x, y, width, height,
			   WINDOW_KEEP_EXISTING | WINDOW_FLAG_INSTANT) == 0)
			mnWindowSetBounds(menu->modalWin, x, y, width, height);
		win = menu->modalWin;
	} else {
		win = menu->descriptionWin;
	}
	mnWindowSprRedrawSavedRectStub(win);
	mnWindowSprClearListStub((void *)win);
#if LANGUAGE != LANGUAGE_JAPANESE
	if(y == -1)
		mnTextDrawMessageAsTilesStub(msg, win, 0, 0);
	else
		mnTextDrawMessageStub(msg, win, 0, 0);
#else
	mnTextDrawMessageStub(msg, win, 0, 0);
#endif
	if(x != -1) {
		sySynch(1);
		do {
			sySynch(1);
		} while(
			!(PerPush & KEY_A) && !(PerPush & KEY_B) && !(PerPush & KEY_START));
#if LANGUAGE != LANGUAGE_JAPANESE
		if(y == -1)
			mnWindowSprRedrawSavedRectStub(win);
#endif
		mnWindowSprClearListStub((void *)win);
	} else {
		gmFlagSetStub(FLAG_MENU_INFO_KEPT);
	}
	menu->completionFlg = 1;
	menu->sideIcons[0]->motion = ICON_SHOWN;
#if ITEM_MENU_CLOSES_MESSAGE_WINDOW
	if(y != -1)
		mnWindowCloseIfOpen(&menu->modalWin, 1);
#endif
}

/* The most bytes a list order copies, its LIST_ORDER_END included. */
#define LIST_ORDER_MAX_LENGTH 32

/* Sorts a party member's INVENTORY_SLOTS inventory slots by the list order of
   mode (LIST_SORT): items are grouped by the order's item types and, within a
   type, run from the highest item id down. */
Sint32 mnItemMenuSort(Uint16 *inventory, Sint32 mode)
{
	Uint16			 sorted[16];
	Uint16			 items[16];
	Uint8			 order[LIST_ORDER_MAX_LENGTH];
	ITEM_DEFINITION *item;
	Sint32			 pass;
	Sint32			 i;
	Sint32			 cnt;
	Sint32			 best;
	Sint32			 bestSlot;
	Sint32			 sortedCnt;
	Sint32			 value;

	bestSlot = 0;
	sortedCnt = 0;
	mnCopyListOrder(mode, order);

	for(i = 0; i < INVENTORY_SLOTS; i++)
		items[i] = inventory[i];

	cnt = 0;
	for(i = 0; i < INVENTORY_SLOTS; i++) {
		if(items[i] != 0)
			cnt++;
	}

	for(i = cnt; i < INVENTORY_SLOTS; i++)
		sorted[i] = 0;

	/* One pass for each entry of the order: move its items across, the highest
	   id first, until none is left. */
	for(pass = 0; order[pass] != LIST_ORDER_END; pass++) {
		for(;;) {
			best = 0;
			for(i = 0; i < cnt; i++) {
				if(items[i] == 0)
					continue;
				item = gmItemGetDirectStub(items[i]);
				if((order[pass] & LIST_ORDER_TYPE_MASK) != item->type)
					continue;
				if((order[pass] & LIST_ORDER_EQUIPPED) &&
					(items[i] & INVENTORY_EQUIPPED) == 0)
					continue;
				value = items[i] & INVENTORY_ITEM_ID_MASK;
				if(best < value) {
					bestSlot = i;
					best = value;
				}
			}
			if(best == 0)
				break;
			sorted[sortedCnt] = items[bestSlot];
			items[bestSlot] = 0;
			sortedCnt++;
		}
	}

	for(i = 0; i < cnt; i++)
		inventory[i] = sorted[i];
	return 1;
}

/* Copies the list order of mode (LIST_SORT; any other mode takes
   LIST_SORT_ITEMS_FIRST's) into order, up to and including its LIST_ORDER_END
   and at most LIST_ORDER_MAX_LENGTH bytes. */
void mnCopyListOrder(Sint32 mode, Uint8 *order)
{
	Uint8 *src;
	Sint32 n;

	src = MenuListOrderItemsFirst;
	switch(mode) {
	case LIST_SORT_EQUIPPED_THEN_ITEMS:
		src = MenuListOrderEquippedThenItems;
		break;
	case LIST_SORT_EQUIPPED_THEN_EQUIPMENT:
		src = MenuListOrderEquippedThenEquipment;
		break;
	case LIST_SORT_EQUIPMENT_FIRST:
		src = MenuListOrderEquipmentFirst;
		break;
	}
	for(n = 0; n < LIST_ORDER_MAX_LENGTH; n++) {
		if((order[n] = src[n]) == LIST_ORDER_END)
			break;
	}
}

/* The page indicator: the window font's digit tiles for pages 1, 2, ...,
   between the two page arrows. */
#define PAGE_LABEL_FIRST 49

/* Moves a paged list's cursor and page with the d-pad's repeating keys: Up and
   Down move the row and Left and Right turn the page, or, when horizontal is
   set, the other way round. Returns 1 when the page turned, 0 when the row
   moved and -1 when nothing did. */
Sint32 mnPagedListHandleInput(Sint32 horizontal, Sint32 cnt, Sint32 pageSize,
	Sint32 *cursor, Sint32 *page)
{
	Sint32 pages;
	Sint32 next;
	Sint32 prev;
	Sint32 pageBack;
	Sint32 pageForward;
	Sint32 result;

	result = -1;
	if(cnt == 0)
		return result;
	/* Animate the page arrows. */
	mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
	pages = cnt / pageSize;
	if(cnt % pageSize != 0)
		pages++;
	if(horizontal) {
		next = PerRepeat & KEY_RIGHT;
		prev = PerRepeat & KEY_LEFT;
		pageBack = PerRepeat & KEY_UP;
		pageForward = PerRepeat & KEY_DOWN;
	} else {
		next = PerRepeat & KEY_DOWN;
		prev = PerRepeat & KEY_UP;
		pageBack = PerRepeat & KEY_LEFT;
		pageForward = PerRepeat & KEY_RIGHT;
	}
	if(pageBack) {
		sySndPlayStub(SE_MENU_CURSOR_MOVE);
		if(--*page < 0)
			*page = pages - 1;
		/* Past the end of a shorter last page: onto its last entry. */
		if(*cursor + *page * pageSize > cnt - 1) {
			*cursor = cnt - *page * pageSize - 1;
			if(*cursor > pageSize - 1)
				*cursor = pageSize - 1;
		}
		syInputResetKeyRepeat();
		result = 1;
		return result;
	}
	if(pageForward) {
		sySndPlayStub(SE_MENU_CURSOR_MOVE);
		if(++*page > pages - 1)
			*page = 0;
		if(*cursor + *page * pageSize > cnt - 1) {
			*cursor = cnt - *page * pageSize - 1;
			if(*cursor > pageSize - 1)
				*cursor = pageSize - 1;
		}
		syInputResetKeyRepeat();
		result = 1;
		return result;
	}
	if(prev) {
		sySndPlayStub(SE_MENU_CURSOR_MOVE);
		/* Back past the first row wraps to the page's last entry: its last row,
		   or the list's last entry on a shorter last page. */
		if(--*cursor < 0) {
			*cursor = pageSize - 1;
			*cursor = cnt - *page * pageSize - 1;
			if(*cursor > pageSize - 1)
				*cursor = pageSize - 1;
		}
		return 0;
	}
	result = -1;
	if(next == 0)
		return result;
	/* Forward past the list's last entry or the page's last row wraps to the
	   first row. */
	sySndPlayStub(SE_MENU_CURSOR_MOVE);
	result = 0;
	if(++*cursor == cnt - *page * pageSize)
		*cursor = result;
	if(*cursor > pageSize - 1)
		*cursor = result;
	return 0;
}

/* Copies WINDOW_PALETTE into background palette paletteNum and lightens its
   WINDOW_BACKGROUND_COLOR by HIGHLIGHT_LIGHTEN_STEPS in each channel: the
   highlight colour of the menu's selected rows. */
void mnPaletteLightenHighlight(Sint32 paletteNum)
{
	Uint16 *palette = (Uint16 *)&BG_PLTT_COLOR(paletteNum, 0);
	Uint32	red;
	Uint32	green;
	Uint32	blue;

	/* Both transfers copy the same 32 bytes, first as 16 halfwords and then as
	   8 words. */
	syDMAXCopy((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
		DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	syDMAXCopy((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
		DMA_COPY_WORDS(8), REG_DMA3);
	/* BGR555: five bits a channel, 0 to 31, red lowest. */
	blue = palette[WINDOW_BACKGROUND_COLOR] >> BLUE_SHIFT;
	green = RGB_GREEN(palette[WINDOW_BACKGROUND_COLOR]);
	red = RGB_RED(palette[WINDOW_BACKGROUND_COLOR]);
	blue += HIGHLIGHT_LIGHTEN_STEPS;
	if(blue > RGB_LEVEL_MAX)
		blue = RGB_LEVEL_MAX;
	green += HIGHLIGHT_LIGHTEN_STEPS;
	if(green > RGB_LEVEL_MAX)
		green = RGB_LEVEL_MAX;
	red += HIGHLIGHT_LIGHTEN_STEPS;
	if(red > RGB_LEVEL_MAX)
		red = RGB_LEVEL_MAX;
	palette[WINDOW_BACKGROUND_COLOR] = RGB(red, green, blue);
}

/* Draws a paged list's page numbers on the top border of window, ending at tile
   rightEdge: one digit per page between the page arrows, selectedPage in
   HIGHLIGHT_PALETTE and the others in WINDOW_PALETTE. */
void mnPagedListDrawPageNumbers(WINDOW *win, Sint32 itemCnt, Sint32 pageSize,
	Sint32 selectedPage, Sint32 rightEdge)
{
	Sint32 pageCnt;
	Sint32 page;
	Sint32 x;
	Sint32 tile;

	x = rightEdge;
	tile = PAGE_LABEL_FIRST;
	pageCnt = itemCnt / pageSize;
	if(itemCnt % pageSize != 0)
		pageCnt++;

	x -= pageCnt;
	if(pageCnt > 1) {
		mnWindowSetTilemapEntryStub(
			win, PAGE_ARROW_LEFT, x - 1, -1, TILEMAP_ENTRY_PLAIN);

		for(page = 0; page < pageCnt; page++) {
			if(page == selectedPage)
				mnWindowSetTilemapEntryStub(
					win, tile, x, -1, TILEMAP_ENTRY_HIGHLIGHT);
			else
				mnWindowSetTilemapEntryStub(
					win, tile, x, -1, TILEMAP_ENTRY_WINDOW);
			tile++;
			x++;
		}

		mnWindowSetTilemapEntryStub(
			win, PAGE_ARROW_RIGHT, x, -1, TILEMAP_ENTRY_PLAIN);
	}
}

/* Switches a rectangle of a window's text between WINDOW_PALETTE (normal) and
   HIGHLIGHT_PALETTE (the highlight made by mnPaletteLightenHighlight): palette
   is the new palette number. */
void mnWindowSetHighlightRect(const WINDOW *win, Sint32 x, Sint32 y,
	Sint32 width, Sint32 height, Uint32 palette)
{
	WIN_STATE *state = WINDOW_STATE;
	Uint8	  *tilemap = (Uint8 *)state->tilemap;

	x += win->x + 1;
	y += win->y + 1;
	palette = BG_TILE_PALETTE(palette);
	if(x < 0) {
		width += x;
		x = 0;
	}
	if(x + width > DISPLAY_TILE_WIDTH - 1) {
		width = DISPLAY_TILE_WIDTH - x;
	}
	if(y < 0) {
		height += y;
		y = 0;
	}
	/* BUG: tests the bottom against the screen's width but clips to its rows,
	   so a rectangle ending in rows 20 to 28 is kept whole and writes past the
	   32 by 20 tilemap into the windows after it. */
#if FIX_RECT_BOTTOM_CLIP
	if(y + height > DISPLAY_TILE_HEIGHT - 1) {
#else
	if(y + height > DISPLAY_TILE_WIDTH - 1) {
#endif
		height = DISPLAY_TILE_HEIGHT - y;
	}
	if(width > 0 && height > 0) {
		/* Byte offset of the first entry: 32 two-byte entries a row. */
		y <<= 6;
		x = y + (x << 1);
		do {
			Uint16 *entry = (Uint16 *)(x + (Uint32)tilemap);
			Sint32	remaining = width;
			while(remaining != 0) {
				Uint32 value = *entry;
				value &= ~BG_TILE_PALETTE(WINDOW_PALETTE ^ HIGHLIGHT_PALETTE);
				value |= palette;
				remaining--;
				*entry = value;
				entry++;
			}
			height--;
			x += 64;
		} while(height != 0);
		state->dirty = 1;
	}
}

/* Lends HIGHLIGHT_PALETTE to the element icons: copies the first sprite
   palette, the icons' colours, into it, then WINDOW_BACKGROUND_COLOR over its
   TEXT_BACKGROUND_COLOR (as mnStatScreenRun does for palettes 0 and 1). */
void mnPaletteLoadIconsIntoHighlight(void)
{
	syDMAXCopy((const void *)OBJ_PLTT,
		(void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
		DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	syDMAXCopy(
		(const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
		(void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
		DMA_COPY_HALFWORDS(1), REG_DMA3);
}

/* Shows the icons of one page of a list, one row of 16 pixels each from (x, y):
   entries first to first + pageSize - 1, stopping early at an empty icon slot
   or the state's itemCount. Hides all the others. */
void mnSetPageIcons(
	Sint32 pageSize, Sint32 first, WINDOW *win, Sint32 x, Sint32 y)
{
	ITEM_MENU_STATE *menu = MenuState;
	WINDOW_SPR		*icon;
	Sint32			 i;

	for(i = 0; i < MENU_ENTRY_ICONS; i++) {
		icon = menu->entryIcons[i];
		if(icon != NULL)
			icon->motion = ICON_HIDDEN;
	}
	for(i = first; i < pageSize + first &&
		(icon = menu->entryIcons[i]) != NULL && i <= menu->itemCnt - 1;
		i++) {
		icon->x = x;
		icon->y = y + (i - first) * 16;
		mnIconShow(icon);
		icon->motion = ICON_SHOWN;
	}
}

/* Writes the party's coins and the "Coins" label into window: the info window
   of the Item and Psynergy screens' party member choice. */
void mnDrawCoins(WINDOW *win)
{
	mnTextDrawNumAtOffsetStub(
		MainRoutineStateWork.holdings.coins, 7, win, 8, 0);
	mnTextDrawMessageAsTilesStub(MsgCoinsLabel, win, 64, 0);
	/* Only the Japanese editions draw a vertical divider at tile column 12. */
#if LANGUAGE == LANGUAGE_JAPANESE
	mnWindowDrawDividerLineStub(win, 12, 0, 12, 2);
#endif
}

/* Moves and resizes a window, in tiles. */
void mnWindowSetBounds(
	WINDOW *win, Sint32 x, Sint32 y, Sint32 width, Sint32 height)
{
	if(win != NULL) {
		win->width = width;
		win->x = x;
		win->height = height;
		win->y = y;
	}
}

/* Holds back the window tilemap's upload (holdTilemapUpload), so that what a
   menu draws meanwhile appears at once when mnWindowReleaseTilemapUpload lets
   it go. Nothing calls it: the screens set holdTilemapUpload themselves. */
void mnWindowHoldTilemapUpload(void)
{
	WINDOW_STATE->holdTilemapUpload = 1;
}

/* Lets the window tilemap be uploaded again after mnWindowHoldTilemapUpload.
   Nothing calls it. */
void mnWindowReleaseTilemapUpload(void)
{
	WINDOW_STATE->holdTilemapUpload = 0;
}

/* Plays a sound and returns 1. */
Sint32 mnSndPlayReturnOne(Sint32 snd)
{
	sySndPlayStub(snd);
	return 1;
}

/* Event while a menu runs: on Start, plays the cancel sound, sets
   FLAG_MENU_START_PRESSED and removes itself. */
void mnWatchStart(void)
{
	if(PerPush & KEY_START) {
		sySndPlayStub(SE_MENU_CANCEL);
		gmFlagSetStub(FLAG_MENU_START_PRESSED);
		syCloseEvent(mnWatchStart);
	}
}

/* Starts watching for Start: clears FLAG_MENU_START_PRESSED and adds
   mnWatchStart. */
void mnStartWatchingStart(void)
{
	gmFlagClearStub(FLAG_MENU_START_PRESSED);
	sySetEvent(mnWatchStart, EVENT_PASS_TICK);
}

/* Stops watching for Start, if Start has not removed the watch already. */
void mnStopWatchingStart(void)
{
	if(gmFlagGetStub(FLAG_MENU_START_PRESSED) == 0) {
		syCloseEvent(mnWatchStart);
	}
}

/* Makes the text drawn after it TEXT_COLOR_NORMAL. Nothing calls it or the two
   after it; the status panel sets its colours with mnTextSetColStub. */
void mnWindowPaletteSetNormalTextCol(void)
{
	mnTextSetColStub(TEXT_COLOR_NORMAL);
}

/* Makes the text drawn after it TEXT_COLOR_NO_HP. */
void mnWindowPaletteSetNoHpTextCol(void)
{
	mnTextSetColStub(TEXT_COLOR_NO_HP);
}

/* Makes the text drawn after it TEXT_COLOR_LOW_HP. */
void mnWindowPaletteSetLowHpTextCol(void)
{
	mnTextSetColStub(TEXT_COLOR_LOW_HP);
}

#undef STATUS_PANEL_TEXT_X
#undef STATUS_PANEL_CANNOT_EQUIP_X
#undef STATUS_PANEL_LEARN_NAME_Y
#undef STATUS_PANEL_LEARN_STATUS_Y
#undef WINDOW_KEEP_EXISTING
#undef WINDOW_ATTRS_MASK
#undef NUMBER_PIXELS_PER_DIGIT
#undef CURSOR_BOB_STEP
#undef PAGE_LABEL_FIRST
#undef LIST_ORDER_MAX_LENGTH
