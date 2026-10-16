#include "game_state.h"
#include "version.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/altmiller_cave.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/shop/shop.h"
#include "unit/runtime/runtime.h"
#include "iowrite.h"

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
#include "staged_actor.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "edition.h"
#include "object_dispatch.h"
#include "scene_ids.h"
#include "dma.h"
#include "io_reg.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "call.h"
#include "display_transition.h"
#include "effect_slot.h"
#include "item_ids.h"
#include "objcmd.h"
#include "frame.h"
#include "fixed_math.h"
#include "field_runtime.h"
#include "fx_scene.h"

enum {
  ACTOR_INVISIBLE_BABI = 8,
  ACTOR_BABI = 9,
  ACTOR_FIRST_GUARD = 10,
  ACTOR_SECOND_GUARD = 11
};

// Forward declarations.
static void AltmillerCave_ReleaseWallBurst(void);

struct ActorWheel {
  s16 phase;
  s16 ticks;
  s16 unk04;
  u16 angle;
  u16 speed;
  u16 travel;
};

struct RockPuzzle {
  u8 state;
  s8 chosen;
};

#define AltmillerCave_RockPuzzle ((struct RockPuzzle *)&gSceneState[1])

struct ActorWheel gAltmillerActorWheel __attribute__((nocommon, aligned(4)));

// Shared source is included here in ROM order.
#include "common/object_staged_actor.h"

// Begin the battle with the Djinni in the fourth chamber.
static void AltmillerCave_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(13, 0x41);
}

// Return the entrances shared by the cave's six chambers.
struct SceneEntrance *AltmillerCave_GetEntrances(void) {
  return gAltmillerCave_Entrances;
}

// The cave has no additional scene regions.
struct SceneRegion *AltmillerCave_GetRegions(void) { return NULL; }

// Return the connections between the cave chambers.
u32 *AltmillerCave_GetExits(void) { return gAltmillerCave_Exits; }

// Choose the actor placements for the current chamber.
const struct ScenePlacement *AltmillerCave_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_AltmillerCave2) {
    return gAltmillerCavePlacements2;
  }
  if (scene == (s32)&SceneId_AltmillerCave4) {
    return gAltmillerCavePlacements4;
  }
  if (scene == (s32)&SceneId_AltmillerCave6) {
    return gAltmillerCavePlacements6;
  }
  return gAltmillerCavePlacementsOther;
}

// Apply the saved darkness level to the background blend.
static void AltmillerCave_ApplyFadeBlend(void) {
  s8 level = ((s8 *)gSceneState)[4];

  Io_Write16(0x3f42, &REG_BLDCNT);
  if (level == 0) {
    s32 alpha = 0x1000;

    REG_BLDALPHA = alpha;
  } else if (level == 1) {
    s32 alpha = 0xe00;

    REG_BLDALPHA = alpha;
  } else if (level == 2) {
    s32 alpha = 0xc00;

    REG_BLDALPHA = alpha;
  } else if (level == 3) {
    s32 alpha = 0xa00;

    REG_BLDALPHA = alpha;
  } else if (level == 4) {
    s32 alpha = 0x800;

    REG_BLDALPHA = alpha;
  } else {
    s32 alpha = 0x600;

    REG_BLDALPHA = alpha;
  }
}

// Remember a darkness boundary without interrupting Reveal.
static void AltmillerCave_SetFadeLevel(s32 level) {
  struct FieldWork *work;
  u8 *fade = &gSceneState[4];

  work = (struct FieldWork *)gEventWork;
  *fade = level;
  if (work->revealActive == 0) {
    AltmillerCave_ApplyFadeBlend();
  }
}

// Set darkness level 0 when the party crosses its boundary.
static void AltmillerCave_EnterDarknessLevel0(void) {
  AltmillerCave_SetFadeLevel(0);
}

// Set darkness level 1 when the party crosses its boundary.
static void AltmillerCave_EnterDarknessLevel1(void) {
  AltmillerCave_SetFadeLevel(1);
}

// Set darkness level 2 when the party crosses its boundary.
static void AltmillerCave_EnterDarknessLevel2(void) {
  AltmillerCave_SetFadeLevel(2);
}

// Set darkness level 3 when the party crosses its boundary.
static void AltmillerCave_EnterDarknessLevel3(void) {
  AltmillerCave_SetFadeLevel(3);
}

// Set darkness level 4 when the party crosses its boundary.
static void AltmillerCave_EnterDarknessLevel4(void) {
  AltmillerCave_SetFadeLevel(4);
}

// Set darkness level 5 when the party crosses its boundary.
static void AltmillerCave_EnterDarknessLevel5(void) {
  AltmillerCave_SetFadeLevel(5);
}

// Initialize the cave's darkness transition and blend masks.
static void AltmillerCave_InitializeDarkness(void) {
  u8 **workCells = (u8 **)&gEventWork;
  struct DisplayTransitionState *transition;

  {
    struct EventWork *work = (struct EventWork *)*workCells;
    work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transitionFrames = 24;
  }
  Engine_TaskWait(1);
  DisplayTransition_InitializeBattleEffectState(0x4d);
  // UB: the fourth following pointer cell lies past scalar gEventWork.
  // Its named load adds eight bytes in the native EN trial (0x98 to 0xa0).
#ifdef UBFIX
  transition = gDisplayTransition;
#else
  transition = (struct DisplayTransitionState *)workCells[4];
#endif
  {
    u16 *slot = &transition->value;
    s32 value = 5;
    *slot = value;
  }
  if (Engine_GameFlagIsSet(0x201) != 0) {
    {
      s16 *slot = &transition->mask;
      s32 value = 0x1d1d;
      *slot = value;
    }
    {
      s16 *slot = &transition->active;
      s32 value = 0x3f;
      *slot = value;
    }
    AltmillerCave_ApplyFadeBlend();
    return;
  }
  {
    s16 *slot = &transition->mask;
    s32 value = 0x3f3f;
    *slot = value;
  }
  {
    s16 *slot = &transition->active;
    s32 value = 31;
    *slot = value;
  }
  {
    s32 blendControl = 0x3f42;
    REG_BLDCNT = blendControl;
  }
  {
    s32 blendAlpha = 0xc04;
    REG_BLDALPHA = blendAlpha;
  }
}

// Restore chamber-specific blending and rock appearances.
static void AltmillerCave_ApplyRoomVisuals(void) {
  if (gGameState.scene == (s32)&SceneId_AltmillerCave1) {
    s32 alpha = 0x1000;

    REG_BLDALPHA = alpha;
  }
  if (gGameState.scene == (s32)&SceneId_AltmillerCave6) {
    Engine_ActorSetChildValue(16, 1);
    Engine_ActorSetChildValue(17, 4);
    Engine_ActorSetChildValue(18, 11);
    Engine_ActorSetChildValue(19, 2);
    Engine_ActorSetChildValue(20, 3);
  }
}

// Restore darkness and hide the rock symbols after Reveal.
static void AltmillerCave_RestoreDarknessAfterReveal(void) {
  struct BattleFxScene *work = (struct BattleFxScene *)gEffectWork;

  if (Engine_GameFlagIsSet(0x200) != 0) {
    AltmillerCave_InitializeDarkness();
    work->childOption = 1;
  }
  if (gGameState.scene == (s32)&SceneId_AltmillerCave6) {
    Engine_ActorSetChildValue(16, 6);
    Engine_ActorSetChildValue(17, 6);
    Engine_ActorSetChildValue(18, 6);
    Engine_ActorSetChildValue(19, 6);
    Engine_ActorSetChildValue(20, 6);
  }
}

// Transfer the Psynergy target's copied state to a new actor.
static void AltmillerCave_RespawnActorObject(void) {
  struct BattleFxScene *work;
  struct FieldActor *original;
  struct FieldActor *replacement;

  work = (struct BattleFxScene *)gEffectWork;
  original = work->mainObject;
  Engine_ActorFaceDirection(work->firstObjectId, FACING_SOUTH, 0);
  Animation_ApplyChildValues((struct DispatchObject *)original, 0);
  Engine_EventWait(20);
  replacement = Engine_ObjectCreate(0, original->x.fixed, original->y.fixed,
                                    original->z.fixed);
  if (replacement != 0) {
    Dma_Set(original, replacement,
            DMA_ENABLE32 | DMA_32BIT | sizeof(struct FieldActor) / 4, REG_DMA3);
    original->update = 0;
    work->mainObject = replacement;
    original->spriteStorageKind = 0;
  }
}

// Open the passage when the obstacle reaches its destination.
static void AltmillerCave_UpdateMovedObstacle(void) {
  struct FieldActor *actor = Object_GetById(12);

  if (actor->x.fixed >> 20 == 30 && actor->z.fixed >> 20 == 20) {
    actor->motionFlags = 2;
    actor->terrainHeight = 0;
    actor->priorityFlags = 2;
    Map_CopyCellAttributes(30, 20, 1, 1, 32, 20);
#if TBS_PATCH_1
    Engine_GameFlagSet(0x212);
#endif
  }
}

// Advance the staged obstacle and update its passage.
static void AltmillerCave_AdvanceObstacleAndCheckPassage(void) {
  StagedActor_AdvancePair();
  AltmillerCave_UpdateMovedObstacle();
}

// Record whether the Frost target's current position is blocked.
static void AltmillerCave_CheckFrostTargetCollision(void) {
  struct BattleFxScene *effect = (struct BattleFxScene *)gEffectWork;
  struct FieldActor *target = Object_GetById(11);
  struct FieldPosition position;

  position.x = target->x.fixed;
  position.y = target->y.fixed;
  position.z = target->z.fixed;

  if (FieldObject_CheckMovementCollision(target, &position) > 0) {
    effect->childMode = 1;
  }
}

// Make the frozen puddle traversable when its position is clear.
static void AltmillerCave_FreezePuddle(void) {
  struct BattleFxScene *effect = (struct BattleFxScene *)gEffectWork;
  struct FieldActor *puddle = Object_GetById(11);
  s32 blocked;

  blocked = effect->childMode;
  if (blocked == 0) {
    Map_CopyCellAttributes(76, 16, 1, 1, 73, 17);
    if (puddle != 0) {
      s32 motionFlags = 2;
      puddle->motionFlags = motionFlags;
      puddle->priorityFlags = blocked;
    }
    Engine_GameFlagSet(0x211);
  }
}

// Jump the selected actor across a clear adjacent landing.
static void AltmillerCave_HopSelectedActor(void) {
  struct FieldActor *actor = Object_GetById(gGameState.selectedActor);
  s32 motionFlags = actor->motionFlags;
  union {
    struct FieldPosition position;
    s32 coordinates[3];
    union FieldCoordinate components[3];
  } destination;
  union FieldCoordinate *components = destination.components;

  components[0].fixed = actor->x.fixed;
  components[1].fixed = actor->y.fixed;
  components[2].fixed = actor->z.fixed;
  {
    s32 angle = actor->facing & 0xf000;

    Vector_AddPolarOffset(PIXELS(32), angle, destination.coordinates);
  }
  if (FieldObject_CheckMovementCollision(actor, &destination.position) == 0) {
    Engine_EventBegin();
    Object_SetMode(actor, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(152);
    Object_SetMode(actor, 7);
    actor->speed = PIXELS(3);
    actor->acceleration = PIXELS(2);
    actor->velocityY = PIXELS(4);
    actor->motionFlags &= 126;
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ObjectMotionSetPositionAndCommit(0, components[0].part.pixel,
                                            components[2].part.pixel);
    Object_SetMode(actor, 6);
    Engine_ActorSetSpriteFlags(actor, 1);
    actor->motionFlags = motionFlags;
    Engine_EventEnd();
  }
}

// Commit the staged actor's move only when its position is clear.
static void AltmillerCave_MoveStagedActorToClearPosition(void) {
  struct StagedActorProbe probe;

  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
  }
  Engine_EventEnd();
}

// Pulse the selected rock symbol through its four scales.
static void AltmillerCave_UpdateScalePulse(union FieldObject *fieldObject) {
  struct FieldActor *symbol = (struct FieldActor *)fieldObject;
  s32 scale = gAltmillerCave_PulseScales[(symbol->timer >> 2) & 3];

  symbol->scaleX = scale;
  symbol->scaleY = scale;
  symbol->timer++;
  symbol->timer &= 15;
}

// Place one symbol in the vertical wheel centred above the rocks.
static void AltmillerCave_PlaceWheelActor(s32 actorId, s32 radius, s32 angle) {
  s32 centreX = PIXELS(504);
  s32 centreHeight = PIXELS(24);
  s32 depth = PIXELS(144);
  struct FieldActor *symbol = Object_GetById(actorId);
  s32 coordinates[3];
  s32 *position = coordinates;

  position[0] = centreX;
  position[2] = centreHeight;
  Vector_AddPolarOffset(radius, angle, position);
  symbol->x.fixed = position[0];
  symbol->y.fixed = position[2];
  symbol->z.fixed = depth;
}

// Space the five symbols evenly around the wheel.
static void AltmillerCave_PlaceWheelActors(s32 angle) {
  s32 i = 0;

  do {
    AltmillerCave_PlaceWheelActor(i + 11, PIXELS(24), angle);
    angle -= 0x3333; // One fifth of a full turn.
    i++;
  } while (i <= 4);
}

// Accelerate, coast and stop with the chosen symbol at the top.
static void AltmillerCave_SpinActorWheel(void) {
  struct ActorWheel *wheel = gAltmillerCaveWheel;
  s32 moving = 1;
  s32 phase = wheel->phase;

  if (phase == 0) {
    wheel->speed += 16;
    if (wheel->speed > 0xbff) {
      wheel->phase++;
      wheel->ticks = 0;
    }
  } else if (phase == 1) {
    if (wheel->ticks == 30) {
      wheel->phase++;
    }
  } else if (phase == 2) {
    wheel->speed -= 8;
    if (wheel->speed <= 0x2ff) {
      wheel->phase++;
    }
  } else if (phase == 3) {
    s32 chosenSymbol = ((s8 *)gSceneState)[2];
    s32 chosenAngle = Math_Divide(chosenSymbol * Q16_ONE, 5);
    if ((unsigned int)(((u32)(wheel->angle - chosenAngle) << 16) +
                       0xc2ff0000) <= 0x5fe0000) {
      wheel->angle = chosenAngle + 0x4000;
      wheel->phase = 0x63;
      wheel->speed = 0;
      Object_GetById(chosenSymbol + 11)->update =
          AltmillerCave_UpdateScalePulse;
    }
  } else if (phase == 0x63) {
    moving = 0;
  }
  if (moving != 0) {
    gAltmillerCaveWheel->angle += gAltmillerCaveWheel->speed;
    AltmillerCave_PlaceWheelActors(gAltmillerCaveWheel->angle);
    gAltmillerCaveWheel->travel += gAltmillerCaveWheel->speed;
    if (gAltmillerCaveWheel->travel > 0x3000) {
      gAltmillerCaveWheel->travel = 0;
      Engine_AudioPlayCue(0x87);
    }
  }
  gAltmillerCaveWheel->ticks++;
}

// Check the rock order, spin the symbols and open the final wall.
static void AltmillerCave_TurnRock(s32 action) {
  s32 correctRock = 0;
  s32 i;
  struct FieldActor *actor;
  s32 scale;
  s8 state;
  s32 chosenSymbol;
  u8 previousSymbol;
  s32 radius;
  u16 angle;
  u8 *puzzleState;

  Event_Begin();
  Battle_ResetEffectCounter();
  Event_SetMessage(MsgAltmillerRotatedRock);
  Event_ShowMessage(16, 0);
  Event_End();
  for (i = 0; i <= 4; i++) {
    actor = Actor_Get(i + 11);
    actor->update = NULL;
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  {
    struct ActorWheel *wheel;

    state = AltmillerCave_RockPuzzle->state;
    previousSymbol = AltmillerCave_RockPuzzle->chosen;
    wheel = gAltmillerCaveWheel;
    chosenSymbol = AltmillerCave_RockPuzzle->chosen;
    wheel->angle = (chosenSymbol * Q16_ONE) / 5 + 0x4000;
  }
  // The first two rocks establish the sequence before the symbol rounds.
  if (state == 0) {
    if (action == 16) {
      state = 1;
      Event_PlayCue(110);
    } else {
      Event_PlayCue(114);
    }
    gSceneState[0] = 0;
  } else if (state == 1) {
    if (action == 16) {
      Event_PlayCue(110);
    } else if (action == 20) {
      state = 2;
      Event_PlayCue(110);
      Task_Wait(30);
      angle = gAltmillerCaveWheel->angle;
      for (i = 0; i <= 4; i++) {
        AltmillerCave_PlaceWheelActor(i + 11, PIXELS(24), angle);
        Event_PlayCue(151);
        actor = Actor_Get(i + 11);
        actor->scaleX = 0;
        scale = 2 * Q16_ONE / 5;
        do {
          actor->scaleY = scale;
          actor->scaleX = scale;
          Task_Wait(1);
          scale += 3 * Q16_ONE / 64;
        } while (actor->scaleX < Q16_ONE);
        angle -= 0x3333;
      }
      Task_Wait(30);
      correctRock = 1;
    } else {
      Event_PlayCue(114);
      state = 0;
    }
  } else if (state == 2) {
    if (action != chosenSymbol + 16) {
      state = 0;
      Event_PlayCue(114);
      Task_Wait(30);
      for (i = 0; i <= 4; i++) {
        actor = Actor_Get(i + 11);
        Event_PlayCue(151);
        for (scale = actor->scaleX; actor->scaleX > 2 * Q16_ONE / 5;
             scale -= 3 * Q16_ONE / 64) {
          actor->scaleY = scale;
          actor->scaleX = scale;
          Task_Wait(1);
        }
        Actor_SetPosition(i + 11, 0, 0);
      }
    } else {
      Event_PlayCue(110);
      correctRock = 1;
      Task_Wait(30);
    }
  }
  puzzleState = &gSceneState[1];
  puzzleState[0] = state;
  // Choose a different symbol, then spin to the next round.
  if (correctRock) {
    u8 rounds = ++puzzleState[-1];

    puzzleState[1] =
        ((s8)(((u32)(Random_Next() << 2) >> 16) + previousSymbol + 1) + 5) % 5;
    gAltmillerCaveWheel->phase = 0;
    gAltmillerCaveWheel->ticks = 0;
    gAltmillerCaveWheel->speed = 0x200;
    gAltmillerCaveWheel->travel = 0x3000;
    Task_AddCallback(AltmillerCave_SpinActorWheel, TASK_PRIORITY_SCENE);
    if (rounds <= 2) {
      while (gAltmillerCaveWheel->phase != 99) {
        Task_Wait(1);
      }
      Task_Wait(10);
      Event_PlayCue(110);
    } else {
      // Collapse the symbols and scatter the wall after the third success.
      puzzleState[0] = 99;
      while (gAltmillerCaveWheel->phase != 2) {
        Task_Wait(1);
      }
      gAltmillerCaveWheel->phase = 2;
      gAltmillerCaveWheel->ticks = 0;
      Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
      Engine_EventWait(20);
      Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      gAltmillerCaveWheel->phase = 99;
      Event_PlayCue(190);
      radius = PIXELS(24);
      angle = gAltmillerCaveWheel->angle;
      do {
        for (i = 0; i <= 4; i++) {
          actor = Actor_Get(i + 11);
          actor->scaleX -= 16;
          actor->scaleY -= 16;
          AltmillerCave_PlaceWheelActor(i + 11, radius, angle);
          angle -= 0x3333;
        }
        radius -= PIXELS(1) / 5;
        angle += 0xc00;
        Task_Wait(1);
      } while (radius > 0);
      for (i = 0; i <= 4; i++) {
        Actor_SetPosition(i + 11, 0, 0);
      }
      AltmillerCave_ReleaseWallBurst();
      Event_PlayCue(80);
    }
    Engine_TaskRemoveCallback(AltmillerCave_SpinActorWheel);
  }
}

// Divide the nonnegative random value for wall-fragment scaling.
s32 Math_DivideUnsigned(s32 value, s32 divisor);

// Turn all four party members toward Babi without waiting.
static __inline__ void AltmillerCave_FacePartyNorth(void) {
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
}

// Restore Babi with his draught and play his invitation to Colosso.
static void AltmillerCave_RestoreBabiAndInviteParty(void) {
  struct DisplayTransitionState *display;
  struct FieldActor *actor;
  struct FieldActor *leader;
  s32 i;
  s32 eva;
  s32 evb;

  // Give Babi the draught and bring the party beside him.
  GameFlag_Set(0x962);
  Engine_PartyRemoveItem(237);
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgAltmillerCameBackPleaseDraughtQuickly);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 160);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_EventWait(50);
  Engine_EventWait(10);
  Camera_MoveTo(PIXELS(264), -1, PIXELS(200), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 264, 208);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  FieldMotion_LaunchFromFocusedObject(GARET, -16, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(MIA, 0, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(IVAN, 16, 16, FACING_NORTH);
  Engine_ActorWaitForMove(GARET);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 8, 40);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(20);
  actor = Actor_Get(ACTOR_INVISIBLE_BABI);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(60);
  Event_PlayCue(17);
  actor->sprite->rotation = 0;
  Engine_ActorJump(ACTOR_INVISIBLE_BABI, 10, 70);
  // Flicker between sprite priorities before replacing invisible Babi.
  for (i = 0; i < 30; i++) {
    actor->sprite->image.priority = 0;
    Engine_EventWait(2);
    actor->sprite->image.priority = 2;
    Engine_EventWait(2);
  }
  Engine_EventWait(40);
  {
    s32 x;
    s32 z;

    x = Actor_Get(ACTOR_INVISIBLE_BABI)->x.part.pixel;
    z = Actor_Get(ACTOR_INVISIBLE_BABI)->z.part.pixel;
    Actor_SetPosition(ACTOR_INVISIBLE_BABI, 0, 0);
    Actor_SetPosition(ACTOR_BABI, PIXELS(x), PIXELS(z));
  }
  actor->sprite->flags = 0;
  Event_PlayCue(30);
  Actor_SetSpeed(ACTOR_BABI, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkByAndWait(ACTOR_BABI, 32, 32);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(MIA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(IVAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, 16);
  Actor_WalkBy(GARET, 0, 16);
  Actor_WalkBy(MIA, 0, 16);
  Actor_WalkByAndWait(IVAN, 0, 16);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(MIA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(IVAN)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(MIA, ANIM_STAND);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Engine_TaskWait(1);
  AltmillerCave_FacePartyNorth();
  Engine_EventWait(30);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 5, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 40);
  Engine_ActorJump(GARET, 4, 13);
  Engine_ActorJump(GARET, 4, 30);
  Event_ShowMessage(GARET, 0);
  Actor_FaceActor(IVAN, GARET, 30);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 40);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 30);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(GARET, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_OpenMessage(ACTOR_BABI, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventSetMessage(MsgAltmillerWouldHaveRevealedMyselfSooner);
    Event_ShowMessage(ACTOR_BABI, 0);
  } else {
    Engine_EventSetMessage(MsgAltmillerMustTraveledIndeedIfDidnt);
    Event_ShowMessage(ACTOR_BABI, 0);
  }
  // Ask how Babi became invisible and hear about his Alchemy.
  Engine_EventSetMessage(MsgAltmillerHowDidBecomeParalyzedWhile);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  AltmillerCave_FacePartyNorth();
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(20);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(IVAN, 0, -48);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Engine_EventWait(40);
  // Read Babi\'s thoughts and discuss the limits of his Alchemy.
  Engine_PsynergyBegin(ABILITY_MIND_READ, 1);
  Engine_PsynergySetTarget(2, 9);
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_EventWait(150);
  Engine_PsynergyPlayEffect(2);
  Engine_PsynergyLowerHands();
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(40);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 40);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_BABI, 4, 13);
  Engine_ActorJump(ACTOR_BABI, 4, 30);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 2, 50);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(50);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Engine_EventWait(20);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(IVAN, 0, 48);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  AltmillerCave_FacePartyNorth();
  Engine_EventWait(50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 80);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_NORTH, 0);
  Engine_EventWait(50);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 40);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgAltmillerThatsKradenSaidAlchemyCould);
    Event_ShowMessage(GARET, 0);
  } else {
    Engine_EventSetMessage(MsgAltmillerForgotKradenSaidAlchemyCould);
    Event_ShowMessage(GARET, 0);
  }
  Engine_EventSetMessage(MsgAltmillerWhatWasThat);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Event_OpenMessage(ACTOR_BABI, 0);
  Engine_EventSetMessage(MsgAltmillerBabi);
  Engine_EventChooseYesNo(0, 0);
  // Fade out the cave darkness as Babi\'s guards arrive.
  display = gDisplayTransition;
  {
    u16 *slot = &display->value;
    s32 value = 32;

    *slot = value;
  }
  for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
    {
      s32 control = 0x3f42;

      REG_BLDCNT = control;
    }
    REG_BLDALPHA = (eva << 8) | evb;
    Engine_TaskWait(7);
  }
  {
    u16 *slot = (u16 *)&display->active;
    s32 value = 0x3f3f;

    *slot = value;
  }
  Actor_SetPosition(ACTOR_FIRST_GUARD, PIXELS(280), PIXELS(304));
  Actor_SetPosition(ACTOR_SECOND_GUARD, PIXELS(296), PIXELS(304));
  Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Engine_EventWait(10);
  Engine_CameraFollowActor(10, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(70);
  Actor_ShowEmote(ACTOR_SECOND_GUARD, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_ActorFollow(ACTOR_PARTY_LEADER, 10);
  Engine_ActorFollow(GARET, 10);
  Engine_ActorFollow(MIA, 10);
  Engine_ActorFollow(IVAN, 10);
  // Stage the guards\' reunion with Babi and the invitation to Colosso.
  Actor_SetSpeed(ACTOR_FIRST_GUARD, PIXELS(7) / 5, PIXELS(7) / 10);
  Actor_SetSpeed(ACTOR_SECOND_GUARD, PIXELS(7) / 5, PIXELS(7) / 10);
  // Lead the guards through the chamber to Babi.
  Actor_SetActionScript(ACTOR_FIRST_GUARD, gAltmillerActionA);
  Engine_EventWait(3);
  Actor_WalkByAndWait(ACTOR_SECOND_GUARD, -16, 0);
  Actor_SetActionScript(ACTOR_SECOND_GUARD, gAltmillerActionA);
  Object_RefreshSelectorById(10);
  Actor_WalkByAndWait(ACTOR_FIRST_GUARD, 0, -16);
  Object_RefreshSelectorById(11);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_EAST, 0);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(MIA);
  Engine_ActorStop(IVAN);
  AltmillerCave_FacePartyNorth();
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Camera_MoveTo(PIXELS(248), -1, PIXELS(216), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_GUARD, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTHEAST, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(50);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_SECOND_GUARD, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Engine_EventWait(10);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_FIRST_GUARD, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Actor_WalkByAndWait(ACTOR_SECOND_GUARD, 16, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_GUARD, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Actor_WalkByAndWait(ACTOR_FIRST_GUARD, 16, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_SECOND_GUARD, 4, 13);
  Engine_ActorJump(ACTOR_SECOND_GUARD, 4, 30);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GUARD, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTHEAST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_SOUTHEAST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTHEAST, 0);
  Engine_EventWait(70);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_EAST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_EAST, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_SECOND_GUARD, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 2, 40);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(ACTOR_FIRST_GUARD, 11, 70);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTHEAST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GUARD, ANIM_NOD);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_GUARD, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  AltmillerCave_FacePartyNorth();
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_SECOND_GUARD, EMOTE_IN_FRONT | 8, 40);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_EAST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_EAST, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_GUARD, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_FIRST_GUARD, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(40);
  Engine_ActorFollow(ACTOR_PARTY_LEADER, 9);
  Engine_ActorFollow(GARET, 9);
  Engine_ActorFollow(MIA, 9);
  Engine_ActorFollow(IVAN, 9);
  Actor_SetSpeed(ACTOR_BABI, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_BABI, -16, 0);
  Actor_WalkBy(ACTOR_FIRST_GUARD, 0, 48);
  Actor_WalkBy(ACTOR_SECOND_GUARD, 0, 48);
  Actor_WalkByAndWait(ACTOR_BABI, -16, 0);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 32);
  Engine_ActorSetAnimation(ACTOR_FIRST_GUARD, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_SECOND_GUARD, ANIM_STAND);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(MIA);
  Engine_ActorStop(IVAN);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 30);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Actor_FaceActor(GARET, 9, 30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Engine_EventWait(70);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(30);
  // Send Babi and his guards back toward the cave entrance.
  Actor_SetActionScript(ACTOR_SECOND_GUARD, gAltmillerActionB);
  Actor_SetActionScript(ACTOR_FIRST_GUARD, gAltmillerActionC);
  Actor_SetActionScript(ACTOR_BABI, gAltmillerActionD);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  Object_RefreshSelectorById(9);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Camera_MoveTo(PIXELS(248), -1, PIXELS(248), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_NORTH, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_GUARD, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_WEST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_WEST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 7, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_FIRST_GUARD, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(ACTOR_SECOND_GUARD, EMOTE_IN_FRONT | 2, 40);
  Engine_EventWait(30);
  Actor_FaceActor(ACTOR_FIRST_GUARD, 11, 60);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_WEST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GUARD, ANIM_NOD);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_BABI, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_FIRST_GUARD, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_EAST, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_EAST, 0);
  Engine_EventWait(40);
  Actor_WalkBy(ACTOR_BABI, 32, 0);
  Actor_WalkBy(ACTOR_SECOND_GUARD, 0, 64);
  Actor_WalkByAndWait(ACTOR_FIRST_GUARD, 16, 0);
  Actor_WalkBy(ACTOR_FIRST_GUARD, 0, 64);
  Engine_ActorWaitForMove(ACTOR_BABI);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 64);
  Actor_SetPosition(ACTOR_BABI, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_GUARD, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_GUARD, 0, 0);
  Engine_EventWait(20);
  for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
    {
      s32 control = 0x3f42;

      REG_BLDCNT = control;
    }
    REG_BLDALPHA = (evb << 8) | eva;
    Engine_TaskWait(7);
  }
  {
    u16 *slot = &display->value;
    s32 value = 5;

    *slot = value;
  }
  {
    u16 *slot = (u16 *)&display->active;
    s32 value = 31;

    *slot = value;
  }
  Engine_TaskWait(1);
  Io_Write16(0x3f42, &REG_BLDCNT);
  Io_Write16(0xc04, &REG_BLDALPHA);
  Engine_CameraMoveToActor(0, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Event_OpenMessage(GARET, 0);
  Actor_FaceActor(MIA, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgAltmillerSoWellDoIt);
    Event_ShowMessage(GARET, 0);
  } else {
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgAltmillerDontWantEitherDo);
    Event_ShowMessage(GARET, 0);
  }
  // Let Isaac answer the invitation and reunite the party.
  Engine_EventSetMessage(MsgAltmillerEitherWayWereStuckHere);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(GARET, 0);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventSetMessage(MsgAltmillerGreatItsDecidedJustHave);
    Event_ShowMessage(GARET, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(MIA, IVAN, 50);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Actor_FaceDirection(IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Event_ShowMessage(IVAN, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_ActorSetAnimation(MIA, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(30);
  } else {
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
    Engine_EventWait(30);
    Engine_EventSetMessage(MsgAltmillerDontLetMeDownLike);
    Event_ShowMessage(GARET, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(MIA, IVAN, 50);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Actor_FaceDirection(IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Event_ShowMessage(IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    Engine_EventWait(30);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 70);
  }
  Event_PlayCue(17);
  Actor_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(IVAN, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
}

// Introduce the invisible Babi and explain how to retrieve his draught.
static void AltmillerCave_MeetInvisibleBabi(void) {
  struct FieldActor *leader;
  s32 line;

  // Gather the party around the invisible voice.
  GameFlag_Set(0x960);
  Engine_AudioPlayCue(24);
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Engine_EventSetMessage(MsgAltmillerSee);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Camera_MoveTo(PIXELS(248), -1, PIXELS(184), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 192);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  FieldMotion_LaunchFromFocusedObject(GARET, -16, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(MIA, 0, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(IVAN, 16, 16, FACING_NORTH);
  Engine_ActorWaitForMove(1);
  Engine_EventWait(20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(40);
  Event_ShowMessage(IVAN, 0);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(3, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT, 40);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, IVAN, 50);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(1, 0);
  Engine_EventWait(10);
  Actor_SetAttachedEffect(ACTOR_INVISIBLE_BABI, 0x102);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(20);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(2, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT, 40);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, 4);
  Engine_EventWait(20);
  Engine_EventShowMessage(3, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 5, 40);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 30);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(GARET, 0);
  Engine_ActorFaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(1, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(MIA, 4);
  Engine_EventWait(20);
  Engine_EventShowMessage(3, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 6, 40);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 0);
  Engine_ActorShowEmote(IVAN, 0x101, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(2, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  Engine_EventShowMessage(1, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(3, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 2, 40);
  Event_OpenMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgAltmillerKiddingHaventActually);
    Engine_EventWait(20);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 40);
    Actor_SetSpeed(GARET, PIXELS(2), PIXELS(1));
    Actor_WalkByAndWait(GARET, 0, -16);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 30);
    Engine_EventShowMessage(1, 0);
  } else {
    Engine_EventSetMessage(MsgAltmillerDontTrustAnyone);
    Engine_EventWait(10);
    Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkByAndWait(GARET, 0, -16);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 30);
    Engine_ActorSetAnimationAndWait(GARET, 3);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
  }

  // Question Babi\'s condition before agreeing to find his draught.
  line = MsgAltmillerSaidCouldntMove;
  Engine_EventSetMessage(line);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 3, 40);
  Actor_SetSpeed(IVAN, PIXELS(2), PIXELS(1));
  Actor_WalkByAndWait(IVAN, 0, -16);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(IVAN, ACTOR_PARTY_LEADER, 30);
  Event_ShowMessage(IVAN, 0);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, MIA, 0);
  Actor_FaceActor(GARET, MIA, 0);
  Engine_ActorFaceActor(IVAN, 3, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(3, 0);
  Engine_EventWait(20);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorWalkByAndWait(GARET, 0, 16);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(GARET, 4);
  Engine_EventWait(30);
  Engine_ActorFaceActor(GARET, 0, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(1, 0);
  Engine_EventWait(30);
  Engine_ActorFaceEachOther(MIA, IVAN, 30);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(30);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorWalkByAndWait(IVAN, 0, 16);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  // Explain the hidden draught and the order of the five rocks.
  line += 7;
  Engine_EventSetMessage(line);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT, 40);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(30);
  Engine_EventShowMessage(2, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 8, 40);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Engine_EventShowMessage(3, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Engine_ActorFaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(1, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(2, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 8, 50);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 40);
  Engine_EventShowMessage(1, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, 4);
  Engine_EventWait(20);
  Engine_EventShowMessage(3, 0);
  Engine_EventWait(10);
  Actor_FaceActor(ACTOR_PARTY_LEADER, MIA, 40);
  Engine_ActorSetAnimationAndWait(0, 3);
  Engine_EventWait(30);
  Actor_FaceActor(GARET, MIA, 0);
  Engine_ActorFaceActor(IVAN, 3, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(30);
  Engine_AudioPlayCue(17);
  Actor_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorSetAnimation(GARET, 2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(1);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(IVAN, 2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(IVAN, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(2);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorSetAnimation(MIA, 2);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(3);
  Actor_SetPosition(MIA, 0, 0);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
}

// Let Babi repeat the order of the rocks guarding his draught.
static void AltmillerCave_RepeatRockOrder(void) {
  s32 question;

  Engine_EventBegin();
  question = MsgAltmillerForgetOrderRock;
  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_EventSetMessage(question + 1);
    Engine_EventShowMessage(8, 0);
  } else {
    Engine_EventWait(20);
    Engine_EventSetMessage(question + 2);
    Engine_EventShowMessage(8, 0);
  }
  Engine_EventEnd();
}

// Ask the departing party to help Babi before leaving.
static void AltmillerCave_StopPartyLeavingBabi(void) {
  if (Engine_GameFlagIsSet(0x960) == 0)
    return;
  if (Engine_GameFlagIsSet(0x962) != 0)
    return;

  Engine_GameFlagSet(0x961);
  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltmillerWait);
  Engine_EventShowMessage(8, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(30);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 8, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(8, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  Engine_EventEnd();
}

// Choose the event table for the current chamber.
const struct SceneEvent *AltmillerCave_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_AltmillerCave2) {
    return gAltmillerCaveEvents2;
  }
  if (scene == (s32)&SceneId_AltmillerCave3) {
    return gAltmillerCaveEvents3;
  }
  if (scene == (s32)&SceneId_AltmillerCave4) {
    return gAltmillerCaveEvents4;
  }
  if (scene == (s32)&SceneId_AltmillerCave5) {
    return gAltmillerCaveEvents5;
  }
  if (scene == (s32)&SceneId_AltmillerCave6) {
    return gAltmillerCaveEvents6;
  }
  return gAltmillerCaveEventsOther;
}

// Keep the staged obstacle at the party leader's sprite priority.
static void AltmillerCave_MatchLeaderPriority(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  if (actor != NULL) {
    actor->priorityFlags = 0;
    actor->sprite->image.priority = Object_GetById(0)->sprite->image.priority;
  }
}

// Restore the three staged obstacle footprints used in either puzzle room.
static __inline__ void AltmillerCave_RedrawObstacleFootprints(void) {
  FieldScene_RedrawActorFootprint(8);
  FieldScene_RedrawActorFootprint(9);
  FieldScene_RedrawActorFootprint(10);
}

// Restore chamber entrances, darkness and persistent puzzle state.
s32 AltmillerCave_Initialize(void) {
  s8 *state;
  struct FieldActor *actor;
  s32 i;

  if (gGameState.entrance == 0) {
    if (gGameState.scene == (s32)&SceneId_AltmillerCave2) {
      gGameState.entrance = 10;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCave3) {
      gGameState.entrance = 20;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCave4) {
      gGameState.entrance = 30;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCave5) {
      gGameState.entrance = 40;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCave6) {
      gGameState.entrance = 50;
    }
  }
  Engine_GameFlagSet(0x200);
  GameFlag_Clear(0x201);
  if (gGameState.scene == (s32)&SceneId_AltmillerCave1) {
    if (gGameState.entrance == 1) {
      if (!Engine_GameFlagIsSet(0x109)) {
        *(u8 *)((u32)gSceneState + 4) = 0;
      }
      Engine_GameFlagSet(0x201);
    }
    if (gGameState.entrance == 2) {
      if (!Engine_GameFlagIsSet(0x109)) {
        *(u8 *)((u32)gSceneState + 4) = 5;
      }
      Engine_GameFlagSet(0x201);
    }
  }
  if (gGameState.scene == (s32)&SceneId_AltmillerCave2) {
    if (Engine_GameFlagIsSet(0x962)) {
      Engine_ActorSetPosition(8, 0, 0);
    } else {
      actor = Object_GetById(8);
      actor->sprite->image.priority = 1;
      actor->sprite->flags = 2;
      actor->sprite->rotation = 0x4000;
    }
  }
  // Restore moved obstacles and the frozen puddle in the fourth chamber.
  if (gGameState.scene == (s32)&SceneId_AltmillerCave4) {
    Engine_GameFlagClear(0x200);
    AltmillerCave_RedrawObstacleFootprints();
    if (Engine_GameFlagIsSet(0x211)) {
      Engine_ActorSetAnimation(11, 5);
      Map_CopyCellAttributes(76, 16, 1, 1, 73, 17);
    } else {
      Object_GetById(11)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    }
    Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
#if TBS_PATCH_1
    if (Engine_GameFlagIsSet(0x212)) {
      Map_CopyCellAttributes(30, 20, 1, 1, 32, 20);
    }
#endif
  }
  // Rebuild the final rock puzzle from its saved phase and chosen symbol.
  if (gGameState.scene == (s32)&SceneId_AltmillerCave6) {
    Engine_GameFlagClear(0x200);
    AltmillerCave_RedrawObstacleFootprints();
    /* FAKEMATCH: the three callbacks are stored as s32, so the entrance read
     * after them may alias them and stays below the last store. 2026-10-13:
     * stored through the update field itself (with a cast of the function) or
     * as void *, 2 of 403 differ: the last store moves below the entrance's
     * ldrsh. */
#ifdef UBFIX
    Object_GetById(8)->update = AltmillerCave_MatchLeaderPriority;
    Object_GetById(9)->update = AltmillerCave_MatchLeaderPriority;
    Object_GetById(10)->update = AltmillerCave_MatchLeaderPriority;
#else
    *(s32 *)&Object_GetById(8)->update = (s32)AltmillerCave_MatchLeaderPriority;
    *(s32 *)&Object_GetById(9)->update = (s32)AltmillerCave_MatchLeaderPriority;
    *(s32 *)&Object_GetById(10)->update =
        (s32)AltmillerCave_MatchLeaderPriority;
#endif
    if (gGameState.entrance == 52) {
      Dma_Fill32(0, gAltmillerCaveWheel, 12);
      if (!Engine_GameFlagIsSet(0x109)) {
        u8 *puzzleState = gSceneState;

        puzzleState[0] = 0;
        puzzleState[1] = 0;
        puzzleState[2] = 4;
      }
    }
    state = (s8 *)&gSceneState[1];
    if (state[0] == 99) {
      Map_CopyCellsLayered(41, 55, 3, 2, 30, 55);
      Map_CopyCellAttributes(42, 8, 1, 1, 31, 8);
    }
    if (state[0] == 2) {
      AltmillerCave_PlaceWheelActors((state[1] * Q16_ONE) / 5 + 0x4000);
    }
    for (i = 0; i < 5; i++) {
      actor = Object_GetById(i + 11);
      actor->motionFlags = 0;
      actor->collisionFlags = 0;
      actor->scaleX = Q16_ONE;
      actor->scaleY = Q16_ONE;
      Engine_ActorSetSpriteFlags(Object_GetById(i + 11), 0);
      Engine_ActorSetAnimation(i + 11, i + 1);
    }
    Engine_ActorSetChildValue(11, 1);
    Engine_ActorSetChildValue(12, 4);
    Engine_ActorSetChildValue(13, 11);
    Engine_ActorSetChildValue(14, 2);
    Engine_ActorSetChildValue(15, 3);
    Engine_ActorSetChildValue(16, 6);
    Engine_ActorSetChildValue(17, 6);
    Engine_ActorSetChildValue(18, 6);
    Engine_ActorSetChildValue(19, 6);
    Engine_ActorSetChildValue(20, 6);
    Object_GetById(16)->sprite->image.priority = 3;
    Object_GetById(20)->sprite->image.priority = 3;
    Object_GetById(16)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Object_GetById(20)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Engine_ActorSetSpriteFlags(Object_GetById(16), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(20), 0);
  }
  if (Engine_GameFlagIsSet(0x200)) {
    AltmillerCave_InitializeDarkness();
  } else {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    gEventWork->transitionFrames = 24;
  }
  return 0;
}

// Send each wall fragment toward successive random targets.
static void AltmillerCave_UpdateWallFragment(struct EffectSlot *fragment) {
  s32 position[3];
  s32 x;
  s32 z;
  u8 *phase;
  s32 currentPhase;

  phase = (u8 *)&fragment->state;
  currentPhase = *(s8 *)phase;
  if (currentPhase == 0) {
    z = fragment->originZ;
    x = fragment->originX;
    fragment->z = z;
    position[2] = z;
    fragment->x = x;
    position[0] = x;
    Vector_AddPolarOffset(PIXELS(120), Engine_RandomNext(), position);
    fragment->targetX = position[0];
    fragment->targetZ = position[2];
    fragment->acceleration = PIXELS(5);
    fragment->maxSpeed = PIXELS(5);
    fragment->limitTurnStep = currentPhase;
    (*phase)++;
    if ((gFrameTick & 3) == 0)
      Engine_AudioPlayCue(0x86);
  } else if (currentPhase == 1) {
    if (EffectSlot_HasPendingTarget(fragment) == 0) {
      s32 nextPhase = *phase;
      nextPhase--;
      *phase = nextPhase;
    }
  } else if (currentPhase == 2) {
    if (EffectSlot_HasPendingTarget(fragment) == 0)
      BattleFx_ClearOwnedSlot(fragment);
  }
}

// Scatter the wall fragments, reveal the passage and release the effect.
static void AltmillerCave_ReleaseWallBurst(void) {
  struct BattleFxScene *work;
  struct EffectVector position;
  struct EffectSlot *fragment;
  s32 fragmentIndex;

  BattleFx_InitializeSlots();
  work = (struct BattleFxScene *)gEffectWork;
  Shop_RestoreSceneTiles(0x202108);
  position.x = PIXELS(504);
  position.y = PIXELS(24);
  position.z = PIXELS(144);
  Camera_WorldToScreen(&position);
  fragment = work->slots;
  for (fragmentIndex = 23; fragmentIndex >= 0; fragmentIndex--) {
    s32 scale;

    EffectSlot_Initialize(fragment, 284, position.x, position.z);
    EffectSlot_SetCallback(fragment, AltmillerCave_UpdateWallFragment);
    EffectSlot_SetObjectMode(fragment, 7);
    ObjectGroup_SetChildValueUnlessFifteen(
        fragment->object, (u32)(Engine_RandomNext() * 7) >> 16);
    scale = Math_DivideUnsigned(Engine_RandomNext(), 3) + 3 * Q16_ONE / 2;
    fragment->scaleY = scale;
    fragment->scaleX = scale;
    Engine_TaskWait(1);
    fragment++;
  }
  Engine_TaskWait(80);
  Map_CopyCellsLayered(41, 55, 3, 2, 30, 55);
  Map_CopyCellAttributes(42, 8, 1, 1, 31, 8);
  Engine_TaskWait(50);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_TaskWait(30);
  {
    s32 mode = 2;
    // Walk the state bytes within the complete slot array representation.
    u8 *stateCursor = (u8 *)&work->slots + offsetof(struct EffectSlot, state);

    for (fragmentIndex = 23; fragmentIndex >= 0; fragmentIndex--) {
      if (((s8 *)stateCursor)[offsetof(struct EffectSlot, active) -
                              offsetof(struct EffectSlot, state)] != 0) {
        *stateCursor = mode;
      }
      stateCursor += sizeof(struct EffectSlot);
    }
  }
  Engine_MapWaitWorkValuesBelow256();
  Shop_InitEffect();
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

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

u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8,  32, 8,  -8,  -32, 8,  32, -32, -16, 32, 0,
    -8,  -32, 8,  32, -32, -8,  32, 8,  -8,  -32, 8,  32,
};

struct ActorWheel *gAltmillerCaveWheel = &gAltmillerActorWheel;

struct SceneEntrance gAltmillerCave_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 456, 0, 504, FACING_NORTH, 0, 0, 0, 544, 528, 0},
    {2, CONDITION_ALWAYS, 280, 0, 360, FACING_SOUTH, 0, 0, 0, 544, 528, 0},
    {3, CONDITION_ALWAYS, 120, 0, 696, FACING_NORTH, 0, 0, 552, 240, 720, 0},
    {4, CONDITION_ALWAYS, 120, 0, 600, FACING_SOUTH, 0, 0, 552, 240, 720, 0},
    {10, CONDITION_ALWAYS, 392, 0, 504, FACING_SOUTH, 0, 48, 16, 496, 672, 0},
    {11, CONDITION_ALWAYS, 136, 0, 104, FACING_SOUTH, 0, 48, 16, 496, 672, 0},
    {20, CONDITION_ALWAYS, 232, 0, 216, FACING_SOUTH, 0, 64, 48, 592, 576, 0},
    {21, CONDITION_ALWAYS, 328, 0, 264, FACING_SOUTH, 0, 64, 48, 592, 576, 0},
    {30, CONDITION_ALWAYS, 200, 0, 424, FACING_SOUTH, 0, 48, 80, 576, 480, 0},
    {31, CONDITION_ALWAYS, 520, 0, 328, FACING_SOUTH, 0, 48, 80, 576, 480, 0},
    {40, CONDITION_ALWAYS, 680, 0, 152, FACING_SOUTH, 0, 0, 0, 768, 512, 0},
    {41, CONDITION_ALWAYS, 40, 0, 248, FACING_SOUTH, 0, 0, 0, 768, 512, 0},
    {50, CONDITION_ALWAYS, 296, 0, 392, FACING_SOUTH, 0, 160, 304, 576, 704, 0},
    {51, CONDITION_ALWAYS, 424, 0, 392, FACING_SOUTH, 0, 160, 304, 576, 704, 0},
    {52, CONDITION_ALWAYS, 504, 0, 232, FACING_NORTH, 0, 384, 64, 624, 240, 0},
    {53, CONDITION_ALWAYS, 504, 0, 152, FACING_SOUTH, 0, 384, 64, 624, 240, 0},
    {54, CONDITION_ALWAYS, 184, 0, 216, FACING_NORTH, 0, 64, 64, 304, 240, 0},
    {SCENE_TABLE_END},
};

u32 gAltmillerCave_Exits[] = {
    SCENE_EXITS(146),        SCENE_EXIT(1, 2, 24),    SCENE_EXIT(2, 146, 3),
    SCENE_EXIT(3, 146, 2),   SCENE_EXIT(4, 147, 10),  SCENE_EXIT(0, 147, 0),
    SCENE_EXIT(10, 146, 4),  SCENE_EXIT(11, 148, 20), SCENE_EXIT(0, 148, 0),
    SCENE_EXIT(20, 147, 11), SCENE_EXIT(21, 149, 30), SCENE_EXIT(0, 149, 0),
    SCENE_EXIT(30, 148, 21), SCENE_EXIT(31, 150, 40), SCENE_EXIT(0, 150, 0),
    SCENE_EXIT(40, 149, 31), SCENE_EXIT(41, 151, 50), SCENE_EXIT(0, 151, 0),
    SCENE_EXIT(50, 150, 41), SCENE_EXIT(51, 151, 52), SCENE_EXIT(52, 151, 51),
    SCENE_EXIT(53, 151, 54), SCENE_EXIT(54, 151, 53), SCENE_EXITS_END,
};

struct ScenePlacement gAltmillerCavePlacementsOther[] = {
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltmillerCavePlacements2[] = {
    {GARET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST,
     TALK_FACE_PARTY, 0},
    {IVAN, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY,
     0},
    {MIA, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY,
     0},
    {0x41, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(232), 0, PIXELS(160),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0x34, CONDITION_ALWAYS, ACTOR_STAND, -PIXELS(64), 0, -PIXELS(64),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0xa3, CONDITION_ALWAYS, ACTOR_STAND, -PIXELS(64), 0, -PIXELS(64),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {0xa3, CONDITION_ALWAYS, ACTOR_STAND, -PIXELS(64), 0, -PIXELS(64),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltmillerCavePlacements4[] = {
    {0x129, CONDITION_ALWAYS, 7, PIXELS(328), 0, PIXELS(256), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x12a, CONDITION_ALWAYS, 7, PIXELS(352), 0, PIXELS(296), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x12a, CONDITION_ALWAYS, 7, PIXELS(368), 0, PIXELS(200), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(376), 0, PIXELS(280),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0xd3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(472), 0, PIXELS(328),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0x5d, 0x71, ACTOR_STAND, PIXELS(264), 0, PIXELS(200), FACING_SOUTHWEST,
     TALK_FACE_PARTY, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltmillerCavePlacements6[] = {
    {0x129, CONDITION_ALWAYS, 7, PIXELS(392), 0, PIXELS(416), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x12a, CONDITION_ALWAYS, 7, PIXELS(416), 0, PIXELS(456), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x129, CONDITION_ALWAYS, 7, PIXELS(456), 0, PIXELS(480), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(488), PIXELS(8), PIXELS(1752),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(472), PIXELS(40),
     PIXELS(1752), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(504), PIXELS(56),
     PIXELS(1752), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(536), PIXELS(40),
     PIXELS(1752), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(520), PIXELS(8), PIXELS(1752),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(440), 0, PIXELS(190),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(472), 0, PIXELS(174),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(504), 0, PIXELS(174),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(536), 0, PIXELS(174),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(568), 0, PIXELS(190),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SCENE_TABLE_END},
};

u32 gAltmillerCave_PulseScales[] = {
    Q16_ONE,
    11 * Q16_ONE / 10,
    6 * Q16_ONE / 5,
    11 * Q16_ONE / 10,
};

s32 gAltmillerActionA[] = {
    SC_ARG(2), SC_MOVE_BY(0, 0, -PIXELS(32)),
    SC_SYNC,   SC_MOVE_BY(-PIXELS(64), 0, 0),
    SC_SYNC,   SC_MOVE_BY(0, 0, -PIXELS(64)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gAltmillerActionB[] = {
    SC_ARG(2), SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,   SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,   SC_MOVE_BY(PIXELS(48), 0, 0),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gAltmillerActionC[] = {
    SC_ARG(2), SC_MOVE_BY(0, 0, PIXELS(16)),
    SC_SYNC,   SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,   SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,   SC_MOVE_BY(PIXELS(32), 0, 0),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gAltmillerActionD[] = {
    SC_ARG(2), SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,   SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,   SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,   SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

// clang-format off
struct SceneEvent gAltmillerCaveEventsOther[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {0x31, 4, CONDITION_ALWAYS, 4},
    {EVENT_TOUCH, 60, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_EnterDarknessLevel0)},
    {EVENT_TOUCH, 61, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_EnterDarknessLevel1)},
    {EVENT_TOUCH, 62, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_EnterDarknessLevel2)},
    {EVENT_TOUCH, 63, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_EnterDarknessLevel3)},
    {EVENT_TOUCH, 64, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_EnterDarknessLevel4)},
    {EVENT_TOUCH, 65, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_EnterDarknessLevel5)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_ApplyRoomVisuals)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_RestoreDarknessAfterReveal)},
    {PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_RespawnActorObject)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCaveEvents2[] = {
    {0x21, 10, CONDITION_ALWAYS, 10},
    {0x31, 11, CONDITION_ALWAYS, 11},
    {EVENT_TALK, 8, 0x960, EVENT_SCRIPT(AltmillerCave_MeetInvisibleBabi)},
    {EVENT_TALK, 8, 0xf30, EVENT_SCRIPT(AltmillerCave_RepeatRockOrder)},
    {EVENT_TALK, 8, 0x962, EVENT_SCRIPT(AltmillerCave_RestoreBabiAndInviteParty)},
    {EVENT_TOUCH, 19, 0x961, EVENT_SCRIPT(AltmillerCave_StopPartyLeavingBabi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, 0x960,
        EVENT_SCRIPT(AltmillerCave_MeetInvisibleBabi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0xf30, MSGNO(MsgAltmillerNeedDraught)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, 0x962,
        EVENT_SCRIPT(AltmillerCave_RestoreBabiAndInviteParty)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_RestoreDarknessAfterReveal)},
    {PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_RespawnActorObject)},
    {SEARCH(SEARCH_CHEST), 100, 0xef5, 0x500005},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCaveEvents3[] = {
    {0x21, 20, CONDITION_ALWAYS, 20},
    {0x31, 21, CONDITION_ALWAYS, 21},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_RestoreDarknessAfterReveal)},
    {PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_RespawnActorObject)},
    {SEARCH(SEARCH_CHEST), 100, 0xf9c, FIND_ITEM(ITEM_VIAL)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCaveEvents4[] = {
    {0x21, 30, CONDITION_ALWAYS, 30},
    {0x31, 31, CONDITION_ALWAYS, 31},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 72, 0x211, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {TOUCH_WALKING_INTO(FACING_WEST), 73, 0x211, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 74, 0x211, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {TOUCH_WALKING_INTO(FACING_EAST), 75, 0x211, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {0x202, 71, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_MoveStagedActorToClearPosition)},
    {TOUCH_WALKING_INTO(FACING_EAST), 77, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_AdvanceObstacleAndCheckPassage)},
    {PSYNERGY_ON_ACTOR(0x8c), 12, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_UpdateMovedObstacle)},
    {0x10001815, 11, 0x211, EVENT_SCRIPT(AltmillerCave_CheckFrostTargetCollision)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x211, EVENT_SCRIPT(AltmillerCave_FreezePuddle)},
    {EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_StartDjinnBattle)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCaveEvents5[] = {
    {0x21, 40, CONDITION_ALWAYS, 40},
    {0x31, 41, CONDITION_ALWAYS, 41},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_RestoreDarknessAfterReveal)},
    {PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_RespawnActorObject)},
    {SEARCH(SEARCH_CHEST), 100, 0xf9d, FIND_ITEM(0xc0)},
    {SCENE_EVENTS_END},
};
// clang-format on

struct SceneEvent gAltmillerCaveEvents6[] = {
    {0x21, 50, CONDITION_ALWAYS, 50},
    {EVENT_EXIT, 51, CONDITION_ALWAYS, 51},
    {EVENT_EXIT, 52, CONDITION_ALWAYS, 52},
    {EVENT_EXIT, 53, CONDITION_ALWAYS, 53},
    {EVENT_EXIT, 54, CONDITION_ALWAYS, 54},
    {0x202, 70, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_MoveStagedActorToClearPosition)},
    {EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {SEARCH(SEARCH_CHEST), 100, 0xf30, FIND_ITEM(0xed)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_ApplyRoomVisuals)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_RestoreDarknessAfterReveal)},
    {SCENE_EVENTS_END},
};
