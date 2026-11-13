/// Raises the pillars of a list out of the ground, making their cells solid.
void Pillars_Raise(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *leader;
  s32 height;

  while (((struct ListedActor *)list)->id != 0xffff) {
    struct Actor *object = Actor_GetById(*list);
    u8 *layer = &object->terrainId;
    s32 shape = MapGrid_GetTerrainKind(*layer, object->x, object->z);
    s32 level = Map_GetTerrainHeight(*layer, object->x, object->z);
    s32 index;
    u8 *cells;
    u8 *shapes;
    u8 newShape;
    u8 *grid;

    level >>= 19;
    index = Actor_GetCellIndex(object);
    cells = (u8 *)map->layers[*layer].cells;
    grid = Ram_MapShapeGrid;
    shapes = grid +
             (((struct MapCell *)cells) - (struct MapCell *)Ram_MapCellBuffer);
    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSpriteFlags(object, 0);
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
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < height) {
    leader->terrainHeight = height;
    leader->y = height;
  }
}
