// Mercury Lighthouse and its entrance, once the pool is woken: standing at the pool's heart
// (METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART) gives the leader three lights, shown as a
// column of light over them. Each hop into the pool (the floor's region, its metatile behavior)
// spends one, and a light comes to hold the leader up where they land. A hop within the pool with
// no light left sinks the leader, who is put back at the heart.
// METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP takes the lights away, and Retreat removes the
// column.

// A charging spark rises a pixel a tick for this many ticks, its scale following half a turn of
// sine.
#define SPARK_RISE_TICKS 32

// An actor script call for a charging spark: it rises with the leader, swelling and shrinking, and
// is released after SPARK_RISE_TICKS.
static Bool scMercuryLighthouseRiseSpark(ACTOR *effect)
{
    ACTOR *anchor = (ACTOR *)effect->watchedActor;
    Sint16 step   = ++effect->timer;
    FIXED  scale;

    if (step >= SPARK_RISE_TICKS)
    {
        return FALSE;
    }

    scale            = sySin(step * (TRIG_TURN / 2 / SPARK_RISE_TICKS));
    effect->scaleX   = scale;
    effect->scaleY   = scale;
    effect->x.fixed  = anchor->x.fixed;
    effect->y.fixed += toFIXED(1);
    effect->z.fixed  = anchor->z.fixed;
    return TRUE;
}

// An actor script call for the light that flies ahead to a hop's landing metatile: it grows to
// three quarters of its size over 16 ticks.
static Bool scMercuryLighthouseGrowLight(ACTOR *actor)
{
    Sint16 frame;

    frame = ++actor->timer;
    if (frame > 16)
    {
        return FALSE;
    }
    actor->scaleX = frame * 3 * (toFIXED(1) / 64);
    actor->scaleY = frame * 3 * (toFIXED(1) / 64);
    return TRUE;
}

// An actor script call for a light that goes out: it swells to half as large again over 16 ticks
// before its script releases it.
static Bool scMercuryLighthouseSwellLightOut(ACTOR *actor)
{
    Sint16 frame;

    frame = ++actor->timer;
    if (frame > 16)
    {
        return FALSE;
    }
    actor->scaleX = frame * (toFIXED(1) / 32) + toFIXED(1);
    actor->scaleY = frame * (toFIXED(1) / 32) + toFIXED(1);
    return TRUE;
}

// An actor script call for the column of light: it glides after the leader, an eighth of the way
// each tick up to 2 pixels, and flickers white every two ticks.
static Sint32 scMercuryLighthouseFollowLeader(ACTOR *actor)
{
    ACTOR *leader;
    FIXED  targetZ;
    FIXED  targetX;
    Sint32 deltaX;
    Sint32 deltaZ;
    Sint32 distance;
    Sint32 moveDistance;

    actor->speedLimit     = toFIXED(2);
    leader                = (ACTOR *)actor->watchedActor;
    actor->acceleration   = toFIXED(1);
    targetX               = leader->x.fixed;
    targetZ               = leader->z.fixed;
    actor->targetX        = ACTOR_NO_TARGET;
    actor->targetY        = ACTOR_NO_TARGET;
    actor->targetZ        = ACTOR_NO_TARGET;

    // Measure in whole pixels first; use fixed-point precision near the leader.
    deltaX                = (targetX - actor->x.fixed) / toFIXED(1);
    deltaZ                = (targetZ - actor->z.fixed) / toFIXED(1);
    distance              = (Uint16)sySquartIWRAM(deltaX * deltaX + deltaZ * deltaZ);
    deltaX                = targetX - actor->x.fixed;
    deltaZ                = targetZ - actor->z.fixed;
    distance            <<= 16;
    if (distance < toFIXED(64))
        distance = sySquartFX(syMulFX(deltaX, deltaX) + syMulFX(deltaZ, deltaZ));
    moveDistance = distance / 8;
    if (moveDistance > actor->speedLimit)
        moveDistance = actor->speedLimit;

    if (distance < toFIXED(1) / 4)
    {
        actor->x.fixed = targetX;
        actor->z.fixed = targetZ;
    }
    else
    {
        if (distance > moveDistance)
        {
            deltaX = syMulFX(syDivFX(distance, deltaX), moveDistance);
            deltaZ = syMulFX(syDivFX(distance, deltaZ), moveDistance);
        }
        actor->x.fixed += deltaX;
        actor->z.fixed += deltaZ;
    }

    {
        SPR           *sprite = actor->sprite;
        SPR_ANIMATION *entry  = sprite->entries[0];

        entry->colorMode      = ((TickCount >> 1) & 1) * SPRITE_COLOR_WHITE;
        sprite->dirty         = 1;
    }
    return 1;
}

// A pair of charging sparks rises around the leader, one in front and one behind, with
// SE_PSYNERGY_START.
static void scMercuryLighthouseSpawnSparkPair(void)
{
    ACTOR *leader;
    ACTOR *spark;
    SPR   *sprite;

    leader = FieldStateWork->actors[MainRoutineStateWork.leaderActorId];
    spark =
        syFieldActorCreate(SPRITE_GOLDEN_RINGS, leader->x.fixed, leader->y.fixed, leader->z.fixed);
    if (spark != NULL)
    {
        spark->groundHeight = leader->groundHeight;
        sprite              = spark->sprite;
        fdActorStartScript(spark, MercuryLighthouseSparkScript);
        spark->motionFlags  = 0;
        spark->timer        = 0;
        spark->watchedActor = (ACTOR_OR_EFFECT *)leader;
        if (sprite != NULL)
        {
            fdSpriteStartAnimation(sprite, GOLDEN_RINGS_ANIMATION_FRONT_ARC);
            sprite->flags          = 0;
            sprite->image.priority = 1;
        }
    }
    spark =
        syFieldActorCreate(SPRITE_GOLDEN_RINGS, leader->x.fixed, leader->y.fixed, leader->z.fixed);
    if (spark != NULL)
    {
        spark->groundHeight = leader->groundHeight;
        sprite              = spark->sprite;
        fdActorStartScript(spark, MercuryLighthouseSparkScript);
        spark->motionFlags   = 0;
        spark->timer         = 0;
        spark->watchedActor  = (ACTOR_OR_EFFECT *)leader;
        spark->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        if (sprite != NULL)
        {
            fdSpriteStartAnimation(sprite, GOLDEN_RINGS_ANIMATION_BACK_ARC);
            sprite->flags = 0;
        }
    }
    sySoundPlay(SE_PSYNERGY_START);
}

// METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART, the pool's heart: the column of light appears
// over the leader if it is not there, and grows a stage with each pair of sparks, half a second
// apart, up to three lights. The column's metatile is where a sinking leader comes back to;
// FLAG_NO_ENCOUNTERS_IN_AREA is set while the leader has lights.
void scMercuryLighthouseChargeLights(void)
{
    MERCURY_LIGHTHOUSE_LIGHTS *state;
    ACTOR                     *leader;
    ACTOR                     *pillar;
    ACTOR                     *actor;
    Sint32                     step;

    state  = *(MERCURY_LIGHTHOUSE_LIGHTS **)HEAP_SLOT_BLOCK(HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
    leader = FieldStateWork->actors[MainRoutineStateWork.leaderActorId];
    if (state->levels < 0 || state->levels > 2)
        return;
    fdScriptStart();
    pillar = state->pillar;
    if (pillar == NULL)
    {
        actor = syFieldActorCreate(
            SPRITE_GOLDEN_RINGS, leader->x.fixed, leader->y.fixed + toFIXED(24), leader->z.fixed);
        if (actor != NULL)
        {
            SPR *sprite         = actor->sprite;

            actor->groundHeight = leader->groundHeight;
            fdActorStartScript(actor, MercuryLighthousePillarScript);
            actor->watchedActor  = (ACTOR_OR_EFFECT *)leader;
            actor->motionFlags   = ACTOR_FLOATS;
            actor->y.fixed      -= toFIXED(1) / 2;
            if (sprite != NULL)
            {
                sprite->flags          = 0;
                sprite->image.priority = 1;
            }
            actor->spriteStorageKind = 0;
            state->pillar            = actor;
            pillar                   = actor;
        }
        else
        {
            pillar = state->pillar;
        }
    }
    for (step = state->levels; step <= 2; step++)
    {
        scMercuryLighthouseSpawnSparkPair();
        sySynch(30);
        pillar->spriteStorageKind = 1;
        fdActorSelectAnimation(pillar, GOLDEN_RINGS_ANIMATION_COLUMN(step + 1));
    }
    state->levels  = 3;
    state->targetX = (pillar->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    state->targetZ = (pillar->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    gmFlagSet(FLAG_NO_ENCOUNTERS_IN_AREA);
    fdScriptEnd();
}

// METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP: the leader loses their lights, and the column
// swells and fades away.
void scMercuryLighthouseLoseLights(void)
{
    MERCURY_LIGHTHOUSE_LIGHTS *state =
        *(MERCURY_LIGHTHOUSE_LIGHTS **)HEAP_SLOT_BLOCK(HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
    ACTOR *target;

    if (state->levels == 0)
    {
        return;
    }

    state->levels = 0;
    gmFlagClear(FLAG_NO_ENCOUNTERS_IN_AREA);

    target = state->pillar;
    if (target != NULL)
    {
        target->timer = 0;
        fdActorStartScript(target, MercuryLighthouseLightOutScript);
        fdActorSelectAnimation(target, GOLDEN_RINGS_ANIMATION_LIGHT_OUT);
        state->pillar = NULL;
    }
}

// Pushing against the pool's edge (METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART,
// METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER and
// METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP): the leader hops two metatiles the way the pad
// points, unless the hop leads into the pool without a light or is blocked. A hop into the pool
// spends a light, which flies ahead to the landing metatile; the column shrinks a stage, or goes
// with the last light. Landing in the pool with no light under them, the leader sinks until a key
// is pressed after half a second, and is put back at the heart.
void scMercuryLighthouseHopAcrossPool(void)
{
    FIELD_STATE               *fieldState;
    MERCURY_LIGHTHOUSE_LIGHTS *state;
    ACTOR                     *actor;
    ACTOR                     *src;
    ACTOR                     *object;
    SPR                       *sprite;
    MAP_GRID_METATILE         *from;
    MAP_GRID_METATILE         *to;
    Uint8                      saved;
    Uint8                     *flags;
    Sint32                     angle;
    Uint32                     keys;
    Sint32                     n;
    Sint32                     frame;
    FIELD_COORDINATE           pos[3];

    state = *(MERCURY_LIGHTHOUSE_LIGHTS **)HEAP_SLOT_BLOCK(HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
    fieldState = FieldStateWork;
    actor      = fieldState->actors[MainRoutineStateWork.leaderActorId];
    flags      = &actor->motionFlags;
    saved      = *flags;
    keys       = PerData;
    angle      = MercuryLighthouseMoveAngles[DPAD_INDEX(keys)];
    if ((Sint16)MercuryLighthouseMoveAngles[DPAD_INDEX(keys)] == -1)
        return;
    pos[0].fixed = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    pos[1].fixed = actor->groundHeight;
    pos[2].fixed = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    from         = &MAPGRID_METATILES[(pos[2].fixed / toFIXED(16) * MAPGRID_SIZE)
                              + pos[0].fixed / toFIXED(16)];
    syVectorAddPolarOffset(toFIXED(32), angle, &pos[0].fixed);
    to = &MAPGRID_METATILES[(pos[2].fixed / toFIXED(16) * MAPGRID_SIZE)
                            + pos[0].fixed / toFIXED(16)];
    if (from->behavior != state->poolBehavior && to->behavior == state->poolBehavior
        && state->levels == 0)
        return;

    fdScriptStart();
    if (syFieldActorCheckMovementCollision(actor, (FIELD_POS *)pos) != 0)
        return;
    object = state->stepEffect;
    if (object != NULL)
    {
        object->timer = 0;
        fdActorStartScript(object, MercuryLighthouseLightOutScript);
        fdActorSelectAnimation(object, GOLDEN_RINGS_ANIMATION_LIGHT_OUT);
        state->stepEffect = NULL;
    }
    if (to->behavior == state->poolBehavior && state->levels != 0)
    {
        src    = state->pillar;
        object = syFieldActorCreate(SPRITE_GOLDEN_RINGS, src->x.fixed, src->y.fixed, src->z.fixed);
        if (object != NULL)
        {
            sprite               = object->sprite;
            object->groundHeight = src->groundHeight;
            fdActorStartScript(object, MercuryLighthouseGrowLightScript);
            object->motionFlags   = 0;
            object->timer         = 0;
            object->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
            object->speedLimit    = toFIXED(4);
            object->acceleration  = toFIXED(2);
            syFieldActorSetMoveTargetFX(object, pos[0].fixed, pos[1].fixed, pos[2].fixed);
            if (sprite != NULL)
            {
                fdSpriteStartAnimation(sprite, GOLDEN_RINGS_ANIMATION_POOL_LIGHT);
                sprite->flags = 0;
            }
            state->stepEffect = object;
        }
        n             = state->levels - 1;
        state->levels = n;
        if (n == 0)
        {
            fdActorRelease(state->pillar);
            state->pillar = NULL;
            gmFlagClear(FLAG_NO_ENCOUNTERS_IN_AREA);
        }
        else if (state->pillar != NULL)
        {
            fdActorSelectAnimation(state->pillar, GOLDEN_RINGS_ANIMATION_COLUMN(n));
        }
    }
    fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
    sySynch(3);
    sySoundPlay(SE_JUMP);
    fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);
    actor->speedLimit    = toFIXED(3);
    actor->acceleration  = toFIXED(2);
    actor->velocityY     = toFIXED(4);
    *flags              &= ACTOR_JUMP_KEEPS_MOTION;
    fdActorSetSpriteFlags(actor, 0);
    fdActorMoveToAndWaitById(ACTOR_PARTY_LEADER, pos[0].part[0].integer, pos[2].part[0].integer);
    fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
    sySynch(2);
    if (to->behavior != state->poolBehavior)
        fdActorSetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    else
        sySoundPlay(SE_SPLASH_SMALL);
    sySynch(1);
    *flags = saved;
    if (to->behavior == state->poolBehavior && state->stepEffect == NULL)
    {
        fdActorSelectAnimation(actor, ANIM_LEADER_BATHE);
        sySoundPlay(SE_WATER_MOVE);
        for (frame = 0;; frame++)
        {
            if ((frame & 15) == 0)
                scMercuryLighthouseSpawnRipple(actor);
            if (frame > 31 && PerPush != 0)
                break;
            sySynch(1);
        }
        sySoundPlay(SE_STOP_ACTION_SE);
        sySynch(1);
        actor->x.fixed = state->targetX;
        actor->z.fixed = state->targetZ;
        fdActorSetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    }
    state->rippleTimer = 0;
    fdScriptEnd();
    fieldState->walkingDistance += syMulFX(fieldState->walkingDistanceScale, toFIXED(32));
}

// METATILE_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER: while the leader stands there, a blue ripple
// spreads from him every 10 to 39 ticks.
void scMercuryLighthouseRippleAroundLeader(void)
{
    MERCURY_LIGHTHOUSE_LIGHTS *state;
    ACTOR                     *leader;

    state  = *(MERCURY_LIGHTHOUSE_LIGHTS **)HEAP_SLOT_BLOCK(HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS);
    leader = FieldStateWork->actors[MainRoutineStateWork.leaderActorId];
    if (state->rippleTimer != 0)
    {
        state->rippleTimer--;
    }
    else
    {
        scMercuryLighthouseSpawnRipple(leader);
        state->rippleTimer = (syRandomBelow(30)) + 10;
    }
}

// Retreat (PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST)): remove the column of
// light.
static void scMercuryLighthouseRemoveColumnOnRetreat(void)
{
    MERCURY_LIGHTHOUSE_LIGHTS **published =
        syHeapSlotAllocateEWRAM(HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS, 4);
    MERCURY_LIGHTHOUSE_LIGHTS *state;
    ACTOR                     *pillar;

    if (published == NULL)
        return;
    state  = *published;
    pillar = state->pillar;
    if (pillar == NULL)
        return;
    fdActorRelease(pillar);
    state->pillar = NULL;
}
