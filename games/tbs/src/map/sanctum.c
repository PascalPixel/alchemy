/// @file
/// The Sanctum of Vale and of four towns on the party's way, which holds six
/// places in one scene (SceneId_Sanctum): the entrance the party comes in by
/// decides which, and Sanctum_GetPlacements and Sanctum_GetEvents choose that
/// place's tables by it. Spoken to from the front, each place's healer opens
/// the Sanctum's services (Engine_SanctumOpen); from any other side the healer
/// talks. Each place's door leads back out to its own town.
///
/// - Vale Sanctum (SANCTUM_FROM_VALE, SANCTUM_FROM_VALE_ERUPTION,
///   SANCTUM_VALE_UNUSED and SANCTUM_FROM_QUEST_RESTART), the Great Healer's,
///   entered from Vale at
///   SANCTUM_FROM_VALE. Before the trip up Mt. Aleph, the Great Healer and two
///   other healers (ACTOR_WEST_HEALER and ACTOR_EAST_HEALER) speak of Psynergy.
///   After the eruption the Great Healer brings Isaac and Garet in from Vale
///   (vale_eruption.c) to SANCTUM_FROM_VALE_ERUPTION, where Dora, the Mayor
///   and the healers hear what happened on the mountain, acted out in sepia
///   (Sanctum_ReenactSolSanctum): the colour buffer turns every background
///   and sprite palette to PALETTE_TRANSFORM_SEPIA for the account and fades
///   back to PALETTE_TRANSFORM_PLAIN over 60 frames after it
///   (https://alchemy.pascalpixel.com/places/sanctum/account-sepia.gif: the
///   account's first ten seconds and its last five, beside the 16 background
///   and 16 sprite palettes). The Great Healer asks Isaac to take on the Wise
///   One's quest (Sanctum_RequestQuestAcceptance). Each time his Psynergy
///   shows, ACTOR_HEALER_AFTERIMAGE, a copy of him in SPRITE_COLOR_RED_TINT,
///   jumps round him by a few pixels a frame (Sanctum_UpdateHealerAfterimage)
///   while effects circle him (Sanctum_SpawnOrbitingEffect)
///   (https://alchemy.pascalpixel.com/places/sanctum/healer-afterimages.gif:
///   the screen, the healer boxed, beside the sprites and the sprites without
///   the afterimage). Accepting plays
///   Sanctum_ReceiveWiseOnesMission and sets FLAG_SANCTUM_QUEST_ACCEPTED, and
///   SANCTUM_TO_VALE_GATE takes the two to SceneId_ValeGate (vale_gate.c).
///   Refusing leaves the room to gSanctum_QuestEvents: the Great Healer asks
///   again when spoken to (Event_ReconsiderQuest), the door draws a warning
///   (FLAG_SCENE_TEMP_00), and leaving anyway ends the game
///   (Event_LeaveAfterRefusing): `And so the world began drifting toward its
///   fated destruction...` From there the player can start again from the
///   Great Healer, by SANCTUM_TO_QUEST_RESTART to SANCTUM_FROM_QUEST_RESTART,
///   which skips the account (FLAG_SCENE_TEMP_01), or leave by
///   SANCTUM_TO_CLEAR to SceneId_Clear. After the party has left Vale
///   (FLAG_VALE_PARTY_LEFT), and again after it has reached Kalay
///   (FLAG_KALAY_REACHED), the three talk of what the eruption and the Wise One
///   changed.
/// - Vault Sanctum (SANCTUM_FROM_VAULT and SANCTUM_FROM_VAULT_INTERIORS). Its
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

/// The placed actors, numbering the rows of the table Sanctum_GetPlacements
/// returns for the entrance; the party's rows take their own ids. Every
/// place's first row is its healer.
enum SanctumActor {
  /// The place's healer: the Great Healer in Vale Sanctum, its own healer
  /// elsewhere, a man with a white beard in a tall hat and a long purple and
  /// blue robe.
  /// ★ Named from the game: "What is it, Great Healer?"
  /// (MsgSanctumWeHadIdeaTrueSanctum)
  ACTOR_HEALER = ACTOR_FIRST_PLACED,
  // Vale Sanctum (gSanctum_ValePlacements, and gSanctum_QuestPlacements at the
  // quest).
  /// The western of Vale Sanctum's two other healers, men with long brown hair
  /// and beards in long robes, as Mind Read hears a villager in
  /// vale_interiors_shops.c call the Sanctum's people: "The healers
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
  // The quest (gSanctum_QuestPlacements).
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
  // Vault Sanctum and Bilibin Sanctum (gSanctum_VaultBilibinPlacements).
  /// The other person of the Sanctum, in the western healer's sprite (a man
  /// with long brown hair and a beard in a long robe), who wanders, and who
  /// speaks of the healer as someone else and of "our teachings" and "our
  /// meditations". In Vault: "I wonder if we'll ever see our stolen gold statue
  /// again..."
  /// ★ Named from the game: "stolen gold statue" (MsgSanctumWonderIfEverSeeOur)
  ACTOR_SANCTUM_MEMBER = ACTOR_FIRST_PLACED + 1,
  /// Vault Sanctum's gold statue (the sprite of the one a chest gives back in
  /// vault_interiors.c), placed off the map until the thieves are jailed
  /// (FLAG_VAULT_THIEVES_JAILED).
  /// ★ Named from the game: "our stolen gold statue"
  /// (MsgSanctumWonderIfEverSeeOur)
  ACTOR_GOLD_STATUE
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
static void Sanctum_SpawnActorSpark(s32 id);
static void Sanctum_RestoreQuestFloor(void);

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 44 to 42 bytes in tbs-en.
#ifdef UBFIX
/// Faces the actor whose ID this callback stores in the timer field.
static void Sanctum_FaceActorNamedByTimer(union FieldObject *fieldObject) {
#else
/// Faces the actor whose ID this callback stores in the timer field.
static s32 Sanctum_FaceActorNamedByTimer(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct FieldActor *target;

  target = Object_GetById((s16)actor->timer);
  actor->facing = CalculateFacingAngle(target->z.fixed - actor->z.fixed,
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

const struct SceneRegion *Sanctum_GetRegions(void) { return NULL; }

const u32 *Sanctum_GetExits(void) { return gSanctum_Exits; }

/// Chooses the actors by entrance: Vale Sanctum's Great Healer and two other
/// healers (SANCTUM_FROM_VALE and SANCTUM_VALE_UNUSED), the quest's gathering
/// (SANCTUM_FROM_VALE_ERUPTION and SANCTUM_FROM_QUEST_RESTART), the healer and
/// ACTOR_SANCTUM_MEMBER of Vault Sanctum and Bilibin Sanctum
/// (SANCTUM_FROM_VAULT, SANCTUM_FROM_VAULT_REVISIT,
/// SANCTUM_FROM_VAULT_INTERIORS and SANCTUM_FROM_BILIBIN), the healer alone in
/// Altin Sanctum and Xian Sanctum (SANCTUM_FROM_ALTIN and SANCTUM_FROM_XIAN),
/// and nobody otherwise.
const struct ScenePlacement *Sanctum_GetPlacements(void) {
  switch (gGameState.entrance) {
  case SANCTUM_FROM_VALE:
  case SANCTUM_VALE_UNUSED:
    return gSanctum_ValePlacements;

  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_VAULT_REVISIT:
  case SANCTUM_FROM_VAULT_INTERIORS:
  case SANCTUM_FROM_BILIBIN:
    return gSanctum_VaultBilibinPlacements;

  case SANCTUM_FROM_VALE_ERUPTION:
  case SANCTUM_FROM_QUEST_RESTART:
    return gSanctum_QuestPlacements;

  case SANCTUM_FROM_ALTIN:
  case SANCTUM_FROM_XIAN:
    return gSanctum_AltinXianPlacements;

  default:
    return gSanctum_NoPlacements;
  }
}

/// Chooses the events by entrance, and in Vale Sanctum before and after the
/// quest by story flag: gSanctum_ValeEvents before the party has left Vale,
/// gSanctum_ValeEventsAfterLeaving once it has (FLAG_VALE_PARTY_LEFT) and
/// gSanctum_ValeEventsAfterKalay once it has reached Kalay
/// (FLAG_KALAY_REACHED); gSanctum_QuestEvents for the quest's visits
/// (SANCTUM_FROM_VALE_ERUPTION, SANCTUM_FROM_QUEST_RESTART and
/// SANCTUM_QUEST_UNUSED).
const struct SceneEvent *Sanctum_GetEvents(void) {
  switch (gGameState.entrance) {
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

/// Vault Sanctum: talking to ACTOR_SANCTUM_MEMBER, about the stolen gold statue
/// until the thieves are jailed (FLAG_VAULT_THIEVES_JAILED) and about fate
/// afterwards; on the later visit (SANCTUM_FROM_VAULT_REVISIT), the statue
/// polished.
static void Event_TalkAboutGoldenStatue(void) {
  Engine_EventStart();

  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
    Engine_EventSetMessage(MsgSanctumWhenStrayFromYourWorldly);
  } else {
    Engine_EventSetMessage(MsgSanctumHealerMustWorriedAboutNever);
  }

  if (gGameState.entrance == SANCTUM_FROM_VAULT_REVISIT) {
    Engine_EventSetMessage(MsgSanctumPolishedGoldStatueReturnedUs);
  }

  Engine_ActorSetAnimation(ACTOR_SANCTUM_MEMBER, 1);
  Engine_ActorFaceEachOther(ACTOR_SANCTUM_MEMBER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(2);
  Engine_EventShowMessage(ACTOR_SANCTUM_MEMBER, 0);
  Engine_EventEnd();
}

/// Vault Sanctum: Mind Read on ACTOR_SANCTUM_MEMBER, about the stolen statue,
/// or once it is back, about Isaac awakening to the Sanctum's teachings; on the
/// later visit, the statue cleaned up.
static void Event_ReadMindAboutGoldenStatue(void) {

  Engine_EventStart();

  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgSanctumWonderIfEverSeeOur);
  } else {
    Engine_EventSetMessage(MsgSanctumChildHasAwakenedOurTeachings);
  }

  if (gGameState.entrance == SANCTUM_FROM_VAULT_REVISIT) {
    Engine_EventSetMessage(MsgSanctumDirtyGoldenStatueCleanedUp);
  }

  Engine_ActorStop(ACTOR_SANCTUM_MEMBER);
  Engine_ActorSetAnimation(ACTOR_SANCTUM_MEMBER, 1);
  Engine_EventWait(2);
  Engine_EventShowMessage(ACTOR_SANCTUM_MEMBER, 0);
  Actor_SetBuiltinAction(ACTOR_SANCTUM_MEMBER, 2);
  Engine_EventEnd();
}

/// Vale Sanctum: talking to the Great Healer after Isaac has refused the quest.
/// "Are you sure?" Changing his mind, Isaac can accept after all, and the
/// quest goes on as if he had accepted at once.
static void Event_ReconsiderQuest(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgSanctumAreYouSure);
  Engine_EventOpenMessage(ACTOR_HEALER, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(20);
  } else {
    Engine_EventWait(20);
    Engine_EventOpenMessage(ACTOR_HEALER, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      Engine_EventOpenMessage(ACTOR_HEALER, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        if (Object_GetById(ACTOR_HEALER)->facing >= FACING_NORTHWEST &&
            Object_GetById(ACTOR_HEALER)->facing <= FACING_NORTHEAST) {
          Actor_SetSpeed(ACTOR_HEALER, PIXELS(1) / 2, PIXELS(1) / 4);
          Engine_ActorFaceDirection(ACTOR_HEALER, FACING_EAST, 0);
          Engine_EventWait(10);
          Object_GetById(ACTOR_HEALER)->steeringFlags &= ~ACTOR_FACES_MOTION;
          Engine_ActorWalkToAndWait(ACTOR_HEALER, 152, 120);
          Engine_EventWait(1);
          Object_GetById(ACTOR_HEALER)->steeringFlags |= ACTOR_FACES_MOTION;
          Engine_EventWait(20);
          Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
          Engine_EventWait(20);
          Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 168, 120);
          Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 192, 168);
          Engine_EventWait(20);
          Engine_ActorWalkToAndWait(ACTOR_HEALER, 168, 120);
          Engine_ActorFaceDirection(ACTOR_HEALER,
                                    FACING_SOUTHEAST + FACING_STEP, 0);
          Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
        } else {
          Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 192, 168);
          Engine_EventWait(20);
          Engine_ActorFaceDirection(ACTOR_HEALER,
                                    FACING_SOUTHEAST + FACING_STEP, 0);
          Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
        }
        Sanctum_ReceiveWiseOnesMission();
        Engine_ColorBufferApplyTarget(RGB(0, 0, 0), PALETTE_HALVES_BOTH);
        Engine_ColorBufferInterpolate(120);
        Engine_EventWait(120);
        Engine_AudioPlayCue(MUS_086);
        AudioCommand_WaitForCompletion();
        Engine_GameFlagSet(FLAG_SANCTUM_QUEST_ACCEPTED);
        Engine_EventRequestExit(SANCTUM_TO_VALE_GATE);
      }
    }
  }
  Engine_EventEnd();
}

#if TBS_PATCH_1
#define SANCTUM_NEW_FRIENDS_SPEAKER 2
#else
#define SANCTUM_NEW_FRIENDS_SPEAKER 1
#endif

/// Vale Sanctum, once the party has reached Kalay: talking to
/// ACTOR_WEST_HEALER, who asks about the party's new friends. "Isaac, are your
/// new friends Adepts by any chance?" Denied, a friend's silence says they must
/// be: in every edition but the Japanese, that friend is Ivan, placed for it;
/// in the Japanese, Garet.
static void Event_AskAboutNewAdepts(void) {
  Engine_EventStart();
#if TBS_PATCH_1
  Event_CallWithLastActiveObjectId(gSanctum_IvanPlacement);
  Engine_TaskWait(1);
#endif
  Engine_EventSetMessage(MsgSanctumIsaacYourNewFriendsAdepts);
  Engine_EventOpenMessage(ACTOR_WEST_HEALER, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventShowMessage(ACTOR_WEST_HEALER, 0);
  } else {
    gEventWork->message++;
    Event_ShowTwoMessagesAndWait(SANCTUM_NEW_FRIENDS_SPEAKER, 16, 1, 24, 1, 3,
                                 7, 16, 1, 14, 0);
    Engine_EventShowMessage(ACTOR_WEST_HEALER, 0);
  }
  Engine_EventEnd();
}

/// Vale Sanctum, once the party has left Vale: talking to ACTOR_EAST_HEALER,
/// who asks whether Isaac feels any change in his Psynergy.
static void Event_AskAboutPsynergyChanges(void) {

  Engine_EventStart();
  Engine_ActorFaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Engine_EventSetMessage(MsgSanctumDoFeelAnyChangeIn);
  Engine_EventOpenMessage(ACTOR_EAST_HEALER, 0);

  if (Engine_EventChooseYesNo(0, 0) == 1) {
    (gEventWork->message)++;
  }

  Engine_EventShowMessage(ACTOR_EAST_HEALER, 0);
  Engine_EventEnd();
}

/// Sets up the place for its entrance and plays its opening scene. Vault
/// Sanctum places ACTOR_GOLD_STATUE once the thieves are jailed; Vale Sanctum's
/// quest visits (SANCTUM_FROM_VALE_ERUPTION, and SANCTUM_FROM_QUEST_RESTART
/// after a refusal, which sets FLAG_SCENE_TEMP_01) restore the floor and play
/// the request, unless the party is returning after a battle.
s32 Sanctum_Initialize(void) {
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, PALETTE_HALVES_BOTH);
  Engine_ColorBufferInterpolate(1);
  Engine_EventWait(1);
  switch (gGameState.entrance) {
  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_VAULT_REVISIT:
  case SANCTUM_FROM_VAULT_INTERIORS:
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Actor_SetPosition(ACTOR_GOLD_STATUE, PIXELS(200), PIXELS(80));
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
    Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Sanctum_RequestQuestAcceptance();
    }
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    break;
  }
  return 0;
}

/// Vale Sanctum: heading for the door after refusing the quest (cell code 2,
/// until FLAG_SCENE_TEMP_00 is set, which it sets). The Great Healer warns
/// that the choice is final: "Once you step outside the village, you cannot
/// take back your decision."
static void Event_WarnBeforeLeaving(void) {
  Engine_EventStart();
  Engine_CameraSetSpeed(PIXELS(1), PIXELS(1) / 8);
  Engine_CameraMoveToActor(GARET, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgSanctumOnceStepOutsideVillageCannot);
  Engine_EventShowMessage(ACTOR_HEALER, 0);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
  Engine_EventEnd();
}

/// Vale Sanctum: leaving after refusing the quest (cell code 1) ends the game.
/// The room turns sepia: `And so the world began drifting toward its fated
/// destruction...` Starting again from the Great Healer takes
/// SANCTUM_TO_QUEST_RESTART, back to SANCTUM_FROM_QUEST_RESTART; the other
/// choices take SANCTUM_TO_CLEAR, to SceneId_Clear.
static void Event_LeaveAfterRefusing(void) {
  struct ColorBuffer *paletteWork;

  Engine_EventStart();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetAnimation(GARET, 0);
  Engine_ActorSetAnimation(ACTOR_DORA, 0);
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 0);
  Engine_ActorSetAnimation(ACTOR_HEALER, 0);
  Engine_ActorSetAnimation(ACTOR_WEST_HEALER, 0);
  Engine_ActorSetAnimation(ACTOR_EAST_HEALER, 0);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_SEPIA, PALETTE_HALVES_BOTH);
  Engine_ColorBufferInterpolate(120);
  Engine_EventWait(180);
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
  Engine_EventWait(1);
  UiText_ShowCenteredMessage(MsgSanctumWorldBeganDrifting, 1, 0);
  Engine_ColorBufferApplyTarget(RGB(0, 0, 0), PALETTE_HALVES_BOTH);
  Engine_ColorBufferInterpolate(120);
  Engine_EventWait(120);
  Engine_EventWait(60);
  if (Sanctum_ChooseRestartOption() == 0) {
    Engine_EventEnd();
    Engine_EventRequestExit(SANCTUM_TO_QUEST_RESTART);
  } else {
    Engine_EventEnd();
    Engine_EventRequestExit(SANCTUM_TO_CLEAR);
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
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  actor->sprite->image.priority =
      Object_GetById(ACTOR_PARTY_LEADER)->sprite->image.priority;
  actor->sprite->shadow.priority =
      Object_GetById(ACTOR_PARTY_LEADER)->sprite->image.priority;
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Has Isaac, Garet, Dora, the Mayor and the two healers nod, waiting for the
/// last of them.
static inline void Sanctum_NodAll(void) {
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimation(ACTOR_DORA, 3);
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 3);
  Engine_ActorSetAnimation(ACTOR_WEST_HEALER, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_EAST_HEALER, 3);
}

/// Starts the same six's repeated motions and waits for the last of them.
static inline void Sanctum_StartAllRepeatedMotion(void) {
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 1);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
  Engine_ActorStartRepeatedMotion(ACTOR_WEST_HEALER, 1);
  Engine_ActorRunRepeatedMotion(ACTOR_EAST_HEALER, 1);
}

/// Shows the same emote on the same six.
static inline void Sanctum_ShowAllEmotes(void) {
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
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
/// SANCTUM_TO_VALE_GATE; refusing leaves the room to gSanctum_QuestEvents.
static void Sanctum_RequestQuestAcceptance(void) {
  u32 effectIndex;

  Engine_EventStart();
  // A repeat visit skips the account of Sol Sanctum.
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_01) != 0) {
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
    Camera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
    Engine_CameraWaitForMove();
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gEventWork->transitionFrames = 64;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(120);
  } else {
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_SEPIA, PALETTE_HALVES_BOTH);
    Engine_ColorBufferInterpolate(1);
    Engine_EventWait(1);
    Camera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
    Engine_CameraWaitForMove();
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    // Reenact the journey before explaining the quest.
    Sanctum_ReenactSolSanctum();
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, PALETTE_HALVES_BOTH);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(100);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_STAND);
    Engine_ActorSetAnimation(GARET, ANIMATION_STAND);
    Engine_EventWait(30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
    Actor_FaceActor(GARET, ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(20);
    Actor_SetBuiltinAction(ACTOR_HEALER, ACTOR_ACTION_IDLE);
    Actor_SetBuiltinAction(ACTOR_GARETS_GRANDFATHER, ACTOR_ACTION_IDLE);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_GARETS_GRANDFATHER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_WEST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_EAST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_SetSpeed(ACTOR_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
    Engine_EventSetMessage(MsgSanctumWeHadIdeaTrueSanctum);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
    Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    Actor_FaceActor(GARET, ACTOR_DORA, 0);
    Engine_EventWait(20);
    Event_OpenMessage(ACTOR_DORA, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
      Engine_EventWait(20);
      Event_ShowMessage(ACTOR_DORA, 0);
      gEventWork->message++;
    } else {
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
      Engine_EventWait(20);
      gEventWork->message++;
      Event_ShowMessage(ACTOR_DORA, 0);
    }
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_WEST_HEALER, 1);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WEST_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_WEST_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_WEST_HEALER, 0);
    Engine_EventWait(20);
    Event_OpenMessage(ACTOR_WEST_HEALER, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(ACTOR_WEST_HEALER, 3);
      Engine_EventWait(20);
      Event_ShowMessage(ACTOR_WEST_HEALER, 0);
      gEventWork->message++;
    } else {
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(ACTOR_WEST_HEALER, 4);
      Engine_EventWait(20);
      gEventWork->message++;
      Event_ShowMessage(ACTOR_WEST_HEALER, 0);
    }
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
    Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
    Engine_ActorSetAnimation(ACTOR_DORA, 3);
    Engine_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 3);
    Engine_ActorSetAnimation(ACTOR_WEST_HEALER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_EAST_HEALER, 3);
    Engine_EventWait(30);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    0);
    Engine_EventWait(60);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_WalkToAndWait(ACTOR_GARETS_GRANDFATHER, 0xe0, 0x78);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_DORA, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 3);
    Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Engine_EventWait(60);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH + FACING_STEP,
                        0);
    Engine_EventWait(60);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 3);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_HEALER, FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH + FACING_STEP,
                        0);
    Engine_EventWait(60);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Engine_EventWait(50);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    0);
    Engine_EventWait(40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 1);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER,
                    EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(10);
    Event_ShowMessage(GARET, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GARET, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_DORA, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_WEST_HEALER, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_EAST_HEALER, 1);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(30);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Engine_EventWait(20);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 2);
    Engine_ActorSetAnimation(ACTOR_HEALER, 0);
    Actor_SetChildValue(ACTOR_HEALER, 0x100);
    Sanctum_ShowHealerAfterimage();
    Event_PlayCue(SE_196);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Engine_EventWait(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Engine_EventWait(16);
    for (effectIndex = 0; effectIndex < 6; effectIndex++) {
      Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
      Engine_EventWait(8);
    }
    Engine_EventWait(8);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Engine_EventWait(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Engine_EventWait(96);
    Engine_EventWait(32);
    Actor_SetChildValue(ACTOR_HEALER, 0);
    Engine_EventWait(30);
    // Return to the gathering and finish the departure conversation.
    Sanctum_HideHealerAfterimage();
    Engine_ActorSetAnimation(ACTOR_HEALER, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(ACTOR_GARETS_GRANDFATHER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_DORA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_WEST_HEALER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_EAST_HEALER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_EAST_HEALER, 1);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_EAST_HEALER, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(30);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
    Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
    Engine_ActorFaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_WEST_HEALER, 3);
    Engine_ActorSetAnimation(ACTOR_EAST_HEALER, 3);
    ObjectMotion_WaitForAnimationChange(ACTOR_EAST_HEALER);
    Engine_EventWait(30);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    Engine_ActorFaceEachOther(GARET, ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
    Engine_ActorSetAnimation(ACTOR_DORA, 3);
    Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
    Engine_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 3);
    ObjectMotion_WaitForAnimationChange(ACTOR_GARETS_GRANDFATHER);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 2);
    Engine_ActorSetAnimation(ACTOR_HEALER, 0);
    Actor_SetChildValue(ACTOR_HEALER, 0x100);
    Sanctum_ShowHealerAfterimage();
    Event_PlayCue(SE_196);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Engine_EventWait(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Engine_EventWait(16);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GARET, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_DORA, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_WEST_HEALER, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_EAST_HEALER, 1);
    for (effectIndex = 0; effectIndex < 6; effectIndex++) {
      Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
      Engine_EventWait(8);
    }
    Engine_EventWait(8);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Engine_EventWait(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, 0x1200);
    Engine_EventWait(128);
    Actor_SetChildValue(ACTOR_HEALER, 0);
    Engine_EventWait(30);
    Sanctum_HideHealerAfterimage();
    Engine_ActorSetAnimation(ACTOR_HEALER, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 2);
    Engine_EventWait(30);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                    0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
    Engine_EventWait(40);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GARET, 1);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_WEST_HEALER, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_EAST_HEALER, 1);
    Engine_ActorFaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
    Actor_SetAttachedEffect(ACTOR_GARETS_GRANDFATHER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_EventWait(60);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(GARET, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
    Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
    Engine_ActorSetAnimation(ACTOR_DORA, 3);
    Engine_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 3);
    Engine_ActorSetAnimation(ACTOR_WEST_HEALER, 3);
    Engine_ActorSetAnimation(ACTOR_EAST_HEALER, 3);
    ObjectMotion_WaitForAnimationChange(ACTOR_EAST_HEALER);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(50);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                    0);
    Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Engine_EventWait(80);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 1);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_DORA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_GARETS_GRANDFATHER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_WEST_HEALER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_EAST_HEALER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 4);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(40);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
    Engine_ActorFaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(60);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_DORA, 0);
    Engine_EventWait(60);
    Actor_WalkToAndWait(ACTOR_HEALER, 0xc8, 0x88);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceEachOther(ACTOR_HEALER, GARET, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(50);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(50);
    Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 1);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Engine_EventWait(60);
    Actor_SetAttachedEffect(ACTOR_DORA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ActorRunRepeatedMotion(ACTOR_DORA, 1);
    Event_ShowMessage(ACTOR_DORA, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(30);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(30);
    Actor_WalkToAndWait(ACTOR_HEALER, 0xa8, 0x78);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    Actor_FaceDirection(ACTOR_HEALER, FACING_NORTH + FACING_STEP, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
    Engine_EventWait(10);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(10);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_HEALER, 0);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Actor_SetAttachedEffect(GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Event_ShowMessage(GARET, 0);
    Engine_EventWait(30);
  }
  // Ask Isaac to commit, or leave the group waiting for his answer.
  Engine_EventSetMessage(MsgSanctumIsaacWillAcceptResponsibilityFor);
  Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 1);
  Engine_EventWait(20);
  Event_OpenMessage(ACTOR_HEALER, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Sanctum_ReceiveWiseOnesMission();
    ColorBuffer_ApplyTarget(RGB(0, 0, 0), PALETTE_HALVES_BOTH);
    Engine_ColorBufferInterpolate(120);
    Engine_EventWait(120);
    Event_PlayCue(MUS_086);
    AudioCommand_WaitForCompletion();
    GameFlag_Set(FLAG_SANCTUM_QUEST_ACCEPTED);
    Engine_EventRequestExit(SANCTUM_TO_VALE_GATE);
  } else {
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 4);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Engine_EventWait(20);
    Object_GetById(ACTOR_EAST_HEALER)->priorityFlags &=
        ~ACTOR_PRIORITY_AUTOMATIC;
#ifdef UBFIX
    Object_GetById(ACTOR_EAST_HEALER)->update = Sanctum_MatchLeaderPriority;
#else
    Object_GetById(ACTOR_EAST_HEALER)->update =
        (void (*)(union FieldObject *))Sanctum_MatchLeaderPriority;
#endif
  }
  Engine_EventEnd();
}

/// Vale Sanctum, once Isaac accepts: Garet and the Mayor react, the Wise One
/// appears before the Great Healer and accepts the two, and the healer relays
/// its guidance; they are to leave the next day.
static void Sanctum_ReceiveWiseOnesMission(void) {

  u32 sparkIndex;
  struct FieldActor *wiseOne;

  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_GARETS_GRANDFATHER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_WEST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_EAST_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_HEALER, PIXELS(4) / 5, PIXELS(2) / 5);
  Camera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
  Engine_CameraWaitForMove();
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_HEALER, 3);
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 3);
  Engine_ActorSetAnimation(ACTOR_WEST_HEALER, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_EAST_HEALER, 3);
  Engine_EventWait(50);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
  Engine_EventWait(20);
  // Garet and the Mayor react to Isaac accepting the burden.
  Engine_EventSetMessage(MsgSanctumAcceptIsaacCantMean);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, GARET, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  Engine_ActorShowEmote(ACTOR_GARETS_GRANDFATHER,
                        EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_EventWait(60);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Actor_FaceActor(GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(40);
  Actor_FaceActor(GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
  Engine_EventWait(20);
  Engine_ActorFaceDirection(ACTOR_HEALER, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(80);
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PALE, PALETTE_HALF_BG);
  Engine_ColorBufferInterpolate(60);
  Engine_EventWait(40);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(192), -1, PIXELS(104), 1);
  Engine_EventWait(120);
  Event_PlayCue(MUS_021);
  Event_PlayCue(SE_308);
  // The Wise One enters while the healer's afterimages appear.
  Actor_SetPosition(ACTOR_WISE_ONE, PIXELS(200), PIXELS(8));
  Actor_SetSpeed(ACTOR_WISE_ONE, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_MoveToAndWait(ACTOR_WISE_ONE, 200, 72);
  Event_PlayCue(SE_STOP_SLOT3);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 2);
  Engine_ActorSetAnimation(ACTOR_HEALER, 0);
  Sanctum_ShowHealerAfterimage();
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(GARET, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_WISE_ONE, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_WISE_ONE, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_WEST_HEALER, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_EAST_HEALER, 2);
  Engine_EventWait(40);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_EventWait(20);
  Engine_EventWait(40);
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Engine_EventWait(60);
  Engine_ActorSetChildValue(ACTOR_WISE_ONE, 0x100);
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  Event_PlayCue(SE_308);
  // Shrink the Wise One amid sparks before restoring the room.
  for (sparkIndex = 0; sparkIndex < 32; sparkIndex++) {
    Sanctum_SpawnActorSpark(ACTOR_WISE_ONE);
    Engine_EventWait(4);
    wiseOne = Object_GetById(ACTOR_WISE_ONE);
    wiseOne->scaleX += -Q16_ONE / 100;
    wiseOne = Object_GetById(ACTOR_WISE_ONE);
    wiseOne->scaleY += -Q16_ONE / 100;
  }
  Event_PlayCue(SE_STOP_SLOT3);
  Actor_SetChildValue(ACTOR_WISE_ONE, 0);
  Actor_SetPosition(ACTOR_WISE_ONE, 0, 0);
  Engine_EventWait(40);
  Camera_MoveTo(PIXELS(192), -1, PIXELS(160), 1);
  Engine_CameraWaitForMove();
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, PALETTE_HALVES_BOTH);
  Engine_ColorBufferInterpolate(60);
  Engine_EventWait(120);
  Sanctum_HideHealerAfterimage();
  Engine_ActorSetAnimation(ACTOR_HEALER, 1);
  Event_PlayCue(MUS_002);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 2);
  Engine_EventWait(20);
  Sanctum_StartAllRepeatedMotion();
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_EventWait(30);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(30);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Sanctum_ShowAllEmotes();
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Sanctum_NodAll();
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
  Engine_EventWait(180);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_EventWait(60);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_ActorFaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
  Engine_EventWait(40);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(60);
  Sanctum_StartAllRepeatedMotion();
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(80);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(60);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_HEALER, 1);
  Engine_ActorShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Engine_EventWait(20);
  Sanctum_StartAllRepeatedMotion();
  Engine_EventWait(40);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Sanctum_NodAll();
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(30);
  Sanctum_NodAll();
  Engine_EventWait(120);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 4);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_EventWait(60);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HEALER, 3);
  Event_ShowMessage(ACTOR_HEALER, 0);
  Engine_EventWait(40);
  Actor_WalkToAndWait(ACTOR_HEALER, 168, 176);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
  Actor_WalkToAndWait(ACTOR_HEALER, 200, 200);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Actor_FaceActor(GARET, ACTOR_HEALER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_WEST_HEALER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_EAST_HEALER, FACING_WEST, 0);
  Actor_WalkTo(ACTOR_HEALER, 200, 0x110);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_WEST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_EAST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  Engine_ActorWaitForMove(ACTOR_HEALER);
  Actor_SetPosition(ACTOR_HEALER, 0, 0);
  Engine_EventWait(60);
  Sanctum_ShowAllEmotes();
  Engine_EventWait(60);
  Engine_EventWait(120);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_WEST_HEALER, 0);
  Actor_FaceActor(GARET, ACTOR_EAST_HEALER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_WEST_HEALER, 0);
  Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_WEST_HEALER, 0);
  Engine_EventWait(120);
  Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_EventWait(60);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_WEST_HEALER, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_WEST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(80);
  Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_WEST_HEALER, 1);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_WEST_HEALER, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_EAST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(20);
  Engine_ActorShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_WEST_HEALER, 2);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_EAST_HEALER, 2);
  Engine_EventWait(30);
  Engine_ActorFaceDirection(ACTOR_EAST_HEALER, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_EAST_HEALER, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_WEST_HEALER, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_EAST_HEALER, 3);
  Engine_EventWait(20);
  Actor_WalkTo(ACTOR_WEST_HEALER, 200, 0x110);
  Actor_WalkToAndWait(ACTOR_EAST_HEALER, 200, 0x110);
  Actor_SetPosition(ACTOR_WEST_HEALER, 0, 0);
  Actor_SetPosition(ACTOR_EAST_HEALER, 0, 0);
  Engine_ActorShowEmote(ACTOR_GARETS_GRANDFATHER,
                        EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Engine_EventWait(60);
  Actor_WalkToAndWait(ACTOR_GARETS_GRANDFATHER, 200, 136);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceActor(GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceActor(ACTOR_DORA, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 4);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_DORA, 168, 168);
  Actor_FaceActor(ACTOR_DORA, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Actor_FaceActor(GARET, ACTOR_DORA, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, 4);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 4);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Actor_FaceActor(GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(60);
  Engine_ActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        0);
  Engine_EventWait(60);
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ActorShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_DORA, 0);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_DORA, 3);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 3);
}

/// Copies the healer into the afterimage actor.
static void Sanctum_ShowHealerAfterimage(void) {
  u32 entryIndex;
  struct FieldActor *afterimage;
  struct FieldActor *actor;
  struct AnimationObject *sprite;

  actor = Object_GetById(ACTOR_HEALER);
  if (actor != 0) {
    Engine_ActorSetPosition(ACTOR_HEALER_AFTERIMAGE, actor->x.fixed,
                            actor->z.fixed);
  }
  Engine_ActorSetAnimation(ACTOR_HEALER_AFTERIMAGE, 0);
  afterimage = Object_GetById(ACTOR_HEALER_AFTERIMAGE);
  actor = Object_GetById(ACTOR_HEALER);
  afterimage->facing = actor->facing;
  actor = Object_GetById(ACTOR_HEALER_AFTERIMAGE);
  actor->update = Sanctum_UpdateHealerAfterimage;
  actor = Object_GetById(ACTOR_HEALER_AFTERIMAGE);
  sprite = actor->sprite;
  {
    for (entryIndex = 0; entryIndex < sprite->count; entryIndex++) {
      struct AnimationEntry *entry = sprite->entries[entryIndex];

      if (entry != 0 && entry->script != 0) {
        entry->colorMode = SPRITE_COLOR_RED_TINT;
      }
    }
  }
  sprite->dirty = 1;
  Object_GetById(ACTOR_HEALER_AFTERIMAGE)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 2;
}

static void Sanctum_HideHealerAfterimage(void) {
  struct FieldActor *state = Object_GetById(ACTOR_HEALER_AFTERIMAGE);
  state->update = 0;
  Engine_ActorSetPosition(ACTOR_HEALER_AFTERIMAGE, 0, 0);
}

/// Cycles the afterimage through four offsets from the healer.
static void Sanctum_UpdateHealerAfterimage(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct FieldActor *healer;

  healer = Object_GetById(ACTOR_HEALER);
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
static void Sanctum_UpdateOrbitingEffect(union FieldObject *fieldObject) {
  struct FieldActor *effect = (struct FieldActor *)fieldObject;
  struct FieldActor *anchor = effect->linkedObject;
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
  struct FieldActor *owner;
  struct FieldActor *effect;
  struct AnimationObject *sprite;
  struct AnimationObject *sourceSprite;

  orbitStep = angleStep;
  owner = Object_GetById(ownerId);
  if (owner != 0) {
    effect = Object_Create(SPRITE_SPARKLE, owner->x.fixed,
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

  Engine_ActorSetPosition(ACTOR_HEALER, 0, 0);
  Engine_ActorSetPosition(ACTOR_WEST_HEALER, 0, 0);
  Engine_ActorSetPosition(ACTOR_EAST_HEALER, 0, 0);
  Engine_ActorSetPosition(GARET, 0, 0);
  Engine_ActorSetPosition(ACTOR_DORA, 0, 0);
  Engine_ActorSetPosition(ACTOR_GARETS_GRANDFATHER, 0, 0);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, PALETTE_HALF_OBJ);
  Engine_ColorBufferInterpolate(1);
  Engine_EventWait(1);
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
    Engine_EventWait(1);
  }
  UiTextResource_Release(slot);
  UiWork_Finalize(window, 1);
  return choice;
}

/// Vale Sanctum, the quest's account: Isaac and Garet act out, in sepia, what
/// happened in Sol Sanctum.
static void Sanctum_ReenactSolSanctum(void) {

  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_SetSpeed(GARET, PIXELS(3) / 2, PIXELS(3) / 4);
  Engine_ActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(15);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 184, 168);
  Object_GetById(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorWalkToAndWait(GARET, 200, 168);
  Engine_EventWait(1);
  Object_GetById(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_STAND);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Object_GetById(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorJump(GARET, 2, 0);
  Engine_EventWait(15);
  Engine_ActorFaceActor(GARET, ACTOR_HEALER, 0);
  Engine_EventWait(5);
  Engine_ActorJump(GARET, 2, 0);
  Engine_EventWait(25);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_ActorFaceActor(GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(5);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(5);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(10);
  Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(15);
  Engine_ActorSetAnimation(ACTOR_DORA, 3);
  Engine_ActorSetAnimation(ACTOR_GARETS_GRANDFATHER, 3);
  Engine_ActorSetAnimation(ACTOR_HEALER, 3);
  Engine_ActorSetAnimation(ACTOR_WEST_HEALER, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_EAST_HEALER, 3);
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_ActorFaceActor(GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_ActorFaceActor(GARET, ACTOR_DORA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Engine_EventWait(15);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(10);
  // Illustrate the account with the floating item icon.
  Sanctum_RaiseItemIcon(222, PIXELS(184), PIXELS(27), PIXELS(168));
  Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(10);
  Engine_ActorJump(GARET, 4, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(15);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Engine_ActorJump(GARET, 4, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(15);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(60);
  Engine_ActorFaceActor(GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(10);
  Object_GetById(ACTOR_HEALER)->timer = GARET;
#ifdef UBFIX
  Object_GetById(ACTOR_HEALER)->update = Sanctum_FaceActorNamedByTimer;
#else
  Object_GetById(ACTOR_HEALER)->update =
      (void (*)(union FieldObject *))Sanctum_FaceActorNamedByTimer;
#endif
  Object_GetById(ACTOR_GARETS_GRANDFATHER)->timer = GARET;
#ifdef UBFIX
  Object_GetById(ACTOR_GARETS_GRANDFATHER)->update =
      Sanctum_FaceActorNamedByTimer;
#else
  Object_GetById(ACTOR_GARETS_GRANDFATHER)->update =
      (void (*)(union FieldObject *))Sanctum_FaceActorNamedByTimer;
#endif
  Engine_ActorWalkToAndWait(GARET, 196, 180);
  Engine_ActorWalkToAndWait(GARET, 184, 184);
  Engine_ActorWalkToAndWait(GARET, 180, 180);
  Engine_ActorWalkToAndWait(GARET, 168, 168);
  Engine_ActorWalkToAndWait(GARET, 180, 156);
  Engine_ActorWalkTo(GARET, 200, 104);
  Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 192, 168);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_ActorWaitForMove(GARET);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(15);
  Object_GetById(ACTOR_GARETS_GRANDFATHER)->update = NULL;
  Object_GetById(ACTOR_HEALER)->update = NULL;
  Engine_ActorStartRepeatedMotion(ACTOR_HEALER, 2);
  Engine_ActorShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_HEALER, 0);
  Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(60);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, 3);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
  Engine_EventWait(20);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
  Engine_EventWait(20);
  Engine_EventWait(15);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 3);
  Engine_EventWait(60);
  Engine_ActorWalkToAndWait(GARET, 208, 168);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_ActorFaceActor(GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_SHAKE_HEAD);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
  Engine_EventWait(10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(10);
  Engine_ActorFaceActor(GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 3);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Engine_ActorFaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  // Repeat the synchronized eight-point paths around the room.
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gSanctum_LeaderCircleScript);
  Actor_SetActionScript(GARET, gSanctum_GaretCircleScript);
  Object_RefreshSelectorById(ACTOR_PARTY_LEADER);
  Object_RefreshSelectorById(GARET);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
  Actor_SetSpeed(GARET, PIXELS(3) / 2, PIXELS(3) / 4);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Engine_ActorJump(GARET, 6, 0);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_WEST_HEALER, 0);
  Engine_ActorFaceActor(GARET, ACTOR_HEALER, 0);
  Engine_EventWait(1);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_ActorFaceActor(GARET, ACTOR_DORA, 0);
  Engine_EventWait(1);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
  Engine_ActorFaceActor(GARET, ACTOR_WEST_HEALER, 0);
  Engine_EventWait(1);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 192, 168);
  Engine_ActorWalkToAndWait(GARET, 208, 168);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorFaceDirection(GARET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
  Engine_ActorFaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(20);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Engine_ActorFaceActor(GARET, ACTOR_GARETS_GRANDFATHER, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_DORA, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, 3);
  Engine_EventWait(30);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_WALK);
  Engine_ActorSetAnimation(GARET, ANIMATION_WALK);
  Engine_EventWait(60);
}

/// Shows an item icon rising from the supplied fixed-point position.
static void Sanctum_RaiseItemIcon(s32 item, s32 fixedX, s32 fixedY,
                                  s32 fixedZ) {
  struct FieldActor *icon;
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
  struct FieldActor *spark = &fieldObject->actor;
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
static void Sanctum_SpawnActorSpark(s32 id) {
  struct FieldActor *actor;
  struct FieldActor *spark;
  struct AnimationObject *sprite;
  s32 x;
  s32 randomOffset;

  actor = Object_GetById(id);
  if (actor == 0)
    return;
  randomOffset = Math_RemainderUnsigned(Engine_RandomNext(), 20);
  x = actor->x.fixed;
  x += randomOffset << 16;
  x += -PIXELS(10);
  spark = Engine_ObjectCreate(
      0x11e, x,
      actor->y.fixed + ((Engine_RandomNext() & 15) << 16) + -PIXELS(8),
      actor->z.fixed);
  if (spark == 0)
    return;
  sprite = spark->sprite;
  spark->motionFlags = 0;
  spark->timer = Math_RemainderUnsigned(Engine_RandomNext(), 10) + 5;
  spark->work = Math_RemainderUnsigned(Engine_RandomNext(), 60) + 30;
  spark->update = Sanctum_UpdateRisingSpark;
  sprite->flags = 0;
  sprite->image.priority = actor->sprite->image.priority;
}

/// Copies two blocks of map cells for the quest's visits, the tiles of one and
/// the collision of the other.
static void Sanctum_RestoreQuestFloor(void) {
  {
    Map_CopyCellsTo(0, 64, 11, 68, 3, 2);
  }
  {
    Map_CopyCellAttributes(11, 10, 3, 2, 11, 8);
  }
  Engine_TaskWait(1);
}

static s32 Sanctum_IsLeaderFacingHealer(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

  if ((u32)((actor->facing + 0x5fff) << 16) <= 0x3ffe0000) {
    return 1;
  }
  return 0;
}

/// Vale Sanctum: talking to the Great Healer outside the quest. From the front
/// the healer opens the Sanctum's services; otherwise the words follow the
/// story, from "Wielders of Psynergy are called Adepts" before the party has
/// left Vale to the closed path to Sol Sanctum once it has reached Kalay.
static void Event_TalkToHealer(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(ACTOR_HEALER);
    return;
  }

  Engine_EventStart();
  if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0)
    Engine_EventSetMessage(MsgSanctumPathSolSanctumStillClosed);
  else if (Engine_GameFlagIsSet(FLAG_VALE_PARTY_LEFT) != 0)
    Engine_EventSetMessage(MsgSanctumMyControlOverPsynergyHas);
  else
    Engine_EventSetMessage(MsgSanctumWieldersPsynergyCalledAdeptsAdepts);
  Engine_EventShowMessage(ACTOR_HEALER, 0);
  Engine_EventEnd();
}

/// Vault Sanctum: talking to the healer. From the front the healer opens the
/// Sanctum's services; otherwise the healer speaks of the gold statue or, on
/// the later visit, of the earthquakes. Its case for the quest's entrances
/// (SANCTUM_FROM_VALE_ERUPTION, SANCTUM_FROM_QUEST_RESTART and
/// SANCTUM_QUEST_UNUSED) passes to Event_ReconsiderQuest, though their events
/// (gSanctum_QuestEvents) call that directly.
static void Event_TalkToHealerOnVisit(void) {
  s32 entrance;

  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(ACTOR_HEALER);
    return;
  }

  Engine_EventStart();

  entrance = gGameState.entrance;

  switch (entrance) {
  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_VAULT_INTERIORS:
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Engine_EventSetMessage(MsgSanctumWasHandFateReturnedGold);
    } else {
      Engine_EventSetMessage(MsgSanctumWeWillHelpAnytimeAs);
    }
    break;
  case SANCTUM_FROM_VAULT_REVISIT:
    Engine_EventSetMessage(MsgSanctumMayWrongButLatelyThere);
    break;
  case SANCTUM_FROM_VALE_ERUPTION:
  case SANCTUM_FROM_QUEST_RESTART:
  case SANCTUM_QUEST_UNUSED:
    Engine_EventEnd();
    Event_ReconsiderQuest();
    return;
  default:
    break;
  }

  Engine_EventShowMessage(ACTOR_HEALER, 0);
  Engine_EventEnd();
}

/// Bilibin Sanctum: talking to the healer, about Kolima's curse, lifted or not;
/// from the front, the Sanctum's services.
static void Event_TalkAboutCurse(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(ACTOR_HEALER);
    return;
  }

  Engine_EventStart();
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0)
    Engine_EventSetMessage(MsgSanctumCurseMayOverButWe);
  else
    Engine_EventSetMessage(MsgSanctumAmStartingFeelOnlyBeginning);
  Engine_EventShowMessage(ACTOR_HEALER, 0);
  Engine_EventEnd();
}

/// Altin Sanctum: talking to the healer, about the beasts in the mine, or the
/// party's saving Altin from them; from the front, the Sanctum's services.
static void Event_TalkAboutAltinMonsters(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(ACTOR_HEALER);
    return;
  }

  Engine_EventStart();
  Engine_EventSetMessage(MsgSanctumWasAfterEerieNightWhen);
  if (Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED) != 0)
    Engine_EventSetMessage(MsgSanctumSavedAltinFromMonstersClearly);
  Engine_EventShowMessage(ACTOR_HEALER, 0);
  Engine_EventEnd();
}

/// Xian Sanctum: talking to the healer, who came to Xian from very distant
/// lands; from the front, the Sanctum's services.
static void Event_TalkAboutJourneyFromXian(void) {
  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Engine_SanctumOpen(ACTOR_HEALER);
    return;
  }

  Engine_EventStart();
  Engine_EventSetMessage(MsgSanctumCameXianFromVeryDistant);
  Engine_EventShowMessage(ACTOR_HEALER, 0);
  Engine_EventEnd();
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

struct SceneEntrance gSanctum_Entrances[] = {
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
    { SCENE_TABLE_END },
};

u32 gSanctum_Exits[] = {
    SCENE_EXITS(Sanctum),
    SCENE_EXIT(SANCTUM_TO_VALE, Vale, VALE_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_VAULT, Vault, VAULT_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_QUEST_RESTART, Sanctum, SANCTUM_FROM_QUEST_RESTART),
    SCENE_EXIT(SANCTUM_TO_VALE_GATE, ValeGate, VALE_GATE_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_ALTIN, Altin, ALTIN_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_BILIBIN, Bilibin, BILIBIN_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_XIAN, Xian, XIAN_FROM_SANCTUM_WEST),
    SCENE_EXIT(SANCTUM_TO_CLEAR, Clear, CLEAR_FROM_SANCTUM),
    SCENE_EXITS_END,
};

struct ScenePlacement gSanctum_NoPlacements[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gSanctum_ValePlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(130), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

#if TBS_PATCH_1
struct ScenePlacement gSanctum_IvanPlacement[] = {
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};
#endif

struct ScenePlacement gSanctum_VaultBilibinPlacements[] = {
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(130), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(200), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xdd, CONDITION_ALWAYS, 7,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSanctum_QuestPlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(208), 0, PIXELS(168), FACING_NORTH + FACING_STEP, 3, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(224), 0, PIXELS(216), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(198), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_DORA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(208), FACING_NORTH + FACING_STEP, 3, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, 3, 0 },
    { 0xfc, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSanctum_AltinXianPlacements[] = {
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(132), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSanctum_ValeEvents[] = {
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealer) },
    { EVENT_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumOurPsynergyWasGivenToUs) },
    { EVENT_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumPsynergyIsAPowerThatShould) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_VaultEvents[] = {
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealerOnVisit) },
    { EVENT_TALK, ACTOR_SANCTUM_MEMBER, FLAG_VAULT_THIEVES_JAILED, EVENT_SCRIPT(Event_TalkAboutGoldenStatue) },
    { EVENT_TALK, ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutGoldenStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumGoOnIfThatEasesThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindAboutGoldenStatue) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VAULT },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_VaultRevisitEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VAULT_REVISIT },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealerOnVisit) },
    { EVENT_TALK, ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutGoldenStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumIHopeTheEarthquakesArentA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindAboutGoldenStatue) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_QuestEvents[] = {
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReconsiderQuest) },
    { EVENT_TALK, GARET, CONDITION_ALWAYS, MSGNO(MsgSanctumItIsATerribleBurdenTo) },
    { EVENT_TALK, ACTOR_DORA, CONDITION_ALWAYS, MSGNO(MsgSanctumMotherWouldNeverDoubtYourCourage) },
    { EVENT_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgSanctumIDontBlameYouICouldnt) },
    { EVENT_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWasItRightToRefuseWhen) },
    { EVENT_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItsClearThatTheGreatHealer) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveAfterRefusing) },
    { EVENT_TOUCH, 2, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_WarnBeforeLeaving) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_ValeEventsAfterLeaving[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealer) },
    { EVENT_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumThePowerOfMtAlephStems) },
    { EVENT_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutPsynergyChanges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumThePowerOfMyPsynergyHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumTheAngaranContinentWillCertainlySee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumMyPsynergyHasntChangedAtAll) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_ValeEventsAfterKalay[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealer) },
    { EVENT_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutNewAdepts) },
    { EVENT_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWhyIsItThatDifferentAdepts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItllBeDifficultToReachSol) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumISeeABeautifulGirlHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItsNotFairIWantTo) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_AltinEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_ALTIN },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutAltinMonsters) },
    // Unused: a copy of the row above, which always answers first.
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutAltinMonsters) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgSanctumPleaseSaveTheHelplessInAltin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumAlmightyFateMustHaveHeardMe) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_BilibinEvents[] = {
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutCurse) },
    { EVENT_TALK, ACTOR_SANCTUM_MEMBER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumPleaseGiveMeStrengthAtTimes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumWillITurnIntoATree) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumSomeoneWillSaveUsJustKeep) },
    { EVENT_TALK, ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, MSGNO(MsgSanctumHowAboutThatOurWishesHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumTheSignsOfDangerAreActually) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, MSGNO(MsgSanctumPeaceWillBeKeptAsLong) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_BILIBIN },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSanctum_XianEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, SANCTUM_TO_XIAN },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutJourneyFromXian) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWhenYouStayInXianFor) },
    { SCENE_EVENTS_END },
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
