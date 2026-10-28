/// @file
/// The Sanctum of Vale and of four towns on the party's way, which holds six
/// places in one scene (SceneId_Sanctum): the entrance the party arrives
/// at decides which, and Sanctum_GetActorPlacements and Sanctum_GetTriggers
/// choose that place's tables by it. Spoken to from the front, each place's
/// healer opens the Sanctum's services (Sanctum_Run); from any other side the
/// healer talks. Each place's door leads back out to its own town.
///
/// - Vale Sanctum (SANCTUM_FROM_VALE, SANCTUM_FROM_VALE_ERUPTION,
///   SANCTUM_VALE_UNUSED and SANCTUM_FROM_QUEST_RESTART), the Great Healer's,
///   entered from Vale at SANCTUM_FROM_VALE. Before the trip up Mt. Aleph, the
///   Great Healer and two other healers (ACTOR_WEST_HEALER and
///   ACTOR_EAST_HEALER) speak of Psynergy. After the eruption the Great
///   Healer brings Isaac and Garet in from Vale (vale_eruption.c) to
///   SANCTUM_FROM_VALE_ERUPTION, where Dora, the Mayor and the healers hear
///   what happened on the mountain, acted out in sepia
///   (Sanctum_ReenactSolSanctum): the palette fade turns every background and
///   sprite palette to PALETTE_TINT_SEPIA for the account and fades back to
///   PALETTE_TINT_NONE over 60 frames after it. The Great Healer asks Isaac to
///   take on the Wise One's quest (Sanctum_RequestQuestAcceptance). Each time
///   his Psynergy shows, ACTOR_HEALER_AFTERIMAGE, a copy of him in
///   SPRITE_COLOR_RED_TINT, jumps round him by a few pixels a frame
///   (Sanctum_UpdateHealerAfterimage) while effects circle him
///   (Sanctum_SpawnOrbitingEffect). Accepting plays
///   Sanctum_ReceiveWiseOnesMission and sets FLAG_SANCTUM_QUEST_ACCEPTED, and
///   SANCTUM_TO_VALE_PLAZA takes the two to SceneId_ValePlaza (vale_plaza.c).
///   Refusing leaves the room to gSanctum_QuestTriggers: the Great Healer asks
///   again when spoken to (Sanctum_ReconsiderQuest), the door draws a
///   warning (FLAG_SANCTUM_WARNED_BEFORE_LEAVING), and leaving anyway ends the
///   game (Sanctum_LeaveAfterRefusing): `And so the world began drifting
///   toward its fated destruction...` From there the player can start again
///   from the Great Healer, by SANCTUM_TO_QUEST_RESTART to
///   SANCTUM_FROM_QUEST_RESTART, which skips the account
///   (FLAG_SANCTUM_QUEST_RESTARTED), or leave by SANCTUM_TO_FILE_SCREEN to
///   SceneId_FileScreen. After the party has left Vale (FLAG_VALE_PARTY_LEFT),
///   and again after it has reached Kalay (FLAG_KALAY_REACHED), the three talk
///   of what the eruption and the Wise One changed.
/// - Vault Sanctum (SANCTUM_FROM_VAULT and SANCTUM_FROM_LOG_ROOMS_A). Its
///   healer and the other person there (ACTOR_SANCTUM_MEMBER) miss a gold
///   statue stolen during the eruption, until the party has the thieves jailed
///   and the statue is returned (FLAG_VAULT_THIEVES_JAILED); then the statue
///   (ACTOR_GOLD_STATUE), placed off the map until then, stands behind the
///   healer.
/// - Bilibin Sanctum (SANCTUM_FROM_BILIBIN), whose healer and the other person
///   there (ACTOR_SANCTUM_MEMBER) talk of Kolima's curse, before and after it
///   is lifted (FLAG_KOLIMA_CURSE_LIFTED).
/// - Xian Sanctum (SANCTUM_FROM_XIAN), whose healer came to Xian from very
///   distant lands.
/// - Altin Sanctum (SANCTUM_FROM_ALTIN), whose healer tells of the
///   water-spewing beasts in the mine until the party has saved Altin from them
///   (FLAG_HYDROS_STATUES_DEFEATED).
/// - Vault Sanctum again (SANCTUM_FROM_VAULT_REVISIT), when the party passes
///   through Vault later (vault_revisit.c): the statue has been polished, and
///   the healer worries about earthquakes on Mt. Aleph.
/// - Sanctum, the name the game gives every other entrance, which
///   leaves only SANCTUM_UNUSED; no exit leads there, and it places nobody.

#include "speaker.h"
#include "flags.h"
#include "adventure.h"
#include "unit/scene/sanctum.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
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
#include "event_layer.h"
#include "animspr.h"
#include "field_effect.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "io_reg.h"
#include "save.h"
#include "event_inline.h"
#include "window.h"
#include "uiflow.h"
#include "resource.h"
#include "mem.h"
#include "actor_builtin_scripts.h"
#include "ability_ids.h"
#include "actor_script.h"
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

// Scene flags, in Vale Sanctum: 00 is set once the Great Healer has
// warned Isaac that leaving the village cannot be undone; 01 marks the visit
// after a refusal (SANCTUM_FROM_QUEST_RESTART), which skips the account of Sol
// Sanctum.

/// The placed actors, numbering the rows of the table
/// Sanctum_GetActorPlacements returns for the entrance; the
/// party's rows take their own ids. Every place's first row is its healer.
enum SanctumActorId {
  /// The place's healer: the Great Healer in Vale Sanctum, its own healer
  /// elsewhere, a man with a white beard in a tall hat and a long purple and
  /// blue robe.
  /// ★ Named from the game: "What is it, Great Healer?"
  /// (MsgSanctumWeHadIdeaTrueSanctum)
  ACTOR_HEALER = ACTOR_FIRST_PLACED,
  // Vale Sanctum (gSanctum_ValeActorPlacements, and
  // gSanctum_QuestActorPlacements at the
  // quest).
  /// The western of Vale Sanctum's two other healers, men with long brown hair
  /// and beards in long robes, as Mind Read hears a villager in
  /// cottage_rooms_a.c call the Sanctum's people: "The healers
  /// are supposed to guard Sol Sanctum." Before the quest: "Our Psynergy was
  /// given to us by the sacred power of Mt. Aleph."
  /// ★ Named from the game: "The healers are supposed to guard"
  /// (MsgValeTheHealersAreSupposedToGuard)
  ACTOR_WEST_HEALER,
  /// The eastern one. "Psynergy is a power that should only be used for
  /// good... Never for evil!"
  /// ★ Named from the game: "should only be used for"
  /// (MsgSanctumPsynergyIsAPowerThatShould)
  ACTOR_EAST_HEALER,
  // The quest (gSanctum_QuestActorPlacements).
  /// Isaac's mother, a woman with fair hair tied up.
  /// ★ Named from the game: "Please, call me Dora."
  /// (MsgValeAsStubbornAsYourFather)
  ACTOR_DORA,
  /// The Mayor, Garet's grandfather, an old man with white hair and a cane, as
  /// the Great Healer calls him: "That is all, Mayor."
  /// ★ Named from the game: "That is all, Mayor"
  /// (MsgSanctumAcceptIsaacCantMean)
  ACTOR_GARETS_GRANDFATHER,
  /// Appears to the Great Healer.
  /// ★ Named from the game: "It must be the Wise One, guardian of the Elemental
  /// Stars." (MsgSanctumWeHadIdeaTrueSanctum)
  ACTOR_WISE_ONE,
  /// A copy of the Great Healer for its afterimages.
  /// ★ Named from what the player sees: a red-tinted copy of the Great Healer
  /// beside him while his Psynergy shows, healer-afterimages.gif (its render)
  ACTOR_HEALER_AFTERIMAGE,
  // Vault Sanctum and Bilibin Sanctum
  // (gSanctum_VaultBilibinActorPlacements).
  /// The other person of the Sanctum, in the western healer's sprite (a man
  /// with long brown hair and a beard in a long robe), who wanders, and who
  /// speaks of the healer as someone else and of "our teachings" and "our
  /// meditations". In Vault: "I wonder if we'll ever see our stolen gold statue
  /// again..."
  /// ★ Named from the game: "stolen gold statue" (MsgSanctumWonderIfEverSeeOur)
  ACTOR_SANCTUM_MEMBER = ACTOR_FIRST_PLACED + 1,
  /// Vault Sanctum's gold statue (the sprite of the one a chest gives back in
  /// log_rooms_a.c), placed off the map until the thieves are jailed
  /// (FLAG_VAULT_THIEVES_JAILED).
  /// ★ Named from the game: "our stolen gold statue"
  /// (MsgSanctumWonderIfEverSeeOur)
  ACTOR_GOLD_STATUE
};

static void Sanctum_RequestQuestAcceptance(void);
static void Sanctum_ReceiveWiseOnesMission(void);
static void Sanctum_ShowHealerAfterimage(void);
static void Sanctum_HideHealerAfterimage(void);
static void Sanctum_UpdateHealerAfterimage(struct Actor *actor);
static void Sanctum_SpawnOrbitingEffect(s32 actorId, s32 angleStep);
static s32 Sanctum_ChooseRestartOption(void);
static void Sanctum_ReenactSolSanctum(void);
static void Sanctum_RaiseItemIcon(s32 item, s32 fixedX, s32 fixedY, s32 fixedZ);
static void Sanctum_SpawnActorSpark(s32 id);
static void Sanctum_RestoreQuestFloor(void);

// UB: calling this s32 function through a void update routine is incompatible.
// Plain void changes 44 to 42 bytes in tbs-en.
#ifdef UBFIX
/// Faces the actor whose ID this update routine stores in the timer field.
static void Sanctum_FaceActorNamedByTimer(struct Actor *updated) {
#else
/// Faces the actor whose ID this update routine stores in the timer field.
static s32 Sanctum_FaceActorNamedByTimer(struct Actor *updated) {
#endif
  struct Actor *actor = (struct Actor *)updated;
  struct Actor *target;

  target = Actor_GetById((s16)actor->timer);
  actor->facing = ArcTan2(target->z.fixed - actor->z.fixed,
                          target->x.fixed - actor->x.fixed);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Returns the entrances, shared by all six places.
const struct SceneEntrance *Sanctum_GetEntrances(void) {
  return gSanctum_Entrances;
}

const struct SceneExitRegion *Sanctum_GetRegions(void) { return NULL; }

const u32 *Sanctum_GetExits(void) { return gSanctum_Exits; }

/// Chooses the actors by entrance: Vale Sanctum's Great Healer
/// and two other healers (SANCTUM_FROM_VALE and SANCTUM_VALE_UNUSED), the
/// quest's gathering (SANCTUM_FROM_VALE_ERUPTION and
/// SANCTUM_FROM_QUEST_RESTART), the healer and ACTOR_SANCTUM_MEMBER of Vault
/// Sanctum and Bilibin Sanctum (SANCTUM_FROM_VAULT, SANCTUM_FROM_VAULT_REVISIT,
/// SANCTUM_FROM_LOG_ROOMS_A and SANCTUM_FROM_BILIBIN), the healer alone in
/// Altin Sanctum and Xian Sanctum (SANCTUM_FROM_ALTIN and SANCTUM_FROM_XIAN),
/// and nobody otherwise.
const struct ActorPlacement *Sanctum_GetActorPlacements(void) {
  switch (gAdventure.entrance) {
  case SANCTUM_FROM_VALE:
  case SANCTUM_VALE_UNUSED:
    return gSanctum_ValeActorPlacements;

  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_VAULT_REVISIT:
  case SANCTUM_FROM_LOG_ROOMS_A:
  case SANCTUM_FROM_BILIBIN:
    return gSanctum_VaultBilibinActorPlacements;

  case SANCTUM_FROM_VALE_ERUPTION:
  case SANCTUM_FROM_QUEST_RESTART:
    return gSanctum_QuestActorPlacements;

  case SANCTUM_FROM_ALTIN:
  case SANCTUM_FROM_XIAN:
    return gSanctum_AltinXianActorPlacements;

  default:
    return gSanctum_NoActorPlacements;
  }
}

/// Chooses the triggers by entrance, and in Vale Sanctum before and
/// after the quest by story flag: gSanctum_ValeTriggers before the party has
/// left Vale, gSanctum_ValeTriggersAfterLeaving once it has
/// (FLAG_VALE_PARTY_LEFT) and gSanctum_ValeTriggersAfterKalay once it has
/// reached Kalay (FLAG_KALAY_REACHED); gSanctum_QuestTriggers for the quest's
/// visits (SANCTUM_FROM_VALE_ERUPTION, SANCTUM_FROM_QUEST_RESTART and
/// SANCTUM_QUEST_UNUSED).
const struct Trigger *Sanctum_GetTriggers(void) {
  switch (gAdventure.entrance) {
  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_LOG_ROOMS_A:
    return gSanctum_VaultTriggers;
  case SANCTUM_FROM_VAULT_REVISIT:
    return gSanctum_VaultRevisitTriggers;
  case SANCTUM_FROM_VALE_ERUPTION:
  case SANCTUM_FROM_QUEST_RESTART:
  case SANCTUM_QUEST_UNUSED:
    return gSanctum_QuestTriggers;
  case SANCTUM_FROM_XIAN:
    return gSanctum_XianTriggers;
  case SANCTUM_FROM_ALTIN:
    return gSanctum_AltinTriggers;
  case SANCTUM_FROM_BILIBIN:
    return gSanctum_BilibinTriggers;
  default:
    break;
  }

  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
    return gSanctum_ValeTriggersAfterKalay;
  }
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    return gSanctum_ValeTriggersAfterLeaving;
  }
  return gSanctum_ValeTriggers;
}

/// Vault Sanctum: talking to ACTOR_SANCTUM_MEMBER, about the stolen gold
/// statue until the thieves are jailed (FLAG_VAULT_THIEVES_JAILED) and about
/// fate afterwards; on the later visit (SANCTUM_FROM_VAULT_REVISIT), the statue
/// polished.
static void Sanctum_TalkAboutGoldenStatue(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
    Event_SetMessageId(MsgSanctumWhenStrayFromYourWorldly);
  } else {
    Event_SetMessageId(MsgSanctumHealerMustWorriedAboutNever);
  }

  if (gAdventure.entrance == SANCTUM_FROM_VAULT_REVISIT) {
    Event_SetMessageId(MsgSanctumPolishedGoldStatueReturnedUs);
  }

  Import_ActorSetAnimation(ACTOR_SANCTUM_MEMBER, 1);
  Actor_LinkPair(ACTOR_SANCTUM_MEMBER, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(2);
  Event_ShowMessageAndWait(ACTOR_SANCTUM_MEMBER, 0);
  Event_End();
}

/// Vault Sanctum: Mind Read on ACTOR_SANCTUM_MEMBER, about the stolen statue,
/// or once it is back, about Isaac awakening to the Sanctum's teachings; on the
/// later visit, the statue cleaned up.
static void Sanctum_ReadMindAboutGoldenStatue(void) {

  Event_Start();

  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgSanctumWonderIfEverSeeOur);
  } else {
    Event_SetMessageId(MsgSanctumChildHasAwakenedOurTeachings);
  }

  if (gAdventure.entrance == SANCTUM_FROM_VAULT_REVISIT) {
    Event_SetMessageId(MsgSanctumDirtyGoldenStatueCleanedUp);
  }

  Actor_Stop(ACTOR_SANCTUM_MEMBER);
  Import_ActorSetAnimation(ACTOR_SANCTUM_MEMBER, 1);
  Event_WaitFrames(2);
  Event_ShowMessageAndWait(ACTOR_SANCTUM_MEMBER, 0);
  Actor_SetMovementTypeById(ACTOR_SANCTUM_MEMBER, 2);
  Event_End();
}

/// Vale Sanctum: talking to the Great Healer after Isaac has refused the quest.
/// "Are you sure?" Changing his mind, Isaac can accept after all, and the
/// quest goes on as if he had accepted at once.
static void Sanctum_ReconsiderQuest(void) {
  Event_Start();
  Event_SetMessageId(MsgSanctumAreYouSure);
  FieldMessage_ShowAtActor(ACTOR_HEALER, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
    Event_WaitFrames(20);
  } else {
    Event_WaitFrames(20);
    FieldMessage_ShowAtActor(ACTOR_HEALER, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_WaitFrames(20);
      FieldMessage_ShowAtActor(ACTOR_HEALER, 0);
      if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
        Event_WaitFrames(20);
        if (Actor_GetById(ACTOR_HEALER)->facing >= FACING_NORTHWEST &&
            Actor_GetById(ACTOR_HEALER)->facing <= FACING_NORTHEAST) {
          Actor_SetSpeed(ACTOR_HEALER, PIXELS(1) / 2, PIXELS(1) / 4);
          Actor_ArmCallback(ACTOR_HEALER, FACING_EAST, 0);
          Event_WaitFrames(10);
          Actor_GetById(ACTOR_HEALER)->steeringFlags &= ~ACTOR_FACES_MOTION;
          Actor_SetPositionAndReset(ACTOR_HEALER, 152, 120);
          Event_WaitFrames(1);
          Actor_GetById(ACTOR_HEALER)->steeringFlags |= ACTOR_FACES_MOTION;
          Event_WaitFrames(20);
          Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
          Event_WaitFrames(20);
          Actor_SetPositionAndReset(ACTOR_PARTY_LEADER, 168, 120);
          Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 192, 168);
          Event_WaitFrames(20);
          Actor_SetPositionAndReset(ACTOR_HEALER, 168, 120);
          Actor_ArmCallback(ACTOR_HEALER, FACING_SOUTHEAST + FACING_STEP, 0);
          Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
        } else {
          Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 192, 168);
          Event_WaitFrames(20);
          Actor_ArmCallback(ACTOR_HEALER, FACING_SOUTHEAST + FACING_STEP, 0);
          Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
        }
        Sanctum_ReceiveWiseOnesMission();
        PaletteFade_SetTarget(RGB(0, 0, 0), PALETTES_ALL);
        PaletteFade_StartInterpolation(120);
        Event_WaitFrames(120);
        Sound_PlaySong(MUS_INN);
        Sound_WaitForSong();
        GameFlag_Set(FLAG_SANCTUM_QUEST_ACCEPTED);
        Import_EventSetExitRequest(SANCTUM_TO_VALE_PLAZA);
      }
    }
  }
  Event_End();
}

#if TBS_PATCH_1
#define SANCTUM_NEW_FRIENDS_SPEAKER 2
#else
#define SANCTUM_NEW_FRIENDS_SPEAKER 1
#endif

/// Vale Sanctum, once the party has reached Kalay: talking to
/// ACTOR_WEST_HEALER, who asks about the party's new friends. "Isaac, are
/// your new friends Adepts by any chance?" Denied, a friend's silence says they
/// must be: in every edition but the Japanese, that friend is Ivan, placed for
/// it; in the Japanese, Garet.
static void Sanctum_AskAboutNewAdepts(void) {
  Event_Start();
#if TBS_PATCH_1
  Event_CallWithLastActiveActorId(gSanctum_IvanActorPlacement);
  WaitFrames(1);
#endif
  Event_SetMessageId(MsgSanctumIsaacYourNewFriendsAdepts);
  FieldMessage_ShowAtActor(ACTOR_WEST_HEALER, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_ShowMessageAndWait(ACTOR_WEST_HEALER, 0);
  } else {
    gFieldWork->message++;
    FieldEvent_ShowTwoMessagesAndWait(SANCTUM_NEW_FRIENDS_SPEAKER, 16, 1, 24, 1,
                                      3, 7, 16, 1, 14, 0);
    Event_ShowMessageAndWait(ACTOR_WEST_HEALER, 0);
  }
  Event_End();
}

/// Vale Sanctum, once the party has left Vale: talking to ACTOR_EAST_HEALER,
/// who asks whether Isaac feels any change in his Psynergy.
static void Sanctum_AskAboutPsynergyChanges(void) {

  Event_Start();
  Actor_SetAngleToward(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(10);
  Event_SetMessageId(MsgSanctumDoFeelAnyChangeIn);
  FieldMessage_ShowAtActor(ACTOR_EAST_HEALER, 0);

  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    (gFieldWork->message)++;
  }

  Event_ShowMessageAndWait(ACTOR_EAST_HEALER, 0);
  Event_End();
}

/// Sets up the place for its entrance and plays its opening event.
/// Vault Sanctum places ACTOR_GOLD_STATUE once the thieves are jailed; Vale
/// Sanctum's quest visits (SANCTUM_FROM_VALE_ERUPTION, and
/// SANCTUM_FROM_QUEST_RESTART after a refusal, which sets
/// FLAG_SANCTUM_QUEST_RESTARTED) restore the floor and play the request, unless
/// the party is returning after a battle.
s32 Sanctum_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  Event_WaitFrames(1);
  switch (gAdventure.entrance) {
  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_VAULT_REVISIT:
  case SANCTUM_FROM_LOG_ROOMS_A:
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Actor_SetPosition(ACTOR_GOLD_STATUE, PIXELS(200), PIXELS(80));
    }
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
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
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    break;
  case SANCTUM_FROM_QUEST_RESTART:
    Sanctum_RestoreQuestFloor();
    GameFlag_Set(FLAG_SANCTUM_QUEST_RESTARTED);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Sanctum_RequestQuestAcceptance();
    }
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    break;
  }
  return 0;
}

/// Vale Sanctum: heading for the door after refusing the quest (cell
/// behavior 2, until FLAG_SANCTUM_WARNED_BEFORE_LEAVING is set, which it sets).
/// The Great Healer warns that the choice is final: "Once you step outside the
/// village, you cannot take back your decision."
static void Sanctum_WarnBeforeLeaving(void) {
  Event_Start();
  FieldCamera_SetSpeedParameters(PIXELS(1), PIXELS(1) / 8);
  Actor_PlaceCurrentWithinCameraBounds(ACTOR_GARET, 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(20);
  Actor_SetAngleToward(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_HEALER, 4);
  Event_WaitFrames(20);
  Event_SetMessageId(MsgSanctumOnceStepOutsideVillageCannot);
  Event_ShowMessageAndWait(ACTOR_HEALER, 0);
  GameFlag_Set(FLAG_SANCTUM_WARNED_BEFORE_LEAVING);
  Event_End();
}

/// Vale Sanctum: leaving after refusing the quest (cell behavior 1) ends
/// the game. The room turns sepia: `And so the world began drifting toward its
/// fated destruction...` Starting again from the Great Healer takes
/// SANCTUM_TO_QUEST_RESTART, back to SANCTUM_FROM_QUEST_RESTART; the other
/// choices take SANCTUM_TO_FILE_SCREEN, to SceneId_FileScreen.
static void Sanctum_LeaveAfterRefusing(void) {
  struct PaletteFade *paletteWork;

  Event_Start();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STILL);
  Import_ActorSetAnimation(ACTOR_DORA, 0);
  Import_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 0);
  Import_ActorSetAnimation(ACTOR_HEALER, 0);
  Import_ActorSetAnimation(ACTOR_WEST_HEALER, 0);
  Import_ActorSetAnimation(ACTOR_EAST_HEALER, 0);
  PaletteFade_SetTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  Event_WaitFrames(180);
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
  Event_WaitFrames(1);
  FieldMessage_ShowCentered(MsgSanctumWorldBeganDrifting, 1, 0);
  PaletteFade_SetTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  Event_WaitFrames(120);
  Event_WaitFrames(60);
  if (Sanctum_ChooseRestartOption() == 0) {
    Event_End();
    Import_EventSetExitRequest(SANCTUM_TO_QUEST_RESTART);
  } else {
    Event_End();
    Import_EventSetExitRequest(SANCTUM_TO_FILE_SCREEN);
  }
}

// UB: calling this s32 function through a void update routine is incompatible.
// Plain void changes 66 to 64 bytes in tbs-en.
#ifdef UBFIX
/// Keeps this actor at the party leader's drawing priority.
static void Sanctum_MatchLeaderPriority(struct Actor *updated) {
#else
/// Keeps this actor at the party leader's drawing priority.
static s32 Sanctum_MatchLeaderPriority(struct Actor *updated) {
#endif
  struct Actor *actor = (struct Actor *)updated;
  actor->sprite->image.priority =
      Actor_GetById(ACTOR_PARTY_LEADER)->sprite->image.priority;
  actor->sprite->shadow.priority =
      Actor_GetById(ACTOR_PARTY_LEADER)->sprite->image.priority;
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Has Isaac, Garet, Dora, the Mayor and the two healers nod, waiting for the
/// last of them.
static inline void Sanctum_NodAll(void) {
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_DORA, 3);
  Import_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 3);
  Import_ActorSetAnimation(ACTOR_WEST_HEALER, 3);
  Actor_SetAnimationAndWait(ACTOR_EAST_HEALER, 3);
}

/// Starts the same six's repeated motions and waits for the last of them.
static inline void Sanctum_StartAllRepeatedMotion(void) {
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_StartRepeatedMotion(ACTOR_DORA, 1);
  Actor_StartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
  Actor_StartRepeatedMotion(ACTOR_WEST_HEALER, 1);
  Actor_RunRepeatedMotion(ACTOR_EAST_HEALER, 1);
}

/// Shows the same emote on the same six.
static inline void Sanctum_ShowAllEmotes(void) {
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
}

/// Vale Sanctum, SANCTUM_FROM_VALE_ERUPTION and SANCTUM_FROM_QUEST_RESTART: the
/// gathering hears what happened in Sol Sanctum (skipped after a refusal), the
/// Great Healer explains the Wise One's warning, and asks Isaac to accept
/// responsibility for the fate of the land. Accepting plays
/// Sanctum_ReceiveWiseOnesMission, sets FLAG_SANCTUM_QUEST_ACCEPTED and takes
/// SANCTUM_TO_VALE_PLAZA; refusing leaves the room to gSanctum_QuestTriggers.
static void Sanctum_RequestQuestAcceptance(void) {
  u32 effectIndex;

  Event_Start();
  // A repeat visit skips the account of Sol Sanctum.
  if (GameFlag_Get(FLAG_SANCTUM_QUEST_RESTARTED) != 0) {
    Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
    FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
    FieldCamera_WaitForArrival();
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gFieldWork->transitionFrames = 64;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_WaitFrames(120);
  } else {
    PaletteFade_ApplyTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
    PaletteFade_StartInterpolation(1);
    Event_WaitFrames(1);
    FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
    FieldCamera_WaitForArrival();
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    // Reenact the journey before explaining the quest.
    Sanctum_ReenactSolSanctum();
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_StartInterpolation(60);
    Event_WaitFrames(100);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
    Event_WaitFrames(30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
    Event_WaitFrames(20);
    Actor_SetMovementTypeById(ACTOR_HEALER, MOVEMENT_TYPE_STAND);
    Actor_SetMovementTypeById(ACTOR_GARETS_GRANDFATHER, MOVEMENT_TYPE_STAND);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_GARETS_GRANDFATHER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_WEST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_EAST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
    Event_WaitFrames(30);
    Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
    Event_SetMessageId(MsgSanctumWeHadIdeaTrueSanctum);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_WaitFrames(10);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 3);
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    Event_WaitFrames(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_DORA, 0);
    Event_WaitFrames(20);
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_WaitFrames(20);
      Actor_SetAnimationAndWait(ACTOR_DORA, 3);
      Event_WaitFrames(20);
      Event_ShowMessage(ACTOR_DORA, 0);
      gFieldWork->message++;
    } else {
      Event_WaitFrames(20);
      Actor_SetAnimationAndWait(ACTOR_DORA, 4);
      Event_WaitFrames(20);
      gFieldWork->message++;
      Event_ShowMessage(ACTOR_DORA, 0);
    }
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_WEST_HEALER, 1);
    Event_WaitFrames(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WEST_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_WEST_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_WEST_HEALER, 0);
    Event_WaitFrames(20);
    Event_OpenMessage(ACTOR_WEST_HEALER, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_WaitFrames(20);
      Actor_SetAnimationAndWait(ACTOR_WEST_HEALER, 3);
      Event_WaitFrames(20);
      Event_ShowMessage(ACTOR_WEST_HEALER, 0);
      gFieldWork->message++;
    } else {
      Event_WaitFrames(20);
      Actor_SetAnimationAndWait(ACTOR_WEST_HEALER, 4);
      Event_WaitFrames(20);
      gFieldWork->message++;
      Event_ShowMessage(ACTOR_WEST_HEALER, 0);
    }
    Event_WaitFrames(20);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_DORA, 3);
    Import_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 3);
    Import_ActorSetAnimation(ACTOR_WEST_HEALER, 3);
    Actor_SetAnimationAndWait(ACTOR_EAST_HEALER, 3);
    Event_WaitFrames(30);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    0);
    Event_WaitFrames(60);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Event_WaitFrames(20);
    Actor_WalkToAndWait(ACTOR_GARETS_GRANDFATHER, 0xe0, 0x78);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_WaitFrames(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Event_WaitFrames(10);
    Actor_RunRepeatedMotion(ACTOR_DORA, 1);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_DORA, 0);
    Event_WaitFrames(30);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 3);
    Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Event_WaitFrames(60);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH + FACING_STEP,
                        0);
    Event_WaitFrames(60);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 3);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_HEALER, FACING_SOUTHEAST + FACING_STEP, 0);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH + FACING_STEP,
                        0);
    Event_WaitFrames(60);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Event_WaitFrames(50);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    0);
    Event_WaitFrames(40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Event_WaitFrames(60);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 1);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER,
                    EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Event_WaitFrames(60);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(10);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GARET, 1);
    Actor_StartRepeatedMotion(ACTOR_DORA, 1);
    Actor_StartRepeatedMotion(ACTOR_WEST_HEALER, 1);
    Actor_RunRepeatedMotion(ACTOR_EAST_HEALER, 1);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 4);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(30);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_PARTY_LEADER, 0);
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_WaitFrames(60);
    Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
    Event_WaitFrames(20);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_WaitFrames(30);
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    Event_WaitFrames(20);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Event_WaitFrames(20);
    Event_WaitFrames(40);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 2);
    Import_ActorSetAnimation(ACTOR_HEALER, 0);
    Actor_SetSpriteColorById(ACTOR_HEALER, SPRITE_COLOR_BLINK);
    Sanctum_ShowHealerAfterimage();
    Event_PlaySong(SE_WISE_ONE_QUEST);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Event_WaitFrames(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Event_WaitFrames(16);
    for (effectIndex = 0; effectIndex < 6; effectIndex++) {
      Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
      Event_WaitFrames(8);
    }
    Event_WaitFrames(8);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Event_WaitFrames(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Event_WaitFrames(96);
    Event_WaitFrames(32);
    Actor_SetSpriteColorById(ACTOR_HEALER, SPRITE_COLOR_NORMAL);
    Event_WaitFrames(30);
    // Return to the gathering and finish the departure conversation.
    Sanctum_HideHealerAfterimage();
    Import_ActorSetAnimation(ACTOR_HEALER, 1);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 2);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(20);
    Actor_SetAttachedEffect(ACTOR_GARETS_GRANDFATHER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_DORA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_WEST_HEALER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_EAST_HEALER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_WaitFrames(60);
    Actor_RunRepeatedMotion(ACTOR_EAST_HEALER, 1);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_EAST_HEALER, 0);
    Event_WaitFrames(20);
    Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    Event_WaitFrames(60);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 4);
    Event_WaitFrames(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(30);
    Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    Event_WaitFrames(20);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_LinkPair(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
    Event_WaitFrames(20);
    Import_ActorSetAnimation(ACTOR_WEST_HEALER, 3);
    Import_ActorSetAnimation(ACTOR_EAST_HEALER, 3);
    Actor_WaitForAnimationChange(ACTOR_EAST_HEALER);
    Event_WaitFrames(30);
    Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    Actor_LinkPair(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
    Event_WaitFrames(20);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_DORA, 3);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 3);
    Actor_WaitForAnimationChange(ACTOR_GARETS_GRANDFATHER);
    Event_WaitFrames(60);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 2);
    Import_ActorSetAnimation(ACTOR_HEALER, 0);
    Actor_SetSpriteColorById(ACTOR_HEALER, SPRITE_COLOR_BLINK);
    Sanctum_ShowHealerAfterimage();
    Event_PlaySong(SE_WISE_ONE_QUEST);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Event_WaitFrames(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Event_WaitFrames(16);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GARET, 1);
    Actor_StartRepeatedMotion(ACTOR_DORA, 1);
    Actor_StartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Actor_StartRepeatedMotion(ACTOR_WEST_HEALER, 1);
    Actor_StartRepeatedMotion(ACTOR_EAST_HEALER, 1);
    for (effectIndex = 0; effectIndex < 6; effectIndex++) {
      Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
      Event_WaitFrames(8);
    }
    Event_WaitFrames(8);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Event_WaitFrames(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Event_WaitFrames(128);
    Actor_SetSpriteColorById(ACTOR_HEALER, SPRITE_COLOR_NORMAL);
    Event_WaitFrames(30);
    Sanctum_HideHealerAfterimage();
    Import_ActorSetAnimation(ACTOR_HEALER, 1);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 2);
    Event_WaitFrames(30);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                    0);
    Event_WaitFrames(20);
    Actor_LinkPair(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
    Event_WaitFrames(40);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Event_WaitFrames(20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GARET, 1);
    Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    Actor_StartRepeatedMotion(ACTOR_WEST_HEALER, 1);
    Actor_StartRepeatedMotion(ACTOR_EAST_HEALER, 1);
    Actor_LinkPair(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
    Actor_SetAttachedEffect(ACTOR_GARETS_GRANDFATHER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_WaitFrames(60);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Event_WaitFrames(60);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 4);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(20);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_DORA, 3);
    Import_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 3);
    Import_ActorSetAnimation(ACTOR_WEST_HEALER, 3);
    Import_ActorSetAnimation(ACTOR_EAST_HEALER, 3);
    Actor_WaitForAnimationChange(ACTOR_EAST_HEALER);
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_WaitFrames(20);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(50);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_WaitFrames(80);
    Actor_RunRepeatedMotion(ACTOR_DORA, 1);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 4);
    Event_WaitFrames(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Event_WaitFrames(80);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 1);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_DORA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_GARETS_GRANDFATHER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_WEST_HEALER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_EAST_HEALER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_WaitFrames(60);
    Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 4);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 1);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 4);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(40);
    Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    Actor_LinkPair(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Event_WaitFrames(60);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_DORA, 1);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Event_WaitFrames(60);
    Actor_WalkToAndWait(ACTOR_HEALER, 0xc8, 0x88);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Actor_LinkPair(ACTOR_HEALER, ACTOR_GARET, 0);
    Event_WaitFrames(40);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
    Event_WaitFrames(50);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Event_WaitFrames(40);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
    Event_WaitFrames(50);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 1);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_WaitFrames(60);
    Actor_SetAttachedEffect(ACTOR_DORA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_DORA, 1);
    Event_ShowMessage(ACTOR_DORA, 0);
    Event_WaitFrames(30);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
    Event_WaitFrames(30);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_StartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_WaitFrames(30);
    Actor_WalkToAndWait(ACTOR_HEALER, 0xa8, 0x78);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Actor_FaceDirection(ACTOR_HEALER, FACING_NORTH + FACING_STEP, 0);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 4);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_WaitFrames(60);
    Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
    Event_WaitFrames(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(10);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_WaitFrames(20);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_WaitFrames(30);
  }
  // Ask Isaac to commit, or leave the group waiting for his answer.
  Event_SetMessageId(MsgSanctumIsaacWillAcceptResponsibilityFor);
  Actor_RunRepeatedMotion(ACTOR_HEALER, 1);
  Event_WaitFrames(20);
  Event_OpenMessage(ACTOR_HEALER, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Sanctum_ReceiveWiseOnesMission();
    PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
    PaletteFade_StartInterpolation(120);
    Event_WaitFrames(120);
    Event_PlaySong(MUS_INN);
    Sound_WaitForSong();
    GameFlag_Set(FLAG_SANCTUM_QUEST_ACCEPTED);
    Import_EventSetExitRequest(SANCTUM_TO_VALE_PLAZA);
  } else {
    Event_WaitFrames(30);
    Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 4);
    Event_WaitFrames(20);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_WaitFrames(20);
    Actor_GetById(ACTOR_EAST_HEALER)->priorityFlags &=
        ~ACTOR_PRIORITY_AUTOMATIC;
#ifdef UBFIX
    Actor_GetById(ACTOR_EAST_HEALER)->update = Sanctum_MatchLeaderPriority;
#else
    Actor_GetById(ACTOR_EAST_HEALER)->update =
        (void (*)(struct Actor *))Sanctum_MatchLeaderPriority;
#endif
  }
  Event_End();
}

/// Vale Sanctum, once Isaac accepts: Garet and the Mayor react, the Wise One
/// appears before the Great Healer and accepts the two, and the healer relays
/// its guidance; they are to leave the next day.
static void Sanctum_ReceiveWiseOnesMission(void) {

  u32 sparkIndex;
  struct Actor *wiseOne;

  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_GARETS_GRANDFATHER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_WEST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_EAST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
  FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_DORA, 3);
  Event_WaitFrames(60);
  Import_ActorSetAnimation(ACTOR_HEALER, 3);
  Import_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 3);
  Import_ActorSetAnimation(ACTOR_WEST_HEALER, 3);
  Actor_SetAnimationAndWait(ACTOR_EAST_HEALER, 3);
  Event_WaitFrames(50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  // Garet and the Mayor react to Isaac accepting the burden.
  Event_SetMessageId(MsgSanctumAcceptIsaacCantMean);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(20);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_GARET, 0);
  Actor_StartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  FieldEffect_SpawnAttached(ACTOR_GARETS_GRANDFATHER,
                            EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Event_WaitFrames(60);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Actor_FaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(40);
  Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
  Event_WaitFrames(20);
  Actor_ArmCallback(ACTOR_HEALER, FACING_NORTH + FACING_STEP, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_WaitFrames(80);
  Event_PlaySong(SONG_FADE_OUT_BGM);
  PaletteFade_SetTarget(PALETTE_TINT_PALE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  Event_WaitFrames(40);
  FieldCamera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(104), 1);
  Event_WaitFrames(120);
  Event_PlaySong(MUS_FOREST);
  Event_PlaySong(SE_MATERIALIZE);
  // The Wise One enters while the healer's afterimages appear.
  Actor_SetPosition(ACTOR_WISE_ONE, PIXELS(200), PIXELS(8));
  Actor_SetSpeed(ACTOR_WISE_ONE, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_MoveToAndWait(ACTOR_WISE_ONE, 200, 72);
  Event_PlaySong(SE_STOP_ACTION_SE);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(ACTOR_HEALER, 2);
  Import_ActorSetAnimation(ACTOR_HEALER, 0);
  Sanctum_ShowHealerAfterimage();
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_WISE_ONE, 0);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_DORA, 2);
  Actor_StartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  Actor_StartRepeatedMotion(ACTOR_WEST_HEALER, 2);
  Actor_StartRepeatedMotion(ACTOR_EAST_HEALER, 2);
  Event_WaitFrames(40);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_WaitFrames(20);
  Event_WaitFrames(40);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_WaitFrames(60);
  Actor_ConfigureSpriteColor(ACTOR_WISE_ONE, SPRITE_COLOR_BLINK);
  Event_PlaySong(SONG_FADE_OUT_BGM);
  Event_PlaySong(SE_MATERIALIZE);
  // Shrink the Wise One amid sparks before restoring the room.
  for (sparkIndex = 0; sparkIndex < 32; sparkIndex++) {
    Sanctum_SpawnActorSpark(ACTOR_WISE_ONE);
    Event_WaitFrames(4);
    wiseOne = Actor_GetById(ACTOR_WISE_ONE);
    wiseOne->scaleX += -Q16_ONE / 100;
    wiseOne = Actor_GetById(ACTOR_WISE_ONE);
    wiseOne->scaleY += -Q16_ONE / 100;
  }
  Event_PlaySong(SE_STOP_ACTION_SE);
  Actor_SetSpriteColorById(ACTOR_WISE_ONE, SPRITE_COLOR_NORMAL);
  Actor_SetPosition(ACTOR_WISE_ONE, 0, 0);
  Event_WaitFrames(40);
  FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
  FieldCamera_WaitForArrival();
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  Event_WaitFrames(120);
  Sanctum_HideHealerAfterimage();
  Import_ActorSetAnimation(ACTOR_HEALER, 1);
  Event_PlaySong(MUS_TOWN_IN_TROUBLE);
  Event_WaitFrames(60);
  Actor_RunRepeatedMotion(ACTOR_HEALER, 2);
  Event_WaitFrames(20);
  Sanctum_StartAllRepeatedMotion();
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_WaitFrames(30);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(30);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
  Event_WaitFrames(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(20);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
  Event_WaitFrames(20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_WaitFrames(20);
  Sanctum_ShowAllEmotes();
  Event_WaitFrames(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_WaitFrames(20);
  Sanctum_NodAll();
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
  Event_WaitFrames(180);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_WaitFrames(60);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Actor_LinkPair(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
  Event_WaitFrames(40);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
  Event_WaitFrames(40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_WaitFrames(60);
  Sanctum_StartAllRepeatedMotion();
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_WaitFrames(80);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_WaitFrames(60);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_WaitFrames(60);
  Actor_RunRepeatedMotion(ACTOR_HEALER, 1);
  FieldEffect_SpawnAttached(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Event_WaitFrames(20);
  Sanctum_StartAllRepeatedMotion();
  Event_WaitFrames(40);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_WaitFrames(20);
  Sanctum_NodAll();
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_WaitFrames(30);
  Sanctum_NodAll();
  Event_WaitFrames(120);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_WaitFrames(60);
  Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(20);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_HEALER, 4);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_WaitFrames(60);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_HEALER, 3);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_WaitFrames(40);
  Actor_WalkToAndWait(ACTOR_HEALER, 168, 176);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
  Actor_WalkToAndWait(ACTOR_HEALER, 200, 200);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_WEST_HEALER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_EAST_HEALER, FACING_WEST, 0);
  Actor_WalkTo(ACTOR_HEALER, 200, 0x110);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_WEST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_EAST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  Actor_WaitForMoveById(ACTOR_HEALER);
  Actor_SetPosition(ACTOR_HEALER, 0, 0);
  Event_WaitFrames(60);
  Sanctum_ShowAllEmotes();
  Event_WaitFrames(60);
  Event_WaitFrames(120);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WEST_HEALER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_EAST_HEALER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_WEST_HEALER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_WEST_HEALER, 0);
  Event_WaitFrames(120);
  Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_WaitFrames(60);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_WEST_HEALER, 2);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_WEST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_WaitFrames(80);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
  Actor_RunRepeatedMotion(ACTOR_WEST_HEALER, 1);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_WEST_HEALER, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_EAST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(20);
  FieldEffect_SpawnAttached(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                            0);
  Event_WaitFrames(60);
  Actor_RunRepeatedMotion(ACTOR_WEST_HEALER, 2);
  Event_WaitFrames(20);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(ACTOR_EAST_HEALER, 2);
  Event_WaitFrames(30);
  Actor_ArmCallback(ACTOR_EAST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_EAST_HEALER, 0);
  Event_WaitFrames(20);
  Import_ActorSetAnimation(ACTOR_WEST_HEALER, 3);
  Actor_SetAnimationAndWait(ACTOR_EAST_HEALER, 3);
  Event_WaitFrames(20);
  Actor_WalkTo(ACTOR_WEST_HEALER, 200, 0x110);
  Actor_WalkToAndWait(ACTOR_EAST_HEALER, 200, 0x110);
  Actor_SetPosition(ACTOR_WEST_HEALER, 0, 0);
  Actor_SetPosition(ACTOR_EAST_HEALER, 0, 0);
  FieldEffect_SpawnAttached(ACTOR_GARETS_GRANDFATHER,
                            EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_WaitFrames(60);
  Actor_WalkToAndWait(ACTOR_GARETS_GRANDFATHER, 200, 136);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 4);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(30);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_WaitFrames(20);
  Actor_WalkToAndWait(ACTOR_DORA, 168, 168);
  Actor_FaceActor(ACTOR_DORA, ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_DORA, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_DORA, 4);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_WaitFrames(60);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 4);
  Event_WaitFrames(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_WaitFrames(60);
  FieldEffect_SpawnAttached(ACTOR_GARETS_GRANDFATHER,
                            EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_WaitFrames(60);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEffect_SpawnAttached(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_WaitFrames(60);
  Actor_SetAnimationAndWait(ACTOR_DORA, 3);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_WaitFrames(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Event_WaitFrames(30);
  Import_ActorSetAnimation(ACTOR_DORA, 3);
  Event_WaitFrames(30);
  Import_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 3);
}

/// Copies the healer into the afterimage actor.
static void Sanctum_ShowHealerAfterimage(void) {
  u32 entryIndex;
  struct Actor *afterimage;
  struct Actor *actor;
  struct Sprite *sprite;

  actor = Actor_GetById(ACTOR_HEALER);
  if (actor != 0) {
    Import_ActorSetPosition(ACTOR_HEALER_AFTERIMAGE, actor->x.fixed,
                            actor->z.fixed);
  }
  Import_ActorSetAnimation(ACTOR_HEALER_AFTERIMAGE, 0);
  afterimage = Actor_GetById(ACTOR_HEALER_AFTERIMAGE);
  actor = Actor_GetById(ACTOR_HEALER);
  afterimage->facing = actor->facing;
  actor = Actor_GetById(ACTOR_HEALER_AFTERIMAGE);
  actor->update = Sanctum_UpdateHealerAfterimage;
  actor = Actor_GetById(ACTOR_HEALER_AFTERIMAGE);
  sprite = actor->sprite;
  {
    for (entryIndex = 0; entryIndex < sprite->count; entryIndex++) {
      struct SpriteAnimation *entry = sprite->entries[entryIndex];

      if (entry != 0 && entry->script != 0) {
        entry->colorMode = SPRITE_COLOR_RED_TINT;
      }
    }
  }
  sprite->dirty = 1;
  Actor_GetById(ACTOR_HEALER_AFTERIMAGE)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 2;
}

static void Sanctum_HideHealerAfterimage(void) {
  struct Actor *state = Actor_GetById(ACTOR_HEALER_AFTERIMAGE);
  state->update = 0;
  Import_ActorSetPosition(ACTOR_HEALER_AFTERIMAGE, 0, 0);
}

/// Cycles the afterimage through four offsets from the healer.
static void Sanctum_UpdateHealerAfterimage(struct Actor *updated) {
  struct Actor *actor = (struct Actor *)updated;
  struct Actor *healer;

  healer = Actor_GetById(ACTOR_HEALER);
  actor->targetX = actor->x.fixed = healer->x.fixed;
  actor->targetY = actor->y.fixed = healer->y.fixed;
  actor->targetZ = actor->z.fixed = healer->z.fixed - PIXELS(2);
  switch (gFrameCount & 3) {
  case 0:
    actor->targetX = actor->x.fixed = healer->x.fixed - PIXELS(7) / 2;
    break;
  case 1:
    actor->targetX = actor->x.fixed = healer->x.fixed + PIXELS(3);
    break;
  case 2:
    actor->targetY = actor->y.fixed = healer->y.fixed + PIXELS(2);
    break;
  case 3:
    actor->targetZ = actor->z.fixed = healer->z.fixed;
    break;
  }
}

/// Advances an effect around its linked actor on an elliptical path.
static void Sanctum_UpdateOrbitingEffect(struct Actor *updated) {
  struct Actor *effect = (struct Actor *)updated;
  struct Actor *anchor = effect->linkedActor;
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

  *angle = (*angle + effect->userValue);
}

/// Creates an orbiting effect with the requested angular step.
static void Sanctum_SpawnOrbitingEffect(s32 actorId, s32 angleStep) {
  s32 orbitStep;
  struct Actor *center;
  struct Actor *effect;
  struct Sprite *sprite;
  struct Sprite *sourceSprite;

  orbitStep = angleStep;
  center = Actor_GetById(actorId);
  if (center != 0) {
    effect = FieldActor_Create(SPRITE_SPARKLE, center->x.fixed,
                               center->y.fixed + PIXELS(45), center->z.fixed);
    if (effect != 0) {
      sprite = effect->sprite;
      Actor_StartScript(effect, gSanctum_OwnerEffectScript);
      {
        s32 zero = 0;

        effect->motionFlags = zero;
        effect->timer = zero;
      }
      effect->userValue = orbitStep;
      effect->update = Sanctum_UpdateOrbitingEffect;
      {
        u16 clearFlags = 0;
        sprite->flags = clearFlags;
      }
      sourceSprite = center->sprite;
      effect->linkedActor = center;
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

  Import_ActorSetPosition(ACTOR_HEALER, 0, 0);
  Import_ActorSetPosition(ACTOR_WEST_HEALER, 0, 0);
  Import_ActorSetPosition(ACTOR_EAST_HEALER, 0, 0);
  Import_ActorSetPosition(ACTOR_GARET, 0, 0);
  Import_ActorSetPosition(ACTOR_DORA, 0, 0);
  Import_ActorSetPosition(ACTOR_GARETS_GRANDFATHER, 0, 0);
  Import_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(1);
  Event_WaitFrames(1);
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
    Event_WaitFrames(1);
  }
  MenuArrow_Release(slot);
  Window_Close(window, 1);
  return choice;
}

/// Vale Sanctum, the quest's account: Isaac and Garet act out, in sepia, what
/// happened in Sol Sanctum.
static void Sanctum_ReenactSolSanctum(void) {

  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 3);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(15);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 184, 168);
  Actor_GetById(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetPositionAndReset(ACTOR_GARET, 200, 168);
  Event_WaitFrames(1);
  Actor_GetById(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Jump(ACTOR_GARET, 2, 0);
  Event_WaitFrames(15);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_HEALER, 0);
  Event_WaitFrames(5);
  Actor_Jump(ACTOR_GARET, 2, 0);
  Event_WaitFrames(25);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(5);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(5);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(10);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(15);
  Import_ActorSetAnimation(ACTOR_DORA, 3);
  Import_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 3);
  Import_ActorSetAnimation(ACTOR_HEALER, 3);
  Import_ActorSetAnimation(ACTOR_WEST_HEALER, 3);
  Actor_SetAnimationAndWait(ACTOR_EAST_HEALER, 3);
  Event_WaitFrames(20);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_DORA, 0);
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Event_WaitFrames(15);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_WaitFrames(10);
  // Illustrate the account with the floating item icon.
  Sanctum_RaiseItemIcon(ITEM_MARS_STAR, PIXELS(184), PIXELS(27), PIXELS(168));
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_WaitFrames(10);
  Actor_Jump(ACTOR_GARET, 4, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
  Event_WaitFrames(15);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
  Event_WaitFrames(30);
  Actor_Jump(ACTOR_GARET, 4, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_WaitFrames(15);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_WaitFrames(60);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(10);
  Actor_GetById(ACTOR_HEALER)->timer = GARET;
#ifdef UBFIX
  Actor_GetById(ACTOR_HEALER)->update = Sanctum_FaceActorNamedByTimer;
#else
  Actor_GetById(ACTOR_HEALER)->update =
      (void (*)(struct Actor *))Sanctum_FaceActorNamedByTimer;
#endif
  Actor_GetById(ACTOR_GARETS_GRANDFATHER)->timer = GARET;
#ifdef UBFIX
  Actor_GetById(ACTOR_GARETS_GRANDFATHER)->update =
      Sanctum_FaceActorNamedByTimer;
#else
  Actor_GetById(ACTOR_GARETS_GRANDFATHER)->update =
      (void (*)(struct Actor *))Sanctum_FaceActorNamedByTimer;
#endif
  Actor_SetPositionAndReset(ACTOR_GARET, 196, 180);
  Actor_SetPositionAndReset(ACTOR_GARET, 184, 184);
  Actor_SetPositionAndReset(ACTOR_GARET, 180, 180);
  Actor_SetPositionAndReset(ACTOR_GARET, 168, 168);
  Actor_SetPositionAndReset(ACTOR_GARET, 180, 156);
  Actor_ResetAndSetPositionInMode2(ACTOR_GARET, 200, 104);
  Actor_SetPositionAndReset(ACTOR_PARTY_LEADER, 192, 168);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_GARET);
  Event_WaitFrames(30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH + FACING_STEP, 0);
  Event_WaitFrames(15);
  Actor_GetById(ACTOR_GARETS_GRANDFATHER)->update = NULL;
  Actor_GetById(ACTOR_HEALER)->update = NULL;
  Actor_StartRepeatedMotion(ACTOR_HEALER, 2);
  FieldEffect_SpawnAttached(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                            0);
  Event_WaitFrames(60);
  Import_ActorSetAnimation(ACTOR_HEALER, 0);
  FieldEffect_SpawnAttached(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                            0);
  Event_WaitFrames(60);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_DORA, 3);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_WaitFrames(10);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
  Event_WaitFrames(20);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
  Event_WaitFrames(20);
  Event_WaitFrames(15);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 3);
  Event_WaitFrames(60);
  Actor_SetPositionAndReset(ACTOR_GARET, 208, 168);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_WaitFrames(10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_WaitFrames(10);
  Actor_ArmCallback(ACTOR_GARET, FACING_EAST, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(10);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 3);
  Event_WaitFrames(10);
  Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_ArmCallback(ACTOR_GARET, FACING_EAST, 0);
  Event_WaitFrames(10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(10);
  // Repeat the synchronized eight-point paths around the room.
  Actor_SetScript(ACTOR_PARTY_LEADER, gSanctum_LeaderCircleScript);
  Actor_SetScript(ACTOR_GARET, gSanctum_GaretCircleScript);
  Actor_RefreshSelectorById(ACTOR_PARTY_LEADER);
  Actor_RefreshSelectorById(ACTOR_GARET);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_WEST_HEALER, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_HEALER, 0);
  Event_WaitFrames(1);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_DORA, 0);
  Event_WaitFrames(1);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_WEST_HEALER, 0);
  Event_WaitFrames(1);
  Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 192, 168);
  Actor_SetPositionAndReset(ACTOR_GARET, 208, 168);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ArmCallback(ACTOR_GARET, FACING_NORTH + FACING_STEP, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Actor_ArmCallback(ACTOR_GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Event_WaitFrames(20);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_WaitFrames(20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_DORA, 3);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 3);
  Event_WaitFrames(30);
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Event_WaitFrames(20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(10);
  Actor_ArmCallback(ACTOR_GARET, FACING_EAST, 0);
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  Event_WaitFrames(60);
}

/// Shows an item icon rising from the supplied fixed-point position.
static void Sanctum_RaiseItemIcon(s32 item, s32 fixedX, s32 fixedY,
                                  s32 fixedZ) {
  struct Actor *icon;
  struct Sprite *sprite;
  u8 *transferBuffer;
  u32 frame;
  u8 zero;

  icon = FieldActor_Create(SPRITE_BLANK, fixedX, fixedY, fixedZ);
  zero = 0;
  if (icon != 0) {
    Actor_StartScript(icon, gSanctum_ItemIconGrowScript);
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
    Actor_StartScript(icon, gSanctum_ItemIconEndScript);
  }
}

/// Moves a spark upward, cycles its scale, and expires its lifetime.
static void Sanctum_UpdateRisingSpark(struct Actor *updated) {
  struct Actor *spark = updated;
  s32 scale;
  s32 timer;

  // The timer stores the upward step in sixteenths of a pixel.
  spark->y.fixed += spark->timer << 12;
  spark->targetY = spark->y.fixed;
  scale = 0;
  switch (((s16)spark->userValue >> 2) & 3) {
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
  timer = spark->userValue - 1;
  spark->userValue = timer;
  if ((s16)timer <= 0)
    Actor_StartScript(spark, gSanctum_SparkEndScript);
}

/// Creates a short-lived spark beside the specified actor.
static void Sanctum_SpawnActorSpark(s32 id) {
  struct Actor *actor;
  struct Actor *spark;
  struct Sprite *sprite;
  s32 x;
  s32 randomOffset;

  actor = Actor_GetById(id);
  if (actor == 0)
    return;
  randomOffset = Math_RemainderUnsigned(Import_RandomNext(), 20);
  x = actor->x.fixed;
  x += randomOffset << 16;
  x += -PIXELS(10);
  spark = FieldActor_Create(SPRITE_GLINT, x,
                            actor->y.fixed +
                                ((Import_RandomNext() & 15) << 16) + -PIXELS(8),
                            actor->z.fixed);
  if (spark == 0)
    return;
  sprite = spark->sprite;
  spark->motionFlags = 0;
  spark->timer = Math_RemainderUnsigned(Import_RandomNext(), 10) + 5;
  spark->userValue = Math_RemainderUnsigned(Import_RandomNext(), 60) + 30;
  spark->update = Sanctum_UpdateRisingSpark;
  sprite->flags = 0;
  sprite->image.priority = actor->sprite->image.priority;
}

/// Copies two blocks of map grid cells for the quest's visits, the cell
/// pictures of one and the cell behaviors of the other.
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
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);

  if ((u32)((actor->facing + 0x5fff) << 16) <= 0x3ffe0000) {
    return 1;
  }
  return 0;
}

/// Vale Sanctum: talking to the Great Healer outside the quest. From the front
/// the healer opens the Sanctum's services; otherwise the words follow the
/// story, from "Wielders of Psynergy are called Adepts" before the party has
/// left Vale to the closed path to Sol Sanctum once it has reached Kalay.
static void Sanctum_TalkToHealer(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Sanctum_Run(ACTOR_HEALER);
    return;
  }

  Event_Start();
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0)
    Event_SetMessageId(MsgSanctumPathSolSanctumStillClosed);
  else if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0)
    Event_SetMessageId(MsgSanctumMyControlOverPsynergyHas);
  else
    Event_SetMessageId(MsgSanctumWieldersPsynergyCalledAdeptsAdepts);
  Event_ShowMessageAndWait(ACTOR_HEALER, 0);
  Event_End();
}

/// Vault Sanctum: talking to the healer. From the front the healer opens the
/// Sanctum's services; otherwise the healer speaks of the gold statue or, on
/// the later visit, of the earthquakes. Its case for the quest's
/// entrances (SANCTUM_FROM_VALE_ERUPTION, SANCTUM_FROM_QUEST_RESTART and
/// SANCTUM_QUEST_UNUSED) passes to Sanctum_ReconsiderQuest, though their
/// triggers (gSanctum_QuestTriggers) call that directly.
static void Sanctum_TalkToHealerOnVisit(void) {
  s32 entrance;

  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Sanctum_Run(ACTOR_HEALER);
    return;
  }

  Event_Start();

  entrance = gAdventure.entrance;

  switch (entrance) {
  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_LOG_ROOMS_A:
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Event_SetMessageId(MsgSanctumWasHandFateReturnedGold);
    } else {
      Event_SetMessageId(MsgSanctumWeWillHelpAnytimeAs);
    }
    break;
  case SANCTUM_FROM_VAULT_REVISIT:
    Event_SetMessageId(MsgSanctumMayWrongButLatelyThere);
    break;
  case SANCTUM_FROM_VALE_ERUPTION:
  case SANCTUM_FROM_QUEST_RESTART:
  case SANCTUM_QUEST_UNUSED:
    Event_End();
    Sanctum_ReconsiderQuest();
    return;
  default:
    break;
  }

  Event_ShowMessageAndWait(ACTOR_HEALER, 0);
  Event_End();
}

/// Bilibin Sanctum: talking to the healer, about Kolima's curse, lifted or not;
/// from the front, the Sanctum's services.
static void Sanctum_TalkAboutCurse(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Sanctum_Run(ACTOR_HEALER);
    return;
  }

  Event_Start();
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0)
    Event_SetMessageId(MsgSanctumCurseMayOverButWe);
  else
    Event_SetMessageId(MsgSanctumAmStartingFeelOnlyBeginning);
  Event_ShowMessageAndWait(ACTOR_HEALER, 0);
  Event_End();
}

/// Altin Sanctum: talking to the healer, about the beasts in the mine, or the
/// party's saving Altin from them; from the front, the Sanctum's services.
static void Sanctum_TalkAboutAltinMonsters(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Sanctum_Run(ACTOR_HEALER);
    return;
  }

  Event_Start();
  Event_SetMessageId(MsgSanctumWasAfterEerieNightWhen);
  if (GameFlag_Get(FLAG_HYDROS_STATUES_DEFEATED) != 0)
    Event_SetMessageId(MsgSanctumSavedAltinFromMonstersClearly);
  Event_ShowMessageAndWait(ACTOR_HEALER, 0);
  Event_End();
}

/// Xian Sanctum: talking to the healer, who came to Xian from very distant
/// lands; from the front, the Sanctum's services.
static void Sanctum_TalkAboutJourneyFromXian(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Sanctum_Run(ACTOR_HEALER);
    return;
  }

  Event_Start();
  Event_SetMessageId(MsgSanctumCameXianFromVeryDistant);
  Event_ShowMessageAndWait(ACTOR_HEALER, 0);
  Event_End();
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
    SC_RELEASE,
};

struct SceneEntrance gSanctum_Entrances[] = {
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
    { SCENE_TABLE_END },
};

u32 gSanctum_Exits[] = {
    SCENE_EXITS(Sanctum),
    SCENE_EXIT(SANCTUM_TO_VALE, Vale, VALE_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_VAULT, Vault, VAULT_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_QUEST_RESTART, Sanctum, SANCTUM_FROM_QUEST_RESTART),
    SCENE_EXIT(SANCTUM_TO_VALE_PLAZA, ValePlaza, VALE_PLAZA_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_ALTIN, Altin, ALTIN_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_BILIBIN, Bilibin, BILIBIN_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_XIAN, Xian, XIAN_FROM_SANCTUM_WEST),
    SCENE_EXIT(SANCTUM_TO_FILE_SCREEN, FileScreen, FILE_SCREEN_FROM_SANCTUM),
    SCENE_EXITS_END,
};

struct ActorPlacement gSanctum_NoActorPlacements[] = {
    { SCENE_TABLE_END },
};

struct ActorPlacement gSanctum_ValeActorPlacements[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(130), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

#if TBS_PATCH_1
struct ActorPlacement gSanctum_IvanActorPlacement[] = {
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};
#endif

struct ActorPlacement gSanctum_VaultBilibinActorPlacements[] = {
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(130), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(200), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { 0xdd, CONDITION_ALWAYS, 7,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gSanctum_QuestActorPlacements[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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
    { SCENE_TABLE_END },
};

struct ActorPlacement gSanctum_AltinXianActorPlacements[] = {
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(132), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gSanctum_ValeTriggers[] = {
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkToHealer) },
    { TRIGGER_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumOurPsynergyWasGivenToUs) },
    { TRIGGER_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumPsynergyIsAPowerThatShould) },
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { TRIGGERS_END },
};

struct Trigger gSanctum_VaultTriggers[] = {
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkToHealerOnVisit) },
    { TRIGGER_TALK, ACTOR_SANCTUM_MEMBER, FLAG_VAULT_THIEVES_JAILED, EVENT(Sanctum_TalkAboutGoldenStatue) },
    { TRIGGER_TALK, ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT(Sanctum_TalkAboutGoldenStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumGoOnIfThatEasesThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT(Sanctum_ReadMindAboutGoldenStatue) },
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VAULT },
    { TRIGGERS_END },
};

struct Trigger gSanctum_VaultRevisitTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VAULT_REVISIT },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkToHealerOnVisit) },
    { TRIGGER_TALK, ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT(Sanctum_TalkAboutGoldenStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumIHopeTheEarthquakesArentA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT(Sanctum_ReadMindAboutGoldenStatue) },
    { TRIGGERS_END },
};

struct Trigger gSanctum_QuestTriggers[] = {
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_ReconsiderQuest) },
    { TRIGGER_TALK, ACTOR_GARET, CONDITION_ALWAYS, MSGNO(MsgSanctumItIsATerribleBurdenTo) },
    { TRIGGER_TALK, ACTOR_DORA, CONDITION_ALWAYS, MSGNO(MsgSanctumMotherWouldNeverDoubtYourCourage) },
    { TRIGGER_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgSanctumIDontBlameYouICouldnt) },
    { TRIGGER_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWasItRightToRefuseWhen) },
    { TRIGGER_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItsClearThatTheGreatHealer) },
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Sanctum_LeaveAfterRefusing) },
    { TRIGGER_STEP, 2, FLAG_SANCTUM_WARNED_BEFORE_LEAVING, EVENT(Sanctum_WarnBeforeLeaving) },
    { TRIGGERS_END },
};

struct Trigger gSanctum_ValeTriggersAfterLeaving[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkToHealer) },
    { TRIGGER_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumThePowerOfMtAlephStems) },
    { TRIGGER_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_AskAboutPsynergyChanges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumThePowerOfMyPsynergyHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumTheAngaranContinentWillCertainlySee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumMyPsynergyHasntChangedAtAll) },
    { TRIGGERS_END },
};

struct Trigger gSanctum_ValeTriggersAfterKalay[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkToHealer) },
    { TRIGGER_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_AskAboutNewAdepts) },
    { TRIGGER_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWhyIsItThatDifferentAdepts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItllBeDifficultToReachSol) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumISeeABeautifulGirlHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItsNotFairIWantTo) },
    { TRIGGERS_END },
};

struct Trigger gSanctum_AltinTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_ALTIN },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkAboutAltinMonsters) },
    // Unused: a copy of the row above, which always answers first.
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkAboutAltinMonsters) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgSanctumPleaseSaveTheHelplessInAltin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumAlmightyFateMustHaveHeardMe) },
    { TRIGGERS_END },
};

struct Trigger gSanctum_BilibinTriggers[] = {
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkAboutCurse) },
    { TRIGGER_TALK, ACTOR_SANCTUM_MEMBER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumPleaseGiveMeStrengthAtTimes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumWillITurnIntoATree) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumSomeoneWillSaveUsJustKeep) },
    { TRIGGER_TALK, ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, MSGNO(MsgSanctumHowAboutThatOurWishesHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumTheSignsOfDangerAreActually) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, MSGNO(MsgSanctumPeaceWillBeKeptAsLong) },
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_BILIBIN },
    { TRIGGERS_END },
};

struct Trigger gSanctum_XianTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_XIAN },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkAboutJourneyFromXian) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWhenYouStayInXianFor) },
    { TRIGGERS_END },
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
    SC_RELEASE,
};

s32 gSanctum_SparkEndScript[] = {
    SC_RELEASE,
};
