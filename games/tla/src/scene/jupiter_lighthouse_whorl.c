/// @file
/// The great hall of Jupiter Lighthouse's right tower, behind its archer
/// statue, where the Jupiter Djinni Whorl waits on a ledge
/// (JupiterLighthouse_FightWhorl). Two energy path blocks, pushed or Moved into
/// two of the three gaps in the floor (JupiterLighthouse_PlacePathBlock), light
/// the path and raise a cone-shaped pillar; Pounding it throws the blocks back
/// out of their gaps (JupiterLighthouse_RaiseBlocksFromGaps). A third block in
/// the gap by the west wall lights the path its grey blocks stand on, and the
/// pillar pushed onto the button by the door opens it for good, a shortcut.
#include "flags.h"
#include "djinn_ids.h"
#include "types.h"
#include "system/iwram_call.h"
#include "field/field_sequence.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "field/field_sequence_event.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "system/input.h"
#include "text/msg_ids.h"
#include "system/frame.h"

extern s16 gJupiterLighthouse_WestPathCells[][2];

void JupiterLighthouse_LightWestPath(s32 mode);
void JupiterLighthouse_ThrowOutBlock(s32 id, s32 x, s32 z, s32 dx, s32 dz);
void Leader_CenterInCell(void);
#include "field/call.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/field_event.h"
#include "field/field_sprite.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "scene/scene.h"
#include "system/heap_release.h"
#include "field/cell_target.h"
#include "system/io_write.h"
#include "field/field_effect.h"
#include "macros.h"
#include "shared/tables.h"
#include "field/actor_script.h"

/// An energy path block in one of the three gaps, a flag for each block and
/// gap: FLAG_PATH_BLOCK_IN_GAP(gap, block), gap the PathGap, block 0 for
/// ACTOR_FIRST_PATH_BLOCK and 1 for ACTOR_SECOND_PATH_BLOCK.
#define FLAG_PATH_BLOCK_IN_GAP(gap, block) (0xa10 + (gap) * 2 + (block))
/// The cone-shaped pillar the two blocks raise, Pounded down again (and down
/// until both blocks are in gaps).
#define FLAG_FIRST_CONE_SUNK 0xa16
/// The cone-shaped pillar the third block raises, Pounded down again.
#define FLAG_SECOND_CONE_SUNK 0xa17
/// The third energy path block lies in its gap by the west wall at (8, 12).
#define FLAG_THIRD_BLOCK_IN_GAP 0xa1a
/// The pillar stands on the button at (8, 37): the door stays open.
#define FLAG_BUTTON_PILLAR_PLACED 0xa1b

/// The three gaps the first two energy path blocks fit, by cell.
enum PathGap {
  PATH_GAP_EAST,  ///< cell (35, 15)
  PATH_GAP_WEST,  ///< cell (31, 16)
  PATH_GAP_NORTH, ///< cell (33, 10)
};

enum JupiterLighthouseWhorlActorId {
  ACTOR_ARCHER_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_CONE,
  /// The six grey blocks on the third block's path, raised while it is lit.
  ACTOR_FIRST_GREY_BLOCK,
  ACTOR_LAST_GREY_BLOCK = ACTOR_FIRST_GREY_BLOCK + 5,
  ACTOR_BUTTON_PILLAR,
  ACTOR_FIRST_CONE,
  ACTOR_FIRST_PATH_BLOCK,
  ACTOR_SECOND_PATH_BLOCK,
  ACTOR_THIRD_PATH_BLOCK,
  ACTOR_WHORL
};

extern s16 gJupiterLighthouse_WhorlPoundTargets[];
extern s32 gPaletteCyclePausesInEvents;
extern s16 gLightSpriteCount;
void JupiterLighthouse_OpenDoor(s32 value);
void JupiterLighthouse_CloseDoor(void);
void JupiterLighthouse_CheckBlockOnCell(void);
void MapGrid_SetCellHeight(s32 x, s32 z, s32 a, s32 b);
void MapGrid_ClearAttributeBBitsAt(s32 x, s32 z, s32 a, s32 b);
void Field_TryJumpForward(void);

void JupiterLighthouse_FightWhorl(void) {
  Field_RequestDjinnBattle(ACTOR_WHORL, DJINNI_WHORL);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Opens a door three cells high, with its sound when `mode` is 1.
void JupiterLighthouse_OpenDoor(s32 mode) {
  if (mode == 1) {
    Event_PlaySound(SE_IMPACT);
    MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(81, 42, 4, 3, 76, 35);
  MapGrid_CopyCellAttributes(12, 41, 4, 1, 12, 35);
}

/// Closes a door three cells high.
void JupiterLighthouse_CloseDoor(void) {
  Event_PlaySound(SE_HEAVY_IMPACT);
  MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
  System_Wait(6);
  MapGrid_CopyCellsLayered(71, 42, 4, 3, 76, 35);
  MapGrid_CopyCellAttributes(12, 36, 4, 1, 12, 35);
}

/// Opens the door for good once the pillar stands on its button, cell (8, 37).
void JupiterLighthouse_CheckBlockOnCell(void) {
  struct Actor *object = Actor_Get(ACTOR_BUTTON_PILLAR);

  if (!Event_GetFlag(FLAG_BUTTON_PILLAR_PLACED) &&
      FIELD_CELL(object->x.fixed) == 8 && FIELD_CELL(object->z.fixed) == 37) {
    Event_SetFlag(FLAG_BUTTON_PILLAR_PLACED);
    JupiterLighthouse_OpenDoor(1);
  }
}

/// The leader steps onto a door's button: the door opens and 0x202 says the
/// button holds it.
void JupiterLighthouse_StepOnButton(void) {
  if (!Event_GetFlag(FLAG_SCENE_TEMP_02)) {
    Event_SetFlag(FLAG_SCENE_TEMP_02);
    JupiterLighthouse_OpenDoor(1);
  }
}

/// The leader steps off a door's button: a door the button held open (0x202
/// set) closes, unless the block now stands on the button
/// (JupiterLighthouse_CheckBlockOnCell).
void JupiterLighthouse_StepOffButton(void) {
  if (Event_GetFlag(FLAG_SCENE_TEMP_02) == 1) {
    Event_ClearFlag(FLAG_SCENE_TEMP_02);
    JupiterLighthouse_CloseDoor();
  }
  JupiterLighthouse_CheckBlockOnCell();
}

void JupiterLighthouse_MoveButtonPillar(void) {
  System_Wait(10);
  JupiterLighthouse_CheckBlockOnCell();
}

/// Marks the gap the first two blocks have just come to rest in; returns its
/// flag's place after FLAG_PATH_BLOCK_IN_GAP(0, 0), or -1 for none.
s32 JupiterLighthouse_FindBlockGap(void) {
  s32 x, z, gap;
  struct Actor *second;
  struct Actor *first;

  first = Actor_Get(ACTOR_FIRST_PATH_BLOCK);
  second = Actor_Get(ACTOR_SECOND_PATH_BLOCK);
  gap = 1;
  x = FIELD_CELL(first->x.fixed);
  gap = -gap;
  z = FIELD_CELL(first->z.fixed);
  if (x == 35 && z == 15 &&
      !Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 1))) {
    Event_SetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 0));
    gap = 0;
  }
  if (x == 31 && z == 16 &&
      !Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 1))) {
    Event_SetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 0));
    gap = 2;
  }
  if (x == 33 && z == 10 &&
      !Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 1))) {
    Event_SetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 0));
    gap = 4;
  }
  x = FIELD_CELL(second->x.fixed);
  z = FIELD_CELL(second->z.fixed);
  if (x == 35 && z == 15 &&
      !Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 0))) {
    Event_SetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 1));
    gap = 1;
  }
  if (x == 31 && z == 16 &&
      !Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 0))) {
    Event_SetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 1));
    gap = 3;
  }
  if (x == 33 && z == 10 &&
      !Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 0))) {
    Event_SetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 1));
    gap = 5;
  }
  return gap;
}

/// How many of the first two blocks lie in gaps.
s32 JupiterLighthouse_CountBlocksInGaps(void) {
  s32 count = 0;

  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 0)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 0)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 0)))
    count = 1;
  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 1)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 1)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 1)))
    count++;
  return count;
}

/// Draws the floor for the blocks that lie in gaps: each filled gap and the
/// lit path between them.
void JupiterLighthouse_DrawBlocksInGaps(void) {
  MapGrid_CopyCellsLayered(94, 60, 9, 8, 94, 10);
  Call4((void (*)())MapGrid_SetCellHeight, 0x2300000, 0xf00000, 0, -2);
  Call4((void (*)())MapGrid_SetCellHeight, 0x1f00000, 0x1000000, 0, -2);
  Call4((void (*)())MapGrid_SetCellHeight, 0x2100000, 0xa00000, 0, -2);
  FieldMapGrid_SetCellBehavior(toFIXED(608), toFIXED(160), 2, 0);
  FieldMapGrid_SetCellBehavior(toFIXED(496), toFIXED(160), 2, 0);
  FieldMapGrid_SetCellBehavior(toFIXED(496), toFIXED(224), 2, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 0)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 1)))
    MapGrid_SetCellHeight(0x2300000, 0xf00000, 0, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 0)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 1)))
    MapGrid_SetCellHeight(0x1f00000, 0x1000000, 0, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 0)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 1)))
    MapGrid_SetCellHeight(0x2100000, 0xa00000, 0, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 0))) {
    MapGrid_CopyCellsLayered(96, 56, 7, 2, 96, 15);
    MapGrid_CopyCellsLayered(111, 51, 1, 1, 99, 15);
    if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 1))) {
      MapGrid_CopyCellsLayered(105, 51, 1, 3, 95, 14);
      FieldMapGrid_SetCellBehavior(toFIXED(496), toFIXED(224), 2, 0xec);
    }
    if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 1)))
      MapGrid_CopyCellsLayered(111, 53, 1, 1, 97, 10);
  } else if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 1))) {
    MapGrid_CopyCellsLayered(98, 51, 5, 1, 98, 10);
    MapGrid_CopyCellsLayered(99, 52, 4, 5, 99, 11);
    FieldMapGrid_SetCellBehavior(toFIXED(608), toFIXED(160), 2, 0xec);
    if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 0)))
      MapGrid_CopyCellsLayered(106, 53, 1, 1, 95, 16);
    if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 0))) {
      MapGrid_CopyCellsLayered(109, 51, 3, 1, 95, 10);
      FieldMapGrid_SetCellBehavior(toFIXED(496), toFIXED(160), 2, 0xec);
    }
  } else {
    if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 0)))
      MapGrid_CopyCellsLayered(106, 53, 1, 1, 95, 16);
    if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 1)))
      MapGrid_CopyCellsLayered(107, 53, 1, 1, 95, 16);
    if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 0)))
      MapGrid_CopyCellsLayered(111, 52, 1, 1, 97, 10);
    if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 1)))
      MapGrid_CopyCellsLayered(111, 53, 1, 1, 97, 10);
  }
}

s32 JupiterLighthouse_IsOnWestPath(struct Actor *actor) {
  s32 i;

  for (i = 0; gJupiterLighthouse_WestPathCells[i][0] != -1; i++) {
    if (gJupiterLighthouse_WestPathCells[i][0] == FIELD_CELL(actor->x.fixed) &&
        gJupiterLighthouse_WestPathCells[i][1] == FIELD_CELL(actor->z.fixed))
      return 1;
  }
  return 0;
}

/// Lights or darkens the path of grey blocks by the west wall as the third
/// block lies in its gap or not, raising the blocks on it (gradually for
/// mode 1).
void JupiterLighthouse_LightWestPath(s32 mode) {
  struct Actor *object;
  s32 i, frame;

  if (!Event_GetFlag(FLAG_THIRD_BLOCK_IN_GAP)) {
    MapGrid_CopyCellsLayered(80, 46, 1, 7, 72, 9);
    MapGrid_CopyCellsLayered(80, 53, 8, 5, 72, 16);
    MapGrid_CopyCellsLayered(78, 47, 1, 1, 8, 74);
    MapGrid_SetCellHeight(0x800000, 0xc00000, 0, 4);
    if (mode == 1) {
      frame = 0;
      do {
        i = 0;
        do {
          object = Actor_Get(ACTOR_FIRST_GREY_BLOCK + i);
          if (JupiterLighthouse_IsOnWestPath(object)) {
            object->motionFlags = 0;
            object->y.fixed -= toFIXED(2);
          }
        } while (++i <= 5);
        System_Wait(1);
        frame++;
      } while (frame <= 15);
    }
    i = 0;
    do {
      object = Actor_Get(ACTOR_FIRST_GREY_BLOCK + i);
      object->motionFlags = 3;
      i++;
      object->y.fixed = object->groundHeight;
    } while (i <= 5);
  } else {
    MapGrid_CopyCellsLayered(72, 46, 1, 7, 72, 9);
    MapGrid_CopyCellsLayered(72, 53, 8, 5, 72, 16);
    MapGrid_CopyCellsLayered(70, 47, 1, 1, 8, 74);
    MapGrid_SetCellHeight(0x800000, 0xc00000, 0, 6);
    FieldActor_Stop(gMainRoutineWork.leaderActorId);
    Actor_SetPosition(ACTOR_THIRD_PATH_BLOCK, 0, 0);
    if (mode == 1) {
      frame = 0;
      do {
        i = 0;
        do {
          object = Actor_Get(ACTOR_FIRST_GREY_BLOCK + i);
          if (JupiterLighthouse_IsOnWestPath(object)) {
            object->motionFlags = 0;
            object->y.fixed += toFIXED(2);
          }
        } while (++i <= 5);
        System_Wait(1);
        frame++;
      } while (frame <= 15);
    }
    i = 0;
    do {
      object = Actor_Get(ACTOR_FIRST_GREY_BLOCK + i);
      if (JupiterLighthouse_IsOnWestPath(object)) {
        object->motionFlags = 4;
        object->y.fixed = toFIXED(65) / 2;
      }
    } while (++i <= 5);
  }
}

void JupiterLighthouse_LiftPathBlock(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == ACTOR_THIRD_PATH_BLOCK) {
    MapGrid_SetCellHeight(object->x.fixed, object->z.fixed, 0, 6);
    MapGrid_ClearAttributeBBitsAt(object->x.fixed, object->z.fixed, 0, 128);
  }
}

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void MapGrid_SetAttributeBBitsAt(s32 x, s32 z, s32 a, s32 b);

/// An energy path block set down after a push or Move: it falls into place,
/// and in a gap it lights the path there. Once both of the first two lie in
/// gaps the cone-shaped pillar rises; the third, in its gap by the west wall,
/// lights the path of grey blocks and raises the second cone.
void JupiterLighthouse_PlacePathBlock(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 spot;
  s32 i;

  if (object->groundHeight !=
      Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed)) {
    object->motionFlags = 3;
    wait(2);
    i = 0;
    if (object->y.fixed > object->groundHeight || object->velocityY != 0) {
      do {
        wait(1);
        i++;
        if (i > 29)
          break;
      } while (object->y.fixed > object->groundHeight ||
               object->velocityY != 0);
    }
    Event_PlaySound(SE_THUMP);
    object->motionFlags = 0;
  }
  if ((u32)(id - ACTOR_FIRST_PATH_BLOCK) <= 1) {
    spot = JupiterLighthouse_FindBlockGap();
    if (spot == -1)
      return;
    Actor_SetSpriteFlags(Actor_GetById(id), 0);
    Event_Wait(30);
    JupiterLighthouse_DrawBlocksInGaps();
    if (spot == 0)
      Event_PlaySound(SE_MAP_TRANSITION);
    if (spot == 1)
      Event_PlaySound(SE_MAP_TRANSITION);
    if (spot == 3 && Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 0)))
      Event_PlaySound(SE_MAP_TRANSITION);
    if (spot == 4 && Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 1)))
      Event_PlaySound(SE_MAP_TRANSITION);
    Actor_Stop(gMainRoutineWork.leaderActorId);
    Actor_SetPosition(id, 0, 0);
    if (JupiterLighthouse_CountBlocksInGaps() != 2 ||
        !Event_GetFlag(FLAG_FIRST_CONE_SUNK))
      return;
    object = Actor_GetById(ACTOR_FIRST_CONE);
    Event_ClearFlag(FLAG_FIRST_CONE_SUNK);
    Event_Wait(10);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldCamera_MoveTo(664, CAMERA_KEEP, 136, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(20);
    Event_PlaySound(SE_LIGHT_APPEAR);
    Actor_SelectAnimation(object, 3);
    object->priorityFlags &= ~2;
    FieldMapGrid_SetCellBehavior(object->x.fixed, object->z.fixed,
                                 object->layer, 255);
    Event_Wait(30);
    EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
    FieldCamera_WaitForArrival();
    Event_End();
    return;
  }
  if (id != ACTOR_THIRD_PATH_BLOCK)
    return;
  if (FIELD_CELL(object->x.fixed) == 8 &&
      FIELD_CELL(*(s32 *)&object->z.fixed) == 12) {
    Event_SetFlag(FLAG_THIRD_BLOCK_IN_GAP);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_THIRD_PATH_BLOCK), 0);
    Event_Wait(30);
    Event_PlaySound(SE_MAP_TRANSITION);
    JupiterLighthouse_LightWestPath(1);
    if (!Event_GetFlag(FLAG_SECOND_CONE_SUNK))
      return;
    object = Actor_GetById(ACTOR_SECOND_CONE);
    Event_ClearFlag(FLAG_SECOND_CONE_SUNK);
    Event_Wait(20);
    Event_PlaySound(SE_LIGHT_APPEAR);
    Actor_SelectAnimation(object, 3);
    object->priorityFlags &= ~2;
    FieldMapGrid_SetCellBehavior(object->x.fixed, object->z.fixed,
                                 object->layer, 255);
  } else {
    MapGrid_SetCellHeight(object->x.fixed, object->z.fixed, 0, 8);
    MapGrid_SetAttributeBBitsAt(object->x.fixed, object->z.fixed, 0, 128);
  }
}

void JupiterLighthouse_StartPound(void) { Event_ResolvePendingActions(0); }

void JupiterLighthouse_SinkPoundTarget(s32 a, s32 b) {
  Pound_SinkTarget(gJupiterLighthouse_WhorlPoundTargets, a, b);
}

/// Throws a block up out of its gap at cell (x, z) to land beside it.
void JupiterLighthouse_ThrowOutBlock(s32 id, s32 x, s32 z, s32 dx, s32 dz) {
  struct Actor *object = Actor_Get(id);

  Actor_SetPositionFX(id, (x << 20) + toFIXED(8), (z << 20) + toFIXED(8));
  object->motionFlags = 3;
  object->groundHeight = toFIXED(-16);
  object->y.fixed = toFIXED(-16);
  object->velocityY = toFIXED(8);
  Actor_SetSpeed(id, 1.6);
  Actor_SetDestinationOffset(id, dx, dz);
  Event_PlaySound(SE_WHIRLWIND);
}

/// The cone-shaped pillar Pounded down throws the two blocks back out of their
/// gaps, the camera watching.
void JupiterLighthouse_RaiseBlocksFromGaps(void) {
  struct Actor *object;
  s32 i;

  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 0)) +
          Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 0)) +
          Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 0)) +
          Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 1)) +
          Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 1)) +
          Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 1)) ==
      0)
    return;
  FieldCamera_MoveTo(592, CAMERA_KEEP, 248, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 0)))
    JupiterLighthouse_ThrowOutBlock(ACTOR_FIRST_PATH_BLOCK, 35, 15, -16, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 1)))
    JupiterLighthouse_ThrowOutBlock(ACTOR_SECOND_PATH_BLOCK, 35, 15, -16, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 0)))
    JupiterLighthouse_ThrowOutBlock(ACTOR_FIRST_PATH_BLOCK, 31, 16, 16, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 1)))
    JupiterLighthouse_ThrowOutBlock(ACTOR_SECOND_PATH_BLOCK, 31, 16, 16, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 0)))
    JupiterLighthouse_ThrowOutBlock(ACTOR_FIRST_PATH_BLOCK, 33, 10, -16, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 1)))
    JupiterLighthouse_ThrowOutBlock(ACTOR_SECOND_PATH_BLOCK, 33, 10, -16, 0);
  Event_ClearFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 0));
  Event_ClearFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 1));
  Event_ClearFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 0));
  Event_ClearFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 1));
  Event_ClearFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 0));
  Event_ClearFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 1));
  JupiterLighthouse_DrawBlocksInGaps();
  System_Wait(2);
  object = Actor_Get(ACTOR_FIRST_PATH_BLOCK);
  i = 0;
  if (object->y.fixed > object->groundHeight || object->velocityY != 0) {
    do {
      System_Wait(1);
      i++;
      if (i > 29)
        break;
    } while (object->y.fixed > object->groundHeight || object->velocityY != 0);
  }
  object->motionFlags = 0;
  object = Actor_Get(ACTOR_SECOND_PATH_BLOCK);
  i = 0;
  if (object->y.fixed > object->groundHeight || object->velocityY != 0) {
    do {
      System_Wait(1);
      i++;
      if (i > 29)
        break;
    } while (object->y.fixed > object->groundHeight || object->velocityY != 0);
  }
  object->motionFlags = 0;
  FieldEvent_Wait(30);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
}

/// The second cone Pounded down throws the third block back out of its gap.
void JupiterLighthouse_RaiseThirdBlockFromGap(void) {
  struct Actor *object;
  s32 i;

  if (Event_GetFlag(FLAG_THIRD_BLOCK_IN_GAP))
    JupiterLighthouse_ThrowOutBlock(ACTOR_THIRD_PATH_BLOCK, 8, 12, 16, 0);
  Event_ClearFlag(FLAG_THIRD_BLOCK_IN_GAP);
  JupiterLighthouse_LightWestPath(1);
  System_Wait(2);
  object = Actor_Get(ACTOR_THIRD_PATH_BLOCK);
  for (i = 0; i < 30; i++) {
    if (object->y.fixed <= object->groundHeight && object->velocityY == 0)
      break;
    System_Wait(1);
  }
  object->motionFlags = 0;
}

void Leader_PushObjectAhead(void);

/// Walking into a block on the floor pushes it; walking at a gap with no block
/// in it jumps the gap.
void JupiterLighthouse_JumpOrPushBlock(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *first = Actor_Get(ACTOR_FIRST_PATH_BLOCK);
  struct Actor *second = Actor_Get(ACTOR_SECOND_PATH_BLOCK);
  FIXED position[3];
  s32 found = 0;
  s32 x;
  s32 z;
  s32 facing;

  facing = (leader->facing + 0x2000) & 0xc000;
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  FieldVector_AddPolarOffset(16, facing, position);
  x = FIELD_CELL(position[0]);
  z = FIELD_CELL(position[2]);
  if (first->y.fixed == 0 && x == FIELD_CELL(first->x.fixed) &&
      z == FIELD_CELL(first->z.fixed))
    found = 1;
  if (second->y.fixed == 0 && x == FIELD_CELL(second->x.fixed) &&
      z == FIELD_CELL(second->z.fixed))
    found = 1;
  if (found == 0) {
    if (x == 35 && z == 15 &&
        (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 0)) ||
         Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 1))))
      found = 2;
    if (x == 31 && z == 16 &&
        (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 0)) ||
         Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 1))))
      found = 2;
    if (x == 33 && z == 10 &&
        (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 0)) ||
         Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 1))))
      found = 2;
    if (found == 0) {
      Field_TryJumpForward();
      return;
    }
  }
  if (found == 1)
    Leader_PushObjectAhead();
}

void JupiterLighthouse_JumpForward(void) { Field_TryJumpForward(); }

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

void Actors_SetCellsOpen();
void LightSprites_InitB();
void LightSprites_AddCounted();

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if ((u16)(gMainRoutineWork.entrance - 3) <= 1)
    Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(ACTOR_ARCHER_STATUE)->scaleX = FIXED_ONE * 3 / 4;
  Actor_Get(ACTOR_ARCHER_STATUE)->scaleY = FIXED_ONE * 3 / 4;
  Actors_SetCellsOpen((s32)&gJupiterLighthouse_WhorlPoundTargets);
  if (Event_GetFlag(FLAG_BUTTON_PILLAR_PLACED)) {
    Actor_SetPosition(ACTOR_BUTTON_PILLAR, 136, 600);
    JupiterLighthouse_OpenDoor(0);
  }
  if (Event_GetFlag(FLAG_SCENE_TEMP_02)) {
    JupiterLighthouse_OpenDoor(0);
  }
  LightSprites_InitB();
  LightSprites_SetPriority(2);
  LightSprites_AddCounted(ACTOR_FIRST_PATH_BLOCK, 2);
  LightSprites_AddCounted(ACTOR_SECOND_PATH_BLOCK, 1);
  LightSprites_AddCounted(ACTOR_THIRD_PATH_BLOCK, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 0)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 0)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 0)))
    Actor_SetPosition(ACTOR_FIRST_PATH_BLOCK, 0, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 1)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 1)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 1)))
    Actor_SetPosition(ACTOR_SECOND_PATH_BLOCK, 0, 0);
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 0x40;
  Actor_Get(ACTOR_FIRST_GREY_BLOCK)->priorityFlags |= 0x80;
  Actor_Get(ACTOR_FIRST_GREY_BLOCK + 1)->priorityFlags |= 0x80;
  Actor_Get(ACTOR_FIRST_GREY_BLOCK + 2)->priorityFlags |= 0x80;
  Actor_Get(ACTOR_FIRST_GREY_BLOCK + 3)->priorityFlags |= 0x80;
  Actor_Get(ACTOR_FIRST_GREY_BLOCK + 4)->priorityFlags |= 0x80;
  Actor_Get(ACTOR_LAST_GREY_BLOCK)->priorityFlags |= 0x80;
  return 0;
}

void JupiterLighthouse_LoadPalettes(s32 mode);
s32 MapGrid_GetCellHeight(s32 x, s32 z, s32 layer);

s32 Scene_PrepareMap(void) {
  struct Actor *object;
  s32 value;

  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(1);
  if (gMainRoutineWork.entrance == 12) {
    Actor_SetPosition(ACTOR_FIRST_GREY_BLOCK, 168, 264);
    Actor_SetPosition(ACTOR_FIRST_GREY_BLOCK + 1, 200, 264);
    Actor_SetPosition(ACTOR_FIRST_GREY_BLOCK + 2, 232, 264);
    Actor_SetPosition(ACTOR_FIRST_GREY_BLOCK + 3, 168, 296);
    Actor_SetPosition(ACTOR_FIRST_GREY_BLOCK + 4, 136, 328);
    Actor_SetPosition(ACTOR_LAST_GREY_BLOCK, 168, 328);
  }
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
      JupiterLighthouse_CountBlocksInGaps() != 2) {
    Event_ClearFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 0));
    Event_ClearFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 0));
    Event_ClearFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 0));
    Event_ClearFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_EAST, 1));
    Event_ClearFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_WEST, 1));
    Event_ClearFlag(FLAG_PATH_BLOCK_IN_GAP(PATH_GAP_NORTH, 1));
  }
  if (JupiterLighthouse_CountBlocksInGaps() == 2)
    Event_ClearFlag(FLAG_FIRST_CONE_SUNK);
  else
    Event_SetFlag(FLAG_FIRST_CONE_SUNK);
  if (Event_GetFlag(FLAG_SECOND_CONE_SUNK))
    Event_ClearFlag(FLAG_THIRD_BLOCK_IN_GAP);
  else
    Event_SetFlag(FLAG_THIRD_BLOCK_IN_GAP);
  Actor_Get(ACTOR_FIRST_PATH_BLOCK)->motionFlags = 0;
  Actor_Get(ACTOR_SECOND_PATH_BLOCK)->motionFlags = 0;
  Actor_Get(ACTOR_THIRD_PATH_BLOCK)->motionFlags = 0;
  JupiterLighthouse_DrawBlocksInGaps();
  if (!Event_GetFlag(FLAG_THIRD_BLOCK_IN_GAP)) {
    object = Actor_Get(ACTOR_THIRD_PATH_BLOCK);
    value = MapGrid_GetCellHeight(object->x.fixed, object->z.fixed, 0);
    MapGrid_SetCellHeight(object->x.fixed, object->z.fixed, 0, value + 2);
    MapGrid_SetAttributeBBitsAt(object->x.fixed, object->z.fixed, 0, 0x80);
  }
  JupiterLighthouse_LightWestPath(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}
extern u16 gPaletteCycleColors[];

#include "shared/palette_cycle_step.h"

extern u16 gLighthousePalette13[];
extern u16 gLighthouseUnlitColors[];

void MapGrid_SetAttributeBAt();

void JupiterLighthouse_LoadPalettes(s32 mode) {
  gPaletteCyclePausesInEvents = 1;
  if (mode != 2) {
    Iwram_CopyWordsEntry(gPaletteCycleColors, (void *)&BG_PLTT_COLOR(12, 0),
                         32);
    Iwram_CopyWordsEntry(gLighthousePalette13, (void *)&BG_PLTT_COLOR(13, 0),
                         32);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) || mode == 1) {
    PaletteCycle_SetFrame(0);
    Task_Add(PaletteCycle_Step, TASK_PASS_VBLANK);
  } else {
    Dma_Set(gLighthouseUnlitColors, (void *)&BG_PLTT_COLOR(12, 2),
            DMA_COPY_WORDS(2), REG_DMA3);
  }
}

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];
extern s32 gWhirlwindRiding;

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
FIXED Trig_Cos(s32 angle);
void Actor_StartScript(struct Actor *object, const s32 *script);

#include "shared/whirlwind_update.h"

struct CellTarget;
struct CellTargetEntry;
struct LightOam;

void Whirlwind_CarryLeader(void);

void JupiterLighthouse_OpenDoor(s32 value);

void JupiterLighthouse_CloseDoor(void);

void JupiterLighthouse_CheckBlockOnCell(void);

void JupiterLighthouse_StepOffButton(void);

void JupiterLighthouse_MoveButtonPillar(void);

void JupiterLighthouse_StartPound(void);
void JupiterLighthouse_RaiseBlocksFromGaps(void);

void JupiterLighthouse_RaiseThirdBlockFromGap(void);

void JupiterLighthouse_JumpOrPushBlock(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

void Actor_SetCellOpen(s32 id, u32 mode, s32 flag, s32 layer);

s32 Actor_GetCellIndex(struct Actor *object);

extern s16 gLightSpriteTile;
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritesLarge;

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

void CrackedTile_Fall(struct CellTarget *table, struct CellTargetEntry *entry);

extern s16 gLightSpriteCount;
void MapGrid_SetCellHeight(s32 x, s32 z, s32 a, s32 b);
void MapGrid_ClearAttributeBBitsAt(s32 x, s32 z, s32 a, s32 b);
void Field_TryJumpForward(void);

void Actor_SetSpriteFlags(struct Actor *object, u32 value);
void Whirlwind_Update(struct Actor *self);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_rising_sparks.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/actors_set_cells_open.h"

/// Finds the current target's table row and shrinks it.
void Pound_SinkTarget(u16 *list, s32 mode, s32 count) {
  struct GabombaStatueEffectWork *work = Ram_HeapSlots->fieldEffectWork;

  if (*list == 0xffff)
    return;
  do {
    {
      u32 entry = *list;
      s32 id = work->objectId;

      if (id == entry) {
        struct Actor *object = Actor_Get(id);
        s32 a;
        s32 flag;

        list++;
        a = list[0];
        flag = list[1];
        if (count < 8)
          object->scaleY -= FIXED_ONE * 31 / 256;
        if (mode == 1) {
          Event_SetFlag(flag);
          Actor_SetCellOpen(id, a, flag, object->layer);
          object->scaleY = FIXED_ONE;
        }
        return;
      }
    }
    list += 3;
  } while (*list != 0xffff);
}

#include "shared/actor_set_cell_open.h"

extern u32 gLightSpriteActors[];
extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add_counted.h"

void LightSprites_SetPriority(u16 value) { gLightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *object, u32 value);

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

// The cells CrackedTiles_Step watches, and the cell it copies over the one the
// leader steps on.
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tiles_find.h"

#include "shared/leader_center_in_cell.h"

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/cracked_tile_drop_in.h"

extern struct CellTarget gCrackedTiles;

void GameFlag_SetByte(s32 flag, s32 value);

#include "shared/cracked_tiles_step.h"

#include "shared/cracked_tile_place_landing.h"

// clang-format off
s16 gJupiterLighthouse_WestPathCells[][2] = {
    {8, 16}, {10, 16}, {11, 16}, {12, 16}, {14, 16}, {15, 16}, {10, 18},
    {12, 18}, {13, 18}, {8, 20},  {10, 20}, {13, 20}, {15, 20}, {-1, 0},
};
// clang-format on

void JupiterLighthouse_FightWhorl(void);
void JupiterLighthouse_StepOnButton(void);
void JupiterLighthouse_JumpForward(void);

// clang-format off
LATE_DATA u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

LATE_DATA u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

LATE_DATA u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

LATE_DATA s16 gJupiterLighthouse_WhorlPoundTargets[] = {
    ACTOR_FIRST_CONE, 1, FLAG_FIRST_CONE_SUNK, ACTOR_SECOND_CONE, 1, FLAG_SECOND_CONE_SUNK, -1, 0,
};

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {                                                        
      SCENE_EXITS(JupiterLighthouseWhorl),         SCENE_EXIT(JUPITER_LIGHTHOUSE_WHORL_TO_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST, JupiterLighthouseRoomPhaetonsBladeChest, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_FROM_JUPITER_LIGHTHOUSE_WHORL),   SCENE_EXIT(2, JupiterLighthouseWhorl, 3),   
      SCENE_EXIT(3, JupiterLighthouseWhorl, 2),    SCENE_EXIT(4, JupiterLighthouseWhorl, 5),   SCENE_EXIT(5, JupiterLighthouseWhorl, 4),   
      SCENE_EXIT(6, JupiterLighthouseWhorl, 7),   SCENE_EXIT(7, JupiterLighthouseWhorl, 6),  SCENE_EXIT(JUPITER_LIGHTHOUSE_WHORL_TO_JUPITER_LIGHTHOUSE_ROOM_M, JupiterLighthouseRoomM, JUPITER_LIGHTHOUSE_ROOM_M_FROM_JUPITER_LIGHTHOUSE_WHORL),   
      SCENE_EXIT(9, JupiterLighthouseWhorl, 10),   SCENE_EXIT(10, JupiterLighthouseWhorl, 9), SCENE_EXIT(11, JupiterLighthouseWhorl, 12), 
      SCENE_EXIT(12, JupiterLighthouseWhorl, 11), SCENE_EXITS_END,                                
  };

LATE_DATA struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(340, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 564, 0, 712, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 216, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 264, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 264, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(312, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 136, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(428, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(428, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(428, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_WHORL), MOVEMENT_TYPE_STAND, 440, 0, 216, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_TRIGGER_WALKING, 30, IF_FLAG_SET(FLAG_THIRD_BLOCK_IN_GAP), EVENT(JupiterLighthouse_JumpForward)},
    {TRIGGER_STEP, 20, FLAG_BUTTON_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOnButton)},
    {TRIGGER_STEP, 21, FLAG_BUTTON_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOffButton)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 40, CONDITION_ALWAYS, EVENT(JupiterLighthouse_JumpOrPushBlock)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 41, CONDITION_ALWAYS, EVENT(JupiterLighthouse_JumpOrPushBlock)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 42, CONDITION_ALWAYS, EVENT(JupiterLighthouse_JumpOrPushBlock)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 43, CONDITION_ALWAYS, EVENT(JupiterLighthouse_JumpOrPushBlock)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_WHORL, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FightWhorl)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePathBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SECOND_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePathBlock)},
    {TRIGGER_PUSH, 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPathBlock)},
    {TRIGGER_PUSHED, 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePathBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_THIRD_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_GREY_BLOCK, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_GREY_BLOCK + 1, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_GREY_BLOCK + 2, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_GREY_BLOCK + 3, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_GREY_BLOCK + 4, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LAST_GREY_BLOCK, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BUTTON_PILLAR, FLAG_BUTTON_PILLAR_PLACED, EVENT(JupiterLighthouse_MoveButtonPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_FIRST), ACTOR_FIRST_CONE, FLAG_FIRST_CONE_SUNK, EVENT(JupiterLighthouse_StartPound)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_FIRST_CONE, FLAG_FIRST_CONE_SUNK, EVENT(JupiterLighthouse_SinkPoundTarget)},
    {PSYNERGY_ON_ACTOR(ABILITY_POUND), ACTOR_FIRST_CONE, FLAG_FIRST_CONE_SUNK, EVENT(JupiterLighthouse_RaiseBlocksFromGaps)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_SECOND_CONE, FLAG_SECOND_CONE_SUNK, EVENT(JupiterLighthouse_SinkPoundTarget)},
    {PSYNERGY_ON_ACTOR(ABILITY_POUND), ACTOR_SECOND_CONE, FLAG_SECOND_CONE_SUNK, EVENT(JupiterLighthouse_RaiseThirdBlockFromGap)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s32 gPaletteCycleFrame = -1;

LATE_DATA s32 gPaletteCyclePausesInEvents = 1;

LATE_DATA s32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
