/// @file
/// The Sanctum of Vale and of four towns on the party's way, which holds six
/// places in one map (SceneId_Sanctum): the warp destination the party arrives
/// at decides which, and Sanctum_GetObjectEventTemplates and Sanctum_GetEvents
/// choose that place's tables by it. Spoken to from the front, each place's
/// healer opens the Sanctum's services (Sanctum_Run); from any other side the
/// healer talks. Each place's door leads back out to its own town.
///
/// - Vale Sanctum (SANCTUM_FROM_VALE, SANCTUM_FROM_VALE_ERUPTION,
///   SANCTUM_VALE_UNUSED and SANCTUM_FROM_QUEST_RESTART), the Great Healer's,
///   entered from Vale at SANCTUM_FROM_VALE. Before the trip up Mt. Aleph, the
///   Great Healer and two other healers (LOCALID_WEST_HEALER and
///   LOCALID_EAST_HEALER) speak of Psynergy. After the eruption the Great
///   Healer brings Isaac and Garet in from Vale (vale_eruption.c) to
///   SANCTUM_FROM_VALE_ERUPTION, where Dora, the Mayor and the healers hear
///   what happened on the mountain, acted out in sepia
///   (Sanctum_ReenactSolSanctum): the palette fade turns every background and
///   sprite palette to PALETTE_TINT_SEPIA for the account and fades back to
///   PALETTE_TINT_NONE over 60 frames after it. The Great Healer asks Isaac to
///   take on the Wise One's quest (Sanctum_RequestQuestAcceptance). Each time
///   his Psynergy shows, LOCALID_HEALER_AFTERIMAGE, a copy of him in
///   SPRITE_COLOR_RED_TINT, jumps round him by a few pixels a frame
///   (Sanctum_UpdateHealerAfterimage) while effects circle him
///   (Sanctum_SpawnOrbitingEffect). Accepting plays
///   Sanctum_ReceiveWiseOnesMission and sets FLAG_SANCTUM_QUEST_ACCEPTED, and
///   SANCTUM_TO_VALE_PLAZA takes the two to SceneId_ValePlaza (vale_plaza.c).
///   Refusing leaves the room to gSanctum_QuestEvents: the Great Healer asks
///   again when spoken to (EventScript_ReconsiderQuest), the door draws a
///   warning (FLAG_SANCTUM_WARNED_BEFORE_LEAVING), and leaving anyway ends the
///   game (EventScript_LeaveAfterRefusing): `And so the world began drifting
///   toward its fated destruction...` From there the player can start again
///   from the Great Healer, by SANCTUM_TO_QUEST_RESTART to
///   SANCTUM_FROM_QUEST_RESTART, which skips the account
///   (FLAG_SANCTUM_QUEST_RESTARTED), or leave by SANCTUM_TO_MAIN_MENU to
///   SceneId_MainMenu. After the party has left Vale (FLAG_VALE_PARTY_LEFT),
///   and again after it has reached Kalay (FLAG_KALAY_REACHED), the three talk
///   of what the eruption and the Wise One changed.
/// - Vault Sanctum (SANCTUM_FROM_VAULT and SANCTUM_FROM_LOG_ROOMS_A). Its
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
/// - Sanctum, the name the game gives every other warp destination, which
///   leaves only SANCTUM_UNUSED; no warp leads there, and it places nobody.

#include "speaker.h"
#include "flags.h"
#include "save_block.h"
#include "unit/map/sanctum.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
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
#include "text/msg_ids.h"
#include "io_reg.h"
#include "save.h"
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
#include "heap.h"
#include "item_ids.h"
#include "audio_engine_symbols.h"

/// After the quest is refused, the Great Healer has warned that stepping
/// outside the village is final.
#define FLAG_SANCTUM_WARNED_BEFORE_LEAVING FLAG_SCENE_TEMP_00
/// The party is starting again from the Great Healer after refusing the quest,
/// so the account of Sol Sanctum is skipped.
#define FLAG_SANCTUM_QUEST_RESTARTED FLAG_SCENE_TEMP_01

// Map flags, in Vale Sanctum: 00 is set once the Great Healer has
// warned Isaac that leaving the village cannot be undone; 01 marks the visit
// after a refusal (SANCTUM_FROM_QUEST_RESTART), which skips the account of Sol
// Sanctum.

/// The placed object events, numbering the rows of the table
/// Sanctum_GetObjectEventTemplates returns for the warp destination; the
/// party's rows take their own ids. Every place's first row is its healer.
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
  /// cottage_rooms_a.c call the Sanctum's people: "The healers
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
  /// log_rooms_a.c), placed off the map until the thieves are jailed
  /// (FLAG_VAULT_THIEVES_JAILED).
  /// ★ Named from the game: "our stolen gold statue"
  /// (MsgSanctumWonderIfEverSeeOur)
  LOCALID_GOLD_STATUE
};

static void Sanctum_RequestQuestAcceptance(void);
static void Sanctum_ReceiveWiseOnesMission(void);
static void Sanctum_ShowHealerAfterimage(void);
static void Sanctum_HideHealerAfterimage(void);
static void
Sanctum_UpdateHealerAfterimage(union ObjectEventOrEffect *fieldObject);
static void Sanctum_SpawnOrbitingEffect(s32 localId, s32 angleStep);
static s32 Sanctum_ChooseRestartOption(void);
static void Sanctum_ReenactSolSanctum(void);
static void Sanctum_RaiseItemIcon(s32 item, s32 fixedX, s32 fixedY, s32 fixedZ);
static void Sanctum_SpawnObjectEventSpark(s32 id);
static void Sanctum_RestoreQuestFloor(void);

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 44 to 42 bytes in tbs-en.
#ifdef UBFIX
/// Faces the object event whose ID this callback stores in the timer field.
static void
Sanctum_FaceObjectEventNamedByTimer(union ObjectEventOrEffect *fieldObject) {
#else
/// Faces the object event whose ID this callback stores in the timer field.
static s32
Sanctum_FaceObjectEventNamedByTimer(union ObjectEventOrEffect *fieldObject) {
#endif
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *target;

  target = ObjectEvent_GetById((s16)objectEvent->timer);
  objectEvent->facing = ArcTan2(target->z.fixed - objectEvent->z.fixed,
                                target->x.fixed - objectEvent->x.fixed);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Returns the warp destinations, shared by all six places.
const struct MapWarpDest *Sanctum_GetWarpDests(void) {
  return gSanctum_WarpDests;
}

const struct MapWarpRegion *Sanctum_GetRegions(void) { return NULL; }

const u32 *Sanctum_GetWarps(void) { return gSanctum_Warps; }

/// Chooses the object events by warp destination: Vale Sanctum's Great Healer
/// and two other healers (SANCTUM_FROM_VALE and SANCTUM_VALE_UNUSED), the
/// quest's gathering (SANCTUM_FROM_VALE_ERUPTION and
/// SANCTUM_FROM_QUEST_RESTART), the healer and LOCALID_SANCTUM_MEMBER of Vault
/// Sanctum and Bilibin Sanctum (SANCTUM_FROM_VAULT, SANCTUM_FROM_VAULT_REVISIT,
/// SANCTUM_FROM_LOG_ROOMS_A and SANCTUM_FROM_BILIBIN), the healer alone in
/// Altin Sanctum and Xian Sanctum (SANCTUM_FROM_ALTIN and SANCTUM_FROM_XIAN),
/// and nobody otherwise.
const struct ObjectEventTemplate *Sanctum_GetObjectEventTemplates(void) {
  switch (gSaveBlock.warpDest) {
  case SANCTUM_FROM_VALE:
  case SANCTUM_VALE_UNUSED:
    return gSanctum_ValeObjectEventTemplates;

  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_VAULT_REVISIT:
  case SANCTUM_FROM_LOG_ROOMS_A:
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

/// Chooses the map events by warp destination, and in Vale Sanctum before and
/// after the quest by story flag: gSanctum_ValeEvents before the party has left
/// Vale, gSanctum_ValeEventsAfterLeaving once it has (FLAG_VALE_PARTY_LEFT) and
/// gSanctum_ValeEventsAfterKalay once it has reached Kalay
/// (FLAG_KALAY_REACHED); gSanctum_QuestEvents for the quest's visits
/// (SANCTUM_FROM_VALE_ERUPTION, SANCTUM_FROM_QUEST_RESTART and
/// SANCTUM_QUEST_UNUSED).
const struct MapEvent *Sanctum_GetEvents(void) {
  switch (gSaveBlock.warpDest) {
  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_LOG_ROOMS_A:
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

  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
    return gSanctum_ValeEventsAfterKalay;
  }
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    return gSanctum_ValeEventsAfterLeaving;
  }
  return gSanctum_ValeEvents;
}

/// Vault Sanctum: talking to LOCALID_SANCTUM_MEMBER, about the stolen gold
/// statue until the thieves are jailed (FLAG_VAULT_THIEVES_JAILED) and about
/// fate afterwards; on the later visit (SANCTUM_FROM_VAULT_REVISIT), the statue
/// polished.
static void EventScript_TalkAboutGoldenStatue(void) {
  FieldScript_Start();

  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
    FieldScript_SetMessageId(MsgSanctumWhenStrayFromYourWorldly);
  } else {
    FieldScript_SetMessageId(MsgSanctumHealerMustWorriedAboutNever);
  }

  if (gSaveBlock.warpDest == SANCTUM_FROM_VAULT_REVISIT) {
    FieldScript_SetMessageId(MsgSanctumPolishedGoldStatueReturnedUs);
  }

  Import_ObjectEventSetAnimation(LOCALID_SANCTUM_MEMBER, 1);
  ObjectEvent_LinkPair(LOCALID_SANCTUM_MEMBER, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(2);
  FieldScript_ShowMessageAndWait(LOCALID_SANCTUM_MEMBER, 0);
  FieldScript_End();
}

/// Vault Sanctum: Mind Read on LOCALID_SANCTUM_MEMBER, about the stolen statue,
/// or once it is back, about Isaac awakening to the Sanctum's teachings; on the
/// later visit, the statue cleaned up.
static void EventScript_ReadMindAboutGoldenStatue(void) {

  FieldScript_Start();

  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    FieldScript_SetMessageId(MsgSanctumWonderIfEverSeeOur);
  } else {
    FieldScript_SetMessageId(MsgSanctumChildHasAwakenedOurTeachings);
  }

  if (gSaveBlock.warpDest == SANCTUM_FROM_VAULT_REVISIT) {
    FieldScript_SetMessageId(MsgSanctumDirtyGoldenStatueCleanedUp);
  }

  ObjectMotion_EnableActionAndResetMotion(LOCALID_SANCTUM_MEMBER);
  Import_ObjectEventSetAnimation(LOCALID_SANCTUM_MEMBER, 1);
  FieldScript_WaitFrames(2);
  FieldScript_ShowMessageAndWait(LOCALID_SANCTUM_MEMBER, 0);
  ObjectEvent_SetBuiltinAction(LOCALID_SANCTUM_MEMBER, 2);
  FieldScript_End();
}

/// Vale Sanctum: talking to the Great Healer after Isaac has refused the quest.
/// "Are you sure?" Changing his mind, Isaac can accept after all, and the
/// quest goes on as if he had accepted at once.
static void EventScript_ReconsiderQuest(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgSanctumAreYouSure);
  FieldMessage_ShowAtObject(LOCALID_HEALER, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
    FieldScript_WaitFrames(20);
  } else {
    FieldScript_WaitFrames(20);
    FieldMessage_ShowAtObject(LOCALID_HEALER, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      FieldScript_WaitFrames(20);
      FieldMessage_ShowAtObject(LOCALID_HEALER, 0);
      if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
        FieldScript_WaitFrames(20);
        if (ObjectEvent_GetById(LOCALID_HEALER)->facing >= FACING_NORTHWEST &&
            ObjectEvent_GetById(LOCALID_HEALER)->facing <= FACING_NORTHEAST) {
          ObjectEvent_SetSpeed(LOCALID_HEALER, PIXELS(1) / 2, PIXELS(1) / 4);
          ObjectMotion_ArmCallback(LOCALID_HEALER, FACING_EAST, 0);
          FieldScript_WaitFrames(10);
          ObjectEvent_GetById(LOCALID_HEALER)->steeringFlags &=
              ~OBJ_EVENT_FACES_MOTION;
          ObjectMotion_SetPositionAndReset(LOCALID_HEALER, 152, 120);
          FieldScript_WaitFrames(1);
          ObjectEvent_GetById(LOCALID_HEALER)->steeringFlags |=
              OBJ_EVENT_FACES_MOTION;
          FieldScript_WaitFrames(20);
          ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
          FieldScript_WaitFrames(20);
          ObjectMotion_SetPositionAndReset(LOCALID_PARTY_LEADER, 168, 120);
          ObjectMotion_ResetAndSetPositionInMode2(LOCALID_PARTY_LEADER, 192,
                                                  168);
          FieldScript_WaitFrames(20);
          ObjectMotion_SetPositionAndReset(LOCALID_HEALER, 168, 120);
          ObjectMotion_ArmCallback(LOCALID_HEALER,
                                   FACING_SOUTHEAST + FACING_STEP, 0);
          ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
        } else {
          ObjectMotion_ResetAndSetPositionInMode2(LOCALID_PARTY_LEADER, 192,
                                                  168);
          FieldScript_WaitFrames(20);
          ObjectMotion_ArmCallback(LOCALID_HEALER,
                                   FACING_SOUTHEAST + FACING_STEP, 0);
          ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
        }
        Sanctum_ReceiveWiseOnesMission();
        PaletteFade_SetTarget(RGB(0, 0, 0), PALETTES_ALL);
        PaletteFade_StartInterpolation(120);
        FieldScript_WaitFrames(120);
        Sound_PlaySong(MUS_INN);
        Sound_WaitForSong();
        GameFlag_Set(FLAG_SANCTUM_QUEST_ACCEPTED);
        Import_ScriptRequestWarp(SANCTUM_TO_VALE_PLAZA);
      }
    }
  }
  FieldScript_End();
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
  FieldScript_Start();
#if TBS_PATCH_1
  FieldScript_CallWithLastActiveObjectId(gSanctum_IvanObjectEventTemplate);
  WaitFrames(1);
#endif
  FieldScript_SetMessageId(MsgSanctumIsaacYourNewFriendsAdepts);
  FieldMessage_ShowAtObject(LOCALID_WEST_HEALER, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_ShowMessageAndWait(LOCALID_WEST_HEALER, 0);
  } else {
    gFieldWork->message++;
    Script_ShowTwoMessagesAndWait(SANCTUM_NEW_FRIENDS_SPEAKER, 16, 1, 24, 1, 3,
                                  7, 16, 1, 14, 0);
    FieldScript_ShowMessageAndWait(LOCALID_WEST_HEALER, 0);
  }
  FieldScript_End();
}

/// Vale Sanctum, once the party has left Vale: talking to LOCALID_EAST_HEALER,
/// who asks whether Isaac feels any change in his Psynergy.
static void EventScript_AskAboutPsynergyChanges(void) {

  FieldScript_Start();
  ObjectMotion_SetAngleToward(LOCALID_EAST_HEALER, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(10);
  FieldScript_SetMessageId(MsgSanctumDoFeelAnyChangeIn);
  FieldMessage_ShowAtObject(LOCALID_EAST_HEALER, 0);

  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 1) {
    (gFieldWork->message)++;
  }

  FieldScript_ShowMessageAndWait(LOCALID_EAST_HEALER, 0);
  FieldScript_End();
}

/// Sets up the place for its warp destination and plays its opening event.
/// Vault Sanctum places LOCALID_GOLD_STATUE once the thieves are jailed; Vale
/// Sanctum's quest visits (SANCTUM_FROM_VALE_ERUPTION, and
/// SANCTUM_FROM_QUEST_RESTART after a refusal, which sets
/// FLAG_SANCTUM_QUEST_RESTARTED) restore the floor and play the request, unless
/// the party is returning after a battle.
s32 Sanctum_Initialize(void) {
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  FieldScript_WaitFrames(1);
  switch (gSaveBlock.warpDest) {
  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_VAULT_REVISIT:
  case SANCTUM_FROM_LOG_ROOMS_A:
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      ObjectEvent_SetPosition(LOCALID_GOLD_STATUE, PIXELS(200), PIXELS(80));
    }
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
    break;
  case SANCTUM_FROM_VALE_ERUPTION:
    Sanctum_RestoreQuestFloor();
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Sanctum_RequestQuestAcceptance();
    }
    // Fall through.
  case SANCTUM_FROM_ALTIN:
  case SANCTUM_FROM_XIAN:
  case SANCTUM_FROM_BILIBIN:
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
    break;
  case SANCTUM_FROM_QUEST_RESTART:
    Sanctum_RestoreQuestFloor();
    GameFlag_Set(FLAG_SANCTUM_QUEST_RESTARTED);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Sanctum_RequestQuestAcceptance();
    }
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
    break;
  }
  return 0;
}

/// Vale Sanctum: heading for the door after refusing the quest (metatile
/// behavior 2, until FLAG_SANCTUM_WARNED_BEFORE_LEAVING is set, which it sets).
/// The Great Healer warns that the choice is final: "Once you step outside the
/// village, you cannot take back your decision."
static void EventScript_WarnBeforeLeaving(void) {
  FieldScript_Start();
  FieldCamera_SetSpeedParameters(PIXELS(1), PIXELS(1) / 8);
  ObjectEvent_PlaceCurrentWithinCameraBounds(LOCALID_GARET, 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAngleToward(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 4);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(MsgSanctumOnceStepOutsideVillageCannot);
  FieldScript_ShowMessageAndWait(LOCALID_HEALER, 0);
  GameFlag_Set(FLAG_SANCTUM_WARNED_BEFORE_LEAVING);
  FieldScript_End();
}

/// Vale Sanctum: leaving after refusing the quest (metatile behavior 1) ends
/// the game. The room turns sepia: `And so the world began drifting toward its
/// fated destruction...` Starting again from the Great Healer takes
/// SANCTUM_TO_QUEST_RESTART, back to SANCTUM_FROM_QUEST_RESTART; the other
/// choices take SANCTUM_TO_MAIN_MENU, to SceneId_MainMenu.
static void EventScript_LeaveAfterRefusing(void) {
  struct PaletteFade *paletteWork;

  FieldScript_Start();
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STILL);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STILL);
  Import_ObjectEventSetAnimation(LOCALID_DORA, 0);
  Import_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 0);
  Import_ObjectEventSetAnimation(LOCALID_HEALER, 0);
  Import_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 0);
  Import_ObjectEventSetAnimation(LOCALID_EAST_HEALER, 0);
  PaletteFade_SetTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  FieldScript_WaitFrames(180);
  paletteWork = gPaletteFadeWork;
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
  FieldScript_WaitFrames(1);
  FieldMessage_ShowCentered(MsgSanctumWorldBeganDrifting, 1, 0);
  PaletteFade_SetTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  FieldScript_WaitFrames(120);
  FieldScript_WaitFrames(60);
  if (Sanctum_ChooseRestartOption() == 0) {
    FieldScript_End();
    Import_ScriptRequestWarp(SANCTUM_TO_QUEST_RESTART);
  } else {
    FieldScript_End();
    Import_ScriptRequestWarp(SANCTUM_TO_MAIN_MENU);
  }
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 66 to 64 bytes in tbs-en.
#ifdef UBFIX
/// Keeps this object event at the party leader's drawing priority.
static void
Sanctum_MatchLeaderPriority(union ObjectEventOrEffect *fieldObject) {
#else
/// Keeps this object event at the party leader's drawing priority.
static s32 Sanctum_MatchLeaderPriority(union ObjectEventOrEffect *fieldObject) {
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
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_DORA, 3);
  Import_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 3);
  Import_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 3);
  ObjectMotion_SetAnimationAndWait(LOCALID_EAST_HEALER, 3);
}

/// Starts the same six's repeated motions and waits for the last of them.
static inline void Sanctum_StartAllRepeatedMotion(void) {
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 1);
  ObjectMotion_SetVariantCallback(LOCALID_DORA, 1);
  ObjectMotion_SetVariantCallback(LOCALID_GARETS_GRANDFATHER, 1);
  ObjectMotion_SetVariantCallback(LOCALID_WEST_HEALER, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_EAST_HEALER, 1);
}

/// Shows the same emote on the same six.
static inline void Sanctum_ShowAllEmotes(void) {
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
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
/// SANCTUM_TO_VALE_PLAZA; refusing leaves the room to gSanctum_QuestEvents.
static void Sanctum_RequestQuestAcceptance(void) {
  u32 effectIndex;

  FieldScript_Start();
  // A repeat visit skips the account of Sol Sanctum.
  if (GameFlag_Get(FLAG_SANCTUM_QUEST_RESTARTED) != 0) {
    ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_PARTY_LEADER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER,
                                LOCALID_PARTY_LEADER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_PARTY_LEADER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_PARTY_LEADER, 0);
    FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
    FieldCamera_WaitForArrival();
    gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gFieldWork->transitionFrames = 64;
    FieldScript_StartScreenTransition();
    FieldScript_WaitTransitionFrames();
    FieldScript_WaitFrames(120);
  } else {
    PaletteFade_ApplyTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
    PaletteFade_StartInterpolation(1);
    FieldScript_WaitFrames(1);
    FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
    FieldCamera_WaitForArrival();
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    FieldScript_StartScreenTransition();
    FieldScript_WaitTransitionFrames();
    // Reenact the journey before explaining the quest.
    Sanctum_ReenactSolSanctum();
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_StartInterpolation(60);
    FieldScript_WaitFrames(100);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
    FieldScript_WaitFrames(30);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER,
                                LOCALID_GARETS_GRANDFATHER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_GARETS_GRANDFATHER, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_SetBuiltinAction(LOCALID_HEALER, OBJ_EVENT_ACTION_IDLE);
    ObjectEvent_SetBuiltinAction(LOCALID_GARETS_GRANDFATHER,
                                 OBJ_EVENT_ACTION_IDLE);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_GARETS_GRANDFATHER, PIXELS(4) / 5,
                         PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_WEST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_EAST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
    FieldScript_WaitFrames(30);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_GRANDFATHER, 2);
    FieldScript_SetMessageId(MsgSanctumWeHadIdeaTrueSanctum);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    FieldScript_WaitFrames(10);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
    FieldScript_WaitFrames(30);
    ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 3);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_DORA, 0);
    FieldScript_WaitFrames(20);
    Script_OpenMessage(LOCALID_DORA, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      FieldScript_WaitFrames(20);
      ObjectMotion_SetAnimationAndWait(LOCALID_DORA, 3);
      FieldScript_WaitFrames(20);
      Script_ShowMessage(LOCALID_DORA, 0);
      gFieldWork->message++;
    } else {
      FieldScript_WaitFrames(20);
      ObjectMotion_SetAnimationAndWait(LOCALID_DORA, 4);
      FieldScript_WaitFrames(20);
      gFieldWork->message++;
      Script_ShowMessage(LOCALID_DORA, 0);
    }
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_WEST_HEALER, 1);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_WEST_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_WEST_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_WEST_HEALER, 0);
    FieldScript_WaitFrames(20);
    Script_OpenMessage(LOCALID_WEST_HEALER, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      FieldScript_WaitFrames(20);
      ObjectMotion_SetAnimationAndWait(LOCALID_WEST_HEALER, 3);
      FieldScript_WaitFrames(20);
      Script_ShowMessage(LOCALID_WEST_HEALER, 0);
      gFieldWork->message++;
    } else {
      FieldScript_WaitFrames(20);
      ObjectMotion_SetAnimationAndWait(LOCALID_WEST_HEALER, 4);
      FieldScript_WaitFrames(20);
      gFieldWork->message++;
      Script_ShowMessage(LOCALID_WEST_HEALER, 0);
    }
    FieldScript_WaitFrames(20);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
    Import_ObjectEventSetAnimation(LOCALID_DORA, 3);
    Import_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 3);
    Import_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 3);
    ObjectMotion_SetAnimationAndWait(LOCALID_EAST_HEALER, 3);
    FieldScript_WaitFrames(30);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    FieldScript_WaitFrames(60);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_WalkToAndWait(LOCALID_GARETS_GRANDFATHER, 0xe0, 0x78);
    FieldScript_WaitFrames(10);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 1);
    FieldScript_WaitFrames(10);
    Script_ShowMessage(LOCALID_DORA, 0);
    FieldScript_WaitFrames(30);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEALER, 3);
    ObjectEvent_ShowEmote(LOCALID_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          0);
    FieldScript_WaitFrames(60);
    ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_GARETS_GRANDFATHER, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
    FieldScript_WaitFrames(10);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceDirection(LOCALID_GARETS_GRANDFATHER,
                              FACING_SOUTH + FACING_STEP, 0);
    FieldScript_WaitFrames(60);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 3);
    FieldScript_WaitFrames(10);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceDirection(LOCALID_HEALER, FACING_SOUTHEAST + FACING_STEP,
                              0);
    FieldScript_WaitFrames(10);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceDirection(LOCALID_GARETS_GRANDFATHER,
                              FACING_SOUTH + FACING_STEP, 0);
    FieldScript_WaitFrames(60);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
    FieldScript_WaitFrames(50);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    FieldScript_WaitFrames(40);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(LOCALID_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    FieldScript_WaitFrames(60);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEALER, 1);
    FieldScript_WaitFrames(10);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(10);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_WEST_HEALER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_EAST_HEALER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    FieldScript_WaitFrames(60);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
    FieldScript_WaitFrames(10);
    Script_ShowMessage(LOCALID_GARET, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
    FieldScript_WaitFrames(10);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(10);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
    ObjectMotion_SetVariantCallback(LOCALID_GARET, 1);
    ObjectMotion_SetVariantCallback(LOCALID_DORA, 1);
    ObjectMotion_SetVariantCallback(LOCALID_WEST_HEALER, 1);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_EAST_HEALER, 1);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 4);
    FieldScript_WaitFrames(10);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(30);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER,
                                LOCALID_PARTY_LEADER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_PARTY_LEADER, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
    FieldScript_WaitFrames(60);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_GRANDFATHER, 2);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    FieldScript_WaitFrames(30);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_DORA, 0);
    FieldScript_WaitFrames(20);
    FieldScript_WaitFrames(40);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEALER, 2);
    Import_ObjectEventSetAnimation(LOCALID_HEALER, 0);
    ObjectEvent_SetSpriteColorById(LOCALID_HEALER, SPRITE_COLOR_BLINK);
    Sanctum_ShowHealerAfterimage();
    Script_PlaySong(SE_WISE_ONE_QUEST);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    FieldScript_WaitFrames(32);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    FieldScript_WaitFrames(16);
    for (effectIndex = 0; effectIndex < 6; effectIndex++) {
      Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
      FieldScript_WaitFrames(8);
    }
    FieldScript_WaitFrames(8);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    FieldScript_WaitFrames(32);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    FieldScript_WaitFrames(96);
    FieldScript_WaitFrames(32);
    ObjectEvent_SetSpriteColorById(LOCALID_HEALER, SPRITE_COLOR_NORMAL);
    FieldScript_WaitFrames(30);
    // Return to the gathering and finish the departure conversation.
    Sanctum_HideHealerAfterimage();
    Import_ObjectEventSetAnimation(LOCALID_HEALER, 1);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEALER, 2);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_SetAttachedEffect(LOCALID_GARETS_GRANDFATHER,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(
        LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(LOCALID_DORA, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(
        LOCALID_WEST_HEALER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(
        LOCALID_EAST_HEALER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(60);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_EAST_HEALER, 1);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_EAST_HEALER, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
    FieldScript_WaitFrames(60);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 4);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_HEALER, 0);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(30);
    ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
    FieldScript_WaitFrames(20);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
    ObjectEvent_LinkPair(LOCALID_WEST_HEALER, LOCALID_EAST_HEALER, 0);
    FieldScript_WaitFrames(20);
    Import_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 3);
    Import_ObjectEventSetAnimation(LOCALID_EAST_HEALER, 3);
    ObjectMotion_WaitForAnimationChange(LOCALID_EAST_HEALER);
    FieldScript_WaitFrames(30);
    ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
    ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_GARETS_GRANDFATHER, 0);
    FieldScript_WaitFrames(20);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
    Import_ObjectEventSetAnimation(LOCALID_DORA, 3);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
    Import_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 3);
    ObjectMotion_WaitForAnimationChange(LOCALID_GARETS_GRANDFATHER);
    FieldScript_WaitFrames(60);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEALER, 2);
    Import_ObjectEventSetAnimation(LOCALID_HEALER, 0);
    ObjectEvent_SetSpriteColorById(LOCALID_HEALER, SPRITE_COLOR_BLINK);
    Sanctum_ShowHealerAfterimage();
    Script_PlaySong(SE_WISE_ONE_QUEST);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    FieldScript_WaitFrames(32);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
    FieldScript_WaitFrames(16);
    ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
    ObjectMotion_SetVariantCallback(LOCALID_GARET, 1);
    ObjectMotion_SetVariantCallback(LOCALID_DORA, 1);
    ObjectMotion_SetVariantCallback(LOCALID_GARETS_GRANDFATHER, 1);
    ObjectMotion_SetVariantCallback(LOCALID_WEST_HEALER, 1);
    ObjectMotion_SetVariantCallback(LOCALID_EAST_HEALER, 1);
    for (effectIndex = 0; effectIndex < 6; effectIndex++) {
      Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
      FieldScript_WaitFrames(8);
    }
    FieldScript_WaitFrames(8);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    FieldScript_WaitFrames(32);
    Sanctum_SpawnOrbitingEffect(LOCALID_HEALER, 0x1200);
    FieldScript_WaitFrames(128);
    ObjectEvent_SetSpriteColorById(LOCALID_HEALER, SPRITE_COLOR_NORMAL);
    FieldScript_WaitFrames(30);
    Sanctum_HideHealerAfterimage();
    Import_ObjectEventSetAnimation(LOCALID_HEALER, 1);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEALER, 2);
    FieldScript_WaitFrames(30);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_LinkPair(LOCALID_WEST_HEALER, LOCALID_EAST_HEALER, 0);
    FieldScript_WaitFrames(40);
    ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_DORA, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
    ObjectMotion_SetVariantCallback(LOCALID_GARET, 1);
    ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
    ObjectMotion_SetVariantCallback(LOCALID_WEST_HEALER, 1);
    ObjectMotion_SetVariantCallback(LOCALID_EAST_HEALER, 1);
    ObjectEvent_LinkPair(LOCALID_WEST_HEALER, LOCALID_EAST_HEALER, 0);
    ObjectEvent_SetAttachedEffect(LOCALID_GARETS_GRANDFATHER,
                                  ATTACHED_EFFECT_IN_FRONT |
                                      ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(60);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_GARET, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(LOCALID_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    FieldScript_WaitFrames(60);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 4);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(20);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
    Import_ObjectEventSetAnimation(LOCALID_DORA, 3);
    Import_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 3);
    Import_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 3);
    Import_ObjectEventSetAnimation(LOCALID_EAST_HEALER, 3);
    ObjectMotion_WaitForAnimationChange(LOCALID_EAST_HEALER);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_GRANDFATHER, 1);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_GARETS_GRANDFATHER, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(50);
    ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(30);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    ObjectEvent_ShowEmote(LOCALID_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    FieldScript_WaitFrames(80);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 1);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_DORA, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 4);
    FieldScript_WaitFrames(20);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(LOCALID_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    FieldScript_WaitFrames(80);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEALER, 1);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_SetAttachedEffect(
        LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
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
    FieldScript_WaitFrames(60);
    ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 4);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEALER, 1);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 4);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(40);
    ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
    ObjectEvent_LinkPair(LOCALID_WEST_HEALER, LOCALID_EAST_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER,
                                LOCALID_PARTY_LEADER, 0);
    FieldScript_WaitFrames(60);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 1);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_DORA, 0);
    FieldScript_WaitFrames(60);
    ObjectEvent_WalkToAndWait(LOCALID_HEALER, 0xc8, 0x88);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER,
                                LOCALID_PARTY_LEADER, 0);
    ObjectEvent_LinkPair(LOCALID_HEALER, LOCALID_GARET, 0);
    FieldScript_WaitFrames(40);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
    FieldScript_WaitFrames(50);
    ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
    FieldScript_WaitFrames(40);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
    FieldScript_WaitFrames(50);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_HEALER, 1);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(30);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    FieldScript_WaitFrames(60);
    ObjectEvent_SetAttachedEffect(LOCALID_DORA, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 1);
    Script_ShowMessage(LOCALID_DORA, 0);
    FieldScript_WaitFrames(30);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
    FieldScript_WaitFrames(30);
    ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    ObjectMotion_SetVariantCallback(LOCALID_GARETS_GRANDFATHER, 1);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    FieldScript_WaitFrames(30);
    ObjectEvent_WalkToAndWait(LOCALID_HEALER, 0xa8, 0x78);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_HEALER, 0);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
    ObjectEvent_FaceDirection(LOCALID_HEALER, FACING_NORTH + FACING_STEP, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 4);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(30);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    FieldScript_WaitFrames(60);
    ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
    FieldScript_WaitFrames(10);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(10);
    ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_HEALER, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER,
                                LOCALID_PARTY_LEADER, 0);
    ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ONCE);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_GARET, 0);
    FieldScript_WaitFrames(30);
  }
  // Ask Isaac to commit, or leave the group waiting for his answer.
  FieldScript_SetMessageId(MsgSanctumIsaacWillAcceptResponsibilityFor);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_HEALER, 1);
  FieldScript_WaitFrames(20);
  Script_OpenMessage(LOCALID_HEALER, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    Sanctum_ReceiveWiseOnesMission();
    PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
    PaletteFade_StartInterpolation(120);
    FieldScript_WaitFrames(120);
    Script_PlaySong(MUS_INN);
    Sound_WaitForSong();
    GameFlag_Set(FLAG_SANCTUM_QUEST_ACCEPTED);
    Import_ScriptRequestWarp(SANCTUM_TO_VALE_PLAZA);
  } else {
    FieldScript_WaitFrames(30);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_GRANDFATHER, 1);
    ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 4);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_GetById(LOCALID_EAST_HEALER)->priorityFlags &=
        ~OBJ_EVENT_PRIORITY_AUTOMATIC;
#ifdef UBFIX
    ObjectEvent_GetById(LOCALID_EAST_HEALER)->update =
        Sanctum_MatchLeaderPriority;
#else
    ObjectEvent_GetById(LOCALID_EAST_HEALER)->update =
        (void (*)(union ObjectEventOrEffect *))Sanctum_MatchLeaderPriority;
#endif
  }
  FieldScript_End();
}

/// Vale Sanctum, once Isaac accepts: Garet and the Mayor react, the Wise One
/// appears before the Great Healer and accepts the two, and the healer relays
/// its guidance; they are to leave the next day.
static void Sanctum_ReceiveWiseOnesMission(void) {

  u32 sparkIndex;
  struct ObjectEvent *wiseOne;

  ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_GARETS_GRANDFATHER, PIXELS(4) / 5,
                       PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_WEST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_EAST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
  FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
  FieldCamera_WaitForArrival();
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_PARTY_LEADER,
                              0);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, 3);
  FieldScript_WaitFrames(60);
  Import_ObjectEventSetAnimation(LOCALID_HEALER, 3);
  Import_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 3);
  Import_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 3);
  ObjectMotion_SetAnimationAndWait(LOCALID_EAST_HEALER, 3);
  FieldScript_WaitFrames(50);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  // Garet and the Mayor react to Isaac accepting the burden.
  FieldScript_SetMessageId(MsgSanctumAcceptIsaacCantMean);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_GARET, 0);
  ObjectMotion_SetVariantCallback(LOCALID_GARETS_GRANDFATHER, 2);
  FieldEffect_SpawnLinked(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  FieldScript_WaitFrames(60);
  Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
  FieldScript_WaitFrames(20);
  ObjectMotion_ArmCallback(LOCALID_HEALER, FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_HEALER, 0);
  FieldScript_WaitFrames(80);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  PaletteFade_SetTarget(PALETTE_TINT_PALE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  FieldScript_WaitFrames(40);
  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(104), 1);
  FieldScript_WaitFrames(120);
  Script_PlaySong(MUS_FOREST);
  Script_PlaySong(SE_MATERIALIZE);
  // The Wise One enters while the healer's afterimages appear.
  ObjectEvent_SetPosition(LOCALID_WISE_ONE, PIXELS(200), PIXELS(8));
  ObjectEvent_SetSpeed(LOCALID_WISE_ONE, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectEvent_MoveToAndWait(LOCALID_WISE_ONE, 200, 72);
  Script_PlaySong(SE_STOP_ACTION_SE);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_HEALER, 2);
  Import_ObjectEventSetAnimation(LOCALID_HEALER, 0);
  Sanctum_ShowHealerAfterimage();
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_WISE_ONE, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_WISE_ONE, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_WISE_ONE, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_WISE_ONE, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_WISE_ONE, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_WISE_ONE, 0);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  ObjectMotion_SetVariantCallback(LOCALID_DORA, 2);
  ObjectMotion_SetVariantCallback(LOCALID_GARETS_GRANDFATHER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_WEST_HEALER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_EAST_HEALER, 2);
  FieldScript_WaitFrames(40);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(40);
  Script_ShowMessage(LOCALID_WISE_ONE, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_ConfigureSpriteColor(LOCALID_WISE_ONE, SPRITE_COLOR_BLINK);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  Script_PlaySong(SE_MATERIALIZE);
  // Shrink the Wise One amid sparks before restoring the room.
  for (sparkIndex = 0; sparkIndex < 32; sparkIndex++) {
    Sanctum_SpawnObjectEventSpark(LOCALID_WISE_ONE);
    FieldScript_WaitFrames(4);
    wiseOne = ObjectEvent_GetById(LOCALID_WISE_ONE);
    wiseOne->scaleX += -Q16_ONE / 100;
    wiseOne = ObjectEvent_GetById(LOCALID_WISE_ONE);
    wiseOne->scaleY += -Q16_ONE / 100;
  }
  Script_PlaySong(SE_STOP_ACTION_SE);
  ObjectEvent_SetSpriteColorById(LOCALID_WISE_ONE, SPRITE_COLOR_NORMAL);
  ObjectEvent_SetPosition(LOCALID_WISE_ONE, 0, 0);
  FieldScript_WaitFrames(40);
  FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
  FieldCamera_WaitForArrival();
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  FieldScript_WaitFrames(120);
  Sanctum_HideHealerAfterimage();
  Import_ObjectEventSetAnimation(LOCALID_HEALER, 1);
  Script_PlaySong(MUS_TOWN_IN_TROUBLE);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_HEALER, 2);
  FieldScript_WaitFrames(20);
  Sanctum_StartAllRepeatedMotion();
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_DORA, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_HEALER, 0);
  FieldScript_WaitFrames(20);
  Sanctum_ShowAllEmotes();
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_HEALER, 0);
  FieldScript_WaitFrames(20);
  Sanctum_NodAll();
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
  FieldScript_WaitFrames(180);
  ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
  ObjectEvent_LinkPair(LOCALID_WEST_HEALER, LOCALID_EAST_HEALER, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  Sanctum_StartAllRepeatedMotion();
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_DORA, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(80);
  ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_HEALER, 1);
  FieldEffect_SpawnLinked(LOCALID_HEALER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  FieldScript_WaitFrames(20);
  Sanctum_StartAllRepeatedMotion();
  FieldScript_WaitFrames(40);
  Script_ShowMessage(LOCALID_HEALER, 0);
  FieldScript_WaitFrames(20);
  Sanctum_NodAll();
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_HEALER, 0);
  FieldScript_WaitFrames(30);
  Sanctum_NodAll();
  FieldScript_WaitFrames(120);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_HEALER, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(LOCALID_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_GRANDFATHER, 1);
  Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 4);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_HEALER, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARETS_GRANDFATHER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                        0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceObjectEvent(LOCALID_HEALER, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_HEALER, 3);
  Script_ShowMessage(LOCALID_HEALER, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_WalkToAndWait(LOCALID_HEALER, 168, 176);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_HEALER, 0);
  ObjectEvent_WalkToAndWait(LOCALID_HEALER, 200, 200);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_HEALER, 0);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_WEST_HEALER, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_EAST_HEALER, FACING_WEST, 0);
  ObjectEvent_WalkTo(LOCALID_HEALER, 200, 0x110);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_WEST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_EAST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_HEALER);
  ObjectEvent_SetPosition(LOCALID_HEALER, 0, 0);
  FieldScript_WaitFrames(60);
  Sanctum_ShowAllEmotes();
  FieldScript_WaitFrames(60);
  FieldScript_WaitFrames(120);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_WEST_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_EAST_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_WEST_HEALER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARETS_GRANDFATHER, LOCALID_WEST_HEALER,
                              0);
  FieldScript_WaitFrames(120);
  ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  FieldScript_WaitFrames(60);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WEST_HEALER, 2);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_WEST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(80);
  ObjectEvent_FaceObjectEvent(LOCALID_WEST_HEALER, LOCALID_EAST_HEALER, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WEST_HEALER, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_WEST_HEALER, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_EAST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(20);
  FieldEffect_SpawnLinked(LOCALID_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_WEST_HEALER, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_EAST_HEALER, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_EAST_HEALER, 2);
  FieldScript_WaitFrames(30);
  ObjectMotion_ArmCallback(LOCALID_EAST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_EAST_HEALER, 0);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 3);
  ObjectMotion_SetAnimationAndWait(LOCALID_EAST_HEALER, 3);
  FieldScript_WaitFrames(20);
  ObjectEvent_WalkTo(LOCALID_WEST_HEALER, 200, 0x110);
  ObjectEvent_WalkToAndWait(LOCALID_EAST_HEALER, 200, 0x110);
  ObjectEvent_SetPosition(LOCALID_WEST_HEALER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_EAST_HEALER, 0, 0);
  FieldEffect_SpawnLinked(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  FieldScript_WaitFrames(60);
  ObjectEvent_WalkToAndWait(LOCALID_GARETS_GRANDFATHER, 200, 136);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARETS_GRANDFATHER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_GARETS_GRANDFATHER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 4);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(30);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_DORA, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_WalkToAndWait(LOCALID_DORA, 168, 168);
  ObjectEvent_FaceObjectEvent(LOCALID_DORA, LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_DORA, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, 4);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_DORA, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 4);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_GARETS_GRANDFATHER,
                              0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_DORA, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_DORA, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  FieldEffect_SpawnLinked(LOCALID_GARETS_GRANDFATHER,
                          EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  Script_ShowMessage(LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEffect_SpawnLinked(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, 3);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_DORA, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  FieldScript_WaitFrames(30);
  Import_ObjectEventSetAnimation(LOCALID_DORA, 3);
  FieldScript_WaitFrames(30);
  Import_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 3);
}

/// Copies the healer into the afterimage object event.
static void Sanctum_ShowHealerAfterimage(void) {
  u32 entryIndex;
  struct ObjectEvent *afterimage;
  struct ObjectEvent *objectEvent;
  struct Sprite *sprite;

  objectEvent = ObjectEvent_GetById(LOCALID_HEALER);
  if (objectEvent != 0) {
    Import_ObjectEventSetPosition(LOCALID_HEALER_AFTERIMAGE,
                                  objectEvent->x.fixed, objectEvent->z.fixed);
  }
  Import_ObjectEventSetAnimation(LOCALID_HEALER_AFTERIMAGE, 0);
  afterimage = ObjectEvent_GetById(LOCALID_HEALER_AFTERIMAGE);
  objectEvent = ObjectEvent_GetById(LOCALID_HEALER);
  afterimage->facing = objectEvent->facing;
  objectEvent = ObjectEvent_GetById(LOCALID_HEALER_AFTERIMAGE);
  objectEvent->update = Sanctum_UpdateHealerAfterimage;
  objectEvent = ObjectEvent_GetById(LOCALID_HEALER_AFTERIMAGE);
  sprite = objectEvent->sprite;
  {
    for (entryIndex = 0; entryIndex < sprite->count; entryIndex++) {
      struct SpriteAnim *entry = sprite->entries[entryIndex];

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
  Import_ObjectEventSetPosition(LOCALID_HEALER_AFTERIMAGE, 0, 0);
}

/// Cycles the afterimage through four offsets from the healer.
static void
Sanctum_UpdateHealerAfterimage(union ObjectEventOrEffect *fieldObject) {
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

/// Advances an effect around its linked object event on an elliptical path.
static void
Sanctum_UpdateOrbitingEffect(union ObjectEventOrEffect *fieldObject) {
  struct ObjectEvent *effect = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *anchor = effect->linkedObject;
  u16 *angle = &effect->timer;
  u16 orbitAngle = *angle;
  s32 x;
  s32 z;
  s32 currentX;

  x = anchor->x.fixed + Trig_Cos(orbitAngle) * 14;
  effect->x.fixed = x;

  z = anchor->z.fixed + Trig_Sin(orbitAngle) * 10;
  currentX = effect->x.fixed;
  effect->z.fixed = z;
  effect->targetZ = z;
  effect->targetX = currentX;

  *angle = (*angle + effect->work);
}

/// Creates an orbiting effect with the requested angular step.
static void Sanctum_SpawnOrbitingEffect(s32 localId, s32 angleStep) {
  s32 orbitStep;
  struct ObjectEvent *center;
  struct ObjectEvent *effect;
  struct Sprite *sprite;
  struct Sprite *sourceSprite;

  orbitStep = angleStep;
  center = ObjectEvent_GetById(localId);
  if (center != 0) {
    effect = ObjectEvent_Create(SPRITE_SPARKLE, center->x.fixed,
                                center->y.fixed + PIXELS(45), center->z.fixed);
    if (effect != 0) {
      sprite = effect->sprite;
      ObjectEvent_SetScript(effect, gSanctum_OwnerEffectScript);
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
      sourceSprite = center->sprite;
      effect->linkedObject = center;
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
  struct Window *window;
  s32 text;
  s32 choice;

  Import_ObjectEventSetPosition(LOCALID_HEALER, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_WEST_HEALER, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_EAST_HEALER, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_GARET, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_DORA, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_GARETS_GRANDFATHER, 0, 0);
  Import_ObjectEventSetPosition(LOCALID_PARTY_LEADER, 0, 0);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(1);
  FieldScript_WaitFrames(1);
  window = Window_Create(QUESTION_X, 7, QUESTION_WIDTH, 5, 1);
  text = MsgSanctumGreatHealer;
  Text_DrawMessage(text, window, 16, 0);
  if (Save_CountRecordsExcludingFlagged(1) == 0) {
    Text_DrawMessage(text + 2, window, 16, 16);
  } else {
    Text_DrawMessage(text + 1, window, 16, 16);
  }
  FieldMenuArrow_Create(&cursor, &slot);
  FieldMenuArrow_SetPosition(&cursor, 72, 60);
  choice = 0;
  while ((gKeysPressed & KEY_A) == 0) {
    s32 x;

    if (gKeysRepeat & KEYS_VERTICAL) {
      choice ^= 1;
    }
    x = gSanctum_CursorBob[(gVBlankCount >> 1) & 15] + CURSOR_X;
    FieldMenuArrow_SetPosition(&cursor, x, (choice << 4) + 60);
    FieldScript_WaitFrames(1);
  }
  MenuArrow_Release(slot);
  Window_Close(window, 1);
  return choice;
}

/// Vale Sanctum, the quest's account: Isaac and Garet act out, in sepia, what
/// happened in Sol Sanctum.
static void Sanctum_ReenactSolSanctum(void) {

  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARETS_GRANDFATHER, 2);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 3);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(15);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectMotion_ResetAndSetPositionInMode2(LOCALID_PARTY_LEADER, 184, 168);
  ObjectEvent_GetById(LOCALID_GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectMotion_SetPositionAndReset(LOCALID_GARET, 200, 168);
  FieldScript_WaitFrames(1);
  ObjectEvent_GetById(LOCALID_GARET)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  ObjectEvent_GetById(LOCALID_GARET)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectMotion_Launch(LOCALID_GARET, 2, 0);
  FieldScript_WaitFrames(15);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_HEALER, 0);
  FieldScript_WaitFrames(5);
  ObjectMotion_Launch(LOCALID_GARET, 2, 0);
  FieldScript_WaitFrames(25);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(5);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(5);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(15);
  Import_ObjectEventSetAnimation(LOCALID_DORA, 3);
  Import_ObjectEventSetAnimation(LOCALID_GARETS_GRANDFATHER, 3);
  Import_ObjectEventSetAnimation(LOCALID_HEALER, 3);
  Import_ObjectEventSetAnimation(LOCALID_WEST_HEALER, 3);
  ObjectMotion_SetAnimationAndWait(LOCALID_EAST_HEALER, 3);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_GARETS_GRANDFATHER,
                              0);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_DORA, 0);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  FieldScript_WaitFrames(15);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(10);
  // Illustrate the account with the floating item icon.
  Sanctum_RaiseItemIcon(ITEM_MARS_STAR, PIXELS(184), PIXELS(27), PIXELS(168));
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(10);
  ObjectMotion_Launch(LOCALID_GARET, 4, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(15);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_Launch(LOCALID_GARET, 4, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(15);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectEvent_GetById(LOCALID_HEALER)->timer = GARET;
#ifdef UBFIX
  ObjectEvent_GetById(LOCALID_HEALER)->update =
      Sanctum_FaceObjectEventNamedByTimer;
#else
  ObjectEvent_GetById(LOCALID_HEALER)->update = (void (*)(
      union ObjectEventOrEffect *))Sanctum_FaceObjectEventNamedByTimer;
#endif
  ObjectEvent_GetById(LOCALID_GARETS_GRANDFATHER)->timer = GARET;
#ifdef UBFIX
  ObjectEvent_GetById(LOCALID_GARETS_GRANDFATHER)->update =
      Sanctum_FaceObjectEventNamedByTimer;
#else
  ObjectEvent_GetById(LOCALID_GARETS_GRANDFATHER)->update = (void (*)(
      union ObjectEventOrEffect *))Sanctum_FaceObjectEventNamedByTimer;
#endif
  ObjectMotion_SetPositionAndReset(LOCALID_GARET, 196, 180);
  ObjectMotion_SetPositionAndReset(LOCALID_GARET, 184, 184);
  ObjectMotion_SetPositionAndReset(LOCALID_GARET, 180, 180);
  ObjectMotion_SetPositionAndReset(LOCALID_GARET, 168, 168);
  ObjectMotion_SetPositionAndReset(LOCALID_GARET, 180, 156);
  ObjectMotion_ResetAndSetPositionInMode2(LOCALID_GARET, 200, 104);
  ObjectMotion_SetPositionAndReset(LOCALID_PARTY_LEADER, 192, 168);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 0);
  FieldScript_WaitFrames(15);
  ObjectEvent_GetById(LOCALID_GARETS_GRANDFATHER)->update = NULL;
  ObjectEvent_GetById(LOCALID_HEALER)->update = NULL;
  ObjectMotion_SetVariantCallback(LOCALID_HEALER, 2);
  FieldEffect_SpawnLinked(LOCALID_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          0);
  FieldScript_WaitFrames(60);
  Import_ObjectEventSetAnimation(LOCALID_HEALER, 0);
  FieldEffect_SpawnLinked(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          0);
  FieldScript_WaitFrames(60);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_DORA, 3);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(10);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 2, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 2, 0);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(15);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 3);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetPositionAndReset(LOCALID_GARET, 208, 168);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_SHAKE_HEAD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
  FieldScript_WaitFrames(10);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_EAST, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 3);
  FieldScript_WaitFrames(10);
  ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_EAST, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(10);
  // Repeat the synchronized eight-point paths around the room.
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                              gSanctum_LeaderCircleScript);
  ObjectEvent_SetActionScript(LOCALID_GARET, gSanctum_GaretCircleScript);
  ObjectEvent_RefreshSelectorById(LOCALID_PARTY_LEADER);
  ObjectEvent_RefreshSelectorById(LOCALID_GARET);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(3) / 2, PIXELS(3) / 4);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 6, 0);
  ObjectMotion_Launch(LOCALID_GARET, 6, 0);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_WEST_HEALER, 0);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_HEALER, 0);
  FieldScript_WaitFrames(1);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_GARETS_GRANDFATHER,
                              0);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_DORA, 0);
  FieldScript_WaitFrames(1);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_HEALER, 0);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_WEST_HEALER, 0);
  FieldScript_WaitFrames(1);
  ObjectMotion_ResetAndSetPositionInMode2(LOCALID_PARTY_LEADER, 192, 168);
  ObjectMotion_SetPositionAndReset(LOCALID_GARET, 208, 168);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_NORTH + FACING_STEP, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH + FACING_STEP,
                            0);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_DORA, 0);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_GARETS_GRANDFATHER, 0);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_DORA, 3);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARETS_GRANDFATHER, 3);
  FieldScript_WaitFrames(30);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, LOCALID_GARET, 0);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_EAST, 0);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  FieldScript_WaitFrames(60);
}

/// Shows an item icon rising from the supplied fixed-point position.
static void Sanctum_RaiseItemIcon(s32 item, s32 fixedX, s32 fixedY,
                                  s32 fixedZ) {
  struct ObjectEvent *icon;
  struct Sprite *sprite;
  u8 *transferBuffer;
  u32 frame;
  u8 zero;

  icon = ObjectEvent_Create(SPRITE_BLANK, fixedX, fixedY, fixedZ);
  zero = 0;
  if (icon != 0) {
    ObjectEvent_SetScript(icon, gSanctum_ItemIconGrowScript);
    sprite = icon->sprite;
    sprite->flags = zero;
    sprite->count = zero;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    icon->velocityY = PIXELS(2);
    icon->gravity = PIXELS(1) / 4;
    transferBuffer = AllocHeapSlot(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    Icon_DrawItem(item);
    VramCache_Load(sprite->slot, 128, transferBuffer + 0x400);
    FreeHeapSlot(HEAP_SLOT_GLYPH_TRANSFER);
    for (frame = 0; frame < 60; frame++) {
      if ((u32)(icon->velocityY + 255) <= 0x1fe)
        icon->motionFlags = 0;
      WaitFrames(1);
    }
    ObjectEvent_SetScript(icon, gSanctum_ItemIconEndScript);
  }
}

/// Moves a spark upward, cycles its scale, and expires its lifetime.
static void Sanctum_UpdateRisingSpark(union ObjectEventOrEffect *fieldObject) {
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
    ObjectEvent_SetScript(spark, gSanctum_SparkEndScript);
}

/// Creates a short-lived spark beside the specified object event.
static void Sanctum_SpawnObjectEventSpark(s32 id) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *spark;
  struct Sprite *sprite;
  s32 x;
  s32 randomOffset;

  objectEvent = ObjectEvent_GetById(id);
  if (objectEvent == 0)
    return;
  randomOffset = Math_RemainderUnsigned(Import_RandomNext(), 20);
  x = objectEvent->x.fixed;
  x += randomOffset << 16;
  x += -PIXELS(10);
  spark = ObjectEvent_Create(
      SPRITE_GLINT, x,
      objectEvent->y.fixed + ((Import_RandomNext() & 15) << 16) + -PIXELS(8),
      objectEvent->z.fixed);
  if (spark == 0)
    return;
  sprite = spark->sprite;
  spark->motionFlags = 0;
  spark->timer = Math_RemainderUnsigned(Import_RandomNext(), 10) + 5;
  spark->work = Math_RemainderUnsigned(Import_RandomNext(), 60) + 30;
  spark->update = Sanctum_UpdateRisingSpark;
  sprite->flags = 0;
  sprite->image.priority = objectEvent->sprite->image.priority;
}

/// Copies two blocks of map grid cells for the quest's visits, the metatiles of
/// one and the metatile behaviors of the other.
static void Sanctum_RestoreQuestFloor(void) {
  {
    MapGrid_CopyCellsTo(0, 64, 11, 68, 3, 2);
  }
  {
    MapGrid_CopyCellAttributes(11, 10, 3, 2, 11, 8);
  }
  WaitFrames(1);
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
    Sanctum_Run(LOCALID_HEALER);
    return;
  }

  FieldScript_Start();
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0)
    FieldScript_SetMessageId(MsgSanctumPathSolSanctumStillClosed);
  else if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0)
    FieldScript_SetMessageId(MsgSanctumMyControlOverPsynergyHas);
  else
    FieldScript_SetMessageId(MsgSanctumWieldersPsynergyCalledAdeptsAdepts);
  FieldScript_ShowMessageAndWait(LOCALID_HEALER, 0);
  FieldScript_End();
}

/// Vault Sanctum: talking to the healer. From the front the healer opens the
/// Sanctum's services; otherwise the healer speaks of the gold statue or, on
/// the later visit, of the earthquakes. Its case for the quest's warp
/// destinations (SANCTUM_FROM_VALE_ERUPTION, SANCTUM_FROM_QUEST_RESTART and
/// SANCTUM_QUEST_UNUSED) passes to EventScript_ReconsiderQuest, though their
/// map events (gSanctum_QuestEvents) call that directly.
static void EventScript_TalkToHealerOnVisit(void) {
  s32 warpDest;

  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Sanctum_Run(LOCALID_HEALER);
    return;
  }

  FieldScript_Start();

  warpDest = gSaveBlock.warpDest;

  switch (warpDest) {
  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_LOG_ROOMS_A:
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      FieldScript_SetMessageId(MsgSanctumWasHandFateReturnedGold);
    } else {
      FieldScript_SetMessageId(MsgSanctumWeWillHelpAnytimeAs);
    }
    break;
  case SANCTUM_FROM_VAULT_REVISIT:
    FieldScript_SetMessageId(MsgSanctumMayWrongButLatelyThere);
    break;
  case SANCTUM_FROM_VALE_ERUPTION:
  case SANCTUM_FROM_QUEST_RESTART:
  case SANCTUM_QUEST_UNUSED:
    FieldScript_End();
    EventScript_ReconsiderQuest();
    return;
  default:
    break;
  }

  FieldScript_ShowMessageAndWait(LOCALID_HEALER, 0);
  FieldScript_End();
}

/// Bilibin Sanctum: talking to the healer, about Kolima's curse, lifted or not;
/// from the front, the Sanctum's services.
static void EventScript_TalkAboutCurse(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Sanctum_Run(LOCALID_HEALER);
    return;
  }

  FieldScript_Start();
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0)
    FieldScript_SetMessageId(MsgSanctumCurseMayOverButWe);
  else
    FieldScript_SetMessageId(MsgSanctumAmStartingFeelOnlyBeginning);
  FieldScript_ShowMessageAndWait(LOCALID_HEALER, 0);
  FieldScript_End();
}

/// Altin Sanctum: talking to the healer, about the beasts in the mine, or the
/// party's saving Altin from them; from the front, the Sanctum's services.
static void EventScript_TalkAboutAltinMonsters(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Sanctum_Run(LOCALID_HEALER);
    return;
  }

  FieldScript_Start();
  FieldScript_SetMessageId(MsgSanctumWasAfterEerieNightWhen);
  if (GameFlag_Get(FLAG_HYDROS_STATUES_DEFEATED) != 0)
    FieldScript_SetMessageId(MsgSanctumSavedAltinFromMonstersClearly);
  FieldScript_ShowMessageAndWait(LOCALID_HEALER, 0);
  FieldScript_End();
}

/// Xian Sanctum: talking to the healer, who came to Xian from very distant
/// lands; from the front, the Sanctum's services.
static void EventScript_TalkAboutJourneyFromXian(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Sanctum_Run(LOCALID_HEALER);
    return;
  }

  FieldScript_Start();
  FieldScript_SetMessageId(MsgSanctumCameXianFromVeryDistant);
  FieldScript_ShowMessageAndWait(LOCALID_HEALER, 0);
  FieldScript_End();
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
    { SANCTUM_FROM_LOG_ROOMS_A, CONDITION_ALWAYS, 199, 0, 164, FACING_SOUTH, 0, 64, 8, 336, 272, 0 },
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
    MAP_WARP(SANCTUM_TO_VALE_PLAZA, ValePlaza, VALE_PLAZA_FROM_SANCTUM),
    MAP_WARP(SANCTUM_TO_ALTIN, Altin, ALTIN_FROM_SANCTUM),
    MAP_WARP(SANCTUM_TO_BILIBIN, Bilibin, BILIBIN_FROM_SANCTUM),
    MAP_WARP(SANCTUM_TO_XIAN, Xian, XIAN_FROM_SANCTUM_WEST),
    MAP_WARP(SANCTUM_TO_MAIN_MENU, MainMenu, MAIN_MENU_FROM_SANCTUM),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gSanctum_NoObjectEventTemplates[] = {
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSanctum_ValeObjectEventTemplates[] = {
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { LOCALID_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(130), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

#if TBS_PATCH_1
struct ObjectEventTemplate gSanctum_IvanObjectEventTemplate[] = {
    { LOCALID_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};
#endif

struct ObjectEventTemplate gSanctum_VaultBilibinObjectEventTemplates[] = {
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(130), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(200), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { 0xdd, CONDITION_ALWAYS, 7,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSanctum_QuestObjectEventTemplates[] = {
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(208), 0, PIXELS(168), FACING_NORTH + FACING_STEP, 3, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(224), 0, PIXELS(216), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(246), 0, PIXELS(198), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(208), FACING_NORTH + FACING_STEP, 3, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, 3, 0 },
    { 0xfc, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSanctum_AltinXianObjectEventTemplates[] = {
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(132), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
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
    { MAP_EVENT_TALK, LOCALID_GARET, CONDITION_ALWAYS, MSGNO(MsgSanctumItIsATerribleBurdenTo) },
    { MAP_EVENT_TALK, LOCALID_DORA, CONDITION_ALWAYS, MSGNO(MsgSanctumMotherWouldNeverDoubtYourCourage) },
    { MAP_EVENT_TALK, LOCALID_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgSanctumIDontBlameYouICouldnt) },
    { MAP_EVENT_TALK, LOCALID_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWasItRightToRefuseWhen) },
    { MAP_EVENT_TALK, LOCALID_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItsClearThatTheGreatHealer) },
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveAfterRefusing) },
    { MAP_EVENT_COORD, 2, FLAG_SANCTUM_WARNED_BEFORE_LEAVING, EVENT_SCRIPT(EventScript_WarnBeforeLeaving) },
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
