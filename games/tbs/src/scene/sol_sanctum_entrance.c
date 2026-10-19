/// @file
/// Sol Sanctum's entrance, in two maps: the door, the two minotaur reliefs that
/// open passages once the Small Jewels are set in them, and the rooms of
/// movable blocks.

#include "game_state.h"
#include "unit/field/field.h"
#include "unit/import.h"
#include "unit/scene/sol_sanctum_entrance.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_service.h"
#include "sol_sanctum_entrance.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_obj.h"
#include "fixed_math.h"
#include "macros.h"
#include "songs.h"
#include "item_ids.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.

// The column each movable block stands in, east or west of its pair.
#define FLAG_SOL_SANCTUM_OUTER_RIGHT_BLOCK_EAST 0x300
#define FLAG_SOL_SANCTUM_OUTER_RIGHT_BLOCK_WEST 0x301
#define FLAG_SOL_SANCTUM_OUTER_LEFT_BLOCK_EAST 0x302
#define FLAG_SOL_SANCTUM_OUTER_LEFT_BLOCK_WEST 0x303
#define FLAG_SOL_SANCTUM_INNER_BLOCK_1_EAST 0x310
#define FLAG_SOL_SANCTUM_INNER_BLOCK_1_WEST 0x311
#define FLAG_SOL_SANCTUM_INNER_BLOCK_2_EAST 0x312
#define FLAG_SOL_SANCTUM_INNER_BLOCK_2_WEST 0x313
#define FLAG_SOL_SANCTUM_INNER_BLOCK_3_EAST 0x314
#define FLAG_SOL_SANCTUM_INNER_BLOCK_3_WEST 0x315
#define FLAG_SOL_SANCTUM_INNER_BLOCK_4_EAST 0x316
#define FLAG_SOL_SANCTUM_INNER_BLOCK_4_WEST 0x317
#define FLAG_SOL_SANCTUM_INNER_BLOCK_5_EAST 0x318
#define FLAG_SOL_SANCTUM_INNER_BLOCK_5_WEST 0x319
#define FLAG_SOL_SANCTUM_INNER_BLOCK_6_EAST 0x31a
#define FLAG_SOL_SANCTUM_INNER_BLOCK_6_WEST 0x31b

// Forward declarations.
static void SolSanctumEntrance_ApplyEntryState(void);
static void SolSanctumEntrance_RestoreInteriorState(void);
static void SolSanctumEntrance_IntroduceSanctum(void);
static void SolSanctumEntrance_KradenSuspectsHiddenPassage(void);
static void SolSanctumEntrance_InspectInnerStatues(void);
static void SolSanctumEntrance_RestoreInnerBlockCells(s32 placeActors);
static void SolSanctumEntrance_UpdateEruptionRumbles(void);

s32 gSolSanctumEntrance_RumbleTimer __attribute__((section(".bss")));

void InitializeSceneRecordBuffer();

/// Selects the entrance table for the current sanctum map.
const struct SceneEntrance *SolSanctumEntrance_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_SolSanctumEntrance2) {
    return gSolSanctumEntranceEntrances2;
  }
  if (scene == (s32)&SceneId_SolSanctumEntrance1) {
    return gSolSanctumEntranceEntrances1;
  }
  return gSolSanctumEntranceEntrancesOther;
}

/// The sanctum entrance maps have no region table.
const struct SceneRegion *SolSanctumEntrance_GetRegions(void) { return 0; }

/// Returns the shared sanctum exit routes.
const u32 *SolSanctumEntrance_GetExits(void) {
  return gSolSanctumEntrance_Exits;
}

/// Selects the actors for the current map and puzzle room.
const struct ScenePlacement *SolSanctumEntrance_GetPlacements(void) {
  struct ScenePlacement *table;
  s32 firstBlockEntrance = 11;

  if (gGameState.scene == (s32)&SceneId_SolSanctumEntrance1) {
    if (gGameState.entrance >= firstBlockEntrance) {
      if (gGameState.entrance > 13) {
        if (gGameState.entrance <= 16) {
          return gSolSanctumEntrancePlacements1Entrances14To16;
        }
      } else {
        return gSolSanctumEntrancePlacements1Entrances11To13;
      }
    }
    table = gSolSanctumEntrancePlacements1;
    FieldScene_PrepareActors(table);
    return table;
  }
  if (gGameState.scene == (s32)&SceneId_SolSanctumEntrance2) {
    return gSolSanctumEntrancePlacements2;
  }
  return gSolSanctumEntrancePlacementsOther;
}

/// Selects the interactions for the current map and puzzle room.
const struct SceneEvent *SolSanctumEntrance_GetEvents(void) {
  s32 firstBlockEntrance = 11;

  if (gGameState.scene == (s32)&SceneId_SolSanctumEntrance2) {
    return gSolSanctumEntranceEvents2;
  }
  if (gGameState.scene == (s32)&SceneId_SolSanctumEntrance1) {
    if (gGameState.entrance >= firstBlockEntrance) {
      if (gGameState.entrance > 13) {
        if (gGameState.entrance <= 16) {
          return gSolSanctumEntranceEvents1Entrances14To16;
        }
      } else {
        return gSolSanctumEntranceEvents1Entrances11To13;
      }
    }
    return gSolSanctumEntranceEvents1;
  }
  return gSolSanctumEntranceEventsOther;
}

/// Walking up to the sanctum's door: it slides open and the leader walks in.
static void Event_OpenDoorAndEnter(void) {
  s32 width;
  s32 height;

  Engine_EventBegin();
  // The door opens over three frames.
  width = 3;
  height = 2;
  Event_PlayCue(SE_181);
  Map_CopyCellsTo(16, 28, 21, 3, width, height);
  Engine_TaskWait(10);
  Map_CopyCellsTo(16, 30, 21, 3, width, height);
  Engine_TaskWait(10);
  Map_CopyCellsTo(16, 32, 21, 3, width, height);
  Engine_TaskWait(10);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 98);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
  Engine_EventWait(10);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(2);
  Engine_EventEnd();
}

/// Searching the first minotaur relief: one eye sparkles, or both once the
/// Small Jewel is set. "The right eye is sparkling, but there's a hole where
/// the left eye should be." Holding the jewel sets the event work's unk172;
/// what reads it is not known.
static void Event_InspectFirstMinotaurRelief(void) {

  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FIRST_GEM_PLACED) != 0) {
    Engine_MessageShowCentered(MsgSolSanctumMinotaurReliefBothEyes, 1);
  } else {
    Engine_MessageShowCentered(MsgSolSanctumMinotaurReliefOneEye, 1);
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND) != 0) {
      u16 *stateSlot = &gEventWork->unk172;
      u16 value = 1;
      *stateSlot = value;
    }
  }

  Engine_EventEnd();
}

/// The room shakes as stone shifts; the leader looks around and jumps.
static inline void SolSanctumEntrance_ReactToOpeningPassage(void) {
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 20);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 40);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_EventWait(40);
}

/// Using the Small Jewel on the first relief: the leader sets it in and a
/// passage opens. "set the small gem into the empty socket.", "You hear the
/// sound of shifting stone."
static void Event_SetFirstSmallJewel(void) {
  s32 message;
  s32 oneCell;
  s32 reliefWidth;

  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND) == 0) {
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_FIRST_GEM_PLACED) != 0) {
    } else {
      Engine_EventBegin();
      Battle_ResetEffectCounter();
      // The eye sparkles, then the wall opens.
      oneCell = 1;
      Event_PlayCue(SE_182);
      Map_CopyCellsTo(0, 70, 30, 42, oneCell, oneCell);
      Engine_MapRedraw();
      Engine_EventWait(40);
      message = MsgSolSanctumSetSmallGem;
      Engine_MessageShowCentered(message, 1);
      Engine_EventWait(20);
      reliefWidth = 3;
      Event_PlayCue(SE_183);
      Map_CopyCellsTo(0, 29, 3, 1, reliefWidth, 2);
      Map_CopyCellAttributes(0, 29, 3, 2, reliefWidth, oneCell);
      Map_CopyCellsTo(1, 109, 4, 81, oneCell, oneCell);
      Engine_MapRedraw();
      SolSanctumEntrance_ReactToOpeningPassage();
      Engine_MessageShowCentered(message + 1, 1);
      GameFlag_Set(FLAG_ITEM_USED_UP);
      GameFlag_Set(FLAG_SOL_SANCTUM_FIRST_GEM_PLACED);
      Engine_EventEnd();
    }
  }
}

/// Searching the second minotaur relief, as the first.
static void Event_InspectSecondMinotaurRelief(void) {

  struct EventWork *work;

  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SECOND_GEM_PLACED) != 0) {
    Engine_MessageShowCentered(MsgSolSanctumMinotaurReliefBothEyes, 1);
  } else if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND) !=
             0) {
    work = gEventWork;
    Engine_MessageShowCentered(MsgSolSanctumMinotaurReliefOneEye, 1);
    {

      u16 *stateSlot = &work->unk172;
      s32 one = 1;
      *stateSlot = one;
    }
  } else {
    Engine_MessageShowCentered(MsgSolSanctumMinotaurReliefOneEye, 1);
  }

  Engine_EventEnd();
}

/// Using the Small Jewel on the second relief: as the first, and the inner
/// doorway opens.
static void Event_SetSecondSmallJewel(void) {
  s32 message;

  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND) != 0 &&
      GameFlag_IsSet(FLAG_SOL_SANCTUM_SECOND_GEM_PLACED) == 0) {
    Engine_EventBegin();
    Battle_ResetEffectCounter();
    // The eye sparkles, then the doorway opens.
    Engine_AudioPlayCue(SE_182);
    Map_CopyCellsTo(0, 71, 100, 71, 1, 1);
    Engine_MapRedraw();
    Engine_EventWait(40);
    message = MsgSolSanctumSetSmallGem;
    Engine_MessageShowCentered(message, 1);
    Engine_EventWait(20);
    Engine_AudioPlayCue(SE_183);
    Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
    Map_CopyCellAttributes(122, 20, 1, 2, 120, 30);
    Engine_MapRedraw();
    SolSanctumEntrance_ReactToOpeningPassage();
    Engine_MessageShowCentered(message + 1, 1);
    GameFlag_Set(FLAG_ITEM_USED_UP);
    GameFlag_Set(FLAG_SOL_SANCTUM_SECOND_GEM_PLACED);
    Engine_EventEnd();
  }
}

/// Remembers which of a movable block's two destination columns it occupies.
static inline void SolSanctumEntrance_RecordBlockColumn(s32 column,
                                                        s32 flagBase,
                                                        s32 leftColumn,
                                                        s32 rightColumn) {
  GameFlag_Clear(flagBase);
  GameFlag_Clear(flagBase + 1);
  if (column == leftColumn) {
    Engine_GameFlagSet(flagBase + 1);
  } else if (column == rightColumn) {
    Engine_GameFlagSet(flagBase);
  }
}

/// Moving the outer left block with Move: the column it stops in is saved.
static void Event_OuterLeftBlockMoved(void) {
  struct FieldActor *block = Object_GetById(ACTOR_OUTER_LEFT_BLOCK);
  s32 column;

  if (block == NULL) {
    return;
  }
  column = block->x.fixed >> 20;
  SolSanctumEntrance_RecordBlockColumn(
      column, FLAG_SOL_SANCTUM_OUTER_LEFT_BLOCK_EAST, 93, 95);
}

/// Moving the outer right block with Move: the column it stops in is saved.
static void Event_OuterRightBlockMoved(void) {
  struct FieldActor *block = Object_GetById(ACTOR_OUTER_RIGHT_BLOCK);
  s32 column;

  if (block == NULL) {
    return;
  }
  column = block->x.fixed >> 20;
  GameFlag_Clear(FLAG_SOL_SANCTUM_OUTER_RIGHT_BLOCK_EAST);
  GameFlag_Clear(FLAG_SOL_SANCTUM_OUTER_RIGHT_BLOCK_WEST);
  if (column == 115) {
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_OUTER_RIGHT_BLOCK_EAST);
  } else if (column == 113) {
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_OUTER_RIGHT_BLOCK_WEST);
  }
}

/// Moving an inner block with Move: the column it stops in is saved and the
/// floor under all six is set again.
static void Event_InnerBlock1Moved(void) {
  struct FieldActor *block = Object_GetById(ACTOR_INNER_BLOCK_1);
  s32 column;

  if (block == NULL) {
    return;
  }
  column = block->x.fixed >> 20;
  SolSanctumEntrance_RecordBlockColumn(
      column, FLAG_SOL_SANCTUM_INNER_BLOCK_1_EAST, 99, 101);
  SolSanctumEntrance_RestoreInnerBlockCells(0);
}

/// As Event_InnerBlock1Moved, for the second block.
static void Event_InnerBlock2Moved(void) {
  struct FieldActor *block = Object_GetById(ACTOR_INNER_BLOCK_2);
  s32 column;

  if (block == NULL) {
    return;
  }
  column = block->x.fixed >> 20;
  SolSanctumEntrance_RecordBlockColumn(
      column, FLAG_SOL_SANCTUM_INNER_BLOCK_2_EAST, 103, 105);
  SolSanctumEntrance_RestoreInnerBlockCells(0);
}

/// As Event_InnerBlock1Moved, for the third block.
static void Event_InnerBlock3Moved(void) {
  struct FieldActor *block = Object_GetById(ACTOR_INNER_BLOCK_3);
  s32 column;

  if (block == NULL) {
    return;
  }
  column = block->x.fixed >> 20;
  SolSanctumEntrance_RecordBlockColumn(
      column, FLAG_SOL_SANCTUM_INNER_BLOCK_3_EAST, 107, 109);
  SolSanctumEntrance_RestoreInnerBlockCells(0);
}

/// As Event_InnerBlock1Moved, for the fourth block.
static void Event_InnerBlock4Moved(void) {
  struct FieldActor *block = Object_GetById(ACTOR_INNER_BLOCK_4);
  s32 column;

  if (block == NULL) {
    return;
  }
  column = block->x.fixed >> 20;
  SolSanctumEntrance_RecordBlockColumn(
      column, FLAG_SOL_SANCTUM_INNER_BLOCK_4_EAST, 111, 113);
  SolSanctumEntrance_RestoreInnerBlockCells(0);
}

/// As Event_InnerBlock1Moved, for the fifth block.
static void Event_InnerBlock5Moved(void) {
  struct FieldActor *block = Object_GetById(ACTOR_INNER_BLOCK_5);
  s32 column;

  if (block == NULL) {
    return;
  }
  column = block->x.fixed >> 20;
  SolSanctumEntrance_RecordBlockColumn(
      column, FLAG_SOL_SANCTUM_INNER_BLOCK_5_EAST, 115, 117);
  SolSanctumEntrance_RestoreInnerBlockCells(0);
}

/// As Event_InnerBlock1Moved, for the sixth block.
static void Event_InnerBlock6Moved(void) {
  struct FieldActor *block = Object_GetById(ACTOR_INNER_BLOCK_6);
  s32 column;

  if (block == NULL)
    return;
  column = block->x.fixed >> 20;
  SolSanctumEntrance_RecordBlockColumn(
      column, FLAG_SOL_SANCTUM_INNER_BLOCK_6_EAST, 119, 121);
  SolSanctumEntrance_RestoreInnerBlockCells(0);
}

// Shared source is included here in ROM order.
#include "common/object_findcell.h"

/// Walking into a block: the leader pushes it one cell, and its column is
/// saved.
static void Event_PushBlock(void) {
  struct FieldActor *leader;
  struct FieldActor *block;
  s32 stoppedVelocity;
  s32 packedStep;
  u32 direction;
  struct FieldPosition target;
  struct FieldPosition *destination;

  // Find the block one map cell ahead in the leader's facing direction.
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  direction = leader->facing >> 12;
  block = SceneActor_FindActorAtCell(
      (leader->x.part.pixel + (gSolSanctumPushSteps[direction] >> 16)) >> 4,
      (leader->z.part.pixel + (s16)gSolSanctumPushSteps[direction]) >> 4);
  if (block != NULL) {
    stoppedVelocity = 0;
    block->terrainId = 2;
    destination = &target;
    packedStep = gSolSanctumPushSteps[direction];
    SET_POSITION(destination, block->x.fixed + (packedStep & -0x10000),
                 block->y.fixed, block->z.fixed + (packedStep << 16));
    // Move only after the block's proposed position passes collision.
    if (FieldObject_CheckMovementCollision(block, destination) <= 0) {
      Object_SetMode(leader, 8);
      Engine_TaskWait(15);
      Engine_AudioPlayCue(SE_185);
      block->speed = PIXELS(1) / 5;
      block->acceleration = PIXELS(1) / 5;
      Engine_ObjectSetPosition(block, destination->x, destination->y,
                               destination->z);
      leader->speed = PIXELS(1) / 5;
      leader->acceleration = PIXELS(1) / 5;
      Engine_ObjectSetPosition(leader, destination->x, destination->y,
                               destination->z);
      FieldObject_WaitForMove(block);
      block->x.fixed = destination->x;
      block->z.fixed = destination->z;
      block->velocityX = stoppedVelocity;
      block->velocityZ = stoppedVelocity;
      Object_SetMode(leader, 1);
      // Save the moved block in the flag set belonging to this room.
      switch (gGameState.entrance) {
      case 11:
      case 12:
      case 13:
        Event_OuterLeftBlockMoved();
        Event_OuterRightBlockMoved();
        break;
      case 14:
      case 15:
      case 16:
        Event_InnerBlock1Moved();
        Event_InnerBlock2Moved();
        Event_InnerBlock3Moved();
        Event_InnerBlock4Moved();
        Event_InnerBlock5Moved();
        Event_InnerBlock6Moved();
        break;
      }
    }
  }
}

/// Restores the outdoor or interior state for this entrance.
s32 SolSanctumEntrance_Initialize(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_SolSanctumEntrance2) {
    SolSanctumEntrance_ApplyEntryState();
  } else if (scene == (s32)&SceneId_SolSanctumEntrance1) {
    SolSanctumEntrance_RestoreInteriorState();
  }
  return 0;
}

/// Restores the approach's rumbling, collision and actor positions.
static void SolSanctumEntrance_ApplyEntryState(void) {
  // Retreat does not work here (flag 0x144).
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  gEventWork->startTransition = 0x100;
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    gSolSanctumEntrance_RumbleTimer = 0;
    Task_AddCallback(SolSanctumEntrance_UpdateEruptionRumbles, 0xc80);
  }
  if (GameFlag_IsSet(FLAG_VALE_RETURNED_FROM_MT_ALEPH) != 0) {
    Map_CopyCellAttributes(5, 6, 1, 1, 6, 6);
    Map_CopyCellAttributes(5, 6, 1, 1, 7, 6);
    Map_CopyCellAttributes(5, 6, 1, 1, 8, 6);
    Map_CopyCellAttributes(0, 1, 3, 1, 6, 5);
  }
  if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    Actor_SetPosition(ACTOR_KRADEN, PIXELS(120), PIXELS(232));
    Map_CopyCellAttributes(2, 10, 1, 1, 6, 14);
    Map_CopyCellAttributes(2, 10, 1, 1, 7, 14);
    Map_CopyCellAttributes(2, 10, 1, 1, 8, 14);
  }
}

/// Restores opened passages and the current room's block positions.
static void SolSanctumEntrance_RestoreInteriorState(void) {
  s32 entrance;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    BattleFx_SetQueuedSoundAndPlay(SE_141);
    Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    InitializeSceneRecordBuffer();
  }

  entrance = gGameState.entrance;
  switch (entrance) {
  case 1:
  case 2:
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FIRST_GEM_PLACED) != 0) {
      Map_CopyCellsTo(1, 109, 4, 81, 1, 1);
      Map_CopyCellsTo(0, 70, 30, 42, 1, 1);
      Map_CopyCellsTo(0, 29, 3, 1, 3, 2);
      Map_CopyCellAttributes(0, 29, 3, 2, 3, 1);
      Engine_MapRedraw();
    }
    break;

  case 3:
    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
    break;

  case 8:
    gGameState.retreatScene = (s32)&SceneId_SolSanctumEntrance1;
    gGameState.retreatEntrance = 8;
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ENTERED) == 0)
      SolSanctumEntrance_IntroduceSanctum();
    break;

  case 11:
  case 12:
  case 13:
    // Recreate the outer puzzle after its introduction has played.
    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(16), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(19), 0);
    if (Engine_GameFlagIsSet(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE) == 0)
      SolSanctumEntrance_KradenSuspectsHiddenPassage();
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_OUTER_LEFT_BLOCK_WEST) != 0)
      Actor_SetPosition(ACTOR_OUTER_LEFT_BLOCK, PIXELS(1496), PIXELS(136));
    else if (GameFlag_IsSet(FLAG_SOL_SANCTUM_OUTER_LEFT_BLOCK_EAST) != 0)
      Actor_SetPosition(ACTOR_OUTER_LEFT_BLOCK, PIXELS(1528), PIXELS(136));
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_OUTER_RIGHT_BLOCK_WEST) != 0)
      Actor_SetPosition(ACTOR_OUTER_RIGHT_BLOCK, PIXELS(1816), PIXELS(136));
    else if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_OUTER_RIGHT_BLOCK_EAST) != 0)
      Actor_SetPosition(ACTOR_OUTER_RIGHT_BLOCK, PIXELS(1848), PIXELS(136));
    break;

  case 14:
  case 15:
  case 16:
    // Restore the inner blocks and the second gem's doorway.
    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_INNER_STATUES_SEEN) == 0)
      SolSanctumEntrance_InspectInnerStatues();
    SolSanctumEntrance_RestoreInnerBlockCells(1);
    Engine_GameFlagSet(FLAG_SCENE_TEMP_34);
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SECOND_GEM_PLACED) != 0) {
      Map_CopyCellsTo(0, 71, 100, 71, 1, 1);
      Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
      Map_CopyCellAttributes(122, 20, 1, 2, 120, 30);
      Engine_MapRedraw();
    }
    break;
  }
}

/// Introduces Kraden's first visit to Sol Sanctum.
static void SolSanctumEntrance_IntroduceSanctum(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventOpenScreen();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(4);
  Camera_MoveTo(-1, -1, -1, 0);
  // Gather the followers around the leader for Kraden's introduction.
  Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Camera_MoveTo(PIXELS(1224), -1, PIXELS(136), 1);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(JENNA, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(JENNA, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  Engine_ActorSetAnimation(JENNA, 2);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  Actor_SetDestinationOffset(GARET, -16, 0);
  Actor_SetDestinationOffset(JENNA, 16, 0);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 0, -32);
  Engine_ActorWaitForMove(GARET);
  Engine_ActorSetAnimation(GARET, 0);
  Engine_ActorSetAnimation(JENNA, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(JENNA, FACING_NORTH, 0);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ActorJump(ACTOR_KRADEN, 4, 20);
  // Ask whether this is the party's first visit, then rejoin the leader.
  Engine_EventSetMessage(MsgSolSanctumKradenFirstTimeAtSol);
  Event_AskYesNo(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Engine_EventWait(20);
  Camera_MoveTo(PIXELS(1224), -1, PIXELS(148), 1);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorSetAnimation(JENNA, 2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(JENNA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetPosition(JENNA, 0, 0);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(JENNA, 1);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  GameFlag_Set(FLAG_SOL_SANCTUM_ENTERED);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  Engine_EventEnd();
}

/// Surveys the room while Kraden explains the hidden-passage puzzle.
static void SolSanctumEntrance_KradenSuspectsHiddenPassage(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();

  // Spread the party out while Kraden examines the room.
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Engine_ActorSetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Engine_ActorSetPosition(JENNA, leader->x.fixed, leader->z.fixed);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Engine_ActorSetPosition(GARET, leader->x.fixed, leader->z.fixed);

  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(JENNA, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  Engine_ActorSetAnimation(JENNA, 2);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  Actor_SetDestinationOffset(GARET, -16, 0);
  Actor_SetDestinationOffset(JENNA, 16, 0);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 0, -16);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetAnimation(GARET, 0);
  Engine_ActorSetAnimation(JENNA, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 30);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 30);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  Engine_ActorSetDestinationOffset(ACTOR_KRADEN, 0, -16);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Engine_EventWait(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  Engine_ActorSetDestinationOffset(ACTOR_KRADEN, 0, -32);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);

  // Survey both sides of the room before returning to Kraden.
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(1585), -1, PIXELS(150), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Camera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
  Camera_MoveTo(PIXELS(1621), -1, PIXELS(100), 1);
  Engine_CameraWaitForMove();
  Camera_MoveTo(PIXELS(1718), -1, PIXELS(100), 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Camera_MoveTo(PIXELS(1752), -1, PIXELS(150), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_MoveTo(PIXELS(1668), -1, PIXELS(256), 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 3);
  Engine_EventWait(10);

  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 20);
  // Discuss Kraden's suspicion that a passage is hidden here.
  Engine_EventSetMessage(MsgSolSanctumWrongKraden);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | 2, 60);
  Engine_ActorStartRepeatedMotion(ACTOR_KRADEN, 2);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(JENNA, 0x102);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 5, 0);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 20);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Event_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  Engine_EventWait(10);
  Actor_SetAttachedEffect(ACTOR_KRADEN, 0x102);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Engine_ActorJump(ACTOR_KRADEN, 2, 20);

  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 40);
  Actor_FaceDirection(GARET, FACING_EAST, 20);
  // Garet questions whether the party should split up to search.
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgSolSanctumDangerousSplitStay);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventShowMessageAndWait(1, 0, 10);
  } else {
    Engine_EventSetMessage(MsgSolSanctumCantSeriouslyWant);
    Actor_FaceDirection(JENNA, FACING_WEST, 20);
    Event_ShowMessageAndWait(JENNA, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 40);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Engine_EventWait(10);
    Actor_FaceDirection(GARET, FACING_EAST, 0);
    Actor_FaceDirection(JENNA, FACING_WEST, 30);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(10);
    Engine_EventShowMessageAndWait(1, 0, 10);
  }

  // Bring Kraden back and return the followers to the party.
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3) / 5, PIXELS(3) / 10);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  Engine_ActorSetDestinationOffset(ACTOR_KRADEN, 0, 48);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Engine_EventWait(6);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_ActorFaceDirection(JENNA, FACING_NORTHWEST, 0);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(JENNA, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  Engine_ActorSetAnimation(JENNA, 2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Engine_ActorSetDestination(JENNA, leader->x.part.pixel,
                               leader->z.part.pixel);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Engine_ActorSetDestination(ACTOR_KRADEN, leader->x.part.pixel,
                               leader->z.part.pixel);

  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetPosition(JENNA, 0, 0);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(JENNA, 1);
  GameFlag_Set(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  Engine_EventEnd();
}

/// Shows the inner statues that Kraden cannot reach.
static void SolSanctumEntrance_InspectInnerStatues(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 24, -10);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 1);
  Engine_EventWait(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  // Pan across the statues, then return to the party's position.
  Camera_MoveTo(PIXELS(1672), -1, PIXELS(524), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Camera_MoveTo(PIXELS(1880), -1, PIXELS(524), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  Camera_MoveTo(PIXELS(1769), -1, PIXELS(548), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 30);
  Engine_EventSetMessage(MsgSolSanctumMoreStatuesOutOfReach);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0, 10);
  Engine_ActorSetAnimation(ACTOR_KRADEN, 2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  GameFlag_Set(FLAG_SOL_SANCTUM_INNER_STATUES_SEEN);
  Engine_EventEnd();
}

/// Restores block collision cells and, on entry, the saved actors.
static void SolSanctumEntrance_RestoreInnerBlockCells(s32 placeActors) {
  // Clear all six destinations before restoring occupied cells.
  Map_CopyCellAttributes(122, 20, 1, 1, 100, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 104, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 108, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 112, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 116, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 120, 32);
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_INNER_BLOCK_1_WEST) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 100, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_INNER_BLOCK_1, PIXELS(1592), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_INNER_BLOCK_1_EAST) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 100, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_INNER_BLOCK_1, PIXELS(1624), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_INNER_BLOCK_2_WEST) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 104, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_INNER_BLOCK_2, PIXELS(1656), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_INNER_BLOCK_2_EAST) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 104, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_INNER_BLOCK_2, PIXELS(1688), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_INNER_BLOCK_3_WEST) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 108, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_INNER_BLOCK_3, PIXELS(1720), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_INNER_BLOCK_3_EAST) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 108, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_INNER_BLOCK_3, PIXELS(1752), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_INNER_BLOCK_4_WEST) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 112, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_INNER_BLOCK_4, PIXELS(1784), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_INNER_BLOCK_4_EAST) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 112, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_INNER_BLOCK_4, PIXELS(1816), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_INNER_BLOCK_5_WEST) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 116, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_INNER_BLOCK_5, PIXELS(1848), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_INNER_BLOCK_5_EAST) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 116, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_INNER_BLOCK_5, PIXELS(1880), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_INNER_BLOCK_6_WEST) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 120, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_INNER_BLOCK_6, PIXELS(1912), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_INNER_BLOCK_6_EAST) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 120, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_INNER_BLOCK_6, PIXELS(1944), PIXELS(520));
      }
    }
  }
}

/// Searching the door beside the way in. "The door is tightly locked."
static void Event_DoorTightlyLocked(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
  Engine_EventEnd();
}

/// After the eruption the approach rumbles now and then, and each shake fades.
static void SolSanctumEntrance_UpdateEruptionRumbles(void) {
  s32 randomValue;
  s32 *timer;
  s32 remainingFrames;

  timer = &gSolSanctumEntrance_RumbleTimer;
  if (*timer != 0) {
    remainingFrames = (*timer - 1);
    *timer = (*timer - 1);
    if (remainingFrames == 40) {
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    }
  } else {
    randomValue = Engine_RandomNext();
    if (((u32)(((randomValue << 4) - randomValue) << 3) >> 16) == 0) {
      Event_PlayCue(SE_ITEM_BREAK);
      Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
      *timer = 80;
    }
  }
}

struct SceneEntrance gSolSanctumEntranceEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSolSanctumEntranceEntrances2[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 80, 0, 288, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 120, 0, 112, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSolSanctumEntranceEntrances1[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 280, 0, 472, FACING_NORTH, 0, 5, 0, 555, 510, 0},
    {2, CONDITION_ALWAYS, 72, 0, 56, FACING_SOUTH, 0, 5, 0, 555, 510, 0},
    {3, CONDITION_ALWAYS, 744, 0, 152, FACING_NORTH, 0, 620, 0, 860, 195, 0},
    {4, CONDITION_ALWAYS, 696, 0, 568, FACING_NORTH, 0, 575, 265, 815, 610, 0},
    {5, CONDITION_ALWAYS, 696, 0, 328, FACING_SOUTH, 0, 575, 265, 815, 610, 0},
    {6, CONDITION_ALWAYS, 984, 0, 152, FACING_NORTH, 0, 860, 0, 1100, 195, 0},
    {7, CONDITION_ALWAYS, 984, 0, 56, FACING_SOUTH, 0, 860, 0, 1100, 195, 0},
    {8, CONDITION_ALWAYS, 1224, 0, 184, FACING_NORTH, 0, 1100, 20, 1340, 220,
     0},
    {9, CONDITION_ALWAYS, 1224, 0, 104, FACING_SOUTH, 0, 1100, 20, 1340, 220,
     0},
    {10, CONDITION_ALWAYS, 1320, 0, 456, FACING_NORTH, 0, 905, 285, 1365, 490,
     0},
    {11, CONDITION_ALWAYS, 1672, 0, 280, FACING_NORTH, 0, 1455, 0, 1890, 310,
     0},
    {12, CONDITION_ALWAYS, 1512, 0, 136, FACING_SOUTH, 0, 1455, 0, 1890, 310,
     0},
    {13, CONDITION_ALWAYS, 1832, 0, 136, FACING_SOUTH, 0, 1455, 0, 1890, 310,
     0},
    {14, CONDITION_ALWAYS, 1768, 0, 568, FACING_NORTH, 0, 1540, 440, 1995, 600,
     0},
    {15, CONDITION_ALWAYS, 1672, 0, 520, FACING_SOUTH, 0, 1540, 440, 1995, 600,
     0},
    {16, CONDITION_ALWAYS, 1928, 0, 520, FACING_SOUTH, 0, 1540, 440, 1995, 600,
     0},
    {SCENE_TABLE_END},
};

u32 gSolSanctumEntrance_Exits[] = {
    SCENE_EXITS(19),
    SCENE_EXIT_IF(1, 4, 2, FLAG_SOL_SANCTUM_ERUPTED),
    SCENE_EXIT_IF(1, 10, 15, FLAG_PARTY_LEFT_VALE),
    SCENE_EXIT_IF(1, 4, 2, 0xffffffff),
    SCENE_EXIT_IF(2, 16, 8, 0xffffffff),
    SCENE_EXIT(0, 16, 0),
    SCENE_EXIT_IF(1, 15, 2, 0xffffffff),
    SCENE_EXIT_IF(2, 16, 11, 0xffffffff),
    SCENE_EXIT_IF(3, 16, 12, 0xffffffff),
    SCENE_EXIT_IF(4, 16, 9, 0xffffffff),
    SCENE_EXIT_IF(5, 15, 1, 0xffffffff),
    SCENE_EXIT_IF(6, 16, 13, 0xffffffff),
    SCENE_EXIT_IF(7, 14, 1, 0xffffffff),
    SCENE_EXIT_IF(8, 19, 2, 0xffffffff),
    SCENE_EXIT_IF(9, 16, 4, 0xffffffff),
    SCENE_EXIT_IF(10, 16, 15, 0xffffffff),
    SCENE_EXIT_IF(11, 16, 2, 0xffffffff),
    SCENE_EXIT_IF(12, 16, 3, 0xffffffff),
    SCENE_EXIT_IF(13, 16, 6, 0xffffffff),
    SCENE_EXIT_IF(14, 14, 2, 0xffffffff),
    SCENE_EXIT_IF(15, 16, 10, 0xffffffff),
    SCENE_EXIT_IF(16, 11, 1, 0xffffffff),
    SCENE_EXITS_END,
};

struct ScenePlacement gSolSanctumEntrancePlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gSolSanctumEntrancePlacements1[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xcb, 0x0, ACTOR_STAND,
        PIXELS(744), 0, PIXELS(56), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSolSanctumEntrancePlacements1Entrances11To13[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1512), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1832), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1560), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1784), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1640), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1640), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1640), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1672), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1704), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1704), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1704), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSolSanctumEntrancePlacements1Entrances14To16[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1672), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1736), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1800), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1864), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1928), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSolSanctumEntrancePlacements2[] = {
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSolSanctumEntranceEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gSolSanctumEntranceEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndEnter) },
    { SEARCH(SEARCH_UNNAMED), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DoorTightlyLocked) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgValeItIsVeryDangerousToGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgValeIAmAllAloneInThis) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSolSanctumEntranceEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { 0x21, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { SEARCH(SEARCH_CHEST), 100, FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND, FIND_ITEM(ITEM_SMALL_JEWEL) },
    { USE_ITEM(ITEM_SMALL_JEWEL), 0x414, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetFirstSmallJewel) },
    { SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InspectFirstMinotaurRelief) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSolSanctumEntranceEvents1Entrances11To13[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { 0x21, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, FLAG_SCENE_TEMP_34, EVENT_SCRIPT(Event_OuterLeftBlockMoved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, FLAG_SCENE_TEMP_34, EVENT_SCRIPT(Event_OuterRightBlockMoved) },
    { TOUCH_WALKING, 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushBlock) },
    { TOUCH_WALKING_INTO(FACING_EAST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushBlock) },
    { TOUCH_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushBlock) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSolSanctumEntranceEvents1Entrances14To16[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { 0x21, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InnerBlock1Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InnerBlock2Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InnerBlock3Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InnerBlock4Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InnerBlock5Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InnerBlock6Moved) },
    { TOUCH_WALKING, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushBlock) },
    { USE_ITEM(ITEM_SMALL_JEWEL), 0x415, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetSecondSmallJewel) },
    { SEARCH(SEARCH_UNNAMED), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InspectSecondMinotaurRelief) },
    { SCENE_EVENTS_END },
};
// clang-format on

/// Pushes step by direction: X in the upper halfword, Z in the lower (pixels).
#define PUSH_STEP(x, z) (((x) << 16) | ((z) & 0xffff))

s32 gSolSanctumPushSteps[] = {
    PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(0, 16),
    PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(-16, 0),
    PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(0, -16),
    PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(16, 0),
};
