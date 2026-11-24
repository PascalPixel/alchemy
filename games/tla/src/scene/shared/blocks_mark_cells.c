/// For each actor of a list ended by ACTOR_LIST_END: clears its mark and, if it
/// stands on a climbable cell, walls off the overhead layer's cells a row north
/// and south of where it is drawn and marks it.
void Blocks_MarkCells(u16 *ids) {
  struct Actor *actor;
  u8 *marked;
  struct MapState *map;
  struct MapCell **cells;
  struct MapCell *cell;
  s32 x, z;

  for (; *ids != ACTOR_LIST_END; ids++) {
    actor = Actor_Get(*ids);
    marked = &actor->riseEnabled;
    *marked = 0;
    if (MapCell_GetAttributes(MAPGRID_LAYER_GROUND, FIELD_CELL(actor->x.fixed),
                              FIELD_CELL(actor->z.fixed)) &
        MAP_CELL_CLIMBABLE) {
      map = Ram_HeapSlots->mapGrid;
      if (map == NULL)
        return;
      cells = &map->layers[MAPGRID_LAYER_OVERHEAD].cells;
      z = FIELD_CELL(actor->z.fixed - actor->y.fixed);
      x = FIELD_CELL(actor->x.fixed);
      cell = *cells;
      cell += x + ((z + 1) << 7);
      cell->collisionCode = CELL_BEHAVIOR_WALL;
      cell = *cells;
      cell += x + ((z - 1) << 7);
      cell->collisionCode = CELL_BEHAVIOR_WALL;
      *marked = 1;
    }
  }
}
