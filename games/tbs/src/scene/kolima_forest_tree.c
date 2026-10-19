#include "battle_effect_runtime.h"
#include "game_state.h"
#include "unit/scene/kolima_forest_tree.h"
#include "unit/scene/common/kolima_palette.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
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
#include "resource.h"
#include "field_obj.h"
#include "field_effect.h"
#include "battle_unit.h"
#include "types.h"
#include "call.h"
#include "field_event.h"
#include "engine_call.h"
#include "scene_ids.h"
#include "palette_scene.h"
#include "io_reg.h"
#include "field_scene.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "songs.h"

enum {
  ACTOR_TRET = 8,
  ACTOR_LAUREL = 9,
  ACTOR_TRET_AUXILIARY = 10,
  ACTOR_LAUREL_AUXILIARY = 11,
  ACTOR_TRETS_DARK_SIDE = 14
};

// Forward declarations.
static void KolimaForestTree_RejectHermesWater(void);
static void KolimaForestTree_RestoreTret(void);
static void KolimaForestTree_SpawnRejectionSpark(void);
static void KolimaForestTree_UpdateHealingPalette(void);
static void KolimaForestTree_PlayGesture(s32 auxiliaryActorId, s32 gesture);
static void KolimaForestTree_SpawnOrbitSparks(void);
static void KolimaForestTree_ReplaceInventoryItem(s32 oldItemId, s32 newItemId);

s32 gKolimaForestTreeSparkOrigin[3] __attribute__((section(".bss")));
s32 gKolimaForestTreeSparkFrame __attribute__((section(".bss")));
s32 gKolimaForestTreePlaySparkSound __attribute__((section(".bss")));
s32 gKolimaForestTreePaletteFrame __attribute__((section(".bss")));

// Return the clearing's entrances.
const struct SceneEntrance *KolimaForestTree_GetEntrances(void) {
  return gKolimaForestTreeEntrances;
}

// Return the region at the entrance to Tret's interior.
const struct SceneRegion *KolimaForestTree_GetRegions(void) {
  return gKolimaForestTreeRegions;
}

// Return the connections to the bridge and Tret's interior.
const u32 *KolimaForestTree_GetExits(void) { return gKolimaForestTreeExits; }

// Return Tret, Laurel and their auxiliary actors.
const struct ScenePlacement *KolimaForestTree_GetPlacements(void) {
  return gKolimaForestTreePlacements;
}

// Take the touched exit after completing the leader's current movement.
UNUSED static void KolimaForestTree_ExitTouchedRegion(void) {
  struct EventWork *scene;

  scene = gEventWork;
  Engine_EventBegin();
  Engine_ActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 0);
  Engine_EventRequestExit(scene->touchedTrigger);
  Engine_EventEnd();
}

// Return the trees' dialogue, Mind Read and item-use events.
const struct SceneEvent *KolimaForestTree_GetEvents(void) {
  return gKolimaForestTreeEvents;
}

// Introduce Laurel or hear her warning while Tret is still dying.
static void KolimaForestTree_TalkToLaurel(void) {
  Engine_EventBegin();
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 1);
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_EventSetMessage(MsgKolimaForestKolimaAlive);
    Engine_EventShowMessage(9, 0);
  } else if (GameFlag_IsSet(FLAG_MET_LAUREL) != 0) {
    Engine_EventSetMessage(MsgKolimaLeaveBeforeForest);
    Engine_EventShowMessage(9, 0);
  } else {
    Engine_EventSetMessage(MsgKolimaWho);
    Engine_EventShowMessageAndWait(9, 0, 20);
    KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 0);
    Engine_EventWait(60);
    KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 1);
    Engine_EventShowMessageAndWait(9, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(40);
    Engine_EventShowMessage(9, 0);
    KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 0);
    Engine_EventWait(80);
    Engine_EventShowMessageAndWait(9, 0, 20);
    KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 1);
    Engine_EventShowMessageAndWait(9, 0, 20);
    Engine_GameFlagSet(FLAG_MET_LAUREL);
  }
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 0);
  Engine_EventEnd();
}

// Talk to Tret and offer the item prompt when Hermes' Water is available.
static void KolimaForestTree_TalkToTret(void) {
  s32 waterOwner;

  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 1);
    Engine_EventSetMessage(MsgKolimaNowHaveSuchPowerAxe);
    Event_ShowMessage(8, 0);
    KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 0);
  } else {
    if (GameFlag_IsSet(FLAG_TRET_DEFEATED) != 0) {
      KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 1);
      Engine_EventSetMessage(MsgKolimaSilence2);
      Event_ShowMessage(8, 0);
      KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 0);
      waterOwner = PartyInventory_FindOwner(ITEM_HERMES_WATER);
      if (waterOwner != -1) {
        u16 *stateSlot = &gEventWork->unk172;
        u16 requestPrompt = 1;

        *stateSlot = requestPrompt;
      }
    } else {
      Engine_EventSetMessage(MsgKolimaSilence);
      Event_ShowMessage(8, 0);
      ColorBuffer_ApplyTarget(0x406218, 1);
      Engine_ColorBufferInterpolate(20);
      Engine_TaskWait(40);
      Event_ShowMessageAndWait(0x200e, 0, 10);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Event_ShowMessage(0x200e, 0);
      ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
      Engine_ColorBufferInterpolate(20);
      Engine_TaskWait(40);
    }
  }
  Engine_EventEnd();
}

// Read Laurel's thoughts about the healing water or the danger beyond the
// river.
static void KolimaForestTree_ReadLaurelThoughts(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_EventSetMessage(MsgKolimaMustHorribleBeyondRiverAm);
  } else {
    Engine_EventSetMessage(MsgKolimaHealingWatersMercuryLighthouseMight);
  }
  Engine_EventShowMessage(9, 0);
  Engine_EventEnd();
}

// Read Tret's regret before healing, or his concern for the forest afterward.
static void KolimaForestTree_ReadTretThoughts(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_EventSetMessage(MsgKolimaKnowCannotStopButPlease);
  } else {
    Engine_EventSetMessage(MsgKolimaPeopleKolimaForgiveMe);
  }
  Engine_EventShowMessage(8, 0);
  Engine_EventEnd();
}

// Heal Tret only after his hostile heart has been defeated.
static void KolimaForestTree_UseHermesWater(void) {
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  if (Engine_GameFlagIsSet(FLAG_TRET_DEFEATED) == 0) {
    KolimaForestTree_RejectHermesWater();
  } else {
    KolimaForestTree_RestoreTret();
  }
  Engine_EventEnd();
}

// Show Tret's hostile heart repelling the leader's attempt to help.
static void KolimaForestTree_RejectHermesWater(void) {
  struct FieldActor *leader;

  // The hostile heart lifts the leader away before the water can heal Tret.
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(40);
  Event_PlayCue(MUS_017);
  gKolimaForestTreePlaySparkSound = 1;
  FieldScheduler_AddCallback(KolimaForestTree_SpawnRejectionSpark,
                             TASK_PRIORITY_SCENE);
  Engine_TaskWait(30);
  gKolimaForestTreePlaySparkSound = 0;
  Camera_MoveTo(PIXELS(328), -1, PIXELS(235), 1);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(2));
  Event_PlayCue(SE_133);
  leader->velocityY = PIXELS(5);
  leader->gravity = PIXELS(1) / 4;
  leader->bounce = 5 * Q16_ONE / 8;
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 335, 258);
  while (leader->velocityY >= 0) {
    Engine_TaskWait(1);
  }
  do {
    Engine_TaskWait(1);
  } while (leader->velocityY <= 0);
  Event_PlayCue(SE_161);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
  Engine_EventWait(120);
  Engine_TaskRemoveCallback(KolimaForestTree_SpawnRejectionSpark);
  Engine_TaskWait(40);
  leader->bounce = Q16_ONE / 4;
  {
    struct FieldActor *leader = Actor_Get(ACTOR_PARTY_LEADER);
    leader->steeringFlags = ACTOR_FACES_MOTION | leader->steeringFlags;
  }
  Engine_EventWait(80);
  // Hear the heart's refusal, then restore the leader's ordinary motion.
  Engine_EventSetMessage(MsgKolimaControlTretsHeartShallNot);
  Event_ShowMessageAndWait(0x200e, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(0x200e, 0);
  Audio_PlayCueFromEventWork();
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(40);
  {
    s32 facing = FACING_NORTH;

    leader->facing = facing;
  }
  leader->gravity = PIXELS(1);
  leader->bounce = Q16_ONE / 4;
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(40);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
}

// Give all four companions the same nod, waiting on Ivan's animation.
static __inline__ void KolimaForestTree_NodTogether(void) {
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
}

// Turn the companions together and pause after the final facing change.
static __inline__ void KolimaForestTree_FaceParty(s32 facing, s32 waitFrames) {
  Actor_FaceDirection(ACTOR_PARTY_LEADER, facing, 0);
  Actor_FaceDirection(GARET, facing, 0);
  Actor_FaceDirection(MIA, facing, 0);
  Actor_FaceDirection(IVAN, facing, waitFrames);
}

// Heal Tret, lift Kolima's curse and hear the trees' warning.
static void KolimaForestTree_RestoreTret(void) {
  s32 includeMia;
  struct FieldActor *leader;
  s32 *paletteFrame;
  s32 flash;
  void (*spawnOrbitSparks)(void);
  const s32 *finalActions;

  // Gather the companions while Hermes' Water seeps into Tret.
  // Flag 3: Mia is in the party.
  includeMia = GameFlag_IsSet(3);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 212);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Event_PlayCue(MUS_017);
  Engine_MessageShowCentered(MsgKolimaWaterHermesSeepedIntoTret, 1);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetActionScript(GARET, gKolimaForestTree_GaretApproachActions);
  Actor_SetActionScript(IVAN, gKolimaForestTree_IvanApproachActions);
  if (includeMia != 0) {
    Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetActionScript(MIA, gKolimaForestTree_MiaApproachActions);
  }
  Object_RefreshSelectorById(2);
  Engine_EventWait(40);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(32);
  Engine_TaskWait(40);
  // Carry the healing across the forest as its healthy colours return.
  paletteFrame = &gKolimaForestTreePaletteFrame;
  *paletteFrame = 0;
  FieldScheduler_AddCallback(KolimaForestTree_UpdateHealingPalette,
                             TASK_PRIORITY_SCENE);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 20);
  Camera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  Camera_MoveTo(PIXELS(256), -1, PIXELS(254), 1);
  Engine_CameraWaitForMove();
  Event_PlayCue(SE_246);
  Engine_EventWait(40);
  Actor_FaceDirection(IVAN, FACING_SOUTHEAST, 20);
  Camera_MoveTo(PIXELS(413), -1, PIXELS(261), 1);
  Engine_CameraWaitForMove();
  Event_PlayCue(SE_246);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH, 20);
  Camera_MoveTo(PIXELS(326), -1, PIXELS(384), 1);
  Engine_CameraWaitForMove();
  Event_PlayCue(SE_246);
  if (*paletteFrame != 24) {
    do {
      Engine_TaskWait(1);
    } while (*paletteFrame != 24);
  }
  Engine_TaskRemoveCallback(KolimaForestTree_UpdateHealingPalette);
  Engine_TaskWait(10);
  // Flash the two saved palettes before settling on the healthy forest.
  flash = 0;
  do {
    KolimaPalette_Restore(0);
    Engine_ColorBufferInterpolate(6);
    Engine_TaskWait(6);
    KolimaPalette_Restore(1);
    Engine_ColorBufferInterpolate(6);
    flash = (flash + 1);
    Engine_TaskWait(6);
  } while ((u32)flash <= 3);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(40);
  Engine_TaskWait(80);
  Camera_MoveTo(PIXELS(328), PIXELS(8), PIXELS(212), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 1);
  Engine_EventWait(40);
  Event_PlayCue(MUS_007);
  // Tret and Laurel discover that their powers have returned.
  Engine_EventSetMessage(MsgKolimaFeelGreatPowerSpreadingThrough);
  Event_ShowMessage(8, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  KolimaForestTree_FaceParty(FACING_NORTH, 20);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 2);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 3);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 1);
  Engine_EventWait(20);
  Event_ShowMessage(8, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 40);
  Camera_MoveTo(PIXELS(234), 0, PIXELS(232), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 1);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4009, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_SOUTHWEST, 20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 3);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 2);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 3);
  Event_ShowMessageAndWait(0x4009, 0, 10);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 0);
  Engine_EventWait(20);
  Event_ShowMessage(0x8008, 0);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 1);
  Engine_EventWait(20);
  Event_OpenMessage(0x8008, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Event_ShowMessage(0x4009, 0);
    Event_ShowMessage(0x8008, 0);
  } else {
    gEventWork->message += 2;
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 3, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 0);
    Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 3, 40);
    Engine_ActorSetAnimation(GARET, 4);
    Event_ShowMessage(GARET, 0);
    if (includeMia != 0) {
      Engine_ActorRunRepeatedMotion(MIA, 2);
      Event_ShowMessage(MIA, 0);
    } else {
      gEventWork->message += 1;
    }
    Engine_ActorSetAnimationAndWait(IVAN, 3);
    Event_ShowMessage(IVAN, 0);
    Event_ShowMessage(0x4009, 0);
    Event_ShowMessage(0x8008, 0);
  }
  KolimaForestTree_NodTogether();
  Camera_MoveTo(PIXELS(328), PIXELS(8), PIXELS(212), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 0);
  Engine_EventWait(20);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(40);
  Engine_EventWait(60);
  gKolimaForestTreeSparkFrame = 0;
  // Tret sends out the power that frees Kolima's villagers.
  gKolimaForestTreeSparkOrigin[0] = PIXELS(328);
  gKolimaForestTreeSparkOrigin[1] = PIXELS(48);
  spawnOrbitSparks = KolimaForestTree_SpawnOrbitSparks;
  gKolimaForestTreeSparkOrigin[2] = PIXELS(205);
  FieldScheduler_AddCallback(spawnOrbitSparks, TASK_PRIORITY_SCENE);
  Engine_EventWait(100);
  Engine_TaskRemoveCallback(spawnOrbitSparks);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(60);
  Engine_EventWait(100);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(20);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 1);
  Engine_EventWait(10);
  Engine_EventSetMessage(MsgKolimaShouldDoPeopleKolimaCursed);
  Event_ShowMessage(0x8008, 0);
  KolimaForestTree_NodTogether();
  Camera_MoveTo(PIXELS(234), 0, PIXELS(232), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Event_ShowMessage(0x4009, 0);
  Event_ShowMessageAndWait(0x8008, 0, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 10);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  } else {
    Engine_ActorSetAnimationAndWait(GARET, 4);
    gEventWork->message += 1;
  }
  Event_ShowMessage(GARET, 0);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 4);
  Engine_EventWait(20);
  // Explain the curse and how the falling Psynergy Stones changed the forest.
  Engine_EventSetMessage(MsgKolimaWasIndeedAngryPeopleHad);
  Event_ShowMessage(0x8008, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Event_ShowMessage(0x8008, 0);
  KolimaForestTree_NodTogether();
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x8008, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 0);
  Event_ShowMessageAndWait(0x4009, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 3);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4009, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 2);
  Engine_EventWait(20);
  Event_ShowMessage(0x8008, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 80);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 5);
  Engine_EventWait(60);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4009, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 5);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4008, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_FaceDirection(GARET, FACING_WEST, 10);
  Event_ShowMessage(GARET, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 20);
  Event_ShowMessage(0x8002, 0);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4009, 0, 20);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Event_ShowMessageAndWait(GARET, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 1);
  Event_ShowMessageAndWait(0x8008, 0, 10);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 2);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 3);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 0);
  Engine_EventWait(20);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(40);
  Engine_EventWait(60);
  gKolimaForestTreeSparkFrame = 0;
  // Laurel looks beyond the river and warns of the forest to the south.
  gKolimaForestTreeSparkOrigin[0] = PIXELS(136);
  gKolimaForestTreeSparkOrigin[1] = PIXELS(20);
  spawnOrbitSparks = KolimaForestTree_SpawnOrbitSparks;
  gKolimaForestTreeSparkOrigin[2] = PIXELS(258);
  FieldScheduler_AddCallback(spawnOrbitSparks, TASK_PRIORITY_SCENE);
  Engine_EventWait(100);
  KolimaForestTree_FaceParty(FACING_SOUTHWEST, 40);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 20);
  Event_ShowMessageAndWait(0x8002, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 4);
  Engine_EventWait(20);
  Event_ShowMessage(0x8008, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 60);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Event_ShowMessageAndWait(0x8002, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 20);
  Event_ShowMessageAndWait(0x8008, 0, 10);
  KolimaForestTree_NodTogether();
  Engine_EventWait(10);
  KolimaForestTree_FaceParty(FACING_SOUTHWEST, 120);
  Engine_TaskRemoveCallback(spawnOrbitSparks);
  Engine_EventWait(60);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(40);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x8008, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 3);
  Event_ShowMessage(0x4009, 0);
  Event_ShowMessage(0x8008, 0);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4009, 0, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 1);
  Event_OpenMessage(0x8008, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message += 1;
  }
  Engine_EventWait(10);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 2);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 3);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x8008, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 10);
  KolimaForestTree_NodTogether();
  Event_PlayCue(MUS_017);
  // Rejoin the party and exchange the used water for its empty bottle.
  finalActions = gKolimaForestTree_RejoinPartyActions;
  Actor_SetActionScript(GARET, finalActions);
  if (includeMia != 0) {
    Actor_SetActionScript(MIA, finalActions);
  }
  Actor_RunActionScriptAndWait(IVAN, finalActions);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 4);
  KolimaForestTree_PlayGesture(ACTOR_TRET_AUXILIARY, 4);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgKolimaOweGreatDebtHaveSaved);
  Event_ShowMessage(0x8008, 0);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 4);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_AUXILIARY, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x4009, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  GameFlag_Set(FLAG_KOLIMA_CURSE_LIFTED);
  Event_PlayCue(MUS_001);
  KolimaForestTree_ReplaceInventoryItem(ITEM_HERMES_WATER, ITEM_EMPTY_BOTTLE);
}

// Restore the trees' faces, collision and the dying forest's palette.
s32 KolimaForestTree_Initialize(void) {
  struct FieldActor *tretAuxiliary;
  struct FieldActor *tretsDarkSide;
  struct FieldActor *laurelAuxiliary;

  tretAuxiliary = Object_GetById(ACTOR_TRET_AUXILIARY);
  tretsDarkSide = Object_GetById(ACTOR_TRETS_DARK_SIDE);
  laurelAuxiliary = Object_GetById(ACTOR_LAUREL_AUXILIARY);
  Engine_TaskWait(1);
  Engine_ActorSetChildValue(ACTOR_TRETS_DARK_SIDE, 15);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  gGameState.retreatScene = (s32)&SceneId_KolimaForestBridge;
  gGameState.retreatEntrance = 4;
  if (!Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED))
    SceneEffect_AdjustPaletteColors(3);
  Object_GetById(ACTOR_TRET)->radius = 6;
  Object_GetById(ACTOR_LAUREL)->radius = 6;
  Object_GetById(12)->radius = 6;
  Object_GetById(13)->radius = 6;
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_TRETS_DARK_SIDE), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_TRET_AUXILIARY), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_LAUREL_AUXILIARY), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_TRET), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_LAUREL), 0);
  Engine_ActorSetSpritePriority(ACTOR_TRET, 2);
  Engine_ActorSetSpritePriority(ACTOR_TRETS_DARK_SIDE, 2);
  Engine_ActorSetSpritePriority(ACTOR_LAUREL, 2);
  tretAuxiliary->motionFlags = 0;
  tretAuxiliary->y.fixed = PIXELS(28);
  laurelAuxiliary->motionFlags = 0;
  laurelAuxiliary->y.fixed = PIXELS(28);
  tretsDarkSide->motionFlags = 0;
  tretsDarkSide->y.fixed = PIXELS(28);
  Engine_ActorSetAnimation(ACTOR_LAUREL, 3);
  Engine_ActorSetAnimation(ACTOR_TRET, 3);
  Object_GetById(ACTOR_TRET)->collisionFlags |= 8;
  Object_GetById(ACTOR_LAUREL)->collisionFlags |= 8;
  Object_GetById(ACTOR_TRET_AUXILIARY)->collisionFlags |= 8;
  Object_GetById(ACTOR_LAUREL_AUXILIARY)->collisionFlags |= 8;
  Object_GetById(ACTOR_TRETS_DARK_SIDE)->collisionFlags |= 8;
  return 0;
}

// Shared source is included here in ROM order.
#include "common/kolima_grow.h"

// Spawn the growing sparks that repel the party leader.
static void KolimaForestTree_SpawnRejectionSpark(void) {
  struct FieldActor *effect;
  struct AnimationObject *sprite;
  s32 phase;
  s32 effectFlags;
  s32 spawnX = PIXELS(326);
  s32 spawnY = PIXELS(32);
  s32 spawnZ = PIXELS(192);
  s32 targetX = PIXELS(326);
  s32 targetZ = PIXELS(240);

  phase = gFrameCount & 3;
  if (phase != 0)
    return;
  if (gKolimaForestTreePlaySparkSound != 0)
    Engine_AudioPlayCue(SE_200);
  effect = Engine_ObjectCreate(26, spawnX, spawnY, spawnZ);
  if (effect == 0)
    return;
  sprite = effect->sprite;
  sprite->flags = phase;
  effectFlags = 0xff & ~ACTOR_PRIORITY_AUTOMATIC;
  effectFlags &= effect->priorityFlags;
  effect->priorityFlags = effectFlags;
  sprite->image.priority = 1;
  effect->scaleX = Q16_ONE / 10;
  effect->speed = PIXELS(4);
  effect->acceleration = PIXELS(4);
  effect->motionFlags = phase;
  Object_SetMode(effect, 2);
  Engine_ObjectSetPosition(effect, targetX, 0, targetZ);
  Engine_ObjectSetScript(effect, gKolimaForestTreeEffectScript);
}

// Alternate the saved healthy and withered palettes over thirty frames.
static void KolimaForestTree_UpdateHealingPalette(void) {
  s32 step = gKolimaForestTreePaletteFrame;

  if (step == 0) {
    KolimaPalette_Restore(0);
    Engine_ColorBufferInterpolate(20);
  } else if (step == 20) {
    KolimaPalette_Restore(1);
    Engine_ColorBufferInterpolate(8);
  }
  step = gKolimaForestTreePaletteFrame + 1;
  gKolimaForestTreePaletteFrame = step;
  if (step == 30) {
    gKolimaForestTreePaletteFrame = 0;
  }
}

// Alternate two facial frames twice, ending on the first frame.
static __inline__ void KolimaForestTree_AlternateFaceFrames(s32 actorId,
                                                            s32 firstFrame,
                                                            s32 secondFrame) {
  Engine_ActorSetAnimation(actorId, firstFrame);
  Engine_TaskWait(6);
  Engine_ActorSetAnimation(actorId, secondFrame);
  Engine_TaskWait(6);
  Engine_ActorSetAnimation(actorId, firstFrame);
  Engine_TaskWait(6);
  Engine_ActorSetAnimation(actorId, secondFrame);
  Engine_TaskWait(6);
  Engine_ActorSetAnimation(actorId, firstFrame);
}

// Choose Tret or Laurel's facial sequence through their auxiliary actor.
static void KolimaForestTree_PlayGesture(s32 auxiliaryActorId, s32 gesture) {
  if (auxiliaryActorId == ACTOR_TRET_AUXILIARY) {
    switch (gesture) {
    case 0:
      Engine_ActorSetAnimation(ACTOR_TRET, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_TRET, 3);
      break;
    case 1:
      Engine_ActorSetAnimation(ACTOR_TRET, 1);
      break;
    case 2:
      Engine_ActorSetAnimation(ACTOR_TRET, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_TRET, 5);
      break;
    case 3:
      Engine_ActorSetAnimation(ACTOR_TRET, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_TRET, 4);
      break;
    case 4:
      KolimaForestTree_AlternateFaceFrames(ACTOR_TRET, 1, 3);
      break;
    case 5:
      Engine_ActorSetAnimation(ACTOR_TRET, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_TRET, 2);
      break;
    case 6:
      Engine_ActorSetAnimation(ACTOR_TRET, 6);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_TRET, 8);
      break;
    case 8:
      Engine_ActorSetAnimation(ACTOR_TRET, 6);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_TRET, 9);
      break;
    case 9:
      Engine_ActorSetAnimation(ACTOR_TRET, 6);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_TRET, 10);
      break;
    case 10:
      KolimaForestTree_AlternateFaceFrames(ACTOR_TRET, 6, 8);
      break;
    case 7:
    case 11:
      Engine_ActorSetAnimation(ACTOR_TRET, 6);
      break;
    }
  } else {
    switch (gesture) {
    case 0:
      Engine_ActorSetAnimation(ACTOR_LAUREL, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_LAUREL, 3);
      break;
    case 1:
      Engine_ActorSetAnimation(ACTOR_LAUREL, 1);
      break;
    case 2:
      Engine_ActorSetAnimation(ACTOR_LAUREL, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_LAUREL, 5);
      break;
    case 3:
      Engine_ActorSetAnimation(ACTOR_LAUREL, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_LAUREL, 4);
      break;
    case 4:
      KolimaForestTree_AlternateFaceFrames(ACTOR_LAUREL, 1, 3);
      break;
    case 5:
      Engine_ActorSetAnimation(ACTOR_LAUREL, 1);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_LAUREL, 2);
      break;
    }
  }
  Engine_TaskWait(12);
}

// Expand a spark along its spiral for 120 frames, then release its cache slot.
static void KolimaForestTree_UpdateOrbitSpark(union FieldObject *fieldObject) {
  struct FieldActor *spark = &fieldObject->actor;
  s32 position[3];
  s32 age = spark->timer;
  s32 heading;

  if (age <= 119) {
    SET_VECTOR(position, spark->targetX, spark->targetY, spark->targetZ);
    heading = (s16)spark->work;
    Vector_AddPolarOffset(age * Q16_ONE, age * 768 + heading, position);
    spark->x.fixed = position[0];
    spark->y.fixed = position[1];
    spark->z.fixed = position[2];
    spark->scaleX += Q16_ONE / 200;
    spark->scaleY += Q16_ONE / 200;
    spark->timer++;
  } else {
    Resource_ResetEntry(spark->sprite->slot);
    Engine_ObjectDispatchRelease(spark);
  }
}

// Emit six-spark rings at ten-frame intervals during each burst.
static void KolimaForestTree_SpawnOrbitSparks(void) {
  struct FieldActor *spark;
  struct AnimationObject *previousSprite;
  u32 sparkIndex;

  previousSprite = NULL;
  switch (gKolimaForestTreeSparkFrame) {
  case 0:
  case 10:
  case 20:
  case 30:
  case 40:
    Engine_AudioPlayCue(SE_220);
    for (sparkIndex = 0; sparkIndex <= 5; sparkIndex++) {
      spark = Engine_ObjectCreate(0x11d, gKolimaForestTreeSparkOrigin[0],
                                  gKolimaForestTreeSparkOrigin[1],
                                  gKolimaForestTreeSparkOrigin[2]);
      if (spark != 0) {
        previousSprite = Sprite_ShareVramCache(spark->sprite, previousSprite);
        spark->motionFlags = 0;
        spark->sprite->image.priority = 1;
        Engine_ActorSetSpriteFlags(spark, 0);
        Object_SetMode(spark, 1);
        spark->timer = 0;
        spark->work = (sparkIndex * 60 * Q16_ONE) / 360;
        spark->targetX = gKolimaForestTreeSparkOrigin[0];
        spark->targetY = gKolimaForestTreeSparkOrigin[1];
        spark->targetZ = gKolimaForestTreeSparkOrigin[2];
        spark->speed = PIXELS(8) / 5;
        spark->update = KolimaForestTree_UpdateOrbitSpark;
      }
    }
    break;
  }
  if (++gKolimaForestTreeSparkFrame > 120) {
    gKolimaForestTreeSparkFrame = 0;
  }
}

// Replace the used water with its empty bottle in the same inventory slot.
static void KolimaForestTree_ReplaceInventoryItem(s32 oldItemId,
                                                  s32 newItemId) {
  s32 owner = PartyInventory_FindOwner(oldItemId);

  if (owner != -1) {
    s32 inventorySlot = Inventory_Find(owner, oldItemId);

    if (inventorySlot != -1) {
      UnitGet(owner)->inventory[inventorySlot] = newItemId;
    }
  }
}

// Shared source is included here in ROM order.
#include "common/kolima_palette.h"

s32 gKolimaForestTree_GaretApproachActions[] = {
    SC_ARG(2), SC_MOVE(PIXELS(312), 0, PIXELS(230)), SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_NORTH),      SC_WAIT(1),
    SC_END,
};

s32 gKolimaForestTree_IvanApproachActions[] = {
    SC_ARG(2), SC_MOVE(PIXELS(346), 0, PIXELS(224)), SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_NORTH),      SC_WAIT(1),
    SC_END,
};

s32 gKolimaForestTree_MiaApproachActions[] = {
    SC_ARG(2), SC_MOVE(PIXELS(333), 0, PIXELS(240)), SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_NORTH),      SC_WAIT(1),
    SC_END,
};

s32 gKolimaForestTree_RejoinPartyActions[] = {
    SC_ARG(2),  SC_MOVE(PIXELS(328), 0, PIXELS(212)),
    SC_SYNC,    SC_POS(0, 0, 0),
    SC_WAIT(1), SC_END,
};

struct SceneEntrance gKolimaForestTreeEntrances[] = {
    {0, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, 0, 0, 512, 448, 0},
    {1, CONDITION_ALWAYS, 136, 0, 424, FACING_NORTH, 0, 0, 0, 512, 448, 0},
    {2, CONDITION_ALWAYS, 328, 16, 200, FACING_SOUTH, 0, 0, 0, 512, 448, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct SceneRegion gKolimaForestTreeRegions[] = {
    {320, 88, 176, 336, 104, 192, -1, 2},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 gKolimaForestTreeExits[] = {
    SCENE_EXITS(44),
    SCENE_EXIT(1, 43, 2),
    SCENE_EXIT(2, 45, 1),
    SCENE_EXITS_END,
};

struct ScenePlacement gKolimaForestTreePlacements[] = {
    { 0x47, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x38, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(270), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(136), 0, PIXELS(270), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(327), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        PIXELS(136), 0, PIXELS(272), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x37, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gKolimaForestTreeEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    // Hermes' Water (item 0xb8) used on Tret.
    { 0xb814, 12, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(KolimaForestTree_UseHermesWater) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(KolimaForestTree_TalkToLaurel) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(KolimaForestTree_TalkToTret) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, EVENT_SCRIPT(KolimaForestTree_ReadLaurelThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40c, FLAG_TRET_DEFEATED, EVENT_SCRIPT(KolimaForestTree_TalkToTret) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, EVENT_SCRIPT(KolimaForestTree_ReadTretThoughts) },
    { SCENE_EVENTS_END },
};
// clang-format on

s32 gKolimaForestTreeEffectScript[] = {
    SC_ADD(SW_SCALE_X, Q16_ONE / 10),
    SC_CALL(SceneEffect_AdvanceAngleUntilIdle),
    SC_LOOP(SC_FOREVER, SC_TOP),
};
