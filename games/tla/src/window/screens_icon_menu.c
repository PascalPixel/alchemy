/* @file */
/* Collecting the party's battle commands, then the icon menus: a row of */
/* command icons with a label window, the selected icon pulsing, as the field */
/* menu, the File Screen, the transfer screens, the shops, the sanctums, Yes or
 */
/* No and Coins or Lucky Medals use it. */
/* Part of screens.c. */

/* While a party member's commands are chosen (mnBattleCollectPartyCommands), */
/* where the Japanese layout differs: the chosen summon's window and the */
/* columns of its first Djinn cost, the element icon and then the count, and */
/* the chosen item's window, in tiles; the party member's portrait, and the */
/* chosen command's icon beside the summon list, beside the chosen summon and */
/* beside the Djinn list, in pixels. */
#if LANGUAGE != LANGUAGE_JAPANESE
#define SUMMON_WINDOW_X				 10
#define BATTLE_SUMMON_WINDOW_WIDTH	 17
#define BATTLE_SUMMON_ELEMENT_COLUMN 11
#define BATTLE_SUMMON_COST_COLUMN	 12
#define BATTLE_ITEM_WINDOW_WIDTH	 16
#define BATTLE_PORTRAIT_X			 0
#define BATTLE_SUMMON_ICON_X		 80
#define SUMMON_CHOSEN_ICON_X		 56
#define DJINN_MENU_ICON_X			 140
#else
#define SUMMON_WINDOW_X				 9
#define BATTLE_SUMMON_WINDOW_WIDTH	 16
#define BATTLE_SUMMON_ELEMENT_COLUMN 10
#define BATTLE_SUMMON_COST_COLUMN	 11
#define BATTLE_ITEM_WINDOW_WIDTH	 15
#define BATTLE_PORTRAIT_X			 4
#define BATTLE_SUMMON_ICON_X		 88
#define SUMMON_CHOSEN_ICON_X		 48
#define DJINN_MENU_ICON_X			 144
#endif

/* Where "Attack" is written in its window once chosen, in pixels. */
#if LANGUAGE == LANGUAGE_FRENCH
#define BATTLE_COMMAND_NAME_X 14
#else
#define BATTLE_COMMAND_NAME_X 16
#endif

/* The chosen command's icon beside the Psynergy list, in pixels. */
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH)
#define PSYNERGY_MENU_ICON_X 40
#else
#define PSYNERGY_MENU_ICON_X 48
#endif

/* The chosen command's icon beside the window that names Attack, the chosen */
/* Psynergy, the chosen Djinni or the chosen item, and beside the item list, in
 */
/* pixels. */
#define ATTACK_ICON_X		   64
#define PSYNERGY_CHOSEN_ICON_X 40
#define DJINN_CHOSEN_ICON_X	   64
#define ITEM_MENU_ICON_X	   96
#define ITEM_CHOSEN_ICON_X	   48

/* Flee's priority, above every other action's: the party flees first in the */
/* turn. */
#define FLEE_PRIORITY 32766
/* The priority an action takes when its Agility roll runs past 32767. */
#define OVERFLOWED_PRIORITY 2000

/* The x of the first command icon's sprite. */
#define COMMAND_ICON_X(state) ((state)->icons[0].oam.f.x)

/* Collects the party's commands for a turn into `commands`, one for each of */
/* the `commandCount` units in `actors`, with two nested functions: the party */
/* panels' clear and the link wait's message (mnLinkDrawWaitingForOpponent). */
/* Unlike The Broken Seal's, each round first appends the forced actions of */
/* statused members (mnBattlePartySplitChoosers), whose count is added to the */
/* result; B on the party's row undoes a reserve swap; Switch swaps a reserve */
/* member in (mnBattlePartyChooseReserve after choosing the slot); Status lists
 */
/* the reserve as well; and the Djinn tutorial's hint follows an unleash. */
s32 mnBattleCollectPartyCommands(BATTLE_ACTION *commands, u16 *actors,
								 s32 commandCount) {
	u16 highlighted[2];
	BATTLE_MENU_STATE *state;
	BATTLE_ACTION *commandsOut;
	s32 forcedCount;
	BATTLE_ACTION *command;
	UNIT *unit;
	s32 target;
	s32 actorId;
	s32 commandKind;
	s32 parameter;
	s32 largeIconSlot;
	s32 smallIconSlot;
	s32 commandIndex;
	s32 summonIconSlot;
	s32 choice;
	WINDOW *commandWindow;

	void btDrawPartyPanelsWithEmptyList(void) {
		u16 data[2];

		data[0] = BATTLE_UNIT_LIST_END;
		mnPartyStatHighlightMembers(data);
		btHighlightSetModesStub(data, HIGHLIGHT_SELECTED);
	}

	target = PLACE_PARTY;
	commandsOut = NULL;
	choice = 0;
	commandKind = 0;
	parameter = 0;
	largeIconSlot = syVramCacheAllocEntry(COMMAND_ICON_BYTES);
	smallIconSlot = syVramCacheAllocEntry(512);
	state =
		syHeapSlotAllocEwram(HEAP_SLOT_BATTLE_MENU, sizeof(BATTLE_MENU_STATE));
	forcedCount = 0;
	mnCommandIconReloadSlot(largeIconSlot, 0);
	mnGraphicsExpandVramTilesByColTable(
		(u16 *)(BG_CHAR_BLOCK(0) + PAGE_DIGITS_TILE * TILE_SIZE_4BPP));
	mnPageArrowsDraw(LIST_PAGE_ARROW_VRAM);
	mnRangeIconBuildTiles();
	state->swappedUnit = -1;
	state->lastTgt = -1;
	{
		BATTLE_STATE *battle = Ram_HeapSlots->battleState;
		s32 k;

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
			s32 missedLinkFrames;
			u16 *signature;

			state->linkTmr = 1;
			signature = (u16 *)SerialTransfer.payload;
			signature[4] = 'V';
			signature[5] = 'S';
			signature[6] = 'S';
			signature[7] = 'T';
			missedLinkFrames = 0;
			for(;;) {
				u16 *peerSignature = LinkPeerSignatures[battle->linkSide ^ 1];

				if(battle->linkPaused != 0) {
					commandCount = -1;
					state->linkTmr = 0;
					break;
				}
				if((LinkStatusWork & LINK_STATUS_HEARD_BOTH) !=
				   LINK_STATUS_HEARD_BOTH) {
					if(++missedLinkFrames > LINK_MAX_MISSED_FRAMES) {
						commandCount = -1;
						state->linkTmr = 0;
						break;
					}
				}
				else {
					missedLinkFrames = 0;
					if(peerSignature[4] == 'V' && peerSignature[5] == 'S' &&
					   peerSignature[6] == 'S' && peerSignature[7] == 'T')
						break;
					if(peerSignature[4] == 'E' && peerSignature[5] == 'D' &&
					   peerSignature[6] == 'V' && peerSignature[7] == 'S')
						break;
					if(!(peerSignature[0] == 'E' && peerSignature[1] == 'X' &&
						 peerSignature[2] == 'E' && peerSignature[3] == 'C')) {
						commandCount = -1;
						state->linkTmr = 0;
						break;
					}
				}
				sySynch(1);
			}
		}
	}
	sySetEvent(mnBattleMenuUpdateEachFrame, TASK_PASS_FRAME);

	for(;;) {
		commandsOut = commands;
		if(commandCount >= 0) {
			forcedCount = 0;
			commandCount =
				mnBattlePartySplitChoosers(actors, &commandsOut, &forcedCount);
		}
		btDrawPartyPanelsWithEmptyList();
		state->iconShown[2] = 0;
		state->commandActor = -1;
		state->tutorialStep = 0;
		if(syGameFlagGet(FLAG_DJINN_LESSON)) {
			state->tutorialStep = 1;
			state->tutorialDelay = 60;
		}
		if(commandCount >= 0)
			choice = mnBattleCommandRowRun(COMMAND_ROW_PARTY, commandCount);
		else
			choice = COMMAND_ICON_FIGHT;

		/* B on the party's row: undo the last reserve swap. */
		if(choice == -1) {
			s32 swapped = state->swappedUnit;
			BATTLE_STATE *battle;
			s32 old;
			s32 shortSwapped;
			s32 k;

			if(swapped == -1)
				continue;
			battle = Ram_HeapSlots->battleState;
			old = battle->partyUnits[state->swappedPlace];
			battle->partyUnits[state->swappedPlace] = swapped;
			shortSwapped = (s16)swapped;
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
			choice = mnBattleTargetRunSelection(0, ABILITY_TARGET_ALLIES, 1,
												TARGET_HINT_LAST_TARGET);
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
		 */
		/* debug build with Start held the enemies'. */
		if(choice == COMMAND_ICON_STATUS) {
			u16 *block = syHeapAlloc(18);
			s32 side;
			s32 count;

			if(DebugMode != 0 && (InputWork.held & KEY_START))
				side = BATTLE_SIDE_MASK_ENEMIES;
			else
				side = BATTLE_SIDE_MASK_PARTY;
			count = btUnitListIdsStub(side, block);
			if(side == BATTLE_SIDE_MASK_PARTY)
				count += btPartyPrepareReserveOwnersStub(block + count);
			sySynch(1);
			mnBattleStatScrRun(block, count, block[0]);
			syHeapRelease(block);
			continue;
		}
		/* Flee: one COMMAND_FLEE for the party, first in the turn. */
		if(choice == COMMAND_ICON_FLEE) {
			if(mnBattleRefuseFlee() != 0)
				continue;
			commandCount = 1;
			command = commands;
			command->unitId = actors[0];
			forcedCount = 0;
			command->pri = FLEE_PRIORITY;
			command->command = COMMAND_FLEE;
			command->prm = 0;
			command->tgt = PLACE_PARTY;
			break;
		}
		if(choice != COMMAND_ICON_FIGHT)
			break;
		sySndPlayStub(SE_CAST);
		commandIndex = 0;
		if(commandIndex >= commandCount)
			break;

		for(;;) {
			if(commandIndex == 0) {
				btDjinnCountStandbyStub(0, state->standbyDjinn[0]);
			}
			else {
				s32 k;

				for(k = 0; k < 4; k++)
					state->standbyDjinn[commandIndex][k] =
						state->standbyDjinn[commandIndex - 1][k];
			}
			command = &commandsOut[commandIndex];
			actorId = actors[commandIndex];
			/* UB: syUnitGet returns a pointer, not the cast's doubleword
			 * result. */
#ifdef UBFIX
			unit = syUnitGet(actorId);
#else
			unit = (UNIT *)(s32)((s64 (*)(s32))syUnitGet)(actorId);
#endif
			{
				MENU_SPR *iconSprite;

				BattleMenuStateWork->commandActor = actorId;
				BattleMenuStateWork->commandSelection[1] = 0;
				iconSprite = &BattleMenuStateWork->icons[2];
				syMenuSprInit(*iconSprite,
							  OBJ_SIZE_32X32 |
								  OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
				iconSprite->oam.f.tile =
					mnPortraitLoad(unit->enemyId, smallIconSlot);
				iconSprite->oam.f.x = BATTLE_PORTRAIT_X;
				iconSprite->oam.f.y = 128;
				iconSprite->oam.f.pal = PORTRAIT_PALETTE;
				state->iconShown[2] = 1;
			}

		mark_visible:
			mnWindowMarkVisibleTextTiles();
			BattleMenuStateWork->iconShown[0] = 0;
			BattleMenuStateWork->chosenDjinn[commandIndex] =
				BATTLE_MENU_NO_DJINNI;
			highlighted[0] = actorId;
			highlighted[1] = BATTLE_UNIT_LIST_END;
			btHighlightSetModesStub(highlighted, HIGHLIGHT_SELECTED);
			mnPartyStatHighlightMembers(highlighted);
			sySynch(1);
			choice = mnBattleCommandRowRun(COMMAND_ROW_MEMBER, 0);
			sySynch(1);
			if(choice == -2) {
				u16 *block = syHeapAlloc(12);
				s32 count = btUnitListIdsStub(BATTLE_SIDE_MASK_PARTY, block);

				state->iconShown[2] = 0;
				sySynch(1);
				mnBattleStatScrRun(block, count, actorId);
				state->iconShown[2] = 1;
				syHeapRelease(block);
				continue;
			}
			btHighlightSetModesStub(highlighted, HIGHLIGHT_OFF);
			if(choice == -1) {
				if(commandIndex == 0)
					break;
				commandIndex--;
				continue;
			}
			{
				MENU_SPR *iconSprite = &BattleMenuStateWork->icons[0];

				if(BattleMenuStateWork->tmr == 0)
					choice = COMMAND_ICON_DEFEND;
				iconSprite->oam.word.attr23 = 0;
				iconSprite->oam.word.attr01 =
					OBJ_SIZE_32X32 | OBJ_256_COLORS |
					OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
				iconSprite->oam.f.tile =
					mnCommandIconReloadSlot(largeIconSlot, choice);
				if(choice == COMMAND_ICON_DJINN)
					iconSprite->oam.f.x = 128;
				else
					iconSprite->oam.f.x = 96;
				iconSprite->oam.f.y = 136;
				state->iconShown[0] = 1;
			}

			switch(choice) {
			case COMMAND_ICON_ATTACK:
				commandWindow = mnWindowCreate(
					11, 17, 11, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
				mnTextDrawMessageAsTiles((s32)MsgCommandName, commandWindow,
										 BATTLE_COMMAND_NAME_X, 0);
				COMMAND_ICON_X(state) = ATTACK_ICON_X;
				sySndPlayStub(SE_MENU_CONFIRM);
				choice = mnBattleTargetRunSelection(
					actorId, ABILITY_TARGET_FOES, 1, TARGET_HINT_NONE);
				mnWindowClose(commandWindow, 1);
				if(choice == -1)
					goto mark_visible;
				commandKind = COMMAND_ATTACK;
				target = choice;
				command->range = 1;
				break;

			case COMMAND_ICON_PSYNERGY: {
				s32 k;
				MENU_SPR *iconSprite;
				ABILITY_DEFINITION *ability;
				WIN_STATE *windowState;
				u16 *list;
				u8 *pick;
				s32 cost;
				s32 handle;
				s32 count;
				s32 entryValue;

				sySndPlayStub(SE_MENU_CONFIRM);
				/* DEAD: choice cleared before the list sets it; the ROM keeps
				 * its mov. */
				choice = 0;
				BATTLE_MENU_SET_CURSOR(0, 0, 0);
			psynergy_menu:
				list = state->listAbilities;
				pick = state->listSlots;
				COMMAND_ICON_X(state) = PSYNERGY_MENU_ICON_X;
				count = 0;
				for(k = 0; k != PSYNERGY_SLOTS &&
						   (entryValue = unit->psynergy[k].encodedAction &
										 PSYNERGY_ABILITY_MASK) != 0;
					k++) {
					if(btAbilityGet(entryValue)->flg &
					   ABILITY_USABLE_IN_BATTLE) {
						pick[count] = k;
						list[count] = entryValue;
						count++;
					}
				}
				pick[count] = 0;
				list[count] = 0;
				choice = mnBattlePsynergyMenuRun(actorId, list, count);
				if(choice == -1)
					goto mark_visible;
				parameter = unit->psynergy[pick[choice]].encodedAction &
							PSYNERGY_ABILITY_MASK;
				ability = btAbilityGet(parameter);
				cost = ability->range;
				iconSprite = &state->icons[1];
				handle = syVramCacheAllocEntry(128);
				windowState = WINDOW_STATE;
				commandWindow = mnWindowCreate(
					8, 17, 18, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
				COMMAND_ICON_X(state) = PSYNERGY_CHOSEN_ICON_X;
				syMenuSprInit(*iconSprite,
							  OBJ_SIZE_16X16 |
								  OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
				iconSprite->oam.f.x = commandWindow->x * 8 + 8;
				iconSprite->oam.f.y = commandWindow->y * 8 + 4;
				iconSprite->oam.f.tile =
					mnAbilityIconLoadIntoSlot(parameter, handle);
				state->iconShown[1] = 1;
				windowState->textPal = BASIC_COLORS_PALETTE;
				if(ability->ppCost > unit->pp)
					mnTextSetCol(TEXT_COLOR_RED);
				else if(unit->psySeal != 0)
					mnTextSetCol(TEXT_COLOR_PINK);
				mnTextDrawMessageAsTiles(parameter + (s32)MsgAbilityName,
										 commandWindow, 16, 0);
				mnTextDrawNumAtOffset(ability->ppCost, 2, commandWindow, 104,
									  0);
				windowState->textPal = WINDOW_PALETTE;
				mnTextSetCol(TEXT_COLOR_WHITE);
				mnWindowSetTilemapEntry(commandWindow, WINDOW_TILE_PP_LABEL, 11,
										0, 0);
				mnWindowSetTilemapEntry(commandWindow, WINDOW_TILE_PP_LABEL_END,
										12, 0, 0);
				if(ability->element != ELEMENT_NONE)
					mnWindowSetTilemapEntry(
						commandWindow, ability->element + ELEMENT_ICON_TILE, 15,
						0, 0);
				command->range = cost;
				sySndPlayStub(SE_MENU_CONFIRM);
				choice = mnBattleTargetRunSelection(
					actorId, ability->tgtMode, cost,
					mnBattleClassifyEntryKind(ability));
				state->iconShown[1] = 0;
				syVramCacheFreeEntry(handle);
				mnWindowClose(commandWindow, 1);
				if(choice == -1)
					goto psynergy_menu;
				commandKind = COMMAND_PSYNERGY;
				target = choice;
				break;
			}

			case COMMAND_ICON_SUMMON: {
				const SUMMON_DEFINITION *summon;
				ABILITY_DEFINITION *ability;
				MENU_SPR *iconSprite;
				s32 ok;
				s32 j;
				s32 n;

				sySndPlayStub(SE_MENU_CONFIRM);
				BATTLE_MENU_SET_CURSOR(0, 0, 0);
			summon_menu:
				COMMAND_ICON_X(state) = BATTLE_SUMMON_ICON_X;
				mnPartyStatHighlightMembers(highlighted);
				choice = mnBattleSummonMenuRun(
					0, 0, state->standbyDjinn[commandIndex]);
				mnPartyStatHighlightMembers(highlighted);
				if(choice == -1)
					goto mark_visible;
				commandKind = COMMAND_SUMMON;
				parameter = choice;
				sySynch(1);
				iconSprite = &state->icons[1];
				summon = gmSummonDefinitionGet(choice);
				ability = btAbilityGet(summon->abilityId);
				summonIconSlot = syVramCacheAllocEntry(128);
				commandWindow = mnWindowCreate(
					SUMMON_WINDOW_X, 17, BATTLE_SUMMON_WINDOW_WIDTH, 3,
					WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
				n = 0;
				for(j = 0; j <= 3; j++) {
					if(summon->djinnRequired[j] >
					   state->standbyDjinn[commandIndex][j])
						break;
				}
				ok = j == 4;
				COMMAND_ICON_X(state) = SUMMON_CHOSEN_ICON_X;
				((u32 *)&iconSprite->oam)[0] =
					OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
				((u32 *)&iconSprite->oam)[1] = 0;
				ASM_NOTHING();
				iconSprite->oam.f.x = commandWindow->x * 8 + 8;
				iconSprite->oam.f.y = commandWindow->y * 8 + 4;
				iconSprite->oam.f.tile = mnAbilityIconLoadIntoSlot(
					summon->abilityId & ACTION_ID_MASK, summonIconSlot);
				state->iconShown[1] = 1;
				if(ok == 0)
					mnTextSetCol(TEXT_COLOR_RED);
				mnTextDrawMessageAsTiles(
					gmSummonDefinitionGet(choice)->abilityId +
						(s32)MsgAbilityName,
					commandWindow, 16, 0);
				for(j = 0; j <= 3; j++) {
					if(summon->djinnRequired[j] != 0) {
						mnWindowSetTilemapEntry(
							commandWindow, j + ELEMENT_ICON_TILE,
							n * 2 + BATTLE_SUMMON_ELEMENT_COLUMN, 0, 0);
						mnWindowPutGlyph(commandWindow,
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
				choice = mnBattleTargetRunSelection(
					actorId, ability->tgtMode, ability->range,
					mnBattleClassifyEntryKind(ability));
				command->range = ability->range;
				state->iconShown[1] = 0;
				syVramCacheFreeEntry(summonIconSlot);
				mnWindowClose(commandWindow, 1);
				if(choice == -1)
					goto summon_menu;
				for(j = 0; j <= 3; j++) {
					if(summon->djinnRequired[j] >
					   state->standbyDjinn[commandIndex][j]) {
						state->standbyDjinn[commandIndex][j] = 0;
						break;
					}
					state->standbyDjinn[commandIndex][j] -=
						summon->djinnRequired[j];
				}
				target = choice;
				break;
			}

			case COMMAND_ICON_DJINN: {
				ABILITY_DEFINITION *ability;
				s32 cost;
				s32 element;
				s32 index;

				sySndPlayStub(SE_MENU_CONFIRM);
				BATTLE_MENU_SET_CURSOR(0, 0, 0);
			djinn_menu:
				COMMAND_ICON_X(state) = DJINN_MENU_ICON_X;
				BattleMenuStateWork->chosenDjinn[commandIndex] =
					BATTLE_MENU_NO_DJINNI;
				choice = mnBattleDjinnMenuRun(actorId, 1);
				command->range = 0;
				if(choice == -1)
					goto mark_visible;
				parameter = choice;
				commandKind = COMMAND_DJINN;
				BattleMenuStateWork->chosenDjinn[commandIndex] = choice;
				element = DJINN_COMMAND_ELEMENT(parameter);
				index = DJINN_COMMAND_INDEX(parameter);
				if(gmDjinnIsSetStub(actorId, element, index) != 0) {
					ability =
						btAbilityGet(gmDjinnGetAbilityIdStub(element, index));
					cost = ability->range;
					commandWindow = mnWindowCreate(11, 17, 10, 3,
												   WINDOW_FLAG_INSTANT |
													   WINDOW_FLAG_UNREAD);
					COMMAND_ICON_X(state) = DJINN_CHOSEN_ICON_X;
					mnWindowSetTilemapEntry(
						commandWindow, element + ELEMENT_ICON_TILE, 0, 0, 0);
					mnTextDrawMessageAsTiles(element * DJINN_PER_ELEMENT +
												 index + (s32)MsgDjinnName,
											 commandWindow, 16, 0);
					command->range = cost;
					sySynch(1);
					sySndPlayStub(SE_MENU_CONFIRM);
					choice = mnBattleTargetRunSelection(
						actorId, ability->tgtMode, cost,
						mnBattleClassifyEntryKind(ability));
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
					mnWindowClose(commandWindow, 1);
					if(choice == -1)
						goto djinn_menu;
					target = choice;
					break;
				}
				commandWindow = mnWindowCreate(
					11, 17, 10, 3, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
				COMMAND_ICON_X(state) = DJINN_CHOSEN_ICON_X;
				mnTextSetCol(TEXT_COLOR_RED);
				mnWindowSetTilemapEntry(commandWindow,
										element + ELEMENT_ICON_TILE, 0, 0, 0);
				mnTextDrawMessageAsTiles(element * DJINN_PER_ELEMENT + index +
											 (s32)MsgDjinnName,
										 commandWindow, 16, 0);
				mnTextSetCol(TEXT_COLOR_WHITE);
				command->range = 1;
				sySynch(1);
				sySndPlayStub(SE_MENU_CONFIRM);
				choice = mnBattleTargetRunSelection(
					actorId, ABILITY_TARGET_SELF, 0, TARGET_HINT_SET_DJINNI);
				mnWindowClose(commandWindow, 1);
				if(choice == -1)
					goto djinn_menu;
				target = choice;
				break;
			}

			case COMMAND_ICON_ITEM: {
				s32 k;
				MENU_SPR *iconSprite;
				ABILITY_DEFINITION *ability;
				u16 *list;
				u8 *pick;
				s32 cost;
				s32 handle;
				s32 count;
				s32 entryValue;
				s32 selection;

				sySndPlayStub(SE_MENU_CONFIRM);
				/* DEAD: choice cleared before the list sets it; the ROM keeps
				 * its mov. */
				choice = 0;
				BATTLE_MENU_SET_CURSOR(0, 0, 0);
			item_menu:
				COMMAND_ICON_X(state) = ITEM_MENU_ICON_X;
				pick = state->listSlots;
				list = state->listAbilities;
				count = 0;
				for(k = 0; k != INVENTORY_SLOTS &&
						   (entryValue = unit->inventory[k]) != 0;
					k++) {
					gmItemGetDirectStub(entryValue);
					if(mnItemCheckBattleUse(actorId, entryValue) == 0) {
						list[count] = entryValue;
						pick[count++] = k;
					}
				}
				for(k = 0; k != INVENTORY_SLOTS &&
						   (entryValue = unit->inventory[k]) != 0;
					k++) {
					gmItemGetDirectStub(entryValue);
					if(mnItemCheckBattleUse(actorId, entryValue) != 0) {
						list[count] = entryValue;
						pick[count++] = k;
					}
				}
				list[count] = 0;
				choice = mnBattleItemMenuRun(actorId, list, count);
				if(choice == -1)
					goto mark_visible;
				selection = pick[choice];
				parameter = selection;
				ability = btAbilityGet(
					gmItemGetDirectStub(unit->inventory[selection])->abilityId);
				cost = ability->range;
				iconSprite = &state->icons[1];
				handle = syVramCacheAllocEntry(128);
				commandWindow =
					mnWindowCreate(9, 17, BATTLE_ITEM_WINDOW_WIDTH, 3,
								   WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
				COMMAND_ICON_X(state) = ITEM_CHOSEN_ICON_X;
				syMenuSprInit(*iconSprite,
							  OBJ_SIZE_16X16 |
								  OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT));
				iconSprite->oam.f.x = commandWindow->x * 8 + 8;
				iconSprite->oam.f.y = commandWindow->y * 8 + 4;
				iconSprite->oam.f.tile =
					mnItemIconLoadIntoSlot(unit->inventory[selection], handle);
				state->iconShown[1] = 1;
				mnTextDrawMessageAsTiles(
					(unit->inventory[selection] & INVENTORY_ITEM_ID_MASK) +
						(s32)MsgItemName,
					commandWindow, 24, 0);
				command->range = cost;
				sySndPlayStub(SE_MENU_CONFIRM);
				choice = mnBattleTargetRunSelection(
					actorId, ability->tgtMode, cost,
					mnBattleClassifyEntryKind(ability));
				state->iconShown[1] = 0;
				syVramCacheFreeEntry(handle);
				mnWindowClose(commandWindow, 1);
				if(choice == -1)
					goto item_menu;
				commandKind = COMMAND_ITEM;
				target = choice;
				break;
			}

			case COMMAND_ICON_DEFEND:
				commandKind = COMMAND_DEFEND;
				break;
			}

			sySndPlayStub(SE_CHIME);
			command->unitId = actorId;
			{
				s32 priority = unit->agility;

				if(priority != 0)
					priority += (u32)(unit->agility * syRandom()) >> 20;
				command->pri = priority;
				if(commandIndex != 0 &&
				   actors[commandIndex] == actors[commandIndex - 1])
					command->pri = (s16)priority / 2;
			}
			/* A priority past 32767 reads negative. */
			if(command->pri < 0)
				command->pri = OVERFLOWED_PRIORITY;
			command->command = commandKind;
			command->prm = parameter;
			command->tgt = target;
			commandIndex++;
			if(commandIndex < commandCount)
				continue;
			break;
		}
		if(commandIndex < commandCount)
			continue;
		break;
	}

	if(state->linkTmr != 0) {
		u16 *signature = (u16 *)SerialTransfer.payload;

		signature[4] = 'E';
		signature[5] = 'D';
		signature[6] = 'V';
		signature[7] = 'S';
	}
	if(state->tmrWin != NULL)
		mnWindowClose(state->tmrWin, 1);
	syVramCacheFreeEntry(smallIconSlot);
	syVramCacheFreeEntry(largeIconSlot);
	syCloseEvent(mnBattleMenuUpdateEachFrame);
	if(state->linkTmr != 0) {
		BATTLE_STATE *link;

		void mnLinkDrawWaitingForOpponent(void) {
			u16 text[64];

			BATTLE_MENU_MESSAGE_COPY((s32)MsgWaitingForOpponent, text,
									 BATTLE_MENU_MESSAGE_CAPACITY);
			mnTextDrawWideStringAtOffset(text, state->tmrWin, 0, 4);
		}

		/* The wait message shows only when the countdown never opened its */
		/* window, that is while the partner is still choosing; a timer window
		 */
		/* was closed above and is only forgotten here. choice counts the frames
		 */
		/* without the link, and past LINK_MAX_MISSED_FRAMES gives up (-1). */
		link = Ram_HeapSlots->battleState;
		choice = 0;
		if(state->tmrWin == NULL && link->linkPaused == 0) {
			state->tmrWin =
				mnWindowCreate(0, 16, 30, 4,
							   WINDOW_FLAG_INSTANT | WINDOW_FLAG_TEXT_CANVAS |
								   WINDOW_FLAG_BORDERED_CANVAS);
			mnTextFillCanvasBackground();
			mnLinkDrawWaitingForOpponent();
		}
		else {
			state->tmrWin = NULL;
		}
		for(;;) {
			u16 *peerSignature = LinkPeerSignatures[link->linkSide ^ 1];

			if(link->linkPaused != 0) {
				commandCount = -1;
				break;
			}
			if((LinkStatusWork & LINK_STATUS_HEARD_BOTH) !=
			   LINK_STATUS_HEARD_BOTH) {
				choice++;
				if(choice > LINK_MAX_MISSED_FRAMES) {
					commandCount = -1;
					break;
				}
			}
			else {
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
	if(commandCount >= 0)
		commandCount += forcedCount;
	return commandCount;
}

/* The icon menus' bottom row, in tiles, where most of them stand; a Yes or No
 */
/* may stand at the top, row 0, instead. */
#define ICON_MENU_BOTTOM_ROW 17
/* The lines an icon row covers: its label window's three tiles. */
#define ICON_ROW_LINES 24

/* How far the selected icon overhangs its place while it pulses at the 8.8 */
/* `scale`: on each side, half its 24-pixel width grown by the scale, rounded */
/* up; and past its row, half its 32-pixel height grown by the scale, rounded,
 */
/* and a line more. */
#define PULSE_OVERHANG_X(scale) ((((scale) - Q_8_8(1)) * 12 + 255) / 256)
#define PULSE_OVERHANG_LINES(scale)                                            \
	((((scale) - Q_8_8(1)) * 32 + 255) / 512 + 1)

/* Draws the icon menu's icons, a task of the icon menus (mnIconMenuOpen). Each
 */
/* icon is a 32x32, 256-colour sprite, and one at x 0 is not drawn; the */
/* selected one pulses, double-size, between 1.19 and 1.31 times its size, in */
/* front of the others. While a screen transition runs, it widens window 1 over
 */
/* the icon row so the icons show through: from the first icon on the row's */
/* lines, keeping each line's right edge, and around the selected icon on the */
/* lines it overhangs, below a row at the top or above one at the bottom. */
void mnIconMenuDrawIcons(void) {
	ICON_MENU_STATE *iconMenuState;
	ICON_MENU_ENTRY *entry;
	AFFINE_TRANSFORM transform;
	s32 pulseScale;
	s32 affineIndex;
	s32 span;
	s32 spriteY;
	u32 lastLine;
	u32 index;
	/* UB: edge steps from one line's window 1 edges to the next line's, past */
	/* the member it points into. */
	u16 *edge;
	SCR_TRANSITION_STATE *state;
	WINDOW_BUFFER *page;
	s32 halfWidth;

	iconMenuState = Ram_HeapSlots->iconMenu;
	entry = iconMenuState->entries;
	pulseScale = WindowSprite_BounceScales[(FrameCount * 2) & 31];
	state = Ram_HeapSlots->scrTransition;
	pulseScale = (pulseScale - Q_8_8(1)) / 4 + Q_8_8(1.1875);
	transform.scaleX = pulseScale;
	transform.scaleY = pulseScale;
	transform.ang = 0;
	affineIndex = syOamMatrixAllocFromTransform(&transform);
	/* Pulse only the selection; the other icons keep their normal OAM shape. */
	for(index = 0; index < iconMenuState->cnt; index++, entry++) {
		u32 *word;
		s32 drawLayer;
		s32 x;

		x = entry->x;
		if(x == 0)
			continue;
		if(index == iconMenuState->selection) {
			s32 left = x + pulseScale * 7 / 512 - 20;

			/* The icon grows about the middle of its 64-pixel double-size box.
			 * On */
			/* the top row the box starts above the screen, its y wrapped within
			 */
			/* OAM's 8 bits, and the icon's top edge stays at the screen's top.
			 */
			if(entry->y != 0)
				spriteY = entry->y + pulseScale * 3 / 256 - 20;
			else
				spriteY = (pulseScale * 15 / 256 - 30) & OBJ_Y_MASK;
			word = &entry->link;
			*word++ = 0;
			*word++ = OBJ_AFFINE_MATRIX(affineIndex) | spriteY | OBJ_X(left) |
					  OBJ_SIZE_32X32 | OBJ_256_COLORS | OBJ_DOUBLE_SIZE |
					  OBJ_AFFINE;
			*word = VramCacheEntries[entry->slot].ofs >> 5;
			drawLayer = OAM_DEPTH_WINDOW + 6;
		}
		else {
			u32 tileOffset;
			u32 attributes;

			word = &entry->link;
			spriteY = entry->y;
			*word++ = 0;
			attributes = spriteY | OBJ_X(x) | OBJ_SIZE_32X32 | OBJ_256_COLORS;
			tileOffset = VramCacheEntries[entry->slot].ofs;
			*word++ = attributes;
			*word = tileOffset >> 5;
			drawLayer = OAM_DEPTH_WINDOW + 5;
		}
		syOamAddSprAtDepth(entry, drawLayer);
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
		index = ICON_ROW_LINES;
		if(index < lastLine) {
			do {
				*edge = (*edge & WIN_RANGE_END_MASK) | span;
				index++;
				edge += 2;
			} while(index < lastLine);
		}
		span = iconMenuState->entries[0].x;
		if(iconMenuState->selection == 0)
			span -= PULSE_OVERHANG_X(pulseScale);
		span <<= 8;
		edge = &page->lines[0].win1;
		for(index = 0; index < ICON_ROW_LINES; index++) {
			*edge = (*edge & WIN_RANGE_END_MASK) | span;
			edge += 2;
		}
	}
	else {
		s32 firstLine;

		span = iconMenuState->entries[iconMenuState->selection].x;
		halfWidth = PULSE_OVERHANG_X(pulseScale);
		span = ((span - halfWidth) << 8) + (span + halfWidth) + 23;
		firstLine = iconMenuState->entries[iconMenuState->selection].y;
		firstLine -= PULSE_OVERHANG_LINES(pulseScale);
		edge = &page->lines[firstLine].win1;
		index = firstLine;
		if(index < ICON_MENU_BOTTOM_ROW * 8) {
			do {
				*edge = (*edge & WIN_RANGE_END_MASK) | span;
				index++;
				edge += 2;
			} while(index < ICON_MENU_BOTTOM_ROW * 8);
		}
		span = iconMenuState->entries[0].x;
		if(iconMenuState->selection == 0)
			span -= PULSE_OVERHANG_X(pulseScale);
		span <<= 8;
		edge = &page->lines[ICON_MENU_BOTTOM_ROW * 8].win1;
		for(index = ICON_MENU_BOTTOM_ROW * 8; index < DISPLAY_HEIGHT; index++) {
			/* firstLine, done with, carries the mask that keeps the right edge.
			 */
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

/* Allocates and clears the icon menu's state, which lives in */
/* HEAP_SLOT_ICON_MENU until mnIconMenuClose frees it, and starts the task that
 */
/* draws its icons. Every icon menu starts here. */
void *mnIconMenuOpen(void) {
	void *state =
		syHeapSlotAllocEwram(HEAP_SLOT_ICON_MENU, sizeof(ICON_MENU_STATE));

	btDmaFill32(0, state, sizeof(ICON_MENU_STATE));
	sySetEvent((TASK_FUNC)mnIconMenuDrawIcons, TASK_PASS_FRAME - 10);
	return state;
}

/* Stops drawing the icons, closes the label window and releases each loaded */
/* icon before freeing the icon menu's state (HEAP_SLOT_ICON_MENU), then waits
 */
/* a frame. */
void mnIconMenuClose(void) {
	WINDOW *labelWindow;
	s32 i;
	ICON_MENU_STATE *state;

	state = Ram_HeapSlots->iconMenu;
	syCloseEvent(mnIconMenuDrawIcons);
	labelWindow = state->win;
	if(labelWindow != 0) {
		mnWindowClose(labelWindow, 2);
	}
	for(i = 0; i < state->cnt; i++) {
		syVramCacheFreeEntry(state->entries[i].slot);
	}
	syHeapSlotRelease(HEAP_SLOT_ICON_MENU);
	sySynch(1U);
}

/* Selects an icon position with wrapped directional input. A returns the */
/* position; B or Start returns -1. Redraws the label only after a move: the */
/* message labelBase + position when the layout set a labelBase, else the */
/* icon's command name. */
s32 mnIconMenuRun(s32 initial) {
	ICON_MENU_STATE *state = Ram_HeapSlots->iconMenu;
	s32 resource;

	state->selection = initial;
redraw:
	mnWindowSprPrepareForRedraw(state->win);
	if(state->labelBase != 0)
		resource = state->labelBase + state->selection;
	else
		resource = state->commandIcons[state->selection] + (s32)MsgCommandName;
	mnTextDrawMessageAsTiles(resource, state->win, 0, 0);
	for(;;) {
		sySynch(1);
		if(InputWork.pressed & KEY_A) {
			sySndPlayStub(SE_MENU_CONFIRM);
			return state->selection;
		}
		if(InputWork.pressed & KEY_B) {
			sySndPlayStub(SE_MENU_CANCEL);
			return -1;
		}
		if(InputWork.pressed & KEY_START) {
			sySndPlayStub(SE_MENU_CANCEL);
			return -1;
		}
		if((InputWork.repeat & KEY_LEFT) || (InputWork.repeat & KEY_UP)) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			state->selection--;
			if(state->selection < 0)
				state->selection = state->cnt - 1;
		}
		else if((InputWork.repeat & KEY_RIGHT) ||
				(InputWork.repeat & KEY_DOWN)) {
			sySndPlayStub(SE_MENU_CURSOR_MOVE);
			state->selection++;
			if(state->selection >= state->cnt)
				state->selection = 0;
		}
		else
			continue;
		goto redraw;
	}
}

/* Returns the distance between lhs and rhs, reusing their supplied difference.
 */
static inline s32 AbsoluteDifference(s32 diff, s32 lhs, s32 rhs) {
	if(diff >= 0)
		return diff;
	return rhs - lhs;
}

/* Moves the selection from start to goal one icon at a time with the cursor */
/* sound, redrawing the label each step and waiting */
/* Menu_SelectionStepDelays[icons still to go] frames at each (12 more at the */
/* first), then 48 frames on the goal before the confirm sound: a menu choice */
/* made for the player to watch (mnAnimateSelectionToEntry). Returns goal. */
s32 mnIconMenuAnimateTo(s32 start, s32 goal) {
	s16 resourceBase;
	s16 selection;
	s32 diff;
	s32 resourceId;
	s32 distance;
	s32 position;
	s32 step;
	s32 delay;
	const u8 *stepDelays;
	ICON_MENU_STATE *state;

	state = Ram_HeapSlots->iconMenu;
	step = 1;
	delay = 12;
	state->selection = (s16)start;
	if(goal < start)
		step = -1;
	position = start;

	for(;;) {
		mnWindowSprPrepareForRedraw(state->win);
		resourceBase = state->labelBase;
		if(resourceBase != 0) {
			resourceId = resourceBase + state->selection;
		}
		else {
			resourceId =
				state->commandIcons[state->selection] + (s32)MsgCommandName;
		}
		mnTextDrawMessageAsTiles(resourceId, state->win, 0, 0);

		selection = state->selection;
		stepDelays = Menu_SelectionStepDelays;
		diff = selection - goal;
		distance = AbsoluteDifference(diff, selection, goal);
		sySynch(stepDelays[distance] + delay);

		if(position == goal)
			break;

		state->selection += step;
		sySndPlayStub(SE_MENU_CURSOR_MOVE);
		delay = 0;
		position += step;
	}

	sySynch(48);
	sySndPlayStub(SE_MENU_CONFIRM);
	return goal;
}

/* Decodes command icon `index` into the VRAM cache entry `slot`, through a */
/* block it allocates and frees. */
void mnCommandIconLoadIntoSlot(s32 slot, s32 index) {
	s32 size = COMMAND_ICON_BYTES;
	void *buffer = syHeapAlloc(size);
	u16 *offsetTable = syResourceGetTableEntry(RESOURCE_COMMAND_ICONS);

	syDecompressByteLz((void *)((u32)offsetTable + offsetTable[index]), buffer);
	syVramCacheLoad(slot, size, buffer);
	syHeapRelease(buffer);
}

/* Appends a command icon while the menu has room for one (ICON_MENU_STATE's six
 */
/* entries). It starts on the bottom row; mnIconMenuCenter or */
/* mnIconMenuLayoutIcons places the row. Its VRAM cache entry is taken */
/* unchecked. */
void mnIconMenuAddIcon(s32 icon) {
	ICON_MENU_STATE *state;
	ICON_MENU_ENTRY *entry;
	s16 index;
	s32 slot;
	s32 y = ICON_MENU_BOTTOM_ROW * 8;

	state = Ram_HeapSlots->iconMenu;
	index = state->cnt;
	if(index < (s32)ARRAY_COUNT(state->entries)) {
		state->cnt++;
		entry = &state->entries[index];
		slot = syVramCacheFindFreeEntry();
		mnCommandIconLoadIntoSlot(slot, icon);
		entry->x = index * 24 + 32;
		entry->y = y;
		entry->slot = slot;
		state->commandIcons[index] = icon;
	}
}

/* Centres the icons and their label window together on tile row `row`. A */
/* nonzero resourceBase is the first label message; otherwise each icon is */
/* labelled by its command name. The icons stand three tiles apart and the */
/* label window, `width` + 2 tiles wide and 3 tall, follows them; the centring
 */
/* counts two thirds of the window's width. */
void mnIconMenuCenter(s32 row, s32 width, s32 resourceBase) {
	ICON_MENU_STATE *state = Ram_HeapSlots->iconMenu;
	s32 x;
	s32 i;
	s32 count;

	state->width = width + 2;
	state->labelBase = resourceBase;
	state->row = row;
	count = state->cnt;
	x = DISPLAY_TILE_WIDTH / 2 - (count * 3 + state->width * 2 / 3) / 2;
	for(i = 0; i < state->cnt; i++) {
		ICON_MENU_ENTRY *entry = &state->entries[i];

		entry->x = x * 8;
		entry->y = row * 8;
		x += 3;
	}
	state->win = mnWindowCreate(x, row, state->width, 3, WINDOW_FLAG_INSTANT);
}

/* Places the icons three tiles apart from tile (x, y), then opens their label
 */
/* window, `width` + 2 tiles wide, just after them. */
void mnIconMenuLayoutIcons(s32 x, s32 y, s32 width, s32 resourceBase) {
	ICON_MENU_STATE *state;
	s32 i;
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

/* Reads one entry of a field menu table (Menu_TopEntryPositionByCommand). */
static __inline__ s32 mnIconMenuReadCommandPos(s8 *table, s32 index) {
	return table[index];
}

/* The field menu's row of icons: Psynergy, Djinn, Item and Status, without */
/* Djinn when the party has none. `selection` is the command to start on and */
/* the result the command chosen (FIELD_MENU_COMMAND), or -1 when */
/* cancelled; two tables turn commands into icon positions and back. */
s32 mnFieldMenuSelectCommand(s32 selection) {
	s32 group = 0;
	s32 offset;
	s32 groupTriple;
	s32 result;
	s8 *commandPositions;

	if(gmPartySumDjinnCountsStub(-1) == 0) {
		group = 1;
	}

	groupTriple = group * 3;
	commandPositions = Menu_TopEntryPositionByCommand;
	offset = groupTriple << 1;
	selection =
		mnIconMenuReadCommandPos(commandPositions, selection + offset) - 1;
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
		s8 *commands = Menu_TopEntryCommandByPosition;
		s32 index;

		index = result + offset + 1;
		result = commands[index];
	}

	return result;
}

/* Opens the field command menu and animates from start to the command's icon.
 */
/* Returns the final icon position after closing the menu. */
s32 mnAnimateSelectionToEntry(s32 start, s32 command) {
	mnPartyStatOpenAndWait();
	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_PSYNERGY);
	mnIconMenuAddIcon(COMMAND_ICON_DJINN);
	mnIconMenuAddIcon(COMMAND_ICON_ITEM);
	mnIconMenuAddIcon(COMMAND_ICON_STATUS);
	/* Only Japanese centres the entries before selecting. */
#if LANGUAGE == LANGUAGE_JAPANESE
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, FIELD_MENU_ICON_ROW_WIDTH, 0);
#endif
	command = mnIconMenuAnimateTo(start, command - 1);
	mnIconMenuClose();
	mnPartyStatCloseWithMenu();
	return command;
}

/* The File Screen's row of icons, from the save summaries: New Game, */
/* Continue, Copy and Erase for one or two files (starting on Continue); no New
 */
/* Game or Copy with three; with every file cleared no Continue, and only Erase
 */
/* with three such files; then Battle and Update when they are enabled. */
/* Returns the action chosen (FILE_ACTION) by its position; with Battle */
/* off, the action after Erase is Update's. With no files it returns */
/* SAVE_ACTION_NEW_GAME at once, and -1 when cancelled or the backup memory */
/* cannot be read. */
s32 mnFileListSelectAction(void) {
	s32 fileState;
	s32 result;
	s32 initial;
	u32 group;

	group = 0;
	initial = 0;
	fileState = mnSaveScanFileSummaries();
	if(fileState < 0)
		return -1;
	if(fileState == 0)
		return SAVE_ACTION_NEW_GAME;
	if(fileState == SAVE_FILE_COUNT)
		group = 1;
	else if(fileState == SAVE_SCAN_ALL_CLEARED + SAVE_FILE_COUNT)
		group = 2;
	else if(fileState > SAVE_SCAN_ALL_CLEARED)
		group = 3;
	else
		initial = 1;
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
	result = mnIconMenuRun(initial);
	mnIconMenuClose();
	/* The icon position's action, from the group's row of six. */
	if(result >= 0) {
		u32 twice = group * 2;

		result = Menu_SaveSlotActionByPosition[result + (twice + group) * 2];
	}
	if(TitleBattleOptionEnabled == 0 && result == SAVE_ACTION_BATTLE)
		result = SAVE_ACTION_UPDATE;
	return result;
}

/* The label area is narrower in Japanese and wider in French. */
#if LANGUAGE == LANGUAGE_JAPANESE
#define LABEL_RIGHT 80
#elif LANGUAGE == LANGUAGE_FRENCH
#define LABEL_RIGHT 160
#else
#define LABEL_RIGHT 128
#endif

/* When the chosen password level changes, clears the lower window's text area
 */
/* (from x 8 to LABEL_RIGHT) and lists what a password of that level carries: */
/* Gold five lines, Silver three, Bronze two. */
void mnPasswordLevelMenuDrawContents(void) {
	ICON_MENU_STATE *state = Ram_HeapSlots->iconMenu;

	if(state->drawnSelection != state->selection) {
		state->drawnSelection = state->selection;
		mnWindowClearInteriorTiles(state->lowerWin, 8, 40, LABEL_RIGHT, 80);

		/* The lines of what each password carries, from "Levels" */
		/* (MsgPasswordLevel). */
		switch(state->selection) {
		case PASSWORD_LEVEL_GOLD: {
			s32 text = (s32)MsgPasswordLevel;

			mnTextDrawMessageAsTiles(text, state->lowerWin, 8, 40);
			mnTextDrawMessageAsTiles(text + 1, state->lowerWin, 8, 48);
			mnTextDrawMessageAsTiles(text + 2, state->lowerWin, 8, 56);
			mnTextDrawMessageAsTiles(text + 3, state->lowerWin, 8, 64);
			text += 4;
			mnTextDrawMessageAsTiles(text, state->lowerWin, 8, 72);
			break;
		}
		case PASSWORD_LEVEL_SILVER: {
			s32 text = (s32)MsgPasswordLevel;

			mnTextDrawMessageAsTiles(text, state->lowerWin, 8, 40);
			mnTextDrawMessageAsTiles(text + 1, state->lowerWin, 8, 48);
			text += 2;
			mnTextDrawMessageAsTiles(text, state->lowerWin, 8, 56);
			break;
		}
		case PASSWORD_LEVEL_BRONZE: {
			s32 text = (s32)MsgPasswordLevel;

			mnTextDrawMessageAsTiles(text++, state->lowerWin, 8, 40);
			mnTextDrawMessageAsTiles(text, state->lowerWin, 8, 48);
			break;
		}
		}
	}
}

/* The Spanish descriptions are indented. */
#if LANGUAGE == LANGUAGE_SPANISH
#define HELP_X 6
#else
#define HELP_X 0
#endif

/* Redraws the chosen transfer method's help in the lower window. Spanish, */
/* French and Italian describe two methods, the password and everything else */
/* as the cable; French clears the window with mnWindowSprClearList and */
/* redraws only when the description changes. */
void mnTransferMethodMenuDrawHelp(void) {
	ICON_MENU_STATE *state = Ram_HeapSlots->iconMenu;
	s16 *shown = &state->drawnSelection;
	s16 *current = &state->selection;

#if LANGUAGE == LANGUAGE_FRENCH
	if(*shown != *current) {
		if(*current == TRANSFER_METHOD_PASSWORD) {
			mnWindowSprClearList(state->lowerWin);
			mnTextDrawMessage((s32)MsgPasswordTransferHelp, state->lowerWin, 8,
							  4);
			mnTextDrawMessage((s32)MsgPasswordTransferHelp + 1, state->lowerWin,
							  0, 16);
		}
		else if(*shown == TRANSFER_METHOD_PASSWORD) {
			mnWindowSprClearList(state->lowerWin);
			mnTextDrawMessage((s32)MsgCableTransferHelp, state->lowerWin, 0, 4);
			mnTextDrawMessage((s32)MsgCableTransferHelp + 1, state->lowerWin, 0,
							  16);
			mnTextDrawMessage((s32)MsgCableTransferHelp + 2, state->lowerWin, 0,
							  28);
		}
		state->drawnSelection = state->selection;
	}
#else
	if(*shown != *current) {
		*shown = *current;
		mnWindowSprPrepareForRedraw(state->lowerWin);
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
		if(*current == TRANSFER_METHOD_PASSWORD) {
			mnTextDrawMessage((s32)MsgPasswordTransferHelp, state->lowerWin,
							  HELP_X, 4);
			mnTextDrawMessage((s32)MsgPasswordTransferHelp + 1, state->lowerWin,
							  HELP_X, 16);
		}
		else {
			mnTextDrawMessage((s32)MsgCableTransferHelp, state->lowerWin,
							  HELP_X, 4);
			mnTextDrawMessage((s32)MsgCableTransferHelp + 1, state->lowerWin,
							  HELP_X, 16);
			mnTextDrawMessage((s32)MsgCableTransferHelp + 2, state->lowerWin,
							  HELP_X, 28);
		}
#else
		switch(*current) {
		case TRANSFER_METHOD_PASSWORD:
			mnTextDrawMessage((s32)MsgPasswordTransferHelp, state->lowerWin, 0,
							  4);
			mnTextDrawMessage((s32)MsgPasswordTransferHelp + 1, state->lowerWin,
							  0, 16);
			break;
		case TRANSFER_METHOD_CABLE:
			mnTextDrawMessage((s32)MsgCableTransferHelp, state->lowerWin, 0, 4);
			mnTextDrawMessage((s32)MsgCableTransferHelp + 1, state->lowerWin, 0,
							  16);
			mnTextDrawMessage((s32)MsgCableTransferHelp + 2, state->lowerWin, 0,
							  28);
			break;
		default:
			mnTextDrawMessage((s32)MsgClearDataTransferHelp, state->lowerWin, 0,
							  4);
			mnTextDrawMessage((s32)MsgClearDataTransferHelp + 1,
							  state->lowerWin, 0, 16);
			break;
		}
#endif
	}
#endif
}

/* The entries' width and the windows' places and widths, in tiles, and where */
/* the lines of text start, in pixels, are fitted to each language's text. */
#if LANGUAGE == LANGUAGE_JAPANESE
#define ENTRY_WIDTH			 5
#define PASSWORD_UPPER_X	 9
#define PASSWORD_UPPER_WIDTH 13
#define PASSWORD_LOWER_X	 5
#define PASSWORD_LOWER_WIDTH 21
#define TRANSFER_UPPER_X	 9
#define TRANSFER_UPPER_WIDTH 13
#define TRANSFER_TITLE_X	 0
#define TRANSFER_LOWER_X	 5
#define TRANSFER_LOWER_WIDTH 21
#elif LANGUAGE == LANGUAGE_GERMAN
#define ENTRY_WIDTH			 7
#define PASSWORD_UPPER_X	 7
#define PASSWORD_UPPER_WIDTH 16
#define PASSWORD_LOWER_X	 4
#define PASSWORD_LOWER_WIDTH 22
#define TRANSFER_UPPER_X	 6
#define TRANSFER_UPPER_WIDTH 18
#define TRANSFER_TITLE_X	 6
#define TRANSFER_LOWER_X	 2
#define TRANSFER_LOWER_WIDTH 26
#elif LANGUAGE == LANGUAGE_SPANISH
#define ENTRY_WIDTH			 9
#define PASSWORD_UPPER_X	 5
#define PASSWORD_UPPER_WIDTH 21
#define PASSWORD_LOWER_X	 3
#define PASSWORD_LOWER_WIDTH 25
#define TRANSFER_UPPER_X	 4
#define TRANSFER_UPPER_WIDTH 22
#define TRANSFER_TITLE_X	 2
#define TRANSFER_LOWER_X	 1
#define TRANSFER_LOWER_WIDTH 28
#elif LANGUAGE == LANGUAGE_FRENCH
#define ENTRY_WIDTH			 9
#define PASSWORD_UPPER_X	 5
#define PASSWORD_UPPER_WIDTH 20
#define PASSWORD_LOWER_X	 3
#define PASSWORD_LOWER_WIDTH 25
#define TRANSFER_UPPER_X	 5
#define TRANSFER_UPPER_WIDTH 20
#define TRANSFER_TITLE_X	 2
#define TRANSFER_LOWER_X	 1
#define TRANSFER_LOWER_WIDTH 28
#elif LANGUAGE == LANGUAGE_ITALIAN
#define ENTRY_WIDTH			 7
#define PASSWORD_UPPER_X	 7
#define PASSWORD_UPPER_WIDTH 17
#define PASSWORD_LOWER_X	 3
#define PASSWORD_LOWER_WIDTH 25
#define TRANSFER_UPPER_X	 5
#define TRANSFER_UPPER_WIDTH 20
#define TRANSFER_TITLE_X	 2
#define TRANSFER_LOWER_X	 1
#define TRANSFER_LOWER_WIDTH 28
#else
#define ENTRY_WIDTH			 7
#define PASSWORD_UPPER_X	 8
#define PASSWORD_UPPER_WIDTH 14
#define PASSWORD_LOWER_X	 4
#define PASSWORD_LOWER_WIDTH 22
#define TRANSFER_UPPER_X	 6
#define TRANSFER_UPPER_WIDTH 18
#define TRANSFER_TITLE_X	 12
#define TRANSFER_LOWER_X	 5
#define TRANSFER_LOWER_WIDTH 21
#endif

/* Spanish, French and Italian indent the password's lines and set them */
/* closer together. */
#if LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_FRENCH ||             \
	LANGUAGE == LANGUAGE_ITALIAN
#define PASSWORD_TEXT_X		 8
#define PASSWORD_TEXT_Y		 0
#define PASSWORD_LINE_HEIGHT 11
#else
#define PASSWORD_TEXT_X		 0
#define PASSWORD_TEXT_Y		 4
#define PASSWORD_LINE_HEIGHT 12
#endif

/* Selects the transfer method or the password level (`mode`, enum */
/* SEND_OPTION_MENU), with a heading window above and an explanation of the */
/* selected icon below, redrawn by a task while the menu runs. Returns the */
/* place chosen, or -1 when cancelled. */
s32 mnIconMenuChooseSendOption(s32 mode) {
	ICON_MENU_STATE *state;
	WINDOW *window;
	s32 msg;
	WINDOW **upper;
	s32 result;

	mode &= 1;
	mnIconMenuOpen();
	state = Ram_HeapSlots->iconMenu;
	if(mode == SEND_OPTION_TRANSFER_METHOD) {
		mnIconMenuAddIcon(COMMAND_ICON_PASSWORD);
		mnIconMenuAddIcon(COMMAND_ICON_LINK);
	}
	else {
		mnIconMenuAddIcon(COMMAND_ICON_GOLD);
		mnIconMenuAddIcon(COMMAND_ICON_SILVER);
		mnIconMenuAddIcon(COMMAND_ICON_BRONZE);
	}
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, ENTRY_WIDTH, 0);
	if(mode != SEND_OPTION_TRANSFER_METHOD) {
		sySetEvent(mnPasswordLevelMenuDrawContents, TASK_PASS_FRAME - 10);
		state->drawnSelection = -1;
		window = mnWindowCreate(PASSWORD_UPPER_X, 0, PASSWORD_UPPER_WIDTH, 4,
								WINDOW_FLAG_INSTANT);
		msg = (s32)MsgPasswordEntry;
		upper = &state->upperWin;
		*upper = window;
		mnTextDrawMessage(msg, window, 0, 4);
		window = mnWindowCreate(PASSWORD_LOWER_X, 4, PASSWORD_LOWER_WIDTH, 12,
								WINDOW_FLAG_INSTANT);
		state->lowerWin = window;
		mnTextDrawMessage(msg + 1, window, PASSWORD_TEXT_X, PASSWORD_TEXT_Y);
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
	}
	else {
		sySetEvent(mnTransferMethodMenuDrawHelp, TASK_PASS_FRAME - 10);
		state->drawnSelection = -1;
		window = mnWindowCreate(TRANSFER_UPPER_X, 0, TRANSFER_UPPER_WIDTH, 4,
								WINDOW_FLAG_INSTANT);
		upper = &state->upperWin;
		*upper = window;
		mnTextDrawMessage((s32)MsgTransferMethod, window, TRANSFER_TITLE_X, 4);
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
 */
/* row, starting on position `selection`, and returns the position chosen. */
s32 mnSelectShopCommand(s32 selection) {
	s32 result;

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

/* Selects a Sanctum service (Revive, Cure Poison, Repel Evil, Remove Curse), */
/* centred on the bottom row, starting on position `selection`. */
s32 mnSelectSanctumService(s32 selection) {
	s32 result;

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
 */
/* stand on the bottom row when `bottom` is set, else at the top; `message` is
 */
/* the first label message, or 0 for the command names, and `labelWidth` the */
/* label window's width, 3 when 0. */
s32 mnYesNoMenuRun(s32 bottom, s32 message, s32 labelWidth, s32 selection) {
	s32 row;

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
	mnIconMenuCenter(row, labelWidth, message);

	selection = mnIconMenuRun(selection);
	mnIconMenuClose();
	if(selection == -1) {
		selection = NO;
	}
	return selection;
}

/* Selects Yes or No with the icons from tile (x, y), starting on */
/* `selection`; cancelling answers No. */
s32 mnYesNoMenuRunAt(s32 x, s32 y, s32 selection) {
	s32 message = MsgCommandYes;

	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_YES);
	mnIconMenuAddIcon(COMMAND_ICON_NO);
	mnIconMenuLayoutIcons(x, y, 3, message);
	selection = mnIconMenuRun(selection);
	mnIconMenuClose();
	if(selection == -1) {
		selection = NO;
	}
	return selection;
}

/* The label window's width for each language. */
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define PAIR_WIDTH 7
#elif (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define PAIR_WIDTH 10
#else
#define PAIR_WIDTH 9
#endif

/* Selects Coins or Lucky Medals, centred on the bottom row, starting on */
/* `selection`, and returns the position chosen. */
s32 mnSelectCoinsOrMedals(s32 selection) {
	s32 result;

	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_COINS);
	mnIconMenuAddIcon(COMMAND_ICON_LUCKY_MEDALS);
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, PAIR_WIDTH, 0);
	result = mnIconMenuRun(selection);
	mnIconMenuClose();
	return result;
}

/* Unused: starts the debug scene and entrance selector as a task. */
UNUSED void mnDebugSceneSelectSchedule(void) {
	sySetEvent(mnDebugSceneSelectRun, TASK_PASS_FRAME);
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
