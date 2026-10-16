#include "script.h"
#include "animspr.h"
#include "version.h"
#include "game_state.h"
#include "keystate.h"
#include "scene_ids.h"
#include "unit/scene/imil.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
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
#include "field_effect.h"
#include "facing_object.h"
#include "battle_effect_runtime.h"
#include "object_lookup.h"
#include "field_service.h"
#include "iwram_call.h"
#include "frame.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "map.h"

// Keep native call forms while UBFIX uses the declared void contracts.
#ifdef UBFIX
#define IMIL_OPEN_SHOP Engine_ShopOpen
#else
#define IMIL_OPEN_SHOP ((s32 (*)())Engine_ShopOpen)
#endif

// Forward declarations.
static void Imil_MeetMiaAtSickMansHouse(void);
static void Imil_CastPsynergyAtActorNine(void);
static void Imil_SpawnAmbientEffects(void);
static void Imil_UpdateMapLayerByLeaderPosition(void);
static void Imil_MoveSnowmanToChimney(void);
static void Imil_ShowFirstArrival(void);
static void Imil_SayFarewellToHealerStudent(void);
static void Imil_RunIceSlide(void);

// View the table as signed halfwords.
extern s32 gImil_ArcOrigin[];

// Wobble the sick actor's scale and vary the pause between movements.
static s32 Imil_UpdateSickActorWobble(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  switch (actor->timer) {
  case 6:
    actor->scaleX += -Q16_ONE / 4;
    actor->scaleY += Q16_ONE / 8;
    break;
  case 4:
    actor->scaleX += Q16_ONE / 8;
    actor->scaleY -= Q16_ONE / 16;
    break;
  case 2:
    actor->scaleX += Q16_ONE / 16;
    actor->scaleY += -Q16_ONE / 32;
    break;
  case 0:
    actor->scaleX += Q16_ONE / 16;
    actor->scaleY += -Q16_ONE / 32;
    if ((s16)actor->work != 0) {
      actor->timer = (u32)Engine_RandomNext() % 40 + 40;
    } else {
      actor->timer = (u32)Engine_RandomNext() % 20 + 20;
    }
    break;
  }
  actor->timer--;
  return 1;
}

// Shared source is included here in ROM order.
#include "common/object_facing.h"

// Select the town or interior entrance table.
const struct SceneEntrance *Imil_GetEntrances(void) {
  if (gGameState.scene == (s32)&SceneId_Imil2) {
    return gImilEntrances2;
  }
  return gImilEntrancesOther;
}

// Imil has no additional scene region table.
struct SceneRegion *Imil_GetRegions(void) { return NULL; }

// Return Imil's exits.
u32 *Imil_GetExits(void) { return gImil_Exits; }

// Select actor placements and restore the recovered villagers.
const struct ScenePlacement *Imil_GetPlacements(void) {
  struct ScenePlacement *script;

  if (gGameState.scene == ((s32)&SceneId_Imil2)) {
    script = gImilPlacements2;
    FieldScene_PrepareActors(script);
    if (Engine_GameFlagIsSet(0x881) != 0) {
      /* FAKEMATCH: retain row 10's talk-facing byte access (10 * 24 + 22).
       * Direct script[10].talkFacing shortens tbs-en by two bytes
       * (2026-10-16). */
      ((u8 *)script)[262] = 0;
      script[3].x = PIXELS(182);
      script[3].z = PIXELS(564);
      script[3].behavior = 2;
    }
    return script;
  }

  if (Engine_GameFlagIsSet(0x881) != 0) {
    return gImilPlacementsFlag881;
  }
  return gImilPlacements;
}

// Start the movement handler for an ice-slide trigger.
static void Imil_HandleIceSlide(void) { Imil_RunIceSlide(); }

// Select the town or interior interaction table.
const struct SceneEvent *Imil_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_Imil2) {
    return gImilEvents2;
  }
  return gImilEventsOther;
}

// Discuss the villager's cold and Mia's care.
static void Imil_TalkToSickVillager(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(3) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilEvenIfMiaHealsUs));
  } else {
    Engine_EventSetMessage(MSGNO(MsgImilBrrrrrChooImFreezingMia));
  }
  {
    s32 val = 0;
    s32 mode = 8;
    Engine_EventShowMessage(mode, val);
  }
  Engine_EventEnd();
}

// Read the sick villager's thoughts about Mia.
static void Imil_ReadSickVillagerMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(3) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilCantUnderstandWhyAnyOne));
  } else {
    Engine_EventSetMessage(MSGNO(MsgImilMiaShouldHereByNow));
  }
  Engine_EventShowMessage(8, 0);
  Engine_EventEnd();
}

// Have the villager ask whether Isaac is new to Imil.
static void Imil_GreetVisitor(void) {
  Engine_EventBegin();
  Engine_ActorFaceActor(9, ACTOR_PARTY_LEADER, 10);
  Engine_EventSetMessage(MSGNO(MsgImilHiNewInImil));
  Engine_EventAskYesNo(9, 0);
  Engine_EventEnd();
}

// Let the counting child react to Isaac before resuming counting.
static void Imil_TalkToCountingChild(void) {
  Engine_EventBegin();

  if (GameFlag_IsSet(0x881) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilCountingAfterDeparture));
    Event_ShowMessage(10, 0);
    Actor_SetAttachedEffect(10, 258);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(20);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 20);
    Event_AskYesNo(10, 0);
    Actor_FaceDirection(10, FACING_SOUTHEAST + FACING_STEP, 10);
    Engine_ActorSetAnimation(10, 9);
  } else {
    Engine_EventSetMessage(MSGNO(MsgImilOneTwoThreeFour));
    Event_ShowMessage(10, 0);
    Actor_SetAttachedEffect(10, 258);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(20);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 20);
    Event_ShowMessage(10, 0);
    Actor_FaceDirection(10, FACING_SOUTHEAST + FACING_STEP, 10);
    Engine_ActorSetAnimation(10, 9);
  }

  Engine_EventEnd();
}

// Show the patient's illness, improvement, or recovery.
static void Imil_TalkToSickMan(void) {

  struct FieldActor *patient;
  struct FieldActor *caretaker;

  if (GameFlag_IsSet(2177) != 0) {
    Engine_EventBegin();
    Actor_FaceActor(9, 0, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgMercuryReallySayDie);
    Event_AskYesNo(9, 0);
    Engine_EventEnd();
  } else {
    if (GameFlag_IsSet(2091) != 0) {
      Engine_EventBegin();
      Engine_ActorSetAnimation(9, 7);
      Engine_MapAnimateCells(gImil_CellStepsA, 10, 69);
      Engine_EventSetMessage(MsgMercuryFeelMuchBetter);
      Event_ShowMessage(9, 0);
      Engine_ActorSetAnimation(9, 8);
      Map_AnimateCells(gImil_CellStepsB, 10, 69);
      Engine_EventEnd();
    } else {
      Engine_EventBegin();
      patient = Object_GetById(9);
      patient->timer = 10;
      Actor_SetActionScript(9, gImil_ActorScriptA);
      Engine_EventSetMessage(MsgMercuryOoohHelpMe);
      Engine_EventShowMessage(9, 0);
      Engine_ActorStop(8);
      Actor_ShowEmote(8, 256, 40);
      Actor_FaceDirection(8, FACING_NORTH + FACING_STEP, 10);
      Engine_ActorStartRepeatedMotion(8, 2);
      Event_ShowMessageAndWait(8, 0, 20);
      Actor_SetActionScript(0, gImil_ActorScriptC);
      Actor_SetSpeed(8, PIXELS(8) / 5, PIXELS(4) / 5);
      Actor_RunActionScriptAndWait(8, gImil_ActorScriptB);
      Engine_EventWait(40);
      Engine_ActorJump(8, 2, 0);
      Engine_ActorStartRepeatedMotion(8, 2);
      Engine_ActorSetAttachedEffect(8, 258);
      Engine_EventWait(60);
      Event_ShowMessageAndWait(8, 0, 10);
      Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 20);
      Engine_ActorStartRepeatedMotion(8, 2);
      Engine_EventShowMessage(8, 0);
      caretaker = Object_GetById(8);
      caretaker->collisionFlags ^= 0x2;
      GameFlag_Set(0x82c);
      Engine_EventEnd();
    }
  }
}

// Ask whether Isaac wants weapons.
static void Imil_AskAboutWeapons(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MSGNO(MsgImilDoWantWeapons));
  Engine_EventAskYesNo(12, 0);
  Engine_EventEnd();
}

// Ask whether Isaac wants to see the restaurant menu.
static void Imil_AskAboutRestaurantMenu(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MSGNO(MsgImilDoWantSeeRestaurantMenu));
  Engine_EventAskYesNo(18, 0);
  Engine_EventEnd();
}

// Discuss Mia and the old couple who need her help.
static void Imil_AskAboutMia(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(3) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilEveryoneCountsOnMiaThats));
    Engine_EventShowMessage(20, 0);
  } else {
    Engine_EventSetMessage(MsgImilHaveVisitedOldCoupleWho);
    Engine_EventAskYesNo(20, 0);
    Engine_GameFlagSet(0x82a);
    Engine_GameFlagSet(0x82c);
  }
  Engine_EventEnd();
}

// Read the neighbor's thoughts about Mia's responsibilities.
static void Imil_ReadMiaNeighborMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(3) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilWeHaveDoWhateverWe));
  } else {
    Engine_EventSetMessage(MSGNO(MsgImilMiaWasSayingSheHas));
  }
  Engine_EventShowMessage(20, 0);
  Engine_EventEnd();
}

// Discuss what happened at the lighthouse.
static void Imil_DiscussLighthouse(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MSGNO(MsgImilHappenedInLighthouseNortheast));
  Event_ShowMessage(8, 0);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_EventEnd();
}

// Read the caretaker's thoughts about Mia and the patient.
static void Imil_ReadCaretakerMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x82b) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilMiaClanOnceLivedHere));
  } else if (Engine_GameFlagIsSet(0x82c) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilHesAlwaysExaggeratingThingsBut));
  } else {
    Engine_EventSetMessage(MSGNO(MsgImilMiaRunningAroundTownCaring));
  }
  Engine_EventShowMessage(8, 0);
  Engine_EventEnd();
}

// Run the patient's dialogue from the interior event table.
static void Imil_TalkToSickManAtHome(void) { Imil_TalkToSickMan(); }

// Read the child's admiration for Mia.
static void Imil_ReadCountingChildMind(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MSGNO(MsgImilMiaIsAGoodGirlI));
  Engine_EventShowMessage(10, 0);
  Engine_EventEnd();
}

// Read the healer student's hopes and worries.
static void Imil_ReadHealerStudentMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(3) != 0) {
    Engine_EventSetMessage(MSGNO(MsgImilFeelLikeGrownUpWhen));
  } else {
    Engine_EventSetMessage(MSGNO(MsgImilTheseFolkOkayTheyDont));
  }
  Engine_EventShowMessage(19, 0);
  Engine_EventEnd();
}

// Animate the touched door and move Isaac through its exit.
static void Imil_RunExitDoor(void) {
  struct EventWork *event;
  struct FieldActor *actor;
  u32 i;
  s32 exit;

  event = gEventWork;
  Engine_EventBegin();
  for (i = 8; i <= 65; i++) {
    actor = Object_GetById(i);
    if (actor != NULL) {
      actor->motionFlags = 0;
    }
  }
  exit = (s16)(event->touchedTrigger - 50);
  if (exit == 6) {
    Engine_AudioPlayCue(188);
  } else {
    Engine_AudioPlayCue(158);
  }
  {
    s32 x = gImil_ExitCellPoints[exit - 1][0];
    s32 y = gImil_ExitCellPoints[exit - 1][1];

    Engine_MapAnimateCells(gImil_ExitCellSteps[exit - 1], x, y);
  }
  Actor_SetSpeed(0, PIXELS(1) / 2, PIXELS(1) / 4);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (exit == 6) {
    Actor_SetSpeed(0, PIXELS(1) / 5, PIXELS(1) / 10);
    Engine_ActorSetAnimation(0, 2);
    Engine_ActorSetSpritePriority(0, 3);
    Actor_SetDestinationOffset(0, 0, -8);
  } else {
    Object_GetById(0)->motionFlags = 0;
    Actor_CenterAndWalk(0, 3, -16);
  }
  Engine_EventWait(16);
  Engine_EventRequestExit(exit);
  Engine_EventEnd();
}

// Open the weapon shop across the counter or discuss the shopkeeper's illness.
static void Imil_RunWeaponShop(void) {
  s32 facing;

  facing = (s16)Object_GetById(0)->facing;
  if (Engine_GameFlagIsSet(0x881) != 0) {
    if ((u32)((facing << 16) + 0x5fff0000) <= 0x3ffe0000) {
      Engine_ShopOpen(10, 12);
      return;
    }
    Engine_EventBegin();
    Engine_ActorFaceActor(12, 0, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgMercuryCatchUpLostOpportunity);
    Engine_EventShowMessage(12, 0);
    Actor_FaceDirection(12, FACING_SOUTH, 10);
    Engine_EventEnd();
  } else {
    if ((u32)((facing << 16) + 0x5fff0000) <= 0x3ffe0000) {
      Engine_EventBegin();
      Engine_CameraSetSpeed(PIXELS(6), PIXELS(3) / 4);
      Camera_MoveTo(PIXELS(426), -1, PIXELS(492), 1);
      Engine_CameraWaitForMove();
      Engine_EventWait(20);
      Actor_RunActionScriptAndWait(12, gImil_TurnScript);
      Engine_EventSetMessage(MsgMercuryStoreClosedUntilWell);
      Engine_EventShowMessage(12, 0);
      Engine_CameraMoveTo(PIXELS(426), -1, PIXELS(616), 1);
      Engine_CameraWaitForMove();
      Engine_EventEnd();
    }
  }
}

// Open the armor shop across the counter or discuss the shopkeeper's illness.
static void Imil_RunArmorShop(void) {
  s32 facing;

  facing = (s16)Object_GetById(0)->facing;
  if (Engine_GameFlagIsSet(0x881) != 0) {
    if ((u32)((facing << 16) + 0x5fff0000) <= 0x3ffe0000) {
      Engine_ShopOpen(11, 13);
      return;
    }
    Engine_EventBegin();
    Engine_ActorFaceActor(13, 0, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgMercuryShameStoreClosed);
    Engine_EventShowMessage(13, 0);
    Actor_FaceDirection(13, FACING_SOUTH, 10);
    Engine_EventEnd();
  } else {
    if ((u32)((facing << 16) + 0x5fff0000) <= 0x3ffe0000) {
      Engine_EventBegin();
      Engine_CameraSetSpeed(PIXELS(6), PIXELS(3) / 4);
      Camera_MoveTo(PIXELS(426), -1, PIXELS(492), 1);
      Engine_CameraWaitForMove();
      Engine_EventWait(20);
      Actor_RunActionScriptAndWait(13, gImil_TurnScript);
      Engine_EventSetMessage(MsgMercuryTooSickBusiness);
      Engine_EventShowMessage(13, 0);
      Engine_CameraMoveTo(PIXELS(426), -1, PIXELS(616), 1);
      Engine_CameraWaitForMove();
      Engine_EventEnd();
    }
  }
}

// Open the item shop or discuss the shopkeeper and her daughter.
static void Imil_RunItemShop(void) {
  s32 facing;

  facing = (s16)Object_GetById(0)->facing;
  if (Engine_GameFlagIsSet(0x881) != 0) {
    if ((u32)((facing << 16) + 0x5fff0000) <= 0x3ffe0000) {
      Engine_ShopOpen(12, 15);
      return;
    }
    Engine_EventBegin();
    Engine_ActorFaceActor(15, 0, 0);
    Engine_EventSetMessage(MsgMercuryRecoveredThanksFountain);
    Engine_EventShowMessage(15, 0);
    Actor_FaceDirection(15, FACING_SOUTH, 0);
    Engine_EventEnd();
  } else {
    if ((u32)((facing << 16) + 0x5fff0000) <= 0x3ffe0000) {
      Engine_EventBegin();
      Engine_EventSetMessage(MsgMercuryMomsSickGetWhatever);
      Engine_EventShowMessage(14, 0);
      /* FAKEMATCH: called through a cast to the value-returning call the bytes
       * show; called by its void prototype, the two argument moves swap (differ
       * 2). Forced tbs-en on 2026-10-16 confirms the two argument moves swap
       * (four text bytes). A prototyped s32 (*)(s32, s32) cast also matches
       * (2026-10-13).
       */
      IMIL_OPEN_SHOP(12, 14);
      Engine_EventEnd();
    } else {
      Engine_ActorFaceActor(14, 0, 10);
      Engine_EventSetMessage(MsgMercuryDadLumberjack);
      Engine_EventShowMessage(14, 0);
      Actor_FaceDirection(14, FACING_SOUTH + FACING_STEP, 10);
    }
  }
}

// Open the inn across the counter or speak with its keeper.
static void Imil_VisitInn(void) {
  struct FieldActor *leader;

  leader = Object_GetById(0);
  if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
    Engine_InnOpen(4, 16);
  } else {
    Engine_EventBegin();
    Actor_FaceActor(16, ACTOR_PARTY_LEADER, 10);
    if (GameFlag_IsSet(0x881) != 0) {
      Engine_EventSetMessage(MSGNO(MsgImilItsAlmostTimeForLeave));
      Event_AskYesNo(16, 0);
    } else {
      Engine_EventSetMessage(MSGNO(MsgImilWhyHaveTwoGroupsTravelers));
      Event_ShowMessage(16, 0);
    }
    Actor_FaceDirection(16, FACING_SOUTHEAST + FACING_STEP, 10);
    Engine_EventEnd();
  }
}

// Open the sanctum or discuss the student's work in Mia's absence.
static void Imil_VisitSanctum(void) {
  struct FieldActor *leader;

  leader = Object_GetById(0);
  if ((u16)(leader->facing + 0x5fff) <= 0x3ffe) {
    Engine_EventBegin();
    if (GameFlag_IsSet(0x82d) == 0) {
      Engine_EventSetMessage(MSGNO(MsgImilMayOnlyStudentButCan));
      Event_ShowMessage(19, 0);
      GameFlag_Set(0x82d);
    }
    Engine_EventEnd();
    Engine_SanctumOpen(19);
  } else {
    Engine_EventBegin();
    if (GameFlag_IsSet(0x881) != 0) {
      Engine_EventSetMessage(MSGNO(MsgImilMiaGoingOnJourneyWith));
      Event_ShowMessage(19, 0);
    } else if (GameFlag_IsSet(3) != 0) {
      Engine_EventSetMessage(MSGNO(MsgImilAmHealerWhileMiaOut));
      Event_ShowMessage(19, 0);
    } else {
      Engine_EventSetMessage(MSGNO(MsgImilLookingForMia));
      (void)Event_AskYesNo(19, 0);
      Actor_FaceDirection(19, FACING_SOUTHEAST + FACING_STEP, 10);
    }
    Engine_EventEnd();
  }
}

// Restore villagers, shop interiors, and the arrival scenes.
s32 Imil_Initialize(void) {
  struct FieldActor *leader;
  s32 entrance;

  if (gGameState.scene == (s32)&SceneId_Imil1) {
    leader = Object_GetById(0);
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_ActorSetAnimation(10, 9);
    if (GameFlag_IsSet(0x109)) {
      Engine_GameFlagClear(0x200);
      Engine_GameFlagClear(0x201);
    }
    leader->timer = 0;
    leader->work = 0;
    Task_AddCallback(Imil_SpawnAmbientEffects, 0xc80);
    FieldScheduler_AddCallback(Imil_UpdateMapLayerByLeaderPosition, 0xc80);
    Engine_ActorSetSpritePriority(11, 1);
    if (Engine_GameFlagIsSet(0x203)) {
      Imil_MoveSnowmanToChimney();
    }
    if (!Engine_GameFlagIsSet(0x109) && gGameState.entrance == 9) {
      Imil_ShowFirstArrival();
    }
  } else if (gGameState.scene == (s32)&SceneId_Imil2) {
    // Restore each interior's occupants before running its arrival event.
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    entrance = gGameState.entrance;
    if (entrance == 1) {
      Engine_ActorSetChildValue(21, 15);
      Object_GetById(21)->collisionFlags |= 8;
      Engine_ActorSetSpritePriority(21, 1);
      if (Engine_GameFlagIsSet(0x881)) {
        Map_CopyCellAttributes(10, 7, 1, 1, 10, 8);
        Map_CopyCellsTo(3, 125, 9, 69, 3, 3);
        Engine_MapRedraw();
        Engine_TaskWait(1);
        Actor_SetBuiltinAction(8, 2);
        Engine_ActorSetPosition(10, 0, 0);
      } else if (Engine_GameFlagIsSet(0x82c) && Engine_GameFlagIsSet(0x82a)) {
        Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
        Engine_ActorSetPosition(9, PIXELS(174), PIXELS(164));
        Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
        Engine_ActorSetAnimation(9, 5);
        Actor_SetPosition(8, PIXELS(168), PIXELS(152));
        Object_GetById(8)->facing = FACING_SOUTHEAST + FACING_STEP;
        if (!Engine_GameFlagIsSet(0x82b)) {
          Imil_MeetMiaAtSickMansHouse();
        }
      } else {
        Map_CopyCellAttributes(10, 7, 1, 1, 10, 8);
        Map_CopyCellsTo(3, 125, 9, 69, 3, 3);
        Engine_MapRedraw();
        Engine_TaskWait(1);
        if (Engine_GameFlagIsSet(0x82c)) {
          Engine_ActorSetPosition(8, PIXELS(149), PIXELS(116));
          Object_GetById(8)->facing = 0;
          Object_GetById(9)->work = 0;
          Actor_SetActionScript(9, gImil_ActorScriptA);
        } else {
          Actor_SetBuiltinAction(8, 2);
        }
      }
    } else if (entrance == 2) {
      if (!Engine_GameFlagIsSet(0x881)) {
        Object_GetById(11)->work = 1;
        Actor_SetActionScript(11, gImil_ActorScriptA);
      }
    } else if (entrance == 4) {
      if (Engine_GameFlagIsSet(0x881)) {
        Actor_SetPosition(12, PIXELS(364), PIXELS(578));
        Engine_ActorSetSpritePriority(12, 2);
        Object_GetById(12)->collisionFlags |= 4;
        Map_CopyCellsTo(6, 125, 22, 88, 3, 3);
        Actor_SetPosition(13, PIXELS(492), PIXELS(578));
        Engine_ActorSetSpritePriority(13, 2);
        Object_GetById(13)->collisionFlags |= 4;
        Map_CopyCellsTo(9, 125, 28, 88, 3, 3);
      } else {
        Object_GetById(12)->scaleX = -Q16_ONE;
        Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
        Engine_ActorSetAnimation(12, 5);
        Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
        Engine_ActorSetAnimation(13, 5);
      }
    } else if (entrance == 3) {
      if (Engine_GameFlagIsSet(0x881)) {
        Actor_SetPosition(15, PIXELS(460), PIXELS(258));
        Engine_ActorSetSpritePriority(15, 2);
        Object_GetById(15)->collisionFlags |= 4;
        Engine_ActorSetPosition(14, PIXELS(408), PIXELS(264));
        Object_GetById(14)->facing = FACING_EAST + FACING_STEP;
        Map_CopyCellsTo(12, 125, 26, 70, 3, 3);
      } else {
        Actor_SetPosition(14, PIXELS(460), PIXELS(258));
        Engine_ActorSetSpritePriority(14, 2);
        Object_GetById(14)->collisionFlags |= 4;
        Object_GetById(15)->scaleX = -Q16_ONE;
        Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
        Engine_ActorSetAnimation(15, 5);
      }
    } else if (entrance == 7) {
      if (Engine_GameFlagIsSet(0x881)) {
        Object_GetById(20)->facing = FACING_SOUTHEAST + FACING_STEP;
        if (!Engine_GameFlagIsSet(0x82e)) {
          Actor_SetPosition(20, PIXELS(650), PIXELS(161));
          Imil_SayFarewellToHealerStudent();
        } else {
          Actor_SetPosition(20, PIXELS(644), PIXELS(166));
        }
      }
    }
  }
  return 0;
}

// Introduce Mia as she tends the sick man and hears about the lighthouse.
static void Imil_MeetMiaAtSickMansHouse(void) {
  struct FieldActor *leader;
  const s32 *table;
  struct EventWork *work;

  Engine_EventBegin();
  Actor_SetPosition(MIA, PIXELS(182), PIXELS(150));
  Camera_MoveTo(PIXELS(141), -1, PIXELS(221), 0);
  Engine_TaskWait(1);
  Camera_SetSpeed(PIXELS(3) / 10, PIXELS(3) / 80);
  Camera_MoveTo(PIXELS(140), -1, PIXELS(164), 1);
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 40;
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(GARET, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetSpeed(IVAN, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 142, 221);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  leader = Object_GetById(0);
  if (leader != NULL) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(0);
  if (leader != NULL) {
    Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  Actor_WalkTo(GARET, 150, 234);
  Actor_WalkToAndWait(IVAN, 134, 234);
  Engine_ActorSetAnimation(GARET, 1);
  FieldObject_SetTargetAndCallback(0, ACTOR_LINK_KEEP_MOTION | 3,
                                   gImil_PrimaryScript);
  FieldObject_SetTargetAndCallback(1, ACTOR_LINK_KEEP_MOTION | 3,
                                   gImil_PrimaryScript);
  FieldObject_SetTargetAndCallback(2, ACTOR_LINK_KEEP_MOTION | 3,
                                   gImil_PrimaryScript);
  Engine_CameraWaitForMove();
  table = gImil_TurnScript;
  Actor_SetActionScript(9, table);
  Engine_EventWait(40);
  Actor_SetAttachedEffect(MIA, 0x102);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  // Mia checks on the patient while the party enters the house.
  Engine_EventSetMessage(MSGNO(MsgImilHowFeeling));
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_SetActionScript(9, table);
  Event_ShowMessageAndWait(9, 0, 20);
  Actor_FaceDirection(MIA, FACING_WEST, 20);
  Actor_FaceDirection(8, 0, 10);
  Engine_ActorSetAnimationAndWait(8, 4);
  Event_ShowMessageAndWait(8, 0, 40);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(10);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(MIA, 0, 10);
  Actor_SetActionScript(9, table);

  // Play the healing effect, then let the patient respond.
  Imil_CastPsynergyAtActorNine();
  Actor_ShowEmote(MIA, 0x101, 60);
  Event_ShowMessageAndWait(MIA, 0, 40);
  Event_ShowMessageAndWait(9, 0, 20);
  Actor_ShowEmote(8, 0x105, 60);
  Engine_ActorSetAnimation(9, 7);
  Map_AnimateCells(gImil_CellStepsA, 10, 69);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_ActorSetAnimationAndWait(MIA, 4);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorRunRepeatedMotion(9, 1);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(9, 8);
  Map_AnimateCells(gImil_CellStepsB, 10, 69);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(8, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Event_ShowMessageAndWait(8, 0, 10);
  Actor_ShowEmote(MIA, 0x101, 30);
  Actor_FaceDirection(MIA, FACING_WEST, 10);
  Engine_ActorSetAnimation(MIA, 4);
  Event_ShowMessageAndWait(MIA, 0, 10);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(40);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Object_GetById(3)->timer = 0;
  Actor_SetActionScript(MIA, gImil_MiaScriptA);
  while (Object_GetById(3)->timer == 0) {
    Engine_TaskWait(1);
  }
  Camera_MoveTo(PIXELS(140), -1, PIXELS(198), 1);
  Object_RefreshSelectorById(3);
  Actor_ShowEmote(MIA, 0x101, 80);
  Event_ShowMessageAndWait(MIA, 0, 40);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventWait(10);
  Event_ShowMessage(MIA, 0);
  Event_PlayCue(131);
  // The lighthouse interrupts the visit; Mia leaves and the party follows.
  ColorBuffer_ApplySource(PALETTE_TRANSFORM_PLAIN, 0);
  ColorBuffer_ApplyTarget(0x207e9f, 0);
  Engine_ColorBufferInterpolate(10);
  Engine_TaskWait(1);
  Event_PlayCue(220);
  Engine_TaskWait(40);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 0);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  Actor_SetAttachedEffect(MIA, 0x102);
  Engine_EventWait(20);
  Actor_FaceDirection(MIA, 0, 10);
  Actor_SetSpeed(MIA, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(MIA, 202, 198);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Event_ShowMessage(MIA, 0);
  Engine_ActorSetAnimationAndWait(MIA, 4);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_SetAttachedEffect(MIA, 0x102);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(MIA, 0, 40);
  Actor_ShowEmote(MIA, 0x100, 40);
  Event_ShowMessage(MIA, 0);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(IVAN);
  Actor_SetSpeed(MIA, PIXELS(3), PIXELS(3) / 2);
  Object_GetById(3)->timer = 0;
  Actor_SetActionScript(MIA, gImil_MiaScriptB);
  while (Object_GetById(3)->timer == 0) {
    Engine_TaskWait(1);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4), PIXELS(2));
  Actor_SetSpeed(GARET, PIXELS(4), PIXELS(2));
  Actor_SetSpeed(IVAN, PIXELS(4), PIXELS(2));
  Event_PlayCue(152);
  Object_GetById(0)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(1)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Object_GetById(2)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(ACTOR_PARTY_LEADER, 132, 206);
  Actor_SetDestination(GARET, 136, 221);
  Actor_SetDestination(IVAN, 122, 238);
  Object_RefreshSelectorById(3);
  Engine_EventWait(80);
  Object_GetById(0)->steeringFlags |= ACTOR_FACES_MOTION;
  Object_GetById(1)->steeringFlags |= ACTOR_FACES_MOTION;
  Object_GetById(2)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  table = gImil_PrimaryScript2;
  Actor_SetActionScript(GARET, table);
  Actor_RunActionScriptAndWait(2, table);
  Engine_EventWait(20);
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  work->transitionFrames = 24;
  Engine_GameFlagSet(0x82b);
  Engine_EventEnd();
}

// Move an unused effect around its stored origin until its timer expires.
UNUSED static void Imil_UpdateCountdownArc(struct FieldActor *actor) {
  s32 buf[3];
  s32 *b;
  s32 n;
  s32 t;

  if (actor != NULL) {
    n = (u16)actor->timer - 1;
    actor->timer = n;
    t = (s16)n;
    if (t != 0) {
      b = buf;
      b[0] = gImil_ArcOrigin[0];
      b[1] = gImil_ArcOrigin[1] + PIXELS(8);
      b[2] = gImil_ArcOrigin[2];
      Vector_AddPolarOffset(t << 16, (t << 11) + (s16)actor->work, b);
      actor->x.fixed = b[0];
      actor->y.fixed = b[1];
      actor->z.fixed = b[2];
    } else {
      Engine_ObjectDispatchRelease(actor);
    }
  }
}

// Sway an unused actor and occasionally emit a rising spark.
UNUSED static void Imil_SwayAndSpark(struct FieldActor *actor) {
  s32 pos[3];
  struct FieldActor *spark;
  s32 radius;

  actor->x.fixed = gImil_ArcOrigin[0] +
                   Iwram_MulQ16(0x60000, Engine_MathSin(actor->timer << 10));
  actor->timer++;
  actor->timer = (actor->timer + 64) % 64;
  if (gFrameCount % 3 == 0) {
    pos[0] = actor->x.fixed;
    pos[1] = actor->y.fixed + PIXELS(2);
    pos[2] = actor->z.fixed;
    radius = Engine_RandomNext();
    Vector_AddPolarOffset(radius * 6, Engine_RandomNext(), pos);
    spark = Engine_ObjectCreate(0x11d, pos[0], pos[1], pos[2]);
    if (spark != NULL) {
      spark->sprite->image.priority = 0;
      Engine_ActorSetSpriteFlags(spark, 0);
      Object_SetMode(spark, 1);
      spark->scaleX = 3 * Q16_ONE / 5;
      spark->scaleY = 3 * Q16_ONE / 5;
      spark->priorityFlags = 2;
      spark->motionFlags = 0;
      ObjectGroup_SetChildValue(spark, 9);
      Engine_ObjectSetScript(spark, gImil_SparkScript);
    }
  }
}

// Animate the patient's response and show the current message.
static void Imil_ShowActorNineTurnMessage(void) {
  Actor_SetActionScript(9, gImil_TurnScript);
  Engine_EventShowMessage(9, 0);
}

// Play Mia's healing effect with the patient's response callback.
static void Imil_CastPsynergyAtActorNine(void) {
  u8 *work;

  Engine_PsynergyBegin(93, 1);
  work = gEffectWork;
  Engine_PsynergySetTarget(3, 9);
  *(s32 *)(work + 36) = (s32)Imil_ShowActorNineTurnMessage;
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_PsynergyLowerHands();
  BattleEffect_CleanupSceneObjects();
}

// Reset a rising effect's motion, sprite priority, and initial scale.
static void Imil_InitializeRisingEffect(struct FieldActor *work) {
  struct AnimationObject *sprite;

  work->motionFlags = 0;
  work->timer = 0;
  work->priorityFlags &= ~1;

  sprite = work->sprite;
  sprite->image.priority = 1;

  ObjectGroup_SetChildValue(work, 9);
  Engine_ActorSetSpriteFlags(work, 0);

  work->scaleX = Q16_ONE / 2;
  work->scaleY = Q16_ONE / 2;
}

// Move and enlarge an ambient effect, then release it when its timer expires.
static void Imil_UpdateRisingEffect(union FieldObject *object) {
  object->effect.x += (s16)object->effect.spin << 8;
  object->effect.y += PIXELS(1) / 2;
  object->effect.scaleX += 3 * Q16_ONE / 100;
  object->effect.scaleY += 3 * Q16_ONE / 100;
  object->effect.spin += 2;
  if (--object->effect.countdown == 0) {
    Engine_ObjectDispatchRelease(&object->actor);
  }
}

// Initialize a newly created ambient effect for its one-second rise.
static inline void Imil_StartRisingEffect(union FieldObject *object) {
  if (object != NULL) {
    Imil_InitializeRisingEffect(&object->actor);
    object->effect.countdown = 60;
    object->effect.update = Imil_UpdateRisingEffect;
    Object_SetMode(&object->actor, 5);
  }
}

// Stagger five ambient effects over the repeating frame cycle.
static void Imil_SpawnAmbientEffects(void) {
  // volatile: the type of &gFrameCount (tagged in frame.h); the six editions
  // build the same bytes without it
  volatile u32 *frame;
  union FieldObject *object;

  frame = &gFrameCount;
  if (((u32)*frame % (u32)60) == 0) {
    object =
        (union FieldObject *)Object_Create(222, PIXELS(463), 0, PIXELS(292));
    Imil_StartRisingEffect(object);
  }
  if (((u32)(*frame + 30) % (u32)60) == 0) {
    object = (union FieldObject *)Object_Create(222, PIXELS(320), PIXELS(32),
                                                PIXELS(356));
    Imil_StartRisingEffect(object);
  }
  if (((u32)(*frame + 10) % (u32)60) == 0) {
    object =
        (union FieldObject *)Object_Create(222, PIXELS(118), 0, PIXELS(70));
    Imil_StartRisingEffect(object);
  }
  if (((u32)(*frame + 50) % (u32)60) == 0) {
    object =
        (union FieldObject *)Object_Create(222, PIXELS(342), 0, PIXELS(124));
    Imil_StartRisingEffect(object);
  }
  if (((u32)(*frame + 80) % (u32)60) == 0) {
    object =
        (union FieldObject *)Object_Create(222, PIXELS(431), 0, PIXELS(171));
    Imil_StartRisingEffect(object);
  }
}

// Select map-layer state from the leader's position and zone flags.
static void Imil_UpdateMapLayerByLeaderPosition(void) {
  struct FieldActor *actor;
  s32 x;
  s32 column;
  s32 isSet;
  s32 zoneFlag;
  s32 otherZoneFlag;
  struct MapState *map;

  actor = Object_GetById(0);
  x = actor->x.fixed;
  column = x >> 19;
  zoneFlag = 0x200;
  otherZoneFlag = 0x201;
  if ((u32)(column - 24) <= 7 ||
      ((u32)((actor->z.fixed >> 19) - 36) <= 9 && (u32)(column - 22) <= 9)) {
    isSet = Engine_GameFlagIsSet(zoneFlag);
    if (isSet != 0)
      return;
    ((struct MapState *)gMapWork[0])->unk17 = isSet;
    Engine_GameFlagSet(zoneFlag);
    Engine_GameFlagClear(otherZoneFlag);
    return;
  }
  if (x > 0xE80000 && actor->y.fixed > 0x1E0000 && actor->z.fixed > 0xD40000) {
    map = gMapWork[0];
    map->unk17 = 0;
    Engine_GameFlagSet(zoneFlag);
    Engine_GameFlagClear(otherZoneFlag);
    return;
  }
  isSet = Engine_GameFlagIsSet(otherZoneFlag);
  if (isSet != 0)
    return;
  map = gMapWork[0];
  map->unk17 = 1;
  Engine_GameFlagSet(otherZoneFlag);
  Engine_GameFlagClear(zoneFlag);
  return;
}

// Update the collision cell when the snowman reaches the chimney.
static void Imil_MoveSnowmanToChimney(void) {
  s32 a;
  s32 b;

#if TBS_PATCH_3
  struct FieldActor *actor = Object_GetById(11);

  a = actor->x.fixed >> 20;
  if (a != 15)
    return;
  b = actor->z.fixed >> 20;
  if (b != 7)
    return;
  Engine_GameFlagSet(0x203);
  Engine_ActorSetSpritePriority(11, 3);
#else
  Engine_GameFlagSet(0x203);
  Engine_ActorSetSpritePriority(11, 3);
  a = 15;
  b = 7;
#endif
  Map_CopyCellAttributes(15, 6, 1, 1, a, b);
}

// Face Isaac north and leave through the ice-slide entrance.
static void Imil_EnterIceSlide(void) {
  struct FieldActor *work;

  Engine_EventBegin();
  work = Object_GetById(0);
  work->facing = FACING_NORTH;
  Engine_AudioPlayCue(123);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(8);
}

// Walk Isaac into town on his first arrival.
static void Imil_ShowFirstArrival(void) {
  Engine_EventBegin();
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1) / 10);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 232, 204);
  Engine_EventEnd();
}

// Let Mia say farewell before leaving Imil with the party.
static void Imil_SayFarewellToHealerStudent(void) {

  struct FieldActor *leader;
  struct EventWork *work;

  Engine_EventBegin();
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Engine_ActorWalkTo(0, 690, 200);

  Camera_GetEventObject()->motionFlags = 0;
  Engine_CameraSetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Camera_MoveTo(PIXELS(690), 0, PIXELS(164), 1);
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 48;
  Engine_EventOpenScreen();
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(MIA, PIXELS(3) / 5, PIXELS(3) / 10);
  leader = Object_GetById(0);
  if (leader != NULL) {
    Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
  }
  Actor_WalkToAndWait(MIA, 673, 183);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Engine_ActorStartRepeatedMotion(19, 2);
  Engine_ActorRunRepeatedMotion(20, 2);
  Engine_EventWait(40);
  Engine_EventSetMessage(MSGNO(MsgImilMia));
  Event_ShowMessageAndWait(19, 0, 10);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 40);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Actor_SetAttachedEffect(20, 0x102);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4014, 0, 10);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 40);
  Engine_ActorSetAnimationAndWait(MIA, 4);
  Event_ShowMessageAndWait(0x2003, 0, 10);
  Engine_ActorSetAnimationAndWait(20, 3);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(19, 0x102);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(19, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 40);
  Engine_ActorRunRepeatedMotion(20, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4014, 0, 20);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(60);
  Actor_ShowEmote(MIA, 0x105, 60);
  Actor_ShowEmote(19, 0x101, 0);
  Actor_ShowEmote(20, 0x101, 60);
  Engine_ActorRunRepeatedMotion(19, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(19, 0, 10);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 40);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Actor_FaceDirection(MIA, FACING_SOUTHWEST, 80);
  Event_ShowMessageAndWait(0x2003, 0, 20);
  Actor_FaceDirection(20, FACING_NORTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(19, FACING_SOUTHWEST + FACING_STEP, 40);
  Actor_FaceDirection(19, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(20, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_SetSpeed(20, PIXELS(1), PIXELS(1) / 2);
  // Let the student approach Mia before she rejoins the party.
  Object_GetById(20)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(20, 656, 166);
  Engine_EventWait(1);
  Object_GetById(20)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4014, 0, 10);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(40);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 10);
  Event_ShowMessageAndWait(0x2003, 0, 40);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 20);
  Event_ShowMessageAndWait(0x4003, 0, 10);
  Actor_SetAttachedEffect(19, 0x102);
  Actor_SetAttachedEffect(20, 0x102);
  Engine_EventWait(40);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  Engine_ActorSetAnimation(MIA, 4);
  Event_ShowMessageAndWait(0x2003, 0, 20);
  Engine_ActorRunRepeatedMotion(19, 1);
  Event_ShowMessageAndWait(19, 0, 10);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 40);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(20, 1);
  Event_ShowMessageAndWait(0x4014, 0, 20);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Event_ShowMessageAndWait(0x2003, 0, 80);
  Actor_ShowEmote(19, 0x105, 0);
  Actor_ShowEmote(20, 0x105, 60);
  Engine_ActorSetAnimationAndWait(19, 4);
  Event_ShowMessageAndWait(19, 0, 10);
  Engine_ActorSetAnimation(20, 4);
  Event_ShowMessageAndWait(0x4014, 0, 20);
  Actor_ShowEmote(MIA, 0x102, 60);
  Event_ShowMessageAndWait(0x2003, 0, 40);
  Engine_ActorSetAnimationAndWait(19, 3);
  Event_ShowMessageAndWait(19, 0, 10);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Engine_ActorSetAnimationAndWait(20, 3);
  Event_ShowMessageAndWait(0x4014, 0, 10);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 60);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Actor_FaceDirection(MIA, FACING_NORTH, 40);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Event_ShowMessageAndWait(0x2003, 0, 10);
  Engine_ActorSetAnimation(19, 3);
  Engine_ActorSetAnimationAndWait(20, 3);
  Engine_EventWait(40);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 20);
  Event_ShowMessageAndWait(0x4003, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  Actor_WalkToAndWait(MIA, 688, 200);
  Actor_SetPosition(MIA, 0, 0);
  Object_GetById(20)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(20, 644, 166);
  Engine_EventWait(1);
  // Restore the student's steering after the farewell walk.
  /* FAKEMATCH: retain the byte access to steeringFlags; the direct field
   * form swaps the following gGameState load and constant move (two
   * instructions differ in tbs-en, 2026-10-16). */
  *(u8 *)(((s32)Object_GetById(20)) + 90) |= ACTOR_FACES_MOTION;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  GameFlag_Set(0x82e);
  GameFlag_Clear(0x82d);
  Engine_EventEnd();
}

// Turn the sliding actor toward the held-key heading while its timer permits.
static void Imil_TurnSlidingActor(union FieldObject *object) {
  struct FieldActor *actor;
  s32 z;
  s32 t;
  s32 d;
  u16 prev;
  s32 n;

  actor = &object->actor;
  n = (u16)actor->timer;
  z = 0;
  t = ((s16 *)&actor->timer)[z];
  if (t != 0) {
    actor->timer = n - 1;
    return;
  }
  actor->steeringFlags = t;
  z = 1;
  d = gImil_KeyHeadings[(gKeysHeld >> 4) & 0xF];
  z = -z;
  if (d == z) {
    Object_SetMode(actor, 9);
    return;
  }
  prev = actor->facing;
  d = (s16)(d - prev);
  if (d > 0x1000)
    d = 0x1000;
  if (d < -0x1000)
    d = -0x1000;
  actor->facing = prev + d;
  Object_SetMode(actor, 2);
  ObjectDispatch_ApplyValueToChildren((struct DispatchObject *)actor, 0x30);
}

// Follow ice cells until blocked, then return control to the leader.
static void Imil_RunIceSlide(void) {

  struct FieldActor *subject;
  struct FieldPosition probe;
  s32 heading;
  s32 goal;
  s32 marker;
  s32 z;
  s32 x;
  u8 *terrainId;

  subject = Engine_ActorLookup(gGameState.selectedActor);

  for (;;) {
    heading = gImil_SwayHeadings[(gKeysHeld >> 4) & 15];
    if ((heading << 16) == -0x10000) {
      return;
    }

    Engine_EventBegin();

    probe.x = (subject->x.fixed & 0xfff00000) + PIXELS(8);
    probe.y = subject->y.fixed;
    probe.z = (subject->z.fixed & 0xfff00000) + PIXELS(8);
    z = probe.z;
    x = probe.x;
    terrainId = &subject->terrainId;
    goal = GetMapCellCollision(*terrainId, x, z);
    Vector_AddPolarOffset(PIXELS(16), heading, &probe);

    marker = GetMapCellCollision(*terrainId, probe.x, probe.z);
    if (marker == 255 ||
        Map_GetTerrainHeight(*terrainId, probe.x, probe.z) - subject->y.fixed >
            PIXELS(8)) {
      subject->facing = heading;
      goto tail;
    }

    probe.x = x;
    probe.z = z;
    subject->speed = PIXELS(2);
    subject->acceleration = PIXELS(1) / 10;
    subject->timer = 0;
    Engine_ObjectSetPosition(subject, x, subject->y.fixed, z);
    Object_SetMode(subject, 2);
    ObjectDispatch_ApplyValueToChildren((struct DispatchObject *)subject, 48);
    FieldObject_WaitForMove(subject);
    subject->update = Imil_TurnSlidingActor;

    // Advance along the slide until its next cell is blocked or too high.
    for (;;) {
      FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);
      if ((marker = GetMapCellCollision(*terrainId, probe.x, probe.z)) == 255) {
        break;
      }
      if (Map_GetTerrainHeight(*terrainId, probe.x, probe.z) -
              subject->y.fixed >
          0x80000) {
        break;
      }
      x = probe.x;
      z = probe.z;
      subject->speed = PIXELS(2);
      subject->acceleration = PIXELS(1) / 10;
      Engine_ObjectSetPosition(subject, probe.x, probe.y, probe.z);
      FieldObject_WaitForMove(subject);
      if (marker != goal) {
        goto blocked;
      }
    }

    subject->speed = PIXELS(2);
    subject->acceleration = PIXELS(1);
    Engine_ObjectSetPosition(subject, x, subject->y.fixed, z);
    FieldObject_WaitForMove(subject);
    Engine_TaskWait(2);
  }

blocked:
  subject->update = NULL;
  subject->steeringFlags |= 1;

  subject->acceleration = PIXELS(1) / 4;

tail:
  Engine_TaskWait(10);
  Engine_EventEnd();
}

// clang-format off
s16 gImil_KeyHeadings[] = {
    -1, FACING_EAST, FACING_WEST, -1, FACING_NORTH, FACING_NORTHEAST, FACING_NORTHWEST, FACING_NORTH,
    FACING_SOUTH, FACING_SOUTHEAST, FACING_SOUTHWEST, FACING_SOUTH, -1, FACING_EAST, FACING_WEST, -1,
    -1, FACING_EAST, FACING_WEST, -1, FACING_NORTH, -1, -1, FACING_NORTH,
    FACING_SOUTH, -1, -1, FACING_SOUTH, -1, FACING_EAST, FACING_WEST, -1,
};

s16 gImil_SwayHeadings[] = {
    -1, FACING_EAST, FACING_WEST, -1, FACING_NORTH, FACING_EAST, FACING_WEST, FACING_NORTH,
    FACING_SOUTH, FACING_EAST, FACING_WEST, FACING_SOUTH, -1, FACING_EAST, FACING_WEST, -1,
};

s32 gImilPlacementData[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_ADD(SW_SCALE_X, -5 * Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 16),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 32),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, 5 * Q16_ONE / 64),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gImil_ActorScriptA[] = {
    SC_CALL(Imil_UpdateSickActorWobble),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gImil_ActorScriptB[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(136), 0, PIXELS(192)),
    SC_SYNC,
    SC_MOVE(PIXELS(136), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(149), 0, PIXELS(116)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_EAST),
    SC_ARG(1),
    SC_END,
};

s32 gImil_ActorScriptC[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(155), 0, PIXELS(135)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xd000),
    SC_END,
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 32),
    SC_WAIT(4),
    SC_END,
};

s32 gImil_TurnScript[] = {
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 32),
    SC_WAIT(4),
    SC_LOOP(2, SC_TOP),
    SC_END,
};

s32 gImil_SparkScript[] = {
    SC_ADD(SW_Y, -PIXELS(2) / 5),
    SC_WAIT(1),
    SC_LOOP(72, SC_TOP),
    SC_FREE,
};

s32 gImil_MiaScriptA[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(177), 0, PIXELS(136)),
    SC_SYNC,
    SC_MOVE(PIXELS(161), 0, PIXELS(124)),
    SC_SYNC,
    SC_SET(0x23, 1),
    SC_MOVE(PIXELS(134), 0, PIXELS(139)),
    SC_SYNC,
    SC_MOVE(PIXELS(135), 0, PIXELS(189)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gImil_MiaScriptB[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(165), 0, PIXELS(207)),
    SC_SYNC,
    SC_SET(0x23, 1),
    SC_MOVE(PIXELS(147), 0, PIXELS(252)),
    SC_SYNC,
    SC_MOVE(PIXELS(150), 0, PIXELS(268)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gImil_PrimaryScript[] = {
    SC_CALL(OverlayObject_UpdateFacingTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gImil_PrimaryScript2[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(132), 0, PIXELS(206)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gImilEntrancesOther[] = {
    { 0, CONDITION_ALWAYS, 380, 0, 433, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 288, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 480, 0, 280, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { 3, CONDITION_ALWAYS, 312, 0, 424, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 4, CONDITION_ALWAYS, 424, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 5, CONDITION_ALWAYS, 456, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 6, CONDITION_ALWAYS, 376, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 7, CONDITION_ALWAYS, 120, 0, 208, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 8, CONDITION_ALWAYS, 64, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 9, CONDITION_ALWAYS, 229, 0, 88, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilEntrances2[] = {
    { 0, CONDITION_ALWAYS, 136, 0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 144, 0, 242, FACING_NORTH, 0, 32, 52, 256, 280, 0 },
    { 2, CONDITION_ALWAYS, 143, 0, 596, FACING_NORTH, 0, 42, 424, 256, 631, 0 },
    { 3, CONDITION_ALWAYS, 399, 0, 322, FACING_NORTH, 0, 320, 58, 536, 360, 0 },
    { 4, CONDITION_ALWAYS, 400, 0, 659, FACING_NORTH, 0, 307, 382, 545, 697, 0 },
    { 5, CONDITION_ALWAYS, 129, 0, 915, FACING_NORTH, 0, 50, 706, 350, 949, 0 },
    { 6, CONDITION_ALWAYS, 446, 0, 825, FACING_SOUTH, 0, 402, 740, 670, 921, 0 },
    { 7, CONDITION_ALWAYS, 687, 0, 264, FACING_NORTH, 0, 594, 49, 806, 295, 0 },
    { 8, CONDITION_ALWAYS, 182, 0, 772, FACING_SOUTH, 0, 50, 706, 350, 949, 0 },
    { SCENE_TABLE_END },
};

u32 gImil_Exits[] = {
    SCENE_EXITS(50),
    SCENE_EXIT(1, 51, 1),
    SCENE_EXIT(2, 51, 5),
    SCENE_EXIT(3, 51, 3),
    SCENE_EXIT(4, 51, 4),
    SCENE_EXIT(5, 51, 2),
    SCENE_EXIT(6, 51, 7),
    SCENE_EXIT(8, 52, 1),
    SCENE_EXIT(11, 2, 9),
    SCENE_EXIT(12, 2, 39),
    SCENE_EXIT(0, 51, 0),
    SCENE_EXIT(1, 50, 3),
    SCENE_EXIT(2, 50, 7),
    SCENE_EXIT(3, 50, 5),
    SCENE_EXIT(4, 50, 6),
    SCENE_EXIT(5, 50, 4),
    SCENE_EXIT(6, 51, 8),
    SCENE_EXIT(7, 50, 8),
    SCENE_EXIT(8, 51, 6),
    SCENE_EXITS_END,
};

struct ScenePlacement gImilPlacements[] = {
    { 0x6c, CONDITION_ALWAYS, (s32)gImilPlacementData,
        PIXELS(99), 0, PIXELS(234), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(376), 0, PIXELS(408), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(248), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xea, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(262), 0, PIXELS(124), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gImilPlacementsFlag881[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(99), 0, PIXELS(234), FACING_NORTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(376), 0, PIXELS(408), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(248), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0xea, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(262), 0, PIXELS(124), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gImilPlacements2[] = {
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(111), 0, PIXELS(215), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, 0x0, ACTOR_STAND,
        PIXELS(166), 0, PIXELS(116), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, 0x0, 7,
        PIXELS(174), 0, PIXELS(173), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x67, 0x0, ACTOR_STAND,
        PIXELS(118), 0, PIXELS(488), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x67, 0x0, ACTOR_STAND,
        PIXELS(370), 0, PIXELS(468), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x67, 0x0, ACTOR_STAND,
        PIXELS(478), 0, PIXELS(468), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x66, 0x0, ACTOR_STAND,
        PIXELS(460), 0, PIXELS(266), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(436), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x77, 0x0, ACTOR_STAND,
        PIXELS(100), 0, PIXELS(830), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x80, 0x0, ACTOR_WANDER,
        PIXELS(160), 0, PIXELS(880), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, 0x0, ACTOR_STAND,
        PIXELS(249), 0, PIXELS(832), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6b, 0x0, ACTOR_STAND,
        PIXELS(686), 0, PIXELS(148), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x66, 0x0, ACTOR_STAND,
        PIXELS(717), 0, PIXELS(229), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xdf, 0x0, 7,
        PIXELS(192), 0, PIXELS(124), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gImilEventsOther[] = {
    { EVENT_EXIT, 60, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 61, CONDITION_ALWAYS, 12 },
    { PSYNERGY_ON_ACTOR(0x8c), 11, 0x200, EVENT_SCRIPT(Imil_MoveSnowmanToChimney) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 51, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunExitDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 52, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunExitDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 53, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunExitDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 54, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunExitDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 55, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunExitDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 56, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunExitDoor) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_EnterIceSlide) },
    { EVENT_TOUCH, 90, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_HandleIceSlide) },
    { EVENT_TALK, 8, 0x881, EVENT_SCRIPT(Imil_TalkToSickVillager) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgImilTheMercuryLighthouseIsShiningMia) },
    { EVENT_TALK, 9, 0x881, EVENT_SCRIPT(Imil_GreetVisitor) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgImilImilWillBeFineDiseaseIs) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_TalkToCountingChild) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x881, EVENT_SCRIPT(Imil_ReadSickVillagerMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgImilMiaWasOnlyInImilTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x881, MSGNO(MsgImilWeveHadALotOfStrangers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgImilDontWorryAboutUsMiaWell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x881, MSGNO(MsgImilPeopleGetSickBecauseTheyStay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgImilNowThatIveLetMyGuard) },
    { SEARCH(SEARCH_JAR), 100, 0xf5f, FIND_COINS(9) },
    { SEARCH(SEARCH_CHEST), 101, 0xf60, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_GRAVE), 102, 0xf61, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gImilEvents2[] = {
    { EVENT_TALK, 8, 0x82c, MSGNO(MsgImilTheresBeenAnEpidemicGoingAround) },
    { EVENT_TALK, 8, 0x82b, MSGNO(MsgImilGetMiaHurry) },
    { EVENT_TALK, 8, 0x881, EVENT_SCRIPT(Imil_DiscussLighthouse) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgImilThatFountainWaterReallyMadeGrandpa) },
    { EVENT_TALK, 9, 0x82c, EVENT_SCRIPT(Imil_TalkToSickMan) },
    { EVENT_TALK, 9, 0x82b, MSGNO(MsgImilItHurts) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_TalkToSickMan) },
    { EVENT_TALK, 10, IF_FLAG_SET(0x82b), EVENT_SCRIPT(Imil_TalkToSickManAtHome) },
    { EVENT_TALK, 11, 0x881, MSGNO(MsgImilCoughTheEpidemicHasHitAlmost) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgImilItsNiceThatTheFountainCan) },
    { EVENT_TALK, 12, 0x881, EVENT_SCRIPT(Imil_AskAboutWeapons) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunWeaponShop) },
    { EVENT_TALK, 13, 0x881, MSGNO(MsgImilWereClosedSorryIGetA) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunArmorShop) },
    { EVENT_TALK, 15, 0x881, MSGNO(MsgImilIfYouWantToBuyAnything) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RunItemShop) },
    { EVENT_TALK, 14, 0x881, EVENT_SCRIPT(Imil_RunItemShop) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgImilImSoHappyThatMommyIs) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_VisitInn) },
    { EVENT_TALK, 17, 0x881, MSGNO(MsgImilWereTheOnlyInnThatStays) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgImilThankGoodnessTheTownIsBack) },
    { EVENT_TALK, 18, 0x881, EVENT_SCRIPT(Imil_AskAboutRestaurantMenu) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgImilImGladTheBeaconHasBeen) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_VisitSanctum) },
    { EVENT_TALK, 20, 0x881, EVENT_SCRIPT(Imil_AskAboutMia) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgImilIWillBeAKindGirl) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x881, EVENT_SCRIPT(Imil_ReadCaretakerMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgImilGrandpasAllBetterNowThanksTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x409, 0x82c, EVENT_SCRIPT(Imil_TalkToSickMan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x82b, MSGNO(MsgImilOoohIDontFeelSoGood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x881, MSGNO(MsgImilMiaIsAGoodGirlI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgImilNowMiaCanLeaveTheVillage) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x82b), EVENT_SCRIPT(Imil_ReadCountingChildMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x881, MSGNO(MsgImilHerPalmsShineWithABlue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgImilThatBlueLightFromHerHands) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x881, MSGNO(MsgImilWhoAreThesePeopleTheyMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgImilCustomersAreHardToComeBy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x881, MSGNO(MsgImilWhyDoTheyAlwaysComeWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgImilTheCustomersAreBackIThought) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x881, MSGNO(MsgImilIfOnlyWeHadSomeOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgImilMiaSaidItWouldntBeGood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x881, MSGNO(MsgImilIWonderIfTheyAreHeaded) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgImilMyDaddysStillInKolimaIll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x881, MSGNO(MsgImilSoManyCustomersItJustGoes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgImilMiaWillBeLeavingWithYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x881, MSGNO(MsgImilWintersAreToughHereSoIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgImilTheyCameToOurVillageAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x881, MSGNO(MsgImilWeDontGetToEatMany) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgImilINeverToldMiaThatI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0x881, EVENT_SCRIPT(Imil_ReadHealerStudentMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgImilMiasGoingToDefendTheHonor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0x881, EVENT_SCRIPT(Imil_ReadMiaNeighborMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgImilImSorryMiaButICant) },
    { 0xc403, 10, 0x881, EVENT_SCRIPT(Imil_RunWeaponShop) },
    { 0xc403, 11, 0x881, EVENT_SCRIPT(Imil_RunArmorShop) },
    { SEARCH(SEARCH_CHEST), 100, 0xf26, FIND_ITEM(0xb9) },
    { SEARCH(SEARCH_BARREL), 101, 0xf62, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_OVEN), 102, 0xf63, FIND_ITEM(0xc4) },
    { SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgImilItLooksLikePorridgeItIs)) },
    { SEARCH(SEARCH_OVEN), 102, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgImilItsSomeHotStewItWarms)) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { SCENE_EVENTS_END },
};

u16 gImil_ExitCellStep5[] = {
    MAP_CELL_STEP(40, 37, 4, 2, 5), MAP_CELL_STEP(40, 39, 4, 2, 5), MAP_CELL_STEPS_END,
    MAP_CELL_STEP(35, 35, 2, 2, 5), MAP_CELL_STEP(33, 35, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_ExitCellStep4[] = {
    MAP_CELL_STEP(35, 37, 2, 2, 5), MAP_CELL_STEP(33, 37, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_ExitCellStep1[] = {
    MAP_CELL_STEP(35, 43, 2, 2, 5), MAP_CELL_STEP(33, 43, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_ExitCellStep0[] = {
    MAP_CELL_STEP(35, 39, 2, 2, 5), MAP_CELL_STEP(33, 39, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_ExitCellStep2[] = {
    MAP_CELL_STEP(42, 48, 2, 2, 5), MAP_CELL_STEP(40, 48, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gImil_ExitCellStep3[] = {
    MAP_CELL_STEP(42, 46, 2, 2, 5), MAP_CELL_STEP(40, 46, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 *gImil_ExitCellSteps[] = {
    gImil_ExitCellStep0,
    gImil_ExitCellStep1,
    gImil_ExitCellStep2,
    gImil_ExitCellStep3,
    gImil_ExitCellStep4,
    gImil_ExitCellStep5,
};

// Cell position (x, y) for each exit.
s16 gImil_ExitCellPoints[][2] = {
    { 50, 24 }, { 57, 22 }, { 59, 13 }, { 54, 10 }, { 38, 8 }, { 34, 19 },
};
// clang-format on

u16 gImil_CellStepsA[] = {
    MAP_CELL_STEP(1, 125, 1, 2, 10),
    MAP_CELL_STEP(0, 125, 1, 2, 10),
    MAP_CELL_STEPS_END,
};

u16 gImil_CellStepsB[] = {
    MAP_CELL_STEP(1, 125, 1, 2, 10),
    MAP_CELL_STEP(2, 125, 1, 2, 10),
    MAP_CELL_STEPS_END,
};

s32 gImil_ArcOrigin[3];
