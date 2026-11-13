/// Sets the shape of a cell to the height given.
s32 MagmaRockInside_SetCellShape(s32 layer, s32 x, s32 z, s32 height) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 shape = MapGrid_GetTerrainKind(layer, x, z);
  u8 *out = Ram_MapShapeGrid +
            (map->layers[layer].cells - (struct MapCell *)Ram_MapCellBuffer);

  out += x + (z << 7);
  *out = MapCollision_FindOrAddTag(layer, shape, height);
  return 0;
}
