/* The row of command icons the player picks a command from. Part of
   screens.c. */

#include "menu/djinn_menu.h"

/* Opens a message at (x, y), waits for its text to finish, and returns its
   window so the caller can hold it open for a separate prompt. */
WINDOW *mnMessageShowAndWait(Sint32 msg, Sint32 x, Sint32 y)
{
	WINDOW *result;

	result = mnMessageOpenWindow(msg, x, y, MESSAGE_WINDOW_ZOOM);
	while(mnMessageAllTyped() == 0)
		sySynch(1);
	return result;
}

/* The tile column where a party member's command row ends and its name window
   begins. */
#define WINDOW_X	   JAPANESE_OR(23, 22)
/* The same column for the party's row of Fight, Flee and Status. */
#define PARTY_WINDOW_X 20

/* BG1's tile number of the command row's first icon tile. */
#define COMMAND_ROW_ICON_TILE (BG_CHAR_SIZE / (8 * 8))

/* BG1's map row shown on the command row's top row, at the battle background's
   scroll. */
#define COMMAND_ROW_BG1_ROW (COMMAND_ROW_TOP + BATTLE_BACKGROUND_SCROLL_Y / 8)

/* The selected icon's smallest scale, 8.8 fixed point, as it pulses. */
#define PULSE_SCALE_MIN Q_8_8(1.1875)

/* Runs a battle command row: the commands as icons along the bottom of the
   screen, the selected one pulsing as a sprite and named in a window, until a
   command is chosen. Returns the command chosen (COMMAND_ICON), -1 when B
   cancels, or -2 when L or R asks for the Status screen; the last two in a
   party member's row only. When the link battle's time runs out the row takes
   its first command. mnBattleCollectPartyCommands runs both rows.

   COMMAND_ROW_PARTY is the party's row at the start of a turn: Fight, Flee and
   Status. COMMAND_ROW_MEMBER, as any other mode, is a party member's row:
   Attack, Psynergy, Djinn once the party's Djinn are enough for a summon
   (DJINN_RECOVERY_TABLE.summonMask), Summon while any Djinni is on standby,
   Item and Defend. Each row remembers its choice in
   BATTLE_MENU_STATE.commandSelection. While the Djinn tutorial runs, the row
   presses the keys for the player. */
Sint32 mnBattleCommandRowRun(Sint32 mode, Sint32 fleeAllowed)
{
	Uint8					  standby[4];
	WIN_STATE				 *state;
	BATTLE_COMMAND_ROW_STATE *selectionUi;
	BATTLE_MENU_STATE		 *battleMenu;
	BATTLE_MENU_STATE		 *navigation;
	Bool					  stripDirty;
	Sint32					  standbyCount;
	Sint32					  pulseScale;
	Sint32					  command;
	Sint32					  entryIndex;
	Sint32					  keys;
	Sint32					  repeat;
	Sint32					  iconSlot;
	Sint32					  i;
	Sint32					  kindCount;

	state = WINDOW_STATE;
	/* One 1 KB entry for the selected icon's 32x32, 256-colour sprite. */
	iconSlot = syVRAMCacheAllocateEntry(32 * 32);
	stripDirty = TRUE;
	standbyCount = 0;
	battleMenu = BattleMenuStateWork;
	selectionUi = syHeapAllocateEWRAM(sizeof(*selectionUi));
	/* No icon colour placed yet; the shared palette starts at BG entry 1. */
	for(i = ARRAY_COUNT(selectionUi->palette.slot) - 1; i >= 0; i--)
		selectionUi->palette.slot[i] = GLYPH_PALETTE_SLOT_FREE;
	selectionUi->palette.count = 1;
	if(mode == COMMAND_ROW_PARTY)
		selectionUi->window = mnWindowCreate(PARTY_WINDOW_X, COMMAND_ROW_TOP,
			DISPLAY_TILE_WIDTH - PARTY_WINDOW_X, 3,
			WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
	else
		selectionUi->window = mnWindowCreate(WINDOW_X, COMMAND_ROW_TOP,
			DISPLAY_TILE_WIDTH - WINDOW_X, 3,
			WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
	selectionUi->drawnIndex = -1;
	if(mode == COMMAND_ROW_PARTY) {
		kindCount = 0;
		selectionUi->commandKinds[kindCount++] = COMMAND_ICON_FIGHT;
		if(RamHeapSlots->battleState->isLink == 0 &&
			battleMenu->swappedUnit == -1 && gmPartyCountMembersStub() > 4)
			selectionUi->commandKinds[kindCount++] = COMMAND_ICON_SWITCH;
		if(fleeAllowed > 0 &&
			MainRoutineStateWork.battleType != BATTLE_TYPE_NO_ESCAPE)
			selectionUi->commandKinds[kindCount++] = COMMAND_ICON_FLEE;
		selectionUi->commandKinds[kindCount++] = COMMAND_ICON_STATUS;
		selectionUi->commandKinds[kindCount] = -1;
		selectionUi->selectedIndex = BattleMenuStateWork->commandSelection[0];
	} else {
		Sint32 memberCount;

		selectionUi->selectedIndex = BattleMenuStateWork->commandSelection[1];
		memberCount = 0;
		selectionUi->commandKinds[memberCount++] = COMMAND_ICON_ATTACK;
		selectionUi->commandKinds[memberCount++] = COMMAND_ICON_PSYNERGY;
		if(*(Sint32 *)gmDjinnGetRecoveryTableStub(0) != 0) {
			btDjinnCountStandbyStub(0, standby);
			for(i = 0; i <= 3; i++)
				standbyCount += standby[i];
			selectionUi->commandKinds[memberCount++] = COMMAND_ICON_DJINN;
			if(standbyCount != 0)
				selectionUi->commandKinds[memberCount++] = COMMAND_ICON_SUMMON;
		}
		selectionUi->commandKinds[memberCount++] = COMMAND_ICON_ITEM;
		selectionUi->commandKinds[memberCount++] = COMMAND_ICON_DEFEND;
		selectionUi->commandKinds[memberCount] = -1;
	}
	for(entryIndex = 0; entryIndex <= 5; entryIndex++) {
		selectionUi->entries[entryIndex].resource = iconSlot;
		if(selectionUi->commandKinds[entryIndex] == -1)
			break;
		mnBattleCommandRowLoadIcon(
			selectionUi, entryIndex, selectionUi->commandKinds[entryIndex]);
	}
	selectionUi->iconCount = entryIndex;
	selectionUi->affine.scaleX = Q_8_8(1.25);
	selectionUi->affine.scaleY = PULSE_SCALE_MIN;
	selectionUi->affine.angle = 0;
	entryIndex = 0;
	if(entryIndex < selectionUi->iconCount) {
		do {
			Sint32 x = (entryIndex - selectionUi->iconCount) * 24;

			selectionUi->entries[entryIndex].posX = x + 155;
			if(mode != COMMAND_ROW_PARTY)
				selectionUi->entries[entryIndex].posX = x + WINDOW_X * 8 - 5;
			selectionUi->entries[entryIndex].posY = COMMAND_ROW_TOP * 8;
			entryIndex++;
		} while(entryIndex < selectionUi->iconCount);
	}
	/* BG1 shows the icons: behind the other layers above the row, in front from
	   its top line down (mnCommandRowPutBg1Behind). */
	sySetEvent(mnCommandRowPutBg1Behind, EVENT_PASS_VBLANK);
	syIntFunction(IRQ_VCOUNT, COMMAND_ROW_TOP * 8, mnCommandRowPutBg1InFront);

	for(;;) {
		/* PULSE_SCALE_MIN to 1.3125 times, over 16 ticks. */
		pulseScale = WindowSpriteBounceScales[(TickCount * 2) & 31];
		pulseScale = (pulseScale - Q_8_8(1)) / 4 + PULSE_SCALE_MIN;
		selectionUi->affine.scaleY = selectionUi->affine.scaleX = pulseScale;
		BgScrollWork[1].x = 0;
		BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
		/* Draw the strip once, six panels of 3x3 tiles ending at the name
		   window:
		   the panels left empty as WINDOW_TILE_COMMAND_PANEL in the UI tilemap,
		   then each icon's tiles in BG1's map, the UI tilemap cleared over
		   them. */
		if(stripDirty != 0) {
			stripDirty = FALSE;
			state->holdTilemapUpload = 1;
			if(mode != COMMAND_ROW_PARTY) {
				Sint32 panel;

				for(panel = 0; panel < 6 - selectionUi->iconCount; panel++) {
					for(entryIndex = 0; entryIndex <= 2; entryIndex++) {
						Sint32 column;

						for(column = 0; column <= 2; column++)
							*(state->tilemap[0] +
								(entryIndex + COMMAND_ROW_TOP) *
									BG_SCREEN_WIDTH +
								panel * 3 + WINDOW_X - 18 + (column & 3)) =
								WINDOW_TILE_COMMAND_PANEL;
					}
				}
				for(panel = 0; panel < selectionUi->iconCount; panel++) {
					for(entryIndex = 0; entryIndex <= 2; entryIndex++) {
						Sint32 column;

						for(column = 0; column <= 2; column++) {
							volatile Uint16 *vram =
								(volatile Uint16 *)BG_SCREEN_BLOCK(
									BATTLE_BG1_SCREEN_BLOCK) +
								COMMAND_ROW_BG1_ROW * BG_SCREEN_WIDTH +
								WINDOW_X;

							vram[panel * 3 + entryIndex * BG_SCREEN_WIDTH +
								(column & 3) - selectionUi->iconCount * 3] =
								panel * 16 + entryIndex * 4 + column +
								COMMAND_ROW_ICON_TILE;
							*(state->tilemap[0] +
								(entryIndex + COMMAND_ROW_TOP) *
									BG_SCREEN_WIDTH +
								panel * 3 + (column & 3) -
								selectionUi->iconCount * 3 + WINDOW_X) = 0;
						}
					}
				}
			} else {
				Sint32 panel;

				for(panel = 0; panel < 6 - selectionUi->iconCount; panel++) {
					for(entryIndex = 0; entryIndex <= 2; entryIndex++) {
						Sint32 column;

						for(column = 0; column <= 2; column++)
							*(state->tilemap[0] +
								(entryIndex + COMMAND_ROW_TOP) *
									BG_SCREEN_WIDTH +
								panel * 3 + 2 + (column & 3)) =
								WINDOW_TILE_COMMAND_PANEL;
					}
				}
				for(panel = 0; panel < selectionUi->iconCount; panel++) {
					for(entryIndex = 0; entryIndex <= 2; entryIndex++) {
						Sint32 column;

						for(column = 0; column <= 2; column++) {
							volatile Uint16 *vram =
								(volatile Uint16 *)BG_SCREEN_BLOCK(
									BATTLE_BG1_SCREEN_BLOCK) +
								COMMAND_ROW_BG1_ROW * BG_SCREEN_WIDTH +
								PARTY_WINDOW_X;

							vram[panel * 3 + entryIndex * BG_SCREEN_WIDTH +
								(column & 3) - selectionUi->iconCount * 3] =
								panel * 16 + entryIndex * 4 + column +
								COMMAND_ROW_ICON_TILE;
							*(state->tilemap[0] +
								(entryIndex + COMMAND_ROW_TOP) *
									BG_SCREEN_WIDTH +
								panel * 3 + (column & 3) -
								selectionUi->iconCount * 3 + PARTY_WINDOW_X) =
								0;
						}
					}
				}
			}
		}
		if(selectionUi->drawnIndex != selectionUi->selectedIndex) {
			mnWindowSpritePrepareForRedraw(selectionUi->window);
			mnTextDrawMessageAsTiles(
				selectionUi->entries[selectionUi->selectedIndex].kind +
					(Sint32)MsgCommandName,
				selectionUi->window, 0, 0);
			selectionUi->drawnIndex = selectionUi->selectedIndex;
			mnBattleCommandRowLoadIcon(selectionUi, selectionUi->selectedIndex,
				selectionUi->commandKinds[selectionUi->selectedIndex]);
		}
		selectionUi->affineIndex =
			syOAMMatrixAllocateFromTransform(&selectionUi->affine);
		for(entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++) {
			BATTLE_COMMAND_ROW_ICON *entry = &selectionUi->entries[entryIndex];

			if(entryIndex == selectionUi->selectedIndex) {
				entry->sprite.oam.f.affineIndex = selectionUi->affineIndex;
				entry->sprite.oam.f.affine = 3;
				entry->sprite.oam.f.x = entry->posX + pulseScale * 7 / 512 - 14;
				entry->sprite.oam.f.y = entry->posY + pulseScale * 3 / 256 - 20;
				syOAMAddSpriteAtDepth(entry, OAM_DEPTH_WINDOW + 1);
			} else {
				entry->sprite.oam.f.x = entry->posX;
				entry->sprite.oam.f.y = entry->posY;
				entry->sprite.oam.f.affineIndex = 0;
				entry->sprite.oam.f.affine = 0;
			}
		}

		/* The Djinn tutorial presses keys for the player: Felix is led to the
		   Djinn and Summon commands, Jenna and Sheba defend, and the party's
		   row takes Fight. */
		keys = PerWork.push;
		repeat = PerWork.repeat;
		navigation = BattleMenuStateWork;
		if(navigation->tutorialStep != 0) {
			keys = repeat = 0;
			if(navigation->tutorialDelay == 0) {
				if((Uint32)(navigation->commandActor - JENNA) <=
					SHEBA - JENNA) {
					if(selectionUi->commandKinds[selectionUi->selectedIndex] ==
						COMMAND_ICON_DEFEND)
						keys = repeat = KEY_A;
					else
						keys = repeat = KEY_LEFT;
					navigation->tutorialDelay = 30;
				} else if(navigation->commandActor == FELIX) {
					Sint32 kind =
						selectionUi->commandKinds[selectionUi->selectedIndex];

					if(kind == COMMAND_ICON_SUMMON ||
						(standbyCount == 0 && kind == COMMAND_ICON_DJINN)) {
						if(navigation->tutorialStep == 1) {
							void *msg;

							((WIN_STATE *)RamHeapSlots->windowState)
								->battleMode |= WINDOW_GLYPHS_BLENDED;
							if(selectionUi->commandKinds[selectionUi
									   ->selectedIndex] == COMMAND_ICON_DJINN)
								msg = mnMessageShowAndWait(
									mnDjinnGetGiveTutorialMessage() +
										DJINN_TUTORIAL_LINE_UNLEASH_COMMAND,
									15, 8);
							else if(selectionUi->commandKinds[selectionUi
											->selectedIndex] ==
								COMMAND_ICON_SUMMON)
								msg = mnMessageShowAndWait(
									mnDjinnGetGiveTutorialMessage() +
										DJINN_TUTORIAL_LINE_SUMMON,
									15, 8);
							((WIN_STATE *)RamHeapSlots->windowState)
								->battleMode &= ~WINDOW_GLYPHS_BLENDED;
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
			command = -2;
			break;
		}
		if(keys & KEY_A) {
			command = selectionUi->commandKinds[selectionUi->selectedIndex];
			break;
		}
		if(keys & KEY_B) {
			sySoundPlayStub(SE_MENU_CANCEL);
			command = -1;
			break;
		}
		if(repeat & (KEY_RIGHT | KEY_DOWN)) {
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			selectionUi->selectedIndex =
				(selectionUi->selectedIndex + 1) % selectionUi->iconCount;
		} else if(repeat & (KEY_LEFT | KEY_UP)) {
			sySoundPlayStub(SE_MENU_CURSOR_MOVE);
			selectionUi->selectedIndex =
				(selectionUi->selectedIndex + selectionUi->iconCount - 1) %
				selectionUi->iconCount;
		} else if(BattleMenuStateWork->timer == 0) {
			/* The link battle's time is up. */
			command = selectionUi->commandKinds[0];
			break;
		}
		syIOWriteQueueStore16((Uint32)&REG_DISPCNT,
			DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON |
				DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON);
		state->holdTilemapUpload = 0;
		sySynch(1);
	}
	if(mode != COMMAND_ROW_PARTY)
		BattleMenuStateWork->commandSelection[1] = selectionUi->selectedIndex;
	else
		BattleMenuStateWork->commandSelection[0] = selectionUi->selectedIndex;
	for(entryIndex = 0; entryIndex < selectionUi->iconCount; entryIndex++)
		syVRAMCacheFreeEntry(selectionUi->entries[entryIndex].resource);
	state->holdTilemapUpload = 1;
	mnWindowClose(selectionUi->window, 1);
	/* Cover the strip again. */
	{
		Sint32 panel;

		for(panel = 0; panel <= 6; panel++) {
			for(entryIndex = 0; entryIndex <= 2; entryIndex++) {
				Sint32 column;

				for(column = 0; column <= 2; column++)
					*(volatile Uint16 *)(state->tilemap[0] +
						(entryIndex + COMMAND_ROW_TOP) * BG_SCREEN_WIDTH +
						panel * 3 + 5 + (column & 3)) =
						WINDOW_TILE_COMMAND_PANEL;
			}
		}
	}
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
	WIN_STATE *state = RamHeapSlots->windowState;

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
			Uint16 *cell = &state->tilemap[y][x];
			Sint32	remaining = width;

			while(remaining != 0) {
				Uint32 tile = *cell;
				remaining--;
				tile = (tile & ~BG_TILE_PALETTE(1)) | paletteBit;
				*cell = tile;
				cell++;
			}
			WINDOW_MARK_TILEMAP_ROW(state, y);
			height--;
			y++;
		} while(height != 0);
	}
}

/* Clears the highlight from the party status window's party member columns,
   then highlights the columns of the units in `ids` (up to four, ended by
   BATTLE_UNIT_LIST_END): the party member whose commands are being chosen. A
   unit not in the party is skipped. The window is right-aligned on the 30-tile
   row, so its party member columns start at column 29 - 6 * members (the Djinn
   column, when shown, lies left of them). Returns 0.
   mnBattleCollectPartyCommands calls it. */
Sint32 mnPartyStatusHighlightMembers(Uint16 *ids)
{
	BATTLE_STATE *battle = RamHeapSlots->battleState;
	Sint32		  count;
	Sint32		  i;
	Sint32		  j;

	count = btUnitListIdsStub(BATTLE_SIDE_MASK_PARTY, 0);
	mnWindowHighlightRect(29 - count * 6, 0, 25, 5, WINDOW_PALETTE);
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
				29 - (count - j) * 6, 0, 7, 5, HIGHLIGHT_PALETTE);
		if(++i < 4 && ids[i] != BATTLE_UNIT_LIST_END)
			goto next_id;
	}
	return 0;
}

#undef WINDOW_X
#undef PARTY_WINDOW_X
