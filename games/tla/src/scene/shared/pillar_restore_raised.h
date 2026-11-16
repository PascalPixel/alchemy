/// The actor id that ends a list of actors or of (actor, kind, flag) entries.
#define ACTOR_LIST_END 0xffff

/// Pillar_Restore without the reset, lifting the leader onto the ground.
void MagmaRockInside_RestoreCrumblingPillars(const u16 *table) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
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

  if (*table == ACTOR_LIST_END)
    goto done;
next:
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
    height = Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
    Actor_DisableById(id);
    cell->attributeB = 128;
    *slot = MapCollision_FindOrAddTag(actor->layer, shape, (height >> 19) + 4);
    if (Event_GetFlag(flag))
      Actor_SelectAnimation(actor, 0);
  }
  table += 2;
  if (*table != ACTOR_LIST_END)
    goto next;
done:
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  ground =
      Map_GetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
  if (leader->y.fixed < ground) {
    leader->groundHeight = ground;
    leader->y.fixed = ground;
  }
}
