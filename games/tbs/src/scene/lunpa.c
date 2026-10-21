/// @file
/// Lunpa's overlay, which holds two places, each a scene of its own.
///
/// Where they fall in the story: Master Hammet, a wealthy landowner and
/// merchant from Kalay, is held for ransom in Lunpa Fortress by Dodonpa, the
/// grandson of Lunpa, the noble thief who founded the town. Dodonpa's father,
/// Donpa, is too old to lead, and Dodonpa keeps him hidden in the fortress.
/// Dodonpa's men have sealed the town: no one may come in without his
/// authorization, and no one may leave. Once Donpa has judged Dodonpa in the
/// fortress (FLAG_LUNPA_TRADE_REOPENED, set there), Scene_GetEvents gives the
/// town gLunpaReopenedEvents instead of gLunpaSealedEvents, and the people
/// talk of the intruders who got into the fortress.
///
/// - Lunpa (Lunpa, scene 104), the town. Two guards keep the road in from
///   the world map (LUNPA_TO_WORLD_MAP) and turn the party away. The east
///   guard wants Dodonpa's authorization; once the guards suspect Kalay
///   (FLAG_GUARDS_SUSPECT_KALAY, set on the world map) he asks whether the
///   party came from there, and his thoughts tell of the gate in the cave
///   west of town, which opens only from the inside. LUNPA_TO_WEST_LUNPA_CAVE
///   leads to West Lunpa Cave, the doors into the homes, the shops, the inn
///   and Lunpa Temple (LunpaSuhalla, lunpa_suhalla.c), and
///   LUNPA_TO_LUNPA_FORTRESS_GATES up to the Lunpa Fortress Gates. Every entry
///   and the end of Reveal hide the town's secrets (Event_HideSecrets,
///   FLAG_SCENE_TEMP_01), and Reveal shows them (Event_RevealSecrets): a
///   Psynergy Stone, a puddle that Frost freezes into a pillar to stand on
///   (FLAG_SCENE_TEMP_00), and the glint of a switch. Searching the switch
///   opens a passage (FLAG_SCENE_TEMP_02) that, while Reveal shows it, leads
///   to the jail (LUNPA_TO_VAULT_JAIL, arriving at VAULT_JAIL_FROM_LUNPA; the
///   jail's area name there is Lunpa). A Nut floats over the town for Catch, a
///   grave and a barrel hold a Sleep Bomb and a Nut, and frogs croak in the
///   well.
/// - Lunpa Fortress Gates (LunpaFortressGates, scene 159), the steps up to the
///   fortress door, kept by two guards. Stepping on the ground before them
///   uncloaked (TRIGGER_GUARDED_GROUND, Event_GuardsBlockGate) brings them in
///   front of the door (FLAG_SCENE_TEMP_40). Cloak (Event_CastCloak,
///   FLAG_SCENE_TEMP_41) lets the leader slip past to the door
///   (TRIGGER_FORTRESS_DOOR, Event_EnterFortress, FLAG_SCENE_TEMP_42), and
///   LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_1 leads into Lunpa Fortress. An
///   uncloaked leader in front of the guards (Lunpa_WatchForUncloakedParty),
///   or one still among them when Cloak fades (Event_CloakFades), is marched
///   back down the steps (Event_GuardsCatchParty, FLAG_SCENE_TEMP_44). A party
///   caught in the fortress arrives by LUNPA_FORTRESS_GATES_AFTER_THROWN_OUT
///   and is thrown down the steps (Event_GuardsThrowLeaderOut); one that slips
///   out arrives by LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_4 and casts Cloak
///   to get round the guards (Event_LeaderSneaksOut). Every entry resets the
///   fortress's puzzles and beaten guards. After Donpa's judgment, until
///   Hammet finds Bunza, a Kalay merchant, and his wagon in West Lunpa Cave
///   (FLAG_LUNPA_CAVE_REUNION_SEEN), Garet stops the leader from going back up
///   (TRIGGER_FORTRESS_APPROACH, FLAG_SCENE_TEMP_43) and the left guard nearly
///   recognizes the man with the party (FLAG_LUNPA_GATE_GUARD_SAW_HAMMET).
///   LUNPA_FORTRESS_GATES_TO_LUNPA leads back down to Lunpa.

#include "game_state.h"
#include "version.h"
#include "scene_ids.h"
#include "unit/scene/lunpa.h"
#include "unit/scene/common/object_orbit.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "types.h"
#include "field_effect.h"
#include "field_event.h"
#include "engine_call.h"
#include "field_scene.h"
#include "field_runtime.h"
#include "edition.h"
#include "lunpa.h"
#include "text/msg_ids.h"
#include "field_obj.h"
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

const struct SceneEntrance *Scene_GetEntrances(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Lunpa) {
    return gLunpaEntrances;
  }
  if (scene == (s32)&SceneId_LunpaFortressGates) {
    return gGateEntrances;
  }
  return gLunpaEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Lunpa) {
    return gLunpaExits;
  }
  if (scene == (s32)&SceneId_LunpaFortressGates) {
    return gGateExits;
  }
  return gLunpaExits;
}

/// The townsfolk of Lunpa, or the guards of the Lunpa Fortress Gates.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Lunpa) {
    return gLunpaPlacements;
  }
  if (scene == (s32)&SceneId_LunpaFortressGates) {
    return gGatePlacements;
  }
  return gLunpaPlacements;
}

/// Lunpa: Catch on the Nut floating over the town gives it to the party.
static void Event_CatchFloatingNut(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(ACTOR_FLOATING_NUT, 0, 0);
  Engine_GameFlagSet(FLAG_LUNPA_NUT_CAUGHT);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

/// Restores the frozen pillar's three collision cells, including its sides.
static inline void Lunpa_ApplyFrozenPuddleCollision(void) {
  Map_CopyCellAttributes(17, 4, 1, 1, 14, 4);
  Map_CopyCellAttributes(15, 3, 1, 1, 15, 4);
  Map_CopyCellAttributes(15, 3, 1, 1, 13, 4);
}

/// Lunpa: Frost on the puddle Reveal shows freezes it into a pillar the party
/// can stand on.
static void Event_FreezeHiddenPuddle(void) {
  struct FieldActor *pillar;

  pillar = Actor_Get(ACTOR_HIDDEN_PUDDLE);
  Actor_Get(ACTOR_PARTY_LEADER);
  Lunpa_ApplyFrozenPuddleCollision();
  if (pillar != NULL) {
    Engine_ActorSetSpriteFlags(pillar, 0);
    pillar->motionFlags = ACTOR_FALLS;
    pillar->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }
  GameFlag_Set(FLAG_SCENE_TEMP_00);
}

/// Every frame while the pillar stands: draw it under the leader once the
/// leader is on top.
static void Lunpa_UpdateIcePillarDrawOrder(void) {
  if (Object_GetById(ACTOR_PARTY_LEADER)->y.fixed >= PILLAR_TOP_HEIGHT) {
    Object_GetById(ACTOR_HIDDEN_PUDDLE)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
  } else {
    Object_GetById(ACTOR_HIDDEN_PUDDLE)->priorityFlags =
        ACTOR_PRIORITY_AUTOMATIC;
  }
}

/// Draws the three map regions that make one frame of the hidden passage.
static inline void Lunpa_DrawHiddenPassageFrame(s32 sourceColumn,
                                                s32 sourceRow) {
  Map_CopyCells(sourceColumn, sourceRow, 3, 4, 1, 14);
  Map_CopyCells(sourceColumn + 3, sourceRow, 3, 4, 33, 14);
  Map_CopyCells(sourceColumn + 6, sourceRow, 3, 4, 1, 46);
}

/// Lunpa, casting Reveal: the Psynergy Stone's spot, the hidden switch's
/// glint, the puddle and, once opened, the passage to the jail show.
static void Event_RevealSecrets(void) {
  struct FieldActor *actor;
  s32 cellX;
  s32 cellZ;

  // Restore the hidden stone and pillar before exposing the switch or passage.
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  cellX = actor->x.fixed / CELL_SIZE;
  cellZ = actor->z.fixed / CELL_SIZE;
  if (GameFlag_IsSet(FLAG_LUNPA_PSYNERGY_STONE) == 0) {
    if (cellX == 7 && cellZ == 16) {
      Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 16);
    }
    MapObject_SetPosition(MAP_OBJECT_PSYNERGY_STONE, -1, -1);
    Map_CopyCellAttributes(28, 31, 1, 1, 7, 16);
  }
  Map_CopyCells(47, 4, 1, 1, 46, 4);
  Map_CopyCellAttributes(34, 37, 3, 3, 13, 3);
  Actor_SetPosition(ACTOR_HIDDEN_PUDDLE, PIXELS(232), PIXELS(72));
  Actor_Get(ACTOR_HIDDEN_PUDDLE)->y.fixed = 0;
#if TBS_PATCH_4
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_00)) {
    Lunpa_ApplyFrozenPuddleCollision();
  }
#endif
  // The opened passage replaces the glint; otherwise show the hidden switch.
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_02) != 0) {
    Lunpa_DrawHiddenPassageFrame(41, 49);
  } else {
    Actor_SetPosition(ACTOR_SWITCH_GLINT, PIXELS(56), PIXELS(268));
    Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_SWITCH_GLINT), 0);
    actor = Actor_Get(ACTOR_SWITCH_GLINT);
    if (actor != NULL) {
      actor->motionFlags = GLINT_MOTION_FLAGS;
      actor->y.fixed = PIXELS(16);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->scaleX = GLINT_SCALE_X;
      actor->scaleY = GLINT_SCALE_Y;
    }
  }
  FieldScheduler_AddCallback(Lunpa_UpdateIcePillarDrawOrder,
                             TASK_PRIORITY_SCENE);
  GameFlag_Clear(FLAG_SCENE_TEMP_01);
}

/// Lunpa, as Reveal ends and on every entry: the secrets hide again.
static void Event_HideSecrets(void) {
  struct FieldActor *puddle;

  Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetPosition(ACTOR_SWITCH_GLINT, 0, 0);
  Actor_SetPosition(ACTOR_HIDDEN_PUDDLE, 0, 0);
  Map_CopyCells(38, 38, 1, 1, 46, 4);
  Map_CopyCellAttributes(37, 37, 3, 3, 13, 3);
  Map_CopyCellAttributes(37, 37, 1, 1, 14, 2);
  Map_CopyCellAttributes(8, 16, 1, 1, 7, 16);
  MapObject_SetPosition(MAP_OBJECT_PSYNERGY_STONE, 0, 0);
  Map_CopyCells(32, 42, 3, 2, 1, 15);
  GameFlag_Clear(FLAG_SCENE_TEMP_00);
  Engine_ActorSetAnimation(ACTOR_HIDDEN_PUDDLE, ANIM_STAND);
  puddle = Actor_Get(ACTOR_HIDDEN_PUDDLE);
  puddle->update = NULL;
  Engine_ObjectSetPartPalettes(Actor_Get(ACTOR_HIDDEN_PUDDLE), 0);
  Engine_TaskRemoveCallback(Lunpa_UpdateIcePillarDrawOrder);
  GameFlag_Set(FLAG_SCENE_TEMP_01);
}

/// Searching the well (TRIGGER_WELL), in Lunpa or at the Lunpa Fortress Gates:
/// frogs croak in it.
static void Event_SearchWell(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgLunpaWellFrogs, 1);
  Engine_EventEnd();
}

/// The events of the Lunpa Fortress Gates, or the town's before and after
/// Donpa's judgment.
const struct SceneEvent *Scene_GetEvents(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_LunpaFortressGates) {
    Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED);
    return gGateEvents;
  }
  if (scene == (s32)&SceneId_Lunpa &&
      Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    return gLunpaReopenedEvents;
  }
  return gLunpaSealedEvents;
}

/// Lunpa, before Donpa's judgment: the west guard on the road in asks whether
/// the party wants to enter Lunpa, and turns them away either way.
static void Event_TalkToWestGuard(void) {
  Engine_EventBegin();
  Engine_ActorShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | 2, 60);
  Engine_EventSetMessage(MsgLunpaWestGuardAsksAboutEntering);
  Engine_EventAskYesNo(ACTOR_WEST_GUARD, 0);
  Engine_EventEnd();
}

/// Lunpa, before Donpa's judgment: the east guard on the road in wants
/// Dodonpa's authorization, or, once the guards suspect Kalay, asks whether
/// the party came from there and whether they are here to save Hammet.
static void Event_TalkToEastGuard(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_GUARDS_SUSPECT_KALAY) == 0) {
    Engine_EventSetMessage(MsgLunpaEastGuardDemandsAuthorization);
    Engine_EventShowMessage(ACTOR_EAST_GUARD, 0);
  } else {
    Engine_EventSetMessage(MsgLunpaEastGuardAsksIfFrom);
    Engine_EventOpenMessage(ACTOR_EAST_GUARD, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      gEventWork->message++;
      Engine_EventOpenMessage(ACTOR_EAST_GUARD, 0);
      if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
        gEventWork->message++;
      }
    }
    Engine_EventShowMessage(ACTOR_EAST_GUARD, 0);
  }
  Engine_EventEnd();
}

/// Lunpa: Mind Read on the east guard. He fears Dodonpa's blame, trusts the
/// cave's gate once he suspects Kalay, and after Donpa's judgment thinks the
/// merchant who was desperate to do business in the village is still around
/// somewhere, causing no trouble.
static void Event_ReadEastGuardsThoughts(void) {
  s32 thoughts;

  if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) == 0) {
    thoughts = Engine_GameFlagIsSet(FLAG_GUARDS_SUSPECT_KALAY);
    if (thoughts == 0) {
      thoughts = MsgLunpaEastGuardFearsBlame;
    } else {
      thoughts = MsgLunpaEastGuardTrustsCaveGate;
    }
    Engine_EventSetMessage(thoughts);
    Engine_EventShowMessage(ACTOR_EAST_GUARD, 0);
  } else {
    Engine_EventSetMessage(MsgLunpaEastGuardThinksMerchantHarmless);
    Engine_EventShowMessage(ACTOR_EAST_GUARD, 0);
  }
}

/// Lunpa, before Donpa's judgment: a villager asks how long the party has
/// been in Lunpa.
static void Event_AskAboutLengthOfStay(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgLunpaVillagerAAsksHowLong);
  Engine_EventAskYesNo(ACTOR_VILLAGER_A, 0);
  Engine_EventEnd();
}

/// Lunpa, before Donpa's judgment: a villager asks whether Dodonpa kidnapped
/// Hammet for ransom.
static void Event_AskAboutHammetsKidnapping(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgLunpaVillagerCAsksAboutKidnapping);
  Engine_EventAskYesNo(ACTOR_VILLAGER_C, 0);
  Engine_EventEnd();
}

/// Lunpa, before Donpa's judgment: a villager asks whether the party wants
/// to see Donpa, who is in the fortress.
static void Event_AskAboutSeeingDonpa(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgLunpaVillagerGAsksAboutDonpa);
  Engine_EventAskYesNo(ACTOR_VILLAGER_G, 0);
  Engine_EventEnd();
}

/// Lunpa, after Donpa's judgment: a villager shivers and complains that it
/// has become colder.
static void Event_ComplainAboutCold(void) {
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_VILLAGER_B, 3);
  Engine_EventSetMessage(MsgLunpaVillagerBShivers);
  Engine_EventShowMessage(ACTOR_VILLAGER_B, 0);
  Engine_EventEnd();
}

/// Lunpa, after Donpa's judgment: a villager offers to explain the commotion
/// at the fortress.
static void Event_AskAboutFortressCommotion(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgLunpaVillagerDAsksAboutCommotion);
  Engine_EventAskYesNo(ACTOR_VILLAGER_D, 0);
  Engine_EventEnd();
}

/// Lunpa Fortress Gates: the left guard hears someone while the party is
/// cloaked; after Donpa's judgment, he nearly recognizes Hammet with the
/// party.
static void Event_TalkToLeftGuard(void) {
  s32 recognitionMessageBase;

  if (gGameState.cloaked != 0) {
    Engine_EventSetMessage(MsgLunpaLeftGuardHearsSomeone);
  } else if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0 &&
             Engine_GameFlagIsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
    Engine_ActorShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 1, 60);
    recognitionMessageBase = MsgLunpaLeftGuardRecognizesHammet;
    Engine_EventSetMessage(recognitionMessageBase + RECOGNITION_SEEN_THAT_MAN);
    Engine_EventShowMessage(ACTOR_LEFT_GUARD, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
    Engine_EventSetMessage(recognitionMessageBase + RECOGNITION_IMPOSSIBLE);
    Engine_GameFlagSet(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
  } else {
    Engine_EventSetMessage(MsgLunpaLeftGuardResentsDodonpa);
  }
  Engine_EventShowMessage(ACTOR_LEFT_GUARD, 0);
}

/// Lunpa Fortress Gates: the right guard boasts, finds the place creepy while
/// the party is cloaked, and after Donpa's judgment wonders how the warriors
/// from Kalay got in.
static void Event_TalkToRightGuard(void) {
  if (gGameState.cloaked != 0) {
    Engine_EventSetMessage(MsgLunpaRightGuardFeelsCreepy);
  } else if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    Engine_EventSetMessage(MsgLunpaRightGuardWondersHow);
  } else {
    Engine_EventSetMessage(MsgLunpaRightGuardBoasts);
  }
  Engine_EventShowMessage(ACTOR_RIGHT_GUARD, 0);
}

/// Lunpa Fortress Gates: Mind Read on the left guard. After Donpa's judgment
/// and before Hammet finds Bunza, he thinks the man with the party looks like
/// that merchant, and Mind Read is cut short the first time as he turns to
/// look.
static void Event_ReadLeftGuardsThoughts(void) {
  s32 recognizedHammet;

  if (gGameState.cloaked == 0 &&
      GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0 &&
      GameFlag_IsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
    recognizedHammet = GameFlag_IsSet(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
    if (recognizedHammet == 0) {
      ((struct FieldWork *)gEventWork)->command = recognizedHammet;
      Engine_PsynergyCancel();
      Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
      Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 1, 60);
      Engine_EventSetMessage(MsgLunpaLeftGuardRecognizesHammet);
      GameFlag_Set(FLAG_LUNPA_GATE_GUARD_SAW_HAMMET);
    } else {
      Engine_EventSetMessage(MsgLunpaLeftGuardGlimpsedMerchant);
    }
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
    Engine_EventSetMessage(MsgLunpaLeftGuardDismissesThought);
  } else {
    Engine_EventSetMessage(MsgLunpaLeftGuardThoughts);
  }
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
}

/// Lunpa Fortress Gates: Mind Read on the right guard.
static void Event_ReadRightGuardsThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    Engine_EventSetMessage(MsgLunpaRightGuardReopenedThoughts);
  } else {
    Engine_EventSetMessage(MsgLunpaYoudShadowSneak);
  }
  Engine_EventShowMessage(ACTOR_RIGHT_GUARD, 0);
}

/// Every frame at the Lunpa Fortress Gates after Donpa's judgment: stepping up
/// to row 10 raises Garet's warning (TRIGGER_FORTRESS_APPROACH).
static void Lunpa_WatchForFortressApproach(void) {
  s32 leaderRow = Actor_Get(ACTOR_PARTY_LEADER)->z.fixed / CELL_SIZE;

  if (GameFlag_IsSet(FLAG_SCENE_TEMP_43) == 0 && leaderRow == 10) {
    GameFlag_Set(FLAG_SCENE_TEMP_43);
    ((struct FieldWork *)gEventWork)->action = TRIGGER_FORTRESS_APPROACH;
  }
}

/// Lunpa Fortress Gates, TRIGGER_FORTRESS_APPROACH after Donpa's judgment:
/// Garet reminds the leader they cannot go into the fortress, and the leader
/// steps back.
static void Event_GaretTurnsLeaderBack(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventSetMessage(MsgLunpaGaretRefusesToReturn);
  Event_ShowMessage(GARET, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 100);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 12);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  GameFlag_Clear(FLAG_SCENE_TEMP_43);
  Engine_EventEnd();
}

/// Lunpa, walking north into a doorway: the door opens and the leader goes
/// into the home, shop, inn or temple behind it.
static void Event_EnterDoor(void) {
  struct FieldWork *fieldWork;
  struct FieldActor *placedActor;
  u32 actorId;
  s32 doorIndex;

  fieldWork = (struct FieldWork *)gEventWork;
  Engine_EventBegin();
  // Freeze placed actors before opening the selected door and crossing it.
  for (actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_PLACED; actorId++) {
    placedActor = Actor_Get(actorId);
    if (placedActor != NULL) {
      placedActor->motionFlags = 0;
    }
  }
  Event_PlayCue(SE_DOOR_OPEN);
  doorIndex = fieldWork->action - TRIGGER_FIRST_HOME_DOOR;
  Map_AnimateCells(gLunpaDoors[doorIndex].steps, gLunpaDoors[doorIndex].x,
                   gLunpaDoors[doorIndex].y);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (doorIndex != DOOR_TEMPLE) {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
    Engine_EventWait(10);
  }
  Engine_EventRequestExit(fieldWork->action);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

/// Lunpa, TRIGGER_HIDDEN_PASSAGE: the hidden passage leads to the jail once
/// opened, while Reveal shows it.
static void Event_EnterHiddenPassage(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_02) != 0) {
    if (GameFlag_IsSet(FLAG_SCENE_TEMP_01) == 0) {
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
      Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
      Engine_EventWait(13);
      Engine_EventRequestExit(LUNPA_TO_VAULT_JAIL);
    }
  }
}

/// Lunpa Fortress Gates, TRIGGER_FORTRESS_DOOR: through the fortress door at
/// the top of the steps, into Lunpa Fortress.
static void Event_EnterFortress(void) {
  GameFlag_Set(FLAG_SCENE_TEMP_42);
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -8);
  Event_PlayCue(SE_DOOR_OPEN);
  Map_CopyCells(53, 4, 2, 2, 41, 4);
  Engine_EventWait(10);
  Map_CopyCells(53, 6, 2, 2, 41, 4);
  Engine_EventWait(10);
  Engine_EventRequestExit(LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_1);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

static void UNUSED Lunpa_CheckAhead(void) { Engine_LeaderCheckAhead(); }

/// Reveal's sound when it uncovers something.
UNUSED static void Lunpa_PlayRevealCue(void) {
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01) == 0) {
    Engine_AudioPlayCue(MUS_TREASURE_FOUND);
  }
}

/// Lunpa: searching the switch Reveal shows opens the passage to the jail in
/// five steps.
static void Event_PressHiddenSwitch(void) {
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_01) != 0) {
    return;
  }
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_02) != 0) {
    return;
  }
  // Open the passage over five map frames, then remember its final state.
  Actor_SetPosition(ACTOR_SWITCH_GLINT, 0, 0);
  Event_PlayCue(SE_HIDDEN_PASSAGE_OPEN);
  Engine_TaskWait(1);
  Lunpa_DrawHiddenPassageFrame(32, 45);
  Engine_TaskWait(10);
  Lunpa_DrawHiddenPassageFrame(41, 45);
  Engine_TaskWait(10);
  Lunpa_DrawHiddenPassageFrame(50, 45);
  Engine_TaskWait(10);
  Lunpa_DrawHiddenPassageFrame(32, 49);
  Engine_TaskWait(10);
  Lunpa_DrawHiddenPassageFrame(41, 49);
  Engine_TaskWait(10);
  GameFlag_Set(FLAG_SCENE_TEMP_02);
}

/// Copies one open or blocked source cell across the three-cell gateway.
static inline void Lunpa_CopyGatewayCollision(s32 sourceColumn, s32 sourceRow) {
  Map_CopyCellAttributes(sourceColumn, sourceRow, 1, 1, 7, 11);
  Map_CopyCellAttributes(sourceColumn, sourceRow, 1, 1, 8, 11);
  Map_CopyCellAttributes(sourceColumn, sourceRow, 1, 1, 9, 11);
}

/// Lunpa Fortress Gates, TRIGGER_GUARDED_GROUND: coming close uncloaked, the
/// guards step in front of the fortress door and the gateway closes.
static void Event_GuardsBlockGate(void) {
  if (gGameState.cloaked == 0) {
    Engine_EventBegin();
    Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 0, 2);
    Actor_ShowEmote(ACTOR_RIGHT_GUARD, EMOTE_IN_FRONT | 0, 15);
    Engine_EventWait(30);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 168);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 168);
    Engine_ActorWaitForMove(ACTOR_LEFT_GUARD);
    Engine_ActorWaitForMove(ACTOR_RIGHT_GUARD);
    Engine_ActorStop(ACTOR_LEFT_GUARD);
    Engine_ActorSetAnimation(ACTOR_LEFT_GUARD, 0);
    Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_ActorStop(ACTOR_RIGHT_GUARD);
    Engine_ActorSetAnimation(ACTOR_RIGHT_GUARD, 0);
    Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH + FACING_STEP, 0);
    Engine_EventSetMessage(MsgLunpaGuardsWarnPartyAway);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    GameFlag_Set(FLAG_SCENE_TEMP_40);
    Lunpa_CopyGatewayCollision(6, 11);
    Engine_EventEnd();
  }
}

static void UNUSED Lunpa_DoNothing(void) {}

/// Lunpa Fortress Gates, casting Cloak: the gateway closes behind the guards.
static void Event_CastCloak(void) {
  Lunpa_CopyGatewayCollision(6, 11);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_41);
}

/// Lunpa Fortress Gates, raised when the guards spot the party: they wonder
/// how the party got there and march them back down to the foot of the steps.
static void Event_GuardsCatchParty(void) {
  struct FieldActor *leader;
  s32 warningMessageBase;

  if (GameFlag_IsSet(FLAG_SCENE_TEMP_44) != 0) {
    return;
  }
  GameFlag_Set(FLAG_SCENE_TEMP_44);
  Engine_EventBegin();
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
  Engine_ActorStartRepeatedMotion(ACTOR_RIGHT_GUARD, 1);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 2, 60);
  // Warn the party before escorting everyone down the two sections of the gate.
  warningMessageBase = MsgLunpaGuardsCatchParty;
  Engine_EventSetMessage(warningMessageBase + CATCH_LEFT_GUARD_CHALLENGES);
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
  Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x20000, 0x10000);
  Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x20000, 0x10000);
  Engine_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_SHAKE_HEAD);
  Engine_EventWait(35);
  Engine_EventSetMessage(warningMessageBase + CATCH_RIGHT_GUARD_WONDERS);
  Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
  Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 3, 30);
  Engine_EventSetMessage(warningMessageBase + CATCH_LEFT_GUARD_REFUSES_ENTRY);
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
  Engine_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_NOD);
  Engine_EventWait(25);
  Engine_EventSetMessage(warningMessageBase +
                         CATCH_RIGHT_GUARD_SENDS_PARTY_OFF);
  Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
  Actor_WalkTo(ACTOR_LEFT_GUARD, leader->x.part.pixel - 1,
               leader->z.part.pixel);
  Engine_ActorWaitForMove(ACTOR_LEFT_GUARD);
  // Bring the party and both guards together at the first landing.
  Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 216);
  Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 200);
  Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 200);
  Engine_ActorWaitForMove(ACTOR_LEFT_GUARD);
  Engine_ActorWaitForMove(ACTOR_RIGHT_GUARD);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(12);
  // Continue to the foot of the gate before enabling the watcher again.
  Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 272);
  Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 256);
  Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 256);
  Engine_ActorWaitForMove(ACTOR_LEFT_GUARD);
  Engine_ActorWaitForMove(ACTOR_RIGHT_GUARD);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_EventEnd();
  FieldScheduler_AddCallback(Lunpa_WatchForUncloakedParty, TASK_PRIORITY_SCENE);
}

static void Lunpa_ReopenGateway(void) { Lunpa_CopyGatewayCollision(7, 12); }

/// Lunpa Fortress Gates, as Cloak fades: if the leader is still among the
/// guards, the party is caught.
static void Event_CloakFades(void) {
#if TBS_PATCH_1
  struct FieldWork *fieldWork;
#endif
  struct FieldActor *leader;

#if TBS_PATCH_1
  fieldWork = (struct FieldWork *)gEventWork;
#endif
  Engine_GameFlagClear(FLAG_SCENE_TEMP_41);
  Engine_GameFlagClear(FLAG_SCENE_TEMP_40);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader->x.fixed > PIXELS(104) && leader->x.fixed < PIXELS(240) &&
      leader->z.fixed > PIXELS(160) && leader->z.fixed < PIXELS(248)) {
    Engine_TaskRemoveCallback(Lunpa_WatchForUncloakedParty);
#if TBS_PATCH_1
    fieldWork->event = TRIGGER_PARTY_SPOTTED;
#else
    Event_GuardsCatchParty();
#endif
  }
  Lunpa_ReopenGateway();
  Engine_GameFlagClear(FLAG_SCENE_TEMP_44);
}

/// Every frame at the Lunpa Fortress Gates: an uncloaked leader in front of
/// the guards is spotted.
static void Lunpa_WatchForUncloakedParty(void) {
  struct FieldWork *fieldWork;
  struct FieldActor *leader;

  fieldWork = (struct FieldWork *)gEventWork;
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (gGameState.cloaked == 0 &&
      (u32)(leader->x.fixed - PIXELS(144)) <= PIXELS(32) &&
      leader->z.fixed >= PIXELS(168) && leader->z.fixed < PIXELS(176)) {
    Engine_TaskRemoveCallback(Lunpa_WatchForUncloakedParty);
    fieldWork->event = TRIGGER_PARTY_SPOTTED;
  }
}

/// Every 16 frames while the leader is thrown: a puff of dust.
static void Lunpa_KickUpLeaderDust(void) {
  struct FieldActor *leader;
  s32 dustAngle;
  s32 dustVelocity[3];

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if ((gFrameCount & 15) == 0) {
    dustAngle = (((u32)Engine_RandomNext() * 52) >> 16) * 64 + 230;
    SET_VECTOR(dustVelocity, Engine_MathCos(dustAngle) / 4, 0,
               Engine_MathSin(dustAngle) / 2);
    Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                 dustVelocity[0], dustVelocity[1], dustVelocity[2], 0, NULL);
  }
}

/// Lunpa Fortress Gates, LUNPA_FORTRESS_GATES_AFTER_THROWN_OUT, after being
/// caught in the fortress: the guards throw the leader out and down the steps.
static void Event_GuardsThrowLeaderOut(void) {
  Engine_EventBegin();
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(160), PIXELS(128));
  Actor_SetPosition(ACTOR_LEFT_GUARD, PIXELS(152), PIXELS(112));
  Actor_SetPosition(ACTOR_RIGHT_GUARD, PIXELS(168), PIXELS(112));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH + FACING_STEP, 0);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
  Engine_EventOpenScreen();
  Engine_EventWait(30);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1cccc, 0xe666);
  Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x1cccc, 0xe666);
  Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x1cccc, 0xe666);
  Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 288);
  Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 288);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 160, 296);
  FieldScheduler_AddCallback(Lunpa_KickUpLeaderDust, TASK_PRIORITY_SCENE);
  Engine_EventWait(1);
  Event_PlayCue(SE_SCUFFLE);
  Engine_EventWait(20);
  Engine_ActorSetSpritePriority(ACTOR_LEFT_GUARD, 3);
  Engine_ActorSetSpritePriority(ACTOR_RIGHT_GUARD, 3);
  Event_PlayCue(SE_SCUFFLE);
  Engine_EventWait(30);
  Actor_Get(ACTOR_LEFT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_RIGHT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Event_PlayCue(SE_SCUFFLE);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
  Engine_TaskRemoveCallback(Lunpa_KickUpLeaderDust);
  // Throw the leader down the steps, then wait for the first landing.
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags |= ACTOR_FALLS;
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = PIXELS(6);
  Actor_Get(ACTOR_PARTY_LEADER)->velocityZ = PIXELS(6);
  Engine_EventWait(1);
  while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
    Engine_EventWait(1);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SPRAWLED);
  Event_PlayCue(SE_LANDING_THUD);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  FieldScheduler_AddCallback(Lunpa_KickUpLeaderDust, TASK_PRIORITY_SCENE);
  Engine_EventWait(2);
  // A smaller second bounce ends with the guards' final warning.
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = PIXELS(3);
  Engine_EventWait(1);
  while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
    Engine_EventWait(1);
  }
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_TaskRemoveCallback(Lunpa_KickUpLeaderDust);
  Engine_EventWait(50);
  Engine_EventSetMessage(MsgLunpaGuardForbidsReturn);
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
  Actor_Get(ACTOR_LEFT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_RIGHT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x10000, 0x8000);
  Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x10000, 0x8000);
  Actor_WalkTo(ACTOR_LEFT_GUARD, 144, 200);
  Actor_WalkTo(ACTOR_RIGHT_GUARD, 176, 200);
  Engine_ActorWaitForMove(ACTOR_LEFT_GUARD);
  Engine_ActorWaitForMove(ACTOR_RIGHT_GUARD);
  Engine_ActorSetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventEnd();
}

/// Lunpa Fortress Gates, LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_4, after
/// slipping out of the fortress: the leader casts Cloak and slips round the
/// guards.
static void Event_LeaderSneaksOut(void) {
  Engine_EventBegin();
  Engine_EventOpenScreen();
  Actor_WalkTo(ACTOR_PARTY_LEADER, 152, 168);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_EventWait(20);
  // Cast Cloak before following the route around the guards.
  Engine_PsynergyBegin(ABILITY_CLOAK, 1);
  Engine_PsynergySetTarget(ACTOR_PARTY_LEADER, 0);
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_PsynergyLowerHands();
  Actor_WalkTo(ACTOR_PARTY_LEADER, 144, 184);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 184);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 200);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 72, 200);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 72, 288);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 288);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_EventEnd();
}

/// Sets Lunpa or the Lunpa Fortress Gates up: in the town, Reveal's secrets
/// hidden and the floating Nut; at the gates, the thrown-out or sneaking-out
/// scene, the watchers, and the fortress's puzzle and guard flags cleared.
s32 Scene_Initialize(void) {
  struct FieldActor *puddle;

  if (gGameState.scene == (s32)&SceneId_Lunpa) {
    ((struct FieldWork *)gEventWork)->transition =
        SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_HideSecrets();
    if (Engine_GameFlagIsSet(FLAG_LUNPA_NUT_CAUGHT) == 0) {
      Scene_FloatNut(ACTOR_FLOATING_NUT);
    }
    puddle = Object_GetById(ACTOR_HIDDEN_PUDDLE);
    if (puddle != NULL) {
      Engine_ActorSetSpriteFlags(puddle, 0);
    }
    Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
  }
  // Restore the gate's arrival scene and active detection tasks separately.
  if (gGameState.scene == (s32)&SceneId_LunpaFortressGates) {
    ((struct FieldWork *)gEventWork)->transition =
        SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gGameState.retreatEntrance = GATE_RETREAT_ENTRANCE;
    if (gGameState.entrance == LUNPA_FORTRESS_GATES_AFTER_THROWN_OUT &&
        Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Event_GuardsThrowLeaderOut();
    }
    if (gGameState.entrance == LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_4 &&
        Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Event_LeaderSneaksOut();
    }
    if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0 &&
        Engine_GameFlagIsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
      FieldScheduler_AddCallback(Lunpa_WatchForFortressApproach,
                                 TASK_PRIORITY_SCENE);
    }
#if TBS_PATCH_4
    if (gGameState.cloaked != 0)
      Event_CastCloak();
#endif
    FieldScheduler_AddCallback(Lunpa_WatchForUncloakedParty,
                               TASK_PRIORITY_SCENE);
    // The fortress's puzzles and beaten guards are reset.
    Engine_GameFlagClear(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_UNCOVERED);
    Engine_GameFlagClear(FLAG_LUNPA_FORTRESS_PASSAGE_UNCOVERED);
    Engine_GameFlagClear(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_UNCOVERED);
    Engine_GameFlagClear(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED);
    Engine_GameFlagClear(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED);
    Engine_GameFlagClear(FLAG_LUNPA_FORTRESS_COLUMN_LOWERED);
    Engine_GameFlagClear(FLAG_LUNPA_FORTRESS_HALL_GUARD_BEATEN);
    Engine_GameFlagClear(FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN);
    Engine_GameFlagClear(FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN);
    Engine_GameFlagClear(FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN);
  }
  return 0;
}

#include "common/object_orbit.h"

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
    LUNPA_ENTRANCE(LUNPA_FROM_EIGHTH_ROOM, 216, 344, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_FROM_VAULT_JAIL, 40, 328, FACING_SOUTH),
    {SCENE_TABLE_END},
};

struct SceneEntrance gGateEntrances[] = {
    GATE_ENTRANCE(0, 152, 328, FACING_NORTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_1, 160, 104,
                  FACING_SOUTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA, 152, 328, FACING_NORTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_4, 144, 104,
                  FACING_SOUTH),
    GATE_ENTRANCE(LUNPA_FORTRESS_GATES_AFTER_THROWN_OUT, 160, 224, FACING_EAST),
    {SCENE_TABLE_END},
};

#define ROOM_EXIT(exit) SCENE_EXIT((exit), LunpaSuhalla, ROOM_ENTRANCE(exit))

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
    ROOM_EXIT(LUNPA_TO_EIGHTH_ROOM),
    SCENE_EXIT(LUNPA_TO_VAULT_JAIL, VaultJail, VAULT_JAIL_FROM_LUNPA),
    SCENE_EXITS_END,
};

u32 gGateExits[] = {
    SCENE_EXITS(LunpaFortressGates),
    SCENE_EXIT(LUNPA_FORTRESS_GATES_TO_LUNPA_FORTRESS_1, LunpaFortress1,
               LUNPA_FORTRESS_1_FROM_LUNPA_FORTRESS_GATES),
    SCENE_EXIT(LUNPA_FORTRESS_GATES_TO_LUNPA, Lunpa,
               LUNPA_FROM_LUNPA_FORTRESS_GATES),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gLunpaPlacements[] = {
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(520), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_TOWNSPERSON_D, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(112), 0, PIXELS(168), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_C, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(280), 0, PIXELS(352), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_TOWNSPERSON_I, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(120), 0, PIXELS(312), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_J, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(136), 0, PIXELS(328), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_B, CONDITION_ALWAYS, ACTOR_WANDER,
      PIXELS(368), 0, PIXELS(256), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_E, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(280), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_H, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(216), 0, PIXELS(40), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_TOWNSPERSON_G, CONDITION_ALWAYS, ACTOR_WANDER,
      PIXELS(152), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_GUARD, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(192), 0, PIXELS(448), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_GUARD, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(256), 0, PIXELS(448), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_GLINT, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(520), 0, PIXELS(520), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_ITEM_ICON, FLAG_LUNPA_NUT_CAUGHT, ACTOR_FLOAT,
      PIXELS(120), 0, PIXELS(472), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END, CONDITION_ALWAYS },
};

struct ScenePlacement gGatePlacements[] = {
    { SPRITE_GUARD, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(144), 0, PIXELS(200), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_GUARD, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(176), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

#define TALK(actor, value) {EVENT_TALK, (actor), CONDITION_ALWAYS, (value)}
#define MIND_READ(actor, value)                                                \
  {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), (actor), CONDITION_ALWAYS, (value)}

#define DOOR(trigger)                                                          \
  {TOUCH_WALKING_INTO(FACING_NORTH), (trigger), CONDITION_ALWAYS,              \
   EVENT_SCRIPT(Event_EnterDoor)}

#define LUNPA_ROADS_AND_DOORS                                                  \
  {EVENT_EXIT, TRIGGER_WORLD_MAP_ROAD, CONDITION_ALWAYS, LUNPA_TO_WORLD_MAP},  \
      {EVENT_EXIT, TRIGGER_GATE_ROAD, CONDITION_ALWAYS,                        \
       LUNPA_TO_LUNPA_FORTRESS_GATES},                                         \
      {EVENT_EXIT, TRIGGER_CAVE_ROAD, CONDITION_ALWAYS,                        \
       LUNPA_TO_WEST_LUNPA_CAVE},                                              \
      DOOR(TRIGGER_FIRST_HOME_DOOR), DOOR(TRIGGER_SECOND_HOME_DOOR),           \
      DOOR(TRIGGER_THIRD_HOME_DOOR), DOOR(TRIGGER_ARMS_SHOP_DOOR),             \
      DOOR(TRIGGER_ITEM_SHOP_DOOR), DOOR(TRIGGER_INN_DOOR),                    \
      DOOR(TRIGGER_TEMPLE_DOOR), DOOR(TRIGGER_EIGHTH_ROOM_DOOR), {             \
    TOUCH_WALKING_INTO(FACING_NORTH), TRIGGER_HIDDEN_PASSAGE,                  \
        CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterHiddenPassage)               \
  }

#define LUNPA_SECRETS                                                          \
  {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_HIDDEN_PUDDLE, FLAG_SCENE_TEMP_00,  \
   EVENT_SCRIPT(Event_FreezeHiddenPuddle)},                                    \
      {PSYNERGY_IN_AREA(ABILITY_REVEAL, REVEAL_SHOWN), 0, CONDITION_ALWAYS,    \
       EVENT_SCRIPT(Event_RevealSecrets)},                                     \
      {PSYNERGY_IN_AREA(ABILITY_REVEAL, REVEAL_FADED), 0, CONDITION_ALWAYS,    \
       EVENT_SCRIPT(Event_HideSecrets)},                                       \
      {SEARCH(SEARCH_UNNAMED), TRIGGER_HIDDEN_SWITCH, FLAG_SCENE_TEMP_01,      \
       EVENT_SCRIPT(Event_PressHiddenSwitch)},                                 \
      {SEARCH(SEARCH_UNNAMED), TRIGGER_PSYNERGY_STONE,                         \
       FLAG_LUNPA_PSYNERGY_STONE, FIND_PSYNERGY_STONE},                        \
      {SEARCH(SEARCH_GRAVE), TRIGGER_GRAVE_SLEEP_BOMB,                         \
       FLAG_LUNPA_GRAVE_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB)},               \
      {SEARCH(SEARCH_BARREL), TRIGGER_BARREL_NUT, FLAG_LUNPA_BARREL_NUT,       \
       FIND_ITEM(ITEM_NUT)},                                                   \
      {PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FLOATING_NUT,                   \
       FLAG_LUNPA_NUT_CAUGHT, EVENT_SCRIPT(Event_CatchFloatingNut)},           \
      {SEARCH(SEARCH_UNNAMED), TRIGGER_WELL, CONDITION_ALWAYS,                 \
       EVENT_SCRIPT(Event_SearchWell)},                                        \
  {                                                                            \
    SCENE_EVENTS_END                                                           \
  }

struct SceneEvent gLunpaSealedEvents[] = {
    LUNPA_ROADS_AND_DOORS,
    TALK(ACTOR_VILLAGER_A, EVENT_SCRIPT(Event_AskAboutLengthOfStay)),
    TALK(ACTOR_VILLAGER_B, MSGNO(MsgLunpaVillageVillagerBSealed)),
    TALK(ACTOR_VILLAGER_C, EVENT_SCRIPT(Event_AskAboutHammetsKidnapping)),
    TALK(ACTOR_VILLAGER_D, MSGNO(MsgLunpaVillageVillagerDSealed)),
    TALK(ACTOR_VILLAGER_E, MSGNO(MsgLunpaVillageVillagerESealed)),
    TALK(ACTOR_VILLAGER_F, MSGNO(MsgLunpaVillageVillagerFSealed)),
    TALK(ACTOR_VILLAGER_G, EVENT_SCRIPT(Event_AskAboutSeeingDonpa)),
    TALK(ACTOR_VILLAGER_H, MSGNO(MsgLunpaVillageVillagerHSealed)),
    TALK(ACTOR_WEST_GUARD, EVENT_SCRIPT(Event_TalkToWestGuard)),
    TALK(ACTOR_EAST_GUARD, EVENT_SCRIPT(Event_TalkToEastGuard)),
    MIND_READ(ACTOR_VILLAGER_A, MSGNO(MsgLunpaVillageVillagerASealedThoughts)),
    MIND_READ(ACTOR_VILLAGER_B, MSGNO(MsgLunpaVillageVillagerBSealedThoughts)),
    MIND_READ(ACTOR_VILLAGER_C, MSGNO(MsgLunpaVillageVillagerCSealedThoughts)),
    MIND_READ(ACTOR_VILLAGER_D, MSGNO(MsgLunpaVillageVillagerDSealedThoughts)),
    MIND_READ(ACTOR_VILLAGER_E, MSGNO(MsgLunpaVillageVillagerESealedThoughts)),
    MIND_READ(ACTOR_VILLAGER_F, MSGNO(MsgLunpaVillageVillagerFSealedThoughts)),
    MIND_READ(ACTOR_VILLAGER_G, MSGNO(MsgLunpaVillageVillagerGSealedThoughts)),
    MIND_READ(ACTOR_VILLAGER_H, MSGNO(MsgLunpaVillageVillagerHSealedThoughts)),
    MIND_READ(ACTOR_WEST_GUARD, MSGNO(MsgLunpaVillageWestGuardSealedThoughts)),
    MIND_READ(ACTOR_EAST_GUARD, EVENT_SCRIPT(Event_ReadEastGuardsThoughts)),
    LUNPA_SECRETS,
};

struct SceneEvent gLunpaReopenedEvents[] = {
    LUNPA_ROADS_AND_DOORS,
    TALK(ACTOR_VILLAGER_A, MSGNO(MsgLunpaVillageVillagerAReopened)),
    TALK(ACTOR_VILLAGER_B, EVENT_SCRIPT(Event_ComplainAboutCold)),
    TALK(ACTOR_VILLAGER_C, MSGNO(MsgLunpaVillageVillagerCReopened)),
    TALK(ACTOR_VILLAGER_D, EVENT_SCRIPT(Event_AskAboutFortressCommotion)),
    TALK(ACTOR_VILLAGER_E, MSGNO(MsgLunpaVillageVillagerEReopened)),
    TALK(ACTOR_VILLAGER_F, MSGNO(MsgLunpaVillageVillagerFReopened)),
    TALK(ACTOR_VILLAGER_G, MSGNO(MsgLunpaVillageVillagerGReopened)),
    TALK(ACTOR_VILLAGER_H, MSGNO(MsgLunpaVillageVillagerHReopened)),
    TALK(ACTOR_WEST_GUARD, MSGNO(MsgLunpaVillageWestGuardReopened)),
    TALK(ACTOR_EAST_GUARD, MSGNO(MsgLunpaVillageEastGuardReopened)),
    MIND_READ(ACTOR_VILLAGER_A,
              MSGNO(MsgLunpaVillageVillagerAReopenedThoughts)),
    MIND_READ(ACTOR_VILLAGER_B,
              MSGNO(MsgLunpaVillageVillagerBReopenedThoughts)),
    MIND_READ(ACTOR_VILLAGER_C,
              MSGNO(MsgLunpaVillageVillagerCReopenedThoughts)),
    MIND_READ(ACTOR_VILLAGER_D,
              MSGNO(MsgLunpaVillageVillagerDReopenedThoughts)),
    MIND_READ(ACTOR_VILLAGER_E,
              MSGNO(MsgLunpaVillageVillagerEReopenedThoughts)),
    MIND_READ(ACTOR_VILLAGER_F,
              MSGNO(MsgLunpaVillageVillagerFReopenedThoughts)),
    MIND_READ(ACTOR_VILLAGER_G,
              MSGNO(MsgLunpaVillageVillagerGReopenedThoughts)),
    MIND_READ(ACTOR_VILLAGER_H,
              MSGNO(MsgLunpaVillageVillagerHReopenedThoughts)),
    MIND_READ(ACTOR_WEST_GUARD,
              MSGNO(MsgLunpaVillageWestGuardReopenedThoughts)),
    MIND_READ(ACTOR_EAST_GUARD, EVENT_SCRIPT(Event_ReadEastGuardsThoughts)),
    LUNPA_SECRETS,
};

// clang-format off
struct SceneEvent gGateEvents[] = {
    { EVENT_EXIT, TRIGGER_LUNPA_ROAD, CONDITION_ALWAYS, LUNPA_FORTRESS_GATES_TO_LUNPA },
    { EVENT_EXIT, TRIGGER_THIRD_ROAD, CONDITION_ALWAYS, GATE_EXIT_THIRD },
    { PSYNERGY_IN_AREA(ABILITY_CLOAK, CLOAK_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CastCloak) },
    { PSYNERGY_IN_AREA(ABILITY_CLOAK, CLOAK_FADED), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CloakFades) },
    { EVENT_TOUCH, TRIGGER_FORTRESS_DOOR, FLAG_SCENE_TEMP_42,
      EVENT_SCRIPT(Event_EnterFortress) },
    { EVENT_TOUCH, TRIGGER_GUARDED_GROUND, FLAG_SCENE_TEMP_40,
      EVENT_SCRIPT(Event_GuardsBlockGate) },
    { EVENT_TOUCH, TRIGGER_FORTRESS_APPROACH, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_GaretTurnsLeaderBack) },
    { EVENT_RAISED, TRIGGER_PARTY_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(Event_GuardsCatchParty) },
    TALK(ACTOR_LEFT_GUARD, EVENT_SCRIPT(Event_TalkToLeftGuard)),
    TALK(ACTOR_RIGHT_GUARD, EVENT_SCRIPT(Event_TalkToRightGuard)),
    MIND_READ(ACTOR_LEFT_GUARD, EVENT_SCRIPT(Event_ReadLeftGuardsThoughts)),
    MIND_READ(ACTOR_RIGHT_GUARD, EVENT_SCRIPT(Event_ReadRightGuardsThoughts)),
    { SEARCH(SEARCH_UNNAMED), TRIGGER_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchWell) },
    { SCENE_EVENTS_END },
};
// clang-format on

u16 gLunpaDoorSteps[DOOR_COUNT][11] = {
    {MAP_CELL_STEP(38, 35, 2, 2, 5), MAP_CELL_STEP(40, 35, 2, 2, 5),
     MAP_CELL_STEPS_END},
    {MAP_CELL_STEP(42, 35, 2, 2, 5), MAP_CELL_STEP(44, 35, 2, 2, 5),
     MAP_CELL_STEPS_END},
    {MAP_CELL_STEP(46, 33, 2, 2, 5), MAP_CELL_STEP(48, 33, 2, 2, 5),
     MAP_CELL_STEPS_END},
    {MAP_CELL_STEP(46, 35, 2, 2, 5), MAP_CELL_STEP(48, 35, 2, 2, 5),
     MAP_CELL_STEPS_END},
    {MAP_CELL_STEP(50, 33, 2, 2, 5), MAP_CELL_STEP(52, 33, 2, 2, 5),
     MAP_CELL_STEPS_END},
    {MAP_CELL_STEP(50, 35, 2, 2, 5), MAP_CELL_STEP(52, 35, 2, 2, 5),
     MAP_CELL_STEPS_END},
    {MAP_CELL_STEP(42, 33, 4, 2, 5), MAP_CELL_STEP(38, 33, 4, 2, 5),
     MAP_CELL_STEPS_END},
    {MAP_CELL_STEP(54, 33, 2, 2, 5), MAP_CELL_STEP(56, 33, 2, 2, 5),
     MAP_CELL_STEPS_END},
};

/// Each door's opening animation and the map cell it plays at, by its
/// trigger from TRIGGER_FIRST_HOME_DOOR.
struct MapCellAnim gLunpaDoors[] = {
    {gLunpaDoorSteps[0], 46, 9},  {gLunpaDoorSteps[1], 52, 12},
    {gLunpaDoorSteps[2], 52, 18}, {gLunpaDoorSteps[3], 45, 20},
    {gLunpaDoorSteps[4], 51, 5},  {gLunpaDoorSteps[5], 37, 9},
    {gLunpaDoorSteps[6], 33, 6},  {gLunpaDoorSteps[7], 44, 17},
};

u32 gLunpaUnusedWord = 0;
