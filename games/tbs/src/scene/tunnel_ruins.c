#include "script.h"
#include "version.h"
#include "game_state.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/tunnel_ruins.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "object_effect.h"

#define FIELD_STAGED_ACTOR_IMPORTS
#include "field_obj.h"
#include "types.h"
#include "text/msg_ids.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "field_effect.h"
#include "staged_actor.h"
#include "spawn_configured_effect.h"
#include "edition.h"
#include "scene_ids.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "field_runtime.h"

enum { TUNNEL_CALLBACK_PRIORITY = 3200 };
#include "field_runtime.h"

// Forward declarations.
#ifdef UBFIX
static void
TunnelRuins_FollowSupportingPlatform(union FieldObject *fieldObject);
#else
static s32 TunnelRuins_FollowSupportingPlatform(union FieldObject *fieldObject);
#endif
static void TunnelRuins_RestoreFirstPairCells(void);
static void TunnelRuins_RestoreSecondPairCells(void);
static void TunnelRuins_RestorePlatformCells(void);
static void TunnelRuins_MarkActorCells(void);

// Position record used by the shared radial-effect source below.
struct Vec {
  s32 x;
  s32 y;
  s32 z;
};

// Shared source is included here in ROM order.
#include "common/object_staged_actor.h"

// Shared source is included here in ROM order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

// Refresh the actor presentation after its shrink script finishes.
static s32
TunnelRuins_RefreshActorAfterShrinking(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  Engine_ActorSetSpriteFlags(actor, 0);
  return 0;
}

// Later patches land at the tile center; the base release uses the target
// pixel.
static __inline__ s32 TunnelRuins_GetLandingPixel(s32 coordinate) {
#if TBS_PATCH_1
  return ((coordinate >> 20) << 4) + 8;
#else
  return (s16)(coordinate >> 16);
#endif
}

// Jump to the target when both the adjacent tile and destination are clear.
static s32 TunnelRuins_MoveLeaderToPoint(struct FieldPosition *target) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  u8 savedMotionFlags = actor->motionFlags;
  struct FieldPosition adjacentTile;

  adjacentTile.x = (actor->x.fixed & 0xfff00000) + PIXELS(8);
  adjacentTile.y = actor->y.fixed;
  adjacentTile.z = (actor->z.fixed & 0xfff00000) + PIXELS(8);

  FieldVector_AddPolarOffset(PIXELS(16), (actor->facing + 0x2000) & 0xc000,
                             &adjacentTile);

  if (FieldObject_CheckMovementCollision(actor, &adjacentTile) != 1 &&
      FieldObject_CheckMovementCollision(actor, target) == 0) {
    Engine_EventBegin();
    Object_SetMode(actor, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(152);
    Object_SetMode(actor, 7);

    actor->speed = PIXELS(3);
    actor->acceleration = PIXELS(2);
    actor->velocityY = PIXELS(4);
    actor->motionFlags &= 0x7e;

    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER,
                              TunnelRuins_GetLandingPixel(target->x),
                              TunnelRuins_GetLandingPixel(target->z));
    Object_SetMode(actor, 6);
    Engine_ActorSetSpriteFlags(actor, 1);
    Engine_TaskWait(6);

    actor->motionFlags = savedMotionFlags;
    Engine_EventEnd();
    return 0;
  }
  return 1;
}

// Try to move the leader 32 pixels south.
static void TunnelRuins_MoveLeaderSouth(void) {
  struct FieldPosition target;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  target.x = leader->x.fixed;
  target.y = leader->y.fixed;
  target.z = leader->z.fixed + PIXELS(32);
  TunnelRuins_MoveLeaderToPoint(&target);
}

// Try to move the leader 32 pixels north.
static void TunnelRuins_MoveLeaderNorth(void) {
  struct FieldPosition target;
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed;
  target.y = actor->y.fixed;
  target.z = actor->z.fixed - PIXELS(32);
  TunnelRuins_MoveLeaderToPoint(&target);
}

// Track the leader only beyond the entrance-specific depth limit.
static void TunnelRuins_UpdateTrackedActor(void) {
  struct FieldActor *leader;
  struct FadeOverlayWork *effectWork;
  s32 depthLimit;

  leader = Object_GetById(0);
  effectWork = gActorEffectWork;
  depthLimit = 0;
  // Each tracked entrance measures depth in eight-pixel steps.
  if (gGameState.scene == (s32)&SceneId_TunnelRuins1) {
    switch (gGameState.entrance) {
    case 3:
    case 4:
      depthLimit = 94;
      break;
    case 8:
    case 9:
      depthLimit = 74;
      break;
    case 12:
    case 13:
      depthLimit = 118;
      break;
    }
  } else if (gGameState.entrance == 12) {
    depthLimit = 93;
  }
  if ((leader->z.fixed >> 19) <= depthLimit) {
    effectWork->actor = NULL;
  } else {
    effectWork->actor = leader;
  }
}

// Shared source is included here in ROM order.
#include "common/object_depth.h"

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 156 to 152 bytes in tbs-en.
// Alternate the actor mode and scatter small effects every four frames.
#ifdef UBFIX
static void TunnelRuins_UpdateFlickerEffect(union FieldObject *fieldObject) {
#else
static s32 TunnelRuins_UpdateFlickerEffect(union FieldObject *fieldObject) {
#endif
  struct FieldActor *object = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  s32 x;
  s32 z;

  if (gFrameCount & 2)
    Object_SetMode(object, 1);
  else
    Object_SetMode(object, 2);
  if (gFrameCount & 3)
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  options.startScaleX = 3 * Q16_ONE / 10;
  options.startScaleY = 3 * Q16_ONE / 10;
  options.palette = 5;
  x = object->x.fixed + ((((u32)(Engine_RandomNext() * 7) >> 16) - 3) << 16);
  z = object->z.fixed + ((((u32)(Engine_RandomNext() * 7) >> 16) - 3) << 16);
  Effect_Spawn(x, object->y.fixed + PIXELS(16), z, 0, 0, 0, 0x90001, &options);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void removes its zero result; its aligned extent stays 80 bytes.
// Track actor 8 at a lowered height while it occupies row 10.
#ifdef UBFIX
static void TunnelRuins_TrackActor8AtRow10(union FieldObject *fieldObject) {
#else
static s32 TunnelRuins_TrackActor8AtRow10(union FieldObject *fieldObject) {
#endif
  struct FieldActor *effect = (struct FieldActor *)fieldObject;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (leader->y.fixed > -PIXELS(48) &&
      (Object_GetById(8)->z.fixed >> 20) == 10) {
    effect->x.fixed = Object_GetById(8)->x.fixed;
    effect->y.fixed = -PIXELS(32);
    effect->z.fixed = Object_GetById(8)->z.fixed;
  } else {
    effect->x.fixed = 0;
    effect->y.fixed = 0;
    effect->z.fixed = 0;
  }
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

// Wait up to 60 frames for landing, then stop vertical motion at the terrain.
static void TunnelRuins_WaitForActorToLand(struct FieldActor *actor) {
  s32 remainingFrames = 60;
  s32 terrainHeight;

  for (;;) {
    if (remainingFrames != 0) {
      s32 height;

      Engine_TaskWait(1);
      height = actor->y.fixed;
      terrainHeight = actor->terrainHeight;
      remainingFrames--;
      if (height <= terrainHeight) {
        break;
      }
      continue;
    }
    terrainHeight = actor->terrainHeight;
    break;
  }
  actor->velocityY = 0;
  actor->y.fixed = terrainHeight;
  actor->targetY = ACTOR_NO_TARGET;
}

// Shared source is included here in ROM order.
#include "common/effect_advance.h"

// Jump the leader down while spraying effects behind the landing.
static void TunnelRuins_RunLeaderSpray(void) {
  struct FieldActor *leader = Object_GetById(0);
  struct EffectOptions options;
  u32 i;

  Engine_EventBegin();
  Object_SetMode(leader, 6);
  ObjectMotion_WaitForAnimationChange(0);
  Object_SetMode(leader, 1);
  Engine_ActorSetSpriteFlags(leader, 0);
  leader->motionFlags |= ACTOR_FALLS;
  Engine_AudioPlayCue(152);
  leader->velocityY = PIXELS(4);
  Engine_ObjectSetPosition(leader, leader->x.fixed, leader->y.fixed,
                           leader->z.fixed + PIXELS(12));
  Engine_TaskWait(6);
  leader->motionFlags = 0;
  // Emit particles on alternate frames while the leader descends.
  options.update = (void (*)(union FieldObject *))Effect_AdvanceMotion;
  Engine_AudioPlayCue(127);
  for (i = 0; i < 8; i++) {
    leader->y.fixed -= PIXELS(2);
    leader->targetY = leader->y.fixed;
    Engine_TaskWait(1);
    if (i & 1) {
      s32 horizontalRoll = (u32)Engine_RandomNext() % 10 - 5;
      s32 velocityX = horizontalRoll * 0x3332;
      s32 velocityZ;

      velocityZ = (u32)Engine_RandomNext() % 10 * -0x1999 - 0x7ffd;
      Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, velocityX,
                   0, velocityZ, 0x1000001, &options);
    }
  }
  Engine_ActorSetSpriteFlags(leader, 1);
  leader->motionFlags = 3;
  Engine_EventEnd();
}

#ifdef UBFIX
#define ACTOR_UPDATE_IDLE SceneActor_SetFlagBitByRelativeDepth
#else
#define ACTOR_UPDATE_IDLE                                                      \
  ((void (*)(union FieldObject *))SceneActor_SetFlagBitByRelativeDepth)
#endif
#ifdef UBFIX
#define ACTOR_UPDATE_PANEL TunnelRuins_FollowSupportingPlatform
#else
#define ACTOR_UPDATE_PANEL                                                     \
  ((void (*)(union FieldObject *))TunnelRuins_FollowSupportingPlatform)
#endif

// Shared source is included here in ROM order.
#include "common/effect_radial.h"

// Select the entrance table for the current tunnel ruins scene.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_TunnelRuins1) {
    return gTunnelRuinsEntrances1;
  }
  if (sceneId == (s32)&SceneId_TunnelRuins2) {
    return gTunnelRuinsEntrances2;
  }
  return gTunnelRuinsEntrancesOther;
}

// Return the region table only for the second ruins scene.
const struct SceneRegion *Scene_GetRegions(void) {
  if (gGameState.scene == (s32)&SceneId_TunnelRuins2) {
    return gTunnelRuinsRegions2;
  }
  return 0;
}

// Return the tunnel ruins exit table.
u32 *TunnelRuins_GetExits(void) { return gTunnelRuinsExits; }

// Select the actor placements and prepare their scene state.
struct ScenePlacement *SceneData_SelectAndApplyTableBySceneId(void) {
  struct ScenePlacement *table;

  if (gGameState.scene == (s32)&SceneId_TunnelRuins1) {
    table = gTunnelRuinsPlacements1;
  } else {
    table = gTunnelRuinsPlacementsOther;
  }
  FieldScene_PrepareActors(table);
  return table;
}

// Move the first pair and rebuild its collision cells.
static void TunnelRuins_MoveFirstActorPair(void) {
  Engine_EventBegin();
  Map_CopyCellAttributes(73, 38, 5, 5, 9, 38);
  StagedActor_AdvancePair();
  TunnelRuins_RestoreFirstPairCells();
  Engine_EventEnd();
}

// Try to move the leader 32 pixels west.
static void TunnelRuins_MoveLeaderWest(void) {
  struct FieldPosition target;
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed - PIXELS(32);
  target.y = actor->y.fixed;
  target.z = actor->z.fixed;
  TunnelRuins_MoveLeaderToPoint(&target);
}

// Move the second pair and rebuild its collision cells.
static void TunnelRuins_MoveSecondActorPair(void) {
  Engine_EventBegin();
  Map_CopyCellAttributes(93, 30, 6, 5, 29, 30);
  StagedActor_AdvancePair();
  TunnelRuins_RestoreSecondPairCells();
  Engine_EventEnd();
}

// Run the leader ledge-jump sequence in mode 6.
static void TunnelRuins_JumpLeaderDownLedge(void) {
  FieldBattleFx_RunRisingObjectSequence(0, 6, 0);
}

// Try to move the leader 32 pixels east.
static void TunnelRuins_MoveLeaderEast(void) {
  struct FieldPosition target;
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed + PIXELS(32);
  target.y = actor->y.fixed;
  target.z = actor->z.fixed;
  TunnelRuins_MoveLeaderToPoint(&target);
}

// Move the platform group and restore its collision cells.
static void TunnelRuins_MovePlatformGroup(void) {
  Engine_EventBegin();
  Map_CopyCellAttributes(89, 49, 3, 2, 25, 49);
  Map_CopyCellAttributes(89, 51, 8, 5, 25, 51);
  StagedActor_AdvancePair();
  TunnelRuins_RestorePlatformCells();
  Engine_EventEnd();
}

// Try the southward jump before moving the platform group.
static void TunnelRuins_EnterPlatformGroup(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition target;

  target.x = actor->x.fixed;
  target.y = actor->y.fixed;
  target.z = actor->z.fixed + PIXELS(32);
  if (TunnelRuins_MoveLeaderToPoint(&target) != 0) {
    TunnelRuins_MovePlatformGroup();
  }
}

// Record switch 15 and update actor 14 for the two-switch state.
static void TunnelRuins_ActivateWhirlwindSwitch15(void) {
  u8 *flags;
  u8 value;

  Engine_GameFlagSet(0x200);
  if (Engine_GameFlagIsSet(0x201) != 0) {
    Object_GetById(14)->riseCounter = 0;
    Object_GetById(14)->collisionFlags &= 0xf7;
  } else {
    Object_GetById(14)->riseCounter = 1;
    flags = &Object_GetById(14)->collisionFlags;
    value = 8;
    value |= *flags;
    *flags = value;
  }
}

// Record switch 16 and update actor 14 for the two-switch state.
static void TunnelRuins_ActivateWhirlwindSwitch16(void) {
  u8 *flags;
  u8 value;

  Engine_GameFlagSet(0x201);
  if (Engine_GameFlagIsSet(0x200) != 0) {
    Object_GetById(14)->riseCounter = 0;
    Object_GetById(14)->collisionFlags &= 0xf7;
  } else {
    Object_GetById(14)->riseCounter = 1;
    flags = &Object_GetById(14)->collisionFlags;
    value = 8;
    value |= *flags;
    *flags = value;
  }
}

// Record completion of the Whirlwind interaction with actor 17.
static void TunnelRuins_CompleteWhirlwind17(void) { Engine_GameFlagSet(0x970); }

// Try the westward jump unless the leader occupies tile 32, 50.
static void TunnelRuins_MoveWestUnlessAtTile32x50(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

  if ((actor->x.fixed >> 20) != 32 || (actor->z.fixed >> 20) != 50) {
    TunnelRuins_MoveLeaderWest();
  }
}

// Try the northward jump unless the leader occupies tile 30, 52.
static void TunnelRuins_MoveNorthUnlessAtTile30x52(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

  if ((actor->x.fixed >> 20) != 30 || (actor->z.fixed >> 20) != 52) {
    TunnelRuins_MoveLeaderNorth();
  }
}

// Draw an impact frame of the passage and start a decaying vertical shake.
static __inline__ void TunnelRuins_AnimateLoweringImpact(s32 sourceColumn,
                                                         s32 column, s32 row) {
  Engine_AudioPlayCue(188);
  Map_CopyCellsTo(sourceColumn, 8, column, row, 1, 3);
  Map_SetShake(0, PIXELS(5), Q16_ONE);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
}

// Lower actor 18 at column 46 and animate the resulting passage.
static void TunnelRuins_LowerActor18(void) {
  u32 i;
  struct FieldActor *temporaryActor;
  s32 randomX;
  struct FieldActor *actor;
  s32 randomZ;
  struct EffectOptions options;
  struct EffectOptions *effectOptions;

  Engine_EventBegin();
  actor = Object_GetById(18);
  if ((actor->x.fixed >> 20) == 46) {
    Engine_EventWait(30);
    temporaryActor =
        OverlayObject_PrepareObject(PIXELS(744), 0, PIXELS(184), 253);
    // Lower the actor one pixel at a time and scatter the surrounding effects.
    effectOptions = &options;
    effectOptions->startScaleX = 3 * Q16_ONE / 5;
    effectOptions->startScaleY = 3 * Q16_ONE / 5;
    effectOptions->palette = 7;
    Actor_Get(18)->motionFlags = 0;
    Engine_AudioPlayCue(185);
    for (i = 0; i < 16; i++) {
      Engine_TaskWait(3);
      Actor_Get(18)->y.fixed -= PIXELS(1);
      randomX = Random_Next();
      randomX = ((((u32)(randomX << 4) >> 16) << 16) + PIXELS(736));
      randomZ = Engine_RandomNext();
      Effect_Spawn(randomX, 0,
                   ((((u32)(((randomZ << 3) + randomZ) << 1) >> 16) << 16) +
                    PIXELS(128)),
                   0, 0, 0, 0x90000, effectOptions);
    }
    // Restore the passage in two impact frames.
    Map_CopyCellAttributes(51, 8, 1, 1, 49, 8);
    Engine_EventWait(30);
    Actor_Get(18)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Engine_ActorSetAnimation(18, 3);
    Engine_ObjectDispatchRelease(temporaryActor);
    Map_CopyCellAttributes(45, 4, 1, 1, 46, 8);
    Actor_SetPosition(20, PIXELS(744), PIXELS(136));
    TunnelRuins_AnimateLoweringImpact(58, 49, 8);
    Engine_EventWait(20);
    TunnelRuins_AnimateLoweringImpact(59, 49, 8);
    Engine_MapRenderWaitForValues();
    Engine_EventWait(10);
    GameFlag_Set(0x971);
  }
  Engine_EventEnd();
}

// Advance the movable pair, then test whether actor 18 can lower.
static void TunnelRuins_MoveActor18AndCheckLowering(void) {
  Engine_EventBegin();
  StagedActor_AdvancePair();
  Engine_EventEnd();
  TunnelRuins_LowerActor18();
}

// Freeze actor 20 and restore its normal depth handling.
static void TunnelRuins_FreezeActor20(void) {
  u8 *flags;

  Engine_ActorSetAnimation(20, 1);
  Engine_ActorSetChildValue(20, 0);
  Engine_ActorSetAnimation(20, 2);
  flags = &Object_GetById(20)->priorityFlags;
  *flags &= ~ACTOR_PRIORITY_UNDERFOOT;
  Engine_GameFlagSet(0x200);
}

// Lower actor 19 after Douse when it occupies column 48.
static void TunnelRuins_LowerActor19(void) {
  u32 i;
  struct FieldActor *temporaryActor;
  s32 randomX;
  struct FieldActor *actor;
  s32 randomZ;
  s32 effectX;
  s32 effectZ;
  struct EffectOptions options;
  struct EffectOptions *effectOptions;

  Engine_EventBegin();
  actor = Object_GetById(19);
  if ((actor->x.fixed >> 20) == 48) {
    if (GameFlag_IsSet(0x202) != 0) {
      Engine_EventWait(30);
      temporaryActor =
          OverlayObject_PrepareObject(PIXELS(770), 0, PIXELS(274), 223);
      // Lower the Doused actor and scatter effects before restoring the
      // passage.
      effectOptions = &options;
      effectOptions->startScaleX = 3 * Q16_ONE / 5;
      effectOptions->startScaleY = 3 * Q16_ONE / 5;
      effectOptions->palette = 7;
      Actor_Get(19)->motionFlags = 0;
      Engine_AudioPlayCue(185);
      for (i = 0; i < 16; i++) {
        Engine_TaskWait(3);
        Actor_Get(19)->y.fixed -= PIXELS(1);
        randomX = Engine_RandomNext();
        effectX = ((((u32)(randomX << 4) >> 16) << 16) + PIXELS(768));
        randomZ = Engine_RandomNext();
        effectZ = ((((u32)(((randomZ << 3) + randomZ) << 1) >> 16) << 16) +
                   PIXELS(224));
        Effect_Spawn(effectX, 0, effectZ, 0, 0, 0, 0x90000, effectOptions);
      }
      // Restore the passage in two impact frames.
      Map_CopyCellAttributes(51, 8, 1, 1, 45, 14);
      Engine_EventWait(30);
      Actor_Get(19)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      Engine_ActorSetAnimation(19, 3);
      Engine_ObjectDispatchRelease(temporaryActor);
      Map_CopyCellAttributes(45, 4, 1, 1, 48, 14);
      Actor_SetPosition(21, PIXELS(776), PIXELS(232));
      TunnelRuins_AnimateLoweringImpact(58, 45, 14);
      Engine_EventWait(20);
      TunnelRuins_AnimateLoweringImpact(59, 45, 14);
      Engine_MapRenderWaitForValues();
      Engine_EventWait(10);
      GameFlag_Set(0x972);
    }
  }
  Engine_EventEnd();
}

// Advance the movable pair, then test whether actor 19 can lower.
static void TunnelRuins_MoveActor19AndCheckLowering(void) {
  Engine_EventBegin();
  StagedActor_AdvancePair();
  Engine_EventEnd();
  TunnelRuins_LowerActor19();
}

// Start the timed flicker script on actor 19.
static void TunnelRuins_StartActor19Flicker(void) {
  Actor_SetActionScript(19, gTunnelRuins_FlickerScript);
}

// Record Douse on actor 19 and test whether it can lower.
static void TunnelRuins_DouseActor19(void) {
  Engine_GameFlagSet(0x202);
  TunnelRuins_LowerActor19();
}

// Freeze actor 21 and restore its normal depth handling.
static void TunnelRuins_FreezeActor21(void) {
  u8 *flags;

  Engine_ActorSetAnimation(21, 1);
  Engine_ActorSetChildValue(21, 0);
  Engine_ActorSetAnimation(21, 2);
  flags = &Object_GetById(21)->priorityFlags;
  *flags &= ~ACTOR_PRIORITY_UNDERFOOT;
  Engine_GameFlagSet(0x201);
}

// Show the first statue message for the current answer state.
static void TunnelRuins_ReadFirstStatue(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  if (Engine_GameFlagIsSet(0x982) != 0 || Engine_GameFlagIsSet(0x983) != 0) {
    Engine_MessageShowCentered(MSGNO(MsgTunnelRuinsStatueSpeaksAfterAnswer), 1);
  } else {
    Engine_MessageShowCentered(MSGNO(MsgTunnelRuinsStatueSpeaksIsaacSoulYe), 1);
  }
  Engine_EventEnd();
}

// Enter and leave the scene event after the actor 8 interaction.
static void TunnelRuins_FinishActor8Move(void) {
  Engine_EventBegin();
  Engine_EventEnd();
}

// Advance the movable pair and finish the actor 8 interaction.
static void TunnelRuins_MoveActor8Pair(void) {
  Engine_EventBegin();
  StagedActor_AdvancePair();
  Engine_EventEnd();
  TunnelRuins_FinishActor8Move();
}

// Start actor 8 flickering and let actor 9 track its position.
static void TunnelRuins_StartActor8Flicker(void) {
  struct FieldActor *owner;

  Actor_SetActionScript(8, gTunnelRuins_FlickerScript);
  Engine_GameFlagSet(0x203);
  owner = Object_GetById(9);
#ifdef UBFIX
  owner->update = TunnelRuins_TrackActor8AtRow10;
#else
  owner->update = (void (*)(union FieldObject *))TunnelRuins_TrackActor8AtRow10;
#endif
}

// Freeze actor 9 and restore both actors to the depth update callback.
static void TunnelRuins_FreezeActor9(void) {
  struct FieldActor *actor;
  s32 column;
  s32 row;

  Engine_EventBegin();
  Engine_ActorSetSpritePriority(9, 1);
  Engine_ActorSetAnimation(9, 1);
  Engine_ActorSetChildValue(9, 0);
  Engine_ActorSetAnimation(9, 2);

  {
    u8 *flag = &Object_GetById(9)->priorityFlags;
    *flag &= ~ACTOR_PRIORITY_UNDERFOOT;
  }

  Engine_GameFlagSet(0x204);

  column = Object_GetById(9)->x.fixed;
  row = Object_GetById(9)->z.fixed >> 20;
  Map_CopyCellAttributes(26, 8, 1, 1, column >> 20, row);

  actor = Object_GetById(9);
  actor->update = ACTOR_UPDATE_IDLE;

  actor = Object_GetById(8);
  actor->update = ACTOR_UPDATE_IDLE;

  Engine_EventEnd();
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 34 to 32 bytes in tbs-en.
// Keep this actor 16 pixels above the supporting actor saved in its timer.
#ifdef UBFIX
static void
TunnelRuins_FollowSupportingPlatform(union FieldObject *fieldObject) {
#else
static s32
TunnelRuins_FollowSupportingPlatform(union FieldObject *fieldObject) {
#endif
  struct FieldActor *supportedActor = (struct FieldActor *)fieldObject;
  s16 *supportingActorId = &supportedActor->timer;
  struct FieldActor *supportingActor = Object_GetById(*supportingActorId);

  supportedActor->y.fixed = supportingActor->y.fixed + PIXELS(16);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

// Move both steps to their indexed heights and restore shallow collision cells.
static void TunnelRuins_SettleSteps(s32 wait) {
  u32 i;

  Actor_SetSpeed(10, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(11, PIXELS(1) / 2, PIXELS(1) / 4);
  if (wait != 0) {
    Engine_AudioPlayCue(180);
  }
  Engine_ObjectSetPosition(
      Object_GetById(10), Object_GetById(10)->x.fixed,
      gTunnelRuins_SettleHeights[Object_GetById(10)->timer],
      Object_GetById(10)->z.fixed);
  Engine_ObjectSetPosition(
      Object_GetById(11), Object_GetById(11)->x.fixed,
      gTunnelRuins_SettleHeights[Object_GetById(11)->timer],
      Object_GetById(11)->z.fixed);
  Engine_ActorWaitForMove(10);
  Engine_ActorWaitForMove(11);
  Object_GetById(10)->y.fixed =
      gTunnelRuins_SettleHeights[Object_GetById(10)->timer];
  Object_GetById(11)->y.fixed =
      gTunnelRuins_SettleHeights[Object_GetById(11)->timer];
  if (wait != 0) {
    Engine_AudioPlayCue(0x121);
  }
  for (i = 0; i < 5; i++) {
    if (Object_GetById(i + 10)->y.fixed / Q16_ONE < 0 &&
        Object_GetById(i + 10)->y.fixed / Q16_ONE > -30) {
      Engine_MapCopyCellAttributes(4, 9, 1, 1,
                                   Object_GetById(i + 10)->x.fixed >> 20,
                                   Object_GetById(i + 10)->z.fixed >> 20);
    }
  }
  Engine_EventWait(wait);
}

// Place the carried actor above the highest actor sharing its tile.
static void TunnelRuins_StackActorOnHighestPlatform(s32 actorId) {
  s32 highestHeight = -PIXELS(80);
  u32 i;

  for (i = 0; i <= 4; i++) {
    s32 candidateId = i + 10;

    if (candidateId == actorId)
      continue;

    if ((Object_GetById(candidateId)->x.fixed >> 20) !=
        (Object_GetById(actorId)->x.fixed >> 20))
      continue;
    if ((Object_GetById(candidateId)->z.fixed >> 20) !=
        (Object_GetById(actorId)->z.fixed >> 20))
      continue;

    if (highestHeight > Object_GetById(candidateId)->y.fixed + PIXELS(16))
      continue;

    highestHeight = Object_GetById(candidateId)->y.fixed + PIXELS(16);
    Object_GetById(actorId)->timer = candidateId;
  }

  Engine_ActorSetSpeed(actorId, PIXELS(4), PIXELS(2));

  {
    struct FieldActor *actor = Object_GetById(actorId);
    struct FieldActor *sourceActor = Object_GetById(actorId);
    s32 z = Object_GetById(actorId)->z.fixed;

    Engine_ObjectSetPosition(actor, sourceActor->x.fixed, highestHeight, z);
  }

  Engine_ActorWaitForMove(actorId);
  Engine_AudioPlayCue(188);
  SceneEffect_SpawnNineRadialEffects(actorId);
  Engine_EventWait(30);
}

// Settle a carried actor, stack it, and adjust the two step heights.
static void TunnelRuins_PlaceCarriedActor(void) {
  struct FieldActor savedPosition;
  s32 i;
  s32 j;
  s32 settledIndex;

  Engine_EventBegin();
  for (i = 0; i <= 2; i++) {
    if (Object_GetById(i + 12)->sprite->image.priority == 3 &&
        GameFlag_IsSet(i + 0x200) == 0) {
      TunnelRuins_WaitForActorToLand(Actor_Get(i + 12));
      Actor_SetPosition(i + 12, 0, 0);
      GameFlag_Set(i + 0x200);
      break;
    }
    if ((Object_GetById(i + 12)->z.fixed >> 20) == 9 &&
        GameFlag_IsSet(i + 0x200) == 0) {
      Object_GetById(i + 12)->terrainHeight = 0;
      Object_GetById(i + 12)->velocityY = 0;
      Object_GetById(i + 12)->targetY = ACTOR_NO_TARGET;
      Actor_Get(i + 12)->motionFlags = 0;
      Object_GetById(i + 12)->timer = 0;
      // Keep the first available actor slot at the carried object position.
      settledIndex = i;
      for (j = 0; j < i; j++) {
        if (GameFlag_IsSet(0x200 + j) == 0) {
          savedPosition.x.fixed = Object_GetById(i + 12)->x.fixed;
          savedPosition.y.fixed = Object_GetById(i + 12)->y.fixed;
          savedPosition.z.fixed = Object_GetById(i + 12)->z.fixed;
          Actor_Get(i + 12)->x.fixed = Object_GetById(j + 12)->x.fixed;
          Object_GetById(i + 12)->y.fixed = Object_GetById(j + 12)->y.fixed;
          Object_GetById(i + 12)->z.fixed = Object_GetById(j + 12)->z.fixed;
          Actor_Get(j + 12)->x.fixed = savedPosition.x.fixed;
          Actor_Get(j + 12)->y.fixed = savedPosition.y.fixed;
          Actor_Get(j + 12)->z.fixed = savedPosition.z.fixed;
          settledIndex = j;
          break;
        }
      }
      Object_GetById(settledIndex + 12)->terrainHeight = 0;
      Actor_Get(settledIndex + 12)->velocityY = 0;
      Object_GetById(settledIndex + 12)->targetY = ACTOR_NO_TARGET;
      Actor_Get(settledIndex + 12)->motionFlags = 0;
      Object_GetById(settledIndex + 12)->timer = 0;
      // Stack the actor, then rebalance the paired steps.
      Engine_CameraSetSpeed(PIXELS(3), PIXELS(3) / 8);
      Camera_GetEventObject()->motionFlags = 0;
      Camera_MoveTo(PIXELS(168), PIXELS(8), PIXELS(184), 1);
      Engine_CameraWaitForMove();
      TunnelRuins_StackActorOnHighestPlatform(settledIndex + 12);
      if ((Object_GetById(settledIndex + 12)->x.fixed >> 20) == 8) {
        Object_GetById(10)->timer++;
        (Object_GetById(11)->timer)--;
      } else {
        (Object_GetById(10)->timer)--;
        Object_GetById(11)->timer++;
      }
#ifdef UBFIX
      Actor_Get(settledIndex + 12)->update =
          TunnelRuins_FollowSupportingPlatform;
#else
      Actor_Get(settledIndex + 12)->update =
          (void (*)(union FieldObject *))TunnelRuins_FollowSupportingPlatform;
#endif
      TunnelRuins_SettleSteps(40);
      Actor_Get(settledIndex + 12)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      GameFlag_Set(0x200 + settledIndex);
      break;
    }
  }
  Engine_EventEnd();
}

// Advance the movable pair within a scene event.
static void TunnelRuins_AdvanceMovablePair(void) {
  Engine_EventBegin();
  StagedActor_AdvancePair();
  Engine_EventEnd();
}

// Save the two movable actors column states and rebuild their grid cells.
static void TunnelRuins_SaveActor17And18Positions(void) {
  Engine_EventBegin();

  if ((Object_GetById(17)->x.fixed >> 20) == 45) {
    Engine_GameFlagSet(0x974);
  } else {
    Engine_GameFlagClear(0x974);
  }

  if ((Object_GetById(18)->x.fixed >> 20) == 46) {
    Engine_GameFlagSet(0x975);
  } else {
    Engine_GameFlagClear(0x975);
  }

  TunnelRuins_MarkActorCells();
  Engine_EventEnd();
}

// Advance the movable pair and save the resulting positions.
static void TunnelRuins_MoveAndSaveActorPositions(void) {
  Engine_EventBegin();
  StagedActor_AdvancePair();
  TunnelRuins_SaveActor17And18Positions();
  Engine_EventEnd();
}

// Show the Reveal objects and restore their collision cells.
static void TunnelRuins_ShowRevealObjects(void) {
  Map_CopyCellAttributes(72, 49, 1, 1, 8, 49);
  Map_CopyCellAttributes(113, 43, 1, 1, 49, 43);
  MapObject_SetPosition(100, 0, 0);
  MapObject_SetPosition(101, 0, 0);
  Actor_SetPosition(15, PIXELS(136), PIXELS(792));
  Actor_SetPosition(16, PIXELS(792), PIXELS(696));
}

// Hide the Reveal objects and restore the alternate collision cells.
static void TunnelRuins_HideRevealObjects(void) {
  Map_CopyCellAttributes(8, 113, 1, 1, 8, 49);
  Map_CopyCellAttributes(49, 107, 1, 1, 49, 43);
  MapObject_SetPosition(100, -1, -1);
  MapObject_SetPosition(101, -1, -1);
  Actor_SetPosition(15, 0, 0);
  Actor_SetPosition(16, 0, 0);
}

// Open the door only while all five actors occupy their target tiles.
static void TunnelRuins_UpdateFiveActorDoor(void) {
  struct FieldActor *actor;
  s32 correctPositions;
  s32 column;
  s32 row;

  correctPositions = 0;
  Engine_EventBegin();
  Map_CopyCellAttributes(83, 45, 11, 8, 19, 45);
  actor = Object_GetById(19);
  column = actor->x.fixed;
  row = Object_GetById(19)->z.fixed;
  row >>= 20;
  column >>= 20;
  Map_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Object_GetById(20);
  column = actor->x.fixed;
  row = Object_GetById(20)->z.fixed;
  row >>= 20;
  column >>= 20;
  Map_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Object_GetById(21);
  column = actor->x.fixed;
  row = Object_GetById(21)->z.fixed;
  row >>= 20;
  column >>= 20;
  Map_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Object_GetById(22);
  column = actor->x.fixed;
  row = Object_GetById(22)->z.fixed;
  row >>= 20;
  column >>= 20;
  Map_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Object_GetById(23);
  column = actor->x.fixed;
  row = Object_GetById(23)->z.fixed;
  row >>= 20;
  column >>= 20;
  Map_CopyCellAttributes(20, 56, 1, 1, column, row);
  // Count actors at the five required tile positions.
  actor = Object_GetById(19);
  if ((actor->x.fixed >> 20) == 25) {
    actor = Object_GetById(19);
    if ((actor->z.fixed >> 20) == 49) {
      correctPositions = 1;
    }
  }
  actor = Object_GetById(20);
  if ((actor->x.fixed >> 20) == 23) {
    actor = Object_GetById(20);
    if ((actor->z.fixed >> 20) == 49) {
      correctPositions = (correctPositions + 1);
    }
  }
  actor = Object_GetById(21);
  if ((actor->x.fixed >> 20) == 25) {
    actor = Object_GetById(21);
    if ((actor->z.fixed >> 20) == 47) {
      correctPositions = (correctPositions + 1);
    }
  }
  actor = Object_GetById(22);
  if ((actor->x.fixed >> 20) == 23) {
    actor = Object_GetById(22);
    if ((actor->z.fixed >> 20) == 47) {
      correctPositions = (correctPositions + 1);
    }
  }
  actor = Object_GetById(23);
  if ((actor->x.fixed >> 20) == 24) {
    actor = Object_GetById(23);
    if ((actor->z.fixed >> 20) == 48) {
      correctPositions = (correctPositions + 1);
    }
  }
  // Animate the door only when the completed arrangement changes.
  if (correctPositions == 5) {
    if (GameFlag_IsSet(0x984) != 0) {
      Engine_EventEnd();
      return;
    }
    Engine_EventWait(20);
    Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
    Camera_MoveTo(PIXELS(472), -1, PIXELS(780), 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(30);
    GameFlag_Set(0x984);
    Engine_AudioPlayCue(158);
    Map_AnimateCells(gTunnelRuins_CellStepsA, 32, 46);
    Map_CopyCellAttributes(24, 60, 1, 1, 32, 47);
    Engine_EventWait(40);
  } else {
    if (GameFlag_IsSet(0x984) != 0) {
      Engine_EventWait(20);
      Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
      Camera_MoveTo(PIXELS(472), -1, PIXELS(780), 1);
      Engine_CameraWaitForMove();
      Engine_EventWait(30);
      GameFlag_Clear(0x984);
      Engine_AudioPlayCue(159);
      Map_AnimateCells(gTunnelRuins_CellStepsB, 32, 46);
      Map_CopyCellAttributes(31, 47, 1, 1, 32, 47);
      Engine_EventWait(40);
    }
  }
  Engine_EventEnd();
}

// Move the selected actor and reevaluate the five-actor door puzzle.
static void TunnelRuins_MoveDoorPuzzleActor(void) {
  Engine_EventBegin();
  {
    Map_CopyCellAttributes(83, 45, 11, 8, 19, 45);
  }
  StagedActor_AdvancePair();
  TunnelRuins_UpdateFiveActorDoor();
  Engine_EventEnd();
}

// Show the second statue soul-reading message.
static void TunnelRuins_ReadSecondStatue(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Engine_MessageShowCentered(MSGNO(MsgTunnelRuinsStatueSeemsSpeakYourSoul), 1);
  Engine_EventEnd();
}

// Show the tightly locked door message.
static void TunnelRuins_SearchLockedDoor(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MSGNO(MsgFieldDoorTightlyLocked), 1);
  Engine_EventEnd();
}

// Select the event bindings for the current tunnel ruins scene.
s32 TunnelRuins_SelectEvents(void) {
  if (gGameState.scene == (s32)&SceneId_TunnelRuins1) {
    return (s32)gTunnelRuinsEvents1;
  }
  return (s32)gTunnelRuinsEventsOther;
}

// Order actors 8 and 9 by depth and restore their collision cells.
static void TunnelRuins_RestoreFirstPairCells(void) {
  s32 row;

  {
    Map_CopyCellAttributes(73, 38, 5, 5, 9, 38);
  }
  SceneActor_SwapPositionsByDepth(9, 8);

  {
    s32 col = Object_GetById(8)->x.fixed >> 20;
    row = Object_GetById(8)->z.fixed >> 20;
    Map_CopyCellAttributes(2, 36, 1, 1, col, row);
  }

  {
    s32 col = Object_GetById(9)->x.fixed >> 20;
    row = Object_GetById(9)->z.fixed >> 20;
    Map_CopyCellAttributes(2, 36, 1, 1, col, row);
  }
}

// Order actors 10 and 11 by depth and restore their collision cells.
static void TunnelRuins_RestoreSecondPairCells(void) {
  s32 row;

  {
    Map_CopyCellAttributes(93, 30, 6, 5, 29, 30);
  }
  SceneActor_SwapPositionsByDepth(11, 10);

  {
    s32 column = Object_GetById(10)->x.fixed >> 20;
    row = Object_GetById(10)->z.fixed >> 20;
    Map_CopyCellAttributes(2, 36, 1, 1, column, row);
  }

  {
    s32 column = Object_GetById(11)->x.fixed >> 20;
    row = Object_GetById(11)->z.fixed >> 20;
    Map_CopyCellAttributes(2, 36, 1, 1, column, row);
  }
}

// Restore the platform group collision cells at each actor position.
static void TunnelRuins_RestorePlatformCells(void) {
  struct FieldActor *actor;
  s32 column;

  Map_CopyCellAttributes(89, 49, 3, 2, 25, 49);
  Map_CopyCellAttributes(89, 51, 8, 5, 25, 51);
  Object_GetById(14)->terrainId = 1;
  actor = Object_GetById(12);
  column = actor->x.fixed;
  actor = Object_GetById(12);
  column >>= 20;
  Map_CopyCellAttributes(22, 52, 1, 1, column, (actor->z.fixed >> 20));
  actor = Object_GetById(13);
  column = actor->x.fixed;
  actor = Object_GetById(13);
  column >>= 20;
  Map_CopyCellAttributes(22, 52, 1, 1, column, (actor->z.fixed >> 20));
  actor = Object_GetById(14);
  column = actor->x.fixed;
  actor = Object_GetById(14);
  column >>= 20;
  Map_CopyCellAttributes(22, 52, 1, 1, column, (actor->z.fixed >> 20));
}

// Restore the base grid and mark both movable actor footprints.
static void TunnelRuins_MarkActorCells(void) {
  Map_CopyCellAttributes(108, 19, 4, 1, 44, 19);
  StagedActor_FillGridAttributeRectangle(0, Object_GetById(17)->x.fixed >> 20,
                                         Object_GetById(17)->z.fixed >> 20, 1,
                                         1, 0xff);
  StagedActor_FillGridAttributeRectangle(0, Object_GetById(18)->x.fixed >> 20,
                                         Object_GetById(18)->z.fixed >> 20, 1,
                                         1, 0xff);
}

// Restore the current entrance puzzle state and actor callbacks.
s32 FieldScene_InitializeActorGroups(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gGameState.scene == (s32)&SceneId_TunnelRuins1 ||
      gGameState.scene == (s32)&SceneId_TunnelRuins2) {
    BattleFx_StartFadeOverlay(0);
    gGameState.retreatEntrance = 1;
    gGameState.retreatScene = (s32)&SceneId_TunnelRuins1;
  }
  if (gGameState.scene == (s32)&SceneId_TunnelRuins1) {
    switch (gGameState.entrance) {
    case 1:
    case 2:
      if (Engine_GameFlagIsSet(0x982)) {
        Map_CopyCellsTo(121, 4, 74, 9, 5, 8);
        Map_CopyCellsTo(18, 83, 9, 73, 3, 2);
        Map_CopyCellsTo(18, 81, 9, 75, 3, 2);
        Map_CopyCellsTo(18, 83, 9, 77, 3, 2);
        Map_CopyCellsTo(18, 83, 9, 79, 3, 2);
        Map_CopyCellsTo(18, 83, 11, 78, 3, 2);
        Map_CopyCellsTo(18, 83, 13, 79, 3, 2);
      } else {
        if (!Engine_GameFlagIsSet(0x983))
          break;
        Map_CopyCellsTo(121, 13, 74, 9, 5, 8);
        Map_CopyCellsTo(18, 85, 11, 74, 3, 2);
        Map_CopyCellsTo(18, 83, 13, 75, 3, 2);
        Map_CopyCellsTo(18, 85, 11, 76, 3, 2);
        Map_CopyCellsTo(18, 83, 11, 78, 3, 2);
        Map_CopyCellsTo(18, 83, 13, 79, 3, 2);
      }
      break;
    case 3:
    case 4:
      TunnelRuins_RestoreFirstPairCells();
      Object_GetById(8)->motionFlags = 0;
      Object_GetById(9)->motionFlags = 0;
      Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
      Object_GetById(8)->update = ACTOR_UPDATE_IDLE;
      Object_GetById(9)->update = ACTOR_UPDATE_IDLE;
      FieldScheduler_AddCallback(TunnelRuins_UpdateTrackedActor,
                                 TUNNEL_CALLBACK_PRIORITY);
      break;
    case 5:
    case 6:
    case 7:
      if (Engine_GameFlagIsSet(0x982))
        Map_CopyCells(23, 17, 1, 2, 30, 8);
      if (Engine_GameFlagIsSet(0x983))
        Map_CopyCells(23, 17, 1, 2, 32, 10);
      break;
    case 8:
    case 9:
      TunnelRuins_RestoreSecondPairCells();
      Object_GetById(10)->motionFlags = 0;
      Object_GetById(11)->motionFlags = 0;
      Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
      Object_GetById(10)->update = ACTOR_UPDATE_IDLE;
      Object_GetById(11)->update = ACTOR_UPDATE_IDLE;
      FieldScheduler_AddCallback(TunnelRuins_UpdateTrackedActor,
                                 TUNNEL_CALLBACK_PRIORITY);
      break;
    case 10:
    case 11:
      Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(19), 0);
      Engine_ActorSetAnimation(18, 2);
      Engine_ActorSetSpriteFlags(Object_GetById(20), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
      Engine_ActorSetChildValue(20, 15);
      Engine_ActorSetChildValue(21, 15);
      if (Engine_GameFlagIsSet(0x971)) {
        Map_CopyCellsTo(59, 8, 49, 8, 1, 3);
        Map_CopyCellAttributes(51, 8, 1, 1, 49, 8);
        Object_GetById(18)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetAnimation(18, 3);
        Map_CopyCellAttributes(45, 4, 1, 1, 46, 8);
        Actor_SetPosition(18, PIXELS(744), PIXELS(136));
        Object_GetById(18)->y.fixed = -PIXELS(16);
        Actor_SetPosition(20, PIXELS(744), PIXELS(136));
      }
      if (Engine_GameFlagIsSet(0x200)) {
        Engine_ActorSetChildValue(20, 0);
        Engine_ActorSetAnimation(20, 5);
      }
      if (GameFlag_IsSet(0x202))
        Engine_ActorSetAnimation(19, 2);
      if (Engine_GameFlagIsSet(0x972)) {
        Map_CopyCellsTo(59, 8, 45, 14, 1, 3);
        Map_CopyCellAttributes(51, 8, 1, 1, 45, 14);
        Object_GetById(19)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetAnimation(19, 3);
        Map_CopyCellAttributes(45, 4, 1, 1, 48, 14);
        Actor_SetPosition(19, PIXELS(776), PIXELS(232));
        Object_GetById(19)->y.fixed = -PIXELS(16);
        Engine_ActorSetPosition(21, PIXELS(776), PIXELS(232));
        Engine_GameFlagSet(0x202);
      }
      if (Engine_GameFlagIsSet(0x201)) {
        Engine_ActorSetChildValue(21, 0);
        Engine_ActorSetAnimation(21, 5);
      }
      break;
    case 12:
    case 13:
      TunnelRuins_RestorePlatformCells();
      Object_GetById(12)->motionFlags = 0;
      Object_GetById(13)->motionFlags = 0;
      Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(16), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
      Object_GetById(12)->update = ACTOR_UPDATE_IDLE;
      Object_GetById(13)->update = ACTOR_UPDATE_IDLE;
      Object_GetById(14)->update = ACTOR_UPDATE_IDLE;
      FieldScheduler_AddCallback(TunnelRuins_UpdateTrackedActor,
                                 TUNNEL_CALLBACK_PRIORITY);
      break;
    }
  } else {
    switch (gGameState.entrance) {
    case 0:
      break;
    case 1:
    case 2:
    case 3:
      gGameState.retreatEntrance = 1;
      gGameState.retreatScene = (s32)&SceneId_BabiLighthouse3;
      Engine_GameFlagClear(0x12f);
      Engine_ActorSetChildValue(17, 6);
      Engine_ActorSetChildValue(18, 6);
      if (Engine_GameFlagIsSet(0x974))
        Actor_SetPosition(17, PIXELS(728), PIXELS(312));
      if (Engine_GameFlagIsSet(0x975))
        Actor_SetPosition(18, PIXELS(744), PIXELS(312));
      TunnelRuins_MarkActorCells();
      break;
    case 6:
    case 7:
      Engine_ActorSetSpritePriority(8, 1);
      Object_GetById(8)->motionFlags = 0;
      Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
      Engine_ActorSetSpritePriority(9, 1);
      Engine_ActorSetChildValue(9, 15);
      Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
      Object_GetById(9)->motionFlags = 0;
      if (!Engine_GameFlagIsSet(0x204))
        break;
      Engine_ActorSetChildValue(9, 0);
      Engine_ActorSetAnimation(9, 5);
      Engine_MapCopyCellAttributes(26, 8, 1, 1,
                                   Object_GetById(9)->x.fixed >> 20,
                                   Object_GetById(9)->z.fixed >> 20);
      Object_GetById(9)->update = ACTOR_UPDATE_IDLE;
      Object_GetById(8)->update = ACTOR_UPDATE_IDLE;
      break;
    case 4:
    case 5:
      if (!Engine_GameFlagIsSet(0x109)) {
        Object_GetById(10)->motionFlags = 0;
        Object_GetById(11)->motionFlags = 0;
        Object_GetById(10)->y.fixed = -PIXELS(48);
        Object_GetById(11)->y.fixed = -PIXELS(48);
        Object_GetById(10)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        Object_GetById(11)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        Object_GetById(10)->collisionFlags &= 0xfe;
        Object_GetById(11)->collisionFlags &= 0xfe;
        Object_GetById(10)->timer = 3;
        Object_GetById(11)->timer = 3;
        Engine_ActorSetSpritePriority(10, 1);
        Engine_ActorSetSpritePriority(11, 1);
        Object_GetById(12)->motionFlags = 0;
        Object_GetById(13)->motionFlags = 0;
        Object_GetById(14)->motionFlags = 0;
        Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
        Object_GetById(12)->timer = 0;
        Object_GetById(13)->timer = 0;
        Object_GetById(14)->timer = 0;
        if (gGameState.entrance != 5)
          break;
        Object_GetById(10)->y.fixed = -PIXELS(32);
        Object_GetById(11)->y.fixed = -PIXELS(64);
        Object_GetById(10)->timer = 2;
        Object_GetById(11)->timer = 4;
        Actor_SetPosition(12, PIXELS(200), PIXELS(152));
        Object_GetById(12)->timer = 11;
        Object_GetById(12)->update = ACTOR_UPDATE_PANEL;
        Object_GetById(12)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(13, PIXELS(200), PIXELS(152));
        Object_GetById(13)->timer = 12;
        Object_GetById(13)->update = ACTOR_UPDATE_PANEL;
        Object_GetById(13)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetPosition(14, PIXELS(136), PIXELS(152));
        Object_GetById(14)->timer = 10;
        Object_GetById(14)->update = ACTOR_UPDATE_PANEL;
        Object_GetById(14)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        Engine_EventWait(2);
        Engine_GameFlagSet(0x200);
        Engine_GameFlagSet(0x201);
        Engine_GameFlagSet(0x202);
      }
      TunnelRuins_SettleSteps(0);
      break;
    case 8:
    case 9:
    case 10:
    case 11:
      if (Engine_GameFlagIsSet(0x982))
        Map_CopyCells(10, 30, 1, 2, 16, 30);
      if (Engine_GameFlagIsSet(0x983))
        Map_CopyCells(10, 30, 1, 2, 22, 30);
      Engine_GameFlagSet(0x973);
      break;
    case 12:
      Map_CopyCellAttributes(8, 49, 1, 1, 8, 113);
      TunnelRuins_ShowRevealObjects();
      FieldScheduler_AddCallback(TunnelRuins_UpdateTrackedActor,
                                 TUNNEL_CALLBACK_PRIORITY);
      break;
    case 13:
    case 14:
      Engine_EventWait(1);
      if (Engine_GameFlagIsSet(0x984)) {
        Map_CopyCells(24, 59, 1, 2, 32, 46);
        Actor_SetPosition(19, PIXELS(408), PIXELS(792));
        Actor_SetPosition(20, PIXELS(376), PIXELS(792));
        Actor_SetPosition(21, PIXELS(408), PIXELS(760));
        Actor_SetPosition(22, PIXELS(376), PIXELS(760));
        Actor_SetPosition(23, PIXELS(392), PIXELS(776));
      }
      Object_GetById(19)->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
      Object_GetById(20)->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
      Object_GetById(21)->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
      Object_GetById(22)->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
      Object_GetById(23)->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
      Engine_ActorSetChildValue(19, 4);
      Engine_ActorSetChildValue(20, 1);
      Engine_ActorSetChildValue(21, 4);
      Engine_ActorSetChildValue(22, 10);
      Engine_ActorSetChildValue(23, 0);
      Engine_ActorSetAnimation(19, 2);
      Engine_ActorSetAnimation(23, 2);
      Engine_MapCopyCellAttributes(20, 56, 1, 1,
                                   Object_GetById(19)->x.fixed >> 20,
                                   Object_GetById(19)->z.fixed >> 20);
      Engine_MapCopyCellAttributes(20, 56, 1, 1,
                                   Object_GetById(20)->x.fixed >> 20,
                                   Object_GetById(20)->z.fixed >> 20);
      Engine_MapCopyCellAttributes(20, 56, 1, 1,
                                   Object_GetById(21)->x.fixed >> 20,
                                   Object_GetById(21)->z.fixed >> 20);
      Engine_MapCopyCellAttributes(20, 56, 1, 1,
                                   Object_GetById(22)->x.fixed >> 20,
                                   Object_GetById(22)->z.fixed >> 20);
      Engine_MapCopyCellAttributes(20, 56, 1, 1,
                                   Object_GetById(23)->x.fixed >> 20,
                                   Object_GetById(23)->z.fixed >> 20);
      break;
    case 17:
      Map_CopyCellAttributes(49, 43, 1, 1, 49, 107);
      TunnelRuins_ShowRevealObjects();
      break;
    }
  }
  return 0;
}

// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8,  32, 8,  -8,  -32, 8,  32, -32, -16, 32, 0,
    -8,  -32, 8,  32, -32, -8,  32, 8,  -8,  -32, 8,  32,
};

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

// Step heights indexed by the supporting actor's timer.
u32 gTunnelRuins_SettleHeights[] = {
    -PIXELS(16), -PIXELS(16), -PIXELS(32), -PIXELS(48),
    -PIXELS(64), -PIXELS(80), -PIXELS(80),
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gTunnelRuins_ShrinkScript[] = {
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 5),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 5),
    SC_SET(SW_COLLISION, 0),
    SC_CALL(TunnelRuins_RefreshActorAfterShrinking),
    SC_END,
};

s32 gTunnelRuins_StretchScript[] = {
    SC_SET(SW_SCALE_Y, 8 * Q16_ONE / 5),
    SC_END,
};

s32 gTunnelRuins_FlickerScript[] = {
    SC_WAIT(35),  SC_SET(SW_HOOK, (s32)TunnelRuins_UpdateFlickerEffect),
    SC_WAIT(130), SC_SET(SW_HOOK, 0),
    SC_ARG(2),    SC_END,
};

u16 gTunnelRuins_CellStepsA[] = {
    MAP_CELL_STEP(28, 59, 1, 2, 4),
    MAP_CELL_STEP(26, 59, 1, 2, 4),
    MAP_CELL_STEP(24, 59, 1, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gTunnelRuins_CellStepsB[] = {
    MAP_CELL_STEP(26, 59, 1, 2, 4),
    MAP_CELL_STEP(28, 59, 1, 2, 4),
    MAP_CELL_STEP(30, 59, 1, 2, 4),
    MAP_CELL_STEPS_END,
};

struct SceneEntrance gTunnelRuinsEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 744, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 440, 0, 552, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gTunnelRuinsEntrances1[] = {
    {0, CONDITION_ALWAYS, 744, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 104, 0, 152, FACING_SOUTH, 0, 48, 48, 288, 368, 0},
    {2, CONDITION_ALWAYS, 200, 0, 328, FACING_NORTH, 0, 48, 48, 288, 368, 0},
    {3, CONDITION_ALWAYS, 184, 0, 552, FACING_SOUTH, 0, 64, 464, 304, 816, 0},
    {4, CONDITION_ALWAYS, 168, 0, 816, FACING_NORTH, 0, 64, 464, 304, 816, 0},
    {5, CONDITION_ALWAYS, 520, 0, 104, FACING_SOUTH, 0, 336, 48, 576, 288, 0},
    {6, CONDITION_ALWAYS, 392, 0, 192, FACING_NORTH, 0, 336, 48, 576, 288, 0},
    {7, CONDITION_ALWAYS, 520, 0, 256, FACING_NORTH, 0, 336, 48, 576, 288, 0},
    {8, CONDITION_ALWAYS, 568, 0, 440, FACING_SOUTH, 0, 384, 352, 624, 640, 0},
    {9, CONDITION_ALWAYS, 440, 0, 648, FACING_NORTH, 0, 384, 352, 624, 640, 0},
    {10, CONDITION_ALWAYS, 744, 0, 104, FACING_SOUTH, 0, 624, 40, 864, 288, 0},
    {11, CONDITION_ALWAYS, 680, 0, 256, FACING_NORTH, 0, 624, 40, 864, 288, 0},
    {12, CONDITION_ALWAYS, 504, 0, 792, FACING_SOUTH, 0, 368, 720, 608, 976, 0},
    {13, CONDITION_ALWAYS, 568, 0, 872, FACING_SOUTH, 0, 368, 720, 608, 976, 0},
    {14, CONDITION_ALWAYS, 776, 0, 432, FACING_NORTH, 0, 688, 368, 928, 640, 0},
    {15, CONDITION_ALWAYS, 840, 0, 608, FACING_NORTH, 0, 688, 368, 928, 640, 0},
    {16, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 688, 736, 928, 976, 0},
    {17, CONDITION_ALWAYS, 744, 0, 944, FACING_NORTH, 0, 688, 736, 928, 976, 0},
    {18, CONDITION_ALWAYS, 872, 0, 880, FACING_NORTH, 0, 688, 736, 928, 976, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gTunnelRuinsEntrances2[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 632, 0, 344, FACING_SOUTH, 0, 592, 64, 880, 400, 0},
    {2, CONDITION_ALWAYS, 840, 0, 120, FACING_SOUTH, 0, 592, 64, 880, 400, 0},
    {3, CONDITION_ALWAYS, 824, 0, 344, FACING_SOUTH, 0, 592, 64, 880, 400, 0},
    {4, CONDITION_ALWAYS, 216, 0, 104, FACING_SOUTH, 0, 32, 48, 272, 320, 0},
    {5, CONDITION_ALWAYS, 216, 0, 296, FACING_NORTH, 0, 32, 48, 272, 320, 0},
    {6, CONDITION_ALWAYS, 360, 0, 104, FACING_SOUTH, 0, 304, 48, 544, 352, 0},
    {7, CONDITION_ALWAYS, 488, 0, 328, FACING_NORTH, 0, 304, 48, 544, 352, 0},
    {8, CONDITION_ALWAYS, 120, 0, 568, FACING_NORTH, 0, 80, 400, 448, 592, 0},
    {9, CONDITION_ALWAYS, 216, 0, 456, FACING_SOUTH, 0, 80, 400, 448, 592, 0},
    {10, CONDITION_ALWAYS, 312, 0, 568, FACING_NORTH, 0, 80, 400, 448, 592, 0},
    {11, CONDITION_ALWAYS, 408, 0, 456, FACING_SOUTH, 0, 80, 400, 448, 592, 0},
    {12, CONDITION_ALWAYS, 136, 0, 712, FACING_SOUTH, 0, 16, 656, 256, 880, 0},
    {13, CONDITION_ALWAYS, 392, 0, 712, FACING_SOUTH, 0, 288, 656, 560, 864, 0},
    {14, CONDITION_ALWAYS, 520, 0, 776, FACING_SOUTH, 0, 288, 656, 560, 864, 0},
    {15, CONDITION_ALWAYS, 648, 0, 952, FACING_NORTH, 0, 608, 816, 848, 976, 0},
    {16, CONDITION_ALWAYS, 776, 0, 904, FACING_SOUTH, 0, 608, 816, 848, 976, 0},
    {17, CONDITION_ALWAYS, 696, 0, 680, FACING_SOUTH, 0, 624, 592, 864, 768, 0},
    {SCENE_TABLE_END},
};

s16 gTunnelRuinsRegions2[] = {
    624, 16, 336, 640, 32, 352, -1, 15, 816, 16, 336, 832, 32, 352, -1, 16,
    832, 16, 112, 848, 32, 128, -1, 17, -1,  0,  0,   0,   0,  0,   0,  0,
};

u32 gTunnelRuinsExits[] = {
    SCENE_EXITS(172),        SCENE_EXIT(1, 174, 1),   SCENE_EXIT(2, 172, 3),
    SCENE_EXIT(3, 172, 2),   SCENE_EXIT(4, 172, 5),   SCENE_EXIT(5, 172, 4),
    SCENE_EXIT(6, 172, 8),   SCENE_EXIT(7, 172, 10),  SCENE_EXIT(8, 172, 6),
    SCENE_EXIT(9, 172, 12),  SCENE_EXIT(10, 172, 7),  SCENE_EXIT(11, 172, 16),
    SCENE_EXIT(12, 172, 9),  SCENE_EXIT(13, 172, 14), SCENE_EXIT(14, 172, 13),
    SCENE_EXIT(15, 173, 4),  SCENE_EXIT(16, 172, 11), SCENE_EXIT(17, 173, 6),
    SCENE_EXIT(18, 173, 17), SCENE_EXIT(0, 173, 0),   SCENE_EXIT(1, 172, 15),
    SCENE_EXIT(2, 173, 9),   SCENE_EXIT(3, 172, 17),  SCENE_EXIT(4, 173, 11),
    SCENE_EXIT(5, 173, 5),   SCENE_EXIT(6, 173, 7),   SCENE_EXIT(7, 173, 12),
    SCENE_EXIT(8, 173, 13),  SCENE_EXIT(9, 173, 8),   SCENE_EXIT(10, 173, 10),
    SCENE_EXIT(11, 173, 15), SCENE_EXIT(12, 172, 18), SCENE_EXIT(13, 173, 14),
    SCENE_EXIT(14, 183, 18), SCENE_EXIT(15, 176, 4),  SCENE_EXIT(16, 176, 5),
    SCENE_EXIT(17, 176, 6),  SCENE_EXITS_END,
};

struct ScenePlacement gTunnelRuinsPlacements1[] = {
    {0xfd, 0x0, ACTOR_STAND, PIXELS(152), 0, PIXELS(616), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0xfd, 0x0, ACTOR_STAND, PIXELS(168), 0, PIXELS(616), FACING_SOUTH,
     TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {0xfd, 0x0, ACTOR_STAND, PIXELS(520), 0, PIXELS(488), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0xfd, 0x0, ACTOR_STAND, PIXELS(536), 0, PIXELS(488), FACING_SOUTH,
     TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {0xfe, 0x0, ACTOR_STAND, PIXELS(488), 0, PIXELS(840), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0xfe, 0x0, ACTOR_STAND, PIXELS(520), 0, PIXELS(840), FACING_SOUTH,
     TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {0xfe, 0x0, ACTOR_STAND, PIXELS(424), 0, PIXELS(792), FACING_SOUTH,
     TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {0xf8, 0x0, (s32)gTunnelRuins_StretchScript, PIXELS(416), 0, PIXELS(794),
     FACING_NORTH, TALK_FACE_PARTY, 0},
    {0xf8, 0x0, (s32)gTunnelRuins_StretchScript, PIXELS(432), 0, PIXELS(794),
     FACING_NORTH, TALK_FACE_PARTY, 0},
    {0xf8, 0x970, (s32)gTunnelRuins_StretchScript, PIXELS(568), 0, PIXELS(856),
     FACING_NORTH, TALK_FACE_PARTY, 0},
    {0xff, 0x0, ACTOR_STAND, PIXELS(680), 0, PIXELS(136), FACING_NORTH,
     TALK_KEEP_FACING, 0},
    {0xff, 0x0, ACTOR_STAND, PIXELS(808), 0, PIXELS(232), FACING_NORTH,
     TALK_KEEP_FACING, 0},
    {0xe3, 0x0, ACTOR_STAND, PIXELS(728), 0, PIXELS(184), FACING_NORTH,
     TALK_KEEP_FACING, 0},
    {0xe3, 0x0, ACTOR_STAND, PIXELS(728), 0, PIXELS(184), FACING_NORTH,
     TALK_KEEP_FACING, 0},
    {0x114, 0x0, (s32)gTunnelRuins_ShrinkScript, PIXELS(232), 0, PIXELS(248),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gTunnelRuinsPlacementsOther[] = {
    {255, 0, ACTOR_STAND, PIXELS(392), 0, PIXELS(168), FACING_NORTH,
     TALK_KEEP_FACING, 0},
    {227, 0, ACTOR_STAND, PIXELS(392), 0, PIXELS(248), FACING_NORTH,
     TALK_KEEP_FACING, 0},
    {257, 0, 7, PIXELS(136), 0, PIXELS(152), FACING_NORTH, TALK_KEEP_FACING, 0},
    {257, 0, 7, PIXELS(200), 0, PIXELS(152), FACING_NORTH, TALK_KEEP_FACING,
     PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {256, 0, ACTOR_STAND, PIXELS(152), 0, PIXELS(136), FACING_NORTH,
     TALK_KEEP_FACING, 0},
    {256, 0, ACTOR_STAND, PIXELS(184), 0, PIXELS(136), FACING_NORTH,
     TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {256, 0, ACTOR_STAND, PIXELS(216), 0, PIXELS(136), FACING_NORTH,
     TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {258, 0, ACTOR_STAND, PIXELS(136), 0, PIXELS(792), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {258, 0, ACTOR_STAND, PIXELS(792), 0, PIXELS(696), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {253, 0, ACTOR_STAND, PIXELS(744), 0, PIXELS(312), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {253, 0, ACTOR_STAND, PIXELS(760), 0, PIXELS(312), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {500, 0, ACTOR_STAND, PIXELS(344), 0, PIXELS(760), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {500, 0, ACTOR_STAND, PIXELS(440), 0, PIXELS(760), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {500, 0, ACTOR_STAND, PIXELS(344), 0, PIXELS(808), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {500, 0, ACTOR_STAND, PIXELS(440), 0, PIXELS(808), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {500, 0, ACTOR_STAND, PIXELS(392), 0, PIXELS(824), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct SceneEvent gTunnelRuinsEvents1[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
    {EVENT_EXIT, 7, CONDITION_ALWAYS, 7},
    {EVENT_EXIT, 8, CONDITION_ALWAYS, 8},
    {EVENT_EXIT, 9, CONDITION_ALWAYS, 9},
    {EVENT_EXIT, 10, CONDITION_ALWAYS, 10},
    {EVENT_EXIT, 11, CONDITION_ALWAYS, 11},
    {EVENT_EXIT, 12, CONDITION_ALWAYS, 12},
    {EVENT_EXIT, 13, CONDITION_ALWAYS, 13},
    {EVENT_EXIT, 14, CONDITION_ALWAYS, 14},
    {EVENT_EXIT, 15, CONDITION_ALWAYS, 15},
    {EVENT_EXIT, 16, CONDITION_ALWAYS, 16},
    {EVENT_EXIT, 17, CONDITION_ALWAYS, 17},
    {EVENT_EXIT, 18, CONDITION_ALWAYS, 18},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_JumpLeaderDownLedge)},
    {TOUCH_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderWest)},
    {0x202, 24, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_MoveFirstActorPair)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_RunLeaderSpray)},
    {0x202, 26, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_MoveSecondActorPair)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_JumpLeaderDownLedge)},
    {TOUCH_WALKING_INTO(FACING_EAST), 27, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderEast)},
    {TOUCH_WALKING_INTO(FACING_WEST), 50, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_MoveWestUnlessAtTile32x50)},
    {TOUCH_WALKING_INTO(FACING_EAST), 50, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderEast)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 50, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_MoveNorthUnlessAtTile30x52)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 50, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderSouth)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 28, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_EnterPlatformGroup)},
    {0x202, 28, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_MovePlatformGroup)},
    {0x202, 29, 0x971, EVENT_SCRIPT(TunnelRuins_MoveActor18AndCheckLowering)},
    {TOUCH_WALKING_INTO(FACING_EAST), 29, 0x200,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderEast)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 29, 0x200,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderSouth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 29, 0x200,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderNorth)},
    {TOUCH_WALKING_INTO(FACING_WEST), 31, 0x200,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderWest)},
    {TOUCH_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderEast)},
    {0x202, 30, 0x972, EVENT_SCRIPT(TunnelRuins_MoveActor19AndCheckLowering)},
    {TOUCH_WALKING_INTO(FACING_WEST), 30, 0x201,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderWest)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 30, 0x201,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderSouth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 30, 0x201,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderNorth)},
    {TOUCH_WALKING_INTO(FACING_EAST), 32, 0x202, EVENT_SCRIPT(TunnelRuins_MoveActor19AndCheckLowering)},
    {TOUCH_WALKING_INTO(FACING_EAST), 32, 0x201,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderEast)},
    {TOUCH_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderWest)},
    {SEARCH(SEARCH_UNNAMED), 40, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_ReadFirstStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_RestoreFirstPairCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_RestoreFirstPairCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_RestoreSecondPairCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_RestoreSecondPairCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_RestorePlatformCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_RestorePlatformCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_RestorePlatformCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 15, 0x200, EVENT_SCRIPT(TunnelRuins_ActivateWhirlwindSwitch15)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 16, 0x201, EVENT_SCRIPT(TunnelRuins_ActivateWhirlwindSwitch16)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 17, 0x970, EVENT_SCRIPT(TunnelRuins_CompleteWhirlwind17)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, 0x971, EVENT_SCRIPT(TunnelRuins_LowerActor18)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 20, 0x200, EVENT_SCRIPT(TunnelRuins_FreezeActor20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, 0x972, EVENT_SCRIPT(TunnelRuins_LowerActor19)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 21, 0x201, EVENT_SCRIPT(TunnelRuins_FreezeActor21)},
    {0x10002115, 19, 0x202, EVENT_SCRIPT(TunnelRuins_StartActor19Flicker)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 19, 0x202, EVENT_SCRIPT(TunnelRuins_DouseActor19)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gTunnelRuinsEventsOther[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
    {EVENT_EXIT, 7, CONDITION_ALWAYS, 7},
    {EVENT_EXIT, 8, CONDITION_ALWAYS, 8},
    {EVENT_EXIT, 9, CONDITION_ALWAYS, 9},
    {EVENT_EXIT, 10, CONDITION_ALWAYS, 10},
    {EVENT_EXIT, 11, CONDITION_ALWAYS, 11},
    {EVENT_EXIT, 12, CONDITION_ALWAYS, 12},
    {EVENT_EXIT, 13, CONDITION_ALWAYS, 13},
    {0x21, 14, CONDITION_ALWAYS, 14},
    {0x202, 25, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_MoveActor8Pair)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_JumpLeaderDownLedge)},
    {TOUCH_WALKING_INTO(FACING_WEST), 26, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderWest)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_RunLeaderSpray)},
    {0x202, 28, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_AdvanceMovablePair)},
    {TOUCH_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderEast)},
    {TOUCH_WALKING_INTO(FACING_WEST), 30, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderWest)},
    {0x202, 30, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_AdvanceMovablePair)},
    {TOUCH_WALKING_INTO(FACING_EAST), 29, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_MoveAndSaveActorPositions)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 29, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderSouth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 29, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_MoveLeaderNorth)},
    {0x202, 41, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_MoveDoorPuzzleActor)},
    {SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_SearchLockedDoor)},
    {SEARCH(SEARCH_UNNAMED), 42, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_ReadSecondStatue)},
    {SEARCH(SEARCH_CHEST), 100, 0xf32, FIND_ITEM(0x71)},
    {SEARCH(SEARCH_CHEST), 101, 0xf33, FIND_ITEM(0x54)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x204, EVENT_SCRIPT(TunnelRuins_FinishActor8Move)},
    {0x10002115, 8, 0x203, EVENT_SCRIPT(TunnelRuins_StartActor8Flicker)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x204, EVENT_SCRIPT(TunnelRuins_FreezeActor9)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), 12, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_PlaceCarriedActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), 13, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_PlaceCarriedActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), 14, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_PlaceCarriedActor)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_HideRevealObjects)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_ShowRevealObjects)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_SaveActor17And18Positions)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_SaveActor17And18Positions)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateFiveActorDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 20, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateFiveActorDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 21, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateFiveActorDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 22, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateFiveActorDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 23, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateFiveActorDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsRedShinesNorthwestOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsYellowShinesNortheastOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsRedShinesDueWestOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsVioletShinesSoutheastOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsVioletPointsSoutheast)},
    {SCENE_EVENTS_END},
};
// clang-format on
