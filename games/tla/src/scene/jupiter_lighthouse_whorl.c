/// @file
/// The great hall of Jupiter Lighthouse's right tower, behind its archer
/// statue, where the Jupiter Djinni Whorl waits on a ledge
/// (JupiterLighthouse_FightWhorl). Two path blocks, one carrying a straight
/// piece of the floor's path and one a corner, pushed or Moved into two of the
/// three gaps in the floor of the east hall (JupiterLighthouse_PlacePathBlock),
/// light the path and raise a stake; Pounding it throws the blocks back out of
/// their gaps (JupiterLighthouse_RaiseBlocksFromGaps). In the west hall a third
/// block in its gap lights the path the grey blocks stand on, and the west
/// stake Pounded throws it out. The pillar pushed onto the button by the pit
/// raises the bridge across it for good, a shortcut.
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

/// Where the first two path blocks lie, each block in each of the east hall's
/// three gaps: the block's flag after FLAG_PATH_BLOCK_PLACED(0), and what
/// JupiterLighthouse_FindBlockGap returns.
enum PathBlockPlace {
  STRAIGHT_BLOCK_IN_EAST_GAP,
  CORNER_BLOCK_IN_EAST_GAP,
  STRAIGHT_BLOCK_IN_WEST_GAP,
  CORNER_BLOCK_IN_WEST_GAP,
  STRAIGHT_BLOCK_IN_NORTH_GAP,
  CORNER_BLOCK_IN_NORTH_GAP,
};

/// A path block lies in a gap of the east hall, a flag for each PathBlockPlace.
#define FLAG_PATH_BLOCK_PLACED(place) (0xa10 + (place))
/// The east stake is down in the floor: Pounded, or not yet raised by the two
/// blocks both lying in gaps.
#define FLAG_EAST_STAKE_SUNK 0xa16
/// The west stake is down in the floor, Pounded; the third block is then out
/// of its gap.
#define FLAG_WEST_STAKE_SUNK 0xa17
/// The third path block lies in its gap below the west hall's north door.
#define FLAG_THIRD_BLOCK_IN_GAP 0xa1a
/// The pillar stands on the button by the pit: the bridge stays up.
#define FLAG_BUTTON_PILLAR_PLACED 0xa1b
/// Set while the leader stands on the button, which holds the bridge up.
#define FLAG_BRIDGE_BUTTON_HELD FLAG_SCENE_TEMP_02

/// The east hall's three gaps, by cell.
#define EAST_GAP_CELL_X 35
#define EAST_GAP_CELL_Z 15
#define WEST_GAP_CELL_X 31
#define WEST_GAP_CELL_Z 16
#define NORTH_GAP_CELL_X 33
#define NORTH_GAP_CELL_Z 10
/// The third block's gap, by cell.
#define THIRD_GAP_CELL_X 8
#define THIRD_GAP_CELL_Z 12
/// The button by the pit, by cell.
#define BUTTON_CELL_X 8
#define BUTTON_CELL_Z 37
/// A gap's floor, 16 pixels below the hall's, where a thrown block starts.
#define GAP_FLOOR_HEIGHT toFIXED(-16)
/// The most frames the room waits for a block to come down.
#define BLOCK_LANDING_FRAMES 30
/// The way in from the bridge's floor, through its west arch.
#define ENTRANCE_FROM_BRIDGE_FLOOR 12

enum JupiterLighthouseWhorlActorId {
  ACTOR_ARCHER_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_WEST_STAKE,
  /// The six grey blocks; those on the west hall's path rise while it is lit.
  ACTOR_FIRST_GREY_BLOCK,
  ACTOR_LAST_GREY_BLOCK = ACTOR_FIRST_GREY_BLOCK + 5,
  ACTOR_BUTTON_PILLAR,
  ACTOR_EAST_STAKE,
  ACTOR_STRAIGHT_PATH_BLOCK,
  ACTOR_CORNER_PATH_BLOCK,
  ACTOR_THIRD_PATH_BLOCK,
  ACTOR_WHORL
};

extern s16 gJupiterLighthouse_WhorlPoundTargets[];
extern s32 gPaletteCyclePausesInEvents;
s32 gWhirlwindRiding BSS_DATA;
u16 gPaletteCycleColors[16] BSS_DATA;
/// Palette 13's first two colours; it is copied whole, with the unlit colours
/// after them.
u16 gPaletteCycleUnlitPalette[2] BSS_DATA;
u16 gPaletteCycleUnlitColors[14] BSS_DATA;
s16 gLightSpriteTile BSS_DATA;
s16 gLightSpriteCount BSS_DATA;
u32 gLightSpriteActors[16] BSS_DATA;
struct LightOam gLightSpriteOams[16] BSS_DATA;
s16 gLightSpritesLarge BSS_DATA;
s16 gLightSpritePriority BSS_DATA;
struct CellTarget gCrackedTiles BSS_DATA;
void JupiterLighthouse_RaiseBridge(s32 mode);
void JupiterLighthouse_LowerBridge(void);
void JupiterLighthouse_CheckPillarOnButton(void);
void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height);
void MapGrid_ClearAttributeBBitsAt(s32 x, s32 z, s32 layer, s32 bits);
void Field_TryJumpForward(void);

/// Talking to Whorl: the Djinni fights the party.
void JupiterLighthouse_FightWhorl(void) {
  Field_RequestDjinnBattle(ACTOR_WHORL, DJINNI_WHORL);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Swings the bridge's two halves up out of the pit to lie across it; in mode
/// 1 with its sound and a frame halfway up.
void JupiterLighthouse_RaiseBridge(s32 mode) {
  if (mode == 1) {
    Event_PlaySound(SE_IMPACT);
    MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(81, 42, 4, 3, 76, 35);
  MapGrid_CopyCellAttributes(12, 41, 4, 1, 12, 35);
}

/// Lets the bridge's two halves swing back down into the pit.
void JupiterLighthouse_LowerBridge(void) {
  Event_PlaySound(SE_HEAVY_IMPACT);
  MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
  System_Wait(6);
  MapGrid_CopyCellsLayered(71, 42, 4, 3, 76, 35);
  MapGrid_CopyCellAttributes(12, 36, 4, 1, 12, 35);
}

/// Raises the bridge for good once the pillar stands on the button.
void JupiterLighthouse_CheckPillarOnButton(void) {
  struct Actor *actor = Actor_Get(ACTOR_BUTTON_PILLAR);

  if (!Event_GetFlag(FLAG_BUTTON_PILLAR_PLACED) &&
      FIELD_CELL(actor->x.fixed) == BUTTON_CELL_X &&
      FIELD_CELL(actor->z.fixed) == BUTTON_CELL_Z) {
    Event_SetFlag(FLAG_BUTTON_PILLAR_PLACED);
    JupiterLighthouse_RaiseBridge(1);
  }
}

/// The leader steps onto the button: the bridge rises, held up while the
/// leader stands there.
void JupiterLighthouse_StepOnButton(void) {
  if (!Event_GetFlag(FLAG_BRIDGE_BUTTON_HELD)) {
    Event_SetFlag(FLAG_BRIDGE_BUTTON_HELD);
    JupiterLighthouse_RaiseBridge(1);
  }
}

/// The leader steps off the button: the bridge the button held up falls
/// again, unless the pillar now stands on the button.
void JupiterLighthouse_StepOffButton(void) {
  if (Event_GetFlag(FLAG_BRIDGE_BUTTON_HELD) == 1) {
    Event_ClearFlag(FLAG_BRIDGE_BUTTON_HELD);
    JupiterLighthouse_LowerBridge();
  }
  JupiterLighthouse_CheckPillarOnButton();
}

/// After the pillar is pushed or Moved: once it settles, checks whether it
/// stands on the button.
void JupiterLighthouse_MoveButtonPillar(void) {
  System_Wait(10);
  JupiterLighthouse_CheckPillarOnButton();
}

/// Marks the gap the first two blocks have just come to rest in; returns
/// where the block lies, or -1 for no gap.
s32 JupiterLighthouse_FindBlockGap(void) {
  s32 x, z, place;
  struct Actor *corner;
  struct Actor *straight;

  straight = Actor_Get(ACTOR_STRAIGHT_PATH_BLOCK);
  corner = Actor_Get(ACTOR_CORNER_PATH_BLOCK);
  place = 1;
  x = FIELD_CELL(straight->x.fixed);
  place = -place;
  z = FIELD_CELL(straight->z.fixed);
  if (x == EAST_GAP_CELL_X && z == EAST_GAP_CELL_Z &&
      !Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP))) {
    Event_SetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP));
    place = STRAIGHT_BLOCK_IN_EAST_GAP;
  }
  if (x == WEST_GAP_CELL_X && z == WEST_GAP_CELL_Z &&
      !Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP))) {
    Event_SetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP));
    place = STRAIGHT_BLOCK_IN_WEST_GAP;
  }
  if (x == NORTH_GAP_CELL_X && z == NORTH_GAP_CELL_Z &&
      !Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP))) {
    Event_SetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP));
    place = STRAIGHT_BLOCK_IN_NORTH_GAP;
  }
  x = FIELD_CELL(corner->x.fixed);
  z = FIELD_CELL(corner->z.fixed);
  if (x == EAST_GAP_CELL_X && z == EAST_GAP_CELL_Z &&
      !Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP))) {
    Event_SetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP));
    place = CORNER_BLOCK_IN_EAST_GAP;
  }
  if (x == WEST_GAP_CELL_X && z == WEST_GAP_CELL_Z &&
      !Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP))) {
    Event_SetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP));
    place = CORNER_BLOCK_IN_WEST_GAP;
  }
  if (x == NORTH_GAP_CELL_X && z == NORTH_GAP_CELL_Z &&
      !Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP))) {
    Event_SetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP));
    place = CORNER_BLOCK_IN_NORTH_GAP;
  }
  return place;
}

/// How many of the first two blocks lie in gaps.
s32 JupiterLighthouse_CountBlocksInGaps(void) {
  s32 count = 0;

  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)))
    count = 1;
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)))
    count++;
  return count;
}

/// Draws the east hall's floor for the blocks that lie in gaps: each filled
/// gap, the path lit between them, and the Hover tiles that path reaches.
void JupiterLighthouse_DrawBlocksInGaps(void) {
  // Every gap empty, two height steps deep, and no Hover tile.
  MapGrid_CopyCellsLayered(94, 60, 9, 8, 94, 10);
  Call4((void (*)())MapGrid_SetCellHeight, toFIXED(560), toFIXED(240), 0, -2);
  Call4((void (*)())MapGrid_SetCellHeight, toFIXED(496), toFIXED(256), 0, -2);
  Call4((void (*)())MapGrid_SetCellHeight, toFIXED(528), toFIXED(160), 0, -2);
  FieldMapGrid_SetCellBehavior(toFIXED(608), toFIXED(160), 2, 0);
  FieldMapGrid_SetCellBehavior(toFIXED(496), toFIXED(160), 2, 0);
  FieldMapGrid_SetCellBehavior(toFIXED(496), toFIXED(224), 2, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP)))
    MapGrid_SetCellHeight(toFIXED(560), toFIXED(240), 0, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP)))
    MapGrid_SetCellHeight(toFIXED(496), toFIXED(256), 0, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)))
    MapGrid_SetCellHeight(toFIXED(528), toFIXED(160), 0, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP))) {
    MapGrid_CopyCellsLayered(96, 56, 7, 2, 96, 15);
    MapGrid_CopyCellsLayered(111, 51, 1, 1, 99, 15);
    if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP))) {
      MapGrid_CopyCellsLayered(105, 51, 1, 3, 95, 14);
      FieldMapGrid_SetCellBehavior(toFIXED(496), toFIXED(224), 2,
                                   CELL_BEHAVIOR_HOVER);
    }
    if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)))
      MapGrid_CopyCellsLayered(111, 53, 1, 1, 97, 10);
  } else if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP))) {
    MapGrid_CopyCellsLayered(98, 51, 5, 1, 98, 10);
    MapGrid_CopyCellsLayered(99, 52, 4, 5, 99, 11);
    FieldMapGrid_SetCellBehavior(toFIXED(608), toFIXED(160), 2,
                                 CELL_BEHAVIOR_HOVER);
    if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)))
      MapGrid_CopyCellsLayered(106, 53, 1, 1, 95, 16);
    if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP))) {
      MapGrid_CopyCellsLayered(109, 51, 3, 1, 95, 10);
      FieldMapGrid_SetCellBehavior(toFIXED(496), toFIXED(160), 2,
                                   CELL_BEHAVIOR_HOVER);
    }
  } else {
    if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)))
      MapGrid_CopyCellsLayered(106, 53, 1, 1, 95, 16);
    if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP)))
      MapGrid_CopyCellsLayered(107, 53, 1, 1, 95, 16);
    if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)))
      MapGrid_CopyCellsLayered(111, 52, 1, 1, 97, 10);
    if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)))
      MapGrid_CopyCellsLayered(111, 53, 1, 1, 97, 10);
  }
}

/// Whether the actor stands on one of the cells of the west hall's path.
s32 JupiterLighthouse_IsOnWestPath(struct Actor *actor) {
  s32 i;

  for (i = 0; gJupiterLighthouse_WestPathCells[i][0] != -1; i++) {
    if (gJupiterLighthouse_WestPathCells[i][0] == FIELD_CELL(actor->x.fixed) &&
        gJupiterLighthouse_WestPathCells[i][1] == FIELD_CELL(actor->z.fixed))
      return 1;
  }
  return 0;
}

/// Lights or darkens the west hall's path as the third block lies in its gap
/// or not, raising the grey blocks that stand on it or letting them down
/// (gradually in mode 1).
void JupiterLighthouse_LightWestPath(s32 mode) {
  struct Actor *actor;
  s32 i, frame;

  if (!Event_GetFlag(FLAG_THIRD_BLOCK_IN_GAP)) {
    MapGrid_CopyCellsLayered(80, 46, 1, 7, 72, 9);
    MapGrid_CopyCellsLayered(80, 53, 8, 5, 72, 16);
    MapGrid_CopyCellsLayered(78, 47, 1, 1, 8, 74);
    MapGrid_SetCellHeight(toFIXED(128), toFIXED(192), 0, 4);
    if (mode == 1) {
      frame = 0;
      do {
        i = 0;
        do {
          actor = Actor_Get(ACTOR_FIRST_GREY_BLOCK + i);
          if (JupiterLighthouse_IsOnWestPath(actor)) {
            actor->motionFlags = 0;
            actor->y.fixed -= toFIXED(2);
          }
        } while (++i <= 5);
        System_Wait(1);
        frame++;
      } while (frame <= 15);
    }
    i = 0;
    do {
      actor = Actor_Get(ACTOR_FIRST_GREY_BLOCK + i);
      actor->motionFlags = 3;
      i++;
      actor->y.fixed = actor->groundHeight;
    } while (i <= 5);
  } else {
    MapGrid_CopyCellsLayered(72, 46, 1, 7, 72, 9);
    MapGrid_CopyCellsLayered(72, 53, 8, 5, 72, 16);
    MapGrid_CopyCellsLayered(70, 47, 1, 1, 8, 74);
    MapGrid_SetCellHeight(toFIXED(128), toFIXED(192), 0, 6);
    FieldActor_Stop(gMainRoutineWork.leaderActorId);
    Actor_SetPosition(ACTOR_THIRD_PATH_BLOCK, 0, 0);
    if (mode == 1) {
      frame = 0;
      do {
        i = 0;
        do {
          actor = Actor_Get(ACTOR_FIRST_GREY_BLOCK + i);
          if (JupiterLighthouse_IsOnWestPath(actor)) {
            actor->motionFlags = 0;
            actor->y.fixed += toFIXED(2);
          }
        } while (++i <= 5);
        System_Wait(1);
        frame++;
      } while (frame <= 15);
    }
    i = 0;
    do {
      actor = Actor_Get(ACTOR_FIRST_GREY_BLOCK + i);
      if (JupiterLighthouse_IsOnWestPath(actor)) {
        actor->motionFlags = 4;
        actor->y.fixed = toFIXED(65) / 2;
      }
    } while (++i <= 5);
  }
}

/// As a push or Move takes up a block: the third block's cell goes back to the
/// west hall's floor, six height steps up, and is no longer a cell to jump.
void JupiterLighthouse_LiftPathBlock(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);

  if (id == ACTOR_THIRD_PATH_BLOCK) {
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, 6);
    MapGrid_ClearAttributeBBitsAt(actor->x.fixed, actor->z.fixed, 0,
                                  MAP_CELL_JUMP);
  }
}

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void MapGrid_SetAttributeBBitsAt(s32 x, s32 z, s32 layer, s32 bits);

/// A path block set down after a push or Move: it falls into place, and in a
/// gap it lights the path there. Once both of the first two lie in gaps the
/// east stake rises; the third, in its gap, lights the west hall's path and
/// raises the west stake. Set down anywhere else, the third block's cell stands
/// its 16 pixels higher, a cell to jump onto.
void JupiterLighthouse_PlacePathBlock(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 place;
  s32 i;

  if (actor->groundHeight !=
      Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed)) {
    actor->motionFlags = 3;
    wait(2);
    i = 0;
    if (actor->y.fixed > actor->groundHeight || actor->velocityY != 0) {
      do {
        wait(1);
        i++;
        if (i >= BLOCK_LANDING_FRAMES)
          break;
      } while (actor->y.fixed > actor->groundHeight || actor->velocityY != 0);
    }
    Event_PlaySound(SE_THUMP);
    actor->motionFlags = 0;
  }
  if ((u32)(id - ACTOR_STRAIGHT_PATH_BLOCK) <= 1) {
    place = JupiterLighthouse_FindBlockGap();
    if (place == -1)
      return;
    Actor_SetSpriteFlags(Actor_GetById(id), 0);
    Event_Wait(30);
    JupiterLighthouse_DrawBlocksInGaps();
    // A sound as the lit path reaches the block.
    if (place == STRAIGHT_BLOCK_IN_EAST_GAP)
      Event_PlaySound(SE_MAP_TRANSITION);
    if (place == CORNER_BLOCK_IN_EAST_GAP)
      Event_PlaySound(SE_MAP_TRANSITION);
    if (place == CORNER_BLOCK_IN_WEST_GAP &&
        Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)))
      Event_PlaySound(SE_MAP_TRANSITION);
    if (place == STRAIGHT_BLOCK_IN_NORTH_GAP &&
        Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP)))
      Event_PlaySound(SE_MAP_TRANSITION);
    Actor_Stop(gMainRoutineWork.leaderActorId);
    Actor_SetPosition(id, 0, 0);
    if (JupiterLighthouse_CountBlocksInGaps() != 2 ||
        !Event_GetFlag(FLAG_EAST_STAKE_SUNK))
      return;
    actor = Actor_GetById(ACTOR_EAST_STAKE);
    Event_ClearFlag(FLAG_EAST_STAKE_SUNK);
    Event_Wait(10);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldCamera_MoveTo(664, CAMERA_KEEP, 136, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(20);
    Event_PlaySound(SE_LIGHT_APPEAR);
    Actor_SelectAnimation(actor, 3);
    actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    FieldMapGrid_SetCellBehavior(actor->x.fixed, actor->z.fixed, actor->layer,
                                 CELL_BEHAVIOR_WALL);
    Event_Wait(30);
    EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
    FieldCamera_WaitForArrival();
    Event_End();
    return;
  }
  if (id != ACTOR_THIRD_PATH_BLOCK)
    return;
  if (FIELD_CELL(actor->x.fixed) == THIRD_GAP_CELL_X &&
      FIELD_CELL(actor->z.fixed) == THIRD_GAP_CELL_Z) {
    Event_SetFlag(FLAG_THIRD_BLOCK_IN_GAP);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_THIRD_PATH_BLOCK), 0);
    Event_Wait(30);
    Event_PlaySound(SE_MAP_TRANSITION);
    JupiterLighthouse_LightWestPath(1);
    if (!Event_GetFlag(FLAG_WEST_STAKE_SUNK))
      return;
    actor = Actor_GetById(ACTOR_WEST_STAKE);
    Event_ClearFlag(FLAG_WEST_STAKE_SUNK);
    Event_Wait(20);
    Event_PlaySound(SE_LIGHT_APPEAR);
    Actor_SelectAnimation(actor, 3);
    actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    FieldMapGrid_SetCellBehavior(actor->x.fixed, actor->z.fixed, actor->layer,
                                 CELL_BEHAVIOR_WALL);
  } else {
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, 8);
    MapGrid_SetAttributeBBitsAt(actor->x.fixed, actor->z.fixed, 0,
                                MAP_CELL_JUMP);
  }
}

/// As Pound is cast: ends what the party was doing.
void JupiterLighthouse_StartPound(void) { Event_ResolvePendingActions(0); }

/// Pound on one of the room's stakes: sinks it into the floor.
void JupiterLighthouse_SinkPoundTarget(s32 mode, s32 frame) {
  Pound_SinkTarget(gJupiterLighthouse_WhorlPoundTargets, mode, frame);
}

/// Throws a block up out of its gap at cell (x, z) to land beside it.
void JupiterLighthouse_ThrowOutBlock(s32 id, s32 x, s32 z, s32 dx, s32 dz) {
  struct Actor *actor = Actor_Get(id);

  Actor_SetPositionFX(id, FIELD_CELL_CENTER(x), FIELD_CELL_CENTER(z));
  actor->motionFlags = 3;
  actor->groundHeight = GAP_FLOOR_HEIGHT;
  actor->y.fixed = GAP_FLOOR_HEIGHT;
  actor->velocityY = toFIXED(8);
  Actor_SetSpeed(id, 1.6);
  Actor_SetDestinationOffset(id, dx, dz);
  Event_PlaySound(SE_WHIRLWIND);
}

/// The east stake Pounded down throws the two blocks back out of their gaps,
/// the camera watching.
void JupiterLighthouse_RaiseBlocksFromGaps(void) {
  struct Actor *actor;
  s32 i;

  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)) +
          Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)) +
          Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)) +
          Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP)) +
          Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP)) +
          Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)) ==
      0)
    return;
  FieldCamera_MoveTo(592, CAMERA_KEEP, 248, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)))
    JupiterLighthouse_ThrowOutBlock(ACTOR_STRAIGHT_PATH_BLOCK, EAST_GAP_CELL_X,
                                    EAST_GAP_CELL_Z, -16, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP)))
    JupiterLighthouse_ThrowOutBlock(ACTOR_CORNER_PATH_BLOCK, EAST_GAP_CELL_X,
                                    EAST_GAP_CELL_Z, -16, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)))
    JupiterLighthouse_ThrowOutBlock(ACTOR_STRAIGHT_PATH_BLOCK, WEST_GAP_CELL_X,
                                    WEST_GAP_CELL_Z, 16, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP)))
    JupiterLighthouse_ThrowOutBlock(ACTOR_CORNER_PATH_BLOCK, WEST_GAP_CELL_X,
                                    WEST_GAP_CELL_Z, 16, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)))
    JupiterLighthouse_ThrowOutBlock(ACTOR_STRAIGHT_PATH_BLOCK, NORTH_GAP_CELL_X,
                                    NORTH_GAP_CELL_Z, -16, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)))
    JupiterLighthouse_ThrowOutBlock(ACTOR_CORNER_PATH_BLOCK, NORTH_GAP_CELL_X,
                                    NORTH_GAP_CELL_Z, -16, 0);
  Event_ClearFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP));
  Event_ClearFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP));
  Event_ClearFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP));
  Event_ClearFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP));
  Event_ClearFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP));
  Event_ClearFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP));
  JupiterLighthouse_DrawBlocksInGaps();
  System_Wait(2);
  actor = Actor_Get(ACTOR_STRAIGHT_PATH_BLOCK);
  i = 0;
  if (actor->y.fixed > actor->groundHeight || actor->velocityY != 0) {
    do {
      System_Wait(1);
      i++;
      if (i >= BLOCK_LANDING_FRAMES)
        break;
    } while (actor->y.fixed > actor->groundHeight || actor->velocityY != 0);
  }
  actor->motionFlags = 0;
  actor = Actor_Get(ACTOR_CORNER_PATH_BLOCK);
  i = 0;
  if (actor->y.fixed > actor->groundHeight || actor->velocityY != 0) {
    do {
      System_Wait(1);
      i++;
      if (i >= BLOCK_LANDING_FRAMES)
        break;
    } while (actor->y.fixed > actor->groundHeight || actor->velocityY != 0);
  }
  actor->motionFlags = 0;
  FieldEvent_Wait(30);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
}

/// The west stake Pounded down throws the third block back out of its gap.
void JupiterLighthouse_RaiseThirdBlockFromGap(void) {
  struct Actor *actor;
  s32 i;

  if (Event_GetFlag(FLAG_THIRD_BLOCK_IN_GAP))
    JupiterLighthouse_ThrowOutBlock(ACTOR_THIRD_PATH_BLOCK, THIRD_GAP_CELL_X,
                                    THIRD_GAP_CELL_Z, 16, 0);
  Event_ClearFlag(FLAG_THIRD_BLOCK_IN_GAP);
  JupiterLighthouse_LightWestPath(1);
  System_Wait(2);
  actor = Actor_Get(ACTOR_THIRD_PATH_BLOCK);
  for (i = 0; i < BLOCK_LANDING_FRAMES; i++) {
    if (actor->y.fixed <= actor->groundHeight && actor->velocityY == 0)
      break;
    System_Wait(1);
  }
  actor->motionFlags = 0;
}

/// Walking into a block on the floor pushes it; walking at a gap with no block
/// in it jumps across.
void JupiterLighthouse_JumpOrPushBlock(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *straight = Actor_Get(ACTOR_STRAIGHT_PATH_BLOCK);
  struct Actor *corner = Actor_Get(ACTOR_CORNER_PATH_BLOCK);
  FIXED position[3];
  s32 found = 0;
  s32 x;
  s32 z;
  s32 facing;

  facing = (leader->facing + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed;
  FieldVector_AddPolarOffset(16, facing, position);
  x = FIELD_CELL(position[0]);
  z = FIELD_CELL(position[2]);
  if (straight->y.fixed == 0 && x == FIELD_CELL(straight->x.fixed) &&
      z == FIELD_CELL(straight->z.fixed))
    found = 1;
  if (corner->y.fixed == 0 && x == FIELD_CELL(corner->x.fixed) &&
      z == FIELD_CELL(corner->z.fixed))
    found = 1;
  if (found == 0) {
    if (x == EAST_GAP_CELL_X && z == EAST_GAP_CELL_Z &&
        (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)) ||
         Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP))))
      found = 2;
    if (x == WEST_GAP_CELL_X && z == WEST_GAP_CELL_Z &&
        (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)) ||
         Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP))))
      found = 2;
    if (x == NORTH_GAP_CELL_X && z == NORTH_GAP_CELL_Z &&
        (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)) ||
         Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP))))
      found = 2;
    if (found == 0) {
      Field_TryJumpForward();
      return;
    }
  }
  if (found == 1)
    Leader_PushObjectAhead();
}

/// A jump spot's step: the leader jumps forward.
void JupiterLighthouse_JumpForward(void) { Field_TryJumpForward(); }

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

void Actors_SetCellsOpen();
void LightSprites_InitB();
void LightSprites_AddCounted();

/// Sets up the hall as the party enters: the archer statue at three quarters
/// of its size, the stakes and the bridge as their flags leave them, the
/// blocks' glow, and the grey blocks as platforms. Returns 0.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if ((u16)(gMainRoutineWork.entrance - 3) <= 1)
    Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(ACTOR_ARCHER_STATUE)->scaleX = FIXED_ONE * 3 / 4;
  Actor_Get(ACTOR_ARCHER_STATUE)->scaleY = FIXED_ONE * 3 / 4;
  Actors_SetCellsOpen((s32)&gJupiterLighthouse_WhorlPoundTargets);
  if (Event_GetFlag(FLAG_BUTTON_PILLAR_PLACED)) {
    Actor_SetPosition(ACTOR_BUTTON_PILLAR, 136, 600);
    JupiterLighthouse_RaiseBridge(0);
  }
  if (Event_GetFlag(FLAG_BRIDGE_BUTTON_HELD)) {
    JupiterLighthouse_RaiseBridge(0);
  }
  LightSprites_InitB();
  LightSprites_SetPriority(2);
  LightSprites_AddCounted(ACTOR_STRAIGHT_PATH_BLOCK, 2);
  LightSprites_AddCounted(ACTOR_CORNER_PATH_BLOCK, 1);
  LightSprites_AddCounted(ACTOR_THIRD_PATH_BLOCK, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)))
    Actor_SetPosition(ACTOR_STRAIGHT_PATH_BLOCK, 0, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)))
    Actor_SetPosition(ACTOR_CORNER_PATH_BLOCK, 0, 0);
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  Actor_Get(ACTOR_FIRST_GREY_BLOCK)->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_FIRST_GREY_BLOCK + 1)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_FIRST_GREY_BLOCK + 2)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_FIRST_GREY_BLOCK + 3)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_FIRST_GREY_BLOCK + 4)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_LAST_GREY_BLOCK)->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  return 0;
}

void JupiterLighthouse_LoadPalettes(s32 mode);
s32 MapGrid_GetCellHeight(FIXED x, FIXED z, s32 layer);

/// Lays out the blocks as the map is drawn: the first two stay in their gaps
/// only when both lie in them or the party comes back to where it left, the
/// third lies in its gap while the west stake stands, and the floor and the
/// west hall's path follow. Returns 0.
s32 Scene_PrepareMap(void) {
  struct Actor *actor;
  s32 value;

  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(1);
  // Arriving from the bridge's floor, the grey blocks stand afresh on six of
  // the west hall's path cells.
  if (gMainRoutineWork.entrance == ENTRANCE_FROM_BRIDGE_FLOOR) {
    Actor_SetPosition(ACTOR_FIRST_GREY_BLOCK, 168, 264);
    Actor_SetPosition(ACTOR_FIRST_GREY_BLOCK + 1, 200, 264);
    Actor_SetPosition(ACTOR_FIRST_GREY_BLOCK + 2, 232, 264);
    Actor_SetPosition(ACTOR_FIRST_GREY_BLOCK + 3, 168, 296);
    Actor_SetPosition(ACTOR_FIRST_GREY_BLOCK + 4, 136, 328);
    Actor_SetPosition(ACTOR_LAST_GREY_BLOCK, 168, 328);
  }
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
      JupiterLighthouse_CountBlocksInGaps() != 2) {
    Event_ClearFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP));
    Event_ClearFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP));
    Event_ClearFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP));
    Event_ClearFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP));
    Event_ClearFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP));
    Event_ClearFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP));
  }
  if (JupiterLighthouse_CountBlocksInGaps() == 2)
    Event_ClearFlag(FLAG_EAST_STAKE_SUNK);
  else
    Event_SetFlag(FLAG_EAST_STAKE_SUNK);
  if (Event_GetFlag(FLAG_WEST_STAKE_SUNK))
    Event_ClearFlag(FLAG_THIRD_BLOCK_IN_GAP);
  else
    Event_SetFlag(FLAG_THIRD_BLOCK_IN_GAP);
  Actor_Get(ACTOR_STRAIGHT_PATH_BLOCK)->motionFlags = 0;
  Actor_Get(ACTOR_CORNER_PATH_BLOCK)->motionFlags = 0;
  Actor_Get(ACTOR_THIRD_PATH_BLOCK)->motionFlags = 0;
  JupiterLighthouse_DrawBlocksInGaps();
  if (!Event_GetFlag(FLAG_THIRD_BLOCK_IN_GAP)) {
    actor = Actor_Get(ACTOR_THIRD_PATH_BLOCK);
    value = MapGrid_GetCellHeight(actor->x.fixed, actor->z.fixed, 0);
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, value + 2);
    MapGrid_SetAttributeBBitsAt(actor->x.fixed, actor->z.fixed, 0,
                                MAP_CELL_JUMP);
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

#include "shared/palette_cycle_step.h"

void MapGrid_SetAttributeBAt();

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
FIXED Trig_Cos(s32 angle);
void Actor_StartScript(struct Actor *actor, const s32 *script);

#include "shared/whirlwind_update.h"

struct CellTarget;
struct CellTargetEntry;
struct LightOam;

void Whirlwind_CarryLeader(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

void Actor_SetCellOpen(s32 id, u32 mode, s32 flag, s32 layer);

s32 Actor_GetCellIndex(struct Actor *actor);

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

void CrackedTile_Fall(struct CellTarget *table, struct CellTargetEntry *entry);

void Actor_SetSpriteFlags(struct Actor *actor, u32 value);
void Whirlwind_Update(struct Actor *self);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_rising_sparks.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/actors_set_cells_open.h"

#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one with gLightSpritesLarge, at its animation picture
/// and depth.
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

void Actor_SetSpriteFlags(struct Actor *actor, u32 value);

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

// clang-format off
s16 gJupiterLighthouse_WestPathCells[][2] = {
    {8, 16}, {10, 16}, {11, 16}, {12, 16}, {14, 16}, {15, 16}, {10, 18},
    {12, 18}, {13, 18}, {8, 20},  {10, 20}, {13, 20}, {15, 20}, {-1, 0},
};
// clang-format on

// clang-format off
#define FIELD_EFFECT_DURATION_SCRIPT_SECTION LATE_DATA
#include "shared/field_effect_duration_scripts.c"

LATE_DATA s16 gJupiterLighthouse_WhorlPoundTargets[] = {
    ACTOR_EAST_STAKE, 1, FLAG_EAST_STAKE_SUNK, ACTOR_WEST_STAKE, 1, FLAG_WEST_STAKE_SUNK, -1, 0,
};

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

LATE_DATA u32 gSceneExits[] = {                                                        
      SCENE_EXITS(JupiterLighthouseWhorl),         SCENE_EXIT(JUPITER_LIGHTHOUSE_WHORL_TO_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST, JupiterLighthouseRoomPhaetonsBladeChest, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_FROM_JUPITER_LIGHTHOUSE_WHORL),   SCENE_EXIT(2, JupiterLighthouseWhorl, 3),   
      SCENE_EXIT(3, JupiterLighthouseWhorl, 2),    SCENE_EXIT(4, JupiterLighthouseWhorl, 5),   SCENE_EXIT(5, JupiterLighthouseWhorl, 4),   
      SCENE_EXIT(6, JupiterLighthouseWhorl, 7),   SCENE_EXIT(7, JupiterLighthouseWhorl, 6),  SCENE_EXIT(JUPITER_LIGHTHOUSE_WHORL_TO_JUPITER_LIGHTHOUSE_ROOM_M, JupiterLighthouseRoomM, JUPITER_LIGHTHOUSE_ROOM_M_FROM_JUPITER_LIGHTHOUSE_WHORL),   
      SCENE_EXIT(9, JupiterLighthouseWhorl, 10),   SCENE_EXIT(10, JupiterLighthouseWhorl, 9), SCENE_EXIT(11, JupiterLighthouseWhorl, 12), 
      SCENE_EXIT(12, JupiterLighthouseWhorl, 11), SCENE_EXITS_END,                                
  };

LATE_DATA struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ARCHER_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 564, 0, 712, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 216, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 264, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 264, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 376, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 136, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_STRAIGHT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STRAIGHT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePathBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CORNER_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CORNER_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePathBlock)},
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
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_FIRST), ACTOR_EAST_STAKE, FLAG_EAST_STAKE_SUNK, EVENT(JupiterLighthouse_StartPound)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_EAST_STAKE, FLAG_EAST_STAKE_SUNK, EVENT(JupiterLighthouse_SinkPoundTarget)},
    {PSYNERGY_ON_ACTOR(ABILITY_POUND), ACTOR_EAST_STAKE, FLAG_EAST_STAKE_SUNK, EVENT(JupiterLighthouse_RaiseBlocksFromGaps)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_WEST_STAKE, FLAG_WEST_STAKE_SUNK, EVENT(JupiterLighthouse_SinkPoundTarget)},
    {PSYNERGY_ON_ACTOR(ABILITY_POUND), ACTOR_WEST_STAKE, FLAG_WEST_STAKE_SUNK, EVENT(JupiterLighthouse_RaiseThirdBlockFromGap)},
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
