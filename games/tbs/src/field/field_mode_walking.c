/* Field requests and each frame of the leader's walking: exits, encounters,
   damage. Part of field_mode.c. */

#include "field_mode_internal.h"

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

/* Returns the sum of the field's twelve pending requests (counting the talk
   request twice): non-zero while any request is pending. */
Sint32 fdCountRequests(void)
{
	FIELD_STATE *state = FieldStateWork;

	/* BUG: a sum can be 0 with requests pending. fdUpdateWalking raises
	   FIELD_RAISED_CUE_PARTY_FELL (-1) on the frame a party member falls to
	   poison, and the step cue request is the metatile's metatile behavior, 1
	   on step cue 1's metatiles (154 scenes have them); together they sum to 0,
	   and both wait until another request breaks the tie, by which time a later
	   metatile's behavior may have replaced the step cue. BUGFIX tests each
	   request instead. */
#if FIX_REQUEST_SUM_CANCELS
	return state->stepCue | state->pushMetatile | state->exit |
		state->fieldMenu | state->aButton | state->pauseMenu |
		(Sint16)state->talk | state->examineMetatile | state->encounter |
		state->psynergy | state->item | state->raisedCue;
#else
	return state->stepCue + state->pushMetatile + state->exit +
		state->fieldMenu + state->aButton + state->pauseMenu +
		(Sint16)state->talk + state->examineMetatile + (Sint16)state->talk +
		state->encounter + state->psynergy + state->item + state->raisedCue;
#endif
}

/* Returns the metatile behavior of the map metatile 16 pixels ahead of the
   party leader (a World Map metatile's tiles on the World Map), or 0 without a
   leader. */
Sint32 fdGetCollisionAheadOfLeader(void)
{
	FIELD_STATE			  *state;
	ACTOR				  *leader;
	MAP_GRID_SCROLL_STATE *gridState;
	POLAR_OFFSET_POS	   position;
	Uint8				  *tile;
	FIXED				   fixedX;
	FIXED				   fixedZ;

	state = FieldStateWork;
	leader = state->actors[MainRoutineStateWork.leaderActorId];

	gridState = MapGridState[0];

	if(leader == 0)
		return 0;

	position.vector.x = leader->x.fixed;
	position.vector.y = leader->y.fixed;
	position.vector.z = leader->z.fixed;
	syVectorAddPolarOffset(toFIXED(16), leader->facing, position.coordinates);

	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		/* World Map metatile tiles are 32 pixels square, 32 to a row. */
		Uint32 tileX;
		Uint32 tileY;

		tileX = position.vector.x / toFIXED(32) & 31;
		tileY = position.vector.z / toFIXED(32) & 31;

		tile = (Uint8 *)MetatileTiles + ((tileX + (tileY << 5)) << 2);
	} else {
		/* Map metatiles are 16 pixels square, 128 to a row. */
		tile = (Uint8 *)gridState->layers[MAPGRID_LAYER_GROUND].metatiles;

		fixedX = position.vector.x;
		{
			Uint32 tileX = fixedX / toFIXED(MAPGRID_METATILE_PIXELS);

			fixedZ = position.vector.z;

			tile = (Uint8 *)((Uint32)tile +
				((tileX +
					 fixedZ / toFIXED(MAPGRID_METATILE_PIXELS) * MAPGRID_SIZE)
					<< 2));
		}
	}

	return tile[2];
}

/* Takes the exit of the first of the scene's exit regions that contains the
   position (x, y, z), in 16.16 pixels, and whose condition holds: records the
   region's exit value for fdModeRun, plays the exit sound and starts leaving
   the scene. */
void fdCheckExitRegions(FIXED positionX, FIXED positionY, FIXED positionZ)
{
	Sint32					 exitValue;
	FIELD_STATE				*state;
	FIXED					 fixedZ;
	FIXED					 fixedY;
	FIXED					 fixedX;
	Sint16					 condition;
	Sint16					 minZ;
	Sint16					 maxY;
	Sint16					 minY;
	Sint32					 maxZ;
	Sint16					 maxX;
	Sint16					 minX;
	const SCENE_EXIT_REGION *region;

	fixedX = positionX;
	fixedY = positionY;
	fixedZ = positionZ;
	region = OverlayArea.regions();
	if(region != 0 &&
		(state = FieldStateWork, minX = region->minX, minX != -1)) {
	loop:
		minY = region->minY;
		minZ = region->minZ;
		maxX = region->maxX;
		maxY = region->maxY;
		maxZ = region->maxZ;
		condition = region->condition;
		exitValue = region->value;
		if(fdGameFlagIsConditionActive(condition) != 0 &&
			fixedY >= toFIXED(minY) && fixedY < toFIXED(maxY) &&
			fixedX >= toFIXED(minX) && fixedX < toFIXED(maxX) &&
			fixedZ >= toFIXED(minZ) && fixedZ < toFIXED(maxZ)) {
			state->exit = exitValue;
			sySndPlayStub(SE_MAP_EXIT);
			fdLeaderStopMoving();
			return;
		}
		state = FieldStateWork;
		region++;
		minX = region->minX;
		if(minX != -1)
			goto loop;
	}
}

/* Handles one frame of the party leader's walking onto the metatile at (x, y,
   z) of map grid layer: records step cues and push metatiles, takes exits, and
   while walking advances the encounter counter, regenerates PP, hurts the party
   with poison and hazardous ground, and wears Avoid off. */
void fdUpdateWalking(Sint32 layer, FIXED x, FIXED y, FIXED z)
{
	FIELD_STATE			  *state = FieldStateWork;
	MAP_GRID_SCROLL_STATE *gridState = MapGridState[0];
	Sint32				   leaderId = MainRoutineStateWork.leaderActorId;
	ACTOR				  *leader = state->actors[leaderId];
	Sint16				   previousHp[8];
	MAP_GRID_METATILE	  *tile;
	UNIT				  *unitVitals;
	Uint32				   partyCount;
	Sint32				   partyHurt;
	Uint32				   i;
	Uint32				   event;
	Sint32				   encounterSpeed;
	Sint32				   stepDistance;
	Sint32				   amount;
	Sint32				   accumulatedDistance;
	Sint32				   fullEncounterRate;

	partyHurt = 0;
	if(state == NULL)
		return;

	/* Note each character's HP, to see who falls. */
	partyCount = gmPartyCountMembersStub();
	for(i = 0; i < partyCount; i++)
		previousHp[i] = gmUnitGetStub(MainRoutineStateWork.party[i])->hp;

	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		tile =
			&((MAP_GRID_METATILE *)Ram_MetatileTiles)[((x / toFIXED(32)) & 31) +
				(((z / toFIXED(32)) & 31) << 5)];
	} else {
		if((Uint32)layer <= MAPGRID_LAYER_OVERHEAD)
			tile = gridState->layers[layer].metatiles;
		else
			tile = (MAP_GRID_METATILE *)MapGridMetatileBuffer;
		tile = &tile[(x / toFIXED(MAPGRID_METATILE_PIXELS)) +
			(z / toFIXED(MAPGRID_METATILE_PIXELS)) * MAPGRID_SIZE];
	}
	event = tile->behavior;
	state->prevMetatile = state->curMetatile;
	state->curMetatile = tile;
	/* Exit regions are checked on any marked metatile; the behavior then picks
	   the request it raises. */
	if(event != 0)
		fdCheckExitRegions(x, y, z);
	if(event >= METATILE_BEHAVIOR_CUE_FIRST &&
		event <= METATILE_BEHAVIOR_CUE_LAST)
		state->stepCue = event;
	if(event >= METATILE_BEHAVIOR_LEDGE &&
		event <= METATILE_BEHAVIOR_JUMP_FORWARD)
		state->pushMetatile = event;

	/* Only while walking (not climbing) towards a target. */
	if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_WALK &&
		leader != NULL && leader->tgtX != ACTOR_NO_TARGET) {
		/* Encounters: the zone comes from the World Map's encounter region and
		   battle environment, or from the ground the leader stands on. */
		encounterSpeed = leader->speedLimit;
		if(gmFlagGetStub(FLAG_LURE_CAP))
			encounterSpeed <<= 1;
		if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
			fullEncounterRate = 1;
			state->encounter = fdEncounterRollWorldMap(
				(Sint32)&leader->x.fixed, encounterSpeed);
		} else if(event == METATILE_BEHAVIOR_ENCOUNTER_GROUND_A ||
			event == METATILE_BEHAVIOR_ENCOUNTER_GROUND_B) {
			fullEncounterRate = state->encounterZones[event -
				(METATILE_BEHAVIOR_ENCOUNTER_GROUND_A - 1)];
			state->encounter = fdEncounterRollMapStep(
				event - (METATILE_BEHAVIOR_ENCOUNTER_GROUND_A - 1),
				encounterSpeed);
		} else {
			fullEncounterRate = state->encounterZones[0];
			state->encounter = fdEncounterRollMapStep(0, encounterSpeed);
		}

		/* Every toFIXED(1) of distance walked (counted at half speed on ground
		   whose encounter zone is 0; fullEncounterRate holds that zone, 1 on
		   the World Map), the party regains PP, Djinn recover, and poison
		   hurts. */
		stepDistance = syMulFX(state->walkingDistanceScale, leader->speedLimit);
		if(!fullEncounterRate)
			stepDistance /= 2;
		accumulatedDistance = state->walkingDistance + stepDistance;
		state->walkingDistance = accumulatedDistance;
		if(accumulatedDistance >= toFIXED(1)) {
			amount = accumulatedDistance / toFIXED(1);
			state->walkingDistance = accumulatedDistance & (toFIXED(1) - 1);
			fdPartyAddPp(amount, 0);
			if(btDjinnRecoveryCountDownStub())
				sySndPlayStub(SE_DJINNI_SET);
			fdPartyClearUnusableShortcuts();
			partyHurt = fdPartyApplyPoisonDamage();
		}

		/* Hazardous ground: entering it starts the count half way; then each
		   frame walked on it adds the leader's speed limit in whole pixels, so
		   the count follows the distance walked there (the Lamakan Desert's
		   limit is 600). */
		if(MainRoutineStateWork.hazardMode == HAZARD_COUNT_ON_METATILES &&
			event == METATILE_BEHAVIOR_DESERT_SAND) {
			if(state->prevMetatile->behavior == METATILE_BEHAVIOR_DESERT_SAND)
				MainRoutineStateWork.hazardSteps +=
					leader->speedLimit / toFIXED(1);
			else
				MainRoutineStateWork.hazardSteps =
					MainRoutineStateWork.hazardLimit / 2;
		}

		/* Avoid wears off with distance, outside towns. */
		if(MainRoutineStateWork.avoidDistance != 0 &&
			MainRoutineStateWork.sceneType != SCENE_TYPE_TOWN) {
			MainRoutineStateWork.avoidDistance -= leader->speedLimit;
			if(MainRoutineStateWork.avoidDistance <= 0) {
				MainRoutineStateWork.avoidDistance = 1;
				/* Raise Avoid as a Psynergy request that pays no PP; Avoid's
				   end handler says it wore off. */
				if(state->psynergy == 0)
					state->psynergy = ACT_PACKED | ABILITY_AVOID;
			}
		}
	}

	/* With HAZARD_COUNT_FRAMES, which nothing in the game sets, the count rises
	   by one each call, that is each frame the field mode runs. */
	if(MainRoutineStateWork.hazardMode == HAZARD_COUNT_FRAMES) {
		MainRoutineStateWork.hazardSteps++;
		if(MainRoutineStateWork.hazardSteps ==
			MainRoutineStateWork.hazardLimit / 2)
			fdEffectSetAttached(
				leaderId, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
		if(MainRoutineStateWork.hazardSteps == MainRoutineStateWork.hazardLimit)
			fdEffectSetAttached(
				leaderId, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	}
	if(MainRoutineStateWork.hazardSteps >= MainRoutineStateWork.hazardLimit) {
		/* hazardDamage: the amount below HAZARD_DAMAGE_PERCENT, which makes it
		   a percentage of maximum HP. */
		amount = MainRoutineStateWork.hazardDamage;
		MainRoutineStateWork.hazardSteps = 0;
		fdPartyAddHp(-(amount & (HAZARD_DAMAGE_PERCENT - 1)),
			amount & HAZARD_DAMAGE_PERCENT);
		partyHurt++;
	}

	/* The leader flinches (velocityY, a hop of 4 pixels a frame) and shows a
	   one-off SPRITE_ATTACHED_EFFECT; each character whose HP reached 0 on this
	   frame is listed, loses its poison and raises FIELD_RAISED_CUE_PARTY_FELL,
	   which fdModeRun handles. */
	if(partyHurt) {
		state->fallenCnt = 0;
		state->survivorCnt = 0;
		leader->velY = toFIXED(4);
		fdEffectSetAttached(
			leaderId, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		for(i = 0; i < partyCount; i++) {
			unitVitals = gmUnitGetStub(MainRoutineStateWork.party[i]);
			if(unitVitals->hp > 0) {
				state->survivorCnt++;
			} else if(previousHp[i] != 0) {
				state->fallenOwners[state->fallenCnt++] =
					MainRoutineStateWork.party[i];
				state->raisedCue = FIELD_RAISED_CUE_PARTY_FELL;
				unitVitals->poison = 0;
			}
		}
	}
}

/* Gives each character in the party amount PP (PP regenerates as the party
   walks). */
void fdPartyAddPp(Sint32 amount, Sint32 unused)
{
	Sint32 partyCount = gmPartyCountMembersStub();
	Sint32 partyIndex;

	for(partyIndex = 0; partyIndex < partyCount; partyIndex++)
		gmUnitAddPpStub(MainRoutineStateWork.party[partyIndex], amount);
}

/* Damage past this plays SE_HEAVY_IMPACT rather than SE_IMPACT. */
#define HEAVY_DAMAGE 10

/* Changes each party member's HP by amount, or by that percentage of maximum HP
   when percent is set. */
void fdPartyAddHp(Sint32 amount, Sint32 percent)
{
	Sint32 partyCount;
	Sint32 partyIndex;
	Sint32 hpDelta;
	UNIT  *unit;

	if(amount < 0) {
		/* The screen flashes orange. */
		fdPaletteFadeSetSource(RGB(31, 15, 0), PALETTES_ALL);
		fdPaletteFadeStartInterpolation(4);
		if(amount < -HEAVY_DAMAGE)
			sySndPlayStub(SE_HEAVY_IMPACT);
		else
			sySndPlayStub(SE_IMPACT);
	} else {
		sySndPlayStub(SE_RECOVERY);
	}

	partyCount = gmPartyCountMembersStub();
	for(partyIndex = 0; partyIndex < partyCount; partyIndex++) {
		unit = gmUnitGetStub(MainRoutineStateWork.party[partyIndex]);
		if(!percent) {
			hpDelta = amount;
		} else {
			hpDelta = unit->hpMax * amount / 100;
			/* The fallback drops the damage's sign, so a damaging percentage
			   that truncates to zero heals instead. */
			if(hpDelta == 0) {
				hpDelta = amount;
				if(hpDelta < 0)
					hpDelta = -hpDelta;
			}
		}
		gmUnitAddHpStub(MainRoutineStateWork.party[partyIndex], hpDelta);
	}
}

/* The share of maximum HP poison and deadly poison take each time, as one part
   in this many. */
#define POISON_HP_PARTS 20
#define VENOM_HP_PARTS	10

/* Hurts each poisoned character in the party: poison takes a twentieth of their
   maximum HP and deadly poison a tenth, rounded to the nearest and at least
   1. */
Sint32 fdPartyApplyPoisonDamage(void)
{
	Sint32 worstPoison = 0;
	Sint32 partyCount = gmPartyCountMembersStub();
	Sint32 partyIndex;

	for(partyIndex = 0; partyIndex < partyCount; partyIndex++) {
		UNIT  *unit = gmUnitGetStub(MainRoutineStateWork.party[partyIndex]);
		Sint32 hpDelta;

		switch(unit->poison) {
		case POISON_NORMAL:
			hpDelta = -((unit->hpMax + POISON_HP_PARTS / 2) / POISON_HP_PARTS);
			if(hpDelta == 0)
				hpDelta = -1;
			if(worstPoison <= 0)
				worstPoison = POISON_NORMAL;
			break;
		case POISON_VENOM:
			hpDelta = -((unit->hpMax + VENOM_HP_PARTS / 2) / VENOM_HP_PARTS);
			if(hpDelta == 0)
				hpDelta = -1;
			if(worstPoison <= POISON_NORMAL)
				worstPoison = POISON_VENOM;
			break;
		default:
			hpDelta = 0;
			break;
		}
		gmUnitAddHpStub(MainRoutineStateWork.party[partyIndex], hpDelta);
	}

	if(worstPoison != 0) {
		fdPaletteFadeSetSource(RGB(31, 15, 0), PALETTES_ALL);
		fdPaletteFadeStartInterpolation(4);
		sySndPlayStub(SE_IMPACT);
	}

	return worstPoison;
}

/* Before a script or menu on the World Map: ends any screen transition in
   progress, stops the camera and holds the view as a still image. */
void fdWorldMapFreeze(void)
{
	FIELD_STATE				*fieldState;
	SCREEN_TRANSITION_STATE *state;

	fieldState = syHeapSlotAllocEWRAM(HEAP_SLOT_FIELD, sizeof(*fieldState));
	if(fieldState->mode == ACTOR_SYSTEM_WORLD_MAP) {
		state = syHeapSlotAllocEWRAM(
			HEAP_SLOT_SCREEN_TRANSITION, sizeof(SCREEN_TRANSITION_STATE));
		if((state != NULL) && (state->step != 0)) {
			state->start = 0;
			state->end = 0;
			state->duration = 1;
			state->step = 0;
		}
		fieldState->camera->paused = 1;
		fdWorldMapStartStillImageStub();
	}
}

/* Ends what fdWorldMapFreeze started: on the World Map, reloads the area's
   graphics and lets the camera move again. */
void fdWorldMapUnfreeze(void)
{
	FIELD_STATE *state;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_FIELD, sizeof(*state));
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		fdWorldMapLoadAreaGraphicsStub();
		state->camera->paused = 0;
	}
}

#undef HEAVY_DAMAGE
#undef POISON_HP_PARTS
#undef VENOM_HP_PARTS
