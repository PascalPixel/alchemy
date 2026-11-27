// For each actor of a list ended by ACTOR_LIST_END: clears its mark and, if it stands on a
// climbable metatile, walls off the overhead layer's metatiles a row north and south of where it is
// drawn and marks it.
void scBlocksMarkMetatiles(Uint16 *ids)
{
	ACTOR         *actor;
	Uint8         *marked;
	MAP_STATE     *map;
	MAP_METATILE **metatiles;
	MAP_METATILE  *metatile;
	Sint32         x, z;

	for (; *ids != ACTOR_LIST_END; ids++)
	{
		actor   = fdActorGet(*ids);
		marked  = &actor->sceneByte;
		*marked = 0;
		if (fdMapMetatileGetAttributes(MAPGRID_LAYER_GROUND,
		                               FIELD_METATILE(actor->x.fixed),
		                               FIELD_METATILE(actor->z.fixed))
		    & MAP_METATILE_CLIMBABLE)
		{
			map = RamHeapSlots->mapGrid;
			if (map == NULL)
				return;
			metatiles                = &map->layers[MAPGRID_LAYER_OVERHEAD].metatiles;
			z                        = FIELD_METATILE(actor->z.fixed - actor->y.fixed);
			x                        = FIELD_METATILE(actor->x.fixed);
			metatile                 = *metatiles;
			metatile                += x + ((z + 1) << 7);
			metatile->collisionCode  = METATILE_BEHAVIOR_WALL;
			metatile                 = *metatiles;
			metatile                += x + ((z - 1) << 7);
			metatile->collisionCode  = METATILE_BEHAVIOR_WALL;
			*marked                  = 1;
		}
	}
}
