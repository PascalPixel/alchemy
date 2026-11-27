/* @file */
/* Cloak, Douse, falling particles and steering a lifted target. */
/* Part of event_commands.c. */

/* The Psynergy request that ends Cloak, without paying PP: what the ground */
/* check (fdEffectCheckCloakTile) makes when the caster does not stand on a */
/* tile it can hide on. */
#define CLOAK_END (ABILITY_CLOAK | ACT_PACKED)
/* The times the caster blinks as Cloak hides it. */
#define CLOAK_BLINKS 10

/* The drops Douse rains: cast at nothing, cast on a target, and cast on a */
/* target where they sway down (FIELD_EFFECT_STATE.lit). */
#define DOUSE_DROPS_AT_NOTHING		  10
#define DOUSE_DROPS_ON_TARGET		  24
#define DOUSE_SWAYING_DROPS_ON_TARGET 30
/* The drop after which the target's Douse trigger runs. */
#define DOUSE_TARGET_DROP 12
/* The sparkles Carry throws out as it drops its target. */
#define CARRY_SHARDS 20

/* Cloak the caster: flicker it CLOAK_BLINKS times, leave it half transparent,
 */
/* then check the ground under it every frame. When it cannot hide where it */
/* stands, show it again at once. */
void fdPsynergyStartCloak(void) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	ACTOR *caster =
		((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->caster;
	SPR *sprite = caster->spr;
	SPR_ANIMATION *entry = sprite->entries[0];
	u32 n;
	void (*check)(void);
	s32 order;

	sySndPlayStub(SE_PSYNERGY_START);
	fdActorSelectAnimationStub(caster, ANIM_STILL);
	caster->update = 0;

	for(n = 0; n < CLOAK_BLINKS; n++) {
		entry->colMode = SPRITE_COLOR_WHITE;
		sprite->dirty = 1;
		sprite->flg = SPRITE_FLAG_OUTLINED;
		sySynch(2);
		sprite->dirty = 1;
		sprite->flg = 0;
		sySynch(2);
	}

	order = TASK_PASS_FRAME;
	sprite->flg = SPRITE_FLAG_OUTLINED;
	entry->colMode = SPRITE_COLOR_NORMAL;
	sprite->dirty = 1;
	check = fdEffectCheckCloakTile;
	sySetEvent(check, order);
	MainRoutineStateWork.cloaked = 1;
	check();
	if(event->psynergy == CLOAK_END) {
		fdPsynergyEndCloak();
		event->psynergy = 0;
	}
}

/* A Douse drop's per-frame update: once it reaches its ground it starts the */
/* shared particle script and stops updating. */
void fdEffectLandDouseDrop(ACTOR *actor) {
	if(actor->y.fixed <= actor->groundHeight) {
		actor->wait = 2;
		fdActorStartScriptStub(actor, (u32)FieldEffect_ReleaseScript);
		actor->update = 0;
	}
}

/* Per-frame update: drifts from side to side, up to a pixel a frame, as it */
/* falls 0.3 pixels a frame, and is released on reaching its ground height. */
void fdEffectUpdateDriftingFallActor(ACTOR *particle) {
	s32 drift;

	particle->y.fixed -= toFIXED(3) / 10;
	drift = syRandom();
	particle->x.fixed += drift - syRandom();
	if(particle->y.fixed <= particle->groundHeight) {
		fdActorStartScriptStub(particle, FieldEffect_ReleaseScript);
	}
}

/* Runs Douse cast on a target, through fdPsynergyDouse. */
void fdPsynergyDouseOnTarget(s32 unused) { fdPsynergyDouse(); }

/* A rain cloud rises from 16 pixels above the caster to 32 pixels above the */
/* destination (80 with childOption, 32 more over a Moai) over GROW_STEPS */
/* frames, growing as it goes, rains drops, and returns. */
void fdPsynergyDouse(void) {
	VECTOR3 pos;
	VECTOR3 start;
	VECTOR3 end;
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;
	ACTOR *target = state->tgt;
	ACTOR *caster = state->caster;
	s32 count;
	ACTOR *cloud;
	s32 raised;
	s32 i;
	s32 steps = GROW_STEPS + 1;
	s32 y;
	s32 n;
	ACTOR *particle;
	PENDING_EVENT *trigger;

	cloud = fdActorSpawn(SPRITE_RAIN_CLOUD, i = raised = 0, 0, 0);
	if(cloud == 0)
		return;
	fdPsynergyFadeToCastCol();
	sySndPlayStub(SE_ITEM_BREAK);
	if(state->tgt == 0) {
		state->x = caster->x.fixed;
		state->z = caster->z.fixed;
		syVectorAddPolarOffset(toFIXED(16), state->ang, (VECTOR3 *)&state->x);
	}
	SET_POSITION(&start, caster->x.fixed, caster->y.fixed + toFIXED(16),
				 caster->z.fixed);
	end.x = state->x;
	y = state->y;
	end.y = y + toFIXED(32);
	end.z = state->z;
	if((s8)state->sceneVariant != 0)
		end.y = y + toFIXED(80);
	if(fdActorGetSprIdStub(target) == SPRITE_MOAI) {
		end.y += toFIXED(32);
		raised = 1;
	}

	for(; i < steps; i++) {
		s32 scale;

		SET_ACTOR_POSITION(cloud, start.x + (end.x - start.x) * i / GROW_STEPS,
						   start.y + (end.y - start.y) * i / GROW_STEPS,
						   start.z + (end.z - start.z) * i / GROW_STEPS);
		scale = fdEffectInterpolate(FIXED_ONE, FIXED_ONE / 4, i);
		cloud->scaleX = scale;
		cloud->scaleY = scale;
		sySynch(1);
	}
	sySynch(10);

	if((s8)state->lit == 0) {
		count = DOUSE_DROPS_AT_NOTHING;
		if((s8)state->aimAhead == 0)
			count = DOUSE_DROPS_ON_TARGET;
		for(n = 0; n < count; n++) {
			SET_POSITION(&pos, cloud->x.fixed, cloud->y.fixed, cloud->z.fixed);
			syVectorAddPolarOffset(syRandom() * 5 + toFIXED(3), syRandom(),
								   &pos);
			if(n == count - 1) {
				sySynch(20);
				SET_POSITION(&pos, cloud->x.fixed, cloud->y.fixed,
							 cloud->z.fixed);
			}
			particle = fdActorSpawn(SPRITE_DROPLET, pos.x, pos.y, pos.z);
			if(particle != 0) {
				ACTOR *ground = state->tgt;

				if(ground != 0) {
					particle->groundHeight = ground->y.fixed;
					if(raised == 1)
						particle->groundHeight = ground->y.fixed + toFIXED(32);
				}
				else {
					particle->groundHeight = pos.y - toFIXED(32);
				}
				particle->update = (OBJECT_UPDATE)fdEffectLandDouseDrop;
				particle->motionFlg = ACTOR_FALLS;
				particle->priFlg =
					ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_OVERHEAD;
			}
			sySndPlayStub(SE_WATER);
			sySynch(6);
			if(n == DOUSE_TARGET_DROP) {
				trigger = scFindPsynergyTrigger(
					PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), FIELD_PSYNERGY_DOUSE,
					scSelectPsynergyTarget(FIELD_PSYNERGY_DOUSE));
				if(trigger != 0)
					fdRunPsynergyEvent(trigger, state->casterId, state->tgtId);
			}
		}
		sySynch(10);
	}
	else {
		if((s8)state->aimAhead != 0)
			count = DOUSE_DROPS_AT_NOTHING;
		else
			count = DOUSE_SWAYING_DROPS_ON_TARGET;
		for(n = 0; n < count; n++) {
			SET_POSITION(&pos, cloud->x.fixed, cloud->y.fixed, cloud->z.fixed);
			syVectorAddPolarOffset(syRandom() * 5 + toFIXED(3), syRandom(),
								   &pos);
			particle = fdActorSpawn(SPRITE_PARTICLE, pos.x, pos.y, pos.z);
			if(particle != 0) {
				particle->update =
					(OBJECT_UPDATE)fdEffectUpdateDriftingFallActor;
				particle->motionFlg = 0;
				((SPR *)particle->spr)->image.pri = 2;
				fdActorSelectAnimationStub(particle, 8);
				fdActorSetSprColModeStub((ACTOR *)particle, SPRITE_COLOR_WHITE);
			}
			sySynch(6);
		}
		sySynch(70);
	}

	i = 0;
	do {
		s32 scale;

		SET_ACTOR_POSITION(cloud, end.x + (start.x - end.x) * i / GROW_STEPS,
						   end.y + (start.y - end.y) * i / GROW_STEPS,
						   end.z + (start.z - end.z) * i / GROW_STEPS);
		scale = fdEffectInterpolate(FIXED_ONE / 4, FIXED_ONE, i);
		cloud->scaleX = scale;
		cloud->scaleY = scale;
		sySynch(1);
		i++;
	} while(i < steps);
	fdActorReleaseStub((ACTOR *)cloud);
	fdPsynergyFadeFromCastCol();
}

/* Task while Carry holds its target: drops one particle a frame round the */
/* lifted actor, 8 to 24 pixels above it and 0 to 48 pixels away in a random */
/* direction (fdEffectInitFallingFragment). */
void fdEffectSpawnFallingParticles(void) {
	ACTOR *liftedActor;
	VECTOR3 position;
	ACTOR *fragment;
	u32 radius;

	liftedActor = ((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->tgt;
	position.x = liftedActor->x.fixed;
	position.y = liftedActor->y.fixed - syRandom() * 16 + toFIXED(24);
	position.z = liftedActor->z.fixed;

	radius = syRandom() * 3;
	syVectorAddPolarOffset(radius * 16, syRandom(), &position);

	fragment = fdActorSpawn(SPRITE_SPARKLE, position.x, position.y, position.z);
	fdEffectInitFallingFragment(fragment);
}

void fdEffectSpawnRadialParticleRing(ACTOR *actor);

/* The collision at position for the actor standing 16 pixels higher. */
static __inline__ s32 fdActorProbeRaised(ACTOR *actor, VECTOR3 *position) {
	s32 hit;

	actor->groundHeight += toFIXED(16);
	hit = fdMapGridCheckCollisionStub(actor, position);
	actor->groundHeight -= toFIXED(16);
	return hit;
}

/* When Carry is cast on a target, two hands lift it to 24 pixels above the */
/* ground; the d-pad moves it 16 pixels at a time and A, B, L or R drops it */
/* where it is. */
void fdPsynergySteerLiftedTarget(s32 targetId) {
	FIELD_EFFECT_STATE *state;
	ACTOR *source;
	ACTOR *target;
	ACTOR *shards[CARRY_SHARDS];
	VECTOR3 position;
	ACTOR *anchors[2];
	ACTOR *actor;
	ACTOR *right;
	s32 i;
	s32 angle;
	s32 magnitude;

	state = Ram_HeapSlots->fieldEffState;
	source = state->caster;
	target = state->tgt;
	if(target == NULL)
		return;
	fdPsynergyFadeToCastCol();
	source->watchedActor = (union FIELD_OBJECT *)target;
	fdActorStartScriptStub(source, (u32)FieldEffect_SourceHoldScript);
	SET_POSITION(&position, state->x, state->y + toFIXED(16), state->z);
	actor = fdEffectSpawnCarryHand(position.x + toFIXED(32), position.y,
								   position.z, DEGtoANG(180));
	anchors[0] = actor;
	right = fdEffectSpawnCarryHand(position.x - toFIXED(32), position.y,
								   position.z, 0);
	anchors[1] = right;
	if(anchors[0] == NULL || right == NULL) {
		fdPsynergyFadeFromCastCol();
		return;
	}
	sySynch(15);
	SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(16),
				 target->z.fixed);
	fdActorSetMoveTargetStub(actor, position.x + toFIXED(16), position.y,
							 position.z);
	fdActorSetMoveTargetStub(right, position.x - toFIXED(16), position.y,
							 position.z);
	fdActorWaitForMoveStub(actor);
	fdActorWaitForMoveStub(right);
	actor->x.fixed = position.x + toFIXED(16);
	actor->velX = 0;
	right->x.fixed = position.x - toFIXED(16);
	right->velX = 0;
	target->update = fdActorCycleSprCols;
	sySetEvent((s32)fdEffectSpawnFallingParticles, TASK_PASS_FRAME);
	sySndPlayStub(SE_PSYNERGY_START);
	target->motionFlg = ACTOR_FLOATS;
	fdActorSetSprFlagsStub(target, 0);
	if(anchors[0] != NULL && anchors[1] != NULL) {
		while(target->y.fixed - target->groundHeight <= toFIXED(24)) {
			actor->y.fixed += toFIXED(3) / 8;
			right->y.fixed += toFIXED(3) / 8;
			target->y.fixed += toFIXED(3) / 8;
			sySynch(1);
		}
	}
	anchors[0]->speedLimit = toFIXED(4);
	anchors[0]->acceleration = toFIXED(1) / 2;
	anchors[1]->speedLimit = toFIXED(4);
	anchors[1]->acceleration = toFIXED(1) / 2;
	target->speedLimit = toFIXED(2) / 5;
	target->acceleration = toFIXED(1) / 5;
	target->steeringFlg = 0;
	target->layer = MAPGRID_LAYER_OVERHEAD;
	for(;;) {
		sySynch(1);
		if((InputWork.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
			break;
		angle = (u16)fdInputGetDpadAng(InputWork.held);
		if(angle == DPAD_ANGLE_NONE) {
			SET_POSITION(&position, target->x.fixed,
						 target->y.fixed + toFIXED(16), target->z.fixed);
			fdActorSetMoveTargetStub(anchors[0], position.x + toFIXED(16),
									 position.y, position.z);
			fdActorSetMoveTargetStub(anchors[1], position.x - toFIXED(16),
									 position.y, position.z);
			fdActorSelectAnimationStub(anchors[0], 1);
			fdActorSelectAnimationStub(anchors[1], 1);
			continue;
		}
		SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(16),
					 target->z.fixed);
		syVectorAddPolarOffset(toFIXED(2), angle, &position);
		fdActorSetMoveTargetStub(anchors[0], position.x + toFIXED(16),
								 position.y, position.z);
		fdActorSetMoveTargetStub(anchors[1], position.x - toFIXED(16),
								 position.y, position.z);
		fdActorWaitForMoveStub(anchors[0]);
		fdActorWaitForMoveStub(anchors[1]);
		SET_POSITION(&position, target->x.fixed, target->groundHeight,
					 target->z.fixed);
		syVectorAddPolarOffset(toFIXED(16), angle, &position);
		i = fdActorScriptCheckOverlapStub(target, &position);
		/* Blocked by an actor or by the ground: the hands strain and a buzz */
		/* sounds. */
		if(i != 0)
			goto blocked;
		if(fdActorProbeRaised(target, &position) > 0) {
		blocked:
			fdActorSelectAnimationStub(anchors[0], 4);
			fdActorSelectAnimationStub(anchors[1], 4);
			if((FrameCount & 15) == 0)
				sySndPlayStub(SE_MENU_ERROR);
			continue;
		}
		{
			ACTOR *second;
			FIXED x;
			FIXED z;

			sySndPlayStub(SE_NOTE_HIGH);
			x = position.x;
			z = position.z;
			fdActorSelectAnimationStub(anchors[0], 4);
			fdActorSelectAnimationStub(anchors[1], 4);
			sySynch(15);
			target->paused = 0;
			target->speedLimit = toFIXED(1) / 5;
			target->acceleration = toFIXED(1) / 5;
			fdActorSetMoveTargetStub(target, position.x, position.y,
									 position.z);
			anchors[0]->speedLimit = toFIXED(1) / 5;
			anchors[0]->acceleration = toFIXED(1) / 5;
			second = anchors[1];
			second->speedLimit = toFIXED(1) / 5;
			second->acceleration = toFIXED(1) / 5;
			fdActorSetMoveTargetStub(anchors[0], position.x + toFIXED(16),
									 position.y, position.z);
			fdActorSetMoveTargetStub(second, position.x - toFIXED(16),
									 position.y, position.z);
			fdActorWaitForMoveStub(target);
			target->x.fixed = x;
			target->z.fixed = z;
			target->velX = 0;
			target->velZ = 0;
			sySynch(10);
			break;
		}
	}
	fdActorSelectAnimationStub(anchors[0], 4);
	fdActorSelectAnimationStub(anchors[1], 4);
	syCloseEvent((u32)fdEffectSpawnFallingParticles);
	sySndPlayStub(SE_CLICK);
	sySynch(15);
	sySndPlayStub(SE_CLICK);
	sySynch(15);
	SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(16),
				 target->z.fixed);
	for(i = 0; i < CARRY_SHARDS; i++) {
		actor =
			fdActorSpawn(SPRITE_SPARKLE, position.x, position.y, position.z);
		/* DEAD: nothing reads the shards back; the ROM keeps the stores. */
		shards[i] = actor;
		if(actor != NULL) {
			fdActorStartScriptStub(actor, (u32)FieldEffect_FragmentScript);
			actor->speedLimit = syRandom() + toFIXED(2);
			actor->acceleration = toFIXED(2);
			actor->motionFlg = 0;
			magnitude = syRandom() * 24 + toFIXED(8);
			fdActorSetTargetPosFromMagnitudeAng(actor, magnitude, syRandom());
		}
	}
	sySndPlayStub(SE_PSYNERGY_SPARKLE);
	fdActorReleaseStub(anchors[0]);
	fdActorReleaseStub(anchors[1]);
	fdActorSetSprColModeStub(target, state->savedPal);
	fdActorStartScriptStub(target, (u32)(state->savedScript));
	target->update = state->savedFunc;
	target->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	target->velY = toFIXED(10);
	target->bounce = toFIXED(1) / 5;
	target->layer = MAPGRID_LAYER_GROUND;
	source->update = NULL;
	fdActorSetSprColModeStub(source, SPRITE_COLOR_NORMAL);
	if((s8)state->sceneVariant != 0) {
		for(i = 0; i < 90 && target->velY >= 0; i++)
			sySynch(1);
		sySynch(1);
		for(i = 0; i < 90 && target->velY < 0; i++)
			sySynch(1);
		fdEffectSpawnRadialParticleRing(target);
		fdPsynergyFadeFromCastCol();
		sySynch(30);
	}
	else {
		fdPsynergyFadeFromCastCol();
	}
}

#undef CLOAK_END
#undef CLOAK_BLINKS
#undef DOUSE_DROPS_AT_NOTHING
#undef DOUSE_DROPS_ON_TARGET
#undef DOUSE_SWAYING_DROPS_ON_TARGET
#undef DOUSE_TARGET_DROP
#undef CARRY_SHARDS
