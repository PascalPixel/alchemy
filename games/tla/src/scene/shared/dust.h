/* @file */
/* The dust a crumbling pillar throws up around the leader. */

extern u32 Dust_ScatterScript[];

/* Eight clouds of dust rise in a row across the leader, 2 pixels apart, and */
/* fly out from him in eight directions when `spread` is set. */
void scDustScatter(s32 spread) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT *effect;
	void *frame = 0;
	u32 i;
	FIXED y;

	for(i = 0; i <= 7; i++) {
		y = leader->y.fixed - toFIXED(syRandomBelow(14));
		effect = (FIELD_EFFECT *)scFieldActorCreate(
			SPRITE_DUST_CLOUD, i * toFIXED(2) + leader->x.fixed - toFIXED(8),
			y - toFIXED(2), leader->z.fixed);
		if(effect != 0) {
			frame = fdSprShareVramCacheEntry(effect->spr, frame);
			effect->motionFlg = 0;
			effect->collisionFlg = 0;
			fdActorSetSprFlags((ACTOR *)effect, 0);
			fdActorSelectAnimation((ACTOR *)effect, 2);
			fdActorStartScript(effect, Dust_ScatterScript);
			effect->scaleY = effect->scaleX = FIXED_ONE * 7 / 10;
			effect->spr->pri = 1;
			if(spread != 0) {
				effect->vel.x =
					syMulFX(syCos(i * DEGtoANG(45)), toFIXED(6) / 5);
				effect->vel.z = syMathSin(i * DEGtoANG(45));
			}
			else {
				effect->vel.x = spread;
				effect->vel.z = spread;
			}
			effect->vel.y = -(syRandomBelow(toFIXED(1) / 4) + toFIXED(3) / 8);
			effect->spin = syRandomBelow(TRIG_TURN / 128) - TRIG_TURN / 256;
			effect->scaleRateX = -(FIXED_ONE / 32);
			effect->scaleRateY = -(FIXED_ONE * 3 / 128);
			effect->update = fdEffectAdvanceMotionAtLeaderPriority;
		}
	}
}

extern u32 Dust_BurstScript[];

/* Throws a ring of dust clouds out from the leader as a pillar lands, the */
/* first thrown twice. */
void scDustBurst(void) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT *effect;
	void *frame;
	u32 i;

	if(leader == 0)
		return;
	frame = 0;
	for(i = 0; i <= DUST_RING_PUFFS; i++) {
		effect = (FIELD_EFFECT *)scFieldActorCreate(
			SPRITE_DUST_CLOUD, leader->x.fixed, leader->y.fixed,
			leader->z.fixed);
		if(effect != 0) {
			frame = fdSprShareVramCacheEntry(effect->spr, frame);
			effect->motionFlg = 0;
			effect->collisionFlg = 0;
			fdActorSetSprFlags((ACTOR *)effect, 0);
			fdActorSelectAnimation((ACTOR *)effect, 2);
			fdActorStartScript(effect, Dust_BurstScript);
			effect->vel.x = syCos(i * (TRIG_TURN / DUST_RING_PUFFS));
			effect->vel.y = 0;
			effect->vel.z = syMathSin(i * (TRIG_TURN / DUST_RING_PUFFS));
			effect->vel.x += effect->vel.x >> 1;
			effect->vel.x += syRandomBelow(toFIXED(3) / 4) - toFIXED(3) / 8;
			effect->vel.z += syRandomBelow(toFIXED(3) / 8) - toFIXED(3) / 16;
			effect->spin = syRandomBelow(TRIG_TURN / 16) - TRIG_TURN / 32;
			effect->scaleRateX = 0;
			effect->scaleRateY = 0;
			effect->update = fdEffectAdvanceMotionAtLeaderPriority;
			effect->spr->pri = ((FIELD_SPR *)leader->spr)->pri;
		}
	}
}
