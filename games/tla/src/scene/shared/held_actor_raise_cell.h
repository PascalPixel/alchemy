/// Raises the cell under the held actor.
void HeldActor_RaiseCell() {
  struct Actor *held = gFieldWork->movedBlock;
  u8 *cells;
  struct ShapeCell *shapes;
  s32 shape;
  s32 level;
  s32 index;
  u8 newShape;

  if (held == NULL)
    return;
  cells = (u8 *)Ram_HeapSlots->mapGrid->layers[held->layer].cells;
  shapes = (struct ShapeCell *)Ram_MapShapeGrid +
           (((struct MapCell *)cells) - (struct MapCell *)Ram_MapCellBuffer);
  shape = MapGrid_GetTerrainKind(held->layer, held->x.fixed, held->z.fixed);
  level = Map_GetTerrainHeight(held->layer, held->x.fixed, held->z.fixed);
  index = Actor_GetCellIndex(held);
  level >>= 19;
  level += 4;
  newShape = MapCollision_FindOrAddTag(held->layer, shape, level);
  shapes += index;
  index <<= 2;
  shapes->shape = newShape;
  cells += index;
  ((struct MapCell *)cells)->attributeB |= 192;
  cells += MAPGRID_SIZE * sizeof(struct MapCell);
  ((struct MapCell *)cells)->attributeB |= 64;
}
