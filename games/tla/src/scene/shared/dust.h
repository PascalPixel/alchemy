// The dust a crumbling pillar throws up around the leader.

extern Uint32 DustScatterScript[];

// Eight clouds of dust rise in a row across the leader, 2 pixels apart, and fly out from him in
// eight directions when `spread` is set.
void          scDustScatter(Sint32 spread)
{
	ACTOR        *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT *effect;
	void         *frame = 0;
	Uint32        i;
	FIXED         y;

	for (i = 0; i <= 7; i++)
	{
		y      = leader->y.fixed - toFIXED(syRandomBelow(14));
		effect = (FIELD_EFFECT *)scFieldActorCreate(SPRITE_DUST_CLOUD,
		                                            i * toFIXED(2) + leader->x.fixed - toFIXED(8),
		                                            y - toFIXED(2),
		                                            leader->z.fixed);
		if (effect != 0)
		{
			frame                  = fdSpriteShareVRAMCacheEntry(effect->sprite, frame);
			effect->motionFlags    = 0;
			effect->collisionFlags = 0;
			fdActorSetSpriteFlags((ACTOR *)effect, 0);
			fdActorSelectAnimation((ACTOR *)effect, 2);
			fdActorStartScript(effect, DustScatterScript);
			effect->scaleY = effect->scaleX = toFIXED(7) / 10;
			effect->sprite->priority        = 1;
			if (spread != 0)
			{
				effect->velocity.x = syMulFX(syCos(i * DEGtoANG(45)), toFIXED(6) / 5);
				effect->velocity.z = syMathSin(i * DEGtoANG(45));
			}
			else
			{
				effect->velocity.x = spread;
				effect->velocity.z = spread;
			}
			effect->velocity.y = -(syRandomBelow(toFIXED(1) / 4) + toFIXED(3) / 8);
			effect->spin       = syRandomBelow(TRIG_TURN / 128) - TRIG_TURN / 256;
			effect->scaleRateX = -(toFIXED(1) / 32);
			effect->scaleRateY = -(toFIXED(3) / 128);
			effect->update     = fdEffectAdvanceMotionAtLeaderPriority;
		}
	}
}

extern Uint32 DustBurstScript[];

// Throws a ring of dust clouds out from the leader as a pillar lands, the first thrown twice.
void          scDustBurst(void)
{
	ACTOR        *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT *effect;
	void         *frame;
	Uint32        i;

	if (leader == 0)
		return;
	frame = 0;
	for (i = 0; i <= DUST_RING_PUFFS; i++)
	{
		effect = (FIELD_EFFECT *)scFieldActorCreate(
			SPRITE_DUST_CLOUD, leader->x.fixed, leader->y.fixed, leader->z.fixed);
		if (effect != 0)
		{
			frame                  = fdSpriteShareVRAMCacheEntry(effect->sprite, frame);
			effect->motionFlags    = 0;
			effect->collisionFlags = 0;
			fdActorSetSpriteFlags((ACTOR *)effect, 0);
			fdActorSelectAnimation((ACTOR *)effect, 2);
			fdActorStartScript(effect, DustBurstScript);
			effect->velocity.x        = syCos(i * (TRIG_TURN / DUST_RING_PUFFS));
			effect->velocity.y        = 0;
			effect->velocity.z        = syMathSin(i * (TRIG_TURN / DUST_RING_PUFFS));
			effect->velocity.x       += effect->velocity.x >> 1;
			effect->velocity.x       += syRandomBelow(toFIXED(3) / 4) - toFIXED(3) / 8;
			effect->velocity.z       += syRandomBelow(toFIXED(3) / 8) - toFIXED(3) / 16;
			effect->spin              = syRandomBelow(TRIG_TURN / 16) - TRIG_TURN / 32;
			effect->scaleRateX        = 0;
			effect->scaleRateY        = 0;
			effect->update            = fdEffectAdvanceMotionAtLeaderPriority;
			effect->sprite->priority  = ((FIELD_SPR *)leader->sprite)->priority;
		}
	}
}
