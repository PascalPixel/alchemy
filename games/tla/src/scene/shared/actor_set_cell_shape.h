/// Sets the shape of the cell under an actor `level` steps above its ground.
void Actor_SetCellShape(s32 level, s32 layer, s32 owner) {
  struct Actor *object = Actor_GetById(owner);
  u8 *shape = Ram_MapShapeGrid + (Ram_HeapSlots->mapWork->layers[layer].cells -
                                  (struct MapCell *)Ram_MapCellBuffer);
  s32 value = MapGrid_GetTerrainKind(object->terrainId, object->x, object->z);
  s32 height =
      Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;

  height += level;

  shape += object->x / 0x100000 + object->z / 0x100000 * 128;
  *shape = MapCollision_FindOrAddTag(object->terrainId, value, height);
}
