/// @file
/// Lunpa, the town of the noble thief's family, sealed by Dodonpa's men while
/// he holds Hammet for ransom in Lunpa Fortress. Two areas share this
/// overlay, each a scene of its own:
///
/// - Lunpa (SceneId_Lunpa, area name Lunpa): the town. Two guards keep the road
///   in from the World Map; doors lead into the homes, shops, inn and temple
///   (RockRooms, rock_rooms.c), roads to West Lunpa Cave and up to
///   the Lunpa Fortress Gates, and a hidden passage to Lunpa's hidden room
///   (LUNPA_TO_STONE_ROOMS, in StoneRooms, whose area name there is Lunpa).
/// - Lunpa Fortress Gates (SceneId_LunpaFortressGates, area name Lunpa Fortress
///   Gates): the steps up to the fortress door, kept by two guards whom the
///   party gets past under Cloak.
///
/// The getters choose by gCommon.sceneId, and the town's triggers also
/// by FLAG_DODONPA_LOCKED_UP, which Donpa's judgment in the fortress sets:
/// before it the townspeople live under Dodonpa's men (gLunpaSealedTriggers),
/// after it they talk of the intruders (gLunpaReopenedTriggers). Both tables
/// share the roads and doors (LUNPA_ROADS_AND_DOORS) and Reveal's secrets
/// (LUNPA_SECRETS).
///
/// The town hides three things that Reveal shows (Lunpa_RevealSecrets)
/// and that every entry and the end of Reveal hide again
/// (Lunpa_HideSecrets): a Psynergy Stone, a puddle that Frost freezes
/// into a pillar to stand on, and the sparkle of a switch. Searching the switch
/// opens the passage to the hidden room, which can be walked through only while
/// Reveal shows it (Lunpa_EnterHiddenPassage). Its scene temp flags:
///
/// - FLAG_LUNPA_PUDDLE_FROZEN: Frost has frozen the puddle.
/// - FLAG_LUNPA_SECRETS_HIDDEN: the secrets are hidden, Reveal not shining.
/// - FLAG_LUNPA_HIDDEN_PASSAGE_OPEN: the switch has opened the passage.
///
/// At the gates, stepping uncloaked on the ground before the guards
/// (MB_LUNPA_FORTRESS_GATES_GUARDED_GROUND) brings them in front of the door,
/// and an uncloaked leader among them, or one still among them as Cloak fades,
/// is marched back down the steps (Lunpa_GuardsCatchParty, raised as
/// RAISED_PARTY_SPOTTED by Lunpa_WatchForUncloakedParty). A party caught in the
/// fortress arrives by LUNPA_FORTRESS_GATES_AFTER_CAUGHT and is thrown down the
/// steps; after the rescue it leaves the fortress by
/// LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA and sneaks past under
/// Cloak. Every entry clears the fortress's puzzle and beaten-guard flags, so
/// the fortress starts over. Between Donpa's judgment and the reunion in West
/// Lunpa Cave (FLAG_LUNPA_CAVE_REUNION_SEEN), Garet stops the leader from going
/// back up (MB_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH). Its scene temp flags:
///
/// - FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR: the guards stand before the
///   door.
/// - FLAG_LUNPA_FORTRESS_GATES_CLOAK_CAST: Cloak has been cast at the gates.
/// - FLAG_LUNPA_FORTRESS_GATES_ENTERING_FORTRESS: the leader is going through
///   the fortress door.
/// - FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING: Garet's warning is under way.
/// - FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY: the guards are marching
///   the party down.

#include "common.h"
#include "version.h"
#include "scene_ids.h"
#include "unit/scene/lunpa.h"
#include "unit/scene/shared/actor_orbit.h"
#include "unit/scene/shared/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "types.h"
#include "field_effect.h"
#include "event_layer.h"
#include "engine_call.h"
#include "scene_tables.h"
#include "scene.h"
#include "edition.h"
#include "lunpa.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "macros.h"
#include "heap.h"

static void Lunpa_WatchForUncloakedParty(void);

enum { PALETTE_TRANSLUCENT_EFFECT = 14, PALETTE_EFFECT = 15 };

#include "shared/effect_create.h"
#include "shared/effect_spawn.h"

/// Frost has frozen the hidden puddle into an ice pillar to stand on.
/// ★ Named from what the player sees: a small blue puddle that only Reveal
/// shows (its render)
#define FLAG_LUNPA_PUDDLE_FROZEN FLAG_SCENE_TEMP_00
/// The town's secrets are hidden: Reveal is not shining.
/// ★ Named from the game: "Reveal" (MsgAbilityName)
#define FLAG_LUNPA_SECRETS_HIDDEN FLAG_SCENE_TEMP_01
/// The hidden switch has opened the passage to the hidden room.
/// ★ Named from the game: "At least those punks won't find the switch that
/// opens the door. Heh heh! It's hidden too well in the wall!"
/// (MsgLunpaHiddenDoorSwitch)
#define FLAG_LUNPA_HIDDEN_PASSAGE_OPEN FLAG_SCENE_TEMP_02
/// The guards have stepped in front of the fortress door.
/// ★ Named from the game: "Don't come any closer to the fortress!"
/// (MsgLunpaGuardsWarnPartyAway)
#define FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR FLAG_SCENE_TEMP_40
/// Cloak has been cast at the gates and has not yet faded.
/// ★ Named from the game: "Cloak" (MsgAbilityName)
#define FLAG_LUNPA_FORTRESS_GATES_CLOAK_CAST FLAG_SCENE_TEMP_41
/// The leader is going through the fortress door.
/// ★ Named from what the player sees: the golden door set in the cliff at the
/// top of the steps, past the two guards (its render)
#define FLAG_LUNPA_FORTRESS_GATES_ENTERING_FORTRESS FLAG_SCENE_TEMP_42
/// Garet's warning against going back up to the fortress is under way.
/// ★ Named from the game: "Don't be stupid! We can't go in there, remember?"
/// (MsgLunpaGaretRefusesToReturn)
#define FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING FLAG_SCENE_TEMP_43
/// The guards have caught the party and are marching it down the steps.
/// ★ Named from the game: "That's strange. I've been on watch this whole time,
/// so how did you get here?" (MsgLunpaGuardsCatchParty)
#define FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY FLAG_SCENE_TEMP_44

/// How long an effect of common/effect_spawn.h lasts: 22, 44 or 126
/// frames, then it moves away and is released.
s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, 0), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),     SC_RELEASE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, 0), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),     SC_RELEASE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, 0), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),      SC_RELEASE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

const struct SceneEntrance *Scene_GetEntrances(void) {
  s32 sceneId = gCommon.sceneId;

  if (sceneId == (s32)&SceneId_Lunpa) {
    return gLunpaEntrances;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressGates) {
    return gGateEntrances;
  }
  return gLunpaEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) {
  s32 sceneId = gCommon.sceneId;

  if (sceneId == (s32)&SceneId_Lunpa) {
    return gLunpaExits;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressGates) {
    return gGateExits;
  }
  return gLunpaExits;
}

/// The townsfolk of Lunpa, or the guards of the Lunpa Fortress Gates.
const struct ActorTemplate *Scene_GetActorTemplates(void) {
  s32 sceneId = gCommon.sceneId;

  if (sceneId == (s32)&SceneId_Lunpa) {
    return gLunpaActorTemplates;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressGates) {
    return gGateActorTemplates;
  }
  return gLunpaActorTemplates;
}

/// Lunpa: Catch on the Nut floating over the town gives it to the party.
static void Lunpa_CatchFloatingNut(void) {
  Event_Start();
  Import_ActorSetPosition(ACTOR_FLOATING_NUT, 0, 0);
  GameFlag_Set(FLAG_LUNPA_NUT_CAUGHT);
  ItemDisplay_ShowItem(ITEM_NUT, 3);
  PartyInventory_GiveItem(ITEM_NUT, 0);
  Event_End();
}

/// Restores the metatile behaviors of the frozen pillar's three cells,
/// including its sides.
static inline void Lunpa_ApplyFrozenPuddleCollision(void) {
  MapGrid_CopyCellAttributes(17, 4, 1, 1, 14, 4);
  MapGrid_CopyCellAttributes(15, 3, 1, 1, 15, 4);
  MapGrid_CopyCellAttributes(15, 3, 1, 1, 13, 4);
}

/// Lunpa: Frost on the puddle Reveal shows freezes it into a pillar the party
/// can stand on.
static void Lunpa_FreezeHiddenPuddle(void) {
  struct Actor *pillar;

  pillar = Actor_Get(ACTOR_HIDDEN_PUDDLE);
  Actor_Get(ACTOR_PARTY_LEADER);
  Lunpa_ApplyFrozenPuddleCollision();
  if (pillar != NULL) {
    Actor_SetSpriteFlags(pillar, 0);
    pillar->motionFlags = ACTOR_FALLS;
    pillar->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }
  GameFlag_Set(FLAG_LUNPA_PUDDLE_FROZEN);
}

/// Every frame while the pillar stands: draw it under the leader once the
/// leader is on top.
static void Lunpa_UpdateIcePillarDrawOrder(void) {
  if (Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed >= PILLAR_TOP_HEIGHT) {
    Actor_GetById(ACTOR_HIDDEN_PUDDLE)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
  } else {
    Actor_GetById(ACTOR_HIDDEN_PUDDLE)->priorityFlags =
        ACTOR_PRIORITY_AUTOMATIC;
  }
}

/// Draws the three map grid regions that make one frame of the hidden passage.
static inline void Lunpa_DrawHiddenPassageFrame(s32 sourceColumn,
                                                s32 sourceRow) {
  MapGrid_CopyCells(sourceColumn, sourceRow, 3, 4, 1, 14);
  MapGrid_CopyCells(sourceColumn + 3, sourceRow, 3, 4, 33, 14);
  MapGrid_CopyCells(sourceColumn + 6, sourceRow, 3, 4, 1, 46);
}

/// Lunpa, casting Reveal: the Psynergy Stone's spot, the hidden switch's
/// sparkle, the puddle and, once opened, the passage to the hidden room show.
static void Lunpa_RevealSecrets(void) {
  struct Actor *actor;
  s32 cellX;
  s32 cellZ;

  // Restore the hidden stone and pillar before exposing the switch or passage.
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  cellX = actor->x.fixed / CELL_SIZE;
  cellZ = actor->z.fixed / CELL_SIZE;
  if (GameFlag_Get(FLAG_LUNPA_PSYNERGY_STONE) == 0) {
    if (cellX == 7 && cellZ == 16) {
      Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 16);
    }
    TreasureSpot_SetPosition(TREASURE_SPOT_PSYNERGY_STONE, -1, -1);
    MapGrid_CopyCellAttributes(28, 31, 1, 1, 7, 16);
  }
  MapGrid_CopyCells(47, 4, 1, 1, 46, 4);
  MapGrid_CopyCellAttributes(34, 37, 3, 3, 13, 3);
  Actor_SetPosition(ACTOR_HIDDEN_PUDDLE, PIXELS(232), PIXELS(72));
  Actor_Get(ACTOR_HIDDEN_PUDDLE)->y.fixed = 0;
#if TBS_PATCH_4
  if (GameFlag_Get(FLAG_LUNPA_PUDDLE_FROZEN)) {
    Lunpa_ApplyFrozenPuddleCollision();
  }
#endif
  // The opened passage replaces the sparkle; otherwise show the hidden switch.
  if (GameFlag_Get(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN) != 0) {
    Lunpa_DrawHiddenPassageFrame(41, 49);
  } else {
    Actor_SetPosition(ACTOR_SWITCH_SPARKLE, PIXELS(56), PIXELS(268));
    Actor_SetSpriteFlags(Actor_Get(ACTOR_SWITCH_SPARKLE), 0);
    actor = Actor_Get(ACTOR_SWITCH_SPARKLE);
    if (actor != NULL) {
      actor->motionFlags = ACTOR_FLOATS_HIGH;
      actor->y.fixed = PIXELS(16);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->scaleX = SPARKLE_SCALE_X;
      actor->scaleY = SPARKLE_SCALE_Y;
    }
  }
  Task_AddFrame(Lunpa_UpdateIcePillarDrawOrder, TASK_PASS_FRAME);
  GameFlag_Clear(FLAG_LUNPA_SECRETS_HIDDEN);
}

/// Lunpa, as Reveal ends and on every entry: the secrets hide again.
static void Lunpa_HideSecrets(void) {
  struct Actor *puddle;

  Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetPosition(ACTOR_SWITCH_SPARKLE, 0, 0);
  Actor_SetPosition(ACTOR_HIDDEN_PUDDLE, 0, 0);
  MapGrid_CopyCells(38, 38, 1, 1, 46, 4);
  MapGrid_CopyCellAttributes(37, 37, 3, 3, 13, 3);
  MapGrid_CopyCellAttributes(37, 37, 1, 1, 14, 2);
  MapGrid_CopyCellAttributes(8, 16, 1, 1, 7, 16);
  TreasureSpot_SetPosition(TREASURE_SPOT_PSYNERGY_STONE, 0, 0);
  MapGrid_CopyCells(32, 42, 3, 2, 1, 15);
  GameFlag_Clear(FLAG_LUNPA_PUDDLE_FROZEN);
  Import_ActorSetAnimation(ACTOR_HIDDEN_PUDDLE, ANIM_STAND);
  puddle = Actor_Get(ACTOR_HIDDEN_PUDDLE);
  puddle->update = NULL;
  Actor_SetColorMode(Actor_Get(ACTOR_HIDDEN_PUDDLE), 0);
  Task_Remove(Lunpa_UpdateIcePillarDrawOrder);
  GameFlag_Set(FLAG_LUNPA_SECRETS_HIDDEN);
}

/// Searching the well (MB_LUNPA_WELL, MB_LUNPA_FORTRESS_GATES_WELL), in Lunpa
/// or at the Lunpa Fortress Gates: frogs croak in it.
static void Lunpa_SearchWell(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldPeeredWell, 1);
  FieldMessage_Show(MsgLunpaWellFrogs, 1);
  Event_End();
}

/// The triggers of the Lunpa Fortress Gates, or the town's before and after
/// Donpa's judgment.
const struct Trigger *Scene_GetTriggers(void) {
  s32 sceneId = gCommon.sceneId;

  if (sceneId == (s32)&SceneId_LunpaFortressGates) {
    GameFlag_Get(FLAG_DODONPA_LOCKED_UP);
    return gGateTriggers;
  }
  if (sceneId == (s32)&SceneId_Lunpa &&
      GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    return gLunpaReopenedTriggers;
  }
  return gLunpaSealedTriggers;
}

/// Lunpa, before Donpa's judgment: the west guard on the road in asks whether
/// the party wants to enter Lunpa, and turns them away either way.
static void Lunpa_TalkToWestGuard(void) {
  Event_Start();
  FieldEffect_SpawnAttached(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_SetMessageId(MsgLunpaWestGuardAsksAboutEntering);
  Event_AskYesNoAndShowResponse(ACTOR_WEST_GUARD, 0);
  Event_End();
}

/// Lunpa, before Donpa's judgment: the east guard on the road in wants
/// Dodonpa's authorization, or, once the guards suspect Kalay, asks whether
/// the party came from there and whether they are here to save Hammet.
static void Lunpa_TalkToEastGuard(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_LUNPA_GUARDS_SUSPECT_KALAY) == 0) {
    Event_SetMessageId(MsgLunpaEastGuardDemandsAuthorization);
    Event_ShowMessageAndWait(ACTOR_EAST_GUARD, 0);
  } else {
    Event_SetMessageId(MsgLunpaEastGuardAsksIfFrom);
    FieldMessage_ShowAtActor(ACTOR_EAST_GUARD, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      gFieldWork->message++;
      FieldMessage_ShowAtActor(ACTOR_EAST_GUARD, 0);
      if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
        gFieldWork->message++;
      }
    }
    Event_ShowMessageAndWait(ACTOR_EAST_GUARD, 0);
  }
  Event_End();
}

/// Lunpa: Mind Read on the east guard. He fears Dodonpa's blame, trusts the
/// cave's gate once he suspects Kalay, and after Donpa's judgment thinks the
/// merchant who was desperate to do business in the village is still around
/// somewhere, causing no trouble.
static void Lunpa_ReadEastGuardsThoughts(void) {
  s32 thoughts;

  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) == 0) {
    thoughts = GameFlag_Get(FLAG_LUNPA_GUARDS_SUSPECT_KALAY);
    if (thoughts == 0) {
      thoughts = MsgLunpaEastGuardFearsBlame;
    } else {
      thoughts = MsgLunpaEastGuardTrustsCaveGate;
    }
    Event_SetMessageId(thoughts);
    Event_ShowMessageAndWait(ACTOR_EAST_GUARD, 0);
  } else {
    Event_SetMessageId(MsgLunpaEastGuardThinksMerchantHarmless);
    Event_ShowMessageAndWait(ACTOR_EAST_GUARD, 0);
  }
}

/// Lunpa, before Donpa's judgment: a man asks how long the party has been
/// in Lunpa.
static void Lunpa_AskAboutLengthOfStay(void) {
  Event_Start();
  Event_SetMessageId(MsgLunpaVillagerAAsksHowLong);
  Event_AskYesNoAndShowResponse(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA, 0);
  Event_End();
}

/// Lunpa, before Donpa's judgment: a stout man asks whether Dodonpa
/// kidnapped Hammet for ransom.
static void Lunpa_AskAboutHammetsKidnapping(void) {
  Event_Start();
  Event_SetMessageId(MsgLunpaVillagerCAsksAboutKidnapping);
  Event_AskYesNoAndShowResponse(ACTOR_MAN_ON_HAMMETS_KIDNAPPING, 0);
  Event_End();
}

/// Lunpa, before Donpa's judgment: a small boy asks whether the party wants
/// to see Donpa, who is in the fortress.
static void Lunpa_AskAboutSeeingDonpa(void) {
  Event_Start();
  Event_SetMessageId(MsgLunpaVillagerGAsksAboutDonpa);
  Event_AskYesNoAndShowResponse(ACTOR_BOY_ASKING_TO_SEE_DONPA, 0);
  Event_End();
}

/// Lunpa, after Donpa's judgment: a woman shivers and complains that it
/// has become colder.
static void Lunpa_ComplainAboutCold(void) {
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_WOMAN_WITH_NO_FREEDOM, 3);
  Event_SetMessageId(MsgLunpaVillagerBShivers);
  Event_ShowMessageAndWait(ACTOR_WOMAN_WITH_NO_FREEDOM, 0);
  Event_End();
}

/// Lunpa, after Donpa's judgment: a woman offers to explain the commotion
/// at the fortress.
static void Lunpa_AskAboutFortressCommotion(void) {
  Event_Start();
  Event_SetMessageId(MsgLunpaVillagerDAsksAboutCommotion);
  Event_AskYesNoAndShowResponse(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD, 0);
  Event_End();
}

/// Lunpa Fortress Gates: the left guard hears someone while the party is
/// cloaked; after Donpa's judgment, he nearly recognizes Hammet with the
/// party.
static void Lunpa_TalkToLeftGuard(void) {
  s32 recognitionMessageBase;

  if (gCommon.cloaked != 0) {
    Event_SetMessageId(MsgLunpaLeftGuardHearsSomeone);
  } else if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0 &&
             GameFlag_Get(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
    FieldEffect_SpawnAttached(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION,
                              60);
    recognitionMessageBase = MsgLunpaLeftGuardRecognizesHammet;
    Event_SetMessageId(recognitionMessageBase + RECOGNITION_SEEN_THAT_MAN);
    Event_ShowMessageAndWait(ACTOR_LEFT_GUARD, 0);
    Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
    Event_SetMessageId(recognitionMessageBase + RECOGNITION_IMPOSSIBLE);
    GameFlag_Set(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
  } else {
    Event_SetMessageId(MsgLunpaLeftGuardResentsDodonpa);
  }
  Event_ShowMessageAndWait(ACTOR_LEFT_GUARD, 0);
}

/// Lunpa Fortress Gates: the right guard boasts, finds the place creepy while
/// the party is cloaked, and after Donpa's judgment wonders how the warriors
/// from Kalay got in.
static void Lunpa_TalkToRightGuard(void) {
  if (gCommon.cloaked != 0) {
    Event_SetMessageId(MsgLunpaRightGuardFeelsCreepy);
  } else if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    Event_SetMessageId(MsgLunpaRightGuardWondersHow);
  } else {
    Event_SetMessageId(MsgLunpaRightGuardBoasts);
  }
  Event_ShowMessageAndWait(ACTOR_RIGHT_GUARD, 0);
}

/// Lunpa Fortress Gates: Mind Read on the left guard. After Donpa's judgment
/// and before Hammet finds Bunza, he thinks the man with the party looks like
/// that merchant, and Mind Read is cut short the first time as he turns to
/// look.
static void Lunpa_ReadLeftGuardsThoughts(void) {
  s32 recognizedHammet;

  if (gCommon.cloaked == 0 && GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0 &&
      GameFlag_Get(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
    recognizedHammet = Event_GetFlag(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
    if (recognizedHammet == 0) {
      gFieldWork->psynergy = recognizedHammet;
      FieldPsynergy_FinishWaveAndReleaseHeapBlock();
      Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
      Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
      Event_SetMessageId(MsgLunpaLeftGuardRecognizesHammet);
      GameFlag_Set(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
    } else {
      Event_SetMessageId(MsgLunpaLeftGuardGlimpsedMerchant);
    }
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
    Event_SetMessageId(MsgLunpaLeftGuardDismissesThought);
  } else {
    Event_SetMessageId(MsgLunpaLeftGuardThoughts);
  }
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
}

/// Lunpa Fortress Gates: Mind Read on the right guard.
static void Lunpa_ReadRightGuardsThoughts(void) {
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    Event_SetMessageId(MsgLunpaRightGuardReopenedThoughts);
  } else {
    Event_SetMessageId(MsgLunpaYoudShadowSneak);
  }
  Event_ShowMessageAndWait(ACTOR_RIGHT_GUARD, 0);
}

/// Every frame at the Lunpa Fortress Gates after Donpa's judgment: stepping up
/// to row 10 raises Garet's warning
/// (MB_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH).
static void Lunpa_WatchForFortressApproach(void) {
  s32 leaderRow = Actor_Get(ACTOR_PARTY_LEADER)->z.fixed / CELL_SIZE;

  if (Event_GetFlag(FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING) == 0 &&
      leaderRow == 10) {
    GameFlag_Set(FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING);
    gFieldWork->stepTrigger = MB_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH;
  }
}

/// Lunpa Fortress Gates, MB_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH after
/// Donpa's judgment: Garet reminds the leader they cannot go into the fortress,
/// and the leader steps back.
static void Lunpa_GaretTurnsIsaacBack(void) {
  Event_Start();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_SetMessageId(MsgLunpaGaretRefusesToReturn);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 100);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 12);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  GameFlag_Clear(FLAG_LUNPA_FORTRESS_GATES_GARET_WARNING);
  Event_End();
}

/// Lunpa, walking north into a doorway: the door opens and the leader goes
/// into the home, shop, inn or temple behind it.
static void Lunpa_EnterDoor(void) {
  struct FieldWork *work;
  struct Actor *placedActor;
  u32 actorId;
  s32 doorIndex;

  work = gFieldWork;
  Event_Start();
  // Freeze placed actors before opening the selected door and crossing
  // it.
  for (actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_ID; actorId++) {
    placedActor = Actor_Get(actorId);
    if (placedActor != NULL) {
      placedActor->motionFlags = 0;
    }
  }
  Event_PlaySong(SE_DOOR_OPEN);
  doorIndex = work->stepTrigger - MB_LUNPA_FIRST_HOME_DOOR;
  MapGrid_AnimateCells(gLunpaDoors[doorIndex].steps, gLunpaDoors[doorIndex].x,
                       gLunpaDoors[doorIndex].y);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (doorIndex != DOOR_TEMPLE) {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
    Event_WaitFrames(10);
  }
  Import_EventSetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Lunpa, MB_LUNPA_HIDDEN_PASSAGE: the hidden passage leads to the hidden room
/// once opened, while Reveal shows it.
static void Lunpa_EnterHiddenPassage(void) {
  if (GameFlag_Get(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN) != 0) {
    if (GameFlag_Get(FLAG_LUNPA_SECRETS_HIDDEN) == 0) {
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
      Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
      Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
      Event_WaitFrames(13);
      Import_EventSetExitRequest(LUNPA_TO_STONE_ROOMS);
    }
  }
}

/// Lunpa Fortress Gates, MB_LUNPA_FORTRESS_GATES_FORTRESS_DOOR: through the
/// fortress door at the top of the steps, into Lunpa Fortress.
static void Lunpa_EnterFortress(void) {
  GameFlag_Set(FLAG_LUNPA_FORTRESS_GATES_ENTERING_FORTRESS);
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -8);
  Event_PlaySong(SE_DOOR_OPEN);
  MapGrid_CopyCells(53, 4, 2, 2, 41, 4);
  Event_WaitFrames(10);
  MapGrid_CopyCells(53, 6, 2, 2, 41, 4);
  Event_WaitFrames(10);
  Import_EventSetExitRequest(
      LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_GROUND_FLOOR);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

static void UNUSED Lunpa_CheckAhead(void) { Field_TryJumpForward(); }

/// Unused: plays the treasure fanfare for Reveal uncovering something, until
/// FLAG_LUNPA_SECRETS_HIDDEN is set; nothing calls it.
UNUSED static void Lunpa_PlayRevealCue(void) {
  if (GameFlag_Get(FLAG_LUNPA_SECRETS_HIDDEN) == 0) {
    Sound_PlaySong(MUS_TREASURE_FOUND);
  }
}

/// Lunpa: searching the switch Reveal shows opens the passage to the hidden
/// room in five steps.
static void Lunpa_PressHiddenSwitch(void) {
  if (GameFlag_Get(FLAG_LUNPA_SECRETS_HIDDEN) != 0) {
    return;
  }
  if (Event_GetFlag(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN) != 0) {
    return;
  }
  // Open the passage over five map frames, then remember its final state.
  Actor_SetPosition(ACTOR_SWITCH_SPARKLE, 0, 0);
  Event_PlaySong(SE_HIDDEN_PASSAGE_OPEN);
  WaitFrames(1);
  Lunpa_DrawHiddenPassageFrame(32, 45);
  WaitFrames(10);
  Lunpa_DrawHiddenPassageFrame(41, 45);
  WaitFrames(10);
  Lunpa_DrawHiddenPassageFrame(50, 45);
  WaitFrames(10);
  Lunpa_DrawHiddenPassageFrame(32, 49);
  WaitFrames(10);
  Lunpa_DrawHiddenPassageFrame(41, 49);
  WaitFrames(10);
  GameFlag_Set(FLAG_LUNPA_HIDDEN_PASSAGE_OPEN);
}

/// Copies one open or blocked source cell across the three-cell gateway.
static inline void Lunpa_CopyGatewayCollision(s32 sourceColumn, s32 sourceRow) {
  MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 1, 1, 7, 11);
  MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 1, 1, 8, 11);
  MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 1, 1, 9, 11);
}

/// Lunpa Fortress Gates, MB_LUNPA_FORTRESS_GATES_GUARDED_GROUND: coming close
/// uncloaked, the guards step in front of the fortress door and the gateway
/// closes.
static void Lunpa_GuardsBlockGate(void) {
  if (gCommon.cloaked == 0) {
    Event_Start();
    Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 2);
    Actor_ShowEmote(ACTOR_RIGHT_GUARD, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 15);
    Event_WaitFrames(30);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 168);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 168);
    Actor_WaitForMoveById(ACTOR_LEFT_GUARD);
    Actor_WaitForMoveById(ACTOR_RIGHT_GUARD);
    Actor_Stop(ACTOR_LEFT_GUARD);
    Import_ActorSetAnimation(ACTOR_LEFT_GUARD, 0);
    Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_Stop(ACTOR_RIGHT_GUARD);
    Import_ActorSetAnimation(ACTOR_RIGHT_GUARD, 0);
    Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH + FACING_STEP, 0);
    Event_SetMessageId(MsgLunpaGuardsWarnPartyAway);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    GameFlag_Set(FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR);
    Lunpa_CopyGatewayCollision(6, 11);
    Event_End();
  }
}

static void UNUSED Lunpa_DoNothing(void) {}

/// Lunpa Fortress Gates, casting Cloak: the gateway closes behind the guards.
static void Lunpa_CastCloak(void) {
  Lunpa_CopyGatewayCollision(6, 11);
  GameFlag_Set(FLAG_LUNPA_FORTRESS_GATES_CLOAK_CAST);
}

/// Lunpa Fortress Gates, raised when the guards spot the party: they wonder
/// how the party got there and march them back down to the foot of the steps.
static void Lunpa_GuardsCatchParty(void) {
  struct Actor *leader;
  s32 warningMessageBase;

  if (Event_GetFlag(FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY) != 0) {
    return;
  }
  GameFlag_Set(FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY);
  Event_Start();
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
  Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
  Actor_StartRepeatedMotion(ACTOR_RIGHT_GUARD, 1);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // Warn the party before escorting everyone down the two sections of the gate.
  warningMessageBase = MsgLunpaGuardsCatchParty;
  Event_SetMessageId(warningMessageBase + CATCH_LEFT_GUARD_CHALLENGES);
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Actor_SetSpeed(ACTOR_LEFT_GUARD, PIXELS(2), PIXELS(1));
  Actor_SetSpeed(ACTOR_RIGHT_GUARD, PIXELS(2), PIXELS(1));
  Import_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_SHAKE_HEAD);
  Event_WaitFrames(35);
  Event_SetMessageId(warningMessageBase + CATCH_RIGHT_GUARD_WONDERS);
  Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
  Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  Event_SetMessageId(warningMessageBase + CATCH_LEFT_GUARD_REFUSES_ENTRY);
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
  Import_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_NOD);
  Event_WaitFrames(25);
  Event_SetMessageId(warningMessageBase + CATCH_RIGHT_GUARD_SENDS_PARTY_OFF);
  Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
  Actor_WalkTo(ACTOR_LEFT_GUARD, leader->x.part.pixel - 1,
               leader->z.part.pixel);
  Actor_WaitForMoveById(ACTOR_LEFT_GUARD);
  // Bring the party and both guards together at the first landing.
  Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 216);
  Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 200);
  Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 200);
  Actor_WaitForMoveById(ACTOR_LEFT_GUARD);
  Actor_WaitForMoveById(ACTOR_RIGHT_GUARD);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(12);
  // Continue to the foot of the gate before enabling the watcher again.
  Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 272);
  Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 256);
  Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 256);
  Actor_WaitForMoveById(ACTOR_LEFT_GUARD);
  Actor_WaitForMoveById(ACTOR_RIGHT_GUARD);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Event_End();
  Task_AddFrame(Lunpa_WatchForUncloakedParty, TASK_PASS_FRAME);
}

static void Lunpa_ReopenGateway(void) { Lunpa_CopyGatewayCollision(7, 12); }

/// Lunpa Fortress Gates, as Cloak fades: if the leader is still among the
/// guards, the party is caught.
static void Lunpa_CloakFades(void) {
#if TBS_PATCH_1
  struct FieldWork *work;
#endif
  struct Actor *leader;

#if TBS_PATCH_1
  work = gFieldWork;
#endif
  GameFlag_Clear(FLAG_LUNPA_FORTRESS_GATES_CLOAK_CAST);
  GameFlag_Clear(FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader->x.fixed > PIXELS(104) && leader->x.fixed < PIXELS(240) &&
      leader->z.fixed > PIXELS(160) && leader->z.fixed < PIXELS(248)) {
    Task_Remove(Lunpa_WatchForUncloakedParty);
#if TBS_PATCH_1
    work->raisedTrigger = RAISED_PARTY_SPOTTED;
#else
    Lunpa_GuardsCatchParty();
#endif
  }
  Lunpa_ReopenGateway();
  GameFlag_Clear(FLAG_LUNPA_FORTRESS_GATES_GUARDS_CAUGHT_PARTY);
}

/// Every frame at the Lunpa Fortress Gates: an uncloaked leader in front of
/// the guards is spotted.
static void Lunpa_WatchForUncloakedParty(void) {
  struct FieldWork *work;
  struct Actor *leader;

  work = gFieldWork;
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (gCommon.cloaked == 0 &&
      (u32)(leader->x.fixed - PIXELS(144)) <= PIXELS(32) &&
      leader->z.fixed >= PIXELS(168) && leader->z.fixed < PIXELS(176)) {
    Task_Remove(Lunpa_WatchForUncloakedParty);
    work->raisedTrigger = RAISED_PARTY_SPOTTED;
  }
}

/// Every 16 frames while the leader is thrown: a puff of dust.
static void Lunpa_KickUpLeaderDust(void) {
  struct Actor *leader;
  s32 dustAngle;
  s32 dustVelocity[3];

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if ((gFrameCount & 15) == 0) {
    dustAngle = (((u32)Import_RandomNext() * 52) >> 16) * 64 + 230;
    SET_VECTOR(dustVelocity, Trig_Cos(dustAngle) / 4, 0,
               Trig_Sin(dustAngle) / 2);
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      dustVelocity[0], dustVelocity[1], dustVelocity[2], 0,
                      NULL);
  }
}

/// Lunpa Fortress Gates, LUNPA_FORTRESS_GATES_AFTER_CAUGHT, after being
/// caught in the fortress: the guards throw the leader out and down the steps.
static void Lunpa_GuardsThrowIsaacOut(void) {
  Event_Start();
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(160), PIXELS(128));
  Actor_SetPosition(ACTOR_LEFT_GUARD, PIXELS(152), PIXELS(112));
  Actor_SetPosition(ACTOR_RIGHT_GUARD, PIXELS(168), PIXELS(112));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#ifdef BUGFIX
  Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH + FACING_STEP, 0);
#else
  // BUG: the town's guards, whose actor ids no actor at the gates holds,
  // so the gates' guards keep their headings.
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH + FACING_STEP, 0);
#endif
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  Event_StartScreenTransition();
  Event_WaitFrames(30);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(9) / 5, PIXELS(9) / 10);
  Actor_SetSpeed(ACTOR_LEFT_GUARD, PIXELS(9) / 5, PIXELS(9) / 10);
  Actor_SetSpeed(ACTOR_RIGHT_GUARD, PIXELS(9) / 5, PIXELS(9) / 10);
  Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 288);
  Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 288);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 160, 296);
  Task_AddFrame(Lunpa_KickUpLeaderDust, TASK_PASS_FRAME);
  Event_WaitFrames(1);
  Event_PlaySong(SE_SCUFFLE);
  Event_WaitFrames(20);
  Actor_SetSpritePriority(ACTOR_LEFT_GUARD, 3);
  Actor_SetSpritePriority(ACTOR_RIGHT_GUARD, 3);
  Event_PlaySong(SE_SCUFFLE);
  Event_WaitFrames(30);
  Actor_Get(ACTOR_LEFT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_RIGHT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Event_PlaySong(SE_SCUFFLE);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Import_ActorSetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
  Task_Remove(Lunpa_KickUpLeaderDust);
  // Throw the leader down the steps, then wait for the first landing.
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags |= ACTOR_FALLS;
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = PIXELS(6);
  Actor_Get(ACTOR_PARTY_LEADER)->velocityZ = PIXELS(6);
  Event_WaitFrames(1);
  while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
    Event_WaitFrames(1);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SPRAWLED);
  Event_PlaySong(SE_LANDING_THUD);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Task_AddFrame(Lunpa_KickUpLeaderDust, TASK_PASS_FRAME);
  Event_WaitFrames(2);
  // A smaller second bounce ends with the guards' final warning.
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = PIXELS(3);
  Event_WaitFrames(1);
  while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
    Event_WaitFrames(1);
  }
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Task_Remove(Lunpa_KickUpLeaderDust);
  Event_WaitFrames(50);
  Event_SetMessageId(MsgLunpaGuardForbidsReturn);
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
  Actor_Get(ACTOR_LEFT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_RIGHT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpeed(ACTOR_LEFT_GUARD, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_RIGHT_GUARD, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_LEFT_GUARD, 144, 200);
  Actor_WalkTo(ACTOR_RIGHT_GUARD, 176, 200);
  Actor_WaitForMoveById(ACTOR_LEFT_GUARD);
  Actor_WaitForMoveById(ACTOR_RIGHT_GUARD);
  Import_ActorSetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH + FACING_STEP, 0);
  Event_End();
}

/// Lunpa Fortress Gates, LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA,
/// after slipping out of the fortress: the leader casts Cloak and slips round
/// the guards.
static void Lunpa_LeaderSneaksOut(void) {
  Event_Start();
  Event_StartScreenTransition();
  Actor_WalkTo(ACTOR_PARTY_LEADER, 152, 168);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Event_WaitFrames(20);
  // Cast Cloak before following the route around the guards.
  FieldPsynergy_LoadResources(ABILITY_CLOAK, 1);
  FieldPsynergy_SetupActorPair(ACTOR_PARTY_LEADER, 0);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  FieldPsynergy_StopCasterUpdate();
  Actor_WalkTo(ACTOR_PARTY_LEADER, 144, 184);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 184);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 200);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 72, 200);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 72, 288);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 288);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Event_End();
}

/// Sets Lunpa or the Lunpa Fortress Gates up: in the town, Reveal's secrets
/// hidden and the floating Nut; at the gates, the thrown-out or sneaking-out
/// event, the watchers, and the fortress's puzzle and guard flags cleared.
s32 Scene_Initialize(void) {
  struct Actor *puddle;

  if (gCommon.sceneId == (s32)&SceneId_Lunpa) {
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Lunpa_HideSecrets();
    if (GameFlag_Get(FLAG_LUNPA_NUT_CAUGHT) == 0) {
      Scene_FloatNut(ACTOR_FLOATING_NUT);
    }
    puddle = Actor_GetById(ACTOR_HIDDEN_PUDDLE);
    if (puddle != NULL) {
      Actor_SetSpriteFlags(puddle, 0);
    }
    GameFlag_Set(FLAG_LUNPA_SECRETS_HIDDEN);
  }
  // Restore the gate's arrival event and active detection tasks separately.
  if (gCommon.sceneId == (s32)&SceneId_LunpaFortressGates) {
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gCommon.retreatEntrance = GATE_RETREAT_ENTRANCE;
    if (gCommon.entrance == LUNPA_FORTRESS_GATES_AFTER_CAUGHT &&
        GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Lunpa_GuardsThrowIsaacOut();
    }
    if (gCommon.entrance == LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA &&
        GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Lunpa_LeaderSneaksOut();
    }
    if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0 &&
        GameFlag_Get(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
      Task_AddFrame(Lunpa_WatchForFortressApproach, TASK_PASS_FRAME);
    }
#if TBS_PATCH_4
    if (gCommon.cloaked != 0)
      Lunpa_CastCloak();
#endif
    Task_AddFrame(Lunpa_WatchForUncloakedParty, TASK_PASS_FRAME);
    // The fortress's puzzles and beaten guards are reset.
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_IVY_BLOWN_AWAY);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_PASSAGE_IVY_BLOWN_AWAY);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_IVY_BLOWN_AWAY);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_CRATES_MOVED);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_MIDDLE_GUARD_BEATEN);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN);
  }
  return 0;
}

#include "shared/actor_orbit.h"

#define LUNPA_ENTRANCE(entrance, x, z, facing)                                 \
  {(entrance), CONDITION_ALWAYS, (x), 0, (z), (facing), 0, -1, -1, -1, -1, 0}
#define GATE_ENTRANCE(entrance, x, z, facing)                                  \
  {(entrance), CONDITION_ALWAYS, (x), 0, (z), (facing), 0, 32, 48, 272, 344, 0}

struct SceneEntrance gLunpaEntrances[] = {
    LUNPA_ENTRANCE(0, 172, 149, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_WORLD_MAP, 224, 472, FACING_NORTH),
    LUNPA_ENTRANCE(LUNPA_FROM_LUNPA_FORTRESS_GATES, 232, 32, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_WEST_LUNPA_CAVE, 120, 104, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_FIRST_HOME, 248, 184, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_SECOND_HOME, 344, 232, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_THIRD_HOME, 344, 328, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_WEAPON_AND_ARMOR_SHOP, 232, 360, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_ITEM_SHOP, 328, 120, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_INN, 104, 184, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_TEMPLE, 48, 168, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_CRATE_ROOM, 216, 344, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_STONE_ROOMS, 40, 328, FACING_SOUTH),
    {SCENE_TABLE_END},
};

struct SceneEntrance gGateEntrances[] = {
    GATE_ENTRANCE(0, 152, 328, FACING_NORTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_GROUND_FLOOR, 160,
                  104, FACING_SOUTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA, 152, 328, FACING_NORTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA, 144, 104,
                  FACING_SOUTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_AFTER_CAUGHT, 160, 224, FACING_EAST),
    {SCENE_TABLE_END},
};

#define ROOM_EXIT(exit) SCENE_EXIT((exit), RockRooms, ROOM_ENTRANCE(exit))

u32 gLunpaExits[] = {
    SCENE_EXITS(Lunpa),
    SCENE_EXIT(LUNPA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LUNPA),
    SCENE_EXIT(LUNPA_TO_LUNPA_FORTRESS_GATES, LunpaFortressGates,
               LUNPA_FORTRESS_GATES_FROM_LUNPA),
    SCENE_EXIT(LUNPA_TO_WEST_LUNPA_CAVE, WestLunpaCave,
               WEST_LUNPA_CAVE_FROM_LUNPA),
    ROOM_EXIT(LUNPA_TO_FIRST_HOME),
    ROOM_EXIT(LUNPA_TO_SECOND_HOME),
    ROOM_EXIT(LUNPA_TO_THIRD_HOME),
    ROOM_EXIT(LUNPA_TO_WEAPON_AND_ARMOR_SHOP),
    ROOM_EXIT(LUNPA_TO_ITEM_SHOP),
    ROOM_EXIT(LUNPA_TO_INN),
    ROOM_EXIT(LUNPA_TO_TEMPLE),
    ROOM_EXIT(LUNPA_TO_CRATE_ROOM),
    SCENE_EXIT(LUNPA_TO_STONE_ROOMS, StoneRooms, STONE_ROOMS_FROM_LUNPA),
    SCENE_EXITS_END,
};

u32 gGateExits[] = {
    SCENE_EXITS(LunpaFortressGates),
    SCENE_EXIT(LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_GROUND_FLOOR,
               LunpaFortressGroundFloor,
               LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_GATES),
    SCENE_EXIT(LUNPA_FORTRESS_GATES_TO_LUNPA, Lunpa,
               LUNPA_FROM_LUNPA_FORTRESS_GATES),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorTemplate gLunpaActorTemplates[] = {
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(520), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(112), 0, PIXELS(168), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(280), 0, PIXELS(352), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(120), 0, PIXELS(312), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(136), 0, PIXELS(328), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
      PIXELS(368), 0, PIXELS(256), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(280), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(216), 0, PIXELS(40), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
      PIXELS(152), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(192), 0, PIXELS(448), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(256), 0, PIXELS(448), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(520), 0, PIXELS(520), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, FLAG_LUNPA_NUT_CAUGHT, MOVEMENT_TYPE_RESET_ACTION,
      PIXELS(120), 0, PIXELS(472), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END, CONDITION_ALWAYS },
};

struct ActorTemplate gGateActorTemplates[] = {
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(144), 0, PIXELS(200), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(176), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

#define TALK(actorId, value)                                                   \
  {TRIGGER_TALK, (actorId), CONDITION_ALWAYS, (value)}
#define MIND_READ(actorId, value)                                              \
  {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), (actorId), CONDITION_ALWAYS, (value)}

#define DOOR(trigger)                                                          \
  {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), (trigger), CONDITION_ALWAYS,       \
   EVENT(Lunpa_EnterDoor)}

#define LUNPA_ROADS_AND_DOORS                                                  \
  {TRIGGER_EXIT, MB_LUNPA_WORLD_MAP_ROAD, CONDITION_ALWAYS,                    \
   LUNPA_TO_WORLD_MAP},                                                        \
      {TRIGGER_EXIT, MB_LUNPA_GATE_ROAD, CONDITION_ALWAYS,                     \
       LUNPA_TO_LUNPA_FORTRESS_GATES},                                         \
      {TRIGGER_EXIT, MB_LUNPA_CAVE_ROAD, CONDITION_ALWAYS,                     \
       LUNPA_TO_WEST_LUNPA_CAVE},                                              \
      DOOR(MB_LUNPA_FIRST_HOME_DOOR), DOOR(MB_LUNPA_SECOND_HOME_DOOR),         \
      DOOR(MB_LUNPA_THIRD_HOME_DOOR), DOOR(MB_LUNPA_ARMS_SHOP_DOOR),           \
      DOOR(MB_LUNPA_ITEM_SHOP_DOOR), DOOR(MB_LUNPA_INN_DOOR),                  \
      DOOR(MB_LUNPA_TEMPLE_DOOR), DOOR(MB_LUNPA_EIGHTH_ROOM_DOOR), {           \
    STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_LUNPA_HIDDEN_PASSAGE,          \
        CONDITION_ALWAYS, EVENT(Lunpa_EnterHiddenPassage)                      \
  }

#define LUNPA_SECRETS                                                          \
  {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_HIDDEN_PUDDLE,                      \
   FLAG_LUNPA_PUDDLE_FROZEN, EVENT(Lunpa_FreezeHiddenPuddle)},                 \
      {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,              \
       CONDITION_ALWAYS, EVENT(Lunpa_RevealSecrets)},                          \
      {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,             \
       CONDITION_ALWAYS, EVENT(Lunpa_HideSecrets)},                            \
      {EXAMINE_TRIGGER(EXAMINE_SILENT), MB_LUNPA_HIDDEN_SWITCH,                \
       FLAG_LUNPA_SECRETS_HIDDEN, EVENT(Lunpa_PressHiddenSwitch)},             \
      {EXAMINE_TRIGGER(EXAMINE_SILENT), MB_LUNPA_PSYNERGY_STONE,               \
       FLAG_LUNPA_PSYNERGY_STONE, FIND_PSYNERGY_STONE},                        \
      {EXAMINE_TRIGGER(EXAMINE_GRAVE), MB_LUNPA_GRAVE_SLEEP_BOMB,              \
       FLAG_LUNPA_GRAVE_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB)},               \
      {EXAMINE_TRIGGER(EXAMINE_BARREL), MB_LUNPA_BARREL_NUT,                   \
       FLAG_LUNPA_BARREL_NUT, FIND_ITEM(ITEM_NUT)},                            \
      {PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FLOATING_NUT,                   \
       FLAG_LUNPA_NUT_CAUGHT, EVENT(Lunpa_CatchFloatingNut)},                  \
      {EXAMINE_TRIGGER(EXAMINE_SILENT), MB_LUNPA_WELL, CONDITION_ALWAYS,       \
       EVENT(Lunpa_SearchWell)},                                               \
  {                                                                            \
    TRIGGERS_END                                                               \
  }

struct Trigger gLunpaSealedTriggers[] = {
    LUNPA_ROADS_AND_DOORS,
    TALK(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA, EVENT(Lunpa_AskAboutLengthOfStay)),
    TALK(ACTOR_WOMAN_WITH_NO_FREEDOM, MSGNO(MsgLunpaVillageVillagerBSealed)),
    TALK(ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
         EVENT(Lunpa_AskAboutHammetsKidnapping)),
    TALK(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
         MSGNO(MsgLunpaVillageVillagerDSealed)),
    TALK(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
         MSGNO(MsgLunpaVillageVillagerESealed)),
    TALK(ACTOR_OLD_WOMAN_ON_DONPA, MSGNO(MsgLunpaVillageVillagerFSealed)),
    TALK(ACTOR_BOY_ASKING_TO_SEE_DONPA, EVENT(Lunpa_AskAboutSeeingDonpa)),
    TALK(ACTOR_GIRL_ON_THE_CHEFS_MEALS, MSGNO(MsgLunpaVillageVillagerHSealed)),
    TALK(ACTOR_WEST_GUARD, EVENT(Lunpa_TalkToWestGuard)),
    TALK(ACTOR_EAST_GUARD, EVENT(Lunpa_TalkToEastGuard)),
    MIND_READ(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
              MSGNO(MsgLunpaVillageVillagerASealedThoughts)),
    MIND_READ(ACTOR_WOMAN_WITH_NO_FREEDOM,
              MSGNO(MsgLunpaVillageVillagerBSealedThoughts)),
    MIND_READ(ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
              MSGNO(MsgLunpaVillageVillagerCSealedThoughts)),
    MIND_READ(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
              MSGNO(MsgLunpaVillageVillagerDSealedThoughts)),
    MIND_READ(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
              MSGNO(MsgLunpaVillageVillagerESealedThoughts)),
    MIND_READ(ACTOR_OLD_WOMAN_ON_DONPA,
              MSGNO(MsgLunpaVillageVillagerFSealedThoughts)),
    MIND_READ(ACTOR_BOY_ASKING_TO_SEE_DONPA,
              MSGNO(MsgLunpaVillageVillagerGSealedThoughts)),
    MIND_READ(ACTOR_GIRL_ON_THE_CHEFS_MEALS,
              MSGNO(MsgLunpaVillageVillagerHSealedThoughts)),
    MIND_READ(ACTOR_WEST_GUARD, MSGNO(MsgLunpaVillageWestGuardSealedThoughts)),
    MIND_READ(ACTOR_EAST_GUARD, EVENT(Lunpa_ReadEastGuardsThoughts)),
    LUNPA_SECRETS,
};

struct Trigger gLunpaReopenedTriggers[] = {
    LUNPA_ROADS_AND_DOORS,
    TALK(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
         MSGNO(MsgLunpaVillageVillagerAReopened)),
    TALK(ACTOR_WOMAN_WITH_NO_FREEDOM, EVENT(Lunpa_ComplainAboutCold)),
    TALK(ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
         MSGNO(MsgLunpaVillageVillagerCReopened)),
    TALK(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
         EVENT(Lunpa_AskAboutFortressCommotion)),
    TALK(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
         MSGNO(MsgLunpaVillageVillagerEReopened)),
    TALK(ACTOR_OLD_WOMAN_ON_DONPA, MSGNO(MsgLunpaVillageVillagerFReopened)),
    TALK(ACTOR_BOY_ASKING_TO_SEE_DONPA,
         MSGNO(MsgLunpaVillageVillagerGReopened)),
    TALK(ACTOR_GIRL_ON_THE_CHEFS_MEALS,
         MSGNO(MsgLunpaVillageVillagerHReopened)),
    TALK(ACTOR_WEST_GUARD, MSGNO(MsgLunpaVillageWestGuardReopened)),
    TALK(ACTOR_EAST_GUARD, MSGNO(MsgLunpaVillageEastGuardReopened)),
    MIND_READ(ACTOR_MAN_ASKING_HOW_LONG_IN_LUNPA,
              MSGNO(MsgLunpaVillageVillagerAReopenedThoughts)),
    MIND_READ(ACTOR_WOMAN_WITH_NO_FREEDOM,
              MSGNO(MsgLunpaVillageVillagerBReopenedThoughts)),
    MIND_READ(ACTOR_MAN_ON_HAMMETS_KIDNAPPING,
              MSGNO(MsgLunpaVillageVillagerCReopenedThoughts)),
    MIND_READ(ACTOR_WOMAN_SAYING_DONPA_IS_TOO_OLD,
              MSGNO(MsgLunpaVillageVillagerDReopenedThoughts)),
    MIND_READ(ACTOR_OLD_MAN_ON_LUNPA_AND_DONPA,
              MSGNO(MsgLunpaVillageVillagerEReopenedThoughts)),
    MIND_READ(ACTOR_OLD_WOMAN_ON_DONPA,
              MSGNO(MsgLunpaVillageVillagerFReopenedThoughts)),
    MIND_READ(ACTOR_BOY_ASKING_TO_SEE_DONPA,
              MSGNO(MsgLunpaVillageVillagerGReopenedThoughts)),
    MIND_READ(ACTOR_GIRL_ON_THE_CHEFS_MEALS,
              MSGNO(MsgLunpaVillageVillagerHReopenedThoughts)),
    MIND_READ(ACTOR_WEST_GUARD,
              MSGNO(MsgLunpaVillageWestGuardReopenedThoughts)),
    MIND_READ(ACTOR_EAST_GUARD, EVENT(Lunpa_ReadEastGuardsThoughts)),
    LUNPA_SECRETS,
};

// clang-format off
struct Trigger gGateTriggers[] = {
    { TRIGGER_EXIT, MB_LUNPA_FORTRESS_GATES_LUNPA_ROAD, CONDITION_ALWAYS, LUNPA_FORTRESS_GATES_TO_LUNPA },
    // XXX (no evidence): where the third road was meant to lead; gGateExits
    // has no row for it, so the engine sends it to LunpaFortressGroundFloor's
    // default entrance.
    { TRIGGER_EXIT, MB_LUNPA_FORTRESS_GATES_THIRD_ROAD, CONDITION_ALWAYS, GATE_EXIT_THIRD },
    { PSYNERGY_ANYWHERE(ABILITY_CLOAK, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Lunpa_CastCloak) },
    { PSYNERGY_ANYWHERE(ABILITY_CLOAK, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, EVENT(Lunpa_CloakFades) },
    { TRIGGER_STEP, MB_LUNPA_FORTRESS_GATES_FORTRESS_DOOR, FLAG_LUNPA_FORTRESS_GATES_ENTERING_FORTRESS,
      EVENT(Lunpa_EnterFortress) },
    { TRIGGER_STEP, MB_LUNPA_FORTRESS_GATES_GUARDED_GROUND, FLAG_LUNPA_FORTRESS_GATES_GUARDS_AT_DOOR,
      EVENT(Lunpa_GuardsBlockGate) },
    { TRIGGER_STEP, MB_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH, CONDITION_ALWAYS,
      EVENT(Lunpa_GaretTurnsIsaacBack) },
    { TRIGGER_RAISED, RAISED_PARTY_SPOTTED, CONDITION_ALWAYS, EVENT(Lunpa_GuardsCatchParty) },
    TALK(ACTOR_LEFT_GUARD, EVENT(Lunpa_TalkToLeftGuard)),
    TALK(ACTOR_RIGHT_GUARD, EVENT(Lunpa_TalkToRightGuard)),
    MIND_READ(ACTOR_LEFT_GUARD, EVENT(Lunpa_ReadLeftGuardsThoughts)),
    MIND_READ(ACTOR_RIGHT_GUARD, EVENT(Lunpa_ReadRightGuardsThoughts)),
    { EXAMINE_TRIGGER(EXAMINE_SILENT), MB_LUNPA_FORTRESS_GATES_WELL, CONDITION_ALWAYS, EVENT(Lunpa_SearchWell) },
    { TRIGGERS_END },
};
// clang-format on

u16 gLunpaDoorSteps[DOOR_COUNT][11] = {
    {MAPGRID_ANIMATION_STEP(38, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(40, 35, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(42, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(44, 35, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(46, 33, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(48, 33, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(46, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(48, 35, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(50, 33, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(52, 33, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(50, 35, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(52, 35, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(42, 33, 4, 2, 5),
     MAPGRID_ANIMATION_STEP(38, 33, 4, 2, 5), MAPGRID_ANIMATION_STEPS_END},
    {MAPGRID_ANIMATION_STEP(54, 33, 2, 2, 5),
     MAPGRID_ANIMATION_STEP(56, 33, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END},
};

/// Each door's opening animation and the map grid cell it plays at, by its
/// step trigger from MB_LUNPA_FIRST_HOME_DOOR.
struct MapGridCellAnimation gLunpaDoors[] = {
    {gLunpaDoorSteps[0], 46, 9},  {gLunpaDoorSteps[1], 52, 12},
    {gLunpaDoorSteps[2], 52, 18}, {gLunpaDoorSteps[3], 45, 20},
    {gLunpaDoorSteps[4], 51, 5},  {gLunpaDoorSteps[5], 37, 9},
    {gLunpaDoorSteps[6], 33, 6},  {gLunpaDoorSteps[7], 44, 17},
};

/// Unused: a zero word nothing reads.
u32 gLunpaUnusedWord = 0;
