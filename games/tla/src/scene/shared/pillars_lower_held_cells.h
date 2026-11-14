/// Lowers the cells under the held actor, or the one the leader carries, for
/// each listed actor it is: the shape grid takes the shape for its new height
/// and the cell flags under it are cleared; the field work keeps the actor as
/// its moved block.
void Pillars_LowerHeldCells(u16 *list) {
  struct Actor *held = Leader_FindActorAhead();
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u8 *grid;

  if (held == NULL) {
    s32 id = Field_FindNearbyPsynergyTarget(gMainRoutineWork.currentOwner, 1);

    if (id != -1)
      held = Actor_GetById(id);
  }
  grid = Ram_MapShapeGrid;
  work->movedBlock = NULL;
  if (*list != 0xffff) {
  loop:
    if (held == Actor_GetById(*list)) {
      u8 *layer = &held->terrainId;
      u8 *cell = (u8 *)Ram_HeapSlots->mapWork->layers[*layer].cells;
      u8 *shape =
          grid + ((struct MapCell *)cell - (struct MapCell *)Ram_MapCellBuffer);
      s32 kind = MapGrid_GetTerrainKind(*layer, held->x, held->z);
      s32 height = Map_GetTerrainHeight(*layer, held->x, held->z);
      s32 index = Actor_GetCellIndex(held);
      s32 result = MapCollision_FindOrAddTag(*layer, kind, (height >> 19) - 4);

      shape += index;
      // The index goes on as the cell offset, four bytes a cell.
      index <<= 2;
      /* FAKEMATCH(signature): the grid byte stored through a one-field struct
       * view. */
      ((struct ShapeCell *)shape)->shape = result;
      cell += index;
      ((struct MapCell *)cell)->attributeB &= 63;
      cell += 0x200;
      ((struct MapCell *)cell)->attributeB &= 191;
      work->movedBlock = held;
    }
    list++;
    if (*list != 0xffff)
      goto loop;
  }
}
