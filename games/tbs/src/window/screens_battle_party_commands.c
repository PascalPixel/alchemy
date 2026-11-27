// Collecting the party's battle commands. Part of screens.c.

// Flee's priority, above every other action's: the party flees first in the turn.
#define FLEE_PRIORITY       32766
// The priority an action takes when its Agility roll runs past 32767.
#define OVERFLOWED_PRIORITY 2000

// Classifies ability `act` for the target selection (the `kind` of mnBattleTargetRunSelection,
// BATTLE_TARGET_HINT), which shows what it would do on a party member and, for the curing and
// reviving hints, starts on one who needs it.
Sint32 mnBattleClassifyEntryKind(const ABILITY_DEFINITION *act)
{
	Sint32 abilityKind;
	Sint32 targetKind;

	targetKind  = TARGET_HINT_NONE;
	abilityKind = act->flags & ABILITY_FORMULA_MASK;
	if (abilityKind == ABILITY_FORMULA_HEAL_HP)
		targetKind = TARGET_HINT_HP;
	if (abilityKind == ABILITY_FORMULA_HEAL_PP)
		targetKind = TARGET_HINT_PP;
	if (act->effect == ABILITY_EFFECT_CURE_POISON)
		targetKind = TARGET_HINT_CURE_POISON;
	if (act->effect == ABILITY_EFFECT_RESTORE)
		targetKind = TARGET_HINT_RESTORE;
	if (act->effect == ABILITY_EFFECT_CURE_ALL)
		targetKind = TARGET_HINT_CURE_ALL;
	if (gmBattleIsReviveEffectStub(act->effect) != 0)
		targetKind = TARGET_HINT_REVIVE;
	return targetKind;
}

// Collects the party's commands for a turn into `commands`, one for each of the `commandCount`
// units in `actors` (a party member with two actions is listed twice), and returns how many it
// wrote: commandCount, 1 when the party flees (COMMAND_FLEE for the first actor), or -1 when the
// link fails.
Sint32 mnBattleCollectPartyCommands(BATTLE_ACTION *commands, Uint16 *actors, Sint32 commandCount)
{
	BATTLE_MENU_STATE *state;
	// Default: each local given NULL or 0 here is set for real before it is read.
	BATTLE_STATE      *battle        = NULL;
	BATTLE_STATE      *linkBattle    = NULL;
	Uint16            *peerSignature = NULL;
	Uint16            *transferWords = NULL;
	BATTLE_ACTION     *command       = NULL;
	UNIT              *unit          = NULL;
	WINDOW            *commandWindow = NULL;
	MENU_SPR          *iconSprite    = NULL;
	Uint16             memberList[2];
	Sint32             target;
	Sint32             unitId;
	Sint32             mode = 0;
	Sint32             commandKind;
	Sint32             parameter;
	Sint32             largeIconSlot;
	Sint32             smallIconSlot;
	Sint32             missedLinkTicks = 0;
	Sint32             commandIndex    = 0;
	Sint32             k               = 0;
	Sint32             priority;

	// An empty ID list clears every party panel's highlight and marks no unit before the next actor
	// is chosen.
	void               mnPartyStatusClearHighlights(void)
	{
		Uint16 data[2];
		data[0] = BATTLE_UNIT_LIST_END;
		mnPartyStatusHighlightMembers(data);
		btHighlightSetModesStub(data, HIGHLIGHT_SELECTED);
	}

	// Set up the shared command UI before the optional link handshake: VRAM for the chosen
	// command's 32x32 icon (1 KB) and the party member's portrait, the lists' page numbers
	// (PAGE_DIGITS_TILE), page arrows (LIST_PAGE_ARROW_VRAM) and range icons
	// (RANGE_ICON_FIRST_TILE), and no Djinni chosen yet (BATTLE_MENU_NO_DJINNI).
	target        = PLACE_PARTY;
	commandKind   = 0;
	parameter     = 0;
	largeIconSlot = syVRAMCacheAllocateEntry(COMMAND_ICON_BYTES);
	smallIconSlot = syVRAMCacheAllocateEntry(512);
	state         = syHeapSlotAllocateEWRAM(HEAP_SLOT_BATTLE_MENU, sizeof(BATTLE_MENU_STATE));
	mnCommandIconReloadSlot(largeIconSlot, 0);
	mnGraphicsExpandVRAMTilesByColorTable(
		(Uint16 *)(BG_CHAR_BLOCK(0) + PAGE_DIGITS_TILE * TILE_SIZE_4BPP));
	mnPageArrowsDraw(LIST_PAGE_ARROW_VRAM);
	mnRangeIconBuildTiles();
	battle = BattleStateWork;
	for (k = 0; k < 8; k++)
	{
		state->chosenDjinn[k] = BATTLE_MENU_NO_DJINNI;
	}
	for (k = 0; k < BATTLE_MENU_ICON_COUNT; k++)
	{
		state->iconShown[k] = 0;
	}
	state->targetViewOffset    = 0;
	state->viewOffset          = 0;
	state->timerWindow         = NULL;
	state->timer               = -1;
	state->commandSelection[0] = 0;
	state->commandSelection[1] = 0;
	state->linkTimer           = 0;
	state->timerHidden         = 0;
	// Both peers announce the command-entry phase before accepting commands: this side writes
	// "VSST" into halfwords 4 to 7 of its signature, then waits for the partner's "VSST" (or
	// "EDVS", already done) while its first four read "EXEC".
	if (battle->isLink != 0)
	{
		state->linkTimer = 1;
		transferWords    = (Uint16 *)SerialTransfer.payload;
		transferWords[4] = 'V';
		transferWords[5] = 'S';
		transferWords[6] = 'S';
		transferWords[7] = 'T';
		missedLinkTicks  = 0;
		for (;;)
		{
			peerSignature = (Uint16 *)SerialPeerPayloads[battle->linkSide ^ 1].payload;
			if (battle->linkPaused != 0)
			{
				commandCount     = -1;
				state->linkTimer = 0;
				break;
			}
			if ((LinkStatusWork & LINK_STATUS_HEARD_BOTH) != LINK_STATUS_HEARD_BOTH)
			{
				if (++missedLinkTicks > LINK_MAX_MISSED_TICKS)
				{
					commandCount     = -1;
					state->linkTimer = 0;
					break;
				}
			}
			else
			{
				missedLinkTicks = 0;
				if (peerSignature[4] == 'V' && peerSignature[5] == 'S' && peerSignature[6] == 'S'
				    && peerSignature[7] == 'T')
				{
					break;
				}
				if (peerSignature[4] == 'E' && peerSignature[5] == 'D' && peerSignature[6] == 'V'
				    && peerSignature[7] == 'S')
				{
					break;
				}
				if (!(peerSignature[0] == 'E' && peerSignature[1] == 'X' && peerSignature[2] == 'E'
				      && peerSignature[3] == 'C'))
				{
					commandCount     = -1;
					state->linkTimer = 0;
					break;
				}
			}
			sySynch(1);
		}
	}
	sySetEvent(mnBattleMenuUpdate, EVENT_PASS_TICK);

	while (1)
	{
		mnPartyStatusClearHighlights();
		state->iconShown[BATTLE_MENU_ICON_PORTRAIT] = 0;
		state->commandActor                         = -1;
		state->tutorialStep                         = 0;
		if (gmFlagGetStub(FLAG_DJINN_LESSON) != 0)
		{
			state->tutorialStep  = 1;
			state->tutorialDelay = 60;
		}
		// The party's row; with no party member to choose, as if Fight were taken.
		if (commandCount > 0)
		{
			mode = mnBattleCommandRowRun(COMMAND_ROW_PARTY);
		}
		else
		{
			mode = COMMAND_ICON_FIGHT;
		}
		// Status: the party's Status screens, from the first party member; the debug build with
		// Start held lists the enemies instead.
		if (mode == COMMAND_ICON_STATUS)
		{
			Uint16 *block;
			Sint32  selection;
			Sint32  handle;

			block = syHeapAllocate(12);
			if (DebugMode != 0 && (PerData & KEY_START) != 0)
			{
				selection = BATTLE_SIDE_MASK_ENEMIES;
			}
			else
			{
				selection = BATTLE_SIDE_MASK_PARTY;
			}
			handle = btUnitListIdsStub(selection, block);
			sySynch(1);
			mnBattleStatusScreenRun(block, handle, block[0]);
			syHeapRelease(block);
			continue;
		}
		// Flee, unless the battle forbids it: one COMMAND_FLEE for the party, first in the turn.
		if (mode == COMMAND_ICON_FLEE)
		{
			if (mnBattleRefuseFlee() != 0)
			{
				continue;
			}
			commandCount      = 1;
			command           = commands;
			command->unitId   = actors[0];
			command->priority = FLEE_PRIORITY;
			command->command  = COMMAND_FLEE;
			command->param    = 0;
			command->target   = PLACE_PARTY;
			break;
		}
		if (mode != COMMAND_ICON_FIGHT)
		{
			break;
		}
		sySoundPlayStub(SE_CAST);
		commandIndex = 0;
		if (commandIndex >= commandCount)
		{
			break;
		}

		// Each party member in turn.
		for (;;)
		{
			if (commandIndex == 0)
			{
				btDjinnCountStandbyStub(0, state->standbyDjinn[0]);
			}
			else
			{
				Sint32 k;

				for (k = 0; k < ELEMENT_COUNT; k++)
				{
					state->standbyDjinn[commandIndex][k] = state->standbyDjinn[commandIndex - 1][k];
				}
			}

			command = &commands[commandIndex];
			unitId  = actors[commandIndex];
			// UB: gmUnitGetStub returns a pointer, not the cast's doubleword result.
#if FIX_FUNCTION_SIGNATURES
			unit = gmUnitGetStub(unitId);
#else
			unit = (UNIT *)(Sint32)((Sint64 (*)(Sint32))gmUnitGetStub)(unitId);
#endif
			// The party member's row starts on Attack; its portrait shows at the left.
			BattleMenuStateWork->commandActor        = unitId;
			BattleMenuStateWork->commandSelection[1] = 0;
			iconSprite                  = &BattleMenuStateWork->icons[BATTLE_MENU_ICON_PORTRAIT];
			iconSprite->oam.word.attr01 = OBJ_SIZE_32X32 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
			iconSprite->oam.word.attr23 = 0;
			iconSprite->oam.f.tile      = mnPortraitLoad(unit->id, smallIconSlot);
			iconSprite->oam.f.x         = BATTLE_PORTRAIT_X;
			iconSprite->oam.f.y         = 128;
			iconSprite->oam.f.palette   = PORTRAIT_PALETTE;
			state->iconShown[BATTLE_MENU_ICON_PORTRAIT] = 1;

		mark_visible:
			mnWindowMarkVisibleTextTiles();
			BattleMenuStateWork->iconShown[BATTLE_MENU_ICON_COMMAND] = 0;
			BattleMenuStateWork->chosenDjinn[commandIndex]           = BATTLE_MENU_NO_DJINNI;
			// memberList is the party member alone, ended by BATTLE_UNIT_LIST_END: marked in the
			// battle and highlighted in the party status window.
			memberList[0]                                            = unitId;
			memberList[1]                                            = BATTLE_UNIT_LIST_END;
			btHighlightSetModesStub(memberList, HIGHLIGHT_SELECTED);
			mnPartyStatusHighlightMembers(memberList);
			sySynch(1);
			mode = mnBattleCommandRowRun(COMMAND_ROW_MEMBER);
			sySynch(1);
			if (mode == COMMAND_ROW_STATUS)
			{
				void  *block;
				Sint32 handle;

				block  = syHeapAllocate(12);
				handle = btUnitListIdsStub(BATTLE_SIDE_MASK_PARTY, block);
				state->iconShown[BATTLE_MENU_ICON_PORTRAIT] = 0;
				sySynch(1);
				mnBattleStatusScreenRun(block, handle, unitId);
				state->iconShown[BATTLE_MENU_ICON_PORTRAIT] = 1;
				syHeapRelease(block);
				continue;
			}
			btHighlightSetModesStub(memberList, HIGHLIGHT_OFF);
			// B: back to the party member before, or from the first to the party's row.
			if (mode == COMMAND_ROW_CANCELLED)
			{
				if (commandIndex == 0)
				{
					break;
				}
				commandIndex--;
				continue;
			}

			// Out of time in a link battle: Defend.
			if (BattleMenuStateWork->timer == 0)
			{
				mode = COMMAND_ICON_DEFEND;
			}
			iconSprite = &BattleMenuStateWork->icons[BATTLE_MENU_ICON_COMMAND];
			MENU_SPR_INIT(*iconSprite,
			              OBJ_SIZE_32X32 | OBJ_256_COLORS | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
			iconSprite->oam.f.tile = mnCommandIconReloadSlot(largeIconSlot, mode);
			if (mode == COMMAND_ICON_DJINN)
			{
				iconSprite->oam.f.x = 128;
			}
			else
			{
				iconSprite->oam.f.x = 96;
			}
			iconSprite->oam.f.y                        = 136;
			state->iconShown[BATTLE_MENU_ICON_COMMAND] = 1;
			// Each command's menus; commandKind is the COMMAND_ value recorded.
			switch (mode)
			{
				case COMMAND_ICON_ATTACK:
					// Attack: one enemy, range 1; COMMAND_ATTACK.
					commandWindow = mnWindowCreate(11, 17, 11, 3, MENU_WINDOW_FLAGS);
					mnTextDrawMessageAsTiles(
						MsgCommandName, commandWindow, BATTLE_COMMAND_NAME_X, 0);
					state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 64;
					sySoundPlayStub(SE_MENU_CONFIRM);
					mode = mnBattleTargetRunSelection(
						unitId, ABILITY_TARGET_FOES, 1, TARGET_HINT_NONE);
					mnWindowClose(commandWindow, 1);
					if (mode == -1)
					{
						goto mark_visible;
					}
					commandKind    = COMMAND_ATTACK;
					target         = mode;
					command->range = 1;
					break;

				case COMMAND_ICON_PSYNERGY:
				{
					MENU_SPR           *iconSprite;
					ABILITY_DEFINITION *ability;
					WIN_STATE          *windowState;
					Uint16             *list;
					Uint8              *pick;
					Sint32              range;
					Sint32              handle;
					Sint32              count;
					Sint32              k;
					Sint32              entryValue;

					// Psynergy: the party member's abilities usable in battle, from its 32 action
					// slots (pick keeps each one's slot); then the ability's window, TEXT_COLOR_RED
					// when the PP fall short and TEXT_COLOR_PINK while sealed, and its target;
					// COMMAND_PSYNERGY with the ability as parameter.
					sySoundPlayStub(SE_MENU_CONFIRM);
					// DEAD: mode cleared before the list sets it; the ROM keeps its mov.
					mode = 0;
					BATTLE_MENU_SET_CURSOR(0, 0, 0);
				psynergy_list:
					list                                           = state->listAbilities;
					pick                                           = state->listSlots;
					state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 48;
					count                                          = 0;
					for (k = 0;
					     k != PSYNERGY_SLOTS
					     && (entryValue = unit->psynergy[k].encodedAction & ACTION_ID_MASK) != 0;
					     k++)
					{
						if ((gmAbilityGetDirectStub(entryValue)->flags & BATTLE_OK) != 0)
						{
							pick[count] = k;
							list[count] = entryValue;
							count++;
						}
					}
					pick[count] = 0;
					list[count] = 0;
					mode        = mnBattlePsynergyMenuRun(unitId, list, count);
					if (mode == -1)
					{
						goto mark_visible;
					}
					parameter     = unit->psynergy[pick[mode]].encodedAction & ACTION_ID_MASK;
					ability       = gmAbilityGetDirectStub(parameter);
					range         = ability->range;
					iconSprite    = &state->icons[BATTLE_MENU_ICON_ABILITY];
					handle        = syVRAMCacheAllocateEntry(128);
					windowState   = WINDOW_STATE;
					commandWindow = mnWindowCreate(8, 17, 18, 3, MENU_WINDOW_FLAGS);
					state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 40;
					iconSprite->oam.word.attr01 =
						OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
					iconSprite->oam.word.attr23 = 0;
					iconSprite->oam.f.x         = commandWindow->x * 8 + 8;
					iconSprite->oam.f.y         = commandWindow->y * 8 + 4;
					ASM_NOTHING();
					iconSprite->oam.f.tile = mnAbilityIconLoadIntoSlot(parameter, handle);
					state->iconShown[BATTLE_MENU_ICON_ABILITY] = 1;
					windowState->textPalette                   = BASIC_COLORS_PALETTE;
					if (ability->ppCost > unit->pp)
					{
						mnTextSetColor(TEXT_COLOR_RED);
					}
					else if (unit->psySeal != 0)
					{
						mnTextSetColor(TEXT_COLOR_PINK);
					}
					mnTextDrawMessageAsTiles(parameter + MsgAbilityName, commandWindow, 16, 0);
					mnTextDrawNumberAtOffset(ability->ppCost, 2, commandWindow, 104, 0);
					windowState->textPalette = WINDOW_PALETTE;
					mnTextSetColor(TEXT_COLOR_WHITE);
					mnWindowSetTilemapEntry(commandWindow, PSYNERGY_COST_FIRST_TILE, 11, 0, 0);
					mnWindowSetTilemapEntry(commandWindow, PSYNERGY_COST_SECOND_TILE, 12, 0, 0);
					if (ability->element != ELEMENT_NONE)
					{
						mnWindowSetTilemapEntry(
							commandWindow, ability->element + LIST_TILE_SUMMON_ELEMENT, 15, 0, 0);
					}
					command->range = range;
					sySoundPlayStub(SE_MENU_CONFIRM);
					mode = mnBattleTargetRunSelection(
						unitId, ability->targetMode, range, mnBattleClassifyEntryKind(ability));
					state->iconShown[BATTLE_MENU_ICON_ABILITY] = 0;
					syVRAMCacheFreeEntry(handle);
					mnWindowClose(commandWindow, 1);
					if (mode == -1)
					{
						goto psynergy_list;
					}
					commandKind = COMMAND_PSYNERGY;
					target      = mode;
					break;
				}

				case COMMAND_ICON_SUMMON:
				{
					const SUMMON_DEFINITION *summon;
					ABILITY_DEFINITION      *ability;
					MENU_SPR                *iconSprite;
					Sint32                   handle;
					Sint32                   ok;
					Sint32                   j;
					Sint32                   n;

					// Summon: the summon list from this command's standby Djinn, the summon's
					// window with its cost (its name in TEXT_COLOR_RED and the error sound when it
					// cannot be paid for), and its target; COMMAND_SUMMON with the summon as
					// parameter.
					sySoundPlayStub(SE_MENU_CONFIRM);
					BATTLE_MENU_SET_CURSOR(0, 0, 0);
				summon_list:
					state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = BATTLE_SUMMON_ICON_X;
					mnPartyStatusHighlightMembers(memberList);
					mode = mnBattleSummonMenuRun(0, 0, state->standbyDjinn[commandIndex]);
					mnPartyStatusHighlightMembers(memberList);
					if (mode == -1)
					{
						goto mark_visible;
					}
					commandKind = COMMAND_SUMMON;
					parameter   = mode;
					sySynch(1);
					iconSprite = &state->icons[BATTLE_MENU_ICON_ABILITY];
					summon     = gmSummonDefinitionGetNearStub(mode);
					ability    = gmAbilityGetDirectStub(summon->abilityId);
					handle     = syVRAMCacheAllocateEntry(128);
					commandWindow =
						mnWindowCreate(10, 17, BATTLE_SUMMON_WINDOW_WIDTH, 3, MENU_WINDOW_FLAGS);
					n = 0;
					for (j = 0; j < ELEMENT_COUNT; j++)
					{
						if (summon->djinnRequired[j] > state->standbyDjinn[commandIndex][j])
						{
							break;
						}
					}
					ok                                             = (j == ELEMENT_COUNT);
					state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 56;
					iconSprite->oam.word.attr01 =
						OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
					iconSprite->oam.word.attr23 = 0;
					ASM_NOTHING();
					iconSprite->oam.f.x = commandWindow->x * 8 + 8;
					iconSprite->oam.f.y = commandWindow->y * 8 + 4;
					iconSprite->oam.f.tile =
						mnAbilityIconLoadIntoSlot(summon->abilityId & ACTION_ID_MASK, handle);
					state->iconShown[BATTLE_MENU_ICON_ABILITY] = 1;
					if (ok == 0)
					{
						mnTextSetColor(TEXT_COLOR_RED);
					}
					mnTextDrawMessageAsTiles(gmSummonDefinitionGetNearStub(mode)->abilityId
					                             + MsgAbilityName,
					                         commandWindow,
					                         16,
					                         0);
					for (j = 0; j < ELEMENT_COUNT; j++)
					{
						if (summon->djinnRequired[j] != 0)
						{
							mnWindowSetTilemapEntry(commandWindow,
							                        j + LIST_TILE_SUMMON_ELEMENT,
							                        n * 2 + BATTLE_SUMMON_ELEMENT_COLUMN,
							                        0,
							                        0);
							mnWindowPutGlyph(commandWindow,
							                 summon->djinnRequired[j] + '0',
							                 n * 2 + BATTLE_SUMMON_COST_COLUMN,
							                 0,
							                 GLYPH_DRAW_CHARACTER);
							n++;
						}
					}
					if (ok != 0)
					{
						sySoundPlayStub(SE_MENU_CONFIRM);
					}
					else
					{
						sySoundPlayStub(SE_MENU_ERROR);
					}
					mode                                       = mnBattleTargetRunSelection(unitId,
                                                      ability->targetMode,
                                                      ability->range,
                                                      mnBattleClassifyEntryKind(ability));
					command->range                             = ability->range;
					state->iconShown[BATTLE_MENU_ICON_ABILITY] = 0;
					syVRAMCacheFreeEntry(handle);
					mnWindowClose(commandWindow, 1);
					if (mode == -1)
					{
						goto summon_list;
					}
					// Spend the cost from this command's standby Djinn: an element that falls short
					// is left at 0, and the elements after it are not spent.
					for (j = 0; j < ELEMENT_COUNT; j++)
					{
						if (summon->djinnRequired[j] > state->standbyDjinn[commandIndex][j])
						{
							state->standbyDjinn[commandIndex][j] = 0;
							break;
						}
						state->standbyDjinn[commandIndex][j] -= summon->djinnRequired[j];
					}
					target = mode;
					break;
				}

				case COMMAND_ICON_DJINN:
				{
					ABILITY_DEFINITION *ability;
					WINDOW             *hint;
					Sint32              range;
					Sint32              element;
					Sint32              index;

					// Djinn: the party member's Djinn list; COMMAND_DJINN with the Djinni's entry
					// as parameter, kept in chosenDjinn so no later command takes it.
					sySoundPlayStub(SE_MENU_CONFIRM);
					BATTLE_MENU_SET_CURSOR(0, 0, 0);
				djinn_list:
					state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 144;
					BattleMenuStateWork->chosenDjinn[commandIndex] = BATTLE_MENU_NO_DJINNI;
					mode           = mnBattleDjinnMenuRun(unitId, 1);
					command->range = 0;
					if (mode == -1)
					{
						goto mark_visible;
					}
					commandKind                                    = COMMAND_DJINN;
					parameter                                      = mode;
					BattleMenuStateWork->chosenDjinn[commandIndex] = mode;
					element = DJINN_COMMAND_ELEMENT(parameter);
					index   = DJINN_COMMAND_INDEX(parameter);
					if (gmDjinnIsSetStub(unitId, element, index) != 0)
					{
						ability = gmAbilityGetDirectStub(gmDjinnGetAbilityIdStub(element, index));
						range   = ability->range;
						commandWindow = mnWindowCreate(11, 17, 10, 3, MENU_WINDOW_FLAGS);
						state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 64;
						mnWindowSetTilemapEntry(
							commandWindow, element + LIST_TILE_SUMMON_ELEMENT, 0, 0, 0);
						mnTextDrawMessageAsTiles(element * DJINN_PER_ELEMENT + index + MsgDjinnName,
						                         commandWindow,
						                         16,
						                         0);
						command->range = range;
						sySynch(1);
						sySoundPlayStub(SE_MENU_CONFIRM);
						mode = mnBattleTargetRunSelection(
							unitId, ability->targetMode, range, mnBattleClassifyEntryKind(ability));
						if (state->tutorialStep != 0)
						{
							hint = mnMessageShowAndWait(MsgDjinnUnleashHint, 15, 8);
							while (mnMessageAllTyped() == 0)
							{
								sySynch(1);
							}
							mnWindowClose(hint, 1);
							state->tutorialStep++;
							state->tutorialDelay = 45;
						}
						mnWindowClose(commandWindow, 1);
						if (mode == -1)
						{
							goto djinn_list;
						}
						target = mode;
						break;
					}
					commandWindow = mnWindowCreate(11, 17, 10, 3, MENU_WINDOW_FLAGS);
					state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 64;
					mnTextSetColor(TEXT_COLOR_RED);
					mnWindowSetTilemapEntry(
						commandWindow, element + LIST_TILE_SUMMON_ELEMENT, 0, 0, 0);
					mnTextDrawMessageAsTiles(
						element * DJINN_PER_ELEMENT + index + MsgDjinnName, commandWindow, 16, 0);
					mnTextSetColor(TEXT_COLOR_WHITE);
					command->range = 1;
					sySynch(1);
					sySoundPlayStub(SE_MENU_CONFIRM);
					mode = mnBattleTargetRunSelection(
						unitId, ABILITY_TARGET_SELF, 0, TARGET_HINT_SET_DJINNI);
					mnWindowClose(commandWindow, 1);
					if (mode == -1)
					{
						goto djinn_list;
					}
					target = mode;
					break;
				}

				case COMMAND_ICON_ITEM:
				{
					MENU_SPR           *iconSprite;
					ABILITY_DEFINITION *ability;
					Uint16             *list;
					Uint8              *pick;
					Sint32              range;
					Sint32              handle;
					Sint32              count;
					Sint32              k;
					Sint32              entryValue;
					Sint32              selection;

					// Item: the party member's 15 inventory places, the usable items first and the
					// rest after (pick keeps each one's place); then the item's window and its
					// action's target; COMMAND_ITEM with the place as parameter.
					sySoundPlayStub(SE_MENU_CONFIRM);
					// DEAD: mode cleared before the list sets it; the ROM keeps its mov.
					mode = 0;
					BATTLE_MENU_SET_CURSOR(0, 0, 0);
				item_list:
					state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 96;
					pick                                           = state->listSlots;
					list                                           = state->listAbilities;
					count                                          = 0;
					for (k = 0; k != INVENTORY_SLOTS && (entryValue = unit->inventory[k]) != 0; k++)
					{
						gmItemGetDirectStub(entryValue);
						if (mnItemCheckBattleUse(unitId, entryValue) == BATTLE_ITEM_USABLE)
						{
							list[count]   = entryValue;
							pick[count++] = k;
						}
					}
					for (k = 0; k != INVENTORY_SLOTS && (entryValue = unit->inventory[k]) != 0; k++)
					{
						gmItemGetDirectStub(entryValue);
						if (mnItemCheckBattleUse(unitId, entryValue) != BATTLE_ITEM_USABLE)
						{
							list[count]   = entryValue;
							pick[count++] = k;
						}
					}
					list[count] = 0;
					mode        = mnBattleItemMenuRun(unitId, list, count);
					if (mode == -1)
					{
						goto mark_visible;
					}
					selection = pick[mode];
					parameter = selection;
					ability   = gmAbilityGetDirectStub(
                        gmItemGetDirectStub(unit->inventory[selection])->abilityId);
					range      = ability->range;
					iconSprite = &state->icons[BATTLE_MENU_ICON_ABILITY];
					handle     = syVRAMCacheAllocateEntry(128);
					commandWindow =
						mnWindowCreate(9, 17, BATTLE_ITEM_WINDOW_WIDTH, 3, MENU_WINDOW_FLAGS);
					state->icons[BATTLE_MENU_ICON_COMMAND].oam.f.x = 48;
					iconSprite->oam.word.attr01 =
						OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
					iconSprite->oam.word.attr23 = 0;
					ASM_NOTHING();
					iconSprite->oam.f.x = commandWindow->x * 8 + 8;
					iconSprite->oam.f.y = commandWindow->y * 8 + 4;
					iconSprite->oam.f.tile =
						mnItemIconLoadIntoSlot(unit->inventory[selection], handle);
					state->iconShown[BATTLE_MENU_ICON_ABILITY] = 1;
					mnTextDrawMessageAsTiles((unit->inventory[selection] & INVENTORY_ITEM_ID_MASK)
					                             + MsgItemName,
					                         commandWindow,
					                         24,
					                         0);
					command->range = range;
					sySoundPlayStub(SE_MENU_CONFIRM);
					mode = mnBattleTargetRunSelection(
						unitId, ability->targetMode, range, mnBattleClassifyEntryKind(ability));
					state->iconShown[BATTLE_MENU_ICON_ABILITY] = 0;
					syVRAMCacheFreeEntry(handle);
					mnWindowClose(commandWindow, 1);
					if (mode == -1)
					{
						goto item_list;
					}
					commandKind = COMMAND_ITEM;
					target      = mode;
					break;
				}

				case COMMAND_ICON_DEFEND:
					// Defend: COMMAND_DEFEND, nothing to choose.
					commandKind = COMMAND_DEFEND;
					break;
			}

			sySoundPlayStub(SE_CHIME);
			command->unitId = unitId;
			// Draw the priority random value once, only for a nonzero agility.
			priority        = unit->agility;
			if (priority != 0)
			{
				priority += (Uint32)(unit->agility * syRandom()) >> 20;
			}
			command->priority = priority;
			if (commandIndex != 0 && actors[commandIndex] == actors[commandIndex - 1])
			{
				command->priority = (Sint16)priority / 2;
			}
			// A priority past 32767 reads negative as Sint16.
			if ((Sint16)command->priority < 0)
			{
				command->priority = OVERFLOWED_PRIORITY;
			}
			command->command = commandKind;
			command->param   = parameter;
			command->target  = target;
			commandIndex++;
			if (commandIndex < commandCount)
			{
				continue;
			}
			break;
		}

		if (commandIndex < commandCount)
		{
			continue;
		}
		break;
	}

	// Publish completion, then wait for the peer to finish or disconnect.
	if (state->linkTimer != 0)
	{
		transferWords    = (Uint16 *)SerialTransfer.payload;
		transferWords[4] = 'E';
		transferWords[5] = 'D';
		transferWords[6] = 'V';
		transferWords[7] = 'S';
	}
	if (state->timerWindow != 0)
	{
		mnWindowClose(state->timerWindow, 1);
	}
	syVRAMCacheFreeEntry(smallIconSlot);
	syVRAMCacheFreeEntry(largeIconSlot);
	syCloseEvent(mnBattleMenuUpdate);
	if (state->linkTimer != 0)
	{
		// Keep the completion wait visible while the peer finishes its commands. UB: declared to
		// return a value it never returns.
		Sint32 mnLinkDrawWaitingForOpponent(void)
		{
			Uint16 data[64];
			BATTLE_MENU_MESSAGE_COPY(MsgWaitingForOpponent, data, BATTLE_MENU_MESSAGE_CAPACITY);
			mnTextDrawWideStringAtOffset(data, state->timerWindow, 0, 4);
#if FIX_FUNCTION_SIGNATURES
			return 0;
#endif
		}

		// The wait message shows only when the countdown never opened its window, that is while the
		// partner is still choosing; a timer window was closed above and is only forgotten here.
		// mode counts the ticks without the link, and past LINK_MAX_MISSED_TICKS gives up (-1).
		linkBattle = BattleStateWork;
		mode       = 0;
		if (state->timerWindow == 0 && linkBattle->linkPaused == 0)
		{
			state->timerWindow = mnWindowCreate(0,
			                                    16,
			                                    30,
			                                    4,
			                                    WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS
			                                        | WINDOW_FLAG_BORDERED_CANVAS);
			mnTextFillCanvasBackground();
			mnLinkDrawWaitingForOpponent();
		}
		else
		{
			state->timerWindow = 0;
		}
		for (;;)
		{
			peerSignature = (Uint16 *)SerialPeerPayloads[linkBattle->linkSide ^ 1].payload;
			if (linkBattle->linkPaused != 0)
			{
				commandCount = -1;
				break;
			}
			if ((LinkStatusWork & LINK_STATUS_HEARD_BOTH) != LINK_STATUS_HEARD_BOTH)
			{
				mode++;
				if (mode > LINK_MAX_MISSED_TICKS)
				{
					commandCount = -1;
					break;
				}
			}
			else
			{
				mode = 0;
				if (peerSignature[4] == 'E' && peerSignature[5] == 'D' && peerSignature[6] == 'V'
				    && peerSignature[7] == 'S')
				{
					break;
				}
				if (!(peerSignature[4] == 'V' && peerSignature[5] == 'S' && peerSignature[6] == 'S'
				      && peerSignature[7] == 'T'))
				{
					mode = 1;
				}
			}
			sySynch(1);
		}
		if (state->timerWindow != 0)
		{
			mnWindowClose(state->timerWindow, 1);
		}
	}
	btCameraSetCommandViewStub(0);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_MENU);
	return commandCount;
}

#undef FLEE_PRIORITY
#undef OVERFLOWED_PRIORITY
