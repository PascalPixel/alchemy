// Eight clouds of dust rise in a row across the leader, 2 pixels apart, and fly out from him in
// eight directions when `spread` is set.
void scDustScatter(Sint32 spread)
{
    ACTOR        *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
    FIELD_EFFECT *effect;
    Uint32        frame = 0;
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
            fdActorSetSpriteFlags(effect, 0);
            fdActorSelectAnimation(effect, 2);
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
            effect->velocity.y = -(syRandomBelow(16384) + (toFIXED(3) / 8));
            effect->spin       = syRandomBelow(512) - (TRIG_TURN / 256);
            effect->scaleRateX = -toFIXED(1) / 32;
            effect->scaleRateY = -(toFIXED(3) / 128);
            effect->update     = fdEffectAdvanceMotionAtLeaderPriority;
        }
    }
}
