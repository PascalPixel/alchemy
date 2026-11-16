/// Opens or closes the cell under an actor by flag, setting its pose by `kind`.
void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer) {
  struct Actor *actor = Actor_GetById(id);
  struct MapCell *cell = Ram_HeapSlots->mapGrid->layers[layer].cells;

  cell += Actor_GetCellIndex(actor);
  if (GameFlag_Get(flag)) {
    cell->collisionCode = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    switch (kind) {
    case 0:
      Actor_SelectAnimation(actor, 2);
      break;
    case 1:
      Actor_SelectAnimation(actor, 4);
      break;
    case 2:
      Actor_SelectAnimation(actor, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (kind) {
    case 0:
      Actor_SelectAnimation(actor, 1);
      break;
    case 1:
      Actor_SelectAnimation(actor, 3);
      break;
    case 2:
      Actor_SelectAnimation(actor, 5);
      break;
    }
  }
}
