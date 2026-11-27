/* Raises the metatile under the pillar a push or Move has just set down
   (FIELD_STATE.movedBlock), as scPillarRaise does. */
void scHeldActorRaiseMetatile()
{
	ACTOR		   *held = FieldStateWork->movedBlock;
	Uint8		   *metatiles;
	SHAPE_METATILE *shapes;
	Sint32			shape;
	Sint32			level;
	Sint32			index;
	Uint8			newShape;

	if(held == NULL)
		return;
	metatiles = (Uint8 *)((MAP_STATE *)Ram_HeapSlots->mapGrid)
					->layers[held->layer]
					.metatiles;
	shapes = (SHAPE_METATILE *)Ram_MapShapeGrid +
		(((MAP_METATILE *)metatiles) - (MAP_METATILE *)Ram_MapMetatileBuffer);
	shape = fdMapGridGetTerrainKind(held->layer, held->x.fixed, held->z.fixed);
	level = fdMapGetTerrainHeight(held->layer, held->x.fixed, held->z.fixed);
	index = fdActorGetMetatileIndex(held);
	level = METATILE_HEIGHT_STEPS(level);
	level += 4;
	newShape = fdMapShapeFindOrAdd(held->layer, shape, level);
	shapes += index;
	index <<= 2;
	shapes->shape = newShape;
	metatiles += index;
	((MAP_METATILE *)metatiles)->attributeB |=
		MAP_METATILE_JUMP | MAP_METATILE_CLIMBABLE;
	metatiles += MAPGRID_SIZE * sizeof(MAP_METATILE);
	((MAP_METATILE *)metatiles)->attributeB |= MAP_METATILE_CLIMBABLE;
}
