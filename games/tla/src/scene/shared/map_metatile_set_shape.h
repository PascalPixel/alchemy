// Sets the shape of a metatile to the height given.
Sint32 scMagmaRockInsideSetMetatileShape(Sint32 layer, Sint32 x, Sint32 z, Sint32 height)
{
    MAP_STATE *map   = RamHeapSlots->mapGrid;
    Sint32     shape = fdMapGridGetTerrainKind(layer, x, z);
    Uint8     *out =
        RamMapShapeGrid + (map->layers[layer].metatiles - (MAP_METATILE *)RamMapMetatileBuffer);

    out  += x + z * MAPGRID_SIZE;
    *out  = fdMapShapeFindOrAdd(layer, shape, height);
    return 0;
}
