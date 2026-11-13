/// Opens or closes the cell under an actor by flag, setting its pose by `kind`.
void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer) {
  struct Actor *object = Actor_GetById(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Actor_GetCellIndex(object);
  if (GameFlag_Get(flag)) {
    cell->collisionCode = 0;
    object->priorityFlags |= 2;
    switch (kind) {
    case 0:
      Actor_SelectAnimation(object, 2);
      break;
    case 1:
      Actor_SelectAnimation(object, 4);
      break;
    case 2:
      Actor_SelectAnimation(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (kind) {
    case 0:
      Actor_SelectAnimation(object, 1);
      break;
    case 1:
      Actor_SelectAnimation(object, 3);
      break;
    case 2:
      Actor_SelectAnimation(object, 5);
      break;
    }
  }
}
