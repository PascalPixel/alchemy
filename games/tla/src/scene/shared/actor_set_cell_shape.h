/// Sets the shape of the cell under an actor `level` steps above its ground.
void Actor_SetCellShape(s32 level, s32 layer, s32 owner) {
  struct Actor *actor = Actor_GetById(owner);
  u8 *shape = Ram_MapShapeGrid + (Ram_HeapSlots->mapGrid->layers[layer].cells -
                                  (struct MapCell *)Ram_MapCellBuffer);
  s32 value =
      MapGrid_GetTerrainKind(actor->layer, actor->x.fixed, actor->z.fixed);
  s32 height =
      Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed) >> 19;

  height += level;

  shape += actor->x.fixed / toFIXED(16) + actor->z.fixed / toFIXED(16) * 128;
  *shape = MapCollision_FindOrAddTag(actor->layer, value, height);
}
