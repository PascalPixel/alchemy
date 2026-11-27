/* @file */
/* A whole battle, btRun, its tutorial helpers, and the lists of */
/* the units in it. */
/* Part of battle.c. */

#include "window/stubs.h"
#include "battle_effect/stubs.h"
#include "menu/djinn_menu.h"
#include "sdk/sound_driver_symbols.h"

/* The six enemy units, which syUnitGet reads for unit ids 128 to 133 while the
 */
/* battle lasts (heap slot HEAP_SLOT_ENEMY_UNITS). */
#define ENEMY_UNITS_SIZE (ENEMY_BATTLE_SIZE * BATTLE_UNIT_SIZE)

/* The camera's zoom, in percent, from the battle's setup until the opening */
/* sets its own: the curtain's a frame later, or the Doom Dragon's shattering */
/* opening's, so no frame shows it. */
#define BATTLE_ZOOM_BEFORE_OPENING 190

/* With the front line down and someone living, sends the reserve members in */
/* to take the fallen ones' places, as many as there are of both. */
void btPartySwapInReserves(void) {
	u16 actors[6];
	u16 reserves[6];
	s32 actorCount;
	s32 reserveCount;
	s32 count;
	BATTLE_STATE *battle = Ram_HeapSlots->battleState;
	s32 i;

	if(btUnitListLivingWithReserve(BATTLE_SIDE_MASK_PARTY, 0) == 0)
		return;
	count = actorCount = btUnitListIds(BATTLE_SIDE_MASK_PARTY, actors);
	reserveCount = btPartyPrepareReserveOwners(reserves);
	if(count > reserveCount)
		count = reserveCount;
	btHighlightSetModes(actors, HIGHLIGHT_FADE_OUT);
	sySynch(32);
	for(i = 0; i < count; i++) {
		s32 unit = battle->partyUnits[i];
		s32 id = reserves[i];
		BATTLE_OBJECT *from = btObjectGet(unit);
		BATTLE_OBJECT *to = btObjectGet(id);
		MOTION_OBJECT *object = to->obj;
		MOTION_OBJECT *place = from->obj;

		to->anchorX = from->anchorX;
		to->anchorZ = from->anchorZ;
		fdActorSetPosAndResetMotionStub((void *)object, place->x, place->y,
										place->z);
		MainRoutineStateWork.party[i] = id;
		battle->partyUnits[i] = id;
	}
	for(i = 0; i < count; i++)
		MainRoutineStateWork.party[actorCount + i] = actors[i];
	mnPartyStatDrawStub(Ram_HeapSlots->battleState->partyStat);
	btObjectPut();
	btHighlightSetModes(reserves, HIGHLIGHT_FADE_IN);
	sySynch(32);
}

/* Allocates the battle camera and sets its starting view. */
void btCameraCreate(void) {
	BATTLE_CAMERA *camera =
		syHeapSlotAllocEwram(HEAP_SLOT_CAMERA, sizeof(BATTLE_CAMERA));
	s32 *focus = camera->focus;

	SET_VECTOR(focus, 0, toFIXED(64), 0);
	SET_VECTOR(camera->eye, 0, toFIXED(180), toFIXED(64));
	camera->yaw = TRIG_TURN * 5 / 32;
	camera->pitch = TRIG_TURN * 5 / 16;
	camera->distance = toFIXED(256);
}

/* Name letters past every edition's last, which leave an enemy's name bare: */
/* 255, and NAME_LETTER_ALPHABET, past Z, the last letter the Japanese edition
 */
/* gives. */
#define NAME_LETTER_NONE	 255
#define NAME_LETTER_PAST_ALL NAME_LETTER_ALPHABET

/* Replaces the first enemy with the Doom Dragon's next form: */
/* ENEMY_DOOM_DRAGON_E after FORMATION_DOOM_DRAGON_A, else ENEMY_DOOM_DRAGON_H.
 */
/* The form change's battle effect calls it through its stub call. */
void btChangeEnemyForm(s32 id) {
	BATTLE_STATE *battle = Ram_HeapSlots->battleState;
	s32 unit;
	BATTLE_OBJECT *battleObject;

	if(id == FORMATION_DOOM_DRAGON_A)
		unit = ENEMY_DOOM_DRAGON_E;
	else
		unit = ENEMY_DOOM_DRAGON_H;
	btObjectMarkForRelease(FIRST_ENEMY_UNIT);
	btMotionDestroyAllSlotObjects();
	btEnemyClaimNameLetter(unit, 1);
	btUnitSetStub(FIRST_ENEMY_UNIT, unit, NAME_LETTER_NONE);
	battle->enemyIds[0] = FIRST_ENEMY_UNIT;
	battle->enemyIds[1] = BATTLE_UNIT_LIST_END;
	btPlacementRelayoutEnemies();
	battleObject = btObjectGet(FIRST_ENEMY_UNIT);
	btObjectMake(battleObject, FIRST_ENEMY_UNIT,
				 battleObject->anchorX / FIXED_ONE,
				 battleObject->anchorZ / FIXED_ONE);
	btObjectPut();
}

/* Plays the Doom Dragon's form change effect for its formation value */
/* (FORMATION_DOOM_DRAGON_A to _C) on the first enemy. */
void btPlayFormChange(s32 value) {
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

/* Returns the first of the Djinn tutorial's messages: the second set's */
/* (MsgDjinnGiveTutorialB) while FLAG_ALREADY_KNOWS_DJINN is set, as Up held */
/* when Echo's tutorial begins sets it. */
s32 btTutorialGetDjinnGiveMessage(void) {
	if(syGameFlagGet(FLAG_ALREADY_KNOWS_DJINN))
		return (s32)MsgDjinnGiveTutorialB;
	return (s32)MsgDjinnGiveTutorialA;
}

void fdActorSystemInitStub(s32 mode);

/* Fights the battle against the enemy formation enemyGroup, from the opening */
/* to the end. Returns the number of enemies defeated when the party won or the
 */
/* link broke, BATTLE_OUTCOME_FLED or BATTLE_OUTCOME_LOST. */
s32 btRun(s32 enemyGroup) {
	BATTLE_CAMERA *camera;
	BATTLE_STATE *battle;
	BATTLE_BACKGROUND_STATE *background;
	s32 enemyCount;
	u8 *enemyUnitDestination;
	s32 turn;
	s32 actionCount;
	s32 i;
	s32 result;
	s32 window;
	s32 linkWaitFrames;
	u32 copyIndex;
	u32 actingUnit;
	s16 cue;

	camera = syHeapSlotAllocEwram(HEAP_SLOT_CAMERA, sizeof(BATTLE_CAMERA));
	battle = syHeapSlotAllocEwram(HEAP_SLOT_BATTLE, sizeof(BATTLE_STATE));
	syHeapSlotAllocEwram(HEAP_SLOT_ENEMY_UNITS, ENEMY_UNITS_SIZE);
	background = syHeapSlotAllocEwram(HEAP_SLOT_BATTLE_BACKGROUND,
									  sizeof(BATTLE_BACKGROUND_STATE));
	syHeapSlotAllocEwram(HEAP_SLOT_BATTLE_UNUSED, 640);
	result = 0;
	syWordsClearEntry(Ram_HeapSlots->enemyUnits, ENEMY_UNITS_SIZE);
	syInitEvent();
	background->tgtYaw = YAW_PARTY_ACTION;
	background->frames = 0;
	background->keepRange = 1;
	background->unusedAfterKeepRange[0] = 0;
	background->unusedAfterKeepRange[1] = 0;
	REG_DISPCNT = DISPCNT_MODE_1;
	syGameFlagSet(FLAG_IN_BATTLE);
	/* XXX (no evidence): what flag 0x169 marks; it is set as every battle */
	/* starts, as The Broken Seal's, and no code of either game reads it, so */
	/* nothing the game does shows what it means. */
	syGameFlagSet(0x169);
	syInitMatrix();

	IWRAM_FILL_WORDS(camera, sizeof(BATTLE_CAMERA), 0);
	syWordsFillEntry(battle, sizeof(BATTLE_STATE), 0);
	battle->arrowTiles = -1;
	battle->formation = enemyGroup;
	syHeapSlotAllocEwram(HEAP_SLOT_BATTLE_MESSAGE,
						 sizeof(BATTLE_MESSAGE_STATE));
	IWRAM_FILL_WORDS(Ram_HeapSlots->battleMsg, sizeof(BATTLE_MESSAGE_STATE), 0);
	battle->bg = btBackgroundGetSelectedStub();
	syHeapSlotAllocEwram(HEAP_SLOT_SPRITES, 3584);
	syHeapSlotAllocEwram(HEAP_SLOT_SPRITE_ANIMATIONS, 1536);
	fdActorSystemInitStub(ACTOR_SYSTEM_BATTLE);
	/* The Djinn lesson's battle loads the glyph tiles too. */
	if(syGameFlagGet(FLAG_DJINN_LESSON) != 0)
		mnWindowSystemInitBattleStub(TRUE);
	else
		mnWindowSystemInitBattleStub(FALSE);

	btCameraCreate();
	enemyCount = btFormationBuildEnemyList(battle->formation);

	if(syGameFlagGet(FLAG_LINK_BATTLE) != 0) {
		battle->isLink = 1;
		btSetType(BATTLE_TYPE_LINK);
	}
	if(battle->isLink != 0) {
		btSetRandomSeed(0);
		for(linkWaitFrames = 0;;) {
			if((LinkStatusWork & LINK_BOTH_CONNECTED) == LINK_BOTH_CONNECTED)
				break;
			sySynch(1);
			linkWaitFrames++;
			if(linkWaitFrames > LINK_MAX_MISSED_FRAMES) {
				battle->linkPaused = 1;
				break;
			}
		}
		battle->linkSide = SIO_ID_OF(REG_SIOCNT);
		enemyUnitDestination = Ram_HeapSlots->enemyUnits;
		for(copyIndex = 0; copyIndex < ENEMY_UNITS_SIZE; copyIndex++)
			*enemyUnitDestination++ = ActorSpriteSlots[copyIndex];
		enemyCount = syGameFlagGetByte(FLAG_AREA_TEMP_F0);
		btPartyAssignOpponentSlots();
		battle->layout = FORMATION_LAYOUT_SHUFFLED;
	}
	sySetEvent(btCameraUpdate, TASK_PASS_FRAME - 1);

	cue = MainRoutineStateWork.battleMusic;
	if(cue != 0) {
		sySndPlayStub(cue);
		if(syGameFlagGet(FLAG_LINK_BATTLE) != 0) {
			sySndPlayStub(MUS_TEST_YOUR_SKILLS);
			sySndLoadReverbCode(SOUND_REVERB_NONE);
		}
	}
	else {
		sySndPlayStub(MUS_BATTLE_THE_LOST_AGE);
	}

	btUnitCollect();
	btUnitRefreshPlacement();
	btDjinnRecoveryReconcile();
	btSummonUpdateAvailability();
	if((u16)gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_PARTY)->summonMask != 0)
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
	btCameraSetRange(toFIXED(160), toFIXED(80), 0, 0, FIXED_ONE * 2);
	btCameraSetView(0, 0, 0, BATTLE_ZOOM_BEFORE_OPENING);
	btPartyBattleSetupNoOp(1);
	REG_BLDCNT = 0;
	btSpoilsClear();
	battle->arrowTiles = syVramCacheAllocEntry(128);

	battle->firstStrike = FIRST_STRIKE_NONE;
	if(syGameFlagGet(FLAG_DJINN_LESSON) != 0) {
		battle->firstStrike = FIRST_STRIKE_PARTY;
	}
	else if(MainRoutineStateWork.battleType == BATTLE_TYPE_NORMAL) {
		if((btRandomStub() & 15) == 0)
			battle->firstStrike = FIRST_STRIKE_PARTY;
		else if((btRandomStub() & 31) == 0)
			battle->firstStrike = FIRST_STRIKE_ENEMIES;
	}
	else if(MainRoutineStateWork.battleType == BATTLE_TYPE_LOST) {
		battle->firstStrike = FIRST_STRIKE_ENEMIES;
	}
	btTransitionRun(enemyCount, enemyGroup);
	background->keepRange = 0;
	CpuLoadDisplayEnabled = 0;
	sySetEvent(btObjectUpdateSprPriorities, TASK_PASS_FRAME);

	for(turn = 1;; turn++) {
		if(btUnitListLivingIds(BATTLE_SIDE_MASK_PARTY, 0) == 0)
			btPartySwapInReserves();
		btTurnStartNoOp();
		btSummonUpdateAvailability();
		if((u16)gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_PARTY)->summonMask !=
		   0)
			battle->partyStat = PARTY_STATUS_SUMMONS;
		else
			battle->partyStat = PARTY_STATUS_NO_SUMMONS;
		background->tgtYaw = TRIG_TURN * 5 / 32;
		background->frames = 60;
		mnPartyStatDrawStub(battle->partyStat);

		syWordsClearEntry(battle->actions, sizeof(battle->actions));
		syVramCacheFreeEntry(battle->arrowTiles);
		if(syGameFlagGet(FLAG_AUTO_BATTLE) == 0)
			actionCount = btGatherActions(battle->actions);
		else
			actionCount = btAutoBattleBuildActions(battle->actions);
		battle->arrowTiles = syVramCacheAllocEntry(128);
		mnPartyStatDrawStub(battle->partyStat);
		if(actionCount < 0)
			goto aborted;

		for(i = 0; i < actionCount; i++) {
			actingUnit = battle->actions[i].unitId;
			battle->counterUnitId = -1;
			battle->ceaseFire = 0;
			if(syGameFlagGet(FLAG_AUTO_BATTLE) == 0) {
				if(btStagingDispatchAction(&battle->actions[i],
										   i == 0 ? 10 : 0) == 1)
					goto fled;
			}
			else {
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
			if(i == actionCount - 1 && battle->daedalusStrikes != 0) {
				u32 queue = battle->daedalusFromEnemies;

				battle->daedalusFromEnemies = queue >> 1;
				queue &= 1;
				battle->daedalusStrikes--;
				/* The strike is the summoner's side's: unit 0's or the first
				 * enemy's. */
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
#if TLA_PATCH_1
		/* Japanese leaves the next turn's queue in place. */
		battle->nextDaedalusFromEnemies = 0;
#endif
		battle->daedalusStrikes = battle->nextDaedalusStrikes;
		battle->nextDaedalusStrikes = 0;
		battle->firstStrike = FIRST_STRIKE_NONE;
		btTurnEndNoOp();
		btProcessTurnEnd();
		btMotionDestroyAllSlotObjects();
		if((enemyGroup == FORMATION_KARST_AND_AGATIO_A ||
			enemyGroup == FORMATION_KARST_AND_AGATIO_B) &&
		   turn == 2)
			goto won;
		if(battle->isLink != 0) {
			if(btLinkBattleSyncTurn() < 0)
				goto aborted;
		}
		else {
			sySynch(20);
		}
		if(syGameFlagGet(FLAG_DJINN_LESSON) == 0)
			continue;

		/* "See! What did I tell you? When you unleashed me, I used my special
		 */
		/* attack ability!" */
		window = mnMessageOpenWindowStub(btTutorialGetDjinnGiveMessage() +
											 DJINN_TUTORIAL_LINE_UNLEASHED,
										 0, 4, MESSAGE_WINDOW_ZOOM);
		while(mnMessageAllTypedStub() == 0)
			sySynch(1);
		mnWindowCloseStub(window, 1);
		sySynch(1);
		/* "After being unleashed, we Djinn will then stand by, ready to be */
		/* summoned back again..." */
		window = mnMessageOpenWindowStub(btTutorialGetDjinnGiveMessage() +
											 DJINN_TUTORIAL_LINE_STANDBY,
										 10, 4, MESSAGE_WINDOW_ZOOM);
		btMessageWaitForAdvanceAt(44, 24);
		mnWindowCloseStub(window, 1);
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
					 * letter. */
					btUnitSetStub(FIRST_ENEMY_UNIT,
								  battle->enemyKinds[battle->firstDefeated],
								  NAME_LETTER_PAST_ALL);
					mnMessageArgumentClearAllStub();
					mnMessageArgumentPushStub(FIRST_ENEMY_UNIT,
											  MESSAGE_ARGUMENT_ACTOR);
					mnBattleMessageShowAndWaitStub(battle->defeatState +
												   (s32)MsgDowned);
					btMessageWaitForAdvance();
				}
			}
			btAwardSpoils();
		}
		sySndPlayStub(SOUND_FADE_OUT_BGM);
	}
	syScrFadeToBlack(30);
	syScrFadeWait();
	result = battle->spoils.defeated;
	goto finished;

aborted:
	btClearSerialTransferPayload();
	syTaskSuspend(0);
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
				mnBattleMessageShowAndWaitStub((s32)MsgGoesDownInDefeat);
			else
				mnBattleMessageShowAndWaitStub((s32)MsgPartyDefeated);
			btMessageWaitForAdvance();
		}
		sySndPlayStub(SOUND_FADE_OUT_BGM);
	}
	syScrFadeToBlack(30);
	result = BATTLE_OUTCOME_LOST;
	syScrFadeWait();
	goto finished;

fled:
	sySndPlayStub(SOUND_FADE_OUT_BGM);
	syScrFadeToBlack(30);
	syScrFadeWait();
	result = BATTLE_OUTCOME_FLED;

finished:
	if((enemyGroup == FORMATION_KARST_AND_AGATIO_A ||
		enemyGroup == FORMATION_KARST_AND_AGATIO_B))
		btScriptedBattleSaveUnits();
	btPartyClearBattleStatuses();
	btTurnEndNoOp();
	btDjinnRecoveryCountDown();
	btSetType(BATTLE_TYPE_NORMAL);
	syCloseEvent(btObjectUpdateSprPriorities);
	btBackgroundFree();
	return result;
}

/* Lists the party members who fight, three in a link battle, into owners when
 */
/* it is not null, and returns how many. */
s32 btPartyPrepareMembers(u16 *owners) {
	s32 limit;
	s32 count;
	s32 index;

	limit = PARTY_BATTLE_SIZE;
	if(Ram_HeapSlots->battleState->isLink != 0)
		limit = LINK_PARTY_SIZE;

	count = gmPartyCountMembersStub();
	if(count > limit)
		count = limit;

	for(index = 0; index < count; index++) {
		s32 owner = MainRoutineStateWork.party[index];

		if(owners != 0)
			*owners++ = owner;
		syUnitGet(owner)->side = BATTLE_SIDE_PARTY;
	}

	if(owners != 0)
		*owners = BATTLE_UNIT_LIST_END;
	return count;
}

/* Lists the party's reserve members, those past the fourth (none in a link */
/* battle), into owners when it is not null, puts each on the party's side and
 */
/* returns how many there are. */
s32 btPartyPrepareReserveOwners(u16 *owners) {
	s32 count;
	s32 index;

	count = gmPartyCountMembersStub();
	if(count > PARTY_BATTLE_SIZE)
		count -= PARTY_BATTLE_SIZE;
	else
		count = 0;
	if(Ram_HeapSlots->battleState->isLink != 0)
		count = 0;

	for(index = 0; index < count; index++) {
		s32 owner = MainRoutineStateWork.party[index + PARTY_BATTLE_SIZE];

		if(owners != 0)
			*owners++ = owner;
		syUnitGet(owner)->side = BATTLE_SIDE_PARTY;
	}

	if(owners != 0)
		*owners = BATTLE_UNIT_LIST_END;
	return count;
}

/* Lists the enemy units present and returns how many, 0 when unitIds is null.
 */
s32 btEnemyList(s16 *unitIds) {
	s16 *output;
	s32 count;
	s32 id;
	s32 limit;
	s32 listed;

	output = unitIds;
	listed = 0;
	limit = ENEMY_BATTLE_SIZE;
	count = 0;
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
		count = listed;
		*output = BATTLE_UNIT_LIST_END;
	}
	return count;
}

/* Lists the units still standing on the sides in sideMask, party first, into */
/* unitIds when it is not null, and returns how many. */
s32 btUnitListLiving(s32 sideMask, u16 *unitIds) {
	u16 activeMembers[8];
	s32 remaining;
	s32 enemyLimit;
	u16 *output;
	s32 livingCount;
	s32 enemyCapacity;
	u16 *member;
	s32 unitId;
	s16 hp;
	s32 activeCount;
	UNIT *unit;

	output = unitIds;
	livingCount = 0;
	enemyCapacity = ENEMY_BATTLE_SIZE;
	if(syGameFlagGet(FLAG_LINK_BATTLE) != 0) {
		enemyCapacity = LINK_PARTY_SIZE;
	}
	if(sideMask & BATTLE_SIDE_MASK_PARTY) {
		activeCount = btPartyPrepareMembers(activeMembers);
		if(livingCount < activeCount) {
			member = activeMembers;
			remaining = activeCount;
			do {
				unitId = *member;
				member += 1;
				hp = syUnitGet(unitId)->hp;
				if(hp > 0) {
					if(output != NULL) {
						*output = unitId;
						output += 1;
					}
					livingCount += 1;
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
					livingCount += 1;
				}
			}
		}
	}
	if(output != NULL) {
		*output = BATTLE_UNIT_LIST_END;
	}
	return livingCount;
}

/* As btUnitListLiving, with the reserve owners listed after the */
/* active ones. */
/* A copy rather than a flag to btUnitListLiving: its list of members has */
/* room for ten where that one's has eight, and one function would not keep */
/* both. */
s32 btUnitListLivingWithReserve(s32 sideMask, u16 *unitIds) {
	u16 members[10];
	s32 remaining;
	s32 enemyLimit;
	u16 *output;
	s32 livingCount;
	s32 enemyCapacity;
	u16 *member;
	s32 unitId;
	s16 hp;
	s32 count;
	UNIT *unit;

	output = unitIds;
	livingCount = 0;
	enemyCapacity = ENEMY_BATTLE_SIZE;
	if(syGameFlagGet(FLAG_LINK_BATTLE) != 0) {
		enemyCapacity = LINK_PARTY_SIZE;
	}
	if(sideMask & BATTLE_SIDE_MASK_PARTY) {
		count = btPartyPrepareMembers(members);
		count += btPartyPrepareReserveOwners(members + count);
		if(livingCount < count) {
			member = members;
			remaining = count;
			do {
				unitId = *member;
				member += 1;
				hp = syUnitGet(unitId)->hp;
				if(hp > 0) {
					if(output != NULL) {
						*output = unitId;
						output += 1;
					}
					livingCount += 1;
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
					livingCount += 1;
				}
			}
		}
	}
	if(output != NULL) {
		*output = BATTLE_UNIT_LIST_END;
	}
	return livingCount;
}

/* Lists the units in the battle on the sides in sideMask, standing or not, and
 */
/* returns how many. */
s32 btUnitListIds(s32 sideMask, u16 *unitIds) {
	BATTLE_STATE *battle;
	s32 count;
	s32 i;

	battle = Ram_HeapSlots->battleState;
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

/* As btUnitListIds, listing only the units whose HP is not 0. */
s32 btUnitListLivingIds(s32 sideMask, u16 *unitIds) {
	BATTLE_STATE *battle;
	s32 count;
	s32 i;

	battle = Ram_HeapSlots->battleState;
	count = 0;
	if(sideMask & BATTLE_SIDE_MASK_PARTY) {
		for(i = 0; battle->partyUnits[i] != BATTLE_UNIT_LIST_END; i++) {
			if(battle->partyUnits[i] != BATTLE_UNIT_REMOVED &&
			   syUnitGet(battle->partyUnits[i])->hp != 0) {
				if(unitIds != NULL)
					*unitIds++ = battle->partyUnits[i];
				count++;
			}
		}
	}
	if(sideMask & BATTLE_SIDE_MASK_ENEMIES) {
		for(i = 0; battle->enemyIds[i] != BATTLE_UNIT_LIST_END; i++) {
			if(battle->enemyIds[i] != BATTLE_UNIT_REMOVED &&
			   syUnitGet(battle->enemyIds[i])->hp != 0) {
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
