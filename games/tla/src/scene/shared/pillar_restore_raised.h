/// Puts the crumbling pillars of a list back where their flags left them: one
/// whose second flag is set has fallen and is put away; the others stand on
/// their cells raised 32 pixels, gaps to jump onto, in animation 0 once their
/// first flag is set, as the first step on one leaves it. Then a leader left
/// below the ground is lifted onto it.
void MagmaRockInside_RestoreCrumblingPillars(const u16 *table) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  struct Actor *leader;
  s32 ground;

  for (; *table != ACTOR_LIST_END; table += 2) {
    id = table[0];
    actor = Actor_Get(id);
    flag = table[1];
    nextFlag = flag + 1;
    cells = map->layers[actor->layer].cells;
    shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
    if (Event_GetFlag(nextFlag)) {
      Actor_SetPosition(id, 0, 0);
    } else {
      s32 shape;
      u8 *slot;
      s32 height;

      index = Actor_GetCellIndex(actor);
      cell = &cells[index];
      slot = shapes + index;
      shape =
          MapGrid_GetTerrainKind(actor->layer, actor->x.fixed, actor->z.fixed);
      height =
          Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
      Actor_DisableById(id);
      cell->attributeB = MAP_CELL_JUMP;
      *slot = MapShape_FindOrAdd(actor->layer, shape,
                                 CELL_HEIGHT_STEPS(height) +
                                     CELL_HEIGHT_STEPS(toFIXED(32)));
      if (Event_GetFlag(flag))
        Actor_SelectAnimation(actor, 0);
    }
  }
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  ground =
      Map_GetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
  if (leader->y.fixed < ground) {
    leader->groundHeight = ground;
    leader->y.fixed = ground;
  }
}
