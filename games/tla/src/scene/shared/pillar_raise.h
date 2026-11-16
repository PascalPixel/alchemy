/// Raises the pillars of a list out of the ground, making their cells solid.
void Pillar_Raise(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *leader;
  s32 height;

  /* FAKEMATCH(signature): the form kept; as *list != 0xffff and *shapes =
   * newShape, 6 of 123 instructions differ in tla-en. */
  while (((struct ListedActor *)list)->id != 0xffff) {
    struct Actor *actor = Actor_GetById(*list);
    u8 *layer = &actor->layer;
    s32 shape = MapGrid_GetTerrainKind(*layer, actor->x.fixed, actor->z.fixed);
    s32 level = Map_GetTerrainHeight(*layer, actor->x.fixed, actor->z.fixed);
    s32 index;
    u8 *cells;
    u8 *shapes;
    u8 newShape;
    u8 *grid;

    level >>= 19;
    index = Actor_GetCellIndex(actor);
    cells = (u8 *)map->layers[*layer].cells;
    grid = Ram_MapShapeGrid;
    shapes = grid +
             (((struct MapCell *)cells) - (struct MapCell *)Ram_MapCellBuffer);
    actor->motionFlags = 0;
    actor->collisionFlags = 0;
    Actor_SetSpriteFlags(actor, 0);
    level += 4;
    newShape = MapCollision_FindOrAddTag(*layer, shape, level);
    shapes += index;
    index <<= 2;
    ((struct ShapeCell *)shapes)->shape = newShape;
    cells += index;
    ((struct MapCell *)cells)->attributeB |= 192;
    cells += 0x200;
    ((struct MapCell *)cells)->attributeB |= 64;
    list++;
  }
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  height =
      Map_GetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
  if (leader->y.fixed < height) {
    leader->groundHeight = height;
    leader->y.fixed = height;
  }
}
