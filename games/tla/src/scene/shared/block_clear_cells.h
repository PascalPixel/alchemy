/// Opens again the overhead cells a marked pillar walled off (Blocks_MarkCells)
/// and clears its mark.
void MagmaRockInside_ClearBlockCells(struct Actor *actor) {
  struct MapState *map;
  struct MapCell **cells;
  struct MapCell *cell;
  s32 x;
  s32 z;

  if (actor->riseEnabled == 0)
    return;
  map = Ram_HeapSlots->mapGrid;
  if (map == NULL)
    return;
  cells = &map->layers[MAPGRID_LAYER_OVERHEAD].cells;
  cell = *cells;
  x = FIELD_CELL(actor->x.fixed);
  z = FIELD_CELL(actor->z.fixed - actor->y.fixed);
  cell += x + ((z + 1) << 7);
  cell->collisionCode = CELL_BEHAVIOR_NORMAL;
  cell = *cells;
  cell += x + ((z - 1) << 7);
  cell->collisionCode = CELL_BEHAVIOR_NORMAL;
  actor->riseEnabled = 0;
}
