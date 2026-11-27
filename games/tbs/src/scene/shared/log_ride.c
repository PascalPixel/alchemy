// Fuchin Falls and Vale's cave: the logs that float the leader across a pool.

// Floats the log actor id and the leader standing on it by (dx, dz) pixels, to the centres of their
// metatiles, at a pixel a tick. The leader is drawn in ANIM_LEADER_WALK_BACKWARDS and the log in
// its sheet's animation 4 going north or west, 3 going south or east.
static void scActorFloatLogWithLeader(Sint32 id, Sint32 dx, Sint32 dz)
{
    ACTOR *leader;
    ACTOR *raft;
    FIXED  x;
    FIXED  z;

    leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    raft   = fdActorGetById(id);
    fdScriptStart();
    x                    = ((leader->x.fixed + toFIXED(dx)) & FIELD_METATILE_MASK) + toFIXED(8);
    z                    = ((leader->z.fixed + toFIXED(dz)) & FIELD_METATILE_MASK) + toFIXED(8);

    leader->speedLimit   = toFIXED(1);
    leader->acceleration = toFIXED(1) / 2;
    syFieldActorSetMoveTargetFX(leader, x, leader->y.fixed, z);
    fdActorSelectAnimation(leader, ANIM_LEADER_WALK_BACKWARDS);
    x                  = ((raft->x.fixed + toFIXED(dx)) & FIELD_METATILE_MASK) + toFIXED(8);
    z                  = ((raft->z.fixed + toFIXED(dz)) & FIELD_METATILE_MASK) + toFIXED(8);

    raft->speedLimit   = toFIXED(1);
    raft->acceleration = toFIXED(1) / 2;
    syFieldActorSetMoveTargetFX(raft, x, raft->y.fixed, z);
    if (dx < 0 || dz < 0)
    {
        fdActorSelectAnimation(raft, 4);
    }
    else
    {
        fdActorSelectAnimation(raft, 3);
    }
    fdActorWaitForMove(leader);
    fdScriptEnd();
}
