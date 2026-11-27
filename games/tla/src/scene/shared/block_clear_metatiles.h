// Opens again the overhead metatiles a marked pillar walled off (scBlocksMarkMetatiles) and clears
// its mark.
void scMagmaRockInsideClearBlockMetatiles(ACTOR *actor)
{
    MAP_STATE     *map;
    MAP_METATILE **metatiles;
    MAP_METATILE  *metatile;
    Sint32         x;
    Sint32         z;

    if (actor->sceneByte == 0)
        return;
    map = RamHeapSlots->mapGrid;
    if (map == NULL)
        return;
    metatiles                = &map->layers[MAPGRID_LAYER_OVERHEAD].metatiles;
    metatile                 = *metatiles;
    x                        = FIELD_METATILE(actor->x.fixed);
    z                        = FIELD_METATILE(actor->z.fixed - actor->y.fixed);
    metatile                += x + ((z + 1) << 7);
    metatile->collisionCode  = METATILE_BEHAVIOR_NORMAL;
    metatile                 = *metatiles;
    metatile                += x + ((z - 1) << 7);
    metatile->collisionCode  = METATILE_BEHAVIOR_NORMAL;
    actor->sceneByte         = 0;
}
