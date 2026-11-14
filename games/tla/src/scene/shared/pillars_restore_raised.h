/// Pillars_Restore without the reset, lifting the leader onto the ground.
void MagmaRockInside_RestoreCrumblingPillars(const u16 *table) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  struct Actor *leader;
  s32 ground;

  if (*table == 0xffff)
    goto done;
next:
  id = table[0];
  object = Actor_Get(id);
  flag = table[1];
  nextFlag = flag + 1;
  cells = map->layers[object->terrainId].cells;
  shapes = gMapShapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  if (Event_GetFlag(nextFlag)) {
    Actor_SetPosition(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Actor_GetCellIndex(object);
    cell = &cells[index];
    slot = shapes + index;
    shape = MapGrid_GetTerrainKind(object->terrainId, object->x, object->z);
    height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
    Actor_DisableById(id);
    cell->attributeB = 128;
    *slot =
        MapCollision_FindOrAddTag(object->terrainId, shape, (height >> 19) + 4);
    if (Event_GetFlag(flag))
      Actor_SelectAnimation(object, 0);
  }
  table += 2;
  if (*table != 0xffff)
    goto next;
done:
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ground = Map_GetTerrainHeight(leader->terrainId, leader->x, leader->z);
  if (leader->y < ground) {
    leader->terrainHeight = ground;
    leader->y = ground;
  }
}
