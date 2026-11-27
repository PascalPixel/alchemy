// Puts the crumbling pillars of a list back where their flags left them: one whose second flag is
// set has fallen and is put away; the others stand on their metatiles raised 32 pixels, gaps to
// jump onto, in animation 0 once their first flag is set, as the first step on one leaves it. Then
// a leader left below the ground is lifted onto it.
void scMagmaRockInsideRestoreCrumblingPillars(const Uint16 *table)
{
    MAP_STATE    *map = RamHeapSlots->mapGrid;
    ACTOR        *actor;
    Sint32        id;
    Sint32        flag;
    Sint32        nextFlag;
    MAP_METATILE *metatiles;
    Uint8        *shapes;
    MAP_METATILE *metatile;
    Sint32        index;
    ACTOR        *leader;
    Sint32        ground;

    for (; *table != ACTOR_LIST_END; table += 2)
    {
        id        = table[0];
        actor     = fdActorGet(id);
        flag      = table[1];
        nextFlag  = flag + 1;
        metatiles = map->layers[actor->layer].metatiles;
        shapes    = MapShapeGrid + (metatiles - (MAP_METATILE *)RamMapMetatileBuffer);
        if (fdScriptGetFlag(nextFlag))
        {
            fdActorSetPos(id, 0, 0);
        }
        else
        {
            Sint32 shape;
            Uint8 *slot;
            Sint32 height;

            index    = fdActorGetMetatileIndex(actor);
            metatile = &metatiles[index];
            slot     = shapes + index;
            shape    = fdMapGridGetTerrainKind(actor->layer, actor->x.fixed, actor->z.fixed);
            height   = fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
            fdActorDisableById(id);
            metatile->attributeB = MAP_METATILE_JUMP;
            *slot                = fdMapShapeFindOrAdd(actor->layer,
                                        shape,
                                        METATILE_HEIGHT_STEPS(height)
                                            + METATILE_HEIGHT_STEPS(toFIXED(32)));
            if (fdScriptGetFlag(flag))
                fdActorSelectAnimation(actor, 0);
        }
    }
    leader = fdActorGet(MainRoutineStateWork.leaderActorId);
    ground = fdMapGetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
    if (leader->y.fixed < ground)
    {
        leader->groundHeight = ground;
        leader->y.fixed      = ground;
    }
}
