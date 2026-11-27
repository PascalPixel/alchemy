// The pieces the menu screens share: windows, the status panel, numbers, icons, the party figures,
// the hand cursor, paged lists, palettes, the cancel sound and text colours. Part of menu.c.

#include "sounds.h"

// mnWindowUpdateOrCreate: keep an existing window as it is, without redrawing its saved rectangle.
// The low byte of its flags is the new window's (WINDOW_FLAG_*).
#define WINDOW_KEEP_EXISTING 0x100
#define WINDOW_STYLE_MASK    0xff

// Resumes the overlay events and tileset animations when a menu closes, and clears
// FLAG_WEATHER_HELD and FLAG_PALETTE_BLEND_HELD: undoes mnSuspendFieldEvents.
void mnResumeFieldEvents(void)
{
	syResumeOverlayEvents();
	fdTilesetAnimationResumeAllStub();
	syGameFlagClear(FLAG_WEATHER_HELD);
	syGameFlagClear(FLAG_PALETTE_BLEND_HELD);
}

// Suspends the overlay events and tileset animations while a menu is open, and sets
// FLAG_PALETTE_BLEND_HELD and FLAG_WEATHER_HELD, so that the field's palette blend and weather
// stand still behind it (see mnResumeFieldEvents, which undoes it).
Sint32 mnSuspendFieldEvents(void)
{
	syGameFlagSet(FLAG_PALETTE_BLEND_HELD);
	syGameFlagSet(FLAG_WEATHER_HELD);
	fdTilesetAnimationSuspendAllStub();
	return sySuspendOverlayEvents();
}

// Clears the whole menu state and sets its defaults: no side selected, one entry in each side, and
// a 1x1 cursor grid.
void mnInitState(Sint32 unused)
{
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;

	DMA_FILL32(0, menu, MENU_STATE_SIZE);
	menu->sideIndex[0] = -1;
	menu->sideCount[0] = 1;
	menu->sideCount[1] = 1;
	menu->columnCount  = 1;
	menu->rowCount     = 1;
}

void    mnWindowCloseStub();
WINDOW *mnWindowCreateStub(Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 style);
// Redraws a window empty, its border and fill, at its current rectangle.
void    mnWindowSpriteRedrawSavedRectStub(WINDOW *window);

// Creates *handle's window if absent and returns 1. Otherwise returns 0, redrawing its saved
// rectangle unless flags has WINDOW_KEEP_EXISTING set.
Sint32  mnWindowUpdateOrCreate(Sint32 *handle, Sint32 x, Sint32 y, Sint32 w, Sint32 h, Sint32 flags)
{
	WINDOW *window;
	Sint32  style;

	window = *handle;
	if (window != 0)
	{
		if (WINDOW_KEEP_EXISTING & flags)
		{
			return 0;
		}
		mnWindowSpriteRedrawSavedRectStub(window);
		return 0;
	}
	style   = flags & WINDOW_STYLE_MASK;
	*handle = mnWindowCreateStub(x, y, w, h, style);
	return 1;
}

// Closes *handle's window if present, erasing it at once when release is set, then sets the handle
// to NULL.
void mnWindowCloseIfOpen(void **handle, Sint32 release)
{
	if (*handle != NULL)
	{
		mnWindowCloseStub(*handle, release);
		*handle = NULL;
	}
}

// Japanese draws the name and the ailment labels 8 pixels further right, the Lv label through
// mnTextDrawStringInWindowStub, and the equip panel's "cannot equip" line 16 pixels in, as the
// class panel's is. Spanish swaps the learn panel's two lines: the note above the Psynergy's name.
#define STATUS_PANEL_NAME_X         JAPANESE_OR(40, 32)
#define STATUS_PANEL_CANNOT_EQUIP_X JAPANESE_OR(16, 0)
#define STATUS_PANEL_LEARN_NAME_Y   SPANISH_OR(56, 48)
#define STATUS_PANEL_LEARN_NOTE_Y   SPANISH_OR(48, 56)

// The tile of BG character block 0 that holds the class change's down arrow, which mnWatchStart
// blinks.
#define CLASS_CHANGE_ARROW_TILE     298

// A status panel request: the panel's kind in the low byte, and a bit to draw only its lower part.
#define STATUS_PANEL_LOWER_ONLY     0x100
#define STATUS_PANEL_KIND_MASK      0xff

// What the status panel shows in its lower part (STATUS_PANEL_KIND_MASK's bits): the experience,
// the equipment, the Psynergy learned, the class, the stats, or the equipment with the class it
// would give.
typedef enum STAT_PANEL_KIND
{
	STATUS_PANEL_EXPERIENCE  = 0,
	STATUS_PANEL_EQUIP       = 2,
	STATUS_PANEL_LEARN       = 4,
	STATUS_PANEL_CLASS       = 6,
	STATUS_PANEL_STATS       = 8,
	STATUS_PANEL_EQUIP_CLASS = 9
} STAT_PANEL_KIND;

// The status panel's messages, as its functions pass them.
#define MESSAGE_CLASS_NAME       ((Sint32)&MsgClassName)
#define MESSAGE_EXP_LABEL        ((Sint32)&MsgExpLabel)
#define MESSAGE_PANEL_STAT_LABEL ((Sint32)&MsgPanelAttackLabel)
#define MESSAGE_ABILITY_NAME     ((Sint32)&MsgAbilityName)
#define MESSAGE_POISON_LABEL     ((Sint32)&MsgPoisonLabel)
#define MESSAGE_VENOM_LABEL      ((Sint32)&MsgVenomLabel)
#define MESSAGE_CURSE_LABEL      ((Sint32)&MsgCurseLabel)
#define MESSAGE_HAUNT_LABEL      ((Sint32)&MsgHauntLabel)
#define MESSAGE_CANNOT_EQUIP     ((Sint32)&MsgCannotEquip)
#define MESSAGE_WILL_LEARN       ((Sint32)&MsgWillLearn)
#define MESSAGE_LEARNED          ((Sint32)&MsgLearned)

// The status panel's level label: `Lv`; `Nv.` in Spanish, `Niv` in French.
extern const Uint8 StatusPanelLvString[];

// Clears an area of a window's interior to the window fill: left, top, right and bottom are pixels
// inside it, and the area covers every tile they touch.
void               mnWindowClearInteriorTilesStub(
				  WINDOW *window, Sint32 left, Sint32 top, Sint32 right, Sint32 bottom);
// Releases every window sprite in a window's list and empties it.
void mnWindowSpriteClearListStub(WINDOW *window);
// Creates party member owner's portrait in window at pixel (x, y), with side's palette.
void *
mnPortraitCreateStub(Sint32 owner, Sint32 pos, Sint32 side, WINDOW *window, Sint32 x, Sint32 y);
void gmUnitUpdateClassStub(Sint32 owner);

// Draws the status panel of the Item and Psynergy screens for party member owner (window at tile
// 0,5, 13 by 12): the name, up to two statuses or the level, the party member's portrait, and below
// them what mode's panel kind asks for. slot is the inventory slot of the item the panel is about,
// for the equipment and Psynergy item panels. The first parameter, the status window, is not used:
// the window is the menu state's.
//
// Opens the window on first use and keeps it in the state's statusWindow; unless mode has
// STATUS_PANEL_LOWER_ONLY, the portrait it creates replaces the state's cursor. The equipment panel
// equips the item on the real unit for a moment and puts the unit back from a heap copy it frees
// before returning.
void mnStatusPanelDraw(WINDOW *unused, Sint32 owner, Sint32 slot, Sint32 mode)
{
	Sint32           created;
	ITEM_DEFINITION *definition;
	Uint32           item;
	ITEM_MENU_STATE *state;
	UNIT            *unit;
	WINDOW          *window;
	Sint32           ailmentRows;
	Sint32           value;
	UNIT            *backup;
	Uint8            ailments[8];

	created    = 0;
	state      = (ITEM_MENU_STATE *)RamHeapSlots->menuState;
	unit       = syUnitGet(owner);
	item       = unit->inventory[slot];
	definition = gmItemGetDirectStub(item & INVENTORY_ITEM_ID_MASK);
	if (!(mode & STATUS_PANEL_LOWER_ONLY))
		created = mnWindowUpdateOrCreate((Sint32 *)&state->statusWindow,
		                                 0,
		                                 5,
		                                 13,
		                                 12,
		                                 WINDOW_KEEP_EXISTING | WINDOW_FLAG_INSTANT);
	window = state->statusWindow;

	if (!(mode & STATUS_PANEL_LOWER_ONLY))
	{
		if (!created)
		{
			sySynch(1);
			mnWindowClearInteriorTilesStub(state->statusWindow, 0, 0, 88, 32);
		}
		mnTextDrawStringAtOffsetStub(unit->name, (Sint32)window, STATUS_PANEL_NAME_X, 0);
		mnStatusScreenListStatuses(ailments, 1, owner);
		ailmentRows = 0;
		if (ailments[CHARACTER_POISON])
		{
			mnTextDrawMessageAsTilesStub(
				MESSAGE_POISON_LABEL, window, STATUS_PANEL_NAME_X, ailmentRows * 8 + 8);
			ailmentRows++;
		}
		if (ailments[CHARACTER_VENOM])
		{
			mnTextDrawMessageAsTilesStub(
				MESSAGE_VENOM_LABEL, window, STATUS_PANEL_NAME_X, ailmentRows * 8 + 8);
			ailmentRows++;
		}
		if (ailments[CHARACTER_CURSE])
		{
			mnTextDrawMessageAsTilesStub(
				MESSAGE_CURSE_LABEL, window, STATUS_PANEL_NAME_X, ailmentRows * 8 + 8);
			ailmentRows++;
		}
		if (ailments[CHARACTER_HAUNT])
		{
			mnTextDrawMessageAsTilesStub(
				MESSAGE_HAUNT_LABEL, window, STATUS_PANEL_NAME_X, ailmentRows * 8 + 8);
			ailmentRows++;
		}
		if (ailmentRows < 2)
		{
			value = unit->level;
			MENU_DRAW_STATUS_LABEL((Uint8 *)StatusPanelLvString, (Sint32)window, 40, 16);
			mnTextDrawNumberInWindowStub(value, 4, (Sint32)window, 56, 16);
		}
	}

	if (!created)
	{
		sySynch(1);
		mnWindowClearInteriorTilesStub(state->statusWindow, 0, 32, 88, 80);
	}
	mnWindowSpriteClearListStub(window);
	if (!(mode & STATUS_PANEL_LOWER_ONLY))
		state->cursor = mnPortraitCreateStub(owner, 0, 0, window, 0, 0);

	switch (mode & STATUS_PANEL_KIND_MASK)
	{
		case STATUS_PANEL_EQUIP_CLASS:
			// Only removing an equipped item or equipping another can change the class.
			if (item & INVENTORY_EQUIPPED)
			{
				if (state->equipPreview)
					goto classChange;
				goto experience;
			}
			if (state->equipPreview)
				goto experience;
		classChange:
			value = unit->classId + MESSAGE_CLASS_NAME;
			mnTextDrawMessageAsTilesStub(value, window, 0, 32);
			if (!gmItemCanUnitEquipDirectStub(owner, item))
			{
				mnTextDrawMessageAsTilesStub(MESSAGE_CANNOT_EQUIP, window, 16, 48);
				break;
			}
			backup = syHeapAllocate(sizeof(UNIT));
			syWordsCopyEntry(backup, unit, sizeof(UNIT));
			if (state->equipPreview)
			{
				unit->inventory[slot] &= ~INVENTORY_EQUIPPED;
				gmUnitUpdateClassStub(owner);
			}
			else
			{
				gmInventoryEquipStub(owner, slot);
			}
			gmUnitCalculateStub(owner);
			// The blinking arrow down from the class now to the class the equipment would give.
			mnWindowSetTilemapEntryStub(
				(Sint32)window, WINDOW_TILE(CLASS_CHANGE_ARROW_TILE), 3, 5, 0);
			value = unit->classId + MESSAGE_CLASS_NAME;
			mnTextDrawMessageAsTilesStub(value, window, 0, 48);
			syWordsCopy(unit, backup, sizeof(UNIT));
			syHeapRelease(backup);
			break;
		case STATUS_PANEL_EXPERIENCE:
		experience:
			value = unit->classId + MESSAGE_CLASS_NAME;
			mnTextDrawMessageAsTilesStub(value, window, 0, 32);
			mnStatusPanelDrawHpPp(unit, (Sint32)window);
			value = unit->experience;
			mnTextDrawMessageAsTilesStub(MESSAGE_EXP_LABEL, window, 0, 64);
			mnTextDrawNumberInWindowStub(value, 8, (Sint32)window, 24, 72);
			break;
		case STATUS_PANEL_CLASS:
			value = unit->classId + MESSAGE_CLASS_NAME;
			mnTextDrawMessageAsTilesStub(value, window, 0, 32);
			mnStatusPanelDrawHpPp(unit, (Sint32)window);
			break;
		case STATUS_PANEL_EQUIP:
		case STATUS_PANEL_EQUIP + 1:
			if (!gmItemCanUnitEquipDirectStub(owner, item))
			{
				mnTextDrawMessageAsTilesStub(
					MESSAGE_CANNOT_EQUIP, window, STATUS_PANEL_CANNOT_EQUIP_X, 48);
				break;
			}
			backup = syHeapAllocate(sizeof(UNIT));
			syWordsCopyEntry(backup, unit, sizeof(UNIT));
			if (state->equipPreview)
				unit->inventory[slot] &= ~INVENTORY_EQUIPPED;
			else
				gmInventoryEquipStub(owner, slot);
			gmUnitCalculateStub(owner);
			mnTextDrawStatComparison(unit, backup, (Sint32)window);
			syWordsCopy(unit, backup, sizeof(UNIT));
			syHeapRelease(backup);
			break;
		case STATUS_PANEL_LEARN:
		{
			Sint32 psynergy = definition->abilityId;
			Sint32 known    = 0;
			Sint32 i;

			for (i = 0; i < PSYNERGY_SLOTS; i++)
			{
				if ((unit->psynergy[i].encodedAction & PSYNERGY_ABILITY_MASK) == psynergy)
				{
					known = 1;
					break;
				}
			}
			if (known)
			{
				mnTextDrawMessageAsTilesStub(
					psynergy + MESSAGE_ABILITY_NAME, window, 0, STATUS_PANEL_LEARN_NAME_Y);
				mnTextDrawMessageAsTilesStub(MESSAGE_LEARNED, window, 0, STATUS_PANEL_LEARN_NOTE_Y);
			}
			else
			{
				mnTextDrawMessageAsTilesStub(
					psynergy + MESSAGE_ABILITY_NAME, window, 0, STATUS_PANEL_LEARN_NAME_Y);
				mnTextDrawMessageAsTilesStub(
					MESSAGE_WILL_LEARN, window, 0, STATUS_PANEL_LEARN_NOTE_Y);
			}
			break;
		}
		case STATUS_PANEL_STATS:
		{
			Sint32 labels = MESSAGE_PANEL_STAT_LABEL;

			mnTextDrawMessageAsTilesStub(labels, window, 0, 40);     // Attack
			value = unit->attack;
			mnTextDrawNumberInWindowStub(value, 3, (Sint32)window, 64, 40);
			mnTextDrawMessageAsTilesStub(labels + 1, window, 0, 48); // Defense
			value = unit->defense;
			mnTextDrawNumberInWindowStub(value, 3, (Sint32)window, 64, 48);
			mnTextDrawMessageAsTilesStub(labels + 4, window, 0, 56); // Agility
			value = unit->agility;
			mnTextDrawNumberInWindowStub(value, 3, (Sint32)window, 64, 56);
			mnTextDrawMessageAsTilesStub(labels + 3, window, 0, 64); // Luck
			value = unit->luck;
			mnTextDrawNumberInWindowStub(value, 3, (Sint32)window, 64, 64);
			break;
		}
	}
}

#if (LANGUAGE != LANGUAGE_JAPANESE)
// The pixels the field allows each digit. The Latin font's digits are 7 pixels wide, so the digits
// themselves take a pixel less each.
#	define NUMBER_PIXELS_PER_DIGIT 8
// Where mnCountDigits would stop counting: past an Sint32's 10 digits, so it only bounds the loop.
#	define DIGIT_COUNT_LIMIT       16

// Returns how many decimal digits number has, but at least 2: a number under 10 has its one digit
// counted twice, as the units and as the leading digit.
static inline Sint32 mnCountDigits(Sint32 number)
{
	Sint32 count;

	for (count = 1; count < DIGIT_COUNT_LIMIT; count++)
	{
		number /= 10;
		if (number < 10)
		{
			break;
		}
	}
	count++; // the leading digit
	return count;
}

// Draws number right-aligned in a field ending at pixel x of window, 8 pixels a digit and at least
// two digits wide: the HP and PP figures outside Japan. Japanese pads them with spaces, its font's
// space as wide as a digit; the Latin font's space is narrower, so each figure gets a field as wide
// as its digits instead.
void mnTextDrawNumberRightAligned(Sint32 number, Sint32 window, Sint32 x, Sint32 y)
{
	Sint32 width  = mnCountDigits(number);

	// Step x back to the field's left edge; the number writer right-aligns the number in a field of
	// width characters.
	x            -= width * NUMBER_PIXELS_PER_DIGIT;
	mnTextDrawNumberAtOffsetStub(number, width, window, x, y);
}

#endif

// Draws a party member's HP and PP into window as two rows, "HP current/maximum" at pixel row 40
// and "PP current/maximum" at row 48. The current HP is yellow below a quarter of the maximum, and
// red at 0.
void mnStatusPanelDrawHpPp(UNIT *unit, Sint32 window)
{
	Sint16 value; // each figure in turn

#if (LANGUAGE != LANGUAGE_JAPANESE)
	// The label at the left and a slash at 48. A current figure ends before the slash, at 48, and a
	// maximum at 88, each in a field as wide as its digits.
	mnTextDrawStringAtOffsetStub(StatusPanelHpString, window, 0, 40);
	mnTextDrawStringInWindowStub((Uint8 *)StatusPanelSlashString, window, 48, 40);
	value = unit->hpMax;
	mnTextDrawNumberRightAligned(value, window, 88, 40);
	value = unit->hp;
	if (value < unit->hpMax >> 2)
	{ // below a quarter of maximum HP
		mnTextSetColorStub(TEXT_COLOR_YELLOW);
	}
	if (value == 0)
	{
		mnTextSetColorStub(TEXT_COLOR_RED);
	}
	mnTextDrawNumberRightAligned(value, window, 48, 40);
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
	mnTextDrawStringAtOffsetStub(StatusPanelPpString, window, 0, 48);
	mnTextDrawStringInWindowStub((Uint8 *)StatusPanelSlashString, window, 48, 48);
	value = unit->pp;
	mnTextDrawNumberRightAligned(value, window, 48, 48);
	value = unit->ppMax;
	mnTextDrawNumberRightAligned(value, window, 88, 48);
#else
	// Each row string ("HP    /" and "EP    /") holds the label and the slash,
	// one character to an 8-pixel cell, the slash in the cell at 48. Each figure is four characters
	// padded with spaces: the current one drawn from 16 at the font's widths, the maximum from 56 a
	// cell a character, ending at 88.
	mnTextDrawStringInWindowStub((Uint8 *)StatusPanelHpRowString, window, 0, 40);
	value = unit->hpMax;
	mnTextDrawNumberInWindowStub(value, 4, window, 56, 40);
	value = unit->hp;
	if (value < unit->hpMax >> 2)
	{ // below a quarter of maximum HP
		mnTextSetColorStub(TEXT_COLOR_YELLOW);
	}
	if (value == 0)
	{
		mnTextSetColorStub(TEXT_COLOR_RED);
	}
	mnTextDrawNumberAtOffsetStub(value, 4, window, 16, 40);
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
	mnTextDrawStringInWindowStub((Uint8 *)StatusPanelPpRowString, window, 0, 48);
	value = unit->pp;
	mnTextDrawNumberAtOffsetStub(value, 4, window, 16, 48);
	value = unit->ppMax;
	mnTextDrawNumberInWindowStub(value, 4, window, 56, 48);
#endif
}

Sint32 mnIconCreateStatChangeArrow(WINDOW *window, Sint32 x, Sint32 y, Sint32 variant);

// The stat arrows: the small up and down arrows the menu loads into its two VRAM cache slots
// (mnLoadStatArrows).
typedef enum STAT_ARROW
{
	STAT_ARROW_UP,
	STAT_ARROW_DOWN
} STAT_ARROW;

// Writes Attack, Defense and Agility into a status window as they are now (before), and beside each
// one that changes its new value (after) with an up or down arrow: the equipment preview.
void mnTextDrawStatComparison(const UNIT *after, const UNIT *before, Sint32 window)
{
	mnTextDrawMessageAsTilesStub((Sint32)MsgPanelAttackLabel, (WINDOW *)window, 0, 32);
	mnTextDrawNumberAtOffsetStub(before->attack, 3, window, 16, 40);
	if (after->attack != before->attack)
	{
		mnTextDrawNumberAtOffsetStub(after->attack, 3, window, 64, 40);
		if (after->attack > before->attack)
		{
			mnIconCreateStatChangeArrow(window, 44, 36, STAT_ARROW_UP);
		}
		else
		{
			mnIconCreateStatChangeArrow(window, 44, 36, STAT_ARROW_DOWN);
		}
	}
	mnTextDrawMessageAsTilesStub((Sint32)MsgPanelDefenseLabel, (WINDOW *)window, 0, 48);
	mnTextDrawNumberAtOffsetStub(before->defense, 3, window, 16, 56);
	if (after->defense != before->defense)
	{
		mnTextDrawNumberAtOffsetStub(after->defense, 3, window, 64, 56);
		if (after->defense > before->defense)
		{
			mnIconCreateStatChangeArrow(window, 44, 52, STAT_ARROW_UP);
		}
		else
		{
			mnIconCreateStatChangeArrow(window, 44, 52, STAT_ARROW_DOWN);
		}
	}
	mnTextDrawMessageAsTilesStub((Sint32)MsgAgilityLabel, (WINDOW *)window, 0, 64);
	mnTextDrawNumberAtOffsetStub(before->agility, 3, window, 16, 72);
	if (after->agility != before->agility)
	{
		mnTextDrawNumberAtOffsetStub(after->agility, 3, window, 64, 72);
		if (after->agility > before->agility)
		{
			mnIconCreateStatChangeArrow(window, 44, 68, STAT_ARROW_UP);
		}
		else
		{
			mnIconCreateStatChangeArrow(window, 44, 68, STAT_ARROW_DOWN);
		}
	}
}

// Creates a hand cursor sprite in window at (x, y), loading the hand's tiles into a free VRAM
// block.
Sint32 mnIconCreateHandCursor(WINDOW *window, Sint32 x, Sint32 y)
{
	Sint32 slot;
	Sint32 icon;

	icon = 0;
	slot = syVRAMCacheFindFreeEntry();
	if (slot != 0)
	{
		mnVRAMBlockLoadResourceStub(slot, ICON_TILE_BYTES, RESOURCE_UI_ICON_TILES);
		icon = (Sint32)mnWindowSpriteCreateStub(slot, WINDOW_SPRITE_SIZE_16X16, window, x, y);
	}
	return icon;
}

// Shows an icon at its place, unscaled.
void mnIconShow(WINDOW_SPR *icon)
{
	if (icon != NULL)
	{
		icon->motion                = ICON_SHOWN;
		icon->sprite.oam.x          = icon->x;
		icon->sprite.oam.y          = icon->y;
		icon->sprite.oam.matrix     = 0;
		icon->sprite.oam.affineMode = 0;
	}
}

// Loads the graphics of party member unit into menu entry slot 0. The calling menu argument is not
// used.
void mnReloadMemberPortrait(Sint32 menu, Sint32 unit)
{
	mnPortraitReloadStub(0, unit, 0);
}

// Opens the menu's top-left window (13 by 5 tiles) with a divider, puts the hand cursor in it,
// hidden, and adds the up and down arrow icons at its right edge when a VRAM entry is free. Returns
// the window.
WINDOW *mnCursorCreate(void *src)
{
	PSYNERGY_MENU_STATE *menu = src;
	WINDOW              *window;
	WINDOW_SPR          *icon;
	Sint32               slot;

	menu->auxiliaryWindow = NULL;
	mnWindowUpdateOrCreate((Sint32 *)&menu->auxiliaryWindow, 0, 0, 13, 5, WINDOW_FLAG_INSTANT);
	window = menu->auxiliaryWindow;
	mnWindowDrawDividerLineStub((Sint32)window, 0, 3, 12, 3);
	icon                     = (WINDOW_SPR *)mnIconCreateHandCursor(window, -8, 11);
	icon->motion             = ICON_HIDDEN;
	menu->tabIndex[0]        = -1;
	menu->sideIcon[0]        = icon;
	icon->depth              = OAM_DEPTH_FRONT - 1;
	menu->tabIndex[1]        = 0;
	menu->sideIcon[1]->depth = OAM_DEPTH_FRONT;
	slot                     = syVRAMCacheFindFreeEntry();
	if (slot < VRAM_CACHE_ENTRY_COUNT)
	{
		mnVRAMBlockLoadResourceStub(slot, ICON_TILE_BYTES, RESOURCE_SHOP_UP_ARROW_TILES);
		icon         = mnWindowSpriteCreateStub(slot, WINDOW_SPRITE_SIZE_16X16, window, 84, -4);
		icon->motion = ICON_HIDDEN;
		menu->pageArrows[0] = icon;
	}
	slot = syVRAMCacheFindFreeEntry();
	if (slot < VRAM_CACHE_ENTRY_COUNT)
	{
		mnVRAMBlockLoadResourceStub(slot, ICON_TILE_BYTES, RESOURCE_SHOP_DOWN_ARROW_TILES);
		icon         = mnWindowSpriteCreateStub(slot, WINDOW_SPRITE_SIZE_16X16, window, 84, 8);
		icon->motion = ICON_HIDDEN;
		menu->pageArrows[1] = icon;
	}
	return window;
}

// Event: each tick, places every party member sprite created by mnCreatePartyFigures at its menu
// position and scale, through the field's projected placement.
void mnPlacePartyFigures(void)
{
	PSYNERGY_MENU_STATE *menu = (PSYNERGY_MENU_STATE *)RamHeapSlots->menuState;
	SPR_SCALE            scale;
	SPR_PLACEMENT        pos;
	Sint32               i;
	Sint32               count;

	count = (Uint16)gmPartyCountMembersStub();
	for (i = 0; i < count; i++)
	{
		SPR  *figure = menu->tabSprites[i];
		// 114.5 less half the row's screen y, in 16.16.
		FIXED placeY = toFIXED(229) / 2 - menu->rowPositions[i] * (toFIXED(1) / 2);

		if (figure != 0)
		{
			figure->image.priority = 0;
			scale.x                = menu->memberScale[i];
			scale.y                = menu->memberScale[i];
			pos.x                  = toFIXED(menu->tabX[i]);
			pos.height             = placeY;
			pos.y                  = toFIXED(menu->rowPositions[i]) + placeY;
			pos.groundHeight       = 0;
			fdRenderApplyProjectedPlacementAtDepthStub(
				figure, &pos, &scale, FACING_SOUTH, MENU_FIGURE_DEPTH);
		}
	}
}

// The hand cursor's bob in x, a step every two ticks (mnCursorPos).
extern Uint8 MenuCursorBobX[];
// The hand cursor's bob in y: 0, -1, -1, -2, -2, -1, -1, 0.
extern Uint8 MenuCursorBobY[];

// The hand cursor's step through MenuCursorBobY, one every two ticks.
#define CURSOR_BOB_STEP() ((TickCount >> 1) & 7)

// Places the hand cursor at (xOffset, yOffset) inside the menu's window, bobbing.
void mnCursorPos(Sint32 xOffset, Sint32 yOffset)
{
	PSYNERGY_MENU_STATE *menu   = (PSYNERGY_MENU_STATE *)RamHeapSlots->menuState;
	WINDOW_SPR          *cursor = menu->sideIcon[0];

	cursor->sprite.oam.x        = cursor->x =
		MenuCursorBobX[CURSOR_BOB_STEP()] + xOffset + (Uint16)menu->auxiliaryWindow->x * 8 + 8;
	cursor->sprite.oam.y = cursor->y =
		MenuCursorBobY[CURSOR_BOB_STEP()] + yOffset + (Uint16)menu->auxiliaryWindow->y * 8 + 8;
}

// Slides the hand cursor from where it is to (x, y) inside the menu's top-left window over two
// ticks, in 1/16 pixel steps.
void mnCursorSlide(Sint32 x, Sint32 y)
{
	PSYNERGY_MENU_STATE *menu = (PSYNERGY_MENU_STATE *)RamHeapSlots->menuState;
	WINDOW_SPR          *cursor;
	Sint32               steps;
	Sint32               startX;
	Sint32               startY;
	Sint32               px;
	Sint32               py;
	Sint32               dx;
	Sint32               dy;

	steps = 2;
	if (menu->skipSlide != 0)
	{
		menu->skipSlide = 0;
		return;
	}
	cursor = menu->sideIcon[0];
	{
		Sint32 spriteX = cursor->sprite.oam.x;
		Sint32 spriteY = cursor->sprite.oam.y;

		cursor->x      = spriteX + 64;
		cursor->y      = spriteY + 64;
	}
	x += 64;
	y += 64;
	if (cursor->x - 8 > 0)
		cursor->x -= 8;
	py = cursor->y;
	if (py - 8 > 0)
	{
		cursor->y -= 8;
		py         = cursor->y;
	}
	startX = cursor->x << 4;
	dx     = ((x << 4) - startX + 1) / steps;
	startY = py << 4;
	dy     = ((y << 4) - startY + 1) / steps;
	px     = startX;
	py     = startY;
	do
	{
		px += dx;
		{
			Sint32 spriteX       = (px >> 4) + ((Uint16)menu->auxiliaryWindow->x << 3);

			cursor->sprite.oam.x = cursor->x = spriteX - 56;
		}
		py += dy;
		cursor->sprite.oam.y = cursor->y = (py >> 4) + ((Uint16)menu->auxiliaryWindow->y << 3) - 56;
		steps--;
		if (steps != 0)
			sySynch(1);
	} while (steps != 0);
}

void        mnPsynergyMenuPlaceEntryIcons(Sint32 x, Sint32 y, Sint32 width);

// Unused: lays the entry icons out eight to a row from (108, 40).
UNUSED void mnPsynergyMenuPlaceEntryIconsDefault(void)
{
	mnPsynergyMenuPlaceEntryIcons(108, 40, 8);
}

// Lays the menu's entry icons (up to 32) out in a grid of 16x16 cells, columns wide, from (originX,
// originY); empty slots keep their place.
void mnPsynergyMenuPlaceEntryIcons(Sint32 originX, Sint32 originY, Sint32 columns)
{
	WINDOW_SPR **slot;
	WINDOW_SPR **scan;
	Sint32       index;

	index = 0;
	slot  = ((PSYNERGY_MENU_STATE *)RamHeapSlots->menuState)->entryIcons;
	scan  = slot;
	do
	{
		if (*scan++ != NULL)
		{
			mnPsynergyMenuPlaceEntryIcon(slot, index, originX, originY, columns);
		}
		index++;
		slot++;
	} while (index < MENU_ENTRY_ICONS);
}

// Places the icon in *slot at cell index of a grid of 16x16 cells, columns wide, from (originX,
// originY), and shows it; an index past the last entry is taken as 0.
void mnPsynergyMenuPlaceEntryIcon(
	WINDOW_SPR **slot, Sint32 index, Sint32 originX, Sint32 originY, Sint32 columns)
{
	WINDOW_SPR *icon;

	if (index > MENU_ENTRY_ICONS - 1)
	{
		index = 0;
	}
	icon    = *slot;
	icon->y = (index / columns) * 16 + originY;
	icon->x = (index % columns) * 16 + originX;
	mnIconShow(icon);
}

// Places the icon in *slot at slotIndex in a columns-wide grid of 24x16 cells from (originX,
// originY), and shows it; a slotIndex past INVENTORY_SLOTS is taken as 0.
void mnItemMenuPlaceIcon(
	WINDOW_SPR **slot, Sint32 slotIndex, Sint32 originX, Sint32 originY, Sint32 columns)
{
	WINDOW_SPR *icon;

	if (slotIndex > INVENTORY_SLOTS)
		slotIndex = 0;
	icon    = *slot;
	icon->y = slotIndex / columns * 16 + originY;
	icon->x = slotIndex % columns * 24 + originX;
	mnIconShow(icon);
}

// Lays the inventory's INVENTORY_SLOTS item icons out five to a row from (116, y): y is 56 for
// layout 1 and 40 otherwise.
void mnItemMenuPlaceIcons(Sint32 layout)
{
	WINDOW_SPR     **slot;
	WINDOW_SPR     **scan;
	WINDOW_SPR      *icon;
	Sint32           index;
	Sint32           originY;
	ITEM_MENU_STATE *menu;

	menu    = RamHeapSlots->menuState;
	originY = 56;
	if (layout != 1)
	{
		originY = 40;
	}
	slot  = menu->entryIcons;
	index = 0;
	scan  = slot;
	do
	{
		icon = *scan++;
		if (icon != NULL)
		{
			mnItemMenuPlaceIcon(slot, index, 116, originY, MENU_ITEM_GRID_COLUMNS);
		}
		index++;
		slot++;
	} while (index < INVENTORY_SLOTS);
}

void mnTextGetMessageSizeStub(Sint32 msg, Sint32 *left, Sint32 *top, Sint32 *width, Sint32 *height);

// Shows message in the item menu, with the hand cursor hidden meanwhile.
//
// With y == -1 the message goes into the info window; otherwise a modal window sized to the message
// opens at (x, y). With x == -1 the message stays up and FLAG_INFO_WINDOW_HOLDS_MESSAGE records
// that the info window holds it; otherwise it waits for A, B or Start and clears it again.
void mnItemMenuShowMessage(Sint32 msg, Sint32 x, Sint32 y)
{
	ITEM_MENU_STATE *menu = (ITEM_MENU_STATE *)RamHeapSlots->menuState;
	WINDOW          *window;
	Sint32           height;
	Sint32           width;
	Sint32           top;
	Sint32           left;

	menu->sideIcons[0]->motion = ICON_HIDDEN;
	if (y != -1)
	{
		mnTextGetMessageSizeStub(msg, &left, &top, &width, &height);
		if (mnWindowUpdateOrCreate((Sint32 *)&menu->modalWindow,
		                           x,
		                           y,
		                           width,
		                           height,
		                           WINDOW_KEEP_EXISTING | WINDOW_FLAG_INSTANT)
		    == 0)
			mnWindowSetBounds(menu->modalWindow, x, y, width, height);
		window = menu->modalWindow;
	}
	else
	{
		window = menu->infoWindow;
	}
	mnWindowSpriteRedrawSavedRectStub((WINDOW *)window);
	mnWindowSpriteClearListStub(window);
#if LANGUAGE == LANGUAGE_JAPANESE
	mnTextDrawMessageStub(msg, (Sint32)window, 0, 0);
#else
	if (y == -1)
		mnTextDrawMessageAsTilesStub(msg, window, 0, 0);
	else
		mnTextDrawMessageStub(msg, (Sint32)window, 0, 0);
#endif
	if (x != -1)
	{
		sySynch(1);
		do
		{
			sySynch(1);
		} while (!(PerWork.push & KEY_A) && !(PerWork.push & KEY_B) && !(PerWork.push & KEY_START));
		mnWindowSpriteClearListStub(window);
	}
	else
	{
		syGameFlagSet(FLAG_INFO_WINDOW_HOLDS_MESSAGE);
	}
	menu->completionFlag       = 1;
	menu->sideIcons[0]->motion = ICON_SHOWN;
#if ITEM_MENU_CLOSES_MESSAGE_WINDOW
	if (y != -1)
		mnWindowCloseIfOpen((void **)&menu->modalWindow, 1);
#endif
}

// The most bytes a list order copies, its LIST_ORDER_END included.
#define LIST_ORDER_MAX_LENGTH 32

void   mnCopyListOrder(Sint32 mode, Uint8 *order);

// Sorts a party member's INVENTORY_SLOTS inventory slots by the list order of mode: items are
// grouped by the order's item types and, within a type, run from the highest item id down.
Sint32 mnItemMenuSort(Uint16 *inventory, Sint32 mode)
{
	Uint16           sorted[16];
	Uint16           items[16];
	Uint8            order[LIST_ORDER_MAX_LENGTH];
	ITEM_DEFINITION *item;
	Sint32           pass;
	Sint32           i;
	Sint32           count;
	Sint32           best;
	Sint32           bestSlot;
	Sint32           sortedCount;
	Sint32           value;

	bestSlot    = 0;
	sortedCount = 0;
	mnCopyListOrder(mode, order);

	for (i = 0; i < INVENTORY_SLOTS; i++)
		items[i] = inventory[i];

	count = 0;
	for (i = 0; i < INVENTORY_SLOTS; i++)
	{
		if (items[i] != 0)
			count++;
	}

	for (i = count; i < INVENTORY_SLOTS; i++)
		sorted[i] = 0;

	// One pass for each entry of the order: move its items across, the highest id first, until none
	// is left.
	for (pass = 0; order[pass] != LIST_ORDER_END; pass++)
	{
		for (;;)
		{
			best = 0;
			for (i = 0; i < count; i++)
			{
				if (items[i] == 0)
					continue;
				item = gmItemGetDirectStub(items[i]);
				if ((order[pass] & LIST_ORDER_TYPE_MASK) != item->type)
					continue;
				if ((order[pass] & LIST_ORDER_EQUIPPED) && (items[i] & INVENTORY_EQUIPPED) == 0)
					continue;
				value = items[i] & INVENTORY_ITEM_ID_MASK;
				if (best < value)
				{
					bestSlot = i;
					best     = value;
				}
			}
			if (best == 0)
				break;
			sorted[sortedCount] = items[bestSlot];
			items[bestSlot]     = 0;
			sortedCount++;
		}
	}

	for (i = 0; i < count; i++)
		inventory[i] = sorted[i];
	return 1;
}

// Copies the list order of mode (LIST_SORT; any other mode takes LIST_SORT_ITEMS_FIRST's) into
// order, up to and including its LIST_ORDER_END and at most LIST_ORDER_MAX_LENGTH bytes.
void mnCopyListOrder(Sint32 mode, Uint8 *order)
{
	Uint8 *src;
	Sint32 n;

	src = MenuListOrderItemsFirst;
	switch (mode)
	{
		case LIST_SORT_EQUIPPED_THEN_ITEMS    : src = MenuListOrderEquippedThenItems; break;
		case LIST_SORT_EQUIPPED_THEN_EQUIPMENT: src = MenuListOrderEquippedThenEquipment; break;
		case LIST_SORT_EQUIPMENT_FIRST        : src = MenuListOrderEquipmentFirst; break;
	}
	*order = *src;
	n      = 0;
	if (*order != LIST_ORDER_END)
	{
		do
		{
			if (++n >= LIST_ORDER_MAX_LENGTH)
				break;
			src++;
			order++;
			*order = *src;
		} while (*order != LIST_ORDER_END);
	}
}

// Moves a paged list's cursor and page with the d-pad's repeating keys.
//
// count entries are shown pageSize at a time; *cursor is the row on the page and *page the page.
// Vertical lists (horizontal 0) move the cursor with Up and Down and turn pages with Left and
// Right; horizontal lists the other way round. Moving past either end wraps round; turning to a
// shorter page pulls the cursor up to its last entry. Returns 1 after a page turn, 0 after a cursor
// move and -1 when nothing moved or the list is empty.
Sint32 mnPagedListHandleInput(
	Sint32 horizontal, Sint32 count, Sint32 pageSize, Sint32 *cursor, Sint32 *page)
{
	Sint32 pages;
	Sint32 next;
	Sint32 prev;
	Sint32 pageForward;
	Sint32 result;

	result = -1;
	if (count == 0)
		return result;
	// Animate the page arrows.
	mnPageArrowsDrawStub(PAGE_ARROW_VRAM);
	pages = count / pageSize;
	if (count % pageSize != 0)
		pages++;
	// horizontal is reused for the page-back key.
	if (horizontal)
	{
		next        = PerWork.repeat & KEY_RIGHT;
		prev        = PerWork.repeat & KEY_LEFT;
		horizontal  = PerWork.repeat & KEY_UP;
		pageForward = PerWork.repeat & KEY_DOWN;
	}
	else
	{
		next        = PerWork.repeat & KEY_DOWN;
		prev        = PerWork.repeat & KEY_UP;
		horizontal  = PerWork.repeat & KEY_LEFT;
		pageForward = PerWork.repeat & KEY_RIGHT;
	}
	if (horizontal)
	{
		sySoundPlayStub(SE_MENU_CURSOR_MOVE);
		if (--*page < 0)
			*page = pages - 1;
		if (*cursor + *page * pageSize > count - 1)
		{
			*cursor = count - *page * pageSize - 1;
			if (*cursor > pageSize - 1)
				*cursor = pageSize - 1;
		}
		syInputResetKeyRepeat();
		result = 1;
		return result;
	}
	if (pageForward)
	{
		sySoundPlayStub(SE_MENU_CURSOR_MOVE);
		if (++*page > pages - 1)
			*page = 0;
		if (*cursor + *page * pageSize > count - 1)
		{
			*cursor = count - *page * pageSize - 1;
			if (*cursor > pageSize - 1)
				*cursor = pageSize - 1;
		}
		syInputResetKeyRepeat();
		result = 1;
		return result;
	}
	if (prev)
	{
		sySoundPlayStub(SE_MENU_CURSOR_MOVE);
		if (--*cursor < 0)
		{
			// DEAD: a store the next overwrites; the ROM keeps it.
			*cursor = pageSize - 1;
			*cursor = count - *page * pageSize - 1;
			if (*cursor > pageSize - 1)
				*cursor = pageSize - 1;
		}
		return 0;
	}
	result = -1;
	if (next == 0)
		return result;
	sySoundPlayStub(SE_MENU_CURSOR_MOVE);
	result = 0;
	if (++*cursor == count - *page * pageSize)
		*cursor = result;
	if (*cursor > pageSize - 1)
		*cursor = result;
	return 0;
}

// Copies WINDOW_PALETTE into background palette paletteNumber and lightens its
// WINDOW_BACKGROUND_COLOR by HIGHLIGHT_LIGHTEN_STEPS in each channel: the highlight colour of the
// menu's selected rows.
void mnPaletteLightenHighlight(Sint32 paletteNumber)
{
	Uint16 *palette = (Uint16 *)&BG_PLTT_COLOR(paletteNumber, 0);
	Uint32  red;
	Uint32  green;
	Uint32  blue;

	// Both transfers copy the same 32 bytes, first as 16 halfwords and then as 8 words.
	syDMAXCopy((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0),
	           palette,
	           DMA_COPY_HALFWORDS(PALETTE_COLORS),
	           REG_DMA3);
	syDMAXCopy(
		(const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette, DMA_COPY_WORDS(8), REG_DMA3);
	blue   = palette[WINDOW_BACKGROUND_COLOR] >> RGB_BLUE_SHIFT;
	green  = RGB_GREEN(palette[WINDOW_BACKGROUND_COLOR]);
	red    = RGB_RED(palette[WINDOW_BACKGROUND_COLOR]);
	blue  += HIGHLIGHT_LIGHTEN_STEPS;
	if (blue > RGB_LEVEL_MAX)
		blue = RGB_LEVEL_MAX;
	green += HIGHLIGHT_LIGHTEN_STEPS;
	if (green > RGB_LEVEL_MAX)
		green = RGB_LEVEL_MAX;
	red += HIGHLIGHT_LIGHTEN_STEPS;
	if (red > RGB_LEVEL_MAX)
		red = RGB_LEVEL_MAX;
	palette[WINDOW_BACKGROUND_COLOR] = RGB(red, green, blue);
}

// The tile of page 1's number; each later page's is the tile after.
#define PAGE_LABEL_FIRST 49

// Draws a paged list's page numbers on the top border of window, ending at tile rightEdge: one
// digit per page between the page arrows, selectedPage in the highlight palette and the others in
// the window's.
void mnPagedListDrawPageNumbers(
	Sint32 window, Sint32 itemCount, Sint32 pageSize, Sint32 selectedPage, Sint32 rightEdge)
{
	Sint32 pageCount;
	Sint32 page;
	Sint32 x;
	Sint32 tile;

	x         = rightEdge;
	tile      = PAGE_LABEL_FIRST;
	pageCount = itemCount / pageSize;
	if (itemCount % pageSize != 0)
		pageCount++;

	x -= pageCount;
	if (pageCount > 1)
	{
		mnWindowSetTilemapEntryStub(window, PAGE_ARROW_LEFT, x - 1, -1, 0);

		for (page = 0; page < pageCount; page++)
		{
			if (page == selectedPage)
				mnWindowSetTilemapEntryStub(window, tile, x, -1, TILEMAP_ENTRY_HIGHLIGHT);
			else
				mnWindowSetTilemapEntryStub(window, tile, x, -1, TILEMAP_ENTRY_WINDOW);
			tile++;
			x++;
		}

		mnWindowSetTilemapEntryStub(window, PAGE_ARROW_RIGHT, x, -1, 0);
	}
}

// Switches a rectangle of a window's text between WINDOW_PALETTE (normal) and HIGHLIGHT_PALETTE
// (the highlight made by mnPaletteLightenHighlight): palette is the new palette number.
void mnWindowSetHighlightRect(
	const WINDOW *window, Sint32 x, Sint32 y, Sint32 width, Sint32 height, Uint32 palette)
{
	WIN_STATE *state  = (WIN_STATE *)RamHeapSlots->windowState;

	x                += window->x + 1;
	y                += window->y + 1;
	palette           = BG_TILE_PALETTE(palette);
	if (x < 0)
	{
		width += x;
		x      = 0;
	}
	if (x + width > DISPLAY_TILE_WIDTH - 1)
	{
		width = DISPLAY_TILE_WIDTH - x;
	}
	if (y < 0)
	{
		height += y;
		y       = 0;
	}
	// BUG: tests the bottom against the screen's width but clips to its rows, so a rectangle ending
	// in rows 20 to 28 is kept whole and writes past the 32 by 20 tilemap into the windows after
	// it.
#if FIX_RECT_BOTTOM_CLIP
	if (y + height > DISPLAY_TILE_HEIGHT - 1)
	{
#else
	if (y + height > DISPLAY_TILE_WIDTH - 1)
	{
#endif
		height = DISPLAY_TILE_HEIGHT - y;
	}
	if (width > 0 && height > 0)
	{
		do
		{
			Uint16 *entry     = &state->tilemap[y][x];
			Sint32  remaining = width;
			while (remaining != 0)
			{
				Uint32 value  = *entry;
				value        &= ~BG_TILE_PALETTE(WINDOW_PALETTE ^ HIGHLIGHT_PALETTE);
				value        |= palette;
				remaining--;
				*entry = value;
				entry++;
			}
			height--;
			y++;
		} while (height != 0);
		state->dirty = 1;
	}
}

// Lends HIGHLIGHT_PALETTE to the element icons: copies the first sprite palette, the icons'
// colours, into it, then WINDOW_BACKGROUND_COLOR over its TEXT_BACKGROUND_COLOR.
void mnPaletteLoadIconsIntoHighlight(void)
{
	syDMAXCopy((const void *)OBJ_PLTT,
	           (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
	           DMA_COPY_HALFWORDS(16),
	           REG_DMA3);
	syDMAXCopy((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
	           (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
	           DMA_COPY_HALFWORDS(1),
	           REG_DMA3);
}

// Shows the icons of one page of a list, one row of 16 pixels each from (x, y): entries first to
// first + pageSize - 1, stopping early at an empty icon slot or the state's itemCount. Hides all
// the others.
void mnSetPageIcons(Sint32 pageSize, Sint32 first, Sint32 window, Sint32 x, Sint32 y)
{
	ITEM_MENU_STATE *menu = RamHeapSlots->menuState;
	WINDOW_SPR      *icon;
	Sint32           i;

	for (i = 0; i < MENU_ENTRY_ICONS; i++)
	{
		icon = menu->entryIcons[i];
		if (icon != NULL)
			icon->motion = WINDOW_SPRITE_MOTION_HIDDEN;
	}
	for (i = first;
	     i < pageSize + first && (icon = menu->entryIcons[i]) != NULL && i <= menu->itemCount - 1;
	     i++)
	{
		icon->x = x;
		icon->y = y + (i - first) * 16;
		mnIconShow(icon);
		icon->motion = WINDOW_SPRITE_MOTION_STILL;
	}
}

// Writes the party's coins and the "Coins" label into window: the info window of the Item and
// Psynergy screens' party member choice.
void mnDrawCoins(Sint32 window)
{
	mnTextDrawNumberAtOffsetStub(MainRoutineStateWork.coins, 7, window, 8, 0);
	mnTextDrawMessageAsTilesStub(MsgCoinsLabel, window, 64, 0);
	// Only the Japanese editions draw a vertical divider at tile column 12.
#if LANGUAGE == LANGUAGE_JAPANESE
	mnWindowDrawDividerLineStub(window, 12, 0, 12, 2);
#endif
}

// Moves and resizes a window, in tiles.
void mnWindowSetBounds(WINDOW *window, Sint32 x, Sint32 y, Sint32 width, Sint32 height)
{
	if (window != NULL)
	{
		window->width  = width;
		window->x      = x;
		window->height = height;
		window->y      = y;
	}
}

// Plays sound sound and returns 1.
Sint32 mnSoundPlayReturnOne(Sint32 sound)
{
	sySoundPlayStub(sound);
	return 1;
}

// An event while the menus watch for Start: blinks the class change's down arrow every four ticks,
// and on Start plays the cancel sound, sets FLAG_MENU_START_PRESSED and removes itself.
void mnWatchStart(void)
{
	Uint32       frame;
	Uint32      *node;
	INPUT_STATE *input;
	Uint32       mask;

	frame = TickCount;
	if ((frame & 3) == 0)
	{
		if (frame & 4)
			syWordsCopy(BG_CHAR_BLOCK(0) + CLASS_CHANGE_ARROW_TILE * TILE_SIZE_4BPP,
			            MenuClassChangeArrowTile,
			            TILE_SIZE_4BPP);
		else
			syWordsFill(BG_CHAR_BLOCK(0) + CLASS_CHANGE_ARROW_TILE * TILE_SIZE_4BPP,
			            TILE_SIZE_4BPP,
			            TEXT_CANVAS_BACKGROUND_WORD);
	}
	input = &PerWork;
	mask  = KEY_START;
	if (input->push & mask)
	{
		sySoundPlayStub(SE_MENU_CANCEL);
		syGameFlagSet(FLAG_MENU_START_PRESSED);
		syCloseEvent((Uint32)((Sint32)mnWatchStart));
	}
	// DEAD: walks the free window sprites to the end of their list and keeps nothing; the ROM keeps
	// the loop.
	node = (Uint32 *)&((WIN_STATE *)RamHeapSlots->windowState)->freeSprites.head;
	while ((node = (Uint32 *)*node) != 0)
		;
}

// Starts watching for Start: clears FLAG_MENU_START_PRESSED and adds mnWatchStart.
void mnStartWatchingStart(void)
{
	syGameFlagClear(FLAG_MENU_START_PRESSED);
	sySetEvent((Sint32)mnWatchStart, EVENT_PASS_TICK);
}

// Stops watching for Start, if Start has not removed the watch already.
void mnStopWatchingStart(void)
{
	if (syGameFlagGet(FLAG_MENU_START_PRESSED) == 0)
	{
		syCloseEvent((Uint32)((Sint32)mnWatchStart));
	}
}

// Unused: draws the next text in TEXT_COLOR_NORMAL.
UNUSED void mnTextSetNormalColor(void)
{
	mnTextSetColorStub(TEXT_COLOR_NORMAL);
}

// Unused: draws the next text in TEXT_COLOR_RED.
UNUSED void mnTextSetRedColor(void)
{
	mnTextSetColorStub(TEXT_COLOR_RED);
}

// Unused: draws the next text in TEXT_COLOR_YELLOW.
UNUSED void mnTextSetYellowColor(void)
{
	mnTextSetColorStub(TEXT_COLOR_YELLOW);
}

#undef STATUS_PANEL_LEARN_NOTE_Y
#undef DIGIT_COUNT_LIMIT
