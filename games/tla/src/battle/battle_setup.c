/* The battle camera each tick, the opening announcement, the unit lists, the
   tile pattern, and the party's battle state and Djinn as a battle starts. Part
   of battle.c. */

/* Each tick of a turn the camera closes this share of the gap to its target
   yaw, one sixteenth, so it slows as it nears it. */
#define CAMERA_YAW_EASE 16

/* Runs each tick of a battle: pauses a link battle whose connection is lost,
   turns the camera a sixteenth of the way toward its target yaw, places it, and
   refreshes the background's zoom. */
void btCameraUpdate(void)
{
	BATTLE_CAMERA			*camera = RamHeapSlots->cameraState;
	BATTLE_BACKGROUND_STATE *background = RamHeapSlots->battleBg;
	POS						 eyeOfs;
	Sint32					*focus;
	Sint16					 delta;
	Uint32					 linkId;

	/* The link check: a missed tick counts only while the link is down, and
	   any tick with it up resets the count. */
	if(RamHeapSlots->battleState->isLink != 0) {
		if((LinkStatusWork & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
			RamHeapSlots->battleState->linkMisses++;
			if(RamHeapSlots->battleState->linkMisses > LINK_MAX_MISSED_TICKS) {
				RamHeapSlots->battleState->linkPaused = 1;
			}
		} else {
			linkId = SIO_ID_OF(REG_SIOCNT);
			if(RamHeapSlots->battleState->linkSide != linkId) {
				RamHeapSlots->battleState->linkPaused = 1;
			}
			RamHeapSlots->battleState->linkMisses = 0;
		}
	}

	if(background->ticks != 0) {
		delta = background->tgtYaw - camera->yaw;
		delta /= CAMERA_YAW_EASE;
		/* The step is added to the yaw read as an unsigned halfword. */
		camera->yaw = (Uint16)camera->yaw + delta;
		background->ticks--;
	}

	focus = camera->focus;
	if(camera->followPos != 0) {
		focus = camera->followPos;
	}

	syMatrixRewindStack();
	syMatrixSetRotateY(camera->yaw);
	syRotX(camera->pitch);
	syMatrixSetTranslation(focus);

	/* The eye sits distance behind the focus, along the turned view. */
	SET_POSITION(&eyeOfs, 0, 0, camera->distance);
	syCalcPoint(&eyeOfs.x, (Sint32 *)camera);

	if(background->keepRange == 0) {
		btCameraSetRange(toFIXED(120), toFIXED(120), 0, 0, toFIXED(1));
	}
}

/* Announces the enemies at the start of a battle, "<enemy> appeared!" for each,
   then whether the party strikes first or is caught by surprise. */
void btIntroAnnounceEncounter(Sint32 enemyCnt)
{
	Sint16		  enemies[8];
	BATTLE_STATE *battle;
	Sint16		 *enemy;
	Sint32		  announced;

	battle = RamHeapSlots->battleState;
	mnMessageArgumentClearAllStub();
	btEnemyList(enemies);

	announced = 0;
	if(enemyCnt != 0) {
		enemy = enemies;
		do {
			mnMessageArgumentPushStub((Uint16)*enemy++, MESSAGE_ARGUMENT_ACTOR);
			if(announced == enemyCnt - 1)
				mnBattleMessageShowAndWaitStub((Sint32)MsgLastEnemyAppeared);
			else
				mnBattleMessageShowAndWaitStub((Sint32)MsgEnemyAppeared);
			announced++;
			btMessageWaitForAdvance();
		} while(announced != enemyCnt);
	}

	mnBattleMessageCloseStub();
	if(battle->firstStrike == FIRST_STRIKE_PARTY) {
		mnMessageArgumentClearAllStub();
		mnBattleMessageShowAndWaitStub((Sint32)MsgPartyStrikesFirst);
		btMessageWaitForAdvance();
	} else if(battle->firstStrike == FIRST_STRIKE_ENEMIES) {
		mnMessageArgumentClearAllStub();
		mnBattleMessageShowAndWaitStub((Sint32)MsgPartySurprised);
		btMessageWaitForAdvance();
	}
}

/* Fills the battle's two unit lists: the party in its order, and the enemies
   left to right, or for a fixed formation out from the centre. */
void btUnitCollect(void)
{
	Uint16		  unitIds[UNIT_MAX];
	BATTLE_STATE *battle;
	Sint32		  cnt;
	Sint32		  i;
	Sint32		  pos;
	Sint32		  formation;

	battle = RamHeapSlots->battleState;
	cnt = btPartyPrepareMembers(unitIds);
	for(i = 0; i < cnt; i++)
		battle->partyUnits[i] = unitIds[i];
	battle->partyUnits[cnt] = BATTLE_UNIT_LIST_END;

	/* The shuffled and interleaved formations keep the enemies in unit order;
	   the others put the first in the centre and the rest either side of it. */
	cnt = btEnemyList(unitIds);
	formation = battle->layout;
	if(formation >= FORMATION_LAYOUT_SHUFFLED) {
		if(formation <= FORMATION_LAYOUT_INTERLEAVED) {
			for(i = 0; i < cnt; i++)
				battle->enemyIds[i] = unitIds[i];
			goto placed;
		}
	}
	for(i = 0; i < cnt; i++) {
		pos = BattleFormationCenteredOffsets[i] + cnt / 2;
		battle->enemyIds[pos] = unitIds[i];
	}
placed:
	battle->enemyIds[cnt] = BATTLE_UNIT_LIST_END;
}

/* Unused: moves 16 bytes of VRAM down by 16 and clears the 20 from 12 bytes
   past where they land. */
UNUSED Sint32 btShiftVRAMTiles(void)
{
	/* XXX (no evidence): what the bytes moved are, tiles of character block 0
	   or entries of screen block 0; nothing calls this in either game. */
	syDMAXCopy((const void *)0x06000290, (void *)0x06000280,
		DMA_COPY_HALFWORDS(8), REG_DMA3);
	return syWordsClear((void *)0x0600028c, 20);
}

/* Unused: clears BG0's vertical scroll. */
UNUSED void btClearBg0Vofs(void)
{
	syIOWrite16(0, &REG_BG0VOFS);
}

/* Does nothing; btRun calls it with 1 as the battle is set up. */
void btPartyBattleSetupNoOperation(Sint32 unused)
{
}

/* Clears the party's battle-only statuses, and works their stats out again, as
   a battle ends. */
UNREAD_RESULT btPartyClearBattleStatuses(void)
{
	Uint16 owners[10];
	Sint32 cnt;
	Sint32 i;

	cnt = btPartyPrepareMembers(owners);
	cnt += btPartyPrepareReserveOwners(owners + cnt);

	for(i = 0; i < cnt; i++) {
		UNIT  *unit = syUnitGet(owners[i]);
		Sint32 element;

		for(element = 0; element < ELEMENT_COUNT; element++)
			unit->summonBoost[element] = 0;

		unit->attackTurns = 0;
		unit->attackStage = 0;
		unit->defenseTurns = 0;
		unit->defenseStage = 0;
		unit->resistanceTurns = 0;
		unit->resistanceStage = 0;
		unit->delusion = 0;
		unit->confusion = 0;
		unit->charm = 0;
		unit->stun = 0;
		unit->sleep = 0;
		unit->psySeal = 0;
		unit->regenerate = 0;
		unit->reflect = 0;
		unit->grimReaper = 0;
		unit->equipBonus[0] = 0;
		unit->equipBonus[1] = 0;
		unit->ready = 0;
		unit->immobile = 0;
		unit->agilityTurns = 0;
		unit->agilityStage = 0;
		unit->challenge = 0;

		gmUnitCalcStub(owners[i]);
	}
}

/* Clears every unit's guard and Ground's hold at the end of a turn: the party's
   members and reserves and the opponents. btRun calls it once the turn's
   actions have played. */
void btPartyClearGuards(void)
{
	Uint16	ids[16];
	Uint16 *id = ids;
	Sint32	cnt;

	cnt = btPartyPrepareMembers(ids);
	cnt += btPartyPrepareReserveOwners(ids + cnt);
	cnt += btUnitListIds(BATTLE_SIDE_MASK_ENEMIES, ids + cnt);
	if(cnt > 0) {
		do {
			UNIT *unit = syUnitGet(*id++);

			unit->defendLevel = 0;
			unit->immobile = 0;
			cnt--;
		} while(cnt != 0);
	}
}

/* As a battle starts, lists every Djinni a fighting party member holds unset
   that its side's Djinn recovery list lacks, on standby: the party's on the
   party's list, a link opponent's (past ALLY_MAX) on the opponent's. */
void btDjinnRecoveryReconcile(void)
{
	Uint16				 owners[10];
	Sint32				 cnt;
	Sint32				 n;
	Uint32				 owner;
	Sint32				 element;
	Sint32				 djinn;
	Sint32				 i;
	DJINN_RECOVERY_LIST *list;

	cnt = btPartyPrepareMembers(owners);
	for(n = 0; n < cnt; n++) {
		owner = owners[n];
		for(element = 0; element < ELEMENT_COUNT; element++) {
			for(djinn = 0; djinn < DJINN_PER_ELEMENT; djinn++) {
				if(gmDjinnCanSetStub(owner, element, djinn) != 0) {
					list =
						&(gmDjinnGetRecoveryTableStub(owner > ALLY_MAX))->list;
					for(i = 0; i < list->cnt; i++) {
						if(element == list->entries[i].element &&
							djinn == list->entries[i].idx)
							break;
					}
					if(i == list->cnt)
						gmDjinnRecoveryAddStandbyStub(owner, element, djinn);
				}
			}
		}
	}
}

/* The summon mask's bits of the summons from SUMMON_ZAGAN on, which the party
   learns ("can now summon") rather than earns by its Djinn:
   btSummonUpdateAvailability keeps them as they are. */
#define SUMMON_MASK_LEARNED (~0u << SUMMON_ZAGAN)

/* Works out which of the summons up to SUMMON_THOR the party can call: those
   whose Djinn of every element the party holds (djinnHeldCnts, set or not), all
   members together, and stores them in the party's summon mask beside the ones
   learned. Whether enough are on standby is checked as a summon is chosen
   (mnBattleSummonMenuRun) and built (btActionRunBuild). btRun calls it as the
   battle is set up and at the start of each turn. */
void btSummonUpdateAvailability(void)
{
	Uint8  totals[ELEMENT_COUNT];
	Uint16 partyMembers[10];
	Sint32 partySize;
	Sint32 i; /* an element, then a summon */
	Uint32 availableMask;

	partySize = btPartyPrepareMembers(partyMembers);
	availableMask =
		gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_PARTY)->summonMask &
		SUMMON_MASK_LEARNED;

	for(i = 0; i < ELEMENT_COUNT; i++) {
		totals[i] = 0;
		{
			Sint32 partySlot;

			for(partySlot = 0; partySlot < partySize; partySlot++) {
				UNIT *member = syUnitGet(partyMembers[partySlot]);

				totals[i] += member->djinnHeldCnts[i];
			}
		}
	}

	for(i = 0; i <= SUMMON_THOR; i++) {
		const SUMMON_DEFINITION *summon = gmSummonDefinitionGetNearStub(i);
		Sint32					 elementsMet;

		if(summon != 0) {
			const Uint8 *required = summon->djinnRequired;

			elementsMet = 0;
			if(totals[0] >= required[0]) {
				Uint8 *total = totals;

				do {
					elementsMet++;
					if(elementsMet >= ELEMENT_COUNT)
						break;
					total++;
					required++;
				} while(*total >= *required);
			}

			if(elementsMet == ELEMENT_COUNT)
				availableMask |= 1u << i;
		}
	}

	gmDjinnGetRecoveryTableStub(DJINN_RECOVERY_PARTY)->summonMask =
		availableMask;
}

/* Unused: does nothing. */
UNUSED void btSummonReservedNoOperation(void)
{
}
