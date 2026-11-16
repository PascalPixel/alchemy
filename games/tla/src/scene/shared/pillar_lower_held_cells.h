/// Lowers the cells under the held actor, or the one the leader carries, for
/// each listed actor it is: the shape grid takes the shape for its new height
/// and the cell flags under it are cleared; the field work keeps the actor as
/// its moved block.
void Pillar_LowerHeldCells(u16 *list) {
  struct Actor *held = Leader_FindActorAhead();
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct ShapeCell *grid;

  if (held == NULL) {
    s32 id = Field_FindNearbyPsynergyTarget(gMainRoutineWork.leaderActorId, 1);

    if (id != -1)
      held = Actor_GetById(id);
  }
  grid = (struct ShapeCell *)Ram_MapShapeGrid;
  work->movedBlock = NULL;
  while (*list != ACTOR_LIST_END) {
    if (held == Actor_GetById(*list)) {
      u8 *layer = &held->layer;
      u8 *cell = (u8 *)Ram_HeapSlots->mapGrid->layers[*layer].cells;
      struct ShapeCell *slot =
          grid + ((struct MapCell *)cell - (struct MapCell *)Ram_MapCellBuffer);
      s32 kind = MapGrid_GetTerrainKind(*layer, held->x.fixed, held->z.fixed);
      s32 height = Map_GetTerrainHeight(*layer, held->x.fixed, held->z.fixed);
      s32 index = Actor_GetCellIndex(held);
      s32 result = MapCollision_FindOrAddTag(*layer, kind, (height >> 19) - 4);

      slot += index;
      // The index goes on as the cell offset, four bytes a cell.
      index <<= 2;
      slot->shape = result;
      cell += index;
      ((struct MapCell *)cell)->attributeB &= 63;
      cell += MAPGRID_SIZE * sizeof(struct MapCell);
      ((struct MapCell *)cell)->attributeB &= 191;
      work->movedBlock = held;
    }
    list++;
  }
}
