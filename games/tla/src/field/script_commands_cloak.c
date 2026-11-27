/* Cloak, Douse, falling particles and steering a lifted target. Part of
   script_commands.c. */

/* The Psynergy request that ends Cloak, without paying PP: what the ground
   check (fdEffectCheckCloakTile) makes when the caster does not stand on a tile
   it can hide on. */
#define CLOAK_END	 (ABILITY_CLOAK | ACT_PACKED)
/* The times the caster blinks as Cloak hides it. */
#define CLOAK_BLINKS 10

/* The drops Douse rains: cast at nothing, cast on a target, and cast on a
   target where they sway down (FIELD_EFFECT_STATE.lit). */
#define DOUSE_DROPS_AT_NOTHING		  10
#define DOUSE_DROPS_ON_TARGET		  24
#define DOUSE_SWAYING_DROPS_ON_TARGET 30
/* The drop after which the target's Douse cue runs. */
#define DOUSE_TARGET_DROP			  12
/* The sparkles Carry throws out as it drops its target. */
#define CARRY_SHARDS				  20

/* Cloak the caster: flicker it CLOAK_BLINKS times, leave it half transparent,
   then check the ground under it every tick. When it cannot hide where it
   stands, show it again at once. */
void fdPsynergyStartCloak(void)
{
	FIELD_STATE *event = RamHeapSlots->fieldState;
	ACTOR *caster = ((FIELD_EFFECT_STATE *)RamHeapSlots->fieldEffState)->caster;
	SPR	  *spr = caster->spr;
	SPR_ANIMATION *entry = spr->entries[0];
	Uint32		   n;
	void (*check)(void);
	Sint32 order;

	sySndPlayStub(SE_PSYNERGY_START);
	fdActorSelectAnimationStub(caster, ANIM_STILL);
	caster->update = 0;

	for(n = 0; n < CLOAK_BLINKS; n++) {
		entry->colMode = SPRITE_COLOR_WHITE;
		spr->dirty = 1;
		spr->flg = SPRITE_FLAG_OUTLINED;
		sySynch(2);
		spr->dirty = 1;
		spr->flg = 0;
		sySynch(2);
	}

	order = EVENT_PASS_TICK;
	spr->flg = SPRITE_FLAG_OUTLINED;
	entry->colMode = SPRITE_COLOR_NORMAL;
	spr->dirty = 1;
	check = fdEffectCheckCloakTile;
	sySetEvent(check, order);
	MainRoutineStateWork.cloaked = 1;
	check();
	if(event->psynergy == CLOAK_END) {
		fdPsynergyEndCloak();
		event->psynergy = 0;
	}
}

/* A Douse drop's per-tick update: once it reaches its ground it starts the
   shared particle script and stops updating. */
void fdEffectLandDouseDrop(ACTOR *actor)
{
	if(actor->y.fixed <= actor->groundHeight) {
		actor->wait = 2;
		fdActorStartScriptStub(actor, (Uint32)FieldEffectReleaseScript);
		actor->update = 0;
	}
}

/* Per-tick update: drifts from side to side, up to a pixel a tick, as it
   falls 0.3 pixels a tick, and is released on reaching its ground height. */
void fdEffectUpdateDriftingFallActor(ACTOR *particle)
{
	Sint32 drift;

	particle->y.fixed -= toFIXED(3) / 10;
	drift = syRandom();
	particle->x.fixed += drift - syRandom();
	if(particle->y.fixed <= particle->groundHeight) {
		fdActorStartScriptStub(particle, FieldEffectReleaseScript);
	}
}

#include "field/psynergy_douse_on_target.c"

/* A rain cloud rises from 16 pixels above the caster to 32 pixels above the
   destination (80 with childOption, 32 more over a Moai) over GROW_STEPS
   ticks, growing as it goes, rains drops, and returns. */
void fdPsynergyDouse(void)
{
	VECTOR3				pos;
	VECTOR3				start;
	VECTOR3				end;
	FIELD_EFFECT_STATE *state = RamHeapSlots->fieldEffState;
	ACTOR			   *tgt = state->tgt;
	ACTOR			   *caster = state->caster;
	Sint32				cnt;
	ACTOR			   *cloud;
	Sint32				raised;
	Sint32				i;
	Sint32				steps = GROW_STEPS + 1;
	Sint32				y;
	Sint32				n;
	ACTOR			   *particle;
	PENDING_CUE		   *cue;

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
	if((Sint8)state->sceneVariant != 0)
		end.y = y + toFIXED(80);
	if(fdActorGetSprIdStub(tgt) == SPRITE_MOAI) {
		end.y += toFIXED(32);
		raised = 1;
	}

	for(; i < steps; i++) {
		Sint32 scale;

		SET_ACTOR_POSITION(cloud, start.x + (end.x - start.x) * i / GROW_STEPS,
			start.y + (end.y - start.y) * i / GROW_STEPS,
			start.z + (end.z - start.z) * i / GROW_STEPS);
		scale = fdEffectInterpolate(toFIXED(1), toFIXED(1) / 4, i);
		cloud->scaleX = scale;
		cloud->scaleY = scale;
		sySynch(1);
	}
	sySynch(10);

	if((Sint8)state->lit == 0) {
		cnt = DOUSE_DROPS_AT_NOTHING;
		if((Sint8)state->aimAhead == 0)
			cnt = DOUSE_DROPS_ON_TARGET;
		for(n = 0; n < cnt; n++) {
			SET_POSITION(&pos, cloud->x.fixed, cloud->y.fixed, cloud->z.fixed);
			syVectorAddPolarOffset(
				syRandom() * 5 + toFIXED(3), syRandom(), &pos);
			if(n == cnt - 1) {
				sySynch(20);
				SET_POSITION(
					&pos, cloud->x.fixed, cloud->y.fixed, cloud->z.fixed);
			}
			particle = fdActorSpawn(SPRITE_DROPLET, pos.x, pos.y, pos.z);
			if(particle != 0) {
				ACTOR *ground = state->tgt;

				if(ground != 0) {
					particle->groundHeight = ground->y.fixed;
					if(raised == 1)
						particle->groundHeight = ground->y.fixed + toFIXED(32);
				} else {
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
				cue = fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_ACTS),
					FIELD_PSYNERGY_DOUSE,
					fdSelectPsynergyTarget(FIELD_PSYNERGY_DOUSE));
				if(cue != 0)
					fdRunPsynergyScript(cue, state->casterId, state->tgtId);
			}
		}
		sySynch(10);
	} else {
		if((Sint8)state->aimAhead != 0)
			cnt = DOUSE_DROPS_AT_NOTHING;
		else
			cnt = DOUSE_SWAYING_DROPS_ON_TARGET;
		for(n = 0; n < cnt; n++) {
			SET_POSITION(&pos, cloud->x.fixed, cloud->y.fixed, cloud->z.fixed);
			syVectorAddPolarOffset(
				syRandom() * 5 + toFIXED(3), syRandom(), &pos);
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
		Sint32 scale;

		SET_ACTOR_POSITION(cloud, end.x + (start.x - end.x) * i / GROW_STEPS,
			end.y + (start.y - end.y) * i / GROW_STEPS,
			end.z + (start.z - end.z) * i / GROW_STEPS);
		scale = fdEffectInterpolate(toFIXED(1) / 4, toFIXED(1), i);
		cloud->scaleX = scale;
		cloud->scaleY = scale;
		sySynch(1);
		i++;
	} while(i < steps);
	fdActorReleaseStub((ACTOR *)cloud);
	fdPsynergyFadeFromCastCol();
}

/* Event while Carry holds its target: drops one particle a tick round the
   lifted actor, 8 to 24 pixels above it and 0 to 48 pixels away in a random
   direction (fdEffectInitFallingFragment). */
void fdEffectSpawnFallingParticles(void)
{
	ACTOR  *liftedActor;
	VECTOR3 pos;
	ACTOR  *fragment;
	Uint32	radius;

	liftedActor = ((FIELD_EFFECT_STATE *)RamHeapSlots->fieldEffState)->tgt;
	pos.x = liftedActor->x.fixed;
	pos.y = liftedActor->y.fixed - syRandom() * 16 + toFIXED(24);
	pos.z = liftedActor->z.fixed;

	radius = syRandom() * 3;
	syVectorAddPolarOffset(radius * 16, syRandom(), &pos);

	fragment = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
	fdEffectInitFallingFragment(fragment);
}

void fdEffectSpawnRadialParticleRing(ACTOR *actor);

/* The collision at position for the actor standing 16 pixels higher. */
static __inline__ Sint32 fdActorProbeRaised(ACTOR *actor, VECTOR3 *pos)
{
	Sint32 hit;

	actor->groundHeight += toFIXED(16);
	hit = fdMapGridCheckCollisionStub(actor, pos);
	actor->groundHeight -= toFIXED(16);
	return hit;
}

/* When Carry is cast on a target, two hands lift it to 24 pixels above the
   ground; the d-pad moves it 16 pixels at a time and A, B, L or R drops it
   where it is. */
void fdPsynergySteerLiftedTarget(Sint32 tgtId)
{
	FIELD_EFFECT_STATE *state;
	ACTOR			   *src;
	ACTOR			   *tgt;
	ACTOR			   *shards[CARRY_SHARDS];
	VECTOR3				pos;
	ACTOR			   *anchors[2];
	ACTOR			   *actor;
	ACTOR			   *right;
	Sint32				i;
	Sint32				ang;
	Sint32				magnitude;

	state = RamHeapSlots->fieldEffState;
	src = state->caster;
	tgt = state->tgt;
	if(tgt == NULL)
		return;
	fdPsynergyFadeToCastCol();
	src->watchedActor = (union FIELD_OBJECT *)tgt;
	fdActorStartScriptStub(src, (Uint32)FieldEffectSourceHoldScript);
	SET_POSITION(&pos, state->x, state->y + toFIXED(16), state->z);
	actor = fdEffectSpawnCarryHand(
		pos.x + toFIXED(32), pos.y, pos.z, DEGtoANG(180));
	anchors[0] = actor;
	right = fdEffectSpawnCarryHand(pos.x - toFIXED(32), pos.y, pos.z, 0);
	anchors[1] = right;
	if(anchors[0] == NULL || right == NULL) {
		fdPsynergyFadeFromCastCol();
		return;
	}
	sySynch(15);
	SET_POSITION(&pos, tgt->x.fixed, tgt->y.fixed + toFIXED(16), tgt->z.fixed);
	fdActorSetMoveTargetStub(actor, pos.x + toFIXED(16), pos.y, pos.z);
	fdActorSetMoveTargetStub(right, pos.x - toFIXED(16), pos.y, pos.z);
	fdActorWaitForMoveStub(actor);
	fdActorWaitForMoveStub(right);
	actor->x.fixed = pos.x + toFIXED(16);
	actor->velX = 0;
	right->x.fixed = pos.x - toFIXED(16);
	right->velX = 0;
	tgt->update = fdActorCycleSprCols;
	sySetEvent((Sint32)fdEffectSpawnFallingParticles, EVENT_PASS_TICK);
	sySndPlayStub(SE_PSYNERGY_START);
	tgt->motionFlg = ACTOR_FLOATS;
	fdActorSetSprFlagsStub(tgt, 0);
	if(anchors[0] != NULL && anchors[1] != NULL) {
		while(tgt->y.fixed - tgt->groundHeight <= toFIXED(24)) {
			actor->y.fixed += toFIXED(3) / 8;
			right->y.fixed += toFIXED(3) / 8;
			tgt->y.fixed += toFIXED(3) / 8;
			sySynch(1);
		}
	}
	anchors[0]->speedLimit = toFIXED(4);
	anchors[0]->acceleration = toFIXED(1) / 2;
	anchors[1]->speedLimit = toFIXED(4);
	anchors[1]->acceleration = toFIXED(1) / 2;
	tgt->speedLimit = toFIXED(2) / 5;
	tgt->acceleration = toFIXED(1) / 5;
	tgt->steeringFlg = 0;
	tgt->layer = MAPGRID_LAYER_OVERHEAD;
	for(;;) {
		sySynch(1);
		if((PerWork.push & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
			break;
		ang = (Uint16)fdInputGetDpadAng(PerWork.data);
		if(ang == DPAD_ANGLE_NONE) {
			SET_POSITION(
				&pos, tgt->x.fixed, tgt->y.fixed + toFIXED(16), tgt->z.fixed);
			fdActorSetMoveTargetStub(
				anchors[0], pos.x + toFIXED(16), pos.y, pos.z);
			fdActorSetMoveTargetStub(
				anchors[1], pos.x - toFIXED(16), pos.y, pos.z);
			fdActorSelectAnimationStub(anchors[0], 1);
			fdActorSelectAnimationStub(anchors[1], 1);
			continue;
		}
		SET_POSITION(
			&pos, tgt->x.fixed, tgt->y.fixed + toFIXED(16), tgt->z.fixed);
		syVectorAddPolarOffset(toFIXED(2), ang, &pos);
		fdActorSetMoveTargetStub(anchors[0], pos.x + toFIXED(16), pos.y, pos.z);
		fdActorSetMoveTargetStub(anchors[1], pos.x - toFIXED(16), pos.y, pos.z);
		fdActorWaitForMoveStub(anchors[0]);
		fdActorWaitForMoveStub(anchors[1]);
		SET_POSITION(&pos, tgt->x.fixed, tgt->groundHeight, tgt->z.fixed);
		syVectorAddPolarOffset(toFIXED(16), ang, &pos);
		i = fdActorScriptCheckOverlapStub(tgt, &pos);
		/* Blocked by an actor or by the ground: the hands strain and a buzz
		   sounds. */
		if(i != 0)
			goto blocked;
		if(fdActorProbeRaised(tgt, &pos) > 0) {
		blocked:
			fdActorSelectAnimationStub(anchors[0], 4);
			fdActorSelectAnimationStub(anchors[1], 4);
			if((TickCount & 15) == 0)
				sySndPlayStub(SE_MENU_ERROR);
			continue;
		}
		{
			ACTOR *second;
			FIXED  x;
			FIXED  z;

			sySndPlayStub(SE_NOTE_HIGH);
			x = pos.x;
			z = pos.z;
			fdActorSelectAnimationStub(anchors[0], 4);
			fdActorSelectAnimationStub(anchors[1], 4);
			sySynch(15);
			tgt->paused = 0;
			tgt->speedLimit = toFIXED(1) / 5;
			tgt->acceleration = toFIXED(1) / 5;
			fdActorSetMoveTargetStub(tgt, pos.x, pos.y, pos.z);
			anchors[0]->speedLimit = toFIXED(1) / 5;
			anchors[0]->acceleration = toFIXED(1) / 5;
			second = anchors[1];
			second->speedLimit = toFIXED(1) / 5;
			second->acceleration = toFIXED(1) / 5;
			fdActorSetMoveTargetStub(
				anchors[0], pos.x + toFIXED(16), pos.y, pos.z);
			fdActorSetMoveTargetStub(second, pos.x - toFIXED(16), pos.y, pos.z);
			fdActorWaitForMoveStub(tgt);
			tgt->x.fixed = x;
			tgt->z.fixed = z;
			tgt->velX = 0;
			tgt->velZ = 0;
			sySynch(10);
			break;
		}
	}
	fdActorSelectAnimationStub(anchors[0], 4);
	fdActorSelectAnimationStub(anchors[1], 4);
	syCloseEvent((Uint32)fdEffectSpawnFallingParticles);
	sySndPlayStub(SE_CLICK);
	sySynch(15);
	sySndPlayStub(SE_CLICK);
	sySynch(15);
	SET_POSITION(&pos, tgt->x.fixed, tgt->y.fixed + toFIXED(16), tgt->z.fixed);
	for(i = 0; i < CARRY_SHARDS; i++) {
		actor = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
		/* DEAD: nothing reads the shards back; the ROM keeps the stores. */
		shards[i] = actor;
		if(actor != NULL) {
			fdActorStartScriptStub(actor, (Uint32)FieldEffectFragmentScript);
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
	fdActorSetSprColModeStub(tgt, state->savedPal);
	fdActorStartScriptStub(tgt, (Uint32)(state->savedScript));
	tgt->update = state->savedFunc;
	tgt->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	tgt->velY = toFIXED(10);
	tgt->bounce = toFIXED(1) / 5;
	tgt->layer = MAPGRID_LAYER_GROUND;
	src->update = NULL;
	fdActorSetSprColModeStub(src, SPRITE_COLOR_NORMAL);
	if((Sint8)state->sceneVariant != 0) {
		for(i = 0; i < 90 && tgt->velY >= 0; i++)
			sySynch(1);
		sySynch(1);
		for(i = 0; i < 90 && tgt->velY < 0; i++)
			sySynch(1);
		fdEffectSpawnRadialParticleRing(tgt);
		fdPsynergyFadeFromCastCol();
		sySynch(30);
	} else {
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
