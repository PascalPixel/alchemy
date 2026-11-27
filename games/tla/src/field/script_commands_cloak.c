// Cloak, Douse, falling particles and steering a lifted target. Part of script_commands.c.

// The Psynergy request that ends Cloak, without paying PP: what the ground check
// (fdEffectCheckCloakTile) makes when the caster does not stand on a tile it can hide on.
#define CLOAK_END                     (ABILITY_CLOAK | ACT_PACKED)
// The times the caster blinks as Cloak hides it.
#define CLOAK_BLINKS                  10

// The drops Douse rains: cast at nothing, cast on a target, and cast on a target where they sway
// down (FIELD_EFFECT_STATE.lit).
#define DOUSE_DROPS_AT_NOTHING        10
#define DOUSE_DROPS_ON_TARGET         24
#define DOUSE_SWAYING_DROPS_ON_TARGET 30
// The drop after which the target's Douse cue runs.
#define DOUSE_TARGET_DROP             12
// The sparkles Carry throws out as it drops its target.
#define CARRY_SHARDS                  20

// Cloak the caster: flicker it CLOAK_BLINKS times, leave it half transparent, then check the ground
// under it every tick. When it cannot hide where it stands, show it again at once.
void fdPsynergyStartCloak(void)
{
    FIELD_STATE   *event  = RamHeapSlots->fieldState;
    ACTOR         *caster = ((FIELD_EFFECT_STATE *)RamHeapSlots->fieldEffectState)->caster;
    SPR           *sprite = caster->sprite;
    SPR_ANIMATION *entry  = sprite->entries[0];
    Uint32         n;
    void           (*check)(void);
    Sint32         order;

    sySoundPlayStub(SE_PSYNERGY_START);
    fdActorSelectAnimationStub(caster, ANIM_STILL);
    caster->update = 0;

    for (n = 0; n < CLOAK_BLINKS; n++)
    {
        entry->colorMode = SPRITE_COLOR_WHITE;
        sprite->dirty    = 1;
        sprite->flags    = SPRITE_FLAG_OUTLINED;
        sySynch(2);
        sprite->dirty = 1;
        sprite->flags = 0;
        sySynch(2);
    }

    order            = EVENT_PASS_TICK;
    sprite->flags    = SPRITE_FLAG_OUTLINED;
    entry->colorMode = SPRITE_COLOR_NORMAL;
    sprite->dirty    = 1;
    check            = fdEffectCheckCloakTile;
    sySetEvent(check, order);
    MainRoutineStateWork.cloaked = 1;
    check();
    if (event->psynergy == CLOAK_END)
    {
        fdPsynergyEndCloak();
        event->psynergy = 0;
    }
}

// A Douse drop's per-tick update: once it reaches its ground it starts the shared particle script
// and stops updating.
void fdEffectLandDouseDrop(ACTOR *actor)
{
    if (actor->y.fixed <= actor->groundHeight)
    {
        actor->wait = 2;
        fdActorStartScriptStub(actor, (Uint32)FieldEffectReleaseScript);
        actor->update = 0;
    }
}

// Per-tick update: drifts from side to side, up to a pixel a tick, as it falls 0.3 pixels a tick,
// and is released on reaching its ground height.
void fdEffectUpdateDriftingFallActor(ACTOR *particle)
{
    Sint32 drift;

    particle->y.fixed -= toFIXED(3) / 10;
    drift              = syRandom();
    particle->x.fixed += drift - syRandom();
    if (particle->y.fixed <= particle->groundHeight)
    {
        fdActorStartScriptStub(particle, FieldEffectReleaseScript);
    }
}

// Runs Douse cast on a target, through fdPsynergyDouse.
void fdPsynergyDouseOnTarget(Sint32 unused)
{
    fdPsynergyDouse();
}

// A rain cloud rises from 16 pixels above the caster to 32 pixels above the destination (80 with
// childOption, 32 more over a Moai) over GROW_STEPS ticks, growing as it goes, rains drops, and
// returns.
void fdPsynergyDouse(void)
{
    VECTOR3             pos;
    VECTOR3             start;
    VECTOR3             end;
    FIELD_EFFECT_STATE *state  = RamHeapSlots->fieldEffectState;
    ACTOR              *target = state->target;
    ACTOR              *caster = state->caster;
    Sint32              count;
    ACTOR              *cloud;
    Sint32              raised;
    Sint32              i;
    Sint32              steps = GROW_STEPS + 1;
    Sint32              y;
    Sint32              n;
    ACTOR              *particle;
    PENDING_CUE        *cue;

    cloud = fdActorSpawn(SPRITE_RAIN_CLOUD, i = raised = 0, 0, 0);
    if (cloud == 0)
        return;
    fdPsynergyFadeToCastColor();
    sySoundPlayStub(SE_ITEM_BREAK);
    if (state->target == 0)
    {
        state->x = caster->x.fixed;
        state->z = caster->z.fixed;
        syVectorAddPolarOffset(toFIXED(16), state->angle, (VECTOR3 *)&state->x);
    }
    SET_POSITION(&start, caster->x.fixed, caster->y.fixed + toFIXED(16), caster->z.fixed);
    end.x = state->x;
    y     = state->y;
    end.y = y + toFIXED(32);
    end.z = state->z;
    if ((Sint8)state->sceneVariant != 0)
        end.y = y + toFIXED(80);
    if (fdActorGetSpriteIdStub(target) == SPRITE_MOAI)
    {
        end.y  += toFIXED(32);
        raised  = 1;
    }

    for (; i < steps; i++)
    {
        Sint32 scale;

        SET_ACTOR_POSITION(cloud,
                           start.x + (end.x - start.x) * i / GROW_STEPS,
                           start.y + (end.y - start.y) * i / GROW_STEPS,
                           start.z + (end.z - start.z) * i / GROW_STEPS);
        scale         = fdEffectInterpolate(toFIXED(1), toFIXED(1) / 4, i);
        cloud->scaleX = scale;
        cloud->scaleY = scale;
        sySynch(1);
    }
    sySynch(10);

    if ((Sint8)state->lit == 0)
    {
        count = DOUSE_DROPS_AT_NOTHING;
        if ((Sint8)state->aimAhead == 0)
            count = DOUSE_DROPS_ON_TARGET;
        for (n = 0; n < count; n++)
        {
            SET_POSITION(&pos, cloud->x.fixed, cloud->y.fixed, cloud->z.fixed);
            syVectorAddPolarOffset(syRandom() * 5 + toFIXED(3), syRandom(), &pos);
            if (n == count - 1)
            {
                sySynch(20);
                SET_POSITION(&pos, cloud->x.fixed, cloud->y.fixed, cloud->z.fixed);
            }
            particle = fdActorSpawn(SPRITE_DROPLET, pos.x, pos.y, pos.z);
            if (particle != 0)
            {
                ACTOR *ground = state->target;

                if (ground != 0)
                {
                    particle->groundHeight = ground->y.fixed;
                    if (raised == 1)
                        particle->groundHeight = ground->y.fixed + toFIXED(32);
                }
                else
                {
                    particle->groundHeight = pos.y - toFIXED(32);
                }
                particle->update        = (OBJECT_UPDATE)fdEffectLandDouseDrop;
                particle->motionFlags   = ACTOR_FALLS;
                particle->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_OVERHEAD;
            }
            sySoundPlayStub(SE_WATER);
            sySynch(6);
            if (n == DOUSE_TARGET_DROP)
            {
                cue = fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_ACTS),
                                        FIELD_PSYNERGY_DOUSE,
                                        fdSelectPsynergyTarget(FIELD_PSYNERGY_DOUSE));
                if (cue != 0)
                    fdRunPsynergyScript(cue, state->casterId, state->targetId);
            }
        }
        sySynch(10);
    }
    else
    {
        if ((Sint8)state->aimAhead != 0)
            count = DOUSE_DROPS_AT_NOTHING;
        else
            count = DOUSE_SWAYING_DROPS_ON_TARGET;
        for (n = 0; n < count; n++)
        {
            SET_POSITION(&pos, cloud->x.fixed, cloud->y.fixed, cloud->z.fixed);
            syVectorAddPolarOffset(syRandom() * 5 + toFIXED(3), syRandom(), &pos);
            particle = fdActorSpawn(SPRITE_PARTICLE, pos.x, pos.y, pos.z);
            if (particle != 0)
            {
                particle->update      = (OBJECT_UPDATE)fdEffectUpdateDriftingFallActor;
                particle->motionFlags = 0;
                ((SPR *)particle->sprite)->image.priority = 2;
                fdActorSelectAnimationStub(particle, 8);
                fdActorSetSpriteColorModeStub((ACTOR *)particle, SPRITE_COLOR_WHITE);
            }
            sySynch(6);
        }
        sySynch(70);
    }

    i = 0;
    do
    {
        Sint32 scale;

        SET_ACTOR_POSITION(cloud,
                           end.x + (start.x - end.x) * i / GROW_STEPS,
                           end.y + (start.y - end.y) * i / GROW_STEPS,
                           end.z + (start.z - end.z) * i / GROW_STEPS);
        scale         = fdEffectInterpolate(toFIXED(1) / 4, toFIXED(1), i);
        cloud->scaleX = scale;
        cloud->scaleY = scale;
        sySynch(1);
        i++;
    } while (i < steps);
    fdActorReleaseStub((ACTOR *)cloud);
    fdPsynergyFadeFromCastColor();
}

// Event while Carry holds its target: drops one particle a tick round the lifted actor, 8 to 24
// pixels above it and 0 to 48 pixels away in a random direction (fdEffectInitFallingFragment).
void fdEffectSpawnFallingParticles(void)
{
    ACTOR  *liftedActor;
    VECTOR3 pos;
    ACTOR  *fragment;
    Uint32  radius;

    liftedActor = ((FIELD_EFFECT_STATE *)RamHeapSlots->fieldEffectState)->target;
    pos.x       = liftedActor->x.fixed;
    pos.y       = liftedActor->y.fixed - syRandom() * 16 + toFIXED(24);
    pos.z       = liftedActor->z.fixed;

    radius      = syRandom() * 3;
    syVectorAddPolarOffset(radius * 16, syRandom(), &pos);

    fragment = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
    fdEffectInitFallingFragment(fragment);
}

void                     fdEffectSpawnRadialParticleRing(ACTOR *actor);

// The collision at position for the actor standing 16 pixels higher.
static __inline__ Sint32 fdActorProbeRaised(ACTOR *actor, VECTOR3 *pos)
{
    Sint32 hit;

    actor->groundHeight += toFIXED(16);
    hit                  = fdMapGridCheckCollisionStub(actor, pos);
    actor->groundHeight -= toFIXED(16);
    return hit;
}

// When Carry is cast on a target, two hands lift it to 24 pixels above the ground; the d-pad moves
// it 16 pixels at a time and A, B, L or R drops it where it is.
void fdPsynergySteerLiftedTarget(Sint32 targetId)
{
    FIELD_EFFECT_STATE *state;
    ACTOR              *src;
    ACTOR              *target;
    ACTOR              *shards[CARRY_SHARDS];
    VECTOR3             pos;
    ACTOR              *anchors[2];
    ACTOR              *actor;
    ACTOR              *right;
    Sint32              i;
    Sint32              angle;
    Sint32              magnitude;

    state  = RamHeapSlots->fieldEffectState;
    src    = state->caster;
    target = state->target;
    if (target == NULL)
        return;
    fdPsynergyFadeToCastColor();
    src->watchedActor = (union FIELD_OBJECT *)target;
    fdActorStartScriptStub(src, (Uint32)FieldEffectSourceHoldScript);
    SET_POSITION(&pos, state->x, state->y + toFIXED(16), state->z);
    actor      = fdEffectSpawnCarryHand(pos.x + toFIXED(32), pos.y, pos.z, DEGtoANG(180));
    anchors[0] = actor;
    right      = fdEffectSpawnCarryHand(pos.x - toFIXED(32), pos.y, pos.z, 0);
    anchors[1] = right;
    if (anchors[0] == NULL || right == NULL)
    {
        fdPsynergyFadeFromCastColor();
        return;
    }
    sySynch(15);
    SET_POSITION(&pos, target->x.fixed, target->y.fixed + toFIXED(16), target->z.fixed);
    fdActorSetMoveTargetStub(actor, pos.x + toFIXED(16), pos.y, pos.z);
    fdActorSetMoveTargetStub(right, pos.x - toFIXED(16), pos.y, pos.z);
    fdActorWaitForMoveStub(actor);
    fdActorWaitForMoveStub(right);
    actor->x.fixed   = pos.x + toFIXED(16);
    actor->velocityX = 0;
    right->x.fixed   = pos.x - toFIXED(16);
    right->velocityX = 0;
    target->update   = fdActorCycleSpriteColors;
    sySetEvent((Sint32)fdEffectSpawnFallingParticles, EVENT_PASS_TICK);
    sySoundPlayStub(SE_PSYNERGY_START);
    target->motionFlags = ACTOR_FLOATS;
    fdActorSetSpriteFlagsStub(target, 0);
    if (anchors[0] != NULL && anchors[1] != NULL)
    {
        while (target->y.fixed - target->groundHeight <= toFIXED(24))
        {
            actor->y.fixed  += toFIXED(3) / 8;
            right->y.fixed  += toFIXED(3) / 8;
            target->y.fixed += toFIXED(3) / 8;
            sySynch(1);
        }
    }
    anchors[0]->speedLimit   = toFIXED(4);
    anchors[0]->acceleration = toFIXED(1) / 2;
    anchors[1]->speedLimit   = toFIXED(4);
    anchors[1]->acceleration = toFIXED(1) / 2;
    target->speedLimit       = toFIXED(2) / 5;
    target->acceleration     = toFIXED(1) / 5;
    target->steeringFlags    = 0;
    target->layer            = MAPGRID_LAYER_OVERHEAD;
    for (;;)
    {
        sySynch(1);
        if ((PerWork.push & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
            break;
        angle = (Uint16)fdInputGetDpadAngle(PerWork.data);
        if (angle == DPAD_ANGLE_NONE)
        {
            SET_POSITION(&pos, target->x.fixed, target->y.fixed + toFIXED(16), target->z.fixed);
            fdActorSetMoveTargetStub(anchors[0], pos.x + toFIXED(16), pos.y, pos.z);
            fdActorSetMoveTargetStub(anchors[1], pos.x - toFIXED(16), pos.y, pos.z);
            fdActorSelectAnimationStub(anchors[0], 1);
            fdActorSelectAnimationStub(anchors[1], 1);
            continue;
        }
        SET_POSITION(&pos, target->x.fixed, target->y.fixed + toFIXED(16), target->z.fixed);
        syVectorAddPolarOffset(toFIXED(2), angle, &pos);
        fdActorSetMoveTargetStub(anchors[0], pos.x + toFIXED(16), pos.y, pos.z);
        fdActorSetMoveTargetStub(anchors[1], pos.x - toFIXED(16), pos.y, pos.z);
        fdActorWaitForMoveStub(anchors[0]);
        fdActorWaitForMoveStub(anchors[1]);
        SET_POSITION(&pos, target->x.fixed, target->groundHeight, target->z.fixed);
        syVectorAddPolarOffset(toFIXED(16), angle, &pos);
        i = fdActorScriptCheckOverlapStub(target, &pos);
        // Blocked by an actor or by the ground: the hands strain and a buzz sounds.
        if (i != 0)
            goto blocked;
        if (fdActorProbeRaised(target, &pos) > 0)
        {
        blocked:
            fdActorSelectAnimationStub(anchors[0], 4);
            fdActorSelectAnimationStub(anchors[1], 4);
            if ((TickCount & 15) == 0)
                sySoundPlayStub(SE_MENU_ERROR);
            continue;
        }
        {
            ACTOR *second;
            FIXED  x;
            FIXED  z;

            sySoundPlayStub(SE_NOTE_HIGH);
            x = pos.x;
            z = pos.z;
            fdActorSelectAnimationStub(anchors[0], 4);
            fdActorSelectAnimationStub(anchors[1], 4);
            sySynch(15);
            target->paused       = 0;
            target->speedLimit   = toFIXED(1) / 5;
            target->acceleration = toFIXED(1) / 5;
            fdActorSetMoveTargetStub(target, pos.x, pos.y, pos.z);
            anchors[0]->speedLimit   = toFIXED(1) / 5;
            anchors[0]->acceleration = toFIXED(1) / 5;
            second                   = anchors[1];
            second->speedLimit       = toFIXED(1) / 5;
            second->acceleration     = toFIXED(1) / 5;
            fdActorSetMoveTargetStub(anchors[0], pos.x + toFIXED(16), pos.y, pos.z);
            fdActorSetMoveTargetStub(second, pos.x - toFIXED(16), pos.y, pos.z);
            fdActorWaitForMoveStub(target);
            target->x.fixed   = x;
            target->z.fixed   = z;
            target->velocityX = 0;
            target->velocityZ = 0;
            sySynch(10);
            break;
        }
    }
    fdActorSelectAnimationStub(anchors[0], 4);
    fdActorSelectAnimationStub(anchors[1], 4);
    syCloseEvent((Uint32)fdEffectSpawnFallingParticles);
    sySoundPlayStub(SE_CLICK);
    sySynch(15);
    sySoundPlayStub(SE_CLICK);
    sySynch(15);
    SET_POSITION(&pos, target->x.fixed, target->y.fixed + toFIXED(16), target->z.fixed);
    for (i = 0; i < CARRY_SHARDS; i++)
    {
        actor     = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
        // DEAD: nothing reads the shards back; the ROM keeps the stores.
        shards[i] = actor;
        if (actor != NULL)
        {
            fdActorStartScriptStub(actor, (Uint32)FieldEffectFragmentScript);
            actor->speedLimit   = syRandom() + toFIXED(2);
            actor->acceleration = toFIXED(2);
            actor->motionFlags  = 0;
            magnitude           = syRandom() * 24 + toFIXED(8);
            fdActorSetTargetPosFromMagnitudeAngle(actor, magnitude, syRandom());
        }
    }
    sySoundPlayStub(SE_PSYNERGY_SPARKLE);
    fdActorReleaseStub(anchors[0]);
    fdActorReleaseStub(anchors[1]);
    fdActorSetSpriteColorModeStub(target, state->savedPalette);
    fdActorStartScriptStub(target, (Uint32)(state->savedScript));
    target->update      = state->savedCallback;
    target->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    target->velocityY   = toFIXED(10);
    target->bounce      = toFIXED(1) / 5;
    target->layer       = MAPGRID_LAYER_GROUND;
    src->update         = NULL;
    fdActorSetSpriteColorModeStub(src, SPRITE_COLOR_NORMAL);
    if ((Sint8)state->sceneVariant != 0)
    {
        for (i = 0; i < 90 && target->velocityY >= 0; i++)
            sySynch(1);
        sySynch(1);
        for (i = 0; i < 90 && target->velocityY < 0; i++)
            sySynch(1);
        fdEffectSpawnRadialParticleRing(target);
        fdPsynergyFadeFromCastColor();
        sySynch(30);
    }
    else
    {
        fdPsynergyFadeFromCastColor();
    }
}

#undef CLOAK_END
#undef CLOAK_BLINKS
#undef DOUSE_DROPS_AT_NOTHING
#undef DOUSE_DROPS_ON_TARGET
#undef DOUSE_SWAYING_DROPS_ON_TARGET
#undef DOUSE_TARGET_DROP
#undef CARRY_SHARDS
