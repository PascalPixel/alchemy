// Actors' movement types and what they attach, and the leader's moves off the ground: jumping a
// gap, climbing a wall up and down, and stepping onto and off a rope or tightrope. Part of
// script_commands.c.

extern const Uint8 ActorWaitScript[];

// The script command "actor set attached effect", by ATTACHED_EFFECT_FLAG: removes the actor
// attached to an actor, attaches one (SPRITE_RAY_ARC, animation 1) unless one already is, or shows
// a one-off one (animation 2) that ends by itself (ActorWaitScript).
void               fdEffectSetAttached(Sint32 actorId, Sint32 flags)
{
    ACTOR *actor = fdActorLookup(actorId);
    ACTOR *effect;
    SPR   *visual;
    Sint32 mode;

    effect = 0;
    visual = 0;

    if (actor == 0)
        return;

    if ((flags & 3) != 0)
    {
        if ((flags & 3) == ATTACHED_EFFECT_ONCE
            || fdActorIsNotAttachedEffect((ACTOR *)actor->watchedActor))
        {
            effect =
                fdActorCreateStub(SPRITE_RAY_ARC, actor->x.fixed, actor->y.fixed, actor->z.fixed);
        }
    }
    else
    {
        effect = (ACTOR *)actor->watchedActor;
        if (effect == 0)
            return;
        fdActorReleaseStub(effect);
        actor->watchedActor = NULL;
        return;
    }

    if (effect == 0)
        return;

    mode = flags & 3;
    switch (mode)
    {
        case ATTACHED_EFFECT_ATTACH:
            fdActorSelectAnimationStub(effect, 1);
            actor->watchedActor = (union FIELD_OBJECT *)effect;
            effect->timer       = 1;
            break;
        case ATTACHED_EFFECT_ONCE:
            fdActorSelectAnimationStub(effect, 2);
            fdActorStartScriptStub(effect, ActorWaitScript);
            effect->timer = 1;
            break;
    }

    effect->userValue    = actorId;
    effect->motionFlags  = 0;
    effect->update       = (OBJECT_UPDATE)fdEffectCopyWatchedActorPos;
    visual               = effect->sprite;
    visual->flags        = 0;
    effect->watchedActor = (union FIELD_OBJECT *)actor;

    // The effect takes sprite priority 1, or the actor's.
    if (flags & ATTACHED_EFFECT_IN_FRONT)
        ((PLACED_SPR *)visual)->priority = 1;
    else
        ((PLACED_SPR *)visual)->priority = ((PLACED_SPR *)actor->sprite)->priority;
}

// The SC_CALL function of MOVEMENT_TYPE_CLEAR_COLLISION: clears an actor's sprite flags and its
// collision flags, so nothing collides with it.
Sint32 fdActorClearSpriteAndCollisionFlags(void *object)
{
    fdActorSetSpriteFlagsStub(object, 0);
    ((ACTOR *)object)->collisionFlags = 0;
    return 0;
}

// Actor script hook: clears the actor's sprite flags (outline and shadow).
Sint32 fdActorClearSpriteFlags(ACTOR *object)
{
    fdActorSetSpriteFlagsStub((ACTOR *)object, 0);
    return 0;
}

// Actor script hook: the actor takes its metatiles' priority even where the ground in front rises.
Sint32 fdActorIgnoreRises(ACTOR *actor)
{
    actor->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
    return 0;
}

// The SC_CALL function of MOVEMENT_TYPE_FOLLOW_WATCHED: walks toward the watched actor, setting out
// each tick for 16/timer of the way there, and stands once within its timer's pixels of it.
Sint32 fdActorMoveTowardTarget(ACTOR *actor)
{
    Sint32 step;
    ACTOR *target;
    FIXED  deltaX;
    FIXED  deltaZ;
    Sint32 pixelsX;
    Sint32 pixelsZ;
    FIXED  newX;
    FIXED  newZ;
    Sint32 distance;

    target = (ACTOR *)actor->watchedActor;
    if (target != 0)
    {
        // Whole pixels, rounded toward zero.
        deltaX   = target->x.fixed - actor->x.fixed;
        pixelsX  = deltaX / toFIXED(1);
        deltaZ   = target->z.fixed - actor->z.fixed;
        pixelsZ  = deltaZ / toFIXED(1);
        distance = sySquartIWRAM(pixelsX * pixelsX + pixelsZ * pixelsZ);
        step     = actor->timer;
        if (distance >= step)
        {
            // A target 16 pixels on for each pixel of distance, over the step.
            newX = actor->x.fixed + (Sint32)((Uint32)pixelsX * toFIXED(16)) / step;
            newZ = actor->z.fixed + (Sint32)((Uint32)pixelsZ * toFIXED(16)) / step;
            fdActorSetMoveTargetStub(actor, newX, actor->y.fixed, newZ);
            fdActorSelectAnimationStub(actor, ANIM_WALK);
        }
        else
        {
            fdActorSelectAnimationStub(actor, ANIM_STAND);
        }
    }
    return 1;
}

// The SC_CALL function of MOVEMENT_TYPE_FACE_LEADER: turns the actor toward its watched actor, at
// most 1/16 turn a tick.
Sint32 fdActorTurnTowardWatchedActor(ACTOR *actor)
{
    Sint16 turnStep;
    Uint16 currentAngle;
    Sint32 targetAngle;
    ACTOR *target;

    target = (ACTOR *)actor->watchedActor;
    if (target != NULL)
    {
        // Clear ACTOR_FACES_MOTION, so moving does not turn it away again.
        actor->steeringFlags &= ~ACTOR_FACES_MOTION;
        targetAngle =
            (Uint16)syAtan(target->z.fixed - actor->z.fixed, target->x.fixed - actor->x.fixed);
        currentAngle = actor->facing;
        turnStep     = targetAngle - currentAngle;
        if (turnStep != 0)
        {
            if (turnStep > FACING_STEP)
                turnStep = FACING_STEP;
            if (turnStep < -FACING_STEP)
                turnStep = -FACING_STEP;
            actor->facing = (currentAngle + turnStep);
        }
    }
    return 1;
}

// Keeps an actor turning to face another.
void fdActorWatchActor(ACTOR *actor, ACTOR *target)
{
    actor->watchedActor = (union FIELD_OBJECT *)target;
    actor->update       = fdActorTurnTowardWatchedActor;
}

// Gives an actor the leader's sprite priority, so it is drawn in front of and behind what he is,
// and keeps it there.
Sint32 fdActorTakeLeaderLayer(ACTOR *actor)
{
    ACTOR *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
    Sint32 layer  = ((PLACED_SPR *)leader->sprite)->priority;

    if ((actor->spriteStorageKind & 0xf) == ACTOR_SPRITE_ONE)
    {
        PLACED_SPR *sprite      = actor->sprite;

        sprite->priority        = layer;
        sprite->shadowPriority  = layer;
        actor->priorityFlags   &= ~ACTOR_PRIORITY_AUTOMATIC;
    }
    return 0;
}

extern const Uint8 MovementTypeStandScript[];
extern const Uint8 MovementTypeWanderScript[];
extern const Uint8 MovementTypeWanderNearScript[];
extern const Uint8 MovementTypeWanderCloseScript[];
extern const Uint8 MovementTypeFollowWatchedScript[];
extern const Uint8 MovementTypeFaceLeaderScript[];
extern const Uint8 MovementTypeClearCollisionScript[];
extern const Uint8 MovementTypeSquashScript[];
extern const Uint8 MovementTypeTakeLeaderLayerScript[];
extern const Uint8 MovementTypeIgnoreRisesScript[];
extern const Uint8 MovementTypeClearSpriteFlagsScript[];

// Starts an actor's movement type: an MOVEMENT_TYPE, or any other value as the actor script to run
// itself.
void               fdActorSetMovementType(ACTOR *actor, Sint32 kind)
{
    switch (kind)
    {
        case MOVEMENT_TYPE_STAND         : kind = (Sint32)MovementTypeStandScript; break;
        case MOVEMENT_TYPE_WANDER        : kind = (Sint32)MovementTypeWanderScript; break;
        case MOVEMENT_TYPE_WANDER_NEAR   : kind = (Sint32)MovementTypeWanderNearScript; break;
        case MOVEMENT_TYPE_WANDER_CLOSE  : kind = (Sint32)MovementTypeWanderCloseScript; break;
        case MOVEMENT_TYPE_FOLLOW_WATCHED: kind = (Sint32)MovementTypeFollowWatchedScript; break;
        case MOVEMENT_TYPE_FACE_LEADER:
            actor->watchedActor =
                (union FIELD_OBJECT *)fdActorGetById(MainRoutineStateWork.leaderActorId);
            kind = (Sint32)MovementTypeFaceLeaderScript;
            break;
        case MOVEMENT_TYPE_CLEAR_COLLISION: kind = (Sint32)MovementTypeClearCollisionScript; break;
        case MOVEMENT_TYPE_SQUASH         : kind = (Sint32)MovementTypeSquashScript; break;
        case MOVEMENT_TYPE_TAKE_LEADER_LAYER:
            kind = (Sint32)MovementTypeTakeLeaderLayerScript;
            break;
        case MOVEMENT_TYPE_IGNORE_RISES: kind = (Sint32)MovementTypeIgnoreRisesScript; break;
        case MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS:
            kind = (Sint32)MovementTypeClearSpriteFlagsScript;
            break;
    }
    fdActorStartScriptStub(actor, (const void *)kind);
}

// fdActorFindNearestFacingTarget's reach in pixels, and the distance past which an actor counts
// only within 3/16 turn of the facing.
#define FACING_TARGET_REACH 40
#define FACING_TARGET_NEAR  23

// Finds the nearest animated actor to self within FACING_TARGET_REACH and 48 pixels of height,
// ignoring actors more than 3/16 turn to either side once they are past FACING_TARGET_NEAR; returns
// it if its first sprite animation is sprite id, as The Broken Seal's.
ACTOR *fdActorFindNearestFacingTarget(ACTOR *self, Sint32 id)
{
    ACTOR *entry;
    ACTOR *found;
    Sint32 count;
    Sint32 best;
    FIXED  dy;
    Sint32 dx;
    Sint32 dz;
    Sint32 distance;
    Sint32 angle;
    Sint32 turn;

    found = NULL;
    best  = FACING_TARGET_REACH;
    entry = RamHeapSlots->actors;
    for (count = 0; count < ACTOR_COUNT; count++, entry++)
    {
        if (entry->script == NULL)
            continue;
        if (entry == self)
            continue;
        if (entry->spriteStorageKind != ACTOR_SPRITE_ONE)
            continue;
        dy = entry->y.fixed - self->y.fixed;
        if (dy >= 0)
        {
            if (dy >= toFIXED(48))
                continue;
        }
        else
        {
            if (self->y.fixed - entry->y.fixed >= toFIXED(48))
                continue;
        }
        dx       = (entry->x.fixed - self->x.fixed) / toFIXED(1);
        dz       = (entry->z.fixed - self->z.fixed) / toFIXED(1);
        distance = sySquartIWRAM(dx * dx + dz * dz);
        if (distance >= best)
            continue;
        angle = (Uint16)syAtan(entry->z.fixed - self->z.fixed, entry->x.fixed - self->x.fixed);
        if (distance > FACING_TARGET_NEAR)
        {
            turn = (Sint16)(angle - self->facing);
            if (turn < -(TRIG_TURN * 3 / 16 - 1))
                continue;
            if (turn > TRIG_TURN * 3 / 16 - 1)
                continue;
        }
        found = entry;
        best  = distance;
    }
    if (found == NULL)
        return NULL;
    if (((SPR *)found->sprite)->entries[0]->animationId != id)
        return NULL;
    return found;
}

// Whether the ground at position stops a jump: anything but flat ground or a plain slope
// (TERRAIN_FLAT to TERRAIN_SLOPE_Y) on a field map; never on the world map or with no map.
Bool fdMapGridBlocksJump(ACTOR *actor, Sint32 *pos)
{
    MAP_STATE   *map   = RamHeapSlots->mapGrid;
    FIELD_STATE *event = RamHeapSlots->fieldState;

    if (map == 0 || event->mode == ACTOR_SYSTEM_WORLD_MAP
        || fdMapGridGetTerrainKindStub(actor->layer, pos[0], pos[2]) <= TERRAIN_SLOPE_Y)
        return FALSE;
    return TRUE;
}

extern MAP_METATILE MapGridMetatileBuffer[];

// The middle pixel of the map metatile a whole pixel lies in.
#define METATILE_MIDDLE(pixels)                                                                    \
    (MAPGRID_METATILE_PIXELS / 2 + ((pixels) & MAPGRID_METATILE_PIXEL_MASK))

// The leader jumps two metatiles forward, over the metatile in front, when it faces a quarter turn,
// the map allows it (fdMapGridBlocksJump and the step check) or a platform actor
// (ACTOR_PRIORITY_PLATFORM) waits 16 pixels below the landing, and no solid actor stands there. A
// log underfoot (a SPRITE_LOG_WEST_EAST or SPRITE_LOG_NORTH_SOUTH under the party) dips 2 pixels
// with the leader. Each jump adds to the walking distance, and the leader keeps jumping while it
// lands on METATILE_BEHAVIOR_JUMP_FORWARD. Returns 0 after a jump, -1 when the leader cannot jump.
Sint32 fdTryJumpForward(void)
{
    ACTOR           *leader      = fdActorGetById(MainRoutineStateWork.leaderActorId);
    Sint32           result      = -1;
    Sint32           angle       = leader->facing;
    Uint8           *flags       = &leader->motionFlags;
    Uint8            saved       = *flags;
    FIELD_STATE     *state       = RamHeapSlots->fieldState;
    Sint16           spriteFlags = SPRITE_FLAG_SHADOW;
    FIELD_COORDINATE pos[3];
    ACTOR           *entry;
    ACTOR           *platform;
    Bool             canJump;
    Sint32           i;

    if (MainRoutineStateWork.movementMode == LEADER_MOVEMENT_KEYS)
        return result;
    if (MainRoutineStateWork.movementMode == LEADER_MOVEMENT_ROPE)
        return result;
    if ((angle + FACING_STEP) & DEGtoANG(45)) // not facing a quarter turn
        return result;
    angle = (angle + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
again:
    // The middle of the tile in front, then of the tile beyond.
    pos[0].fixed = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    pos[1].fixed = leader->y.fixed;
    pos[2].fixed = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    syVectorAddPolarOffset(toFIXED(16), angle, &pos[0].fixed);
    if (fdMapGridCheckCollisionStub(leader, &pos[0].fixed) == 1)
        return -1;
    pos[0].fixed = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    pos[1].fixed = leader->y.fixed;
    pos[2].fixed = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    syVectorAddPolarOffset(toFIXED(32), angle, &pos[0].fixed);
    canJump = FALSE;
    if (fdMapGridBlocksJump(leader, &pos[0].fixed) == 0
        && fdMapGridCheckCollisionStub(leader, &pos[0].fixed) == 0)
        canJump = TRUE;

    // A platform 16 pixels below the landing makes it a jump spot.
    if (leader->priorityFlags & ACTOR_PRIORITY_WALKS_ON_PLATFORMS)
    {
        entry = RamHeapSlots->actors;
        for (i = 0; i < ACTOR_COUNT; i++, entry++)
        {
            Sint32 bound = toFIXED(8) - 1;
            FIXED  delta;
            FIXED  dy;

            if (entry->script == NULL)
                continue;
            if (!(entry->priorityFlags & ACTOR_PRIORITY_PLATFORM))
                continue;
            if (entry->arrivalAxis != 0)
                continue;
            if (entry == leader)
                continue;
            if (FIELD_METATILE(pos[0].fixed - entry->x.fixed + toFIXED(8)) != 0)
                continue;
            if (FIELD_METATILE(pos[2].fixed - entry->z.fixed + toFIXED(8)) != 0)
                continue;
            delta = pos[1].fixed - entry->y.fixed;
            dy    = delta - toFIXED(16);
            if (dy >= 0)
            {
                if (dy <= bound)
                    goto hit;
            }
            else
            {
                dy = toFIXED(16) - delta;
                if (dy <= bound)
                {
                hit:
                    canJump = TRUE;
                    break;
                }
            }
        }
    }

    // A solid object at the landing blocks it.
    entry = RamHeapSlots->actors;
    for (i = 0; i < ACTOR_COUNT; i++, entry++)
    {
        if (entry->script != NULL && !(entry->priorityFlags & ACTOR_PRIORITY_PLATFORM)
            && entry != leader && entry->collisionFlags != 0
            && FIELD_METATILE(pos[0].fixed - entry->x.fixed + toFIXED(8)) == 0
            && FIELD_METATILE(pos[2].fixed - entry->z.fixed + toFIXED(8)) == 0)
        {
            FIXED d = pos[1].fixed - entry->y.fixed;
            if (d >= 0)
            {
                if (d < toFIXED(8))
                    goto blocked;
            }
            else if (entry->y.fixed - pos[1].fixed < toFIXED(8))
            {
            blocked:
                canJump = FALSE;
                break;
            }
        }
    }

    if (canJump)
    {
        MAP_STATE *map;
        Uint8      attribute;

        // Crouch, then jump there.
        if (leader->spriteStorageKind == ACTOR_SPRITE_ONE)
            spriteFlags = ((SPR *)leader->sprite)->flags;
        fdScriptStart();
        fdActorSelectAnimationStub(leader, ANIM_LEADER_SPRING);
        sySynch(6);
        sySoundPlayStub(SE_JUMP);
        fdActorSelectAnimationStub(leader, ANIM_LEADER_LEAP);
        leader->speedLimit    = toFIXED(3);
        leader->acceleration  = toFIXED(2);
        leader->velocityY     = toFIXED(4);
        *flags               &= ACTOR_JUMP_KEEPS_MOTION;
        fdActorSetSpriteFlagsStub(leader, spriteFlags & (Uint8)~SPRITE_FLAG_SHADOW);
        fdActorMoveToAndWaitById(
            MainRoutineStateWork.leaderActorId, pos[0].part[0].integer, pos[2].part[0].integer);
        fdActorSelectAnimationStub(leader, ANIM_LEADER_SPRING);
        fdActorSetSpriteFlagsStub(leader, spriteFlags);

        map       = RamHeapSlots->mapGrid;
        attribute = map->groundEffects;
        if (attribute & GROUND_EFFECTS_WADING_MASK)
        {
            Sint32        metatileX = FIELD_METATILE(leader->x.fixed);
            Sint32        metatileZ = FIELD_METATILE(leader->z.fixed);
            MAP_METATILE *metatiles;

            if (map == NULL)
                metatiles = MapGridMetatileBuffer;
            else
                metatiles = map->layers[leader->layer & 3].metatiles;
            metatiles += metatileX + metatileZ * MAPGRID_SIZE;
            if (metatiles->attributeB & MAP_METATILE_WATER)
                fdActorSpawnWaterSplashStub(leader, GROUND_EFFECTS_WADING(attribute));
        }

        // A log underfoot dips 2 pixels with the leader and comes back up.
        if (((platform = fdActorFindNearestFacingTarget(leader, SPRITE_LOG_WEST_EAST)) != NULL
             || (platform = fdActorFindNearestFacingTarget(leader, SPRITE_LOG_NORTH_SOUTH)) != NULL)
            && (platform->priorityFlags & ACTOR_PRIORITY_UNDERFOOT))
        {
            fdActorSelectAnimationStub(platform, 7);
            leader->y.fixed      -= toFIXED(1);
            leader->groundHeight -= toFIXED(1);
            sySynch(2);
            leader->y.fixed      -= toFIXED(1);
            leader->groundHeight -= toFIXED(1);
            sySynch(10);
            leader->y.fixed      += toFIXED(1);
            leader->groundHeight += toFIXED(1);
            sySynch(4);
            leader->y.fixed      += toFIXED(1);
            leader->groundHeight += toFIXED(1);
        }
        else
        {
            sySynch(6);
        }
        *flags = saved;
        fdScriptEnd();
        if (state != NULL)
            state->walkingDistance += syMulFX(state->walkingDistanceScale, toFIXED(32));
        if (fdMapGridGetMetatileBehaviorAtStub(leader->layer, pos[0].fixed, pos[2].fixed)
            == METATILE_BEHAVIOR_JUMP_FORWARD)
        {
            fdActorSelectAnimationStub(leader, ANIM_STAND);
            sySynch(6);
            goto again;
        }
        result = 0;
    }
    leader->timer     = 8;
    leader->userValue = 0;
    return result;
}

// Climbs down onto the wall below the leader, as The Broken Seal's fdClimbDown does, or lets go and
// falls when it cannot.
Sint32 fdClimbDown(void)
{
    ACTOR        *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    MAP_STATE    *map    = RamHeapSlots->mapGrid;
    Sint32        gridX;
    Sint16        spriteFlags;
    MAP_METATILE *ground;
    Sint32        gridY;
    Sint32        gridZ;
    FIXED         position[3];

    spriteFlags = SPRITE_FLAG_SHADOW;
    gridX       = METATILE_MIDDLE(FIXEDtoINT(leader->x.fixed));
    gridY       = METATILE_MIDDLE(FIXEDtoINT(leader->y.fixed));
    gridZ       = METATILE_MIDDLE(FIXEDtoINT(leader->z.fixed));
    fdScriptStart();
    if (leader->spriteStorageKind == ACTOR_SPRITE_ONE)
        spriteFlags = ((SPR *)leader->sprite)->flags;
    if (MainRoutineStateWork.movementMode == LEADER_MOVEMENT_WALK)
    {
        ACTOR        *entry;
        MAP_METATILE *metatile;
        MAP_METATILE *south;
        Sint32        metatileX = gridX / 16;
        Sint32        metatileY = gridY / 16;
        Sint32        metatileZ = gridZ / 16;
        Sint32        radius;
        Sint32        i;

        ground   = &map->layers[leader->layer].metatiles[metatileX + metatileZ * MAPGRID_SIZE];
        metatile = ground;
        south    = metatile + MAPGRID_SIZE;
        if (!(metatile->attributeB & MAP_METATILE_CLIMBABLE)
            || !(south->attributeB & MAP_METATILE_CLIMBABLE))
            goto failure;
        metatile = &map->layers[MAPGRID_LAYER_OVERHEAD]
                        .metatiles[metatileX + (metatileZ - metatileY) * MAPGRID_SIZE];
        south = metatile + MAPGRID_SIZE;
        if (south->collisionCode == METATILE_BEHAVIOR_WALL)
            goto failure;
        if (fdMapGridGetTerrainHeightStub(leader->layer, toFIXED(gridX), toFIXED(gridZ))
            <= fdMapGridGetTerrainHeightStub(leader->layer, toFIXED(gridX), INTtoFIXED(gridZ + 16)))
            goto failure;
        position[0] = leader->x.fixed;
        position[1] = leader->y.fixed - toFIXED(16);
        position[2] = leader->z.fixed;
        if (fdMapGridIsWallAtStub(position) != 0)
            goto failure;
        radius      = leader->radius - 2;
        position[0] = leader->x.fixed;
        position[1] = leader->y.fixed - toFIXED(32);
        position[2] = leader->z.fixed + toFIXED(16);
        entry       = RamHeapSlots->actors;
        for (i = 0; i < ACTOR_COUNT; i++, entry++)
        {
            if (entry->script != NULL && (entry->collisionFlags & ACTOR_SOLID) && entry != leader
                && fdCheckSpheresOverlapStub(&entry->x.fixed, radius, position, entry->radius - 2)
                       >= 0)
                goto failure;
        }

        // Step to the middle of the tile, face north and climb down a tile.
        fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId, gridX, gridZ);
        leader->speedLimit = toFIXED(1);
        fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
        fdActorWaitForScriptEndById(MainRoutineStateWork.leaderActorId);
        leader->steeringFlags  = ACTOR_FACES_MOTION;
        leader->motionFlags    = 0;
        spriteFlags           &= (Uint8)~SPRITE_FLAG_SHADOW;
        fdActorSetSpriteFlagsStub(leader, spriteFlags);
        fdActorSelectAnimationStub(leader, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
        fdActorSetMoveTargetStub(
            leader, toFIXED(gridX), leader->y.fixed - toFIXED(16), toFIXED(gridZ) + toFIXED(16));
        fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
        MainRoutineStateWork.movementMode = LEADER_MOVEMENT_KEYS;
        leader->z.fixed                   = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    }
    else
    {
        // Let go of the wall and fall.
        fdActorSelectAnimationStub(leader, ANIM_LEADER_CLIMB);
        leader->motionFlags   = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        leader->velocityY     = toFIXED(4);
        leader->groundHeight  = leader->y.fixed;
        spriteFlags          |= SPRITE_FLAG_SHADOW;
        fdActorSetSpriteFlagsStub(leader, spriteFlags);
        fdScriptWait(6);
        MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
        leader->steeringFlags             = ACTOR_FACES_MOTION;
        leader->facing                    = TRIG_TURN * 3 / 4;
        leader->timer                     = 0;
        leader->userValue                 = 0;
    }
    fdScriptEnd();
    return 0;

failure:
    fdScriptEnd();
    return -1;
}

// Climbs up the wall in front of the leader, as The Broken Seal's fdClimbUp does.
Sint32 fdClimbUp(void)
{
    ACTOR        *leader      = fdActorGetById(MainRoutineStateWork.leaderActorId);
    MAP_STATE    *map         = RamHeapSlots->mapGrid;
    Sint16        spriteFlags = SPRITE_FLAG_SHADOW;
    MAP_METATILE *ground;
    Sint32        gridX = METATILE_MIDDLE(FIXEDtoINT(leader->x.fixed));
    Sint32        gridY = MAPGRID_METATILE_PIXELS / 2
                 + (FIXEDtoINT(leader->y.fixed) & ~(MAPGRID_METATILE_PIXELS - 1));
    Sint32 gridZ = METATILE_MIDDLE(FIXEDtoINT(leader->z.fixed));
    FIXED  position[3];
    Sint32 result;

    fdScriptStart();
    if (leader->spriteStorageKind == ACTOR_SPRITE_ONE)
        spriteFlags = ((SPR *)leader->sprite)->flags;
    if (MainRoutineStateWork.movementMode == LEADER_MOVEMENT_WALK)
    {
        MAP_METATILE *metatile;
        MAP_METATILE *north;
        Sint32        metatileX = gridX >> 4;
        Sint32        metatileY = gridY >> 4;
        Sint32        metatileZ = gridZ >> 4;

        ground   = &map->layers[leader->layer].metatiles[metatileX + metatileZ * MAPGRID_SIZE];
        metatile = ground;
        north    = metatile - MAPGRID_SIZE;
        if (!(metatile->attributeB & MAP_METATILE_CLIMBABLE)
            || !(north->attributeB & MAP_METATILE_CLIMBABLE))
            goto failure;
        metatile = &map->layers[MAPGRID_LAYER_OVERHEAD]
                        .metatiles[metatileX + (metatileZ - metatileY) * MAPGRID_SIZE];
        north = metatile - MAPGRID_SIZE;
        if (north->collisionCode == METATILE_BEHAVIOR_WALL)
            goto failure;
        if (fdMapGridGetTerrainHeightStub(leader->layer, toFIXED(gridX), toFIXED(gridZ))
            >= fdMapGridGetTerrainHeightStub(leader->layer, toFIXED(gridX), INTtoFIXED(gridZ - 16)))
            goto failure;
        position[0] = leader->x.fixed;
        position[1] = leader->y.fixed;
        position[2] = leader->z.fixed;
        result      = fdMapGridIsWallAtStub(position);
        if (result != 0)
            goto failure;

        // Step to the middle of the tile, crouch, jump and grab the wall.
        leader->steeringFlags = result;
        fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId, gridX, gridZ);
        fdActorSelectAnimationStub(leader, ANIM_LEADER_SPRING);
        sySynch(4);
        fdActorSelectAnimationStub(leader, ANIM_LEADER_LEAP);
        leader->velocityY = toFIXED(4);
        sySynch(4);
        leader->motionFlags  = result;
        spriteFlags         &= (Uint8)~SPRITE_FLAG_SHADOW;
        fdActorSetSpriteFlagsStub(leader, spriteFlags);
        leader->speedLimit = toFIXED(1);
        leader->velocityY  = result;
        fdActorSelectAnimationStub(leader, ANIM_LEADER_CLING);
        sySynch(4);
        MainRoutineStateWork.movementMode = LEADER_MOVEMENT_KEYS;
        leader->steeringFlags             = ACTOR_FACES_MOTION;
        sySynch(8);
        leader->z.fixed = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    }
    else
    {
        ACTOR *entry;
        Sint32 radius = leader->radius - 2;
        Sint32 i;

        position[0] = leader->x.fixed;
        position[1] = leader->y.fixed + toFIXED(16);
        position[2] = leader->z.fixed - toFIXED(16);
        i           = 0;
        entry       = RamHeapSlots->actors;
        for (; i < ACTOR_COUNT; i++, entry++)
        {
            if (entry->script != NULL && (entry->collisionFlags & ACTOR_SOLID) && entry != leader
                && fdCheckSpheresOverlapStub(&entry->x.fixed, radius, position, entry->radius - 2)
                       >= 0)
                goto failure;
        }

        // Climb over the top: up 8 pixels and a tile north.
        leader->motionFlags = 0;
        fdActorSelectAnimationStub(leader, ANIM_LEADER_CLIMB_OVER_TOP);
        fdActorSetMoveTargetStub(
            leader, toFIXED(gridX), leader->y.fixed + toFIXED(8), toFIXED(gridZ) - toFIXED(16));
        fdActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
        leader->motionFlags   = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        leader->groundHeight  = leader->y.fixed;
        spriteFlags          |= SPRITE_FLAG_SHADOW;
        fdActorSetSpriteFlagsStub(leader, spriteFlags);
        fdScriptWait(4);
        MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
        leader->steeringFlags             = ACTOR_FACES_MOTION;
        leader->timer                     = 0;
        leader->userValue                 = 0;
    }
    fdScriptEnd();
    return 0;

failure:
    fdScriptEnd();
    return -1;
}

// How near a rope or tightrope node must be for the leader to hop onto it, in pixels.
#define ROPE_NODE_REACH 32

// Returns the nearest crossable rope node within reach of position, or NULL.
ROPE_NODE *fdRopeFindNodeNear(Sint32 *position)
{
    ROPE_NODE *found;
    ROPE_NODE *result;
    ROPE_WORK *state;
    ROPE_NODE *node;
    Sint32     best;
    Sint32     i;

    found = NULL;
    state = ROPE_STATE;
    if (state == NULL)
    {
        result = NULL;
    }
    else
    {
        node = state->nodes;
        best = ROPE_NODE_DISTANCE_NONE;
        for (i = 0; i < ROPE_NODE_COUNT; i++, node++)
        {
            if (node->kind == ROPE_NODE_CROSSABLE)
            {
                // In pixels, squared.
                Sint32 dx       = (position[0] - node->x) >> 8;
                Sint32 dy       = (position[1] - node->y) >> 8;
                Sint32 dz       = (position[2] - node->z) >> 8;
                Sint32 distance = syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz);

                if (distance <= ROPE_NODE_REACH * ROPE_NODE_REACH && best > distance)
                {
                    best  = distance;
                    found = node;
                }
            }
        }
        result = found;
    }
    return result;
}

// Hops the leader onto the nearest crossable rope node, to cross the rope (LEADER_MOVEMENT_ROPE);
// returns 0 when none is in reach.
Bool fdLeaderStepOntoRope(void)
{
    ACTOR     *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
    SPR       *sprite = leader->sprite;
    ROPE_NODE *node   = fdRopeFindNodeNear(&leader->x.fixed);
    Uint8     *flags;

    FieldStateWork->blockedSteps = 0;
    if (node == NULL)
        return FALSE;
    sySoundPlayStub(SE_JUMP);
    leader->acceleration = toFIXED(1);
    leader->speedLimit   = toFIXED(2);
    leader->velocityY    = toFIXED(4);
    leader->watchedActor = (union FIELD_OBJECT *)node;
    fdActorSelectAnimationStub(leader, ANIM_WALK);
    fdActorSetMoveTargetStub(leader, node->x, node->y, node->z);
    fdActorWaitForMoveStub(leader);
    leader->timer                     = leader->motionFlags;
    flags                             = &leader->priorityFlags;
    leader->userValue                 = sprite->flags;
    leader->savedSpritePriority       = sprite->image.priority;
    leader->priorityFlags             = *flags & ~ACTOR_PRIORITY_AUTOMATIC;
    sprite->flags                     = 0;
    sprite->image.priority            = 1;
    MainRoutineStateWork.movementMode = LEADER_MOVEMENT_ROPE;
    return TRUE;
}

// Hops the leader off a tied rope a metatile in the direction the pad holds, when that metatile is
// open; returns 0 otherwise.
Bool fdLeaderStepOffRope(void)
{
    ACTOR *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
    SPR   *sprite = leader->sprite;
    FIXED  position[3];
    Uint32 angle;

    FieldStateWork->blockedSteps = 0;
    angle                        = RopeKeyHeadings[DPAD_INDEX(PerWork.data)];
    if (angle == DPAD_ANGLE_NONE)
        return FALSE;
    leader->x.fixed = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    leader->z.fixed = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    position[0]     = leader->x.fixed;
    position[1]     = leader->y.fixed;
    position[2]     = leader->z.fixed;
    syVectorAddPolarOffset(toFIXED(16), angle, position);
    leader->groundHeight = leader->y.fixed - toFIXED(8);
    if (fdMapGridGetTerrainKindAtStub(leader->layer, position[0], position[2])
        || fdMapGridCheckCollisionStub(leader, position))
        return FALSE;
    leader->facing         = angle;
    leader->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    leader->motionFlags    = leader->timer;
    leader->scaleX         = toFIXED(1);
    sprite->flags          = leader->userValue;
    sprite->rotation       = 0;
    sySoundPlayStub(SE_JUMP);
    leader->speedLimit   = toFIXED(2);
    leader->velocityY    = toFIXED(4);
    leader->acceleration = toFIXED(1);
    fdActorSelectAnimationStub(leader, ANIM_WALK);
    fdActorSetMoveTargetStub(leader, position[0], position[1], position[2]);
    fdActorWaitForMoveStub(leader);
    sprite->image.priority            = leader->savedSpritePriority;
    MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
    return TRUE;
}

// Returns the nearest crossable tightrope node within reach of position, or NULL.
TIGHTROPE_NODE *fdTightropeFindNodeNear(Sint32 *position)
{
    TIGHTROPE_NODE *found;
    TIGHTROPE_NODE *result;
    TIGHTROPE_WORK *state;
    TIGHTROPE_NODE *node;
    Sint32          best;
    Sint32          i;

    found = NULL;
    state = TIGHTROPE_STATE;
    if (state == NULL)
    {
        result = NULL;
    }
    else
    {
        node = state->nodes;
        best = ROPE_NODE_DISTANCE_NONE;
        for (i = 0; i < ROPE_NODE_COUNT; i++, node++)
        {
            if (node->kind == ROPE_NODE_CROSSABLE)
            {
                // In pixels, squared.
                Sint32 dx       = (position[0] - node->x) >> 8;
                Sint32 dy       = (position[1] - node->y) >> 8;
                Sint32 dz       = (position[2] - node->z) >> 8;
                Sint32 distance = syMulFX(dx, dx) + syMulFX(dy, dy) + syMulFX(dz, dz);

                if (distance <= ROPE_NODE_REACH * ROPE_NODE_REACH && best > distance)
                {
                    best  = distance;
                    found = node;
                }
            }
        }
        result = found;
    }
    return result;
}

// Hops the leader onto the nearest crossable tightrope node (LEADER_MOVEMENT_TIGHTROPE); returns 0
// when none is in reach.
Bool fdLeaderStepOntoTightrope(void)
{
    MAIN_ROUTINE_STATE *state  = &MainRoutineStateWork;
    ACTOR              *leader = fdActorLookup(state->leaderActorId);
    SPR                *sprite = leader->sprite;
    TIGHTROPE_NODE     *node   = fdTightropeFindNodeNear(&leader->x.fixed);
    Uint8              *flags;

    FieldStateWork->blockedSteps = 0;
    if (node == NULL)
        return FALSE;
    sySoundPlayStub(SE_JUMP);
    leader->acceleration = toFIXED(1);
    leader->speedLimit   = toFIXED(2);
    leader->velocityY    = toFIXED(4);
    leader->watchedActor = (union FIELD_OBJECT *)node;
    fdActorSelectAnimationStub(leader, ANIM_WALK);
    fdActorSetMoveTargetStub(leader, node->x, node->y, node->z);
    fdActorWaitForMoveStub(leader);
    leader->timer               = leader->motionFlags;
    leader->userValue           = sprite->flags;
    leader->savedSpritePriority = sprite->image.priority;
    flags                       = &leader->priorityFlags;
    leader->priorityFlags       = *flags & ~ACTOR_PRIORITY_AUTOMATIC;
    sprite->flags               = 0;
    sprite->image.priority      = 1;
    state->movementMode         = LEADER_MOVEMENT_TIGHTROPE;
    return TRUE;
}

// Hops the leader off a tightrope a metatile in the direction the pad holds, when that metatile is
// open; returns 0 otherwise.
Bool fdLeaderStepOffTightrope(void)
{
    ACTOR *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
    SPR   *sprite = leader->sprite;
    FIXED  position[3];
    Uint32 angle;

    FieldStateWork->blockedSteps = 0;
    angle                        = RopeKeyHeadings[DPAD_INDEX(PerWork.data)];
    if (angle == DPAD_ANGLE_NONE)
        return FALSE;
    leader->x.fixed = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    leader->z.fixed = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    position[0]     = leader->x.fixed;
    position[1]     = leader->y.fixed;
    position[2]     = leader->z.fixed;
    syVectorAddPolarOffset(toFIXED(16), angle, position);
    leader->groundHeight = leader->y.fixed - toFIXED(8);
    if (fdMapGridGetTerrainKindAtStub(leader->layer, position[0], position[2])
        || fdMapGridCheckCollisionStub(leader, position))
        return FALSE;
    leader->facing         = angle;
    leader->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    leader->motionFlags    = leader->timer;
    sprite->flags          = leader->userValue;
    sprite->rotation       = 0;
    sySoundPlayStub(SE_JUMP);
    leader->acceleration = toFIXED(1);
    leader->speedLimit   = toFIXED(2);
    leader->velocityY    = toFIXED(4);
    fdActorSelectAnimationStub(leader, ANIM_WALK);
    fdActorSetMoveTargetStub(leader, position[0], position[1], position[2]);
    fdActorWaitForMoveStub(leader);
    sprite->image.priority            = leader->savedSpritePriority;
    MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
    return TRUE;
}
