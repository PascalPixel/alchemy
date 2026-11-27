// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3): the rocks, weights
// and pillars on a stage that Isaac pushes, and finding what stands ahead of him.

// The leader pushes the actor standing in the next metatile ahead one metatile on (a rock, a weight
// or a stone pillar on the stages), when nothing solid stands beyond it or on top of it and the
// ground lets it move, with the pushing pose and SE_PUSH_STEP.
static void scColossoPushActorAhead(void)
{
    ACTOR    *leader;
    ACTOR    *pushed;
    ACTOR    *blocker;
    Uint32    step;
    Uint32    direction;
    FIELD_POS pos;
    Sint32    leaderId;

    leaderId  = MainRoutineStateWork.leaderActorId;
    leader    = fdActorGet(leaderId);

    direction = FACING_INDEX(leader->facing);

    step      = PushedActorDirectionSteps[direction];
    pos.x     = leader->x.fixed + DIRECTION_STEP_X_FIXED(step);
    pos.y     = leader->y.fixed;
    step      = DIRECTION_STEP_Z_FIXED(step);
    pos.z     = leader->z.fixed + step;

    pushed    = scActorFindAtPos(&pos, leader);
    if (pushed == NULL)
    {
        return;
    }

    step    = PushedActorDirectionSteps[direction];
    pos.x   = pushed->x.fixed + DIRECTION_STEP_X_FIXED(step);
    pos.y   = pushed->y.fixed;
    step    = DIRECTION_STEP_Z_FIXED(step);
    pos.z   = pushed->z.fixed + step;

    blocker = scActorFindAtPos(&pos, pushed);
    if (blocker != NULL && (blocker->collisionFlags & ACTOR_SOLID) != 0)
    {
        return;
    }

    pos.x   = pushed->x.fixed;
    pos.y   = pushed->y.fixed + toFIXED(16);
    pos.z   = pushed->z.fixed;

    blocker = scActorFindAtPos(&pos, pushed);
    if (blocker != NULL && (blocker->collisionFlags & ACTOR_SOLID) != 0)
    {
        return;
    }

    pushed->layer = 2;

    step          = PushedActorDirectionSteps[direction];
    pos.x         = pushed->x.fixed + DIRECTION_STEP_X_FIXED(step);
    pos.y         = pushed->y.fixed;
    step          = DIRECTION_STEP_Z_FIXED(step);
    pos.z         = pushed->z.fixed + step;

    if (syFieldActorCheckMovementCollision(pushed, &pos) > 0)
    {
        return;
    }

    fdActorSelectAnimation(leader, ANIM_PUSH);
    sySynch(15);

    pushed->speedLimit   = toFIXED(1) / 5;
    pushed->acceleration = toFIXED(1) / 5;
    syFieldActorSetMoveTargetFX(pushed, pos.x, pos.y, pos.z);

    leader->speedLimit   = toFIXED(1) / 5;
    leader->acceleration = toFIXED(1) / 5;
    syFieldActorSetMoveTargetFX(leader, pos.x, pos.y, pos.z);

    sySoundPlay(SE_PUSH_STEP);
    fdActorWaitForMove(pushed);
    sySoundPlay(SE_STOP_ACTION_SE);

    pushed->x.fixed   = pos.x;
    pushed->z.fixed   = pos.z;
    pushed->velocityX = 0;
    pushed->velocityZ = 0;

    fdActorSelectAnimation(leader, ANIM_STAND);
}

// Returns the placed actor standing one metatile, or else two metatiles, ahead of the actor the
// player moves (straight ahead in the nearest of the four directions), or 0 when there is none.
void *scColossoFindActorAhead(void)
{
    ACTOR    *leader;
    Sint32    facing;
    FIELD_POS pos;
    ACTOR    *occupant;

    leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

    facing = FACING_NEAREST_CARDINAL(leader->facing);

    pos.x  = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    pos.y  = leader->y.fixed;
    pos.z  = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
    syVectorAddPolarOffset(toFIXED(16), facing, (FIXED *)&pos);

    occupant = scActorFindAtPos(&pos, leader);
    if (occupant == NULL)
    {
        pos.x = (leader->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
        pos.y = leader->y.fixed;
        pos.z = (leader->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
        syVectorAddPolarOffset(toFIXED(32), facing, (FIXED *)&pos);

        occupant = scActorFindAtPos(&pos, leader);
    }

    return occupant;
}
