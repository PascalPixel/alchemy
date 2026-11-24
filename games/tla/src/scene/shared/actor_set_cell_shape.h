/// Gives the cell of `layer` under an actor the shape of the ground there,
/// `level` steps of 8 pixels (CELL_HEIGHT_STEPS) higher, or lower when
/// negative, as a crumbling pillar sinks 4.
void Actor_SetCellShape(s32 level, s32 layer, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  u8 *shape =
      Ram_MapShapeGrid +
      (((struct MapState *)Ram_HeapSlots->mapGrid)->layers[layer].cells -
       (struct MapCell *)Ram_MapCellBuffer);
  s32 kind =
      MapGrid_GetTerrainKind(actor->layer, actor->x.fixed, actor->z.fixed);
  s32 height = CELL_HEIGHT_STEPS(
      Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed));

  height += level;

  shape += actor->x.fixed / toFIXED(MAPGRID_CELL_PIXELS) +
           actor->z.fixed / toFIXED(MAPGRID_CELL_PIXELS) * MAPGRID_SIZE;
  *shape = MapShape_FindOrAdd(actor->layer, kind, height);
}
