/// Lowers the cell under the pillar ahead of the leader, or the one Move holds,
/// when it is one of the listed pillars: Pillar_Raise undone, the cell 32
/// pixels down and neither it nor the cell south of it climbable; the
/// field work keeps the pillar as its moved block.
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
      u8 *cell = (u8 *)((struct MapWork *)Ram_HeapSlots->mapGrid)
                     ->layers[held->layer]
                     .cells;
      struct ShapeCell *slot =
          grid + ((struct MapCell *)cell - (struct MapCell *)Ram_MapCellBuffer);
      s32 kind =
          MapGrid_GetTerrainKind(held->layer, held->x.fixed, held->z.fixed);
      s32 height =
          Map_GetTerrainHeight(held->layer, held->x.fixed, held->z.fixed);
      s32 index = Actor_GetCellIndex(held);
      s32 result = MapCollision_FindOrAddTag(
          held->layer, kind,
          CELL_HEIGHT_STEPS(height) - CELL_HEIGHT_STEPS(toFIXED(32)));

      slot += index;
      // The index goes on as the cell offset, four bytes a cell.
      index <<= 2;
      slot->shape = result;
      cell += index;
      ((struct MapCell *)cell)->attributeB &=
          ~(MAP_CELL_JUMP | MAP_CELL_CLIMBABLE);
      cell += MAPGRID_SIZE * sizeof(struct MapCell);
      ((struct MapCell *)cell)->attributeB &= ~MAP_CELL_CLIMBABLE;
      work->movedBlock = held;
    }
    list++;
  }
}
