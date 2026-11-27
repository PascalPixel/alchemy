// Lowers the metatile under the pillar ahead of the leader, or the one Move holds, when it is one
// of the listed pillars: scPillarRaise undone, the metatile 32 pixels down and neither it nor the
// metatile south of it climbable; the field state keeps the pillar as its moved block.
void scPillarLowerHeldMetatiles(Uint16 *list)
{
    ACTOR          *held  = fdLeaderFindActorAhead();
    FIELD_STATE    *state = RamHeapSlots->fieldState;
    SHAPE_METATILE *grid;

    if (held == NULL)
    {
        Sint32 id = fdFindNearbyPsynergyTarget(MainRoutineStateWork.leaderActorId, 1);

        if (id != -1)
            held = fdActorGetById(id);
    }
    grid              = (SHAPE_METATILE *)RamMapShapeGrid;
    state->movedBlock = NULL;
    for (; *list != ACTOR_LIST_END; list++)
    {
        if (held == fdActorGetById(*list))
        {
            Uint8 *metatile =
                (Uint8 *)((MAP_STATE *)RamHeapSlots->mapGrid)->layers[held->layer].metatiles;
            SHAPE_METATILE *slot =
                grid + ((MAP_METATILE *)metatile - (MAP_METATILE *)RamMapMetatileBuffer);
            Sint32 kind     = fdMapGridGetTerrainKind(held->layer, held->x.fixed, held->z.fixed);
            Sint32 height   = fdMapGetTerrainHeight(held->layer, held->x.fixed, held->z.fixed);
            Sint32 index    = fdActorGetMetatileIndex(held);
            Sint32 result   = fdMapShapeFindOrAdd(held->layer,
                                                kind,
                                                METATILE_HEIGHT_STEPS(height)
                                                    - METATILE_HEIGHT_STEPS(toFIXED(32)));

            slot           += index;
            // The index goes on as the metatile offset, four bytes a metatile.
            index         <<= 2;
            slot->shape     = result;
            metatile       += index;
            ((MAP_METATILE *)metatile)->attributeB &= ~(MAP_METATILE_JUMP | MAP_METATILE_CLIMBABLE);
            metatile                               += MAPGRID_SIZE * sizeof(MAP_METATILE);
            ((MAP_METATILE *)metatile)->attributeB &= ~MAP_METATILE_CLIMBABLE;
            state->movedBlock                       = held;
        }
    }
}
