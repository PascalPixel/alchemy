/// A stake's look in stake.png, the kind its (actor, kind, flag) entry gives.
enum StakeKind { STAKE_WOOD, STAKE_STONE, STAKE_MOSSY_STONE };

/// A stake as its flag leaves it: driven down by Pound, its cell is open ground
/// and it is drawn underfoot, sunk to its top; still standing, its cell is a
/// wall. `kind` is its look (enum StakeKind).
void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer) {
  struct Actor *actor = Actor_GetById(id);
  struct MapCell *cell =
      ((struct MapWork *)Ram_HeapSlots->mapGrid)->layers[layer].cells;

  cell += Actor_GetCellIndex(actor);
  if (GameFlag_Get(flag)) {
    cell->collisionCode = CELL_BEHAVIOR_NORMAL;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    switch (kind) {
    case STAKE_WOOD:
      Actor_SelectAnimation(actor, 2);
      break;
    case STAKE_STONE:
      Actor_SelectAnimation(actor, 4);
      break;
    case STAKE_MOSSY_STONE:
      Actor_SelectAnimation(actor, 6);
      break;
    }
  } else {
    cell->collisionCode = CELL_BEHAVIOR_WALL;
    switch (kind) {
    case STAKE_WOOD:
      Actor_SelectAnimation(actor, 1);
      break;
    case STAKE_STONE:
      Actor_SelectAnimation(actor, 3);
      break;
    case STAKE_MOSSY_STONE:
      Actor_SelectAnimation(actor, 5);
      break;
    }
  }
}
