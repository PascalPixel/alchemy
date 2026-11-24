/// @file
/// The lower rooms of Jupiter Lighthouse's right tower, past its stone archer
/// statue. In the big east hall the Jupiter Djinni Whorl waits on a ledge
/// (JupiterLighthouse_FightWhorl). Two energy path blocks, one carrying a
/// straight piece of the path and one a corner, pushed or Moved into two of the
/// three gaps in its floor (JupiterLighthouse_PlacePathBlock), light the path
/// to new Hover pads and raise a cone-shaped stake; Pound on the stake throws
/// the blocks back out of their gaps to try another layout
/// (JupiterLighthouse_RaiseBlocksFromGaps), and one layout reaches Whorl. In
/// the west hall a third block, pushed into the gap at cell (8, 12), lights the
/// path under six grey blocks, which rise so that Felix can jump across from
/// block to block, and the west stake Pounded throws it out again. Down in the
/// room of the pit, the stone pillar pushed or Moved one cell west onto the
/// purple button lowers the bridge across the pit for good, a shortcut.
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
#include "system/main_routine_state.h"
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
#include "scene/lighthouse_palettes.h"

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

/// A path block lies in a gap of the east hall, a flag for each PathBlockPlace
/// from the first.
#define FLAG_PATH_BLOCK_PLACED_FIRST 0xa10
#define FLAG_PATH_BLOCK_PLACED(place) (FLAG_PATH_BLOCK_PLACED_FIRST + (place))
/// The east stake is down in the floor: Pounded, or not yet raised by the two
/// blocks both lying in gaps.
#define FLAG_EAST_STAKE_SUNK 0xa16
/// The west stake is down in the floor, Pounded; the third block is then out
/// of its gap.
#define FLAG_WEST_STAKE_SUNK 0xa17
/// The third path block lies in its gap below the west hall's north door.
#define FLAG_THIRD_BLOCK_IN_GAP 0xa1a
/// The pillar stands on the button by the pit: the bridge stays across it.
#define FLAG_BUTTON_PILLAR_PLACED 0xa1b
/// Set while Felix stands on the button, which holds the bridge across.
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
/// The rooms' own cell behaviors.
enum JupiterLighthouseWhorlCellBehavior {
  /// The purple button by the pit, cell (8, 37), and the eight cells round
  /// it.
  CELL_BEHAVIOR_BRIDGE_BUTTON = 20,
  CELL_BEHAVIOR_OFF_BRIDGE_BUTTON,
  /// The west hall's floor among the grey blocks, (8, 16) to (15, 20), where
  /// Felix jumps from block to block while they stand raised.
  CELL_BEHAVIOR_GREY_BLOCK_JUMP = 30,
  /// The cells beside the gaps, each named for the side of the gap it is on:
  /// walking into the gap from it pushes a block or jumps across.
  CELL_BEHAVIOR_EAST_OF_GAP = 40,
  CELL_BEHAVIOR_SOUTH_OF_GAP,
  CELL_BEHAVIOR_WEST_OF_GAP,
  CELL_BEHAVIOR_NORTH_OF_GAP
};
/// A gap's floor, 16 pixels below the hall's, where a thrown block starts.
#define GAP_FLOOR_HEIGHT toFIXED(-16)
/// The most frames the room waits for a block to come down.
#define BLOCK_LANDING_FRAMES 30
/// The way in from the bridge's floor, through its west arch.
#define ENTRANCE_FROM_BRIDGE_FLOOR 12
/// The first of the two ways into the east hall; the other is the next.
#define ENTRANCE_EAST_HALL_FIRST 3

/// The whorl's actors, numbering its placement table.
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

/// Felix talks to Whorl on its ledge in the east hall, facing north: the
/// Jupiter Djinni fights the party.
void JupiterLighthouse_FightWhorl(void) {
  Field_RequestDjinnBattle(ACTOR_WHORL, DJINNI_WHORL);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// The bridge's two panels, standing on end at the pit's sides, swing down to
/// lie across it; with `mode` 1 they do so with a thud and a picture halfway
/// down, otherwise they simply lie there.
void JupiterLighthouse_RaiseBridge(s32 mode) {
  if (mode == 1) {
    Event_PlaySound(SE_IMPACT);
    MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(81, 42, 4, 3, 76, 35);
  MapGrid_CopyCellAttributes(12, 41, 4, 1, 12, 35);
}

/// The bridge's two panels swing back up on end with a heavy thud.
void JupiterLighthouse_LowerBridge(void) {
  Event_PlaySound(SE_HEAVY_IMPACT);
  MapGrid_CopyCellsLayered(76, 42, 4, 3, 76, 35);
  System_Wait(6);
  MapGrid_CopyCellsLayered(71, 42, 4, 3, 76, 35);
  MapGrid_CopyCellAttributes(12, 36, 4, 1, 12, 35);
}

/// Lays the bridge across the pit for good once the pillar stands on the
/// button.
void JupiterLighthouse_CheckPillarOnButton(void) {
  struct Actor *actor = Actor_Get(ACTOR_BUTTON_PILLAR);

  if (!Event_GetFlag(FLAG_BUTTON_PILLAR_PLACED) &&
      FIELD_CELL(actor->x.fixed) == BUTTON_CELL_X &&
      FIELD_CELL(actor->z.fixed) == BUTTON_CELL_Z) {
    Event_SetFlag(FLAG_BUTTON_PILLAR_PLACED);
    JupiterLighthouse_RaiseBridge(1);
  }
}

/// Felix steps onto the purple button by the pit, cell (8, 37): the bridge's
/// panels swing down across the pit with a thud, and stay while he stands
/// there.
void JupiterLighthouse_StepOnButton(void) {
  if (!Event_GetFlag(FLAG_BRIDGE_BUTTON_HELD)) {
    Event_SetFlag(FLAG_BRIDGE_BUTTON_HELD);
    JupiterLighthouse_RaiseBridge(1);
  }
}

/// Felix steps off the purple button by the pit: the bridge's panels swing back
/// up on end with a heavy thud. If the stone pillar has just been pushed onto
/// the button, the bridge stays across the pit for good instead.
void JupiterLighthouse_StepOffButton(void) {
  if (Event_GetFlag(FLAG_BRIDGE_BUTTON_HELD) == 1) {
    Event_ClearFlag(FLAG_BRIDGE_BUTTON_HELD);
    JupiterLighthouse_LowerBridge();
  }
  JupiterLighthouse_CheckPillarOnButton();
}

/// The stone pillar beside the purple button by the pit, Moved one cell west
/// onto the button at cell (8, 37), holds the bridge across the pit for good,
/// a shortcut back through the tower. Moved anywhere else, it does nothing.
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
  place = -1;
  x = FIELD_CELL(straight->x.fixed);
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
  Call4((void (*)())MapGrid_SetCellHeight, toFIXED(560), toFIXED(240),
        MAPGRID_LAYER_GROUND, -2);
  Call4((void (*)())MapGrid_SetCellHeight, toFIXED(496), toFIXED(256),
        MAPGRID_LAYER_GROUND, -2);
  Call4((void (*)())MapGrid_SetCellHeight, toFIXED(528), toFIXED(160),
        MAPGRID_LAYER_GROUND, -2);
  FieldMapGrid_SetCellBehavior(toFIXED(608), toFIXED(160),
                               MAPGRID_LAYER_OVERHEAD, CELL_BEHAVIOR_NORMAL);
  FieldMapGrid_SetCellBehavior(toFIXED(496), toFIXED(160),
                               MAPGRID_LAYER_OVERHEAD, CELL_BEHAVIOR_NORMAL);
  FieldMapGrid_SetCellBehavior(toFIXED(496), toFIXED(224),
                               MAPGRID_LAYER_OVERHEAD, CELL_BEHAVIOR_NORMAL);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP)))
    MapGrid_SetCellHeight(toFIXED(560), toFIXED(240), MAPGRID_LAYER_GROUND, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP)))
    MapGrid_SetCellHeight(toFIXED(496), toFIXED(256), MAPGRID_LAYER_GROUND, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP)) ||
      Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)))
    MapGrid_SetCellHeight(toFIXED(528), toFIXED(160), MAPGRID_LAYER_GROUND, 0);
  if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_EAST_GAP))) {
    MapGrid_CopyCellsLayered(96, 56, 7, 2, 96, 15);
    MapGrid_CopyCellsLayered(111, 51, 1, 1, 99, 15);
    if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_WEST_GAP))) {
      MapGrid_CopyCellsLayered(105, 51, 1, 3, 95, 14);
      FieldMapGrid_SetCellBehavior(toFIXED(496), toFIXED(224),
                                   MAPGRID_LAYER_OVERHEAD, CELL_BEHAVIOR_HOVER);
    }
    if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_NORTH_GAP)))
      MapGrid_CopyCellsLayered(111, 53, 1, 1, 97, 10);
  } else if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(CORNER_BLOCK_IN_EAST_GAP))) {
    MapGrid_CopyCellsLayered(98, 51, 5, 1, 98, 10);
    MapGrid_CopyCellsLayered(99, 52, 4, 5, 99, 11);
    FieldMapGrid_SetCellBehavior(toFIXED(608), toFIXED(160),
                                 MAPGRID_LAYER_OVERHEAD, CELL_BEHAVIOR_HOVER);
    if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_WEST_GAP)))
      MapGrid_CopyCellsLayered(106, 53, 1, 1, 95, 16);
    if (Event_GetFlag(FLAG_PATH_BLOCK_PLACED(STRAIGHT_BLOCK_IN_NORTH_GAP))) {
      MapGrid_CopyCellsLayered(109, 51, 3, 1, 95, 10);
      FieldMapGrid_SetCellBehavior(toFIXED(496), toFIXED(160),
                                   MAPGRID_LAYER_OVERHEAD, CELL_BEHAVIOR_HOVER);
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
    MapGrid_SetCellHeight(toFIXED(128), toFIXED(192), MAPGRID_LAYER_GROUND, 4);
    if (mode == 1) {
      for (frame = 0; frame <= 15; frame++) {
        for (i = 0; i <= 5; i++) {
          actor = Actor_Get(ACTOR_FIRST_GREY_BLOCK + i);
          if (JupiterLighthouse_IsOnWestPath(actor)) {
            actor->motionFlags = 0;
            actor->y.fixed -= toFIXED(2);
          }
        }
        System_Wait(1);
      }
    }
    i = 0;
    do {
      actor = Actor_Get(ACTOR_FIRST_GREY_BLOCK + i);
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      i++;
      actor->y.fixed = actor->groundHeight;
    } while (i <= 5);
  } else {
    MapGrid_CopyCellsLayered(72, 46, 1, 7, 72, 9);
    MapGrid_CopyCellsLayered(72, 53, 8, 5, 72, 16);
    MapGrid_CopyCellsLayered(70, 47, 1, 1, 8, 74);
    MapGrid_SetCellHeight(toFIXED(128), toFIXED(192), MAPGRID_LAYER_GROUND, 6);
    FieldActor_Stop(gMainRoutineState.leaderActorId);
    Actor_SetPosition(ACTOR_THIRD_PATH_BLOCK, 0, 0);
    if (mode == 1) {
      for (frame = 0; frame <= 15; frame++) {
        for (i = 0; i <= 5; i++) {
          actor = Actor_Get(ACTOR_FIRST_GREY_BLOCK + i);
          if (JupiterLighthouse_IsOnWestPath(actor)) {
            actor->motionFlags = 0;
            actor->y.fixed += toFIXED(2);
          }
        }
        System_Wait(1);
      }
    }
    for (i = 0; i <= 5; i++) {
      actor = Actor_Get(ACTOR_FIRST_GREY_BLOCK + i);
      if (JupiterLighthouse_IsOnWestPath(actor)) {
        actor->motionFlags = ACTOR_FLOATS;
        actor->y.fixed = toFIXED(65) / 2;
      }
    }
  }
}

/// Nothing visible happens as Felix starts to push or Move an energy path
/// block. When it is the west hall's block, the cell it leaves becomes plain
/// floor again, no longer a step to jump onto.
void JupiterLighthouse_LiftPathBlock(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);

  if (id == ACTOR_THIRD_PATH_BLOCK) {
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, MAPGRID_LAYER_GROUND,
                          6);
    MapGrid_ClearAttributeBBitsAt(actor->x.fixed, actor->z.fixed,
                                  MAPGRID_LAYER_GROUND, MAP_CELL_JUMP);
  }
}

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void MapGrid_SetCellAttributeBitsAt(s32 x, s32 z, s32 layer, s32 bits);

/// An energy path block, pushed or Moved, comes to rest, landing with a thump
/// if it dropped off a ledge. In one of the east hall's three gaps, (35, 15),
/// (31, 16) or (33, 10), it sinks in and lights the path through it, with a
/// sound where the lit path reaches it; once both blocks lie in gaps, the
/// camera turns to the cone-shaped stake as it rises. The west hall's block, in
/// its gap at cell (8, 12), lights the path under the grey blocks and raises
/// the west stake. Set down anywhere else, the west hall's block stands as a
/// step Felix can jump onto.
void JupiterLighthouse_PlacePathBlock(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 place;
  s32 i;

  if (actor->groundHeight !=
      Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed)) {
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
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
    // The block is drawn into the floor now, its actor put away.
    Actor_Stop(gMainRoutineState.leaderActorId);
    Actor_SetPosition(id, 0, 0);
    if (JupiterLighthouse_CountBlocksInGaps() != 2 ||
        !Event_GetFlag(FLAG_EAST_STAKE_SUNK))
      return;
    actor = Actor_GetById(ACTOR_EAST_STAKE);
    Event_ClearFlag(FLAG_EAST_STAKE_SUNK);
    Event_Wait(10);
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    FieldCamera_MoveTo(664, CAMERA_KEEP, 136, 1);
    FieldCamera_WaitForArrival();
    Event_Wait(20);
    Event_PlaySound(SE_LIGHT_APPEAR);
    Actor_SelectAnimation(actor, 3);
    actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    FieldMapGrid_SetCellBehavior(actor->x.fixed, actor->z.fixed, actor->layer,
                                 CELL_BEHAVIOR_WALL);
    Event_Wait(30);
    EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
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
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, MAPGRID_LAYER_GROUND,
                          8);
    MapGrid_SetCellAttributeBitsAt(actor->x.fixed, actor->z.fixed,
                                   MAPGRID_LAYER_GROUND, MAP_CELL_JUMP);
  }
}

/// Nothing visible happens as Felix starts to cast Pound on the east hall's
/// stake; whatever the hall was still doing is settled first
/// (`Event_EndLastingPsynergy`).
void JupiterLighthouse_StartPound(void) { Event_EndLastingPsynergy(0); }

/// Pound hammers the cone-shaped stake in the east or the west hall down into
/// the floor.
void JupiterLighthouse_SinkPoundTarget(s32 mode, s32 frame) {
  Pound_SinkTarget(gJupiterLighthouse_WhorlPoundTargets, mode, frame);
}

/// Throws a block up out of its gap at cell (x, z) to land beside it.
void JupiterLighthouse_ThrowOutBlock(s32 id, s32 x, s32 z, s32 dx, s32 dz) {
  struct Actor *actor = Actor_Get(id);

  Actor_SetPositionFX(id, FIELD_CELL_CENTER(x), FIELD_CELL_CENTER(z));
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  actor->groundHeight = GAP_FLOOR_HEIGHT;
  actor->y.fixed = GAP_FLOOR_HEIGHT;
  actor->velocityY = toFIXED(8);
  Actor_SetSpeed(id, 1.6);
  Actor_SetDestinationOffset(id, dx, dz);
  Event_PlaySound(SE_WHIRLWIND);
}

/// Once Pound has hammered the east hall's stake down, the camera turns to the
/// gaps and the two energy path blocks fly up out of them with a rush of wind,
/// each landing on the floor beside its gap, and the path goes dark.
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
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
}

/// Once Pound has hammered the west hall's stake down, its energy path block
/// flies up out of its gap at cell (8, 12) and lands beside it, and the path
/// under the grey blocks goes dark, letting them down.
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

/// Felix walks towards a gap from a cell beside it, in the east hall or round
/// the west hall's gap: a block in front of him he pushes; an empty gap he
/// jumps across, where there is floor to land on; over a gap a block fills,
/// he walks on.
void JupiterLighthouse_JumpOrPushBlock(void) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
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

/// Felix walks among the raised grey blocks in the west hall, between cells
/// (8, 16) and (15, 20), while its path is lit: he jumps forward onto the next
/// block, where there is one to land on.
void JupiterLighthouse_JumpForward(void) { Field_TryJumpForward(); }

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

void Actors_SetCellsOpen();
void LightSprites_InitB();
void LightSprites_AddCounted();

/// Sets up the rooms as the party enters: the archer statue at three quarters
/// of its size, the stakes and the bridge as their flags leave them, the
/// blocks' glow, and the grey blocks as platforms. Returns 0.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  // In the east hall the leader is drawn by the cells under him, never hidden
  // behind the rises in front.
  if ((u16)(gMainRoutineState.entrance - ENTRANCE_EAST_HALL_FIRST) <= 1)
    Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
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
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
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

s32 MapGrid_GetCellHeight(FIXED x, FIXED z, s32 layer);

/// Lays out the blocks as the map is drawn: the first two stay in their gaps
/// only when both lie in them or the party comes back to where it left, the
/// third lies in its gap while the west stake stands, and the floor and the
/// west hall's path follow. Returns 0.
s32 Scene_PrepareMap(void) {
  struct Actor *actor;
  s32 height;

  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(LIGHTHOUSE_PALETTES_LIT);
  // Arriving from the bridge's floor, the grey blocks stand afresh on six of
  // the west hall's path cells.
  if (gMainRoutineState.entrance == ENTRANCE_FROM_BRIDGE_FLOOR) {
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
    height = MapGrid_GetCellHeight(actor->x.fixed, actor->z.fixed,
                                   MAPGRID_LAYER_GROUND);
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, MAPGRID_LAYER_GROUND,
                          height + 2);
    MapGrid_SetCellAttributeBitsAt(actor->x.fixed, actor->z.fixed,
                                   MAPGRID_LAYER_GROUND, MAP_CELL_JUMP);
  }
  JupiterLighthouse_LightWestPath(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Restarts the palette cycle at the given frame.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

void MapGrid_SetCellAttributesAt();

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

#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/actors_set_cells_open.h"

#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

s32 Actor_GetSpritePriority(struct Actor *actor);

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

/// Sets the priority the lit actors' sprites are drawn at.
void LightSprites_SetPriority(u16 value) { gLightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *actor, u32 value);

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
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

/// The cells of the west hall's path, ending at an x of -1.
// clang-format off
s16 gJupiterLighthouse_WestPathCells[][2] = {
    {8, 16}, {10, 16}, {11, 16}, {12, 16}, {14, 16}, {15, 16}, {10, 18},
    {12, 18}, {13, 18}, {8, 20},  {10, 20}, {13, 20}, {15, 20}, {-1, 0},
};
// clang-format on

// clang-format off
#define FIELD_EFFECT_DURATION_SCRIPT_SECTION LATE_DATA
#include "shared/field_effect_duration_scripts.c"

/// The two stakes Pound sinks: each stake, its kind and the flag set once it is
/// sunk, ended by -1.
LATE_DATA s16 gJupiterLighthouse_WhorlPoundTargets[] = {
    ACTOR_EAST_STAKE, STAKE_STONE, FLAG_EAST_STAKE_SUNK, ACTOR_WEST_STAKE, STAKE_STONE, FLAG_WEST_STAKE_SUNK, -1, 0,
};

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

LATE_DATA u32 gSceneExits[] = {                                                        
      SCENE_EXITS(JupiterLighthouseWhorl),         SCENE_EXIT(JUPITER_LIGHTHOUSE_WHORL_TO_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST, JupiterLighthouseRoomPhaetonsBladeChest, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_FROM_JUPITER_LIGHTHOUSE_WHORL),   SCENE_EXIT(2, JupiterLighthouseWhorl, 3),   
      SCENE_EXIT(3, JupiterLighthouseWhorl, 2),    SCENE_EXIT(4, JupiterLighthouseWhorl, 5),   SCENE_EXIT(5, JupiterLighthouseWhorl, 4),   
      SCENE_EXIT(6, JupiterLighthouseWhorl, 7),   SCENE_EXIT(7, JupiterLighthouseWhorl, 6),  SCENE_EXIT(JUPITER_LIGHTHOUSE_WHORL_TO_JUPITER_LIGHTHOUSE_TERRACES, JupiterLighthouseTerraces, JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_WHORL),   
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
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_GREY_BLOCK_JUMP, IF_FLAG_SET(FLAG_THIRD_BLOCK_IN_GAP), EVENT(JupiterLighthouse_JumpForward)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BRIDGE_BUTTON, FLAG_BUTTON_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOnButton)},
    {TRIGGER_STEP, CELL_BEHAVIOR_OFF_BRIDGE_BUTTON, FLAG_BUTTON_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOffButton)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_EAST_OF_GAP, CONDITION_ALWAYS, EVENT(JupiterLighthouse_JumpOrPushBlock)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_SOUTH_OF_GAP, CONDITION_ALWAYS, EVENT(JupiterLighthouse_JumpOrPushBlock)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_WEST_OF_GAP, CONDITION_ALWAYS, EVENT(JupiterLighthouse_JumpOrPushBlock)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_NORTH_OF_GAP, CONDITION_ALWAYS, EVENT(JupiterLighthouse_JumpOrPushBlock)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_WHORL, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FightWhorl)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_STRAIGHT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STRAIGHT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePathBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CORNER_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CORNER_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePathBlock)},
    // A push trigger answers every pushed actor; its 35 is never compared.
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
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};
// clang-format on

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
