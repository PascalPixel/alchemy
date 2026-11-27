/* Script commands that move the leader by the pad, on the map and the world
   map, and the configured keys checked from a script. Part of field.c. */

/* The flying ship's animation once it is 4 pixels up: its picture moved 10
   pixels (ANIMCMD_OFFSET_Y) and its wings beating, the seventh of
   winged_ship_flying_small.tsv. */
#define ANIM_SHIP_FLY_HIGH 6

/* A world map camera turn of less than this either way is made at once. */
#define WORLD_MAP_YAW_SNAP	16
/* The flying ship's camera turns toward the pad's yaw by at most this a
   frame. */
#define FLYING_YAW_STEP_MAX (TRIG_TURN / 2048)

/* SC_WORLD: moves the player on the world map by the direction pad, as
   SC_PLAYER does on the field, and turns the world map camera with the pad. */
Sint32 fdActorMoveOnWorldMap(ACTOR *actor)
{
	POS				position;
	POS				leftPosition;
	POS				rightPosition;
	Sint16			angleOffsets[22];
	Sint32			collision;
	Sint32			angle;
	Uint16			direction;
	Sint32			angleIndex;
	Sint32			movementAnimation;
	Sint32			count;
	Sint32			difference;
	Sint32			terrainCollision;
	FIXED			speed;
	Sint32			targetCameraYaw;
	WORLD_MAP_VIEW *camera;
	Sint32			yawStep;
	PLAYER_SPR	   *playerSprite;
	ACTOR		   *ripple;
	PLAYER_SPR	   *rippleSprite;
	FIELD_STATE	   *state;

	movementAnimation = ANIM_WALK;
	collision = 0;
	count = 0;
	if(PerWork.data & MainRoutineStateWork.dashKeys) {
		actor->speedLimit = WORLD_MAP_RUN_SPEED;
		actor->acceleration = WORLD_MAP_RUN_ACCELERATION;
		movementAnimation = ANIM_LEADER_RUN;
	} else {
		actor->speedLimit = WORLD_MAP_WALK_SPEED;
		actor->acceleration = WORLD_MAP_ACCELERATION;
	}

	angle = WorldMap_KeyHeadings[DPAD_INDEX(PerWork.data)];
	direction = angle;
	if(direction == WORLD_MAP_KEY_HEADING_NONE) {
		collision |= PLAYER_STEP_STOPPED;
	} else {
		collision = 0;
		SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
		syVectorAddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
		if(DebugMode == 0 ||
			((PerWork.data & KEY_L) == 0 &&
				syGameFlagGet(FLAG_DEBUG_STEPS_OFF) == 0)) {
			SET_POSITION(
				&leftPosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
			syVectorAddPolarOffset(WORLD_MAP_PROBE_RADIUS,
				direction + DEGtoANG(30), &leftPosition);
			SET_POSITION(
				&rightPosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
			syVectorAddPolarOffset(WORLD_MAP_PROBE_RADIUS,
				direction - DEGtoANG(30), &rightPosition);
			if((fdWorldMapIsWalkable(actor, &position) |
				   fdWorldMapIsWalkable(actor, &leftPosition) |
				   fdWorldMapIsWalkable(actor, &rightPosition)) != 0) {
				angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
				angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
				angleOffsets[count++] = direction + DEGtoANG(45);
				angleOffsets[count++] = direction - DEGtoANG(45);
				angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
				angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
				for(angleIndex = 0; angleIndex < count; angleIndex++) {
					direction = angleOffsets[angleIndex];
					SET_POSITION(&position, actor->x.fixed, actor->y.fixed,
						actor->z.fixed);
					syVectorAddPolarOffset(
						WORLD_MAP_PROBE_RADIUS, direction, &position);
					SET_POSITION(&leftPosition, actor->x.fixed, actor->y.fixed,
						actor->z.fixed);
					syVectorAddPolarOffset(WORLD_MAP_PROBE_RADIUS,
						direction + DEGtoANG(30), &leftPosition);
					SET_POSITION(&rightPosition, actor->x.fixed, actor->y.fixed,
						actor->z.fixed);
					syVectorAddPolarOffset(WORLD_MAP_PROBE_RADIUS,
						direction - DEGtoANG(30), &rightPosition);
					if((fdWorldMapIsWalkable(actor, &position) |
						   fdWorldMapIsWalkable(actor, &leftPosition) |
						   fdWorldMapIsWalkable(actor, &rightPosition)) != 0)
						continue;
					angle = (Sint16)direction;
					goto update_object;
				}
				collision |= PLAYER_STEP_BLOCKED_BY_MAP;
			}
		}
	}

update_object:
	state = Ram_HeapSlots->fieldState;
	if(state != NULL) {
		if((collision & PLAYER_STEP_BLOCKED) != 0)
			state->blockedSteps++;
		else
			state->blockedSteps = 0;
	}
	if(collision != 0)
		fdActorSelectAnimation(actor, ANIM_LEADER_LOOK_AROUND);
	else
		fdActorSelectAnimation(actor, movementAnimation);

	if(collision != 0) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		actor->velX = 0;
		actor->velZ = 0;
		if((collision & PLAYER_STEP_BLOCKED) != 0) {
			direction = angle;
			difference = (Sint16)(direction - actor->facing);
			if(difference > FIELD_BLOCKED_TURN_MAX)
				difference = FIELD_BLOCKED_TURN_MAX;
			if(difference < -FIELD_BLOCKED_TURN_MAX)
				difference = -FIELD_BLOCKED_TURN_MAX;
			actor->facing += difference;
		}
		actor->tmr = 0;
		actor->userValue = FOOTPRINT_BOTH_FEET;
	} else {
		fdActorSetMoveTarget(actor, position.x, position.y, position.z);
		speed = sySquartFX(syMulFX(actor->velX, actor->velX) +
			syMulFX(actor->velZ, actor->velZ));
		actor->velX = collision;
		actor->velZ = collision;
		syVectorAddPolarOffset(speed, (Uint16)angle, (POS *)&actor->velX);
		if(actor->tmr != 0)
			actor->tmr--;
	}

	camera = (WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid;
	targetCameraYaw = WorldMap_KeyYaws[DPAD_INDEX(PerWork.data)];
	yawStep = (Sint16)(targetCameraYaw - camera->yaw) / 8;
	if(yawStep > WORLD_MAP_YAW_STEP_MAX)
		yawStep = WORLD_MAP_YAW_STEP_MAX;
	if(yawStep < -WORLD_MAP_YAW_STEP_MAX)
		yawStep = -WORLD_MAP_YAW_STEP_MAX;
	if(yawStep > -WORLD_MAP_YAW_SNAP && yawStep < WORLD_MAP_YAW_SNAP)
		yawStep = targetCameraYaw - camera->yaw;
	camera->yaw += yawStep;

	if(actor->sprStorageKind == ACTOR_SPRITE_ONE) {
		playerSprite = actor->spr;
		terrainCollision = fdWorldMapGetCollision((WORLD_POS *)&actor->x.fixed);
		if(terrainCollision == WORLD_MAP_COLLISION_FOREST) {
			playerSprite->secondLayer->pri = 1;
			playerSprite->flg = 0;
		} else {
			playerSprite->secondLayer->pri = SPRITE_LAYER_NOT_DRAWN;
			playerSprite->flg = SPRITE_FLAG_SHADOW;
		}
		if(terrainCollision == WORLD_MAP_COLLISION_SHALLOW_WATER &&
			actor->tmr == 0 && collision == 0) {
			ripple = fdActorCreate(SPRITE_SPLASH_RING, actor->x.fixed,
				actor->y.fixed, actor->z.fixed);
			if(ripple != 0) {
				rippleSprite = ripple->spr;
				fdActorStartScript(ripple, Actor_RippleScript);
				ripple->motionFlg = collision;
				ripple->layer = 1;
				if(rippleSprite != 0) {
					fdSprStartAnimation(rippleSprite, 1);
					rippleSprite->flg = collision;
					rippleSprite->objMode = OBJ_MODE_SEMI_TRANSPARENT;
					rippleSprite->pri = 2;
				}
				actor->tmr = 10;
			}
		}
	}
	fdCheckConfiguredKeys();
	if(PerWork.push & MainRoutineStateWork.aKeys)
		FieldStateWork->raisedCue = WORLD_MAP_RAISED_BOARD_SHIP;
	actor->scriptCursor++;
	return 1;
}

/* SC_KEY_COUNT: checks the configured keys (fdCheckConfiguredKeys) and steps
   the script on. Returns 1. */
Sint32 fdCheckConfiguredKeysAndCount(ACTOR *actor)
{

	fdCheckConfiguredKeys();
	actor->scriptCursor++;
	return 1;
}

/* The ship turns toward the pad by this much a frame, once the turn left is
   wider than FIELD_BLOCKED_TURN_MAX; a narrower one it makes at once. */
#define SHIP_TURN_STEP (TRIG_TURN / 64)

/* SC_SAIL (LEADER_MOVEMENT_SHIP_ON_MAP): the ship moved by the direction pad on
   a field map, as the party's actor (MainRoutineStateWork.ownerOverride), as at
   Lemuria's entrance; A leaves the ship. */
Sint32 fdActorSailOnMap(ACTOR *interpreter)
{
	ACTOR *actor = interpreter;

	POS			 position;
	POS			 testPosition;
	POS			 contact;
	Sint16		 angleOffsets[22];
	Sint32		 collision;
	Sint32		 pushed;
	Sint16		 heading;
	Uint16		 angle;
	Uint16		 direction;
	Sint32		 angleIndex;
	Sint32		 headingCount;
	Bool		 onPlatform;
	Sint32		 difference;
	ACTOR		*other;
	FIELD_STATE *state;

	collision = 0;
	pushed = 0;
	headingCount = 0;
	/* Debug: holding L counts the free sprite VRAM blocks, cache entries,
	   actors and sprites. */
	/* DEAD: nothing reads the four counts, so the tests are gone; the ROM keeps
	   each loop's count and its branch (512, 96, 64 and 64 passes). */
	if(DebugMode != 0) {
		if(PerWork.data & KEY_L) {
			Sint32 freeBlocks = 0;
			Sint32 freeEntries = 0;
			Sint32 freeActors = 0;
			Sint32 freeSprites = 0;
			Sint32 i;

			for(i = 0; i < VRAM_BLOCK_COUNT; i++)
				if(VramCacheBlockOwners[i] == VRAM_BLOCK_OWNER_FREE)
					freeBlocks++;
			for(i = 0; i < VRAM_CACHE_ENTRY_COUNT; i++)
				if(VramCacheEntries[i].ofs == VRAM_CACHE_OFFSET_FREE)
					freeEntries++;
			for(i = 0; i < ACTOR_COUNT; i++)
				if(((ACTOR *)Ram_HeapSlots->actors)[i].script == NULL)
					freeActors++;
			for(i = 0; i < ANIM_OBJECT_COUNT; i++)
				if(((SPR *)Ram_HeapSlots->sprs)[i].width == 0)
					freeSprites++;
		}
	}
	actor->speedLimit = FIELD_WALK_SPEED;
	actor->acceleration = FIELD_ACCELERATION;
	if(syGameFlagGet(FLAG_FAST_WALK) != 0 && (PerWork.data & KEY_B)) {
		actor->speedLimit = FIELD_FAST_WALK_SPEED;
		actor->acceleration = FIELD_FAST_WALK_ACCELERATION;
	}

	angle = WorldMap_KeyHeadings[DPAD_INDEX(PerWork.data)];
	if((Uint16)angle == WORLD_MAP_KEY_HEADING_NONE) {
		collision |= PLAYER_STEP_STOPPED;
		goto counted;
	}
	collision = 0;
	SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	syVectorAddPolarOffset(toFIXED(19), (Uint16)angle, &position);
	if(actor->priFlg & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
		onPlatform = FALSE;
		other = (ACTOR *)Ram_HeapSlots->actors;
		for(angleIndex = 0; angleIndex < ACTOR_COUNT; angleIndex++, other++) {
			FIXED dy;

			if(other->script == NULL)
				continue;
			if(!(other->priFlg & ACTOR_PRIORITY_PLATFORM))
				continue;
			if(other == actor)
				continue;
			dy = actor->y.fixed - other->y.fixed;
			if((dy - toFIXED(16) < 0 ? -(dy - toFIXED(16)) : dy - toFIXED(16)) >
				toFIXED(8) - 1)
				continue;
			if(SAME_METATILE(actor->x.fixed, other->x.fixed) &&
				SAME_METATILE(actor->z.fixed, other->z.fixed))
				onPlatform = TRUE;
			if(SAME_METATILE(position.x, other->x.fixed) &&
				SAME_METATILE(position.z, other->z.fixed)) {
				collision = 0;
				heading = angle;
				goto counted;
			}
		}
		if(onPlatform && !fdMapGridCheckCollision(actor, &position)) {
			collision = 0;
			heading = angle;
			goto counted;
		}
	}
	if(DebugMode != 0 && (PerWork.data & KEY_L)) {
		collision = 0;
		heading = angle;
		goto counted;
	}
	if(!fdMapGridCheckCollision(actor, &position) &&
		SIDE_PROBES_CLEAR(fdMapGridCheckCollision, actor, toFIXED(19),
			(Uint16)angle, &testPosition)) {
		heading = angle;
		goto push_objects;
	}
	direction = angle;
	angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE;
	angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE;
	angleOffsets[headingCount++] = direction + DEGtoANG(45);
	angleOffsets[headingCount++] = direction - DEGtoANG(45);
	angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE * 3;
	angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE * 3;
	for(angleIndex = 0; angleIndex < headingCount; angleIndex++) {
		heading = angleOffsets[angleIndex];
		if(PROBE_CLEAR(fdMapGridCheckCollision, actor, toFIXED(19),
			   (Uint16)heading, &position) &&
			SIDE_PROBES_CLEAR(fdMapGridCheckCollision, actor, toFIXED(19),
				(Uint16)heading, &testPosition))
			goto push_objects;
	}
	SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	collision |= PLAYER_STEP_BLOCKED_BY_MAP;

push_objects:
	SET_POSITION(&contact, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	syVectorAddPolarOffset(toFIXED(15), (Uint16)heading, &contact);
	other = (ACTOR *)Ram_HeapSlots->actors;
	for(angleIndex = 0; angleIndex < ACTOR_COUNT; angleIndex++, other++) {
		Sint32 radius;

		radius = actor->radius - 2;
		if(other->script == NULL)
			continue;
		if(!(other->collisionFlg & ACTOR_SOLID))
			continue;
		if(other == actor)
			continue;
		if(fdCheckSpheresOverlap(
			   &other->x.fixed, radius, &contact.x, other->radius - 2) < 0)
			continue;
		if((other->collisionFlg & ACTOR_PUSHABLE) && other->paused == 0) {
			direction = syAtan(other->z.fixed - actor->z.fixed,
				other->x.fixed - actor->x.fixed);
			SET_POSITION(
				&testPosition, other->x.fixed, other->y.fixed, other->z.fixed);
			heading = direction;
			syVectorAddPolarOffset(toFIXED(1) / 4, direction, &testPosition);
			if(!fdActorScriptCheckOverlap(other, &testPosition.x)) {
				SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
					other->z.fixed);
				syVectorAddPolarOffset(toFIXED(21), direction, &testPosition);
				if(!fdMapGridCheckCollision(other, &testPosition)) {
					SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
						other->z.fixed);
					syVectorAddPolarOffset(toFIXED(21),
						direction + FIELD_PROBE_ANGLE, &testPosition);
					/* The same probe is tested twice. */
					if(!fdMapGridCheckCollision(other, &testPosition) &&
						!fdMapGridCheckCollision(other, &testPosition)) {
						SET_POSITION(&testPosition, other->x.fixed,
							other->y.fixed, other->z.fixed);
						syVectorAddPolarOffset(toFIXED(21),
							direction - FIELD_PROBE_ANGLE, &testPosition);
						if(!fdMapGridCheckCollision(other, &testPosition)) {
							syVectorAddPolarOffset(toFIXED(1) / 4, direction,
								(POS *)&other->x.fixed);
							other->tgtX = ACTOR_NO_TARGET;
							other->tgtY = ACTOR_NO_TARGET;
							other->tgtZ = ACTOR_NO_TARGET;
							pushed |= 1;
							continue;
						}
					}
				}
			}
		}
		collision |= PLAYER_STEP_BLOCKED_BY_ACTOR;
	}
	if(collision == 0 && pushed != 0) {
		actor->speedLimit = toFIXED(1) / 4;
		actor->acceleration = toFIXED(1) / 8;
	}

counted:
	state = Ram_HeapSlots->fieldState;
	if(state != NULL) {
		if(collision & PLAYER_STEP_BLOCKED)
			state->blockedSteps++;
		else
			state->blockedSteps = 0;
		state->heldKeys = PerWork.data;
	}
	if(pushed != 0 || collision != 0)
		fdActorSelectAnimation(actor, ANIM_STAND);
	else
		fdActorSelectAnimation(actor, ANIM_WALK);

	if(PerWork.push & MainRoutineStateWork.aKeys) {
		FieldStateWork->raisedCue = WORLD_MAP_RAISED_LEAVE_SHIP;
		collision |= PLAYER_STEP_STOPPED;
	}

	if(collision != 0) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		actor->velX = 0;
		actor->velZ = 0;
		if(collision & PLAYER_STEP_BLOCKED) {
			difference = (Sint16)((Uint16)angle - actor->facing);
			if(difference > FIELD_BLOCKED_TURN_MAX)
				difference = SHIP_TURN_STEP;
			if(difference < -FIELD_BLOCKED_TURN_MAX)
				difference = -SHIP_TURN_STEP;
			actor->facing += difference;
		}
		actor->tmr = 0;
		actor->userValue = FOOTPRINT_BOTH_FEET;
	} else {
		FIXED speed;

		fdActorSetMoveTarget(actor, position.x, position.y, position.z);
		speed = sySquartFX(syMulFX(actor->velX, actor->velX) +
			syMulFX(actor->velZ, actor->velZ));
		actor->velX = collision;
		actor->velZ = collision;
		syVectorAddPolarOffset(speed, (Uint16)heading, (POS *)&actor->velX);
		if(actor->tmr != 0)
			actor->tmr--;
		actor->steeringFlg &= ~ACTOR_FACES_MOTION;
		difference = (Sint16)((Uint16)angle - actor->facing);
		if(difference > FIELD_BLOCKED_TURN_MAX)
			difference = SHIP_TURN_STEP;
		if(difference < -FIELD_BLOCKED_TURN_MAX)
			difference = -SHIP_TURN_STEP;
		actor->facing += difference;
	}
	if(actor->sprStorageKind == ACTOR_SPRITE_ONE)
		fdWorldMapGetCollision((WORLD_POS *)&actor->x.fixed);
	fdCheckConfiguredKeys();
	actor->scriptCursor++;
	return 1;
}

/* SC_WORLD_SAIL (LEADER_MOVEMENT_SHIP): the ship sailing the world map. It
   keeps the kind of water under the ship in FIELD_STATE.terrainKind, sails at a
   run and turns a little at a time, leaving ripples; A goes ashore, and a dash
   key takes off once the ship has its wings (FIELD_STATE.hoverReady). */
Sint32 fdActorSailOnWorldMap(ACTOR *actor)
{
	POS				position;
	POS				testPosition;
	Sint16			angleOffsets[28];
	Sint32			collision;
	Sint32			angle;
	Uint16			direction;
	Sint32			angleIndex;
	Sint32			count;
	Sint32			difference;
	Sint32			terrain;
	FIXED			speed;
	Sint32			targetCameraYaw;
	WORLD_MAP_VIEW *camera;
	Sint32			yawStep;
	ACTOR		   *ripple;
	PLAYER_SPR	   *rippleSprite;
	FIELD_STATE	   *state;
	FIELD_STATE	   *worldState;
	Uint32			kind;

	count = 0;
	terrain = fdWorldMapGetCollision((WORLD_POS *)&actor->x.fixed);
	worldState = Ram_HeapSlots->fieldState;
	if(terrain >= WORLD_MAP_COLLISION_SEA_FIRST &&
		terrain < WORLD_MAP_COLLISION_SEA_LAST)
		worldState->terrainKind = SHIP_TERRAIN_SEA;
	if(terrain >= WORLD_MAP_COLLISION_WALKABLE_FIRST &&
		terrain <= WORLD_MAP_COLLISION_SHALLOW_WATER)
		worldState->terrainKind = SHIP_TERRAIN_SHALLOWS;
	if(terrain == WORLD_MAP_COLLISION_SEA_LAST)
		worldState->terrainKind = SHIP_TERRAIN_SEA_LAST;
	if(terrain >= WORLD_MAP_COLLISION_DEFAULT &&
		terrain <= WORLD_MAP_COLLISION_WALKABLE_LAST)
		worldState->terrainKind = SHIP_TERRAIN_GROUND;

	collision = 0;
	actor->speedLimit = toFIXED(5) / 4;
	actor->acceleration = toFIXED(1) / 2;
	angle = WorldMap_KeyHeadings[DPAD_INDEX(PerWork.data)];
	direction = angle;
	if(direction == WORLD_MAP_KEY_HEADING_NONE) {
		collision |= PLAYER_STEP_STOPPED;
	} else {
		SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
		syVectorAddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
		if(DebugMode == 0 || (PerWork.data & KEY_L) == 0) {
			if(fdWorldMapIsSailable(actor, &position) == 0 &&
				SIDE_PROBES_CLEAR(fdWorldMapIsSailable, actor,
					WORLD_MAP_PROBE_RADIUS, direction, &testPosition))
				goto update_object;
			angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
			angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
			angleOffsets[count++] = direction + DEGtoANG(45);
			angleOffsets[count++] = direction - DEGtoANG(45);
			angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
			angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
			for(angleIndex = 0; angleIndex < count; angleIndex++) {
				direction = angleOffsets[angleIndex];
				if(PROBE_CLEAR(fdWorldMapIsSailable, actor,
					   WORLD_MAP_PROBE_RADIUS, direction, &position) &&
					SIDE_PROBES_CLEAR(fdWorldMapIsSailable, actor,
						WORLD_MAP_PROBE_RADIUS, direction, &testPosition)) {
					angle = (Sint16)direction;
					goto update_object;
				}
			}
			collision |= PLAYER_STEP_BLOCKED_BY_MAP;
		}
	}

update_object:
	state = Ram_HeapSlots->fieldState;
	if(state != NULL) {
		if((collision & PLAYER_STEP_BLOCKED) != 0)
			state->blockedSteps++;
		else
			state->blockedSteps = 0;
		state->heldKeys = PerWork.data;
	}
	if(collision != 0)
		fdActorSelectAnimation(actor, ANIM_STAND);
	else
		fdActorSelectAnimation(actor, ANIM_WALK);

	if(PerWork.push & MainRoutineStateWork.aKeys) {
		FieldStateWork->raisedCue = WORLD_MAP_RAISED_LEAVE_SHIP;
		collision |= PLAYER_STEP_STOPPED;
	}
	if(PerWork.push & MainRoutineStateWork.dashKeys) {
		FIELD_STATE *fieldState = Ram_HeapSlots->fieldState;
		if(fieldState->hoverReady != 0) {
			syGameFlagClear(FLAG_SHIP_HOVERING);
			((FIELD_STATE *)fieldState)->raisedCue = WORLD_MAP_RAISED_TAKE_OFF;
		}
	}

	if(collision != 0) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		actor->velX = 0;
		actor->velZ = 0;
		if((collision & PLAYER_STEP_BLOCKED) != 0) {
			direction = angle;
			difference = (Sint16)(direction - actor->facing);
			if(difference > FIELD_BLOCKED_TURN_MAX)
				difference = FIELD_BLOCKED_TURN_MAX;
			if(difference < -FIELD_BLOCKED_TURN_MAX)
				difference = -FIELD_BLOCKED_TURN_MAX;
			actor->facing += difference;
		}
		actor->tmr = 0;
		actor->userValue = FOOTPRINT_BOTH_FEET;
	} else {
		fdActorSetMoveTarget(actor, position.x, position.y, position.z);
		speed = sySquartFX(syMulFX(actor->velX, actor->velX) +
			syMulFX(actor->velZ, actor->velZ));
		actor->velX = collision;
		actor->velZ = collision;
		direction = angle;
		syVectorAddPolarOffset(speed, direction, (POS *)&actor->velX);
		if(actor->tmr != 0)
			actor->tmr--;
		actor->steeringFlg &= ~ACTOR_FACES_MOTION;
		difference = (Sint16)(direction - actor->facing);
		if(difference > FIELD_BLOCKED_TURN_MAX)
			difference = SHIP_TURN_STEP;
		if(difference < -FIELD_BLOCKED_TURN_MAX)
			difference = -SHIP_TURN_STEP;
		actor->facing += difference;
	}

	camera = (WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid;
	targetCameraYaw = WorldMap_KeyYaws[DPAD_INDEX(PerWork.data)];
	yawStep = (Sint16)(targetCameraYaw - camera->yaw) / 8;
	if(yawStep > WORLD_MAP_YAW_STEP_MAX)
		yawStep = WORLD_MAP_YAW_STEP_MAX;
	if(yawStep < -WORLD_MAP_YAW_STEP_MAX)
		yawStep = -WORLD_MAP_YAW_STEP_MAX;
	if(yawStep > -WORLD_MAP_YAW_SNAP && yawStep < WORLD_MAP_YAW_SNAP)
		yawStep = targetCameraYaw - camera->yaw;
	camera->yaw += yawStep;

	kind = actor->sprStorageKind;
	if(kind == ACTOR_SPRITE_ONE) {
		fdWorldMapGetCollision((WORLD_POS *)&actor->x.fixed);
		if(actor->tmr == 0 && collision == 0) {
			ripple = fdActorCreate(SPRITE_SPLASH_RING, actor->x.fixed,
				actor->y.fixed, actor->z.fixed);
			if(ripple != 0) {
				rippleSprite = ripple->spr;
				fdActorStartScript(ripple, Actor_RippleScript);
				ripple->motionFlg = 0;
				ripple->layer = kind;
				if(rippleSprite != 0) {
					fdSprStartAnimation(rippleSprite, 2);
					rippleSprite->flg = 0;
					rippleSprite->objMode = OBJ_MODE_NORMAL;
					rippleSprite->pri = 2;
				}
				actor->tmr = 10;
			}
		}
	}
	fdCheckConfiguredKeys();
	actor->scriptCursor++;
	return 1;
}

/* SC_WORLD_FLY (LEADER_MOVEMENT_FLYING_SHIP): the ship with its wings on the
   world map, flying over the ground the sea alone would stop
   (fdWorldMapIsFlyable). A or B climbs, up to 12 pixels, and the ship sinks
   otherwise, faster once the party's PP is spent; coming down after a climb
   lands it. */
Sint32 fdActorFlyOnWorldMap(ACTOR *actor)
{
	POS				position;
	POS				testPosition;
	Sint16			angleOffsets[28];
	Sint32			collision;
	Sint32			angle;
	Uint16			direction;
	Sint32			angleIndex;
	Sint32			count;
	Sint32			difference;
	Sint32			terrain;
	FIXED			speed;
	FIXED			targetCameraYaw;
	WORLD_MAP_VIEW *camera;
	Sint32			yawStep;
	ACTOR		   *ripple;
	PLAYER_SPR	   *rippleSprite;
	FIELD_STATE	   *state;
	FIELD_STATE	   *worldState;
	ACTOR		   *other;
	Uint32			kind;
	FIELD_STATE	   *flightState;

	count = 0;
	terrain = fdWorldMapGetCollision((WORLD_POS *)&actor->x.fixed);
	worldState = Ram_HeapSlots->fieldState;
	if(terrain >= WORLD_MAP_COLLISION_SEA_FIRST &&
		terrain < WORLD_MAP_COLLISION_SEA_LAST)
		worldState->terrainKind = SHIP_TERRAIN_SEA;
	if(terrain >= WORLD_MAP_COLLISION_WALKABLE_FIRST &&
		terrain <= WORLD_MAP_COLLISION_SHALLOW_WATER)
		worldState->terrainKind = SHIP_TERRAIN_SHALLOWS;
	if(terrain == WORLD_MAP_COLLISION_SEA_LAST)
		worldState->terrainKind = SHIP_TERRAIN_SEA_LAST;
	if(terrain >= WORLD_MAP_COLLISION_DEFAULT &&
		terrain <= WORLD_MAP_COLLISION_WALKABLE_LAST)
		worldState->terrainKind = SHIP_TERRAIN_GROUND;

	actor->motionFlg = 0;
	collision = 0;
	actor->speedLimit = actor->y.fixed / 16 + toFIXED(1.25);
	actor->acceleration = toFIXED(1) / 2;
	angle = WorldMap_KeyHeadings[DPAD_INDEX(PerWork.data)];
	direction = angle;
	if(direction == WORLD_MAP_KEY_HEADING_NONE) {
		collision |= PLAYER_STEP_STOPPED;
	} else {
		SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
		syVectorAddPolarOffset(WORLD_MAP_PROBE_RADIUS, direction, &position);
		if(DebugMode == 0 || (PerWork.data & KEY_L) == 0) {
			if(fdWorldMapIsFlyable(actor, &position) == 0 &&
				SIDE_PROBES_CLEAR(fdWorldMapIsFlyable, actor,
					WORLD_MAP_PROBE_RADIUS, direction, &testPosition))
				goto check_actors;
			angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
			angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
			angleOffsets[count++] = direction + DEGtoANG(45);
			angleOffsets[count++] = direction - DEGtoANG(45);
			angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
			angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
			for(angleIndex = 0; angleIndex < count; angleIndex++) {
				direction = angleOffsets[angleIndex];
				if(PROBE_CLEAR(fdWorldMapIsFlyable, actor,
					   WORLD_MAP_PROBE_RADIUS, direction, &position) &&
					SIDE_PROBES_CLEAR(fdWorldMapIsFlyable, actor,
						WORLD_MAP_PROBE_RADIUS, direction, &testPosition)) {
					angle = (Sint16)direction;
					goto check_actors;
				}
			}
			collision |= PLAYER_STEP_BLOCKED_BY_MAP;

		check_actors:
			other = Ram_HeapSlots->actors;
			for(angleIndex = 0; angleIndex < ACTOR_COUNT;
				angleIndex++, other++) {
				if(other->script != NULL &&
					(other->collisionFlg & ACTOR_SOLID) && other != actor &&
					fdCheckSpheresOverlap(&other->x.fixed, other->radius,
						&position.x, actor->radius) >= 0)
					collision |= PLAYER_STEP_BLOCKED_BY_ACTOR;
			}
		}
	}
	state = Ram_HeapSlots->fieldState;
	if(state != NULL) {
		if((collision & PLAYER_STEP_BLOCKED) != 0)
			state->blockedSteps++;
		else
			state->blockedSteps = 0;
		state->heldKeys = PerWork.data;
	}
	if(actor->y.fixed >= toFIXED(4))
		fdActorSelectAnimation(actor, ANIM_SHIP_FLY_HIGH);
	else if(collision != 0)
		fdActorSelectAnimation(actor, ANIM_STAND);
	else
		fdActorSelectAnimation(actor, ANIM_WALK);

	if(collision != 0) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		actor->velX = 0;
		actor->velZ = 0;
		if((collision & PLAYER_STEP_BLOCKED) != 0) {
			direction = angle;
			difference = (Sint16)(direction - actor->facing);
			if(difference > FIELD_BLOCKED_TURN_MAX)
				difference = FIELD_BLOCKED_TURN_MAX;
			if(difference < -FIELD_BLOCKED_TURN_MAX)
				difference = -FIELD_BLOCKED_TURN_MAX;
			actor->facing += difference;
		}
		actor->tmr = 0;
		actor->userValue = FOOTPRINT_BOTH_FEET;
	} else {
		fdActorSetMoveTarget(actor, position.x, position.y, position.z);
		speed = sySquartFX(syMulFX(actor->velX, actor->velX) +
			syMulFX(actor->velZ, actor->velZ));
		actor->velX = collision;
		actor->velZ = collision;
		direction = angle;
		syVectorAddPolarOffset(speed, direction, (POS *)&actor->velX);
		if(actor->tmr != 0)
			actor->tmr--;
		actor->steeringFlg &= ~ACTOR_FACES_MOTION;
		difference = (Sint16)(direction - actor->facing);
		if(difference > FIELD_BLOCKED_TURN_MAX)
			difference = SHIP_TURN_STEP;
		if(difference < -FIELD_BLOCKED_TURN_MAX)
			difference = -SHIP_TURN_STEP;
		actor->facing += difference;
	}

	camera = (WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid;
	targetCameraYaw = syMulFX(WorldMap_KeyYaws[DPAD_INDEX(PerWork.data)],
		actor->y.fixed / 4 + toFIXED(1));
	yawStep = (Sint16)(targetCameraYaw - camera->yaw) / 8;
	if(yawStep > FLYING_YAW_STEP_MAX)
		yawStep = FLYING_YAW_STEP_MAX;
	if(yawStep < -FLYING_YAW_STEP_MAX)
		yawStep = -FLYING_YAW_STEP_MAX;
	if(yawStep >= -1 && yawStep <= 1)
		yawStep = targetCameraYaw - camera->yaw;
	camera->yaw += yawStep;

	kind = actor->sprStorageKind;
	if(kind == ACTOR_SPRITE_ONE) {
		if(actor->y.fixed < toFIXED(4)) {
			if(actor->tmr == 0 && collision == 0) {
				ripple = fdActorCreate(
					SPRITE_SPLASH_RING, actor->x.fixed, 0, actor->z.fixed);
				if(ripple != 0) {
					rippleSprite = ripple->spr;
					fdActorStartScript(ripple, Actor_RippleScript);
					ripple->motionFlg = 0;
					ripple->layer = kind;
					if(rippleSprite != 0) {
						fdSprStartAnimation(rippleSprite, 1);
						rippleSprite->flg = 0;
						rippleSprite->objMode = OBJ_MODE_SEMI_TRANSPARENT;
						rippleSprite->pri = 2;
					}
					actor->tmr = 10;
				}
			}
		} else {
			fdActorSetSprFlags(actor, FrameCount & kind);
		}
	}

	flightState = Ram_HeapSlots->fieldState;
	if(PerWork.data &
		(MainRoutineStateWork.aKeys | MainRoutineStateWork.bKeys)) {
		flightState->climbing = 1;
		actor->y.fixed += toFIXED(1) / 8;
	} else {
		actor->y.fixed -= toFIXED(1) / 4;
	}
	if(gmPartyGetPpRatioStub() == 0)
		actor->y.fixed -= toFIXED(1) / 4;
	if(actor->y.fixed <= 0 && flightState->climbing != 0)
		((FIELD_STATE *)flightState)->raisedCue = WORLD_MAP_RAISED_LAND;
	if(actor->y.fixed >= toFIXED(12))
		actor->y.fixed = toFIXED(12);
	if(actor->y.fixed <= 0)
		actor->y.fixed = 0;
	actor->scriptCursor++;
	return 1;
}
