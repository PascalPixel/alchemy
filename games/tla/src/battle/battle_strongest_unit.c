// Picking a unit on one side: a random living target, or the living unit with the highest Attack.
// Part of battle.c.

// Picks a random living target on one side: the party (PLACE_PARTY | slot) when party is set,
// otherwise the enemies (PLACE_ENEMY | slot); 0 when none lives.
Sint32 btAIChooseRandomTarget(Sint32 party)
{
    BATTLE_STATE *battle;
    Uint16        targets[6];
    Sint32        count;
    Sint32        i;

    count  = 0;
    battle = RamHeapSlots->battleState;

    if (party != 0)
    {
        for (i = 0; battle->partyUnits[i] != BATTLE_UNIT_LIST_END; i++)
        {
            if (battle->partyUnits[i] != BATTLE_UNIT_REMOVED
                && syUnitGet(battle->partyUnits[i])->hp != 0)
                targets[count++] = i | PLACE_PARTY;
        }
    }
    else
    {
        for (i = 0; battle->enemyIds[i] != BATTLE_UNIT_LIST_END; i++)
        {
            if (battle->enemyIds[i] != BATTLE_UNIT_REMOVED
                && syUnitGet(battle->enemyIds[i])->hp != 0)
                targets[count++] = i | PLACE_ENEMY;
        }
    }
    if (count == 0)
        return 0;
    return targets[(Uint32)(count * gmBattleRandomStub()) >> 16];
}

// Finds the living unit with the highest Attack on unitId's side (the party for ids up to ALLY_MAX,
// else the enemies), other than unitId itself; unitId when no other lives.
Sint32 btUnitFindStrongestAlly(Uint32 unitId)
{
    BATTLE_STATE *battle     = RamHeapSlots->battleState;
    Uint32        best       = unitId;
    Sint32        bestAttack = syUnitGet(unitId)->attack;
    Sint32        i;
    Sint32        id;

    if (unitId <= ALLY_MAX)
    {
        for (i = 0; (id = battle->partyUnits[i]) != BATTLE_UNIT_LIST_END; i++)
        {
            if (id != BATTLE_UNIT_REMOVED && syUnitGet(id)->hp != 0)
            {
                if (best == unitId || (id != unitId && syUnitGet(id)->attack > bestAttack))
                {
                    best       = id;
                    bestAttack = syUnitGet(best)->attack;
                }
            }
        }
    }
    else
    {
        for (i = 0; (id = battle->enemyIds[i]) != BATTLE_UNIT_LIST_END; i++)
        {
            if (id != BATTLE_UNIT_REMOVED && syUnitGet(id)->hp != 0)
            {
                if (best == unitId || (id != unitId && syUnitGet(id)->attack > bestAttack))
                {
                    best       = id;
                    bestAttack = syUnitGet(best)->attack;
                }
            }
        }
    }
    return best;
}
