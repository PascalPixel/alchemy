// Imil's ice, in the town and in Imil Falls Cave: the leader slides across it a metatile at a time
// the way the pad points.
//
// The including scene names its table of slide headings, by the held directions (DPAD_INDEX), as
// ICE_SLIDE_HEADINGS.

// While the leader slides on the ice, turns him a sixteenth of a turn a tick toward the way the pad
// points, at three times his walking pace, and has him look around when it points nowhere.
static void scActorSteerByPad(ACTOR *actor)
{
    Sint32 heading;
    Uint16 facing;
    Sint32 turn;

    if (actor->timer != 0)
    {
        actor->timer--;
        return;
    }
    actor->steeringFlags = 0;
    heading              = IceKeyHeadings[DPAD_INDEX(PerData)];
    if (heading == KEY_HEADING_NONE)
    {
        fdActorSelectAnimation(actor, ANIM_LEADER_LOOK_AROUND);
        return;
    }
    facing = actor->facing;
    turn   = (Sint16)(heading - facing);
    if (turn > FACING_STEP)
        turn = FACING_STEP;
    if (turn < -FACING_STEP)
        turn = -FACING_STEP;
    actor->facing = facing + turn;
    fdActorSelectAnimation(actor, ANIM_WALK);
    fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
}

// Slides the leader along the ice, a metatile at a time the way the pad points, until a wall or
// higher ground stops him or he slides off the ice.
static void scLeaderSlideOnIce(void)
{
    ACTOR    *actor;
    FIELD_POS probe;
    Sint32    heading;
    Sint32    startBehavior;
    Sint32    nextBehavior;
    FIXED     z;
    FIXED     x;

    actor = fdActorLookup(MainRoutineStateWork.leaderActorId);

    for (;;)
    {
        heading = ICE_SLIDE_HEADINGS[DPAD_INDEX(PerData)];
        if ((Uint16)heading == (Uint16)KEY_HEADING_NONE)
        {
            return;
        }

        fdScriptStart();

        probe.x       = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
        probe.y       = actor->y.fixed;
        probe.z       = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
        z             = probe.z;
        x             = probe.x;
        startBehavior = fdMapGridGetMetatileBehaviorAt(actor->layer, x, z);
        syFieldVectorAddPolarOffset(16, heading, &probe);

        nextBehavior = fdMapGridGetMetatileBehaviorAt(actor->layer, probe.x, probe.z);
        if (nextBehavior == METATILE_BEHAVIOR_WALL
            || fdMapGridGetTerrainHeight(actor->layer, probe.x, probe.z) - actor->y.fixed
                   > toFIXED(8))
        {
            actor->facing = heading;
            goto tail;
        }

        probe.x             = x;
        probe.z             = z;
        actor->speedLimit   = toFIXED(2);
        actor->acceleration = toFIXED(1) / 10;
        actor->timer        = 0;
        syFieldActorSetMoveTargetFX(actor, x, actor->y.fixed, z);

        fdActorSelectAnimation(actor, ANIM_WALK);
        fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
        fdActorWaitForMove(actor);
        actor->update = scActorSteerByPad;

        for (;;)
        {
            syFieldVectorAddPolarOffset(16, heading, &probe);
            nextBehavior = fdMapGridGetMetatileBehaviorAt(actor->layer, probe.x, probe.z);
            if (nextBehavior == METATILE_BEHAVIOR_WALL)
            {
                break;
            }
            if (fdMapGridGetTerrainHeight(actor->layer, probe.x, probe.z) - actor->y.fixed
                > toFIXED(8))
            {
                break;
            }
            x                   = probe.x;
            z                   = probe.z;
            actor->speedLimit   = toFIXED(2);
            actor->acceleration = toFIXED(1) / 10;
            syFieldActorSetMoveTargetFX(actor, probe.x, probe.y, probe.z);
            fdActorWaitForMove(actor);
            if (nextBehavior != startBehavior)
            {
                goto blocked;
            }
        }

        actor->speedLimit   = toFIXED(2);
        actor->acceleration = toFIXED(1);
        syFieldActorSetMoveTargetFX(actor, x, actor->y.fixed, z);
        fdActorWaitForMove(actor);
        sySynch(2);
    }

blocked:
    // Sliding off the ice ends the slide and gives back normal steering.
    actor->update         = NULL;
    actor->steeringFlags |= ACTOR_FACES_MOTION;

    actor->acceleration   = toFIXED(1) / 4;

tail:
    sySynch(10);
    fdScriptEnd();
}
