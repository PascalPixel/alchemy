/// @file
/// Vault's cave puzzles: movable switches, Frost targets, falling objects
/// and the rumbling mechanism restored when the party returns.

#include "game_state.h"
#include "unit/field/field.h"
#include "engine_call.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/vault_cave.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "types.h"
#include "text/msg_ids.h"
#include "field_event.h"
#include "field_scene.h"
#include "callback_scheduler.h"
#include "call.h"
#include "object_runtime.h"
#include "ability_ids.h"
#include "flags.h"
#include "songs.h"
#include "objcmd.h"
#include "map_scroll.h"
#include "field_runtime.h"
#include "macros.h"
#include "item_ids.h"

// Forward declarations.
static void VaultCave_ApplySouthernSwitchCells(void);
static void VaultCave_UseUnderfootFrostPriorities(void);
static void VaultCave_UseAutomaticFrostPriorities(void);
static void VaultCave_SpawnPuffs(struct FieldActor *source);

s32 Math_RemainderUnsigned();

#include "io_write_queue.h"

#define TILE(position) ((position) / 0x100000)

/// Scene flags. The six Frost targets of the second map keep their frozen
/// state in six flags in a row, read by index as well.
enum {
  FLAG_WEST_UPPER_FROZEN = FLAG_SCENE_TEMP_00,
  FLAG_WEST_LOWER_FROZEN = FLAG_SCENE_TEMP_01,
  FLAG_INNER_WEST_LOWER_FROZEN = FLAG_SCENE_TEMP_02,
  FLAG_EAST_UPPER_FROZEN = FLAG_SCENE_TEMP_03,
  FLAG_EAST_LOWER_FROZEN = FLAG_SCENE_TEMP_04,
  FLAG_FAR_EAST_FROZEN = FLAG_SCENE_TEMP_05,
  /// A falling object struck the northern block before it reached its switch.
  FLAG_NORTH_BLOCK_STRUCK = FLAG_SCENE_TEMP_07,
  FLAG_WHIRLWIND_USED = FLAG_SCENE_TEMP_00 + 0x100
};

/// Selects the cave map's entrance positions and camera bounds.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_VaultCave1) {
    return gVaultCaveEntrances1;
  }
  if (scene == (s32)&SceneId_VaultCave2) {
    return gVaultCaveEntrances2;
  }
  if (scene == (s32)&SceneId_VaultCave3) {
    return gVaultCaveEntrances3;
  }
  return gVaultCaveEntrancesOther;
}

/// Returns the region boxes used by the cave exits.
const struct SceneRegion *Scene_GetRegions(void) { return gVaultCaveRegions; }

/// Selects the current cave map's packed exit destinations.
const u32 *Scene_GetExits(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_VaultCave1) {
    return gVaultCaveExits1;
  }
  if (scene == (s32)&SceneId_VaultCave2) {
    return gVaultCaveExits2;
  }
  if (scene == (s32)&SceneId_VaultCave3) {
    return gVaultCaveExits3;
  }
  return gVaultCaveExitsOther;
}

/// Selects the movable blocks, falling objects and Frost targets.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_VaultCave1) {
    return gVaultCavePlacements1;
  }
  if (scene == (s32)&SceneId_VaultCave2) {
    return gVaultCavePlacements2;
  }
  if (scene == (s32)&SceneId_VaultCave3) {
    return gVaultCavePlacements3;
  }
  return gVaultCavePlacementsOther;
}

/// Finds a placed actor with collision enabled at the requested map cell.
static struct FieldActor *VaultCave_FindCollidableActorAtCell(s32 column,
                                                              s32 row) {
  // UB: scene creation leaves unused actor slots NULL. The plain safe lookup
  // adds four native bytes in EN; retain the original reads for matching.
#ifdef UBFIX
  struct FieldWork *work = (struct FieldWork *)gEventWork;
  u32 actorSlot;

  for (actorSlot = ACTOR_FIRST_PLACED; actorSlot <= 65; actorSlot++) {
    struct FieldActor *actor = work->actors[actorSlot];

    if (actor != NULL && column == (actor->x.fixed >> 20) &&
        row == (actor->z.fixed >> 20) && actor->collisionFlags != 0) {
      return actor;
    }
  }
#else
  s32 **slots = (s32 **)((u8 *)gEventWork + 0x14);
  u32 actorSlot;

  for (actorSlot = 8; actorSlot <= 65; actorSlot++) {
    s32 *actorWords = slots[actorSlot];

    if (column == (actorWords[2] >> 20) && row == (actorWords[4] >> 20) &&
        *((u8 *)actorWords + 0x59) != 0) {
      return (struct FieldActor *)actorWords;
    }
  }
#endif
  return NULL;
}

#ifdef UBFIX
/// The table stores Z then X as halfwords. Build its packed word without
/// reading a halfword array through an incompatible, more-aligned struct.
#define DIRECTION_STEP(heading)                                                \
  ((s32)(((u32)(u16)gVaultCave_DirectionSteps[(heading) * 2 + 1] << 16) |      \
         (u16)gVaultCave_DirectionSteps[(heading) * 2]))
#else
struct DirectionSteps {
  s32 xz[16];
};

#define DIRECTION_STEP(heading)                                                \
  (((struct DirectionSteps *)gVaultCave_DirectionSteps)->xz[heading])
#endif

/// Pushes the faced block one cell unless another actor or terrain blocks it.
static void VaultCave_PushBlockAhead(void) {
  /* FAKEMATCH: DIRECTION_STEP reads the s16-pair table (x high, z low) as one
   * word per heading through an array reference, as native; through a plain s32
   * pointer cast the address is summed once into a register and no longer
   * matches, the table as const s32 words indexed directly differs 79 (143
   * instructions for 146), a const s32 local pointer 94. */
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 direction = leader->facing >> 12;
  struct FieldActor *block;
  struct FieldActor *beyond;
  struct FieldPosition destination;
  s32 fallingIndex;
  s32 column;
  s32 row;

  column = (leader->x.part.pixel + (DIRECTION_STEP(direction) >> 16)) >> 4;
  row = (leader->z.part.pixel + (s16)DIRECTION_STEP(direction)) >> 4;
  block = VaultCave_FindCollidableActorAtCell(column, row);
  // UB: the lookup may return NULL. The plain safe guard moves the null
  // branch ahead of the byte load in EN, so matching keeps its original order.
#ifdef UBFIX
  if (block == NULL || block->collisionFlags == 0) {
#else
  if (block->collisionFlags == 0 || block == NULL) {
#endif
    return;
  }
  for (fallingIndex = 0; fallingIndex <= 3; fallingIndex++) {
    if (block == Object_GetById(fallingIndex + 11)) {
      return;
    }
  }
  column = (block->x.part.pixel + (DIRECTION_STEP(direction) >> 16)) >> 4;
  row = (block->z.part.pixel + (s16)DIRECTION_STEP(direction)) >> 4;
  beyond = VaultCave_FindCollidableActorAtCell(column, row);
  if (beyond != 0 && (beyond->collisionFlags & 1)) {
    return;
  }
  block->terrainId = 2;
  destination.x = block->x.fixed + (DIRECTION_STEP(direction) & 0xffff0000);
  destination.y = block->y.fixed;
  destination.z = block->z.fixed + (s32)((u32)DIRECTION_STEP(direction) << 16);
  if (FieldObject_CheckMovementCollision(block, &destination) > 0) {
    return;
  }
  Object_SetMode(leader, 8);
  Engine_TaskWait(15);
  Engine_AudioPlayCue(SE_238);
  block->speed = 0x3333;
  block->acceleration = 0x3333;
  Engine_ObjectSetPosition(block, destination.x, destination.y, destination.z);
  leader->speed = 0x3333;
  leader->acceleration = 0x3333;
  Engine_ObjectSetPosition(leader, destination.x, destination.y, destination.z);
  FieldObject_WaitForMove(block);
  Engine_AudioPlayCue(SE_288);
  block->x.fixed = destination.x;
  block->z.fixed = destination.z;
  block->velocityX = 0;
  block->velocityZ = 0;
  leader->velocityX = 0;
  leader->velocityZ = 0;
  leader->targetX = ACTOR_NO_TARGET;
  leader->targetZ = ACTOR_NO_TARGET;
  Object_SetMode(leader, 1);
}

/// Whirlwind on the target in the second map blows it away for good.
static void Event_UseWhirlwind(void) {
  Engine_GameFlagSet(FLAG_WHIRLWIND_USED);
}

/// Searching the locked door. "The door is tightly locked."
static void Event_ShowLockedDoor(void) {
  Engine_MessageShowCentered(MSGNO(MsgFieldDoorTightlyLocked), 1);
}

/// Selects the cave map's searches, movement checks and Psynergy callbacks.
const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_VaultCave1) {
    return gVaultCaveEvents1;
  }
  if (scene == (s32)&SceneId_VaultCave2) {
    return gVaultCaveEvents2;
  }
  if (scene == (s32)&SceneId_VaultCave3) {
    return gVaultCaveEvents3;
  }
  return gVaultCaveEventsOther;
}

/// Tests the southern block's cell, rounding fixed coordinates toward zero.
static s32 VaultCave_IsSouthernBlockOnSwitch(void) {
  struct FieldActor *block = Object_GetById(9);
  s32 fixedZ = block->z.fixed;
  s32 fixedX;
  s32 row;
  s32 column;

  if (fixedZ < 0) {
    fixedZ += 0x000FFFFF;
  }
  fixedX = block->x.fixed;
  row = fixedZ >> 20;
  if (fixedX < 0) {
    fixedX += 0x000FFFFF;
  }
  column = fixedX >> 20;
  if (column == 15 && row == 54) {
    return 1;
  }
  return 0;
}

/// Finish the southern switch only after the block reaches its target cell.
static inline void VaultCave_CompleteSouthernSwitch(void) {
  if (VaultCave_IsSouthernBlockOnSwitch() != 0) {
    GameFlag_Set(FLAG_VAULT_CAVE_SOUTH_BLOCK_ON_SWITCH);
    Event_PlayCue(MUS_PUZZLE_SOLVED);
    VaultCave_ApplySouthernSwitchCells();
  }
}

/// Pushing the southern block; once it rests on its switch, the puzzle chime
/// plays and the switch opens the way.
static void Event_PushSouthernBlock(void) {
  if (GameFlag_IsSet(FLAG_VAULT_CAVE_SOUTH_BLOCK_ON_SWITCH) == 0) {
    VaultCave_PushBlockAhead();
    VaultCave_CompleteSouthernSwitch();
  }
}

/// Restores the map cells and attributes of the completed southern switch.
static void VaultCave_ApplySouthernSwitchCells(void) {
  Map_CopyCells(87, 50, 2, 4, 80, 50);
  Map_CopyCells(23, 52, 1, 2, 16, 52);
  Map_CopyCellAttributes(16, 52, 1, 1, 16, 53);
}

/// Move on the southern block, with the same switch check.
static void Event_MoveSouthernBlock(void) {
  if (GameFlag_IsSet(FLAG_VAULT_CAVE_SOUTH_BLOCK_ON_SWITCH) == 0) {
    VaultCave_CompleteSouthernSwitch();
  }
}

/// Unused empty function retained from this overlay.
static void UNUSED VaultCave_UnusedNoOp(void) {}

/// Restores the map cells and attributes of the completed northern switch.
static void VaultCave_ApplyNorthernSwitchCells(void) {
  Map_CopyCells(90, 9, 2, 3, 80, 9);
  Map_CopyCells(27, 10, 1, 2, 17, 10);
  Map_CopyCellAttributes(17, 10, 1, 1, 17, 11);
}

/// Tests the northern block's cell with the same signed-coordinate rounding.
static s32 VaultCave_IsNorthernBlockOnSwitch(void) {
  struct FieldActor *block = Object_GetById(10);
  s32 fixedZ = block->z.fixed;
  s32 fixedX;
  s32 row;
  s32 column;

  if (fixedZ < 0) {
    fixedZ += 0x000FFFFF;
  }
  fixedX = block->x.fixed;
  row = fixedZ >> 20;
  if (fixedX < 0) {
    fixedX += 0x000FFFFF;
  }
  column = fixedX >> 20;
  if (column == 16 && row == 12) {
    return 1;
  }
  return 0;
}

/// The northern switch also waits until the falling-object guard is clear.
static inline void VaultCave_CompleteNorthernSwitch(void) {
  if (GameFlag_IsSet(FLAG_VAULT_CAVE_NORTH_BLOCK_ON_SWITCH) == 0) {
    if (VaultCave_IsNorthernBlockOnSwitch() != 0) {
      if (GameFlag_IsSet(FLAG_NORTH_BLOCK_STRUCK) == 0) {
        Event_PlayCue(MUS_PUZZLE_SOLVED);
        VaultCave_ApplyNorthernSwitchCells();
        GameFlag_Set(FLAG_VAULT_CAVE_NORTH_BLOCK_ON_SWITCH);
      }
    }
  }
}

/// Pushing the northern block; it opens its switch only if no falling object
/// has struck it.
static void Event_PushNorthernBlock(void) {
  VaultCave_PushBlockAhead();
  VaultCave_CompleteNorthernSwitch();
}

/// Move on the southern block in the third map does nothing.
static void Event_IgnoreMove(void) {}

/// Move on the northern block, with the same switch check.
static void Event_MoveNorthernBlock(void) {
  VaultCave_CompleteNorthernSwitch();
}

/// Frost on the upper western target freezes it and changes its map cell;
/// arriving again re-freezes every target whose flag is set.
/// An ordinary shared Frost helper keeps the extents but saves r5-r7 instead
/// of r5 alone and changes the map-copy argument registers in native English.
static void Event_FreezeWestUpperTarget(void) {
  struct FieldActor *target = Object_GetById(16);

  Map_CopyCellAttributes(26, 30, 1, 1, 23, 32);

  if (target != 0) {

    Object_GetById(16)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  Engine_GameFlagSet(FLAG_WEST_UPPER_FROZEN);
}

/// Frost on the lower western target.
static void Event_FreezeWestLowerTarget(void) {
  struct FieldActor *target = Object_GetById(17);

  Map_CopyCellAttributes(26, 30, 1, 1, 23, 34);

  if (target != 0) {

    Object_GetById(17)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  Engine_GameFlagSet(FLAG_WEST_LOWER_FROZEN);
}

/// Frost on the lower target just east of it.
static void Event_FreezeInnerWestLowerTarget(void) {
  struct FieldActor *target = Object_GetById(18);

  Map_CopyCellAttributes(26, 30, 1, 1, 24, 34);

  if (target != 0) {

    Object_GetById(18)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  Engine_GameFlagSet(FLAG_INNER_WEST_LOWER_FROZEN);
}

/// Frost on the upper eastern target.
static void Event_FreezeEastUpperTarget(void) {
  struct FieldActor *target = Object_GetById(19);

  Map_CopyCellAttributes(26, 30, 1, 1, 26, 32);

  if (target != 0) {
    Engine_ActorSetSpriteFlags(target, 0);

    Object_GetById(19)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  Engine_GameFlagSet(FLAG_EAST_UPPER_FROZEN);
}

/// Frost on the lower eastern target.
static void Event_FreezeEastLowerTarget(void) {
  struct FieldActor *target = Object_GetById(20);

  Map_CopyCellAttributes(26, 30, 1, 1, 26, 34);

  if (target != 0) {
    Engine_ActorSetSpriteFlags(target, 0);

    Object_GetById(20)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  Engine_GameFlagSet(FLAG_EAST_LOWER_FROZEN);
}

/// Frost on the far eastern target.
static void Event_FreezeFarEastTarget(void) {
  struct FieldActor *target = Object_GetById(21);

  Map_CopyCellAttributes(26, 30, 1, 1, 28, 33);

  if (target != 0) {
    Engine_ActorSetSpriteFlags(target, 0);

    Object_GetById(21)->motionFlags = 0;
    target->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }

  Engine_GameFlagSet(FLAG_FAR_EAST_FROZEN);
}

/// Changes the Frost targets' draw priorities when the leader is above them.
static void VaultCave_UpdateFrostTargetPriorities(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (leader->y.fixed >= PIXELS(16)) {
    VaultCave_UseUnderfootFrostPriorities();
  } else {
    VaultCave_UseAutomaticFrostPriorities();
  }
}

/// Keeps all six Frost targets beneath the leader's feet.
static void VaultCave_UseUnderfootFrostPriorities(void) {
  s32 targetIndex;
  s32 actorSlot;

  actorSlot = 16;
  for (targetIndex = 0; targetIndex < 6; targetIndex++) {
    Object_GetById(actorSlot)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actorSlot++;
  }
}

/// Restores automatic draw priority for the same six targets.
static void VaultCave_UseAutomaticFrostPriorities(void) {
  s32 actorSlot = 16;
  s32 priority = ACTOR_PRIORITY_AUTOMATIC;
  s32 remaining = 5;

  do {
    struct FieldActor *target = Object_GetById(actorSlot);

    remaining--;
    target->priorityFlags = priority;
    actorSlot++;
  } while (remaining >= 0);
}

/// Moves the third map layer through its phases, then fades the mechanism's
/// final map state.
static void VaultCave_RunRumble(void) {
  struct MapLayerScroll *layer =
      &((struct MapScrollWork *)gMapWork[0])->layers[2];
  s32 frame;
  s32 fadeLevel;
  s32 lowFadeLevel;
  s32 upperFadeBits;

  Engine_EventBegin();
  if (gFrameCount & 1) {
    layer->speedX = 1;
    layer->speedY = 1;
  } else {
    layer->speedX = -1;
    layer->speedY = -1;
  }
  Map_SetShake(0x30000, 0x30000, 0x10000);
  Map_SetShake(-1, -1, 0xe666);
  Event_PlayCue(SE_163);
  // Scroll and shake for 480 frames before the final fade.
  for (frame = 479; frame >= 0; frame--) {
    u32 scrollDrift = (u32)(Random_Next() << 11) >> 16;

    layer->phaseY = (double)layer->phaseY - (4718.592 - (double)scrollDrift);
    Engine_EventWait(1);
  }
  // Keep the packed native value written to BLDALPHA.
  fadeLevel = 6;
  lowFadeLevel = 6;
  frame = 0;
  upperFadeBits = fadeLevel << 10;
  do {
    QueueIoWriteDelay2(0x4000052,
                       upperFadeBits | (fadeLevel << 5) | lowFadeLevel);
    Engine_EventWait(1);
    if (frame % 20 == 0) {
      lowFadeLevel--;
      fadeLevel--;
    }
    frame++;
  } while (frame <= 69);
  // Replace the mechanism's map cells after the fade completes.
  Map_CopyCells(19, 83, 15, 8, 19, 91);
  Event_PlayCue(SE_288);
  Engine_MapRedraw();
  Engine_MapRenderWaitForValues();
  Engine_EventEnd();
}

/// Flips the switch. "Isaac flipped the switch." The first time, the switch
/// turns, the cave rumbles and shakes for eight seconds and the map changes as
/// it fades.
static void Event_FlipSwitch(void) {
  s32 destinationRow;
  s32 destinationColumn;

  if (GameFlag_IsSet(FLAG_VAULT_CAVE_SWITCH_FLIPPED) == 0) {
    Engine_MessageShowCentered(MSGNO(MsgFieldFlippedSwitch), 1);
    GameFlag_Set(FLAG_VAULT_CAVE_SWITCH_FLIPPED);
    // Animate the switch face before the adjacent mechanism starts.
    destinationRow = 27;
    destinationColumn = 92;
    Event_PlayCue(SE_155);
    Map_CopyCells(107, 27, 1, 1, destinationColumn, destinationRow);
    Engine_EventWait(39);
    Map_CopyCells(108, 27, 1, 1, destinationColumn, destinationRow);
    Engine_EventWait(50);
    destinationColumn = 25;
    Event_PlayCue(SE_156);
    Map_CopyCells(1, 24, 1, 2, destinationColumn, destinationRow);
    Engine_EventWait(40);
    Map_CopyCells(2, 24, 1, 2, destinationColumn, destinationRow);
    Engine_EventWait(40);
    VaultCave_RunRumble();
  }
}

/// Resets a low falling object sharing the subject's cell and reports a hit.
static s32 VaultCave_ResetFallingObjectAtActorCell(s32 subjectActorId) {
  struct FieldActor *subject = Object_GetById(subjectActorId);
  s32 fallingIndex = 0;

  do {
    struct FieldActor *fallingActor = Object_GetById(fallingIndex + 11);

    if ((u32)(fallingActor->y.fixed - 1) <= 0x000ffffe) {
      s32 fallingRow = TILE(fallingActor->z.fixed);
      s32 fallingColumn = TILE(fallingActor->x.fixed);
      s32 subjectRow = TILE(subject->z.fixed);
      s32 subjectColumn = TILE(subject->x.fixed);
      s32 zDelta = subjectRow - fallingRow;
      s32 xDelta = subjectColumn - fallingColumn;

      if (xDelta == 0 && zDelta == 0) {
        fallingActor->y.fixed = PIXELS(255);
        fallingActor->gravity = 0;
        fallingActor->velocityY = 0;
        return 1;
      }
    }

    fallingIndex++;
  } while (fallingIndex <= 3);

  return 0;
}

/// Drops four objects on a repeating schedule and handles their landings.
static void VaultCave_UpdateTimedFallingObjects(void) {
  s32 fallingIndex;
  struct FieldActor *fallingActor;
  s32 paused;
  s32 scheduleIndex;

  paused = Object_GetById(10)->paused;
  if (paused == 0) {
    if (++gVaultCave_ScheduleTimer > 190) {
      gVaultCave_ScheduleTimer = 0;
    }
    scheduleIndex = gVaultCave_ScheduleIndex;
    if (gVaultCave_ScheduleFrames[scheduleIndex] == gVaultCave_ScheduleTimer) {
      fallingActor = Actor_Get((scheduleIndex + 11));
      fallingActor->gravity = 0xa3d;
      if (++gVaultCave_ScheduleIndex > 3) {
        gVaultCave_ScheduleIndex = 0;
      }
    }
    // Reset objects that have reached the floor and spawn their landing puffs.
    for (fallingIndex = 0; fallingIndex <= 3; fallingIndex++) {
      fallingActor = Actor_Get((fallingIndex + 11));
      if (fallingActor->velocityY >= 0) {
        if (fallingActor->y.fixed <= 0xffff) {
          VaultCave_SpawnPuffs(fallingActor);
          fallingActor->y.fixed = PIXELS(255);
          fallingActor->gravity = 0;
          fallingActor->velocityY = 0;
          fallingActor->paused = 0;
          Event_PlayCue(SE_106);
        }
      }
    }
    // A falling object landing on either movable block restarts at the top.
    if (VaultCave_ResetFallingObjectAtActorCell(10) != 0) {
      Engine_ActorSetAnimation(10, 1);
      if (GameFlag_IsSet(FLAG_NORTH_BLOCK_STRUCK) == 0) {
        GameFlag_Set(FLAG_NORTH_BLOCK_STRUCK);
        Event_PlayCue(SE_204);
      } else {
        Event_PlayCue(SE_106);
      }
    }
    if (VaultCave_ResetFallingObjectAtActorCell(9) != 0) {
      Event_PlayCue(SE_106);
    }
  }
}

/// Drops a random object every 64 frames, with reset height set by its Frost
/// target.
static void VaultCave_UpdateFrostControlledFallingObjects(void) {

  s32 fallingIndex;
  struct FieldActor *fallingActor;
  s32 stateValue;
  s32 *selectedIndex;

  fallingActor = Object_GetById(10);
  if (fallingActor->paused == 0) {
    // Preserve one random selection per 64 frames.
    if ((++gVaultCave_TickCounter & 63) == 0) {
      selectedIndex = &gVaultCave_TickValue;
      stateValue = Engine_RandomNext();
      stateValue = Math_RemainderUnsigned(stateValue, 6);
      *selectedIndex = stateValue;
      fallingActor = Object_GetById((stateValue + 10));
      fallingActor->gravity = 0xa3d;
    }
    // Each object's paired Frost flag selects its reset height.
    for (fallingIndex = 0; fallingIndex <= 5; fallingIndex++) {
      fallingActor = Object_GetById((fallingIndex + 10));
      stateValue =
          Engine_GameFlagIsSet((fallingIndex + FLAG_WEST_UPPER_FROZEN));
      if (stateValue != 0) {
        if (fallingActor->velocityY <= 0) {
          if (fallingActor->y.fixed > 0x20ffff) {
            continue;
          }
        }
        fallingActor->y.fixed = PIXELS(255);
        fallingActor->gravity = 0;
        fallingActor->velocityY = 0;
        Engine_AudioPlayCue(SE_106);
      } else {
        if (fallingActor->velocityY <= 0) {
          if (fallingActor->y.fixed > 0xffff) {
            continue;
          }
        }
        fallingActor->gravity = stateValue;
        fallingActor->velocityY = stateValue;
        fallingActor->y.fixed = PIXELS(255);
        Engine_AudioPlayCue(SE_106);
      }
    }
  }
}

/// Offsets an actor's current position by a polar displacement.
static void VaultCave_OffsetActorRadially(struct FieldActor *actor, s32 radius,
                                          s32 angle) {
  s32 destination[3];

  if (actor == NULL) {
    return;
  }
  SET_VECTOR(destination, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  Vector_AddPolarOffset(radius, angle, destination);
  Engine_ObjectSetPosition(actor, destination[0], destination[1],
                           destination[2]);
}

/// Spawns up to four short-lived puffs around a landing object.
static void VaultCave_SpawnPuffs(struct FieldActor *source) {
  struct FieldActor *puff;
  s32 puffIndex;

  for (puffIndex = 0; puffIndex < 4; puffIndex++) {
    puff = Engine_ObjectCreate(240, source->x.fixed, source->y.fixed,
                               source->z.fixed);
    if (puff == 0)
      break;
    puff->scaleY = 0x8ccc;
    puff->scaleX = 0x8ccc;
    puff->motionFlags = ACTOR_FALLS;
    puff->velocityY = -0x10000;
    puff->speed = Engine_RandomNext() + 0xcccc;
    puff->collisionFlags = 1;
    VaultCave_OffsetActorRadially(puff, 0x200000, Engine_RandomNext());
    {
      u16 *timer = &puff->unk5e;
      s32 frames = 8;

      *timer = frames;
    }
    Engine_ObjectSetScript(puff, gVaultCave_PuffScript);
  }
}

/// Initializes six falling objects and starts their Frost-controlled updater.
static void VaultCave_StartFrostControlledFalls(void) {
  s32 actorSlot = 10;
  s32 remaining = 5;

  do {
    struct FieldActor *fallingActor;

    Engine_ActorSetSpriteFlags(Object_GetById(actorSlot), 0);
    fallingActor = Object_GetById(actorSlot);
    fallingActor->bounce = 0x1999;
    fallingActor->gravity = 0;
    remaining--;
    fallingActor->y.fixed = PIXELS(255);
    actorSlot++;
  } while (remaining >= 0);

  FieldScheduler_AddCallback(VaultCave_UpdateFrostControlledFallingObjects,
                             TASK_PRIORITY_SCENE);
}

/// Initializes four falling objects and starts their timed updater.
static void VaultCave_StartTimedFalls(void) {
  s32 actorSlot = 11;
  s32 fallingIndex = 0;

  do {
    struct FieldActor *fallingActor;

    Engine_ActorSetSpriteFlags(Object_GetById(actorSlot), 0);
    fallingActor = Object_GetById(actorSlot);
    fallingActor->bounce = 0x1999;
    fallingActor->gravity = 0;
    fallingActor->y.fixed = PIXELS(255);
    Engine_ActorSetSpritePriority(fallingIndex + 11, 1);
    fallingIndex++;
    actorSlot++;
  } while (fallingIndex <= 3);

  FieldScheduler_AddCallback(VaultCave_UpdateTimedFallingObjects,
                             TASK_PRIORITY_SCENE);
}

/// Restores each room's puzzles and actors, then sets the Retreat destination.
s32 Scene_Initialize(void) {
  ((struct FieldWork *)gEventWork)->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gGameState.scene == (s32)&SceneId_VaultCave1) {
    switch (gGameState.entrance) {
    case 5:
    case 6:
    case 7:
    case 8:
    case 13:
      if (Engine_GameFlagIsSet(FLAG_VAULT_CAVE_SWITCH_FLIPPED) == 0) {
        Map_CopyCellAttributes(22, 29, 1, 1, 21, 29);
      } else {
        Map_CopyCells(108, 27, 1, 1, 92, 27);
        Engine_TaskWait(1);
        Map_CopyCells(19, 83, 15, 8, 19, 91);
        Engine_TaskWait(1);
        Map_CopyCells(2, 24, 1, 2, 25, 27);
      }
      Engine_MapRedraw();
      Engine_TaskWait(1);
      break;
    case 10:
      Engine_GameFlagClear(FLAG_VAULT_CAVE_SWITCH_FLIPPED);
      break;
    }
  }
  if (gGameState.scene == (s32)&SceneId_VaultCave2) {
    if (Engine_GameFlagIsSet(FLAG_WHIRLWIND_USED) == 0) {
      Object_GetById(22)->scaleY = 0x18000;
    }
    switch (gGameState.entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
      if (Engine_GameFlagIsSet(FLAG_VAULT_CAVE_SWITCH_FLIPPED) == 0) {
        Map_CopyCells(5, 81, 11, 7, 5, 73);
      } else {
        Map_CopyCellAttributes(5, 12, 1, 1, 6, 12);
        Map_CopyCellAttributes(12, 10, 1, 1, 12, 11);
      }
      break;
    case 8:
    case 9:
    case 14:
      VaultCave_StartFrostControlledFalls();
      if (Engine_GameFlagIsSet(FLAG_WEST_UPPER_FROZEN) != 0) {
        Engine_ActorSetAnimation(16, 5);
        Event_FreezeWestUpperTarget();
      }
      if (Engine_GameFlagIsSet(FLAG_WEST_LOWER_FROZEN) != 0) {
        Engine_ActorSetAnimation(17, 5);
        Event_FreezeWestLowerTarget();
      }
      if (Engine_GameFlagIsSet(FLAG_INNER_WEST_LOWER_FROZEN) != 0) {
        Engine_ActorSetAnimation(18, 5);
        Event_FreezeInnerWestLowerTarget();
      }
      if (Engine_GameFlagIsSet(FLAG_EAST_UPPER_FROZEN) != 0) {
        Engine_ActorSetAnimation(19, 5);
        Event_FreezeEastUpperTarget();
      }
      if (Engine_GameFlagIsSet(FLAG_EAST_LOWER_FROZEN) != 0) {
        Engine_ActorSetAnimation(20, 5);
        Event_FreezeEastLowerTarget();
      }
      if (Engine_GameFlagIsSet(FLAG_FAR_EAST_FROZEN) != 0) {
        Engine_ActorSetAnimation(21, 5);
        Event_FreezeFarEastTarget();
      }
      FieldScheduler_AddCallback(VaultCave_UpdateFrostTargetPriorities,
                                 TASK_PRIORITY_SCENE);
      break;
    case 10:
    case 11:
      if (Engine_GameFlagIsSet(FLAG_VAULT_CAVE_SOUTH_BLOCK_ON_SWITCH) != 0) {
        VaultCave_ApplySouthernSwitchCells();
        Actor_SetPosition(9, PIXELS(248), PIXELS(876));
      }
      Object_GetById(8)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      break;
    }
    Engine_ActorSetAnimation(8, 2);
    Engine_ActorSetAnimation(9, 2);
    Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
    Object_GetById(9)->collisionFlags = 1;
  }
  if (gGameState.scene == (s32)&SceneId_VaultCave3) {
    Engine_ActorSetAnimation(8, 2);
    if (Engine_GameFlagIsSet(FLAG_NORTH_BLOCK_STRUCK) == 0) {
      Engine_ActorSetAnimation(10, 2);
    }
    Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
    Object_GetById(10)->collisionFlags |= 0x80;
    Object_GetById(9)->collisionFlags |= 0x80;
    switch (gGameState.entrance) {
    case 5:
    case 6:
      VaultCave_StartTimedFalls();
      Object_GetById(11)->collisionFlags = 2;
      Object_GetById(12)->collisionFlags = 2;
      Object_GetById(13)->collisionFlags = 2;
      Object_GetById(14)->collisionFlags = 2;
      Object_GetById(8)->collisionFlags = 1;
      Object_GetById(10)->collisionFlags = 1;
      Object_GetById(9)->collisionFlags = 1;
      if (Engine_GameFlagIsSet(FLAG_VAULT_CAVE_NORTH_BLOCK_ON_SWITCH) != 0) {
        VaultCave_ApplyNorthernSwitchCells();
        Actor_SetPosition(10, PIXELS(264), PIXELS(204));
      }
      break;
    }
  }
  // Retreat always returns to the first cave map's outer entrance.
  gGameState.retreatEntrance = 10;
  gGameState.retreatScene = (s32)&SceneId_VaultCave1;
  return 0;
}

// clang-format off
const s16 gVaultCave_DirectionSteps[] = {
    0, 16,
    0, 16,
    0, 16,
    16, 0,
    16, 0,
    16, 0,
    16, 0,
    0, -16,
    0, -16,
    0, -16,
    0, -16,
    -16, 0,
    -16, 0,
    -16, 0,
    -16, 0,
    0, 16,
};

const s32 gVaultCave_PuffScript[] = {
    SC_FREE,
};

struct SceneEntrance gVaultCaveEntrancesOther[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVaultCaveEntrances1[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 424, 0, 216, FACING_SOUTH, 0, 72, 8, 480, 320, 0 },
    { 2, CONDITION_ALWAYS, 136, 0, 104, FACING_SOUTH, 0, 72, 8, 480, 320, 0 },
    { 3, CONDITION_ALWAYS, 120, 0, 256, FACING_NORTH, 0, 72, 8, 480, 320, 0 },
    { 4, CONDITION_ALWAYS, 184, 0, 264, FACING_NORTH, 0, 72, 8, 480, 320, 0 },
    { 5, CONDITION_ALWAYS, 344, 0, 464, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
    { 6, CONDITION_ALWAYS, 488, 0, 488, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
    { 7, CONDITION_ALWAYS, 584, 0, 608, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
    { 8, CONDITION_ALWAYS, 344, 0, 632, FACING_NORTH, 0, 280, 360, 640, 640, 0 },
    { 13, CONDITION_ALWAYS, 408, 0, 472, FACING_SOUTH, 0, 280, 360, 640, 640, 0 },
    { 9, CONDITION_ALWAYS, 88, 0, 704, FACING_SOUTH, 0, 40, 632, 352, 912, 0 },
    { 12, CONDITION_ALWAYS, 184, 0, 864, FACING_SOUTH, 0, 40, 632, 352, 912, 0 },
    { 10, CONDITION_ALWAYS, 456, 0, 800, FACING_SOUTH, 0, 336, 696, 576, 856, 0 },
    { 11, CONDITION_ALWAYS, 456, 0, 840, FACING_NORTH, 0, 336, 696, 576, 856, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVaultCaveEntrances2[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 136, 0, 104, FACING_SOUTH, 0, 40, 8, 400, 328, 0 },
    { 2, CONDITION_ALWAYS, 344, 0, 152, FACING_SOUTH, 0, 40, 8, 400, 328, 0 },
    { 3, CONDITION_ALWAYS, 344, 0, 184, FACING_NORTH, 0, 40, 8, 400, 328, 0 },
    { 4, CONDITION_ALWAYS, 168, 0, 328, FACING_NORTH, 0, 40, 8, 400, 328, 0 },
    { 5, CONDITION_ALWAYS, 56, 0, 480, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
    { 6, CONDITION_ALWAYS, 152, 0, 472, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
    { 7, CONDITION_ALWAYS, 120, 0, 576, FACING_SOUTH, 0, 8, 408, 248, 656, 0 },
    { 8, CONDITION_ALWAYS, 344, 0, 456, FACING_SOUTH, 0, 280, 344, 544, 672, 0 },
    { 9, CONDITION_ALWAYS, 424, 0, 664, FACING_NORTH, 0, 280, 344, 544, 672, 0 },
    { 14, CONDITION_ALWAYS, 456, 0, 472, FACING_SOUTH, 0, 280, 344, 544, 672, 0 },
    { 10, CONDITION_ALWAYS, 200, 0, 776, FACING_SOUTH, 0, 104, 696, 344, 944, 0 },
    { 11, CONDITION_ALWAYS, 264, 0, 808, FACING_SOUTH, 0, 104, 696, 344, 944, 0 },
    { 13, CONDITION_ALWAYS, 536, 0, 168, FACING_NORTH, 0, 416, 24, 656, 184, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gVaultCaveEntrances3[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 312, 0, 664, FACING_NORTH, 0, 200, 520, 440, 680, 0 },
    { 2, CONDITION_ALWAYS, 312, 0, 600, FACING_SOUTH, 0, 200, 520, 440, 680, 0 },
    { 3, CONDITION_ALWAYS, 88, 0, 568, FACING_SOUTH, 0, 8, 472, 248, 632, 0 },
    { 4, CONDITION_ALWAYS, 168, 0, 544, FACING_SOUTH, 0, 8, 472, 248, 632, 0 },
    { 5, CONDITION_ALWAYS, 248, 0, 336, FACING_NORTH, 0, 128, 40, 376, 384, 0 },
    { 6, CONDITION_ALWAYS, 280, 0, 104, FACING_SOUTH, 0, 128, 40, 376, 384, 0 },
    { SCENE_TABLE_END },
};

struct SceneRegion gVaultCaveRegions[] = {
    { 447, 47, 783, 463, 63, 799, -1, 10 },
    { -1, 0, 0, 0, 0, 0, 0, 0 },
};

u32 gVaultCaveExitsOther[] = {
    SCENE_EXITS(511),
};

u32 gVaultCaveExits1[] = {
    SCENE_EXITS(96),
    SCENE_EXIT(1, 98, 2),
    SCENE_EXIT(2, 97, 1),
    SCENE_EXIT(3, 96, 5),
    SCENE_EXIT(4, 96, 13),
    SCENE_EXIT(5, 96, 3),
    SCENE_EXIT(6, 97, 6),
    SCENE_EXIT(7, 97, 10),
    SCENE_EXIT(8, 96, 9),
    SCENE_EXIT(9, 96, 8),
    SCENE_EXIT(10, 23, 13),
    SCENE_EXIT(10, 96, 11),
    SCENE_EXIT(11, 96, 12),
    SCENE_EXIT(12, 96, 11),
    SCENE_EXIT(13, 96, 4),
    SCENE_EXITS_END,
};

u32 gVaultCaveExits2[] = {
    SCENE_EXITS(97),
    SCENE_EXIT(1, 96, 2),
    SCENE_EXIT(2, 98, 6),
    SCENE_EXIT(3, 97, 8),
    SCENE_EXIT(4, 97, 5),
    SCENE_EXIT(5, 97, 4),
    SCENE_EXIT(6, 96, 6),
    SCENE_EXIT(7, 98, 3),
    SCENE_EXIT(8, 97, 3),
    SCENE_EXIT(9, 97, 11),
    SCENE_EXIT(10, 96, 7),
    SCENE_EXIT(11, 97, 9),
    SCENE_EXIT(13, 97, 13),
    SCENE_EXIT(14, 97, 14),
    SCENE_EXITS_END,
};

u32 gVaultCaveExits3[] = {
    SCENE_EXITS(98),
    SCENE_EXIT(1, 23, 14),
    SCENE_EXIT(2, 96, 1),
    SCENE_EXIT(3, 97, 7),
    SCENE_EXIT(4, 98, 5),
    SCENE_EXIT(5, 98, 4),
    SCENE_EXIT(6, 97, 2),
    SCENE_EXITS_END,
};

struct ScenePlacement gVaultCavePlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gVaultCavePlacements1[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gVaultCavePlacements2[] = {
    { 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(876), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(892), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(372), 0, PIXELS(512), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(372), 0, PIXELS(544), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(388), 0, PIXELS(544), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(420), 0, PIXELS(512), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(420), 0, PIXELS(544), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(452), 0, PIXELS(528), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(520), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(424), 0, PIXELS(520), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(424), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(536), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf8, FLAG_WHIRLWIND_USED, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(456), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVaultCavePlacements3[] = {
    { 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(236), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x12b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(300), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(264), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gVaultCaveEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gVaultCaveEvents1[] = {
    { 0x21, 1, CONDITION_ALWAYS, 1 },
    { 0x31, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { 0x31, 6, CONDITION_ALWAYS, 6 },
    { 0x31, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FlipSwitch) },
    { SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ShowLockedDoor) },
    { SEARCH(SEARCH_CHEST), 100, FLAG_VAULT_CAVE_1_CHEST, 0x500003 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVaultCaveEvents2[] = {
    { 0x21, 1, CONDITION_ALWAYS, 1 },
    { 0x31, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { 0x21, 6, CONDITION_ALWAYS, 6 },
    { 0x31, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { 0x21, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { SEARCH(SEARCH_CHEST), 100, FLAG_VAULT_CAVE_2_CHEST, FIND_ITEM(ITEM_VAMBRACE) },
    { SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ShowLockedDoor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, FLAG_VAULT_CAVE_SOUTH_BLOCK_ON_SWITCH, EVENT_SCRIPT(Event_MoveSouthernBlock) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 60, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushSouthernBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), 16, FLAG_WEST_UPPER_FROZEN, EVENT_SCRIPT(Event_FreezeWestUpperTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), 17, FLAG_WEST_LOWER_FROZEN, EVENT_SCRIPT(Event_FreezeWestLowerTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), 18, FLAG_INNER_WEST_LOWER_FROZEN, EVENT_SCRIPT(Event_FreezeInnerWestLowerTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), 19, FLAG_EAST_UPPER_FROZEN, EVENT_SCRIPT(Event_FreezeEastUpperTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), 20, FLAG_EAST_LOWER_FROZEN, EVENT_SCRIPT(Event_FreezeEastLowerTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), 21, FLAG_FAR_EAST_FROZEN, EVENT_SCRIPT(Event_FreezeFarEastTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 22, FLAG_WHIRLWIND_USED, EVENT_SCRIPT(Event_UseWhirlwind) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVaultCaveEvents3[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { 0x31, 2, CONDITION_ALWAYS, 2 },
    { 0x21, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { 0x21, 6, CONDITION_ALWAYS, 6 },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 50, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushNorthernBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, FLAG_VAULT_CAVE_NORTH_BLOCK_ON_SWITCH, EVENT_SCRIPT(Event_MoveNorthernBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_IgnoreMove) },
    { SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ShowLockedDoor) },
    { SCENE_EVENTS_END },
};

u32 gVaultCave_ScheduleFrames[] = {
    20, 80, 115, 170,
};
// clang-format on

s32 gVaultCave_ScheduleTimer = 0;

s32 gVaultCave_ScheduleIndex = 0;

s32 gVaultCave_TickCounter = 0;

s32 gVaultCave_TickValue = 0;
