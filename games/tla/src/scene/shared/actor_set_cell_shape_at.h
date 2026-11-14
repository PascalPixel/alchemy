/// Actor_SetCellShape as Izumo's ruins and Magma Rock compile it.
void MagmaRock_SetActorCellShape(s32 heightOffset, s32 layer, s32 id) {
  struct Actor *actor = Actor_Get(id);
  s32 cell = Ram_HeapSlots->mapGrid->layers[layer].cells -
             (struct MapCell *)Ram_MapCellBuffer;
  s32 z = actor->z.fixed;
  u8 *shapes = gMapShapeGrid + cell;
  s32 value;
  s32 height;

  value = MapGrid_GetTerrainKind(actor->layer, actor->x.fixed, z);
  height =
      Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed) >> 19;
  height += heightOffset;
  shapes += actor->x.fixed / 0x100000 + (actor->z.fixed / 0x100000 << 7);
  *shapes = MapCollision_FindOrAddTag(actor->layer, value, height);
}
