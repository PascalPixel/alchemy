/// @file
/// The Sanctum of Vale and of four towns on the party's way, which holds six
/// places in one scene (MapId_Sanctum): the entrance the party comes in by
/// decides which, and Sanctum_GetObjectEventTemplates and Sanctum_GetEvents
/// choose that place's tables by it. Spoken to from the front, each place's
/// healer opens the Sanctum's services (Engine_SanctumOpen); from any other
/// side the healer talks. Each place's door leads back out to its own town.
///
/// - Vale Sanctum (SANCTUM_FROM_VALE, SANCTUM_FROM_VALE_ERUPTION,
///   SANCTUM_VALE_UNUSED and SANCTUM_FROM_QUEST_RESTART), the Great Healer's,
///   entered from Vale at
///   SANCTUM_FROM_VALE. Before the trip up Mt. Aleph, the Great Healer and two
///   other healers (LOCALID_WEST_HEALER and LOCALID_EAST_HEALER) speak of
///   Psynergy. After the eruption the Great Healer brings Isaac and Garet in
///   from Vale (vale_eruption.c) to SANCTUM_FROM_VALE_ERUPTION, where Dora, the
///   Mayor and the healers hear what happened on the mountain, acted out in
///   sepia (Sanctum_ReenactSolSanctum): the colour buffer turns every
///   background and sprite palette to PALETTE_TRANSFORM_SEPIA for the account
///   and fades back to PALETTE_TRANSFORM_PLAIN over 60 frames after it
///   (https://alchemy.pascalpixel.com/places/sanctum/account-sepia.gif: the
///   account's first ten seconds and its last five, beside the 16 background
///   and 16 sprite palettes). The Great Healer asks Isaac to take on the Wise
///   One's quest (Sanctum_RequestQuestAcceptance). Each time his Psynergy
///   shows, LOCALID_HEALER_AFTERIMAGE, a copy of him in SPRITE_COLOR_RED_TINT,
///   jumps round him by a few pixels a frame (Sanctum_UpdateHealerAfterimage)
///   while effects circle him (Sanctum_SpawnOrbitingEffect)
///   (https://alchemy.pascalpixel.com/places/sanctum/healer-afterimages.gif:
///   the screen, the healer boxed, beside the sprites and the sprites without
///   the afterimage). Accepting plays
///   Sanctum_ReceiveWiseOnesMission and sets FLAG_SANCTUM_QUEST_ACCEPTED, and
///   SANCTUM_TO_VALE_GATE takes the two to MapId_ValeGate (vale_gate.c).
///   Refusing leaves the room to gSanctum_QuestEvents: the Great Healer asks
///   again when spoken to (EventScript_ReconsiderQuest), the door draws a
///   warning (FLAG_MAP_TEMP_00), and leaving anyway ends the game
///   (EventScript_LeaveAfterRefusing): `And so the world began drifting toward
///   its fated destruction...` From there the player can start again from the
///   Great Healer, by SANCTUM_TO_QUEST_RESTART to SANCTUM_FROM_QUEST_RESTART,
///   which skips the account (FLAG_MAP_TEMP_01), or leave by
///   SANCTUM_TO_CLEAR to MapId_Clear. After the party has left Vale
///   (FLAG_VALE_PARTY_LEFT), and again after it has reached Kalay
///   (FLAG_KALAY_REACHED), the three talk of what the eruption and the Wise One
///   changed.
/// - Vault Sanctum (SANCTUM_FROM_VAULT and SANCTUM_FROM_VAULT_INTERIORS). Its
///   healer and the other person there (LOCALID_SANCTUM_MEMBER) miss a gold
///   statue stolen during the eruption, until the party has the thieves jailed
///   and the statue is returned (FLAG_VAULT_THIEVES_JAILED); then the statue
///   (LOCALID_GOLD_STATUE), placed off the map until then, stands behind the
///   healer.
/// - Bilibin Sanctum (SANCTUM_FROM_BILIBIN), whose healer and the other person
///   there (LOCALID_SANCTUM_MEMBER) talk of Kolima's curse, before and after it
///   is lifted (FLAG_KOLIMA_CURSE_LIFTED).
/// - Xian Sanctum (SANCTUM_FROM_XIAN), whose healer came to Xian from very
///   distant lands.
/// - Altin Sanctum (SANCTUM_FROM_ALTIN), whose healer tells of the
///   water-spewing beasts in the mine until the party has saved Altin from them
///   (FLAG_HYDROS_STATUES_DEFEATED).
/// - Vault Sanctum again (SANCTUM_FROM_VAULT_REVISIT), when the party passes
///   through Vault later (vault_revisit.c): the statue has been polished, and
///   the healer worries about earthquakes on Mt. Aleph.
/// - Sanctum, the name the game gives every other entrance, which leaves only
///   SANCTUM_UNUSED; no exit leads there, and it places nobody.
///
/// Open questions: none.

#include "battle_effect_runtime.h"
#include "flags.h"
#include "game_state.h"
#include "unit/map/sanctum.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/sound/sound.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "types.h"
#include "script_layer.h"
#include "animspr.h"
#include "field_effect.h"
#include "map_tables.h"
#include "facing_object.h"
#include "text/msg_ids.h"
#include "io_reg.h"
#include "save_state.h"
#include "field_service.h"
#include "window.h"
#include "uiflow.h"
#include "resource.h"
#include "mem.h"
#include "object_dispatch.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "frame.h"
#include "fixed_math.h"
#include "songs.h"
#include "audio_engine_symbols.h"

// Scene temporary flags, in Vale Sanctum: 00 is set once the Great Healer has
// warned Isaac that leaving the village cannot be undone; 01 marks the visit
// after a refusal (SANCTUM_FROM_QUEST_RESTART), which skips the account of Sol
// Sanctum.

/// The placed actors, numbering the rows of the table
/// Sanctum_GetObjectEventTemplates returns for the entrance; the party's rows
/// take their own ids. Every place's first row is its healer.
enum SanctumLocalId {
  /// The place's healer: the Great Healer in Vale Sanctum, its own healer
  /// elsewhere, a man with a white beard in a tall hat and a long purple and
  /// blue robe.
  /// ★ Named from the game: "What is it, Great Healer?"
  /// (MsgSanctumWeHadIdeaTrueSanctum)
  LOCALID_HEALER = LOCALID_FIRST_PLACED,
  // Vale Sanctum (gSanctum_ValeObjectEventTemplates, and
  // gSanctum_QuestObjectEventTemplates at the
  // quest).
  /// The western of Vale Sanctum's two other healers, men with long brown hair
  /// and beards in long robes, as Mind Read hears a villager in
  /// vale_interiors_shops.c call the Sanctum's people: "The healers
  /// are supposed to guard Sol Sanctum." Before the quest: "Our Psynergy was
  /// given to us by the sacred power of Mt. Aleph."
  /// ★ Named from the game: "The healers are supposed to guard"
  /// (MsgValeTheHealersAreSupposedToGuard)
  LOCALID_WEST_HEALER,
  /// The eastern one. "Psynergy is a power that should only be used for
  /// good... Never for evil!"
  /// ★ Named from the game: "should only be used for"
  /// (MsgSanctumPsynergyIsAPowerThatShould)
  LOCALID_EAST_HEALER,
  // The quest (gSanctum_QuestObjectEventTemplates).
  /// Isaac's mother, a woman with fair hair tied up.
  /// ★ Named from the game: "Please, call me Dora."
  /// (MsgValeAsStubbornAsYourFather)
  LOCALID_DORA,
  /// The Mayor, Garet's grandfather, an old man with white hair and a cane, as
  /// the Great Healer calls him: "That is all, Mayor."
  /// ★ Named from the game: "That is all, Mayor"
  /// (MsgSanctumAcceptIsaacCantMean)
  LOCALID_GARETS_GRANDFATHER,
  /// Appears to the Great Healer.
  /// ★ Named from the game: "It must be the Wise One, guardian of the Elemental
  /// Stars." (MsgSanctumWeHadIdeaTrueSanctum)
  LOCALID_WISE_ONE,
  /// A copy of the Great Healer for its afterimages.
  /// ★ Named from what the player sees: a red-tinted copy of the Great Healer
  /// beside him while his Psynergy shows, healer-afterimages.gif (its render)
  LOCALID_HEALER_AFTERIMAGE,
  // Vault Sanctum and Bilibin Sanctum
  // (gSanctum_VaultBilibinObjectEventTemplates).
  /// The other person of the Sanctum, in the western healer's sprite (a man
  /// with long brown hair and a beard in a long robe), who wanders, and who
  /// speaks of the healer as someone else and of "our teachings" and "our
  /// meditations". In Vault: "I wonder if we'll ever see our stolen gold statue
  /// again..."
  /// ★ Named from the game: "stolen gold statue" (MsgSanctumWonderIfEverSeeOur)
  LOCALID_SANCTUM_MEMBER = LOCALID_FIRST_PLACED + 1,
  /// Vault Sanctum's gold statue (the sprite of the one a chest gives back in
  /// vault_interiors.c), placed off the map until the thieves are jailed
  /// (FLAG_VAULT_THIEVES_JAILED).
  /// ★ Named from the game: "our stolen gold statue"
  /// (MsgSanctumWonderIfEverSeeOur)
  LOCALID_GOLD_STATUE
};

static void Sanctum_RequestQuestAcceptance(void);
static void Sanctum_ReceiveWiseOnesMission(void);
static void Sanctum_ShowHealerAfterimage(void);
static void Sanctum_HideHealerAfterimage(void);
static void Sanctum_UpdateHealerAfterimage(union FieldObject *fieldObject);
static void Sanctum_SpawnOrbitingEffect(s32 ownerId, s32 angleStep);
static s32 Sanctum_ChooseRestartOption(void);
static void Sanctum_ReenactSolSanctum(void);
static void Sanctum_RaiseItemIcon(s32 item, s32 fixedX, s32 fixedY, s32 fixedZ);
static void Sanctum_SpawnObjectEventSpark(s32 id);
static void Sanctum_RestoreQuestFloor(void);

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 44 to 42 bytes in tbs-en.
#ifdef UBFIX
/// Faces the actor whose ID this callback stores in the timer field.
static void
Sanctum_FaceObjectEventNamedByTimer(union FieldObject *fieldObject) {
#else
/// Faces the actor whose ID this callback stores in the timer field.
static s32 Sanctum_FaceObjectEventNamedByTimer(union FieldObject *fieldObject) {
#endif
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *target;

  target = ObjectEvent_GetById((s16)objectEvent->timer);
  objectEvent->facing =
      CalculateFacingAngle(target->z.fixed - objectEvent->z.fixed,
                           target->x.fixed - objectEvent->x.fixed);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Returns the entrances, shared by all six places.
const struct MapWarpDest *Sanctum_GetWarpDests(void) {
  return gSanctum_WarpDests;
}

const struct MapWarpRegion *Sanctum_GetRegions(void) { return NULL; }

const u32 *Sanctum_GetWarps(void) { return gSanctum_Warps; }

/// Chooses the actors by entrance: Vale Sanctum's Great Healer and two other
/// healers (SANCTUM_FROM_VALE and SANCTUM_VALE_UNUSED), the quest's gathering
/// (SANCTUM_FROM_VALE_ERUPTION and SANCTUM_FROM_QUEST_RESTART), the healer and
/// LOCALID_SANCTUM_MEMBER of Vault Sanctum and Bilibin Sanctum
/// (SANCTUM_FROM_VAULT, SANCTUM_FROM_VAULT_REVISIT,
/// SANCTUM_FROM_VAULT_INTERIORS and SANCTUM_FROM_BILIBIN), the healer alone in
/// Altin Sanctum and Xian Sanctum (SANCTUM_FROM_ALTIN and SANCTUM_FROM_XIAN),
/// and nobody otherwise.
const struct ObjectEventTemplate *Sanctum_GetObjectEventTemplates(void) {
  switch (gGameState.warpDest) {
  case SANCTUM_FROM_VALE:
  case SANCTUM_VALE_UNUSED:
    return gSanctum_ValeObjectEventTemplates;

  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_VAULT_REVISIT:
  case SANCTUM_FROM_VAULT_INTERIORS:
  case SANCTUM_FROM_BILIBIN:
    return gSanctum_VaultBilibinObjectEventTemplates;

  case SANCTUM_FROM_VALE_ERUPTION:
  case SANCTUM_FROM_QUEST_RESTART:
    return gSanctum_QuestObjectEventTemplates;

  case SANCTUM_FROM_ALTIN:
  case SANCTUM_FROM_XIAN:
    return gSanctum_AltinXianObjectEventTemplates;

  default:
    return gSanctum_NoObjectEventTemplates;
  }
}

/// Chooses the events by entrance, and in Vale Sanctum before and after the
/// quest by story flag: gSanctum_ValeEvents before the party has left Vale,
/// gSanctum_ValeEventsAfterLeaving once it has (FLAG_VALE_PARTY_LEFT) and
/// gSanctum_ValeEventsAfterKalay once it has reached Kalay
/// (FLAG_KALAY_REACHED); gSanctum_QuestEvents for the quest's visits
/// (SANCTUM_FROM_VALE_ERUPTION, SANCTUM_FROM_QUEST_RESTART and
/// SANCTUM_QUEST_UNUSED).
const struct MapEvent *Sanctum_GetEvents(void) {
  switch (gGameState.warpDest) {
  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_VAULT_INTERIORS:
    return gSanctum_VaultEvents;
  case SANCTUM_FROM_VAULT_REVISIT:
    return gSanctum_VaultRevisitEvents;
  case SANCTUM_FROM_VALE_ERUPTION:
  case SANCTUM_FROM_QUEST_RESTART:
  case SANCTUM_QUEST_UNUSED:
    return gSanctum_QuestEvents;
  case SANCTUM_FROM_XIAN:
    return gSanctum_XianEvents;
  case SANCTUM_FROM_ALTIN:
    return gSanctum_AltinEvents;
  case SANCTUM_FROM_BILIBIN:
    return gSanctum_BilibinEvents;
  default:
    break;
  }

  if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0) {
    return gSanctum_ValeEventsAfterKalay;
  }
  if (Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT) != 0) {
    return gSanctum_ValeEventsAfterLeaving;
  }
  return gSanctum_ValeEvents;
}

/// Vault Sanctum: talking to LOCALID_SANCTUM_MEMBER, about the stolen gold
/// statue until the thieves are jailed (FLAG_VAULT_THIEVES_JAILED) and about
/// fate afterwards; on the later visit (SANCTUM_FROM_VAULT_REVISIT), the statue
/// polished.
static void EventScript_TalkAboutGoldenStatue(void) {
  Engine_ScriptStart();

  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
    Engine_ScriptSetMessage(MsgSanctumWhenStrayFromYourWorldly);
  } else {
    Engine_ScriptSetMessage(MsgSanctumHealerMustWorriedAboutNever);
  }

  if (gGameState.warpDest == SANCTUM_FROM_VAULT_REVISIT) {
    Engine_ScriptSetMessage(MsgSanctumPolishedGoldStatueReturnedUs);
  }

  Engine_ObjectEventSetAnimation(LOCALID_SANCTUM_MEMBER, 1);
  Engine_ObjectEventFaceEachOther(LOCALID_SANCTUM_MEMBER, LOCALID_PARTY_LEADER,
                                  0);
  Engine_ScriptWait(2);
  Engine_ScriptShowMessage(LOCALID_SANCTUM_MEMBER, 0);
  Engine_ScriptEnd();
}

/// Vault Sanctum: Mind Read on LOCALID_SANCTUM_MEMBER, about the stolen statue,
/// or once it is back, about Isaac awakening to the Sanctum's teachings; on the
/// later visit, the statue cleaned up.
static void EventScript_ReadMindAboutGoldenStatue(void) {

  Engine_ScriptStart();

  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_ScriptSetMessage(MsgSanctumWonderIfEverSeeOur);
  } else {
    Engine_ScriptSetMessage(MsgSanctumChildHasAwakenedOurTeachings);
  }

  if (gGameState.warpDest == SANCTUM_FROM_VAULT_REVISIT) {
    Engine_ScriptSetMessage(MsgSanctumDirtyGoldenStatueCleanedUp);
  }

  Engine_ObjectEventStop(LOCALID_SANCTUM_MEMBER);
  Engine_ObjectEventSetAnimation(LOCALID_SANCTUM_MEMBER, 1);
  Engine_ScriptWait(2);
  Engine_ScriptShowMessage(LOCALID_SANCTUM_MEMBER, 0);
  ObjectEvent_SetBuiltinAction(LOCALID_SANCTUM_MEMBER, 2);
  Engine_ScriptEnd();
}

/// Vale Sanctum: talking to the Great Healer after Isaac has refused the quest.
/// "Are you sure?" Changing his mind, Isaac can accept after all, and the
/// quest goes on as if he had accepted at once.
static void EventScript_ReconsiderQuest(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgSanctumAreYouSure);
  Engine_ScriptOpenMessage(LOCALID_HEALER, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
    Engine_ScriptWait(20);
  } else {
    Engine_ScriptWait(20);
    Engine_ScriptOpenMessage(LOCALID_HEALER, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == 0) {
      Engine_ScriptWait(20);
      Engine_ScriptOpenMessage(LOCALID_HEALER, 0);
      if (Engine_ScriptChooseYesNo(0, 0) == 0) {
        Engine_ScriptWait(20);
        if (ObjectEvent_GetById(LOCALID_HEALER)->facing >= FACING_NORTHWEST &&
            ObjectEvent_GetById(LOCALID_HEALER)->facing <= FACING_NORTHEAST) {
          ObjectEvent_SetSpeed(LOCALID_HEALER, PIXELS(1) / 2, PIXELS(1) / 4);
          Engine_ObjectEventFaceDirection(LOCALID_HEALER, FACING_EAST, 0);
          Engine_ScriptWait(10);
          ObjectEvent_GetById(LOCALID_HEALER)->steeringFlags &=
              ~OBJ_EVENT_FACES_MOTION;
          Engine_ObjectEventWalkToAndWait(LOCALID_HEALER, 152, 120);
          Engine_ScriptWait(1);
          ObjectEvent_GetById(LOCALID_HEALER)->steeringFlags |=
              OBJ_EVENT_FACES_MOTION;
          Engine_ScriptWait(20);
          Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
          Engine_ScriptWait(20);
          Engine_ObjectEventWalkToAndWait(LOCALID_PARTY_LEADER, 168, 120);
          Engine_ObjectEventWalkTo(LOCALID_PARTY_LEADER, 192, 168);
          Engine_ScriptWait(20);
          Engine_ObjectEventWalkToAndWait(LOCALID_HEALER, 168, 120);
          Engine_ObjectEventFaceDirection(LOCALID_HEALER,
                                          FACING_SOUTHEAST + FACING_STEP, 0);
          Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
        } else {
          Engine_ObjectEventWalkTo(LOCALID_PARTY_LEADER, 192, 168);
          Engine_ScriptWait(20);
          Engine_ObjectEventFaceDirection(LOCALID_HEALER,
                                          FACING_SOUTHEAST + FACING_STEP, 0);
          Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
        }
        Sanctum_ReceiveWiseOnesMission();
        Engine_ColorBufferApplyTarget(RGB(0, 0, 0), PALETTE_HALVES_BOTH);
        Engine_ColorBufferInterpolate(120);
        Engine_ScriptWait(120);
        Engine_AudioPlayCue(MUS_086);
        AudioCommand_WaitForCompletion();
        Engine_GameFlagSet(FLAG_SANCTUM_QUEST_ACCEPTED);
        Engine_ScriptRequestWarp(SANCTUM_TO_VALE_GATE);
      }
    }
  }
  Engine_ScriptEnd();
}

#if TBS_PATCH_1
#define SANCTUM_NEW_FRIENDS_SPEAKER 2
#else
#define SANCTUM_NEW_FRIENDS_SPEAKER 1
#endif

/// Vale Sanctum, once the party has reached Kalay: talking to
/// LOCALID_WEST_HEALER, who asks about the party's new friends. "Isaac, are
/// your new friends Adepts by any chance?" Denied, a friend's silence says they
/// must be: in every edition but the Japanese, that friend is Ivan, placed for
/// it; in the Japanese, Garet.
static void EventScript_AskAboutNewAdepts(void) {
  Engine_ScriptStart();
#if TBS_PATCH_1
  FieldScript_CallWithLastActiveObjectId(gSanctum_IvanObjectEventTemplate);
  Engine_TaskWait(1);
#endif
  Engine_ScriptSetMessage(MsgSanctumIsaacYourNewFriendsAdepts);
  Engine_ScriptOpenMessage(LOCALID_WEST_HEALER, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ScriptShowMessage(LOCALID_WEST_HEALER, 0);
  } else {
    gEventWork->message++;
    Script_ShowTwoMessagesAndWait(SANCTUM_NEW_FRIENDS_SPEAKER, 16, 1, 24, 1, 3,
                                  7, 16, 1, 14, 0);
    Engine_ScriptShowMessage(LOCALID_WEST_HEALER, 0);
  }
  Engine_ScriptEnd();
}

/// Vale Sanctum, once the party has left Vale: talking to LOCALID_EAST_HEALER,
/// who asks whether Isaac feels any change in his Psynergy.
static void EventScript_AskAboutPsynergyChanges(void) {

  Engine_ScriptStart();
  Engine_ObjectEventFaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_PARTY_LEADER,
                                    0);
  Engine_ScriptWait(10);
  Engine_ScriptSetMessage(MsgSanctumDoFeelAnyChangeIn);
  Engine_ScriptOpenMessage(LOCALID_EAST_HEALER, 0);

  if (Engine_ScriptChooseYesNo(0, 0) == 1) {
    (gEventWork->message)++;
  }

  Engine_ScriptShowMessage(LOCALID_EAST_HEALER, 0);
  Engine_ScriptEnd();
}

/// Sets up the place for its entrance and plays its opening scene. Vault
/// Sanctum places LOCALID_GOLD_STATUE once the thieves are jailed; Vale
/// Sanctum's quest visits (SANCTUM_FROM_VALE_ERUPTION, and
/// SANCTUM_FROM_QUEST_RESTART after a refusal, which sets FLAG_MAP_TEMP_01)
/// restore the floor and play the request, unless the party is returning after
/// a battle.
s32 Sanctum_Initialize(void) {
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, PALETTE_HALVES_BOTH);
  Engine_ColorBufferInterpolate(1);
  Engine_ScriptWait(1);
  switch (gGameState.warpDest) {
  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_VAULT_REVISIT:
  case SANCTUM_FROM_VAULT_INTERIORS:
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
      ObjectEvent_SetPosition(LOCALID_GOLD_STATUE, PIXELS(200), PIXELS(80));
    }
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    break;
  case SANCTUM_FROM_VALE_ERUPTION:
    Sanctum_RestoreQuestFloor();
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Sanctum_RequestQuestAcceptance();
    }
    // Fall through.
  case SANCTUM_FROM_ALTIN:
  case SANCTUM_FROM_XIAN:
  case SANCTUM_FROM_BILIBIN:
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    break;
  case SANCTUM_FROM_QUEST_RESTART:
    Sanctum_RestoreQuestFloor();
    Engine_GameFlagSet(FLAG_MAP_TEMP_01);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Sanctum_RequestQuestAcceptance();
    }
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    break;
  }
  return 0;
}

/// Vale Sanctum: heading for the door after refusing the quest (cell code 2,
/// until FLAG_MAP_TEMP_00 is set, which it sets). The Great Healer warns
/// that the choice is final: "Once you step outside the village, you cannot
/// take back your decision."
static void EventScript_WarnBeforeLeaving(void) {
  Engine_ScriptStart();
  Engine_CameraSetSpeed(PIXELS(1), PIXELS(1) / 8);
  Engine_CameraMoveToObjectEvent(GARET, 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(20);
  Engine_ObjectEventFaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 4);
  Engine_ScriptWait(20);
  Engine_ScriptSetMessage(MsgSanctumOnceStepOutsideVillageCannot);
  Engine_ScriptShowMessage(LOCALID_HEALER, 0);
  Engine_GameFlagSet(FLAG_MAP_TEMP_00);
  Engine_ScriptEnd();
}

/// Vale Sanctum: leaving after refusing the quest (cell code 1) ends the game.
/// The room turns sepia: `And so the world began drifting toward its fated
/// destruction...` Starting again from the Great Healer takes
/// SANCTUM_TO_QUEST_RESTART, back to SANCTUM_FROM_QUEST_RESTART; the other
/// choices take SANCTUM_TO_CLEAR, to MapId_Clear.
static void EventScript_LeaveAfterRefusing(void) {
  struct ColorBuffer *paletteWork;

  Engine_ScriptStart();
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 0);
  Engine_ObjectEventSetAnimation(GARET, 0);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, 0);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ObjectEventSetAnimation(LOCALID_HEALER, 0);
  Engine_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 0);
  Engine_ObjectEventSetAnimation(LOCALID_EAST_HEALER, 0);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_SEPIA, PALETTE_HALVES_BOTH);
  Engine_ColorBufferInterpolate(120);
  Engine_ScriptWait(180);
  paletteWork = gPaletteBufferWork;
  // Colour 15 of the BG palette fades to white rather than sepia: its blue,
  // green and red channels, target[3 * 15] to target[3 * 15 + 2], at 31.
  paletteWork->target[45] = 31 << 10;
  paletteWork->target[46] = 31 << 10;
  paletteWork->target[47] = 31 << 10;
  paletteWork->page = 0;
  paletteWork->duration = 1;
  paletteWork->step = 1;
  // Nothing reads this byte, so the store changes nothing.
  paletteWork->padding2a03 = 1;
  Engine_ScriptWait(1);
  UiText_ShowCenteredMessage(MsgSanctumWorldBeganDrifting, 1, 0);
  Engine_ColorBufferApplyTarget(RGB(0, 0, 0), PALETTE_HALVES_BOTH);
  Engine_ColorBufferInterpolate(120);
  Engine_ScriptWait(120);
  Engine_ScriptWait(60);
  if (Sanctum_ChooseRestartOption() == 0) {
    Engine_ScriptEnd();
    Engine_ScriptRequestWarp(SANCTUM_TO_QUEST_RESTART);
  } else {
    Engine_ScriptEnd();
    Engine_ScriptRequestWarp(SANCTUM_TO_CLEAR);
  }
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 66 to 64 bytes in tbs-en.
#ifdef UBFIX
/// Keeps this actor at the party leader's drawing priority.
static void Sanctum_MatchLeaderPriority(union FieldObject *fieldObject) {
#else
/// Keeps this actor at the party leader's drawing priority.
static s32 Sanctum_MatchLeaderPriority(union FieldObject *fieldObject) {
#endif
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  objectEvent->sprite->image.priority =
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->sprite->image.priority;
  objectEvent->sprite->shadow.priority =
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->sprite->image.priority;
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Has Isaac, Garet, Dora, the Mayor and the two healers nod, waiting for the
/// last of them.
static inline void Sanctum_NodAll(void) {
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, 3);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 3);
  Engine_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 3);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_EAST_HEALER, 3);
}

/// Starts the same six's repeated motions and waits for the last of them.
static inline void Sanctum_StartAllRepeatedMotion(void) {
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  Engine_ObjectEventStartRepeatedMotion(GARET, 1);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_DORA, 1);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_GARETS_GRANDFATHER, 1);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_WEST_HEALER, 1);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_EAST_HEALER, 1);
}

/// Shows the same emote on the same six.
static inline void Sanctum_ShowAllEmotes(void) {
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
}

/// Vale Sanctum, SANCTUM_FROM_VALE_ERUPTION and SANCTUM_FROM_QUEST_RESTART: the
/// gathering hears what happened in Sol Sanctum (skipped after a refusal), the
/// Great Healer explains the Wise One's warning, and asks Isaac to accept
/// responsibility for the fate of the land. Accepting plays
/// Sanctum_ReceiveWiseOnesMission, sets FLAG_SANCTUM_QUEST_ACCEPTED and takes
/// SANCTUM_TO_VALE_GATE; refusing leaves the room to gSanctum_QuestEvents.
static void Sanctum_RequestQuestAcceptance(void) {
  u32 effectIndex;

  Engine_ScriptStart();
  // A repeat visit skips the account of Sol Sanctum.
  if (GameFlag_IsSet(FLAG_MAP_TEMP_01) != 0) {
    Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(GARET, LOCALID_PARTY_LEADER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_PARTY_LEADER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER,
                                LOCALID_PARTY_LEADER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_PARTY_LEADER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_PARTY_LEADER, 0);
    Camera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
    Engine_CameraWaitForMove();
    gEventWork->startTransition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transitionFrames = 64;
    Engine_ScriptOpenScreen();
    Engine_ScriptWaitForScreen();
    Engine_ScriptWait(120);
  } else {
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_SEPIA, PALETTE_HALVES_BOTH);
    Engine_ColorBufferInterpolate(1);
    Engine_ScriptWait(1);
    Camera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
    Engine_CameraWaitForMove();
    gEventWork->startTransition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    Engine_ScriptOpenScreen();
    Engine_ScriptWaitForScreen();
    // Reenact the journey before explaining the quest.
    Sanctum_ReenactSolSanctum();
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, PALETTE_HALVES_BOTH);
    Engine_ColorBufferInterpolate(60);
    Engine_ScriptWait(100);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
    Engine_ScriptWait(30);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER,
                                LOCALID_GARETS_GRANDFATHER, 0);
    ObjectEvent_FaceObjectEvent(GARET, LOCALID_GARETS_GRANDFATHER, 0);
    Engine_ScriptWait(20);
    ObjectEvent_SetBuiltinAction(LOCALID_HEALER, OBJ_EVENT_ACTION_IDLE);
    ObjectEvent_SetBuiltinAction(LOCALID_GARETS_GRANDFATHER,
                                 OBJ_EVENT_ACTION_IDLE);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_GARETS_GRANDFATHER, PIXELS(4) / 5,
                         PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_WEST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_EAST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
    Engine_ScriptWait(30);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_GARETS_GRANDFATHER, 2);
    Engine_ScriptSetMessage(MsgSanctumWeHadIdeaTrueSanctum);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    Engine_ScriptWait(10);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
    Engine_ScriptWait(30);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 3);
    Engine_ScriptWait(20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
    Engine_ScriptWait(20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
    ObjectEvent_FaceObjectEvent(GARET, LOCALID_DORA, 0);
    Engine_ScriptWait(20);
    Script_OpenMessage(LOCALID_DORA, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == 0) {
      Engine_ScriptWait(20);
      Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, 3);
      Engine_ScriptWait(20);
      Script_ShowMessage(LOCALID_DORA, 0);
      gEventWork->message++;
    } else {
      Engine_ScriptWait(20);
      Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, 4);
      Engine_ScriptWait(20);
      gEventWork->message++;
      Script_ShowMessage(LOCALID_DORA, 0);
    }
    Engine_ScriptWait(20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_WEST_HEALER, 1);
    Engine_ScriptWait(20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_WEST_HEALER, 0);
    ObjectEvent_FaceObjectEvent(GARET, LOCALID_WEST_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_WEST_HEALER, 0);
    Engine_ScriptWait(20);
    Script_OpenMessage(LOCALID_WEST_HEALER, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == 0) {
      Engine_ScriptWait(20);
      Engine_ObjectEventSetAnimationAndWait(LOCALID_WEST_HEALER, 3);
      Engine_ScriptWait(20);
      Script_ShowMessage(LOCALID_WEST_HEALER, 0);
      gEventWork->message++;
    } else {
      Engine_ScriptWait(20);
      Engine_ObjectEventSetAnimationAndWait(LOCALID_WEST_HEALER, 4);
      Engine_ScriptWait(20);
      gEventWork->message++;
      Script_ShowMessage(LOCALID_WEST_HEALER, 0);
    }
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
    Engine_ObjectEventSetAnimation(LOCALID_DORA, 3);
    Engine_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 3);
    Engine_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 3);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_EAST_HEALER, 3);
    Engine_ScriptWait(30);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Engine_ScriptWait(60);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
    Engine_ScriptWait(20);
    ObjectEvent_WalkToAndWait(LOCALID_GARETS_GRANDFATHER, 0xe0, 0x78);
    Engine_ScriptWait(10);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    Engine_ScriptWait(20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(GARET, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
    Engine_ScriptWait(10);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 1);
    Engine_ScriptWait(10);
    Script_ShowMessage(LOCALID_DORA, 0);
    Engine_ScriptWait(30);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_HEALER, 3);
    ObjectEvent_ShowEmote(LOCALID_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          0);
    Engine_ScriptWait(60);
    ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_GARETS_GRANDFATHER, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
    Engine_ScriptWait(10);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(20);
    ObjectEvent_FaceDirection(LOCALID_GARETS_GRANDFATHER,
                              FACING_SOUTH + FACING_STEP, 0);
    Engine_ScriptWait(60);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 3);
    Engine_ScriptWait(10);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    Engine_ScriptWait(10);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
    Engine_ScriptWait(20);
    ObjectEvent_FaceDirection(LOCALID_HEALER, FACING_SOUTHEAST + FACING_STEP,
                              0);
    Engine_ScriptWait(10);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(20);
    ObjectEvent_FaceDirection(LOCALID_GARETS_GRANDFATHER,
                              FACING_SOUTH + FACING_STEP, 0);
    Engine_ScriptWait(60);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
    Engine_ScriptWait(50);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Engine_ScriptWait(40);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(LOCALID_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    Engine_ScriptWait(60);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_HEALER, 1);
    Engine_ScriptWait(10);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(10);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_WEST_HEALER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_EAST_HEALER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Engine_ScriptWait(60);
    Engine_ObjectEventRunRepeatedMotion(GARET, 2);
    Engine_ScriptWait(10);
    Script_ShowMessage(GARET, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
    Engine_ScriptWait(10);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(10);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
    Engine_ObjectEventStartRepeatedMotion(GARET, 1);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_DORA, 1);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_WEST_HEALER, 1);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_EAST_HEALER, 1);
    Engine_ScriptWait(10);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 4);
    Engine_ScriptWait(10);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(30);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER,
                                LOCALID_PARTY_LEADER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_PARTY_LEADER, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
    Engine_ScriptWait(60);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_GARETS_GRANDFATHER, 2);
    Engine_ScriptWait(20);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    Engine_ScriptWait(30);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
    Engine_ScriptWait(20);
    ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_DORA, 0);
    Engine_ScriptWait(20);
    Engine_ScriptWait(40);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_HEALER, 2);
    Engine_ObjectEventSetAnimation(LOCALID_HEALER, 0);
    ObjectEvent_SetChildValue(LOCALID_HEALER, 0x100);
    Sanctum_ShowHealerAfterimage();
    Script_PlayCue(SE_196);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    Engine_ScriptWait(32);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    Engine_ScriptWait(16);
    for (effectIndex = 0; effectIndex < 6; effectIndex++) {
      Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
      Engine_ScriptWait(8);
    }
    Engine_ScriptWait(8);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    Engine_ScriptWait(32);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    Engine_ScriptWait(96);
    Engine_ScriptWait(32);
    ObjectEvent_SetChildValue(LOCALID_HEALER, 0);
    Engine_ScriptWait(30);
    // Return to the gathering and finish the departure conversation.
    Sanctum_HideHealerAfterimage();
    Engine_ObjectEventSetAnimation(LOCALID_HEALER, 1);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_HEALER, 2);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(20);
    ObjectEvent_SetAttachedEffect(LOCALID_GARETS_GRANDFATHER,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(
        LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                             ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(LOCALID_DORA, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(
        LOCALID_WEST_HEALER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(
        LOCALID_EAST_HEALER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ScriptWait(60);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_EAST_HEALER, 1);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_EAST_HEALER, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, GARET, 0);
    Engine_ScriptWait(60);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 4);
    Engine_ScriptWait(20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(GARET, LOCALID_HEALER, 0);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(30);
    Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, GARET, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
    Engine_ObjectEventFaceEachOther(LOCALID_WEST_HEALER, LOCALID_EAST_HEALER,
                                    0);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 3);
    Engine_ObjectEventSetAnimation(LOCALID_EAST_HEALER, 3);
    ObjectMotion_WaitForAnimationChange(LOCALID_EAST_HEALER);
    Engine_ScriptWait(30);
    Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
    Engine_ObjectEventFaceEachOther(GARET, LOCALID_GARETS_GRANDFATHER, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    Engine_ObjectEventSetAnimation(LOCALID_DORA, 3);
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
    Engine_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 3);
    ObjectMotion_WaitForAnimationChange(LOCALID_GARETS_GRANDFATHER);
    Engine_ScriptWait(60);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_HEALER, 2);
    Engine_ObjectEventSetAnimation(LOCALID_HEALER, 0);
    ObjectEvent_SetChildValue(LOCALID_HEALER, 0x100);
    Sanctum_ShowHealerAfterimage();
    Script_PlayCue(SE_196);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    Engine_ScriptWait(32);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(GARET, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
    Engine_ScriptWait(16);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
    Engine_ObjectEventStartRepeatedMotion(GARET, 1);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_DORA, 1);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_GARETS_GRANDFATHER, 1);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_WEST_HEALER, 1);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_EAST_HEALER, 1);
    for (effectIndex = 0; effectIndex < 6; effectIndex++) {
      Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
      Engine_ScriptWait(8);
    }
    Engine_ScriptWait(8);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    Engine_ScriptWait(32);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    Engine_ScriptWait(128);
    ObjectEvent_SetChildValue(LOCALID_HEALER, 0);
    Engine_ScriptWait(30);
    Sanctum_HideHealerAfterimage();
    Engine_ObjectEventSetAnimation(LOCALID_HEALER, 1);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_HEALER, 2);
    Engine_ScriptWait(30);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventFaceEachOther(LOCALID_WEST_HEALER, LOCALID_EAST_HEALER,
                                    0);
    Engine_ScriptWait(40);
    ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_DORA, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
    Engine_ObjectEventStartRepeatedMotion(GARET, 1);
    Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, GARET, 0);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_WEST_HEALER, 1);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_EAST_HEALER, 1);
    Engine_ObjectEventFaceEachOther(LOCALID_WEST_HEALER, LOCALID_EAST_HEALER,
                                    0);
    ObjectEvent_SetAttachedEffect(LOCALID_GARETS_GRANDFATHER,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ONCE);
    Engine_ScriptWait(60);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(GARET, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
    Engine_ScriptWait(20);
    Script_ShowMessage(GARET, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(20);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(LOCALID_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    Engine_ScriptWait(60);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 4);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
    Engine_ObjectEventSetAnimation(LOCALID_DORA, 3);
    Engine_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 3);
    Engine_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 3);
    Engine_ObjectEventSetAnimation(LOCALID_EAST_HEALER, 3);
    ObjectMotion_WaitForAnimationChange(LOCALID_EAST_HEALER);
    Engine_ScriptWait(20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_GARETS_GRANDFATHER, 1);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    Engine_ScriptWait(20);
    ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_GARETS_GRANDFATHER, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(50);
    ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(30);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          0);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    ObjectEvent_ShowEmote(LOCALID_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Engine_ScriptWait(80);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 1);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_DORA, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 4);
    Engine_ScriptWait(20);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(LOCALID_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    Engine_ScriptWait(80);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_HEALER, 1);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(20);
    ObjectEvent_SetAttachedEffect(
        LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                             ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(LOCALID_DORA, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(LOCALID_GARETS_GRANDFATHER,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(
        LOCALID_WEST_HEALER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(
        LOCALID_EAST_HEALER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ScriptWait(60);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 4);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_HEALER, 1);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 4);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(40);
    Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, GARET, 0);
    Engine_ObjectEventFaceEachOther(LOCALID_WEST_HEALER, LOCALID_EAST_HEALER,
                                    0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER,
                                LOCALID_PARTY_LEADER, 0);
    Engine_ScriptWait(60);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(GARET, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 1);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_DORA, 0);
    Engine_ScriptWait(60);
    ObjectEvent_WalkToAndWait(LOCALID_HEALER, 0xc8, 0x88);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER,
                                LOCALID_PARTY_LEADER, 0);
    Engine_ObjectEventFaceEachOther(LOCALID_HEALER, GARET, 0);
    Engine_ScriptWait(40);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
    Engine_ScriptWait(50);
    ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
    Engine_ScriptWait(40);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
    Engine_ScriptWait(50);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_HEALER, 1);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(30);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          0);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Engine_ScriptWait(60);
    ObjectEvent_SetAttachedEffect(LOCALID_DORA, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 1);
    Script_ShowMessage(LOCALID_DORA, 0);
    Engine_ScriptWait(30);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
    Engine_ScriptWait(30);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_GARETS_GRANDFATHER, 1);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    Engine_ScriptWait(30);
    ObjectEvent_WalkToAndWait(LOCALID_HEALER, 0xa8, 0x78);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(GARET, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
    ObjectEvent_FaceDirection(LOCALID_HEALER, FACING_NORTH + FACING_STEP, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 4);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(30);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          0);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Engine_ScriptWait(60);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
    Engine_ScriptWait(10);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(10);
    ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptWait(20);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER,
                                LOCALID_PARTY_LEADER, 0);
    ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                             ATTACHED_EFFECT_ONCE);
    Engine_ObjectEventRunRepeatedMotion(GARET, 2);
    ObjectEvent_FaceObjectEvent(GARET, LOCALID_PARTY_LEADER, 0);
    Engine_ScriptWait(20);
    Script_ShowMessage(GARET, 0);
    Engine_ScriptWait(30);
  }
  // Ask Isaac to commit, or leave the group waiting for his answer.
  Engine_ScriptSetMessage(MsgSanctumIsaacWillAcceptResponsibilityFor);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_HEALER, 1);
  Engine_ScriptWait(20);
  Script_OpenMessage(LOCALID_HEALER, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Sanctum_ReceiveWiseOnesMission();
    ColorBuffer_ApplyTarget(RGB(0, 0, 0), PALETTE_HALVES_BOTH);
    Engine_ColorBufferInterpolate(120);
    Engine_ScriptWait(120);
    Script_PlayCue(MUS_086);
    AudioCommand_WaitForCompletion();
    GameFlag_Set(FLAG_SANCTUM_QUEST_ACCEPTED);
    Engine_ScriptRequestWarp(SANCTUM_TO_VALE_GATE);
  } else {
    Engine_ScriptWait(30);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_GARETS_GRANDFATHER, 1);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 4);
    Engine_ScriptWait(20);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    Engine_ScriptWait(20);
    ObjectEvent_GetById(LOCALID_EAST_HEALER)->priorityFlags &=
        ~OBJ_EVENT_PRIORITY_AUTOMATIC;
#ifdef UBFIX
    ObjectEvent_GetById(LOCALID_EAST_HEALER)->update =
        Sanctum_MatchLeaderPriority;
#else
    ObjectEvent_GetById(LOCALID_EAST_HEALER)->update =
        (void (*)(union FieldObject *))Sanctum_MatchLeaderPriority;
#endif
  }
  Engine_ScriptEnd();
}

/// Vale Sanctum, once Isaac accepts: Garet and the Mayor react, the Wise One
/// appears before the Great Healer and accepts the two, and the healer relays
/// its guidance; they are to leave the next day.
static void Sanctum_ReceiveWiseOnesMission(void) {

  u32 sparkIndex;
  struct ObjectEvent *wiseOne;

  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARETS_GRANDFATHER, PIXELS(4) / 5,
                       PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_WEST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_EAST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
  Camera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
  Engine_CameraWaitForMove();
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_PARTY_LEADER,
                              0);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_PARTY_LEADER, 0);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, 3);
  Engine_ScriptWait(60);
  Engine_ObjectEventSetAnimation(LOCALID_HEALER, 3);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 3);
  Engine_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 3);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_EAST_HEALER, 3);
  Engine_ScriptWait(50);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  // Garet and the Mayor react to Isaac accepting the burden.
  Engine_ScriptSetMessage(MsgSanctumAcceptIsaacCantMean);
  Script_ShowMessage(GARET, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, GARET, 0);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_GARETS_GRANDFATHER, 2);
  Engine_ObjectEventShowEmote(LOCALID_GARETS_GRANDFATHER,
                              EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ScriptWait(60);
  Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(20);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(40);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
  Engine_ScriptWait(20);
  Engine_ObjectEventFaceDirection(LOCALID_HEALER, FACING_NORTH + FACING_STEP,
                                  0);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
  Engine_ScriptWait(30);
  Script_ShowMessage(LOCALID_HEALER, 0);
  Engine_ScriptWait(80);
  Script_PlayCue(AUDIO_CUE_FADE_BGM);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PALE, PALETTE_HALF_BG);
  Engine_ColorBufferInterpolate(60);
  Engine_ScriptWait(40);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(192), -1, PIXELS(104), 1);
  Engine_ScriptWait(120);
  Script_PlayCue(MUS_021);
  Script_PlayCue(SE_308);
  // The Wise One enters while the healer's afterimages appear.
  ObjectEvent_SetPosition(LOCALID_WISE_ONE, PIXELS(200), PIXELS(8));
  ObjectEvent_SetSpeed(LOCALID_WISE_ONE, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_MoveToAndWait(LOCALID_WISE_ONE, 200, 72);
  Script_PlayCue(SE_STOP_SLOT3);
  Engine_ScriptWait(30);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_HEALER, 2);
  Engine_ObjectEventSetAnimation(LOCALID_HEALER, 0);
  Sanctum_ShowHealerAfterimage();
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_WISE_ONE, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_WISE_ONE, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_WISE_ONE, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_WISE_ONE, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_WISE_ONE, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_WISE_ONE, 0);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_DORA, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_GARETS_GRANDFATHER, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_WEST_HEALER, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_EAST_HEALER, 2);
  Engine_ScriptWait(40);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  Engine_ScriptWait(20);
  Engine_ScriptWait(40);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventSetChildValue(LOCALID_WISE_ONE, 0x100);
  Script_PlayCue(AUDIO_CUE_FADE_BGM);
  Script_PlayCue(SE_308);
  // Shrink the Wise One amid sparks before restoring the room.
  for (sparkIndex = 0; sparkIndex < 32; sparkIndex++) {
    Sanctum_SpawnObjectEventSpark(LOCALID_WISE_ONE);
    Engine_ScriptWait(4);
    wiseOne = ObjectEvent_GetById(LOCALID_WISE_ONE);
    wiseOne->scaleX += -Q16_ONE / 100;
    wiseOne = ObjectEvent_GetById(LOCALID_WISE_ONE);
    wiseOne->scaleY += -Q16_ONE / 100;
  }
  Script_PlayCue(SE_STOP_SLOT3);
  ObjectEvent_SetChildValue(LOCALID_WISE_ONE, 0);
  ObjectEvent_SetPosition(LOCALID_WISE_ONE, 0, 0);
  Engine_ScriptWait(40);
  Camera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
  Engine_CameraWaitForMove();
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, PALETTE_HALVES_BOTH);
  Engine_ColorBufferInterpolate(60);
  Engine_ScriptWait(120);
  Sanctum_HideHealerAfterimage();
  Engine_ObjectEventSetAnimation(LOCALID_HEALER, 1);
  Script_PlayCue(MUS_002);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_HEALER, 2);
  Engine_ScriptWait(20);
  Sanctum_StartAllRepeatedMotion();
  Engine_ScriptWait(30);
  Script_ShowMessage(LOCALID_DORA, 0);
  Engine_ScriptWait(30);
  ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
  Engine_ScriptWait(30);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(20);
  ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
  Engine_ScriptWait(30);
  Script_ShowMessage(LOCALID_HEALER, 0);
  Engine_ScriptWait(20);
  Sanctum_ShowAllEmotes();
  Engine_ScriptWait(30);
  ObjectEvent_ShowEmote(LOCALID_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(30);
  Script_ShowMessage(LOCALID_HEALER, 0);
  Engine_ScriptWait(20);
  Sanctum_NodAll();
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
  Engine_ScriptWait(180);
  ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, GARET, 0);
  Engine_ObjectEventFaceEachOther(LOCALID_WEST_HEALER, LOCALID_EAST_HEALER, 0);
  Engine_ScriptWait(40);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
  Engine_ScriptWait(40);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(GARET, 0);
  Engine_ScriptWait(30);
  ObjectEvent_ShowEmote(LOCALID_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(60);
  Sanctum_StartAllRepeatedMotion();
  Engine_ScriptWait(30);
  Script_ShowMessage(LOCALID_DORA, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(80);
  ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(30);
  ObjectEvent_ShowEmote(LOCALID_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_HEALER, 1);
  Engine_ObjectEventShowEmote(LOCALID_HEALER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB,
                              0);
  Engine_ScriptWait(20);
  Sanctum_StartAllRepeatedMotion();
  Engine_ScriptWait(40);
  Script_ShowMessage(LOCALID_HEALER, 0);
  Engine_ScriptWait(20);
  Sanctum_NodAll();
  Engine_ScriptWait(30);
  Script_ShowMessage(LOCALID_HEALER, 0);
  Engine_ScriptWait(30);
  Sanctum_NodAll();
  Engine_ScriptWait(120);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(20);
  Script_ShowMessage(GARET, 0);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_HEALER, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(LOCALID_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_GARETS_GRANDFATHER, 1);
  Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 4);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_HEALER, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        0);
  Engine_ScriptWait(60);
  ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HEALER, 3);
  Script_ShowMessage(LOCALID_HEALER, 0);
  Engine_ScriptWait(40);
  ObjectEvent_WalkToAndWait(LOCALID_HEALER, 168, 176);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
  ObjectEvent_WalkToAndWait(LOCALID_HEALER, 200, 200);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_WEST_HEALER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_EAST_HEALER, FACING_WEST, 0);
  ObjectEvent_WalkTo(LOCALID_HEALER, 200, 0x110);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_WEST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_EAST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  Engine_ObjectEventWaitForMove(LOCALID_HEALER);
  ObjectEvent_SetPosition(LOCALID_HEALER, 0, 0);
  Engine_ScriptWait(60);
  Sanctum_ShowAllEmotes();
  Engine_ScriptWait(60);
  Engine_ScriptWait(120);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_WEST_HEALER, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_EAST_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_WEST_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_WEST_HEALER,
                              0);
  Engine_ScriptWait(120);
  ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  Engine_ScriptWait(60);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_PARTY_LEADER, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_WEST_HEALER, 2);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_WEST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(80);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_EAST_HEALER, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_WEST_HEALER, 1);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_WEST_HEALER, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_EAST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventShowEmote(LOCALID_EAST_HEALER,
                              EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_WEST_HEALER, 2);
  Engine_ScriptWait(20);
  ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_PARTY_LEADER, 0);
  Engine_ScriptWait(30);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_EAST_HEALER, 2);
  Engine_ScriptWait(30);
  Engine_ObjectEventFaceDirection(LOCALID_EAST_HEALER,
                                  FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_EAST_HEALER, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 3);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_EAST_HEALER, 3);
  Engine_ScriptWait(20);
  ObjectEvent_WalkTo(LOCALID_WEST_HEALER, 200, 0x110);
  ObjectEvent_WalkToAndWait(LOCALID_EAST_HEALER, 200, 0x110);
  ObjectEvent_SetPosition(LOCALID_WEST_HEALER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_EAST_HEALER, 0, 0);
  Engine_ObjectEventShowEmote(LOCALID_GARETS_GRANDFATHER,
                              EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_ScriptWait(60);
  ObjectEvent_WalkToAndWait(LOCALID_GARETS_GRANDFATHER, 200, 136);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER,
                                  LOCALID_GARETS_GRANDFATHER, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_GARETS_GRANDFATHER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 4);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_DORA, 0);
  Engine_ScriptWait(20);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 168, 168);
  ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_DORA, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, 4);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_DORA, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 4);
  Engine_ScriptWait(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_GARETS_GRANDFATHER,
                              0);
  ObjectEvent_FaceObjectEvent(GARET, LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_DORA, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_DORA, 0);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventShowEmote(LOCALID_GARETS_GRANDFATHER,
                              EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ObjectEventShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, 3);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_DORA, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, 3);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 3);
}

/// Copies the healer into the afterimage actor.
static void Sanctum_ShowHealerAfterimage(void) {
  u32 entryIndex;
  struct ObjectEvent *afterimage;
  struct ObjectEvent *objectEvent;
  struct AnimationObject *sprite;

  objectEvent = ObjectEvent_GetById(LOCALID_HEALER);
  if (objectEvent != 0) {
    Engine_ObjectEventSetPosition(LOCALID_HEALER_AFTERIMAGE,
                                  objectEvent->x.fixed, objectEvent->z.fixed);
  }
  Engine_ObjectEventSetAnimation(LOCALID_HEALER_AFTERIMAGE, 0);
  afterimage = ObjectEvent_GetById(LOCALID_HEALER_AFTERIMAGE);
  objectEvent = ObjectEvent_GetById(LOCALID_HEALER);
  afterimage->facing = objectEvent->facing;
  objectEvent = ObjectEvent_GetById(LOCALID_HEALER_AFTERIMAGE);
  objectEvent->update = Sanctum_UpdateHealerAfterimage;
  objectEvent = ObjectEvent_GetById(LOCALID_HEALER_AFTERIMAGE);
  sprite = objectEvent->sprite;
  {
    for (entryIndex = 0; entryIndex < sprite->count; entryIndex++) {
      struct AnimationEntry *entry = sprite->entries[entryIndex];

      if (entry != 0 && entry->script != 0) {
        entry->colorMode = SPRITE_COLOR_RED_TINT;
      }
    }
  }
  sprite->dirty = 1;
  ObjectEvent_GetById(LOCALID_HEALER_AFTERIMAGE)->priorityFlags &=
      ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  sprite->image.priority = 2;
}

static void Sanctum_HideHealerAfterimage(void) {
  struct ObjectEvent *state = ObjectEvent_GetById(LOCALID_HEALER_AFTERIMAGE);
  state->update = 0;
  Engine_ObjectEventSetPosition(LOCALID_HEALER_AFTERIMAGE, 0, 0);
}

/// Cycles the afterimage through four offsets from the healer.
static void Sanctum_UpdateHealerAfterimage(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *healer;

  healer = ObjectEvent_GetById(LOCALID_HEALER);
  objectEvent->targetX = objectEvent->x.fixed = healer->x.fixed;
  objectEvent->targetY = objectEvent->y.fixed = healer->y.fixed;
  objectEvent->targetZ = objectEvent->z.fixed = healer->z.fixed - PIXELS(2);
  switch (gFrameCount & 3) {
  case 0:
    objectEvent->targetX = objectEvent->x.fixed =
        healer->x.fixed - PIXELS(7) / 2;
    break;
  case 1:
    objectEvent->targetX = objectEvent->x.fixed = healer->x.fixed + PIXELS(3);
    break;
  case 2:
    objectEvent->targetY = objectEvent->y.fixed = healer->y.fixed + PIXELS(2);
    break;
  case 3:
    objectEvent->targetZ = objectEvent->z.fixed = healer->z.fixed;
    break;
  }
}

/// Advances an effect around its linked actor on an elliptical path.
static void Sanctum_UpdateOrbitingEffect(union FieldObject *fieldObject) {
  struct ObjectEvent *effect = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *anchor = effect->linkedObject;
  u16 *angle = &effect->timer;
  u16 orbitAngle = *angle;
  s32 x;
  s32 z;
  s32 currentX;

  x = anchor->x.fixed + Engine_MathCos(orbitAngle) * 14;
  effect->x.fixed = x;

  z = anchor->z.fixed + Engine_MathSin(orbitAngle) * 10;
  currentX = effect->x.fixed;
  effect->z.fixed = z;
  effect->targetZ = z;
  effect->targetX = currentX;

  *angle = (*angle + effect->work);
}

/// Creates an orbiting effect with the requested angular step.
static void Sanctum_SpawnOrbitingEffect(s32 ownerId, s32 angleStep) {
  s32 orbitStep;
  struct ObjectEvent *owner;
  struct ObjectEvent *effect;
  struct AnimationObject *sprite;
  struct AnimationObject *sourceSprite;

  orbitStep = angleStep;
  owner = ObjectEvent_GetById(ownerId);
  if (owner != 0) {
    effect = ObjectEvent_Create(SPRITE_SPARKLE, owner->x.fixed,
                                owner->y.fixed + PIXELS(45), owner->z.fixed);
    if (effect != 0) {
      sprite = effect->sprite;
      Engine_ObjectSetScript(effect, gSanctum_OwnerEffectScript);
      {
        s32 zero = 0;

        effect->motionFlags = zero;
        effect->timer = zero;
      }
      effect->work = orbitStep;
      effect->update = Sanctum_UpdateOrbitingEffect;
      {
        u16 clearFlags = 0;
        sprite->flags = clearFlags;
      }
      sourceSprite = owner->sprite;
      effect->linkedObject = owner;
      sprite->image.priority = sourceSprite->image.priority;
    }
  }
}

#if LANGUAGE != LANGUAGE_JAPANESE
#define QUESTION_X 2
#define QUESTION_WIDTH 25
#define CURSOR_X 24
#else
#define QUESTION_X 7
#define QUESTION_WIDTH 17
#define CURSOR_X 64
#endif

/// Offers the refusal ending's restart choices and returns the selection.
static s32 Sanctum_ChooseRestartOption(void) {
  s32 slot;
  struct MenuSprite cursor;
  struct UiWindow *window;
  s32 text;
  s32 choice;

  Engine_ObjectEventSetPosition(LOCALID_HEALER, 0, 0);
  Engine_ObjectEventSetPosition(LOCALID_WEST_HEALER, 0, 0);
  Engine_ObjectEventSetPosition(LOCALID_EAST_HEALER, 0, 0);
  Engine_ObjectEventSetPosition(GARET, 0, 0);
  Engine_ObjectEventSetPosition(LOCALID_DORA, 0, 0);
  Engine_ObjectEventSetPosition(LOCALID_GARETS_GRANDFATHER, 0, 0);
  Engine_ObjectEventSetPosition(LOCALID_PARTY_LEADER, 0, 0);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, PALETTE_HALF_OBJ);
  Engine_ColorBufferInterpolate(1);
  Engine_ScriptWait(1);
  window = UiWindow_Create(QUESTION_X, 7, QUESTION_WIDTH, 5, 1);
  text = MsgSanctumGreatHealer;
  UiText_DrawResource(text, window, 16, 0);
  if (SaveState_CountRecordsExcludingFlagged(1) == 0) {
    UiText_DrawResource(text + 2, window, 16, 16);
  } else {
    UiText_DrawResource(text + 1, window, 16, 16);
  }
  FieldUiTextResource_Initialize(&cursor, &slot);
  FieldUiTextResource_SetPosition(&cursor, 72, 60);
  choice = 0;
  while ((gKeysPressed & KEY_A) == 0) {
    s32 x;

    if (gKeysRepeat & KEYS_VERTICAL) {
      choice ^= 1;
    }
    x = gSanctum_CursorBob[(gFrameTick >> 1) & 15] + CURSOR_X;
    FieldUiTextResource_SetPosition(&cursor, x, (choice << 4) + 60);
    Engine_ScriptWait(1);
  }
  UiTextResource_Release(slot);
  UiWork_Finalize(window, 1);
  return choice;
}

/// Vale Sanctum, the quest's account: Isaac and Garet act out, in sepia, what
/// happened in Sol Sanctum.
static void Sanctum_ReenactSolSanctum(void) {

  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_SetSpeed(GARET, PIXELS(3) / 2, PIXELS(3) / 4);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_GARETS_GRANDFATHER, 2);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 3);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(15);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, GARET, 0);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  Engine_ObjectEventWalkTo(LOCALID_PARTY_LEADER, 184, 168);
  ObjectEvent_GetById(GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  Engine_ObjectEventWalkToAndWait(GARET, 200, 168);
  Engine_ScriptWait(1);
  ObjectEvent_GetById(GARET)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  ObjectEvent_GetById(GARET)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  Engine_ObjectEventJump(GARET, 2, 0);
  Engine_ScriptWait(15);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_HEALER, 0);
  Engine_ScriptWait(5);
  Engine_ObjectEventJump(GARET, 2, 0);
  Engine_ScriptWait(25);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(5);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(5);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_PARTY_LEADER, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(15);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, 3);
  Engine_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 3);
  Engine_ObjectEventSetAnimation(LOCALID_HEALER, 3);
  Engine_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 3);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_EAST_HEALER, 3);
  Engine_ScriptWait(20);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER,
                                    LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_DORA, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventFaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  Engine_ScriptWait(15);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ScriptWait(10);
  // Illustrate the account with the floating item icon.
  Sanctum_RaiseItemIcon(222, PIXELS(184), PIXELS(27), PIXELS(168));
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_PARTY_LEADER, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(10);
  Engine_ObjectEventJump(GARET, 4, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_ScriptWait(15);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_ScriptWait(30);
  Engine_ObjectEventJump(GARET, 4, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ScriptWait(15);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ScriptWait(30);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(10);
  ObjectEvent_GetById(LOCALID_HEALER)->timer = GARET;
#ifdef UBFIX
  ObjectEvent_GetById(LOCALID_HEALER)->update =
      Sanctum_FaceObjectEventNamedByTimer;
#else
  ObjectEvent_GetById(LOCALID_HEALER)->update =
      (void (*)(union FieldObject *))Sanctum_FaceObjectEventNamedByTimer;
#endif
  ObjectEvent_GetById(LOCALID_GARETS_GRANDFATHER)->timer = GARET;
#ifdef UBFIX
  ObjectEvent_GetById(LOCALID_GARETS_GRANDFATHER)->update =
      Sanctum_FaceObjectEventNamedByTimer;
#else
  ObjectEvent_GetById(LOCALID_GARETS_GRANDFATHER)->update =
      (void (*)(union FieldObject *))Sanctum_FaceObjectEventNamedByTimer;
#endif
  Engine_ObjectEventWalkToAndWait(GARET, 196, 180);
  Engine_ObjectEventWalkToAndWait(GARET, 184, 184);
  Engine_ObjectEventWalkToAndWait(GARET, 180, 180);
  Engine_ObjectEventWalkToAndWait(GARET, 168, 168);
  Engine_ObjectEventWalkToAndWait(GARET, 180, 156);
  Engine_ObjectEventWalkTo(GARET, 200, 104);
  Engine_ObjectEventWalkToAndWait(LOCALID_PARTY_LEADER, 192, 168);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ObjectEventWaitForMove(GARET);
  Engine_ScriptWait(30);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptWait(15);
  ObjectEvent_GetById(LOCALID_GARETS_GRANDFATHER)->update = NULL;
  ObjectEvent_GetById(LOCALID_HEALER)->update = NULL;
  Engine_ObjectEventStartRepeatedMotion(LOCALID_HEALER, 2);
  Engine_ObjectEventShowEmote(LOCALID_HEALER,
                              EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventSetAnimation(LOCALID_HEALER, 0);
  Engine_ObjectEventShowEmote(LOCALID_PARTY_LEADER,
                              EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ScriptWait(60);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_DORA, 3);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ScriptWait(10);
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 2, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 2, 0);
  Engine_ScriptWait(20);
  Engine_ScriptWait(15);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER,
                                  LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 3);
  Engine_ScriptWait(60);
  Engine_ObjectEventWalkToAndWait(GARET, 208, 168);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_SHAKE_HEAD);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(10);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(10);
  Engine_ObjectEventFaceDirection(GARET, FACING_EAST, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 3);
  Engine_ScriptWait(10);
  Engine_ObjectEventFaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  Engine_ObjectEventFaceDirection(GARET, FACING_EAST, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(10);
  // Repeat the synchronized eight-point paths around the room.
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gSanctum_LeaderCircleScript);
  ObjectEvent_SetActionScript(GARET, gSanctum_GaretCircleScript);
  ObjectEvent_RefreshSelectorById(LOCALID_PARTY_LEADER);
  ObjectEvent_RefreshSelectorById(GARET);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_SetSpeed(GARET, PIXELS(3) / 2, PIXELS(3) / 4);
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 6, 0);
  Engine_ObjectEventJump(GARET, 6, 0);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_WEST_HEALER,
                                    0);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_HEALER, 0);
  Engine_ScriptWait(1);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER,
                                    LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_DORA, 0);
  Engine_ScriptWait(1);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_WEST_HEALER, 0);
  Engine_ScriptWait(1);
  Engine_ObjectEventWalkTo(LOCALID_PARTY_LEADER, 192, 168);
  Engine_ObjectEventWalkToAndWait(GARET, 208, 168);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ObjectEventFaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH + FACING_STEP,
                            0);
  Engine_ObjectEventFaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventFaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  Engine_ObjectEventFaceObjectEvent(GARET, LOCALID_GARETS_GRANDFATHER, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimation(LOCALID_DORA, 3);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 3);
  Engine_ScriptWait(30);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, GARET, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(10);
  Engine_ObjectEventFaceDirection(GARET, FACING_EAST, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_WALK);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_WALK);
  Engine_ScriptWait(60);
}

/// Shows an item icon rising from the supplied fixed-point position.
static void Sanctum_RaiseItemIcon(s32 item, s32 fixedX, s32 fixedY,
                                  s32 fixedZ) {
  struct ObjectEvent *icon;
  struct AnimationObject *sprite;
  u8 *transferBuffer;
  u32 frame;
  u8 zero;

  icon = Engine_ObjectCreate(SPRITE_BLANK, fixedX, fixedY, fixedZ);
  zero = 0;
  if (icon != 0) {
    Engine_ObjectSetScript(icon, gSanctum_ItemIconGrowScript);
    sprite = icon->sprite;
    sprite->flags = zero;
    sprite->count = zero;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    icon->velocityY = PIXELS(2);
    icon->gravity = PIXELS(1) / 4;
    transferBuffer = Runtime_AllocateHeapBlock(17, 0x608);
    Ui_PrepareTransferForItem(item);
    Engine_VramLoad(sprite->slot, 128, transferBuffer + 0x400);
    Runtime_ReleaseHeapBlock(17);
    for (frame = 0; frame < 60; frame++) {
      if ((u32)(icon->velocityY + 255) <= 0x1fe)
        icon->motionFlags = 0;
      Engine_TaskWait(1);
    }
    Engine_ObjectSetScript(icon, gSanctum_ItemIconEndScript);
  }
}

/// Moves a spark upward, cycles its scale, and expires its lifetime.
static void Sanctum_UpdateRisingSpark(union FieldObject *fieldObject) {
  struct ObjectEvent *spark = &fieldObject->objectEvent;
  s32 scale;
  s32 timer;

  // The timer stores the upward step in sixteenths of a pixel.
  spark->y.fixed += spark->timer << 12;
  spark->targetY = spark->y.fixed;
  scale = 0;
  switch (((s16)spark->work >> 2) & 3) {
  case 0:
    scale = Q16_ONE;
    break;
  case 1:
  case 3:
    scale = 4 * Q16_ONE / 5;
    break;
  case 2:
    scale = 3 * Q16_ONE / 5;
    break;
  }
  spark->scaleX = scale;
  spark->scaleY = scale;
  timer = spark->work - 1;
  spark->work = timer;
  if ((s16)timer <= 0)
    Engine_ObjectSetScript(spark, gSanctum_SparkEndScript);
}

/// Creates a short-lived spark beside the specified actor.
static void Sanctum_SpawnObjectEventSpark(s32 id) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *spark;
  struct AnimationObject *sprite;
  s32 x;
  s32 randomOffset;

  objectEvent = ObjectEvent_GetById(id);
  if (objectEvent == 0)
    return;
  randomOffset = Math_RemainderUnsigned(Engine_RandomNext(), 20);
  x = objectEvent->x.fixed;
  x += randomOffset << 16;
  x += -PIXELS(10);
  spark = Engine_ObjectCreate(
      0x11e, x,
      objectEvent->y.fixed + ((Engine_RandomNext() & 15) << 16) + -PIXELS(8),
      objectEvent->z.fixed);
  if (spark == 0)
    return;
  sprite = spark->sprite;
  spark->motionFlags = 0;
  spark->timer = Math_RemainderUnsigned(Engine_RandomNext(), 10) + 5;
  spark->work = Math_RemainderUnsigned(Engine_RandomNext(), 60) + 30;
  spark->update = Sanctum_UpdateRisingSpark;
  sprite->flags = 0;
  sprite->image.priority = objectEvent->sprite->image.priority;
}

/// Copies two blocks of map cells for the quest's visits, the tiles of one and
/// the collision of the other.
static void Sanctum_RestoreQuestFloor(void) {
  {
    MapGrid_CopyCellsTo(0, 64, 11, 68, 3, 2);
  }
  {
    MapGrid_CopyCellAttributes(11, 10, 3, 2, 11, 8);
  }
  Engine_TaskWait(1);
}

static s32 Sanctum_IsLeaderFacingHealer(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if ((u32)((objectEvent->facing + 0x5fff) << 16) <= 0x3ffe0000) {
    return 1;
  }
  return 0;
}

/// Vale Sanctum: talking to the Great Healer outside the quest. From the front
/// the healer opens the Sanctum's services; otherwise the words follow the
/// story, from "Wielders of Psynergy are called Adepts" before the party has
/// left Vale to the closed path to Sol Sanctum once it has reached Kalay.
static void EventScript_TalkToHealer(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(LOCALID_HEALER);
    return;
  }

  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0)
    Engine_ScriptSetMessage(MsgSanctumPathSolSanctumStillClosed);
  else if (Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT) != 0)
    Engine_ScriptSetMessage(MsgSanctumMyControlOverPsynergyHas);
  else
    Engine_ScriptSetMessage(MsgSanctumWieldersPsynergyCalledAdeptsAdepts);
  Engine_ScriptShowMessage(LOCALID_HEALER, 0);
  Engine_ScriptEnd();
}

/// Vault Sanctum: talking to the healer. From the front the healer opens the
/// Sanctum's services; otherwise the healer speaks of the gold statue or, on
/// the later visit, of the earthquakes. Its case for the quest's entrances
/// (SANCTUM_FROM_VALE_ERUPTION, SANCTUM_FROM_QUEST_RESTART and
/// SANCTUM_QUEST_UNUSED) passes to EventScript_ReconsiderQuest, though their
/// events (gSanctum_QuestEvents) call that directly.
static void EventScript_TalkToHealerOnVisit(void) {
  s32 warpDest;

  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(LOCALID_HEALER);
    return;
  }

  Engine_ScriptStart();

  warpDest = gGameState.warpDest;

  switch (warpDest) {
  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_VAULT_INTERIORS:
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Engine_ScriptSetMessage(MsgSanctumWasHandFateReturnedGold);
    } else {
      Engine_ScriptSetMessage(MsgSanctumWeWillHelpAnytimeAs);
    }
    break;
  case SANCTUM_FROM_VAULT_REVISIT:
    Engine_ScriptSetMessage(MsgSanctumMayWrongButLatelyThere);
    break;
  case SANCTUM_FROM_VALE_ERUPTION:
  case SANCTUM_FROM_QUEST_RESTART:
  case SANCTUM_QUEST_UNUSED:
    Engine_ScriptEnd();
    EventScript_ReconsiderQuest();
    return;
  default:
    break;
  }

  Engine_ScriptShowMessage(LOCALID_HEALER, 0);
  Engine_ScriptEnd();
}

/// Bilibin Sanctum: talking to the healer, about Kolima's curse, lifted or not;
/// from the front, the Sanctum's services.
static void EventScript_TalkAboutCurse(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(LOCALID_HEALER);
    return;
  }

  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0)
    Engine_ScriptSetMessage(MsgSanctumCurseMayOverButWe);
  else
    Engine_ScriptSetMessage(MsgSanctumAmStartingFeelOnlyBeginning);
  Engine_ScriptShowMessage(LOCALID_HEALER, 0);
  Engine_ScriptEnd();
}

/// Altin Sanctum: talking to the healer, about the beasts in the mine, or the
/// party's saving Altin from them; from the front, the Sanctum's services.
static void EventScript_TalkAboutAltinMonsters(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(LOCALID_HEALER);
    return;
  }

  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgSanctumWasAfterEerieNightWhen);
  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0)
    Engine_ScriptSetMessage(MsgSanctumSavedAltinFromMonstersClearly);
  Engine_ScriptShowMessage(LOCALID_HEALER, 0);
  Engine_ScriptEnd();
}

/// Xian Sanctum: talking to the healer, who came to Xian from very distant
/// lands; from the front, the Sanctum's services.
static void EventScript_TalkAboutJourneyFromXian(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(LOCALID_HEALER);
    return;
  }

  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgSanctumCameXianFromVeryDistant);
  Engine_ScriptShowMessage(LOCALID_HEALER, 0);
  Engine_ScriptEnd();
}

// clang-format off
u32 gSanctum_LeaderCircleScript[] = {
    SC_ANIMATION(2),
    SC_SET(SW_SPEED, PIXELS(5) / 2),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 2),
    SC_LABEL(1),
    SC_MOVE(PIXELS(204), 0, PIXELS(188)),
    SC_SYNC,
    SC_MOVE(PIXELS(216), 0, PIXELS(192)),
    SC_SYNC,
    SC_MOVE(PIXELS(236), 0, PIXELS(188)),
    SC_SYNC,
    SC_MOVE(PIXELS(240), 0, PIXELS(168)),
    SC_SYNC,
    SC_MOVE(PIXELS(236), 0, PIXELS(156)),
    SC_SYNC,
    SC_MOVE(PIXELS(216), 0, PIXELS(144)),
    SC_SYNC,
    SC_MOVE(PIXELS(204), 0, PIXELS(156)),
    SC_SYNC,
    SC_MOVE(PIXELS(192), 0, PIXELS(168)),
    SC_SYNC,
    SC_LOOP(4, SC_LABEL(1)),
    SC_ANIMATION(1),
    SC_END,
};

u32 gSanctum_GaretCircleScript[] = {
    SC_ANIMATION(2),
    SC_SET(SW_SPEED, PIXELS(5) / 2),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 2),
    SC_LABEL(1),
    SC_MOVE(PIXELS(204), 0, PIXELS(156)),
    SC_SYNC,
    SC_MOVE(PIXELS(184), 0, PIXELS(144)),
    SC_SYNC,
    SC_MOVE(PIXELS(172), 0, PIXELS(156)),
    SC_SYNC,
    SC_MOVE(PIXELS(160), 0, PIXELS(168)),
    SC_SYNC,
    SC_MOVE(PIXELS(172), 0, PIXELS(188)),
    SC_SYNC,
    SC_MOVE(PIXELS(184), 0, PIXELS(192)),
    SC_SYNC,
    SC_MOVE(PIXELS(204), 0, PIXELS(188)),
    SC_SYNC,
    SC_MOVE(PIXELS(208), 0, PIXELS(168)),
    SC_SYNC,
    SC_LOOP(4, SC_LABEL(1)),
    SC_ANIMATION(1),
    SC_END,
};

s32 gSanctum_ItemIconGrowScript[] = {
    SC_SET(SW_SCALE_X, Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 4),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 4),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 7 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 7 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 15 * Q16_ONE / 16),
    SC_SET(SW_SCALE_Y, 15 * Q16_ONE / 16),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(16),
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_END,
};

s32 gSanctum_ItemIconEndScript[] = {
    SC_WAIT(60),
    SC_FREE,
};

struct MapWarpDest gSanctum_WarpDests[] = {
    { SANCTUM_UNUSED, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SANCTUM_FROM_VALE, CONDITION_ALWAYS, 199, 0, 246, FACING_NORTH, 0, 64, 8, 336, 272, 0 },
    { SANCTUM_VALE_UNUSED, CONDITION_ALWAYS, 199, 0, 164, FACING_SOUTH, 0, 64, 8, 336, 272, 0 },
    { SANCTUM_FROM_VAULT, CONDITION_ALWAYS, 199, 0, 246, FACING_NORTH, 0, 64, 8, 336, 272, 0 },
    { SANCTUM_FROM_VAULT_REVISIT, CONDITION_ALWAYS, 199, 0, 246, FACING_NORTH, 0, 64, 8, 336, 272, 0 },
    { SANCTUM_FROM_VAULT_INTERIORS, CONDITION_ALWAYS, 199, 0, 164, FACING_SOUTH, 0, 64, 8, 336, 272, 0 },
    { SANCTUM_FROM_VALE_ERUPTION, CONDITION_ALWAYS, 192, 0, 168, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { SANCTUM_FROM_QUEST_RESTART, CONDITION_ALWAYS, 192, 0, 168, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { SANCTUM_FROM_ALTIN, CONDITION_ALWAYS, 200, 0, 240, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { SANCTUM_FROM_XIAN, CONDITION_ALWAYS, 200, 0, 240, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { SANCTUM_FROM_BILIBIN, CONDITION_ALWAYS, 200, 0, 240, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { MAP_TABLE_END },
};

u32 gSanctum_Warps[] = {
    MAP_WARPS(Sanctum),
    MAP_WARP(SANCTUM_TO_VALE, Vale, VALE_FROM_SANCTUM),
    MAP_WARP(SANCTUM_TO_VAULT, Vault, VAULT_FROM_SANCTUM),
    MAP_WARP(SANCTUM_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_SANCTUM),
    MAP_WARP(SANCTUM_TO_QUEST_RESTART, Sanctum, SANCTUM_FROM_QUEST_RESTART),
    MAP_WARP(SANCTUM_TO_VALE_GATE, ValeGate, VALE_GATE_FROM_SANCTUM),
    MAP_WARP(SANCTUM_TO_ALTIN, Altin, ALTIN_FROM_SANCTUM),
    MAP_WARP(SANCTUM_TO_BILIBIN, Bilibin, BILIBIN_FROM_SANCTUM),
    MAP_WARP(SANCTUM_TO_XIAN, Xian, XIAN_FROM_SANCTUM_WEST),
    MAP_WARP(SANCTUM_TO_CLEAR, Clear, CLEAR_FROM_SANCTUM),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gSanctum_NoObjectEventTemplates[] = {
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSanctum_ValeObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(130), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};

#if TBS_PATCH_1
struct ObjectEventTemplate gSanctum_IvanObjectEventTemplate[] = {
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};
#endif

struct ObjectEventTemplate gSanctum_VaultBilibinObjectEventTemplates[] = {
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(130), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(200), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xdd, CONDITION_ALWAYS, 7,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSanctum_QuestObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(208), 0, PIXELS(168), FACING_NORTH + FACING_STEP, 3, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(224), 0, PIXELS(216), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(246), 0, PIXELS(198), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(208), FACING_NORTH + FACING_STEP, 3, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, 3, 0 },
    { 0xfc, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSanctum_AltinXianObjectEventTemplates[] = {
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(132), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gSanctum_ValeEvents[] = {
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToHealer) },
    { MAP_EVENT_TALK, LOCALID_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumOurPsynergyWasGivenToUs) },
    { MAP_EVENT_TALK, LOCALID_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumPsynergyIsAPowerThatShould) },
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { MAP_EVENTS_END },
};

struct MapEvent gSanctum_VaultEvents[] = {
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToHealerOnVisit) },
    { MAP_EVENT_TALK, LOCALID_SANCTUM_MEMBER, FLAG_VAULT_THIEVES_JAILED, EVENT_SCRIPT(EventScript_TalkAboutGoldenStatue) },
    { MAP_EVENT_TALK, LOCALID_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutGoldenStatue) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumGoOnIfThatEasesThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindAboutGoldenStatue) },
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SANCTUM_TO_VAULT },
    { MAP_EVENTS_END },
};

struct MapEvent gSanctum_VaultRevisitEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SANCTUM_TO_VAULT_REVISIT },
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToHealerOnVisit) },
    { MAP_EVENT_TALK, LOCALID_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutGoldenStatue) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumIHopeTheEarthquakesArentA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindAboutGoldenStatue) },
    { MAP_EVENTS_END },
};

struct MapEvent gSanctum_QuestEvents[] = {
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReconsiderQuest) },
    { MAP_EVENT_TALK, GARET, CONDITION_ALWAYS, MSGNO(MsgSanctumItIsATerribleBurdenTo) },
    { MAP_EVENT_TALK, LOCALID_DORA, CONDITION_ALWAYS, MSGNO(MsgSanctumMotherWouldNeverDoubtYourCourage) },
    { MAP_EVENT_TALK, LOCALID_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgSanctumIDontBlameYouICouldnt) },
    { MAP_EVENT_TALK, LOCALID_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWasItRightToRefuseWhen) },
    { MAP_EVENT_TALK, LOCALID_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItsClearThatTheGreatHealer) },
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveAfterRefusing) },
    { MAP_EVENT_COORD, 2, FLAG_MAP_TEMP_00, EVENT_SCRIPT(EventScript_WarnBeforeLeaving) },
    { MAP_EVENTS_END },
};

struct MapEvent gSanctum_ValeEventsAfterLeaving[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToHealer) },
    { MAP_EVENT_TALK, LOCALID_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumThePowerOfMtAlephStems) },
    { MAP_EVENT_TALK, LOCALID_EAST_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutPsynergyChanges) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumThePowerOfMyPsynergyHas) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumTheAngaranContinentWillCertainlySee) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumMyPsynergyHasntChangedAtAll) },
    { MAP_EVENTS_END },
};

struct MapEvent gSanctum_ValeEventsAfterKalay[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToHealer) },
    { MAP_EVENT_TALK, LOCALID_WEST_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutNewAdepts) },
    { MAP_EVENT_TALK, LOCALID_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWhyIsItThatDifferentAdepts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItllBeDifficultToReachSol) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumISeeABeautifulGirlHas) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItsNotFairIWantTo) },
    { MAP_EVENTS_END },
};

struct MapEvent gSanctum_AltinEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SANCTUM_TO_ALTIN },
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutAltinMonsters) },
    // Unused: a copy of the row above, which always answers first.
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutAltinMonsters) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgSanctumPleaseSaveTheHelplessInAltin) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumAlmightyFateMustHaveHeardMe) },
    { MAP_EVENTS_END },
};

struct MapEvent gSanctum_BilibinEvents[] = {
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutCurse) },
    { MAP_EVENT_TALK, LOCALID_SANCTUM_MEMBER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumPleaseGiveMeStrengthAtTimes) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumWillITurnIntoATree) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SANCTUM_MEMBER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumSomeoneWillSaveUsJustKeep) },
    { MAP_EVENT_TALK, LOCALID_SANCTUM_MEMBER, CONDITION_ALWAYS, MSGNO(MsgSanctumHowAboutThatOurWishesHave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumTheSignsOfDangerAreActually) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SANCTUM_MEMBER, CONDITION_ALWAYS, MSGNO(MsgSanctumPeaceWillBeKeptAsLong) },
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SANCTUM_TO_BILIBIN },
    { MAP_EVENTS_END },
};

struct MapEvent gSanctum_XianEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SANCTUM_TO_XIAN },
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutJourneyFromXian) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWhenYouStayInXianFor) },
    { MAP_EVENTS_END },
};
// clang-format on

s32 gSanctum_CursorBob[] = {
    SC_WAIT(0),
    SC_WAIT(1),
    SC_SYNC,
    SC_SYNC,
    SC_POSITION(0x2, 0x3, 0x4),
    SC_MOVE(0x2, 0x2, 0x1),
    SC_SYNC,
    SC_SYNC,
};

s32 gSanctum_OwnerEffectScript[] = {
    SC_ADD(SW_Y, -0x8000),
    SC_ADD(SW_TARGET_Y, -0x8000),
    SC_WAIT(1),
    SC_LOOP(96, SC_TOP),
    SC_FREE,
};

s32 gSanctum_SparkEndScript[] = {
    SC_FREE,
};
