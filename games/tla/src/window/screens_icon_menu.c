/* Collecting the party's battle commands, then the icon menus: a row of command
   icons with a label window, the selected icon pulsing, as the field menu, the
   File Screen, the transfer screens, the shops, the sanctums, Yes or No and
   Coins or Lucky Medals use it. Part of screens.c. */

/* While a party member's commands are chosen (mnBattleCollectPartyCommands),
   where the Japanese layout differs: the chosen summon's window and the columns
   of its first Djinn cost, the element icon and then the count, and the chosen
   item's window, in tiles; the party member's portrait, and the chosen
   command's icon beside the summon list, beside the chosen summon and beside
   the Djinn list, in pixels. */
#define SUMMON_WINDOW_X				 JAPANESE_OR(9, 10)
#define BATTLE_SUMMON_WINDOW_WIDTH	 JAPANESE_OR(16, 17)
#define BATTLE_SUMMON_ELEMENT_COLUMN JAPANESE_OR(10, 11)
#define BATTLE_SUMMON_COST_COLUMN	 JAPANESE_OR(11, 12)
#define BATTLE_ITEM_WINDOW_WIDTH	 JAPANESE_OR(15, 16)
#define BATTLE_PORTRAIT_X			 JAPANESE_OR(4, 0)
#define BATTLE_SUMMON_ICON_X		 JAPANESE_OR(88, 80)
#define SUMMON_CHOSEN_ICON_X		 JAPANESE_OR(48, 56)
#define DJINN_MENU_ICON_X			 JAPANESE_OR(144, 140)

/* Where "Attack" is written in its window once chosen, in pixels. */
#define BATTLE_COMMAND_NAME_X FRENCH_OR(14, 16)

/* The chosen command's icon beside the Psynergy list, in pixels. */
#define PSYNERGY_MENU_ICON_X BY_LANGUAGE(48, 48, 48, 40, 40, 48)

/* The chosen command's icon beside the window that names Attack, the chosen
   Psynergy, the chosen Djinni or the chosen item, and beside the item list, in
   pixels. */
#define ATTACK_ICON_X		   64
#define PSYNERGY_CHOSEN_ICON_X 40
#define DJINN_CHOSEN_ICON_X	   64
#define ITEM_MENU_ICON_X	   96
#define ITEM_CHOSEN_ICON_X	   48

/* Flee's priority, above every other action's: the party flees first in the
   turn. */
#define FLEE_PRIORITY		32766
/* The priority an action takes when its Agility roll runs past 32767. */
#define OVERFLOWED_PRIORITY 2000

/* The x of the first command icon's sprite. */
#define COMMAND_ICON_X(state) ((state)->icons[0].oam.f.x)

/* Collects the party's commands for a turn into `commands`, one for each of the
   `commandCnt` units in `actors`, with two nested functions: the party
   panels' clear and the link wait's message (mnLinkDrawWaitingForOpponent).
   Unlike The Broken Seal's, each round first appends the forced actions of
   statused members (mnBattlePartySplitChoosers), whose count is added to the
   result; B on the party's row undoes a reserve swap; Switch swaps a reserve
   member in (mnBattlePartyChooseReserve after choosing the slot); Status lists
   the reserve as well; and the Djinn tutorial's hint follows an unleash. */
Sint32 mnBattleCollectPartyCommands(
	BATTLE_ACTION *commands, Uint16 *actors, Sint32 commandCnt)
{
	Uint16			   highlighted[2];
	BATTLE_MENU_STATE *state;
	BATTLE_ACTION	  *commandsOut;
	Sint32			   forcedCnt;
	BATTLE_ACTION	  *command;
	UNIT			  *unit;
	Sint32			   tgt;
	Sint32			   actorId;
	Sint32			   commandKind;
	Sint32			   parameter;
	Sint32			   largeIconSlot;
	Sint32			   smallIconSlot;
	Sint32			   commandIdx;
	Sint32			   summonIconSlot;
	Sint32			   choice;
	WINDOW			  *commandWin;

	void btDrawPartyPanelsWithEmptyList(void)
	{
		Uint16 data[2];

		data[0] = BATTLE_UNIT_LIST_END;
		mnPartyStatHighlightMembers(data);
		btHighlightSetModesStub(data, HIGHLIGHT_SELECTED);
	}

	tgt = PLACE_PARTY;
	commandsOut = NULL;
	choice = 0;
	commandKind = 0;
	parameter = 0;
	largeIconSlot = syVRAMCacheAllocEntry(COMMAND_ICON_BYTES);
	smallIconSlot = syVRAMCacheAllocEntry(512);
	state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_BATTLE_MENU, sizeof(BATTLE_MENU_STATE));
	forcedCnt = 0;
	mnCommandIconReloadSlot(largeIconSlot, 0);
	mnGraphicsExpandVRAMTilesByColTable(
		(Uint16 *)(BG_CHAR_BLOCK(0) + PAGE_DIGITS_TILE * TILE_SIZE_4BPP));
	mnPageArrowsDraw(LIST_PAGE_ARROW_VRAM);
	mnRangeIconBuildTiles();
	state->swappedUnit = -1;
	state->lastTgt = -1;
	{
		BATTLE_STATE *battle = Ram_HeapSlots->battleState;
		Sint32		  k;

		for(k = 0; k < 8; k++)
			state->chosenDjinn[k] = BATTLE_MENU_NO_DJINNI;
		for(k = 0; k < 3; k++)
			state->iconShown[k] = 0;
		state->tgtViewOfs = 0;
		state->viewOfs = 0;
		state->commandSelection[0] = 0;
		state->commandSelection[1] = 0;
		state->linkTmr = 0;
		state->tmrHidden = 0;
		state->tmr = -1;
		state->tmrWin = 0;
		if(battle->isLink != 0) {
			Sint32	missedLinkTicks;
			Uint16 *signature;

			state->linkTmr = 1;
			signature = (Uint16 *)SerialTransfer.payload;
			signature[4] = 'V';
			signature[5] = 'S';
			signature[6] = 'S';
			signature[7] = 'T';
			missedLinkTicks = 0;
			for(;;) {
				Uint16 *peerSignature =
					LinkPeerSignatures[battle->linkSide ^ 1];

				if(battle->linkPaused != 0) {
					commandCnt = -1;
					state->linkTmr = 0;
					break;
				}
				if((LinkStatusWork & LINK_STATUS_HEARD_BOTH) !=
					LINK_STATUS_HEARD_BOTH) {
					if(++missedLinkTicks > LINK_MAX_MISSED_TICKS) {
						commandCnt = -1;
						state->linkTmr = 0;
						break;
					}
				} else {
					missedLinkTicks = 0;
					if(peerSignature[4] == 'V' && peerSignature[5] == 'S' &&
						peerSignature[6] == 'S' && peerSignature[7] == 'T')
						break;
					if(peerSignature[4] == 'E' && peerSignature[5] == 'D' &&
						peerSignature[6] == 'V' && peerSignature[7] == 'S')
						break;
					if(!(peerSignature[0] == 'E' && peerSignature[1] == 'X' &&
						   peerSignature[2] == 'E' &&
						   peerSignature[3] == 'C')) {
						commandCnt = -1;
						state->linkTmr = 0;
						break;
					}
				}
				sySynch(1);
			}
		}
	}
	sySetEvent(mnBattleMenuUpdateEachTick, EVENT_PASS_TICK);

	for(;;) {
		commandsOut = commands;
		if(commandCnt >= 0) {
			forcedCnt = 0;
			commandCnt =
				mnBattlePartySplitChoosers(actors, &commandsOut, &forcedCnt);
		}
		btDrawPartyPanelsWithEmptyList();
		state->iconShown[2] = 0;
		state->commandActor = -1;
		state->tutorialStep = 0;
		if(syGameFlagGet(FLAG_DJINN_LESSON)) {
			state->tutorialStep = 1;
			state->tutorialDelay = 60;
		}
		if(commandCnt >= 0)
			choice = mnBattleCommandRowRun(COMMAND_ROW_PARTY, commandCnt);
		else
			choice = COMMAND_ICON_FIGHT;

		/* B on the party's row: undo the last reserve swap. */
		if(choice == -1) {
			Sint32		  swapped = state->swappedUnit;
			BATTLE_STATE *battle;
			Sint32		  old;
			Sint32		  shortSwapped;
			Sint32		  k;

			if(swapped == -1)
				continue;
			battle = Ram_HeapSlots->battleState;
			old = battle->partyUnits[state->swappedPlace];
			battle->partyUnits[state->swappedPlace] = swapped;
			shortSwapped = (Sint16)swapped;
			for(k = 0; k < 8; k++) {
				if(MainRoutineStateWork.party[k] == shortSwapped)
					MainRoutineStateWork.party[k] = old;
				else if(MainRoutineStateWork.party[k] == old)
					MainRoutineStateWork.party[k] = shortSwapped;
			}
			mnPartyStatDraw(Ram_HeapSlots->battleState->partyStat);
			btObjectPutStub();
			state->swappedUnit = -1;
			btSummonUpdateAvailabilityStub();
			BattleMenuStateWork->commandSelection[0] = 1;
			continue;
		}
		/* Switch: choose a party place, then the reserve member for it. */
		if(choice == COMMAND_ICON_SWITCH) {
			sySndPlayStub(SE_MENU_CONFIRM);
		choose_place:
			choice = mnBattleTargetRunSelection(
				0, ABILITY_TARGET_ALLIES, 1, TARGET_HINT_LAST_TARGET);
			if(choice == -1)
				continue;
			choice = mnBattlePartyChooseReserve(choice);
			if(choice == -1)
				goto choose_place;
			btSummonUpdateAvailabilityStub();
			BattleMenuStateWork->commandSelection[0] = 0;
			continue;
		}
		/* Status: the summary screens of the party and the reserve, or in the
		   debug build with Start held the enemies'. */
		if(choice == COMMAND_ICON_STATUS) {
			Uint16 *block = syHeapAlloc(18);
			Sint32	side;
			Sint32	cnt;

			if(DebugMode != 0 && (PerWork.data & KEY_START))
				side = BATTLE_SIDE_MASK_ENEMIES;
			else
				side = BATTLE_SIDE_MASK_PARTY;
			cnt = btUnitListIdsStub(side, block);
			if(side == BATTLE_SIDE_MASK_PARTY)
				cnt += btPartyPrepareReserveOwnersStub(block + cnt);
			sySynch(1);
			mnBattleStatScreenRun(block, cnt, block[0]);
			syHeapRelease(block);
			continue;
		}
		/* Flee: one COMMAND_FLEE for the party, first in the turn. */
		if(choice == COMMAND_ICON_FLEE) {
			if(mnBattleRefuseFlee() != 0)
				continue;
			commandCnt = 1;
			command = commands;
			command->unitId = actors[0];
			forcedCnt = 0;
			command->pri = FLEE_PRIORITY;
			command->command = COMMAND_FLEE;
			command->prm = 0;
			command->tgt = PLACE_PARTY;
			break;
		}
		if(choice != COMMAND_ICON_FIGHT)
			break;
		sySndPlayStub(SE_CAST);
		commandIdx = 0;
		if(commandIdx >= commandCnt)
			break;

		for(;;) {
			if(commandIdx == 0) {
				btDjinnCountStandbyStub(0, state->standbyDjinn[0]);
			} else {
				Sint32 k;

				for(k = 0; k < 4; k++)
					state->standbyDjinn[commandIdx][k] =
						state->standbyDjinn[commandIdx - 1][k];
			}
			command = &commandsOut[commandIdx];
			actorId = actors[commandIdx];
			/* UB: syUnitGet returns a pointer, not the cast's doubleword
			   result. */
#if FIX_FUNCTION_SIGNATURES
			unit = syUnitGet(actorId);
#else
			unit = (UNIT *)(Sint32)((Sint64 (*)(Sint32))syUnitGet)(actorId);
#endif
			{
				MENU_SPR *iconSpr;

				BattleMenuStateWork->commandActor = actorId;
				BattleMenuStateWork->commandSelection[1] = 0;
				iconSpr = &BattleMenuStateWork->icons[2];
				MENU_SPR_INIT(*iconSpr,
					OBJ_SIZE_32X32 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
				iconSpr->oam.f.tile =
					mnPortraitLoad(unit->enemyId, smallIconSlot);
				iconSpr->oam.f.x = BATTLE_PORTRAIT_X;
				iconSpr->oam.f.y = 128;
				iconSpr->oam.f.pal = PORTRAIT_PALETTE;
				state->iconShown[2] = 1;
			}

		mark_visible:
			mnWindowMarkVisibleTextTiles();
			BattleMenuStateWork->iconShown[0] = 0;
			BattleMenuStateWork->chosenDjinn[commandIdx] =
				BATTLE_MENU_NO_DJINNI;
			highlighted[0] = actorId;
			highlighted[1] = BATTLE_UNIT_LIST_END;
			btHighlightSetModesStub(highlighted, HIGHLIGHT_SELECTED);
			mnPartyStatHighlightMembers(highlighted);
			sySynch(1);
			choice = mnBattleCommandRowRun(COMMAND_ROW_MEMBER, 0);
			sySynch(1);
			if(choice == -2) {
				Uint16 *block = syHeapAlloc(12);
				Sint32	cnt = btUnitListIdsStub(BATTLE_SIDE_MASK_PARTY, block);

				state->iconShown[2] = 0;
				sySynch(1);
				mnBattleStatScreenRun(block, cnt, actorId);
				state->iconShown[2] = 1;
				syHeapRelease(block);
				continue;
			}
			btHighlightSetModesStub(highlighted, HIGHLIGHT_OFF);
			if(choice == -1) {
				if(commandIdx == 0)
					break;
				commandIdx--;
				continue;
			}
			{
				MENU_SPR *iconSpr = &BattleMenuStateWork->icons[0];

				if(BattleMenuStateWork->tmr == 0)
					choice = COMMAND_ICON_DEFEND;
				iconSpr->oam.word.attr23 = 0;
				iconSpr->oam.word.attr01 = OBJ_SIZE_32X32 | OBJ_256_COLORS |
					OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
				iconSpr->oam.f.tile =
					mnCommandIconReloadSlot(largeIconSlot, choice);
				if(choice == COMMAND_ICON_DJINN)
					iconSpr->oam.f.x = 128;
				else
					iconSpr->oam.f.x = 96;
				iconSpr->oam.f.y = 136;
				state->iconShown[0] = 1;
			}

			switch(choice) {
			case COMMAND_ICON_ATTACK:
				commandWin = mnWindowCreate(
					11, 17, 11, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
				mnTextDrawMessageAsTiles((Sint32)MsgCommandName, commandWin,
					BATTLE_COMMAND_NAME_X, 0);
				COMMAND_ICON_X(state) = ATTACK_ICON_X;
				sySndPlayStub(SE_MENU_CONFIRM);
				choice = mnBattleTargetRunSelection(
					actorId, ABILITY_TARGET_FOES, 1, TARGET_HINT_NONE);
				mnWindowClose(commandWin, 1);
				if(choice == -1)
					goto mark_visible;
				commandKind = COMMAND_ATTACK;
				tgt = choice;
				command->range = 1;
				break;

			case COMMAND_ICON_PSYNERGY: {
				Sint32				k;
				MENU_SPR		   *iconSpr;
				ABILITY_DEFINITION *ability;
				WIN_STATE		   *winState;
				Uint16			   *list;
				Uint8			   *pick;
				Sint32				cost;
				Sint32				handle;
				Sint32				cnt;
				Sint32				entryValue;

				sySndPlayStub(SE_MENU_CONFIRM);
				/* DEAD: choice cleared before the list sets it; the ROM keeps
				   its mov. */
				choice = 0;
				BATTLE_MENU_SET_CURSOR(0, 0, 0);
			psynergy_menu:
				list = state->listAbilities;
				pick = state->listSlots;
				COMMAND_ICON_X(state) = PSYNERGY_MENU_ICON_X;
				cnt = 0;
				for(k = 0; k != PSYNERGY_SLOTS &&
					(entryValue = unit->psynergy[k].encodedAction &
							PSYNERGY_ABILITY_MASK) != 0;
					k++) {
					if(gmAbilityGetDirectStub(entryValue)->flg &
						ABILITY_USABLE_IN_BATTLE) {
						pick[cnt] = k;
						list[cnt] = entryValue;
						cnt++;
					}
				}
				pick[cnt] = 0;
				list[cnt] = 0;
				choice = mnBattlePsynergyMenuRun(actorId, list, cnt);
				if(choice == -1)
					goto mark_visible;
				parameter = unit->psynergy[pick[choice]].encodedAction &
					PSYNERGY_ABILITY_MASK;
				ability = gmAbilityGetDirectStub(parameter);
				cost = ability->range;
				iconSpr = &state->icons[1];
				handle = syVRAMCacheAllocEntry(128);
				winState = WINDOW_STATE;
				commandWin = mnWindowCreate(
					8, 17, 18, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
				COMMAND_ICON_X(state) = PSYNERGY_CHOSEN_ICON_X;
				MENU_SPR_INIT(*iconSpr,
					OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
				iconSpr->oam.f.x = commandWin->x * 8 + 8;
				iconSpr->oam.f.y = commandWin->y * 8 + 4;
				iconSpr->oam.f.tile =
					mnAbilityIconLoadIntoSlot(parameter, handle);
				state->iconShown[1] = 1;
				winState->textPal = BASIC_COLORS_PALETTE;
				if(ability->ppCost > unit->pp)
					mnTextSetCol(TEXT_COLOR_RED);
				else if(unit->psySeal != 0)
					mnTextSetCol(TEXT_COLOR_PINK);
				mnTextDrawMessageAsTiles(
					parameter + (Sint32)MsgAbilityName, commandWin, 16, 0);
				mnTextDrawNumAtOffset(ability->ppCost, 2, commandWin, 104, 0);
				winState->textPal = WINDOW_PALETTE;
				mnTextSetCol(TEXT_COLOR_WHITE);
				mnWindowSetTilemapEntry(
					commandWin, WINDOW_TILE_PP_LABEL, 11, 0, 0);
				mnWindowSetTilemapEntry(
					commandWin, WINDOW_TILE_PP_LABEL_END, 12, 0, 0);
				if(ability->element != ELEMENT_NONE)
					mnWindowSetTilemapEntry(commandWin,
						ability->element + ELEMENT_ICON_TILE, 15, 0, 0);
				command->range = cost;
				sySndPlayStub(SE_MENU_CONFIRM);
				choice = mnBattleTargetRunSelection(actorId, ability->tgtMode,
					cost, mnBattleClassifyEntryKind(ability));
				state->iconShown[1] = 0;
				syVRAMCacheFreeEntry(handle);
				mnWindowClose(commandWin, 1);
				if(choice == -1)
					goto psynergy_menu;
				commandKind = COMMAND_PSYNERGY;
				tgt = choice;
				break;
			}

			case COMMAND_ICON_SUMMON: {
				const SUMMON_DEFINITION *summon;
				ABILITY_DEFINITION		*ability;
				MENU_SPR				*iconSpr;
				Sint32					 ok;
				Sint32					 j;
				Sint32					 n;

				sySndPlayStub(SE_MENU_CONFIRM);
				BATTLE_MENU_SET_CURSOR(0, 0, 0);
			summon_menu:
				COMMAND_ICON_X(state) = BATTLE_SUMMON_ICON_X;
				mnPartyStatHighlightMembers(highlighted);
				choice = mnBattleSummonMenuRun(
					0, 0, state->standbyDjinn[commandIdx]);
				mnPartyStatHighlightMembers(highlighted);
				if(choice == -1)
					goto mark_visible;
				commandKind = COMMAND_SUMMON;
				parameter = choice;
				sySynch(1);
				iconSpr = &state->icons[1];
				summon = gmSummonDefinitionGetNearStub(choice);
				ability = gmAbilityGetDirectStub(summon->abilityId);
				summonIconSlot = syVRAMCacheAllocEntry(128);
				commandWin = mnWindowCreate(SUMMON_WINDOW_X, 17,
					BATTLE_SUMMON_WINDOW_WIDTH, 3,
					WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
				n = 0;
				for(j = 0; j <= 3; j++) {
					if(summon->djinnRequired[j] >
						state->standbyDjinn[commandIdx][j])
						break;
				}
				ok = j == 4;
				COMMAND_ICON_X(state) = SUMMON_CHOSEN_ICON_X;
				((Uint32 *)&iconSpr->oam)[0] =
					OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
				((Uint32 *)&iconSpr->oam)[1] = 0;
				ASM_NOTHING();
				iconSpr->oam.f.x = commandWin->x * 8 + 8;
				iconSpr->oam.f.y = commandWin->y * 8 + 4;
				iconSpr->oam.f.tile = mnAbilityIconLoadIntoSlot(
					summon->abilityId & ACTION_ID_MASK, summonIconSlot);
				state->iconShown[1] = 1;
				if(ok == 0)
					mnTextSetCol(TEXT_COLOR_RED);
				mnTextDrawMessageAsTiles(
					gmSummonDefinitionGetNearStub(choice)->abilityId +
						(Sint32)MsgAbilityName,
					commandWin, 16, 0);
				for(j = 0; j <= 3; j++) {
					if(summon->djinnRequired[j] != 0) {
						mnWindowSetTilemapEntry(commandWin,
							j + ELEMENT_ICON_TILE,
							n * 2 + BATTLE_SUMMON_ELEMENT_COLUMN, 0, 0);
						mnWindowPutGlyph(commandWin,
							summon->djinnRequired[j] + '0',
							n * 2 + BATTLE_SUMMON_COST_COLUMN, 0,
							GLYPH_DRAW_CHARACTER);
						n++;
					}
				}
				if(ok != 0)
					sySndPlayStub(SE_MENU_CONFIRM);
				else
					sySndPlayStub(SE_MENU_ERROR);
				choice = mnBattleTargetRunSelection(actorId, ability->tgtMode,
					ability->range, mnBattleClassifyEntryKind(ability));
				command->range = ability->range;
				state->iconShown[1] = 0;
				syVRAMCacheFreeEntry(summonIconSlot);
				mnWindowClose(commandWin, 1);
				if(choice == -1)
					goto summon_menu;
				for(j = 0; j <= 3; j++) {
					if(summon->djinnRequired[j] >
						state->standbyDjinn[commandIdx][j]) {
						state->standbyDjinn[commandIdx][j] = 0;
						break;
					}
					state->standbyDjinn[commandIdx][j] -=
						summon->djinnRequired[j];
				}
				tgt = choice;
				break;
			}

			case COMMAND_ICON_DJINN: {
				ABILITY_DEFINITION *ability;
				Sint32				cost;
				Sint32				element;
				Sint32				idx;

				sySndPlayStub(SE_MENU_CONFIRM);
				BATTLE_MENU_SET_CURSOR(0, 0, 0);
			djinn_menu:
				COMMAND_ICON_X(state) = DJINN_MENU_ICON_X;
				BattleMenuStateWork->chosenDjinn[commandIdx] =
					BATTLE_MENU_NO_DJINNI;
				choice = mnBattleDjinnMenuRun(actorId, 1);
				command->range = 0;
				if(choice == -1)
					goto mark_visible;
				parameter = choice;
				commandKind = COMMAND_DJINN;
				BattleMenuStateWork->chosenDjinn[commandIdx] = choice;
				element = DJINN_COMMAND_ELEMENT(parameter);
				idx = DJINN_COMMAND_INDEX(parameter);
				if(gmDjinnIsSetStub(actorId, element, idx) != 0) {
					ability = gmAbilityGetDirectStub(
						gmDjinnGetAbilityIdStub(element, idx));
					cost = ability->range;
					commandWin = mnWindowCreate(11, 17, 10, 3,
						WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
					COMMAND_ICON_X(state) = DJINN_CHOSEN_ICON_X;
					mnWindowSetTilemapEntry(
						commandWin, element + ELEMENT_ICON_TILE, 0, 0, 0);
					mnTextDrawMessageAsTiles(element * DJINN_PER_ELEMENT + idx +
							(Sint32)MsgDjinnName,
						commandWin, 16, 0);
					command->range = cost;
					sySynch(1);
					sySndPlayStub(SE_MENU_CONFIRM);
					choice =
						mnBattleTargetRunSelection(actorId, ability->tgtMode,
							cost, mnBattleClassifyEntryKind(ability));
					if(state->tutorialStep != 0) {
						void *hint = mnMessageShowAndWait(
							mnDjinnGetGiveTutorialMessage() +
								DJINN_TUTORIAL_LINE_UNLEASH_HINT,
							15, 8);

						while(mnMessageAllTyped() == 0)
							sySynch(1);
						mnWindowClose(hint, 1);
						state->tutorialStep++;
						state->tutorialDelay = 45;
					}
					mnWindowClose(commandWin, 1);
					if(choice == -1)
						goto djinn_menu;
					tgt = choice;
					break;
				}
				commandWin = mnWindowCreate(
					11, 17, 10, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
				COMMAND_ICON_X(state) = DJINN_CHOSEN_ICON_X;
				mnTextSetCol(TEXT_COLOR_RED);
				mnWindowSetTilemapEntry(
					commandWin, element + ELEMENT_ICON_TILE, 0, 0, 0);
				mnTextDrawMessageAsTiles(
					element * DJINN_PER_ELEMENT + idx + (Sint32)MsgDjinnName,
					commandWin, 16, 0);
				mnTextSetCol(TEXT_COLOR_WHITE);
				command->range = 1;
				sySynch(1);
				sySndPlayStub(SE_MENU_CONFIRM);
				choice = mnBattleTargetRunSelection(
					actorId, ABILITY_TARGET_SELF, 0, TARGET_HINT_SET_DJINNI);
				mnWindowClose(commandWin, 1);
				if(choice == -1)
					goto djinn_menu;
				tgt = choice;
				break;
			}

			case COMMAND_ICON_ITEM: {
				Sint32				k;
				MENU_SPR		   *iconSpr;
				ABILITY_DEFINITION *ability;
				Uint16			   *list;
				Uint8			   *pick;
				Sint32				cost;
				Sint32				handle;
				Sint32				cnt;
				Sint32				entryValue;
				Sint32				selection;

				sySndPlayStub(SE_MENU_CONFIRM);
				/* DEAD: choice cleared before the list sets it; the ROM keeps
				   its mov. */
				choice = 0;
				BATTLE_MENU_SET_CURSOR(0, 0, 0);
			item_menu:
				COMMAND_ICON_X(state) = ITEM_MENU_ICON_X;
				pick = state->listSlots;
				list = state->listAbilities;
				cnt = 0;
				for(k = 0; k != INVENTORY_SLOTS &&
					(entryValue = unit->inventory[k]) != 0;
					k++) {
					gmItemGetDirectStub(entryValue);
					if(mnItemCheckBattleUse(actorId, entryValue) == 0) {
						list[cnt] = entryValue;
						pick[cnt++] = k;
					}
				}
				for(k = 0; k != INVENTORY_SLOTS &&
					(entryValue = unit->inventory[k]) != 0;
					k++) {
					gmItemGetDirectStub(entryValue);
					if(mnItemCheckBattleUse(actorId, entryValue) != 0) {
						list[cnt] = entryValue;
						pick[cnt++] = k;
					}
				}
				list[cnt] = 0;
				choice = mnBattleItemMenuRun(actorId, list, cnt);
				if(choice == -1)
					goto mark_visible;
				selection = pick[choice];
				parameter = selection;
				ability = gmAbilityGetDirectStub(
					gmItemGetDirectStub(unit->inventory[selection])->abilityId);
				cost = ability->range;
				iconSpr = &state->icons[1];
				handle = syVRAMCacheAllocEntry(128);
				commandWin = mnWindowCreate(9, 17, BATTLE_ITEM_WINDOW_WIDTH, 3,
					WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
				COMMAND_ICON_X(state) = ITEM_CHOSEN_ICON_X;
				MENU_SPR_INIT(*iconSpr,
					OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
				iconSpr->oam.f.x = commandWin->x * 8 + 8;
				iconSpr->oam.f.y = commandWin->y * 8 + 4;
				iconSpr->oam.f.tile =
					mnItemIconLoadIntoSlot(unit->inventory[selection], handle);
				state->iconShown[1] = 1;
				mnTextDrawMessageAsTiles(
					(unit->inventory[selection] & INVENTORY_ITEM_ID_MASK) +
						(Sint32)MsgItemName,
					commandWin, 24, 0);
				command->range = cost;
				sySndPlayStub(SE_MENU_CONFIRM);
				choice = mnBattleTargetRunSelection(actorId, ability->tgtMode,
					cost, mnBattleClassifyEntryKind(ability));
				state->iconShown[1] = 0;
				syVRAMCacheFreeEntry(handle);
				mnWindowClose(commandWin, 1);
				if(choice == -1)
					goto item_menu;
				commandKind = COMMAND_ITEM;
				tgt = choice;
				break;
			}

			case COMMAND_ICON_DEFEND:
				commandKind = COMMAND_DEFEND;
				break;
			}

			sySndPlayStub(SE_CHIME);
			command->unitId = actorId;
			{
				Sint32 pri = unit->agility;

				if(pri != 0)
					pri += (Uint32)(unit->agility * syRandom()) >> 20;
				command->pri = pri;
				if(commandIdx != 0 &&
					actors[commandIdx] == actors[commandIdx - 1])
					command->pri = (Sint16)pri / 2;
			}
			/* A priority past 32767 reads negative. */
			if(command->pri < 0)
				command->pri = OVERFLOWED_PRIORITY;
			command->command = commandKind;
			command->prm = parameter;
			command->tgt = tgt;
			commandIdx++;
			if(commandIdx < commandCnt)
				continue;
			break;
		}
		if(commandIdx < commandCnt)
			continue;
		break;
	}

	if(state->linkTmr != 0) {
		Uint16 *signature = (Uint16 *)SerialTransfer.payload;

		signature[4] = 'E';
		signature[5] = 'D';
		signature[6] = 'V';
		signature[7] = 'S';
	}
	if(state->tmrWin != NULL)
		mnWindowClose(state->tmrWin, 1);
	syVRAMCacheFreeEntry(smallIconSlot);
	syVRAMCacheFreeEntry(largeIconSlot);
	syCloseEvent(mnBattleMenuUpdateEachTick);
	if(state->linkTmr != 0) {
		BATTLE_STATE *link;

		void mnLinkDrawWaitingForOpponent(void)
		{
			Uint16 text[64];

			BATTLE_MENU_MESSAGE_COPY((Sint32)MsgWaitingForOpponent, text,
				BATTLE_MENU_MESSAGE_CAPACITY);
			mnTextDrawWideStringAtOffset(text, state->tmrWin, 0, 4);
		}

		/* The wait message shows only when the countdown never opened its
		   window, that is while the partner is still choosing; a timer window
		   was closed above and is only forgotten here. choice counts the ticks
		   without the link, and past LINK_MAX_MISSED_TICKS gives up (-1). */
		link = Ram_HeapSlots->battleState;
		choice = 0;
		if(state->tmrWin == NULL && link->linkPaused == 0) {
			state->tmrWin = mnWindowCreate(0, 16, 30, 4,
				WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
					WINDOW_FLAG_BORDERED_CANVAS);
			mnTextFillCanvasBackground();
			mnLinkDrawWaitingForOpponent();
		} else {
			state->tmrWin = NULL;
		}
		for(;;) {
			Uint16 *peerSignature = LinkPeerSignatures[link->linkSide ^ 1];

			if(link->linkPaused != 0) {
				commandCnt = -1;
				break;
			}
			if((LinkStatusWork & LINK_STATUS_HEARD_BOTH) !=
				LINK_STATUS_HEARD_BOTH) {
				choice++;
				if(choice > LINK_MAX_MISSED_TICKS) {
					commandCnt = -1;
					break;
				}
			} else {
				choice = 0;
				if(peerSignature[4] == 'E' && peerSignature[5] == 'D' &&
					peerSignature[6] == 'V' && peerSignature[7] == 'S')
					break;
				if(!(peerSignature[4] == 'V' && peerSignature[5] == 'S' &&
					   peerSignature[6] == 'S' && peerSignature[7] == 'T'))
					choice = 1;
			}
			sySynch(1);
		}
		if(state->tmrWin != NULL)
			mnWindowClose(state->tmrWin, 1);
	}
	btCameraSetCommandViewStub(0);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_MENU);
	if(commandCnt >= 0)
		commandCnt += forcedCnt;
	return commandCnt;
}

/* The icon menus' bottom row, in tiles, where most of them stand; a Yes or No
   may stand at the top, row 0, instead. */
#define ICON_MENU_BOTTOM_ROW 17
/* The lines an icon row covers: its label window's three tiles. */
#define ICON_ROW_LINES		 24

/* How far the selected icon overhangs its place while it pulses at the 8.8
   `scale`: on each side, half its 24-pixel width grown by the scale, rounded
   up; and past its row, half its 32-pixel height grown by the scale, rounded,
   and a line more. */
#define PULSE_OVERHANG_X(scale) ((((scale) - Q_8_8(1)) * 12 + 255) / 256)
#define PULSE_OVERHANG_LINES(scale) \
	((((scale) - Q_8_8(1)) * 32 + 255) / 512 + 1)

/* Draws the icon menu's icons, an event of the icon menus (mnIconMenuOpen).
   Each icon is a 32x32, 256-colour sprite, and one at x 0 is not drawn; the
   selected one pulses, double-size, between 1.19 and 1.31 times its size, in
   front of the others. While a screen transition runs, it widens window 1 over
   the icon row so the icons show through: from the first icon on the row's
   lines, keeping each line's right edge, and around the selected icon on the
   lines it overhangs, below a row at the top or above one at the bottom. */
void mnIconMenuDrawIcons(void)
{
	ICON_MENU_STATE *iconMenuState;
	ICON_MENU_ENTRY *entry;
	AFFINE_TRANSFORM transform;
	Sint32			 pulseScale;
	Sint32			 affineIdx;
	Sint32			 span;
	Sint32			 sprY;
	Uint32			 lastLine;
	Uint32			 idx;
	/* UB: edge steps from one line's window 1 edges to the next line's, past
	   the member it points into. */
	Uint16					*edge;
	SCREEN_TRANSITION_STATE *state;
	WINDOW_BUFFER			*page;
	Sint32					 halfWidth;

	iconMenuState = Ram_HeapSlots->iconMenu;
	entry = iconMenuState->entries;
	pulseScale = WindowSpriteBounceScales[(TickCount * 2) & 31];
	state = Ram_HeapSlots->screenTransition;
	pulseScale = (pulseScale - Q_8_8(1)) / 4 + Q_8_8(1.1875);
	transform.scaleX = pulseScale;
	transform.scaleY = pulseScale;
	transform.ang = 0;
	affineIdx = syOAMMatrixAllocFromTransform(&transform);
	/* Pulse only the selection; the other icons keep their normal OAM shape. */
	for(idx = 0; idx < iconMenuState->cnt; idx++, entry++) {
		Uint32 *word;
		Sint32	drawLayer;
		Sint32	x;

		x = entry->x;
		if(x == 0)
			continue;
		if(idx == iconMenuState->selection) {
			Sint32 left = x + pulseScale * 7 / 512 - 20;

			/* The icon grows about the middle of its 64-pixel double-size box.
			   On the top row the box starts above the screen, its y wrapped
			   within OAM's 8 bits, and the icon's top edge stays at the
			   screen's top. */
			if(entry->y != 0)
				sprY = entry->y + pulseScale * 3 / 256 - 20;
			else
				sprY = (pulseScale * 15 / 256 - 30) & OBJ_Y_MASK;
			word = &entry->link;
			*word++ = 0;
			*word++ = OBJ_AFFINE_MATRIX(affineIdx) | sprY | OBJ_X(left) |
				OBJ_SIZE_32X32 | OBJ_256_COLORS | OBJ_DOUBLE_SIZE | OBJ_AFFINE;
			*word = VramCacheEntries[entry->slot].ofs >> 5;
			drawLayer = OAM_DEPTH_WINDOW + 6;
		} else {
			Uint32 tileOfs;
			Uint32 attributes;

			word = &entry->link;
			sprY = entry->y;
			*word++ = 0;
			attributes = sprY | OBJ_X(x) | OBJ_SIZE_32X32 | OBJ_256_COLORS;
			tileOfs = VramCacheEntries[entry->slot].ofs;
			*word++ = attributes;
			*word = tileOfs >> 5;
			drawLayer = OAM_DEPTH_WINDOW + 5;
		}
		syOAMAddSprAtDepth(entry, drawLayer);
	}
	if(state == NULL || state->centreLine != 0 || iconMenuState->cnt == 0)
		return;
	page = (WINDOW_BUFFER *)state->pages[state->page];
	if(iconMenuState->row == 0) {
		span = iconMenuState->entries[iconMenuState->selection].x;
		halfWidth = PULSE_OVERHANG_X(pulseScale);
		span = ((span - halfWidth) << 8) + (span + halfWidth) + 23;
		lastLine =
			iconMenuState->entries[iconMenuState->selection].y + ICON_ROW_LINES;
		lastLine += PULSE_OVERHANG_LINES(pulseScale);
		edge = &page->lines[ICON_ROW_LINES].win1;
		idx = ICON_ROW_LINES;
		if(idx < lastLine) {
			do {
				*edge = (*edge & WIN_RANGE_END_MASK) | span;
				idx++;
				edge += 2;
			} while(idx < lastLine);
		}
		span = iconMenuState->entries[0].x;
		if(iconMenuState->selection == 0)
			span -= PULSE_OVERHANG_X(pulseScale);
		span <<= 8;
		edge = &page->lines[0].win1;
		for(idx = 0; idx < ICON_ROW_LINES; idx++) {
			*edge = (*edge & WIN_RANGE_END_MASK) | span;
			edge += 2;
		}
	} else {
		Sint32 firstLine;

		span = iconMenuState->entries[iconMenuState->selection].x;
		halfWidth = PULSE_OVERHANG_X(pulseScale);
		span = ((span - halfWidth) << 8) + (span + halfWidth) + 23;
		firstLine = iconMenuState->entries[iconMenuState->selection].y;
		firstLine -= PULSE_OVERHANG_LINES(pulseScale);
		edge = &page->lines[firstLine].win1;
		idx = firstLine;
		if(idx < ICON_MENU_BOTTOM_ROW * 8) {
			do {
				*edge = (*edge & WIN_RANGE_END_MASK) | span;
				idx++;
				edge += 2;
			} while(idx < ICON_MENU_BOTTOM_ROW * 8);
		}
		span = iconMenuState->entries[0].x;
		if(iconMenuState->selection == 0)
			span -= PULSE_OVERHANG_X(pulseScale);
		span <<= 8;
		edge = &page->lines[ICON_MENU_BOTTOM_ROW * 8].win1;
		for(idx = ICON_MENU_BOTTOM_ROW * 8; idx < DISPLAY_HEIGHT; idx++) {
			/* firstLine, done with, carries the mask that keeps the right
			   edge. */
			firstLine = WIN_RANGE_END_MASK;
			*edge = (*edge & firstLine) | span;
			edge += 2;
		}
	}
}

#undef SUMMON_WINDOW_X
#undef BATTLE_SUMMON_WINDOW_WIDTH
#undef BATTLE_SUMMON_ELEMENT_COLUMN
#undef BATTLE_SUMMON_COST_COLUMN
#undef BATTLE_ITEM_WINDOW_WIDTH
#undef BATTLE_PORTRAIT_X
#undef BATTLE_SUMMON_ICON_X
#undef SUMMON_CHOSEN_ICON_X
#undef DJINN_MENU_ICON_X
#undef ATTACK_ICON_X
#undef PSYNERGY_CHOSEN_ICON_X
#undef DJINN_CHOSEN_ICON_X
#undef ITEM_MENU_ICON_X
#undef ITEM_CHOSEN_ICON_X
#undef FLEE_PRIORITY
#undef OVERFLOWED_PRIORITY
#undef BATTLE_COMMAND_NAME_X
#undef PSYNERGY_MENU_ICON_X
#undef COMMAND_ICON_X

/* Allocates and clears the icon menu's state, which lives in
   HEAP_SLOT_ICON_MENU until mnIconMenuClose frees it, and starts the event that
   draws its icons. Every icon menu starts here. */
void *mnIconMenuOpen(void)
{
	void *state =
		syHeapSlotAllocEWRAM(HEAP_SLOT_ICON_MENU, sizeof(ICON_MENU_STATE));

	DMA_FILL32(0, state, sizeof(ICON_MENU_STATE));
	sySetEvent((EVENT_FUNC)mnIconMenuDrawIcons, EVENT_PASS_TICK - 10);
	return state;
}

/* Stops drawing the icons, closes the label window and releases each loaded
   icon before freeing the icon menu's state (HEAP_SLOT_ICON_MENU), then waits a
   tick. */
void mnIconMenuClose(void)
{
	WINDOW			*labelWin;
	Sint32			 i;
	ICON_MENU_STATE *state;

	state = Ram_HeapSlots->iconMenu;
	syCloseEvent(mnIconMenuDrawIcons);
	labelWin = state->win;
	if(labelWin != 0) {
		mnWindowClose(labelWin, 2);
	}
	for(i = 0; i < state->cnt; i++) {
		syVRAMCacheFreeEntry(state->entries[i].slot);
	}
	syHeapSlotRelease(HEAP_SLOT_ICON_MENU);
	sySynch(1U);
}

/* Selects an icon position with wrapped directional input. A returns the
   position; B or Start returns -1. Redraws the label only after a move: the
   message labelBase + position when the layout set a labelBase, else the icon's
   command name. */
Sint32 mnIconMenuRun(Sint32 init)
{
	ICON_MENU_STATE *state = Ram_HeapSlots->iconMenu;
	Sint32			 resource;

	state->selection = init;
redraw:
	mnWindowSprPrepareForRedraw(state->win);
	if(state->labelBase != 0)
		resource = state->labelBase + state->selection;
	else
		resource =
			state->commandIcons[state->selection] + (Sint32)MsgCommandName;
	mnTextDrawMessageAsTiles(resource, state->win, 0, 0);
	for(;;) {
		sySynch(1);
		if(PerWork.push & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			return state->selection;
		}
		if(PerWork.push & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			return -1;
		}
		if(PerWork.push & KEY_START) {
			sySndPlayStub(SE_MENU_CANCEL);
			return -1;
		}
		if((PerWork.repeat & KEY_LEFT) || (PerWork.repeat & KEY_UP)) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			state->selection--;
			if(state->selection < 0)
				state->selection = state->cnt - 1;
		} else if((PerWork.repeat & KEY_RIGHT) || (PerWork.repeat & KEY_DOWN)) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			state->selection++;
			if(state->selection >= state->cnt)
				state->selection = 0;
		} else
			continue;
		goto redraw;
	}
}

/* Returns the distance between lhs and rhs, reusing their supplied
   difference. */
static inline Sint32 mnAbsDiff(Sint32 diff, Sint32 lhs, Sint32 rhs)
{
	if(diff >= 0)
		return diff;
	return rhs - lhs;
}

/* Moves the selection from start to goal one icon at a time with the cursor
   sound, redrawing the label each step and waiting
   MenuSelectionStepDelays[icons still to go] ticks at each (12 more at the
   first), then 48 ticks on the goal before the confirm sound: a menu choice
   made for the player to watch (mnAnimateSelectionToEntry). Returns goal. */
Sint32 mnIconMenuAnimateTo(Sint32 start, Sint32 goal)
{
	Sint16			 resourceBase;
	Sint16			 selection;
	Sint32			 diff;
	Sint32			 resourceId;
	Sint32			 distance;
	Sint32			 pos;
	Sint32			 step;
	Sint32			 delay;
	const Uint8		*stepDelays;
	ICON_MENU_STATE *state;

	state = Ram_HeapSlots->iconMenu;
	step = 1;
	delay = 12;
	state->selection = (Sint16)start;
	if(goal < start)
		step = -1;
	pos = start;

	for(;;) {
		mnWindowSprPrepareForRedraw(state->win);
		resourceBase = state->labelBase;
		if(resourceBase != 0) {
			resourceId = resourceBase + state->selection;
		} else {
			resourceId =
				state->commandIcons[state->selection] + (Sint32)MsgCommandName;
		}
		mnTextDrawMessageAsTiles(resourceId, state->win, 0, 0);

		selection = state->selection;
		stepDelays = MenuSelectionStepDelays;
		diff = selection - goal;
		distance = mnAbsDiff(diff, selection, goal);
		sySynch(stepDelays[distance] + delay);

		if(pos == goal)
			break;

		state->selection += step;
		sySndPlayStub(SE_MENU_CURSOR_MOVE);
		delay = 0;
		pos += step;
	}

	sySynch(48);
	sySndPlayStub(SE_MENU_CONFIRM);
	return goal;
}

/* Decodes command icon `idx` into the VRAM cache entry `slot`, through a
   block it allocates and frees. */
void mnCommandIconLoadIntoSlot(Sint32 slot, Sint32 idx)
{
	Sint32	size = COMMAND_ICON_BYTES;
	void   *buf = syHeapAlloc(size);
	Uint16 *ofsTbl = syResourceGetTableEntry(RESOURCE_COMMAND_ICONS);

	syDecompressByteLZ((void *)((Uint32)ofsTbl + ofsTbl[idx]), buf);
	syVRAMCacheLoad(slot, size, buf);
	syHeapRelease(buf);
}

/* Appends a command icon while the menu has room for one (ICON_MENU_STATE's six
   entries). It starts on the bottom row; mnIconMenuCenter or
   mnIconMenuLayoutIcons places the row. Its VRAM cache entry is taken
   unchecked. */
void mnIconMenuAddIcon(Sint32 icon)
{
	ICON_MENU_STATE *state;
	ICON_MENU_ENTRY *entry;
	Sint16			 idx;
	Sint32			 slot;
	Sint32			 y = ICON_MENU_BOTTOM_ROW * 8;

	state = Ram_HeapSlots->iconMenu;
	idx = state->cnt;
	if(idx < (Sint32)ARRAY_COUNT(state->entries)) {
		state->cnt++;
		entry = &state->entries[idx];
		slot = syVRAMCacheFindFreeEntry();
		mnCommandIconLoadIntoSlot(slot, icon);
		entry->x = idx * 24 + 32;
		entry->y = y;
		entry->slot = slot;
		state->commandIcons[idx] = icon;
	}
}

/* Centres the icons and their label window together on tile row `row`. A
   nonzero resourceBase is the first label message; otherwise each icon is
   labelled by its command name. The icons stand three tiles apart and the label
   window, `width` + 2 tiles wide and 3 tall, follows them; the centring counts
   two thirds of the window's width. */
void mnIconMenuCenter(Sint32 row, Sint32 width, Sint32 resourceBase)
{
	ICON_MENU_STATE *state = Ram_HeapSlots->iconMenu;
	Sint32			 x;
	Sint32			 i;
	Sint32			 cnt;

	state->width = width + 2;
	state->labelBase = resourceBase;
	state->row = row;
	cnt = state->cnt;
	x = DISPLAY_TILE_WIDTH / 2 - (cnt * 3 + state->width * 2 / 3) / 2;
	for(i = 0; i < state->cnt; i++) {
		ICON_MENU_ENTRY *entry = &state->entries[i];

		entry->x = x * 8;
		entry->y = row * 8;
		x += 3;
	}
	state->win = mnWindowCreate(x, row, state->width, 3, WINDOW_FLAG_INSTANT);
}

/* Places the icons three tiles apart from tile (x, y), then opens their label
   window, `width` + 2 tiles wide, just after them. */
void mnIconMenuLayoutIcons(
	Sint32 x, Sint32 y, Sint32 width, Sint32 resourceBase)
{
	ICON_MENU_STATE *state;
	Sint32			 i;
	ICON_MENU_ENTRY *entry;

	state = Ram_HeapSlots->iconMenu;
	state->width = width + 2;
	state->labelBase = resourceBase;
	state->row = y;
	for(i = 0; i < state->cnt; i++) {
		entry = &state->entries[i];
		entry->y = y << 3;
		entry->x = x << 3;
		x += 3;
	}
	state->win = mnWindowCreate(x, y, state->width, 3, WINDOW_FLAG_INSTANT);
}

/* Reads one entry of a field menu table (MenuTopEntryPositionByCommand). */
static __inline__ Sint32 mnIconMenuReadCommandPos(Sint8 *tbl, Sint32 idx)
{
	return tbl[idx];
}

/* The field menu's row of icons: Psynergy, Djinn, Item and Status, without
   Djinn when the party has none. `selection` is the command to start on and the
   result the command chosen (FIELD_MENU_COMMAND), or -1 when cancelled; two
   tables turn commands into icon positions and back. */
Sint32 mnFieldMenuSelectCommand(Sint32 selection)
{
	Sint32 group = 0;
	Sint32 ofs;
	Sint32 groupTriple;
	Sint32 result;
	Sint8 *commandPositions;

	if(gmPartySumDjinnCountsStub(-1) == 0) {
		group = 1;
	}

	groupTriple = group * 3;
	commandPositions = MenuTopEntryPositionByCommand;
	ofs = groupTriple << 1;
	selection = mnIconMenuReadCommandPos(commandPositions, selection + ofs) - 1;
	if(selection < 0) {
		selection = 0;
	}

	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_PSYNERGY);
	if(group == 0) {
		mnIconMenuAddIcon(COMMAND_ICON_DJINN);
	}
	mnIconMenuAddIcon(COMMAND_ICON_ITEM);
	mnIconMenuAddIcon(COMMAND_ICON_STATUS);
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, FIELD_MENU_ICON_ROW_WIDTH, 0);
	result = mnIconMenuRun(selection);
	mnIconMenuClose();

	if(result >= 0) {
		Sint8 *commands = MenuTopEntryCommandByPosition;
		Sint32 idx;

		idx = result + ofs + 1;
		result = commands[idx];
	}

	return result;
}

/* Opens the field command menu and animates from start to the command's icon.
   Returns the final icon position after closing the menu. */
Sint32 mnAnimateSelectionToEntry(Sint32 start, Sint32 command)
{
	mnPartyStatOpenAndWait();
	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_PSYNERGY);
	mnIconMenuAddIcon(COMMAND_ICON_DJINN);
	mnIconMenuAddIcon(COMMAND_ICON_ITEM);
	mnIconMenuAddIcon(COMMAND_ICON_STATUS);
	/* Only Japanese centres the entries before selecting. */
#if !FIELD_MENU_DEMO_NO_LABEL_WINDOW
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, FIELD_MENU_ICON_ROW_WIDTH, 0);
#endif
	command = mnIconMenuAnimateTo(start, command - 1);
	mnIconMenuClose();
	mnPartyStatCloseWithMenu();
	return command;
}

/* The File Screen's row of icons, from the save summaries: New Game, Continue,
   Copy and Erase for one or two files (starting on Continue); no New Game or
   Copy with three; with every file cleared no Continue, and only Erase with
   three such files; then Battle and Update when they are enabled. Returns the
   action chosen (FILE_ACTION) by its position; with Battle off, the action
   after Erase is Update's. With no files it returns SAVE_ACTION_NEW_GAME at
   once, and -1 when cancelled or the backup memory cannot be read. */
Sint32 mnFileListSelectAction(void)
{
	Sint32 fileState;
	Sint32 result;
	Bool   init;
	Uint32 group;

	group = 0;
	init = FALSE;
	fileState = mnSaveScanFileSummaries();
	if(fileState < 0)
		return -1;
	if(fileState == 0)
		return SAVE_ACTION_NEW_GAME;
	if(fileState == BUP_FILE_COUNT)
		group = 1;
	else if(fileState == SAVE_SCAN_ALL_CLEARED + BUP_FILE_COUNT)
		group = 2;
	else if(fileState > SAVE_SCAN_ALL_CLEARED)
		group = 3;
	else
		init = TRUE;
	mnIconMenuOpen();
	if(group == 0 || group == 3)
		mnIconMenuAddIcon(COMMAND_ICON_NEW_GAME);
	if(group <= 1)
		mnIconMenuAddIcon(COMMAND_ICON_CONTINUE);
	if(group == 0 || group == 3)
		mnIconMenuAddIcon(COMMAND_ICON_COPY);
	mnIconMenuAddIcon(COMMAND_ICON_ERASE);
	if(TitleBattleOptionEnabled != 0)
		mnIconMenuAddIcon(COMMAND_ICON_BATTLE);
	if(TitleUpdateOptionEnabled != 0)
		mnIconMenuAddIcon(COMMAND_ICON_UPDATE);
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, FILE_SCREEN_ICON_ROW_WIDTH, 0);
	result = mnIconMenuRun(init);
	mnIconMenuClose();
	/* The icon position's action, from the group's row of six. */
	if(result >= 0) {
		Uint32 twice = group * 2;

		result = MenuSaveSlotActionByPosition[result + (twice + group) * 2];
	}
	if(TitleBattleOptionEnabled == 0 && result == SAVE_ACTION_BATTLE)
		result = SAVE_ACTION_UPDATE;
	return result;
}

/* The label area is narrower in Japanese and wider in French. */
#define LABEL_RIGHT BY_LANGUAGE(80, 128, 128, 128, 160, 128)

/* When the chosen password level changes, clears the lower window's text area
   (from x 8 to LABEL_RIGHT) and lists what a password of that level carries:
   Gold five lines, Silver three, Bronze two. */
void mnPasswordLevelMenuDrawContents(void)
{
	ICON_MENU_STATE *state = Ram_HeapSlots->iconMenu;

	if(state->drawnSelection != state->selection) {
		state->drawnSelection = state->selection;
		mnWindowClearInteriorTiles(state->lowerWin, 8, 40, LABEL_RIGHT, 80);

		/* The lines of what each password carries, from "Levels"
		   (MsgPasswordLevel). */
		switch(state->selection) {
		case PASSWORD_LEVEL_GOLD: {
			Sint32 text = (Sint32)MsgPasswordLevel;

			mnTextDrawMessageAsTiles(text, state->lowerWin, 8, 40);
			mnTextDrawMessageAsTiles(text + 1, state->lowerWin, 8, 48);
			mnTextDrawMessageAsTiles(text + 2, state->lowerWin, 8, 56);
			mnTextDrawMessageAsTiles(text + 3, state->lowerWin, 8, 64);
			text += 4;
			mnTextDrawMessageAsTiles(text, state->lowerWin, 8, 72);
			break;
		}
		case PASSWORD_LEVEL_SILVER: {
			Sint32 text = (Sint32)MsgPasswordLevel;

			mnTextDrawMessageAsTiles(text, state->lowerWin, 8, 40);
			mnTextDrawMessageAsTiles(text + 1, state->lowerWin, 8, 48);
			text += 2;
			mnTextDrawMessageAsTiles(text, state->lowerWin, 8, 56);
			break;
		}
		case PASSWORD_LEVEL_BRONZE: {
			Sint32 text = (Sint32)MsgPasswordLevel;

			mnTextDrawMessageAsTiles(text++, state->lowerWin, 8, 40);
			mnTextDrawMessageAsTiles(text, state->lowerWin, 8, 48);
			break;
		}
		}
	}
}

/* The Spanish descriptions are indented. */
#define HELP_X SPANISH_OR(6, 0)

/* Redraws the chosen transfer method's help in the lower window. Spanish,
   French and Italian describe two methods, the password and everything else as
   the cable; French clears the window with mnWindowSprClearList and redraws
   only when the description changes. */
void mnTransferMethodMenuDrawHelp(void)
{
	ICON_MENU_STATE *state = Ram_HeapSlots->iconMenu;
	Sint16			*shown = &state->drawnSelection;
	Sint16			*cur = &state->selection;

#if TRANSFER_HELP_SKIPS_EMPTY_WINDOW_REDRAW
	if(*shown != *cur) {
		if(*cur == TRANSFER_METHOD_PASSWORD) {
			mnWindowSprClearList(state->lowerWin);
			mnTextDrawMessage(
				(Sint32)MsgPasswordTransferHelp, state->lowerWin, 8, 4);
			mnTextDrawMessage(
				(Sint32)MsgPasswordTransferHelp + 1, state->lowerWin, 0, 16);
		} else if(*shown == TRANSFER_METHOD_PASSWORD) {
			mnWindowSprClearList(state->lowerWin);
			mnTextDrawMessage(
				(Sint32)MsgCableTransferHelp, state->lowerWin, 0, 4);
			mnTextDrawMessage(
				(Sint32)MsgCableTransferHelp + 1, state->lowerWin, 0, 16);
			mnTextDrawMessage(
				(Sint32)MsgCableTransferHelp + 2, state->lowerWin, 0, 28);
		}
		state->drawnSelection = state->selection;
	}
#else
	if(*shown != *cur) {
		*shown = *cur;
		mnWindowSprPrepareForRedraw(state->lowerWin);
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
		if(*cur == TRANSFER_METHOD_PASSWORD) {
			mnTextDrawMessage(
				(Sint32)MsgPasswordTransferHelp, state->lowerWin, HELP_X, 4);
			mnTextDrawMessage((Sint32)MsgPasswordTransferHelp + 1,
				state->lowerWin, HELP_X, 16);
		} else {
			mnTextDrawMessage(
				(Sint32)MsgCableTransferHelp, state->lowerWin, HELP_X, 4);
			mnTextDrawMessage(
				(Sint32)MsgCableTransferHelp + 1, state->lowerWin, HELP_X, 16);
			mnTextDrawMessage(
				(Sint32)MsgCableTransferHelp + 2, state->lowerWin, HELP_X, 28);
		}
#else
		switch(*cur) {
		case TRANSFER_METHOD_PASSWORD:
			mnTextDrawMessage(
				(Sint32)MsgPasswordTransferHelp, state->lowerWin, 0, 4);
			mnTextDrawMessage(
				(Sint32)MsgPasswordTransferHelp + 1, state->lowerWin, 0, 16);
			break;
		case TRANSFER_METHOD_CABLE:
			mnTextDrawMessage(
				(Sint32)MsgCableTransferHelp, state->lowerWin, 0, 4);
			mnTextDrawMessage(
				(Sint32)MsgCableTransferHelp + 1, state->lowerWin, 0, 16);
			mnTextDrawMessage(
				(Sint32)MsgCableTransferHelp + 2, state->lowerWin, 0, 28);
			break;
		default:
			mnTextDrawMessage(
				(Sint32)MsgClearDataTransferHelp, state->lowerWin, 0, 4);
			mnTextDrawMessage(
				(Sint32)MsgClearDataTransferHelp + 1, state->lowerWin, 0, 16);
			break;
		}
#endif
	}
#endif
}

/* The entries' width and the windows' places and widths, in tiles, and where
   the lines of text start, in pixels, are fitted to each language's text. */
#define ENTRY_WIDTH			 BY_LANGUAGE(5, 7, 7, 9, 9, 7)
#define PASSWORD_UPPER_X	 BY_LANGUAGE(9, 8, 7, 5, 5, 7)
#define PASSWORD_UPPER_WIDTH BY_LANGUAGE(13, 14, 16, 21, 20, 17)
#define PASSWORD_LOWER_X	 BY_LANGUAGE(5, 4, 4, 3, 3, 3)
#define PASSWORD_LOWER_WIDTH BY_LANGUAGE(21, 22, 22, 25, 25, 25)
#define TRANSFER_UPPER_X	 BY_LANGUAGE(9, 6, 6, 4, 5, 5)
#define TRANSFER_UPPER_WIDTH BY_LANGUAGE(13, 18, 18, 22, 20, 20)
#define TRANSFER_TITLE_X	 BY_LANGUAGE(0, 12, 6, 2, 2, 2)
#define TRANSFER_LOWER_X	 BY_LANGUAGE(5, 5, 2, 1, 1, 1)
#define TRANSFER_LOWER_WIDTH BY_LANGUAGE(21, 21, 26, 28, 28, 28)

/* Spanish, French and Italian indent the password's lines and set them closer
   together. */
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH || \
	LANGUAGE == LANGUAGE_ITALIAN
#define PASSWORD_TEXT_X		 8
#define PASSWORD_TEXT_Y		 0
#define PASSWORD_LINE_HEIGHT 11
#else
#define PASSWORD_TEXT_X		 0
#define PASSWORD_TEXT_Y		 4
#define PASSWORD_LINE_HEIGHT 12
#endif

/* Selects the transfer method or the password level (`mode`, enum
   SEND_OPTION_MENU), with a heading window above and an explanation of the
   selected icon below, redrawn by an event while the menu runs. Returns the
   place chosen, or -1 when cancelled. */
Sint32 mnIconMenuChooseSendOption(Sint32 mode)
{
	ICON_MENU_STATE *state;
	WINDOW			*win;
	Sint32			 msg;
	WINDOW		   **upper;
	Sint32			 result;

	mode &= 1;
	mnIconMenuOpen();
	state = Ram_HeapSlots->iconMenu;
	if(mode == SEND_OPTION_TRANSFER_METHOD) {
		mnIconMenuAddIcon(COMMAND_ICON_PASSWORD);
		mnIconMenuAddIcon(COMMAND_ICON_LINK);
	} else {
		mnIconMenuAddIcon(COMMAND_ICON_GOLD);
		mnIconMenuAddIcon(COMMAND_ICON_SILVER);
		mnIconMenuAddIcon(COMMAND_ICON_BRONZE);
	}
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, ENTRY_WIDTH, 0);
	if(mode != SEND_OPTION_TRANSFER_METHOD) {
		sySetEvent(mnPasswordLevelMenuDrawContents, EVENT_PASS_TICK - 10);
		state->drawnSelection = -1;
		win = mnWindowCreate(
			PASSWORD_UPPER_X, 0, PASSWORD_UPPER_WIDTH, 4, WINDOW_FLAG_INSTANT);
		msg = (Sint32)MsgPasswordEntry;
		upper = &state->upperWin;
		*upper = win;
		mnTextDrawMessage(msg, win, 0, 4);
		win = mnWindowCreate(
			PASSWORD_LOWER_X, 4, PASSWORD_LOWER_WIDTH, 12, WINDOW_FLAG_INSTANT);
		state->lowerWin = win;
		mnTextDrawMessage(msg + 1, win, PASSWORD_TEXT_X, PASSWORD_TEXT_Y);
#if LANGUAGE == LANGUAGE_FRENCH
		/* French writes a third line: "Niveaux" (MsgPasswordLevel). */
		mnTextDrawMessage(msg + 2, state->lowerWin, PASSWORD_TEXT_X,
			PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT);
		msg += 3;
		mnTextDrawMessage(msg, state->lowerWin, PASSWORD_TEXT_X,
			PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT * 2);
#else
		msg += 2;
		mnTextDrawMessage(msg, state->lowerWin, PASSWORD_TEXT_X,
			PASSWORD_TEXT_Y + PASSWORD_LINE_HEIGHT);
#endif
	} else {
		sySetEvent(mnTransferMethodMenuDrawHelp, EVENT_PASS_TICK - 10);
		state->drawnSelection = -1;
		win = mnWindowCreate(
			TRANSFER_UPPER_X, 0, TRANSFER_UPPER_WIDTH, 4, WINDOW_FLAG_INSTANT);
		upper = &state->upperWin;
		*upper = win;
		mnTextDrawMessage((Sint32)MsgTransferMethod, win, TRANSFER_TITLE_X, 4);
		state->lowerWin = mnWindowCreate(
			TRANSFER_LOWER_X, 5, TRANSFER_LOWER_WIDTH, 7, WINDOW_FLAG_INSTANT);
	}
	result = mnIconMenuRun(0);
	if(mode != SEND_OPTION_TRANSFER_METHOD)
		syCloseEvent(mnPasswordLevelMenuDrawContents);
	else
		syCloseEvent(mnTransferMethodMenuDrawHelp);
	mnWindowSprPrepareForRedraw(*upper);
	mnWindowSprPrepareForRedraw(state->lowerWin);
	mnWindowClose(*upper, 2);
	mnWindowClose(state->lowerWin, 2);
	sySynch(1);
	mnIconMenuClose();
	return result;
}

/* Selects a shop command (Buy, Sell, Artifacts, Repair), centred on the bottom
   row, starting on position `selection`, and returns the position chosen. */
Sint32 mnSelectShopCommand(Sint32 selection)
{
	Sint32 result;

	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_BUY);
	mnIconMenuAddIcon(COMMAND_ICON_SELL);
	mnIconMenuAddIcon(COMMAND_ICON_ARTIFACTS);
	mnIconMenuAddIcon(COMMAND_ICON_REPAIR);
#if (LANGUAGE == LANGUAGE_GERMAN)
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, 8, 0);
#elif (LANGUAGE != LANGUAGE_JAPANESE)
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, 7, 0);
#else
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, 6, 0);
#endif
	result = mnIconMenuRun(selection);
	mnIconMenuClose();
	return result;
}

/* Selects a Sanctum service (Revive, Cure Poison, Repel Evil, Remove Curse),
   centred on the bottom row, starting on position `selection`. */
Sint32 mnSelectSanctumService(Sint32 selection)
{
	Sint32 result;

	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_REVIVE);
	mnIconMenuAddIcon(COMMAND_ICON_CURE_POISON);
	mnIconMenuAddIcon(COMMAND_ICON_REPEL_EVIL);
	mnIconMenuAddIcon(COMMAND_ICON_REMOVE_CURSE);
#if (LANGUAGE != LANGUAGE_JAPANESE)
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, 10, 0);
#else
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, 8, 0);
#endif
	result = mnIconMenuRun(selection);
	mnIconMenuClose();
	return result;
}

/* Selects Yes or No, starting on `selection`; cancelling answers No. The icons
   stand on the bottom row when `bottom` is set, else at the top; `msg` is
   the first label message, or 0 for the command names, and `labelWidth` the
   label window's width, 3 when 0. */
Sint32 mnYesNoMenuRun(
	Sint32 bottom, Sint32 msg, Sint32 labelWidth, Sint32 selection)
{
	Sint32 row;

	row = 0;

	mnIconMenuOpen();

	if(labelWidth == 0) {
		labelWidth = 3;
	}
	if(bottom != 0) {
		row = ICON_MENU_BOTTOM_ROW;
	}

	mnIconMenuAddIcon(COMMAND_ICON_YES);
	mnIconMenuAddIcon(COMMAND_ICON_NO);
	mnIconMenuCenter(row, labelWidth, msg);

	selection = mnIconMenuRun(selection);
	mnIconMenuClose();
	if(selection == -1) {
		selection = NO;
	}
	return selection;
}

/* Selects Yes or No with the icons from tile (x, y), starting on `selection`;
   cancelling answers No. */
Sint32 mnYesNoMenuRunAt(Sint32 x, Sint32 y, Sint32 selection)
{
	Sint32 msg = MsgCommandYes;

	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_YES);
	mnIconMenuAddIcon(COMMAND_ICON_NO);
	mnIconMenuLayoutIcons(x, y, 3, msg);
	selection = mnIconMenuRun(selection);
	mnIconMenuClose();
	if(selection == -1) {
		selection = NO;
	}
	return selection;
}

/* The label window's width for each language. */
#define PAIR_WIDTH BY_LANGUAGE(7, 9, 9, 9, 10, 10)

/* Selects Coins or Lucky Medals, centred on the bottom row, starting on
   `selection`, and returns the position chosen. */
Sint32 mnSelectCoinsOrMedals(Sint32 selection)
{
	Sint32 result;

	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_COINS);
	mnIconMenuAddIcon(COMMAND_ICON_LUCKY_MEDALS);
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, PAIR_WIDTH, 0);
	result = mnIconMenuRun(selection);
	mnIconMenuClose();
	return result;
}

/* Unused: starts the debug scene and entrance selector as an event. */
UNUSED void mnDebugSceneSelectSchedule(void)
{
	sySetEvent(mnDebugSceneSelectRun, EVENT_PASS_TICK);
}

#undef LABEL_RIGHT
#undef HELP_X
#undef ENTRY_WIDTH
#undef PASSWORD_UPPER_X
#undef PASSWORD_UPPER_WIDTH
#undef PASSWORD_LOWER_X
#undef PASSWORD_LOWER_WIDTH
#undef TRANSFER_UPPER_X
#undef TRANSFER_UPPER_WIDTH
#undef TRANSFER_TITLE_X
#undef TRANSFER_LOWER_X
#undef TRANSFER_LOWER_WIDTH
#undef PASSWORD_TEXT_X
#undef PASSWORD_TEXT_Y
#undef PASSWORD_LINE_HEIGHT
#undef PAIR_WIDTH
#undef ICON_MENU_BOTTOM_ROW
#undef ICON_ROW_LINES
#undef PULSE_OVERHANG_X
#undef PULSE_OVERHANG_LINES
