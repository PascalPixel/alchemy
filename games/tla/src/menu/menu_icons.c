/* The stat arrows and the other icons the menus load into VRAM blocks, the
   party and Djinn figures, the five status icons, and the statuses the Status
   screens list. Part of menu.c. */

/* Loads the Djinn menu's up and down arrows (djinn_arrow_up and
   djinn_arrow_down) into free VRAM cache slots, the state's resourceSlots, for
   mnIconCreateDjinnArrow. */
void mnLoadDjinnArrows(void)
{
	Sint32			 firstSlot;
	Sint32			 secondSlot;
	ITEM_MENU_STATE *activeState;

	activeState = Ram_HeapSlots->menuState;
	firstSlot = syVRAMCacheFindFreeEntry();
	activeState->resourceSlots[0] = (Sint16)firstSlot;
	if(firstSlot != -1) {
		mnVRAMBlockLoadResourceStub(
			firstSlot, ICON_TILE_BYTES, RESOURCE_ITEM_MENU_TILES_A);
	}
	secondSlot = syVRAMCacheFindFreeEntry();
	activeState->resourceSlots[1] = (Sint16)secondSlot;
	if(secondSlot != -1) {
		mnVRAMBlockLoadResourceStub(
			secondSlot, ICON_TILE_BYTES, RESOURCE_ITEM_MENU_TILES_B);
	}
}

/* Frees the VRAM cache slots of the two arrows the menu loaded
   (mnLoadStatArrows or mnLoadDjinnArrows). */
void mnFreeArrows(void)
{
	ITEM_MENU_STATE *state = Ram_HeapSlots->menuState;

	syVRAMCacheFreeEntry((Uint16)state->resourceSlots[0]);
	syVRAMCacheFreeEntry((Uint16)state->resourceSlots[1]);
}

/* Decodes one icon into a temporary buffer and reloads its VRAM slot: the
   Psynergy icon of ability resourceId (mnAbilityBuildIconTilesStub), built in a
   ICON_TRANSFER in HEAP_SLOT_ICON_TRANSFER, which is released again. */
Sint32 mnIconLoadResourceIntoSlot(Sint32 resourceId, Sint32 slot)
{
	Sint32		   resourceInfo;
	Sint32		   selectedSlot;
	Sint32		   buf;
	ICON_TRANSFER *allocation;

	allocation =
		syHeapSlotAllocEWRAM(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	selectedSlot = slot;
	mnAbilityBuildIconTilesStub(resourceId, 0, &selectedSlot, &resourceInfo, 1);
	buf = syVRAMCacheReload(slot, (Sint32)allocation->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return buf;
}

/* Loads an icon into a free cache slot and creates its sprite when possible:
   ability resourceId's Psynergy icon at (x, y) of window, for the Djinn
   preview's Psynergy lists. */
void mnIconCreatePsynergyIcon(
	WINDOW *win, Sint32 x, Sint32 y, Sint32 resourceId)
{
	Sint32 slot;

	slot = syVRAMCacheFindFreeEntry();
	if(slot != VRAM_CACHE_ENTRY_COUNT) {
		mnIconLoadResourceIntoSlot(resourceId, slot);
		mnWindowSprCreateStub(slot, WINDOW_SPRITE_SIZE_16X16, win, x, y);
	}
}

/* Shows a stat change arrow, up or down (STAT_ARROW), at (x, y) of window.
   Returns 1, or -1 when no window sprite is free. */
Sint32 mnIconCreateStatChangeArrow(
	WINDOW *win, Sint32 x, Sint32 y, Sint32 variant)
{
	WINDOW_SPR		*arrow;
	Uint32			 resource;
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	if(variant == STAT_ARROW_UP)
		resource = menu->resourceSlots[0];
	else
		resource = menu->resourceSlots[1];
	arrow =
		mnWindowSprCreateStub(resource, WINDOW_SPRITE_SIZE_16X16, win, x, y);
	if(arrow == 0) {
		/* FAKEMATCH(dead): a test whose two arms are alike. */
		if(variant || x)
			return -1;
		else
			return -1;
	}
	arrow->kind = 0;
	arrow->anmStep = 0;
	arrow->motion = ICON_SHOWN;
	return 1;
}

/* As mnIconCreateStatChangeArrow, with the arrow raised 3 pixels for variant 0
   and 4 for any other. Returns 1, or -1 when no window sprite is free. */
Sint32 mnIconCreateDjinnArrow(WINDOW *win, Sint32 x, Sint32 y, Sint32 variant)
{
	WINDOW_SPR		*arrow;
	Uint32			 resource;
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	if(variant == STAT_ARROW_UP) {
		resource = menu->resourceSlots[0];
		y -= 3;
	} else {
		resource = menu->resourceSlots[1];
		y -= 4;
	}
	arrow =
		mnWindowSprCreateStub(resource, WINDOW_SPRITE_SIZE_16X16, win, x, y);
	if(arrow == 0) {
		/* FAKEMATCH(dead): a test whose two arms are alike. */
		if(variant || x)
			return -1;
		else
			return -1;
	}
	arrow->kind = 0;
	arrow->anmStep = 0;
	arrow->motion = ICON_SHOWN;
	return 1;
}

/* Moves the selection *cursor among `cnt` entries in rows of `columns`: left
   and right step and wrap, up and down move a row, the last row ending at the
   last entry. Returns 0 when it stays, 1 when it moves within its row and 2
   when it reaches another row (another page of the party members). */
Sint32 mnMoveGridCursor(Sint32 *cursor, Sint32 cnt, Sint32 columns)
{
	INPUT_STATE *input = &PerWork;
	Sint32		 result = 0;
	Sint32		 old = *cursor;
	Sint32		 pos = old;

	if(input->repeat & KEY_LEFT) {
		pos--;
		pos = (pos + cnt) % cnt;
	}
	if(input->repeat & KEY_RIGHT) {
		pos++;
		pos = (pos + cnt) % cnt;
	}
	if(input->repeat & KEY_UP) {
		if(pos - columns >= 0)
			pos -= columns;
	}
	if(input->repeat & KEY_DOWN) {
		if(pos + columns < (columns + cnt - 1) / columns * columns)
			pos += columns;
		if(pos > cnt - 1)
			pos = cnt - 1;
	}
	if(old != pos) {
		sySndPlayStub(SE_MENU_CURSOR_MOVE);
		result = 1;
	}
	if(old / columns * columns != pos / columns * columns)
		result = 2;
	*cursor = pos;
	return result;
}

/* Shows the up arrow when the page of four starting at `idx`'s page has one
   above it and the down arrow when one follows, among `cnt` entries. */
void mnSetPageArrowsUpDown(Sint32 idx, Sint32 cnt)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	WINDOW_SPR		*arrow;
	Sint32			 top = idx / 4 * 4;

	arrow = menu->pageArrows[0];
	arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
	if(top != 0) {
		arrow->motion = WINDOW_SPRITE_MOTION_HOP_UP;
		arrow->anmStep = 0;
	}
	arrow = menu->pageArrows[1];
	arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
	if(top + 4 < cnt) {
		arrow->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
		arrow->anmStep = 0;
	}
}

/* As mnSetPageArrowsUpDown, with the arrows hopping left and right: the Djinn
   menu's columns of party members. */
void mnSetPageArrowsLeftRight(Sint32 idx, Sint32 cnt)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	WINDOW_SPR		*arrow;
	Sint32			 top = idx / 4 * 4;

	arrow = menu->pageArrows[0];
	arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
	if(top != 0) {
		arrow->motion = WINDOW_SPRITE_MOTION_HOP_LEFT;
		arrow->anmStep = 0;
	}
	arrow = menu->pageArrows[1];
	arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
	if(top + 4 < cnt) {
		arrow->motion = WINDOW_SPRITE_MOTION_HOP_RIGHT;
		arrow->anmStep = 0;
	}
}

/* Sets the page arrows for `idx` and sets the chosen party member's figure
   apart from the others on its page of four: its y MENU_MEMBER_CHOSEN_Y, theirs
   MENU_MEMBER_Y. */
void mnMarkChosenMember(Sint32 unused, Sint32 idx, Sint32 cnt)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Sint32			 top = idx / 4 * 4;
	Sint32			 i;

	mnSetPageArrowsUpDown(idx, cnt);
	for(i = 0; i <= 3; i++) {
		if(top + i == idx)
			menu->memberY[i] = MENU_MEMBER_CHOSEN_Y;
		else
			menu->memberY[i] = MENU_MEMBER_Y;
	}
}

/* An event while the party figures show: places each party member's figure at
   its menu position and scale through the projected placement. */
void mnUpdatePartyFigures(void)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Sint32			 scale[2];
	FIXED			 transform[4];
	Sint32			 i;

	if(menu->memberSprsShown != 0) {
		for(i = 0; i <= 7; i++) {
			/* 114.5 less half the figure's screen y, in 16.16. */
			FIXED top = toFIXED(229) / 2 - menu->memberY[i] * (toFIXED(1) / 2);

			if(menu->memberSprs[i] != NULL) {
				scale[0] = menu->memberScale[i];
				scale[1] = menu->memberScale[i];
				transform[0] = toFIXED(menu->memberX[i]);
				transform[1] = top;
				transform[2] = toFIXED(menu->memberY[i]) + top;
				transform[3] = 0;
				fdRenderApplyProjectedPlacementAtDepthStub(menu->memberSprs[i],
					transform, scale, FACING_SOUTH, MENU_FIGURE_DEPTH);
			}
		}
	}
}

/* Returns the sprite party member id's figure shows. */
Sint32 fdPartyGetCharacterSprStub(Sint32 id);

/* mnCreatePartyFigures for two characters (-1 for none): their figures in
   entries 0 and 1, as the Djinn preview shows the pair a Djinni passes
   between. */
void mnCreatePairFigures(Sint32 first, Sint32 second)
{
	ITEM_MENU_STATE *menu = (ITEM_MENU_STATE *)Ram_HeapSlots->menuState;
	SPR				*figure;
	Sint32			 id;
	Sint32			 i;

	for(i = 0; i < 8; i++) {
		figure = menu->memberSprs[i];
		if(figure != NULL) {
			fdSprDestroyStub(figure);
			menu->memberSprs[i] = NULL;
			menu->memberSprIds[i] = -1;
		}
	}
	for(i = 0; i < 2; i++) {
		id = first;
		if(i != 0)
			id = second;
		if(id != -1) {
			menu->memberSprIds[i] = id;
			figure = fdSprCreateStub(fdPartyGetCharacterSprStub(id));
			if(figure != NULL) {
				fdSprStartAnimationStub(figure, ANIM_STAND);
				figure->image.pri = 0;
				figure->flg = 0;
			}
			menu->memberSprs[i] = figure;
		}
	}
}

/* Replaces the menu's party figures with up to four party members' figures,
   from party place first on, standing (ANIM_STAND), and counts the party. */
void mnCreatePartyFigures(Sint32 first)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Uint16			 owners[14];
	SPR				*object;
	Sint32			 i;

	for(i = 0; i < 8; i++) {
		object = menu->memberSprs[i];
		if(object != NULL) {
			fdSprDestroyStub(object);
			menu->memberSprs[i] = NULL;
			menu->memberSprIds[i] = -1;
		}
	}
	menu->sideCnt[0] = gmPartyListMembersStub(owners);
	for(i = 0; i < 4 && owners[first + i] != BATTLE_UNIT_LIST_END; i++) {
		menu->memberSprIds[i] = owners[first + i];
		object = fdSprCreateStub(fdPartyGetCharacterSprStub(owners[first + i]));
		if(object != NULL) {
			fdSprStartAnimationStub(object, ANIM_STAND);
			object->image.pri = 0;
			object->flg = 0;
		}
		menu->memberSprs[i] = object;
	}
}

/* Creates the first four party members' figures in a row from tile (originX,
   originY) inside window, three tiles apart, and starts placing them each tick
   (mnUpdatePartyFigures). */
void mnShowPartyFigures(WINDOW *win, Sint32 originX, Sint32 originY)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Sint32			 i;

	for(i = 0; i < 8; i++)
		menu->memberSprs[i] = NULL;
	mnCreatePartyFigures(0);
	for(i = 0; i <= 3; i++) {
		if(menu->memberSprs[i] != NULL) {
			menu->memberX[i] = (i * 3 + (win->x + originX)) * 8;
			menu->memberY[i] = (win->y + originY) * 8 + 16;
			menu->memberScale[i] = toFIXED(1);
		}
	}
	menu->memberSprsShown = ICON_SHOWN;
	sySetEvent(mnUpdatePartyFigures, EVENT_PASS_TICK);
}

/* Destroys the menu's party figures and stops placing them. */
void mnReleasePartyFigures(void)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	void		   **objects = menu->memberSprs;
	Sint32			 i;

	for(i = 0; i < 8; i++) {
		void *object = *objects++;

		if(object != NULL)
			fdSprDestroyStub(object);
	}
	syCloseEvent(mnUpdatePartyFigures);
}

/* Shows the Djinn figures of the top row (nonzero) or hides them (0). */
void mnDjinnFiguresSetShown(Sint32 shown)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	menu->figuresShown = shown;
}

/* An event while the Djinn figures show: places each at its position, depth and
   scale; a positive scale eases a quarter of the way to full size each tick, a
   negative one holds at its size. */
void mnDjinnFiguresUpdateTopRow(void)
{
	ITEM_MENU_STATE *menu = (ITEM_MENU_STATE *)Ram_HeapSlots->menuState;
	SPR_SCALE		 scale;
	SPR_PLACEMENT	 pos;
	Sint32			 i;

	if(menu->figuresShown == 0)
		return;
	for(i = 0; i <= 3; i++) {
		SPR *figure = menu->figures[i];

		if(figure != NULL) {
			/* 114.5 less half the figure's screen y, in 16.16. */
			FIXED placeY =
				toFIXED(229) / 2 - menu->memberY[i] * (toFIXED(1) / 2);
			Sint32 value = menu->figureScales[i];

			Sint32 next;

			if(value < 0) {
				next = -value;
				scale.x = next;
				scale.y = next;
			} else {
				next = value + (toFIXED(1) - value) / 4;
				scale.x = next;
				scale.y = next;
				menu->figureScales[i] = next;
			}
			pos.x = toFIXED(menu->figureX[i]);
			pos.height = placeY;
			pos.y = toFIXED(menu->figureY[i]) + placeY;
			pos.groundHeight = 0;
			fdRenderApplyProjectedPlacementAtDepthStub(figure, &pos, &scale,
				menu->figureFacings[i], MENU_FIGURE_DEPTH);
		}
	}
}

/* The four Djinn figures' sprites, one an element (menu_tables.c). */
extern Sint32 DjinnFiguresSpriteIds[];

/* The x a new row's figures stand at, parked below the screen
   (MENU_FIGURE_HIDDEN_Y), until their screen places them. */
#define FIGURE_START_X 16

/* Replaces the four Djinn figures with one Djinni of each element
   (DjinnFiguresSpriteIds) walking (ANIM_WALK), at FIGURE_START_X and
   MENU_FIGURE_HIDDEN_Y until the screen moves them, and starts placing them
   each tick (mnDjinnFiguresUpdateTopRow). */
void mnDjinnFiguresInitTopRow(void)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	SPR				*object;
	Sint32			 i;

	for(i = 0; i < 4; i++) {
		if(menu->figures[i] != NULL) {
			fdSprDestroyStub(menu->figures[i]);
			menu->figures[i] = NULL;
		}
	}
	for(i = 0; i < 4; i++) {
		object = fdSprCreateStub(DjinnFiguresSpriteIds[i]);
		if(object != NULL) {
			fdSprStartAnimationStub(object, ANIM_WALK);
			object->image.pri = 0;
		}
		menu->figures[i] = object;
		menu->figureX[i] = FIGURE_START_X;
		menu->figureY[i] = MENU_FIGURE_HIDDEN_Y;
		menu->figureScales[i] = toFIXED(1);
		menu->figureFacings[i] = FACING_SOUTH;
	}
	menu->figuresShown = 1;
	sySetEvent(mnDjinnFiguresUpdateTopRow, EVENT_PASS_TICK);
}

/* Destroys the four Djinn figures and stops placing them. */
void mnDjinnFiguresReleaseTopRow(void)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Sint32			 i;

	for(i = 0; i < 4; i++) {
		if(menu->figures[i] != NULL) {
			fdSprDestroyStub(menu->figures[i]);
			menu->figures[i] = NULL;
		}
	}
	syCloseEvent(mnDjinnFiguresUpdateTopRow);
}

/* Replaces Djinn figure index with a Djinni of element kind
   (DjinnFiguresSpriteIds) playing animation. */
void mnDjinnFiguresReplaceSlot(Sint32 idx, Sint32 kind, Sint32 anm)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	SPR				*object;

	if(menu->figures[idx] != NULL) {
		fdSprDestroyStub(menu->figures[idx]);
		menu->figures[idx] = NULL;
	}
	object = fdSprCreateStub(DjinnFiguresSpriteIds[kind]);
	if(object != NULL) {
		fdSprStartAnimationStub(object, anm);
		object->image.pri = 0;
	}
	menu->figures[idx] = object;
}

/* Moves Djinn figure index to screen pixel (x, y), if it exists. */
void mnDjinnFiguresSetSlotPos(Sint32 idx, Sint32 x, Sint32 y)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	if(menu->figures[idx] != NULL) {
		menu->figureX[idx] = x;
		menu->figureY[idx] = y;
	}
}

/* Turns Djinn figure index to face facing (FACING), if it exists. */
void mnDjinnFiguresSetSlotFacing(Sint32 idx, Sint32 facing)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	if(menu->figures[idx] != NULL)
		menu->figureFacings[idx] = facing;
}

/* Sets Djinn figure index's scale, if it exists: positive grows towards full
   size, negative holds at its size. */
void mnDjinnFiguresSetSlotPhase(Sint32 idx, Sint32 scale)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	if(menu->figures[idx] != NULL)
		menu->figureScales[idx] = scale;
}

/* An event: queues the five status icons for drawing while they show. */
void mnIconPushSlots(void)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	ICON_SPR		*slot;
	Sint32			 i;

	if(menu->menuIconsShown != 0) {
		slot = menu->menuIcons;
		for(i = 0; i < CHARACTER_AILMENT_COUNT; i++) {
			syOAMAddSprAtDepth(slot, OAM_DEPTH_WINDOW);
			slot++;
		}
	}
}

extern Uint16 StatusScreenStatusIcons[5];

/* Loads the five status icons (StatusScreenStatusIcons), one for each status
   mnPanelDrawEntryList lists (UNIT_STAT), into free VRAM cache slots, and
   starts drawing them (mnIconPushSlots). They are the battle Status screen's
   pictures for downed, poison, deadly poison, a cursed item and haunt. */
void mnIconLoadSlots(void)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	ICON_SPR		*slot = menu->menuIcons;
	Sint32			 entry;
	Sint32			 i;

	for(i = 0; i < CHARACTER_AILMENT_COUNT; i++) {
		entry = syVRAMCacheFindFreeEntry();
		if(entry < VRAM_CACHE_ENTRY_COUNT) {
			Uint32 *attributes;

			mnSelectionIconLoadIntoSlotStub(
				ICON_KIND_PATTERN, StatusScreenStatusIcons[i], entry, 0);
			attributes = (Uint32 *)&slot->attr;
			*attributes++ = WINDOW_SPRITE_SIZE_16X16;
			*attributes = 0;
			slot->attr.tile = VramCacheEntries[entry].ofs >> 5;
		}
		menu->menuIconSlots[i] = entry;
		slot++;
	}
	menu->menuIconsShown = ICON_SHOWN;
	sySetEvent(mnIconPushSlots, EVENT_PASS_TICK + 10);
}

/* Stops drawing the five status icons and frees their VRAM cache slots. */
void mnIconReleaseSlots(void)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	Uint16			*resource;
	Sint32			 i;

	syCloseEvent(mnIconPushSlots);
	resource = menu->menuIconSlots;
	for(i = 0; i < CHARACTER_AILMENT_COUNT; i++)
		syVRAMCacheFreeEntry(*resource++);
}

/* Moves status icon index to screen position (x, y). */
void mnIconSetSlot(Sint32 idx, Uint32 x, Sint32 y)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	menu->menuIcons[idx].attr.x = x;
	menu->menuIcons[idx].attr.y = y;
}

/* Shows the five status icons (1) or hides them (0). */
void mnIconSetActive(Sint32 active)
{
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	menu->menuIconsShown = active;
}

/* Lists the statuses enabled marks into panel from (x, y), one a 16-pixel row
   with its icon, or "Your status is normal." when there are none; the icons of
   the others go off screen. */
void mnPanelDrawEntryList(
	WINDOW *panel, Sint32 x, Sint32 y, const Uint8 *enabled)
{
	Sint32 cnt = 0;
	Sint32 iconX = panel->x * 8 + x + 8;
	Sint32 iconY = panel->y * 8 + y + 8;
	Sint32 i;

	for(i = 0; i < CHARACTER_AILMENT_COUNT; i++) {
		if(enabled[i] != 0) {
			mnIconSetSlot(i, iconX, iconY);
			mnTextDrawMessageAsTilesStub(
				(Sint32)MsgStatusNameDown + i, panel, x + 16, y);
			y += 16;
			iconY += 16;
			cnt++;
		} else {
			mnIconSetSlot(i, -16, -16);
		}
	}
	if(cnt == 0)
		mnTextDrawMessageAsTilesStub((Sint32)MsgStatusNameNormal, panel, x, y);
}

/* Fills ailments (UNIT_STAT) with party member memberId's statuses, down only
   when requested is 1, and returns how many they have. */
Sint32 mnStatScreenListStatuses(
	Uint8 *ailments, Sint32 requested, Sint32 memberId)
{
	UNIT  *unit;
	Sint32 ailmentIdx;
	Sint32 cnt;
	Sint32 poison;

	unit = syUnitGet(memberId);
	for(ailmentIdx = 0; ailmentIdx < CHARACTER_AILMENT_COUNT; ailmentIdx++)
		ailments[ailmentIdx] = 0;

	cnt = 0;
	if(unit->hp == 0 && requested == 1) {
		ailments[CHARACTER_DOWN] = requested;
		cnt = 1;
	}

	poison = unit->poison;
	if(poison != 0) {
		if(poison == 1) /* poison; any other value is venom */
			ailments[CHARACTER_POISON] = poison;
		else
			ailments[CHARACTER_VENOM] = 1;
		cnt++;
	}
	if(unit->curse != 0) {
		ailments[CHARACTER_CURSE] = 1;
		cnt++;
	}
	if(unit->haunt != 0) {
		ailments[CHARACTER_HAUNT] = 1;
		cnt++;
	}
	return cnt;
}
