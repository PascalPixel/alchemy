/// @file
/// Lunpa, the town of the noble thief's family, sealed by Dodonpa's men while
/// he holds Hammet for ransom in Lunpa Fortress. Two map sections share this
/// overlay, each a map of its own:
///
/// - Lunpa (MapId_Lunpa, area name Lunpa): the town. Two guards keep the
///   road in from the world map; doors lead into the homes, shops, inn and
///   temple (LunpaInteriors, lunpa_interiors.c), roads to West Lunpa Cave and
///   up to the Lunpa Fortress Gates, and a hidden passage to the jail
///   (LUNPA_TO_VAULT_JAIL, whose area name there is Lunpa).
/// - Lunpa Fortress Gates (MapId_LunpaFortressGates, area name Lunpa
///   Fortress Gates): the steps up to the fortress door, kept by two guards
///   whom the party gets past under Cloak.
///
/// The getters choose by gGameState.mapId, and the town's map events also by
/// FLAG_DODONPA_LOCKED_UP, which Donpa's judgment in the fortress sets: before
/// it the townspeople live under Dodonpa's men (gLunpaSealedEvents), after it
/// they talk of the intruders (gLunpaReopenedEvents). Both tables share the
/// roads and doors (LUNPA_ROADS_AND_DOORS) and Reveal's secrets
/// (LUNPA_SECRETS).
///
/// The town hides three things that Reveal shows (EventScript_RevealSecrets)
/// and that every entry and the end of Reveal hide again
/// (EventScript_HideSecrets): a Psynergy Stone, a puddle that Frost freezes
/// into a pillar to stand on, and the sparkle of a switch. Searching the switch
/// opens the passage to the jail, which can be walked through only while Reveal
/// shows it (EventScript_EnterHiddenPassage). Its map temp flags:
///
/// - FLAG_MAP_TEMP_00: Frost has frozen the puddle.
/// - FLAG_MAP_TEMP_01: the secrets are hidden, Reveal not shining.
/// - FLAG_MAP_TEMP_02: the switch has opened the passage.
///
/// At the gates, stepping uncloaked on the ground before the guards
/// (MB_LUNPA_FORTRESS_GATES_GUARDED_GROUND) brings them in front of the door,
/// and an uncloaked leader among them, or one still among them as Cloak fades,
/// is marched back down the steps (EventScript_GuardsCatchParty, raised as
/// RAISED_PARTY_SPOTTED by Lunpa_WatchForUncloakedParty). A party caught in
/// the fortress arrives by LUNPA_FORTRESS_GATES_AFTER_CAUGHT and is thrown
/// down the steps; after the rescue it leaves the fortress by
/// LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA and sneaks past under
/// Cloak. Every entry clears the fortress's puzzle and beaten-guard flags, so
/// the fortress starts over. Between Donpa's judgment and the reunion in West
/// Lunpa Cave (FLAG_LUNPA_CAVE_REUNION_SEEN), Garet stops the leader from
/// going back up (MB_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH). Its map temp
/// flags:
///
/// - FLAG_MAP_TEMP_40: the guards stand before the door.
/// - FLAG_MAP_TEMP_41: Cloak has been cast at the gates.
/// - FLAG_MAP_TEMP_42: the leader is going through the fortress door.
/// - FLAG_MAP_TEMP_43: Garet's warning is under way.
/// - FLAG_MAP_TEMP_44: the guards are marching the party down.
///
/// Open questions:
/// - XXX (no evidence): where GATE_WARP_THIRD, the gates' third road, was
///   meant to lead; gGateWarps has no row for it, so the engine's fallback
///   sends it to LunpaFortressGroundFloor's default warp destination, and
///   nothing else in the ROM names a destination for it.

#include "game_state.h"
#include "version.h"
#include "map_ids.h"
#include "unit/map/lunpa.h"
#include "unit/map/common/object_orbit.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "types.h"
#include "field_effect.h"
#include "script_layer.h"
#include "engine_call.h"
#include "map_tables.h"
#include "map_header.h"
#include "edition.h"
#include "lunpa.h"
#include "text/msg_ids.h"
#include "object_event.h"
#include "macros.h"

static void Lunpa_WatchForUncloakedParty(void);

enum { PALETTE_TRANSLUCENT_EFFECT = 14, PALETTE_EFFECT = 15 };

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// How long an effect of common/effect_spawn.h lasts: 22, 44 or 126
/// frames, then it moves away and is freed.
s32 gShortEffectScript[] = {
    SCRIPT_WAIT(1), SCRIPT_REPEAT(22, 0),
    SCRIPT_WAIT(1), SCRIPT_SET_POSITION(0, 0, 0),
    SCRIPT_WAIT(1), SCRIPT_DELETE,
};

s32 gMediumEffectScript[] = {
    SCRIPT_WAIT(1), SCRIPT_REPEAT(44, 0),
    SCRIPT_WAIT(1), SCRIPT_SET_POSITION(0, 0, 0),
    SCRIPT_WAIT(1), SCRIPT_DELETE,
};

s32 gLongEffectScript[] = {
    SCRIPT_WAIT(1), SCRIPT_REPEAT(126, 0),
    SCRIPT_WAIT(1), SCRIPT_SET_POSITION(0, 0, 0),
    SCRIPT_WAIT(1), SCRIPT_DELETE,
};

const s32 *gEffectScripts[] = {
    gShortEffectScript,
    gMediumEffectScript,
    gLongEffectScript,
};

const struct MapWarpDest *Map_GetWarpDests(void) {
  s32 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_Lunpa) {
    return gLunpaWarpDests;
  }
  if (mapId == (s32)&MapId_LunpaFortressGates) {
    return gGateWarpDests;
  }
  return gLunpaWarpDests;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) {
  s32 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_Lunpa) {
    return gLunpaWarps;
  }
  if (mapId == (s32)&MapId_LunpaFortressGates) {
    return gGateWarps;
  }
  return gLunpaWarps;
}

/// The townsfolk of Lunpa, or the guards of the Lunpa Fortress Gates.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  s32 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_Lunpa) {
    return gLunpaObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_LunpaFortressGates) {
    return gGateObjectEventTemplates;
  }
  return gLunpaObjectEventTemplates;
}

/// Lunpa: Catch on the Nut floating over the town gives it to the party.
static void EventScript_CatchFloatingNut(void) {
  FieldScript_Start();
  Engine_ObjectEventSetPosition(LOCALID_FLOATING_NUT, 0, 0);
  Engine_GameFlagSet(FLAG_LUNPA_NUT_CAUGHT);
  FieldEffect_StartEffectObject22(ITEM_NUT, 3);
  PartyInventory_GiveItem(ITEM_NUT, 0);
  FieldScript_End();
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
static void EventScript_FreezeHiddenPuddle(void) {
  struct ObjectEvent *pillar;

  pillar = ObjectEvent_Get(LOCALID_HIDDEN_PUDDLE);
  ObjectEvent_Get(LOCALID_PARTY_LEADER);
  Lunpa_ApplyFrozenPuddleCollision();
  if (pillar != NULL) {
    ObjectEvent_SetSpriteFlags(pillar, 0);
    pillar->motionFlags = OBJ_EVENT_FALLS;
    pillar->priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
  }
  GameFlag_Set(FLAG_MAP_TEMP_00);
}

/// Every frame while the pillar stands: draw it under the leader once the
/// leader is on top.
static void Lunpa_UpdateIcePillarDrawOrder(void) {
  if (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed >= PILLAR_TOP_HEIGHT) {
    ObjectEvent_GetById(LOCALID_HIDDEN_PUDDLE)->priorityFlags |=
        OBJ_EVENT_PRIORITY_UNDERFOOT;
  } else {
    ObjectEvent_GetById(LOCALID_HIDDEN_PUDDLE)->priorityFlags =
        OBJ_EVENT_PRIORITY_AUTOMATIC;
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
/// sparkle, the puddle and, once opened, the passage to the jail show.
static void EventScript_RevealSecrets(void) {
  struct ObjectEvent *objectEvent;
  s32 cellX;
  s32 cellZ;

  // Restore the hidden stone and pillar before exposing the switch or passage.
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  cellX = objectEvent->x.fixed / CELL_SIZE;
  cellZ = objectEvent->z.fixed / CELL_SIZE;
  if (GameFlag_IsSet(FLAG_LUNPA_PSYNERGY_STONE) == 0) {
    if (cellX == 7 && cellZ == 16) {
      ObjectEvent_WalkByAndWait(LOCALID_PARTY_LEADER, 0, 16);
    }
    TreasureSpot_SetPosition(TREASURE_SPOT_PSYNERGY_STONE, -1, -1);
    MapGrid_CopyCellAttributes(28, 31, 1, 1, 7, 16);
  }
  MapGrid_CopyCells(47, 4, 1, 1, 46, 4);
  MapGrid_CopyCellAttributes(34, 37, 3, 3, 13, 3);
  ObjectEvent_SetPosition(LOCALID_HIDDEN_PUDDLE, PIXELS(232), PIXELS(72));
  ObjectEvent_Get(LOCALID_HIDDEN_PUDDLE)->y.fixed = 0;
#if TBS_PATCH_4
  if (GameFlag_IsSet(FLAG_MAP_TEMP_00)) {
    Lunpa_ApplyFrozenPuddleCollision();
  }
#endif
  // The opened passage replaces the sparkle; otherwise show the hidden switch.
  if (GameFlag_IsSet(FLAG_MAP_TEMP_02) != 0) {
    Lunpa_DrawHiddenPassageFrame(41, 49);
  } else {
    ObjectEvent_SetPosition(LOCALID_SWITCH_SPARKLE, PIXELS(56), PIXELS(268));
    ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_SWITCH_SPARKLE), 0);
    objectEvent = ObjectEvent_Get(LOCALID_SWITCH_SPARKLE);
    if (objectEvent != NULL) {
      objectEvent->motionFlags = OBJ_EVENT_FLOATS_HIGH;
      objectEvent->y.fixed = PIXELS(16);
      objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
      objectEvent->scaleX = SPARKLE_SCALE_X;
      objectEvent->scaleY = SPARKLE_SCALE_Y;
    }
  }
  FieldScheduler_AddCallback(Lunpa_UpdateIcePillarDrawOrder, TASK_PRIORITY_MAP);
  GameFlag_Clear(FLAG_MAP_TEMP_01);
}

/// Lunpa, as Reveal ends and on every entry: the secrets hide again.
static void EventScript_HideSecrets(void) {
  struct ObjectEvent *puddle;

  ObjectEvent_Get(LOCALID_PARTY_LEADER);
  ObjectEvent_SetPosition(LOCALID_SWITCH_SPARKLE, 0, 0);
  ObjectEvent_SetPosition(LOCALID_HIDDEN_PUDDLE, 0, 0);
  MapGrid_CopyCells(38, 38, 1, 1, 46, 4);
  MapGrid_CopyCellAttributes(37, 37, 3, 3, 13, 3);
  MapGrid_CopyCellAttributes(37, 37, 1, 1, 14, 2);
  MapGrid_CopyCellAttributes(8, 16, 1, 1, 7, 16);
  TreasureSpot_SetPosition(TREASURE_SPOT_PSYNERGY_STONE, 0, 0);
  MapGrid_CopyCells(32, 42, 3, 2, 1, 15);
  GameFlag_Clear(FLAG_MAP_TEMP_00);
  Engine_ObjectEventSetAnimation(LOCALID_HIDDEN_PUDDLE, ANIM_STAND);
  puddle = ObjectEvent_Get(LOCALID_HIDDEN_PUDDLE);
  puddle->update = NULL;
  ObjectEvent_SetColorMode(ObjectEvent_Get(LOCALID_HIDDEN_PUDDLE), 0);
  Task_Remove(Lunpa_UpdateIcePillarDrawOrder);
  GameFlag_Set(FLAG_MAP_TEMP_01);
}

/// Searching the well (MB_LUNPA_WELL, MB_LUNPA_FORTRESS_GATES_WELL), in Lunpa
/// or at the Lunpa Fortress Gates: frogs croak in it.
static void EventScript_SearchWell(void) {
  FieldScript_Start();
  FieldMessage_Show(MsgFieldPeeredWell, 1);
  FieldMessage_Show(MsgLunpaWellFrogs, 1);
  FieldScript_End();
}

/// The map events of the Lunpa Fortress Gates, or the town's before and after
/// Donpa's judgment.
const struct MapEvent *Map_GetEvents(void) {
  s32 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_LunpaFortressGates) {
    GameFlag_Test(FLAG_DODONPA_LOCKED_UP);
    return gGateEvents;
  }
  if (mapId == (s32)&MapId_Lunpa &&
      GameFlag_Test(FLAG_DODONPA_LOCKED_UP) != 0) {
    return gLunpaReopenedEvents;
  }
  return gLunpaSealedEvents;
}

/// Lunpa, before Donpa's judgment: the west guard on the road in asks whether
/// the party wants to enter Lunpa, and turns them away either way.
static void EventScript_TalkToWestGuard(void) {
  FieldScript_Start();
  FieldEffect_SpawnLinked(LOCALID_WEST_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  FieldScript_SetMessageId(MsgLunpaWestGuardAsksAboutEntering);
  FieldScript_AskYesNoAndShowResponse(LOCALID_WEST_GUARD, 0);
  FieldScript_End();
}

/// Lunpa, before Donpa's judgment: the east guard on the road in wants
/// Dodonpa's authorization, or, once the guards suspect Kalay, asks whether
/// the party came from there and whether they are here to save Hammet.
static void EventScript_TalkToEastGuard(void) {
  FieldScript_Start();
  if (GameFlag_Test(FLAG_LUNPA_GUARDS_SUSPECT_KALAY) == 0) {
    FieldScript_SetMessageId(MsgLunpaEastGuardDemandsAuthorization);
    FieldScript_ShowMessageAndWait(LOCALID_EAST_GUARD, 0);
  } else {
    FieldScript_SetMessageId(MsgLunpaEastGuardAsksIfFrom);
    FieldMessage_ShowAtObject(LOCALID_EAST_GUARD, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
        0) {
      gFieldWork->message++;
      FieldMessage_ShowAtObject(LOCALID_EAST_GUARD, 0);
      if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER,
                                                       0) == 1) {
        gFieldWork->message++;
      }
    }
    FieldScript_ShowMessageAndWait(LOCALID_EAST_GUARD, 0);
  }
  FieldScript_End();
}

/// Lunpa: Mind Read on the east guard. He fears Dodonpa's blame, trusts the
/// cave's gate once he suspects Kalay, and after Donpa's judgment thinks the
/// merchant who was desperate to do business in the village is still around
/// somewhere, causing no trouble.
static void EventScript_ReadEastGuardsThoughts(void) {
  s32 thoughts;

  if (GameFlag_Test(FLAG_DODONPA_LOCKED_UP) == 0) {
    thoughts = GameFlag_Test(FLAG_LUNPA_GUARDS_SUSPECT_KALAY);
    if (thoughts == 0) {
      thoughts = MsgLunpaEastGuardFearsBlame;
    } else {
      thoughts = MsgLunpaEastGuardTrustsCaveGate;
    }
    FieldScript_SetMessageId(thoughts);
    FieldScript_ShowMessageAndWait(LOCALID_EAST_GUARD, 0);
  } else {
    FieldScript_SetMessageId(MsgLunpaEastGuardThinksMerchantHarmless);
    FieldScript_ShowMessageAndWait(LOCALID_EAST_GUARD, 0);
  }
}

/// Lunpa, before Donpa's judgment: a man asks how long the party has been
/// in Lunpa.
static void EventScript_AskAboutLengthOfStay(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgLunpaVillagerAAsksHowLong);
  FieldScript_AskYesNoAndShowResponse(LOCALID_MAN_ASKING_HOW_LONG_IN_LUNPA, 0);
  FieldScript_End();
}

/// Lunpa, before Donpa's judgment: a stout man asks whether Dodonpa
/// kidnapped Hammet for ransom.
static void EventScript_AskAboutHammetsKidnapping(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgLunpaVillagerCAsksAboutKidnapping);
  FieldScript_AskYesNoAndShowResponse(LOCALID_MAN_ON_HAMMETS_KIDNAPPING, 0);
  FieldScript_End();
}

/// Lunpa, before Donpa's judgment: a small boy asks whether the party wants
/// to see Donpa, who is in the fortress.
static void EventScript_AskAboutSeeingDonpa(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgLunpaVillagerGAsksAboutDonpa);
  FieldScript_AskYesNoAndShowResponse(LOCALID_BOY_ASKING_TO_SEE_DONPA, 0);
  FieldScript_End();
}

/// Lunpa, after Donpa's judgment: a woman shivers and complains that it
/// has become colder.
static void EventScript_ComplainAboutCold(void) {
  FieldScript_Start();
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WOMAN_WITH_NO_FREEDOM, 3);
  FieldScript_SetMessageId(MsgLunpaVillagerBShivers);
  FieldScript_ShowMessageAndWait(LOCALID_WOMAN_WITH_NO_FREEDOM, 0);
  FieldScript_End();
}

/// Lunpa, after Donpa's judgment: a woman offers to explain the commotion
/// at the fortress.
static void EventScript_AskAboutFortressCommotion(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgLunpaVillagerDAsksAboutCommotion);
  FieldScript_AskYesNoAndShowResponse(LOCALID_WOMAN_SAYING_DONPA_IS_TOO_OLD, 0);
  FieldScript_End();
}

/// Lunpa Fortress Gates: the left guard hears someone while the party is
/// cloaked; after Donpa's judgment, he nearly recognizes Hammet with the
/// party.
static void EventScript_TalkToLeftGuard(void) {
  s32 recognitionMessageBase;

  if (gGameState.cloaked != 0) {
    FieldScript_SetMessageId(MsgLunpaLeftGuardHearsSomeone);
  } else if (GameFlag_Test(FLAG_DODONPA_LOCKED_UP) != 0 &&
             GameFlag_Test(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
    FieldEffect_SpawnLinked(LOCALID_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION,
                            60);
    recognitionMessageBase = MsgLunpaLeftGuardRecognizesHammet;
    FieldScript_SetMessageId(recognitionMessageBase +
                             RECOGNITION_SEEN_THAT_MAN);
    FieldScript_ShowMessageAndWait(LOCALID_LEFT_GUARD, 0);
    ObjectMotion_SetVariantCallback(LOCALID_LEFT_GUARD, 1);
    FieldScript_SetMessageId(recognitionMessageBase + RECOGNITION_IMPOSSIBLE);
    Engine_GameFlagSet(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
  } else {
    FieldScript_SetMessageId(MsgLunpaLeftGuardResentsDodonpa);
  }
  FieldScript_ShowMessageAndWait(LOCALID_LEFT_GUARD, 0);
}

/// Lunpa Fortress Gates: the right guard boasts, finds the place creepy while
/// the party is cloaked, and after Donpa's judgment wonders how the warriors
/// from Kalay got in.
static void EventScript_TalkToRightGuard(void) {
  if (gGameState.cloaked != 0) {
    FieldScript_SetMessageId(MsgLunpaRightGuardFeelsCreepy);
  } else if (GameFlag_Test(FLAG_DODONPA_LOCKED_UP) != 0) {
    FieldScript_SetMessageId(MsgLunpaRightGuardWondersHow);
  } else {
    FieldScript_SetMessageId(MsgLunpaRightGuardBoasts);
  }
  FieldScript_ShowMessageAndWait(LOCALID_RIGHT_GUARD, 0);
}

/// Lunpa Fortress Gates: Mind Read on the left guard. After Donpa's judgment
/// and before Hammet finds Bunza, he thinks the man with the party looks like
/// that merchant, and Mind Read is cut short the first time as he turns to
/// look.
static void EventScript_ReadLeftGuardsThoughts(void) {
  s32 recognizedHammet;

  if (gGameState.cloaked == 0 && GameFlag_IsSet(FLAG_DODONPA_LOCKED_UP) != 0 &&
      GameFlag_IsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
    recognizedHammet = GameFlag_IsSet(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
    if (recognizedHammet == 0) {
      gFieldWork->psynergy = recognizedHammet;
      FieldEffect_FinishWaveAndReleaseHeapBlock();
      ObjectEvent_FaceObjectEvent(LOCALID_LEFT_GUARD, LOCALID_PARTY_LEADER, 0);
      ObjectEvent_ShowEmote(LOCALID_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_QUESTION,
                            60);
      FieldScript_SetMessageId(MsgLunpaLeftGuardRecognizesHammet);
      GameFlag_Set(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
    } else {
      FieldScript_SetMessageId(MsgLunpaLeftGuardGlimpsedMerchant);
    }
    Script_ShowMessage(LOCALID_LEFT_GUARD, 0);
    ObjectMotion_SetVariantCallback(LOCALID_LEFT_GUARD, 1);
    FieldScript_SetMessageId(MsgLunpaLeftGuardDismissesThought);
  } else {
    FieldScript_SetMessageId(MsgLunpaLeftGuardThoughts);
  }
  Script_ShowMessage(LOCALID_LEFT_GUARD, 0);
}

/// Lunpa Fortress Gates: Mind Read on the right guard.
static void EventScript_ReadRightGuardsThoughts(void) {
  if (GameFlag_Test(FLAG_DODONPA_LOCKED_UP) != 0) {
    FieldScript_SetMessageId(MsgLunpaRightGuardReopenedThoughts);
  } else {
    FieldScript_SetMessageId(MsgLunpaYoudShadowSneak);
  }
  FieldScript_ShowMessageAndWait(LOCALID_RIGHT_GUARD, 0);
}

/// Every frame at the Lunpa Fortress Gates after Donpa's judgment: stepping up
/// to row 10 raises Garet's warning
/// (MB_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH).
static void Lunpa_WatchForFortressApproach(void) {
  s32 leaderRow = ObjectEvent_Get(LOCALID_PARTY_LEADER)->z.fixed / CELL_SIZE;

  if (GameFlag_IsSet(FLAG_MAP_TEMP_43) == 0 && leaderRow == 10) {
    GameFlag_Set(FLAG_MAP_TEMP_43);
    gFieldWork->coordEvent = MB_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH;
  }
}

/// Lunpa Fortress Gates, MB_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH after
/// Donpa's judgment: Garet reminds the leader they cannot go into the fortress,
/// and the leader steps back.
static void EventScript_GaretTurnsLeaderBack(void) {
  FieldScript_Start();
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  FieldScript_SetMessageId(MsgLunpaGaretRefusesToReturn);
  Script_ShowMessage(GARET, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        100);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0, 12);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  GameFlag_Clear(FLAG_MAP_TEMP_43);
  FieldScript_End();
}

/// Lunpa, walking north into a doorway: the door opens and the leader goes
/// into the home, shop, inn or temple behind it.
static void EventScript_EnterDoor(void) {
  struct FieldWork *work;
  struct ObjectEvent *placedObjectEvent;
  u32 localId;
  s32 doorIndex;

  work = gFieldWork;
  FieldScript_Start();
  // Freeze placed object events before opening the selected door and crossing
  // it.
  for (localId = LOCALID_FIRST_PLACED; localId <= LOCALID_LAST_SLOT;
       localId++) {
    placedObjectEvent = ObjectEvent_Get(localId);
    if (placedObjectEvent != NULL) {
      placedObjectEvent->motionFlags = 0;
    }
  }
  Script_PlaySong(SE_DOOR_OPEN);
  doorIndex = work->coordEvent - MB_LUNPA_FIRST_HOME_DOOR;
  MapGrid_AnimateCells(gLunpaDoors[doorIndex].steps, gLunpaDoors[doorIndex].x,
                       gLunpaDoors[doorIndex].y);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->motionFlags = 0;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  if (doorIndex != DOOR_TEMPLE) {
    ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 2, -8);
    FieldScript_WaitFrames(10);
  }
  Engine_ScriptRequestWarp(work->coordEvent);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_End();
}

/// Lunpa, MB_LUNPA_HIDDEN_PASSAGE: the hidden passage leads to the jail once
/// opened, while Reveal shows it.
static void EventScript_EnterHiddenPassage(void) {
  if (GameFlag_IsSet(FLAG_MAP_TEMP_02) != 0) {
    if (GameFlag_IsSet(FLAG_MAP_TEMP_01) == 0) {
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
      ObjectEvent_Get(LOCALID_PARTY_LEADER)->motionFlags = 0;
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
      ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 2, -8);
      FieldScript_WaitFrames(13);
      Engine_ScriptRequestWarp(LUNPA_TO_VAULT_JAIL);
    }
  }
}

/// Lunpa Fortress Gates, MB_LUNPA_FORTRESS_GATES_FORTRESS_DOOR: through the
/// fortress door at the top of the steps, into Lunpa Fortress.
static void EventScript_EnterFortress(void) {
  GameFlag_Set(FLAG_MAP_TEMP_42);
  FieldScript_Start();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->motionFlags = 0;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  ObjectEvent_WalkBy(LOCALID_PARTY_LEADER, 0, -8);
  Script_PlaySong(SE_DOOR_OPEN);
  MapGrid_CopyCells(53, 4, 2, 2, 41, 4);
  FieldScript_WaitFrames(10);
  MapGrid_CopyCells(53, 6, 2, 2, 41, 4);
  FieldScript_WaitFrames(10);
  Engine_ScriptRequestWarp(LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_GROUND_FLOOR);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_End();
}

static void UNUSED Lunpa_CheckAhead(void) { Field_TryJumpForward(); }

/// Unused: plays the treasure fanfare for Reveal uncovering something, until
/// FLAG_MAP_TEMP_01 is set; nothing calls it.
UNUSED static void Lunpa_PlayRevealCue(void) {
  if (GameFlag_Test(FLAG_MAP_TEMP_01) == 0) {
    Sound_PlaySong(MUS_TREASURE_FOUND);
  }
}

/// Lunpa: searching the switch Reveal shows opens the passage to the jail in
/// five steps.
static void EventScript_PressHiddenSwitch(void) {
  if (GameFlag_IsSet(FLAG_MAP_TEMP_01) != 0) {
    return;
  }
  if (GameFlag_IsSet(FLAG_MAP_TEMP_02) != 0) {
    return;
  }
  // Open the passage over five map frames, then remember its final state.
  ObjectEvent_SetPosition(LOCALID_SWITCH_SPARKLE, 0, 0);
  Script_PlaySong(SE_HIDDEN_PASSAGE_OPEN);
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
  GameFlag_Set(FLAG_MAP_TEMP_02);
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
static void EventScript_GuardsBlockGate(void) {
  if (gGameState.cloaked == 0) {
    FieldScript_Start();
    ObjectEvent_ShowEmote(LOCALID_LEFT_GUARD,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 2);
    ObjectEvent_ShowEmote(LOCALID_RIGHT_GUARD,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 15);
    FieldScript_WaitFrames(30);
    ObjectEvent_WalkTo(LOCALID_LEFT_GUARD, 152, 168);
    ObjectEvent_WalkTo(LOCALID_RIGHT_GUARD, 168, 168);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_LEFT_GUARD);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_RIGHT_GUARD);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_LEFT_GUARD);
    Engine_ObjectEventSetAnimation(LOCALID_LEFT_GUARD, 0);
    ObjectEvent_FaceDirection(LOCALID_LEFT_GUARD,
                              FACING_SOUTHEAST + FACING_STEP, 0);
    ObjectMotion_EnableActionAndResetMotion(LOCALID_RIGHT_GUARD);
    Engine_ObjectEventSetAnimation(LOCALID_RIGHT_GUARD, 0);
    ObjectEvent_FaceDirection(LOCALID_RIGHT_GUARD, FACING_SOUTH + FACING_STEP,
                              0);
    FieldScript_SetMessageId(MsgLunpaGuardsWarnPartyAway);
    Script_ShowMessage(LOCALID_LEFT_GUARD, 0);
    GameFlag_Set(FLAG_MAP_TEMP_40);
    Lunpa_CopyGatewayCollision(6, 11);
    FieldScript_End();
  }
}

static void UNUSED Lunpa_DoNothing(void) {}

/// Lunpa Fortress Gates, casting Cloak: the gateway closes behind the guards.
static void EventScript_CastCloak(void) {
  Lunpa_CopyGatewayCollision(6, 11);
  Engine_GameFlagSet(FLAG_MAP_TEMP_41);
}

/// Lunpa Fortress Gates, raised when the guards spot the party: they wonder
/// how the party got there and march them back down to the foot of the steps.
static void EventScript_GuardsCatchParty(void) {
  struct ObjectEvent *leader;
  s32 warningMessageBase;

  if (GameFlag_IsSet(FLAG_MAP_TEMP_44) != 0) {
    return;
  }
  GameFlag_Set(FLAG_MAP_TEMP_44);
  FieldScript_Start();
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  ObjectEvent_FaceObjectEvent(LOCALID_LEFT_GUARD, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_RIGHT_GUARD, LOCALID_PARTY_LEADER, 0);
  ObjectMotion_SetVariantCallback(LOCALID_LEFT_GUARD, 1);
  ObjectMotion_SetVariantCallback(LOCALID_RIGHT_GUARD, 1);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // Warn the party before escorting everyone down the two sections of the gate.
  warningMessageBase = MsgLunpaGuardsCatchParty;
  FieldScript_SetMessageId(warningMessageBase + CATCH_LEFT_GUARD_CHALLENGES);
  Script_ShowMessage(LOCALID_LEFT_GUARD, 0);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
  ObjectEvent_SetSpeed(LOCALID_LEFT_GUARD, PIXELS(2), PIXELS(1));
  ObjectEvent_SetSpeed(LOCALID_RIGHT_GUARD, PIXELS(2), PIXELS(1));
  Engine_ObjectEventSetAnimation(LOCALID_RIGHT_GUARD, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(35);
  FieldScript_SetMessageId(warningMessageBase + CATCH_RIGHT_GUARD_WONDERS);
  Script_ShowMessage(LOCALID_RIGHT_GUARD, 0);
  ObjectEvent_ShowEmote(LOCALID_LEFT_GUARD, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                        30);
  FieldScript_SetMessageId(warningMessageBase + CATCH_LEFT_GUARD_REFUSES_ENTRY);
  Script_ShowMessage(LOCALID_LEFT_GUARD, 0);
  Engine_ObjectEventSetAnimation(LOCALID_RIGHT_GUARD, ANIM_NOD);
  FieldScript_WaitFrames(25);
  FieldScript_SetMessageId(warningMessageBase +
                           CATCH_RIGHT_GUARD_SENDS_PARTY_OFF);
  Script_ShowMessage(LOCALID_RIGHT_GUARD, 0);
  ObjectEvent_WalkTo(LOCALID_LEFT_GUARD, leader->x.part.pixel - 1,
                     leader->z.part.pixel);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_LEFT_GUARD);
  // Bring the party and both guards together at the first landing.
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 160, 216);
  ObjectEvent_WalkTo(LOCALID_LEFT_GUARD, 152, 200);
  ObjectEvent_WalkTo(LOCALID_RIGHT_GUARD, 168, 200);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_LEFT_GUARD);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_RIGHT_GUARD);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectEvent_FaceObjectEvent(LOCALID_LEFT_GUARD, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_RIGHT_GUARD, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(12);
  // Continue to the foot of the gate before enabling the watcher again.
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 160, 272);
  ObjectEvent_WalkTo(LOCALID_LEFT_GUARD, 152, 256);
  ObjectEvent_WalkTo(LOCALID_RIGHT_GUARD, 168, 256);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_LEFT_GUARD);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_RIGHT_GUARD);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  FieldScript_End();
  FieldScheduler_AddCallback(Lunpa_WatchForUncloakedParty, TASK_PRIORITY_MAP);
}

static void Lunpa_ReopenGateway(void) { Lunpa_CopyGatewayCollision(7, 12); }

/// Lunpa Fortress Gates, as Cloak fades: if the leader is still among the
/// guards, the party is caught.
static void EventScript_CloakFades(void) {
#if TBS_PATCH_1
  struct FieldWork *work;
#endif
  struct ObjectEvent *leader;

#if TBS_PATCH_1
  work = gFieldWork;
#endif
  GameFlag_ClearBit(FLAG_MAP_TEMP_41);
  GameFlag_ClearBit(FLAG_MAP_TEMP_40);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader->x.fixed > PIXELS(104) && leader->x.fixed < PIXELS(240) &&
      leader->z.fixed > PIXELS(160) && leader->z.fixed < PIXELS(248)) {
    Task_Remove(Lunpa_WatchForUncloakedParty);
#if TBS_PATCH_1
    work->raisedEvent = RAISED_PARTY_SPOTTED;
#else
    EventScript_GuardsCatchParty();
#endif
  }
  Lunpa_ReopenGateway();
  GameFlag_ClearBit(FLAG_MAP_TEMP_44);
}

/// Every frame at the Lunpa Fortress Gates: an uncloaked leader in front of
/// the guards is spotted.
static void Lunpa_WatchForUncloakedParty(void) {
  struct FieldWork *work;
  struct ObjectEvent *leader;

  work = gFieldWork;
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (gGameState.cloaked == 0 &&
      (u32)(leader->x.fixed - PIXELS(144)) <= PIXELS(32) &&
      leader->z.fixed >= PIXELS(168) && leader->z.fixed < PIXELS(176)) {
    Task_Remove(Lunpa_WatchForUncloakedParty);
    work->raisedEvent = RAISED_PARTY_SPOTTED;
  }
}

/// Every 16 frames while the leader is thrown: a puff of dust.
static void Lunpa_KickUpLeaderDust(void) {
  struct ObjectEvent *leader;
  s32 dustAngle;
  s32 dustVelocity[3];

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if ((gFrameCount & 15) == 0) {
    dustAngle = (((u32)Engine_RandomNext() * 52) >> 16) * 64 + 230;
    SET_VECTOR(dustVelocity, Trig_Cos(dustAngle) / 4, 0,
               Trig_Sin(dustAngle) / 2);
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      dustVelocity[0], dustVelocity[1], dustVelocity[2], 0,
                      NULL);
  }
}

/// Lunpa Fortress Gates, LUNPA_FORTRESS_GATES_AFTER_CAUGHT, after being
/// caught in the fortress: the guards throw the leader out and down the steps.
static void EventScript_GuardsThrowLeaderOut(void) {
  FieldScript_Start();
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(160), PIXELS(128));
  ObjectEvent_SetPosition(LOCALID_LEFT_GUARD, PIXELS(152), PIXELS(112));
  ObjectEvent_SetPosition(LOCALID_RIGHT_GUARD, PIXELS(168), PIXELS(112));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
#ifdef BUGFIX
  ObjectEvent_FaceDirection(LOCALID_LEFT_GUARD, FACING_SOUTHEAST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_RIGHT_GUARD, FACING_SOUTH + FACING_STEP, 0);
#else
  // BUG: the town's guards, whose local ids no object event at the gates holds,
  // so the gates' guards keep their headings.
  ObjectEvent_FaceDirection(LOCALID_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_EAST_GUARD, FACING_SOUTH + FACING_STEP, 0);
#endif
  FieldCamera_FollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  FieldScript_StartScreenTransition();
  FieldScript_WaitFrames(30);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(9) / 5, PIXELS(9) / 10);
  ObjectEvent_SetSpeed(LOCALID_LEFT_GUARD, PIXELS(9) / 5, PIXELS(9) / 10);
  ObjectEvent_SetSpeed(LOCALID_RIGHT_GUARD, PIXELS(9) / 5, PIXELS(9) / 10);
  ObjectEvent_WalkTo(LOCALID_LEFT_GUARD, 152, 288);
  ObjectEvent_WalkTo(LOCALID_RIGHT_GUARD, 168, 288);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_SHAKE_HEAD);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 160, 296);
  FieldScheduler_AddCallback(Lunpa_KickUpLeaderDust, TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(1);
  Script_PlaySong(SE_SCUFFLE);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetActionVariant(LOCALID_LEFT_GUARD, 3);
  ObjectMotion_SetActionVariant(LOCALID_RIGHT_GUARD, 3);
  Script_PlaySong(SE_SCUFFLE);
  FieldScript_WaitFrames(30);
  ObjectEvent_Get(LOCALID_LEFT_GUARD)->priorityFlags |=
      OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectEvent_Get(LOCALID_RIGHT_GUARD)->priorityFlags |=
      OBJ_EVENT_PRIORITY_AUTOMATIC;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Script_PlaySong(SE_SCUFFLE);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetAnimation(LOCALID_LEFT_GUARD, ANIM_STAND);
  Engine_ObjectEventSetAnimation(LOCALID_RIGHT_GUARD, ANIM_STAND);
  Task_Remove(Lunpa_KickUpLeaderDust);
  // Throw the leader down the steps, then wait for the first landing.
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->motionFlags |= OBJ_EVENT_FALLS;
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->velocityY = PIXELS(6);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->velocityZ = PIXELS(6);
  FieldScript_WaitFrames(1);
  while (ObjectEvent_Get(LOCALID_PARTY_LEADER)->y.fixed != 0) {
    FieldScript_WaitFrames(1);
  }
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_SPRAWLED);
  Script_PlaySong(SE_LANDING_THUD);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  FieldScheduler_AddCallback(Lunpa_KickUpLeaderDust, TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(2);
  // A smaller second bounce ends with the guards' final warning.
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->velocityY = PIXELS(3);
  FieldScript_WaitFrames(1);
  while (ObjectEvent_Get(LOCALID_PARTY_LEADER)->y.fixed != 0) {
    FieldScript_WaitFrames(1);
  }
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  Task_Remove(Lunpa_KickUpLeaderDust);
  FieldScript_WaitFrames(50);
  FieldScript_SetMessageId(MsgLunpaGuardForbidsReturn);
  Script_ShowMessage(LOCALID_LEFT_GUARD, 0);
  ObjectEvent_Get(LOCALID_LEFT_GUARD)->priorityFlags |=
      OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectEvent_Get(LOCALID_RIGHT_GUARD)->priorityFlags |=
      OBJ_EVENT_PRIORITY_AUTOMATIC;
  ObjectEvent_SetSpeed(LOCALID_LEFT_GUARD, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_RIGHT_GUARD, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkTo(LOCALID_LEFT_GUARD, 144, 200);
  ObjectEvent_WalkTo(LOCALID_RIGHT_GUARD, 176, 200);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_LEFT_GUARD);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_RIGHT_GUARD);
  Engine_ObjectEventSetAnimation(LOCALID_LEFT_GUARD, ANIM_STAND);
  Engine_ObjectEventSetAnimation(LOCALID_RIGHT_GUARD, ANIM_STAND);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_LEFT_GUARD, FACING_SOUTHEAST + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_RIGHT_GUARD, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_End();
}

/// Lunpa Fortress Gates, LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA,
/// after slipping out of the fortress: the leader casts Cloak and slips round
/// the guards.
static void EventScript_LeaderSneaksOut(void) {
  FieldScript_Start();
  FieldScript_StartScreenTransition();
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 152, 168);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  FieldScript_WaitFrames(20);
  // Cast Cloak before following the route around the guards.
  FieldEffect_LoadActionEffectResources(ABILITY_CLOAK, 1);
  FieldEffect_SetupObjectPair(LOCALID_PARTY_LEADER, 0);
  FieldEffect_StartCast();
  Field_DispatchTypeHandler(1);
  FieldEffect_StopCasterHook();
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 144, 184);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 88, 184);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 88, 200);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 72, 200);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 72, 288);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 88, 288);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  FieldScript_End();
}

/// Sets Lunpa or the Lunpa Fortress Gates up: in the town, Reveal's secrets
/// hidden and the floating Nut; at the gates, the thrown-out or sneaking-out
/// cutscene, the watchers, and the fortress's puzzle and guard flags cleared.
s32 Map_Initialize(void) {
  struct ObjectEvent *puddle;

  if (gGameState.mapId == (s32)&MapId_Lunpa) {
    gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    EventScript_HideSecrets();
    if (GameFlag_Test(FLAG_LUNPA_NUT_CAUGHT) == 0) {
      Map_FloatNut(LOCALID_FLOATING_NUT);
    }
    puddle = ObjectEvent_GetById(LOCALID_HIDDEN_PUDDLE);
    if (puddle != NULL) {
      ObjectEvent_SetSpriteFlags(puddle, 0);
    }
    Engine_GameFlagSet(FLAG_MAP_TEMP_01);
  }
  // Restore the gate's arrival cutscene and active detection tasks separately.
  if (gGameState.mapId == (s32)&MapId_LunpaFortressGates) {
    gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gGameState.retreatWarpDest = GATE_RETREAT_WARP_DEST;
    if (gGameState.warpDest == LUNPA_FORTRESS_GATES_AFTER_CAUGHT &&
        GameFlag_Test(FLAG_KEEP_PARTY_POSITION) == 0) {
      EventScript_GuardsThrowLeaderOut();
    }
    if (gGameState.warpDest ==
            LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA &&
        GameFlag_Test(FLAG_KEEP_PARTY_POSITION) == 0) {
      EventScript_LeaderSneaksOut();
    }
    if (GameFlag_Test(FLAG_DODONPA_LOCKED_UP) != 0 &&
        GameFlag_Test(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
      FieldScheduler_AddCallback(Lunpa_WatchForFortressApproach,
                                 TASK_PRIORITY_MAP);
    }
#if TBS_PATCH_4
    if (gGameState.cloaked != 0)
      EventScript_CastCloak();
#endif
    FieldScheduler_AddCallback(Lunpa_WatchForUncloakedParty, TASK_PRIORITY_MAP);
    // The fortress's puzzles and beaten guards are reset.
    GameFlag_ClearBit(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_IVY_BLOWN_AWAY);
    GameFlag_ClearBit(FLAG_LUNPA_FORTRESS_PASSAGE_IVY_BLOWN_AWAY);
    GameFlag_ClearBit(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_IVY_BLOWN_AWAY);
    GameFlag_ClearBit(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED);
    GameFlag_ClearBit(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED);
    GameFlag_ClearBit(FLAG_LUNPA_FORTRESS_CRATES_MOVED);
    GameFlag_ClearBit(FLAG_LUNPA_FORTRESS_MIDDLE_GUARD_BEATEN);
    GameFlag_ClearBit(FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN);
    GameFlag_ClearBit(FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN);
    GameFlag_ClearBit(FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN);
  }
  return 0;
}

#include "common/object_orbit.h"

#define LUNPA_WARP_DEST(warpDest, x, z, facing)                                \
  {(warpDest), CONDITION_ALWAYS, (x), 0, (z), (facing), 0, -1, -1, -1, -1, 0}
#define GATE_WARP_DEST(warpDest, x, z, facing)                                 \
  {(warpDest), CONDITION_ALWAYS, (x), 0, (z), (facing), 0, 32, 48, 272, 344, 0}

struct MapWarpDest gLunpaWarpDests[] = {
    LUNPA_WARP_DEST(0, 172, 149, FACING_SOUTH),
    LUNPA_WARP_DEST(LUNPA_FROM_WORLD_MAP, 224, 472, FACING_NORTH),
    LUNPA_WARP_DEST(LUNPA_FROM_LUNPA_FORTRESS_GATES, 232, 32, FACING_SOUTH),
    LUNPA_WARP_DEST(LUNPA_FROM_WEST_LUNPA_CAVE, 120, 104, FACING_SOUTH),
    LUNPA_WARP_DEST(LUNPA_FROM_FIRST_HOME, 248, 184, FACING_SOUTH),
    LUNPA_WARP_DEST(LUNPA_FROM_SECOND_HOME, 344, 232, FACING_SOUTH),
    LUNPA_WARP_DEST(LUNPA_FROM_THIRD_HOME, 344, 328, FACING_SOUTH),
    LUNPA_WARP_DEST(LUNPA_FROM_WEAPON_AND_ARMOR_SHOP, 232, 360, FACING_SOUTH),
    LUNPA_WARP_DEST(LUNPA_FROM_ITEM_SHOP, 328, 120, FACING_SOUTH),
    LUNPA_WARP_DEST(LUNPA_FROM_INN, 104, 184, FACING_SOUTH),
    LUNPA_WARP_DEST(LUNPA_FROM_TEMPLE, 48, 168, FACING_SOUTH),
    LUNPA_WARP_DEST(LUNPA_FROM_CRATE_ROOM, 216, 344, FACING_SOUTH),
    LUNPA_WARP_DEST(LUNPA_FROM_VAULT_JAIL, 40, 328, FACING_SOUTH),
    {MAP_TABLE_END},
};

struct MapWarpDest gGateWarpDests[] = {
    GATE_WARP_DEST(0, 152, 328, FACING_NORTH),
    GATE_WARP_DEST(LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_GROUND_FLOOR, 160,
                   104, FACING_SOUTH),
    GATE_WARP_DEST(LUNPA_FORTRESS_GATES_FROM_LUNPA, 152, 328, FACING_NORTH),
    GATE_WARP_DEST(LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA, 144, 104,
                   FACING_SOUTH),
    GATE_WARP_DEST(LUNPA_FORTRESS_GATES_AFTER_CAUGHT, 160, 224, FACING_EAST),
    {MAP_TABLE_END},
};

#define ROOM_WARP(warp) MAP_WARP((warp), LunpaInteriors, ROOM_WARP_DEST(warp))

u32 gLunpaWarps[] = {
    MAP_WARPS(Lunpa),
    MAP_WARP(LUNPA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LUNPA),
    MAP_WARP(LUNPA_TO_LUNPA_FORTRESS_GATES, LunpaFortressGates,
             LUNPA_FORTRESS_GATES_FROM_LUNPA),
    MAP_WARP(LUNPA_TO_WEST_LUNPA_CAVE, WestLunpaCave,
             WEST_LUNPA_CAVE_FROM_LUNPA),
    ROOM_WARP(LUNPA_TO_FIRST_HOME),
    ROOM_WARP(LUNPA_TO_SECOND_HOME),
    ROOM_WARP(LUNPA_TO_THIRD_HOME),
    ROOM_WARP(LUNPA_TO_WEAPON_AND_ARMOR_SHOP),
    ROOM_WARP(LUNPA_TO_ITEM_SHOP),
    ROOM_WARP(LUNPA_TO_INN),
    ROOM_WARP(LUNPA_TO_TEMPLE),
    ROOM_WARP(LUNPA_TO_CRATE_ROOM),
    MAP_WARP(LUNPA_TO_VAULT_JAIL, VaultJail, VAULT_JAIL_FROM_LUNPA),
    MAP_WARPS_END,
};

u32 gGateWarps[] = {
    MAP_WARPS(LunpaFortressGates),
    MAP_WARP(LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_GROUND_FLOOR,
             LunpaFortressGroundFloor,
             LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_GATES),
    MAP_WARP(LUNPA_FORTRESS_GATES_TO_LUNPA, Lunpa,
             LUNPA_FROM_LUNPA_FORTRESS_GATES),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gLunpaObjectEventTemplates[] = {
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
    { MAP_TABLE_END, CONDITION_ALWAYS },
};

struct ObjectEventTemplate gGateObjectEventTemplates[] = {
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(144), 0, PIXELS(200), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(176), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};
// clang-format on

#define TALK(localId, value)                                                   \
  {MAP_EVENT_TALK, (localId), CONDITION_ALWAYS, (value)}
#define MIND_READ(localId, value)                                              \
  {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), (localId), CONDITION_ALWAYS,      \
   (value)}

#define DOOR(trigger)                                                          \
  {COORD_EVENT_WALKING_INTO(FACING_NORTH), (trigger), CONDITION_ALWAYS,        \
   EVENT_SCRIPT(EventScript_EnterDoor)}

#define LUNPA_ROADS_AND_DOORS                                                  \
  {MAP_EVENT_WARP, MB_LUNPA_WORLD_MAP_ROAD, CONDITION_ALWAYS,                  \
   LUNPA_TO_WORLD_MAP},                                                        \
      {MAP_EVENT_WARP, MB_LUNPA_GATE_ROAD, CONDITION_ALWAYS,                   \
       LUNPA_TO_LUNPA_FORTRESS_GATES},                                         \
      {MAP_EVENT_WARP, MB_LUNPA_CAVE_ROAD, CONDITION_ALWAYS,                   \
       LUNPA_TO_WEST_LUNPA_CAVE},                                              \
      DOOR(MB_LUNPA_FIRST_HOME_DOOR), DOOR(MB_LUNPA_SECOND_HOME_DOOR),         \
      DOOR(MB_LUNPA_THIRD_HOME_DOOR), DOOR(MB_LUNPA_ARMS_SHOP_DOOR),           \
      DOOR(MB_LUNPA_ITEM_SHOP_DOOR), DOOR(MB_LUNPA_INN_DOOR),                  \
      DOOR(MB_LUNPA_TEMPLE_DOOR), DOOR(MB_LUNPA_EIGHTH_ROOM_DOOR), {           \
    COORD_EVENT_WALKING_INTO(FACING_NORTH), MB_LUNPA_HIDDEN_PASSAGE,           \
        CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterHiddenPassage)         \
  }

#define LUNPA_SECRETS                                                          \
  {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_HIDDEN_PUDDLE,                \
   FLAG_MAP_TEMP_00, EVENT_SCRIPT(EventScript_FreezeHiddenPuddle)},            \
      {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0,        \
       CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RevealSecrets)},             \
      {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0,        \
       CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HideSecrets)},               \
      {BG_EVENT(BG_EVENT_SILENT), MB_LUNPA_HIDDEN_SWITCH, FLAG_MAP_TEMP_01,    \
       EVENT_SCRIPT(EventScript_PressHiddenSwitch)},                           \
      {BG_EVENT(BG_EVENT_SILENT), MB_LUNPA_PSYNERGY_STONE,                     \
       FLAG_LUNPA_PSYNERGY_STONE, FIND_PSYNERGY_STONE},                        \
      {BG_EVENT(BG_EVENT_GRAVE), MB_LUNPA_GRAVE_SLEEP_BOMB,                    \
       FLAG_LUNPA_GRAVE_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB)},               \
      {BG_EVENT(BG_EVENT_BARREL), MB_LUNPA_BARREL_NUT, FLAG_LUNPA_BARREL_NUT,  \
       FIND_ITEM(ITEM_NUT)},                                                   \
      {PSYNERGY_ON_OBJ_EVENT(ABILITY_CATCH), LOCALID_FLOATING_NUT,             \
       FLAG_LUNPA_NUT_CAUGHT, EVENT_SCRIPT(EventScript_CatchFloatingNut)},     \
      {BG_EVENT(BG_EVENT_SILENT), MB_LUNPA_WELL, CONDITION_ALWAYS,             \
       EVENT_SCRIPT(EventScript_SearchWell)},                                  \
  {                                                                            \
    MAP_EVENTS_END                                                             \
  }

struct MapEvent gLunpaSealedEvents[] = {
    LUNPA_ROADS_AND_DOORS,
    TALK(LOCALID_MAN_ASKING_HOW_LONG_IN_LUNPA,
         EVENT_SCRIPT(EventScript_AskAboutLengthOfStay)),
    TALK(LOCALID_WOMAN_WITH_NO_FREEDOM, MSGNO(MsgLunpaVillageVillagerBSealed)),
    TALK(LOCALID_MAN_ON_HAMMETS_KIDNAPPING,
         EVENT_SCRIPT(EventScript_AskAboutHammetsKidnapping)),
    TALK(LOCALID_WOMAN_SAYING_DONPA_IS_TOO_OLD,
         MSGNO(MsgLunpaVillageVillagerDSealed)),
    TALK(LOCALID_OLD_MAN_ON_LUNPA_AND_DONPA,
         MSGNO(MsgLunpaVillageVillagerESealed)),
    TALK(LOCALID_OLD_WOMAN_ON_DONPA, MSGNO(MsgLunpaVillageVillagerFSealed)),
    TALK(LOCALID_BOY_ASKING_TO_SEE_DONPA,
         EVENT_SCRIPT(EventScript_AskAboutSeeingDonpa)),
    TALK(LOCALID_GIRL_ON_THE_CHEFS_MEALS,
         MSGNO(MsgLunpaVillageVillagerHSealed)),
    TALK(LOCALID_WEST_GUARD, EVENT_SCRIPT(EventScript_TalkToWestGuard)),
    TALK(LOCALID_EAST_GUARD, EVENT_SCRIPT(EventScript_TalkToEastGuard)),
    MIND_READ(LOCALID_MAN_ASKING_HOW_LONG_IN_LUNPA,
              MSGNO(MsgLunpaVillageVillagerASealedThoughts)),
    MIND_READ(LOCALID_WOMAN_WITH_NO_FREEDOM,
              MSGNO(MsgLunpaVillageVillagerBSealedThoughts)),
    MIND_READ(LOCALID_MAN_ON_HAMMETS_KIDNAPPING,
              MSGNO(MsgLunpaVillageVillagerCSealedThoughts)),
    MIND_READ(LOCALID_WOMAN_SAYING_DONPA_IS_TOO_OLD,
              MSGNO(MsgLunpaVillageVillagerDSealedThoughts)),
    MIND_READ(LOCALID_OLD_MAN_ON_LUNPA_AND_DONPA,
              MSGNO(MsgLunpaVillageVillagerESealedThoughts)),
    MIND_READ(LOCALID_OLD_WOMAN_ON_DONPA,
              MSGNO(MsgLunpaVillageVillagerFSealedThoughts)),
    MIND_READ(LOCALID_BOY_ASKING_TO_SEE_DONPA,
              MSGNO(MsgLunpaVillageVillagerGSealedThoughts)),
    MIND_READ(LOCALID_GIRL_ON_THE_CHEFS_MEALS,
              MSGNO(MsgLunpaVillageVillagerHSealedThoughts)),
    MIND_READ(LOCALID_WEST_GUARD,
              MSGNO(MsgLunpaVillageWestGuardSealedThoughts)),
    MIND_READ(LOCALID_EAST_GUARD,
              EVENT_SCRIPT(EventScript_ReadEastGuardsThoughts)),
    LUNPA_SECRETS,
};

struct MapEvent gLunpaReopenedEvents[] = {
    LUNPA_ROADS_AND_DOORS,
    TALK(LOCALID_MAN_ASKING_HOW_LONG_IN_LUNPA,
         MSGNO(MsgLunpaVillageVillagerAReopened)),
    TALK(LOCALID_WOMAN_WITH_NO_FREEDOM,
         EVENT_SCRIPT(EventScript_ComplainAboutCold)),
    TALK(LOCALID_MAN_ON_HAMMETS_KIDNAPPING,
         MSGNO(MsgLunpaVillageVillagerCReopened)),
    TALK(LOCALID_WOMAN_SAYING_DONPA_IS_TOO_OLD,
         EVENT_SCRIPT(EventScript_AskAboutFortressCommotion)),
    TALK(LOCALID_OLD_MAN_ON_LUNPA_AND_DONPA,
         MSGNO(MsgLunpaVillageVillagerEReopened)),
    TALK(LOCALID_OLD_WOMAN_ON_DONPA, MSGNO(MsgLunpaVillageVillagerFReopened)),
    TALK(LOCALID_BOY_ASKING_TO_SEE_DONPA,
         MSGNO(MsgLunpaVillageVillagerGReopened)),
    TALK(LOCALID_GIRL_ON_THE_CHEFS_MEALS,
         MSGNO(MsgLunpaVillageVillagerHReopened)),
    TALK(LOCALID_WEST_GUARD, MSGNO(MsgLunpaVillageWestGuardReopened)),
    TALK(LOCALID_EAST_GUARD, MSGNO(MsgLunpaVillageEastGuardReopened)),
    MIND_READ(LOCALID_MAN_ASKING_HOW_LONG_IN_LUNPA,
              MSGNO(MsgLunpaVillageVillagerAReopenedThoughts)),
    MIND_READ(LOCALID_WOMAN_WITH_NO_FREEDOM,
              MSGNO(MsgLunpaVillageVillagerBReopenedThoughts)),
    MIND_READ(LOCALID_MAN_ON_HAMMETS_KIDNAPPING,
              MSGNO(MsgLunpaVillageVillagerCReopenedThoughts)),
    MIND_READ(LOCALID_WOMAN_SAYING_DONPA_IS_TOO_OLD,
              MSGNO(MsgLunpaVillageVillagerDReopenedThoughts)),
    MIND_READ(LOCALID_OLD_MAN_ON_LUNPA_AND_DONPA,
              MSGNO(MsgLunpaVillageVillagerEReopenedThoughts)),
    MIND_READ(LOCALID_OLD_WOMAN_ON_DONPA,
              MSGNO(MsgLunpaVillageVillagerFReopenedThoughts)),
    MIND_READ(LOCALID_BOY_ASKING_TO_SEE_DONPA,
              MSGNO(MsgLunpaVillageVillagerGReopenedThoughts)),
    MIND_READ(LOCALID_GIRL_ON_THE_CHEFS_MEALS,
              MSGNO(MsgLunpaVillageVillagerHReopenedThoughts)),
    MIND_READ(LOCALID_WEST_GUARD,
              MSGNO(MsgLunpaVillageWestGuardReopenedThoughts)),
    MIND_READ(LOCALID_EAST_GUARD,
              EVENT_SCRIPT(EventScript_ReadEastGuardsThoughts)),
    LUNPA_SECRETS,
};

// clang-format off
struct MapEvent gGateEvents[] = {
    { MAP_EVENT_WARP, MB_LUNPA_FORTRESS_GATES_LUNPA_ROAD, CONDITION_ALWAYS, LUNPA_FORTRESS_GATES_TO_LUNPA },
    { MAP_EVENT_WARP, MB_LUNPA_FORTRESS_GATES_THIRD_ROAD, CONDITION_ALWAYS, GATE_WARP_THIRD },
    { PSYNERGY_IN_AREA(ABILITY_CLOAK, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_CastCloak) },
    { PSYNERGY_IN_AREA(ABILITY_CLOAK, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_CloakFades) },
    { MAP_EVENT_COORD, MB_LUNPA_FORTRESS_GATES_FORTRESS_DOOR, FLAG_MAP_TEMP_42,
      EVENT_SCRIPT(EventScript_EnterFortress) },
    { MAP_EVENT_COORD, MB_LUNPA_FORTRESS_GATES_GUARDED_GROUND, FLAG_MAP_TEMP_40,
      EVENT_SCRIPT(EventScript_GuardsBlockGate) },
    { MAP_EVENT_COORD, MB_LUNPA_FORTRESS_GATES_FORTRESS_APPROACH, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_GaretTurnsLeaderBack) },
    { MAP_EVENT_RAISED, RAISED_PARTY_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_GuardsCatchParty) },
    TALK(LOCALID_LEFT_GUARD, EVENT_SCRIPT(EventScript_TalkToLeftGuard)),
    TALK(LOCALID_RIGHT_GUARD, EVENT_SCRIPT(EventScript_TalkToRightGuard)),
    MIND_READ(LOCALID_LEFT_GUARD, EVENT_SCRIPT(EventScript_ReadLeftGuardsThoughts)),
    MIND_READ(LOCALID_RIGHT_GUARD, EVENT_SCRIPT(EventScript_ReadRightGuardsThoughts)),
    { BG_EVENT(BG_EVENT_SILENT), MB_LUNPA_FORTRESS_GATES_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchWell) },
    { MAP_EVENTS_END },
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
/// coord event from MB_LUNPA_FIRST_HOME_DOOR.
struct MapGridCellAnimation gLunpaDoors[] = {
    {gLunpaDoorSteps[0], 46, 9},  {gLunpaDoorSteps[1], 52, 12},
    {gLunpaDoorSteps[2], 52, 18}, {gLunpaDoorSteps[3], 45, 20},
    {gLunpaDoorSteps[4], 51, 5},  {gLunpaDoorSteps[5], 37, 9},
    {gLunpaDoorSteps[6], 33, 6},  {gLunpaDoorSteps[7], 44, 17},
};

/// Unused: a zero word nothing reads.
u32 gLunpaUnusedWord = 0;
