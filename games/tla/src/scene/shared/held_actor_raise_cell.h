/// Raises the cell under the pillar a push or Move has just set down
/// (FieldState.movedBlock), as Pillar_Raise does.
void HeldActor_RaiseCell() {
  struct Actor *held = gFieldState->movedBlock;
  u8 *cells;
  struct ShapeCell *shapes;
  s32 shape;
  s32 level;
  s32 index;
  u8 newShape;

  if (held == NULL)
    return;
  cells = (u8 *)((struct MapState *)Ram_HeapSlots->mapGrid)
              ->layers[held->layer]
              .cells;
  shapes = (struct ShapeCell *)Ram_MapShapeGrid +
           (((struct MapCell *)cells) - (struct MapCell *)Ram_MapCellBuffer);
  shape = MapGrid_GetTerrainKind(held->layer, held->x.fixed, held->z.fixed);
  level = Map_GetTerrainHeight(held->layer, held->x.fixed, held->z.fixed);
  index = Actor_GetCellIndex(held);
  level = CELL_HEIGHT_STEPS(level);
  level += 4;
  newShape = MapShape_FindOrAdd(held->layer, shape, level);
  shapes += index;
  index <<= 2;
  shapes->shape = newShape;
  cells += index;
  ((struct MapCell *)cells)->attributeB |= MAP_CELL_JUMP | MAP_CELL_CLIMBABLE;
  cells += MAPGRID_SIZE * sizeof(struct MapCell);
  ((struct MapCell *)cells)->attributeB |= MAP_CELL_CLIMBABLE;
}
