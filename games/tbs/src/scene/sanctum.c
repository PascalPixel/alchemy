/// @file
/// The Sanctum of Vale and of four towns on the party's way, which holds six
/// places in one scene (SceneId_Sanctum).
///
/// The entrance the party arrives at decides which place it is, and
/// Sanctum_GetActorPlacements and Sanctum_GetTriggers choose that place's
/// tables by it. Spoken to from the front, each place's healer opens the
/// Sanctum's services (Sanctum_Run). From any other side the healer talks. Each
/// place's door leads back out to its own town.
///
/// - Vale Sanctum (SANCTUM_FROM_VALE, SANCTUM_FROM_VALE_ERUPTION,
///   SANCTUM_VALE_UNUSED and SANCTUM_FROM_QUEST_RESTART), the Great Healer's,
///   entered from Vale at SANCTUM_FROM_VALE. Before the trip up Mt. Aleph, the
///   Great Healer and two other healers (ACTOR_WEST_HEALER and
///   ACTOR_EAST_HEALER) speak of Psynergy. After the eruption the Great Healer
///   brings Isaac and Garet in from Vale (vale_eruption.c) to
///   SANCTUM_FROM_VALE_ERUPTION. There Dora, the Mayor and the healers hear
///   what happened on the mountain, acted out in sepia
///   (Sanctum_ReenactSolSanctum). For the account, the palette fade turns every
///   background and sprite palette to PALETTE_TINT_SEPIA, and after it fades
///   back to PALETTE_TINT_NONE over 60 frames. The Great Healer asks Isaac to
///   take on the Wise One's quest (Sanctum_RequestQuestAcceptance). Each time
///   his Psynergy shows, ACTOR_HEALER_AFTERIMAGE, a copy of him in
///   SPRITE_COLOR_RED_TINT, jumps round him by a few pixels a frame
///   (Sanctum_UpdateHealerAfterimage) while effects circle him
///   (Sanctum_SpawnOrbitingEffect). Accepting plays
///   Sanctum_ReceiveWiseOnesMission and sets FLAG_SANCTUM_QUEST_ACCEPTED, and
///   SANCTUM_TO_VALE_PLAZA takes the two to SceneId_ValePlaza (vale_plaza.c).
///   Refusing leaves the room to gSanctum_QuestTriggers. The Great Healer asks
///   again when spoken to (Sanctum_ReconsiderQuest), and the door draws a
///   warning (FLAG_SANCTUM_WARNED_BEFORE_LEAVING). Leaving anyway ends the game
///   (Sanctum_LeaveAfterRefusing): `And so the world began drifting toward its
///   fated destruction...` From there the player can start again from the Great
///   Healer, by SANCTUM_TO_QUEST_RESTART to SANCTUM_FROM_QUEST_RESTART, which
///   skips the account (FLAG_SANCTUM_QUEST_RESTARTED), or leave by
///   SANCTUM_TO_FILE_SCREEN to SceneId_FileScreen. After the party has left
///   Vale (FLAG_VALE_PARTY_LEFT), and again after it has reached Kalay
///   (FLAG_KALAY_REACHED), the three talk of what the eruption and the Wise One
///   changed.
/// - Vault Sanctum (SANCTUM_FROM_VAULT and SANCTUM_FROM_LOG_ROOMS_A). Its
///   healer and the other person there (ACTOR_SANCTUM_MEMBER) miss a gold
///   statue stolen during the eruption, until the party has the thieves jailed
///   and the statue is returned (FLAG_VAULT_THIEVES_JAILED). Then the statue
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
///   through Vault later (vault_revisit.c). The statue has been polished, and
///   the healer worries about earthquakes on Mt. Aleph.
/// - Sanctum, the name the game gives every other entrance, which leaves only
///   SANCTUM_UNUSED. No exit leads there, and it places nobody.

#include "speaker.h"
#include "flags.h"
#include "main_routine_work.h"
#include "module/scene/sanctum.h"
#include "module/field/field_sequence.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/window/window.h"
#include "module/window/screens.h"
#include "module/sound/sound.h"
#include "module/system/system.h"
#include "battle.h"
#include "input.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "types.h"
#include "event.h"
#include "sprite.h"
#include "field_effect.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "io_reg.h"
#include "save.h"
#include "event_inline.h"
#include "window.h"
#include "battle_sprite_id.h"
#include "heap_release.h"
#include "actor_builtin_scripts.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "frame.h"
#include "fixed_math.h"
#include "sounds.h"
#include "heap.h"
#include "item_ids.h"
#include "sound_driver_symbols.h"

/// After the quest is refused, the Great Healer has warned that stepping
/// outside the village is final.
/// ★ Named from the game: "Once you step outside the village, you cannot take
/// back your decision." (MsgSanctumOnceStepOutsideVillageCannot)
#define FLAG_SANCTUM_WARNED_BEFORE_LEAVING FLAG_SCENE_TEMP_00
/// The party is starting again from the Great Healer after refusing the quest,
/// so the account of Sol Sanctum is skipped.
#define FLAG_SANCTUM_QUEST_RESTARTED FLAG_SCENE_TEMP_01

// Scene flags, in Vale Sanctum: 00 is set once the Great Healer has
// warned Isaac that leaving the village cannot be undone; 01 marks the visit
// after a refusal (SANCTUM_FROM_QUEST_RESTART), which skips the account of Sol
// Sanctum.

/// The placed actors, numbering the rows of the table
/// Sanctum_GetActorPlacements returns for the entrance.
///
/// The party's rows take their own ids. Every place's first row is its healer.
enum SanctumActorId {
  /// The place's healer, a man with a white beard in a tall hat and a long
  /// purple and blue robe.
  ///
  /// He is the Great Healer in Vale Sanctum, and the place's own healer
  /// elsewhere.
  /// ★ Named from the game: "What is it, Great Healer?"
  /// (MsgSanctumWeHadIdeaTrueSanctum)
  ACTOR_HEALER = ACTOR_FIRST_PLACED,
  // Vale Sanctum (gSanctum_ValeActorPlacements, and
  // gSanctum_QuestActorPlacements at the
  // quest).
  /// The western of Vale Sanctum's two other healers, men with long brown hair
  /// and beards in long robes.
  ///
  /// Mind Read hears a villager in cottage_rooms_a.c call the Sanctum's people
  /// so: "The healers are supposed to guard Sol Sanctum." Before the quest:
  /// "Our Psynergy was given to us by the sacred power of Mt. Aleph."
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
  /// The Mayor, Garet's grandfather, an old man with white hair and a cane.
  ///
  /// The Great Healer calls him so: "That is all, Mayor."
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
  /// The other person of the Sanctum, who wanders.
  ///
  /// He wears the western healer's sprite (a man with long brown hair and a
  /// beard in a long robe). He speaks of the healer as someone else and of "our
  /// teachings" and "our meditations". In Vault: "I wonder if we'll ever see
  /// our stolen gold statue again..."
  /// ★ Named from the game: "stolen gold statue"
  /// (MsgSanctumWonderIfEverSeeOur)
  ACTOR_SANCTUM_MEMBER = ACTOR_FIRST_PLACED + 1,
  /// Vault Sanctum's gold statue, placed off the map until the thieves are
  /// jailed (FLAG_VAULT_THIEVES_JAILED).
  ///
  /// Its sprite is that of the one a chest gives back in log_rooms_a.c.
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

/// Faces the actor whose ID this update routine stores in the timer field.
static UNREAD_RESULT Sanctum_FaceActorNamedByTimer(struct Actor *actor) {
  struct Actor *target;

  target = Actor_Get((s16)actor->timer);
  actor->facing = ArcTan2(target->z.fixed - actor->z.fixed,
                          target->x.fixed - actor->x.fixed);
  RETURN_UNREAD(0);
}

/// Returns the entrances, shared by all six places.
const struct SceneEntrance *Sanctum_GetEntrances(void) {
  return gSanctum_Entrances;
}

const struct SceneExitRegion *Sanctum_GetRegions(void) { return NULL; }

const u32 *Sanctum_GetExits(void) { return gSanctum_Exits; }

/// Chooses the actors by entrance.
///
/// - Vale Sanctum's Great Healer and two other healers (SANCTUM_FROM_VALE and
///   SANCTUM_VALE_UNUSED).
/// - The quest's gathering (SANCTUM_FROM_VALE_ERUPTION and
///   SANCTUM_FROM_QUEST_RESTART).
/// - The healer and ACTOR_SANCTUM_MEMBER of Vault Sanctum and Bilibin Sanctum
///   (SANCTUM_FROM_VAULT, SANCTUM_FROM_VAULT_REVISIT, SANCTUM_FROM_LOG_ROOMS_A
///   and SANCTUM_FROM_BILIBIN).
/// - The healer alone in Altin Sanctum and Xian Sanctum (SANCTUM_FROM_ALTIN and
///   SANCTUM_FROM_XIAN).
/// - Nobody otherwise.
const struct ActorPlacement *Sanctum_GetActorPlacements(void) {
  switch (gMainRoutineWork.entrance) {
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

/// Chooses the triggers by entrance, and in Vale Sanctum before and after the
/// quest by story flag.
///
/// Vale Sanctum takes gSanctum_ValeTriggers before the party has left Vale,
/// gSanctum_ValeTriggersAfterLeaving once it has (FLAG_VALE_PARTY_LEFT) and
/// gSanctum_ValeTriggersAfterKalay once it has reached Kalay
/// (FLAG_KALAY_REACHED). The quest's visits (SANCTUM_FROM_VALE_ERUPTION,
/// SANCTUM_FROM_QUEST_RESTART and SANCTUM_QUEST_UNUSED) take
/// gSanctum_QuestTriggers.
const struct Trigger *Sanctum_GetTriggers(void) {
  switch (gMainRoutineWork.entrance) {
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

/// Vault Sanctum: talking to ACTOR_SANCTUM_MEMBER.
///
/// He talks about the stolen gold statue until the thieves are jailed
/// (FLAG_VAULT_THIEVES_JAILED) and about fate afterwards. On the later visit
/// (SANCTUM_FROM_VAULT_REVISIT), the statue is polished.
static void Sanctum_TalkAboutGoldenStatue(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
    Event_SetMessageId(MsgSanctumWhenStrayFromYourWorldly);
  } else {
    Event_SetMessageId(MsgSanctumHealerMustWorriedAboutNever);
  }

  if (gMainRoutineWork.entrance == SANCTUM_FROM_VAULT_REVISIT) {
    Event_SetMessageId(MsgSanctumPolishedGoldStatueReturnedUs);
  }

  Actor_SetAnimation(ACTOR_SANCTUM_MEMBER, ANIM_STAND);
  Actor_FaceEachOther(ACTOR_SANCTUM_MEMBER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(2);
  Event_ShowMessage(ACTOR_SANCTUM_MEMBER, 0);
  Event_End();
}

/// Vault Sanctum: Mind Read on ACTOR_SANCTUM_MEMBER.
///
/// He thinks about the stolen statue or, once it is back, about Isaac awakening
/// to the Sanctum's teachings. On the later visit, the statue is cleaned up.
static void Sanctum_ReadMindAboutGoldenStatue(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgSanctumWonderIfEverSeeOur);
  } else {
    Event_SetMessageId(MsgSanctumChildHasAwakenedOurTeachings);
  }

  if (gMainRoutineWork.entrance == SANCTUM_FROM_VAULT_REVISIT) {
    Event_SetMessageId(MsgSanctumDirtyGoldenStatueCleanedUp);
  }

  Actor_Stop(ACTOR_SANCTUM_MEMBER);
  Actor_SetAnimation(ACTOR_SANCTUM_MEMBER, ANIM_STAND);
  Event_Wait(2);
  Event_ShowMessage(ACTOR_SANCTUM_MEMBER, 0);
  Actor_SetMovementTypeById(ACTOR_SANCTUM_MEMBER, 2);
  Event_End();
}

/// Vale Sanctum: talking to the Great Healer after Isaac has refused the quest.
///
/// "Are you sure?" Changing his mind, Isaac can accept after all, and the quest
/// goes on as if he had accepted at once.
static void Sanctum_ReconsiderQuest(void) {
  Event_Start();
  Event_SetMessageId(MsgSanctumAreYouSure);
  // "Are you sure?"
  Event_OpenMessage(ACTOR_HEALER, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    Event_Wait(20);
  } else {
    Event_Wait(20);
    // "Would you like to think it over?"
    Event_OpenMessage(ACTOR_HEALER, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_Wait(20);
      // "Isaac, will you take the fate of the world in your hands?"
      Event_OpenMessage(ACTOR_HEALER, 0);
      if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
        Event_Wait(20);
        if (Actor_Get(ACTOR_HEALER)->facing >= FACING_NORTHWEST &&
            Actor_Get(ACTOR_HEALER)->facing <= FACING_NORTHEAST) {
          Actor_SetSpeed(ACTOR_HEALER, 0.5);
          Actor_FaceDirection(ACTOR_HEALER, FACING_EAST, 0);
          Event_Wait(10);
          Actor_Get(ACTOR_HEALER)->steeringFlags &= ~ACTOR_FACES_MOTION;
          Actor_WalkToAndWait(ACTOR_HEALER, 152, 120);
          Event_Wait(1);
          Actor_Get(ACTOR_HEALER)->steeringFlags |= ACTOR_FACES_MOTION;
          Event_Wait(20);
          Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
          Event_Wait(20);
          Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 120);
          Actor_WalkTo(ACTOR_PARTY_LEADER, 192, 168);
          Event_Wait(20);
          Actor_WalkToAndWait(ACTOR_HEALER, 168, 120);
          Actor_FaceDirection(ACTOR_HEALER, FACING_SOUTH_SOUTHEAST, 0);
          Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
        } else {
          Actor_WalkTo(ACTOR_PARTY_LEADER, 192, 168);
          Event_Wait(20);
          Actor_FaceDirection(ACTOR_HEALER, FACING_SOUTH_SOUTHEAST, 0);
          Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
        }
        Sanctum_ReceiveWiseOnesMission();
        PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
        PaletteFade_StartInterpolation(120);
        Event_Wait(120);
        Sound_Play(MUS_INN);
        Sound_WaitForBgmRestore();
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
/// ACTOR_WEST_HEALER, who asks about the party's new friends.
///
/// "Isaac, are your new friends Adepts by any chance?" Denied, a friend's
/// silence says they must be. In every edition but the Japanese, that friend is
/// Ivan, placed for it. In the Japanese, it is Garet.
static void Sanctum_AskAboutNewAdepts(void) {
  Event_Start();
#if TBS_PATCH_1
  Event_AppendActorPlacements(gSanctum_IvanActorPlacement);
  wait(1);
#endif
  Event_SetMessageId(MsgSanctumIsaacYourNewFriendsAdepts);
  // "Isaac, are your new friends Adepts by any chance?"
  Event_OpenMessage(ACTOR_WEST_HEALER, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    // "So, there are others besides Valeans who have been granted..."
    Event_ShowMessage(ACTOR_WEST_HEALER, 0);
  } else {
    gFieldWork->message++;
    // "......"
    // "......"
    FieldEvent_ShowTwoMessagesAndWait(SANCTUM_NEW_FRIENDS_SPEAKER, 16, 1, 24, 1,
                                      3, 7, 16, 1, 14, 0);
    // "Your friend seems angered by your words, Isaac... That..."
    Event_ShowMessage(ACTOR_WEST_HEALER, 0);
  }
  Event_End();
}

/// Vale Sanctum, once the party has left Vale: talking to ACTOR_EAST_HEALER,
/// who asks whether Isaac feels any change in his Psynergy.
static void Sanctum_AskAboutPsynergyChanges(void) {
  Event_Start();
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(10);
  Event_SetMessageId(MsgSanctumDoFeelAnyChangeIn);
  // "Do you feel any change in your Psynergy?"
  Event_OpenMessage(ACTOR_EAST_HEALER, 0);

  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    (gFieldWork->message)++;
  }

  Event_ShowMessage(ACTOR_EAST_HEALER, 0);
  Event_End();
}

/// Sets up the place for its entrance and plays its opening event.
///
/// Vault Sanctum places ACTOR_GOLD_STATUE once the thieves are jailed.
///
/// Vale Sanctum's quest visits restore the floor and play the request, unless
/// the party is returning after a battle. Those visits are
/// SANCTUM_FROM_VALE_ERUPTION, and SANCTUM_FROM_QUEST_RESTART after a refusal,
/// which sets FLAG_SANCTUM_QUEST_RESTARTED.
s32 Sanctum_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  Event_Wait(1);
  switch (gMainRoutineWork.entrance) {
  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_VAULT_REVISIT:
  case SANCTUM_FROM_LOG_ROOMS_A:
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Actor_SetPosition(ACTOR_GOLD_STATUE, 200, 80);
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

/// Vale Sanctum: heading for the door after refusing the quest (cell behavior
/// 2, until FLAG_SANCTUM_WARNED_BEFORE_LEAVING is set, which it sets).
///
/// The Great Healer warns that the choice is final: "Once you step outside the
/// village, you cannot take back your decision."
static void Sanctum_WarnBeforeLeaving(void) {
  Event_Start();
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveToActorById(ACTOR_GARET, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_SetMessageId(MsgSanctumOnceStepOutsideVillageCannot);
  // "Once you step outside the village, you cannot take back your..."
  Event_ShowMessage(ACTOR_HEALER, 0);
  GameFlag_Set(FLAG_SANCTUM_WARNED_BEFORE_LEAVING);
  Event_End();
}

/// Vale Sanctum: leaving after refusing the quest (cell behavior 1) ends the
/// game.
///
/// The room turns sepia: `And so the world began drifting toward its fated
/// destruction...`
///
/// Starting again from the Great Healer takes SANCTUM_TO_QUEST_RESTART, back to
/// SANCTUM_FROM_QUEST_RESTART. The other choices take SANCTUM_TO_FILE_SCREEN,
/// to SceneId_FileScreen.
static void Sanctum_LeaveAfterRefusing(void) {
  struct PaletteFade *paletteWork;

  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STILL);
  Actor_SetAnimation(ACTOR_DORA, ANIM_STILL);
  Actor_SetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_STILL);
  Actor_SetAnimation(ACTOR_HEALER, ANIM_STILL);
  Actor_SetAnimation(ACTOR_WEST_HEALER, ANIM_STILL);
  Actor_SetAnimation(ACTOR_EAST_HEALER, ANIM_STILL);
  PaletteFade_ApplyTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  Event_Wait(180);
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
  Event_Wait(1);
  FieldMessage_ShowCentered(MsgSanctumWorldBeganDrifting, 1, 0);
  PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(120);
  Event_Wait(120);
  Event_Wait(60);
  if (Sanctum_ChooseRestartOption() == 0) {
    Event_End();
    Import_EventSetExitRequest(SANCTUM_TO_QUEST_RESTART);
  } else {
    Event_End();
    Import_EventSetExitRequest(SANCTUM_TO_FILE_SCREEN);
  }
}

/// Keeps this actor at the party leader's drawing priority.
static UNREAD_RESULT Sanctum_MatchLeaderPriority(struct Actor *actor) {
  actor->sprite->image.priority =
      Actor_Get(ACTOR_PARTY_LEADER)->sprite->image.priority;
  actor->sprite->shadow.priority =
      Actor_Get(ACTOR_PARTY_LEADER)->sprite->image.priority;
  RETURN_UNREAD(0);
}

/// Has Isaac, Garet, Dora, the Mayor and the two healers nod, waiting for the
/// last of them.
static inline void Sanctum_NodAll(void) {
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_WEST_HEALER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_EAST_HEALER, ANIM_NOD);
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
/// Great Healer asks Isaac to take on the quest.
///
/// The gathering hears what happened in Sol Sanctum (skipped after a refusal).
/// The Great Healer explains the Wise One's warning, and asks Isaac to accept
/// responsibility for the fate of the land.
///
/// Accepting plays Sanctum_ReceiveWiseOnesMission, sets
/// FLAG_SANCTUM_QUEST_ACCEPTED and takes SANCTUM_TO_VALE_PLAZA. Refusing leaves
/// the room to gSanctum_QuestTriggers.
static void Sanctum_RequestQuestAcceptance(void) {
  u32 effectIndex;

  Event_Start();
  // A repeat visit skips the account of Sol Sanctum.
  if (GameFlag_Get(FLAG_SANCTUM_QUEST_RESTARTED) != 0) {
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
    FieldCamera_MoveTo(192, CAMERA_KEEP, 160, 1);
    FieldCamera_WaitForMove();
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gFieldWork->transitionFrames = 64;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(120);
  } else {
    PaletteFade_ApplyTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
    PaletteFade_StartInterpolation(1);
    Event_Wait(1);
    FieldCamera_MoveTo(192, CAMERA_KEEP, 160, 1);
    FieldCamera_WaitForMove();
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    // Reenact the journey before explaining the quest.
    Sanctum_ReenactSolSanctum();
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_StartInterpolation(60);
    Event_Wait(100);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
    Event_Wait(30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(20);
    Actor_SetMovementTypeById(ACTOR_HEALER, MOVEMENT_TYPE_STAND);
    Actor_SetMovementTypeById(ACTOR_GARETS_GRANDFATHER, MOVEMENT_TYPE_STAND);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
    Actor_SetSpeed(ACTOR_GARET, 0.8);
    Actor_SetSpeed(ACTOR_DORA, 0.8);
    Actor_SetSpeed(ACTOR_GARETS_GRANDFATHER, 0.8);
    Actor_SetSpeed(ACTOR_WEST_HEALER, 0.8);
    Actor_SetSpeed(ACTOR_EAST_HEALER, 0.8);
    Actor_SetSpeed(ACTOR_HEALER, 0.8);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
    Event_SetMessageId(MsgSanctumWeHadIdeaTrueSanctum);
    // "We had no idea... The true sanctum..."
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_DORA, 0);
    Event_Wait(20);
    // "And Jenna and Kraden... They're prisoners?"
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
      Event_Wait(20);
      // "They've been taken against their will... How awful!"
      Event_ShowMessage(ACTOR_DORA, 0);
      gFieldWork->message++;
    } else {
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
      Event_Wait(20);
      gFieldWork->message++;
      // "Well...we can only hope they're all right."
      Event_ShowMessage(ACTOR_DORA, 0);
    }
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_WEST_HEALER, 1);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WEST_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_WEST_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_WEST_HEALER, 0);
    Event_Wait(20);
    // "Do you think they can be saved?"
    Event_OpenMessage(ACTOR_WEST_HEALER, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_WEST_HEALER, ANIM_NOD);
      Event_Wait(20);
      // "Jenna's brother, Felix, will protect them, won't he?"
      Event_ShowMessage(ACTOR_WEST_HEALER, 0);
      gFieldWork->message++;
    } else {
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_WEST_HEALER, ANIM_SHAKE_HEAD);
      Event_Wait(20);
      gFieldWork->message++;
      // "Then our hopes must lie with Felix."
      Event_ShowMessage(ACTOR_WEST_HEALER, 0);
    }
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimation(ACTOR_DORA, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_WEST_HEALER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_EAST_HEALER, ANIM_NOD);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Event_Wait(20);
    Actor_WalkToAndWait(ACTOR_GARETS_GRANDFATHER, 224, 120);
    Event_Wait(10);
    // "What is it, Great Healer? You have been very quiet."
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_DORA, 1);
    Event_Wait(10);
    // "It's like you're a thousand miles away."
    Event_ShowMessage(ACTOR_DORA, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 3);
    Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    Event_Wait(10);
    // "That hovering stone, the one that appeared before Isaac..."
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH_SOUTHWEST, 0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
    Event_Wait(10);
    // "What about it?"
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_HEALER, FACING_SOUTH_SOUTHEAST, 0);
    Event_Wait(10);
    // "I have seen it before in my dreams."
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH_SOUTHWEST, 0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Event_Wait(50);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    0);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 1);
    Event_Wait(10);
    // "In my mind's eye, it appeared again just now and spoke to me."
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER,
                    EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(10);
    // "You saw it just now?"
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    Event_Wait(10);
    // "It must be the Wise One, guardian of the Elemental Stars."
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GARET, 1);
    Actor_StartRepeatedMotion(ACTOR_DORA, 1);
    Actor_StartRepeatedMotion(ACTOR_WEST_HEALER, 1);
    Actor_RunRepeatedMotion(ACTOR_EAST_HEALER, 1);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
    Event_Wait(10);
    // "It told me what happened to you in Sol Sanctum."
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(30);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Event_Wait(20);
    // "So, what does the Wise One say about the eruption?"
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    // "Will the volcano destroy Vale?"
    Event_ShowMessage(ACTOR_DORA, 0);
    Event_Wait(20);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 2);
    Actor_SetAnimation(ACTOR_HEALER, ANIM_STILL);
    Actor_SetSpriteColorById(ACTOR_HEALER, SPRITE_COLOR_BLINK);
    Sanctum_ShowHealerAfterimage();
    Sound_Play(SE_WISE_ONE_QUEST);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, TRIG_TURN * 9 / 128);
    Event_Wait(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, TRIG_TURN * 9 / 128);
    Event_Wait(16);
    for (effectIndex = 0; effectIndex < 6; effectIndex++) {
      Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, TRIG_TURN * 9 / 128);
      Event_Wait(8);
    }
    Event_Wait(8);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, TRIG_TURN * 9 / 128);
    Event_Wait(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, TRIG_TURN * 9 / 128);
    Event_Wait(96);
    Event_Wait(32);
    Actor_SetSpriteColorById(ACTOR_HEALER, SPRITE_COLOR_NORMAL);
    Event_Wait(30);
    // Return to the gathering and finish the departure conversation.
    Sanctum_HideHealerAfterimage();
    Actor_SetAnimation(ACTOR_HEALER, ANIM_STAND);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(20);
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
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_EAST_HEALER, 1);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_EAST_HEALER, 0);
    Event_Wait(20);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(30);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_FaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_WEST_HEALER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_EAST_HEALER, ANIM_NOD);
    Actor_WaitForAnimationChange(ACTOR_EAST_HEALER);
    Event_Wait(30);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    Actor_FaceEachOther(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_DORA, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
    Actor_WaitForAnimationChange(ACTOR_GARETS_GRANDFATHER);
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 2);
    Actor_SetAnimation(ACTOR_HEALER, ANIM_STILL);
    Actor_SetSpriteColorById(ACTOR_HEALER, SPRITE_COLOR_BLINK);
    Sanctum_ShowHealerAfterimage();
    Sound_Play(SE_WISE_ONE_QUEST);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, TRIG_TURN * 9 / 128);
    Event_Wait(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, TRIG_TURN * 9 / 128);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Event_Wait(16);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GARET, 1);
    Actor_StartRepeatedMotion(ACTOR_DORA, 1);
    Actor_StartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Actor_StartRepeatedMotion(ACTOR_WEST_HEALER, 1);
    Actor_StartRepeatedMotion(ACTOR_EAST_HEALER, 1);
    for (effectIndex = 0; effectIndex < 6; effectIndex++) {
      Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, TRIG_TURN * 9 / 128);
      Event_Wait(8);
    }
    Event_Wait(8);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, TRIG_TURN * 9 / 128);
    Event_Wait(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, TRIG_TURN * 9 / 128);
    Event_Wait(128);
    Actor_SetSpriteColorById(ACTOR_HEALER, SPRITE_COLOR_NORMAL);
    Event_Wait(30);
    Sanctum_HideHealerAfterimage();
    Actor_SetAnimation(ACTOR_HEALER, ANIM_STAND);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 2);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                    0);
    Event_Wait(20);
    Actor_FaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
    Event_Wait(40);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Event_Wait(20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GARET, 1);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    Actor_StartRepeatedMotion(ACTOR_WEST_HEALER, 1);
    Actor_StartRepeatedMotion(ACTOR_EAST_HEALER, 1);
    Actor_FaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
    Actor_SetAttachedEffect(ACTOR_GARETS_GRANDFATHER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(60);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimation(ACTOR_DORA, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_WEST_HEALER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_EAST_HEALER, ANIM_NOD);
    Actor_WaitForAnimationChange(ACTOR_EAST_HEALER);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(50);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_Wait(80);
    Actor_RunRepeatedMotion(ACTOR_DORA, 1);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Event_Wait(80);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 1);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(20);
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
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(40);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    Actor_FaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_DORA, 1);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Event_Wait(60);
    Actor_WalkToAndWait(ACTOR_HEALER, 200, 136);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Actor_FaceEachOther(ACTOR_HEALER, ACTOR_GARET, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    Event_Wait(50);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    Event_Wait(50);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 1);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_Wait(60);
    Actor_SetAttachedEffect(ACTOR_DORA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_DORA, 1);
    Event_ShowMessage(ACTOR_DORA, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_StartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(30);
    Actor_WalkToAndWait(ACTOR_HEALER, 168, 120);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Actor_FaceDirection(ACTOR_HEALER, FACING_NORTH_NORTHEAST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    Event_Wait(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_Wait(30);
  }
  // Ask Isaac to commit, or leave the group waiting for his answer.
  Event_SetMessageId(MsgSanctumIsaacWillAcceptResponsibilityFor);
  Actor_RunRepeatedMotion(ACTOR_HEALER, 1);
  Event_Wait(20);
  // "Isaac, will you accept responsibility for the fate of the land?"
  Event_OpenMessage(ACTOR_HEALER, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Sanctum_ReceiveWiseOnesMission();
    PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
    PaletteFade_StartInterpolation(120);
    Event_Wait(120);
    Sound_Play(MUS_INN);
    Sound_WaitForBgmRestore();
    GameFlag_Set(FLAG_SANCTUM_QUEST_ACCEPTED);
    Import_EventSetExitRequest(SANCTUM_TO_VALE_PLAZA);
  } else {
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "I can't blame you... I couldn't do it if I were in your shoes."
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(20);
    Actor_Get(ACTOR_EAST_HEALER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    Actor_Get(ACTOR_EAST_HEALER)->update =
        (void (*)(struct Actor *))Sanctum_MatchLeaderPriority;
  }
  Event_End();
}

/// Vale Sanctum, once Isaac accepts: the Wise One appears and accepts Isaac and
/// Garet.
///
/// Garet and the Mayor react. The Wise One appears before the Great Healer and
/// accepts the two, and the healer relays its guidance. They are to leave the
/// next day.
static void Sanctum_ReceiveWiseOnesMission(void) {
  u32 sparkIndex;
  struct Actor *wiseOne;

  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_SetSpeed(ACTOR_GARET, 0.8);
  Actor_SetSpeed(ACTOR_DORA, 0.8);
  Actor_SetSpeed(ACTOR_GARETS_GRANDFATHER, 0.8);
  Actor_SetSpeed(ACTOR_WEST_HEALER, 0.8);
  Actor_SetSpeed(ACTOR_EAST_HEALER, 0.8);
  Actor_SetSpeed(ACTOR_HEALER, 0.8);
  FieldCamera_MoveTo(192, CAMERA_KEEP, 160, 1);
  FieldCamera_WaitForMove();
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(60);
  Actor_SetAnimation(ACTOR_HEALER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_WEST_HEALER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_EAST_HEALER, ANIM_NOD);
  Event_Wait(50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // Garet and the Mayor react to Isaac accepting the burden.
  Event_SetMessageId(MsgSanctumAcceptIsaacCantMean);
  // "You accept? Oh, Isaac... You can't mean it..."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_GARET, 0);
  Actor_StartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN,
                  0);
  Event_Wait(60);
  // "You chose to leave your decision to Isaac. There shall be no..."
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(40);
  Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_HEALER, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
  Event_Wait(30);
  // "O, Wise One, these two have accepted your quest. Give us..."
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_Wait(80);
  Sound_Play(SOUND_FADE_OUT_BGM);
  PaletteFade_ApplyTarget(PALETTE_TINT_PALE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  Event_Wait(40);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(192, CAMERA_KEEP, 104, 1);
  Event_Wait(120);
  Sound_Play(MUS_FOREST);
  Event_PlaySound(SE_MATERIALIZE);
  // The Wise One enters while the healer's afterimages appear.
  Actor_SetPosition(ACTOR_WISE_ONE, 200, 8);
  Actor_SetSpeed(ACTOR_WISE_ONE, 0.4);
  Actor_MoveToAndWait(ACTOR_WISE_ONE, 200, 72);
  Sound_Play(SE_STOP_ACTION_SE);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_HEALER, 2);
  Actor_SetAnimation(ACTOR_HEALER, ANIM_STILL);
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
  Event_Wait(40);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_Wait(20);
  Event_Wait(40);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_Wait(60);
  Actor_SetSpriteColorById(ACTOR_WISE_ONE, SPRITE_COLOR_BLINK);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Sound_Play(SE_MATERIALIZE);
  // Shrink the Wise One amid sparks before restoring the room.
  for (sparkIndex = 0; sparkIndex < 32; sparkIndex++) {
    Sanctum_SpawnActorSpark(ACTOR_WISE_ONE);
    Event_Wait(4);
    wiseOne = Actor_Get(ACTOR_WISE_ONE);
    wiseOne->scaleX -= Q16_ONE / 100;
    wiseOne = Actor_Get(ACTOR_WISE_ONE);
    wiseOne->scaleY -= Q16_ONE / 100;
  }
  Sound_Play(SE_STOP_ACTION_SE);
  Actor_SetSpriteColorById(ACTOR_WISE_ONE, SPRITE_COLOR_NORMAL);
  Actor_SetPosition(ACTOR_WISE_ONE, 0, 0);
  Event_Wait(40);
  FieldCamera_MoveTo(192, CAMERA_KEEP, 160, 1);
  FieldCamera_WaitForMove();
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  Event_Wait(120);
  Sanctum_HideHealerAfterimage();
  Actor_SetAnimation(ACTOR_HEALER, ANIM_STAND);
  Sound_Play(MUS_TOWN_IN_TROUBLE);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_HEALER, 2);
  Event_Wait(20);
  Sanctum_StartAllRepeatedMotion();
  Event_Wait(30);
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_Wait(20);
  Sanctum_ShowAllEmotes();
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_Wait(20);
  Sanctum_NodAll();
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
  Event_Wait(180);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(60);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Actor_FaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
  Event_Wait(40);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Sanctum_StartAllRepeatedMotion();
  Event_Wait(30);
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(80);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_HEALER, 1);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Event_Wait(20);
  Sanctum_StartAllRepeatedMotion();
  Event_Wait(40);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_Wait(20);
  Sanctum_NodAll();
  Event_Wait(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_Wait(30);
  Sanctum_NodAll();
  Event_Wait(120);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_Wait(40);
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
  Actor_WalkTo(ACTOR_HEALER, 200, 272);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_WEST_HEALER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_EAST_HEALER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_WaitForMoveById(ACTOR_HEALER);
  Actor_SetPosition(ACTOR_HEALER, 0, 0);
  Event_Wait(60);
  Sanctum_ShowAllEmotes();
  Event_Wait(60);
  Event_Wait(120);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WEST_HEALER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_EAST_HEALER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_WEST_HEALER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_WEST_HEALER, 0);
  Event_Wait(120);
  Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(60);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_WEST_HEALER, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_WEST_HEALER, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(80);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
  Actor_RunRepeatedMotion(ACTOR_WEST_HEALER, 1);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_WEST_HEALER, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_EAST_HEALER, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_WEST_HEALER, 2);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_EAST_HEALER, 2);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_EAST_HEALER, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_EAST_HEALER, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_WEST_HEALER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_EAST_HEALER, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkTo(ACTOR_WEST_HEALER, 200, 272);
  Actor_WalkToAndWait(ACTOR_EAST_HEALER, 200, 272);
  Actor_SetPosition(ACTOR_WEST_HEALER, 0, 0);
  Actor_SetPosition(ACTOR_EAST_HEALER, 0, 0);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(60);
  Actor_WalkToAndWait(ACTOR_GARETS_GRANDFATHER, 200, 136);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_DORA, 168, 168);
  Actor_FaceActor(ACTOR_DORA, ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_DORA, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_DORA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
}

/// Copies the healer into the afterimage actor.
static void Sanctum_ShowHealerAfterimage(void) {
  u32 entryIndex;
  struct Actor *afterimage;
  struct Actor *actor;
  struct Sprite *sprite;

  actor = Actor_Get(ACTOR_HEALER);
  if (actor != 0) {
    Actor_SetPositionFixed(ACTOR_HEALER_AFTERIMAGE, actor->x.fixed,
                           actor->z.fixed);
  }
  Actor_SetAnimation(ACTOR_HEALER_AFTERIMAGE, ANIM_STILL);
  afterimage = Actor_Get(ACTOR_HEALER_AFTERIMAGE);
  actor = Actor_Get(ACTOR_HEALER);
  afterimage->facing = actor->facing;
  actor = Actor_Get(ACTOR_HEALER_AFTERIMAGE);
  actor->update = Sanctum_UpdateHealerAfterimage;
  actor = Actor_Get(ACTOR_HEALER_AFTERIMAGE);
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
  Actor_Get(ACTOR_HEALER_AFTERIMAGE)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 2;
}

static void Sanctum_HideHealerAfterimage(void) {
  struct Actor *state = Actor_Get(ACTOR_HEALER_AFTERIMAGE);
  state->update = 0;
  Actor_SetPosition(ACTOR_HEALER_AFTERIMAGE, 0, 0);
}

/// Cycles the afterimage through four offsets from the healer.
static void Sanctum_UpdateHealerAfterimage(struct Actor *actor) {
  struct Actor *healer;

  healer = Actor_Get(ACTOR_HEALER);
  actor->targetX = actor->x.fixed = healer->x.fixed;
  actor->targetY = actor->y.fixed = healer->y.fixed;
  actor->targetZ = actor->z.fixed = healer->z.fixed - px(2);
  switch (gFrameCount & 3) {
  case 0:
    actor->targetX = actor->x.fixed = healer->x.fixed - px(7) / 2;
    break;
  case 1:
    actor->targetX = actor->x.fixed = healer->x.fixed + px(3);
    break;
  case 2:
    actor->targetY = actor->y.fixed = healer->y.fixed + px(2);
    break;
  case 3:
    actor->targetZ = actor->z.fixed = healer->z.fixed;
    break;
  }
}

/// Advances an effect around its watched actor on an elliptical path.
static void Sanctum_UpdateOrbitingEffect(struct Actor *effect) {
  struct Actor *anchor = effect->watchedActor;
  u16 orbitAngle = effect->timer;
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

  effect->timer += effect->userValue;
}

/// Creates an orbiting effect with the requested angular step.
static void Sanctum_SpawnOrbitingEffect(s32 actorId, s32 angleStep) {
  s32 orbitStep;
  struct Actor *center;
  struct Actor *effect;
  struct Sprite *sprite;
  struct Sprite *sourceSprite;

  orbitStep = angleStep;
  center = Actor_Get(actorId);
  if (center != 0) {
    effect = FieldActor_Create(SPRITE_SPARKLE, center->x.fixed,
                               center->y.fixed + px(45), center->z.fixed);
    if (effect != 0) {
      sprite = effect->sprite;
      Actor_StartScript(effect, gSanctum_OwnerEffectScript);
      effect->motionFlags = 0;
      effect->timer = 0;
      effect->userValue = orbitStep;
      effect->update = Sanctum_UpdateOrbitingEffect;
      sprite->flags = 0;
      sourceSprite = center->sprite;
      effect->watchedActor = center;
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

  Actor_SetPosition(ACTOR_HEALER, 0, 0);
  Actor_SetPosition(ACTOR_WEST_HEALER, 0, 0);
  Actor_SetPosition(ACTOR_EAST_HEALER, 0, 0);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_SetPosition(ACTOR_GARETS_GRANDFATHER, 0, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  PaletteFade_StartInterpolation(1);
  Event_Wait(1);
  window = Window_Create(QUESTION_X, 7, QUESTION_WIDTH, 5, 1);
  text = MsgSanctumGreatHealer;
  Text_DrawMessage(text, window, 16, 0);
  if (Save_CountFilesExcludingSent(1) == 0) {
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
    Event_Wait(1);
  }
  MenuArrow_Release(slot);
  Window_Close(window, 1);
  return choice;
}

/// Vale Sanctum, the quest's account: Isaac and Garet act out, in sepia, what
/// happened in Sol Sanctum.
static void Sanctum_ReenactSolSanctum(void) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
  Actor_SetSpeed(ACTOR_GARET, 1.5);
  Actor_RunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(15);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(ACTOR_PARTY_LEADER, 184, 168);
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_GARET, 200, 168);
  Event_Wait(1);
  Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Jump(ACTOR_GARET, 2, 0);
  Event_Wait(15);
  Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
  Event_Wait(5);
  Actor_Jump(ACTOR_GARET, 2, 0);
  Event_Wait(25);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_FaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(5);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(5);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(15);
  Actor_SetAnimation(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_HEALER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_WEST_HEALER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_EAST_HEALER, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_DORA, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Event_Wait(15);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(10);
  // Illustrate the account with the floating item icon.
  Sanctum_RaiseItemIcon(ITEM_MARS_STAR, px(184), px(27), px(168));
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(10);
  Actor_Jump(ACTOR_GARET, 4, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(15);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  Actor_Jump(ACTOR_GARET, 4, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(15);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Actor_FaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(10);
  Actor_Get(ACTOR_HEALER)->timer = GARET;
  Actor_Get(ACTOR_HEALER)->update =
      (void (*)(struct Actor *))Sanctum_FaceActorNamedByTimer;
  Actor_Get(ACTOR_GARETS_GRANDFATHER)->timer = GARET;
  Actor_Get(ACTOR_GARETS_GRANDFATHER)->update =
      (void (*)(struct Actor *))Sanctum_FaceActorNamedByTimer;
  Actor_WalkToAndWait(ACTOR_GARET, 196, 180);
  Actor_WalkToAndWait(ACTOR_GARET, 184, 184);
  Actor_WalkToAndWait(ACTOR_GARET, 180, 180);
  Actor_WalkToAndWait(ACTOR_GARET, 168, 168);
  Actor_WalkToAndWait(ACTOR_GARET, 180, 156);
  Actor_WalkTo(ACTOR_GARET, 200, 104);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 192, 168);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_GARET);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(15);
  Actor_Get(ACTOR_GARETS_GRANDFATHER)->update = NULL;
  Actor_Get(ACTOR_HEALER)->update = NULL;
  Actor_StartRepeatedMotion(ACTOR_HEALER, 2);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
  Actor_SetAnimation(ACTOR_HEALER, ANIM_STILL);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(10);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
  Event_Wait(20);
  Actor_Jump(ACTOR_PARTY_LEADER, 2, 0);
  Event_Wait(20);
  Event_Wait(15);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Event_Wait(60);
  Actor_WalkToAndWait(ACTOR_GARET, 208, 168);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Event_Wait(10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(10);
  // Repeat the synchronized eight-point paths around the room.
  Actor_SetScript(ACTOR_PARTY_LEADER, gSanctum_LeaderCircleScript);
  Actor_SetScript(ACTOR_GARET, gSanctum_GaretCircleScript);
  Actor_WaitForScriptEndById(ACTOR_PARTY_LEADER);
  Actor_WaitForScriptEndById(ACTOR_GARET);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
  Actor_SetSpeed(ACTOR_GARET, 1.5);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_Jump(ACTOR_GARET, 6, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WEST_HEALER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
  Event_Wait(1);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_DORA, 0);
  Event_Wait(1);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_WEST_HEALER, 0);
  Event_Wait(1);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 192, 168);
  Actor_WalkToAndWait(ACTOR_GARET, 208, 168);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_DORA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  Event_Wait(60);
}

/// Shows an item icon rising from the supplied fixed-point position.
static void Sanctum_RaiseItemIcon(s32 item, s32 fixedX, s32 fixedY,
                                  s32 fixedZ) {
  struct Actor *icon;
  struct Sprite *sprite;
  u8 *transferBuffer;
  u32 frame;

  icon = FieldActor_Create(SPRITE_BLANK, fixedX, fixedY, fixedZ);
  {
    u8 cleared = 0;

    if (icon != 0) {
      Actor_StartScript(icon, gItemIcon_GrowScript);
      sprite = icon->sprite;
      sprite->flags = cleared;
      sprite->count = cleared;
      sprite->image.fullColor = 0;
      sprite->image.palette = 0;
      icon->velocityY = px(2);
      icon->gravity = px(1) / 4;
      transferBuffer =
          HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
      Icon_DrawItem(item);
      VramCache_Load(sprite->slot, 128, transferBuffer + 1024);
      HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
      for (frame = 0; frame < 60; frame++) {
        if ((u32)(icon->velocityY + 255) <= 510)
          icon->motionFlags = 0;
        wait(1);
      }
      Actor_StartScript(icon, gSanctum_ItemIconEndScript);
    }
  }
}

/// Moves a spark upward, cycles its scale, and expires its lifetime.
static void Sanctum_UpdateRisingSpark(struct Actor *spark) {
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

  actor = Actor_Get(id);
  if (actor == 0)
    return;
  randomOffset = Math_RemainderUnsigned(Random_Next(), 20);
  x = actor->x.fixed;
  x += randomOffset << 16;
  x -= px(10);
  spark = FieldActor_Create(
      SPRITE_GLINT, x, actor->y.fixed + ((Random_Next() & 15) << 16) - px(8),
      actor->z.fixed);
  if (spark == 0)
    return;
  sprite = spark->sprite;
  spark->motionFlags = 0;
  spark->timer = Math_RemainderUnsigned(Random_Next(), 10) + 5;
  spark->userValue = Math_RemainderUnsigned(Random_Next(), 60) + 30;
  spark->update = Sanctum_UpdateRisingSpark;
  sprite->flags = 0;
  sprite->image.priority = actor->sprite->image.priority;
}

/// Copies two blocks of map grid cells for the quest's visits, the cell
/// pictures of one and the cell behaviors of the other.
static void Sanctum_RestoreQuestFloor(void) {
  MapGrid_CopyCellsTo(0, 64, 11, 68, 3, 2);
  MapGrid_CopyCellAttributes(11, 10, 3, 2, 11, 8);
  wait(1);
}

static s32 Sanctum_IsLeaderFacingHealer(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);

  if ((u32)((actor->facing + 0x5fff) << 16) <= 0x3ffe0000) {
    return 1;
  }
  return 0;
}

/// Vale Sanctum: talking to the Great Healer outside the quest.
///
/// From the front the healer opens the Sanctum's services. Otherwise the words
/// follow the story, from "Wielders of Psynergy are called Adepts" before the
/// party has left Vale to the closed path to Sol Sanctum once it has reached
/// Kalay.
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
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_End();
}

/// Vault Sanctum: talking to the healer.
///
/// From the front the healer opens the Sanctum's services. Otherwise the healer
/// speaks of the gold statue or, on the later visit, of the earthquakes.
///
/// Its case for the quest's entrances (SANCTUM_FROM_VALE_ERUPTION,
/// SANCTUM_FROM_QUEST_RESTART and SANCTUM_QUEST_UNUSED) passes to
/// Sanctum_ReconsiderQuest, though their triggers (gSanctum_QuestTriggers) call
/// that directly.
static void Sanctum_TalkToHealerOnVisit(void) {
  s32 entrance;

  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Sanctum_Run(ACTOR_HEALER);
    return;
  }

  Event_Start();

  entrance = gMainRoutineWork.entrance;

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

  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_End();
}

/// Bilibin Sanctum: talking to the healer, about Kolima's curse, lifted or not.
///
/// From the front, the healer opens the Sanctum's services.
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
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_End();
}

/// Altin Sanctum: talking to the healer, about the beasts in the mine, or the
/// party's saving Altin from them.
///
/// From the front, the healer opens the Sanctum's services.
static void Sanctum_TalkAboutAltinMonsters(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Sanctum_Run(ACTOR_HEALER);
    return;
  }

  Event_Start();
  Event_SetMessageId(MsgSanctumWasAfterEerieNightWhen);
  if (GameFlag_Get(FLAG_HYDROS_STATUES_DEFEATED) != 0)
    Event_SetMessageId(MsgSanctumSavedAltinFromMonstersClearly);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_End();
}

/// Xian Sanctum: talking to the healer, who came to Xian from very distant
/// lands.
///
/// From the front, the healer opens the Sanctum's services.
static void Sanctum_TalkAboutJourneyFromXian(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Sanctum_Run(ACTOR_HEALER);
    return;
  }

  Event_Start();
  Event_SetMessageId(MsgSanctumCameXianFromVeryDistant);
  // "I came to Xian from very distant lands to spread good will."
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_End();
}

// clang-format off
u32 gSanctum_LeaderCircleScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_SPEED(2.5),
    SC_ACCELERATION(0.5),
    SC_LABEL(1),
    SC_MOVE(204, 0, 188),
    SC_SYNC,
    SC_MOVE(216, 0, 192),
    SC_SYNC,
    SC_MOVE(236, 0, 188),
    SC_SYNC,
    SC_MOVE(240, 0, 168),
    SC_SYNC,
    SC_MOVE(236, 0, 156),
    SC_SYNC,
    SC_MOVE(216, 0, 144),
    SC_SYNC,
    SC_MOVE(204, 0, 156),
    SC_SYNC,
    SC_MOVE(192, 0, 168),
    SC_SYNC,
    SC_LOOP(4, SC_LABEL(1)),
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

u32 gSanctum_GaretCircleScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_SPEED(2.5),
    SC_ACCELERATION(0.5),
    SC_LABEL(1),
    SC_MOVE(204, 0, 156),
    SC_SYNC,
    SC_MOVE(184, 0, 144),
    SC_SYNC,
    SC_MOVE(172, 0, 156),
    SC_SYNC,
    SC_MOVE(160, 0, 168),
    SC_SYNC,
    SC_MOVE(172, 0, 188),
    SC_SYNC,
    SC_MOVE(184, 0, 192),
    SC_SYNC,
    SC_MOVE(204, 0, 188),
    SC_SYNC,
    SC_MOVE(208, 0, 168),
    SC_SYNC,
    SC_LOOP(4, SC_LABEL(1)),
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

#include "shared/item_icon_grow_script.h"

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
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        200, 0, 130, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 152, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 152, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

#if TBS_PATCH_1
struct ActorPlacement gSanctum_IvanActorPlacement[] = {
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};
#endif

struct ActorPlacement gSanctum_VaultBilibinActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        200, 0, 130, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        200, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SANCTUM_GOLD_STATUE, CONDITION_ALWAYS, 7,
        8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gSanctum_QuestActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        208, 0, 168, FACING_NORTH_NORTHEAST, 3, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        224, 0, 216, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        246, 0, 198, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 208, FACING_NORTH_NORTHEAST, 3, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 120, FACING_SOUTH_SOUTHWEST, 3, 0),
    ACTOR_PLACEMENT(SPRITE_WISE_ONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gSanctum_AltinXianActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        200, 0, 132, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

struct Trigger gSanctum_ValeTriggers[] = {
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkToHealer) },
    { TRIGGER_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumOurPsynergyWasGivenToUs) },
    { TRIGGER_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumPsynergyIsAPowerThatShould) },
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { TRIGGER_TABLE_END },
};

struct Trigger gSanctum_VaultTriggers[] = {
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkToHealerOnVisit) },
    { TRIGGER_TALK, ACTOR_SANCTUM_MEMBER, FLAG_VAULT_THIEVES_JAILED, EVENT(Sanctum_TalkAboutGoldenStatue) },
    { TRIGGER_TALK, ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT(Sanctum_TalkAboutGoldenStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumGoOnIfThatEasesThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT(Sanctum_ReadMindAboutGoldenStatue) },
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VAULT },
    { TRIGGER_TABLE_END },
};

struct Trigger gSanctum_VaultRevisitTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VAULT_REVISIT },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkToHealerOnVisit) },
    { TRIGGER_TALK, ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT(Sanctum_TalkAboutGoldenStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumIHopeTheEarthquakesArentA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT(Sanctum_ReadMindAboutGoldenStatue) },
    { TRIGGER_TABLE_END },
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
    { TRIGGER_TABLE_END },
};

struct Trigger gSanctum_ValeTriggersAfterLeaving[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkToHealer) },
    { TRIGGER_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumThePowerOfMtAlephStems) },
    { TRIGGER_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_AskAboutPsynergyChanges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumThePowerOfMyPsynergyHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumTheAngaranContinentWillCertainlySee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumMyPsynergyHasntChangedAtAll) },
    { TRIGGER_TABLE_END },
};

struct Trigger gSanctum_ValeTriggersAfterKalay[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkToHealer) },
    { TRIGGER_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_AskAboutNewAdepts) },
    { TRIGGER_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWhyIsItThatDifferentAdepts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItllBeDifficultToReachSol) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumISeeABeautifulGirlHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItsNotFairIWantTo) },
    { TRIGGER_TABLE_END },
};

struct Trigger gSanctum_AltinTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_ALTIN },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkAboutAltinMonsters) },
    // Unused: a copy of the row above, which always answers first.
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkAboutAltinMonsters) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgSanctumPleaseSaveTheHelplessInAltin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumAlmightyFateMustHaveHeardMe) },
    { TRIGGER_TABLE_END },
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
    { TRIGGER_TABLE_END },
};

struct Trigger gSanctum_XianTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_XIAN },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkAboutJourneyFromXian) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWhenYouStayInXianFor) },
    { TRIGGER_TABLE_END },
};
// clang-format on

/// The choice cursor's sideways bob in pixels, one step every two frames.
s32 gSanctum_CursorBob[] = {
    0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 3, 2, 2, 1, 1, 1,
};

s32 gSanctum_OwnerEffectScript[] = {
    SC_ADD(SW_Y, -px(1) / 2),
    SC_ADD(SW_TARGET_Y, -px(1) / 2),
    SC_WAIT(1),
    SC_LOOP(96, SC_TOP),
    SC_RELEASE,
};

s32 gSanctum_SparkEndScript[] = {
    SC_RELEASE,
};
