/// @file
/// The Sanctum, the room of the Healers that Vale, Vault, Bilibin, Xian and
/// Altin each have (SceneId_Sanctum). One room serves them all: the door the
/// party comes in by decides whose Sanctum it is, which people stand in it,
/// and which town its door leads back out to. The Healer stands at the top of
/// the room before the altar, behind a small stone counter. Talking to him
/// across it, facing north, opens the Sanctum's services; from any other side
/// he only talks.
///
/// - Vale's Sanctum: the Great Healer, with a Healer on each side of the room
///   who speaks of Psynergy. After the eruption of Mt. Aleph, the Great Healer
///   brings Isaac and Garet here from Vale (vale_eruption.c, entrance
///   SANCTUM_FROM_VALE_ERUPTION). Dora and the Mayor are waiting. Isaac and
///   Garet act out what happened in Sol Sanctum while the room is tinted sepia,
///   the Great Healer tells what the Wise One showed him, and he asks Isaac to
///   take on the quest. For these visits the stone counter is taken away and
///   the Great Healer stands beside the altar.
///
///   A yes brings the Wise One down over the altar to give Isaac and Garet
///   their mission; the screen fades to black over the inn's night music, and
///   the two are in Vale's plaza (vale_plaza.c). A no
///   leaves everyone waiting in the room: the Great Healer asks again when
///   spoken to, he warns Isaac as he heads for the door, and stepping out ends
///   the game, "And so the world began drifting toward its fated
///   destruction...", with a choice to start again from the Great Healer
///   (SANCTUM_FROM_QUEST_RESTART, without the acting out) or to go to the file
///   screen. After the party has left Vale (FLAG_VALE_PARTY_LEFT), and again
///   after it has reached Kalay (FLAG_KALAY_REACHED), the three Healers talk of
///   what the eruption and the Wise One changed.
/// - Vault's Sanctum: the Healer, and a man with long brown hair in a long robe
///   who wanders the room. A gold statue was stolen from them during the
///   eruption. Once the party has caught the thieves at the inn
///   (FLAG_VAULT_THIEVES_JAILED) the statue stands back behind the Healer, and
///   on the party's later visit through Vault (vault_revisit.c) it has been
///   polished and the Healer worries about earthquakes on Mt. Aleph.
/// - Bilibin's Sanctum: the Healer and a man like Vault's, who talk of
///   Kolima's curse, before and after it is lifted (FLAG_KOLIMA_CURSE_LIFTED).
/// - Xian's Sanctum: a Healer who came to Xian from very distant lands.
/// - Altin's Sanctum: a Healer who tells of the water-spewing beasts in the
///   mine, until the party has saved Altin from them
///   (FLAG_HYDROS_STATUES_DEFEATED).
///
/// Entrance SANCTUM_UNUSED, which no exit leads to, places nobody and gives
/// the room the name Sanctum alone.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_builtin_scripts.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/scene_tables.h"
#include "field/speaker.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/input.h"
#include "system/main_routine_state.h"
#include "system/save.h"
#include "system/system.h"
#include "window/screens.h"
#include "window/window.h"
#include "text/msg_ids.h"

const struct ActorPlacement *Scene_GetActorPlacements(void);
extern u32 gSanctum_LeaderCircleScript[];
extern u32 gSanctum_GaretCircleScript[];
extern s32 gItemIcon_GrowScript[];
extern s32 gSanctum_ItemIconEndScript[];
extern struct SceneEntrance gSanctum_Entrances[];
extern u32 gSanctum_Exits[];
extern struct ActorPlacement gSanctum_NoActorPlacements[];
extern struct ActorPlacement gSanctum_ValeActorPlacements[];
extern struct ActorPlacement gSanctum_IvanActorPlacement[];
extern struct ActorPlacement gSanctum_VaultBilibinActorPlacements[];
extern struct ActorPlacement gSanctum_QuestActorPlacements[];
extern struct ActorPlacement gSanctum_AltinXianActorPlacements[];
extern struct Trigger gSanctum_ValeTriggers[];
extern struct Trigger gSanctum_VaultTriggers[];
extern struct Trigger gSanctum_VaultRevisitTriggers[];
extern struct Trigger gSanctum_QuestTriggers[];
extern struct Trigger gSanctum_ValeTriggersAfterLeaving[];
extern struct Trigger gSanctum_ValeTriggersAfterKalay[];
extern struct Trigger gSanctum_AltinTriggers[];
extern struct Trigger gSanctum_BilibinTriggers[];
extern struct Trigger gSanctum_XianTriggers[];
extern s32 gSanctum_CursorOffsets[];
extern s32 gSanctum_OrbitingSparkleScript[];
extern s32 gSanctum_SparkEndScript[];

/// The cells the room's step triggers answer.
enum SanctumCellBehavior {
  /// The doorway, cells (10, 16) to (14, 16).
  CELL_BEHAVIOR_SANCTUM_DOORWAY = 1,
  /// The row just inside it, cells (10, 15) to (14, 15).
  CELL_BEHAVIOR_SANCTUM_BEFORE_DOOR
};

/// After the quest is refused, the Great Healer has warned Isaac at the door
/// that stepping outside the village is final.
#define FLAG_SANCTUM_WARNED_BEFORE_LEAVING FLAG_SCENE_TEMP_00
/// The party is starting again from the Great Healer after refusing the quest,
/// so the account of Sol Sanctum is skipped.
#define FLAG_SANCTUM_QUEST_RESTARTED FLAG_SCENE_TEMP_01

/// The placed actors, numbering the rows of the table
/// Scene_GetActorPlacements returns for the entrance.
///
/// The party's rows take their own ids. Every place's first row is its healer.
enum SanctumActorId {
  /// The town's Healer, a man with a white beard in a tall hat and a long
  /// purple and blue robe, before the altar at the top of the room.
  ///
  /// He is the Great Healer in Vale's Sanctum.
  ACTOR_HEALER = ACTOR_FIRST_PLACED,
  // Vale Sanctum (gSanctum_ValeActorPlacements, and
  // gSanctum_QuestActorPlacements at the quest).
  /// The western of Vale's two other Healers, men with long brown hair in long
  /// robes, standing on either side of the stone counter.
  ///
  /// Mind Read hears a villager in cottage_rooms_a.c call the Sanctum's people
  /// so: "The healers are supposed to guard Sol Sanctum." Before the quest:
  /// "Our Psynergy was given to us by the sacred power of Mt. Aleph."
  ACTOR_WEST_HEALER,
  /// The eastern one. "Psynergy is a power that should only be used for
  /// good... Never for evil!"
  ACTOR_EAST_HEALER,
  // The quest (gSanctum_QuestActorPlacements).
  /// Isaac's mother, a woman with fair hair tied up.
  ACTOR_DORA,
  /// The Mayor, Garet's grandfather, an old man with white hair and a cane.
  ///
  /// The Great Healer calls him so: "That is all, Mayor."
  ACTOR_GARETS_GRANDFATHER,
  /// Appears to the Great Healer.
  ACTOR_WISE_ONE,
  /// A copy of the Great Healer for its afterimages.
  ACTOR_HEALER_AFTERIMAGE,
  // Vault Sanctum and Bilibin Sanctum
  // (gSanctum_VaultBilibinActorPlacements).
  /// The other man of Vault's and Bilibin's Sanctums, who wanders the room.
  ///
  /// He wears the western Healer's sprite (a man with long brown hair in a long
  /// robe). He speaks of the healer as someone else and of "our
  /// teachings" and "our meditations". In Vault: "I wonder if we'll ever see
  /// our stolen gold statue again..."
  ACTOR_SANCTUM_MEMBER = ACTOR_FIRST_PLACED + 1,
  /// Vault Sanctum's gold statue, placed off the map until the thieves are
  /// jailed (FLAG_VAULT_THIEVES_JAILED).
  ///
  /// Its sprite is that of the one a chest gives back in log_rooms_a.c.
  ACTOR_GOLD_STATUE
};

static void Sanctum_AskToAcceptQuest(void);
static void Sanctum_ReceiveWiseOnesMission(void);
static void Sanctum_ShowHealerAfterimage(void);
static void Sanctum_HideHealerAfterimage(void);
static void Sanctum_UpdateHealerAfterimage(struct Actor *actor);
static void Sanctum_SpawnOrbitingEffect(s32 actorId, s32 angleStep);

/// How far each sparkle circling the Great Healer goes around him in a frame.
#define SANCTUM_SPARKLE_ORBIT_STEP (TRIG_TURN * 9 / 128)

/// The sparks the Wise One gives off as he shrinks away after giving Isaac and
/// Garet their mission, one each four frames.
#define WISE_ONE_VANISH_SPARKS 32
static s32 Sanctum_ChooseRestartOption(void);
static void Sanctum_ActOutSolSanctum(void);
static void Sanctum_RaiseItemIcon(s32 item, FIXED fixedX, FIXED fixedY,
                                  FIXED fixedZ);
static void Sanctum_SpawnActorSpark(s32 id);
static void Sanctum_RemoveCounter(void);

/// An update routine: the actor keeps turned towards the actor whose id its
/// timer holds.
static UNREAD_RESULT Sanctum_FaceActorNamedByTimer(struct Actor *actor) {
  struct Actor *target;

  target = Actor_Get((s16)actor->timer);
  actor->facing = Trig_Atan(target->z.fixed - actor->z.fixed,
                            target->x.fixed - actor->x.fixed);
  RETURN_UNREAD(0);
}

/// The entrances of every town's Sanctum.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gSanctum_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSanctum_Exits; }

/// Who stands in the room depends on the door the party came in by: the town's
/// Healers, the gathering after the eruption, or nobody.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  switch (gMainRoutineState.entrance) {
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

/// The triggers by the door the party came in by; Vale's Sanctum also changes
/// once the party has left Vale and again once it has reached Kalay.
const struct Trigger *Scene_GetTriggers(void) {
  switch (gMainRoutineState.entrance) {
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

/// Talking to the man in a long robe wandering Vault's Sanctum: while the gold
/// statue is missing he starts to say what worries the Healer, then thinks
/// better of it; once it is back he speaks of fate guiding those who stray. On
/// the later visit through Vault he has polished it: "It's so shiny, you won't
/// even recognize it!"
static void Sanctum_TalkAboutGoldenStatue(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
    Event_SetMessageId(MsgSanctumWhenStrayFromYourWorldly);
  } else {
    Event_SetMessageId(MsgSanctumHealerMustWorriedAboutNever);
  }

  if (gMainRoutineState.entrance == SANCTUM_FROM_VAULT_REVISIT) {
    Event_SetMessageId(MsgSanctumPolishedGoldStatueReturnedUs);
  }

  Actor_SetAnimation(ACTOR_SANCTUM_MEMBER, ANIM_STAND);
  Actor_FaceEachOther(ACTOR_SANCTUM_MEMBER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(2);
  Event_ShowMessage(ACTOR_SANCTUM_MEMBER, 0);
  Event_End();
}

/// Mind Read on the man in a long robe wandering Vault's Sanctum: while the
/// gold statue is missing he wonders if they will ever see it again; once it
/// is back he thinks of a child who has awakened to their teachings; on the
/// later visit through Vault, of how well the dirty statue cleaned up. He
/// stops to be read and wanders on after.
static void Sanctum_ReadMindAboutGoldenStatue(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgSanctumWonderIfEverSeeOur);
  } else {
    Event_SetMessageId(MsgSanctumChildHasAwakenedOurTeachings);
  }

  if (gMainRoutineState.entrance == SANCTUM_FROM_VAULT_REVISIT) {
    Event_SetMessageId(MsgSanctumDirtyGoldenStatueCleanedUp);
  }

  Actor_Stop(ACTOR_SANCTUM_MEMBER);
  Actor_SetAnimation(ACTOR_SANCTUM_MEMBER, ANIM_STAND);
  Event_Wait(2);
  Event_ShowMessage(ACTOR_SANCTUM_MEMBER, 0);
  Actor_SetMovementTypeById(ACTOR_SANCTUM_MEMBER, MOVEMENT_TYPE_WANDER);
  Event_End();
}

/// Talking to the Great Healer after Isaac has refused the quest: he asks "Are
/// you sure?", then whether Isaac would like to think it over, then whether he
/// will take the fate of the world in his hands. A yes to the first keeps the
/// refusal; no, yes and yes accept. Isaac then walks back to his place before
/// the Great Healer, who first steps aside if he was turned to the altar, and
/// the Wise One gives the mission as after a first yes.
static void Sanctum_AskQuestAgain(void) {
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
        Sound_Play(MUS_OVERNIGHT_AT_THE_INN);
        Sound_WaitForBgmRestore();
        GameFlag_Set(FLAG_SANCTUM_QUEST_ACCEPTED);
        Import_EventSetExitRequest(SANCTUM_TO_VALE_PLAZA);
      }
    }
  }
  Event_End();
}

/// Who answers "......" when the healer asks whether Isaac's new friends are
/// Adepts: Ivan from patch 1, Garet before.
#if TBS_PATCH_1
#define SANCTUM_NEW_FRIENDS_SPEAKER ACTOR_IVAN
#else
#define SANCTUM_NEW_FRIENDS_SPEAKER ACTOR_GARET
#endif

/// Talking to the Healer on the west side of Vale's Sanctum once the party has
/// reached Kalay: "Isaac, are your new friends Adepts by any chance?" A yes
/// makes him marvel that others besides Valeans have Psynergy. A no draws a
/// silent "......" from two of the party, and he sees that Isaac's friend is
/// angered by his words. The silent pair is Ivan and Mia in the editions of
/// the first patch on, which first bring Ivan into the room; before it, Garet
/// and Mia.
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
    gFieldState->message++;
    // "......" from both at once.
    FieldEvent_ShowTwoMessagesAndWait(SANCTUM_NEW_FRIENDS_SPEAKER, 16, 1, 24, 1,
                                      ACTOR_MIA, 7, 16, 1, 14, 0);
    // "Your friend seems angered by your words, Isaac... That..."
    Event_ShowMessage(ACTOR_WEST_HEALER, 0);
  }
  Event_End();
}

/// Talking to the Healer on the east side of Vale's Sanctum once the party has
/// left Vale: he turns to Isaac and asks "Do you feel any change in your
/// Psynergy?" A yes makes him wish the Wise One would come to him too; a no,
/// recall that the Great Healer's powers grew when he met the Wise One.
static void Sanctum_AskAboutPsynergyChanges(void) {
  Event_Start();
  Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
  Event_Wait(10);
  Event_SetMessageId(MsgSanctumDoFeelAnyChangeIn);
  // "Do you feel any change in your Psynergy?"
  Event_OpenMessage(ACTOR_EAST_HEALER, 0);

  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    (gFieldState->message)++;
  }

  Event_ShowMessage(ACTOR_EAST_HEALER, 0);
  Event_End();
}

/// Every visit but a plain one to Vale's Sanctum opens without the area name.
/// In Vault's, the gold statue stands behind the Healer once the thieves are
/// jailed. Brought here after the eruption, or starting again after refusing,
/// the stone counter is taken away and the Great Healer's question begins; a
/// return from a battle or the menu (FLAG_KEEP_PARTY_POSITION) skips it.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  Event_Wait(1);
  switch (gMainRoutineState.entrance) {
  case SANCTUM_FROM_VAULT:
  case SANCTUM_FROM_VAULT_REVISIT:
  case SANCTUM_FROM_LOG_ROOMS_A:
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
      Actor_SetPosition(ACTOR_GOLD_STATUE, 200, 80);
    }
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    break;
  case SANCTUM_FROM_VALE_ERUPTION:
    Sanctum_RemoveCounter();
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Sanctum_AskToAcceptQuest();
    }
    // Fall through.
  case SANCTUM_FROM_ALTIN:
  case SANCTUM_FROM_XIAN:
  case SANCTUM_FROM_BILIBIN:
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    break;
  case SANCTUM_FROM_QUEST_RESTART:
    Sanctum_RemoveCounter();
    GameFlag_Set(FLAG_SANCTUM_QUEST_RESTARTED);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Sanctum_AskToAcceptQuest();
    }
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    break;
  }
  return 0;
}

/// When Isaac heads for the door of Vale's Sanctum after refusing the quest
/// (the row just inside it, cells (10, 15) to (14, 15)), the view pans back up
/// the room, the Great Healer turns to him and shakes his head: "Once you step
/// outside the village, you cannot take back your decision." He warns only
/// once (FLAG_SANCTUM_WARNED_BEFORE_LEAVING).
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

/// The last and palest colour of the room's first palette, a pale cream on its
/// brightest highlights.
#define ROOM_PALEST_COLOR 15

/// Stepping into the doorway of Vale's Sanctum after refusing the quest (cells
/// (10, 16) to (14, 16)) ends the game. Everyone freezes and the room fades to
/// sepia, with its palest colour turning white; "And so the world began
/// drifting toward its fated destruction..." shows across the screen, the
/// screen fades to black, and the player chooses to start again "From the Great
/// Healer" or to go "To the File Screen" ("Restart the game" when there is no
/// saved file).
static void Sanctum_LeaveAfterRefusing(void) {
  struct PaletteFade *paletteState;

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
  paletteState = gPaletteFadeState;
  // ROOM_PALEST_COLOR fades to white rather than sepia: its blue, green and
  // red channels, each held in blue's place.
  paletteState->target[3 * ROOM_PALEST_COLOR] = RGB_LEVEL_MAX << BLUE_SHIFT;
  paletteState->target[3 * ROOM_PALEST_COLOR + 1] = RGB_LEVEL_MAX << BLUE_SHIFT;
  paletteState->target[3 * ROOM_PALEST_COLOR + 2] = RGB_LEVEL_MAX << BLUE_SHIFT;
  paletteState->page = 0;
  paletteState->duration = 1;
  paletteState->step = 1;
  // DEAD: a byte nothing reads set to 1; the ROM keeps its store.
  paletteState->padding = 1;
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

/// Back in Vale's Sanctum after the eruption, Isaac and Garet tell the Great
/// Healer, Dora, the Mayor and the Healers what happened in Sol Sanctum, and
/// the Great Healer asks Isaac to take on the quest. The account of Jenna and
/// Kraden's capture is acted out in sepia, and the Great Healer tells of the
/// hovering stone, the Wise One, who showed him all of it. Starting again after
/// a refusal skips straight to the question. A yes gives the mission; a no
/// leaves Isaac free to walk out.
static void Sanctum_AskToAcceptQuest(void) {
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
    gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    gFieldState->transitionFrames = 64;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(120);
  } else {
    PaletteFade_ApplyTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
    PaletteFade_StartInterpolation(1);
    Event_Wait(1);
    FieldCamera_MoveTo(192, CAMERA_KEEP, 160, 1);
    FieldCamera_WaitForMove();
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Sanctum_ActOutSolSanctum();
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
      gFieldState->message++;
    } else {
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
      Event_Wait(20);
      gFieldState->message++;
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
      gFieldState->message++;
    } else {
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_WEST_HEALER, ANIM_SHAKE_HEAD);
      Event_Wait(20);
      gFieldState->message++;
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
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
    Event_Wait(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
    Event_Wait(16);
    for (effectIndex = 0; effectIndex < 6; effectIndex++) {
      Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
      Event_Wait(8);
    }
    Event_Wait(8);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
    Event_Wait(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
    Event_Wait(96);
    Event_Wait(32);
    Actor_SetSpriteColorById(ACTOR_HEALER, SPRITE_COLOR_NORMAL);
    Event_Wait(30);
    Sanctum_HideHealerAfterimage();
    Actor_SetAnimation(ACTOR_HEALER, ANIM_STAND);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 2);
    Event_Wait(20);
    // "It is impossible to stop the eruption now."
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
    // "Will Vale be..."
    Event_ShowMessage(ACTOR_EAST_HEALER, 0);
    Event_Wait(20);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    Event_Wait(20);
    // "Vale will be safe. The Wise One will prevent the lava from reaching us."
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
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
    Event_Wait(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
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
      Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
      Event_Wait(8);
    }
    Event_Wait(8);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
    Event_Wait(32);
    Sanctum_SpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
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
    // "What's the matter, Great Healer?"
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_DORA, 2);
    Event_Wait(20);
    // "...The village will be destroyed, won't it?"
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
    // "Something even more dreadful than Vale's destruction looms ahead."
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    Actor_FaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    Event_Wait(20);
    // "What could be worse than the destruction of Vale?"
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    Event_Wait(20);
    // "A terrible power."
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
    // "The Elemental Star gems house incredible power."
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
    // "...And Vale has been guarding those gems?"
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    Event_Wait(20);
    // "The elemental lighthouses will loose that power upon the world."
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(50);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    // "Once the power of the gems has been released, the whole world will..."
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
    // "The Wise One wants us to get the gems back, doesn't he?"
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
    // "This world is ours, to save or to destroy. Our inaction may bring..."
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
    // "What a pity..."
    Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_HEALER, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "We cannot hope for salvation. We must save ourselves..."
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
    // "Then what can we do?"
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
    // "Acquiring the Elemental Stars is their fate alone."
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_Wait(60);
    Actor_SetAttachedEffect(ACTOR_DORA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_DORA, 1);
    // "Are you suggesting we place the fate of the world... in the hands..."
    Event_ShowMessage(ACTOR_DORA, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_StartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    Event_Wait(20);
    // "Garet is only a child. You can't expect him to bear such a burden..."
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
    // "The Wise One has spoken."
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    Event_Wait(10);
    // "Each of you shares responsibility for this."
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    // "Now, each of you must make your own decision!"
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    // "I don't know what to do. Isaac, you decide."
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_Wait(30);
  }
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
    Sound_Play(MUS_OVERNIGHT_AT_THE_INN);
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

/// Once Isaac accepts, Garet protests and the Mayor tells him the choice was
/// Isaac's; the Great Healer calls on the Wise One, who comes down over the
/// altar to give Isaac and Garet their mission while a red glow shimmers about
/// the Great Healer and sparkles circle him.
static void Sanctum_ReceiveWiseOnesMission(void) {
  u32 sparkIndex;

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
  Sound_Play(MUS_FOREST_THEME);
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
  // "You two are quite brave to take on this quest."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_Wait(20);
  Event_Wait(40);
  // "I shall be watching you. Choose your destiny wisely, young ones."
  Event_ShowMessage(ACTOR_WISE_ONE, 0);
  Event_Wait(60);
  Actor_SetSpriteColorById(ACTOR_WISE_ONE, SPRITE_COLOR_BLINK);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Sound_Play(SE_MATERIALIZE);
  for (sparkIndex = 0; sparkIndex < WISE_ONE_VANISH_SPARKS; sparkIndex++) {
    Sanctum_SpawnActorSpark(ACTOR_WISE_ONE);
    Event_Wait(4);
    Actor_Get(ACTOR_WISE_ONE)->scaleX -= FIXED_ONE / 100;
    Actor_Get(ACTOR_WISE_ONE)->scaleY -= FIXED_ONE / 100;
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
  Sound_Play(MUS_AN_UNSETTLING_FEELING);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_HEALER, 2);
  Event_Wait(20);
  Sanctum_StartAllRepeatedMotion();
  Event_Wait(30);
  // "That was the Wise One..."
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
  // "What did the Wise One say?"
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
  // "Felix must unlock the power."
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_Wait(20);
  Sanctum_ShowAllEmotes();
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(30);
  // "Find the lighthouses... You'll have the guidance of the Elemental Star..."
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
  // "Um... Then what?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Sanctum_StartAllRepeatedMotion();
  Event_Wait(30);
  // "What should they do there?"
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(80);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  // "Surely, that cannot be all the advice you can offer them, Great Healer..."
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_HEALER, 1);
  Actor_ShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
  Event_Wait(20);
  Sanctum_StartAllRepeatedMotion();
  Event_Wait(40);
  // "Seek the elementals... The Djinn..."
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_Wait(20);
  Sanctum_NodAll();
  Event_Wait(30);
  // "They will prove useful."
  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_Wait(30);
  Sanctum_NodAll();
  Event_Wait(120);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  // "Isn't there anything else?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
  Event_Wait(20);
  // "That is all."
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
  // "That's it!? Isn't there anything else you can tell them?"
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "That is all, Mayor."
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
  // "I leave it up to you, Isaac...Garet."
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
  // "H-Hey, isn't it almost... time to meditate?"
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
  // "Oh yeah! Meditation! Let's go!"
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
  // "Well, what good are those guys!? Sending children off with no help..."
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(20);
  // "I agree, but..."
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
  // "They didn't give us any other choice."
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
  // "Then, when should they go?"
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_DORA, 2);
  Event_Wait(20);
  // "It doesn't sound like we can afford to wait."
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Actor_ShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  // "Tomorrow, then?"
  Event_ShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_Wait(20);
  // "Tomorrow? Yes."
  Event_ShowMessage(ACTOR_DORA, 0);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_DORA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
}

/// Puts a red-tinted copy of the Great Healer on him, the glow of his
/// Psynergy.
static void Sanctum_ShowHealerAfterimage(void) {
  u32 entryIndex;
  struct Actor *afterimage;
  struct Actor *actor;
  struct Sprite *sprite;

  actor = Actor_Get(ACTOR_HEALER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_HEALER_AFTERIMAGE, actor->x.fixed,
                        actor->z.fixed);
  }
  Actor_SetAnimation(ACTOR_HEALER_AFTERIMAGE, ANIM_STILL);
  afterimage = Actor_Get(ACTOR_HEALER_AFTERIMAGE);
  afterimage->facing = Actor_Get(ACTOR_HEALER)->facing;
  Actor_Get(ACTOR_HEALER_AFTERIMAGE)->update = Sanctum_UpdateHealerAfterimage;
  sprite = Actor_Get(ACTOR_HEALER_AFTERIMAGE)->sprite;
  {
    for (entryIndex = 0; entryIndex < sprite->count; entryIndex++) {
      struct SpriteAnimation *entry = sprite->entries[entryIndex];

      if (entry != NULL && entry->script != 0) {
        entry->colorMode = SPRITE_COLOR_RED_TINT;
      }
    }
  }
  sprite->dirty = 1;
  Actor_Get(ACTOR_HEALER_AFTERIMAGE)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 2;
}

/// Takes the Great Healer's red glow away.
static void Sanctum_HideHealerAfterimage(void) {
  struct Actor *state = Actor_Get(ACTOR_HEALER_AFTERIMAGE);
  state->update = 0;
  Actor_SetPosition(ACTOR_HEALER_AFTERIMAGE, 0, 0);
}

/// Each frame moves the red copy a few pixels to another side of the Great
/// Healer, so that it shimmers about him.
static void Sanctum_UpdateHealerAfterimage(struct Actor *actor) {
  struct Actor *healer;

  healer = Actor_Get(ACTOR_HEALER);
  actor->targetX = actor->x.fixed = healer->x.fixed;
  actor->targetY = actor->y.fixed = healer->y.fixed;
  actor->targetZ = actor->z.fixed = healer->z.fixed - toFIXED(2);
  switch (gFrameCount & 3) {
  case 0:
    actor->targetX = actor->x.fixed = healer->x.fixed - toFIXED(7) / 2;
    break;
  case 1:
    actor->targetX = actor->x.fixed = healer->x.fixed + toFIXED(3);
    break;
  case 2:
    actor->targetY = actor->y.fixed = healer->y.fixed + toFIXED(2);
    break;
  case 3:
    actor->targetZ = actor->z.fixed = healer->z.fixed;
    break;
  }
}

/// Moves a sparkle one step along its ellipse around the Great Healer.
static void Sanctum_UpdateOrbitingEffect(struct Actor *effect) {
  struct Actor *anchor = effect->watchedActor;
  u16 orbitAngle = effect->timer;
  FIXED x;
  FIXED z;
  FIXED currentX;

  x = anchor->x.fixed + Trig_Cos(orbitAngle) * 14;
  effect->x.fixed = x;

  z = anchor->z.fixed + Trig_Sin(orbitAngle) * 10;
  currentX = effect->x.fixed;
  effect->z.fixed = z;
  effect->targetZ = z;
  effect->targetX = currentX;

  effect->timer += effect->userValue;
}

/// Starts a sparkle circling the actor, high above his feet, going around by
/// the given angle each frame.
static void Sanctum_SpawnOrbitingEffect(s32 actorId, s32 angleStep) {
  struct Actor *center;
  struct Actor *effect;
  struct Sprite *sprite;
  struct Sprite *sourceSprite;

  center = Actor_Get(actorId);
  if (center != NULL) {
    effect = FieldActor_Create(SPRITE_SPARKLE, center->x.fixed,
                               center->y.fixed + toFIXED(45), center->z.fixed);
    if (effect != NULL) {
      sprite = effect->sprite;
      Actor_StartScript(effect, gSanctum_OrbitingSparkleScript);
      effect->motionFlags = 0;
      effect->timer = 0;
      effect->userValue = angleStep;
      effect->update = Sanctum_UpdateOrbitingEffect;
      sprite->flags = 0;
      sourceSprite = center->sprite;
      effect->watchedActor = center;
      sprite->image.priority = sourceSprite->image.priority;
    }
  }
}

/// The question's window after the refusal ending, in tiles, and its cursor's
/// x in pixels.
#if LANGUAGE != LANGUAGE_JAPANESE
#define QUESTION_X 2
#define QUESTION_WIDTH 25
#define CURSOR_X 24
#else
#define QUESTION_X 7
#define QUESTION_WIDTH 17
#define CURSOR_X 64
#endif

/// After the refusal ending, clears the room and asks whether to start again
/// "From the Great Healer" (0) or go to the file screen (1).
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
  // "To the File Screen", or "Restart the game" when there is no saved file.
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
    x = gSanctum_CursorOffsets[(gVBlankCount >> 1) & 15] + CURSOR_X;
    FieldMenuArrow_SetPosition(&cursor, x, (choice << 4) + 60);
    Event_Wait(1);
  }
  MenuArrow_Release(slot);
  Window_Close(window, 1);
  return choice;
}

/// Isaac and Garet act out, in sepia, what happened in Sol Sanctum, the item
/// each part of the story turns on rising over them.
static void Sanctum_ActOutSolSanctum(void) {
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
  // The Mars Star rises over them.
  Sanctum_RaiseItemIcon(ITEM_MARS_STAR, toFIXED(184), toFIXED(27),
                        toFIXED(168));
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
  Actor_Get(ACTOR_HEALER)->timer = ACTOR_GARET;
  Actor_Get(ACTOR_HEALER)->update =
      (void (*)(struct Actor *))Sanctum_FaceActorNamedByTimer;
  Actor_Get(ACTOR_GARETS_GRANDFATHER)->timer = ACTOR_GARET;
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

/// An item's menu icon leaps up from a place (16.16) and hangs where it
/// stops, for a second in all.
static void Sanctum_RaiseItemIcon(s32 item, FIXED fixedX, FIXED fixedY,
                                  FIXED fixedZ) {
  struct Actor *icon;
  struct Sprite *sprite;
  GlyphTransfer *glyph;
  u32 frame;

  icon = FieldActor_Create(SPRITE_BLANK, fixedX, fixedY, fixedZ);
  {
    u8 cleared = 0;

    if (icon != NULL) {
      Actor_StartScript(icon, gItemIcon_GrowScript);
      sprite = icon->sprite;
      sprite->flags = cleared;
      sprite->count = cleared;
      sprite->image.fullColor = 0;
      sprite->image.palette = 0;
      icon->velocityY = toFIXED(2);
      icon->gravity = toFIXED(1) / 4;
      glyph = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
      Icon_DrawItem(item);
      VramCache_Load(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
      HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
      for (frame = 0; frame < 60; frame++) {
        if (icon->velocityY > -toFIXED(1) / 256 &&
            icon->velocityY < toFIXED(1) / 256)
          icon->motionFlags = 0;
        wait(1);
      }
      Actor_StartScript(icon, gSanctum_ItemIconEndScript);
    }
  }
}

/// A glint's update: it drifts up, twinkling between full size and three
/// fifths, and goes once its frames run out.
static void Sanctum_UpdateRisingSpark(struct Actor *spark) {
  FIXED scale;
  s32 framesLeft;

  // The timer holds the upward step in sixteenths of a pixel.
  spark->y.fixed += spark->timer * (toFIXED(1) / 16);
  spark->targetY = spark->y.fixed;
  scale = 0;
  switch (((s16)spark->userValue >> 2) & 3) {
  case 0:
    scale = FIXED_ONE;
    break;
  case 1:
  case 3:
    scale = 4 * FIXED_ONE / 5;
    break;
  case 2:
    scale = 3 * FIXED_ONE / 5;
    break;
  }
  spark->scaleX = scale;
  spark->scaleY = scale;
  framesLeft = spark->userValue - 1;
  spark->userValue = framesLeft;
  if ((s16)framesLeft <= 0)
    Actor_StartScript(spark, gSanctum_SparkEndScript);
}

/// A glint rises from beside the actor, for half a second to a second and a
/// half.
static void Sanctum_SpawnActorSpark(s32 id) {
  struct Actor *actor;
  struct Actor *spark;
  struct Sprite *sprite;
  FIXED x;
  s32 randomOffset;

  actor = Actor_Get(id);
  if (actor == NULL)
    return;
  randomOffset = Math_RemainderUnsigned(Random_Next(), 20);
  x = actor->x.fixed + INTtoFIXED(randomOffset);
  x -= toFIXED(10);
  spark = FieldActor_Create(SPRITE_GLINT, x,
                            actor->y.fixed + INTtoFIXED(Random_Next() & 15) -
                                toFIXED(8),
                            actor->z.fixed);
  if (spark == NULL)
    return;
  sprite = spark->sprite;
  spark->motionFlags = 0;
  spark->timer = Math_RemainderUnsigned(Random_Next(), 10) + 5;
  spark->userValue = Math_RemainderUnsigned(Random_Next(), 60) + 30;
  spark->update = Sanctum_UpdateRisingSpark;
  sprite->flags = 0;
  sprite->image.priority = actor->sprite->image.priority;
}

/// Takes away the small stone counter before the altar for the quest's visits:
/// its picture goes, and its cells (11, 8) to (13, 9) become plain floor.
static void Sanctum_RemoveCounter(void) {
  MapGrid_CopyCellsTo(0, 64, 11, 68, 3, 2);
  MapGrid_CopyCellAttributes(11, 10, 3, 2, 11, 8);
  wait(1);
}

/// Whether the leader faces north, across the counter toward the Healer.
static s32 Sanctum_IsLeaderFacingHealer(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  if (leader->facing > FACING_NORTHWEST && leader->facing < FACING_NORTHEAST) {
    return 1;
  }
  return 0;
}

/// Talking to the Great Healer in Vale's Sanctum: across the counter, the
/// Sanctum's services. From any other side he says that wielders of Psynergy
/// are called Adepts, messengers of good; once the party has left Vale, that
/// his control over Psynergy has grown since he met the Wise One; once it has
/// reached Kalay, that the path to Sol Sanctum is still closed.
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

/// Talking to the Healer of Vault's Sanctum: across the counter, the
/// Sanctum's services. From any other side, "We will help you anytime, as long
/// as you are pure of heart" while the gold statue is missing, "It was the hand
/// of fate that returned the gold statue" once it is back, and on the later
/// visit through Vault, that there seem to be a lot of earthquakes on Mt.
/// Aleph. It also asks Isaac again (`Sanctum_AskQuestAgain`) if run during the
/// quest's visits, whose own trigger table runs that directly instead.
static void Sanctum_TalkToVaultHealer(void) {
  s32 entrance;

  if (Sanctum_IsLeaderFacingHealer() != 0) {
    Sanctum_Run(ACTOR_HEALER);
    return;
  }

  Event_Start();

  entrance = gMainRoutineState.entrance;

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
    Sanctum_AskQuestAgain();
    return;
  default:
    break;
  }

  Event_ShowMessage(ACTOR_HEALER, 0);
  Event_End();
}

/// Talking to the Healer of Bilibin's Sanctum: across the counter, the
/// Sanctum's services. From any other side, that this is only a beginning
/// while Kolima is cursed, and once the curse is lifted, that it is still not
/// safe outside town.
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

/// Talking to the Healer of Altin's Sanctum: across the counter, the Sanctum's
/// services. From any other side, how the water-spewing beasts appeared in the
/// mine after the night the gems fell from the sky; once the party has beaten
/// them, that saving Altin was the hand of Fate.
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

/// Talking to the Healer of Xian's Sanctum: across the counter, the Sanctum's
/// services; from any other side, "I came to Xian from very distant lands to
/// spread good will."
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

#include "shared/item_icon_grow_script.c"

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
    ACTOR_PLACEMENT(SPRITE_SANCTUM_GOLD_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gSanctum_QuestActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        208, 0, 168, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        224, 0, 216, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        246, 0, 198, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 208, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 120, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
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
    { TRIGGER_EXIT, CELL_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { TRIGGER_TABLE_END },
};

struct Trigger gSanctum_VaultTriggers[] = {
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkToVaultHealer) },
    { TRIGGER_TALK, ACTOR_SANCTUM_MEMBER, FLAG_VAULT_THIEVES_JAILED, EVENT(Sanctum_TalkAboutGoldenStatue) },
    { TRIGGER_TALK, ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT(Sanctum_TalkAboutGoldenStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumGoOnIfThatEasesThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT(Sanctum_ReadMindAboutGoldenStatue) },
    { TRIGGER_EXIT, CELL_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_VAULT },
    { TRIGGER_TABLE_END },
};

struct Trigger gSanctum_VaultRevisitTriggers[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_VAULT_REVISIT },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkToVaultHealer) },
    { TRIGGER_TALK, ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT(Sanctum_TalkAboutGoldenStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumIHopeTheEarthquakesArentA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, EVENT(Sanctum_ReadMindAboutGoldenStatue) },
    { TRIGGER_TABLE_END },
};

struct Trigger gSanctum_QuestTriggers[] = {
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_AskQuestAgain) },
    { TRIGGER_TALK, ACTOR_GARET, CONDITION_ALWAYS, MSGNO(MsgSanctumItIsATerribleBurdenTo) },
    { TRIGGER_TALK, ACTOR_DORA, CONDITION_ALWAYS, MSGNO(MsgSanctumMotherWouldNeverDoubtYourCourage) },
    { TRIGGER_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgSanctumIDontBlameYouICouldnt) },
    { TRIGGER_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWasItRightToRefuseWhen) },
    { TRIGGER_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItsClearThatTheGreatHealer) },
    { TRIGGER_STEP, CELL_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, EVENT(Sanctum_LeaveAfterRefusing) },
    { TRIGGER_STEP, CELL_BEHAVIOR_SANCTUM_BEFORE_DOOR, FLAG_SANCTUM_WARNED_BEFORE_LEAVING, EVENT(Sanctum_WarnBeforeLeaving) },
    { TRIGGER_TABLE_END },
};

struct Trigger gSanctum_ValeTriggersAfterLeaving[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkToHealer) },
    { TRIGGER_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumThePowerOfMtAlephStems) },
    { TRIGGER_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_AskAboutPsynergyChanges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumThePowerOfMyPsynergyHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumTheAngaranContinentWillCertainlySee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumMyPsynergyHasntChangedAtAll) },
    { TRIGGER_TABLE_END },
};

struct Trigger gSanctum_ValeTriggersAfterKalay[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkToHealer) },
    { TRIGGER_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_AskAboutNewAdepts) },
    { TRIGGER_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWhyIsItThatDifferentAdepts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItllBeDifficultToReachSol) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumISeeABeautifulGirlHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItsNotFairIWantTo) },
    { TRIGGER_TABLE_END },
};

struct Trigger gSanctum_AltinTriggers[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_ALTIN },
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
    { TRIGGER_EXIT, CELL_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_BILIBIN },
    { TRIGGER_TABLE_END },
};

struct Trigger gSanctum_XianTriggers[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_XIAN },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(Sanctum_TalkAboutJourneyFromXian) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWhenYouStayInXianFor) },
    { TRIGGER_TABLE_END },
};
// clang-format on

/// The choice cursor's offset to the right in pixels, swaying a step every two
/// frames.
s32 gSanctum_CursorOffsets[] = {
    0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 3, 2, 2, 1, 1, 1,
};

/// A sparkle circling the Great Healer rises half a pixel a frame for 96
/// frames, then is gone.
s32 gSanctum_OrbitingSparkleScript[] = {
    SC_ADD(SW_Y, -toFIXED(1) / 2),
    SC_ADD(SW_TARGET_Y, -toFIXED(1) / 2),
    SC_WAIT(1),
    SC_LOOP(96, SC_TOP),
    SC_RELEASE,
};

s32 gSanctum_SparkEndScript[] = {
    SC_RELEASE,
};
