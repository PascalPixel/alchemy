/// Clears the cells a block marked.
void MagmaRockInside_ClearBlockCells(struct Actor *object) {
  struct MapWork *map;
  struct MapCell **cells;
  struct MapCell *cell;
  s32 x;
  s32 z;

  if (object->blockMarked == 0)
    return;
  map = Ram_HeapSlots->mapWork;
  if (map == NULL)
    return;
  cells = &map->layers[2].cells;
  cell = *cells;
  x = object->x >> 20;
  z = (object->z - object->y) >> 20;
  cell += x + ((z + 1) << 7);
  cell->collisionCode = 0;
  cell = *cells;
  cell += x + ((z - 1) << 7);
  cell->collisionCode = 0;
  object->blockMarked = 0;
}
