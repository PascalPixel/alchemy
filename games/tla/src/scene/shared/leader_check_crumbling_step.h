// Stepping onto a crumbling pillar of an (actor, flag) list, as scCrumblingPillarStep: the first
// time it flashes and cracks, the second time it gives way under the leader with a shake, dust and
// a thump.
void scLeaderCheckCrumblingStep(const Uint16 *table)
{
    ACTOR *leader;
    ACTOR *trap;
    FIXED  pos[3];
    Sint32 id;
    Sint32 flag;
    Sint32 second;

    if (fdTryJumpForward() != 0)
        return;
    leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    pos[0] = leader->x.fixed;
    pos[1] = leader->y.fixed;
    pos[2] = leader->z.fixed;
    trap   = fdActorFindAtMetatile(pos, 0);
    if (trap == 0)
        return;
    while (*table != ACTOR_LIST_END)
    {
        id = table[0];
        if (fdActorGetById(id) != trap)
        {
            table += 2;
            continue;
        }
        flag = table[1];
        fdScriptStart();
        if (!syGameFlagGet(flag))
        {
            sySoundPlay(SE_CHEST_CLOSE);
            fdActorSetSpriteColor(fdActorGetById(id), SPRITE_COLOR_WHITE);
            sySynch(2);
            fdActorSelectAnimation(trap, 0);
            fdActorSetSpriteColor(fdActorGetById(id), SPRITE_COLOR_NORMAL);
            sySynch(2);
            fdActorSetSpriteColor(fdActorGetById(id), SPRITE_COLOR_WHITE);
            sySynch(4);
            fdActorSetSpriteColor(fdActorGetById(id), SPRITE_COLOR_NORMAL);
            scDustScatter(0);
            syGameFlagSet(flag);
        }
        else
        {
            second = flag + 1;
            if (!syGameFlagGet(second))
            {
                leader->motionFlags = 0;
                sySoundPlay(SE_PUSH);
                fdMapGridSetShake(1, 4, 1);
                scDustScatter(0);
                sySynch(8);
                fdActorSelectAnimation(trap, 2);
                fdLeaderSetMetatileShapeAt(-2, trap->layer);
                scDustScatter(1);
                sySynch(16);
                fdLeaderSetMetatileShapeAt(-2, trap->layer);
                sySynch(4);
                fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
                sySynch(8);
                leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
                sySynch(5);
                fdActorSetPosAndResetMotion(trap, 0, 0, 0);
                sySynch(2);
                sySoundPlay(SE_THUMP);
                scMagmaRockInsideSpawnDustRing();
                sySynch(20);
                syGameFlagSet(second);
            }
        }
        fdScriptEnd();
        return;
    }
}
