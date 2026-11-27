/* A whole battle, btRun, and the lists of the units in it. Part of battle.c. */

#include "battle_internal.h"

/* A name letter past every edition's last (Z, the Japanese edition's 26th),
   which leaves an enemy's name bare, as The Lost Age names it. */
#define NAME_LETTER_PAST_ALL 26

/* The six enemy units, EnemyUnits, which gmUnitGet reads for unit ids 128 to
   133 while the battle lasts. */
#define ENEMY_UNITS_SIZE (ENEMY_BATTLE_SIZE * BATTLE_UNIT_SIZE)

/* The camera's zoom, in percent, from the battle's setup until the opening sets
   its own: the curtain's a tick later, or the Fusion Dragon's shattering
   opening's, so no tick shows it. */
#define BATTLE_ZOOM_BEFORE_OPENING 190

/* The camera's yaw as a battle starts, and as each turn's commands are chosen:
   about 56 degrees, a little past YAW_PARTY_ACTION. */
#define YAW_TURN_START (TRIG_TURN * 5 / 32)

/* Fights the battle against the enemy formation enemyGroup, from the opening to
   the end. Returns the number of enemies defeated when the party won or the
   link broke, BATTLE_OUTCOME_FLED or BATTLE_OUTCOME_LOST. */
Sint32 btRun(Sint32 enemyGroup)
{
	BATTLE_CAMERA			*camera;
	Sint32					*focus;
	BATTLE_STATE			*battle;
	Uint8					*enemyUnits;
	BATTLE_BACKGROUND_STATE *view;
	BATTLE_MESSAGE_STATE	*display;
	Sint32					 enemyCount;
	Uint8					*enemyUnitDest;
	Sint32					 actionCount;
	Sint32					 i;
	Sint32					 result;
	WINDOW					*tutorialWindow;
	Sint32					 linkWaitTicks;
	Uint32					 copyIndex;
	Uint32					 actingUnit;
	Sint16					 sound;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Uint8	unused[48];
	volatile Uint32 fill;

	/* The battle's state, all in EWRAM. */
	camera = syHeapSlotAllocateEWRAM(HEAP_SLOT_CAMERA, sizeof(BATTLE_CAMERA));
	battle = syHeapSlotAllocateEWRAM(HEAP_SLOT_BATTLE, sizeof(BATTLE_STATE));
	enemyUnits =
		syHeapSlotAllocateEWRAM(HEAP_SLOT_ENEMY_UNITS, ENEMY_UNITS_SIZE);
	view = syHeapSlotAllocateEWRAM(
		HEAP_SLOT_BATTLE_BACKGROUND, sizeof(BATTLE_BACKGROUND_STATE));
	syHeapSlotAllocateEWRAM(HEAP_SLOT_BATTLE_UNUSED, 640);
	focus = camera->focus;
	result = 0;
	syWordsClear(enemyUnits, ENEMY_UNITS_SIZE);
	syInitEvent();
	/* The opening transition owns the background until it is over. */
	view->ticks = 0;
	view->targetYaw = YAW_PARTY_ACTION;
	view->keepRange = 1;
	view->unusedAfterKeepRange[0] = 0;
	view->unusedAfterKeepRange[1] = 0;
	REG_DISPCNT = DISPCNT_MODE_1;
	gmFlagSetStub(FLAG_IN_BATTLE);
	/* XXX (no evidence): what flag 0x169 marks; it is set as every battle
	   starts, as The Lost Age's is, and no code of either game reads or clears
	   it, so nothing the game does shows what it means. */
	gmFlagSetStub(0x169);
	syInitMatrix();

	DMA_FILL32_CELL(fill, 0, camera, sizeof(BATTLE_CAMERA));
	DMA_FILL32_CELL(fill, 0, battle, sizeof(BATTLE_STATE));
	battle->arrowTiles = -1;
	battle->formation = enemyGroup;
	display = syHeapSlotAllocateEWRAM(
		HEAP_SLOT_BATTLE_MESSAGE, sizeof(BATTLE_MESSAGE_STATE));
	DMA_FILL32_CELL(fill, 0, display, sizeof(BATTLE_MESSAGE_STATE));
	battle->bg = fdBattleBackgroundGetSelectedStub();
	syHeapSlotAllocateEWRAM(HEAP_SLOT_SPRITES, 3584);
	syHeapSlotAllocateEWRAM(HEAP_SLOT_SPRITE_ANIMATIONS, 1536);
	fdActorSystemInitStub(ACTOR_SYSTEM_BATTLE);
	if(gmFlagGetStub(FLAG_DJINN_LESSON) != 0)
		mnWindowSystemInitBattleStub(1);
	else
		mnWindowSystemInitBattleStub(0);

	/* The camera's starting view, then the enemies. */
	SET_VECTOR(focus, 0, toFIXED(64), 0);
	SET_VECTOR(camera->eye, 0, toFIXED(180), toFIXED(64));
	camera->yaw = YAW_TURN_START;
	camera->pitch = TRIG_TURN * 5 / 16;
	camera->distance = toFIXED(256);
	enemyCount = btFormationBuildEnemyList(battle->formation);

	/* A link battle: the opponent's party, which the Battle Arena left in
	   ActorSpriteSlots, becomes the enemies, and both consoles start from seed
	   0 so that their rolls agree. */
	if(gmFlagGetStub(FLAG_LINK_BATTLE) != 0) {
		battle->isLink = 1;
		SET_BATTLE_TYPE(BATTLE_TYPE_LINK);
	}
	if(battle->isLink != 0) {
		SET_RANDOM_SEED(0);
		for(linkWaitTicks = 0;;) {
			if((LinkStatusWork & LINK_BOTH_CONNECTED) == LINK_BOTH_CONNECTED)
				break;
			sySynch(1);
			linkWaitTicks++;
			if(linkWaitTicks > LINK_MAX_MISSED_TICKS) {
				battle->linkPaused = 1;
				break;
			}
		}
		battle->linkSide = SIO_MULTI_PLAYER_ID();
		enemyUnitDest = EnemyUnits;
		for(copyIndex = 0; copyIndex < ENEMY_UNITS_SIZE; copyIndex++)
			*enemyUnitDest++ = ActorSpriteSlots[copyIndex];
		enemyCount = gmFlagGetByteStub(FLAG_LINK_PARTY_COUNT);
		btPartyAssignOpponentSlots();
		battle->layout = FORMATION_LAYOUT_SHUFFLED;
	}
	sySetEvent(btCameraUpdate, EVENT_PASS_TICK - 1);

	/* The battle music: the music an event or encounter set for this battle.
	   Every battle piece plays on the music's one player (MUSIC_PLAYER_BGM),
	   where the piece started last replaces the one before within the tick: a
	   link battle hears Test Your Skills, and a battle with none set hears
	   Colosseum Finals, never Sworn Enemies Stand in Your Way. */
	sound = MainRoutineStateWork.battleMusic;
	if(sound != 0) {
		sySoundPlayStub(sound);
		if(gmFlagGetStub(FLAG_LINK_BATTLE) != 0) {
			sySoundPlayStub(MUS_TEST_YOUR_SKILLS);
			sySoundLoadReverbCode(SOUND_REVERB_NONE);
		}
	} else {
		sySoundPlayStub(MUS_SWORN_ENEMIES_STAND_IN_YOUR_WAY);
		sySoundPlayStub(MUS_COLOSSEUM_FINALS);
	}

	/* The units, the windows and the background. */
	btUnitCollect();
	btUnitRefreshPlacement();
	btDjinnRecoveryReconcile();
	btSummonUpdateAvailability();
	if(gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_PARTY)->summonMask != 0)
		battle->partyStatus = PARTY_STATUS_SUMMONS;
	else
		battle->partyStatus = PARTY_STATUS_NO_SUMMONS;
	mnPartyStatusCreateWindowStub(PARTY_STATUS_CLOSED);
	btCameraSetDefaultView();
	btObjectPut();
	btBackgroundInit();
	btBackgroundLoad(BATTLE_BACKGROUND_FIXED, battle->bg, 0);
	btCameraSetRange(toFIXED(160), toFIXED(80), 0, 0, toFIXED(2));
	btCameraSetView(0, 0, 0, BATTLE_ZOOM_BEFORE_OPENING);
	btPartyBattleSetupNoOperation(1);
	REG_BLDCNT = 0;
	btSpoilsClear();
	/* The advance arrow's tiles, taken again each turn. */
	battle->arrowTiles = syVRAMCacheAllocateEntry(128);

	/* A random encounter may open with a first strike (1 in 16) or a surprise
	   (1 in 32 of the rest); the Djinn tutorial always strikes first. */
	battle->firstStrike = FIRST_STRIKE_NONE;
	if(gmFlagGetStub(FLAG_DJINN_LESSON) != 0) {
		battle->firstStrike = FIRST_STRIKE_PARTY;
	} else if(MainRoutineStateWork.battleType == BATTLE_TYPE_NORMAL) {
		if((gmBattleRandomStub() & 15) == 0)
			battle->firstStrike = FIRST_STRIKE_PARTY;
		else if((gmBattleRandomStub() & 31) == 0)
			battle->firstStrike = FIRST_STRIKE_ENEMIES;
	}
	/* After the entry transition the camera event owns the background again,
	   and the sprite priorities are sorted every tick. */
	btTransitionRun(enemyCount, enemyGroup);
	view->keepRange = 0;
	CpuLoadDisplayEnabled = 0;
	sySetEvent(btObjectUpdateSpritePriorities, EVENT_PASS_TICK);

	/* The turns. */
	for(;;) {
		btTurnStartNoOperation();
		btSummonUpdateAvailability();
		if(gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_PARTY)->summonMask != 0)
			battle->partyStatus = PARTY_STATUS_SUMMONS;
		else
			battle->partyStatus = PARTY_STATUS_NO_SUMMONS;
		/* The camera eases back to its starting yaw over 60 ticks, and the
		   party status window is drawn with or without its Djinn column. */
		view->targetYaw = YAW_TURN_START;
		view->ticks = 60;
		mnPartyStatusDrawStub(battle->partyStatus);

		/* Every unit's action for the turn, in action order. */
		syWordsClear(battle->actions, sizeof(battle->actions));
		syVRAMCacheFreeEntry(battle->arrowTiles);
		if(gmFlagGetStub(FLAG_AUTO_BATTLE) == 0) {
			/* DEAD: the heap's free IWRAM and EWRAM measured and dropped,
			   before and after the commands are chosen and around each action;
			   the ROM keeps the calls. */
			syHeapGetRemainingIWRAM();
			syHeapGetRemainingEWRAM();
			actionCount = btGatherActions(battle->actions);
			syHeapGetRemainingIWRAM();
			syHeapGetRemainingEWRAM();
		} else {
			actionCount = btAutoBattleBuildActions(battle->actions);
		}
		battle->arrowTiles = syVRAMCacheAllocateEntry(128);
		mnPartyStatusDrawStub(battle->partyStatus);
		if(actionCount < 0)
			goto aborted;

		/* Each action, until one side is down or the party runs. The turn's
		   first action waits 10 ticks facing the party first. */
		for(i = 0; i < actionCount; i++) {
			actingUnit = battle->actions[i].unitId;
			syHeapGetRemainingIWRAM();
			syHeapGetRemainingEWRAM();
			if(gmFlagGetStub(FLAG_AUTO_BATTLE) == 0) {
				if(btStagingDispatchAction(
					   &battle->actions[i], i == 0 ? 10 : 0) == 1)
					goto fled;
			} else {
				if(btAutoBattleRunAction(&battle->actions[i]) == 1)
					goto fled;
			}
			syHeapGetRemainingIWRAM();
			syHeapGetRemainingEWRAM();
			if(btUnitListLiving(BATTLE_SIDE_MASK_PARTY, 0) == 0)
				goto party_lost;
			if(btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, 0) == 0) {
				if(actingUnit <= ALLY_MAX && battle->spoils.defeated == 1)
					battle->defeatState = DEFEAT_BY_PARTY_MEMBER;
				goto won;
			}
			if(btLinkBattleSyncTurn() < 0)
				goto aborted;
		}

		/* The end of the turn. */
		battle->firstStrike = FIRST_STRIKE_NONE;
		btTurnEndNoOperation();
		btProcessTurnEnd();
		btObjectDestroyAll();
		if(battle->isLink != 0) {
			if(btLinkBattleSyncTurn() < 0)
				goto aborted;
		} else {
			sySynch(20);
		}
		if(gmFlagGetStub(FLAG_DJINN_LESSON) == 0)
			continue;

		/* The Djinn lesson's two lines after each turn. "See! When you
		   unleashed me, I attacked with my mighty stone-cleaver!" */
		tutorialWindow = mnMessageOpenWindowStub(
			MsgTutorialUnleashed, 0, 4, MESSAGE_WINDOW_ZOOM);
		while(mnMessageAllTypedStub() == 0)
			sySynch(1);
		mnWindowCloseStub(tutorialWindow, 1);
		sySynch(1);
		/* "After being unleashed, we Djinn will then stand by to be
		   summoned..." */
		tutorialWindow = mnMessageOpenWindowStub(
			MsgTutorialStandby, 10, 4, MESSAGE_WINDOW_ZOOM);
		btMessageWaitForAdvanceAt(92, 24);
		mnWindowCloseStub(tutorialWindow, 1);
		sySynch(1);
	}

won:
	/* "X has been downed!" or "X's party goes down!", named after the first
	   enemy to fall, with no letter, then the spoils. */
	btClearSerialTransferPayload();
	if(gmFlagGetStub(FLAG_DJINN_LESSON) == 0) {
		if(battle->isLink != 0)
			sySoundPlayStub(MUS_VICTORY);
		if(battle->spoils.defeated != 0) {
			sySoundPlayStub(MUS_VICTORY);
			if(battle->defeatState <= DEFEAT_SEVERAL) {
				gmBattleUnitSetStub(FIRST_ENEMY_UNIT,
					battle->enemyKinds[battle->firstDefeated],
					NAME_LETTER_PAST_ALL);
				mnMessageArgumentClearAllStub();
				mnMessageArgumentPushStub(
					FIRST_ENEMY_UNIT, MESSAGE_ARGUMENT_ACTOR);
				mnBattleMessageShowAndWaitStub(battle->defeatState + MsgDowned);
				btMessageWaitForAdvance();
			}
		}
		btAwardSpoils();
	}
	sySoundPlayStub(SOUND_FADE_OUT_BGM);
	syScreenFadeToBlack(30);
	syScreenFadeWait();
	result = battle->spoils.defeated;
	goto finished;

aborted:
	/* The link broke, or no actions could be gathered. */
	btClearSerialTransferPayload();
	sySuspendEvent(0);
	REG_DISPCNT = DISPCNT_MODE_1;
	result = battle->spoils.defeated;
	gmFlagSetStub(FLAG_AREA_TEMP_E8);
	goto finished;

party_lost:
	/* "X goes down in defeat..." for a lone party member, else "X's party has
	   been defeated!". */
	btClearSerialTransferPayload();
	sySoundPlayStub(MUS_DEFEATED);
	mnMessageArgumentClearAllStub();
	mnMessageArgumentPushStub(
		MainRoutineStateWork.party[0], MESSAGE_ARGUMENT_ACTOR);
	if(btPartyPrepareMembers(0) == 1)
		mnBattleMessageShowAndWaitStub(MsgGoesDownInDefeat);
	else
		mnBattleMessageShowAndWaitStub(MsgPartyDefeated);
	btMessageWaitForAdvance();
	sySoundPlayStub(SOUND_FADE_OUT_BGM);
	syScreenFadeToBlack(30);
	result = BATTLE_OUTCOME_LOST;
	syScreenFadeWait();
	goto finished;

fled:
	sySoundPlayStub(SOUND_FADE_OUT_BGM);
	syScreenFadeToBlack(30);
	syScreenFadeWait();
	result = BATTLE_OUTCOME_FLED;

finished:
	/* Every way out: Djinn recover one step and the next battle starts
	   normally. */
	btPartyClearBattleStatuses();
	btTurnEndNoOperation();
	btDjinnRecoveryCountDown();
	SET_BATTLE_TYPE(BATTLE_TYPE_NORMAL);
	syCloseEvent(btObjectUpdateSpritePriorities);
	btBackgroundFree();
	return result;
}

/* Lists the party members who fight, three in a link battle, into unitIds when
   it is not null, and returns how many. */
Sint32 btPartyPrepareMembers(Uint16 *unitIds)
{
	Sint32 limit;
	Sint32 count;
	Sint32 index;

	limit = PARTY_BATTLE_SIZE;
	if(BattleStateWork->isLink != 0)
		limit = LINK_PARTY_SIZE;

	count = gmPartyCountMembersStub();
	if(count > limit)
		count = limit;

	for(index = 0; index < count; index++) {
		Sint32 unitId = MainRoutineStateWork.party[index];

		if(unitIds != 0)
			*unitIds++ = unitId;
		gmUnitGetStub(unitId)->side = BATTLE_SIDE_PARTY;
	}

	if(unitIds != 0)
		*unitIds = BATTLE_UNIT_LIST_END;
	return count;
}

/* Lists the enemy units present and returns how many, 0 when unitIds is
   null. */
Sint32 btEnemyList(Sint16 *unitIds)
{
	Sint16 *output = unitIds;
	Sint32	listed = 0;
	Sint32	enemyCapacity = ENEMY_BATTLE_SIZE;
	Sint32	count = 0;
	Sint32	unitId;

	if(output != NULL) {
		if(gmFlagGetStub(FLAG_LINK_BATTLE) != 0)
			enemyCapacity = LINK_PARTY_SIZE;
		for(unitId = FIRST_ENEMY_UNIT;
			unitId < FIRST_ENEMY_UNIT + enemyCapacity; unitId++) {
			if(gmUnitGetStub(unitId)->side != BATTLE_SIDE_NONE) {
				*output++ = unitId;
				listed++;
			}
		}
		*output = BATTLE_UNIT_LIST_END;
		count = listed;
	}
	return count;
}

/* Lists the units still standing on the sides in sideMask, party first, into
   unitIds when it is not null, and returns how many. */
Sint32 btUnitListLiving(Sint32 sideMask, Uint16 *unitIds)
{
	Uint16	activeMembers[8];
	Sint32	i;
	Uint16 *output;
	Sint32	livingCount;
	Sint32	enemyCapacity;
	Sint32	unitId;
	Sint16	hp;
	Sint32	activeCount;
	UNIT   *unit;

	output = unitIds;
	livingCount = 0;
	enemyCapacity = ENEMY_BATTLE_SIZE;
	if(gmFlagGetStub(FLAG_LINK_BATTLE) != 0) {
		enemyCapacity = LINK_PARTY_SIZE;
	}
	if(sideMask & BATTLE_SIDE_MASK_PARTY) {
		activeCount = btPartyPrepareMembers(activeMembers);
		for(i = 0; i < activeCount; i++) {
			unitId = activeMembers[i];
			hp = gmUnitGetStub(unitId)->hp;
			if(hp > 0) {
				if(output != NULL) {
					*output = unitId;
					output++;
				}
				livingCount++;
			}
		}
	}
	if(sideMask & BATTLE_SIDE_MASK_ENEMIES) {
		for(i = FIRST_ENEMY_UNIT; i < enemyCapacity + FIRST_ENEMY_UNIT; i++) {
			unit = gmUnitGetStub(i);
			if((unit->side != BATTLE_SIDE_NONE) && (unit->hp > 0)) {
				if(output != NULL) {
					*output = i;
					output++;
				}
				livingCount++;
			}
		}
	}
	if(output != NULL) {
		*output = BATTLE_UNIT_LIST_END;
	}
	return livingCount;
}

/* Lists the units in the battle on the sides in sideMask, standing or not, and
   returns how many. */
Sint32 btUnitListIds(Sint32 sideMask, Uint16 *unitIds)
{
	BATTLE_STATE *battle;
	Sint32		  count;
	Sint32		  i;

	battle = BattleStateWork;
	count = 0;
	if(sideMask & BATTLE_SIDE_MASK_PARTY) {
		for(i = 0; battle->partyUnits[i] != BATTLE_UNIT_LIST_END; i++) {
			if(battle->partyUnits[i] != BATTLE_UNIT_REMOVED) {
				if(unitIds != NULL)
					*unitIds++ = battle->partyUnits[i];
				count++;
			}
		}
	}
	if(sideMask & BATTLE_SIDE_MASK_ENEMIES) {
		for(i = 0; battle->enemyIds[i] != BATTLE_UNIT_LIST_END; i++) {
			if(battle->enemyIds[i] != BATTLE_UNIT_REMOVED) {
				if(unitIds != NULL)
					*unitIds++ = battle->enemyIds[i];
				count++;
			}
		}
	}
	if(unitIds != NULL)
		*unitIds = BATTLE_UNIT_LIST_END;
	return count;
}
