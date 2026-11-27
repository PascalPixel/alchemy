// Gives the metatile of `layer` under an actor the shape of the ground there, `level` steps of 8
// pixels (METATILE_HEIGHT_STEPS) higher, or lower when negative, as a crumbling pillar sinks 4.
void fdActorSetMetatileShape(Sint32 level, Sint32 layer, Sint32 id)
{
	ACTOR *actor = fdActorGetById(id);
	Uint8 *shape = RamMapShapeGrid
	             + (((MAP_STATE *)RamHeapSlots->mapGrid)->layers[layer].metatiles
	                - (MAP_METATILE *)RamMapMetatileBuffer);
	Sint32 kind = fdMapGridGetTerrainKind(actor->layer, actor->x.fixed, actor->z.fixed);
	Sint32 height =
		METATILE_HEIGHT_STEPS(fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed));

	height += level;

	shape  += actor->x.fixed / toFIXED(MAPGRID_METATILE_PIXELS)
	       + actor->z.fixed / toFIXED(MAPGRID_METATILE_PIXELS) * MAPGRID_SIZE;
	*shape = fdMapShapeFindOrAdd(actor->layer, kind, height);
}
