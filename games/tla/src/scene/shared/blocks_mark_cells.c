/// For each id of a list ended by 0xffff: clears the actor's mark and, on a
/// cell with attribute bit 6, blocks the cells ahead of and behind it and sets
/// the mark.
void Blocks_MarkCells(u16 *ids) {
  struct Actor *object;
  u8 *blocked;
  struct MapWork *map;
  struct MapCell **cells;
  struct MapCell *cell;
  s32 x, z;

  while (*ids != 0xffff) {
    object = Actor_Get(*ids);
    blocked = &object->riseEnabled;
    *blocked = 0;
    if (MapCell_GetAttributes(0, FIELD_CELL(object->x.fixed),
                              FIELD_CELL(object->z.fixed)) &
        0x40) {
      map = Ram_HeapSlots->mapGrid;
      if (map == NULL)
        return;
      cells = &map->layers[2].cells;
      z = FIELD_CELL(object->z.fixed - object->y.fixed);
      x = FIELD_CELL(object->x.fixed);
      cell = *cells;
      cell += x + ((z + 1) << 7);
      cell->collisionCode = CELL_BEHAVIOR_WALL;
      cell = *cells;
      cell += x + ((z - 1) << 7);
      cell->collisionCode = CELL_BEHAVIOR_WALL;
      *blocked = 1;
    }
    ids++;
  }
}
