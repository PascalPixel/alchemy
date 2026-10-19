#include "game_state.h"
#include "version.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/crossbone_isle.h"
#include "unit/scene/common/object_orbit.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "field.h"
#include "menu.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "field_effect.h"
#include "debug.h"
#include "game.h"
#include "graphics.h"
#include "minigame.h"
#include "sound.h"
#include "edition.h"
#include "field_service.h"
#include "call.h"
#include "scene_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "field_obj.h"
#include "fixed_math.h"

// Forward declarations.
static void CrossboneIsle_RestoreMovedRoomBlock(void);
static void CrossboneIsle_LeaveClearedRoom(void);
static void CrossboneIsle_RestoreRoomActors(void);
static void CrossboneIsle_DrawBlockPuzzleFootprints(void);

// Shared source is included here in ROM order.
#include "common/object_staged_actor.h"

// Shared source is included here in ROM order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

// Unused callback with no effect.
UNUSED static void NoOpEffectCallback(void) {}

// Select the entrance list for the current island room.
const struct SceneEntrance *CrossboneIsle_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_CrossboneIsle1) {
    return gCrossboneIsleEntrances1;
  }
  if (scene == (s32)&SceneId_CrossboneIsle2) {
    return gCrossboneIsleEntrances2;
  }
  if (scene == (s32)&SceneId_CrossboneIsle3) {
    return gCrossboneIsleEntrances3;
  }
  if (scene == (s32)&SceneId_CrossboneIsle4) {
    return gCrossboneIsleEntrances4;
  }
  if (scene == (s32)&SceneId_CrossboneIsle5) {
    return gCrossboneIsleEntrances5;
  }
  return &gCrossboneIsleEntrances5[2];
}

// These rooms have no region table.
const struct SceneRegion *CrossboneIsle_GetRegions(void) { return NULL; }

// Return the connections between island rooms.
const u32 *CrossboneIsle_GetExits(void) { return gCrossboneIsleExits; }

// Choose the actors for the current room.
const struct ScenePlacement *CrossboneIsle_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_CrossboneIsle1) {
    return gCrossboneIslePlacements1;
  }
  if (scene == (s32)&SceneId_CrossboneIsle3) {
    return gCrossboneIslePlacements3;
  }
  if (scene <= (s32)&SceneId_CrossboneIsle14 &&
      scene >= (s32)&SceneId_CrossboneIsle6) {
    return gCrossboneIslePlacements6To14;
  }
  return gCrossboneIslePlacementsOther;
}

// Shared source is included here in ROM order.
#include "common/effect_advance.h"

// Emit a ring of effects around a moved block.
static void CrossboneIsle_SpawnBlockMovedBurst(s32 objectId) {
  struct FieldActor *actor;
  struct Vector3 velocity;
  struct EffectOptions options;
  u32 angleStep;
  s32 angle;
  s32 velocityX;
  s32 velocityZ;

  actor = Object_GetById(objectId);
  options.update = Effect_AdvanceMotion;
  for (angleStep = 0; angleStep <= 16; angleStep += 2) {
    angle = angleStep << 12;
    velocity.x = Engine_MathCos(angle);
    velocity.y = 0;
    velocityZ = Engine_MathSin(angle);
    velocityX = velocity.x;
    velocity.z = velocityZ;
    velocityX += Math_Divide(velocityX, 3);
    velocity.x = velocityX;
    Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocityX,
                 velocity.y, velocityZ, 0x01000001, &options);
  }
}

// Return to this entrance after the final guard encounter.
static void CrossboneIsle_StartFinalGuardBattle(void) {

  Engine_EventBegin();
  Event_SetNextDestination(gGameState.scene, 5);
  Battle_SetStart(3);
  Field_RequestBattleFromEncounterTable(84, 5);
  Engine_EventEnd();
}

// Choose the interactions for the current room.
const struct SceneEvent *CrossboneIsle_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_CrossboneIsle1) {
    return gCrossboneIsleEvents1;
  }
  if (scene == (s32)&SceneId_CrossboneIsle2) {
    return gCrossboneIsleEvents2;
  }
  if (scene == (s32)&SceneId_CrossboneIsle3) {
    return gCrossboneIsleEvents3;
  }
  if (scene == (s32)&SceneId_CrossboneIsle4) {
    return gCrossboneIsleEvents4;
  }
  if (scene == (s32)&SceneId_CrossboneIsle5) {
    return gCrossboneIsleEvents5;
  }
  return gCrossboneIsleEventsOther;
}

// Catch the nut and remove it from the room.
static void CrossboneIsle_CatchNut(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(8, 0, 0);
  Engine_GameFlagSet(4055);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

// Restore puzzle changes and handle returning from a guard battle.
s32 CrossboneIsle_Initialize(void) {
  struct FieldActor *actor;
  s32 scene;
  s32 current;
  s32 first;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  // The sliding-block room restores its map footprints independently.
  scene = gGameState.scene;
  if (scene == (s32)&SceneId_CrossboneIsle3) {
    CrossboneIsle_DrawBlockPuzzleFootprints();
    return 0;
  }
  if (scene == (s32)&SceneId_CrossboneIsle5) {
    if (GameFlag_IsSet(0xef7) == 0) {
      Map_CopyCellAttributes(0, 3, 1, 1, 13, 40);
      Map_CopyCellAttributes(0, 2, 1, 1, 15, 40);
      MapObject_SetPosition(101, PIXELS(216), PIXELS(648));
    }
    if (gGameState.scene == scene) {
      if (gGameState.entrance != 5) {
        if (GameFlag_IsSet(0x8d1) == 0) {
          return 0;
        }
      }
      GameFlag_Set(0x8d1);
      Map_CopyCellAttributes(0, 1, 1, 1, 13, 30);
      MapObject_SetPosition(100, PIXELS(216), PIXELS(488));
      return 0;
    }
  }
  current = gGameState.scene;
  if (current == (s32)&SceneId_CrossboneIsle1) {
    CrossboneIsle_RestoreRoomActors();
    Actor_Get(8)->targetX = PIXELS(129);
    FieldScene_RedrawActorFootprint(9);
    FieldScene_RedrawActorFootprint(10);
    if (GameFlag_IsSet(0x240) != 0) {
      actor = Actor_Get(11);
      if (actor != 0) {
        actor->collisionFlags = 0;
        Object_SetMode(actor, 4);
        Engine_ActorSetSpriteFlags(actor, 0);
      }
      Field_SetMapCellCollision(0, PIXELS(304), PIXELS(368), 253);
    }
    if (GameFlag_IsSet(0x241) != 0) {
      actor = Actor_Get(12);
      if (actor != 0) {
        actor->collisionFlags = 0;
        Object_SetMode(actor, 4);
        Engine_ActorSetSpriteFlags(actor, 0);
      }
      Field_SetMapCellCollision(0, PIXELS(80), PIXELS(368), 253);
    }
    if (GameFlag_IsSet(0x242) != 0) {
      actor = Actor_Get(13);
      if (actor != 0) {
        actor->collisionFlags = 0;
        Object_SetMode(actor, 4);
        Engine_ActorSetSpriteFlags(actor, 0);
      }
      Field_SetMapCellCollision(0, PIXELS(96), PIXELS(336), 253);
    }
    if (GameFlag_IsSet(0x243) != 0) {
      actor = Actor_Get(14);
      if (actor != 0) {
        actor->collisionFlags = 0;
        Object_SetMode(actor, 4);
        Engine_ActorSetSpriteFlags(actor, 0);
      }
      Field_SetMapCellCollision(0, PIXELS(144), PIXELS(320), 253);
      Field_SetMapCellCollision(0, PIXELS(752), PIXELS(320), 253);
    }
    if (GameFlag_IsSet(0xfd7) != 0) {
      return 0;
    }
    InitializeStagedActorSceneOrbitingEffect(8);
    return 0;
  }
  // Rooms 6-14 share the guard, moved-block, and return-entrance protocol.
  first = (s32)&SceneId_CrossboneIsle6;
  if (current == first) {
    if (GameFlag_IsSet(0xef4) == 0) {
      Map_CopyCellAttributes(0, 0, 1, 1, 37, 10);
      MapObject_SetPosition(100, PIXELS(600), PIXELS(168));
    }
  }
  current = gGameState.scene;
  if (current >= first) {
    if (current <= (s32)&SceneId_CrossboneIsle14) {
      CrossboneIsle_RestoreMovedRoomBlock();
      if (gGameState.entrance == 5) {
        CrossboneIsle_LeaveClearedRoom();
      }
    }
  }
  return 0;
}

// Restore the passage and hidden block when its room flag is set.
static void CrossboneIsle_RestoreMovedRoomBlock(void) {
  struct FieldActor *actor;
  s32 cleared;

  actor = Actor_Get(8);
  cleared = GameFlag_IsSet(
      (gGameState.scene + (0x8d2 - (s32)&SceneId_CrossboneIsle6)));
  if (cleared != 0) {
    Actor_SetPosition(8, PIXELS(650), PIXELS(168));
    actor->y.fixed = -PIXELS(32);
    Engine_ActorSetSpriteFlags(Actor_Get(8), 0);
    Engine_ActorSetSpritePriority(8, 3);
    actor->motionFlags = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Map_CopyCellAttributes(42, 10, 1, 1, 40, 10);
  } else {
    Object_GetById(8)->motionFlags = cleared;
  }
}

// Open the passage when Move brings the block to column 40.
static void CrossboneIsle_OpenPassageWithMovedBlock(void) {
  /* FAKEMATCH: retain the scalar scene-slot address calculation; direct
   * &gGameState.scene folds the address and removes eight bytes in tbs-en. */
  struct FieldActor *block;
  s16 *sceneNumber;
  s32 column;

  block = Object_GetById(8);
  column = block->x.fixed >> 20;
  if (column != 40) {
    return;
  }

#if TBS_PATCH_1
  {
    s32 sceneOffset = offsetof(struct GameState, scene);
    sceneNumber = (s16 *)((u8 *)&gGameState + sceneOffset);
  }
  if (Engine_GameFlagIsSet(*sceneNumber +
                           (0x8d2 - (s32)&SceneId_CrossboneIsle6)) != 0) {
    return;
  }
#endif

  // Play the moved-block reaction before hiding it and opening the passage.
  block->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;

  Engine_EventWait(8);
  CrossboneIsle_SpawnBlockMovedBurst(8);
  Engine_AudioPlayCue(136);
  Engine_EventWait(40);

  Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
  Engine_ActorSetSpritePriority(8, 3);

  block->motionFlags = 0;
  block->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;

  // Replace the blocked cell and remember the room's completed Move puzzle.
  Map_CopyCellAttributes(42, 10, 1, 1, column, 10);

#if !TBS_PATCH_1
  {
    s32 sceneOffset = offsetof(struct GameState, scene);
    sceneNumber = (s16 *)((u8 *)&gGameState + sceneOffset);
  }
#endif
  Engine_GameFlagSet(*sceneNumber + (0x8d2 - (s32)&SceneId_CrossboneIsle6));
}

// Complete the staged Move action, then test the passage.
static void CrossboneIsle_MoveBlockAndCheckPassage(void) {
  Engine_EventBegin();
  StagedActor_AdvancePair();
  Engine_EventEnd();
  CrossboneIsle_OpenPassageWithMovedBlock();
}

// Animate the left room door and enter its exit.
static void CrossboneIsle_OpenLeftRoomDoor(void) {
  Engine_MapAnimateCells(gCrossboneIsle_EntranceCells, 37, 7);
  Engine_AudioPlayCue(183);
  Engine_EventRequestExit(4);
}

// Fight this room's guards once, then open its door on later visits.
static void CrossboneIsle_EnterGuardedRoom(void) {
  if (Engine_GameFlagIsSet(gGameState.scene +
                           (0x8c8 - (s32)&SceneId_CrossboneIsle6)) == 0) {
    Engine_EventBegin();
    Event_SetNextDestination(gGameState.scene, 5);
    Battle_SetStart(3);
    // The first four rooms use encounter table 63; the next five use 84.
    switch (gGameState.scene - (s32)&SceneId_CrossboneIsle6) {
    case 0:
      Field_RequestBattleFromEncounterTable(63, 0);
      break;
    case 1:
      Field_RequestBattleFromEncounterTable(63, 1);
      break;
    case 2:
      Field_RequestBattleFromEncounterTable(63, 2);
      break;
    case 3:
      Field_RequestBattleFromEncounterTable(63, 3);
      break;
    case 4:
      Field_RequestBattleFromEncounterTable(84, 0);
      break;
    case 5:
      Field_RequestBattleFromEncounterTable(84, 1);
      break;
    case 6:
      Field_RequestBattleFromEncounterTable(84, 2);
      break;
    case 7:
      Field_RequestBattleFromEncounterTable(84, 3);
      break;
    case 8:
      Field_RequestBattleFromEncounterTable(84, 4);
      break;
    }
    Engine_EventEnd();
  } else {
    Engine_MapAnimateCells(gCrossboneIsle_EntranceCells, 44, 7);
    Engine_AudioPlayCue(183);
    Engine_EventRequestExit(3);
  }
}

// Record the guard victory and walk through the opened doorway.
static void CrossboneIsle_LeaveClearedRoom(void) {
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set((gGameState.scene + (0x8c8 - (s32)&SceneId_CrossboneIsle6)));
  Engine_EventWait(30);
  Map_AnimateCells(gCrossboneIsle_EntranceCells, 44, 7);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  Engine_EventRequestExit(3);
  Engine_EventEnd();
}

// Remove the collision at the passage opened by Growth on actor 11.
static void CrossboneIsle_OpenGrowthPassage11(s32 actorId) {
  s32 fixedX = PIXELS(304);
  s32 fixedZ = PIXELS(368);
  struct FieldActor *actor = Object_GetById(11);

  if (actor != 0) {
    actor->collisionFlags = 0;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  SetMapCellCollision(0, fixedX, fixedZ, 253);
  Engine_GameFlagSet(576);
}

// Remove the collision at the passage opened by Growth on actor 12.
static void CrossboneIsle_OpenGrowthPassage12(s32 actorId) {
  s32 fixedX = PIXELS(80);
  s32 fixedZ = PIXELS(368);
  struct FieldActor *actor = Object_GetById(12);

  if (actor != 0) {
    actor->collisionFlags = 0;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  SetMapCellCollision(0, fixedX, fixedZ, 253);
  Engine_GameFlagSet(577);
}

// Remove the collision at the passage opened by Growth on actor 13.
static void CrossboneIsle_OpenGrowthPassage13(s32 actorId) {
  s32 fixedX = PIXELS(96);
  s32 fixedZ = PIXELS(336);
  struct FieldActor *actor = Object_GetById(13);

  if (actor != 0) {
    actor->collisionFlags = 0;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  SetMapCellCollision(0, fixedX, fixedZ, 253);
  Engine_GameFlagSet(578);
}

// Remove the collision at the passage opened by Growth on actor 14.
static void CrossboneIsle_OpenGrowthPassage14(s32 actorId) {
  s32 fixedX = PIXELS(144);
  s32 fixedZ = PIXELS(320);
  s32 secondFixedX = PIXELS(752);
  s32 secondFixedZ = PIXELS(320);
  struct FieldActor *actor = Object_GetById(14);

  if (actor != 0) {
    actor->collisionFlags = 0;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  SetMapCellCollision(0, fixedX, fixedZ, 253);
  SetMapCellCollision(0, secondFixedX, secondFixedZ, 253);
  Engine_GameFlagSet(579);
}

// Remember that Whirlwind cleared obstacle 15.
static void CrossboneIsle_ClearWhirlwindObstacle15(void) {
  Engine_GameFlagSet(2244);
  {

    Map_CopyCellAttributes(0, 0, 1, 1, 8, 21);
  }
}

// Remember that Whirlwind cleared obstacle 16.
static void CrossboneIsle_ClearWhirlwindObstacle16(void) {
  Engine_GameFlagSet(2245);
}

// Remember that Whirlwind cleared obstacle 17.
static void CrossboneIsle_ClearWhirlwindObstacle17(void) {
  Engine_GameFlagSet(2246);
}

// Remember that Whirlwind cleared obstacle 18.
static void CrossboneIsle_ClearWhirlwindObstacle18(void) {
  Engine_GameFlagSet(2247);
}

// Restore the Whirlwind obstacles from their persistent flags.
static void CrossboneIsle_RestoreRoomActors(void) {
  struct FieldActor *actor;

  if (GameFlag_IsSet(0x8c4) != 0) {
    Map_CopyCellAttributes(0, 0, 1, 1, 8, 21);
    Actor_SetPosition(15, PIXELS(968), PIXELS(968));
  } else {
    actor = Actor_Get(15);
    actor->scaleY = 8 * Q16_ONE / 5;
  }
  if (GameFlag_IsSet(0x8c5) != 0) {
    Actor_SetPosition(16, PIXELS(968), PIXELS(968));
  } else {
    actor = Actor_Get(16);
    actor->scaleY = 8 * Q16_ONE / 5;
  }
  if (GameFlag_IsSet(0x8c6) != 0) {
    Actor_SetPosition(17, PIXELS(968), PIXELS(968));
  } else {
    actor = Actor_Get(17);
    actor->scaleY = 8 * Q16_ONE / 5;
  }
  if (GameFlag_IsSet(0x8c7) != 0) {
    Actor_SetPosition(18, PIXELS(968), PIXELS(968));
  } else {
    actor = Actor_Get(18);
    actor->scaleY = 8 * Q16_ONE / 5;
  }
}

// Move the selected staged actor when a clear destination exists.
static void CrossboneIsle_MoveSelectedStagedActor(void) {
  struct StagedActorProbe probe;

  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
  }
  Engine_EventEnd();
}

// Move the selected actor and puzzle block by the same pixel offset.
static void CrossboneIsle_MoveSelectedActorAndBlock(s32 actorId, s32 xOffset,
                                                    s32 zOffset) {
  struct FieldActor *selectedActor;
  struct FieldActor *block;
  s32 fixedX;
  s32 fixedZ;

  selectedActor = Object_GetById(gGameState.selectedActor);
  block = Object_GetById(actorId);
  Engine_EventBegin();
  {
    fixedX = ((selectedActor->x.fixed + (xOffset * PIXELS(1))) & 0xFFF00000) +
             PIXELS(8);
    fixedZ = ((selectedActor->z.fixed + (zOffset * PIXELS(1))) & 0xFFF00000) +
             PIXELS(8);

    selectedActor->speed = PIXELS(1);
    selectedActor->acceleration = PIXELS(1) / 2;
    Engine_ObjectSetPosition(selectedActor, fixedX, selectedActor->y.fixed,
                             fixedZ);
  }
  // Position the block on the same cell-center grid as the selected actor.
  Object_SetMode(selectedActor, 27);
  {
    fixedX =
        ((block->x.fixed + (xOffset * PIXELS(1))) & 0xFFF00000) + PIXELS(8);
    fixedZ =
        ((block->z.fixed + (zOffset * PIXELS(1))) & 0xFFF00000) + PIXELS(8);

    block->speed = PIXELS(1);
    block->acceleration = PIXELS(1) / 2;
    Engine_ObjectSetPosition(block, fixedX, block->y.fixed, fixedZ);
  }
  // The block's mode depends on which side of the grid it moves toward.
  if (xOffset < 0 || zOffset < 0) {
    Object_SetMode(block, 4);
  } else {
    Object_SetMode(block, 3);
  }
  Engine_AudioPlayCue(226);
  FieldObject_WaitForMove(selectedActor);
  Engine_AudioPlayCue(288);
  Engine_EventEnd();
}

// Draw the three-cell horizontal footprint centered on this block.
static void CrossboneIsle_DrawHorizontalBlockFootprint(s32 actorId,
                                                       s32 sourceColumn,
                                                       s32 sourceRow,
                                                       s32 unused) {
  struct FieldActor *actor = Object_GetById(actorId);

  if (actor != 0) {
    s32 priorityFlags;

    Engine_ActorSetSpritePriority(actorId, 3);
    priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    priorityFlags |= actor->priorityFlags;
    actor->priorityFlags = priorityFlags;
    {
      s32 row = actor->z.fixed >> 20;
      s32 column = (actor->x.fixed >> 20) - 1;

      Map_CopyCellAttributes(sourceColumn, sourceRow, 3, 1, column, row);
    }
  }
}

// Draw the three-cell vertical footprint centered on this block.
static void CrossboneIsle_DrawVerticalBlockFootprint(s32 actorId,
                                                     s32 sourceColumn,
                                                     s32 sourceRow,
                                                     s32 unused) {
  struct FieldActor *actor = Object_GetById(actorId);

  if (actor != 0) {
    s32 priorityFlags;

    Engine_ActorSetSpritePriority(actorId, 3);
    priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    priorityFlags |= actor->priorityFlags;
    actor->priorityFlags = priorityFlags;
    {
      s32 column = actor->x.fixed >> 20;
      s32 row = (actor->z.fixed >> 20) - 1;

      Map_CopyCellAttributes(sourceColumn, sourceRow, 1, 3, column, row);
    }
  }
}

// Run the two animation changes used when touching a puzzle block.
static void CrossboneIsle_AnimateTouchedBlock(int actorId) {
  Engine_ActorSetAnimation(actorId, 1);
  Engine_ActorSetAnimation(actorId, 2);
}

// Draw all twelve block footprints in the sliding-block room.
static void CrossboneIsle_DrawBlockPuzzleFootprints(void) {
  CrossboneIsle_DrawHorizontalBlockFootprint(8, 17, 30, 21);
  CrossboneIsle_DrawHorizontalBlockFootprint(10, 17, 31, 22);
  CrossboneIsle_DrawVerticalBlockFootprint(11, 20, 30, 23);
  CrossboneIsle_DrawVerticalBlockFootprint(12, 21, 30, 24);
  CrossboneIsle_DrawVerticalBlockFootprint(13, 22, 30, 25);
  CrossboneIsle_DrawVerticalBlockFootprint(15, 23, 30, 26);
  CrossboneIsle_DrawHorizontalBlockFootprint(17, 0, 30, 31);
  CrossboneIsle_DrawHorizontalBlockFootprint(18, 0, 31, 32);
  CrossboneIsle_DrawHorizontalBlockFootprint(9, 0, 32, 33);
  CrossboneIsle_DrawVerticalBlockFootprint(19, 4, 30, 34);
  CrossboneIsle_DrawVerticalBlockFootprint(14, 5, 30, 35);
  CrossboneIsle_DrawVerticalBlockFootprint(16, 6, 30, 36);
}

// Animate puzzle block 8 when its footprint is touched.
static void ActivateSceneActor8(void) { CrossboneIsle_AnimateTouchedBlock(8); }

// Animate puzzle block 10 when its footprint is touched.
static void ActivateSceneActor10(void) {
  CrossboneIsle_AnimateTouchedBlock(10);
}

// Animate puzzle block 11 when its footprint is touched.
static void ActivateSceneActor11(void) {
  CrossboneIsle_AnimateTouchedBlock(11);
}

// Animate puzzle block 12 when its footprint is touched.
static void ActivateSceneActor12(void) {
  CrossboneIsle_AnimateTouchedBlock(12);
}

// Animate puzzle block 13 when its footprint is touched.
static void ActivateSceneActor13(void) {
  CrossboneIsle_AnimateTouchedBlock(13);
}

// Animate puzzle block 15 when its footprint is touched.
static void ActivateSceneActor15(void) {
  CrossboneIsle_AnimateTouchedBlock(15);
}

// Animate puzzle block 17 when its footprint is touched.
static void ActivateSceneActor17(void) {
  CrossboneIsle_AnimateTouchedBlock(17);
}

// Animate puzzle block 18 when its footprint is touched.
static void ActivateSceneActor18(void) {
  CrossboneIsle_AnimateTouchedBlock(18);
}

// Animate puzzle block 9 when its footprint is touched.
static void ActivateSceneActor9(void) { CrossboneIsle_AnimateTouchedBlock(9); }

// Animate puzzle block 19 when its footprint is touched.
static void ActivateSceneActor19(void) {
  CrossboneIsle_AnimateTouchedBlock(19);
}

// Animate puzzle block 14 when its footprint is touched.
static void ActivateSceneActor14(void) {
  CrossboneIsle_AnimateTouchedBlock(14);
}

// Animate puzzle block 16 when its footprint is touched.
static void ActivateSceneActor16(void) {
  CrossboneIsle_AnimateTouchedBlock(16);
}

// Test a step, then move the leader with the block-pushing animation.
static s32 TryPushBlockingSceneActor(struct FieldActor *actor,
                                     struct FieldPosition *requested) {
  u8 *motionFlags = &actor->motionFlags;
  s32 savedMotionFlags = *motionFlags;
#if TBS_PATCH_1
  struct FieldPosition destination;

  destination.x = (actor->x.fixed & 0xfff00000) + PIXELS(8);
  destination.y = actor->y.fixed;
  destination.z = (actor->z.fixed & 0xfff00000) + PIXELS(8);
  {
    s32 direction = (actor->facing + 0x2000) & 0xc000;

    Vector_AddPolarOffset(PIXELS(32), direction, &destination);
  }
  // Keep the original motion flags while playing the leader's push step.
  if (FieldObject_CheckMovementCollision(actor, &destination) == 0) {
#else
  if (FieldObject_CheckMovementCollision(actor, requested) == 0) {
#endif
    s32 movingFlags;

    Engine_EventBegin();
    Object_SetMode(actor, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(152);
    Object_SetMode(actor, 7);
    actor->speed = PIXELS(3);
    actor->acceleration = PIXELS(2);
    actor->velocityY = PIXELS(4);
    movingFlags = 126;
    movingFlags &= *motionFlags;
    *motionFlags = (u8)movingFlags;
    Engine_ActorSetSpriteFlags(actor, 0);
    {
#if TBS_PATCH_1
      s16 *pixelCoordinates = (s16 *)&destination;
#else
      s16 *pixelCoordinates = (s16 *)requested;
#endif

      Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, pixelCoordinates[1],
                                pixelCoordinates[5]);
    }
    Object_SetMode(actor, 6);
    Engine_ActorSetSpriteFlags(actor, 1);
    *motionFlags = (u8)savedMotionFlags;
    Engine_EventEnd();
    return 1;
  }
  return 0;
}

// Try a step 32 pixels toward smaller Z coordinates.
static s32 CheckActorPathSouth(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed;
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed + -PIXELS(32);
  return TryPushBlockingSceneActor(actor, &destination);
}

// Try a step 32 pixels toward larger Z coordinates.
static s32 CheckActorPathNorth(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed;
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed + PIXELS(32);
  return TryPushBlockingSceneActor(actor, &destination);
}

// Try a step 32 pixels toward smaller X coordinates.
static s32 CheckActorPathWest(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed + -PIXELS(32);
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed;
  return TryPushBlockingSceneActor(actor, &destination);
}

// Try a step 32 pixels toward larger X coordinates.
static s32 CheckActorPathEast(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed + PIXELS(32);
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed;
  return TryPushBlockingSceneActor(actor, &destination);
}

// Move block 8 toward smaller Z, accounting for blocks 12 and 15.
static void UpdateEscapeRouteForActorPositions(void) {
  s32 actorX = Object_GetById(8)->x.fixed >> 20;
  s32 actorZ = Object_GetById(8)->z.fixed >> 20;
  s32 actor12X = Object_GetById(12)->x.fixed >> 20;
  s32 actor15X = Object_GetById(15)->x.fixed >> 20;

  if (actorZ == 19) {
    if (actor12X == 24) {
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -80);
    } else if (actor15X == 24) {
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -112);
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -80);
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -112);
    }
  } else if (actorZ == 14) {
    if (actor12X == 24) {
      return;
    }
    if (actor15X == 24) {
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -64);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -112);
    }
  } else if (actorZ == 10) {
    if (actor15X == 24) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -48);
  } else {
    CheckActorPathSouth();
    return;
  }
  Engine_TaskWait(2);
  {
    s32 routeEndZ = Object_GetById(8)->z.fixed >> 20;
    s32 routeX = actorX - 1;

    Map_CopyCellAttributes(routeX, actorZ, 3, 1, routeX, routeEndZ);
  }
  Map_CopyCellAttributes(0, 0, 3, 1, actorX - 1, actorZ);
}

// Copy a horizontal block's old footprint to its new row, then clear the old
// row.
static inline void
CrossboneIsle_RedrawHorizontalBlockRoute(s32 actorId, s32 column, s32 row) {
  Engine_TaskWait(2);
  {
    s32 destinationRow = Object_GetById(actorId)->z.fixed >> 20;
    s32 footprintColumn = column - 1;

    Map_CopyCellAttributes(footprintColumn, row, 3, 1, footprintColumn,
                           destinationRow);
  }
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

// Copy a vertical block's old footprint to its new column, then clear the old
// one.
static inline void CrossboneIsle_RedrawVerticalBlockRoute(s32 actorId,
                                                          s32 column, s32 row) {
  Engine_TaskWait(2);
  {
    s32 destinationColumn = Object_GetById(actorId)->x.fixed >> 20;
    s32 footprintRow = row - 1;

    Map_CopyCellAttributes(column, footprintRow, 1, 3, destinationColumn,
                           footprintRow);
  }
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

// Move block 8 along its return route and redraw its footprint.
static void UpdateActor8ReturnRoute(void) {
  s32 column = Object_GetById(8)->x.fixed >> 20;
  s32 row = Object_GetById(8)->z.fixed >> 20;
  s32 block12Column = Object_GetById(12)->x.fixed >> 20;

  if (row == 7) {
    if (block12Column == 24) {
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, 80);
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, 112);
    }
  } else if (row == 10) {
    if (block12Column == 24) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(8, 0, 144);
  } else if (row == 14) {
    CrossboneIsle_MoveSelectedActorAndBlock(8, 0, 80);
  } else {
    return;
  }
  CrossboneIsle_RedrawHorizontalBlockRoute(8, column, row);
}

// Move block 10 along its retreat route and redraw its footprint.
static void UpdateActor10RetreatRoute(void) {
  s32 column = Object_GetById(10)->x.fixed >> 20;
  s32 row = Object_GetById(10)->z.fixed >> 20;
  s32 block13Column = Object_GetById(13)->x.fixed >> 20;
  s32 block15Column = Object_GetById(15)->x.fixed >> 20;

  if (row == 18) {
    if (block15Column >= 31 && block15Column <= 33) {
      CrossboneIsle_MoveSelectedActorAndBlock(10, 0, -128);
    } else if (block13Column >= 31 && block13Column <= 33) {
      CrossboneIsle_MoveSelectedActorAndBlock(10, 0, -128);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(10, 0, -112);
      CrossboneIsle_MoveSelectedActorAndBlock(10, 0, -64);
    }
  } else if (row == 10) {
    if (block15Column >= 31 && block15Column <= 33) {
      return;
    }
    if (block13Column >= 31 && block13Column <= 33) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(10, 0, -48);
  } else if (row == 7) {
    return;
  }
  CrossboneIsle_RedrawHorizontalBlockRoute(10, column, row);
}

// Move block 10 along its advance route and redraw its footprint.
static void UpdateActor10AdvanceRoute(void) {
  s32 column = Object_GetById(10)->x.fixed >> 20;
  s32 row = Object_GetById(10)->z.fixed >> 20;

  if (row != 18) {
    if (row == 10) {
      CrossboneIsle_MoveSelectedActorAndBlock(10, 0, 128);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(10, 0, 112);
      CrossboneIsle_MoveSelectedActorAndBlock(10, 0, 64);
    }
    CrossboneIsle_RedrawHorizontalBlockRoute(10, column, row);
  }
}

// Move block 11 along its west route and redraw its footprint.
static void UpdateActor11WestRoute(void) {
  s32 column = Object_GetById(11)->x.fixed >> 20;
  s32 row = Object_GetById(11)->z.fixed >> 20;

  if (column != 30) {
    if (column == 34) {
      if ((Object_GetById(10)->z.fixed >> 20) == 18) {
        return;
      }
      CrossboneIsle_MoveSelectedActorAndBlock(11, -64, 0);
    } else if (column == 36) {
      if ((Object_GetById(10)->z.fixed >> 20) == 18) {
        CrossboneIsle_MoveSelectedActorAndBlock(11, -32, 0);
      } else {
        CrossboneIsle_MoveSelectedActorAndBlock(11, -96, 0);
      }
    }
    CrossboneIsle_RedrawVerticalBlockRoute(11, column, row);
  }
}

// Move block 11 along its east route and redraw its footprint.
static void UpdateActor11EastRoute(void) {
  s32 column = Object_GetById(11)->x.fixed >> 20;
  s32 row = Object_GetById(11)->z.fixed >> 20;

  if (column != 36) {
    if (column == 30) {
      if ((Object_GetById(10)->z.fixed >> 20) == 18) {
        return;
      }
      CrossboneIsle_MoveSelectedActorAndBlock(11, 96, 0);
    } else if (column == 34) {
      CrossboneIsle_MoveSelectedActorAndBlock(11, 32, 0);
    }
    CrossboneIsle_RedrawVerticalBlockRoute(11, column, row);
  }
}

// Move block 12 along its west route and redraw its footprint.
static void UpdateActor12WestRoute(void) {
  s32 column = Object_GetById(12)->x.fixed >> 20;
  s32 row = Object_GetById(12)->z.fixed >> 20;

  if (column == 36) {
    CrossboneIsle_MoveSelectedActorAndBlock(12, -96, 0);
    CrossboneIsle_MoveSelectedActorAndBlock(12, -96, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndBlock(12, -96, 0);
    CrossboneIsle_MoveSelectedActorAndBlock(12, -64, 0);
  } else if (column == 24) {
    return;
  }
  CrossboneIsle_RedrawVerticalBlockRoute(12, column, row);
}

// Move block 12 along its east route and redraw its footprint.
static void UpdateActor12EastRoute(void) {
  s32 column = Object_GetById(12)->x.fixed >> 20;
  s32 row = Object_GetById(12)->z.fixed >> 20;

  if (column == 24) {
    CrossboneIsle_MoveSelectedActorAndBlock(12, 96, 0);
    CrossboneIsle_MoveSelectedActorAndBlock(12, 96, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndBlock(12, 32, 0);
  } else if (column == 36) {
    return;
  }
  CrossboneIsle_RedrawVerticalBlockRoute(12, column, row);
}

// Move block 13 along its west route and redraw its footprint.
static void UpdateActor13WestRoute(void) {
  s32 column = Object_GetById(13)->x.fixed >> 20;
  s32 row = Object_GetById(13)->z.fixed >> 20;
  s32 block10Row = Object_GetById(10)->z.fixed >> 20;
  s32 block15Column = Object_GetById(15)->x.fixed >> 20;

  if (column == 36) {
    if (block15Column == 34) {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -16, 0);
    } else if (block10Row == 7) {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -32, 0);
    } else if (block15Column == 30) {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -96, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(13, -80, 0);
    }
  } else if (column == 35) {
    if (block15Column == 34) {
      return;
    }
    if (block10Row == 7) {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -16, 0);
    } else if (block15Column == 30) {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -80, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(13, -80, 0);
    }
  } else if (column == 34) {
    if (block10Row == 7) {
      return;
    }
    if (block15Column == 30) {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -144, 0);
    }
  } else if (column == 31) {
    if (block15Column == 30) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(13, -96, 0);
  } else if (column == 25) {
    return;
  }
  CrossboneIsle_RedrawVerticalBlockRoute(13, column, row);
}

// Move block 13 along its east route and redraw its footprint.
static void UpdateActor13EastRoute(void) {
  s32 column = Object_GetById(13)->x.fixed >> 20;
  s32 row = Object_GetById(13)->z.fixed >> 20;

  Object_GetById(15);
  if (column == 25) {
    CrossboneIsle_MoveSelectedActorAndBlock(13, 96, 0);
    CrossboneIsle_MoveSelectedActorAndBlock(13, 80, 0);
  } else if (column == 31) {
    CrossboneIsle_MoveSelectedActorAndBlock(13, 80, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndBlock(13, 32, 0);
  } else if (column == 35) {
    CrossboneIsle_MoveSelectedActorAndBlock(13, 16, 0);
  } else if (column == 36) {
    return;
  }
  CrossboneIsle_RedrawVerticalBlockRoute(13, column, row);
}

// Move block 15 along its west route and redraw its footprint.
static void UpdateActor15WestRoute(void) {
  s32 column = Object_GetById(15)->x.fixed >> 20;
  s32 row = Object_GetById(15)->z.fixed >> 20;
  s32 block8Row = Object_GetById(8)->z.fixed >> 20;
  s32 block10Row = Object_GetById(10)->z.fixed >> 20;

  if (column == 35) {
    if (block10Row == 7) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, -16, 0);
    } else if (block8Row == 7) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, -112, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(15, -96, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(15, -80, 0);
    }
  } else if (column == 34) {
    if (block10Row == 7) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(15, -96, 0);
    CrossboneIsle_MoveSelectedActorAndBlock(15, -64, 0);
  } else if (column == 33) {
    CrossboneIsle_MoveSelectedActorAndBlock(15, -144, 0);
  } else if (column == 31) {
    CrossboneIsle_MoveSelectedActorAndBlock(15, -80, 0);
  } else if (column == 30) {
    CrossboneIsle_MoveSelectedActorAndBlock(15, -96, 0);
  } else if (column == 24) {
    return;
  }
  CrossboneIsle_RedrawVerticalBlockRoute(15, column, row);
}

// Move block 15 along its east route and redraw its footprint.
static void UpdateActor15EastRoute(void) {
  s32 column = Object_GetById(15)->x.fixed >> 20;
  s32 row = Object_GetById(15)->z.fixed >> 20;
  s32 block10Row = Object_GetById(10)->z.fixed >> 20;
  s32 block13Column = Object_GetById(13)->x.fixed >> 20;

  if (column == 24) {
    if (block10Row == 7 || block13Column == 31) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 96, 0);
    } else if (block13Column == 34) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 64, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(15, 80, 0);
    } else if (block13Column == 35) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 80, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(15, 80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 80, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(15, 96, 0);
    }
  } else if (column == 30 || block13Column == 31) {
    if (block10Row == 7) {
      return;
    }
    if (block13Column == 34) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 48, 0);
    } else if (block13Column == 35) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 80, 0);
    }
  } else if (column == 33) {
    if (block13Column == 34) {
      return;
    }
    if (block13Column == 35) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 16, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 32, 0);
    }
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndBlock(15, 16, 0);
  } else if (column == 35) {
    return;
  }
  CrossboneIsle_RedrawVerticalBlockRoute(15, column, row);
}

// Move block 17 along its south route and redraw its footprint.
static void UpdateActor17SouthRoute(void) {
  s32 column = Object_GetById(17)->x.fixed >> 20;
  s32 row = Object_GetById(17)->z.fixed >> 20;
  s32 block19Column = Object_GetById(19)->x.fixed >> 20;

  if (row == 19) {
    if (block19Column >= 3 && block19Column <= 5) {
      CrossboneIsle_MoveSelectedActorAndBlock(17, 0, -16);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(17, 0, -64);
    }
  } else if (row == 18) {
    if (block19Column >= 3 && block19Column <= 5) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(17, 0, -48);
  } else if (row == 15) {
    return;
  }
  CrossboneIsle_RedrawHorizontalBlockRoute(17, column, row);
}

// Move block 17 along its north route and redraw its footprint.
static void UpdateActor17NorthRoute(void) {
  s32 column = Object_GetById(17)->x.fixed >> 20;
  s32 row = Object_GetById(17)->z.fixed >> 20;

  if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndBlock(17, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndBlock(17, 0, 16);
  } else if (row == 19) {
    return;
  }
  CrossboneIsle_RedrawHorizontalBlockRoute(17, column, row);
}

// These three columns form the crossing lane for the vertical blocks.
#define IS_IN_BLOCK_CROSSING_COLUMNS(x) ((x) >= 6 && (x) <= 8)

// Move block 18 toward smaller Z until another block stops it.
static void CrossboneIsle_MoveBlock18North(void) {
  s32 column;
  s32 row;
  s32 block19Column;
  s32 block14Column;
  s32 block16Column;

  column = Object_GetById(18)->x.fixed >> 20;
  row = Object_GetById(18)->z.fixed >> 20;
  block19Column = Object_GetById(19)->x.fixed >> 20;
  block14Column = Object_GetById(14)->x.fixed >> 20;
  block16Column = Object_GetById(16)->x.fixed >> 20;
  if (row == 19) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block19Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -16);
    } else if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -64);
    } else if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -112);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -64);
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -96);
    }
  } else if (row == 18) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block19Column)) {
      return;
    }
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -48);
    } else if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -96);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -144);
    }
  } else if (row == 15) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      return;
    }
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -48);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -96);
    }
  } else if (row == 14) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      return;
    }
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -80);
    }
  } else if (row == 12) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -48);
  } else if (row == 11) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -32);
  } else if (row == 9) {
    return;
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(18)->z.fixed >> 20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_BLOCK_CROSSING_COLUMNS

// Advance block 18 along the route selected by neighboring blocks.
static void AdvanceActor18AlongEscapeRoute(void) {
  s32 column;
  s32 row;
  s32 companion19Column;
  s32 companion14Column;

  column = Object_GetById(18)->x.fixed >> 20;
  row = Object_GetById(18)->z.fixed >> 20;
  companion19Column = Object_GetById(19)->x.fixed >> 20;
  companion14Column = Object_GetById(14)->x.fixed >> 20;

  if (row == 9) {
    if ((u32)(companion14Column - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 32);
    } else if ((u32)(companion19Column - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 80);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 64);
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 96);
    }
  } else if (row == 11) {
    if ((u32)(companion14Column - 6) <= 2) {
      return;
    }
    if ((u32)(companion19Column - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 128);
    }
  } else if (row == 12) {
    if ((u32)(companion19Column - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 32);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 112);
    }
  } else if (row == 14) {
    if ((u32)(companion19Column - 6) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 80);
  } else if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 16);
  } else if (row == 19) {
    return;
  }

  Engine_TaskWait(2);

  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(18)->z.fixed >> 20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_BLOCK_CROSSING_COLUMNS
#define IS_IN_BLOCK_CROSSING_COLUMNS(x) ((x) >= 9 && (x) <= 11)

// Move block 9 toward smaller Z until another block stops it.
static void CrossboneIsle_MoveBlock9North(void) {
  s32 column;
  s32 row;
  s32 block19Column;
  s32 block14Column;
  s32 block16Column;

  column = Object_GetById(9)->x.fixed >> 20;
  row = Object_GetById(9)->z.fixed >> 20;
  block19Column = Object_GetById(19)->x.fixed >> 20;
  block14Column = Object_GetById(14)->x.fixed >> 20;
  block16Column = Object_GetById(16)->x.fixed >> 20;
  if (row == 19) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block19Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -16);
    } else if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -64);
    } else if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -112);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -80);
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -96);
    }
  } else if (row == 18) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block19Column)) {
      return;
    }
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -48);
    } else if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -96);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -96);
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -64);
    }
  } else if (row == 15) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      return;
    }
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -48);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -112);
    }
  } else if (row == 14) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      return;
    }
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -96);
    }
  } else if (row == 12) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -64);
  } else if (row == 11) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -48);
  } else if ((u32)row <= 9) {
    return;
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(9)->z.fixed >> 20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_BLOCK_CROSSING_COLUMNS

// Advance block 9 along the route selected by neighboring blocks.
static void AdvanceActor9AlongEscapeRoute(void) {
  s32 column;
  s32 row;
  s32 companion19Column;
  s32 companion14Column;
  s32 companion16Column;

  column = Object_GetById(9)->x.fixed >> 20;
  row = Object_GetById(9)->z.fixed >> 20;
  companion19Column = Object_GetById(19)->x.fixed >> 20;
  companion14Column = Object_GetById(14)->x.fixed >> 20;
  companion16Column = Object_GetById(16)->x.fixed >> 20;

  if (row == 8) {
    if ((u32)(companion16Column - 9) <= 2) {
      return;
    }
    if ((u32)(companion14Column - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 48);
    } else {
      if ((u32)(companion19Column - 9) > 2) {
        CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 80);
      }
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 96);
    }
  } else if (row == 11) {
    if ((u32)(companion14Column - 9) <= 2) {
      return;
    }
    if ((u32)(companion19Column - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 128);
    }
  } else if (row == 12) {
    if ((u32)(companion14Column - 9) <= 2) {
      return;
    }
    if ((u32)(companion19Column - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 32);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 112);
    }
  } else if (row == 14) {
    if ((u32)(companion19Column - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 80);
  } else if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 16);
  }

  Engine_TaskWait(2);

  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(9)->z.fixed >> 20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

// Move block 19 toward smaller X until another block stops it.
static void CrossboneIsle_MoveBlock19West(void) {
  s32 column;
  s32 row;
  s32 block17Row;
  s32 block18Row;
  s32 block9Row;

  column = Object_GetById(19)->x.fixed >> 20;
  row = Object_GetById(19)->z.fixed >> 20;
  block17Row = Object_GetById(17)->z.fixed >> 20;
  block18Row = Object_GetById(18)->z.fixed >> 20;
  block9Row = Object_GetById(9)->z.fixed >> 20;
  if (column == 3) {
    return;
  }
  if (column == 13) {
    if (block9Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -16, 0);
    } else if (block18Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -64, 0);
    } else if (block17Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -112, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -112, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(19, -48, 0);
    }
  } else if (column == 6) {
    if (block17Row == 15) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(19, -48, 0);
  } else if (column == 5) {
    CrossboneIsle_MoveSelectedActorAndBlock(19, -32, 0);
  } else if (column == 8) {
    if (block18Row == 15) {
      return;
    }
    if (block17Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -80, 0);
    }
  } else if (column == 9) {
    if (block18Row == 15) {
      return;
    }
    if (block17Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -96, 0);
    }
  } else if (column == 12) {
    if (block9Row == 15) {
      return;
    }
    if (block18Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -48, 0);
    } else if (block17Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -96, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -144, 0);
    }
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(column, row - 1, 1, 3,
                               Object_GetById(19)->x.fixed >> 20, row - 1);
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

// Move block 19 toward larger X until another block stops it.
static void CrossboneIsle_MoveBlock19East(void) {
  s32 column;
  s32 row;
  s32 block18Row;
  s32 block9Row;

  column = Object_GetById(19)->x.fixed >> 20;
  row = Object_GetById(19)->z.fixed >> 20;
  block18Row = Object_GetById(18)->z.fixed >> 20;
  block9Row = Object_GetById(9)->z.fixed >> 20;
  if (column == 3) {
    if (block18Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, 32, 0);
    } else if (block9Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, 80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(19, 112, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(19, 48, 0);
    }
  } else if (column == 5) {
    if (block18Row == 15) {
      return;
    }
    if (block9Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, 48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(19, 128, 0);
    }
  } else if (column == 6) {
    if (block9Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, 32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(19, 112, 0);
    }
  } else if (column == 8) {
    if (block9Row == 15) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(19, 80, 0);
  } else if (column == 9) {
    CrossboneIsle_MoveSelectedActorAndBlock(19, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndBlock(19, 16, 0);
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(column, row - 1, 1, 3,
                               Object_GetById(19)->x.fixed >> 20, row - 1);
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

// Retreat with block 14 along the route selected by neighboring blocks.
static void RetreatActor14AlongEscapeRoute(void) {
  s32 column;
  s32 row;
  s32 companion18Row;
  s32 companion9Row;
  s32 footprintRow;

  column = Object_GetById(14)->x.fixed >> 20;
  row = Object_GetById(14)->z.fixed >> 20;
  companion18Row = Object_GetById(18)->z.fixed >> 20;
  companion9Row = Object_GetById(9)->z.fixed >> 20;

  if (column == 13) {
    if ((u32)(companion9Row - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(14, -16, 0);
    } else if ((u32)(companion18Row - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(14, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(14, -112, 0);
    }
  } else if (column == 12) {
    if ((u32)(companion9Row - 12) <= 2) {
      return;
    }
    if ((u32)(companion18Row - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(14, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(14, -96, 0);
    }
  } else if (column == 9) {
    if ((u32)(companion18Row - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(14, -48, 0);
  } else if (column == 8) {
    if ((u32)(companion18Row - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(14, -32, 0);
  } else if (column == 6) {
    return;
  }

  Engine_TaskWait(2);

  footprintRow = row - 1;
  Engine_MapCopyCellAttributes(column, footprintRow, 1, 3,
                               Object_GetById(14)->x.fixed >> 20, footprintRow);
  Map_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

// Advance block 14 along the route selected by neighboring blocks.
static void AdvanceActor14AlongEscapeRoute(void) {
  s32 column;
  s32 row;
  s32 companion18Row;
  s32 companion9Row;
  s32 footprintRow;

  column = Object_GetById(14)->x.fixed >> 20;
  row = Object_GetById(14)->z.fixed >> 20;
  companion18Row = Object_GetById(18)->z.fixed >> 20;
  companion9Row = Object_GetById(9)->z.fixed >> 20;

  if (column == 6) {
    if ((u32)(companion9Row - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(14, 32, 0);
    } else if ((u32)(companion18Row - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(14, 64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(14, 112, 0);
    }
  } else if (column == 8) {
    if ((u32)(companion9Row - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(14, 80, 0);
  } else if (column == 9) {
    if ((u32)(companion9Row - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(14, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndBlock(14, 16, 0);
  } else if (column == 13) {
    return;
  }

  Engine_TaskWait(2);

  footprintRow = row - 1;
  Engine_MapCopyCellAttributes(column, footprintRow, 1, 3,
                               Object_GetById(14)->x.fixed >> 20, footprintRow);
  Map_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

// Retreat with block 16 along the route selected by neighboring blocks.
static void RetreatActor16AlongEscapeRoute(void) {
  s32 column;
  s32 row;
  s32 companion18Row;
  s32 companion9Row;
  s32 footprintRow;

  column = Object_GetById(16)->x.fixed >> 20;
  row = Object_GetById(16)->z.fixed >> 20;
  companion18Row = Object_GetById(18)->z.fixed >> 20;
  companion9Row = Object_GetById(9)->z.fixed >> 20;

  if (column == 13) {
    if ((u32)(companion9Row - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(16, -16, 0);
    } else if ((u32)(companion18Row - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(16, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(16, -112, 0);
    }
  } else if (column == 12) {
    if ((u32)(companion9Row - 9) <= 2) {
      return;
    }
    if ((u32)(companion18Row - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(16, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(16, -96, 0);
    }
  } else if (column == 9) {
    if ((u32)(companion18Row - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(16, -48, 0);
  } else if (column == 8) {
    CrossboneIsle_MoveSelectedActorAndBlock(16, -32, 0);
  } else if (column == 6) {
    return;
  }

  Engine_TaskWait(2);

  footprintRow = row - 1;
  Engine_MapCopyCellAttributes(column, footprintRow, 1, 3,
                               Object_GetById(16)->x.fixed >> 20, footprintRow);
  Map_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

// Advance block 16 along the route selected by neighboring blocks.
static void AdvanceActor16AlongEscapeRoute(void) {
  s32 column;
  s32 row;
  s32 companionRow;

  column = Object_GetById(16)->x.fixed >> 20;
  row = Object_GetById(16)->z.fixed >> 20;
  companionRow = Object_GetById(9)->z.fixed >> 20;

  if (column == 6) {
    if ((u32)(companionRow - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(16, 32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(16, 112, 0);
    }
  } else if (column == 8) {
    if ((u32)(companionRow - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(16, 80, 0);
  } else if (column == 9) {
    CrossboneIsle_MoveSelectedActorAndBlock(16, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndBlock(16, 16, 0);
  } else if (column == 13) {
    return;
  }

  Engine_TaskWait(2);

  Engine_MapCopyCellAttributes(column, row - 1, 1, 3,
                               Object_GetById(16)->x.fixed >> 20, row - 1);
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

// Shared source is included here in ROM order.
#include "common/object_orbit.h"

// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gEffectScript0[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

u16 gCrossboneIsle_EntranceCells[] = {
    MAP_CELL_STEP(58, 23, 1, 2, 5), MAP_CELL_STEP(59, 23, 1, 2, 5), MAP_CELL_STEPS_END,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
    (const s32 *)0xffff0000,
    (const s32 *)120,
    (const s32 *)0x40000098,
    (const s32 *)0xffff0000,
    (const s32 *)-1,
    (const s32 *)0xffff,
    (const s32 *)0xffff,
    0,
    0,
    0,
    0,
    0,
};

struct SceneEntrance gCrossboneIsleEntrances1[] = {
    { 1, CONDITION_ALWAYS, 280, 0, 544, FACING_NORTH, 0, 0, 0, 512, 560, 0 },
    { 2, CONDITION_ALWAYS, 136, 0, 360, FACING_SOUTH, 0, 0, 0, 512, 560, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances2[] = {
    { 1, CONDITION_ALWAYS, 312, 0, 520, FACING_NORTH, 0, 16, 64, 416, 544, 0 },
    { 2, CONDITION_ALWAYS, 280, 0, 144, FACING_EAST, 0, 16, 64, 416, 544, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances3[] = {
    { 1, CONDITION_ALWAYS, 312, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
    { 2, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
    { 99, CONDITION_ALWAYS, 272, 0, 232, FACING_SOUTH, 0, 0, 16, 640, 464, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances4[] = {
    { 1, CONDITION_ALWAYS, 184, 0, 136, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 40, 0, 184, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances5[] = {
    { 1, CONDITION_ALWAYS, 312, 0, 656, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { 5, CONDITION_ALWAYS, 216, 0, 544, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    // All other rooms enter through this shared tail.

    { 1, CONDITION_ALWAYS, 840, 0, 176, FACING_WEST, 0, 432, 32, 880, 352, 0 },
    { 2, CONDITION_ALWAYS, 488, 0, 176, FACING_EAST, 0, 432, 32, 880, 352, 0 },
    { 3, CONDITION_ALWAYS, 712, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
    { 4, CONDITION_ALWAYS, 600, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
    { 5, CONDITION_ALWAYS, 712, 0, 152, FACING_NORTH, 0, 432, 32, 880, 352, 0 },
    { SCENE_TABLE_END },
};

u32 gCrossboneIsleExits[] = {
    SCENE_EXITS(113),
    SCENE_EXIT(1, 2, 76),
    SCENE_EXIT(2, 114, 1),
    SCENE_EXIT(0, 114, 0),
    SCENE_EXIT(1, 113, 2),
    SCENE_EXIT(2, 126, 1),
    SCENE_EXIT(0, 123, 0),
    SCENE_EXIT(1, 134, 3),
    SCENE_EXIT(2, 134, 4),
    SCENE_EXIT(0, 124, 0),
    SCENE_EXIT(1, 134, 2),
    SCENE_EXIT(2, 125, 1),
    SCENE_EXIT(0, 125, 0),
    SCENE_EXIT(1, 124, 2),
    SCENE_EXIT(0, 126, 0),
    SCENE_EXIT(1, 114, 2),
    SCENE_EXIT(2, 127, 1),
    SCENE_EXIT(3, 115, 1),
    SCENE_EXIT(4, 115, 2),
    SCENE_EXIT(0, 127, 0),
    SCENE_EXIT(1, 126, 2),
    SCENE_EXIT(2, 128, 1),
    SCENE_EXIT(3, 116, 1),
    SCENE_EXIT(4, 116, 2),
    SCENE_EXIT(0, 128, 0),
    SCENE_EXIT(1, 127, 2),
    SCENE_EXIT(2, 129, 1),
    SCENE_EXIT(3, 117, 1),
    SCENE_EXIT(4, 117, 2),
    SCENE_EXIT(0, 129, 0),
    SCENE_EXIT(1, 128, 2),
    SCENE_EXIT(2, 130, 1),
    SCENE_EXIT(3, 118, 1),
    SCENE_EXIT(4, 118, 2),
    SCENE_EXIT(0, 130, 0),
    SCENE_EXIT(1, 129, 2),
    SCENE_EXIT(2, 131, 1),
    SCENE_EXIT(3, 119, 1),
    SCENE_EXIT(4, 119, 2),
    SCENE_EXIT(0, 131, 0),
    SCENE_EXIT(1, 130, 2),
    SCENE_EXIT(2, 132, 1),
    SCENE_EXIT(3, 120, 1),
    SCENE_EXIT(4, 120, 2),
    SCENE_EXIT(0, 132, 0),
    SCENE_EXIT(1, 131, 2),
    SCENE_EXIT(2, 133, 1),
    SCENE_EXIT(3, 121, 1),
    SCENE_EXIT(4, 121, 2),
    SCENE_EXIT(0, 133, 0),
    SCENE_EXIT(1, 132, 2),
    SCENE_EXIT(2, 134, 1),
    SCENE_EXIT(3, 122, 1),
    SCENE_EXIT(4, 122, 2),
    SCENE_EXIT(0, 134, 0),
    SCENE_EXIT(1, 133, 2),
    SCENE_EXIT(2, 124, 1),
    SCENE_EXIT(3, 123, 1),
    SCENE_EXIT(4, 123, 2),
    SCENE_EXITS_END,
};

struct ScenePlacement gCrossboneIslePlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePlacements1[] = {
    { 0x16, 0xfd7, 7,
        PIXELS(344), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(144), 0, PIXELS(392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(352), 0, PIXELS(376), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(368), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(336), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(320), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(352), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePlacements3[] = {
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(408), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(168), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(520), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(584), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(552), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(584), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(392), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(72), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(120), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePlacements6To14[] = {
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(632), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gCrossboneIsleEventsOther[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(CrossboneIsle_EnterGuardedRoom) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(CrossboneIsle_OpenLeftRoomDoor) },
    { 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(CrossboneIsle_MoveBlockAndCheckPassage) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT_SCRIPT(CrossboneIsle_OpenPassageWithMovedBlock) },
    { SEARCH(SEARCH_CHEST), 100, 0xef4, 0x500004 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIsleEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIsleEvents1[] = {
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 8, 0xfd7, EVENT_SCRIPT(CrossboneIsle_CatchNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 11, 0x240, EVENT_SCRIPT(CrossboneIsle_OpenGrowthPassage11) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 12, 0x241, EVENT_SCRIPT(CrossboneIsle_OpenGrowthPassage12) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 13, 0x242, EVENT_SCRIPT(CrossboneIsle_OpenGrowthPassage13) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 14, 0x243, EVENT_SCRIPT(CrossboneIsle_OpenGrowthPassage14) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 15, 0x8c4, EVENT_SCRIPT(CrossboneIsle_ClearWhirlwindObstacle15) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 16, 0x8c5, EVENT_SCRIPT(CrossboneIsle_ClearWhirlwindObstacle16) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 17, 0x8c6, EVENT_SCRIPT(CrossboneIsle_ClearWhirlwindObstacle17) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 18, 0x8c7, EVENT_SCRIPT(CrossboneIsle_ClearWhirlwindObstacle18) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(CrossboneIsle_MoveSelectedStagedActor) },
    { 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(CrossboneIsle_MoveSelectedStagedActor) },
    { 0x8413, 100, 0xec0, FIND_COINS(1) },
    { 0xc413, 100, 0xec0, FIND_COINS(1) },
    { 0xa413, 100, 0xec0, FIND_COINS(1) },
    { 0x8413, 101, 0xec1, FIND_ITEM(0xc3) },
    { 0xc413, 101, 0xec1, FIND_ITEM(0xc3) },
    { 0xa413, 101, 0xec1, FIND_ITEM(0xc3) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIsleEvents3[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_WEST), 40, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 41, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathEast) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 42, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 43, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateEscapeRouteForActorPositions) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor8ReturnRoute) },
    { TOUCH_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor10RetreatRoute) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor10AdvanceRoute) },
    { TOUCH_WALKING_INTO(FACING_EAST), 22, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 22, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathWest) },
    { TOUCH_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor11WestRoute) },
    { TOUCH_WALKING_INTO(FACING_EAST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor11EastRoute) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor12WestRoute) },
    { TOUCH_WALKING_INTO(FACING_EAST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor12EastRoute) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor13WestRoute) },
    { TOUCH_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor13EastRoute) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 26, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor15WestRoute) },
    { TOUCH_WALKING_INTO(FACING_EAST), 26, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor15EastRoute) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor17SouthRoute) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(UpdateActor17NorthRoute) },
    { TOUCH_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(CrossboneIsle_MoveBlock18North) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(AdvanceActor18AlongEscapeRoute) },
    { TOUCH_WALKING_INTO(FACING_EAST), 32, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 33, CONDITION_ALWAYS, EVENT_SCRIPT(CrossboneIsle_MoveBlock9North) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 33, CONDITION_ALWAYS, EVENT_SCRIPT(AdvanceActor9AlongEscapeRoute) },
    { TOUCH_WALKING_INTO(FACING_EAST), 33, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 33, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathWest) },
    { TOUCH_WALKING_INTO(FACING_WEST), 34, CONDITION_ALWAYS, EVENT_SCRIPT(CrossboneIsle_MoveBlock19West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 34, CONDITION_ALWAYS, EVENT_SCRIPT(CrossboneIsle_MoveBlock19East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 34, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 34, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 35, CONDITION_ALWAYS, EVENT_SCRIPT(RetreatActor14AlongEscapeRoute) },
    { TOUCH_WALKING_INTO(FACING_EAST), 35, CONDITION_ALWAYS, EVENT_SCRIPT(AdvanceActor14AlongEscapeRoute) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 35, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 35, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(RetreatActor16AlongEscapeRoute) },
    { TOUCH_WALKING_INTO(FACING_EAST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(AdvanceActor16AlongEscapeRoute) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 36, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathSouth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 36, CONDITION_ALWAYS, EVENT_SCRIPT(CheckActorPathNorth) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor8) },
    { EVENT_TOUCH, 22, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor10) },
    { EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor11) },
    { EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor12) },
    { EVENT_TOUCH, 25, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor13) },
    { EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor15) },
    { EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor17) },
    { EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor18) },
    { EVENT_TOUCH, 33, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor9) },
    { EVENT_TOUCH, 34, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor19) },
    { EVENT_TOUCH, 35, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor14) },
    { EVENT_TOUCH, 36, CONDITION_ALWAYS, EVENT_SCRIPT(ActivateSceneActor16) },
    { SEARCH(SEARCH_CHEST), 100, 0xee2, FIND_COINS(999) },
    { SEARCH(SEARCH_CHEST), 101, 0xee3, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { SEARCH(SEARCH_CHEST), 102, 0xee4, FIND_ITEM(0xb) },
    { SEARCH(SEARCH_CHEST), 103, 0xee5, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gCrossboneIsleEvents4[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {SCENE_EVENTS_END},
};

struct SceneEvent gCrossboneIsleEvents5[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_TOUCH, 10, 0x8d1, EVENT_SCRIPT(CrossboneIsle_StartFinalGuardBattle)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(CheckActorPathNorth)},
    {SEARCH(SEARCH_CHEST), 100, 0xee6, FIND_ITEM(0x53)},
    {SEARCH(SEARCH_CHEST), 101, 0xef7, 0x500007},
    {SCENE_EVENTS_END},
};
