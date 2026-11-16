/// Puts the pillars of a list back where their flags left them.
void MagmaRockInside_RestorePillars(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *object;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  u8 *grid = gMapShapeGrid; /* the shape grid, read once */

  Actor_MoveToObject(8, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(9, gMainRoutineWork.leaderActorId);
  Actor_MoveToObject(10, gMainRoutineWork.leaderActorId);
  System_Wait(1);
  Pillar_TakeOutOfPlay(8);
  Pillar_TakeOutOfPlay(9);
  Pillar_TakeOutOfPlay(10);
  Actor_SetAnimation(9, 0);
  System_Wait(1);
  Actor_SetPosition(8, 0, 0);
  Actor_SetPosition(9, 0, 0);
  Actor_SetPosition(10, 0, 0);
  System_Wait(1);
  if (*list == 0xffff)
    goto done;
next:
  id = list[0];
  object = Actor_Get(id);
  flag = list[1];
  nextFlag = flag + 1;
  cells = map->layers[object->layer].cells;
  shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);
  object->retainSpriteCache = 3;
  if (Event_GetFlag(nextFlag)) {
    Actor_SetPosition(id, 0, 0);
  } else {
    s32 shape;
    u8 *slot;
    s32 height;

    index = Actor_GetCellIndex(object);
    cell = &cells[index];
    slot = shapes + index;
    shape =
        MapGrid_GetTerrainKind(object->layer, object->x.fixed, object->z.fixed);
    height =
        Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed);
    Actor_DisableById(id);
    cell->attributeB = 128;
    *slot = MapCollision_FindOrAddTag(object->layer, shape, (height >> 19) + 4);
    if (Event_GetFlag(flag))
      Actor_CopyGraphics(id, 9);
  }
  list += 2;
  if (*list != 0xffff)
    goto next;
done:
  System_Wait(10);
}
