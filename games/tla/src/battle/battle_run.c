/* A whole battle, btRun, its tutorial helpers, and the lists of the units in
   it. Part of battle.c. */

#include "window/stubs.h"
#include "battle_effect/stubs.h"
#include "menu/djinn_menu.h"
#include "sound/sound_driver_symbols.h"

/* The six enemy units, which syUnitGet reads for unit ids 128 to 133 while the
   battle lasts (heap slot HEAP_SLOT_ENEMY_UNITS). */
#define ENEMY_UNITS_SIZE (ENEMY_BATTLE_SIZE * BATTLE_UNIT_SIZE)

/* The camera's zoom, in percent, from the battle's setup until the opening sets
   its own: the curtain's a tick later, or the Doom Dragon's shattering
   opening's, so no tick shows it. */
#define BATTLE_ZOOM_BEFORE_OPENING 190

/* With the front line down and someone living, sends the reserve members in to
   take the fallen ones' places, as many as there are of both. */
void btPartySwapInReserves(void)
{
	Uint16		  actors[6];
	Uint16		  reserves[6];
	Sint32		  actorCnt;
	Sint32		  reserveCnt;
	Sint32		  cnt;
	BATTLE_STATE *battle = Ram_HeapSlots->battleState;
	Sint32		  i;

	if(btUnitListLivingWithReserve(BATTLE_SIDE_MASK_PARTY, 0) == 0)
		return;
	cnt = actorCnt = btUnitListIds(BATTLE_SIDE_MASK_PARTY, actors);
	reserveCnt = btPartyPrepareReserveOwners(reserves);
	if(cnt > reserveCnt)
		cnt = reserveCnt;
	btHighlightSetModes(actors, HIGHLIGHT_FADE_OUT);
	sySynch(32);
	for(i = 0; i < cnt; i++) {
		Sint32		   unit = battle->partyUnits[i];
		Sint32		   id = reserves[i];
		BATTLE_OBJECT *from = btObjectGet(unit);
		BATTLE_OBJECT *to = btObjectGet(id);
		MOTION_OBJECT *object = to->obj;
		MOTION_OBJECT *place = from->obj;

		to->anchorX = from->anchorX;
		to->anchorZ = from->anchorZ;
		fdActorSetPosAndResetMotionStub(
			(void *)object, place->x, place->y, place->z);
		MainRoutineStateWork.party[i] = id;
		battle->partyUnits[i] = id;
	}
	for(i = 0; i < cnt; i++)
		MainRoutineStateWork.party[actorCnt + i] = actors[i];
	mnPartyStatDrawStub(Ram_HeapSlots->battleState->partyStat);
	btObjectPut();
	btHighlightSetModes(reserves, HIGHLIGHT_FADE_IN);
	sySynch(32);
}

/* Allocates the battle camera and sets its starting view. */
void btCameraCreate(void)
{
	BATTLE_CAMERA *camera =
		syHeapSlotAllocEWRAM(HEAP_SLOT_CAMERA, sizeof(BATTLE_CAMERA));
	Sint32 *focus = camera->focus;

	SET_VECTOR(focus, 0, toFIXED(64), 0);
	SET_VECTOR(camera->eye, 0, toFIXED(180), toFIXED(64));
	camera->yaw = TRIG_TURN * 5 / 32;
	camera->pitch = TRIG_TURN * 5 / 16;
	camera->distance = toFIXED(256);
}

/* Name letters past every edition's last, which leave an enemy's name bare:
   255, and NAME_LETTER_ALPHABET, past Z, the last letter the Japanese edition
   gives. */
#define NAME_LETTER_NONE	 255
#define NAME_LETTER_PAST_ALL NAME_LETTER_ALPHABET

/* Replaces the first enemy with the Doom Dragon's next form:
   ENEMY_DOOM_DRAGON_E after FORMATION_DOOM_DRAGON_A, else ENEMY_DOOM_DRAGON_H.
   The form change's battle effect calls it through its stub call. */
void btChangeEnemyForm(Sint32 id)
{
	BATTLE_STATE  *battle = Ram_HeapSlots->battleState;
	Sint32		   unit;
	BATTLE_OBJECT *battleObject;

	if(id == FORMATION_DOOM_DRAGON_A)
		unit = ENEMY_DOOM_DRAGON_E;
	else
		unit = ENEMY_DOOM_DRAGON_H;
	btObjectMarkForRelease(FIRST_ENEMY_UNIT);
	btMotionDestroyAllSlotObjects();
	btEnemyClaimNameLetter(unit, 1);
	gmBattleUnitSetStub(FIRST_ENEMY_UNIT, unit, NAME_LETTER_NONE);
	battle->enemyIds[0] = FIRST_ENEMY_UNIT;
	battle->enemyIds[1] = BATTLE_UNIT_LIST_END;
	btPlacementRelayoutEnemies();
	battleObject = btObjectGet(FIRST_ENEMY_UNIT);
	btObjectMake(battleObject, FIRST_ENEMY_UNIT,
		battleObject->anchorX / toFIXED(1), battleObject->anchorZ / toFIXED(1));
	btObjectPut();
}

/* Plays the Doom Dragon's form change effect for its formation value
   (FORMATION_DOOM_DRAGON_A to _C) on the first enemy. */
void btPlayFormChange(Sint32 value)
{
	BATTLE_PRESENTATION_STATE request;

	request.range = 0;
	request.actor = FIRST_ENEMY_UNIT;
	request.firstTgt = 0;
	request.cnt = 1;
	request.actors[0] = FIRST_ENEMY_UNIT;
	request.side = 0;
	request.kind = 0;
	request.variant = 0;
	btEffectRunFormChangeStub((BATTLE_PRESENTATION_STATE *)&request, value);
}

/* Returns the first of the Djinn tutorial's messages: the second set's
   (MsgDjinnGiveTutorialB) while FLAG_ALREADY_KNOWS_DJINN is set, as Up held
   when Echo's tutorial begins sets it. */
Sint32 btTutorialGetDjinnGiveMessage(void)
{
	if(syGameFlagGet(FLAG_ALREADY_KNOWS_DJINN))
		return (Sint32)MsgDjinnGiveTutorialB;
	return (Sint32)MsgDjinnGiveTutorialA;
}

void fdActorSystemInitStub(Sint32 mode);

/* Fights the battle against the enemy formation enemyGroup, from the opening to
   the end. Returns the number of enemies defeated when the party won or the
   link broke, BATTLE_OUTCOME_FLED or BATTLE_OUTCOME_LOST. */
Sint32 btRun(Sint32 enemyGroup)
{
	BATTLE_CAMERA			*camera;
	BATTLE_STATE			*battle;
	BATTLE_BACKGROUND_STATE *background;
	Sint32					 enemyCnt;
	Uint8					*enemyUnitDst;
	Sint32					 turn;
	Sint32					 actionCnt;
	Sint32					 i;
	Sint32					 result;
	Sint32					 win;
	Sint32					 linkWaitTicks;
	Uint32					 copyIdx;
	Uint32					 actingUnit;
	Sint16					 snd;

	camera = syHeapSlotAllocEWRAM(HEAP_SLOT_CAMERA, sizeof(BATTLE_CAMERA));
	battle = syHeapSlotAllocEWRAM(HEAP_SLOT_BATTLE, sizeof(BATTLE_STATE));
	syHeapSlotAllocEWRAM(HEAP_SLOT_ENEMY_UNITS, ENEMY_UNITS_SIZE);
	background = syHeapSlotAllocEWRAM(
		HEAP_SLOT_BATTLE_BACKGROUND, sizeof(BATTLE_BACKGROUND_STATE));
	syHeapSlotAllocEWRAM(HEAP_SLOT_BATTLE_UNUSED, 640);
	result = 0;
	syWordsClearEntry(Ram_HeapSlots->enemyUnits, ENEMY_UNITS_SIZE);
	syInitEvent();
	background->tgtYaw = YAW_PARTY_ACTION;
	background->ticks = 0;
	background->keepRange = 1;
	background->unusedAfterKeepRange[0] = 0;
	background->unusedAfterKeepRange[1] = 0;
	REG_DISPCNT = DISPCNT_MODE_1;
	syGameFlagSet(FLAG_IN_BATTLE);
	/* XXX (no evidence): what flag 0x169 marks; it is set as every battle
	   starts, as The Broken Seal's, and no code of either game reads it, so
	   nothing the game does shows what it means. */
	syGameFlagSet(0x169);
	syInitMatrix();

	IWRAM_FILL_WORDS(camera, sizeof(BATTLE_CAMERA), 0);
	syWordsFillEntry(battle, sizeof(BATTLE_STATE), 0);
	battle->arrowTiles = -1;
	battle->formation = enemyGroup;
	syHeapSlotAllocEWRAM(
		HEAP_SLOT_BATTLE_MESSAGE, sizeof(BATTLE_MESSAGE_STATE));
	IWRAM_FILL_WORDS(Ram_HeapSlots->battleMsg, sizeof(BATTLE_MESSAGE_STATE), 0);
	battle->bg = fdBattleBackgroundGetSelectedStub();
	syHeapSlotAllocEWRAM(HEAP_SLOT_SPRITES, 3584);
	syHeapSlotAllocEWRAM(HEAP_SLOT_SPRITE_ANIMATIONS, 1536);
	fdActorSystemInitStub(ACTOR_SYSTEM_BATTLE);
	/* The Djinn lesson's battle loads the glyph tiles too. */
	if(syGameFlagGet(FLAG_DJINN_LESSON) != 0)
		mnWindowSystemInitBattleStub(TRUE);
	else
		mnWindowSystemInitBattleStub(FALSE);

	btCameraCreate();
	enemyCnt = btFormationBuildEnemyList(battle->formation);

	if(syGameFlagGet(FLAG_LINK_BATTLE) != 0) {
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
		battle->linkSide = SIO_ID_OF(REG_SIOCNT);
		enemyUnitDst = Ram_HeapSlots->enemyUnits;
		for(copyIdx = 0; copyIdx < ENEMY_UNITS_SIZE; copyIdx++)
			*enemyUnitDst++ = ActorSpriteSlots[copyIdx];
		enemyCnt = syGameFlagGetByte(FLAG_AREA_TEMP_F0);
		btPartyAssignOpponentSlots();
		battle->layout = FORMATION_LAYOUT_SHUFFLED;
	}
	sySetEvent(btCameraUpdate, EVENT_PASS_TICK - 1);

	snd = MainRoutineStateWork.battleMusic;
	if(snd != 0) {
		sySndPlayStub(snd);
		if(syGameFlagGet(FLAG_LINK_BATTLE) != 0) {
			sySndPlayStub(MUS_TEST_YOUR_SKILLS);
			sySndLoadReverbCode(SOUND_REVERB_NONE);
		}
	} else {
		sySndPlayStub(MUS_BATTLE_THE_LOST_AGE);
	}

	btUnitCollect();
	btUnitRefreshPlacement();
	btDjinnRecoveryReconcile();
	btSummonUpdateAvailability();
	if((Uint16)gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_PARTY)->summonMask !=
		0)
		battle->partyStat = PARTY_STATUS_SUMMONS;
	else
		battle->partyStat = PARTY_STATUS_NO_SUMMONS;
	if(enemyGroup == FORMATION_KARST_AND_AGATIO_B ||
		enemyGroup == FORMATION_KARST_AND_AGATIO_C)
		btScriptedBattleRestoreUnits();
	mnPartyStatCreateWindowStub(PARTY_STATUS_CLOSED);
	btCameraSetDefaultView();
	btObjectPut();
	btBackgroundInit();
	btBackgroundLoad(BATTLE_BACKGROUND_FIXED, battle->bg, 0);
	btCameraSetRange(toFIXED(160), toFIXED(80), 0, 0, toFIXED(2));
	btCameraSetView(0, 0, 0, BATTLE_ZOOM_BEFORE_OPENING);
	btPartyBattleSetupNoOperation(1);
	REG_BLDCNT = 0;
	btSpoilsClear();
	battle->arrowTiles = syVRAMCacheAllocEntry(128);

	battle->firstStrike = FIRST_STRIKE_NONE;
	if(syGameFlagGet(FLAG_DJINN_LESSON) != 0) {
		battle->firstStrike = FIRST_STRIKE_PARTY;
	} else if(MainRoutineStateWork.battleType == BATTLE_TYPE_NORMAL) {
		if((gmBattleRandomStub() & 15) == 0)
			battle->firstStrike = FIRST_STRIKE_PARTY;
		else if((gmBattleRandomStub() & 31) == 0)
			battle->firstStrike = FIRST_STRIKE_ENEMIES;
	} else if(MainRoutineStateWork.battleType == BATTLE_TYPE_LOST) {
		battle->firstStrike = FIRST_STRIKE_ENEMIES;
	}
	btTransitionRun(enemyCnt, enemyGroup);
	background->keepRange = 0;
	CpuLoadDisplayEnabled = 0;
	sySetEvent(btObjectUpdateSprPriorities, EVENT_PASS_TICK);

	for(turn = 1;; turn++) {
		if(btUnitListLivingIds(BATTLE_SIDE_MASK_PARTY, 0) == 0)
			btPartySwapInReserves();
		btTurnStartNoOperation();
		btSummonUpdateAvailability();
		if((Uint16)gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_PARTY)
				->summonMask != 0)
			battle->partyStat = PARTY_STATUS_SUMMONS;
		else
			battle->partyStat = PARTY_STATUS_NO_SUMMONS;
		background->tgtYaw = TRIG_TURN * 5 / 32;
		background->ticks = 60;
		mnPartyStatDrawStub(battle->partyStat);

		syWordsClearEntry(battle->actions, sizeof(battle->actions));
		syVRAMCacheFreeEntry(battle->arrowTiles);
		if(syGameFlagGet(FLAG_AUTO_BATTLE) == 0)
			actionCnt = btGatherActions(battle->actions);
		else
			actionCnt = btAutoBattleBuildActions(battle->actions);
		battle->arrowTiles = syVRAMCacheAllocEntry(128);
		mnPartyStatDrawStub(battle->partyStat);
		if(actionCnt < 0)
			goto aborted;

		for(i = 0; i < actionCnt; i++) {
			actingUnit = battle->actions[i].unitId;
			battle->counterUnitId = -1;
			battle->ceaseFire = 0;
			if(syGameFlagGet(FLAG_AUTO_BATTLE) == 0) {
				if(btStagingDispatchAction(
					   &battle->actions[i], i == 0 ? 10 : 0) == 1)
					goto fled;
			} else {
				if(btAutoBattleRunAction(&battle->actions[i]) == 1)
					goto fled;
			}
			if(enemyGroup == FORMATION_KARST_AND_AGATIO_VS_ISAAC)
				goto party_lost;
			if(btUnitListLivingWithReserve(BATTLE_SIDE_MASK_PARTY, 0) == 0)
				goto party_lost;
			if(btUnitListLiving(BATTLE_SIDE_MASK_PARTY, 0) == 0)
				break;
			if(btUnitListLiving(BATTLE_SIDE_MASK_ENEMIES, 0) == 0) {
				if(enemyGroup == FORMATION_DOOM_DRAGON_A ||
					enemyGroup == FORMATION_DOOM_DRAGON_B) {
					btPlayFormChange(enemyGroup++);
					break;
				}
				if(enemyGroup == FORMATION_DOOM_DRAGON_C)
					btPlayFormChange(FORMATION_DOOM_DRAGON_C);
				if(actingUnit <= ALLY_MAX && battle->spoils.defeated == 1) {
					battle->defeatState = DEFEAT_BY_PARTY_MEMBER;
				}
				goto won;
			}
			if(battle->counterUnitId != -1 &&
				btFindUnitPlace(battle->counterTgtId) != -1) {
				battle->actions[i].unitId = battle->counterUnitId;
				battle->actions[i].command = COMMAND_DJINN_COUNTER;
				battle->actions[i].prm = 0;
				battle->actions[i].tgt = btFindUnitPlace(battle->counterTgtId);
				battle->actions[i].range = 1;
				i--;
			}
			if(btLinkBattleSyncTurn() < 0)
				goto aborted;
			if(i == actionCnt - 1 && battle->daedalusStrikes != 0) {
				Uint32 queue = battle->daedalusFromEnemies;

				battle->daedalusFromEnemies = queue >> 1;
				queue &= 1;
				battle->daedalusStrikes--;
				/* The strike is the summoner's side's: unit 0's or the first
				   enemy's. */
				battle->actions[i].unitId = queue << 7;
				battle->actions[i].command = COMMAND_DAEDALUS;
				battle->actions[i].prm = 0;
				battle->actions[i].tgt = queue != 0 ? 0 : FIRST_ENEMY_UNIT;
				battle->actions[i].range = ABILITY_RANGE_ALL;
				i--;
			}
			if(battle->ceaseFire != 0)
				break;
		}

		btPartyClearGuards();
		battle->daedalusFromEnemies = battle->nextDaedalusFromEnemies;
#if DAEDALUS_STRIKE_SIDES_RESET_EACH_TURN
		/* Japanese leaves the next turn's queue in place. */
		battle->nextDaedalusFromEnemies = 0;
#endif
		battle->daedalusStrikes = battle->nextDaedalusStrikes;
		battle->nextDaedalusStrikes = 0;
		battle->firstStrike = FIRST_STRIKE_NONE;
		btTurnEndNoOperation();
		btProcessTurnEnd();
		btMotionDestroyAllSlotObjects();
		if((enemyGroup == FORMATION_KARST_AND_AGATIO_A ||
			   enemyGroup == FORMATION_KARST_AND_AGATIO_B) &&
			turn == 2)
			goto won;
		if(battle->isLink != 0) {
			if(btLinkBattleSyncTurn() < 0)
				goto aborted;
		} else {
			sySynch(20);
		}
		if(syGameFlagGet(FLAG_DJINN_LESSON) == 0)
			continue;

		/* "See! What did I tell you? When you unleashed me, I used my special
		   attack ability!" */
		win = mnMessageOpenWindowStub(
			btTutorialGetDjinnGiveMessage() + DJINN_TUTORIAL_LINE_UNLEASHED, 0,
			4, MESSAGE_WINDOW_ZOOM);
		while(mnMessageAllTypedStub() == 0)
			sySynch(1);
		mnWindowCloseStub(win, 1);
		sySynch(1);
		/* "After being unleashed, we Djinn will then stand by, ready to be
		   summoned back again..." */
		win = mnMessageOpenWindowStub(
			btTutorialGetDjinnGiveMessage() + DJINN_TUTORIAL_LINE_STANDBY, 10,
			4, MESSAGE_WINDOW_ZOOM);
		btMessageWaitForAdvanceAt(44, 24);
		mnWindowCloseStub(win, 1);
		sySynch(1);
	}

won:
	btClearSerialTransferPayload();
	if((enemyGroup != FORMATION_KARST_AND_AGATIO_A &&
		   enemyGroup != FORMATION_KARST_AND_AGATIO_B)) {
		if(enemyGroup != FORMATION_DOOM_DRAGON_C &&
			syGameFlagGet(FLAG_DJINN_LESSON) == 0) {
			if(battle->isLink != 0)
				sySndPlayStub(MUS_VICTORY);
			if(battle->spoils.defeated != 0) {
				sySndPlayStub(MUS_VICTORY);
				if(battle->defeatState <= DEFEAT_SEVERAL) {
					/* The first defeated enemy's name for the message, with no
					   letter. */
					gmBattleUnitSetStub(FIRST_ENEMY_UNIT,
						battle->enemyKinds[battle->firstDefeated],
						NAME_LETTER_PAST_ALL);
					mnMessageArgumentClearAllStub();
					mnMessageArgumentPushStub(
						FIRST_ENEMY_UNIT, MESSAGE_ARGUMENT_ACTOR);
					mnBattleMessageShowAndWaitStub(
						battle->defeatState + (Sint32)MsgDowned);
					btMessageWaitForAdvance();
				}
			}
			btAwardSpoils();
		}
		sySndPlayStub(SOUND_FADE_OUT_BGM);
	}
	syScreenFadeToBlack(30);
	syScreenFadeWait();
	result = battle->spoils.defeated;
	goto finished;

aborted:
	btClearSerialTransferPayload();
	sySuspendEvent(0);
	REG_DISPCNT = DISPCNT_MODE_1;
	result = battle->spoils.defeated;
	syGameFlagSet(FLAG_AREA_TEMP_E8);
	goto finished;

party_lost:
	btClearSerialTransferPayload();
	if((enemyGroup != FORMATION_KARST_AND_AGATIO_A &&
		   enemyGroup != FORMATION_KARST_AND_AGATIO_B) &&
		enemyGroup != FORMATION_KARST_AND_AGATIO_VS_ISAAC) {
		if(enemyGroup != FORMATION_KARST_AND_AGATIO_C) {
			sySndPlayStub(MUS_DEFEATED);
			mnMessageArgumentClearAllStub();
			if(btPartyPrepareMembers(0) == 1)
				mnBattleMessageShowAndWaitStub((Sint32)MsgGoesDownInDefeat);
			else
				mnBattleMessageShowAndWaitStub((Sint32)MsgPartyDefeated);
			btMessageWaitForAdvance();
		}
		sySndPlayStub(SOUND_FADE_OUT_BGM);
	}
	syScreenFadeToBlack(30);
	result = BATTLE_OUTCOME_LOST;
	syScreenFadeWait();
	goto finished;

fled:
	sySndPlayStub(SOUND_FADE_OUT_BGM);
	syScreenFadeToBlack(30);
	syScreenFadeWait();
	result = BATTLE_OUTCOME_FLED;

finished:
	if((enemyGroup == FORMATION_KARST_AND_AGATIO_A ||
		   enemyGroup == FORMATION_KARST_AND_AGATIO_B))
		btScriptedBattleSaveUnits();
	btPartyClearBattleStatuses();
	btTurnEndNoOperation();
	btDjinnRecoveryCountDown();
	SET_BATTLE_TYPE(BATTLE_TYPE_NORMAL);
	syCloseEvent(btObjectUpdateSprPriorities);
	btBackgroundFree();
	return result;
}

/* Lists the party members who fight, three in a link battle, into owners when
   it is not null, and returns how many. */
Sint32 btPartyPrepareMembers(Uint16 *owners)
{
	Sint32 limit;
	Sint32 cnt;
	Sint32 idx;

	limit = PARTY_BATTLE_SIZE;
	if(Ram_HeapSlots->battleState->isLink != 0)
		limit = LINK_PARTY_SIZE;

	cnt = gmPartyCountMembersStub();
	if(cnt > limit)
		cnt = limit;

	for(idx = 0; idx < cnt; idx++) {
		Sint32 owner = MainRoutineStateWork.party[idx];

		if(owners != 0)
			*owners++ = owner;
		syUnitGet(owner)->side = BATTLE_SIDE_PARTY;
	}

	if(owners != 0)
		*owners = BATTLE_UNIT_LIST_END;
	return cnt;
}

/* Lists the party's reserve members, those past the fourth (none in a link
   battle), into owners when it is not null, puts each on the party's side and
   returns how many there are. */
Sint32 btPartyPrepareReserveOwners(Uint16 *owners)
{
	Sint32 cnt;
	Sint32 idx;

	cnt = gmPartyCountMembersStub();
	if(cnt > PARTY_BATTLE_SIZE)
		cnt -= PARTY_BATTLE_SIZE;
	else
		cnt = 0;
	if(Ram_HeapSlots->battleState->isLink != 0)
		cnt = 0;

	for(idx = 0; idx < cnt; idx++) {
		Sint32 owner = MainRoutineStateWork.party[idx + PARTY_BATTLE_SIZE];

		if(owners != 0)
			*owners++ = owner;
		syUnitGet(owner)->side = BATTLE_SIDE_PARTY;
	}

	if(owners != 0)
		*owners = BATTLE_UNIT_LIST_END;
	return cnt;
}

/* Lists the enemy units present and returns how many, 0 when unitIds is
   null. */
Sint32 btEnemyList(Sint16 *unitIds)
{
	Sint16 *output;
	Sint32	cnt;
	Sint32	id;
	Sint32	limit;
	Sint32	listed;

	output = unitIds;
	listed = 0;
	limit = ENEMY_BATTLE_SIZE;
	cnt = 0;
	if(output != NULL) {
		if(syGameFlagGet(FLAG_LINK_BATTLE) != 0)
			limit = LINK_PARTY_SIZE;
		id = FIRST_ENEMY_UNIT;
		limit += FIRST_ENEMY_UNIT;
		for(; id < limit; id++) {
			if(syUnitGet(id)->side != BATTLE_SIDE_NONE) {
				*output = id;
				listed++;
				output++;
			}
		}
		cnt = listed;
		*output = BATTLE_UNIT_LIST_END;
	}
	return cnt;
}

/* Lists the units still standing on the sides in sideMask, party first, into
   unitIds when it is not null, and returns how many. */
Sint32 btUnitListLiving(Sint32 sideMask, Uint16 *unitIds)
{
	Uint16	activeMembers[8];
	Sint32	remaining;
	Sint32	enemyLimit;
	Uint16 *output;
	Sint32	livingCnt;
	Sint32	enemyCapacity;
	Uint16 *member;
	Sint32	unitId;
	Sint16	hp;
	Sint32	activeCnt;
	UNIT   *unit;

	output = unitIds;
	livingCnt = 0;
	enemyCapacity = ENEMY_BATTLE_SIZE;
	if(syGameFlagGet(FLAG_LINK_BATTLE) != 0) {
		enemyCapacity = LINK_PARTY_SIZE;
	}
	if(sideMask & BATTLE_SIDE_MASK_PARTY) {
		activeCnt = btPartyPrepareMembers(activeMembers);
		if(livingCnt < activeCnt) {
			member = activeMembers;
			remaining = activeCnt;
			do {
				unitId = *member;
				member += 1;
				hp = syUnitGet(unitId)->hp;
				if(hp > 0) {
					if(output != NULL) {
						*output = unitId;
						output += 1;
					}
					livingCnt += 1;
				}
				remaining -= 1;
			} while(remaining != 0);
		}
	}
	if(sideMask & BATTLE_SIDE_MASK_ENEMIES) {
		/* remaining, spent on the members, now walks the enemy units' ids. */
		remaining = FIRST_ENEMY_UNIT;
		enemyLimit = enemyCapacity + FIRST_ENEMY_UNIT;
		if(remaining < enemyLimit) {
			for(; remaining < enemyLimit; remaining += 1) {
				unit = syUnitGet(remaining);
				if(unit->side != BATTLE_SIDE_NONE && unit->hp > 0) {
					if(output != NULL) {
						*output = remaining;
						output += 1;
					}
					livingCnt += 1;
				}
			}
		}
	}
	if(output != NULL) {
		*output = BATTLE_UNIT_LIST_END;
	}
	return livingCnt;
}

/* As btUnitListLiving, with the reserve owners listed after the active ones. A
   copy rather than a flag to btUnitListLiving: its list of members has room for
   ten where that one's has eight, and one function would not keep both. */
Sint32 btUnitListLivingWithReserve(Sint32 sideMask, Uint16 *unitIds)
{
	Uint16	members[10];
	Sint32	remaining;
	Sint32	enemyLimit;
	Uint16 *output;
	Sint32	livingCnt;
	Sint32	enemyCapacity;
	Uint16 *member;
	Sint32	unitId;
	Sint16	hp;
	Sint32	cnt;
	UNIT   *unit;

	output = unitIds;
	livingCnt = 0;
	enemyCapacity = ENEMY_BATTLE_SIZE;
	if(syGameFlagGet(FLAG_LINK_BATTLE) != 0) {
		enemyCapacity = LINK_PARTY_SIZE;
	}
	if(sideMask & BATTLE_SIDE_MASK_PARTY) {
		cnt = btPartyPrepareMembers(members);
		cnt += btPartyPrepareReserveOwners(members + cnt);
		if(livingCnt < cnt) {
			member = members;
			remaining = cnt;
			do {
				unitId = *member;
				member += 1;
				hp = syUnitGet(unitId)->hp;
				if(hp > 0) {
					if(output != NULL) {
						*output = unitId;
						output += 1;
					}
					livingCnt += 1;
				}
				remaining -= 1;
			} while(remaining != 0);
		}
	}
	if(sideMask & BATTLE_SIDE_MASK_ENEMIES) {
		remaining = FIRST_ENEMY_UNIT;
		enemyLimit = enemyCapacity + FIRST_ENEMY_UNIT;
		if(remaining < enemyLimit) {
			for(; remaining < enemyLimit; remaining += 1) {
				unit = syUnitGet(remaining);
				if(unit->side != BATTLE_SIDE_NONE && unit->hp > 0) {
					if(output != NULL) {
						*output = remaining;
						output += 1;
					}
					livingCnt += 1;
				}
			}
		}
	}
	if(output != NULL) {
		*output = BATTLE_UNIT_LIST_END;
	}
	return livingCnt;
}

/* Lists the units in the battle on the sides in sideMask, standing or not, and
   returns how many. */
Sint32 btUnitListIds(Sint32 sideMask, Uint16 *unitIds)
{
	BATTLE_STATE *battle;
	Sint32		  cnt;
	Sint32		  i;

	battle = Ram_HeapSlots->battleState;
	cnt = 0;
	if(sideMask & BATTLE_SIDE_MASK_PARTY) {
		for(i = 0; battle->partyUnits[i] != BATTLE_UNIT_LIST_END; i++) {
			if(battle->partyUnits[i] != BATTLE_UNIT_REMOVED) {
				if(unitIds != NULL)
					*unitIds++ = battle->partyUnits[i];
				cnt++;
			}
		}
	}
	if(sideMask & BATTLE_SIDE_MASK_ENEMIES) {
		for(i = 0; battle->enemyIds[i] != BATTLE_UNIT_LIST_END; i++) {
			if(battle->enemyIds[i] != BATTLE_UNIT_REMOVED) {
				if(unitIds != NULL)
					*unitIds++ = battle->enemyIds[i];
				cnt++;
			}
		}
	}
	if(unitIds != NULL)
		*unitIds = BATTLE_UNIT_LIST_END;
	return cnt;
}

/* As btUnitListIds, listing only the units whose HP is not 0. */
Sint32 btUnitListLivingIds(Sint32 sideMask, Uint16 *unitIds)
{
	BATTLE_STATE *battle;
	Sint32		  cnt;
	Sint32		  i;

	battle = Ram_HeapSlots->battleState;
	cnt = 0;
	if(sideMask & BATTLE_SIDE_MASK_PARTY) {
		for(i = 0; battle->partyUnits[i] != BATTLE_UNIT_LIST_END; i++) {
			if(battle->partyUnits[i] != BATTLE_UNIT_REMOVED &&
				syUnitGet(battle->partyUnits[i])->hp != 0) {
				if(unitIds != NULL)
					*unitIds++ = battle->partyUnits[i];
				cnt++;
			}
		}
	}
	if(sideMask & BATTLE_SIDE_MASK_ENEMIES) {
		for(i = 0; battle->enemyIds[i] != BATTLE_UNIT_LIST_END; i++) {
			if(battle->enemyIds[i] != BATTLE_UNIT_REMOVED &&
				syUnitGet(battle->enemyIds[i])->hp != 0) {
				if(unitIds != NULL)
					*unitIds++ = battle->enemyIds[i];
				cnt++;
			}
		}
	}
	if(unitIds != NULL)
		*unitIds = BATTLE_UNIT_LIST_END;
	return cnt;
}
