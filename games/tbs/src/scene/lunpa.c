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
#include "edition.h"
#include "lunpa.h"
#include "text/msg_ids.h"
#include "field_obj.h"

// Lunpa, the town under Dodonpa's fortress, on two maps: the town itself
// (Lunpa 1), with its well, its hidden passage to the jail and Reveal's
// secrets, and the gate below the fortress (Lunpa 2), where two guards turn
// the party away until Cloak gets them past.

static void Guards_Watch(void);

enum { PALETTE_TRANSLUCENT_EFFECT = 14, PALETTE_EFFECT = 15 };

// Shared scene code, included where the ROM places it.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

// How long an effect of common/effect_spawn.h lasts: 22, 44 or 126
// frames, then it moves away and is freed.
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

// The town's entrances, or the gate's.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Lunpa1) {
    return gLunpaEntrances;
  }
  if (scene == (s32)&SceneId_Lunpa2) {
    return gGateEntrances;
  }
  return gLunpaEntrances;
}

// Lunpa has no regions.
const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

// Where each exit of the town or the gate leads.
const u32 *Scene_GetExits(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Lunpa1) {
    return gLunpaExits;
  }
  if (scene == (s32)&SceneId_Lunpa2) {
    return gGateExits;
  }
  return gLunpaExits;
}

// The townsfolk, or the gate's guards.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Lunpa1) {
    return gLunpaPlacements;
  }
  if (scene == (s32)&SceneId_Lunpa2) {
    return gGatePlacements;
  }
  return gLunpaPlacements;
}

// Catch on the nut floating over the town: the party gets it.
static void FloatingNut_Catch(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(ACTOR_FLOATING_NUT, 0, 0);
  Engine_GameFlagSet(FLAG_LUNPA_NUT_CAUGHT);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

// Frost on the puddle Reveal shows: it freezes into a pillar the party can
// stand on.
static void HiddenPuddle_Freeze(void) {
  struct FieldActor *pillar;

  pillar = Actor_Get(ACTOR_HIDDEN_PUDDLE);
  Actor_Get(ACTOR_PARTY_LEADER);
  Map_CopyCellAttributes(17, 4, 1, 1, 14, 4);
  Map_CopyCellAttributes(15, 3, 1, 1, 15, 4);
  Map_CopyCellAttributes(15, 3, 1, 1, 13, 4);
  if (pillar != NULL) {
    Engine_ActorSetSpriteFlags(pillar, 0);
    pillar->motionFlags = ACTOR_FALLS;
    pillar->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  }
  GameFlag_Set(FLAG_LUNPA_PUDDLE_FROZEN);
}

// Every frame while the pillar stands: draw it under the leader once the
// leader is on top.
static void IcePillar_UpdateDrawOrder(void) {
  if (Object_GetById(ACTOR_PARTY_LEADER)->y.fixed >= PILLAR_TOP_HEIGHT) {
    Object_GetById(ACTOR_HIDDEN_PUDDLE)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
  } else {
    Object_GetById(ACTOR_HIDDEN_PUDDLE)->priorityFlags =
        ACTOR_PRIORITY_AUTOMATIC;
  }
}

// Reveal: the psynergy stone's spot, the hidden switch's glint, the
// puddle and, once opened, the passage show.
static void Reveal_ShowSecrets(void) {
  struct FieldActor *actor;
  s32 cellX;
  s32 cellZ;

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
  if (GameFlag_IsSet(FLAG_LUNPA_PUDDLE_FROZEN)) {
    Map_CopyCellAttributes(17, 4, 1, 1, 14, 4);
    Map_CopyCellAttributes(15, 3, 1, 1, 15, 4);
    Map_CopyCellAttributes(15, 3, 1, 1, 13, 4);
  }
#endif
  if (GameFlag_IsSet(FLAG_LUNPA_PASSAGE_OPEN) != 0) {
    Map_CopyCells(41, 49, 3, 4, 1, 14);
    Map_CopyCells(44, 49, 3, 4, 33, 14);
    Map_CopyCells(47, 49, 3, 4, 1, 46);
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
  FieldScheduler_AddCallback(IcePillar_UpdateDrawOrder, TASK_PRIORITY_SCENE);
  GameFlag_Clear(FLAG_LUNPA_SECRETS_HIDDEN);
}

// Reveal's end, and every entry to the town: the secrets hide again.
static void Reveal_HideSecrets(void) {
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
  GameFlag_Clear(FLAG_LUNPA_PUDDLE_FROZEN);
  Engine_ActorSetAnimation(ACTOR_HIDDEN_PUDDLE, ANIM_STAND);
  puddle = Actor_Get(ACTOR_HIDDEN_PUDDLE);
  puddle->update = NULL;
  Engine_ObjectSetPartPalettes(Actor_Get(ACTOR_HIDDEN_PUDDLE), 0);
  Engine_TaskRemoveCallback(IcePillar_UpdateDrawOrder);
  GameFlag_Set(FLAG_LUNPA_SECRETS_HIDDEN);
}

// Searching the well.
static void Well_Search(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgLunpaWellFrogs, 1);
  Engine_EventEnd();
}

// The gate's events, or the town's before and after trade reopens.
const struct SceneEvent *Scene_GetEvents(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Lunpa2) {
    Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED);
    return gGateEvents;
  }
  if (scene == (s32)&SceneId_Lunpa1 &&
      Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    return gLunpaReopenedEvents;
  }
  return gLunpaSealedEvents;
}

// The west guard of the town asks whether the party wants in.
static void WestGuard_Talk(void) {
  Engine_EventBegin();
  Engine_ActorShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | 2, 60);
  Engine_EventSetMessage(MsgLunpaWestGuardAsksAboutEntering);
  Engine_EventAskYesNo(ACTOR_WEST_GUARD, 0);
  Engine_EventEnd();
}

// The east guard: wants authorization, or once the guards suspect Kalay,
// asks twice whether the party came from there.
static void EastGuard_Talk(void) {
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

// Mind Read on the east guard.
static void EastGuard_MindRead(void) {
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

// The townsfolk's questions and remarks.
static void VillagerA_Talk(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgLunpaVillagerAAsksHowLong);
  Engine_EventAskYesNo(ACTOR_VILLAGER_A, 0);
  Engine_EventEnd();
}

static void VillagerC_Talk(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgLunpaVillagerCAsksAboutKidnapping);
  Engine_EventAskYesNo(ACTOR_VILLAGER_C, 0);
  Engine_EventEnd();
}

static void VillagerG_Talk(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgLunpaVillagerGAsksAboutDonpa);
  Engine_EventAskYesNo(ACTOR_VILLAGER_G, 0);
  Engine_EventEnd();
}

static void VillagerB_Talk(void) {
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_VILLAGER_B, 3);
  Engine_EventSetMessage(MsgLunpaVillagerBShivers);
  Engine_EventShowMessage(ACTOR_VILLAGER_B, 0);
  Engine_EventEnd();
}

static void VillagerD_Talk(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgLunpaVillagerDAsksAboutCommotion);
  Engine_EventAskYesNo(ACTOR_VILLAGER_D, 0);
  Engine_EventEnd();
}

// The gate's left guard: hears someone while the party is cloaked; after
// trade reopens, nearly recognizes Hammet with the party.
static void LeftGuard_Talk(void) {
  s32 recognition;

  if (gGameState.cloaked != 0) {
    Engine_EventSetMessage(MsgLunpaLeftGuardHearsSomeone);
  } else if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0 &&
             Engine_GameFlagIsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
    Engine_ActorShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 1, 60);
    recognition = MsgLunpaLeftGuardRecognizesHammet;
    Engine_EventSetMessage(recognition + RECOGNITION_SEEN_THAT_MAN);
    Engine_EventShowMessage(ACTOR_LEFT_GUARD, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
    Engine_EventSetMessage(recognition + RECOGNITION_IMPOSSIBLE);
    Engine_GameFlagSet(FLAG_GATE_GUARD_SAW_HAMMET);
  } else {
    Engine_EventSetMessage(MsgLunpaLeftGuardResentsDodonpa);
  }
  Engine_EventShowMessage(ACTOR_LEFT_GUARD, 0);
}

// The gate's right guard.
static void RightGuard_Talk(void) {
  if (gGameState.cloaked != 0) {
    Engine_EventSetMessage(MsgLunpaRightGuardFeelsCreepy);
  } else if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    Engine_EventSetMessage(MsgLunpaRightGuardWondersHow);
  } else {
    Engine_EventSetMessage(MsgLunpaRightGuardBoasts);
  }
  Engine_EventShowMessage(ACTOR_RIGHT_GUARD, 0);
}

// Mind Read on the left guard: before the cave reunion he thinks the man
// with the party looks like that merchant, and Mind Read is cut short the
// first time as he turns to look.
static void LeftGuard_MindRead(void) {
  s32 seen;

  if (gGameState.cloaked == 0 &&
      GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0 &&
      GameFlag_IsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
    seen = GameFlag_IsSet(FLAG_GATE_GUARD_SAW_HAMMET);
    if (seen == 0) {
      gEventWork->psynergyRequest = seen;
      Engine_PsynergyCancel();
      Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
      Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 1, 60);
      Engine_EventSetMessage(MsgLunpaLeftGuardRecognizesHammet);
      GameFlag_Set(FLAG_GATE_GUARD_SAW_HAMMET);
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

// Mind Read on the right guard.
static void RightGuard_MindRead(void) {
  if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    Engine_EventSetMessage(MsgLunpaRightGuardReopenedThoughts);
  } else {
    Engine_EventSetMessage(MsgLunpaYoudShadowSneak);
  }
  Engine_EventShowMessage(ACTOR_RIGHT_GUARD, 0);
}

// Every frame at the gate after trade reopens: stepping up to row 10
// raises Garet's warning (trigger TRIGGER_FORTRESS_APPROACH).
static void Party_WatchForFortress(void) {
  s32 cellZ = Actor_Get(ACTOR_PARTY_LEADER)->z.fixed / CELL_SIZE;

  if (GameFlag_IsSet(FLAG_GATE_TURNING_BACK) == 0 && cellZ == 10) {
    GameFlag_Set(FLAG_GATE_TURNING_BACK);
    gEventWork->touchedTrigger = TRIGGER_FORTRESS_APPROACH;
  }
}

// Garet reminds the leader they cannot go into the fortress, and the
// leader steps back.
static void Garet_TurnsBack(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventSetMessage(MsgLunpaGaretRefusesToReturn);
  Event_ShowMessage(GARET, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 100);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 12);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  GameFlag_Clear(FLAG_GATE_TURNING_BACK);
  Engine_EventEnd();
}

// The leader walks into a doorway: the door opens and the leader goes in.
static void Door_Enter(void) {
  struct EventWork *work;
  struct FieldActor *actor;
  u32 id;
  s32 index;

  work = gEventWork;
  Engine_EventBegin();
  for (id = ACTOR_FIRST_PLACED; id <= ACTOR_LAST_PLACED; id++) {
    actor = Actor_Get(id);
    if (actor != NULL) {
      actor->motionFlags = 0;
    }
  }
  Event_PlayCue(SOUND_DOOR_OPEN);
  index = work->touchedTrigger - TRIGGER_FIRST_HOME_DOOR;
  Map_AnimateCells(gLunpaDoors[index].steps, gLunpaDoors[index].x,
                   gLunpaDoors[index].y);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (index != DOOR_TEMPLE) {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
    Engine_EventWait(10);
  }
  Engine_EventRequestExit(work->touchedTrigger);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

// The hidden passage to the jail, once opened and while Reveal shows it.
static void HiddenPassage_Enter(void) {
  if (GameFlag_IsSet(FLAG_LUNPA_PASSAGE_OPEN) != 0) {
    if (GameFlag_IsSet(FLAG_LUNPA_SECRETS_HIDDEN) == 0) {
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
      Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
      Engine_EventWait(13);
      Engine_EventRequestExit(LUNPA_EXIT_TO_JAIL);
    }
  }
}

// Through the fortress door, out of the gate's top.
static void Fortress_Enter(void) {
  GameFlag_Set(FLAG_GATE_FORTRESS_ENTERED);
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -8);
  Event_PlayCue(SOUND_DOOR_OPEN);
  Map_CopyCells(53, 4, 2, 2, 41, 4);
  Engine_EventWait(10);
  Map_CopyCells(53, 6, 2, 2, 41, 4);
  Engine_EventWait(10);
  Engine_EventRequestExit(GATE_EXIT_TO_FORTRESS);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

// The leader looks at what is ahead.
static void Party_CheckAhead(void) { Engine_LeaderCheckAhead(); }

// Reveal's sound when it uncovers something.
static void Reveal_PlayTreasureCue(void) {
  if (Engine_GameFlagIsSet(FLAG_LUNPA_SECRETS_HIDDEN) == 0) {
    Engine_AudioPlayCue(SOUND_TREASURE_FOUND);
  }
}

// Searching the switch Reveal shows: the passage opens in five steps.
static void HiddenSwitch_Press(void) {
  if (GameFlag_IsSet(FLAG_LUNPA_SECRETS_HIDDEN) != 0) {
    return;
  }
  if (GameFlag_IsSet(FLAG_LUNPA_PASSAGE_OPEN) != 0) {
    return;
  }
  Actor_SetPosition(ACTOR_SWITCH_GLINT, 0, 0);
  Event_PlayCue(SOUND_HIDDEN_PASSAGE_OPEN);
  Engine_TaskWait(1);
  Map_CopyCells(32, 45, 3, 4, 1, 14);
  Map_CopyCells(35, 45, 3, 4, 33, 14);
  Map_CopyCells(38, 45, 3, 4, 1, 46);
  Engine_TaskWait(10);
  Map_CopyCells(41, 45, 3, 4, 1, 14);
  Map_CopyCells(44, 45, 3, 4, 33, 14);
  Map_CopyCells(47, 45, 3, 4, 1, 46);
  Engine_TaskWait(10);
  Map_CopyCells(50, 45, 3, 4, 1, 14);
  Map_CopyCells(53, 45, 3, 4, 33, 14);
  Map_CopyCells(56, 45, 3, 4, 1, 46);
  Engine_TaskWait(10);
  Map_CopyCells(32, 49, 3, 4, 1, 14);
  Map_CopyCells(35, 49, 3, 4, 33, 14);
  Map_CopyCells(38, 49, 3, 4, 1, 46);
  Engine_TaskWait(10);
  Map_CopyCells(41, 49, 3, 4, 1, 14);
  Map_CopyCells(44, 49, 3, 4, 33, 14);
  Map_CopyCells(47, 49, 3, 4, 1, 46);
  Engine_TaskWait(10);
  GameFlag_Set(FLAG_LUNPA_PASSAGE_OPEN);
}

// Coming close uncloaked, the guards step in front of the fortress door
// and the gateway closes.
static void Guards_BlockGate(void) {
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
    GameFlag_Set(FLAG_GATE_GUARDS_BLOCKING);
    Map_CopyCellAttributes(6, 11, 1, 1, 7, 11);
    Map_CopyCellAttributes(6, 11, 1, 1, 8, 11);
    Map_CopyCellAttributes(6, 11, 1, 1, 9, 11);
    Engine_EventEnd();
  }
}

// A trigger that does nothing.
static void Scene_DoNothing(void) {}

// Cloak: the gateway closes behind the guards.
static void Cloak_Begin(void) {
  Map_CopyCellAttributes(6, 11, 1, 1, 7, 11);
  Map_CopyCellAttributes(6, 11, 1, 1, 8, 11);
  Map_CopyCellAttributes(6, 11, 1, 1, 9, 11);
  Engine_GameFlagSet(FLAG_GATE_CLOAK_CAST);
}

// Raised when the guards spot the party: they recognize the man with them,
// and march the party back down to the gate's foot.
static void Guards_CatchParty(void) {
  struct FieldActor *leader;
  s32 warning;

  if (GameFlag_IsSet(FLAG_GATE_PARTY_CAUGHT) != 0) {
    return;
  }
  GameFlag_Set(FLAG_GATE_PARTY_CAUGHT);
  Engine_EventBegin();
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
  Engine_ActorStartRepeatedMotion(ACTOR_RIGHT_GUARD, 1);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 2, 60);
  warning = MsgLunpaGuardsCatchParty;
  Engine_EventSetMessage(warning + CATCH_LEFT_GUARD_CHALLENGES);
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
  Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x20000, 0x10000);
  Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x20000, 0x10000);
  Engine_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_SHAKE_HEAD);
  Engine_EventWait(35);
  Engine_EventSetMessage(warning + CATCH_RIGHT_GUARD_WONDERS);
  Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
  Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 3, 30);
  Engine_EventSetMessage(warning + CATCH_LEFT_GUARD_REFUSES_ENTRY);
  Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
  Engine_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_NOD);
  Engine_EventWait(25);
  Engine_EventSetMessage(warning + CATCH_RIGHT_GUARD_SENDS_PARTY_OFF);
  Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
  Actor_WalkTo(ACTOR_LEFT_GUARD, leader->x.part.pixel - 1,
               leader->z.part.pixel);
  Engine_ActorWaitForMove(ACTOR_LEFT_GUARD);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 216);
  Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 200);
  Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 200);
  Engine_ActorWaitForMove(ACTOR_LEFT_GUARD);
  Engine_ActorWaitForMove(ACTOR_RIGHT_GUARD);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(12);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 272);
  Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 256);
  Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 256);
  Engine_ActorWaitForMove(ACTOR_LEFT_GUARD);
  Engine_ActorWaitForMove(ACTOR_RIGHT_GUARD);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_EventEnd();
  FieldScheduler_AddCallback(Guards_Watch, TASK_PRIORITY_SCENE);
}

// The gateway opens again.
static void Gateway_Reopen(void) {
  Map_CopyCellAttributes(7, 12, 1, 1, 7, 11);
  Map_CopyCellAttributes(7, 12, 1, 1, 8, 11);
  Map_CopyCellAttributes(7, 12, 1, 1, 9, 11);
}

// Cloak fades: if the leader is still among the guards, they are caught.
static void Cloak_End(void) {
  struct EventWork *work;
  struct FieldActor *leader;

  work = gEventWork;
  Engine_GameFlagClear(FLAG_GATE_CLOAK_CAST);
  Engine_GameFlagClear(FLAG_GATE_GUARDS_BLOCKING);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader->x.fixed > PIXELS(104) && leader->x.fixed < PIXELS(240) &&
      leader->z.fixed > PIXELS(160) && leader->z.fixed < PIXELS(248)) {
    Engine_TaskRemoveCallback(Guards_Watch);
#if TBS_PATCH_1
    work->raisedTrigger = TRIGGER_PARTY_SPOTTED;
#else
    Guards_CatchParty();
#endif
  }
  Gateway_Reopen();
  Engine_GameFlagClear(FLAG_GATE_PARTY_CAUGHT);
}

// Every frame at the gate: an uncloaked leader in front of the guards is
// spotted.
static void Guards_Watch(void) {
  struct EventWork *work;
  struct FieldActor *leader;

  work = gEventWork;
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (gGameState.cloaked == 0 &&
      (u32)(leader->x.fixed - PIXELS(144)) <= PIXELS(32) &&
      leader->z.fixed >= PIXELS(168) && leader->z.fixed < PIXELS(176)) {
    Engine_TaskRemoveCallback(Guards_Watch);
    work->raisedTrigger = TRIGGER_PARTY_SPOTTED;
  }
}

// Every 16 frames while the leader is thrown: a puff of dust.
static void Leader_KickUpDust(void) {
  struct FieldActor *leader;
  s32 angle;
  s32 velocity[3];

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if ((gFrameCount & 15) == 0) {
    angle = (((u32)Engine_RandomNext() * 52) >> 16) * 64 + 230;
    velocity[0] = Engine_MathCos(angle) / 4;
    velocity[1] = 0;
    velocity[2] = Engine_MathSin(angle) / 2;
    Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, velocity[0],
                 velocity[1], velocity[2], 0, NULL);
  }
}

// Entering the gate after being caught inside: the guards throw the
// leader out and down the steps.
static void Party_ThrownOut(void) {
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
  FieldScheduler_AddCallback(Leader_KickUpDust, TASK_PRIORITY_SCENE);
  Engine_EventWait(1);
  Event_PlayCue(SOUND_SCUFFLE);
  Engine_EventWait(20);
  Engine_ActorSetSpritePriority(ACTOR_LEFT_GUARD, 3);
  Engine_ActorSetSpritePriority(ACTOR_RIGHT_GUARD, 3);
  Event_PlayCue(SOUND_SCUFFLE);
  Engine_EventWait(30);
  Actor_Get(ACTOR_LEFT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Actor_Get(ACTOR_RIGHT_GUARD)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Event_PlayCue(SOUND_SCUFFLE);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
  Engine_TaskRemoveCallback(Leader_KickUpDust);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags |= ACTOR_FALLS;
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = PIXELS(6);
  Actor_Get(ACTOR_PARTY_LEADER)->velocityZ = PIXELS(6);
  Engine_EventWait(1);
  while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
    Engine_EventWait(1);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SPRAWLED);
  Event_PlayCue(SOUND_LANDING_THUD);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  FieldScheduler_AddCallback(Leader_KickUpDust, TASK_PRIORITY_SCENE);
  Engine_EventWait(2);
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = PIXELS(3);
  Engine_EventWait(1);
  while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
    Engine_EventWait(1);
  }
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_TaskRemoveCallback(Leader_KickUpDust);
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

// Entering the gate after sneaking out of the fortress: the leader casts
// Cloak and slips around the guards.
static void Leader_SneaksOut(void) {
  Engine_EventBegin();
  Engine_EventOpenScreen();
  Actor_WalkTo(ACTOR_PARTY_LEADER, 152, 168);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_EventWait(20);
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

// Sets either map up: Reveal's secrets hidden and the floating nut in the
// town; at the gate, the thrown-out or sneaking-out scene, the watchers
// and the fortress visit flags cleared.
s32 Scene_Initialize(void) {
  struct FieldActor *puddle;

  if (gGameState.scene == (s32)&SceneId_Lunpa1) {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Reveal_HideSecrets();
    if (Engine_GameFlagIsSet(FLAG_LUNPA_NUT_CAUGHT) == 0) {
      InitializeStagedActorSceneOrbitingEffect(ACTOR_FLOATING_NUT);
    }
    puddle = Object_GetById(ACTOR_HIDDEN_PUDDLE);
    if (puddle != NULL) {
      Engine_ActorSetSpriteFlags(puddle, 0);
    }
    Engine_GameFlagSet(FLAG_LUNPA_SECRETS_HIDDEN);
  }
  if (gGameState.scene == (s32)&SceneId_Lunpa2) {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gGameState.retreatEntrance = GATE_RETREAT_ENTRANCE;
    if (gGameState.entrance == GATE_ENTRANCE_THROWN_OUT &&
        Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Party_ThrownOut();
    }
    if (gGameState.entrance == GATE_ENTRANCE_SNEAKING_OUT &&
        Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Leader_SneaksOut();
    }
    if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0 &&
        Engine_GameFlagIsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
      FieldScheduler_AddCallback(Party_WatchForFortress, TASK_PRIORITY_SCENE);
    }
#if TBS_PATCH_4
    if (gGameState.cloaked != 0)
      Cloak_Begin();
#endif
    FieldScheduler_AddCallback(Guards_Watch, TASK_PRIORITY_SCENE);
    Engine_GameFlagClear(FLAG_FORTRESS_VISIT + 1);
    Engine_GameFlagClear(FLAG_FORTRESS_VISIT + 2);
    Engine_GameFlagClear(FLAG_FORTRESS_VISIT + 3);
    Engine_GameFlagClear(FLAG_FORTRESS_VISIT + 4);
    Engine_GameFlagClear(FLAG_FORTRESS_VISIT + 5);
    Engine_GameFlagClear(FLAG_FORTRESS_VISIT);
    Engine_GameFlagClear(FLAG_FORTRESS_VISIT + 6);
    Engine_GameFlagClear(FLAG_FORTRESS_VISIT + 7);
    Engine_GameFlagClear(FLAG_FORTRESS_VISIT + 8);
    Engine_GameFlagClear(FLAG_FORTRESS_VISIT + 9);
  }
  return 0;
}

// Shared scene code, included where the ROM places it.
#include "common/object_orbit.h"

#define LUNPA_ENTRANCE(entrance, x, z, facing)                                 \
  {(entrance), CONDITION_ALWAYS, (x), 0, (z), (facing), 0, -1, -1, -1, -1, 0}
#define GATE_ENTRANCE(entrance, x, z, facing)                                  \
  {(entrance), CONDITION_ALWAYS, (x), 0, (z), (facing), 0, 32, 48, 272, 344, 0}

struct SceneEntrance gLunpaEntrances[] = {
    LUNPA_ENTRANCE(0, 172, 149, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_WORLD_MAP, 224, 472, FACING_NORTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_GATE, 232, 32, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_CAVE, 120, 104, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_FIRST_HOME, 248, 184, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_SECOND_HOME, 344, 232, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_THIRD_HOME, 344, 328, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_ARMS_SHOP, 232, 360, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_ITEM_SHOP, 328, 120, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_INN, 104, 184, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_TEMPLE, 48, 168, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_EIGHTH_ROOM, 216, 344, FACING_SOUTH),
    LUNPA_ENTRANCE(LUNPA_ENTRANCE_FROM_JAIL, 40, 328, FACING_SOUTH),
    {SCENE_TABLE_END},
};

struct SceneEntrance gGateEntrances[] = {
    GATE_ENTRANCE(0, 152, 328, FACING_NORTH),
    GATE_ENTRANCE(GATE_ENTRANCE_FROM_FORTRESS, 160, 104, FACING_SOUTH),
    GATE_ENTRANCE(GATE_ENTRANCE_FROM_LUNPA, 152, 328, FACING_NORTH),
    GATE_ENTRANCE(GATE_ENTRANCE_SNEAKING_OUT, 144, 104, FACING_SOUTH),
    GATE_ENTRANCE(GATE_ENTRANCE_THROWN_OUT, 160, 224, FACING_EAST),
    {SCENE_TABLE_END},
};

#define ROOM_EXIT(exit)                                                        \
  SCENE_EXIT((exit), SCENE_LUNPA_SUHALLA, ROOM_ENTRANCE(exit))

u32 gLunpaExits[] = {
    SCENE_EXITS(SCENE_LUNPA),
    SCENE_EXIT(LUNPA_EXIT_TO_WORLD_MAP, SCENE_WORLD_MAP,
               WORLD_MAP_ENTRANCE_FROM_LUNPA),
    SCENE_EXIT(LUNPA_EXIT_TO_GATE, SCENE_LUNPA_FORTRESS_GATE,
               GATE_ENTRANCE_FROM_LUNPA),
    SCENE_EXIT(LUNPA_EXIT_TO_CAVE, SCENE_WEST_LUNPA_CAVE,
               CAVE_ENTRANCE_FROM_LUNPA),
    ROOM_EXIT(LUNPA_EXIT_TO_FIRST_HOME),
    ROOM_EXIT(LUNPA_EXIT_TO_SECOND_HOME),
    ROOM_EXIT(LUNPA_EXIT_TO_THIRD_HOME),
    ROOM_EXIT(LUNPA_EXIT_TO_ARMS_SHOP),
    ROOM_EXIT(LUNPA_EXIT_TO_ITEM_SHOP),
    ROOM_EXIT(LUNPA_EXIT_TO_INN),
    ROOM_EXIT(LUNPA_EXIT_TO_TEMPLE),
    ROOM_EXIT(LUNPA_EXIT_TO_EIGHTH_ROOM),
    SCENE_EXIT(LUNPA_EXIT_TO_JAIL, SCENE_VAULT_JAIL, JAIL_ENTRANCE_FROM_LUNPA),
    SCENE_EXITS_END,
};

u32 gGateExits[] = {
    SCENE_EXITS(SCENE_LUNPA_FORTRESS_GATE),
    SCENE_EXIT(GATE_EXIT_TO_FORTRESS, SCENE_LUNPA_FORTRESS,
               FORTRESS_ENTRANCE_FROM_GATE),
    SCENE_EXIT(GATE_EXIT_TO_LUNPA, SCENE_LUNPA, LUNPA_ENTRANCE_FROM_GATE),
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
   EVENT_SCRIPT(Door_Enter)}

#define LUNPA_ROADS_AND_DOORS                                                  \
  {EVENT_EXIT, TRIGGER_WORLD_MAP_ROAD, CONDITION_ALWAYS,                       \
   LUNPA_EXIT_TO_WORLD_MAP},                                                   \
      {EVENT_EXIT, TRIGGER_GATE_ROAD, CONDITION_ALWAYS, LUNPA_EXIT_TO_GATE},   \
      {EVENT_EXIT, TRIGGER_CAVE_ROAD, CONDITION_ALWAYS, LUNPA_EXIT_TO_CAVE},   \
      DOOR(TRIGGER_FIRST_HOME_DOOR), DOOR(TRIGGER_SECOND_HOME_DOOR),           \
      DOOR(TRIGGER_THIRD_HOME_DOOR), DOOR(TRIGGER_ARMS_SHOP_DOOR),             \
      DOOR(TRIGGER_ITEM_SHOP_DOOR), DOOR(TRIGGER_INN_DOOR),                    \
      DOOR(TRIGGER_TEMPLE_DOOR), DOOR(TRIGGER_EIGHTH_ROOM_DOOR), {             \
    TOUCH_WALKING_INTO(FACING_NORTH), TRIGGER_HIDDEN_PASSAGE,                  \
        CONDITION_ALWAYS, EVENT_SCRIPT(HiddenPassage_Enter)                    \
  }

#define LUNPA_SECRETS                                                          \
  {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_HIDDEN_PUDDLE,                      \
   FLAG_LUNPA_PUDDLE_FROZEN, EVENT_SCRIPT(HiddenPuddle_Freeze)},               \
      {PSYNERGY_IN_AREA(ABILITY_REVEAL, REVEAL_SHOWN), 0, CONDITION_ALWAYS,    \
       EVENT_SCRIPT(Reveal_ShowSecrets)},                                      \
      {PSYNERGY_IN_AREA(ABILITY_REVEAL, REVEAL_FADED), 0, CONDITION_ALWAYS,    \
       EVENT_SCRIPT(Reveal_HideSecrets)},                                      \
      {SEARCH(SEARCH_UNNAMED), TRIGGER_HIDDEN_SWITCH,                          \
       FLAG_LUNPA_SECRETS_HIDDEN, EVENT_SCRIPT(HiddenSwitch_Press)},           \
      {SEARCH(SEARCH_UNNAMED), TRIGGER_PSYNERGY_STONE,                         \
       FLAG_LUNPA_PSYNERGY_STONE, FIND_PSYNERGY_STONE},                        \
      {SEARCH(SEARCH_GRAVE), TRIGGER_GRAVE_SLEEP_BOMB,                         \
       FLAG_LUNPA_GRAVE_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB)},               \
      {SEARCH(SEARCH_BARREL), TRIGGER_BARREL_NUT, FLAG_LUNPA_BARREL_NUT,       \
       FIND_ITEM(ITEM_NUT)},                                                   \
      {PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_FLOATING_NUT,                   \
       FLAG_LUNPA_NUT_CAUGHT, EVENT_SCRIPT(FloatingNut_Catch)},                \
      {SEARCH(SEARCH_UNNAMED), TRIGGER_WELL, CONDITION_ALWAYS,                 \
       EVENT_SCRIPT(Well_Search)},                                             \
  {                                                                            \
    SCENE_EVENTS_END                                                           \
  }

struct SceneEvent gLunpaSealedEvents[] = {
    LUNPA_ROADS_AND_DOORS,
    TALK(ACTOR_VILLAGER_A, EVENT_SCRIPT(VillagerA_Talk)),
    TALK(ACTOR_VILLAGER_B, MSGNO(MsgLunpaVillageVillagerBSealed)),
    TALK(ACTOR_VILLAGER_C, EVENT_SCRIPT(VillagerC_Talk)),
    TALK(ACTOR_VILLAGER_D, MSGNO(MsgLunpaVillageVillagerDSealed)),
    TALK(ACTOR_VILLAGER_E, MSGNO(MsgLunpaVillageVillagerESealed)),
    TALK(ACTOR_VILLAGER_F, MSGNO(MsgLunpaVillageVillagerFSealed)),
    TALK(ACTOR_VILLAGER_G, EVENT_SCRIPT(VillagerG_Talk)),
    TALK(ACTOR_VILLAGER_H, MSGNO(MsgLunpaVillageVillagerHSealed)),
    TALK(ACTOR_WEST_GUARD, EVENT_SCRIPT(WestGuard_Talk)),
    TALK(ACTOR_EAST_GUARD, EVENT_SCRIPT(EastGuard_Talk)),
    MIND_READ(ACTOR_VILLAGER_A, MSGNO(MsgLunpaVillageVillagerASealedThoughts)),
    MIND_READ(ACTOR_VILLAGER_B, MSGNO(MsgLunpaVillageVillagerBSealedThoughts)),
    MIND_READ(ACTOR_VILLAGER_C, MSGNO(MsgLunpaVillageVillagerCSealedThoughts)),
    MIND_READ(ACTOR_VILLAGER_D, MSGNO(MsgLunpaVillageVillagerDSealedThoughts)),
    MIND_READ(ACTOR_VILLAGER_E, MSGNO(MsgLunpaVillageVillagerESealedThoughts)),
    MIND_READ(ACTOR_VILLAGER_F, MSGNO(MsgLunpaVillageVillagerFSealedThoughts)),
    MIND_READ(ACTOR_VILLAGER_G, MSGNO(MsgLunpaVillageVillagerGSealedThoughts)),
    MIND_READ(ACTOR_VILLAGER_H, MSGNO(MsgLunpaVillageVillagerHSealedThoughts)),
    MIND_READ(ACTOR_WEST_GUARD, MSGNO(MsgLunpaVillageWestGuardSealedThoughts)),
    MIND_READ(ACTOR_EAST_GUARD, EVENT_SCRIPT(EastGuard_MindRead)),
    LUNPA_SECRETS,
};

struct SceneEvent gLunpaReopenedEvents[] = {
    LUNPA_ROADS_AND_DOORS,
    TALK(ACTOR_VILLAGER_A, MSGNO(MsgLunpaVillageVillagerAReopened)),
    TALK(ACTOR_VILLAGER_B, EVENT_SCRIPT(VillagerB_Talk)),
    TALK(ACTOR_VILLAGER_C, MSGNO(MsgLunpaVillageVillagerCReopened)),
    TALK(ACTOR_VILLAGER_D, EVENT_SCRIPT(VillagerD_Talk)),
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
    MIND_READ(ACTOR_EAST_GUARD, EVENT_SCRIPT(EastGuard_MindRead)),
    LUNPA_SECRETS,
};

// clang-format off
struct SceneEvent gGateEvents[] = {
    { EVENT_EXIT, TRIGGER_LUNPA_ROAD, CONDITION_ALWAYS, GATE_EXIT_TO_LUNPA },
    { EVENT_EXIT, TRIGGER_THIRD_ROAD, CONDITION_ALWAYS, GATE_EXIT_THIRD },
    { PSYNERGY_IN_AREA(ABILITY_CLOAK, CLOAK_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Cloak_Begin) },
    { PSYNERGY_IN_AREA(ABILITY_CLOAK, CLOAK_FADED), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Cloak_End) },
    { EVENT_TOUCH, TRIGGER_FORTRESS_DOOR, FLAG_GATE_FORTRESS_ENTERED,
      EVENT_SCRIPT(Fortress_Enter) },
    { EVENT_TOUCH, TRIGGER_GUARDED_GROUND, FLAG_GATE_GUARDS_BLOCKING,
      EVENT_SCRIPT(Guards_BlockGate) },
    { EVENT_TOUCH, TRIGGER_FORTRESS_APPROACH, CONDITION_ALWAYS,
      EVENT_SCRIPT(Garet_TurnsBack) },
    { EVENT_RAISED, TRIGGER_PARTY_SPOTTED, CONDITION_ALWAYS, EVENT_SCRIPT(Guards_CatchParty) },
    TALK(ACTOR_LEFT_GUARD, EVENT_SCRIPT(LeftGuard_Talk)),
    TALK(ACTOR_RIGHT_GUARD, EVENT_SCRIPT(RightGuard_Talk)),
    MIND_READ(ACTOR_LEFT_GUARD, EVENT_SCRIPT(LeftGuard_MindRead)),
    MIND_READ(ACTOR_RIGHT_GUARD, EVENT_SCRIPT(RightGuard_MindRead)),
    { SEARCH(SEARCH_UNNAMED), TRIGGER_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Well_Search) },
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

struct LunpaDoor gLunpaDoors[] = {
    {gLunpaDoorSteps[0], 46, 9},  {gLunpaDoorSteps[1], 52, 12},
    {gLunpaDoorSteps[2], 52, 18}, {gLunpaDoorSteps[3], 45, 20},
    {gLunpaDoorSteps[4], 51, 5},  {gLunpaDoorSteps[5], 37, 9},
    {gLunpaDoorSteps[6], 33, 6},  {gLunpaDoorSteps[7], 44, 17},
};

u32 gLunpaUnusedWord = 0;
