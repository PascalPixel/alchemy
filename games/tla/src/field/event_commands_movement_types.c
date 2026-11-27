/* @file */
/* Actors' movement types and what they attach, and the leader's moves off the
 */
/* ground: jumping a gap, climbing a wall up and down, and stepping onto and */
/* off a rope or tightrope. */
/* Part of event_commands.c. */

extern const u8 Actor_WaitScript[];

/* The event command "actor set attached effect", by ATTACHED_EFFECT_FLAG: */
/* removes the actor attached to an actor, attaches one */
/* (SPRITE_RAY_ARC, animation 1) unless one already is, or shows a */
/* one-off one (animation 2) that ends by itself (Actor_WaitScript). */
void fdEffectSetAttached(s32 actorId, s32 flags) {
	ACTOR *actor = fdActorLookup(actorId);
	ACTOR *effect;
	SPR *visual;
	s32 mode;

	effect = 0;
	visual = 0;

	if(actor == 0)
		return;

	if((flags & 3) != 0) {
		if((flags & 3) == ATTACHED_EFFECT_ONCE ||
		   fdActorIsNotAttachedEffect((ACTOR *)actor->watchedActor)) {
			effect = fdActorCreateStub(SPRITE_RAY_ARC, actor->x.fixed,
									   actor->y.fixed, actor->z.fixed);
		}
	}
	else {
		effect = (ACTOR *)actor->watchedActor;
		if(effect == 0)
			return;
		fdActorReleaseStub(effect);
		actor->watchedActor = NULL;
		return;
	}

	if(effect == 0)
		return;

	mode = flags & 3;
	switch(mode) {
	case ATTACHED_EFFECT_ATTACH:
		fdActorSelectAnimationStub(effect, 1);
		actor->watchedActor = (union FIELD_OBJECT *)effect;
		effect->tmr = 1;
		break;
	case ATTACHED_EFFECT_ONCE:
		fdActorSelectAnimationStub(effect, 2);
		fdActorStartScriptStub(effect, Actor_WaitScript);
		effect->tmr = 1;
		break;
	}

	effect->userValue = actorId;
	effect->motionFlg = 0;
	effect->update = (OBJECT_UPDATE)fdEffectCopyWatchedActorPos;
	visual = effect->spr;
	visual->flg = 0;
	effect->watchedActor = (union FIELD_OBJECT *)actor;

	/* The effect takes sprite priority 1, or the actor's. */
	if(flags & ATTACHED_EFFECT_IN_FRONT)
		((PLACED_SPR *)visual)->pri = 1;
	else
		((PLACED_SPR *)visual)->pri = ((PLACED_SPR *)actor->spr)->pri;
}

/* The SC_CALL function of MOVEMENT_TYPE_CLEAR_COLLISION: clears an actor's */
/* sprite flags and its collision flags, so nothing collides with it. */
s32 fdActorClearSprAndCollisionFlags(void *obj) {
	fdActorSetSprFlagsStub(obj, 0);
	((ACTOR *)obj)->collisionFlg = 0;
	return 0;
}

/* Actor script hook: clears the actor's sprite flags (outline and shadow). */
s32 fdActorClearSprFlags(ACTOR *obj) {
	fdActorSetSprFlagsStub((ACTOR *)obj, 0);
	return 0;
}

/* Actor script hook: the actor takes its cells' priority even where the */
/* ground in front rises. */
s32 fdActorIgnoreRises(ACTOR *actor) {
	actor->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	return 0;
}

/* The SC_CALL function of MOVEMENT_TYPE_FOLLOW_WATCHED: walks toward the */
/* watched actor, setting out each frame for 16/timer of the way there, and */
/* stands once within its timer's pixels of it. */
s32 fdActorMoveTowardTarget(ACTOR *actor) {
	s32 step;
	ACTOR *target;
	FIXED deltaX;
	FIXED deltaZ;
	s32 pixelsX;
	s32 pixelsZ;
	FIXED newX;
	FIXED newZ;
	s32 distance;

	target = (ACTOR *)actor->watchedActor;
	if(target != 0) {
		/* Whole pixels, rounded toward zero. */
		deltaX = target->x.fixed - actor->x.fixed;
		pixelsX = deltaX / FIXED_ONE;
		deltaZ = target->z.fixed - actor->z.fixed;
		pixelsZ = deltaZ / FIXED_ONE;
		distance = sySquart(pixelsX * pixelsX + pixelsZ * pixelsZ);
		step = actor->tmr;
		if(distance >= step) {
			/* A target 16 pixels on for each pixel of distance, over the step.
			 */
			newX = actor->x.fixed + (s32)((u32)pixelsX * toFIXED(16)) / step;
			newZ = actor->z.fixed + (s32)((u32)pixelsZ * toFIXED(16)) / step;
			fdActorSetMoveTargetStub(actor, newX, actor->y.fixed, newZ);
			fdActorSelectAnimationStub(actor, ANIM_WALK);
		}
		else {
			fdActorSelectAnimationStub(actor, ANIM_STAND);
		}
	}
	return 1;
}

/* The SC_CALL function of MOVEMENT_TYPE_FACE_LEADER: turns the actor toward */
/* its watched actor, at most 1/16 turn a frame. */
s32 fdActorTurnTowardWatchedActor(ACTOR *actor) {
	s16 turnStep;
	u16 currentAngle;
	s32 targetAngle;
	ACTOR *target;

	target = (ACTOR *)actor->watchedActor;
	if(target != NULL) {
		/* Clear ACTOR_FACES_MOTION, so moving does not turn it away again. */
		actor->steeringFlg &= ~ACTOR_FACES_MOTION;
		targetAngle = (u16)syAtan(target->z.fixed - actor->z.fixed,
								  target->x.fixed - actor->x.fixed);
		currentAngle = actor->facing;
		turnStep = targetAngle - currentAngle;
		if(turnStep != 0) {
			if(turnStep > FACING_STEP)
				turnStep = FACING_STEP;
			if(turnStep < -FACING_STEP)
				turnStep = -FACING_STEP;
			actor->facing = (currentAngle + turnStep);
		}
	}
	return 1;
}

/* Keeps an actor turning to face another. */
void fdActorWatchActor(ACTOR *actor, ACTOR *target) {
	actor->watchedActor = (union FIELD_OBJECT *)target;
	actor->update = fdActorTurnTowardWatchedActor;
}

/* Gives an actor the leader's sprite priority, so it is drawn in front of */
/* and behind what he is, and keeps it there. */
s32 fdActorTakeLeaderLayer(ACTOR *actor) {
	ACTOR *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	s32 layer = ((PLACED_SPR *)leader->spr)->pri;

	if((actor->sprStorageKind & 0xf) == ACTOR_SPRITE_ONE) {
		PLACED_SPR *sprite = actor->spr;

		sprite->pri = layer;
		sprite->shadowPri = layer;
		actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	}
	return 0;
}

extern const u8 MovementType_StandScript[];
extern const u8 MovementType_WanderScript[];
extern const u8 MovementType_WanderNearScript[];
extern const u8 MovementType_WanderCloseScript[];
extern const u8 MovementType_FollowWatchedScript[];
extern const u8 MovementType_FaceLeaderScript[];
extern const u8 MovementType_ClearCollisionScript[];
extern const u8 MovementType_SquashScript[];
extern const u8 MovementType_TakeLeaderLayerScript[];
extern const u8 MovementType_IgnoreRisesScript[];
extern const u8 MovementType_ClearSpriteFlagsScript[];

/* Starts an actor's movement type: an MOVEMENT_TYPE, or any other value as
 */
/* the actor script to run itself. */
void fdActorSetMovementType(ACTOR *actor, s32 kind) {
	switch(kind) {
	case MOVEMENT_TYPE_STAND:
		kind = (s32)MovementType_StandScript;
		break;
	case MOVEMENT_TYPE_WANDER:
		kind = (s32)MovementType_WanderScript;
		break;
	case MOVEMENT_TYPE_WANDER_NEAR:
		kind = (s32)MovementType_WanderNearScript;
		break;
	case MOVEMENT_TYPE_WANDER_CLOSE:
		kind = (s32)MovementType_WanderCloseScript;
		break;
	case MOVEMENT_TYPE_FOLLOW_WATCHED:
		kind = (s32)MovementType_FollowWatchedScript;
		break;
	case MOVEMENT_TYPE_FACE_LEADER:
		actor->watchedActor = (union FIELD_OBJECT *)fdActorGetById(
			MainRoutineStateWork.leaderActorId);
		kind = (s32)MovementType_FaceLeaderScript;
		break;
	case MOVEMENT_TYPE_CLEAR_COLLISION:
		kind = (s32)MovementType_ClearCollisionScript;
		break;
	case MOVEMENT_TYPE_SQUASH:
		kind = (s32)MovementType_SquashScript;
		break;
	case MOVEMENT_TYPE_TAKE_LEADER_LAYER:
		kind = (s32)MovementType_TakeLeaderLayerScript;
		break;
	case MOVEMENT_TYPE_IGNORE_RISES:
		kind = (s32)MovementType_IgnoreRisesScript;
		break;
	case MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS:
		kind = (s32)MovementType_ClearSpriteFlagsScript;
		break;
	}
	fdActorStartScriptStub(actor, (const void *)kind);
}

/* fdActorFindNearestFacingTarget's reach in pixels, and the distance past */
/* which an actor counts only within 3/16 turn of the facing. */
#define FACING_TARGET_REACH 40
#define FACING_TARGET_NEAR	23

/* Finds the nearest animated actor to self within FACING_TARGET_REACH and 48 */
/* pixels of height, ignoring actors more than 3/16 turn to either side once */
/* they are past FACING_TARGET_NEAR; returns it if its first sprite animation */
/* is sprite id, as The Broken Seal's. */
ACTOR *fdActorFindNearestFacingTarget(ACTOR *self, s32 id) {
	ACTOR *entry;
	ACTOR *found;
	s32 cnt;
	s32 best;
	FIXED dy;
	s32 dx;
	s32 dz;
	s32 dist;
	s32 angle;
	s32 turn;

	found = NULL;
	best = FACING_TARGET_REACH;
	entry = Ram_HeapSlots->actors;
	for(cnt = 0; cnt < ACTOR_COUNT; cnt++, entry++) {
		if(entry->script == NULL)
			continue;
		if(entry == self)
			continue;
		if(entry->sprStorageKind != ACTOR_SPRITE_ONE)
			continue;
		dy = entry->y.fixed - self->y.fixed;
		if(dy >= 0) {
			if(dy >= toFIXED(48))
				continue;
		}
		else {
			if(self->y.fixed - entry->y.fixed >= toFIXED(48))
				continue;
		}
		dx = (entry->x.fixed - self->x.fixed) / FIXED_ONE;
		dz = (entry->z.fixed - self->z.fixed) / FIXED_ONE;
		dist = sySquart(dx * dx + dz * dz);
		if(dist >= best)
			continue;
		angle = (u16)syAtan(entry->z.fixed - self->z.fixed,
							entry->x.fixed - self->x.fixed);
		if(dist > FACING_TARGET_NEAR) {
			turn = (s16)(angle - self->facing);
			if(turn < -(TRIG_TURN * 3 / 16 - 1))
				continue;
			if(turn > TRIG_TURN * 3 / 16 - 1)
				continue;
		}
		found = entry;
		best = dist;
	}
	if(found == NULL)
		return NULL;
	if(((SPR *)found->spr)->entries[0]->anmId != id)
		return NULL;
	return found;
}

/* Whether the ground at position stops a jump: anything but flat ground or a */
/* plain slope (TERRAIN_FLAT to TERRAIN_SLOPE_Y) on a field map; never on the */
/* world map or with no map. */
s32 fdMapGridBlocksJump(ACTOR *actor, s32 *position) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	FIELD_STATE *event = Ram_HeapSlots->fieldState;

	if(map == 0 || event->mode == ACTOR_SYSTEM_WORLD_MAP ||
	   fdMapGridGetTerrainKindStub(actor->layer, position[0], position[2]) <=
		   TERRAIN_SLOPE_Y)
		return 0;
	return 1;
}

extern MAP_CELL MapGridCellBuffer[];

/* The middle pixel of the map cell a whole pixel lies in. */
#define CELL_MIDDLE(pixels)                                                    \
	(MAPGRID_CELL_PIXELS / 2 + ((pixels) & MAPGRID_CELL_PIXEL_MASK))

/* The leader jumps two cells forward, over the cell in front, when it faces */
/* a quarter turn, the map allows it (fdMapGridBlocksJump and the step check) */
/* or a platform actor (ACTOR_PRIORITY_PLATFORM) waits 16 pixels below the */
/* landing, and no solid actor stands there. A log underfoot (a */
/* SPRITE_LOG_WEST_EAST or SPRITE_LOG_NORTH_SOUTH under the party) dips 2 */
/* pixels with the leader. Each jump adds to the walking distance, and the */
/* leader keeps jumping while it lands on CELL_BEHAVIOR_JUMP_FORWARD. Returns */
/* 0 after a jump, -1 when the leader cannot jump. */
s32 fdTryJumpForward(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	s32 result = -1;
	s32 angle = leader->facing;
	u8 *flags = &leader->motionFlg;
	u8 saved = *flags;
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s16 spriteFlags = SPRITE_FLAG_SHADOW;
	FIELD_COORDINATE position[3];
	ACTOR *entry;
	ACTOR *platform;
	s32 canJump;
	s32 i;

	if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_KEYS)
		return result;
	if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_ROPE)
		return result;
	if((angle + FACING_STEP) & DEGtoANG(45)) /* not facing a quarter turn */
		return result;
	angle = (angle + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
again:
	/* The middle of the tile in front, then of the tile beyond. */
	position[0].fixed = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
	position[1].fixed = leader->y.fixed;
	position[2].fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
	syVectorAddPolarOffset(toFIXED(16), angle, &position[0].fixed);
	if(fdMapGridCheckCollisionStub(leader, &position[0].fixed) == 1)
		return -1;
	position[0].fixed = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
	position[1].fixed = leader->y.fixed;
	position[2].fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
	syVectorAddPolarOffset(toFIXED(32), angle, &position[0].fixed);
	canJump = 0;
	if(fdMapGridBlocksJump(leader, &position[0].fixed) == 0 &&
	   fdMapGridCheckCollisionStub(leader, &position[0].fixed) == 0)
		canJump = 1;

	/* A platform 16 pixels below the landing makes it a jump spot. */
	if(leader->priFlg & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
		entry = Ram_HeapSlots->actors;
		for(i = 0; i < ACTOR_COUNT; i++, entry++) {
			s32 bound = toFIXED(8) - 1;
			FIXED delta;
			FIXED dy;

			if(entry->script == NULL)
				continue;
			if(!(entry->priFlg & ACTOR_PRIORITY_PLATFORM))
				continue;
			if(entry->arrivalAxis != 0)
				continue;
			if(entry == leader)
				continue;
			if(FIELD_CELL(position[0].fixed - entry->x.fixed + toFIXED(8)) != 0)
				continue;
			if(FIELD_CELL(position[2].fixed - entry->z.fixed + toFIXED(8)) != 0)
				continue;
			delta = position[1].fixed - entry->y.fixed;
			dy = delta - toFIXED(16);
			if(dy >= 0) {
				if(dy <= bound)
					goto hit;
			}
			else {
				dy = toFIXED(16) - delta;
				if(dy <= bound) {
				hit:
					canJump = 1;
					break;
				}
			}
		}
	}

	/* A solid object at the landing blocks it. */
	entry = Ram_HeapSlots->actors;
	for(i = 0; i < ACTOR_COUNT; i++, entry++) {
		if(entry->script != NULL &&
		   !(entry->priFlg & ACTOR_PRIORITY_PLATFORM) && entry != leader &&
		   entry->collisionFlg != 0 &&
		   FIELD_CELL(position[0].fixed - entry->x.fixed + toFIXED(8)) == 0 &&
		   FIELD_CELL(position[2].fixed - entry->z.fixed + toFIXED(8)) == 0) {
			FIXED d = position[1].fixed - entry->y.fixed;
			if(d >= 0) {
				if(d < toFIXED(8))
					goto blocked;
			}
			else if(entry->y.fixed - position[1].fixed < toFIXED(8)) {
			blocked:
				canJump = 0;
				break;
			}
		}
	}

	if(canJump) {
		MAP_STATE *map;
		u8 attr;

		/* Crouch, then jump there. */
		if(leader->sprStorageKind == ACTOR_SPRITE_ONE)
			spriteFlags = ((SPR *)leader->spr)->flg;
		fdEventStart();
		fdActorSelectAnimationStub(leader, ANIM_LEADER_SPRING);
		sySynch(6);
		sySndPlayStub(SE_JUMP);
		fdActorSelectAnimationStub(leader, ANIM_LEADER_LEAP);
		leader->speedLimit = toFIXED(3);
		leader->acceleration = toFIXED(2);
		leader->velY = toFIXED(4);
		*flags &= ACTOR_JUMP_KEEPS_MOTION;
		fdActorSetSprFlagsStub(leader, spriteFlags & (u8)~SPRITE_FLAG_SHADOW);
		fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId,
								 position[0].part[0].integer,
								 position[2].part[0].integer);
		fdActorSelectAnimationStub(leader, ANIM_LEADER_SPRING);
		fdActorSetSprFlagsStub(leader, spriteFlags);

		map = Ram_HeapSlots->mapGrid;
		attr = map->groundEffs;
		if(attr & GROUND_EFFECTS_WADING_MASK) {
			s32 cellX = FIELD_CELL(leader->x.fixed);
			s32 cellZ = FIELD_CELL(leader->z.fixed);
			MAP_CELL *cells;

			if(map == NULL)
				cells = MapGridCellBuffer;
			else
				cells = map->layers[leader->layer & 3].cells;
			cells += cellX + cellZ * MAPGRID_SIZE;
			if(cells->attributeB & MAP_CELL_WATER)
				fdActorSpawnWaterSplashStub(leader,
											GROUND_EFFECTS_WADING(attr));
		}

		/* A log underfoot dips 2 pixels with the leader and comes back up. */
		if(((platform = fdActorFindNearestFacingTarget(
				 leader, SPRITE_LOG_WEST_EAST)) != NULL ||
			(platform = fdActorFindNearestFacingTarget(
				 leader, SPRITE_LOG_NORTH_SOUTH)) != NULL) &&
		   (platform->priFlg & ACTOR_PRIORITY_UNDERFOOT)) {
			fdActorSelectAnimationStub(platform, 7);
			leader->y.fixed -= toFIXED(1);
			leader->groundHeight -= toFIXED(1);
			sySynch(2);
			leader->y.fixed -= toFIXED(1);
			leader->groundHeight -= toFIXED(1);
			sySynch(10);
			leader->y.fixed += toFIXED(1);
			leader->groundHeight += toFIXED(1);
			sySynch(4);
			leader->y.fixed += toFIXED(1);
			leader->groundHeight += toFIXED(1);
		}
		else {
			sySynch(6);
		}
		*flags = saved;
		fdEventEnd();
		if(state != NULL)
			state->walkingDistance +=
				syMulFX(state->walkingDistanceScale, toFIXED(32));
		if(fdMapGridGetCellBehaviorAtStub(leader->layer, position[0].fixed,
										  position[2].fixed) ==
		   CELL_BEHAVIOR_JUMP_FORWARD) {
			fdActorSelectAnimationStub(leader, ANIM_STAND);
			sySynch(6);
			goto again;
		}
		result = 0;
	}
	leader->tmr = 8;
	leader->userValue = 0;
	return result;
}

/* Climbs down onto the wall below the leader, as The Broken Seal's */
/* fdClimbDown does, or lets go and falls when it cannot. */
s32 fdClimbDown(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s32 gridX;
	s16 spriteFlags;
	MAP_CELL *ground;
	s32 gridY;
	s32 gridZ;
	FIXED position[3];

	spriteFlags = SPRITE_FLAG_SHADOW;
	gridX = CELL_MIDDLE(FIXEDtoINT(leader->x.fixed));
	gridY = CELL_MIDDLE(FIXEDtoINT(leader->y.fixed));
	gridZ = CELL_MIDDLE(FIXEDtoINT(leader->z.fixed));
	fdEventStart();
	if(leader->sprStorageKind == ACTOR_SPRITE_ONE)
		spriteFlags = ((SPR *)leader->spr)->flg;
	if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_WALK) {
		ACTOR *entry;
		MAP_CELL *cell;
		MAP_CELL *south;
		s32 cellX = gridX / 16;
		s32 cellY = gridY / 16;
		s32 cellZ = gridZ / 16;
		s32 radius;
		s32 i;

		ground =
			&map->layers[leader->layer].cells[cellX + cellZ * MAPGRID_SIZE];
		cell = ground;
		south = cell + MAPGRID_SIZE;
		if(!(cell->attributeB & MAP_CELL_CLIMBABLE) ||
		   !(south->attributeB & MAP_CELL_CLIMBABLE))
			goto failure;
		cell = &map->layers[MAPGRID_LAYER_OVERHEAD]
					.cells[cellX + (cellZ - cellY) * MAPGRID_SIZE];
		south = cell + MAPGRID_SIZE;
		if(south->collisionCode == CELL_BEHAVIOR_WALL)
			goto failure;
		if(fdMapGridGetTerrainHeightStub(leader->layer, INTtoFIXED(gridX),
										 INTtoFIXED(gridZ)) <=
		   fdMapGridGetTerrainHeightStub(leader->layer, INTtoFIXED(gridX),
										 INTtoFIXED(gridZ + 16)))
			goto failure;
		position[0] = leader->x.fixed;
		position[1] = leader->y.fixed - toFIXED(16);
		position[2] = leader->z.fixed;
		if(fdMapGridIsWallAtStub(position) != 0)
			goto failure;
		radius = leader->radius - 2;
		position[0] = leader->x.fixed;
		position[1] = leader->y.fixed - toFIXED(32);
		position[2] = leader->z.fixed + toFIXED(16);
		entry = Ram_HeapSlots->actors;
		for(i = 0; i < ACTOR_COUNT; i++, entry++) {
			if(entry->script != NULL && (entry->collisionFlg & ACTOR_SOLID) &&
			   entry != leader &&
			   fdCheckSpheresOverlapStub(&entry->x.fixed, radius, position,
										 entry->radius - 2) >= 0)
				goto failure;
		}

		/* Step to the middle of the tile, face north and climb down a tile. */
		fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId, gridX,
								 gridZ);
		leader->speedLimit = toFIXED(1);
		fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId,
								 FACING_NORTH, 0);
		fdActorWaitForScriptEndById(MainRoutineStateWork.leaderActorId);
		leader->steeringFlg = ACTOR_FACES_MOTION;
		leader->motionFlg = 0;
		spriteFlags &= (u8)~SPRITE_FLAG_SHADOW;
		fdActorSetSprFlagsStub(leader, spriteFlags);
		fdActorSelectAnimationStub(leader, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
		fdActorSetMoveTargetStub(leader, INTtoFIXED(gridX),
								 leader->y.fixed - toFIXED(16),
								 INTtoFIXED(gridZ) + toFIXED(16));
		fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_KEYS;
		leader->z.fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
	}
	else {
		/* Let go of the wall and fall. */
		fdActorSelectAnimationStub(leader, ANIM_LEADER_CLIMB);
		leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		leader->velY = toFIXED(4);
		leader->groundHeight = leader->y.fixed;
		spriteFlags |= SPRITE_FLAG_SHADOW;
		fdActorSetSprFlagsStub(leader, spriteFlags);
		fdEventWait(6);
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
		leader->steeringFlg = ACTOR_FACES_MOTION;
		leader->facing = TRIG_TURN * 3 / 4;
		leader->tmr = 0;
		leader->userValue = 0;
	}
	fdEventEnd();
	return 0;

failure:
	fdEventEnd();
	return -1;
}

/* Climbs up the wall in front of the leader, as The Broken Seal's */
/* fdClimbUp does. */
s32 fdClimbUp(void) {
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s16 spriteFlags = SPRITE_FLAG_SHADOW;
	MAP_CELL *ground;
	s32 gridX = CELL_MIDDLE(FIXEDtoINT(leader->x.fixed));
	s32 gridY = MAPGRID_CELL_PIXELS / 2 +
				(FIXEDtoINT(leader->y.fixed) & ~(MAPGRID_CELL_PIXELS - 1));
	s32 gridZ = CELL_MIDDLE(FIXEDtoINT(leader->z.fixed));
	FIXED position[3];
	s32 result;

	fdEventStart();
	if(leader->sprStorageKind == ACTOR_SPRITE_ONE)
		spriteFlags = ((SPR *)leader->spr)->flg;
	if(MainRoutineStateWork.movementMode == LEADER_MOVEMENT_WALK) {
		MAP_CELL *cell;
		MAP_CELL *north;
		s32 cellX = gridX >> 4;
		s32 cellY = gridY >> 4;
		s32 cellZ = gridZ >> 4;

		ground =
			&map->layers[leader->layer].cells[cellX + cellZ * MAPGRID_SIZE];
		cell = ground;
		north = cell - MAPGRID_SIZE;
		if(!(cell->attributeB & MAP_CELL_CLIMBABLE) ||
		   !(north->attributeB & MAP_CELL_CLIMBABLE))
			goto failure;
		cell = &map->layers[MAPGRID_LAYER_OVERHEAD]
					.cells[cellX + (cellZ - cellY) * MAPGRID_SIZE];
		north = cell - MAPGRID_SIZE;
		if(north->collisionCode == CELL_BEHAVIOR_WALL)
			goto failure;
		if(fdMapGridGetTerrainHeightStub(leader->layer, INTtoFIXED(gridX),
										 INTtoFIXED(gridZ)) >=
		   fdMapGridGetTerrainHeightStub(leader->layer, INTtoFIXED(gridX),
										 INTtoFIXED(gridZ - 16)))
			goto failure;
		position[0] = leader->x.fixed;
		position[1] = leader->y.fixed;
		position[2] = leader->z.fixed;
		result = fdMapGridIsWallAtStub(position);
		if(result != 0)
			goto failure;

		/* Step to the middle of the tile, crouch, jump and grab the wall. */
		leader->steeringFlg = result;
		fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId, gridX,
								 gridZ);
		fdActorSelectAnimationStub(leader, ANIM_LEADER_SPRING);
		sySynch(4);
		fdActorSelectAnimationStub(leader, ANIM_LEADER_LEAP);
		leader->velY = toFIXED(4);
		sySynch(4);
		leader->motionFlg = result;
		spriteFlags &= (u8)~SPRITE_FLAG_SHADOW;
		fdActorSetSprFlagsStub(leader, spriteFlags);
		leader->speedLimit = toFIXED(1);
		leader->velY = result;
		fdActorSelectAnimationStub(leader, ANIM_LEADER_CLING);
		sySynch(4);
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_KEYS;
		leader->steeringFlg = ACTOR_FACES_MOTION;
		sySynch(8);
		leader->z.fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
	}
	else {
		ACTOR *entry;
		s32 radius = leader->radius - 2;
		s32 i;

		position[0] = leader->x.fixed;
		position[1] = leader->y.fixed + toFIXED(16);
		position[2] = leader->z.fixed - toFIXED(16);
		i = 0;
		entry = Ram_HeapSlots->actors;
		for(; i < ACTOR_COUNT; i++, entry++) {
			if(entry->script != NULL && (entry->collisionFlg & ACTOR_SOLID) &&
			   entry != leader &&
			   fdCheckSpheresOverlapStub(&entry->x.fixed, radius, position,
										 entry->radius - 2) >= 0)
				goto failure;
		}

		/* Climb over the top: up 8 pixels and a tile north. */
		leader->motionFlg = 0;
		fdActorSelectAnimationStub(leader, ANIM_LEADER_CLIMB_OVER_TOP);
		fdActorSetMoveTargetStub(leader, INTtoFIXED(gridX),
								 leader->y.fixed + toFIXED(8),
								 INTtoFIXED(gridZ) - toFIXED(16));
		fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
		leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		leader->groundHeight = leader->y.fixed;
		spriteFlags |= SPRITE_FLAG_SHADOW;
		fdActorSetSprFlagsStub(leader, spriteFlags);
		fdEventWait(4);
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
		leader->steeringFlg = ACTOR_FACES_MOTION;
		leader->tmr = 0;
		leader->userValue = 0;
	}
	fdEventEnd();
	return 0;

failure:
	fdEventEnd();
	return -1;
}

/* How near a rope or tightrope node must be for the leader to hop onto it, */
/* in pixels. */
#define ROPE_NODE_REACH 32

/* Returns the nearest crossable rope node within reach of position, or NULL. */
ROPE_NODE *fdRopeFindNodeNear(s32 *position) {
	ROPE_NODE *found;
	ROPE_NODE *result;
	ROPE_WORK *state;
	ROPE_NODE *node;
	s32 best;
	s32 i;

	found = NULL;
	state = ROPE_STATE;
	if(state == NULL) {
		result = NULL;
	}
	else {
		node = state->nodes;
		best = ROPE_NODE_DISTANCE_NONE;
		for(i = 0; i < ROPE_NODE_COUNT; i++, node++) {
			if(node->kind == ROPE_NODE_CROSSABLE) {
				/* In pixels, squared. */
				s32 dx = (position[0] - node->x) >> 8;
				s32 dy = (position[1] - node->y) >> 8;
				s32 dz = (position[2] - node->z) >> 8;
				s32 distance =
					syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz);

				if(distance <= ROPE_NODE_REACH * ROPE_NODE_REACH &&
				   best > distance) {
					best = distance;
					found = node;
				}
			}
		}
		result = found;
	}
	return result;
}

/* Hops the leader onto the nearest crossable rope node, to cross the rope */
/* (LEADER_MOVEMENT_ROPE); returns 0 when none is in reach. */
s32 fdLeaderStepOntoRope(void) {
	ACTOR *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	SPR *sprite = leader->spr;
	ROPE_NODE *node = fdRopeFindNodeNear(&leader->x.fixed);
	u8 *flags;

	FieldStateWork->blockedSteps = 0;
	if(node == NULL)
		return 0;
	sySndPlayStub(SE_JUMP);
	leader->acceleration = toFIXED(1);
	leader->speedLimit = toFIXED(2);
	leader->velY = toFIXED(4);
	leader->watchedActor = (union FIELD_OBJECT *)node;
	fdActorSelectAnimationStub(leader, ANIM_WALK);
	fdActorSetMoveTargetStub(leader, node->x, node->y, node->z);
	fdActorWaitForMoveStub(leader);
	leader->tmr = leader->motionFlg;
	flags = &leader->priFlg;
	leader->userValue = sprite->flg;
	leader->savedSprPri = sprite->image.pri;
	leader->priFlg = *flags & ~ACTOR_PRIORITY_AUTOMATIC;
	sprite->flg = 0;
	sprite->image.pri = 1;
	MainRoutineStateWork.movementMode = LEADER_MOVEMENT_ROPE;
	return 1;
}

/* Hops the leader off a tied rope a cell in the direction the pad holds, */
/* when that cell is open; returns 0 otherwise. */
s32 fdLeaderStepOffRope(void) {
	ACTOR *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	SPR *sprite = leader->spr;
	FIXED position[3];
	u32 angle;

	FieldStateWork->blockedSteps = 0;
	angle = Rope_KeyHeadings[DPAD_INDEX(InputWork.held)];
	if(angle == DPAD_ANGLE_NONE)
		return 0;
	leader->x.fixed = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
	leader->z.fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	syVectorAddPolarOffset(toFIXED(16), angle, position);
	leader->groundHeight = leader->y.fixed - toFIXED(8);
	if(fdMapGridGetTerrainKindAtStub(leader->layer, position[0], position[2]) ||
	   fdMapGridCheckCollisionStub(leader, position))
		return 0;
	leader->facing = angle;
	leader->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	leader->motionFlg = leader->tmr;
	leader->scaleX = FIXED_ONE;
	sprite->flg = leader->userValue;
	sprite->rotation = 0;
	sySndPlayStub(SE_JUMP);
	leader->speedLimit = toFIXED(2);
	leader->velY = toFIXED(4);
	leader->acceleration = toFIXED(1);
	fdActorSelectAnimationStub(leader, ANIM_WALK);
	fdActorSetMoveTargetStub(leader, position[0], position[1], position[2]);
	fdActorWaitForMoveStub(leader);
	sprite->image.pri = leader->savedSprPri;
	MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
	return 1;
}

/* Returns the nearest crossable tightrope node within reach of position, or */
/* NULL. */
TIGHTROPE_NODE *fdTightropeFindNodeNear(s32 *position) {
	TIGHTROPE_NODE *found;
	TIGHTROPE_NODE *result;
	TIGHTROPE_WORK *state;
	TIGHTROPE_NODE *node;
	s32 best;
	s32 i;

	found = NULL;
	state = TIGHTROPE_STATE;
	if(state == NULL) {
		result = NULL;
	}
	else {
		node = state->nodes;
		best = ROPE_NODE_DISTANCE_NONE;
		for(i = 0; i < ROPE_NODE_COUNT; i++, node++) {
			if(node->kind == ROPE_NODE_CROSSABLE) {
				/* In pixels, squared. */
				s32 dx = (position[0] - node->x) >> 8;
				s32 dy = (position[1] - node->y) >> 8;
				s32 dz = (position[2] - node->z) >> 8;
				s32 distance =
					syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz);

				if(distance <= ROPE_NODE_REACH * ROPE_NODE_REACH &&
				   best > distance) {
					best = distance;
					found = node;
				}
			}
		}
		result = found;
	}
	return result;
}

/* Hops the leader onto the nearest crossable tightrope node */
/* (LEADER_MOVEMENT_TIGHTROPE); returns 0 when none is in reach. */
s32 fdLeaderStepOntoTightrope(void) {
	MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;
	ACTOR *leader = fdActorLookup(state->leaderActorId);
	SPR *sprite = leader->spr;
	TIGHTROPE_NODE *node = fdTightropeFindNodeNear(&leader->x.fixed);
	u8 *flags;

	FieldStateWork->blockedSteps = 0;
	if(node == NULL)
		return 0;
	sySndPlayStub(SE_JUMP);
	leader->acceleration = toFIXED(1);
	leader->speedLimit = toFIXED(2);
	leader->velY = toFIXED(4);
	leader->watchedActor = (union FIELD_OBJECT *)node;
	fdActorSelectAnimationStub(leader, ANIM_WALK);
	fdActorSetMoveTargetStub(leader, node->x, node->y, node->z);
	fdActorWaitForMoveStub(leader);
	leader->tmr = leader->motionFlg;
	leader->userValue = sprite->flg;
	leader->savedSprPri = sprite->image.pri;
	flags = &leader->priFlg;
	leader->priFlg = *flags & ~ACTOR_PRIORITY_AUTOMATIC;
	sprite->flg = 0;
	sprite->image.pri = 1;
	state->movementMode = LEADER_MOVEMENT_TIGHTROPE;
	return 1;
}

/* Hops the leader off a tightrope a cell in the direction the pad holds, when
 */
/* that cell is open; returns 0 otherwise. */
s32 fdLeaderStepOffTightrope(void) {
	ACTOR *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	SPR *sprite = leader->spr;
	FIXED position[3];
	u32 angle;

	FieldStateWork->blockedSteps = 0;
	angle = Rope_KeyHeadings[DPAD_INDEX(InputWork.held)];
	if(angle == DPAD_ANGLE_NONE)
		return 0;
	leader->x.fixed = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
	leader->z.fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	syVectorAddPolarOffset(toFIXED(16), angle, position);
	leader->groundHeight = leader->y.fixed - toFIXED(8);
	if(fdMapGridGetTerrainKindAtStub(leader->layer, position[0], position[2]) ||
	   fdMapGridCheckCollisionStub(leader, position))
		return 0;
	leader->facing = angle;
	leader->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	leader->motionFlg = leader->tmr;
	sprite->flg = leader->userValue;
	sprite->rotation = 0;
	sySndPlayStub(SE_JUMP);
	leader->acceleration = toFIXED(1);
	leader->speedLimit = toFIXED(2);
	leader->velY = toFIXED(4);
	fdActorSelectAnimationStub(leader, ANIM_WALK);
	fdActorSetMoveTargetStub(leader, position[0], position[1], position[2]);
	fdActorWaitForMoveStub(leader);
	sprite->image.pri = leader->savedSprPri;
	MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
	return 1;
}
