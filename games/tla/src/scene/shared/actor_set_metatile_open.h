/* A stake's look in stake.png, the kind its (actor, kind, flag) entry gives. */
typedef enum STAKE_KIND {
	STAKE_WOOD,
	STAKE_STONE,
	STAKE_MOSSY_STONE
} STAKE_KIND;

/* A stake as its flag leaves it: driven down by Pound, its metatile is open
   ground and it is drawn underfoot, sunk to its top; still standing, its
   metatile is a wall. `kind` is its look (STAKE_KIND). */
void fdActorSetMetatileOpen(Sint32 id, Uint32 kind, Sint32 flag, Sint32 layer)
{
	ACTOR		 *actor = fdActorGetById(id);
	MAP_METATILE *metatile =
		((MAP_STATE *)RamHeapSlots->mapGrid)->layers[layer].metatiles;

	metatile += fdActorGetMetatileIndex(actor);
	if(syGameFlagGet(flag)) {
		metatile->collisionCode = METATILE_BEHAVIOR_NORMAL;
		actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
		switch(kind) {
		case STAKE_WOOD:
			fdActorSelectAnimation(actor, 2);
			break;
		case STAKE_STONE:
			fdActorSelectAnimation(actor, 4);
			break;
		case STAKE_MOSSY_STONE:
			fdActorSelectAnimation(actor, 6);
			break;
		}
	} else {
		metatile->collisionCode = METATILE_BEHAVIOR_WALL;
		switch(kind) {
		case STAKE_WOOD:
			fdActorSelectAnimation(actor, 1);
			break;
		case STAKE_STONE:
			fdActorSelectAnimation(actor, 3);
			break;
		case STAKE_MOSSY_STONE:
			fdActorSelectAnimation(actor, 5);
			break;
		}
	}
}
