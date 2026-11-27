/* Mercury Lighthouse and its entrance, once the pool is woken: standing at the
   pool's heart (METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART) gives the
   leader three lights, shown as a column of light over them. Each hop into the
   pool (the floor's region, its metatile behavior) spends one, and a light
   comes to hold the leader up where they land. A hop within the pool with no
   light left sinks the leader, who is put back at the heart.
   METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP takes the lights away, and
   Retreat removes the column. */

/* A charging spark rises a pixel a tick for this many ticks, its scale
   following half a turn of sine. */
#define SPARK_RISE_TICKS 32

/* An actor script call for a charging spark: it rises with the leader, swelling
   and shrinking, and is released after SPARK_RISE_TICKS. */
static Bool scMercuryLighthouseRiseSpark(ACTOR *effect)
{
	ACTOR *anchor = (ACTOR *)effect->watchedActor;
	Sint16 step = ++effect->tmr;
	FIXED  scale;

	if(step >= SPARK_RISE_TICKS) {
		return FALSE;
	}

	scale = sySin(step * (TRIG_TURN / 2 / SPARK_RISE_TICKS));
	effect->scaleX = scale;
	effect->scaleY = scale;
	effect->x.fixed = anchor->x.fixed;
	effect->y.fixed += toFIXED(1);
	effect->z.fixed = anchor->z.fixed;
	return TRUE;
}

/* An actor script call for the light that flies ahead to a hop's landing
   metatile: it grows to three quarters of its size over 16 ticks. */
static Bool scMercuryLighthouseGrowLight(ACTOR *actor)
{
	Sint16 frm;

	frm = ++actor->tmr;
	if(frm > 16) {
		return FALSE;
	}
	actor->scaleX = frm * 3 * (toFIXED(1) / 64);
	actor->scaleY = frm * 3 * (toFIXED(1) / 64);
	return TRUE;
}

/* An actor script call for a light that goes out: it swells to half as large
   again over 16 ticks before its script releases it. */
static Bool scMercuryLighthouseSwellLightOut(ACTOR *actor)
{
	Sint16 frm;

	frm = ++actor->tmr;
	if(frm > 16) {
		return FALSE;
	}
	actor->scaleX = frm * (toFIXED(1) / 32) + toFIXED(1);
	actor->scaleY = frm * (toFIXED(1) / 32) + toFIXED(1);
	return TRUE;
}

/* An actor script call for the column of light: it glides after the leader, an
   eighth of the way each tick up to 2 pixels, and flickers white every two
   ticks. */
static Sint32 scMercuryLighthouseFollowLeader(ACTOR *actor)
{
	ACTOR *leader;
	FIXED  tgtZ;
	FIXED  tgtX;
	Sint32 deltaX;
	Sint32 deltaZ;
	Sint32 distance;
	Sint32 moveDistance;

	actor->speedLimit = toFIXED(2);
	leader = (ACTOR *)actor->watchedActor;
	actor->acceleration = toFIXED(1);
	tgtX = leader->x.fixed;
	tgtZ = leader->z.fixed;
	actor->tgtX = ACTOR_NO_TARGET;
	actor->tgtY = ACTOR_NO_TARGET;
	actor->tgtZ = ACTOR_NO_TARGET;

	/* Measure in whole pixels first; use fixed-point precision near the
	   leader. */
	deltaX = (tgtX - actor->x.fixed) / toFIXED(1);
	deltaZ = (tgtZ - actor->z.fixed) / toFIXED(1);
	distance = (Uint16)sySquartIWRAM(deltaX * deltaX + deltaZ * deltaZ);
	deltaX = tgtX - actor->x.fixed;
	deltaZ = tgtZ - actor->z.fixed;
	distance <<= 16;
	if(distance < toFIXED(64))
		distance =
			sySquartFX(syMulFX(deltaX, deltaX) + syMulFX(deltaZ, deltaZ));
	moveDistance = distance / 8;
	if(moveDistance > actor->speedLimit)
		moveDistance = actor->speedLimit;

	if(distance < toFIXED(1) / 4) {
		actor->x.fixed = tgtX;
		actor->z.fixed = tgtZ;
	} else {
		if(distance > moveDistance) {
			deltaX = syMulFX(syDivFX(distance, deltaX), moveDistance);
			deltaZ = syMulFX(syDivFX(distance, deltaZ), moveDistance);
		}
		actor->x.fixed += deltaX;
		actor->z.fixed += deltaZ;
	}

	{
		SPR			  *spr = actor->spr;
		SPR_ANIMATION *entry = spr->entries[0];

		entry->colMode = ((TickCount >> 1) & 1) * SPRITE_COLOR_WHITE;
		spr->dirty = 1;
	}
	return 1;
}

/* A pair of charging sparks rises around the leader, one in front and one
   behind, with SE_PSYNERGY_START. */
static void scMercuryLighthouseSpawnSparkPair(void)
{
	ACTOR *leader;
	ACTOR *spark;
	SPR	  *spr;

	leader = FieldStateWork->actors[MainRoutineStateWork.leaderActorId];
	spark = syFieldActorCreate(
		SPRITE_GOLDEN_RINGS, leader->x.fixed, leader->y.fixed, leader->z.fixed);
	if(spark != NULL) {
		spark->groundHeight = leader->groundHeight;
		spr = spark->spr;
		fdActorStartScript(spark, MercuryLighthouseSparkScript);
		spark->motionFlg = 0;
		spark->tmr = 0;
		spark->watchedActor = (ACTOR_OR_EFFECT *)leader;
		if(spr != NULL) {
			fdSprStartAnimation(spr, GOLDEN_RINGS_ANIMATION_FRONT_ARC);
			spr->flg = 0;
			spr->image.pri = 1;
		}
	}
	spark = syFieldActorCreate(
		SPRITE_GOLDEN_RINGS, leader->x.fixed, leader->y.fixed, leader->z.fixed);
	if(spark != NULL) {
		spark->groundHeight = leader->groundHeight;
		spr = spark->spr;
		fdActorStartScript(spark, MercuryLighthouseSparkScript);
		spark->motionFlg = 0;
		spark->tmr = 0;
		spark->watchedActor = (ACTOR_OR_EFFECT *)leader;
		spark->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		if(spr != NULL) {
			fdSprStartAnimation(spr, GOLDEN_RINGS_ANIMATION_BACK_ARC);
			spr->flg = 0;
		}
	}
	sySndPlay(SE_PSYNERGY_START);
}

/* METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART, the pool's heart: the column
   of light appears over the leader if it is not there, and grows a stage with
   each pair of sparks, half a second apart, up to three lights. The column's
   metatile is where a sinking leader comes back to; FLAG_NO_ENCOUNTERS_IN_AREA
   is set while the leader has lights. */
void scMercuryLighthouseChargeLights(void)
{
	MERCURY_LIGHTHOUSE_LIGHTS *state;
	ACTOR					  *leader;
	ACTOR					  *pillar;
	ACTOR					  *actor;
	Sint32					   step;

	state = *(MERCURY_LIGHTHOUSE_LIGHTS **)HEAP_SLOT_BLOCK(
		HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
	leader = FieldStateWork->actors[MainRoutineStateWork.leaderActorId];
	if(state->levels < 0 || state->levels > 2)
		return;
	fdScriptStart();
	pillar = state->pillar;
	if(pillar == NULL) {
		actor = syFieldActorCreate(SPRITE_GOLDEN_RINGS, leader->x.fixed,
			leader->y.fixed + toFIXED(24), leader->z.fixed);
		if(actor != NULL) {
			SPR *spr = actor->spr;

			actor->groundHeight = leader->groundHeight;
			fdActorStartScript(actor, MercuryLighthousePillarScript);
			actor->watchedActor = (ACTOR_OR_EFFECT *)leader;
			actor->motionFlg = ACTOR_FLOATS;
			actor->y.fixed -= toFIXED(1) / 2;
			if(spr != NULL) {
				spr->flg = 0;
				spr->image.pri = 1;
			}
			actor->sprStorageKind = 0;
			state->pillar = actor;
			pillar = actor;
		} else {
			pillar = state->pillar;
		}
	}
	for(step = state->levels; step <= 2; step++) {
		scMercuryLighthouseSpawnSparkPair();
		sySynch(30);
		pillar->sprStorageKind = 1;
		fdActorSelectAnimation(pillar, GOLDEN_RINGS_ANIMATION_COLUMN(step + 1));
	}
	state->levels = 3;
	state->tgtX = (pillar->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	state->tgtZ = (pillar->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	gmFlagSet(FLAG_NO_ENCOUNTERS_IN_AREA);
	fdScriptEnd();
}

/* METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP: the leader loses their
   lights, and the column swells and fades away. */
void scMercuryLighthouseLoseLights(void)
{
	MERCURY_LIGHTHOUSE_LIGHTS *state =
		*(MERCURY_LIGHTHOUSE_LIGHTS **)HEAP_SLOT_BLOCK(
			HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
	ACTOR *tgt;

	if(state->levels == 0) {
		return;
	}

	state->levels = 0;
	gmFlagClear(FLAG_NO_ENCOUNTERS_IN_AREA);

	tgt = state->pillar;
	if(tgt != NULL) {
		tgt->tmr = 0;
		fdActorStartScript(tgt, MercuryLighthouseLightOutScript);
		fdActorSelectAnimation(tgt, GOLDEN_RINGS_ANIMATION_LIGHT_OUT);
		state->pillar = NULL;
	}
}

/* Pushing against the pool's edge
   (METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART,
   METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER and
   METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP): the leader hops two
   metatiles the way the pad points, unless the hop leads into the pool without
   a light or is blocked. A hop into the pool spends a light, which flies ahead
   to the landing metatile; the column shrinks a stage, or goes with the last
   light. Landing in the pool with no light under them, the leader sinks until a
   key is pressed after half a second, and is put back at the heart. */
void scMercuryLighthouseHopAcrossPool(void)
{
	FIELD_STATE				  *fieldState;
	MERCURY_LIGHTHOUSE_LIGHTS *state;
	ACTOR					  *actor;
	ACTOR					  *src;
	ACTOR					  *obj;
	SPR						  *spr;
	MAP_GRID_METATILE		  *from;
	MAP_GRID_METATILE		  *to;
	Uint8					   saved;
	Uint8					  *flg;
	Sint32					   ang;
	Uint32					   keys;
	Sint32					   n;
	Sint32					   frm;
	FIELD_COORDINATE		   pos[3];

	state = *(MERCURY_LIGHTHOUSE_LIGHTS **)HEAP_SLOT_BLOCK(
		HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
	fieldState = FieldStateWork;
	actor = fieldState->actors[MainRoutineStateWork.leaderActorId];
	flg = &actor->motionFlg;
	saved = *flg;
	keys = PerData;
	ang = MercuryLighthouseMoveAngles[DPAD_INDEX(keys)];
	if((Sint16)MercuryLighthouseMoveAngles[DPAD_INDEX(keys)] == -1)
		return;
	pos[0].fixed = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	pos[1].fixed = actor->groundHeight;
	pos[2].fixed = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	from = &MAPGRID_METATILES[(pos[2].fixed / toFIXED(16) * MAPGRID_SIZE) +
		pos[0].fixed / toFIXED(16)];
	syVectorAddPolarOffset(toFIXED(32), ang, &pos[0].fixed);
	to = &MAPGRID_METATILES[(pos[2].fixed / toFIXED(16) * MAPGRID_SIZE) +
		pos[0].fixed / toFIXED(16)];
	if(from->behavior != state->poolBehavior &&
		to->behavior == state->poolBehavior && state->levels == 0)
		return;

	fdScriptStart();
	if(syFieldActorCheckMovementCollision(actor, (FIELD_POS *)pos) != 0)
		return;
	obj = state->stepEff;
	if(obj != NULL) {
		obj->tmr = 0;
		fdActorStartScript(obj, MercuryLighthouseLightOutScript);
		fdActorSelectAnimation(obj, GOLDEN_RINGS_ANIMATION_LIGHT_OUT);
		state->stepEff = NULL;
	}
	if(to->behavior == state->poolBehavior && state->levels != 0) {
		src = state->pillar;
		obj = syFieldActorCreate(
			SPRITE_GOLDEN_RINGS, src->x.fixed, src->y.fixed, src->z.fixed);
		if(obj != NULL) {
			spr = obj->spr;
			obj->groundHeight = src->groundHeight;
			fdActorStartScript(obj, MercuryLighthouseGrowLightScript);
			obj->motionFlg = 0;
			obj->tmr = 0;
			obj->priFlg = ACTOR_PRIORITY_UNDERFOOT;
			obj->speedLimit = toFIXED(4);
			obj->acceleration = toFIXED(2);
			syFieldActorSetMoveTargetFX(
				obj, pos[0].fixed, pos[1].fixed, pos[2].fixed);
			if(spr != NULL) {
				fdSprStartAnimation(spr, GOLDEN_RINGS_ANIMATION_POOL_LIGHT);
				spr->flg = 0;
			}
			state->stepEff = obj;
		}
		n = state->levels - 1;
		state->levels = n;
		if(n == 0) {
			fdActorRelease(state->pillar);
			state->pillar = NULL;
			gmFlagClear(FLAG_NO_ENCOUNTERS_IN_AREA);
		} else if(state->pillar != NULL) {
			fdActorSelectAnimation(
				state->pillar, GOLDEN_RINGS_ANIMATION_COLUMN(n));
		}
	}
	fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
	sySynch(3);
	sySndPlay(SE_JUMP);
	fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);
	actor->speedLimit = toFIXED(3);
	actor->acceleration = toFIXED(2);
	actor->velY = toFIXED(4);
	*flg &= ACTOR_JUMP_KEEPS_MOTION;
	fdActorSetSprFlags(actor, 0);
	fdActorMoveToAndWaitById(
		ACTOR_PARTY_LEADER, pos[0].part[0].integer, pos[2].part[0].integer);
	fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
	sySynch(2);
	if(to->behavior != state->poolBehavior)
		fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
	else
		sySndPlay(SE_SPLASH_SMALL);
	sySynch(1);
	*flg = saved;
	if(to->behavior == state->poolBehavior && state->stepEff == NULL) {
		fdActorSelectAnimation(actor, ANIM_LEADER_BATHE);
		sySndPlay(SE_WATER_MOVE);
		for(frm = 0;; frm++) {
			if((frm & 15) == 0)
				scMercuryLighthouseSpawnRipple(actor);
			if(frm > 31 && PerPush != 0)
				break;
			sySynch(1);
		}
		sySndPlay(SE_STOP_ACTION_SE);
		sySynch(1);
		actor->x.fixed = state->tgtX;
		actor->z.fixed = state->tgtZ;
		fdActorSetSprFlags(actor, SPRITE_FLAG_SHADOW);
	}
	state->rippleTmr = 0;
	fdScriptEnd();
	fieldState->walkingDistance +=
		syMulFX(fieldState->walkingDistanceScale, toFIXED(32));
}

/* METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER: while the leader stands
   there, a blue ripple spreads from him every 10 to 39 ticks. */
void scMercuryLighthouseRippleAroundLeader(void)
{
	MERCURY_LIGHTHOUSE_LIGHTS *state;
	ACTOR					  *leader;

	state = *(MERCURY_LIGHTHOUSE_LIGHTS **)HEAP_SLOT_BLOCK(
		HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
	leader = FieldStateWork->actors[MainRoutineStateWork.leaderActorId];
	if(state->rippleTmr != 0) {
		state->rippleTmr--;
	} else {
		scMercuryLighthouseSpawnRipple(leader);
		state->rippleTmr = (syRandomBelow(30)) + 10;
	}
}

/* Retreat (PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST)):
   remove the column of light. */
static void scMercuryLighthouseRemoveColumnOnRetreat(void)
{
	MERCURY_LIGHTHOUSE_LIGHTS **published =
		syHeapSlotAllocEWRAM(HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS, 4);
	MERCURY_LIGHTHOUSE_LIGHTS *state;
	ACTOR					  *pillar;

	if(published == NULL)
		return;
	state = *published;
	pillar = state->pillar;
	if(pillar == NULL)
		return;
	fdActorRelease(pillar);
	state->pillar = NULL;
}
