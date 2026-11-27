/* The row of command icons the player picks a command from. Part of
   screens.c. */

/* Opens a message at (x, y), waits for its text to finish, and returns its
   window so the caller can hold it open for a separate prompt. */
WINDOW *mnMessageShowAndWait(Sint32 msg, Sint32 x, Sint32 y)
{
	WINDOW *result;

	result = mnMessageOpenWindow(msg, x, y, 1);
	while(mnMessageAllTyped() == 0)
		sySynch(1);
	return result;
}

/* The tile column where a party member's command row ends and its name window
   begins. */
#define WINDOW_X JAPANESE_OR(23, 22)

/* The command icons stand 24 pixels apart on the row. */
#define COMMAND_ICON_SPACING 24

/* Runs a battle command row: the commands as icons along the bottom of the
   screen, the selected one pulsing as a sprite and named in a window, until a
   command is chosen: returns its icon (COMMAND_ICON), COMMAND_ROW_CANCELLED or
   COMMAND_ROW_STATUS. */
Sint32 mnBattleCommandRowRun(Sint32 mode)
{
	Uint8					  standbyDjinn[ELEMENT_COUNT];
	WIN_STATE				 *state;
	BATTLE_COMMAND_ROW_STATE *selectionUi;
	BATTLE_MENU_STATE		 *navigation;
	Sint32					  stripDirty, standbyCnt, pulseScale, command;
	Sint32					  entryIdx;
	Sint32					  keys, repeat;
	Sint32					  iconSlot;
	Sint32					  i, kindCnt;

	state = WINDOW_STATE;
	/* One 1 KB entry for the selected icon's 32x32, 256-colour sprite. */
	iconSlot = syVRAMCacheAllocEntry(COMMAND_ICON_BYTES);
	stripDirty = 1;
	standbyCnt = 0;
	selectionUi = syHeapAllocEWRAM(sizeof(*selectionUi));
	/* No icon colour placed yet; the shared palette starts at BG entry 1. */
	for(i = 0; i < ICON_PALETTE_COLORS; i++)
		selectionUi->pal.slot[i] = ICON_PALETTE_SLOT_UNSET;
	selectionUi->pal.cnt = 1;
	if(mode == COMMAND_ROW_PARTY)
		selectionUi->win = mnWindowCreate(20, 17, 10, 3, MENU_WINDOW_FLAGS);
	else
		selectionUi->win = mnWindowCreate(WINDOW_X, COMMAND_ROW_TOP,
			DISPLAY_TILE_WIDTH - WINDOW_X, 3, MENU_WINDOW_FLAGS);
	selectionUi->drawnIdx = -1;
	/* The commands: Fight, Flee and Status for the party; for a party member
	   Attack, Psynergy, Djinn and Summon while the party can call a summon (the
	   recovery table's first word, DJINN_RECOVERY_TABLE.summonMask), Summon
	   only with a Djinni on standby, Item and Defend. */
	if(mode == COMMAND_ROW_PARTY) {
		kindCnt = 0;
		selectionUi->commandKinds[kindCnt++] = COMMAND_ICON_FIGHT;
		selectionUi->commandKinds[kindCnt++] = COMMAND_ICON_FLEE;
		selectionUi->commandKinds[kindCnt++] = COMMAND_ICON_STATUS;
		selectionUi->commandKinds[kindCnt] = -1;
		selectionUi->selectedIdx = BattleMenuStateWork->commandSelection[0];
	} else {
		selectionUi->selectedIdx = BattleMenuStateWork->commandSelection[1];
		kindCnt = 0;
		selectionUi->commandKinds[kindCnt++] = COMMAND_ICON_ATTACK;
		selectionUi->commandKinds[kindCnt++] = COMMAND_ICON_PSYNERGY;
		if(*((Sint32 *)gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_PARTY)) !=
			0) {
			btDjinnCountStandbyStub(0, standbyDjinn);
			for(i = 0; i < ELEMENT_COUNT; i++)
				standbyCnt += standbyDjinn[i];
			selectionUi->commandKinds[kindCnt++] = COMMAND_ICON_DJINN;
			if(standbyCnt != 0)
				selectionUi->commandKinds[kindCnt++] = COMMAND_ICON_SUMMON;
		}
		selectionUi->commandKinds[kindCnt++] = COMMAND_ICON_ITEM;
		selectionUi->commandKinds[kindCnt++] = COMMAND_ICON_DEFEND;
		/* UB: with all six commands this -1 lands in commandKinds[6], one word
		   past the end of the block; the loops below stop at six and never read
		   it. */
		selectionUi->commandKinds[kindCnt] = -1;
	}
	/* Decode each visible command through one shared remapped palette. */
	for(entryIdx = 0; entryIdx <= 5; entryIdx++) {
		selectionUi->entries[entryIdx].resource = iconSlot;
		if(selectionUi->commandKinds[entryIdx] == -1)
			break;
		mnBattleCommandRowLoadIcon(
			selectionUi, entryIdx, selectionUi->commandKinds[entryIdx]);
	}
	selectionUi->iconCnt = entryIdx;
	selectionUi->affine.scaleX = Q_8_8(1.25);
	selectionUi->affine.scaleY = Q_8_8(19.0 / 16);
	selectionUi->affine.ang = 0;
	/* The icons on the command row's top line, the last ending by the name
	   window. */
	entryIdx = 0;
	if(entryIdx < selectionUi->iconCnt) {
		do {
			Sint32 x = (entryIdx - selectionUi->iconCnt) * COMMAND_ICON_SPACING;

			selectionUi->entries[entryIdx].posX = x + 155;
			if(mode != COMMAND_ROW_PARTY)
				selectionUi->entries[entryIdx].posX = x + WINDOW_X * 8 - 5;
			selectionUi->entries[entryIdx].posY = COMMAND_ROW_TOP * 8;
			entryIdx++;
		} while(entryIdx < selectionUi->iconCnt);
	}
	/* BG1 shows the icons: behind the other layers above line 136, in front
	   from there down (mnCommandRowPutBg1Behind). */
	sySetEvent(mnCommandRowPutBg1Behind, EVENT_PASS_VBLANK);
	syIntFunction(IRQ_VCOUNT, COMMAND_ROW_TOP * 8, mnCommandRowPutBg1InFront);

	for(;;) {
		/* 1.19 to 1.31 times, over 16 ticks. */
		pulseScale = WindowSpriteBounceScales[(TickCount * 2) & 31];
		pulseScale = (pulseScale - Q_8_8(1)) / 4 + Q_8_8(19.0 / 16);
		selectionUi->affine.scaleY = selectionUi->affine.scaleX = pulseScale;
		/* Scrolled up 32 pixels, BG1's map rows 21 to 23 show on rows 17 to
		   19. */
		BgScrollWork[1].x = 0;
		BgScrollWork[1].y = 32;
		/* Draw the strip once, six panels of 3x3 tiles ending at the name
		   window: the panels left empty as WINDOW_TILE_COMMAND_PANEL in the UI
		   tilemap, then each icon's top-left 3x3 tiles in BG1's map (screen
		   block 31), the UI tilemap cleared to 0 over them. */
		if(stripDirty != 0) {
			stripDirty = 0;
			state->holdTilemapUpload = 1;
			if(mode != COMMAND_ROW_PARTY) {
				Sint32 panel;
				for(panel = 0; panel < 6 - selectionUi->iconCnt; panel++) {
					for(entryIdx = 0; entryIdx <= 2; entryIdx++) {
						Sint32 col;
						for(col = 0; col <= 2; col++)
							*(state->tilemap +
								(entryIdx + COMMAND_ROW_TOP) * BG_SCREEN_WIDTH +
								panel * 3 + WINDOW_X - 18 + (col & 3)) =
								WINDOW_TILE_COMMAND_PANEL;
					}
				}
				for(panel = 0; panel < selectionUi->iconCnt; panel++) {
					for(entryIdx = 0; entryIdx <= 2; entryIdx++) {
						Sint32 col;
						for(col = 0; col <= 2; col++) {
							volatile Uint16 *vram =
								(volatile Uint16 *)BG_SCREEN_BLOCK(
									BATTLE_BG1_SCREEN_BLOCK) +
								21 * BG_SCREEN_WIDTH + WINDOW_X;

							vram[panel * 3 + entryIdx * BG_SCREEN_WIDTH +
								(col & 3) - selectionUi->iconCnt * 3] =
								panel * 16 + entryIdx * 4 + col +
								COMMAND_ROW_ICON_TILE;
							*(state->tilemap +
								(entryIdx + COMMAND_ROW_TOP) * BG_SCREEN_WIDTH +
								panel * 3 + (col & 3) -
								selectionUi->iconCnt * 3 + WINDOW_X) = 0;
						}
					}
				}
			} else {
				Sint32 panel;
				for(panel = 0; panel < 6 - selectionUi->iconCnt; panel++) {
					for(entryIdx = 0; entryIdx <= 2; entryIdx++) {
						Sint32 col;
						for(col = 0; col <= 2; col++)
							*(state->tilemap +
								(entryIdx + COMMAND_ROW_TOP) * BG_SCREEN_WIDTH +
								panel * 3 + 2 + (col & 3)) =
								WINDOW_TILE_COMMAND_PANEL;
					}
				}
				for(panel = 0; panel < selectionUi->iconCnt; panel++) {
					for(entryIdx = 0; entryIdx <= 2; entryIdx++) {
						Sint32 col;
						for(col = 0; col <= 2; col++) {
							volatile Uint16 *vram =
								(volatile Uint16 *)BG_SCREEN_BLOCK(
									BATTLE_BG1_SCREEN_BLOCK) +
								21 * BG_SCREEN_WIDTH + 20;

							vram[panel * 3 + entryIdx * BG_SCREEN_WIDTH +
								(col & 3) - selectionUi->iconCnt * 3] =
								panel * 16 + entryIdx * 4 + col +
								COMMAND_ROW_ICON_TILE;
							*(state->tilemap +
								(entryIdx + COMMAND_ROW_TOP) * BG_SCREEN_WIDTH +
								panel * 3 + (col & 3) -
								selectionUi->iconCnt * 3 + 20) = 0;
						}
					}
				}
			}
		}
		/* The selection moved: name the new command and load its icon into the
		   sprite's entry. */
		if(selectionUi->drawnIdx != selectionUi->selectedIdx) {
			mnWindowSprPrepareForRedraw(selectionUi->win);
			mnTextDrawMessageAsTiles(
				selectionUi->entries[selectionUi->selectedIdx].kind +
					MsgCommandName,
				selectionUi->win, 0, 0);
			selectionUi->drawnIdx = selectionUi->selectedIdx;
			mnBattleCommandRowLoadIcon(selectionUi, selectionUi->selectedIdx,
				selectionUi->commandKinds[selectionUi->selectedIdx]);
		}
		selectionUi->affineIdx =
			syOAMMatrixAllocFromTransform(&selectionUi->affine);
		/* Only the selected icon is drawn as a sprite: double-size affine (mode
		   3), on layer 241, moved with the pulse. The others are BG1 tiles. */
		for(entryIdx = 0; entryIdx < selectionUi->iconCnt; entryIdx++) {
			BATTLE_COMMAND_ROW_ICON *entry = &selectionUi->entries[entryIdx];
			if(entryIdx == selectionUi->selectedIdx) {
				entry->spr.oam.f.affineIdx = selectionUi->affineIdx;
				entry->spr.oam.f.affine = 3;
				entry->spr.oam.f.x = entry->posX + pulseScale * 7 / 512 - 14;
				entry->spr.oam.f.y = entry->posY + pulseScale * 3 / 256 - 20;
				syOAMAddSprAtDepth(entry, OAM_DEPTH_WINDOW + 1);
			} else {
				entry->spr.oam.f.x = entry->posX;
				entry->spr.oam.f.y = entry->posY;
				entry->spr.oam.f.affineIdx = 0;
				entry->spr.oam.f.affine = 0;
			}
		}
		/* The Djinn tutorial presses keys for the player, one each delay. In
		   the party's row it takes Fight. */
		keys = PerPush;
		repeat = PerRepeat;
		navigation = BattleMenuStateWork;
		if(navigation->tutorialStep != 0) {
			keys = repeat = 0;
			if(navigation->tutorialDelay == 0) {
				if(navigation->commandActor == 1) {
					if(selectionUi->commandKinds[selectionUi->selectedIdx] ==
						COMMAND_ICON_DEFEND)
						keys = repeat = KEY_A;
					else
						keys = repeat = KEY_LEFT;
					navigation->tutorialDelay = 30;
				} else if(navigation->commandActor == 0) {
					Sint32 kind =
						selectionUi->commandKinds[selectionUi->selectedIdx];
					if(kind == COMMAND_ICON_SUMMON ||
						(standbyCnt == 0 && kind == COMMAND_ICON_DJINN)) {
						if(navigation->tutorialStep == 1) {
							WINDOW *msg;
							if(kind == COMMAND_ICON_DJINN)
								msg = mnMessageShowAndWait(
									MsgDjinnTutorialUnleashCommand, 15, 8);
							else if(kind == COMMAND_ICON_SUMMON)
								msg = mnMessageShowAndWait(
									MsgDjinnTutorialSummon, 15, 8);
							btMessageWaitForAdvanceAtStub(102, 155);
							mnWindowClose(msg, 1);
							navigation->tutorialStep++;
							navigation->tutorialDelay = 45;
						} else {
							keys = repeat = KEY_A;
							navigation->tutorialDelay = 200;
						}
					} else {
						keys = repeat = KEY_RIGHT;
						navigation->tutorialDelay = 40;
					}
				} else {
					navigation->tutorialDelay = 60;
					keys = repeat = KEY_A;
				}
			} else {
				navigation->tutorialDelay--;
			}
		}
		if((keys & KEYS_SHOULDERS) && mode != COMMAND_ROW_PARTY) {
			command = COMMAND_ROW_STATUS;
			break;
		}
		if(keys & KEY_A) {
			command = selectionUi->commandKinds[selectionUi->selectedIdx];
			break;
		}
		if(mode != COMMAND_ROW_PARTY && (keys & KEY_B)) {
			sySndPlayStub(SE_MENU_CANCEL);
			command = COMMAND_ROW_CANCELLED;
			break;
		}
		if(repeat & (KEY_DOWN | KEY_RIGHT)) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectionUi->selectedIdx =
				(selectionUi->selectedIdx + 1) % selectionUi->iconCnt;
		} else if(repeat & (KEY_UP | KEY_LEFT)) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			selectionUi->selectedIdx =
				(selectionUi->selectedIdx + selectionUi->iconCnt - 1) %
				selectionUi->iconCnt;
		} else if(BattleMenuStateWork->tmr == 0) {
			/* The link battle's time is up. */
			command = selectionUi->commandKinds[0];
			break;
		}
		/* BG1 on while the row is open; the exit below turns it off again. */
		syIOWriteQueueStore16((Uint32)&REG_DISPCNT,
			DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
				DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON);
		state->holdTilemapUpload = 0;
		sySynch(1);
	}
	if(mode != COMMAND_ROW_PARTY)
		BattleMenuStateWork->commandSelection[1] = selectionUi->selectedIdx;
	else
		BattleMenuStateWork->commandSelection[0] = selectionUi->selectedIdx;
	/* Every icon names the same entry, so it is released once per icon. */
	for(entryIdx = 0; entryIdx < selectionUi->iconCnt; entryIdx++)
		syVRAMCacheFreeEntry(selectionUi->entries[entryIdx].resource);
	state->holdTilemapUpload = 1;
	mnWindowClose(selectionUi->win, 1);
	/* Cover the strip again: seven panels of WINDOW_TILE_COMMAND_PANEL from
	   column 5. */
	{
		Sint32 panel;
		for(panel = 0; panel <= 6; panel++) {
			for(entryIdx = 0; entryIdx <= 2; entryIdx++) {
				Sint32 col;
				/* The VBlank handler uploads this tilemap
				   (mnWindowSystemUploadTilemap). */
				for(col = 0; col <= 2; col++)
					*(volatile Uint16 *)(state->tilemap +
						(entryIdx + COMMAND_ROW_TOP) * BG_SCREEN_WIDTH +
						panel * 3 + 5 + (col & 3)) = WINDOW_TILE_COMMAND_PANEL;
			}
		}
	}
	/* Restore the normal BG1 handler after clearing the command strip. */
	state->dirty = 1;
	sySynch(1);
	syCloseEvent(mnCommandRowPutBg1Behind);
	syIntFunction(IRQ_VCOUNT, 0, 0);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON |
			DISPCNT_OBJ_ON,
		IO_WRITE_HALFWORD);
	syHeapRelease(selectionUi);
	state->holdTilemapUpload = 0;
	sySynch(1);
	return command;
}

/* Switches the UI tilemap entries of the `width` x `height` tile rectangle at
   screen tile (x, y) to WINDOW_PALETTE or HIGHLIGHT_PALETTE by bit 12, keeping
   the rest of each entry: the menus' row highlight. `paletteBit` is the
   palette, of which only the low bit counts. */
void mnWindowHighlightRect(
	Sint32 x, Sint32 y, Sint32 width, Sint32 height, Sint32 paletteBit)
{
	WIN_STATE *state = WINDOW_STATE;

	paletteBit &= 1;
	paletteBit = BG_TILE_PALETTE(paletteBit);

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
	/* BUG: tests the bottom against the screen's width, so a window reaching
	   below the screen draws into the map's unseen rows. */
#if FIX_RECT_BOTTOM_CLIP
	if(y + height > DISPLAY_TILE_HEIGHT - 1) {
#else
	if(y + height > DISPLAY_TILE_WIDTH - 1) {
#endif
		height = DISPLAY_TILE_HEIGHT - y;
	}

	if(width > 0 && height > 0) {
		do {
			Uint16 *cell = &((Uint16 (*)[BG_SCREEN_WIDTH])state->tilemap)[y][x];
			Sint32	remaining = width;

			while(remaining != 0) {
				Uint32 tile = *cell;
				tile = (tile & ~BG_TILE_PALETTE(1)) | paletteBit;
				remaining--;
				*cell = tile;
				cell++;
			}

			mnWindowMarkTilemapRow(state, y);
			height--;
			y++;
		} while(height != 0);
	}
}

/* Clears the highlight from the party status window's party member columns,
   then highlights the columns of the units in `ids` (up to four, ended by
   BATTLE_UNIT_LIST_END): the party member whose commands are being chosen. */
Sint32 mnPartyStatHighlightMembers(Uint16 *ids)
{
	BATTLE_STATE *battle = BattleStateWork;
	Sint32		  cnt;
	Sint32		  i;
	Sint32		  j;

	cnt = btUnitListIdsStub(BATTLE_SIDE_MASK_PARTY, 0);
	mnWindowHighlightRect(29 - cnt * 6, 0, 25, 5, WINDOW_PALETTE);
	i = 0;
	if(ids[0] != BATTLE_UNIT_LIST_END) {
	next_id:
		for(j = 0; j < 4 && battle->partyUnits[j] != ids[i]; j++) {
			if(battle->partyUnits[j] == BATTLE_UNIT_LIST_END) {
				j = 4;
				break;
			}
		}
		if(j != 4)
			mnWindowHighlightRect(
				29 - (cnt - j) * 6, 0, 7, 5, HIGHLIGHT_PALETTE);
		if(++i < 4 && ids[i] != BATTLE_UNIT_LIST_END)
			goto next_id;
	}
	return 0;
}
