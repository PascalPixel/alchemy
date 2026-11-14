/// Raises the cell under the held actor.
void HeldActor_RaiseCell(void) {
  struct Actor *held = HELD_OBJECT(Ram_HeapSlots->fieldWork);
  u8 *layer;
  u8 *cells;
  u8 *shapes;
  s32 shape;
  s32 level;
  s32 index;
  u8 newShape;

  if (held == NULL)
    return;
  layer = &held->terrainId;
  cells = (u8 *)Ram_HeapSlots->mapWork->layers[*layer].cells;
  shapes = Ram_MapShapeGrid +
           (((struct MapCell *)cells) - (struct MapCell *)Ram_MapCellBuffer);
  shape = MapGrid_GetTerrainKind(*layer, held->x, held->z);
  level = Map_GetTerrainHeight(*layer, held->x, held->z);
  index = Actor_GetCellIndex(held);
  level >>= 19;
  level += 4;
  newShape = MapCollision_FindOrAddTag(*layer, shape, level);
  shapes += index;
  index <<= 2;
  /* FAKEMATCH(signature): the grid byte stored through a one-field struct
   * view. */
  ((struct ShapeCell *)shapes)->shape = newShape;
  cells += index;
  ((struct MapCell *)cells)->attributeB |= 192;
  cells += 0x200;
  ((struct MapCell *)cells)->attributeB |= 64;
}
