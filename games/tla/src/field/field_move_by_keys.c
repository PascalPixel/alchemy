/* @file */
/* Moving actors by the direction pad: the party's keys, crossing a rope or a */
/* tightrope, and moving as Sand, on a map and on the world map. */
/* Part of field.c. */

#include "field/sequence_stubs.h"
#include "field/map.h"

/* SPRITE_SAND_RIPPLE's animations, the leader as Sand: the mound of sand */
/* shifting as it moves, and standing upright as it climbs a face. */
#define SAND_ANIMATION_MOVE	 1
#define SAND_ANIMATION_CLIMB 3
/* The puff of sand the leader leaves behind, thinning away over 64 frames */
/* (SPRITE_SAND_PUFF), and a grain dropping off as he climbs, a cloud that */
/* scatters into specks over 48 frames (SPRITE_DUST_CLOUD). */
#define SAND_PUFF_ANIMATION_THIN	 1
#define DUST_CLOUD_ANIMATION_SCATTER 2

/* Whether the actor can step 8 pixels toward angle: probe is set to the */
/* step's place and tested against the map. */
#define STEP_IS_CLEAR(actor, probe, angle)                                     \
	(SET_POSITION(probe, (actor)->x.fixed, (actor)->y.fixed,                   \
				  (actor)->z.fixed),                                           \
	 syVectorAddPolarOffset(toFIXED(8), angle, probe),                         \
	 !fdMapGridCheckCollision(actor, probe))
/* Sets probe to the place 8 pixels toward angle from an actor climbing a */
/* wall, the step's depth turned into height. */
#define SET_CLIMB_STEP(probe, actor, angle)                                    \
	(SET_POSITION(probe, (actor)->x.fixed, 0, 0),                              \
	 syVectorAddPolarOffset(toFIXED(8), angle, probe),                         \
	 (probe)->y = (actor)->y.fixed - (probe)->z,                               \
	 (probe)->z = (actor)->z.fixed)
/* Whether an actor climbing a wall can step 8 pixels toward angle. */
#define CLIMB_STEP_IS_CLEAR(actor, probe, angle)                               \
	(SET_CLIMB_STEP(probe, actor, angle),                                      \
	 !fdMapGridCheckCollision(actor, probe))

/* SC_KEYS (LEADER_MOVEMENT_KEYS), the leader's control on a climbable wall: */
/* moves it 8 pixels at a time by the direction pad, up and down changing its */
/* height rather than its depth, reaching east or west as it goes and clinging
 */
/* while the pad is not held. Refuses a step into a wall, from or onto a cell */
/* that is not MAP_CELL_CLIMBABLE, or onto ground too high (up) or too low */
/* (down), and records the held keys in the field state. Returns 1. */
s32 fdActorMoveByKeys(ACTOR *actor) {
	POS position;
	MAP_CELL *sourceCell;
	MAP_CELL *destinationCell;
	FIELD_STATE *state;
	u16 angle;
	s32 animation;
	s32 collisionFlags;
	s32 deltaZ;
	FIXED terrainHeight;
	MAP_STATE *map;

	actor->acceleration = toFIXED(1) / 4;
	animation = ANIM_LEADER_CLING;
	actor->speedLimit = toFIXED(1) / 2;
	/* DEAD: a debug read of the held keys nothing uses; the ROM keeps the load.
	 */
	if(DebugMode != 0)
		(void)InputWork.held;
	angle = WorldMap_KeyHeadings[DPAD_INDEX(InputWork.held)];
	if(angle == WORLD_MAP_KEY_HEADING_NONE) {
		collisionFlags = PLAYER_STEP_STOPPED;
	}
	else {
		animation = ANIM_LEADER_CLIMB_EAST;
		if(FACING_SIXTEENTH(angle) != FACING_EAST) {
			animation = ANIM_LEADER_CLIMB_WEST;
			if(FACING_SIXTEENTH(angle) != FACING_WEST)
				animation = ANIM_LEADER_CLIMB;
		}
		collisionFlags = 0;
		SET_POSITION(&position, 0, 0, 0);
		syVectorAddPolarOffset(toFIXED(8), angle, &position);
		position.x += actor->x.fixed;
		deltaZ = position.z;
		if(deltaZ < 0)
			actor->facing = TRIG_TURN * 3 / 4;
		if(deltaZ > 0)
			actor->facing = DEGtoANG(90);
		position.y = actor->y.fixed - position.z;
		position.z = actor->z.fixed;
		map = Ram_HeapSlots->mapGrid;
		sourceCell =
			&map->layers[actor->layer].cells[fdActorGetCellIndexStub(actor)];
		destinationCell =
			&map->layers[actor->layer]
				 .cells[position.x / toFIXED(16) +
						(position.z / toFIXED(16)) * MAPGRID_SIZE];
		if(fdMapGridIsWallAt((s32 *)&position) != 0 ||
		   !(sourceCell->attributeB & MAP_CELL_CLIMBABLE) ||
		   !(destinationCell->attributeB & MAP_CELL_CLIMBABLE)) {
			collisionFlags = PLAYER_STEP_STOPPED;
			animation = ANIM_LEADER_CLING;
		}
		else if(InputWork.held & KEY_UP) {
			terrainHeight = fdMapGridGetTerrainHeight(actor->layer, position.x,
													  position.z - toFIXED(16));
			if(terrainHeight - actor->y.fixed < toFIXED(16))
				goto refused;
		}
		else if(InputWork.held & KEY_DOWN) {
			terrainHeight =
				fdMapGridGetTerrainHeight(actor->layer, position.x, position.z);
			if(terrainHeight - actor->y.fixed > -toFIXED(8)) {
			refused:
				collisionFlags = PLAYER_STEP_BLOCKED_BY_MAP;
				animation = ANIM_LEADER_CLING;
			}
		}
	}
	state = Ram_HeapSlots->fieldState;
	if(state != NULL) {
		if(collisionFlags & PLAYER_STEP_BLOCKED)
			state->blockedSteps++;
		else
			state->blockedSteps = 0;
		state->heldKeys = InputWork.held;
	}
	fdActorSelectAnimation(actor, animation);
	if(collisionFlags != 0) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		actor->velX = 0;
		actor->velY = 0;
		actor->velZ = 0;
	}
	else {
		fdActorSetMoveTarget(actor, position.x, position.y, position.z);
	}
	actor->scriptCursor++;
	return 1;
}

/* SC_ROPE (LEADER_MOVEMENT_ROPE), the leader crossing a tied rope hand over */
/* hand: the pad moves him from node to node along it, a half cell a step, and
 */
/* sets the rope swinging. */
s32 fdActorCrossRope(ACTOR *actor) {
	POS position;
	s32 collisionFlags;
	SPR *sprite;
	ROPE_WORK *rope;
	s32 bestDistance;
	s32 animationSpeed;
	s32 speed;
	ROPE_NODE *best;
	ROPE_NODE *node;
	ROPE_SLOT *slot;
	s32 count;
	u16 angle;
	s32 turn;
	ACTOR *first;
	ACTOR *second;
	FIELD_STATE *state;

	sprite = actor->spr;
	/* The rope slot, read by its number. */
	rope = HEAP_SLOT_AT(Ram_HeapSlots, HEAP_SLOT_ROPE);
	speed = FIXED_ONE;
	collisionFlags = 0;
	actor->speedLimit = toFIXED(1) / 2;
	actor->acceleration = toFIXED(1) / 2;
	actor->motionFlg = 0;
	angle = WorldMap_KeyHeadings[DPAD_INDEX(InputWork.held)];
	if(angle == WORLD_MAP_KEY_HEADING_NONE) {
		best = (ROPE_NODE *)actor->watchedActor;
		animationSpeed = 0;
	}
	else {
		animationSpeed = ANIM_STEP_NORMAL;
		SET_POSITION(&position, 0, 0, 0);
		syVectorAddPolarOffset(toFIXED(8), angle, &position);
		position.x += actor->x.fixed;
		position.y = actor->y.fixed - position.z;
		position.z = actor->z.fixed;
		rope = HEAP_SLOT_AT(Ram_HeapSlots, HEAP_SLOT_ROPE);
		node = rope->nodes;
		best = NULL;
		bestDistance = ROPE_NODE_DISTANCE_NONE;
		if(rope == NULL) {
			collisionFlags = PLAYER_STEP_BLOCKED_BY_MAP;
			state = Ram_HeapSlots->fieldState;
			goto report;
		}
		for(count = 0; count < ROPE_NODE_COUNT; count++, node++) {
			if(node->kind == ROPE_NODE_CROSSABLE) {
				s32 dx = (position.x - node->x) >> 8;
				s32 dy = (position.y - node->y) >> 8;
				s32 dz = (position.z - node->z) >> 8;
				s32 distance =
					syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz);

				if(distance <= 32 * 32 && bestDistance > distance) {
					bestDistance = distance;
					best = node;
				}
			}
		}
	}
	if(best == NULL) {
		collisionFlags |= PLAYER_STEP_BLOCKED_BY_MAP;
	}
	else {
		slot = &rope->slots[best->slot - 1];
		first = fdActorGetByIdStub(slot->first);
		second = fdActorGetByIdStub(slot->second);
		if(first->x.fixed > second->x.fixed)
			speed = -speed;
		first = fdActorGetByIdStub(slot->first);
		second = fdActorGetByIdStub(slot->second);
		if(first->y.fixed > second->y.fixed) {
			speed = -speed;
			turn = TRIG_TURN * 3 / 4;
		}
		else {
			turn = DEGtoANG(90);
		}
		actor->scaleX = speed;
		if(best->x == actor->x.fixed && best->y == actor->y.fixed &&
		   best->z == actor->z.fixed) {
			collisionFlags = PLAYER_STEP_BLOCKED_BY_MAP;
		}
		else {
			slot->swinging = 1;
			slot->amplitude += toFIXED(1) / 2;
			if(slot->amplitude > toFIXED(2))
				slot->amplitude = toFIXED(2);
			actor->watchedActor = (union FIELD_OBJECT *)best;
			SET_POSITION(&position, best->x, best->y, best->z);
			sprite->rotation = best->ang + turn;
		}
	}
	state = Ram_HeapSlots->fieldState;
report:
	if(state != NULL) {
		if(collisionFlags & PLAYER_STEP_BLOCKED)
			state->blockedSteps++;
		else
			state->blockedSteps = 0;
		state->heldKeys = InputWork.held;
	}
	fdActorSelectAnimation(actor, ANIM_LEADER_CROSS_ROPE);
	fdActorSetAnimationSpeed(actor, animationSpeed);
	if(collisionFlags != 0) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		actor->velX = 0;
		actor->velY = 0;
		actor->velZ = 0;
	}
	else {
		fdActorSetMoveTarget(actor, position.x, position.y, position.z);
	}
	actor->scriptCursor++;
	return 1;
}

/* Turns the actor toward angle, at most a sixteenth of a turn. */
static inline void TurnToward(ACTOR *actor, u16 angle) {
	s16 delta = angle - actor->facing;

	if(delta > TRIG_TURN / 16)
		delta = TRIG_TURN / 16;
	if(delta < -(TRIG_TURN / 16))
		delta = -(TRIG_TURN / 16);
	actor->facing += delta;
}

/* SC_TIGHTROPE (LEADER_MOVEMENT_TIGHTROPE): fdActorCrossRope on a tightrope, */
/* which the leader walks, the rope wobbling under him. */
s32 fdActorCrossTightrope(ACTOR *actor) {
	POS position;
	s32 collisionFlags;
	SPR *sprite;
	s32 animation;
	TIGHTROPE_WORK *tightrope;
	TIGHTROPE_NODE *best;
	TIGHTROPE_NODE *point;
	TIGHTROPE_SLOT *slot;
	s32 bestDistance;
	u16 angle;
	s32 count;
	FIELD_STATE *state;

	sprite = actor->spr;
	animation = ANIM_STAND;
	collisionFlags = 0;
	actor->speedLimit = toFIXED(1) / 2;
	actor->acceleration = toFIXED(1) / 2;
	actor->motionFlg = 0;
	angle = WorldMap_KeyHeadings[DPAD_INDEX(InputWork.held)];
	if(angle == WORLD_MAP_KEY_HEADING_NONE) {
		collisionFlags = PLAYER_STEP_STOPPED;
		best = (TIGHTROPE_NODE *)actor->watchedActor;
		SET_POSITION(&position, actor->x.fixed, best->y, actor->z.fixed);
		state = Ram_HeapSlots->fieldState;
	}
	else {
		SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
		syVectorAddPolarOffset(toFIXED(8), angle, &position);
		/* The tightrope slot, read by its number. */
		tightrope = HEAP_SLOT_AT(Ram_HeapSlots, HEAP_SLOT_TIGHTROPE);
		point = tightrope->nodes;
		best = NULL;
		bestDistance = ROPE_NODE_DISTANCE_NONE;
		if(tightrope == NULL) {
			collisionFlags = PLAYER_STEP_BLOCKED_BY_MAP;
			state = Ram_HeapSlots->fieldState;
		}
		else {
			for(count = 0; count < ROPE_NODE_COUNT; count++, point++) {
				if(point->kind == ROPE_NODE_CROSSABLE) {
					s32 dx = (position.x - point->x) >> 8;
					s32 dy = (position.y - point->y) >> 8;
					s32 dz = (position.z - point->z) >> 8;
					s32 distance =
						syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz);

					if(distance <= 32 * 32 && bestDistance > distance) {
						bestDistance = distance;
						best = point;
					}
				}
			}
			if(best == NULL) {
				collisionFlags |= PLAYER_STEP_BLOCKED_BY_MAP;
			}
			else if(best->x == actor->x.fixed && best->y == actor->y.fixed &&
					best->z == actor->z.fixed) {
				collisionFlags = PLAYER_STEP_BLOCKED_BY_MAP;
			}
			else {
				if(best->slot != 0) {
					slot = &tightrope->slots[best->slot - 1];
					slot->wobbling = 1;
					slot->amplitude += toFIXED(1) / 2;
					if(slot->amplitude > toFIXED(2))
						slot->amplitude = toFIXED(2);
					actor->watchedActor = (union FIELD_OBJECT *)best;
				}
				/* FAKEMATCH(dead): position.x copied onto itself before the
				 * copies. */
				position.x = position.x;
				SET_POSITION(&position, best->x, best->y, best->z);
			}
			state = Ram_HeapSlots->fieldState;
		}
	}
	if(state != NULL) {
		if(collisionFlags & PLAYER_STEP_BLOCKED)
			state->blockedSteps++;
		else
			state->blockedSteps = 0;
		state->heldKeys = InputWork.held;
	}
	if(collisionFlags == 0)
		animation = ANIM_WALK;
	fdActorSelectAnimation(actor, animation);
	fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL / 2);
	sprite->ofsY = 4;
	if(collisionFlags != 0) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		actor->velX = 0;
		actor->velY = 0;
		actor->velZ = 0;
		if(collisionFlags & PLAYER_STEP_BLOCKED)
			TurnToward(actor, angle);
		actor->y.fixed = position.y;
	}
	else {
		actor->y.fixed = position.y;
		fdActorSetMoveTarget(actor, position.x, position.y, position.z);
		if(actor->x.fixed == position.x && actor->z.fixed == position.z)
			TurnToward(actor, angle);
	}
	actor->scriptCursor++;
	return 1;
}

/* Update of a grain the Sand leaves as the leader moves: it turns and */
/* jitters sideways at random. */
void fdSandJitterGrain(ACTOR *actor) {
	u32 first;
	u32 second;
	FIXED x;

	((SPR *)actor->spr)->rotation += TRIG_TURN / 64;
	first = syRandom();
	second = syRandom();
	x = actor->x.fixed;

	second >>= 1;
	first >>= 1;
	actor->x.fixed = x + (first - second);
}

/* Whether the player ends Sand: when the Select, A or Start keys are pressed,
 */
/* plays SE_MENU_CANCEL where the leader is blocked (fdActorCheckBlockedStub),
 */
/* and otherwise, while FLAG_SAND_CANNOT_END is clear, asks for Sand again */
/* (ABILITY_SAND, packed) and returns 1. */
s32 fdSandCheckEndKeys(ACTOR *actor) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 pressed = 0;

	if(state == NULL)
		return 0;
	if(InputWork.pressed & MainRoutineStateWork.selectKeys)
		pressed = 1;
	else if(InputWork.pressed & MainRoutineStateWork.aKeys)
		pressed = 1;
	else if(InputWork.pressed & MainRoutineStateWork.startKeys)
		pressed = 1;
	if(pressed) {
		if(fdActorCheckBlockedStub(actor)) {
			sySndPlayStub(SE_MENU_CANCEL);
		}
		else if(!syGameFlagGet(FLAG_SAND_CANNOT_END)) {
			state->psynergy = ABILITY_SAND | ACT_PACKED;
			return 1;
		}
	}
	return 0;
}

/* SC_SAND (LEADER_MOVEMENT_SAND): the leader turned to sand by the Sand */
/* Psynergy, moved by the direction pad on a field map, leaving puffs of sand;
 */
/* up against a face, or down at a drop, he climbs it as sand. */
s32 fdActorMoveAsSand(ACTOR *actor) {
	POS position;
	POS testPosition;
	s16 angleOffsets[10];
	POS spawn;
	s32 collision;
	s32 heading;
	FIXED groundHere;
	FIXED y;
	FIXED groundAhead;
	s32 angle;
	s32 cellX;
	s32 cellZ;
	s32 angleIndex;
	s32 count;
	/* FAKEMATCH(temporary): wakeTimer, a pointer to actor->wakeTmr; as the
	 * field by name. */
	u8 *wakeTimer;
	ACTOR *effect;
	SPR *sprite;

	heading = 0;
	count = 0;
	cellX = actor->x.fixed & FIELD_CELL_MASK;
	cellZ = actor->z.fixed & FIELD_CELL_MASK;
	y = actor->y.fixed;
	groundHere = fdMapGridGetTerrainHeight(actor->layer, cellX, cellZ);
	groundAhead =
		fdMapGridGetTerrainHeight(actor->layer, cellX, cellZ - toFIXED(16));
	if(actor->tmr != 0) {
		if(actor->tgtX != ACTOR_NO_TARGET)
			goto done;
		actor->tmr = 0;
		actor->groundHeight = groundHere;
	}
	if(actor->userValue != 0) {
		if(groundAhead < y) {
			FIXED topZ = cellZ - toFIXED(8);

			if(fdMapGridCheckCollisionAt(actor->x.fixed, topZ, actor->layer,
										 actor->groundHeight) == 2) {
				actor->y.fixed -= toFIXED(1);
			}
			else {
				fdActorSetMoveTarget(actor, actor->x.fixed, groundAhead, topZ);
				actor->tmr = 1;
				actor->userValue = 0;
				goto done;
			}
		}
		if(groundHere > y) {
			actor->userValue = 0;
			actor->groundHeight = groundHere;
			actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			goto done;
		}

		collision = 0;
		/* DEAD: a debug read of the held keys nothing uses; the ROM keeps the
		 * load. */
		if(DebugMode != 0)
			(void)InputWork.held;
		actor->speedLimit = toFIXED(1) / 2;
		actor->acceleration = toFIXED(1) / 4;
		angle = WorldMap_KeyHeadings[DPAD_INDEX(InputWork.held)];
		if((u16)angle == WORLD_MAP_KEY_HEADING_NONE) {
			collision = PLAYER_STEP_STOPPED;
		}
		else {
			SET_CLIMB_STEP(&position, actor, (u16)angle);
			fdActorSelectAnimation(actor, SAND_ANIMATION_CLIMB);
			if(!(CLIMB_STEP_IS_CLEAR(actor, &testPosition, (u16)angle) &&
				 CLIMB_STEP_IS_CLEAR(actor, &testPosition,
									 (u16)angle + FIELD_PROBE_ANGLE) &&
				 CLIMB_STEP_IS_CLEAR(actor, &testPosition,
									 (u16)angle - FIELD_PROBE_ANGLE) &&
				 CLIMB_STEP_IS_CLEAR(actor, &testPosition,
									 (u16)angle + DEGtoANG(45)) &&
				 CLIMB_STEP_IS_CLEAR(actor, &testPosition,
									 (u16)angle - DEGtoANG(45)))) {
				SET_POSITION(&position, actor->x.fixed, actor->y.fixed,
							 actor->z.fixed);
				collision |= PLAYER_STEP_BLOCKED_BY_MAP;
			}
		}
		if(collision != 0) {
			actor->tgtX = ACTOR_NO_TARGET;
			actor->tgtY = ACTOR_NO_TARGET;
			actor->tgtZ = ACTOR_NO_TARGET;
			actor->velX = 0;
			actor->velZ = 0;
		}
		else {
			FIXED speed;

			heading = (s16)angle;
			fdActorSetMoveTarget(actor, position.x, position.y, position.z);
			speed = sySquartFX(syMulFX(actor->velX, actor->velX) +
							   syMulFX(actor->velZ, actor->velZ));
			actor->velX = 0;
			actor->velZ = 0;
			syVectorAddPolarOffset(speed, (u16)heading, (POS *)&actor->velX);
			actor->velY = -actor->velZ;
			actor->velZ = 0;
		}
		/* Every 16th frame, a grain of sand drops beside the climber. */
		if((FrameCount & 15) == 0) {
			SET_POSITION(&spawn, actor->x.fixed - syRandom() * 6 + toFIXED(3),
						 actor->y.fixed, actor->z.fixed);
			effect =
				fdActorCreate(SPRITE_DUST_CLOUD, spawn.x, spawn.y, spawn.z);
			if(effect != NULL) {
				effect->groundHeight = actor->groundHeight;
				sprite = effect->spr;
				fdActorStartScript(effect, Sand_GrainScript);
				effect->update = fdSandJitterGrain;
				effect->motionFlg = ACTOR_FALLS;
				if(collision == 0)
					sySndPlayStub(SE_SAND_MOVE);
				effect->gravity = toFIXED(1) / 25;
				effect->velY = 0;
				effect->scaleX = toFIXED(0.3);
				effect->scaleY = toFIXED(0.3);
				if(sprite != NULL) {
					fdSprStartAnimation(sprite, DUST_CLOUD_ANIMATION_SCATTER);
					sprite->flg = 0;
					sprite->image.pri = 0;
				}
			}
		}
		fdSandCheckEndKeys(actor);
	}
	else {

		/* On foot. */
		collision = 0;
		actor->speedLimit = toFIXED(1);
		actor->acceleration = toFIXED(1) / 4;
		angle = WorldMap_KeyHeadings[DPAD_INDEX(InputWork.held)];
		if((u16)angle == WORLD_MAP_KEY_HEADING_NONE) {
			collision = PLAYER_STEP_STOPPED;
		}
		else {
			SET_POSITION(&position, actor->x.fixed, actor->y.fixed,
						 actor->z.fixed);
			syVectorAddPolarOffset(toFIXED(8), (u16)angle, &position);
			if(DebugMode != 0 && (InputWork.held & KEY_L)) {
				heading = (s16)angle;
			}
			else if(!fdMapGridCheckCollision(actor, &position) &&
					STEP_IS_CLEAR(actor, &testPosition,
								  (u16)angle + FIELD_PROBE_ANGLE) &&
					STEP_IS_CLEAR(actor, &testPosition,
								  (u16)angle - FIELD_PROBE_ANGLE) &&
					STEP_IS_CLEAR(actor, &testPosition,
								  (u16)angle + DEGtoANG(45)) &&
					STEP_IS_CLEAR(actor, &testPosition,
								  (u16)angle - DEGtoANG(45))) {
				heading = (s16)angle;
			}
			else {
				u16 direction = angle;

				angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
				angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
				angleOffsets[count++] = direction + DEGtoANG(45);
				angleOffsets[count++] = direction - DEGtoANG(45);
				angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
				angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
				for(angleIndex = 0; angleIndex < count; angleIndex++) {
					heading = angleOffsets[angleIndex];
					if(STEP_IS_CLEAR(actor, &position, (u16)heading) &&
					   STEP_IS_CLEAR(actor, &testPosition,
									 (u16)heading + FIELD_PROBE_ANGLE) &&
					   STEP_IS_CLEAR(actor, &testPosition,
									 (u16)heading - FIELD_PROBE_ANGLE) &&
					   STEP_IS_CLEAR(actor, &testPosition,
									 (u16)heading + DEGtoANG(45)) &&
					   STEP_IS_CLEAR(actor, &testPosition,
									 (u16)heading - DEGtoANG(45)))
						goto counted;
				}
				SET_POSITION(&position, actor->x.fixed, actor->y.fixed,
							 actor->z.fixed);
				collision |= PLAYER_STEP_BLOCKED_BY_MAP;
				/* Up against a face more than 8 pixels high: start climbing it.
				 */
				if((InputWork.held & KEY_UP) && groundAhead > y + toFIXED(8)) {
					cellX = (position.x & FIELD_CELL_MASK) + toFIXED(8);
					cellZ = (position.z & FIELD_CELL_MASK) - toFIXED(8);

					if(fdMapGridIsClimbableCellStub(cellX, cellZ, 2)) {
						actor->motionFlg = 0;
						actor->y.fixed += toFIXED(1);
						actor->userValue = 1;
						actor->velX = 0;
						actor->velY = 0;
						actor->velZ = 0;
						fdActorSelectAnimation(actor, SAND_ANIMATION_CLIMB);
						goto done;
					}
				}
				/* Down at a drop: climb down the face south of the cell. */
				if(InputWork.held & KEY_DOWN) {
					cellX = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
					cellZ = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(24);

					if(fdMapGridGetTerrainHeight(actor->layer, cellX, cellZ) <
						   y &&
					   fdMapGridIsClimbableCellStub(cellX, cellZ, 2)) {
						actor->motionFlg = 0;
						fdActorSetMoveTarget(actor, actor->x.fixed,
											 groundHere - toFIXED(2), cellZ);
						actor->tmr = 1;
						actor->userValue = 1;
						actor->velX = 0;
						actor->velY = 0;
						actor->velZ = 0;
						fdActorSelectAnimation(actor, SAND_ANIMATION_CLIMB);
						goto done;
					}
				}
			}
		}
	counted: {
		FIELD_STATE *state = Ram_HeapSlots->fieldState;

		if(state != NULL) {
			if(collision & PLAYER_STEP_BLOCKED)
				state->blockedSteps++;
			else
				state->blockedSteps = 0;
			state->heldKeys = InputWork.held;
		}
	}
		fdActorSelectAnimation(actor, SAND_ANIMATION_MOVE);
		if(collision != 0) {
			actor->tgtX = ACTOR_NO_TARGET;
			actor->tgtY = ACTOR_NO_TARGET;
			actor->tgtZ = ACTOR_NO_TARGET;
			actor->velX = 0;
			actor->velZ = 0;
			if(collision & PLAYER_STEP_BLOCKED) {
				s32 difference;
				u16 turn = angle;

				difference = (s16)(turn - actor->facing);
				if(difference > FIELD_BLOCKED_TURN_MAX)
					difference = FIELD_BLOCKED_TURN_MAX;
				if(difference < -FIELD_BLOCKED_TURN_MAX)
					difference = -FIELD_BLOCKED_TURN_MAX;
				actor->facing += difference;
			}
			wakeTimer = &actor->wakeTmr;
			*wakeTimer = 0;
		}
		else {
			FIXED speed;

			fdActorSetMoveTarget(actor, position.x, position.y, position.z);
			speed = sySquartFX(syMulFX(actor->velX, actor->velX) +
							   syMulFX(actor->velZ, actor->velZ));
			actor->velX = 0;
			actor->velZ = 0;
			syVectorAddPolarOffset(speed, (u16)heading, (POS *)&actor->velX);
			wakeTimer = &actor->wakeTmr;
			if(*wakeTimer != 0)
				(*wakeTimer)--;
		}
		/* While the leader moves, a puff of sand is left behind every fifth */
		/* frame. */
		if(actor->wakeTmr == 0 && collision == 0) {
			effect = fdActorCreate(SPRITE_SAND_PUFF, actor->x.fixed,
								   actor->y.fixed, actor->z.fixed - toFIXED(2));
			if(effect != NULL) {
				effect->groundHeight = actor->groundHeight;
				sprite = effect->spr;
				fdActorStartScript(effect, Actor_WakeScript);
				effect->priFlg = ACTOR_PRIORITY_OWN_DEPTH;
				effect->wakeFrm = 100;
				effect->motionFlg = 0;
				sySndPlayStub(SE_SAND_MOVE);
				if(sprite != NULL) {
					FIELD_STATE *state = Ram_HeapSlots->fieldState;

					fdSprStartAnimation(sprite, SAND_PUFF_ANIMATION_THIN);
					sprite->flg = 0;
					/* DEAD: overwritten by the next line; the ROM keeps its
					 * mask and OR. */
					sprite->image.pri = 2;
					sprite->image.pri = state->sandTrailPri;
				}
				*wakeTimer = 5;
			}
		}
		fdSandCheckEndKeys(actor);
	}
done:
	actor->scriptCursor++;
	return 1;
}

/* SC_WORLD_SAND (LEADER_MOVEMENT_WORLD_SAND): the leader as sand on the world
 */
/* map (the Sand Psynergy). Moves him on the world map by the direction pad */
/* at a fixed pace (faster while L repeats), trying six turned headings when */
/* the way is blocked, as fdActorMoveOnWorldMap does; records the held keys in
 */
/* the event state and leaves a puff of sand (SPRITE_SAND_PUFF) behind it every
 */
/* fifth frame it moves. */
s32 fdActorMoveAsSandOnWorldMap(ACTOR *actor) {
	POS position;
	POS leftPosition;
	POS rightPosition;
	s16 angleOffsets[22];
	s32 collision;
	s32 angle;
	u16 direction;
	s32 angleIndex;
	s32 count;
	s32 difference;
	s32 terrainCollision;
	FIXED speed;
	s32 targetCameraYaw;
	WORLD_MAP_VIEW *camera;
	s32 yawStep;
	PLAYER_SPR *playerSprite;
	ACTOR *ripple;
	PLAYER_SPR *rippleSprite;
	FIELD_STATE *state;

	collision = 0;
	count = 0;
	actor->userValue = 0;
	actor->tmr = 1;
	actor->speedLimit = toFIXED(1) / 2;
	actor->acceleration = toFIXED(1) / 4;
	if(InputWork.repeat & KEY_L)
		actor->speedLimit = toFIXED(4);

	angle = WorldMap_KeyHeadings[DPAD_INDEX(InputWork.held)];
	direction = angle;
	if(direction == WORLD_MAP_KEY_HEADING_NONE) {
		collision |= PLAYER_STEP_STOPPED;
	}
	else {
		SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
		syVectorAddPolarOffset(toFIXED(8), direction, &position);
		if(DebugMode != 0 && (InputWork.held & KEY_L)) {
			/* FAKEMATCH(dead): direction takes angle, which it already holds
			 * (set just above), the debug branch's only statement; as the
			 * branch left empty. */
			direction = angle;
		}
		else {
			SET_POSITION(&leftPosition, actor->x.fixed, actor->y.fixed,
						 actor->z.fixed);
			syVectorAddPolarOffset(toFIXED(8), direction + DEGtoANG(30),
								   &leftPosition);
			SET_POSITION(&rightPosition, actor->x.fixed, actor->y.fixed,
						 actor->z.fixed);
			syVectorAddPolarOffset(toFIXED(8), direction - DEGtoANG(30),
								   &rightPosition);
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
					syVectorAddPolarOffset(toFIXED(8), direction, &position);
					SET_POSITION(&leftPosition, actor->x.fixed, actor->y.fixed,
								 actor->z.fixed);
					syVectorAddPolarOffset(toFIXED(8), direction + DEGtoANG(30),
										   &leftPosition);
					SET_POSITION(&rightPosition, actor->x.fixed, actor->y.fixed,
								 actor->z.fixed);
					syVectorAddPolarOffset(toFIXED(8), direction - DEGtoANG(30),
										   &rightPosition);
					if((fdWorldMapIsWalkable(actor, &position) |
						fdWorldMapIsWalkable(actor, &leftPosition) |
						fdWorldMapIsWalkable(actor, &rightPosition)) == 0) {
						angle = (s16)direction;
						goto update_object;
					}
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
		state->heldKeys = InputWork.held;
	}
	fdActorSelectAnimation(actor, SAND_ANIMATION_MOVE);

	if(collision != 0) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		actor->velX = 0;
		actor->velZ = 0;
		if((collision & PLAYER_STEP_BLOCKED) != 0) {
			direction = angle;
			difference = (s16)(direction - actor->facing);
			if(difference > FIELD_BLOCKED_TURN_MAX)
				difference = FIELD_BLOCKED_TURN_MAX;
			if(difference < -FIELD_BLOCKED_TURN_MAX)
				difference = -FIELD_BLOCKED_TURN_MAX;
			actor->facing += difference;
		}
		actor->wakeTmr = 0;
		actor->userValue = FOOTPRINT_BOTH_FEET;
	}
	else {
		fdActorSetMoveTarget(actor, position.x, position.y, position.z);
		speed = sySquartFX(syMulFX(actor->velX, actor->velX) +
						   syMulFX(actor->velZ, actor->velZ));
		actor->velX = 0;
		actor->velZ = 0;
		syVectorAddPolarOffset(speed, (u16)angle, (POS *)&actor->velX);
		if(actor->wakeTmr != 0)
			actor->wakeTmr--;
	}

	camera = (WORLD_MAP_VIEW *)Ram_HeapSlots->mapGrid;
	targetCameraYaw = WorldMap_KeyYaws[DPAD_INDEX(InputWork.held)];
	yawStep = (s16)(targetCameraYaw - camera->yaw) / 8;
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
		if(terrainCollision == WORLD_MAP_COLLISION_FOREST)
			playerSprite->secondLayer->pri = 1;
		else
			playerSprite->secondLayer->pri = SPRITE_LAYER_NOT_DRAWN;
	}
	if(actor->wakeTmr == 0 && collision == 0) {
		ripple = fdActorCreate(SPRITE_SAND_PUFF, actor->x.fixed, actor->y.fixed,
							   actor->z.fixed - toFIXED(2));
		if(ripple != NULL) {
			ripple->groundHeight = actor->groundHeight;
			rippleSprite = ripple->spr;
			fdActorStartScript(ripple, Actor_WakeScript);
			ripple->priFlg = ACTOR_PRIORITY_OWN_DEPTH;
			ripple->wakeFrm = 100;
			ripple->motionFlg = 0;
			if(rippleSprite != NULL) {
				fdSprStartAnimation(rippleSprite, SAND_PUFF_ANIMATION_THIN);
				rippleSprite->flg = 0;
				/* DEAD: overwritten by the next line; the ROM keeps its mask
				 * and OR. */
				rippleSprite->pri = 2;
				rippleSprite->pri = 1;
			}
			actor->wakeTmr = 5;
		}
	}
	fdSandCheckEndKeys(actor);
	actor->scriptCursor++;
	return 1;
}

#undef STEP_IS_CLEAR
#undef SET_CLIMB_STEP
#undef CLIMB_STEP_IS_CLEAR
