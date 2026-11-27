/* A stake's look in stake.png, the kind its (actor, kind, flag) entry gives. */
typedef enum STAKE_KIND {
	STAKE_WOOD,
	STAKE_STONE,
	STAKE_MOSSY_STONE
} STAKE_KIND;

/* A stake as its flag leaves it: driven down by Pound, its cell is open ground
 */
/* and it is drawn underfoot, sunk to its top; still standing, its cell is a */
/* wall. `kind` is its look (STAKE_KIND). */
void fdActorSetCellOpen(s32 id, u32 kind, s32 flag, s32 layer) {
	ACTOR *actor = fdActorGetById(id);
	MAP_CELL *cell = ((MAP_STATE *)Ram_HeapSlots->mapGrid)->layers[layer].cells;

	cell += fdActorGetCellIndex(actor);
	if(syGameFlagGet(flag)) {
		cell->collisionCode = CELL_BEHAVIOR_NORMAL;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
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
	}
	else {
		cell->collisionCode = CELL_BEHAVIOR_WALL;
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
