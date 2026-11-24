#include "pillar_stand_in.h"

/// Unused: as MagmaRockInside_RestoreCrumblingPillars, in a room whose pillars
/// share one sprite, as Dehkan Plateau's CrumblingPillar_PlaceWithStandIn
/// does: first loads the stand-in's and the two pictures' sprites beside the
/// leader, the cracked one showing its cracks, then puts them out of sight; a
/// cracked pillar takes the cracked picture.
UNUSED void MagmaRockInside_RestorePillars(u16 *list) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor;
  s32 id;
  s32 flag;
  s32 nextFlag;
  struct MapCell *cells;
  u8 *shapes;
  struct MapCell *cell;
  s32 index;
  u8 *grid = gMapShapeGrid;

  Actor_PlaceOnActor(ACTOR_PILLAR_STAND_IN, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_CRACKED_PILLAR_SPRITE,
                     gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_WHOLE_PILLAR_SPRITE,
                     gMainRoutineState.leaderActorId);
  System_Wait(1);
  Pillar_TakeOutOfPlay(ACTOR_PILLAR_STAND_IN);
  Pillar_TakeOutOfPlay(ACTOR_CRACKED_PILLAR_SPRITE);
  Pillar_TakeOutOfPlay(ACTOR_WHOLE_PILLAR_SPRITE);
  Actor_SetAnimation(ACTOR_CRACKED_PILLAR_SPRITE, 0);
  System_Wait(1);
  Actor_SetPosition(ACTOR_PILLAR_STAND_IN, 0, 0);
  Actor_SetPosition(ACTOR_CRACKED_PILLAR_SPRITE, 0, 0);
  Actor_SetPosition(ACTOR_WHOLE_PILLAR_SPRITE, 0, 0);
  System_Wait(1);
  while (*list != ACTOR_LIST_END) {
    id = list[0];
    actor = Actor_Get(id);
    flag = list[1];
    nextFlag = flag + 1;
    cells = map->layers[actor->layer].cells;
    shapes = grid + (cells - (struct MapCell *)Ram_MapCellBuffer);
    actor->retainSpriteCache = 3;
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
      *slot = MapCollision_FindOrAddTag(actor->layer, shape,
                                        CELL_HEIGHT_STEPS(height) + 4);
      if (Event_GetFlag(flag))
        Actor_CopyGraphics(id, ACTOR_CRACKED_PILLAR_SPRITE);
    }
    list += 2;
  }
  System_Wait(10);
}
