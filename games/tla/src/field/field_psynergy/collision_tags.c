/// @file
/// The map collision's tags and the flag blocks.
// Part of field_psynergy.c.

#include "../field_psynergy_internal.h"

/// FlagBlock_PlaceAll that also marks the four cells beside each block raised
/// where they are open (MapGrid_MarkRaisedIfOpen), as Alhafra and its cave
/// need.
void FlagBlock_PlaceAllMarkingEdges(struct ActorFlag *list) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct ActorFlag *first = list;
  struct Actor *owner;

  for (; list->actor != 0xffff; list++) {
    struct Actor *object = Actor_GetById(list->actor);
    u16 flag = list->flag;
    s32 slot = Actor_GetCellIndex(object);
    struct MapCell *cell = &map->layers[object->layer].cells[slot];

    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    Actor_SetSpriteFlagsFar(object, 0);
    cell->collisionCode = 0xff;
    MapGrid_MarkRaisedIfOpen(object->x.fixed + 0x100000, object->z.fixed,
                             object->layer, object->groundHeight);
    MapGrid_MarkRaisedIfOpen(object->x.fixed - 0x100000, object->z.fixed,
                             object->layer, object->groundHeight);
    MapGrid_MarkRaisedIfOpen(object->x.fixed, object->z.fixed + 0x100000,
                             object->layer, object->groundHeight);
    MapGrid_MarkRaisedIfOpen(object->x.fixed, object->z.fixed - 0x100000,
                             object->layer, object->groundHeight);
    if (GameFlag_Get(flag)) {
      Actor_SelectAnimationFar(object, 0);
      GameFlag_Clear(flag);
    } else {
      Actor_SelectAnimationFar(object, 1);
    }
  }
  FlagBlock_ApplyFlags(first);
  owner = Actor_GetById(Party_GetLeaderActor());
  if (MapGrid_GetTerrainHeightAboveFar(owner->x.fixed, owner->z.fixed,
                                       owner->layer, owner->groundHeight))
    owner->groundHeight = owner->y.fixed = MapGrid_GetTerrainHeightFar(
        owner->layer, owner->x.fixed, owner->z.fixed);
}

struct SwitchCell {
  u8 unk00[2];
  u8 block;
  u8 unk03_0 : 7;
  u8 raised : 1;
};

struct RestoreLayer {
  u8 unk00[0x138 - 0x108];
  struct SwitchCell *cells;
  u8 unk34[4];
};

struct RestoreMap {
  u8 unk000[0x108];
  struct RestoreLayer layers[1];
};

/// For each flag block whose picture disagrees with its flag: with the flag
/// set its cell is lowered 32 pixels and the flag cleared, with it clear the
/// cell is raised and the flag set; the event work remembers the block.
void FlagBlock_ApplyFlags(struct ActorFlag *list) {
  u8 *event = Ram_HeapSlots->fieldWork;
  u8 *grid = gMapShapeGrid;

  *(struct Actor **)(event + 0xcbc) = NULL;
  for (; list->actor != 0xffff; list++) {
    s32 change = 0;
    struct Actor *object = Actor_GetById(list->actor);
    u8 frame = ((u8 *)object->sprite)[24];
    u16 flag = list->flag;

    if (GameFlag_Get(flag)) {
      if (frame != 0)
        change = 1;
    } else {
      if (frame != 1)
        change = 1;
    }
    if (change) {
      struct SwitchCell *cells = ((struct RestoreMap *)Ram_HeapSlots->mapGrid)
                                     ->layers[object->layer]
                                     .cells;
      u8 *shapes;
      s32 slot;
      u32 key;
      s32 height;

      shapes = &grid[((s32)cells - (s32)Ram_MapCellBuffer) >> 2];
      slot = Actor_GetCellIndex(object);
      key = MapGrid_GetTerrainKindFar(object->layer, object->x.fixed,
                                      object->z.fixed);
      height = MapGrid_GetTerrainHeightFar(object->layer, object->x.fixed,
                                           object->z.fixed) >>
               19;

      *(struct Actor **)(event + 0xcbc) = object;
      if (GameFlag_Get(flag)) {
        height -= 4;
        GameFlag_Clear(flag);
      } else {
        height += 4;
        GameFlag_Set(flag);
      }
      if ((object->x.fixed | object->z.fixed) != 0) {

        cells += slot;
        shapes += slot;
        *shapes = MapCollision_FindOrAddTag(object->layer, key, height);
        if (GameFlag_Get(flag)) {
          s32 bits;

          cells->block = 0;
          // The bit set in int arithmetic, where the clear below is a bitfield.
          bits = ((u8 *)cells)[3] | 0x80;
          ((u8 *)cells)[3] = bits;
        } else {
          cells->block = 0xff;
          cells->raised = 0;
        }
      }
    }
  }
}

/// Returns the index of object's cell in a 128-cell-wide map layer.
s32 Actor_GetCellIndex(struct Actor *object) {
  return object->x.fixed / 0x100000 + (object->z.fixed / 0x100000) * 128;
}

/// Finds the tag (key, value) in the 4-byte entries at gMapGridShapes, or adds
/// it at the first free entry (key 0xff), and returns its index.
/// Actor_PlaceBlocks gives a raised cell's shape this index.
s32 MapCollision_FindOrAddTag(s32 unused, s32 key, s32 value) {
  u8 *entry = Ram_MapCollision;
  s8 tagValue;
  s32 i = 0;

  tagValue = value;
  while (1) {
    if (entry[0] == key) {
      if (*(s8 *)++entry == tagValue)
        return i;
      entry--;
    } else if (entry[0] == 0xff) {
      entry[0] = key;
      entry[1] = tagValue;
      return i;
    }
    entry += 4;
    i++;
  }
}
