/// @file
/// The map's shape entries and the flag blocks: the puddles Frost freezes
/// into pillars of ice, each with the flag that says it is frozen.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"

/// Sets up Alhafra's and the Alhafran Cave's puddles as FlagBlock_PlaceAll
/// does, and marks the open cells on each puddle's four sides a gap to jump
/// across (MapGrid_MarkJumpIfOpen), so the leader jumps onto a frozen one.
void FlagBlock_PlaceAllMarkingEdges(struct ActorFlag *list) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct ActorFlag *first = list;
  struct Actor *leader;

  for (; list->actor != ACTOR_LIST_END; list++) {
    struct Actor *actor = Actor_GetById(list->actor);
    u16 flag = list->flag;
    s32 slot = Actor_GetCellIndex(actor);
    struct MapCell *cell = &map->layers[actor->layer].cells[slot];

    actor->motionFlags = 0;
    actor->collisionFlags &= ~ACTOR_SOLID;
    Actor_SetSpriteFlagsFar(actor, 0);
    cell->collisionCode = CELL_BEHAVIOR_WALL;
    MapGrid_MarkJumpIfOpen(actor->x.fixed + toFIXED(MAPGRID_CELL_PIXELS),
                           actor->z.fixed, actor->layer, actor->groundHeight);
    MapGrid_MarkJumpIfOpen(actor->x.fixed - toFIXED(MAPGRID_CELL_PIXELS),
                           actor->z.fixed, actor->layer, actor->groundHeight);
    MapGrid_MarkJumpIfOpen(actor->x.fixed,
                           actor->z.fixed + toFIXED(MAPGRID_CELL_PIXELS),
                           actor->layer, actor->groundHeight);
    MapGrid_MarkJumpIfOpen(actor->x.fixed,
                           actor->z.fixed - toFIXED(MAPGRID_CELL_PIXELS),
                           actor->layer, actor->groundHeight);
    if (GameFlag_Get(flag)) {
      Actor_SelectAnimationFar(actor, PUDDLE_ANIMATION_FROZEN);
      GameFlag_Clear(flag);
    } else {
      Actor_SelectAnimationFar(actor, PUDDLE_ANIMATION_PUDDLE);
    }
  }
  FlagBlock_ApplyFlags(first);
  leader = Actor_GetById(Party_GetLeaderActor());
  if (MapGrid_GetTerrainHeightAboveFar(leader->x.fixed, leader->z.fixed,
                                       leader->layer, leader->groundHeight))
    leader->groundHeight = leader->y.fixed = MapGrid_GetTerrainHeightFar(
        leader->layer, leader->x.fixed, leader->z.fixed);
}

/// For each flag block whose animation disagrees with its flag, turns its cell
/// and flag over: with the flag set and the ice pillar not showing, the cell
/// is lowered 32 pixels to a wall and the flag cleared; with the flag clear
/// and the puddle not showing (Frost freezing it), the cell is raised 32
/// pixels, open and a gap to jump across, and the flag set. The field work
/// remembers the block.
void FlagBlock_ApplyFlags(struct ActorFlag *list) {
  struct FieldWork *field = Ram_HeapSlots->fieldWork;
  u8 *grid = gMapShapeGrid;

  field->movedBlock = NULL;
  for (; list->actor != ACTOR_LIST_END; list++) {
    s32 change = 0;
    struct Actor *actor = Actor_GetById(list->actor);
    u8 animation = ((struct Sprite *)actor->sprite)->lastNo;
    u16 flag = list->flag;

    if (GameFlag_Get(flag)) {
      if (animation != PUDDLE_ANIMATION_FROZEN)
        change = 1;
    } else {
      if (animation != PUDDLE_ANIMATION_PUDDLE)
        change = 1;
    }
    if (change) {
      struct MapCell *cells = ((struct MapWork *)Ram_HeapSlots->mapGrid)
                                  ->layers[actor->layer]
                                  .cells;
      u8 *shapes;
      s32 slot;
      u32 kind;
      s32 height;

      shapes = &grid[((s32)cells - (s32)Ram_MapCellBuffer) >> 2];
      slot = Actor_GetCellIndex(actor);
      kind = MapGrid_GetTerrainKindFar(actor->layer, actor->x.fixed,
                                       actor->z.fixed);
      height = CELL_HEIGHT_STEPS(MapGrid_GetTerrainHeightFar(
          actor->layer, actor->x.fixed, actor->z.fixed));

      field->movedBlock = actor;
      if (GameFlag_Get(flag)) {
        height -= CELL_HEIGHT_STEPS(toFIXED(32));
        GameFlag_Clear(flag);
      } else {
        height += CELL_HEIGHT_STEPS(toFIXED(32));
        GameFlag_Set(flag);
      }
      if ((actor->x.fixed | actor->z.fixed) != 0) {

        cells += slot;
        shapes += slot;
        *shapes = MapCollision_FindOrAddTag(actor->layer, kind, height);
        if (GameFlag_Get(flag)) {
          s32 bits;

          cells->collisionCode = CELL_BEHAVIOR_NORMAL;
          bits = cells->attributeB | MAP_CELL_JUMP;
          cells->attributeB = bits;
        } else {
          cells->collisionCode = CELL_BEHAVIOR_WALL;
          cells->attributeB &= ~MAP_CELL_JUMP;
        }
      }
    }
  }
}

/// Returns the index of the actor's cell in its map layer.
s32 Actor_GetCellIndex(struct Actor *actor) {
  return actor->x.fixed / toFIXED(MAPGRID_CELL_PIXELS) +
         (actor->z.fixed / toFIXED(MAPGRID_CELL_PIXELS)) * MAPGRID_SIZE;
}

/// Returns the index of the shape table's entry (gMapGridShapes) of kind key
/// whose first height is height, in 8-pixel steps, writing one at the first
/// free entry (kind COLLISION_TAG_FREE) when there is none: the index a raised
/// or lowered cell's shape takes. The layer goes unread.
s32 MapCollision_FindOrAddTag(s32 layer, s32 key, s32 height) {
  u8 *entry = Ram_MapCollision;
  s8 entryHeight;
  s32 i = 0;

  entryHeight = height;
  while (1) {
    if (entry[0] == key) {
      if (*(s8 *)++entry == entryHeight)
        return i;
      entry--;
    } else if (entry[0] == COLLISION_TAG_FREE) {
      entry[0] = key;
      entry[1] = entryHeight;
      return i;
    }
    entry += 4;
    i++;
  }
}
