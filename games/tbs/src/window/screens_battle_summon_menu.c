/* The battle summon menu. Part of screens.c. */

/* The tile column of a summon's first Djinn cost in the list: its element icon,
   the count after it, two tiles for each element it needs. */
#define SUMMON_COST_X JAPANESE_OR(10, 13)

/* The summons a page of the list shows. */
#define SUMMON_LIST_ROWS 4

/* The battle summon list: the summons the party holds enough Djinn for
   (gmSummonListLearnedStub, DJINN_RECOVERY_TABLE.summonMask), SUMMON_LIST_ROWS
   to a page, those `standby` can pay for first, each with its Djinn cost by
   element, the others in TEXT_COLOR_RED. */
Sint32 mnBattleSummonMenuRun(
	Sint32 unusedA, Sint32 unusedB, const Uint8 *standby)
{
	MENU_SPR   cursorStorage;
	MENU_SPR   sprites[SUMMON_LIST_ROWS];
	Uint8	   learnedSummons[33]; /* in menu order */
	Uint8	   orderedSummons[33]; /* those standby can pay for first */
	Uint16	   text[64];
	Sint32	   iconSlots[SUMMON_LIST_ROWS];
	Uint8	   visibleSummons[SUMMON_LIST_ROWS];
	Sint32	   glyph;
	Sint32	   count;
	WINDOW	  *window;
	WIN_STATE *state;
	Sint32	   drawnRow, drawnFirstEntry, selectedRow, firstEntry;
	Sint32	   cursorHandle;
	WINDOW	  *description, *counts;
	Sint32	   mask, preferredRow;
	MENU_POINT cursorPosition;
	const SUMMON_DEFINITION *summon;
	BATTLE_MENU_STATE		*navigation;
	MENU_SPR				*sprite;
	MENU_SPR				*cur;
	Sint32					 orderedCount;
	Sint32					 id, tile, affordable;
	Sint32					 pressed, repeated, result;
	Sint32					 index;

	state = WINDOW_STATE;
	drawnRow = -1;
	drawnFirstEntry = -1;
	/* One 16x16 sprite. */
	cursorHandle = (Sint16)syVRAMCacheAllocEntry(128);
	description = mnWindowCreate(0, 4, 30, 4,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
			WINDOW_FLAG_BORDERED_CANVAS);
	counts = mnWindowCreate(20, 8, 10, 3, MENU_WINDOW_FLAGS);
	mask = 0;
	BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
	window = mnWindowCreate(SUMMON_MENU_WINDOW_X, 11, SUMMON_MENU_WINDOW_WIDTH,
		9, MENU_WINDOW_FLAGS);

	/* Icon sprites for each row. */
	for(index = 0; index < SUMMON_LIST_ROWS; index++) {
		Sint32 y = index * 2;

		sprite = &sprites[index];
		MENU_SPR_INIT(sprites[index], OBJ_SIZE_16X16);
		sprite->oam.f.x = window->x * 8 + 8;
		sprite->oam.f.y = (y + window->y) * 8 + 4;
	}
	for(index = 0; index < SUMMON_LIST_ROWS; index++) {
		Sint32 slot = syVRAMCacheAllocEntry(128);
		iconSlots[index] = slot;
		sprites[index].oam.f.tile = syVRAMCacheReload(slot, -1);
	}

	/* Move usable summons to the front in reverse traversal; keep others in
	   order. */
	count = gmSummonListLearnedStub(learnedSummons);
	/* FAKEMATCH(dead): the first scan in a once-only block. */
	do {
		Sint32 i;

		orderedCount = 0;
		for(i = count - 1; i >= 0; i--) {
			const SUMMON_DEFINITION *definition;
			Sint32					 n, k;

			k = learnedSummons[i];
			definition = gmSummonDefinitionGetNearStub(k);
			for(n = 0; n < ELEMENT_COUNT; n++) {
				if(definition->djinnRequired[n] > standby[n])
					break;
			}
			if(n == ELEMENT_COUNT) {
				orderedSummons[orderedCount++] = k;
				learnedSummons[i] = SUMMON_END;
			}
		}
	} while(0);
	{
		Sint32 i, k;

		for(i = 0; i < count; i++) {
			k = learnedSummons[i];
			if(k != SUMMON_END)
				orderedSummons[orderedCount++] = k;
		}
	}
	orderedSummons[orderedCount] = SUMMON_END;

	for(;;) {
		/* FAKEMATCH(temporary): cur holds &cursorStorage beside sprite, which
		 * names the cursor below; as &cursorStorage at each use. */
		cur = &cursorStorage;

		/* Redraw when the page or row changes. */
		if(firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
			state->holdTilemapUpload = 1;
			mnWindowHighlightRect(window->x + 1, window->y + drawnRow * 2 + 1,
				window->width - 2, 1, WINDOW_PALETTE);
			mnTextFillCanvasBackground();

			/* Description and required Djinni elements: mask has bit n for each
			   element n the summon needs, which blink below. */
			{
				const SUMMON_DEFINITION *definition;
				Sint32					 element;

				definition = gmSummonDefinitionGetNearStub(
					orderedSummons[firstEntry + selectedRow]);
				BATTLE_MENU_MESSAGE_COPY(
					definition->abilityId + MsgAbilityDescription, text,
					BATTLE_MENU_MESSAGE_CAPACITY);
				mnTextDrawWideStringAtOffset(text, (WINDOW *)description, 0, 4);
				drawnRow = selectedRow;
				mask = 0;
				for(element = 0; element < ELEMENT_COUNT; element++) {
					if(definition->djinnRequired[element] != 0)
						mask |= 1 << element;
				}
			}

			/* Standby counts and the four summons on this page. */
			if(firstEntry != drawnFirstEntry) {
				mnWindowSprRedrawSavedRect(window);
				{
					Sint32 element;

					for(element = 0; element < ELEMENT_COUNT; element++) {
						mnWindowSetTilemapEntry(counts,
							element + LIST_TILE_SUMMON_ELEMENT, element * 2, 0,
							0);
						mnWindowPutGlyph(counts, standby[element] + '0',
							element * 2 + 1, 0, GLYPH_DRAW_CHARACTER);
					}
				}
				{
					Sint32 element, col, y;

					index = 0;
					if((id = orderedSummons[firstEntry + index]) !=
						SUMMON_END) {
					next:
						summon = gmSummonDefinitionGetNearStub(id);
						col = 0;
						for(element = 0; element < ELEMENT_COUNT; element++) {
							if(summon->djinnRequired[element] >
								standby[element])
								break;
						}
						affordable = element == ELEMENT_COUNT;
						mnAbilityBuildIconTiles(
							summon->abilityId & ACTION_ID_MASK, 0,
							&iconSlots[index], &glyph, 1);
						y = index * 2;
						sprites[index].oam.f.tile = glyph;
						/* A summon the party cannot afford is drawn in
						   TEXT_COLOR_RED. */
						if(!affordable)
							mnTextSetCol(TEXT_COLOR_RED);
						mnTextDrawMessageAsTiles(
							gmSummonDefinitionGetNearStub(id)->abilityId +
								MsgAbilityName,
							window, 16, index * 16);
						for(element = 0; element < ELEMENT_COUNT; element++) {
							if(summon->djinnRequired[element] != 0) {
								mnWindowSetTilemapEntry(window,
									element + LIST_TILE_SUMMON_ELEMENT,
									col * 2 + SUMMON_COST_X, y, 0);
								mnWindowPutGlyph(window,
									summon->djinnRequired[element] + '0',
									col * 2 + SUMMON_COST_X + 1, y,
									GLYPH_DRAW_CHARACTER);
								col++;
							}
						}
						mnTextSetCol(TEXT_COLOR_WHITE);
						visibleSummons[index++] = 1;
						if(index < SUMMON_LIST_ROWS &&
							(id = orderedSummons[firstEntry + index]) !=
								SUMMON_END)
							goto next;
					}
					while(index < SUMMON_LIST_ROWS)
						visibleSummons[index++] = 0;
				}
				drawnFirstEntry = firstEntry;
			}

			/* The page numbers. */
			if(count > SUMMON_LIST_ROWS) {
				for(index = 0;
					index < (count + SUMMON_LIST_ROWS - 1) / SUMMON_LIST_ROWS;
					index++) {
					tile = index + PAGE_NUMBER_TILE;
					if(index == firstEntry / SUMMON_LIST_ROWS)
						tile = index + PAGE_NUMBER_LIT_TILE;
					mnWindowSetTilemapEntry(window, tile,
						window->width -
							(count + SUMMON_LIST_ROWS - 1) / SUMMON_LIST_ROWS +
							index - 2,
						-1, 0);
				}
			}
			mnWindowHighlightRect(window->x + 1,
				window->y + selectedRow * 2 + 1, window->width - 2, 1,
				HIGHLIGHT_PALETTE);
			state->dirty = 1;
			state->holdTilemapUpload = 0;
		}

		/* Each frame: the row icons and the cursor. */
		for(index = 0; index < SUMMON_LIST_ROWS; index++) {
			if(visibleSummons[index] != 0)
				syOAMAddSprAtDepth(&sprites[index], OAM_DEPTH_WINDOW);
		}

		/* The cursor wobbles every four frames. */
		MENU_PLACE_CURSOR(cursorPosition, window, selectedRow, 2, cur);
		cur->oam.f.tile =
			syVRAMCacheReload((Uint16)cursorHandle, (Sint32)PointingHandTiles);
		cur->oam.f.x = cursorPosition.x + ((FrameCount & 4) >> 1) - 4;
		cur->oam.f.y = cursorPosition.y - ((FrameCount & 4) >> 2) - 8;
		/* sprite, done with the row icons, now names the cursor. */
		sprite = &cursorStorage;
		syOAMAddSprAtDepth(sprite, OAM_DEPTH_WINDOW + 2);

		/* The standby counts of the elements the summon under the cursor needs
		   blink every eight frames. */
		{
			Sint32 element, palette, blink;

			blink = FrameCount & 8;
			for(element = 0; element < ELEMENT_COUNT; element++) {
				/* HIGHLIGHT_PALETTE, one below, while the blink is on. */
				palette = WINDOW_PALETTE - (blink != 0);
				if((1 << element & mask) == 0)
					palette = WINDOW_PALETTE;
				mnWindowHighlightRect(
					counts->x + element * 2 + 1, counts->y + 1, 2, 1, palette);
			}
		}

		/* Page numbers (current page blinking) and left/right arrows. */
		if(count > SUMMON_LIST_ROWS) {
			for(index = 0;
				index < (count + SUMMON_LIST_ROWS - 1) / SUMMON_LIST_ROWS;
				index++) {
				tile = index + PAGE_NUMBER_TILE;
				if((FrameCount & 15) <= 11 &&
					index == firstEntry / SUMMON_LIST_ROWS)
					tile = index + PAGE_NUMBER_LIT_TILE;
				mnWindowSetTilemapEntry(window, tile,
					window->width -
						(count + SUMMON_LIST_ROWS - 1) / SUMMON_LIST_ROWS +
						index - 2,
					-1, 0);
			}
			mnWindowSetTilemapEntry(window, LIST_TILE_PAGES_LEFT,
				window->width -
					(count + SUMMON_LIST_ROWS - 1) / SUMMON_LIST_ROWS - 3,
				-1, 0);
			mnWindowSetTilemapEntry(
				window, LIST_TILE_PAGES_RIGHT, window->width - 2, -1, 0);
			mnWindowMarkTilemapRow(state, window->y - 1);
		}

		BATTLE_MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
		navigation = BattleMenuStateWork;
		pressed = PerPush;
		repeated = PerRepeat;

		/* Automatic input confirms every 120 frames, while the Djinn tutorial
		   runs. */
		if(navigation->tutorialStep != 0) {
			repeated = 0;
			pressed = 0;
			if(navigation->tutorialDelay == 0) {
				navigation->tutorialDelay = 120;
				repeated = KEY_A;
				pressed = KEY_A;
			} else {
				navigation->tutorialDelay--;
			}
		}
		/* A takes the summon under the cursor, affordable or not. */
		if(pressed & KEY_A) {
			result = orderedSummons[firstEntry + selectedRow];
			break;
		}
		if(BattleMenuStateWork->tmr == 0 || (pressed & KEY_B)) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if(repeated & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectedRow++;
			if(selectedRow == SUMMON_LIST_ROWS ||
				firstEntry + selectedRow == count)
				selectedRow = 0;
			preferredRow = selectedRow;
		} else if(repeated & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectedRow--;
			if(selectedRow < 0) {
				/* On the last page the cursor goes to its last entry. */
				if(firstEntry ==
					((count - 1) / SUMMON_LIST_ROWS) * SUMMON_LIST_ROWS)
					selectedRow = count - firstEntry - 1;
				else
					selectedRow = SUMMON_LIST_ROWS - 1;
			}
			preferredRow = selectedRow;
		} else if(repeated & KEY_RIGHT) {
			/* Next page, wrapping from last to first. */
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			syInputResetKeyRepeat();
			if(firstEntry + SUMMON_LIST_ROWS >= count) {
				if(firstEntry != 0) {
					firstEntry = 0;
					selectedRow = preferredRow;
				}
			} else {
				firstEntry += SUMMON_LIST_ROWS;
				selectedRow = preferredRow;
				if(firstEntry ==
					((count - 1) / SUMMON_LIST_ROWS) * SUMMON_LIST_ROWS) {
					selectedRow = count - firstEntry - 1;
					if(selectedRow > preferredRow)
						selectedRow = preferredRow;
				}
			}
		} else if(repeated & KEY_LEFT) {
			/* Previous page, wrapping from first to last. */
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			syInputResetKeyRepeat();
			if(firstEntry != 0) {
				firstEntry -= SUMMON_LIST_ROWS;
				selectedRow = preferredRow;
			} else {
				firstEntry =
					((count - 1) / SUMMON_LIST_ROWS) * SUMMON_LIST_ROWS;
				selectedRow = preferredRow;
				if(firstEntry != 0) {
					selectedRow = count - firstEntry - 1;
					if(selectedRow > preferredRow)
						selectedRow = preferredRow;
				}
			}
		}
		sySynch(1);
	}

	sySynch(1);
	for(index = 0; index < SUMMON_LIST_ROWS; index++)
		syVRAMCacheFreeEntry(iconSlots[index]);
	syVRAMCacheFreeEntry((Uint16)cursorHandle);
	mnWindowClose(counts, 1);
	mnWindowClose(description, 1);
	mnWindowClose(window, 1);
	sySynch(1);
	return result;
}
