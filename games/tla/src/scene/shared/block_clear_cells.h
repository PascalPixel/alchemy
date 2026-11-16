/// Clears the cells a block marked.
void MagmaRockInside_ClearBlockCells(struct Actor *actor) {
  struct MapWork *map;
  struct MapCell **cells;
  struct MapCell *cell;
  s32 x;
  s32 z;

  if (actor->riseEnabled == 0)
    return;
  map = Ram_HeapSlots->mapGrid;
  if (map == NULL)
    return;
  cells = &map->layers[2].cells;
  cell = *cells;
  x = FIELD_CELL(actor->x.fixed);
  z = (actor->z.fixed - actor->y.fixed) >> 20;
  cell += x + ((z + 1) << 7);
  cell->collisionCode = 0;
  cell = *cells;
  cell += x + ((z - 1) << 7);
  cell->collisionCode = 0;
  actor->riseEnabled = 0;
}
