/// @file
/// Lord McCoy's palace in Bilibin. At the first audience McCoy asks the party
/// to lift the curse on Kolima, then turns them away as too young; once the
/// curse is lifted, his guards bring them back and he rewards them with one of
/// four treasures.
///
/// Scene flags: FLAG_SCENE_TEMP_00, the escort has opened the front door;
/// FLAG_SCENE_TEMP_02, the party is choosing its reward; FLAG_SCENE_TEMP_10 to
/// _13, the chest it chose. The flags from 0x300 are reused from place to
/// place: 0x302 and 0x303, the admirer has spoken or been read once; 0x321,
/// McCoy has turned the party away; 0x322, the reward is still to choose;
/// 0x333, the reward visit is over. McCoy speaks as SPEAKER_PORTRAIT_RIGHT |
/// ACTOR_LORD_MCCOY, a speaker bit field_event.h does not name yet.

#include "game_state.h"
#include "scene_ids.h"
#include "unit/scene/mccoy_palace.h"
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
#include "flags.h"
#include "character_ids.h"
#include "types.h"
#include "call.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"
#include "facing_object.h"
#include "fixed_math.h"
#include "songs.h"

/// The palace's people and the reward room's chests.
enum {
  ACTOR_KOLIMA_ASKER = 10,
  ACTOR_STARTLED_RESIDENT = 14,
  ACTOR_PARTY_ADMIRER = 15,
  ACTOR_KOLIMA_WATCHER = 16,
  ACTOR_REWARD_KEEPER = 17,
  ACTOR_LORD_MCCOY = 18,
  ACTOR_ESCORT = 19,
  ACTOR_BARRICADE_KEY = 20,
  ACTOR_VIAL_CHEST = 21,
  ACTOR_POTION_CHEST = 22,
  ACTOR_PSY_CRYSTAL_CHEST = 23,
  ACTOR_WATER_OF_LIFE_CHEST = 24
};

// Forward declarations.
static void McCoyPalace_ClaimReward(s32 chestActorId, s32 itemId,
                                    s32 chestFlag);
static void Event_AudienceWithMcCoy(void);
static void Event_EscortHintsAtBarricade(void);
static void Event_McCoyRewardsParty(void);
static void Event_PresentRewardChests(void);

/// The talkFacing bytes of placement rows 6 to 9: actors 14 to 17.
struct PalacePlacementTalkFacingView {
  u8 unk000[166];
  u8 startledResident;
  u8 unk167[23];
  u8 admirer;
  u8 unk191[23];
  u8 kolimaWatcher;
  u8 unk215[23];
  u8 rewardKeeper;
};

// The shared facing callback precedes the scene's entry points.
#include "common/object_facing.h"

/// Where the party can enter the palace.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gMcCoyPalaceEntrances;
}

/// The palace has no regions.
const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

/// Where each door and stair leads.
const u32 *Scene_GetExits(void) { return gMcCoyPalaceExits; }

/// The palace's people. Once the reward is taken, the startled resident,
/// the admirer, the Kolima watcher and the reward keeper turn differently
/// when talked to.
const struct ScenePlacement *Scene_GetPlacements(void) {
  struct ScenePlacement *placements;

  if (gGameState.scene == (s32)&SceneId_McCoyPalace) {
    placements = gMcCoyPalacePlacements;
    FieldScene_PrepareActors(placements);

    if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
      /* FAKEMATCH: retain the existing byte-facing view of placement rows
       * 6..9. Direct talkFacing fields change native address decomposition
       * from base+166 to base+144/member+22; a true member cursor and a
       * sizeof/offsetof byte span each remove four native bytes. */
      struct PalacePlacementTalkFacingView *facings =
          (struct PalacePlacementTalkFacingView *)placements;

      facings->startledResident = TALK_KEEP_FACING;
      facings->admirer = TALK_FACE_PARTY;
      facings->kolimaWatcher = 3;
      facings->rewardKeeper = TALK_FACE_PARTY_AND_BACK;
    }

    return placements;
  }
  return gMcCoyPalacePlacementsOther;
}

/// What the party can talk to and search in the palace.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_McCoyPalace) {
    return gMcCoyPalaceEvents;
  }
  return gMcCoyPalaceEventsOther;
}

/// Asks about going to Kolima. "Oh, are you willing to go to Kolima Forest?"
static void Event_AskAboutGoingToKolima(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinYouWillingGoKolimaForest);
  Engine_EventAskYesNo(ACTOR_KOLIMA_ASKER, 0);
  Engine_EventEnd();
}

/// The resident in a nightgown screams, then turns to the party. "AAAAH!!!"
static void Event_TalkToStartledResident(void) {
  Engine_EventBegin();
  Actor_ShowEmote(ACTOR_STARTLED_RESIDENT, EMOTE_IN_FRONT | 2, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_STARTLED_RESIDENT, 2);
  Engine_EventWait(40);
  Engine_EventSetMessage(MsgBilibinAaaah);
  Event_ShowMessageAndWait(ACTOR_STARTLED_RESIDENT, 0, 20);
  Actor_FaceActor(ACTOR_STARTLED_RESIDENT, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_STARTLED_RESIDENT, 0, 10);
  Actor_FaceDirection(ACTOR_STARTLED_RESIDENT, FACING_NORTHWEST + FACING_STEP,
                      10);
  Engine_EventEnd();
}

/// The admirer praises the party the first time, then warns of the holy tree.
/// "You are a fine warrior." After, "It's very reckless for such young
/// warriors to face the holy tree..."
static void Event_TalkToAdmirer(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinFineWarrior);
  if (GameFlag_IsSet(0x302) != 0) {
    Engine_EventSetMessage(MsgBilibinItsVeryRecklessForSuch);
  }
  Event_ShowMessage(ACTOR_PARTY_ADMIRER, 0);
  GameFlag_Set(0x302);
  Engine_EventEnd();
}

/// The watcher saw the party go to Kolima. "I was watching from here. After
/// all that fuss, you still went to Kolima?"
static void Event_TalkToKolimaWatcher(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinWasWatchingFromHereAfter);
  Engine_EventAskYesNo(ACTOR_KOLIMA_WATCHER, 0);
  Engine_EventEnd();
}

/// The keeper of McCoy's treasures: while the party chooses, only one item;
/// after, the reward was the greatest; before, Bilibin's great treasures
/// go to whoever ends the curse.
static void Event_TalkToRewardKeeper(void) {
  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02) != 0) {
    Engine_EventSetMessage(MsgBilibinMayChooseOnlyOneItem);
  } else if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    Engine_EventSetMessage(MsgBilibinRewardReceivedWasIndeedGreatest);
  } else {
    Engine_EventSetMessage(MsgBilibinDoThinkTheseBilibinsGreat);
    if (Engine_GameFlagIsSet(0x84d) != 0) {
      gEventWork->message++;
    }
  }

  Engine_EventShowMessage(ACTOR_REWARD_KEEPER, 0);
  Engine_EventEnd();
}

/// Mind Read on the admirer shows two thoughts. "Sometimes, we need children
/// to remind us of our own responsibilities." Then "You looked very courageous
/// walking toward Kolima."
static void Event_ReadAdmirersThoughts(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinSometimesWeNeedChildrenRemind);
  if (GameFlag_IsSet(0x303) != 0) {
    Engine_EventSetMessage(MsgBilibinLookedVeryCourageousWalkingToward);
  }
  Event_ShowMessage(ACTOR_PARTY_ADMIRER, 0);
  GameFlag_Set(0x303);
  Engine_EventEnd();
}

/// Mind Read on the keeper of McCoy's treasures.
static void Event_ReadRewardKeepersThoughts(void) {
  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02) != 0) {
    Engine_EventSetMessage(MsgBilibinNonethelessIfYourLuckSour);
  } else if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    Engine_EventSetMessage(MsgBilibinGoodTreasureIfGetTurned);
  } else {
    Engine_EventSetMessage(MsgBilibinWeReallyGivingOurTreasure);
    if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
      Engine_EventSetMessage(MsgBilibinGotPrettyNiceRewardBut);
    }
  }

  Engine_EventShowMessage(ACTOR_REWARD_KEEPER, 0);
  Engine_EventEnd();
}

/// Walking through one of the palace's doors: it opens with a sound, the
/// leader walks through and the next room loads. The front door makes no
/// sound once the escort has opened it.
static void Event_WalkThroughDoor(void) {
  struct EventWork *work;
  s32 hideDoorLayers;

  work = gEventWork;
  hideDoorLayers = 0;
  if (work->touchedTrigger == 9) {
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00) == 0) {
      Engine_AudioPlayCue(SE_188);
      hideDoorLayers = 1;
    }
  } else {
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    hideDoorLayers = 1;
  }
  if (hideDoorLayers != 0) {
    Map_ClearLayerEntryFlag(1);
    Map_ClearLayerEntryFlag(2);
  }
  Engine_EventBegin();
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (work->touchedTrigger == 9)
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  else
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  Engine_EventWait(16);
  Engine_EventRequestExit(work->touchedTrigger);
  Engine_EventEnd();
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
}

/// The chest is empty. "Isaac checked the chest... but it was empty."
static void McCoyPalace_ShowChestEmpty(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgBilibinIsaacCheckedChestButWas, 1);
  Engine_EventEnd();
}

/// The chest is locked. "The treasure chest is locked."
static void McCoyPalace_ShowChestLocked(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgBilibinTreasureChestLocked, 1);
  Engine_EventEnd();
}

/// Searching a reward chest after the reward: the chosen one is empty, the
/// others locked.
static void McCoyPalace_ShowUnavailableChest(s32 chestFlag) {
  if (Engine_GameFlagIsSet(chestFlag) != 0) {
    McCoyPalace_ShowChestEmpty();
  } else {
    McCoyPalace_ShowChestLocked();
  }
}

/// Searching the first reward chest: a Vial, while no reward is taken.
static void Event_SearchVialChest(void) {
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_SCENE_TEMP_10);
  } else {
    McCoyPalace_ClaimReward(ACTOR_VIAL_CHEST, ITEM_VIAL, FLAG_SCENE_TEMP_10);
  }
}

/// Searching the second reward chest: a Potion, while no reward is taken.
static void Event_SearchPotionChest(void) {
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_SCENE_TEMP_11);
  } else {
    McCoyPalace_ClaimReward(ACTOR_POTION_CHEST, ITEM_POTION,
                            FLAG_SCENE_TEMP_11);
  }
}

/// Searching the third reward chest: a Psy Crystal, while no reward is
/// taken.
static void Event_SearchPsyCrystalChest(void) {
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_SCENE_TEMP_12);
  } else {
    McCoyPalace_ClaimReward(ACTOR_PSY_CRYSTAL_CHEST, ITEM_PSY_CRYSTAL,
                            FLAG_SCENE_TEMP_12);
  }
}

/// Searching the fourth reward chest: a Water of Life, while no reward is
/// taken.
static void Event_SearchWaterOfLifeChest(void) {
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_SCENE_TEMP_13);
  } else {
    McCoyPalace_ClaimReward(ACTOR_WATER_OF_LIFE_CHEST, ITEM_WATER_OF_LIFE,
                            FLAG_SCENE_TEMP_13);
  }
}

/// The chest opens and gives its item; with no room for it, the chest
/// closes again and the reward is still to choose.
static void McCoyPalace_ClaimReward(s32 chestActorId, s32 itemId,
                                    s32 chestFlag) {
  s32 emitterHandle;

  Engine_EventBegin();

  emitterHandle =
      BattleFx_PlayCueAndStartEmitterOnTarget(0, chestActorId, itemId);

  if (Engine_PartyGiveItem(itemId, 0) != -1) {
    Engine_ActorSetAnimation(chestActorId, 2);
    Engine_GameFlagSet(FLAG_MCCOY_PALACE_REWARD_TAKEN);
    Engine_GameFlagSet(chestFlag);
    Engine_GameFlagClear(0x322);
    Engine_GameFlagClear(FLAG_SCENE_TEMP_02);
  } else {
    Engine_AudioPlayCue(SE_125);
    Engine_ActorSetAnimation(chestActorId, 5);
  }

  Engine_ObjectDispatchRelease((struct FieldActor *)emitterHandle);
  Engine_EventEnd();
}

/// Leaving the reward room without a reward: the escort walks the leader back.
/// "Please take your reward before your depart!"
static void Event_EscortAsksPartyToChoose(void) {
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) == 0) {
    if (Engine_GameFlagIsSet(0x322) != 0) {
      Engine_EventBegin();
      Actor_ShowEmote(ACTOR_ESCORT, EMOTE_IN_FRONT, 0);
      Actor_FaceDirection(ACTOR_ESCORT, FACING_SOUTHWEST + FACING_STEP, 10);
      Engine_ActorRunRepeatedMotion(ACTOR_ESCORT, 2);
      Engine_EventWait(20);
      Engine_EventSetMessage(MsgBilibinPleaseTakeYourRewardBefore);
      Event_ShowMessage(ACTOR_ESCORT, 0);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 616, 762);
      Actor_FaceDirection(ACTOR_ESCORT, FACING_NORTH + FACING_STEP, 10);
      Engine_EventEnd();
    }
  }
}

/// Leaving the reward room with a reward: the escort leaves.
/// "You are always welcome in the palace of Lord McCoy."
static void Event_EscortSeesPartyOff(void) {
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    Engine_EventBegin();
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_ESCORT, 0);
    Actor_SetSpeed(ACTOR_ESCORT, PIXELS(3) / 5, PIXELS(3) / 10);
    Actor_WalkToAndWait(ACTOR_ESCORT, 622, 764);
    Actor_FaceDirection(ACTOR_ESCORT, FACING_NORTHEAST + FACING_STEP, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_ESCORT, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_REWARD_KEEPER, 3);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_ESCORT, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_ESCORT, 3);
    Engine_EventSetMessage(MsgBilibinAlwaysWelcomeInPalaceLord);
    Event_ShowMessageAndWait(ACTOR_ESCORT, 0, 10);
    Actor_SetSpeed(ACTOR_ESCORT, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkToAndWait(ACTOR_ESCORT, 570, 758);
    Actor_SetPosition(ACTOR_ESCORT, 0, 0);
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    GameFlag_Set(FLAG_MCCOY_PALACE_PARTY_REWARDED);
    GameFlag_Set(0x333);
    Engine_EventEnd();
  }
}

/// The four reward chests stand in a row.
static __inline__ void McCoyPalace_PositionRewardChests(void) {
  Actor_SetPosition(ACTOR_VIAL_CHEST, PIXELS(616), PIXELS(728));
  Actor_SetPosition(ACTOR_POTION_CHEST, PIXELS(632), PIXELS(728));
  Actor_SetPosition(ACTOR_PSY_CRYSTAL_CHEST, PIXELS(648), PIXELS(728));
  Engine_ActorSetPosition(ACTOR_WATER_OF_LIFE_CHEST, PIXELS(664), PIXELS(728));
}

/// The cells before the chest row can be walked on.
static __inline__ void McCoyPalace_OpenRewardChestRow(void) {
  Map_CopyCellAttributes(38, 55, 4, 1, 38, 45);
  Map_CopyCellAttributes(42, 55, 4, 1, 38, 46);
}

/// Sets the palace up: McCoy's first audience, the escort at the door after
/// it, the reward audience once Kolima's curse is lifted, or the reward
/// chests where the party left them.
s32 Scene_Initialize(void) {
  s32 rewardTaken;
  s32 curseLifted;
  struct FieldActor *chest;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  Engine_GameFlagSet(FLAG_MCCOY_PALACE_VISITED);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    Engine_GameFlagClear(FLAG_SCENE_TEMP_00);
  }
  // A first audience and a return to the entrance have different greetings.
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_BARRICADE_HINT) == 0 &&
      Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    if (gGameState.entrance == 29) {
      Event_AudienceWithMcCoy();
    } else if (gGameState.entrance == 9 && Engine_GameFlagIsSet(0x321) != 0) {
      Event_EscortHintsAtBarricade();
    }
  } else {
    rewardTaken = Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN);
    if (rewardTaken != 0) {
    } else {
      if (gGameState.entrance == 29) {
        if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_PARTY_REWARDED) == 0) {
          curseLifted = Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED);
          if (curseLifted != 0) {
            Event_McCoyRewardsParty();
          }
        }
      } else {
        // Restore the displayed chest row after reloading the reward room.
        if (gGameState.entrance == 28) {
          if (Engine_GameFlagIsSet(0x322) != 0) {
            if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
              McCoyPalace_OpenRewardChestRow();
              McCoyPalace_PositionRewardChests();
              chest = Object_GetById(ACTOR_VIAL_CHEST);
              Engine_ActorSetSpriteFlags(chest, 0);
              chest = Object_GetById(ACTOR_POTION_CHEST);
              Engine_ActorSetSpriteFlags(chest, 0);
              chest = Object_GetById(ACTOR_PSY_CRYSTAL_CHEST);
              Engine_ActorSetSpriteFlags(chest, 0);
              chest = Object_GetById(ACTOR_WATER_OF_LIFE_CHEST);
              Engine_ActorSetSpriteFlags(chest, 0);
              Object_GetById(ACTOR_VIAL_CHEST)->motionFlags = rewardTaken;
              Object_GetById(ACTOR_POTION_CHEST)->motionFlags = rewardTaken;
              Object_GetById(ACTOR_PSY_CRYSTAL_CHEST)->motionFlags =
                  rewardTaken;
              Object_GetById(ACTOR_WATER_OF_LIFE_CHEST)->motionFlags =
                  rewardTaken;
              chest = Object_GetById(ACTOR_VIAL_CHEST);
              chest->y.fixed = -PIXELS(4);
              chest = Object_GetById(ACTOR_POTION_CHEST);
              chest->y.fixed = -PIXELS(4);
              chest = Object_GetById(ACTOR_PSY_CRYSTAL_CHEST);
              chest->y.fixed = -PIXELS(4);
              chest = Object_GetById(ACTOR_WATER_OF_LIFE_CHEST);
              chest->y.fixed = -PIXELS(4);
            } else {
              Event_PresentRewardChests();
            }
          }
        }
      }
    }
  }
  return 0;
}

/// Garet, Ivan and Mia get ready to walk up to McCoy.
static __inline__ void McCoyPalace_PrepareCompanionApproach(void) {
  Actor_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(IVAN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
}

/// The first audience: the escort announces the party and McCoy asks them to
/// lift Kolima's curse. Refusing, he calls them too young and they leave;
/// coming back, they may change their minds. Accepting, McCoy gives the key
/// to the barricades, Garet bristles at being called children, and McCoy
/// takes the key back.
static void Event_AudienceWithMcCoy(void) {
  struct FieldActor *actor;

  // Stage the first audience, or place the leader for a return visit.
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Camera_MoveTo(PIXELS(894), -1, PIXELS(664), 0);
  Engine_TaskWait(1);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_JOB_REFUSED) != 0) {
    Camera_MoveTo(PIXELS(894), -1, PIXELS(698), 0);
    Actor_SetPosition(ACTOR_ESCORT, PIXELS(876), PIXELS(634));
    Actor_FaceDirection(ACTOR_ESCORT, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(894), PIXELS(798));
  }
  Engine_MapRedraw();
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 40;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_JOB_REFUSED) == 0) {
    Engine_EventWait(80);
    Actor_SetPosition(ACTOR_ESCORT, PIXELS(894), PIXELS(798));
    Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
    Camera_MoveTo(PIXELS(894), -1, PIXELS(698), 1);
    Actor_SetSpeed(ACTOR_ESCORT, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkTo(ACTOR_ESCORT, 894, 696);
    Engine_EventWait(80);
    Camera_MoveTo(PIXELS(894), -1, PIXELS(664), 1);
    Engine_ActorWaitForMove(19);
    Actor_WalkToAndWait(ACTOR_ESCORT, 842, 696);
    Actor_WalkToAndWait(ACTOR_ESCORT, 842, 636);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 20);
    Actor_WalkToAndWait(ACTOR_ESCORT, 876, 634);
    Engine_ActorSetAnimationAndWait(ACTOR_ESCORT, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
    Engine_EventWait(10);
    // The escort announces the warriors before they enter the audience.
    Engine_EventSetMessage(MsgBilibinMatter);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_ESCORT, 2);
    Engine_EventShowMessageAndWait(ACTOR_ESCORT, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_ESCORT, 3);
    Engine_EventWait(40);
    Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 5, 60);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    Engine_ActorSetAttachedEffect(ACTOR_ESCORT, EMOTE_IN_FRONT | 2);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_ESCORT, FACING_SOUTHEAST + FACING_STEP, 10);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 10);
    Camera_MoveTo(PIXELS(894), -1, PIXELS(698), 1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(894), PIXELS(798));
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 726);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    Engine_EventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    Camera_MoveTo(PIXELS(894), -1, PIXELS(664), 1);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Engine_ActorSetPosition(GARET, actor->x.fixed, actor->z.fixed);
    }
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Engine_ActorSetPosition(IVAN, actor->x.fixed, actor->z.fixed);
    }
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Engine_ActorSetPosition(MIA, actor->x.fixed, actor->z.fixed);
      }
    }
    McCoyPalace_PrepareCompanionApproach();
    Actor_SetDestinationOffset(GARET, -16, 16);
    Engine_ActorSetDestinationOffset(IVAN, 16, 16);
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
      Engine_ActorSetDestinationOffset(MIA, 32, 16);
    }
    Engine_ActorWaitForMove(IVAN);
    Engine_ActorSetAnimation(GARET, ANIM_STAND);
    Engine_ActorSetAnimation(IVAN, ANIM_STAND);
    Engine_ActorSetAnimation(MIA, ANIM_STAND);
    Engine_EventWait(10);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH, 0);
    Actor_FaceDirection(IVAN, FACING_NORTH, 20);
    Engine_ActorJump(ACTOR_LORD_MCCOY, 2, 20);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 10);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    Actor_FaceDirection(ACTOR_ESCORT, FACING_EAST + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_ESCORT, 3);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 40);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    Engine_ActorSetAttachedEffect(ACTOR_ESCORT, EMOTE_IN_FRONT | 2);
    Engine_EventWait(40);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 20);
    Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 5, 40);
    // Let the party accept the assignment or leave after McCoy refuses them.
    Engine_EventOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      // The initial offer and reconsideration share their accept/refuse paths.
      goto accepted;
    }
  declined:
    Engine_EventSetMessage(MsgBilibinTooYoungForTheJob);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    Actor_FaceDirection(ACTOR_ESCORT, FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
    Engine_EventShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    Actor_FaceDirection(GARET, FACING_NORTH, 0);
    Actor_FaceDirection(IVAN, FACING_NORTH, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(GARET, ANIM_WALK);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Engine_ActorSetDestination(GARET, actor->x.part.pixel,
                                 actor->z.part.pixel);
    }
    Engine_ActorSetAnimation(IVAN, ANIM_WALK);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Engine_ActorSetDestination(IVAN, actor->x.part.pixel,
                                 actor->z.part.pixel);
    }
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
      Engine_ActorSetAnimation(MIA, ANIM_WALK);
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Engine_ActorSetDestination(MIA, actor->x.part.pixel,
                                   actor->z.part.pixel);
      }
    }
    Engine_ActorWaitForMove(IVAN);
    Engine_ActorSetPosition(GARET, 0, 0);
    Engine_ActorSetPosition(IVAN, 0, 0);
    Actor_SetPosition(MIA, 0, 0);
    Engine_GameFlagSet(FLAG_MCCOY_PALACE_JOB_REFUSED);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 752);
    gEventWork->transitionFrames = 16;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    goto leave;
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 894, 684);
  Engine_EventWait(80);
  Engine_CameraSetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Engine_CameraMoveTo(PIXELS(894), -1, PIXELS(664), 1);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Engine_ActorSetPosition(MIA, actor->x.fixed, actor->z.fixed);
    }
  }
  McCoyPalace_PrepareCompanionApproach();
  Actor_SetDestinationOffset(GARET, -16, 16);
  Engine_ActorSetDestinationOffset(IVAN, 16, 16);
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    Engine_ActorSetDestinationOffset(MIA, 32, 16);
  }
  Engine_ActorWaitForMove(IVAN);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Engine_ActorSetAnimation(MIA, ANIM_STAND);
  Engine_EventWait(10);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 20);
  Engine_ActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 1, 60);
  // A returning party can reconsider the assignment without replaying the
  // escort.
  Engine_EventSetMessage(MsgBilibinYehveChangeHeart);
  Engine_EventOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    // Reuse the initial refusal's dialogue and departure.
    goto declined;
  }
accepted:
  // Both ways of accepting join the same explanation and departure.
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 20);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_ActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 5, 60);
  Engine_EventSetMessage(MsgBilibinHmmmWellGrant);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
  // Animate McCoy tossing the barricade key to the party.
  actor = Object_GetById(ACTOR_BARRICADE_KEY);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_BARRICADE_KEY);
  actor->scaleX = Q16_ONE / 2;
  actor->scaleY = Q16_ONE / 2;
  actor = Object_GetById(ACTOR_LORD_MCCOY);
  if (actor != 0) {
    Engine_ActorSetPosition(ACTOR_BARRICADE_KEY, actor->x.fixed,
                            actor->z.fixed);
  }
  Engine_TaskWait(1);
  Engine_ActorJump(ACTOR_BARRICADE_KEY, 6, 0);
  Actor_SetSpeed(ACTOR_BARRICADE_KEY, PIXELS(2), PIXELS(1));
  Actor_MoveToAndWait(ACTOR_BARRICADE_KEY, 894, 668);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 60);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 10);
  Engine_EventOpenMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    do {
      Engine_ActorStartRepeatedMotion(GARET, 2);
      Engine_ActorRunRepeatedMotion(IVAN, 2);
      Engine_EventSetMessage(MsgBilibinSeriousDoesntBother);
      Engine_EventOpenMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
    } while (Engine_EventChooseYesNo(0, 0) != 1);
  }
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  // Resolve Garet\'s objections before McCoy dismisses the party.
  Engine_EventSetMessage(MsgBilibinWellDontLet);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 5, 60);
  Engine_EventShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 60);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 1, 60);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 40);
  Engine_ActorFaceDirection(GARET, FACING_EAST, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  Actor_FaceDirection(MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 60);
  Actor_FaceDirection(IVAN, FACING_WEST, 10);
  Engine_EventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 10);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_EventOpenMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  while (Engine_EventChooseYesNo(0, 0) != 0) {
    Engine_EventSetMessage(MsgBilibinComeSaidYoud);
    Engine_EventOpenMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  }
  Engine_EventWait(10);
  Actor_FaceDirection(MIA, FACING_WEST, 0);
  Engine_ActorFaceDirection(GARET, FACING_EAST, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 60);
  Actor_FaceDirection(IVAN, FACING_WEST, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventSetMessage(MsgBilibinWellSGoing);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 20);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(40);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 5, 80);
  Actor_FaceDirection(ACTOR_ESCORT, FACING_EAST + FACING_STEP, 10);
  Actor_SetAttachedEffect(ACTOR_ESCORT, EMOTE_IN_FRONT | 2);
  Engine_EventWait(40);
  Engine_EventShowMessageAndWait(ACTOR_ESCORT, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_ESCORT, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
  Engine_ActorSetAnimation(ACTOR_LORD_MCCOY, 4);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorJump(ACTOR_BARRICADE_KEY, 6, 0);
  actor = Object_GetById(ACTOR_LORD_MCCOY);
  if (actor != 0) {
    Engine_ActorSetDestination(ACTOR_BARRICADE_KEY, actor->x.part.pixel,
                               actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_BARRICADE_KEY);
  Engine_ActorSetPosition(ACTOR_BARRICADE_KEY, 0, 0);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(MIA, EMOTE_IN_FRONT | 2);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  Actor_SetAttachedEffect(GARET, EMOTE_IN_FRONT | 2);
  Engine_ActorSetAttachedEffect(IVAN, EMOTE_IN_FRONT | 2);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_ESCORT, 2);
  Engine_EventShowMessageAndWait(ACTOR_ESCORT, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_ESCORT, 3);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_ESCORT, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 60);
  Engine_EventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorSetAnimation(ACTOR_LORD_MCCOY, 4);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorSetAnimation(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 7, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 7, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 7, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 7, 60);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_ESCORT, 2);
  Actor_FaceDirection(ACTOR_ESCORT, FACING_EAST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_ESCORT, 3);
  Engine_EventWait(20);
  FieldObject_SetTargetAndCallback(ACTOR_PARTY_LEADER,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_ESCORT,
                                   gPalace_FacingActions);
  // Keep the party looking at its escort while he leaves the audience.
  FieldObject_SetTargetAndCallback(GARET, ACTOR_LINK_KEEP_MOTION | ACTOR_ESCORT,
                                   gPalace_FacingActions);
  FieldObject_SetTargetAndCallback(IVAN, ACTOR_LINK_KEEP_MOTION | ACTOR_ESCORT,
                                   gPalace_FacingActions);
  FieldObject_SetTargetAndCallback(MIA, ACTOR_LINK_KEEP_MOTION | ACTOR_ESCORT,
                                   gPalace_FacingActions);
  Actor_SetSpeed(ACTOR_ESCORT, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_ESCORT, 852, 646);
  Actor_WalkToAndWait(ACTOR_ESCORT, 852, 666);
  Actor_WalkToAndWait(ACTOR_ESCORT, 864, 672);
  Actor_FaceDirection(ACTOR_ESCORT, FACING_EAST + FACING_STEP, 10);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_ESCORT, 0, 20);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(IVAN);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 60);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  FieldObject_SetTargetAndCallback(ACTOR_ESCORT,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                   gPalace_FacingActions);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    Engine_ActorSetAnimation(MIA, ANIM_WALK);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Engine_ActorSetDestination(MIA, actor->x.part.pixel, actor->z.part.pixel);
    }
  }
  Engine_ActorWaitForMove(IVAN);
  Engine_ActorSetPosition(GARET, 0, 0);
  Engine_ActorSetPosition(IVAN, 0, 0);
  Engine_ActorSetPosition(MIA, 0, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 752);
  gEventWork->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_GameFlagSet(0x321);
leave:
  Engine_EventRequestExit(29);
  Engine_EventEnd();
}

/// At the front door after the audience: McCoy's escort is sorry the party
/// was rejected, asks them not to go to Kolima and lets slip that the
/// barricade may not need a key.
static void Event_EscortHintsAtBarricade(void) {
  Engine_EventBegin();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Actor_FaceDirection(ACTOR_ESCORT, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 256, 660);
  Engine_EventWait(20);
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
  Event_PlayCue(SE_188);
  Map_ClearLayerEntryFlag(1);
  Map_ClearLayerEntryFlag(2);
  Engine_ActorSetPosition(ACTOR_ESCORT, PIXELS(256), PIXELS(632));
  Engine_TaskWait(1);
  Actor_SetSpeed(ACTOR_ESCORT, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_ESCORT, 256, 644);
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_ESCORT, 2);
  Engine_EventSetMessage(MsgBilibinNameSorryRejected);
  Engine_EventShowMessageAndWait(ACTOR_ESCORT, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 40);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 264, 660);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_WalkToAndWait(ACTOR_ESCORT, 248, 660);
  Actor_FaceDirection(ACTOR_ESCORT, FACING_EAST + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_ESCORT, 4);
  Engine_EventShowMessage(ACTOR_ESCORT, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_ESCORT, 3);
  Engine_EventAskYesNo(ACTOR_ESCORT, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_ESCORT, 2);
  Engine_EventShowMessageAndWait(ACTOR_ESCORT, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 60);
  Actor_SetAttachedEffect(ACTOR_ESCORT, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_ESCORT, 1);
  Engine_EventShowMessageAndWait(ACTOR_ESCORT, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_ESCORT, 3);
  Event_ShowMessage(ACTOR_ESCORT, 0);
  Actor_SetSpeed(ACTOR_ESCORT, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_ESCORT, 248, 772);
  Engine_ActorSetPosition(ACTOR_ESCORT, 0, 0);
  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  Engine_GameFlagSet(FLAG_MCCOY_PALACE_BARRICADE_HINT);
  Engine_EventEnd();
}

/// Garet, Ivan and Mia turn to talk among themselves.
static void McCoyPalace_FaceCompanionsForDiscussion(void) {
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 0);
}

/// Garet, Ivan and Mia face McCoy again.
static void McCoyPalace_FaceCompanionsForAudience(void) {
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
}

/// Brought in after lifting Kolima's curse: McCoy thanks the party and sends
/// them to choose a reward.
static void Event_McCoyRewardsParty(void) {
  struct FieldActor *viewCenter;
  struct FieldActor *actor;
  s32 advanceAlternateReply;

  Engine_EventBegin();
  // Reveal the approach, then walk the leader and escort into the audience.
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  viewCenter = Engine_EventGetViewCenter();
  advanceAlternateReply = 0;
  viewCenter->motionFlags = advanceAlternateReply;
  Camera_MoveTo(PIXELS(894), -1, PIXELS(664), 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  gEventWork->transitionFrames = 16;

  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Actor_SetPosition(ACTOR_ESCORT, PIXELS(888), PIXELS(798));
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(904), PIXELS(798));
  Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Camera_MoveTo(PIXELS(894), -1, PIXELS(698), 1);
  Actor_SetSpeed(ACTOR_ESCORT, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkTo(ACTOR_ESCORT, 888, 720);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 904, 736);
  Engine_EventWait(60);
  Engine_ActorWaitForMove(19);
  Engine_ActorSetAnimation(ACTOR_ESCORT, 1);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_ESCORT, 2);
  // Present the returning warriors, using the reply for their earlier
  // reception.
  Engine_EventSetMessage(MsgBilibinWeveBroughtWarriorsMilord);
  // Adjacent variants depend on whether McCoy's earlier request was declined.

  advanceAlternateReply = 1;
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_HINT) == 0) {
    gEventWork->message++;
    advanceAlternateReply = 0;
  }
  Event_ShowMessage(ACTOR_ESCORT, 0);
  if (advanceAlternateReply != 0) {
    gEventWork->message++;
  }

  Camera_MoveTo(PIXELS(894), -1, PIXELS(664), 1);
  Actor_SetActionScript(ACTOR_ESCORT, gPalace_PacingActions);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(MIA, actor->x.fixed, actor->z.fixed);
  }

  McCoyPalace_PrepareCompanionApproach();
  Actor_SetDestinationOffset(GARET, -16, 16);
  Actor_SetDestinationOffset(IVAN, 16, 16);
  Actor_SetDestinationOffset(MIA, 32, 16);
  Engine_ActorWaitForMove(IVAN);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Engine_ActorSetAnimation(MIA, ANIM_STAND);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ActorWaitForMove(MIA);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
  Object_RefreshSelectorById(19);
  Engine_EventWait(20);

  advanceAlternateReply = 1;
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_HINT) == 0) {
    gEventWork->message++;
    advanceAlternateReply = 0;
  }
  Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
  if (advanceAlternateReply != 0) {
    gEventWork->message++;
  }

  advanceAlternateReply = 1;
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_HINT) == 0) {
    gEventWork->message++;
    advanceAlternateReply = 0;
  }
  Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  if (advanceAlternateReply != 0) {
    gEventWork->message++;
  }

  // Let the companions answer McCoy\'s thanks in turn.
  McCoyPalace_FaceCompanionsForDiscussion();
  Engine_EventWait(20);
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_HINT) != 0) {
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 40);
  } else {
    Engine_EventWait(40);
  }

  Actor_FaceDirection(GARET, FACING_NORTH, 10);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | MIA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);

  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_HINT) != 0) {
    Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    McCoyPalace_FaceCompanionsForDiscussion();
    advanceAlternateReply = 1;
    if (Engine_EventChooseYesNo(0, 0) != 0) {
      gEventWork->message++;
      advanceAlternateReply = 0;
    }
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 0);
    McCoyPalace_FaceCompanionsForAudience();
    Engine_EventWait(10);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    if (advanceAlternateReply != 0) {
      gEventWork->message++;
    }
    Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 2, 60);
  } else {
    gEventWork->message += 4;
  }

  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
  McCoyPalace_FaceCompanionsForDiscussion();
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgBilibinHumblyThank);
  } else {
    Engine_EventSetMessage(MsgBilibinNaeYehDinnaeNeedTae);
  }

  McCoyPalace_FaceCompanionsForAudience();
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_ESCORT, 1);
  // McCoy explains his caution and offers a choice from the treasury.
  Engine_EventSetMessage(MsgBilibinWasButWorriedYehMight);
  Event_ShowMessageAndWait(ACTOR_ESCORT, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_EAST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  McCoyPalace_FaceCompanionsForAudience();
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 5, 60);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 8, 60);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);

  McCoyPalace_FaceCompanionsForDiscussion();
  advanceAlternateReply = 1;
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
    advanceAlternateReply = 0;
  }
  McCoyPalace_FaceCompanionsForAudience();
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  if (advanceAlternateReply != 0) {
    gEventWork->message++;
  }

  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_ESCORT, 1);
  Actor_FaceDirection(ACTOR_ESCORT, FACING_EAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_ESCORT, 3);
  Event_ShowMessageAndWait(ACTOR_ESCORT, 0, 10);
  Actor_FaceDirection(ACTOR_ESCORT, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);

  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(MIA, actor->x.part.pixel, actor->z.part.pixel);
  }

  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 0);
  FieldObject_SetTargetAndCallback(ACTOR_PARTY_LEADER,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_ESCORT,
                                   gPalace_FacingActions);
  Actor_WalkToAndWait(ACTOR_ESCORT, 852, 646);
  Actor_WalkToAndWait(ACTOR_ESCORT, 852, 666);
  Actor_WalkToAndWait(ACTOR_ESCORT, 864, 672);
  Actor_FaceDirection(ACTOR_ESCORT, FACING_EAST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_ESCORT, 1);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_ESCORT, 0, 10);
  Actor_WalkToAndWait(ACTOR_ESCORT, 886, 708);
  Actor_WalkTo(ACTOR_ESCORT, 894, 764);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 764);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(802);
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_HINT) == 0) {
    GameFlag_Set(FLAG_MCCOY_PALACE_BARRICADE_HINT);
    GameFlag_Set(FLAG_MCCOY_PALACE_ADMITTED);
  }
  Engine_EventRequestExit(6);
  Engine_EventEnd();
}

/// In the reward room: the escort shows the four chests and the keeper
/// explains that only one may be chosen.
static void Event_PresentRewardChests(void) {
  struct FieldActor *actor;

  Engine_EventBegin();
  // Restore the chest-row collision patches before presenting the reward.
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Engine_CameraMoveTo(0x2740000, -1, 0x2ec0000, 0);
  McCoyPalace_OpenRewardChestRow();
  Object_GetById(ACTOR_PARTY_LEADER)->facing = 0;
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(577), PIXELS(760));
  Object_GetById(ACTOR_ESCORT)->facing = 0;
  Engine_ActorSetPosition(ACTOR_ESCORT, PIXELS(592), PIXELS(760));
  Object_GetById(ACTOR_REWARD_KEEPER)->facing = FACING_WEST + FACING_STEP;
  Actor_SetPosition(ACTOR_REWARD_KEEPER, PIXELS(662), PIXELS(764));
  McCoyPalace_PositionRewardChests();
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_VIAL_CHEST), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_POTION_CHEST), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PSY_CRYSTAL_CHEST), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_WATER_OF_LIFE_CHEST), 0);
  actor = Object_GetById(ACTOR_VIAL_CHEST);
  actor->motionFlags = 0;
  Object_GetById(ACTOR_POTION_CHEST)->motionFlags = 0;
  Object_GetById(ACTOR_PSY_CRYSTAL_CHEST)->motionFlags = 0;
  Object_GetById(ACTOR_WATER_OF_LIFE_CHEST)->motionFlags = 0;
  Object_GetById(ACTOR_VIAL_CHEST)->y.fixed = -PIXELS(4);
  Object_GetById(ACTOR_POTION_CHEST)->y.fixed = -PIXELS(4);
  Object_GetById(ACTOR_PSY_CRYSTAL_CHEST)->y.fixed = -PIXELS(4);
  Object_GetById(ACTOR_WATER_OF_LIFE_CHEST)->y.fixed = -PIXELS(4);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  gEventWork->transitionFrames = 16;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Actor_SetSpeed(ACTOR_ESCORT, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkTo(ACTOR_ESCORT, 628, 764);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 612, 764);
  Engine_ActorSetAnimation(ACTOR_ESCORT, 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_ESCORT, 1);
  // The escort and keeper explain that only one chest may be chosen.
  Engine_EventSetMessage(MsgBilibinLordMccoyOrdered);
  Engine_EventShowMessageAndWait(ACTOR_ESCORT, 0, 10);
  Actor_WalkToAndWait(ACTOR_ESCORT, 622, 780);
  Actor_FaceDirection(ACTOR_ESCORT, FACING_NORTH, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_REWARD_KEEPER, 2);
  Engine_EventShowMessageAndWait(ACTOR_REWARD_KEEPER, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_02);
  Engine_EventEnd();
}

s32 gPalace_FacingActions[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gPalace_PacingActions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(842), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(842), 0, PIXELS(636)),
    SC_SYNC,
    SC_MOVE(PIXELS(876), 0, PIXELS(634)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gMcCoyPalaceEntrances[] = {
    {0, CONDITION_ALWAYS, 256, 0, 808, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 256, 0, 808, FACING_NORTH, 0, 32, 496, 480, 856, 0},
    {2, CONDITION_ALWAYS, 120, 0, 792, FACING_EAST, 0, 32, 496, 480, 856, 0},
    {3, CONDITION_ALWAYS, 392, 0, 792, FACING_WEST, 0, 32, 496, 480, 856, 0},
    {4, CONDITION_ALWAYS, 232, 0, 712, FACING_EAST, 0, 32, 496, 480, 856, 0},
    {5, CONDITION_ALWAYS, 280, 0, 712, FACING_WEST, 0, 32, 496, 480, 856, 0},
    {6, CONDITION_ALWAYS, 152, 0, 632, FACING_WEST, 0, 32, 496, 480, 856, 0},
    {7, CONDITION_ALWAYS, 104, 0, 552, FACING_EAST, 0, 32, 496, 480, 856, 0},
    {8, CONDITION_ALWAYS, 408, 0, 552, FACING_WEST, 0, 32, 496, 480, 856, 0},
    {9, CONDITION_ALWAYS, 256, 0, 632, FACING_SOUTH, 0, 32, 496, 480, 856, 0},
    {10, CONDITION_ALWAYS, 152, 0, 376, FACING_WEST, 0, 56, 304, 360, 464, 0},
    {11, CONDITION_ALWAYS, 264, 0, 376, FACING_EAST, 0, 56, 304, 360, 464, 0},
    {12, CONDITION_ALWAYS, 104, 0, 376, FACING_SOUTH, 0, 56, 304, 360, 464, 0},
    {13, CONDITION_ALWAYS, 312, 0, 376, FACING_SOUTH, 0, 56, 304, 360, 464, 0},
    {14, CONDITION_ALWAYS, 696, 0, 96, FACING_WEST, 0, 592, 0, 832, 160, 0},
    {15, CONDITION_ALWAYS, 680, 0, 72, FACING_WEST, 0, 592, 0, 832, 160, 0},
    {16, CONDITION_ALWAYS, 952, 0, 72, FACING_EAST, 0, 800, 0, 1040, 160, 0},
    {17, CONDITION_ALWAYS, 936, 0, 96, FACING_EAST, 0, 800, 0, 1040, 160, 0},
    {18, CONDITION_ALWAYS, 88, 0, 200, FACING_NORTH, 0, 0, 16, 240, 240, 0},
    {19, CONDITION_ALWAYS, 360, 0, 200, FACING_NORTH, 0, 208, 16, 448, 240, 0},
    {20, CONDITION_ALWAYS, 536, 0, 112, FACING_EAST, 0, 400, 0, 640, 160, 0},
    {21, CONDITION_ALWAYS, 536, 0, 88, FACING_EAST, 0, 400, 0, 640, 160, 0},
    {22, CONDITION_ALWAYS, 520, 0, 312, FACING_WEST, 0, 400, 208, 640, 368, 0},
    {23, CONDITION_ALWAYS, 520, 0, 296, FACING_WEST, 0, 400, 208, 640, 368, 0},
    {24, CONDITION_ALWAYS, 728, 0, 264, FACING_EAST, 0, 592, 192, 832, 352, 0},
    {25, CONDITION_ALWAYS, 904, 0, 264, FACING_WEST, 0, 792, 192, 1032, 352, 0},
    {26, CONDITION_ALWAYS, 744, 0, 424, FACING_EAST, 0, 592, 352, 832, 512, 0},
    {27, CONDITION_ALWAYS, 888, 0, 424, FACING_WEST, 0, 792, 352, 1032, 512, 0},
    {28, CONDITION_ALWAYS, 600, 0, 760, FACING_EAST, 0, 512, 560, 752, 816, 0},
    {29, CONDITION_ALWAYS, 896, 0, 728, FACING_NORTH, 0, 776, 560, 1016, 768,
     0},
    {SCENE_TABLE_END},
};

u32 gMcCoyPalaceExits[] = {
    SCENE_EXITS(33),        SCENE_EXIT(1, 34, 2),   SCENE_EXIT(2, 33, 14),
    SCENE_EXIT(3, 33, 17),  SCENE_EXIT(4, 33, 10),  SCENE_EXIT(5, 33, 11),
    SCENE_EXIT(6, 33, 28),  SCENE_EXIT(7, 33, 22),  SCENE_EXIT(8, 33, 20),
    SCENE_EXIT(9, 33, 29),  SCENE_EXIT(10, 33, 4),  SCENE_EXIT(11, 33, 5),
    SCENE_EXIT(12, 33, 18), SCENE_EXIT(13, 33, 19), SCENE_EXIT(14, 33, 2),
    SCENE_EXIT(15, 33, 26), SCENE_EXIT(16, 33, 27), SCENE_EXIT(17, 33, 3),
    SCENE_EXIT(18, 33, 12), SCENE_EXIT(19, 33, 13), SCENE_EXIT(20, 33, 8),
    SCENE_EXIT(21, 33, 25), SCENE_EXIT(22, 33, 7),  SCENE_EXIT(23, 33, 24),
    SCENE_EXIT(24, 33, 23), SCENE_EXIT(25, 33, 21), SCENE_EXIT(26, 33, 15),
    SCENE_EXIT(27, 33, 16), SCENE_EXIT(28, 33, 6),  SCENE_EXIT(29, 33, 9),
    SCENE_EXITS_END,
};

struct ScenePlacement gMcCoyPalacePlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gMcCoyPalacePlacements[] = {
    { 0x65, 0x0, ACTOR_STAND,
        PIXELS(265), 0, PIXELS(702), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(342), 0, PIXELS(632), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(632), FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, 0x0, ACTOR_WANDER,
        PIXELS(166), 0, PIXELS(728), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(326), 0, PIXELS(378), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_WANDER,
        PIXELS(362), 0, PIXELS(142), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(87), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x67, 0x0, ACTOR_STAND,
        PIXELS(952), 0, PIXELS(289), FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x96, 0x0, ACTOR_STAND,
        PIXELS(951), 0, PIXELS(438), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x96, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(616), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x3a, 0x0, ACTOR_STAND,
        PIXELS(894), 0, PIXELS(634), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x96, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xf5, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x14, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x14, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x14, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x14, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHWEST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x14, 0x0, 7,
        PIXELS(616), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x14, 0x0, 7,
        PIXELS(632), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x14, 0x0, 7,
        PIXELS(648), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x14, 0x0, 7,
        PIXELS(664), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x14, 0x0, 7,
        PIXELS(616), 0, PIXELS(648), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x14, 0x0, 7,
        PIXELS(648), 0, PIXELS(648), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x14, 0x0, 7,
        PIXELS(664), 0, PIXELS(648), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x14, 0x0, 7,
        PIXELS(616), 0, PIXELS(664), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x14, 0x0, 7,
        PIXELS(648), 0, PIXELS(664), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x14, 0x0, 7,
        PIXELS(664), 0, PIXELS(664), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct SceneEvent gMcCoyPalaceEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gMcCoyPalaceEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { TOUCH_FACING(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WalkThroughDoor) },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { TOUCH_FACING(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WalkThroughDoor) },
    { TOUCH_FACING(FACING_NORTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WalkThroughDoor) },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { EVENT_EXIT, 17, CONDITION_ALWAYS, 17 },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 19, CONDITION_ALWAYS, 19 },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, 20 },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, 21 },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, 22 },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, 23 },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, 24 },
    { EVENT_EXIT, 25, CONDITION_ALWAYS, 25 },
    { EVENT_EXIT, 26, CONDITION_ALWAYS, 26 },
    { EVENT_EXIT, 27, CONDITION_ALWAYS, 27 },
    { EVENT_EXIT, 28, CONDITION_ALWAYS, 28 },
    { EVENT_EXIT, 29, CONDITION_ALWAYS, 29 },
    { EVENT_TALK, 8, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIfYouWantToSeeLord) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBilibinThePeopleOfBilibinWillNever) },
    { EVENT_TALK, 9, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinMakeSureYouDontOffendLord) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgBilibinIHeardYouReceivedASpectacular) },
    { EVENT_TALK, ACTOR_KOLIMA_ASKER, FLAG_MCCOY_PALACE_REWARD_TAKEN, EVENT_SCRIPT(Event_AskAboutGoingToKolima) },
    { EVENT_TALK, ACTOR_KOLIMA_ASKER, CONDITION_ALWAYS, MSGNO(MsgBilibinLadyMccoyIsQuiteUpsetThat) },
    { EVENT_TALK, 11, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIAlwaysBelievedThatTheOld) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgBilibinThingsAreLookingGrimSouthOf) },
    { EVENT_TALK, 12, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThisIsLadyMccoysRoomSo) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBilibinMiladyWasVeryUpsetWhenShe) },
    { EVENT_TALK, 13, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinMiladyWasRenownedFarAndWide) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgBilibinForOnceMilordDidntLetLady) },
    { EVENT_TALK, ACTOR_STARTLED_RESIDENT, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinOhSomedayIWantToSleep) },
    { EVENT_TALK, ACTOR_STARTLED_RESIDENT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToStartledResident) },
    { EVENT_TALK, ACTOR_PARTY_ADMIRER, FLAG_MCCOY_PALACE_BARRICADE_HINT, MSGNO(MsgBilibinYouCanSeeKolimaPrettyWell) },
    { EVENT_TALK, ACTOR_PARTY_ADMIRER, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHeyIfYouCantDoThe) },
    { EVENT_TALK, ACTOR_PARTY_ADMIRER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToAdmirer) },
    { EVENT_TALK, ACTOR_KOLIMA_WATCHER, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHmmItSeemsTheCurseOf) },
    { EVENT_TALK, ACTOR_KOLIMA_WATCHER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToKolimaWatcher) },
    { EVENT_TALK, ACTOR_ESCORT, CONDITION_ALWAYS, MSGNO(MsgBilibinYouMaySelectASingleItem) },
    { EVENT_TALK, ACTOR_REWARD_KEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToRewardKeeper) },
    { EVENT_TALK, ACTOR_LORD_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinSorryLadsButKolimasFutureIs) },
    { EVENT_TALK, ACTOR_LORD_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinYehveTaughtMeThatICannae) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHowManyTimesDoIHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBilibinIFeelSorryForTheWarriors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinTheyShouldNeverHaveLetSuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBilibinIWonderWhatKindOfTreasure) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KOLIMA_ASKER, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinSendingTheseChildrenToKolimaForest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KOLIMA_ASKER, CONDITION_ALWAYS, MSGNO(MsgBilibinIWishShedJustStopWhining) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIShouldntHaveSaidAnythingWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBilibinTheydBeGuaranteedAGoodLife) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinOfAllTheNerveComingInto) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBilibinICanTellWhenShesUpset) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinWillIEverFindSomeoneTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgBilibinSometimesAMansJustGottaPut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STARTLED_RESIDENT, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinOhNoThisNightgownHasA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STARTLED_RESIDENT, CONDITION_ALWAYS, MSGNO(MsgBilibinTheHoleInThisNightgownIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PARTY_ADMIRER, FLAG_MCCOY_PALACE_BARRICADE_HINT, MSGNO(MsgBilibinLordMccoyHasntComeHereFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PARTY_ADMIRER, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinItsAToughThingBeingSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PARTY_ADMIRER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadAdmirersThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KOLIMA_WATCHER, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinKolimaForestUsedToBeSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KOLIMA_WATCHER, CONDITION_ALWAYS, MSGNO(MsgBilibinACourageousWarriorWouldntThinkTwice) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ESCORT, CONDITION_ALWAYS, MSGNO(MsgBilibinTheseCourageousWarriorsDeserveToReceive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_REWARD_KEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRewardKeepersThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LORD_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThereIsNothingMoreToTalk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LORD_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinButICannaeFiggerOutHow) },
    { SEARCH(SEARCH_UNNAMED), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchVialChest) },
    { SEARCH(SEARCH_UNNAMED), 101, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchPotionChest) },
    { SEARCH(SEARCH_UNNAMED), 102, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchPsyCrystalChest) },
    { SEARCH(SEARCH_UNNAMED), 103, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchWaterOfLifeChest) },
    { EVENT_TOUCH, 50, 0x333, EVENT_SCRIPT(Event_EscortSeesPartyOff) },
    { EVENT_TOUCH, 51, 0x333, EVENT_SCRIPT(Event_EscortAsksPartyToChoose) },
    { SEARCH(SEARCH_BARREL), 120, FLAG_MCCOY_PALACE_BARREL_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { SEARCH(SEARCH_BARREL), 121, FLAG_MCCOY_PALACE_BARREL_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_WARDROBE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresASilkNegligeeIsaacPut)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresASheetOfPaperTitled)) },
    { SCENE_EVENTS_END },
};
// clang-format on
