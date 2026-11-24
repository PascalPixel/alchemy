/// Raises the cell under each listed pillar 32 pixels, to its top, and
/// marks it MAP_CELL_JUMP and MAP_CELL_CLIMBABLE, and the cell south of it, its
/// face, climbable; then puts the leader back on the ground if it rose over
/// him.
void Pillar_Raise(u16 *list) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct Actor *leader;
  s32 height;

  for (; *list != ACTOR_LIST_END; list++) {
    struct Actor *actor = Actor_GetById(*list);
    u8 *layer = &actor->layer;
    s32 shape = MapGrid_GetTerrainKind(*layer, actor->x.fixed, actor->z.fixed);
    s32 level = Map_GetTerrainHeight(*layer, actor->x.fixed, actor->z.fixed);
    s32 index;
    u8 *cells;
    u8 *shapes;
    u8 newShape;
    u8 *grid;

    level = CELL_HEIGHT_STEPS(level);
    index = Actor_GetCellIndex(actor);
    cells = (u8 *)map->layers[*layer].cells;
    grid = Ram_MapShapeGrid;
    shapes = grid +
             (((struct MapCell *)cells) - (struct MapCell *)Ram_MapCellBuffer);
    actor->motionFlags = 0;
    actor->collisionFlags = 0;
    Actor_SetSpriteFlags(actor, 0);
    level += CELL_HEIGHT_STEPS(toFIXED(32));
    newShape = MapCollision_FindOrAddTag(*layer, shape, level);
    shapes += index;
    index <<= 2;
    ((struct ShapeCell *)shapes)->shape = newShape;
    cells += index;
    ((struct MapCell *)cells)->attributeB |= MAP_CELL_JUMP | MAP_CELL_CLIMBABLE;
    cells += MAPGRID_SIZE * sizeof(struct MapCell);
    ((struct MapCell *)cells)->attributeB |= MAP_CELL_CLIMBABLE;
  }
  leader = Actor_GetById(gMainRoutineState.leaderActorId);
  height =
      Map_GetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
  if (leader->y.fixed < height) {
    leader->groundHeight = height;
    leader->y.fixed = height;
  }
}
