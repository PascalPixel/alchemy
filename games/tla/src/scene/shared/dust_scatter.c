/* Eight clouds of dust rise in a row across the leader, 2 pixels apart, and */
/* fly out from him in eight directions when `spread` is set. */
void scDustScatter(s32 spread) {
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT *effect;
	u32 frame = 0;
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
			fdActorSetSprFlags(effect, 0);
			fdActorSelectAnimation(effect, 2);
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
			effect->vel.y = -(syRandomBelow(16384) + (toFIXED(3) / 8));
			effect->spin = syRandomBelow(512) - (TRIG_TURN / 256);
			effect->scaleRateX = -FIXED_ONE / 32;
			effect->scaleRateY = -(FIXED_ONE * 3 / 128);
			effect->update = fdEffectAdvanceMotionAtLeaderPriority;
		}
	}
}
