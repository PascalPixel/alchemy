/* The battle summon menu. Part of screens.c. */

/* Where the Japanese layout differs: the summon list's window and its Djinn
   costs, in tiles. */
#define SUMMON_MENU_WINDOW_X	 JAPANESE_OR(14, 13)
#define SUMMON_MENU_WINDOW_WIDTH JAPANESE_OR(16, 17)
#define SUMMON_COST_X			 JAPANESE_OR(10, 11)

/* The battle summon list: the summons the party holds enough Djinn for
   (gmSummonListLearnedStub, DJINN_RECOVERY_TABLE.summonMask), four to a page,
   those `standby` can pay for first, each with its Djinn cost by element, the
   others in TEXT_COLOR_RED. */
Sint32 mnBattleSummonMenuRun(
	Sint32 unusedA, Sint32 unusedB, const Uint8 *standby)
{
	MENU_SPR   cursorStorage;
	MENU_SPR   sprites[4];
	Uint8	   learnedSummons[33];
	Uint8	   orderedSummons[33];
	Uint16	   text[64];
	Sint32	   iconSlots[4];
	Uint8	   visibleSummons[4];
	Sint32	   glyph;
	Sint32	   count;
	WINDOW	  *window;
	WIN_STATE *state;
	Sint32	   drawnRow;
	Sint32	   drawnFirstEntry;
	Sint32	   selectedRow;
	Sint32	   firstEntry;
	Sint32	   cursorHandle;
	WINDOW	  *description;
	WINDOW	  *counts;
	Sint32	   mask;
	Sint32	   preferredRow;

	struct {
		Sint32 x, y;
	} cursorPos;

	const SUMMON_DEFINITION *summon;
	BATTLE_MENU_STATE		*navigation;
	MENU_SPR				*sprite;
	Sint32					 orderedCount;
	Sint32					 id;
	Sint32					 tile;
	Sint32					 affordable;
	Sint32					 pressed;
	Sint32					 repeated;
	Sint32					 result;
	Sint32					 index;

	state = RamHeapSlots->windowState;
	drawnRow = -1;
	drawnFirstEntry = -1;
	cursorHandle = (Sint16)syVRAMCacheAllocateEntry(128);
	description = mnWindowCreate(0, 4, 30, 4,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
			WINDOW_FLAG_BORDERED_CANVAS);
	counts =
		mnWindowCreate(20, 8, 10, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
	mask = 0;
	BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
	window = mnWindowCreate(SUMMON_MENU_WINDOW_X, 11, SUMMON_MENU_WINDOW_WIDTH,
		9, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);

	for(index = 0; index < 4; index++) {
		Sint32 y = index * 2;

		sprite = &sprites[index];
		MENU_SPR_INIT(sprites[index], OBJ_SIZE_16X16);
		sprite->oam.f.x = window->x * 8 + 8;
		sprite->oam.f.y = (y + window->y) * 8 + 4;
	}
	for(index = 0; index < 4; index++) {
		Sint32 slot = syVRAMCacheAllocateEntry(128);

		iconSlots[index] = slot;
		sprites[index].oam.f.tile = syVRAMCacheReload(slot, (const void *)-1);
	}

	count = gmSummonListLearnedStub(learnedSummons);
	/* FAKEMATCH(dead): the first scan in a once-only block; as a plain
	 * block. */
	do {
		Sint32 i;

		orderedCount = 0;
		for(i = count - 1; i >= 0; i--) {
			const SUMMON_DEFINITION *definition;
			Sint32					 element;
			Sint32					 k;

			k = learnedSummons[i];
			definition = gmSummonDefinitionGetNearStub(k);
			for(element = 0; element <= 3; element++) {
				if(definition->djinnRequired[element] > standby[element])
					break;
			}
			if(element == 4) {
				orderedSummons[orderedCount] = k;
				learnedSummons[i] = SUMMON_END;
				orderedCount++;
			}
		}
	} while(0);
	{
		Sint32 i;
		Sint32 k;

		for(i = 0; i < count; i++) {
			k = learnedSummons[i];
			if(k != SUMMON_END)
				orderedSummons[orderedCount++] = k;
		}
	}
	orderedSummons[orderedCount] = SUMMON_END;

	for(;;) {
		sprite = &cursorStorage;

		if(firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
			state->holdTilemapUpload = 1;
			mnWindowHighlightRect(window->x + 1, window->y + drawnRow * 2 + 1,
				window->width - 2, 1, WINDOW_PALETTE);
			mnTextFillCanvasBackground();
			{
				const SUMMON_DEFINITION *definition;
				Sint32					 element;

				definition = gmSummonDefinitionGetNearStub(
					orderedSummons[firstEntry + selectedRow]);
				BATTLE_MENU_MESSAGE_COPY(
					definition->abilityId + (Sint32)MsgAbilityDescription, text,
					BATTLE_MENU_MESSAGE_CAPACITY);
				mnTextDrawWideStringAtOffset(text, description, 0, 4);
				drawnRow = selectedRow;
				mask = 0;
				for(element = 0; element < ELEMENT_COUNT; element++) {
					if(definition->djinnRequired[element] != 0)
						mask |= 1 << element;
				}
			}

			if(firstEntry != drawnFirstEntry) {
				/* FAKEMATCH(dead): the redraw in a once-only block. */
				do {
					mnWindowSpriteRedrawSavedRect(window);
				} while(0);
				{
					Sint32 element;

					for(element = 0; element <= 3; element++) {
						mnWindowSetTilemapEntry(counts,
							element + ELEMENT_ICON_TILE, element * 2, 0, 0);
						tile = DJINN_COUNT_TILE(standby[element]);
						mnWindowSetTilemapEntry(counts, tile, element * 2 + 1,
							0, GLYPH_DRAW_CHARACTER);
					}
				}
				{
					Sint32 element;
					Sint32 column;
					Sint32 y;

					index = 0;
					if((id = orderedSummons[firstEntry + index]) !=
						SUMMON_END) {
					next:
						summon = gmSummonDefinitionGetNearStub(id);
						column = 0;
						for(element = 0; element <= 3; element++) {
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
						if(!affordable)
							mnTextSetColor(TEXT_COLOR_RED);
						mnTextDrawMessageAsTiles(
							gmSummonDefinitionGetNearStub(id)->abilityId +
								(Sint32)MsgAbilityName,
							window, 16, index * 16);
						for(element = 0; element <= 3; element++) {
							if(summon->djinnRequired[element] != 0) {
								mnWindowSetTilemapEntry(window,
									element + ELEMENT_ICON_TILE,
									column * 2 + SUMMON_COST_X, y, 0);
								mnWindowPutGlyph(window,
									summon->djinnRequired[element] + '0',
									column * 2 + SUMMON_COST_X + 1, y,
									GLYPH_DRAW_CHARACTER);
								column++;
							}
						}
						mnTextSetColor(TEXT_COLOR_WHITE);
						visibleSummons[index++] = 1;
						if(index <= 3 &&
							(id = orderedSummons[firstEntry + index]) !=
								SUMMON_END)
							goto next;
					}
					while(index <= 3)
						visibleSummons[index++] = 0;
				}
				drawnFirstEntry = firstEntry;
			}

			if(count > 4) {
				for(index = 0; index < (count + 3) / 4; index++) {
					tile = index + PAGE_NUMBER_TILE;
					if(index == firstEntry / 4)
						tile = index + PAGE_NUMBER_LIT_TILE;
					mnWindowSetTilemapEntry(window, tile,
						window->width - (count + 3) / 4 + index - 2, -1, 0);
				}
			}
			mnWindowHighlightRect(window->x + 1,
				window->y + selectedRow * 2 + 1, window->width - 2, 1,
				HIGHLIGHT_PALETTE);
			state->dirty = 1;
			state->holdTilemapUpload = 0;
		}

		for(index = 0; index <= 3; index++) {
			if(visibleSummons[index] != 0)
				syOAMAddSpriteAtDepth(&sprites[index], OAM_DEPTH_WINDOW);
		}

		MENU_PLACE_CURSOR(cursorPos, window, selectedRow, 2, sprite);
		sprite->oam.f.tile =
			syVRAMCacheReload((Uint16)cursorHandle, PointingHandTiles);
		sprite->oam.f.x = cursorPos.x + ((TickCount & 4) >> 1) - 4;
		sprite->oam.f.y = cursorPos.y - ((TickCount & 4) >> 2) - 8;
		syOAMAddSpriteAtDepth(sprite, OAM_DEPTH_WINDOW + 2);

		{
			Sint32 element, palette, blink;

			/* The elements the summon under the cursor needs Djinn of blink. */
			blink = TickCount & 8;
			for(element = 0; element < ELEMENT_COUNT; element++) {
				/* HIGHLIGHT_PALETTE, one below, while the blink is on. */
				palette = WINDOW_PALETTE - (blink != 0);
				if((1 << element & mask) == 0)
					palette = WINDOW_PALETTE;
				mnWindowHighlightRect(
					counts->x + element * 2 + 1, counts->y + 1, 2, 1, palette);
			}
		}

		if(count > 4) {
			for(index = 0; index < (count + 3) / 4; index++) {
				tile = index + PAGE_NUMBER_TILE;
				if((TickCount & 15) <= 11 && index == firstEntry / 4)
					tile = index + PAGE_NUMBER_LIT_TILE;
				mnWindowSetTilemapEntry(window, tile,
					window->width - (count + 3) / 4 + index - 2, -1, 0);
			}
			mnWindowSetTilemapEntry(window, LIST_TILE_PAGES_LEFT,
				window->width - (count + 3) / 4 - 3, -1, 0);
			mnWindowSetTilemapEntry(
				window, LIST_TILE_PAGES_RIGHT, window->width - 2, -1, 0);
			WINDOW_MARK_TILEMAP_ROW(state, window->y - 1);
		}

		BATTLE_MENU_SET_CURSOR(firstEntry, selectedRow, preferredRow);
		navigation = BattleMenuStateWork;
		pressed = PerWork.push;
		repeated = PerWork.repeat;
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
		if(pressed & KEY_A) {
			result = orderedSummons[firstEntry + selectedRow];
			break;
		}
		if(BattleMenuStateWork->timer == 0 || (pressed & KEY_B)) {
			sySoundPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if(repeated & KEY_DOWN) {
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			selectedRow++;
			if(selectedRow == 4 || firstEntry + selectedRow == count)
				selectedRow = 0;
			preferredRow = selectedRow;
		} else if(repeated & KEY_UP) {
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			selectedRow--;
			if(selectedRow < 0) {
				if(firstEntry == (count - 1) / 4 * 4)
					selectedRow = count - firstEntry - 1;
				else
					selectedRow = 3;
			}
			preferredRow = selectedRow;
		} else if(repeated & KEY_RIGHT) {
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			syInputResetKeyRepeat();
			if(firstEntry + 4 >= count) {
				if(firstEntry != 0) {
					firstEntry = 0;
					selectedRow = preferredRow;
				}
			} else {
				firstEntry += 4;
				selectedRow = preferredRow;
				if(firstEntry == (count - 1) / 4 * 4) {
					selectedRow = count - firstEntry - 1;
					if(selectedRow > preferredRow)
						selectedRow = preferredRow;
				}
			}
		} else if(repeated & KEY_LEFT) {
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			syInputResetKeyRepeat();
			if(firstEntry != 0) {
				firstEntry -= 4;
				selectedRow = preferredRow;
			} else {
				firstEntry = (count - 1) / 4 * 4;
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
	for(index = 0; index < 4; index++)
		syVRAMCacheFreeEntry(iconSlots[index]);
	syVRAMCacheFreeEntry((Uint16)cursorHandle);
	mnWindowClose(counts, 1);
	mnWindowClose(description, 1);
	mnWindowClose(window, 1);
	sySynch(1);
	return result;
}

#undef SUMMON_MENU_WINDOW_X
#undef SUMMON_MENU_WINDOW_WIDTH
#undef SUMMON_COST_X
