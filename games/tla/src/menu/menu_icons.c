/* @file */
/* The stat arrows and the other icons the menus load into VRAM blocks, the */
/* party and Djinn figures, the five status icons, and the statuses the */
/* Status screens list. */
/* Part of menu.c. */

/* Loads the Djinn menu's up and down arrows (djinn_arrow_up and */
/* djinn_arrow_down) into free VRAM cache slots, the state's resourceSlots, */
/* for mnIconCreateDjinnArrow. */
void mnLoadDjinnArrows(void) {
	s32 firstSlot;
	s32 secondSlot;
	ITEM_MENU_STATE *activeState;

	activeState = Ram_HeapSlots->menuState;
	firstSlot = syVramCacheFindFreeEntry();
	activeState->resourceSlots[0] = (s16)firstSlot;
	if(firstSlot != -1) {
		mnVramBlockLoadResourceStub(firstSlot, ICON_TILE_BYTES,
									RESOURCE_ITEM_MENU_TILES_A);
	}
	secondSlot = syVramCacheFindFreeEntry();
	activeState->resourceSlots[1] = (s16)secondSlot;
	if(secondSlot != -1) {
		mnVramBlockLoadResourceStub(secondSlot, ICON_TILE_BYTES,
									RESOURCE_ITEM_MENU_TILES_B);
	}
}

/* Frees the VRAM cache slots of the two arrows the menu loaded */
/* (mnLoadStatArrows or mnLoadDjinnArrows). */
void mnFreeArrows(void) {
	ITEM_MENU_STATE *state = Ram_HeapSlots->menuState;

	syVramCacheFreeEntry((u16)state->resourceSlots[0]);
	syVramCacheFreeEntry((u16)state->resourceSlots[1]);
}

/* Decodes one icon into a temporary buffer and reloads its VRAM slot: the */
/* Psynergy icon of ability resourceId (btAbilityBuildIconTilesStub), built in
 */
/* a ICON_TRANSFER in HEAP_SLOT_ICON_TRANSFER, which is released again. */
s32 mnIconLoadResourceIntoSlot(s32 resourceId, s32 slot) {
	s32 resourceInfo;
	s32 selectedSlot;
	s32 buffer;
	ICON_TRANSFER *allocation;

	allocation =
		syHeapSlotAllocEwram(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	selectedSlot = slot;
	btAbilityBuildIconTilesStub(resourceId, 0, &selectedSlot, &resourceInfo, 1);
	buffer = syVramCacheReload(slot, (s32)allocation->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return buffer;
}

/* Loads an icon into a free cache slot and creates its sprite when possible: */
/* ability resourceId's Psynergy icon at (x, y) of window, for the Djinn */
/* preview's Psynergy lists. */
void mnIconCreatePsynergyIcon(WINDOW *window, s32 x, s32 y, s32 resourceId) {
	s32 slot;

	slot = syVramCacheFindFreeEntry();
	if(slot != VRAM_CACHE_ENTRY_COUNT) {
		mnIconLoadResourceIntoSlot(resourceId, slot);
		mnWindowSprCreateStub(slot, WINDOW_SPRITE_SIZE_16X16, window, x, y);
	}
}

/* Shows a stat change arrow, up or down (STAT_ARROW), at (x, y) of */
/* window. Returns 1, or -1 when no window sprite is free. */
s32 mnIconCreateStatChangeArrow(WINDOW *window, s32 x, s32 y, s32 variant) {
	WINDOW_SPR *arrow;
	u32 resource;
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	if(variant == STAT_ARROW_UP)
		resource = menu->resourceSlots[0];
	else
		resource = menu->resourceSlots[1];
	arrow =
		mnWindowSprCreateStub(resource, WINDOW_SPRITE_SIZE_16X16, window, x, y);
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

/* As mnIconCreateStatChangeArrow, with the arrow raised 3 pixels for variant */
/* 0 and 4 for any other. Returns 1, or -1 when no window sprite is free. */
s32 mnIconCreateDjinnArrow(WINDOW *window, s32 x, s32 y, s32 variant) {
	WINDOW_SPR *arrow;
	u32 resource;
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	if(variant == STAT_ARROW_UP) {
		resource = menu->resourceSlots[0];
		y -= 3;
	}
	else {
		resource = menu->resourceSlots[1];
		y -= 4;
	}
	arrow =
		mnWindowSprCreateStub(resource, WINDOW_SPRITE_SIZE_16X16, window, x, y);
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

/* Moves the selection *cursor among `count` entries in rows of `columns`: */
/* left and right step and wrap, up and down move a row, the last row ending */
/* at the last entry. Returns 0 when it stays, 1 when it moves within its row */
/* and 2 when it reaches another row (another page of the party members). */
s32 mnMoveGridCursor(s32 *cursor, s32 count, s32 columns) {
	INPUT_STATE *input = &InputWork;
	s32 result = 0;
	s32 old = *cursor;
	s32 pos = old;

	if(input->repeat & KEY_LEFT) {
		pos--;
		pos = (pos + count) % count;
	}
	if(input->repeat & KEY_RIGHT) {
		pos++;
		pos = (pos + count) % count;
	}
	if(input->repeat & KEY_UP) {
		if(pos - columns >= 0)
			pos -= columns;
	}
	if(input->repeat & KEY_DOWN) {
		if(pos + columns < (columns + count - 1) / columns * columns)
			pos += columns;
		if(pos > count - 1)
			pos = count - 1;
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

/* Shows the up arrow when the page of four starting at `index`'s page has one
 */
/* above it and the down arrow when one follows, among `count` entries. */
void mnSetPageArrowsUpDown(s32 index, s32 count) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	WINDOW_SPR *arrow;
	s32 top = index / 4 * 4;

	arrow = menu->pageArrows[0];
	arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
	if(top != 0) {
		arrow->motion = WINDOW_SPRITE_MOTION_HOP_UP;
		arrow->anmStep = 0;
	}
	arrow = menu->pageArrows[1];
	arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
	if(top + 4 < count) {
		arrow->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
		arrow->anmStep = 0;
	}
}

/* As mnSetPageArrowsUpDown, with the arrows hopping left and right: the */
/* Djinn menu's columns of party members. */
void mnSetPageArrowsLeftRight(s32 index, s32 count) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	WINDOW_SPR *arrow;
	s32 top = index / 4 * 4;

	arrow = menu->pageArrows[0];
	arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
	if(top != 0) {
		arrow->motion = WINDOW_SPRITE_MOTION_HOP_LEFT;
		arrow->anmStep = 0;
	}
	arrow = menu->pageArrows[1];
	arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
	if(top + 4 < count) {
		arrow->motion = WINDOW_SPRITE_MOTION_HOP_RIGHT;
		arrow->anmStep = 0;
	}
}

/* Sets the page arrows for `index` and sets the chosen party member's figure */
/* apart from the others on its page of four: its y MENU_MEMBER_CHOSEN_Y, */
/* theirs MENU_MEMBER_Y. */
void mnMarkChosenMember(s32 unused, s32 index, s32 count) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	s32 top = index / 4 * 4;
	s32 i;

	mnSetPageArrowsUpDown(index, count);
	for(i = 0; i <= 3; i++) {
		if(top + i == index)
			menu->memberY[i] = MENU_MEMBER_CHOSEN_Y;
		else
			menu->memberY[i] = MENU_MEMBER_Y;
	}
}

/* A task while the party figures show: places each party member's */
/* figure at its menu position and scale through the projected placement. */
void mnUpdatePartyFigures(void) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	s32 scale[2];
	FIXED transform[4];
	s32 i;

	if(menu->memberSprsShown != 0) {
		for(i = 0; i <= 7; i++) {
			/* 114.5 less half the figure's screen y, in 16.16. */
			FIXED top = toFIXED(229) / 2 - menu->memberY[i] * (FIXED_ONE / 2);

			if(menu->memberSprs[i] != NULL) {
				scale[0] = menu->memberScale[i];
				scale[1] = menu->memberScale[i];
				transform[0] = toFIXED(menu->memberX[i]);
				transform[1] = top;
				transform[2] = toFIXED(menu->memberY[i]) + top;
				transform[3] = 0;
				btRenderApplyProjectedPlacementAtDepthStub(
					menu->memberSprs[i], transform, scale, FACING_SOUTH,
					MENU_FIGURE_DEPTH);
			}
		}
	}
}

/* Returns the sprite party member id's figure shows. */
s32 gmPartyGetCharacterSprStub(s32 id);

/* mnCreatePartyFigures for two characters (-1 for none): their */
/* figures in entries 0 and 1, as the Djinn preview shows the pair a Djinni */
/* passes between. */
void mnCreatePairFigures(s32 first, s32 second) {
	ITEM_MENU_STATE *menu = (ITEM_MENU_STATE *)Ram_HeapSlots->menuState;
	SPR *figure;
	s32 id;
	s32 i;

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
			figure = fdSprCreateStub(gmPartyGetCharacterSprStub(id));
			if(figure != NULL) {
				fdSprStartAnimationStub(figure, ANIM_STAND);
				figure->image.pri = 0;
				figure->flg = 0;
			}
			menu->memberSprs[i] = figure;
		}
	}
}

/* Replaces the menu's party figures with up to four party members' figures, */
/* from party place first on, standing (ANIM_STAND), and counts the party. */
void mnCreatePartyFigures(s32 first) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	u16 owners[14];
	SPR *object;
	s32 i;

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
		object = fdSprCreateStub(gmPartyGetCharacterSprStub(owners[first + i]));
		if(object != NULL) {
			fdSprStartAnimationStub(object, ANIM_STAND);
			object->image.pri = 0;
			object->flg = 0;
		}
		menu->memberSprs[i] = object;
	}
}

/* Creates the first four party members' figures in a row from tile (originX, */
/* originY) inside window, three tiles apart, and starts placing them each */
/* frame (mnUpdatePartyFigures). */
void mnShowPartyFigures(WINDOW *window, s32 originX, s32 originY) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	s32 i;

	for(i = 0; i < 8; i++)
		menu->memberSprs[i] = NULL;
	mnCreatePartyFigures(0);
	for(i = 0; i <= 3; i++) {
		if(menu->memberSprs[i] != NULL) {
			menu->memberX[i] = (i * 3 + (window->x + originX)) * 8;
			menu->memberY[i] = (window->y + originY) * 8 + 16;
			menu->memberScale[i] = FIXED_ONE;
		}
	}
	menu->memberSprsShown = ICON_SHOWN;
	sySetEvent(mnUpdatePartyFigures, TASK_PASS_FRAME);
}

/* Destroys the menu's party figures and stops placing them. */
void mnReleasePartyFigures(void) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	void **objects = menu->memberSprs;
	s32 i;

	for(i = 0; i < 8; i++) {
		void *object = *objects++;

		if(object != NULL)
			fdSprDestroyStub(object);
	}
	syCloseEvent(mnUpdatePartyFigures);
}

/* Shows the Djinn figures of the top row (nonzero) or hides them (0). */
void mnDjinnFiguresSetShown(s32 shown) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	menu->figuresShown = shown;
}

/* A task while the Djinn figures show: places each at its position, */
/* depth and scale; a positive scale eases a quarter of the way to full size */
/* each frame, a negative one holds at its size. */
void mnDjinnFiguresUpdateTopRow(void) {
	ITEM_MENU_STATE *menu = (ITEM_MENU_STATE *)Ram_HeapSlots->menuState;
	SPR_SCALE scale;
	SPR_PLACEMENT position;
	s32 i;

	if(menu->figuresShown == 0)
		return;
	for(i = 0; i <= 3; i++) {
		SPR *figure = menu->figures[i];

		if(figure != NULL) {
			/* 114.5 less half the figure's screen y, in 16.16. */
			FIXED placeY =
				toFIXED(229) / 2 - menu->memberY[i] * (FIXED_ONE / 2);
			s32 value = menu->figureScales[i];

			s32 next;

			if(value < 0) {
				next = -value;
				scale.x = next;
				scale.y = next;
			}
			else {
				next = value + (FIXED_ONE - value) / 4;
				scale.x = next;
				scale.y = next;
				menu->figureScales[i] = next;
			}
			position.x = toFIXED(menu->figureX[i]);
			position.height = placeY;
			position.y = toFIXED(menu->figureY[i]) + placeY;
			position.groundHeight = 0;
			btRenderApplyProjectedPlacementAtDepthStub(
				figure, &position, &scale, menu->figureFacings[i],
				MENU_FIGURE_DEPTH);
		}
	}
}

/* The four Djinn figures' sprites, one an element (menu_tables.c). */
extern s32 DjinnFigures_SpriteIds[];

/* The x a new row's figures stand at, parked below the screen */
/* (MENU_FIGURE_HIDDEN_Y), until their screen places them. */
#define FIGURE_START_X 16

/* Replaces the four Djinn figures with one Djinni of each element */
/* (DjinnFigures_SpriteIds) walking (ANIM_WALK), at FIGURE_START_X and */
/* MENU_FIGURE_HIDDEN_Y until the screen moves them, and starts placing them */
/* each frame (mnDjinnFiguresUpdateTopRow). */
void mnDjinnFiguresInitTopRow(void) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	SPR *object;
	s32 i;

	for(i = 0; i < 4; i++) {
		if(menu->figures[i] != NULL) {
			fdSprDestroyStub(menu->figures[i]);
			menu->figures[i] = NULL;
		}
	}
	for(i = 0; i < 4; i++) {
		object = fdSprCreateStub(DjinnFigures_SpriteIds[i]);
		if(object != NULL) {
			fdSprStartAnimationStub(object, ANIM_WALK);
			object->image.pri = 0;
		}
		menu->figures[i] = object;
		menu->figureX[i] = FIGURE_START_X;
		menu->figureY[i] = MENU_FIGURE_HIDDEN_Y;
		menu->figureScales[i] = FIXED_ONE;
		menu->figureFacings[i] = FACING_SOUTH;
	}
	menu->figuresShown = 1;
	sySetEvent(mnDjinnFiguresUpdateTopRow, TASK_PASS_FRAME);
}

/* Destroys the four Djinn figures and stops placing them. */
void mnDjinnFiguresReleaseTopRow(void) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	s32 i;

	for(i = 0; i < 4; i++) {
		if(menu->figures[i] != NULL) {
			fdSprDestroyStub(menu->figures[i]);
			menu->figures[i] = NULL;
		}
	}
	syCloseEvent(mnDjinnFiguresUpdateTopRow);
}

/* Replaces Djinn figure index with a Djinni of element kind */
/* (DjinnFigures_SpriteIds) playing animation. */
void mnDjinnFiguresReplaceSlot(s32 index, s32 kind, s32 animation) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	SPR *object;

	if(menu->figures[index] != NULL) {
		fdSprDestroyStub(menu->figures[index]);
		menu->figures[index] = NULL;
	}
	object = fdSprCreateStub(DjinnFigures_SpriteIds[kind]);
	if(object != NULL) {
		fdSprStartAnimationStub(object, animation);
		object->image.pri = 0;
	}
	menu->figures[index] = object;
}

/* Moves Djinn figure index to screen pixel (x, y), if it exists. */
void mnDjinnFiguresSetSlotPos(s32 index, s32 x, s32 y) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	if(menu->figures[index] != NULL) {
		menu->figureX[index] = x;
		menu->figureY[index] = y;
	}
}

/* Turns Djinn figure index to face facing (FACING), if it exists. */
void mnDjinnFiguresSetSlotFacing(s32 index, s32 facing) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	if(menu->figures[index] != NULL)
		menu->figureFacings[index] = facing;
}

/* Sets Djinn figure index's scale, if it exists: positive grows towards full */
/* size, negative holds at its size. */
void mnDjinnFiguresSetSlotPhase(s32 index, s32 scale) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	if(menu->figures[index] != NULL)
		menu->figureScales[index] = scale;
}

/* A task: queues the five status icons for drawing while they show. */
void mnIconPushSlots(void) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	ICON_SPR *slot;
	s32 i;

	if(menu->menuIconsShown != 0) {
		slot = menu->menuIcons;
		for(i = 0; i < CHARACTER_AILMENT_COUNT; i++) {
			syOamAddSprAtDepth(slot, OAM_DEPTH_WINDOW);
			slot++;
		}
	}
}

extern u16 StatusScreen_StatusIcons[5];

/* Loads the five status icons (StatusScreen_StatusIcons), one for each */
/* status mnPanelDrawEntryList lists (UNIT_STAT), into free VRAM cache */
/* slots, and starts drawing them (mnIconPushSlots). They are the battle */
/* Status screen's pictures for downed, poison, deadly poison, a cursed item */
/* and haunt. */
void mnIconLoadSlots(void) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	ICON_SPR *slot = menu->menuIcons;
	s32 entry;
	s32 i;

	for(i = 0; i < CHARACTER_AILMENT_COUNT; i++) {
		entry = syVramCacheFindFreeEntry();
		if(entry < VRAM_CACHE_ENTRY_COUNT) {
			u32 *attributes;

			mnSelectionIconLoadIntoSlotStub(
				ICON_KIND_PATTERN, StatusScreen_StatusIcons[i], entry, 0);
			attributes = (u32 *)&slot->attr;
			*attributes++ = WINDOW_SPRITE_SIZE_16X16;
			*attributes = 0;
			slot->attr.tile = VramCacheEntries[entry].ofs >> 5;
		}
		menu->menuIconSlots[i] = entry;
		slot++;
	}
	menu->menuIconsShown = ICON_SHOWN;
	sySetEvent(mnIconPushSlots, TASK_PASS_FRAME + 10);
}

/* Stops drawing the five status icons and frees their VRAM cache slots. */
void mnIconReleaseSlots(void) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;
	u16 *resource;
	s32 i;

	syCloseEvent(mnIconPushSlots);
	resource = menu->menuIconSlots;
	for(i = 0; i < CHARACTER_AILMENT_COUNT; i++)
		syVramCacheFreeEntry(*resource++);
}

/* Moves status icon index to screen position (x, y). */
void mnIconSetSlot(s32 index, u32 x, s32 y) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	menu->menuIcons[index].attr.x = x;
	menu->menuIcons[index].attr.y = y;
}

/* Shows the five status icons (1) or hides them (0). */
void mnIconSetActive(s32 active) {
	ITEM_MENU_STATE *menu = Ram_HeapSlots->menuState;

	menu->menuIconsShown = active;
}

/* Lists the statuses enabled marks into panel from (x, y), one a 16-pixel row
 */
/* with its icon, or "Your status is normal." when there are none; the icons */
/* of the others go off screen. */
void mnPanelDrawEntryList(WINDOW *panel, s32 x, s32 y, const u8 *enabled) {
	s32 count = 0;
	s32 iconX = panel->x * 8 + x + 8;
	s32 iconY = panel->y * 8 + y + 8;
	s32 i;

	for(i = 0; i < CHARACTER_AILMENT_COUNT; i++) {
		if(enabled[i] != 0) {
			mnIconSetSlot(i, iconX, iconY);
			mnTextDrawMessageAsTilesStub((s32)MsgStatusNameDown + i, panel,
										 x + 16, y);
			y += 16;
			iconY += 16;
			count++;
		}
		else {
			mnIconSetSlot(i, -16, -16);
		}
	}
	if(count == 0)
		mnTextDrawMessageAsTilesStub((s32)MsgStatusNameNormal, panel, x, y);
}

/* Fills ailments (UNIT_STAT) with party member memberId's statuses, */
/* down only when requested is 1, and returns how many they have. */
s32 mnStatScrListStatuses(u8 *ailments, s32 requested, s32 memberId) {
	UNIT *unit;
	s32 ailmentIndex;
	s32 count;
	s32 poison;

	unit = syUnitGet(memberId);
	for(ailmentIndex = 0; ailmentIndex < CHARACTER_AILMENT_COUNT;
		ailmentIndex++)
		ailments[ailmentIndex] = 0;

	count = 0;
	if(unit->hp == 0 && requested == 1) {
		ailments[CHARACTER_DOWN] = requested;
		count = 1;
	}

	poison = unit->poison;
	if(poison != 0) {
		if(poison == 1) /* poison; any other value is venom */
			ailments[CHARACTER_POISON] = poison;
		else
			ailments[CHARACTER_VENOM] = 1;
		count++;
	}
	if(unit->curse != 0) {
		ailments[CHARACTER_CURSE] = 1;
		count++;
	}
	if(unit->haunt != 0) {
		ailments[CHARACTER_HAUNT] = 1;
		count++;
	}
	return count;
}
