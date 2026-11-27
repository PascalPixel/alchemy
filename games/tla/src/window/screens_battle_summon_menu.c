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
	MENU_SPR   sprs[4];
	Uint8	   learnedSummons[33];
	Uint8	   orderedSummons[33];
	Uint16	   text[64];
	Sint32	   iconSlots[4];
	Uint8	   visibleSummons[4];
	Sint32	   glyph;
	Sint32	   cnt;
	WINDOW	  *win;
	WIN_STATE *state;
	Sint32	   drawnRow;
	Sint32	   drawnFirstEntry;
	Sint32	   selectedRow;
	Sint32	   firstEntry;
	Sint32	   cursorHandle;
	WINDOW	  *description;
	WINDOW	  *cnts;
	Sint32	   mask;
	Sint32	   preferredRow;

	struct {
		Sint32 x, y;
	} cursorPos;

	const SUMMON_DEFINITION *summon;
	BATTLE_MENU_STATE		*navigation;
	MENU_SPR				*spr;
	Sint32					 orderedCnt;
	Sint32					 id;
	Sint32					 tile;
	Sint32					 affordable;
	Sint32					 pressed;
	Sint32					 repeated;
	Sint32					 result;
	Sint32					 idx;

	state = RamHeapSlots->winState;
	drawnRow = -1;
	drawnFirstEntry = -1;
	cursorHandle = (Sint16)syVRAMCacheAllocEntry(128);
	description = mnWindowCreate(0, 4, 30, 4,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
			WINDOW_FLAG_BORDERED_CANVAS);
	cnts =
		mnWindowCreate(20, 8, 10, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
	mask = 0;
	BATTLE_MENU_GET_CURSOR(firstEntry, selectedRow, preferredRow);
	win = mnWindowCreate(SUMMON_MENU_WINDOW_X, 11, SUMMON_MENU_WINDOW_WIDTH, 9,
		WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);

	for(idx = 0; idx < 4; idx++) {
		Sint32 y = idx * 2;

		spr = &sprs[idx];
		MENU_SPR_INIT(sprs[idx], OBJ_SIZE_16X16);
		spr->oam.f.x = win->x * 8 + 8;
		spr->oam.f.y = (y + win->y) * 8 + 4;
	}
	for(idx = 0; idx < 4; idx++) {
		Sint32 slot = syVRAMCacheAllocEntry(128);

		iconSlots[idx] = slot;
		sprs[idx].oam.f.tile = syVRAMCacheReload(slot, (const void *)-1);
	}

	cnt = gmSummonListLearnedStub(learnedSummons);
	/* FAKEMATCH(dead): the first scan in a once-only block; as a plain
	 * block. */
	do {
		Sint32 i;

		orderedCnt = 0;
		for(i = cnt - 1; i >= 0; i--) {
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
				orderedSummons[orderedCnt] = k;
				learnedSummons[i] = SUMMON_END;
				orderedCnt++;
			}
		}
	} while(0);
	{
		Sint32 i;
		Sint32 k;

		for(i = 0; i < cnt; i++) {
			k = learnedSummons[i];
			if(k != SUMMON_END)
				orderedSummons[orderedCnt++] = k;
		}
	}
	orderedSummons[orderedCnt] = SUMMON_END;

	for(;;) {
		spr = &cursorStorage;

		if(firstEntry != drawnFirstEntry || selectedRow != drawnRow) {
			state->holdTilemapUpload = 1;
			mnWindowHighlightRect(win->x + 1, win->y + drawnRow * 2 + 1,
				win->width - 2, 1, WINDOW_PALETTE);
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
					mnWindowSprRedrawSavedRect(win);
				} while(0);
				{
					Sint32 element;

					for(element = 0; element <= 3; element++) {
						mnWindowSetTilemapEntry(cnts,
							element + ELEMENT_ICON_TILE, element * 2, 0, 0);
						tile = DJINN_COUNT_TILE(standby[element]);
						mnWindowSetTilemapEntry(cnts, tile, element * 2 + 1, 0,
							GLYPH_DRAW_CHARACTER);
					}
				}
				{
					Sint32 element;
					Sint32 col;
					Sint32 y;

					idx = 0;
					if((id = orderedSummons[firstEntry + idx]) != SUMMON_END) {
					next:
						summon = gmSummonDefinitionGetNearStub(id);
						col = 0;
						for(element = 0; element <= 3; element++) {
							if(summon->djinnRequired[element] >
								standby[element])
								break;
						}
						affordable = element == ELEMENT_COUNT;
						mnAbilityBuildIconTiles(
							summon->abilityId & ACTION_ID_MASK, 0,
							&iconSlots[idx], &glyph, 1);
						y = idx * 2;
						sprs[idx].oam.f.tile = glyph;
						if(!affordable)
							mnTextSetCol(TEXT_COLOR_RED);
						mnTextDrawMessageAsTiles(
							gmSummonDefinitionGetNearStub(id)->abilityId +
								(Sint32)MsgAbilityName,
							win, 16, idx * 16);
						for(element = 0; element <= 3; element++) {
							if(summon->djinnRequired[element] != 0) {
								mnWindowSetTilemapEntry(win,
									element + ELEMENT_ICON_TILE,
									col * 2 + SUMMON_COST_X, y, 0);
								mnWindowPutGlyph(win,
									summon->djinnRequired[element] + '0',
									col * 2 + SUMMON_COST_X + 1, y,
									GLYPH_DRAW_CHARACTER);
								col++;
							}
						}
						mnTextSetCol(TEXT_COLOR_WHITE);
						visibleSummons[idx++] = 1;
						if(idx <= 3 &&
							(id = orderedSummons[firstEntry + idx]) !=
								SUMMON_END)
							goto next;
					}
					while(idx <= 3)
						visibleSummons[idx++] = 0;
				}
				drawnFirstEntry = firstEntry;
			}

			if(cnt > 4) {
				for(idx = 0; idx < (cnt + 3) / 4; idx++) {
					tile = idx + PAGE_NUMBER_TILE;
					if(idx == firstEntry / 4)
						tile = idx + PAGE_NUMBER_LIT_TILE;
					mnWindowSetTilemapEntry(
						win, tile, win->width - (cnt + 3) / 4 + idx - 2, -1, 0);
				}
			}
			mnWindowHighlightRect(win->x + 1, win->y + selectedRow * 2 + 1,
				win->width - 2, 1, HIGHLIGHT_PALETTE);
			state->dirty = 1;
			state->holdTilemapUpload = 0;
		}

		for(idx = 0; idx <= 3; idx++) {
			if(visibleSummons[idx] != 0)
				syOAMAddSprAtDepth(&sprs[idx], OAM_DEPTH_WINDOW);
		}

		MENU_PLACE_CURSOR(cursorPos, win, selectedRow, 2, spr);
		spr->oam.f.tile =
			syVRAMCacheReload((Uint16)cursorHandle, PointingHandTiles);
		spr->oam.f.x = cursorPos.x + ((TickCount & 4) >> 1) - 4;
		spr->oam.f.y = cursorPos.y - ((TickCount & 4) >> 2) - 8;
		syOAMAddSprAtDepth(spr, OAM_DEPTH_WINDOW + 2);

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
					cnts->x + element * 2 + 1, cnts->y + 1, 2, 1, palette);
			}
		}

		if(cnt > 4) {
			for(idx = 0; idx < (cnt + 3) / 4; idx++) {
				tile = idx + PAGE_NUMBER_TILE;
				if((TickCount & 15) <= 11 && idx == firstEntry / 4)
					tile = idx + PAGE_NUMBER_LIT_TILE;
				mnWindowSetTilemapEntry(
					win, tile, win->width - (cnt + 3) / 4 + idx - 2, -1, 0);
			}
			mnWindowSetTilemapEntry(win, LIST_TILE_PAGES_LEFT,
				win->width - (cnt + 3) / 4 - 3, -1, 0);
			mnWindowSetTilemapEntry(
				win, LIST_TILE_PAGES_RIGHT, win->width - 2, -1, 0);
			WINDOW_MARK_TILEMAP_ROW(state, win->y - 1);
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
		if(BattleMenuStateWork->tmr == 0 || (pressed & KEY_B)) {
			sySndPlayStub(SE_MENU_CANCEL);
			result = -1;
			break;
		}
		if(repeated & KEY_DOWN) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectedRow++;
			if(selectedRow == 4 || firstEntry + selectedRow == cnt)
				selectedRow = 0;
			preferredRow = selectedRow;
		} else if(repeated & KEY_UP) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectedRow--;
			if(selectedRow < 0) {
				if(firstEntry == (cnt - 1) / 4 * 4)
					selectedRow = cnt - firstEntry - 1;
				else
					selectedRow = 3;
			}
			preferredRow = selectedRow;
		} else if(repeated & KEY_RIGHT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			syInputResetKeyRepeat();
			if(firstEntry + 4 >= cnt) {
				if(firstEntry != 0) {
					firstEntry = 0;
					selectedRow = preferredRow;
				}
			} else {
				firstEntry += 4;
				selectedRow = preferredRow;
				if(firstEntry == (cnt - 1) / 4 * 4) {
					selectedRow = cnt - firstEntry - 1;
					if(selectedRow > preferredRow)
						selectedRow = preferredRow;
				}
			}
		} else if(repeated & KEY_LEFT) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			syInputResetKeyRepeat();
			if(firstEntry != 0) {
				firstEntry -= 4;
				selectedRow = preferredRow;
			} else {
				firstEntry = (cnt - 1) / 4 * 4;
				selectedRow = preferredRow;
				if(firstEntry != 0) {
					selectedRow = cnt - firstEntry - 1;
					if(selectedRow > preferredRow)
						selectedRow = preferredRow;
				}
			}
		}
		sySynch(1);
	}

	sySynch(1);
	for(idx = 0; idx < 4; idx++)
		syVRAMCacheFreeEntry(iconSlots[idx]);
	syVRAMCacheFreeEntry((Uint16)cursorHandle);
	mnWindowClose(cnts, 1);
	mnWindowClose(description, 1);
	mnWindowClose(win, 1);
	sySynch(1);
	return result;
}

#undef SUMMON_MENU_WINDOW_X
#undef SUMMON_MENU_WINDOW_WIDTH
#undef SUMMON_COST_X
