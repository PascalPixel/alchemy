/* Sets stone head `id` out of the wall at the hold under (x, y, z), in 16.16
   pixels, and makes that hold solid rock. */
void scStoneHeadPlace(Sint32 id, Sint32 x, Sint32 y, Sint32 z)
{
	ACTOR		 *actor = fdActorGet(id);
	MAP_METATILE *metatile = ((MAP_STATE *)Ram_HeapSlots->mapGrid)
								 ->layers[MAPGRID_LAYER_OVERHEAD]
								 .metatiles;
	Sint32 metatileX = x / toFIXED(16);
	Sint32 metatileZ = (z - y + toFIXED(1) / 2 - 1) / toFIXED(16);

	metatile += metatileX + metatileZ * MAPGRID_SIZE;
	metatile->collisionCode = METATILE_BEHAVIOR_WALL;
	actor->x.fixed = (x & FIELD_METATILE_MASK) + toFIXED(8);
	actor->z.fixed = (z & FIELD_METATILE_MASK) + toFIXED(8);
	actor->y.fixed = actor->z.fixed - FIELD_METATILE_ORIGIN(metatileZ);
	actor->motionFlg = 0;
	actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetSprFlags(actor, 0);
}
