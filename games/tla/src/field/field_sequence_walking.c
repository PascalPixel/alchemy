/* @file */
/* Field requests and each frame of the leader's walking: exits, encounters, */
/* damage. */
/* Part of field_sequence.c. */

#include "field/emote.h"

/* MAIN_ROUTINE_STATE.hazardDamage's amount, below HAZARD_DAMAGE_PERCENT. */
#define HAZARD_DAMAGE_AMOUNT 0xff
/* A loss of more HP than this plays SE_HEAVY_IMPACT rather than SE_IMPACT. */
#define HEAVY_DAMAGE 10
/* Poison takes 1/POISON_DAMAGE_DIVISOR of a character's maximum HP, rounded to
 */
/* the nearest, and venom ("deadly poison") 1/VENOM_DAMAGE_DIVISOR. */
#define POISON_DAMAGE_DIVISOR 20
#define VENOM_DAMAGE_DIVISOR  10

/* Unused: returns 0. */
UNUSED s32 fdSequenceReturnZero(void) { return 0; }

/* Clears the field's twelve pending requests (see FIELD_STATE). */
void fdClearRequests(void) {
	FIELD_STATE *state = FieldStateWork;

	state->stepTrigger = 0;
	state->pushCell = 0;
	state->exit = 0;
	state->fieldMenu = 0;
	state->aButton = 0;
	state->pauseMenu = 0;
	state->talk = 0;
	state->examineCell = 0;
	state->encounter = 0;
	state->psynergy = 0;
	state->item = 0;
	state->raisedTrigger = 0;
}

/* Returns the sum of the field's twelve pending requests: non-zero while any */
/* request is pending. */
s32 fdCountRequests(void) {
	FIELD_STATE *state = FieldStateWork;

	/* BUG: a sum can be 0 with requests pending. fdUpdateWalking raises */
	/* FIELD_RAISED_TRIGGER_PARTY_FELL (-1) on the frame a party member falls to
	 */
	/* poison, and the step trigger request is the cell's cell behavior, 1 on */
	/* step trigger 1's cells; together they sum to 0, and both wait until */
	/* another request breaks the tie. BUGFIX tests each request instead. */
#ifdef BUGFIX
	return state->stepTrigger | state->pushCell | state->exit |
		   state->fieldMenu | state->aButton | state->pauseMenu |
		   (s16)state->talk | state->examineCell | state->encounter |
		   state->psynergy | state->item | state->raisedTrigger;
#else
	return state->stepTrigger + state->pushCell + state->exit +
		   state->fieldMenu + state->aButton + state->pauseMenu + state->talk +
		   state->examineCell + state->encounter + state->psynergy +
		   state->item + state->raisedTrigger;
#endif
}

/* Sets the field state's hoverReady when the ship flies (FLAG_SHIP_FLIES) and
 */
/* a party member knows Hover; clears it otherwise. The world map's pad */
/* movement reads it. */
void fdUpdateHoverReady(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	if(syGameFlagGet(FLAG_SHIP_FLIES) &&
	   gmPartyFindOwnerWithActionStub(ABILITY_HOVER) != -1)
		state->hoverReady = 1;
	else
		state->hoverReady = 0;
}

/* Returns the cell behavior of the map cell 16 pixels ahead of the party */
/* leader (on the World Map, of its 32-pixel cell), or 0 without a leader. */
s32 fdGetCollisionAheadOfLeader(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *actor = fdGetPartyActor();
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	FIXED position[3];
	u8 *cell;

	if(actor == NULL)
		return 0;
	position[0] = actor->x.fixed;
	position[1] = actor->y.fixed;
	position[2] = actor->z.fixed;
	syVectorAddPolarOffset(toFIXED(16), actor->facing, position);
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		s32 x, z;

		x = position[0];
		z = position[2];
		cell = syRamMapBlocks + ((((x / toFIXED(WORLD_MAP_CELL_PIXELS)) &
								   (WORLD_MAP_VIEW_CELLS - 1)) +
								  (((z / toFIXED(WORLD_MAP_CELL_PIXELS)) &
									(WORLD_MAP_VIEW_CELLS - 1)) *
								   WORLD_MAP_VIEW_CELLS))
								 << 2);
	}
	else {
		FIXED x, z;

		x = position[0];
		cell = (u8 *)map->layers[0].cells;
		z = position[2];
		cell += (x / toFIXED(MAPGRID_CELL_PIXELS) +
					 z / toFIXED(MAPGRID_CELL_PIXELS) * MAPGRID_SIZE
				 << 2);
	}
	return cell[2];
}

/* The collision code of the cell under the leader (on the world map, of its */
/* block), or 0 with no leader. Teleport's triggers match it. */
s32 fdGetCollisionUnderLeader(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *actor = fdGetPartyActor();
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	VECTOR3 position;
	MAP_CELL *cell;

	if(actor == NULL)
		return 0;
	SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		cell = (MAP_CELL *)syRamMapBlocks +
			   (((position.x / toFIXED(WORLD_MAP_CELL_PIXELS)) &
				 (WORLD_MAP_VIEW_CELLS - 1)) +
				((position.z / toFIXED(WORLD_MAP_CELL_PIXELS)) &
				 (WORLD_MAP_VIEW_CELLS - 1)) *
					WORLD_MAP_VIEW_CELLS);
	}
	else {
		cell = map->layers[0].cells;
		cell += position.x / toFIXED(MAPGRID_CELL_PIXELS) +
				position.z / toFIXED(MAPGRID_CELL_PIXELS) * MAPGRID_SIZE;
	}
	return cell->collisionCode;
}

/* Takes the exit of the first of the scene's exit regions that contains the */
/* position (x, y, z), in 16.16 pixels, and whose condition holds: records the
 */
/* region's exit value for fdSequenceRun, plays the exit sound and starts */
/* leaving the scene. */
void fdCheckExitRegions(FIXED positionX, FIXED positionY, FIXED positionZ) {
	s32 selectedValue;
	FIELD_STATE *state;
	FIXED z;
	FIXED y;
	FIXED x;
	s16 condition;
	s16 minZ;
	s16 maxY;
	s16 minY;
	s32 maxZ;
	s16 maxX;
	s16 minX;
	MAP_REGION *region;

	x = positionX;
	y = positionY;
	z = positionZ;
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
		if(fdGameFlagIsConditionActive(condition) != 0 &&
		   y >= INTtoFIXED(minY) && y < INTtoFIXED(maxY) &&
		   x >= INTtoFIXED(minX) && x < INTtoFIXED(maxX) &&
		   z >= INTtoFIXED(minZ) && z < INTtoFIXED(maxZ)) {
			state->exit = (u16)selectedValue;
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

/* Brings the party's recovering Djinn one turn nearer, setting each that is */
/* ready, and returns 1 when any was set. */
s32 btDjinnRecoveryCountDownStub(void);

/* Handles one frame of the party leader's walking onto the cell at (x, y, z) */
/* of map grid layer: records step triggers and push cells, takes exits, and */
/* while walking advances the encounter counter, regenerates PP, hurts the */
/* party with poison and hazardous ground, and wears Avoid off. */
void fdUpdateWalking(u32 layer, FIXED x, FIXED y, FIXED z) {
	FIELD_STATE *state = FieldStateWork;
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s32 leaderId = fdPartyGetLeaderActor();
	ACTOR *leader = fdGetPartyActor();
	s16 previousHp[8];
	MAP_CELL *tile;
	UNIT *unit;
	s32 partyHurt;
	u32 partyCount;
	u32 i;
	u32 event;
	u32 attributes;
	s32 speed;
	s32 step;
	s32 amount;
	s32 distance;
	s32 zone;

	partyHurt = 0;
	if(state == NULL)
		return;

	partyCount = gmPartyCountMembersStub();
	for(i = 0; i < partyCount; i++)
		previousHp[i] = syUnitGet(MainRoutineStateWork.party[i])->hp;

	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		tile = &(
			(MAP_CELL *)syRamMapBlocks)[((x / toFIXED(WORLD_MAP_CELL_PIXELS)) &
										 (WORLD_MAP_VIEW_CELLS - 1)) +
										(((z / toFIXED(WORLD_MAP_CELL_PIXELS)) &
										  (WORLD_MAP_VIEW_CELLS - 1)) *
										 WORLD_MAP_VIEW_CELLS)];
		event = tile->collisionCode;
		attributes = 0;
	}
	else {
		if(layer <= 2)
			tile = map->layers[layer].cells;
		else
			tile = (MAP_CELL *)Ram_MapCellBuffer;
		tile = &tile[x / toFIXED(MAPGRID_CELL_PIXELS) +
					 z / toFIXED(MAPGRID_CELL_PIXELS) * MAPGRID_SIZE];
		event = tile->collisionCode;
		attributes = tile->attributeB;
	}
	state->prevCell = state->curCell;
	state->curCell = tile;
	if(attributes != 0)
		fdCheckExitRegions(x, y, z);
	if(event - CELL_BEHAVIOR_EVENT_FIRST <=
	   CELL_BEHAVIOR_EVENT_LAST - CELL_BEHAVIOR_EVENT_FIRST)
		state->stepTrigger = event;
	if(attributes &
	   (MAP_CELL_PUSH | MAP_CELL_LEDGE | MAP_CELL_CLIMBABLE | MAP_CELL_JUMP))
		state->pushCell = attributes;

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
				fdEncounterRollWorldMap((s32)&leader->x.fixed, speed);
		}
		else {
			if(attributes & MAP_CELL_SECOND_ZONE)
				zone = 1;
			state->encounter = fdEncounterRollMapStep(zone, speed);
		}

		/* Every FIXED_ONE of distance walked (counted at half speed on ground
		 * whose */
		/* encounter zone is 0), the party regains PP, Djinn recover, and poison
		 */
		/* hurts. */
		step = syMulFX(state->walkingDistanceScale, leader->speedLimit);
		if(!zone)
			step /= 2;
		distance = state->walkingDistance + step;
		state->walkingDistance = distance;
		if(distance >= FIXED_ONE) {
			amount = distance / FIXED_ONE;
			state->walkingDistance = distance & (FIXED_ONE - 1);
			fdPartyAddPp(amount, 0);
			if(btDjinnRecoveryCountDownStub())
				sySndPlayStub(SE_DJINNI_SET);
			fdPartyClearUnusableShortcuts();
			partyHurt = fdPartyApplyPoisonDamage();
		}

		if(MainRoutineStateWork.hazardMode == HAZARD_COUNT_ON_CELLS &&
		   (attributes & MAP_CELL_HAZARD)) {
			if(state->prevCell->attributeB & MAP_CELL_HAZARD)
				MainRoutineStateWork.hazardSteps +=
					leader->speedLimit / FIXED_ONE;
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

	/* With HAZARD_COUNT_FRAMES, which nothing in the game sets, the count rises
	 */
	/* by one each call, that is each frame the field sequence runs. */
	if(MainRoutineStateWork.hazardMode == HAZARD_COUNT_FRAMES) {
		MainRoutineStateWork.hazardSteps++;
		if((s16)MainRoutineStateWork.hazardSteps ==
		   MainRoutineStateWork.hazardLimit / 2)
			fdEffectSetAttached(leaderId, ATTACHED_EFFECT_IN_FRONT |
											  ATTACHED_EFFECT_ATTACH);
		if((s16)MainRoutineStateWork.hazardSteps ==
		   MainRoutineStateWork.hazardLimit)
			fdEffectSetAttached(leaderId, ATTACHED_EFFECT_IN_FRONT |
											  ATTACHED_EFFECT_REMOVE);
	}
	if((s16)MainRoutineStateWork.hazardSteps >=
	   MainRoutineStateWork.hazardLimit) {
		amount = MainRoutineStateWork.hazardDamage;
		MainRoutineStateWork.hazardSteps = 0;
		fdPartyAddHp(-(amount & HAZARD_DAMAGE_AMOUNT),
					 amount & HAZARD_DAMAGE_PERCENT);
		partyHurt++;
	}

	if(partyHurt) {
		state->fallenCnt = 0;
		state->survivorCnt = 0;
		fdEffectSetAttached(leaderId,
							ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		for(i = 0; i < partyCount; i++) {
			unit = syUnitGet(MainRoutineStateWork.party[i]);
			if(unit->hp > 0) {
				state->survivorCnt++;
			}
			else if(previousHp[i] != 0) {
				state->fallenOwners[state->fallenCnt++] =
					MainRoutineStateWork.party[i];
				state->raisedTrigger = FIELD_RAISED_TRIGGER_PARTY_FELL;
				unit->poison = 0;
			}
		}
	}
}

/* Gives each character in the party amount PP (PP regenerates as the party */
/* walks). */
void fdPartyAddPp(s32 amount, s32 unused) {
	s32 partyCount = gmPartyCountMembersStub();
	s32 partyIndex;

	for(partyIndex = 0; partyIndex < partyCount; partyIndex++)
		gmUnitAddPpStub(MainRoutineStateWork.party[partyIndex], amount);
}

/* Changes each party member's HP by amount, or by that percentage of maximum */
/* HP when scaled is set; a loss flashes the screen orange. */
void fdPartyAddHp(s32 amount, s32 scaled) {
	s32 count;
	s32 i;
	s32 value;
	UNIT *unit;
	if(amount < 0) {
		fdPaletteFadeSetSource(RGB(31, 15, 0), 0);
		fdPaletteFadeStartInterpolation(4);
		if(amount < -HEAVY_DAMAGE)
			sySndPlayStub(SE_HEAVY_IMPACT);
		else
			sySndPlayStub(SE_IMPACT);
	}
	else {
		sySndPlayStub(SE_RECOVERY);
	}
	count = gmPartyCountMembersStub();
	for(i = 0; i < count; i++) {
		unit = syUnitGet(MainRoutineStateWork.party[i]);
		if(!scaled) {
			value = amount;
		}
		else {
			value = unit->hpMax * amount / 100;
			/* XXX: a percentage that comes to 0 changes HP by amount's size as
			 * a */
			/* gain, a loss included; a hazard of 1% on a character under 100 */
			/* maximum HP, watched, would show whether the game heals there. */
			if(value == 0) {
				value = amount;
				if(value < 0)
					value = -value;
			}
		}
		gmUnitAddHpStub(MainRoutineStateWork.party[i], value);
	}
}

/* Hurts each poisoned character in the party: poison takes a twentieth of */
/* their maximum HP and deadly poison a tenth, rounded to the nearest and at */
/* least 1. Returns the worst poison found: 0 none, 1 poison, 2 deadly poison.
 */
s32 fdPartyApplyPoisonDamage(void) {
	s32 result = 0;
	s32 count = gmPartyCountMembersStub();

	if(result < count) {
		MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;
		u8 *entry = state->party;
		s32 remaining = count;

		do {
			UNIT *unit = syUnitGet(*entry);
			s32 amount;

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

/* On the World Map, with no area graphics already pending: marks them */
/* pending, pauses the camera's actor and, when a screen transition is */
/* running, ends it at once and waits 2 frames; then shows the still image */
/* (scWorldMapStartStillImageStub). */
void fdWorldMapFreeze(void) {
	FIELD_STATE *fieldState =
		syHeapSlotAllocEwram(HEAP_SLOT_FIELD, sizeof(*fieldState));

	/* The mode and areaGraphicsPending read as one halfword. */
	if(*(u16 *)&fieldState->mode == ACTOR_SYSTEM_WORLD_MAP) {
		SCR_TRANSITION_STATE *state;

		fieldState->areaGraphicsPending = 1;

		fieldState->camera->paused = 1;
		state =
			syHeapSlotAllocEwram(HEAP_SLOT_SCREEN_TRANSITION, sizeof(*state));
		if(state != NULL && state->step != 0) {
			state->start = DISPLAY_HEIGHT / 2;
			state->end = 0;
			state->duration = 1;
			state->step = 0;
			sySynch(2);
		}
		scWorldMapStartStillImageStub();
	}
}

/* On the World Map, loads the area graphics fdWorldMapFreeze left pending and
 */
/* lets the camera's actor move again. */
void fdWorldMapReloadPendingAreaGraphics(void) {
	FIELD_STATE *state = syHeapSlotAllocEwram(HEAP_SLOT_FIELD, sizeof(*state));

	if(state->mode == ACTOR_SYSTEM_WORLD_MAP &&
	   state->areaGraphicsPending != 0) {
		state->areaGraphicsPending = 0;
		state->camera->paused = 0;
		scWorldMapLoadAreaGraphicsStub();
	}
}

/* Returns the actor that stands for the party: the leader's, or the first */
/* placed actor while MainRoutineStateWork.ownerOverride gives the party's */
/* control to it (the ship, when the party sails it). */
ACTOR *fdGetPartyActor(void) {
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR *leader;

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
