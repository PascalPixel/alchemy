/* Field requests and each tick of the leader's walking: exits, encounters,
   damage. Part of field_mode.c. */

#include "field/emote.h"

/* MAIN_ROUTINE_STATE.hazardDamage's amount, below HAZARD_DAMAGE_PERCENT. */
#define HAZARD_DAMAGE_AMOUNT  0xff
/* A loss of more HP than this plays SE_HEAVY_IMPACT rather than SE_IMPACT. */
#define HEAVY_DAMAGE		  10
/* Poison takes 1/POISON_DAMAGE_DIVISOR of a character's maximum HP, rounded to
   the nearest, and venom ("deadly poison") 1/VENOM_DAMAGE_DIVISOR. */
#define POISON_DAMAGE_DIVISOR 20
#define VENOM_DAMAGE_DIVISOR  10

/* Unused: returns 0. */
UNUSED Sint32 fdModeReturnZero(void)
{
	return 0;
}

/* Clears the field's twelve pending requests (see FIELD_STATE). */
void fdClearRequests(void)
{
	FIELD_STATE *state = FieldStateWork;

	state->stepCue = 0;
	state->pushMetatile = 0;
	state->exit = 0;
	state->fieldMenu = 0;
	state->aButton = 0;
	state->pauseMenu = 0;
	state->talk = 0;
	state->examineMetatile = 0;
	state->encounter = 0;
	state->psynergy = 0;
	state->item = 0;
	state->raisedCue = 0;
}

/* Returns the sum of the field's twelve pending requests: non-zero while any
   request is pending. */
Sint32 fdCountRequests(void)
{
	FIELD_STATE *state = FieldStateWork;

	/* BUG: a sum can be 0 with requests pending. fdUpdateWalking raises
	   FIELD_RAISED_CUE_PARTY_FELL (-1) on the tick a party member falls to
	   poison, and the step cue request is the metatile's metatile behavior, 1
	   on step cue 1's metatiles; together they sum to 0, and both wait until
	   another request breaks the tie. BUGFIX tests each request instead. */
#if FIX_REQUEST_SUM_CANCELS
	return state->stepCue | state->pushMetatile | state->exit |
		state->fieldMenu | state->aButton | state->pauseMenu |
		(Sint16)state->talk | state->examineMetatile | state->encounter |
		state->psynergy | state->item | state->raisedCue;
#else
	return state->stepCue + state->pushMetatile + state->exit +
		state->fieldMenu + state->aButton + state->pauseMenu + state->talk +
		state->examineMetatile + state->encounter + state->psynergy +
		state->item + state->raisedCue;
#endif
}

/* Sets the field state's hoverReady when the ship flies (FLAG_SHIP_FLIES) and a
   party member knows Hover; clears it otherwise. The world map's pad movement
   reads it. */
void fdUpdateHoverReady(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	if(syGameFlagGet(FLAG_SHIP_FLIES) &&
		gmPartyFindOwnerWithActionStub(ABILITY_HOVER) != -1)
		state->hoverReady = 1;
	else
		state->hoverReady = 0;
}

/* Returns the metatile behavior of the map metatile 16 pixels ahead of the
   party leader (on the World Map, of its 32-pixel metatile), or 0 without a
   leader. */
Sint32 fdGetCollisionAheadOfLeader(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR		*actor = fdGetPartyActor();
	MAP_STATE	*map = Ram_HeapSlots->mapGrid;
	FIXED		 pos[3];
	Uint8		*metatile;

	if(actor == NULL)
		return 0;
	pos[0] = actor->x.fixed;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed;
	syVectorAddPolarOffset(toFIXED(16), actor->facing, pos);
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		Sint32 x, z;

		x = pos[0];
		z = pos[2];
		metatile = syRamMapBlocks +
			((((x / toFIXED(WORLD_MAP_METATILE_PIXELS)) &
				  (WORLD_MAP_VIEW_METATILES - 1)) +
				 (((z / toFIXED(WORLD_MAP_METATILE_PIXELS)) &
					  (WORLD_MAP_VIEW_METATILES - 1)) *
					 WORLD_MAP_VIEW_METATILES))
				<< 2);
	} else {
		FIXED x, z;

		x = pos[0];
		metatile = (Uint8 *)map->layers[0].metatiles;
		z = pos[2];
		metatile += (x / toFIXED(MAPGRID_METATILE_PIXELS) +
				z / toFIXED(MAPGRID_METATILE_PIXELS) * MAPGRID_SIZE
			<< 2);
	}
	return metatile[2];
}

/* The collision code of the metatile under the leader (on the world map, of its
   block), or 0 with no leader. Teleport's cues match it. */
Sint32 fdGetCollisionUnderLeader(void)
{
	FIELD_STATE	 *state = Ram_HeapSlots->fieldState;
	ACTOR		 *actor = fdGetPartyActor();
	MAP_STATE	 *map = Ram_HeapSlots->mapGrid;
	VECTOR3		  pos;
	MAP_METATILE *metatile;

	if(actor == NULL)
		return 0;
	SET_POSITION(&pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		metatile = (MAP_METATILE *)syRamMapBlocks +
			(((pos.x / toFIXED(WORLD_MAP_METATILE_PIXELS)) &
				 (WORLD_MAP_VIEW_METATILES - 1)) +
				((pos.z / toFIXED(WORLD_MAP_METATILE_PIXELS)) &
					(WORLD_MAP_VIEW_METATILES - 1)) *
					WORLD_MAP_VIEW_METATILES);
	} else {
		metatile = map->layers[0].metatiles;
		metatile += pos.x / toFIXED(MAPGRID_METATILE_PIXELS) +
			pos.z / toFIXED(MAPGRID_METATILE_PIXELS) * MAPGRID_SIZE;
	}
	return metatile->collisionCode;
}

/* Takes the exit of the first of the scene's exit regions that contains the
   position (x, y, z), in 16.16 pixels, and whose condition holds: records the
   region's exit value for fdModeRun, plays the exit sound and starts leaving
   the scene. */
void fdCheckExitRegions(FIXED posX, FIXED posY, FIXED posZ)
{
	Sint32		 selectedValue;
	FIELD_STATE *state;
	FIXED		 z;
	FIXED		 y;
	FIXED		 x;
	Sint16		 condition;
	Sint16		 minZ;
	Sint16		 maxY;
	Sint16		 minY;
	Sint32		 maxZ;
	Sint16		 maxX;
	Sint16		 minX;
	MAP_REGION	*region;

	x = posX;
	y = posY;
	z = posZ;
	region = (MAP_REGION *)SCENE_ENTRIES->regions();
	if(region != 0 &&
		(state = Ram_HeapSlots->fieldState, minX = region->minX, minX != -1)) {
	loop:
		minY = region->minY;
		minZ = region->minZ;
		maxX = region->maxX;
		maxY = region->maxY;
		maxZ = region->maxZ;
		condition = region->condition;
		selectedValue = region->value;
		if(fdGameFlagIsConditionActive(condition) != 0 && y >= toFIXED(minY) &&
			y < toFIXED(maxY) && x >= toFIXED(minX) && x < toFIXED(maxX) &&
			z >= toFIXED(minZ) && z < toFIXED(maxZ)) {
			state->exit = (Uint16)selectedValue;
			sySndPlayStub(SE_MAP_EXIT);
			fdLeaderStopMoving();
			return;
		}
		state = Ram_HeapSlots->fieldState;
		region++;
		minX = region->minX;
		if(minX != -1)
			goto loop;
	}
}

/* Brings the party's recovering Djinn one turn nearer, setting each that is
   ready, and returns 1 when any was set. */
Sint32 btDjinnRecoveryCountDownStub(void);

/* Handles one tick of the party leader's walking onto the metatile at (x, y,
   z) of map grid layer: records step cues and push metatiles, takes exits, and
   while walking advances the encounter counter, regenerates PP, hurts the party
   with poison and hazardous ground, and wears Avoid off. */
void fdUpdateWalking(Uint32 layer, FIXED x, FIXED y, FIXED z)
{
	FIELD_STATE	 *state = FieldStateWork;
	MAP_STATE	 *map = Ram_HeapSlots->mapGrid;
	Sint32		  leaderId = fdPartyGetLeaderActor();
	ACTOR		 *leader = fdGetPartyActor();
	Sint16		  prevHp[8];
	MAP_METATILE *tile;
	UNIT		 *unit;
	Sint32		  partyHurt;
	Uint32		  partyCnt;
	Uint32		  i;
	Uint32		  event;
	Uint32		  attributes;
	Sint32		  speed;
	Sint32		  step;
	Sint32		  amount;
	Sint32		  distance;
	Sint32		  zone;

	partyHurt = 0;
	if(state == NULL)
		return;

	partyCnt = gmPartyCountMembersStub();
	for(i = 0; i < partyCnt; i++)
		prevHp[i] = syUnitGet(MainRoutineStateWork.party[i])->hp;

	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		tile = &((MAP_METATILE *)
				syRamMapBlocks)[((x / toFIXED(WORLD_MAP_METATILE_PIXELS)) &
									(WORLD_MAP_VIEW_METATILES - 1)) +
			(((z / toFIXED(WORLD_MAP_METATILE_PIXELS)) &
				 (WORLD_MAP_VIEW_METATILES - 1)) *
				WORLD_MAP_VIEW_METATILES)];
		event = tile->collisionCode;
		attributes = 0;
	} else {
		if(layer <= 2)
			tile = map->layers[layer].metatiles;
		else
			tile = (MAP_METATILE *)Ram_MapMetatileBuffer;
		tile = &tile[x / toFIXED(MAPGRID_METATILE_PIXELS) +
			z / toFIXED(MAPGRID_METATILE_PIXELS) * MAPGRID_SIZE];
		event = tile->collisionCode;
		attributes = tile->attributeB;
	}
	state->prevMetatile = state->curMetatile;
	state->curMetatile = tile;
	if(attributes != 0)
		fdCheckExitRegions(x, y, z);
	if(event - METATILE_BEHAVIOR_CUE_FIRST <=
		METATILE_BEHAVIOR_CUE_LAST - METATILE_BEHAVIOR_CUE_FIRST)
		state->stepCue = event;
	if(attributes &
		(MAP_METATILE_PUSH | MAP_METATILE_LEDGE | MAP_METATILE_CLIMBABLE |
			MAP_METATILE_JUMP))
		state->pushMetatile = attributes;

	if((MainRoutineStateWork.movementMode <= LEADER_MOVEMENT_WORLD_MAP ||
		   MainRoutineStateWork.movementMode == LEADER_MOVEMENT_SHIP_ON_MAP ||
		   MainRoutineStateWork.movementMode == LEADER_MOVEMENT_SHIP) &&
		leader != NULL && leader->tgtX != ACTOR_NO_TARGET) {
		zone = 0;
		speed = leader->speedLimit;
		if(syGameFlagGet(FLAG_LURE_CAP))
			speed <<= 1;
		if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
			state->encounter =
				fdEncounterRollWorldMap((Sint32)&leader->x.fixed, speed);
		} else {
			if(attributes & MAP_METATILE_SECOND_ZONE)
				zone = 1;
			state->encounter = fdEncounterRollMapStep(zone, speed);
		}

		/* Every toFIXED(1) of distance walked (counted at half speed on ground
		   whose encounter zone is 0), the party regains PP, Djinn recover, and
		   poison hurts. */
		step = syMulFX(state->walkingDistanceScale, leader->speedLimit);
		if(!zone)
			step /= 2;
		distance = state->walkingDistance + step;
		state->walkingDistance = distance;
		if(distance >= toFIXED(1)) {
			amount = distance / toFIXED(1);
			state->walkingDistance = distance & (toFIXED(1) - 1);
			fdPartyAddPp(amount, 0);
			if(btDjinnRecoveryCountDownStub())
				sySndPlayStub(SE_DJINNI_SET);
			fdPartyClearUnusableShortcuts();
			partyHurt = fdPartyApplyPoisonDamage();
		}

		if(MainRoutineStateWork.hazardMode == HAZARD_COUNT_ON_METATILES &&
			(attributes & MAP_METATILE_HAZARD)) {
			if(state->prevMetatile->attributeB & MAP_METATILE_HAZARD)
				MainRoutineStateWork.hazardSteps +=
					leader->speedLimit / toFIXED(1);
			else
				MainRoutineStateWork.hazardSteps =
					MainRoutineStateWork.hazardLimit / 2;
		}

		if(MainRoutineStateWork.avoidDistance != 0 &&
			MainRoutineStateWork.sceneType != SCENE_TYPE_TOWN) {
			MainRoutineStateWork.avoidDistance -= leader->speedLimit;
			if(MainRoutineStateWork.avoidDistance <= 0) {
				MainRoutineStateWork.avoidDistance = 1;
				if(state->psynergy == 0)
					state->psynergy = ACT_PACKED | ABILITY_AVOID;
			}
		}
	}

	/* With HAZARD_COUNT_TICKS, which nothing in the game sets, the count rises
	   by one each call, that is each tick the field mode runs. */
	if(MainRoutineStateWork.hazardMode == HAZARD_COUNT_TICKS) {
		MainRoutineStateWork.hazardSteps++;
		if((Sint16)MainRoutineStateWork.hazardSteps ==
			MainRoutineStateWork.hazardLimit / 2)
			fdEffectSetAttached(
				leaderId, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
		if((Sint16)MainRoutineStateWork.hazardSteps ==
			MainRoutineStateWork.hazardLimit)
			fdEffectSetAttached(
				leaderId, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	}
	if((Sint16)MainRoutineStateWork.hazardSteps >=
		MainRoutineStateWork.hazardLimit) {
		amount = MainRoutineStateWork.hazardDamage;
		MainRoutineStateWork.hazardSteps = 0;
		fdPartyAddHp(
			-(amount & HAZARD_DAMAGE_AMOUNT), amount & HAZARD_DAMAGE_PERCENT);
		partyHurt++;
	}

	if(partyHurt) {
		state->fallenCnt = 0;
		state->survivorCnt = 0;
		fdEffectSetAttached(
			leaderId, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		for(i = 0; i < partyCnt; i++) {
			unit = syUnitGet(MainRoutineStateWork.party[i]);
			if(unit->hp > 0) {
				state->survivorCnt++;
			} else if(prevHp[i] != 0) {
				state->fallenOwners[state->fallenCnt++] =
					MainRoutineStateWork.party[i];
				state->raisedCue = FIELD_RAISED_CUE_PARTY_FELL;
				unit->poison = 0;
			}
		}
	}
}

/* Gives each character in the party amount PP (PP regenerates as the party
   walks). */
void fdPartyAddPp(Sint32 amount, Sint32 unused)
{
	Sint32 partyCnt = gmPartyCountMembersStub();
	Sint32 partyIdx;

	for(partyIdx = 0; partyIdx < partyCnt; partyIdx++)
		gmUnitAddPpStub(MainRoutineStateWork.party[partyIdx], amount);
}

/* Changes each party member's HP by amount, or by that percentage of maximum HP
   when scaled is set; a loss flashes the screen orange. */
void fdPartyAddHp(Sint32 amount, Sint32 scaled)
{
	Sint32 cnt;
	Sint32 i;
	Sint32 value;
	UNIT  *unit;
	if(amount < 0) {
		fdPaletteFadeSetSource(RGB(31, 15, 0), 0);
		fdPaletteFadeStartInterpolation(4);
		if(amount < -HEAVY_DAMAGE)
			sySndPlayStub(SE_HEAVY_IMPACT);
		else
			sySndPlayStub(SE_IMPACT);
	} else {
		sySndPlayStub(SE_RECOVERY);
	}
	cnt = gmPartyCountMembersStub();
	for(i = 0; i < cnt; i++) {
		unit = syUnitGet(MainRoutineStateWork.party[i]);
		if(!scaled) {
			value = amount;
		} else {
			value = unit->hpMax * amount / 100;
			/* XXX: a percentage that comes to 0 changes HP by amount's size as
			   a gain, a loss included; a hazard of 1% on a character under 100
			   maximum HP, watched, would show whether the game heals there. */
			if(value == 0) {
				value = amount;
				if(value < 0)
					value = -value;
			}
		}
		gmUnitAddHpStub(MainRoutineStateWork.party[i], value);
	}
}

/* Hurts each poisoned character in the party: poison takes a twentieth of their
   maximum HP and deadly poison a tenth, rounded to the nearest and at least 1.
   Returns the worst poison found: 0 none, 1 poison, 2 deadly poison. */
Sint32 fdPartyApplyPoisonDamage(void)
{
	Sint32 result = 0;
	Sint32 cnt = gmPartyCountMembersStub();

	if(result < cnt) {
		MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;
		Uint8			   *entry = state->party;
		Sint32				remaining = cnt;

		do {
			UNIT  *unit = syUnitGet(*entry);
			Sint32 amount;

			switch(unit->poison) {
			case POISON_NORMAL:
				amount = -((unit->hpMax + POISON_DAMAGE_DIVISOR / 2) /
					POISON_DAMAGE_DIVISOR);
				if(amount == 0)
					amount = -1;
				if(result <= 0)
					result = 1;
				break;
			case POISON_VENOM:
				amount = -((unit->hpMax + VENOM_DAMAGE_DIVISOR / 2) /
					VENOM_DAMAGE_DIVISOR);
				if(amount == 0)
					amount = -1;
				if(result <= 1)
					result = 2;
				break;
			default:
				amount = 0;
				break;
			}

			remaining--;
			gmUnitAddHpStub(*entry, amount);
			entry++;
		} while(remaining != 0);
	}

	if(result != 0) {
		fdPaletteFadeSetSource(RGB(31, 15, 0), 0);
		fdPaletteFadeStartInterpolation(4);
		sySndPlayStub(SE_IMPACT);
	}

	return result;
}

/* On the World Map, with no area graphics already pending: marks them pending,
   pauses the camera's actor and, when a screen transition is running, ends it
   at once and waits 2 ticks; then shows the still image
   (fdWorldMapStartStillImageStub). */
void fdWorldMapFreeze(void)
{
	FIELD_STATE *fieldState =
		syHeapSlotAllocEWRAM(HEAP_SLOT_FIELD, sizeof(*fieldState));

	/* The mode and areaGraphicsPending read as one halfword. */
	if(*(Uint16 *)&fieldState->mode == ACTOR_SYSTEM_WORLD_MAP) {
		SCREEN_TRANSITION_STATE *state;

		fieldState->areaGraphicsPending = 1;

		fieldState->camera->paused = 1;
		state =
			syHeapSlotAllocEWRAM(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));
		if(state != NULL && state->step != 0) {
			state->start = DISPLAY_HEIGHT / 2;
			state->end = 0;
			state->duration = 1;
			state->step = 0;
			sySynch(2);
		}
		fdWorldMapStartStillImageStub();
	}
}

/* On the World Map, loads the area graphics fdWorldMapFreeze left pending and
   lets the camera's actor move again. */
void fdWorldMapReloadPendingAreaGraphics(void)
{
	FIELD_STATE *state = syHeapSlotAllocEWRAM(HEAP_SLOT_FIELD, sizeof(*state));

	if(state->mode == ACTOR_SYSTEM_WORLD_MAP &&
		state->areaGraphicsPending != 0) {
		state->areaGraphicsPending = 0;
		state->camera->paused = 0;
		fdWorldMapLoadAreaGraphicsStub();
	}
}

/* Returns the actor that stands for the party: the leader's, or the first
   placed actor while MainRoutineStateWork.ownerOverride gives the party's
   control to it (the ship, when the party sails it). */
ACTOR *fdGetPartyActor(void)
{
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	FIELD_STATE		   *state = Ram_HeapSlots->fieldState;
	ACTOR			   *leader;

	if(party->ownerOverride == 0)
		leader = state->actors[party->leaderActorId];
	else
		leader = state->actors[ACTOR_FIRST_PLACED];
	return leader;
}

#undef HAZARD_DAMAGE_AMOUNT
#undef HEAVY_DAMAGE
#undef POISON_DAMAGE_DIVISOR
#undef VENOM_DAMAGE_DIVISOR
