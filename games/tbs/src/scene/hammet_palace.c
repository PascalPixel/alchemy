/// @file
/// Hammet Palace (SceneId_HammetPalace), the rooms of Lord Hammet's palace in
/// Kalay. The map before its door, also Hammet Palace in the game's area names,
/// is kalay.c's HammetPalaceGrounds.
///
/// Two events in the room where Lady Layana stands frame the party's part in
/// Hammet's rescue from Lunpa, and between them three flags pick where she
/// stands and what she says:
///
/// - Ivan's report (HAMMET_PALACE_LAYANAS_ROOM_FROM_HAMMET_PALACE_GROUNDS),
///   when the guards at the door bring the party in (kalay.c); it ends by
///   setting FLAG_IVAN_REPORTED_TO_LAYANA.
/// - Between the two, until the party has heard in Lunpa that Hammet is held
///   there (area temp flag FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA, set by
///   kalay.c's events), Layana talks of Dodonpa's spies. Afterwards she
///   stands elsewhere in her room, and the first time they speak she scolds
///   `Isaac, you sneaked into Lunpa? That's far too dangerous!`
///   (FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS); from then on she talks of
///   Dodonpa's greed.
/// - Hammet's homecoming (HAMMET_PALACE_LAYANAS_ROOM_FROM_ENTRANCE_ROOM, the
///   first time once Hammet is free, FLAG_DODONPA_LOCKED_UP;
///   FLAG_HAMMET_BACK_HOME_SEEN), in which Hammet sends the party on to
///   Hesperia. Afterwards Hammet stands beside Layana, and the door to the
///   underground passage, opened for the party in thanks, is drawn open on
///   entering by HAMMET_PALACE_PASSAGE_DOOR_FROM_EAST_WING or
///   HAMMET_PALACE_FROM_KALAY_TUNNEL_TREASURE_ROOM with FLAG_KALAY_TUNNEL_OPEN
///   set.
///
/// The people of the palace (ACTOR_WORRIED_ABOUT_THE_TOWNSFOLK to
/// ACTOR_ASKING_ABOUT_THE_SILK_TRADE) have one line before Hammet is free and
/// another after. Walking north into a door (step triggers 2, 3, 4 and 14)
/// opens it; the other exits join the rooms plainly.
/// HAMMET_PALACE_TO_HAMMET_PALACE_GROUNDS leads out to the front of the palace
/// (HAMMET_PALACE_GROUNDS_FROM_HAMMET_PALACE), and
/// HAMMET_PALACE_TO_KALAY_TUNNEL_TREASURE_ROOM down into the Kalay Tunnel
/// (KALAY_TUNNEL_TREASURE_ROOM_FROM_HAMMET_PALACE). Searching finds a turban,
/// the symbol of Kalay's soldiers, and two books, `Business and You` and `The
/// Good Host`; a chest holds a Water Jacket and a barrel a Nut.
#include "save_block.h"
#include "scene_ids.h"
#include "unit/scene/hammet_palace.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "flags.h"
#include "songs.h"
#include "palbuf.h"

/// The party has heard in Lunpa that Hammet is held there, or has freed him, so
/// Layana waits elsewhere in her room.
/// ★ Named from the game: "you sneaked into Lunpa? That's far too dangerous!"
/// (MsgKalayIsaacSneakedIntoLunpaThats)
#define FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA FLAG_AREA_TEMP_21

/// The palace's actors, numbered in the order of
/// gHammetPalaceActorTemplates from ACTOR_FIRST_PLACED; its rows for
/// Garet, Ivan and Mia keep the party's own slots.
enum HammetPalaceActorId {
  /// ★ Named from the game: "Lady Layana was crying again last night."
  /// (MsgHammetPalaceLadyLayanaWasCryingAgainLast)
  ACTOR_LAYANA = ACTOR_FIRST_PLACED,
  /// Beside Layana, and kept out of sight until he is free.
  /// ★ Named from the game: "I'm so happy Hammet made it back safely"
  /// (MsgHammetPalaceImSoHappyHammetMadeIt)
  ACTOR_HAMMET,
  /// The soldiers of Kalay in Ivan's report (SPRITE_SOLDIER_IN_TURBAN, as the
  /// guards at the palace door in kalay.c), out of sight at other times.
  /// ★ Named from what the player sees: a soldier of Kalay in a white turban
  /// with a spear (its render)
  ACTOR_FIRST_SOLDIER,
  /// ★ Named from what the player sees: the second soldier of Kalay (its
  /// render)
  ACTOR_SECOND_SOLDIER,
  // The people of the palace, each with one line before Hammet is free and
  // another after.
  /// A young man with brown hair in a purple shirt who worries that the
  /// townsfolk will learn Hammet is a hostage. "The townsfolk don't know about
  /// Lord Hammet yet, do they?" After: "I hear Hammet was saved by you
  /// people..."
  /// ★ Named from the game: "The townsfolk don't know about Lord Hammet yet"
  /// (MsgHammetPalaceTheTownsfolkDontKnowAboutLord)
  ACTOR_WORRIED_ABOUT_THE_TOWNSFOLK,
  /// A woman with fair hair who asks whether Hammet will be freed. "Lord Hammet
  /// will be released soon. Right?" After: "They say Hammet's return is
  /// supposed to be a secret for now."
  /// ★ Named from the game: "Lord Hammet will be released soon. Right?"
  /// (MsgKalayLordHammetWillReleasedSoon)
  ACTOR_ASKING_IF_HAMMET_WILL_BE_RELEASED,
  /// A grey-haired man in a black suit who tells of Layana's grief, and later
  /// of the door Layana had opened for the party. "Layana is putting up a
  /// strong front whenever people are around." After: "I left the door to the
  /// underground passage open, so go have a look."
  /// ★ Named from the game: "I left the door to the underground passage open"
  /// (MsgHammetPalaceILeftTheDoorToThe)
  ACTOR_LEFT_THE_PASSAGE_DOOR_OPEN,
  /// A bald man in red who fears Dodonpa's men. "Merchants are supposed to be
  /// protected under the laws of Angara." After: "I wonder if any of Dodonpa's
  /// thugs have come here after Hammet..."
  /// ★ Named from the game: "I wonder if any of Dodonpa's thugs have come here"
  /// (MsgHammetPalaceIWonderIfAnyOfDodonpas)
  ACTOR_AFRAID_OF_DODONPAS_THUGS,
  /// A young man with brown hair in green who asks whether Hammet's silk trade
  /// has ended. "Has the legacy of Lord Hammet's silk trading come to an end?"
  /// After: "The secret passage is locked because Hammet's treasure is in
  /// there."
  /// ★ Named from the game: "Has the legacy of Lord Hammet's silk trading come
  /// to an end?" (MsgKalayHasLegacyLordHammetsSilk)
  ACTOR_ASKING_ABOUT_THE_SILK_TRADE
};

// Entrance setup and the two long conversations use the helpers below.
static void HammetPalace_SetUpEntrance(void);
static void HammetPalace_IvanReportsToLayana(void);
static void HammetPalace_FacePartyOutward(s32 waitFrames);
static void HammetPalace_FacePartyNorth(s32 waitFrames);
static void HammetPalace_HammetsHomecoming(void);

/// Advances within the current conversation's consecutive message block.
static __inline__ void HammetPalace_AdvanceMessage(s32 amount) {

  gFieldWork->message += amount;
}

/// Selects Hammet Palace's entrances, or the fallback table for any
/// other map number.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_HammetPalace) {
    return gHammetPalaceEntrances;
  }
  return gHammetPalaceEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gHammetPalaceExits; }

/// Applies actor template clipping before returning Hammet Palace's
/// actor rows.
const struct ActorTemplate *Scene_GetActorTemplates(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_HammetPalace) {
    Scene_AssignViewFlags(gHammetPalaceActorTemplates);
    return gHammetPalaceActorTemplates;
  }
  return gHammetPalaceActorTemplatesOther;
}

/// Selects Hammet Palace's dialogue and doorway handlers.
const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_HammetPalace) {
    return gHammetPalaceTriggers;
  }
  return gHammetPalaceTriggersOther;
}

/// Talking to one of the people of Hammet Palace before Hammet is free: he
/// hopes for his release. `Lord Hammet will be released soon. Right?`
static void EventScript_AskWhetherHammetWillBeReleased(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgKalayLordHammetWillReleasedSoon);
  FieldScript_AskYesNoAndShowResponse(ACTOR_ASKING_IF_HAMMET_WILL_BE_RELEASED,
                                      0);
  FieldScript_End();
}

/// Talking to one of the people of Hammet Palace before Hammet is free: he
/// worries for Hammet's silk trade. `Has the legacy of Lord Hammet's silk
/// trading come to an end?`
static void EventScript_AskWhetherSilkTradeHasEnded(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgKalayHasLegacyLordHammetsSilk);
  FieldScript_AskYesNoAndShowResponse(ACTOR_ASKING_ABOUT_THE_SILK_TRADE, 0);
  FieldScript_End();
}

/// Layana scolds Isaac for sneaking into Lunpa. It's the first talk with her in
/// Hammet Palace after the party has heard in Lunpa that Hammet is held there
/// (area temp flag FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA). `Isaac, you
/// sneaked into Lunpa? That's far too dangerous!` She is glad Hammet is unhurt,
/// but asks the party not to go back, and to fulfill their quest.
static void EventScript_LayanaScoldsLunpaVisit(void) {

  FieldScript_Start();
  FieldEffect_SpawnLinked(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  FieldScript_SetMessageId(MsgKalayIsaacSneakedIntoLunpaThats);
  FieldScript_ShowMessageThenWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 2);
  FieldScript_ShowMessageThenWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
  FieldScript_ShowMessageThenWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  FieldScript_ShowMessageThenWait(ACTOR_LAYANA, 0, 10);
  GameFlag_Set(FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS);
  FieldScript_End();
}

/// Walking north into a door of Hammet Palace (step triggers 2, 3, 4 and 14):
/// it opens, and the leader walks through it to the next room.
static void EventScript_LeaveThroughDoor(void) {

  struct FieldWork *work;

  work = gFieldWork;
  FieldScript_Start();
  FieldScript_WaitFrames(10);
  if (work->stepTrigger == 4) {
    Script_PlaySong(SE_THUMP);
  } else {
    Script_PlaySong(SE_DOOR_OPEN);
  }
  TilesetAnim_Resume(1);
  TilesetAnim_Resume(2);
  FieldScript_WaitFrames(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (work->stepTrigger == 4) {
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  } else {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  }
  FieldScript_WaitFrames(16);
  Import_ScriptRequestExit(work->stepTrigger);
  // Restore the layer entries before returning control to the field engine.
  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);
  FieldScript_End();
}

/// Opens Hammet Palace with its window transition and runs
/// entrance-specific setup.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  if (gSaveBlock.sceneId == (u32)&SceneId_HammetPalace) {
    HammetPalace_SetUpEntrance();
  }
  return 0;
}

/// Sets up the room the party enters by. In Layana's room
/// (HAMMET_PALACE_LAYANAS_ROOM_FROM_ENTRANCE_ROOM), Hammet stands beside her
/// once he is free, and his homecoming plays the first time; before that she
/// stands elsewhere once the party has heard of him in Lunpa
/// (FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA). By the door to the underground
/// passage (HAMMET_PALACE_PASSAGE_DOOR_FROM_EAST_WING and
/// HAMMET_PALACE_FROM_KALAY_TUNNEL_TREASURE_ROOM) the door is drawn open once
/// FLAG_KALAY_TUNNEL_OPEN is set.
/// HAMMET_PALACE_LAYANAS_ROOM_FROM_HAMMET_PALACE_GROUNDS is Ivan's report.
static void HammetPalace_SetUpEntrance(void) {
  struct Actor *actor;
  s32 facing;
  s32 waitingX = PIXELS(906);
  s32 waitingZ = PIXELS(422);

  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);

  switch (gSaveBlock.entrance) {
  case HAMMET_PALACE_LAYANAS_ROOM_FROM_ENTRANCE_ROOM:
    if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
      // Hammet is home.
      actor = Actor_GetById(ACTOR_LAYANA);
      facing = FACING_EAST + FACING_STEP;
      actor->facing = facing;

      if (GameFlag_Get(FLAG_HAMMET_BACK_HOME_SEEN) == 0) {
        HammetPalace_HammetsHomecoming();
      }
    } else {
      Import_ActorSetPosition(ACTOR_HAMMET, 0, 0);
      if (GameFlag_Get(FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA) != 0) {
        Import_ActorSetPosition(ACTOR_LAYANA, waitingX, waitingZ);
        actor = Actor_GetById(ACTOR_LAYANA);
        facing = FACING_NORTH + FACING_STEP;
        actor->facing = facing;
      }
    }
    break;

  case HAMMET_PALACE_PASSAGE_DOOR_FROM_EAST_WING:
  case HAMMET_PALACE_FROM_KALAY_TUNNEL_TREASURE_ROOM:
    if (GameFlag_Get(FLAG_KALAY_TUNNEL_OPEN) != 0) {
      MapGrid_CopyCellsTo(58, 70, 54, 70, 4, 3);
      {
        MapGrid_CopyCellAttributes(55, 9, 2, 1, 55, 8);
      }
      MapGrid_CenterOnFollowedPoint();
      WaitFrames(1);
    }
    break;

  case HAMMET_PALACE_LAYANAS_ROOM_FROM_HAMMET_PALACE_GROUNDS:
    Import_ActorSetPosition(ACTOR_HAMMET, 0, 0);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      HammetPalace_IvanReportsToLayana();
    }
    break;

  default:
    break;
  }
}

/// Ivan's report in Hammet Palace, when the guards at the door bring the party
/// to Layana: she had hoped Hammet came back with him.
/// The ransom was paid long ago, yet Dodonpa holds Hammet still; Layana will
/// not risk him by sending the soldiers, nor the party. She tells of the
/// prophecy, the Jupiter Adept who gave Hammet the Shaman's Rod and Ivan, and
/// what the rod is to find, then sends everyone back to their posts.
static void HammetPalace_IvanReportsToLayana(void) {

  struct FieldWork *work;
  s32 advanceAfterReply;
  const s32 *partyScript;

  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  PaletteFade_ApplySource(PALETTE_TINT_SEPIA, PALETTES_ALL);
  // The report opens in sepia, with the party placed under the closed screen;
  // the colours come back after the silent exchange of nods that opens it.
  PaletteFade_ApplyTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
  PaletteFade_StartInterpolation(1);
  WaitFrames(1);
  work = gFieldWork;
  work->transitionFrames = 24;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Actor_SetPosition(ACTOR_LAYANA, PIXELS(856), PIXELS(440));
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(856), PIXELS(486));
  Actor_SetPosition(ACTOR_GARET, PIXELS(848), PIXELS(502));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(872), PIXELS(486));
  Actor_SetPosition(ACTOR_MIA, PIXELS(880), PIXELS(502));
  Actor_SetPosition(ACTOR_FIRST_SOLDIER, PIXELS(840), PIXELS(518));
  Actor_SetPosition(ACTOR_SECOND_SOLDIER, PIXELS(888), PIXELS(518));
  FieldCamera_MoveTo(PIXELS(864), -1, PIXELS(472), 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 1);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_IVAN, 1);
  ObjectMotion_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_PARTY_LEADER, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetVariantCallback(ACTOR_GARET, 1);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_MIA, 1);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 10);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  HammetPalace_FacePartyNorth(20);
  Actor_SetAttachedEffect(ACTOR_LAYANA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  ObjectMotion_SetVariantCallback(ACTOR_LAYANA, 2);
  FieldScript_WaitFrames(60);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTH + FACING_STEP, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectMotion_SetVariantCallback(ACTOR_FIRST_SOLDIER, 2);
  FieldScript_WaitFrames(60);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_NORTHEAST + FACING_STEP, 20);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_SECOND_SOLDIER, 2);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_WEST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_NORTHWEST + FACING_STEP, 20);
  ObjectMotion_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 2);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_SECOND_SOLDIER, 2);
  ObjectMotion_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  ObjectMotion_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_StartInterpolation(40);
  WaitFrames(60);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldScript_SetMessageId(MsgKalayWhenHeardWereBackIvan);
  // The following replies advance through this conversation's message block.
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_FIRST_SOLDIER, 2);
  Script_ShowMessageAndWait(ACTOR_FIRST_SOLDIER, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(ACTOR_SECOND_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 10);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_SECOND_SOLDIER, 2);
  Script_ShowMessageAndWait(ACTOR_SECOND_SOLDIER, 0, 10);
  Import_ActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(ACTOR_FIRST_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_SECOND_SOLDIER, 2);
  Script_ShowMessageAndWait(ACTOR_SECOND_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_FIRST_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 10);
  ObjectMotion_SetVariantCallback(ACTOR_IVAN, 1);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 10);
  Script_AskYesNo(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN,
                  0);
  Actor_SetAttachedEffect(ACTOR_LAYANA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_IVAN, 2);
  FieldScript_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
  ObjectMotion_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_GARET, 0, 10);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  ObjectMotion_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(ACTOR_MIA, 0, 40);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_EAST + FACING_STEP, 40);
  Actor_SetSpeed(ACTOR_LAYANA, 2 * PIXELS(1) / 5, PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_LAYANA, 892, 440);
  FieldScript_WaitFrames(40);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_NORTH + FACING_STEP, 20);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  HammetPalace_FacePartyOutward(60);
  HammetPalace_FacePartyNorth(40);
  Actor_WalkToAndWait(ACTOR_LAYANA, 856, 440);
  FieldScript_WaitFrames(40);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_WEST + FACING_STEP, 20);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_NORTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_WEST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_NORTHWEST + FACING_STEP, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 1);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 20);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_FIRST_SOLDIER, 2);
  Script_ShowMessageAndWait(ACTOR_FIRST_SOLDIER, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 2);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTH + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_WEST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_NORTHEAST + FACING_STEP, 20);
  Script_ShowMessageAndWait(ACTOR_FIRST_SOLDIER, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_SECOND_SOLDIER, 2);
  FieldScript_WaitFrames(20);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_NORTHWEST + FACING_STEP, 20);
  ObjectMotion_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_SECOND_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_WEST + FACING_STEP, 40);
  Import_ActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_NORTHWEST + FACING_STEP, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_FIRST_SOLDIER, 2);
  Script_ShowMessageAndWait(ACTOR_FIRST_SOLDIER, 0, 20);
  HammetPalace_FacePartyNorth(20);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 2);
  FieldScript_WaitFrames(40);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_LAYANA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 20);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Import_ActorSetAnimation(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_GARET, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
  Script_OpenMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  advanceAfterReply = 1;
  if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 1) {
    gFieldWork->message++;
    advanceAfterReply = 0;
  }
  Script_ShowMessageAndWait(ACTOR_GARET, 0, 10);
  if (advanceAfterReply != 0) {
    gFieldWork->message++;
  }
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 10);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTHEAST + FACING_STEP, 20);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_IVAN, 1);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
  FieldScript_ShowMessageThenWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
  Script_OpenMessage(ACTOR_LAYANA, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
    gFieldWork->message++;
  } else {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    gFieldWork->message++;
    Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  }
  HammetPalace_FacePartyNorth(20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 40);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 10);
  Script_ShowMessageAndWait(ACTOR_MIA, 0, 20);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 40);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 2);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 40);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_IVAN, 1);
  FieldScript_WaitFrames(20);
  Actor_SetSpeed(ACTOR_IVAN, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_IVAN, 868, 472);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  HammetPalace_FacePartyOutward(40);
  HammetPalace_FacePartyNorth(20);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_GARET, 2);
  Script_ShowMessageAndWait(ACTOR_GARET, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 1);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTH + FACING_STEP, 10);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTHEAST + FACING_STEP, 10);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  Script_OpenMessage(ACTOR_LAYANA, 0);
  ObjectMotion_SetVariantCallback(ACTOR_GARET, 1);
  ObjectMotion_SetVariantCallback(ACTOR_IVAN, 1);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_MIA, 1);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    Actor_SetAttachedEffect(ACTOR_LAYANA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(40);
    Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
    gFieldWork->message += 2;
  } else {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(ACTOR_GARET, 2);
    gFieldWork->message++;
    Script_ShowMessageAndWait(ACTOR_GARET, 0, 10);
    Actor_SetAttachedEffect(ACTOR_LAYANA,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldScript_WaitFrames(40);
    Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  }
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 10);
  Script_ShowMessage(ACTOR_MIA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 1);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  HammetPalace_FacePartyNorth(10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  ObjectMotion_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Actor_SetAttachedEffect(ACTOR_LAYANA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  HammetPalace_FacePartyOutward(40);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  HammetPalace_FacePartyNorth(20);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_IVAN, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(ACTOR_MIA, 0, 10);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_MIA, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessageAndWait(ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTH + FACING_STEP, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_FIRST_SOLDIER, 2);
  ObjectMotion_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(ACTOR_FIRST_SOLDIER, 0, 10);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTHEAST + FACING_STEP, 10);
  Import_ActorSetAnimation(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 1);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTH + FACING_STEP, 10);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_IVAN, 1);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTHEAST + FACING_STEP, 10);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  HammetPalace_FacePartyOutward(40);
  HammetPalace_FacePartyNorth(20);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_GARET, 1);
  Script_ShowMessageAndWait(ACTOR_GARET, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Import_ActorSetAnimation(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 1);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetVariantCallback(ACTOR_IVAN, 1);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_MIA, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 1);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Import_ActorSetAnimation(ACTOR_LAYANA, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 10);
  Script_ShowMessageAndWait(ACTOR_MIA, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  ObjectMotion_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 10);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_MIA, 2);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 10);
  Script_ShowMessageAndWait(ACTOR_MIA, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetVariantCallback(ACTOR_FIRST_SOLDIER, 2);
  Script_ShowMessageAndWait(ACTOR_FIRST_SOLDIER, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_SECOND_SOLDIER, 0, 20);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  FieldScript_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 1);
  Script_OpenMessage(ACTOR_LAYANA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    HammetPalace_FacePartyNorth(10);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    FieldScript_WaitFrames(10);
    Script_ShowMessageAndWait(ACTOR_GARET, 0, 10);
    gFieldWork->message += 2;
  } else {
    FieldScript_WaitFrames(10);
    ObjectMotion_SetVarCbAndRefresh(ACTOR_GARET, 2);
    gFieldWork->message++;
    Script_ShowMessageAndWait(ACTOR_GARET, 0, 10);
    HammetPalace_FacePartyNorth(10);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    FieldScript_WaitFrames(10);
    Script_ShowMessageAndWait(ACTOR_GARET, 0, 10);
  }
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 40);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTH + FACING_STEP, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 1);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_FIRST_SOLDIER, 1);
  FieldScript_WaitFrames(20);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTHEAST + FACING_STEP, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 1);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_SECOND_SOLDIER, 1);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Import_ActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Actor_SetSpeed(ACTOR_FIRST_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_SECOND_SOLDIER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_FIRST_SOLDIER, 848, 540);
  Actor_WalkToAndWait(ACTOR_SECOND_SOLDIER, 880, 540);
  Actor_SetPosition(ACTOR_FIRST_SOLDIER, PIXELS(0), PIXELS(0));
  Actor_SetPosition(ACTOR_SECOND_SOLDIER, PIXELS(0), PIXELS(0));
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  partyScript = gHammetPalace_PartyActions;
  Actor_SetActionScript(ACTOR_GARET, partyScript);
  Actor_SetActionScript(ACTOR_IVAN, partyScript);
  Actor_RunActionScriptAndWait(ACTOR_MIA, partyScript);
  work = gFieldWork;
  work->transitionFrames = 16;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  GameFlag_Set(FLAG_IVAN_REPORTED_TO_LAYANA);
  FieldScript_End();
}

/// Turns the party outward from the group, then optionally waits.
static void HammetPalace_FacePartyOutward(s32 waitFrames) {
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  if (waitFrames != 0) {
    FieldScript_WaitFrames(waitFrames);
  }
}

/// Turns all four party members north toward Layana, then optionally waits.
static void HammetPalace_FacePartyNorth(s32 waitFrames) {
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  if (waitFrames != 0) {
    FieldScript_WaitFrames(waitFrames);
  }
}

/// Hammet's homecoming in Hammet Palace, the first time the party walks into
/// Layana's room once he is free: Hammet welcomes them, Layana scolds Ivan for
/// going to Lunpa against her word, and Ivan decides to leave at once on the
/// quest. Hammet sends them to Hesperia with the Shaman's Rod, as the Jupiter
/// Adept said.
static void HammetPalace_HammetsHomecoming(void) {

  struct Actor *leader;
  struct FieldWork *work;
  s32 v5;
  const s32 *partyScript;

  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  FieldCamera_MoveTo(PIXELS(864), -1, PIXELS(536), 0);
  // Pan from the entrance while the party walks into its speaking positions.
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(864), PIXELS(630));
  gFieldWork->transition = v5 = 0x100;
  gFieldWork->transitionFrames = 40;
  FieldScript_StartScreenTransition();
  FieldCamera_SetSpeed(0x6666, 0xccc);
  FieldCamera_MoveTo(PIXELS(864), -1, PIXELS(472), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_SetSpeed(ACTOR_GARET, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_SetSpeed(ACTOR_IVAN, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_SetSpeed(ACTOR_MIA, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 864, 498);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
  }
  Actor_WalkTo(ACTOR_PARTY_LEADER, 856, 486);
  Actor_WalkTo(ACTOR_GARET, 848, 502);
  Actor_WalkTo(ACTOR_IVAN, 872, 486);
  Actor_WalkToAndWait(ACTOR_MIA, 880, 502);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  FieldScript_WaitFrames(10);
  HammetPalace_FacePartyNorth(10);
  Actor_ShowEmote(ACTOR_HAMMET, v5, 20);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 20);
  FieldScript_SetMessageId(MsgKalayItsIvanHisCompanionsPerfect);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMET, 0, 10);
  Actor_ShowEmote(ACTOR_LAYANA, v5, 20);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTHEAST + FACING_STEP, 20);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 20);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(60);
  Actor_ShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTHWEST + FACING_STEP, 10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMET, 0, 10);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_EAST + FACING_STEP, 10);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 40);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 2);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 20);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTHEAST + FACING_STEP, 10);
  Script_OpenMessage(ACTOR_LAYANA, 0);
  v5 = 1;
  if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(10);
    Import_ActorSetAnimation(ACTOR_LAYANA, ANIM_NOD);
  } else {
    FieldScript_WaitFrames(10);
    HammetPalace_AdvanceMessage(1);
    Import_ActorSetAnimation(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    v5 = 0;
  }
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  if (v5 != 0) {
    HammetPalace_AdvanceMessage(1);
  }
  ObjectMotion_SetVariantCallback(ACTOR_HAMMET, 2);
  Actor_SetAttachedEffect(ACTOR_HAMMET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(80);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMET, 0, 10);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_EAST + FACING_STEP, 10);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_IVAN, 3);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceDirection(ACTOR_LAYANA, FACING_SOUTHEAST + FACING_STEP, 60);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  ObjectMotion_SetVariantCallback(ACTOR_GARET, 2);
  Script_ShowMessageAndWait(ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 10);
  ObjectMotion_SetVariantCallback(ACTOR_IVAN, 2);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
  ObjectMotion_SetVariantCallback(ACTOR_IVAN, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 20);
  ObjectMotion_SetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
  Script_ShowMessageAndWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Script_ShowMessageAndWait(ACTOR_MIA, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_IVAN, 2);
  FieldScript_WaitFrames(80);
  ObjectMotion_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Script_OpenMessage(ACTOR_GARET, 0);
  if (FieldScript_AskYesNoWithActorAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    HammetPalace_AdvanceMessage(1);
  } else {
    FieldScript_WaitFrames(20);
    Script_ShowMessageAndWait(ACTOR_GARET, 0, 10);
  }
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  HammetPalace_FacePartyNorth(20);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_HAMMET, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMET, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_MIA, 2);
  Script_ShowMessageAndWait(ACTOR_MIA, 0, 10);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTHEAST + FACING_STEP, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMET, 0, 10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_HAMMET, 1);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_SOUTH + FACING_STEP, 10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMET, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_GARET, 2);
  Script_ShowMessageAndWait(ACTOR_GARET, 0, 10);
  ObjectMotion_SetAnimationAndWait(ACTOR_HAMMET, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMET, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_MIA, 1);
  Script_ShowMessageAndWait(ACTOR_MIA, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(ACTOR_LAYANA, 1);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(ACTOR_LAYANA, 0, 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  partyScript = gHammetPalace_PartyActions;
  Actor_SetActionScript(ACTOR_GARET, partyScript);
  Actor_SetActionScript(ACTOR_IVAN, partyScript);
  Actor_RunActionScriptAndWait(ACTOR_MIA, partyScript);
  work = gFieldWork;
  work->transitionFrames = 16;
  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  GameFlag_Set(FLAG_HAMMET_BACK_HOME_SEEN);
  FieldScript_End();
}

s32 gHammetPalace_PartyActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(856), 0, PIXELS(486)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gHammetPalaceEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct SceneEntrance gHammetPalaceEntrances[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {HAMMET_PALACE_FROM_HAMMET_PALACE_GROUNDS, CONDITION_ALWAYS, 240, 0, 648,
     FACING_NORTH, 0, 45, 360, 440, 680, 0},
    {HAMMET_PALACE_ENTRANCE_ROOM_FROM_WEST_WING, CONDITION_ALWAYS, 88, 0, 552,
     FACING_SOUTH, 0, 45, 360, 440, 680, 0},
    {HAMMET_PALACE_ENTRANCE_ROOM_FROM_EAST_WING, CONDITION_ALWAYS, 392, 0, 552,
     FACING_SOUTH, 0, 45, 360, 440, 680, 0},
    {HAMMET_PALACE_ENTRANCE_ROOM_FROM_LAYANAS_ROOM, CONDITION_ALWAYS, 240, 0,
     408, FACING_SOUTH, 0, 45, 360, 440, 680, 0},
    {HAMMET_PALACE_ENTRANCE_ROOM_FROM_LANDING, CONDITION_ALWAYS, 152, 0, 432,
     FACING_SOUTH, 0, 45, 360, 440, 680, 0},
    {HAMMET_PALACE_WEST_WING_FROM_ENTRANCE_ROOM, CONDITION_ALWAYS, 264, 0, 136,
     FACING_NORTH, 0, 40, 20, 305, 285, 0},
    {HAMMET_PALACE_EAST_WING_FROM_ENTRANCE_ROOM, CONDITION_ALWAYS, 504, 0, 152,
     FACING_NORTH, 0, 380, 20, 755, 290, 0},
    {HAMMET_PALACE_EAST_WING_FROM_PASSAGE_DOOR, CONDITION_ALWAYS, 448, 0, 112,
     FACING_SOUTH, 0, 380, 20, 755, 290, 0},
    {HAMMET_PALACE_LAYANAS_ROOM_FROM_ENTRANCE_ROOM, CONDITION_ALWAYS, 864, 0,
     576, FACING_NORTH, 0, 750, 370, 985, 635, 0},
    {HAMMET_PALACE_PASSAGE_DOOR_FROM_EAST_WING, CONDITION_ALWAYS, 928, 0, 224,
     FACING_WEST, 0, 800, 20, 1040, 250, 0},
    {HAMMET_PALACE_FROM_KALAY_TUNNEL_TREASURE_ROOM, CONDITION_ALWAYS, 904, 0,
     72, FACING_SOUTH, 0, 800, 20, 1040, 250, 0},
    {HAMMET_PALACE_BEDROOM_FROM_LANDING, CONDITION_ALWAYS, 616, 0, 824,
     FACING_NORTH, 0, 465, 640, 705, 875, 0},
    {HAMMET_PALACE_LANDING_FROM_ENTRANCE_ROOM, CONDITION_ALWAYS, 128, 0, 808,
     FACING_EAST, 0, 20, 730, 260, 890, 0},
    {HAMMET_PALACE_LANDING_FROM_BEDROOM, CONDITION_ALWAYS, 168, 0, 808,
     FACING_SOUTH, 0, 20, 730, 260, 890, 0},
    {HAMMET_PALACE_LAYANAS_ROOM_FROM_HAMMET_PALACE_GROUNDS, CONDITION_ALWAYS,
     864, 0, 576, FACING_NORTH, 0, 750, 370, 985, 635, 0},
    {MAP_TABLE_END},
};

u32 gHammetPalaceExits[] = {
    SCENE_EXITS(HammetPalace),
    SCENE_EXIT(HAMMET_PALACE_TO_HAMMET_PALACE_GROUNDS, HammetPalaceGrounds,
               HAMMET_PALACE_GROUNDS_FROM_HAMMET_PALACE),
    SCENE_EXIT(HAMMET_PALACE_ENTRANCE_ROOM_TO_WEST_WING, HammetPalace,
               HAMMET_PALACE_WEST_WING_FROM_ENTRANCE_ROOM),
    SCENE_EXIT(HAMMET_PALACE_ENTRANCE_ROOM_TO_EAST_WING, HammetPalace,
               HAMMET_PALACE_EAST_WING_FROM_ENTRANCE_ROOM),
    SCENE_EXIT(HAMMET_PALACE_ENTRANCE_ROOM_TO_LAYANAS_ROOM, HammetPalace,
               HAMMET_PALACE_LAYANAS_ROOM_FROM_ENTRANCE_ROOM),
    SCENE_EXIT(HAMMET_PALACE_ENTRANCE_ROOM_TO_LANDING, HammetPalace,
               HAMMET_PALACE_LANDING_FROM_ENTRANCE_ROOM),
    SCENE_EXIT(HAMMET_PALACE_WEST_WING_TO_ENTRANCE_ROOM, HammetPalace,
               HAMMET_PALACE_ENTRANCE_ROOM_FROM_WEST_WING),
    SCENE_EXIT(HAMMET_PALACE_EAST_WING_TO_ENTRANCE_ROOM, HammetPalace,
               HAMMET_PALACE_ENTRANCE_ROOM_FROM_EAST_WING),
    SCENE_EXIT(HAMMET_PALACE_EAST_WING_TO_PASSAGE_DOOR, HammetPalace,
               HAMMET_PALACE_PASSAGE_DOOR_FROM_EAST_WING),
    SCENE_EXIT(HAMMET_PALACE_LAYANAS_ROOM_TO_ENTRANCE_ROOM, HammetPalace,
               HAMMET_PALACE_ENTRANCE_ROOM_FROM_LAYANAS_ROOM),
    SCENE_EXIT(HAMMET_PALACE_PASSAGE_DOOR_TO_EAST_WING, HammetPalace,
               HAMMET_PALACE_EAST_WING_FROM_PASSAGE_DOOR),
    SCENE_EXIT(HAMMET_PALACE_TO_KALAY_TUNNEL_TREASURE_ROOM,
               KalayTunnelTreasureRoom,
               KALAY_TUNNEL_TREASURE_ROOM_FROM_HAMMET_PALACE),
    SCENE_EXIT(HAMMET_PALACE_BEDROOM_TO_LANDING, HammetPalace,
               HAMMET_PALACE_LANDING_FROM_BEDROOM),
    SCENE_EXIT(HAMMET_PALACE_LANDING_TO_ENTRANCE_ROOM, HammetPalace,
               HAMMET_PALACE_ENTRANCE_ROOM_FROM_LANDING),
    SCENE_EXIT(HAMMET_PALACE_LANDING_TO_BEDROOM, HammetPalace,
               HAMMET_PALACE_BEDROOM_FROM_LANDING),
    SCENE_EXITS_END,
};

struct ActorTemplate gHammetPalaceActorTemplatesOther[] = {
    {MAP_TABLE_END},
};

// clang-format off
struct ActorTemplate gHammetPalaceActorTemplates[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LAYANA, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(856), 0, PIXELS(440), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_HAMMET, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(872), 0, PIXELS(440), FACING_SOUTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(616), 0, PIXELS(113), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(712), 0, PIXELS(246), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_BLACK_SUIT, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(212), 0, PIXELS(522), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_MAN_IN_RED_KALAY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(169), 0, PIXELS(114), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN_KALAY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(114), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct Trigger gHammetPalaceTriggersOther[] = {
    { TRIGGERS_END },
};

struct Trigger gHammetPalaceTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, HAMMET_PALACE_TO_HAMMET_PALACE_GROUNDS },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveThroughDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveThroughDoor) },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, HAMMET_PALACE_ENTRANCE_ROOM_TO_LANDING },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, HAMMET_PALACE_WEST_WING_TO_ENTRANCE_ROOM },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, HAMMET_PALACE_EAST_WING_TO_ENTRANCE_ROOM },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, HAMMET_PALACE_EAST_WING_TO_PASSAGE_DOOR },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, HAMMET_PALACE_LAYANAS_ROOM_TO_ENTRANCE_ROOM },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, HAMMET_PALACE_PASSAGE_DOOR_TO_EAST_WING },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, HAMMET_PALACE_TO_KALAY_TUNNEL_TREASURE_ROOM },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, HAMMET_PALACE_BEDROOM_TO_LANDING },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, HAMMET_PALACE_LANDING_TO_ENTRANCE_ROOM },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveThroughDoor) },
    { TRIGGER_TALK, ACTOR_LAYANA, FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA, MSGNO(MsgHammetPalaceDodonpaIsAVeryShrewdMan) },
    { TRIGGER_TALK, ACTOR_LAYANA, FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS, EVENT_SCRIPT(EventScript_LayanaScoldsLunpaVisit) },
    { TRIGGER_TALK, ACTOR_LAYANA, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceDodonpaPlansToUseHammetTo) },
    { TRIGGER_TALK, ACTOR_LAYANA, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceNoOneInKalayKnowsHammet) },
    { TRIGGER_TALK, ACTOR_HAMMET, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceTakeYourShamansRodAndSet) },
    { TRIGGER_TALK, ACTOR_WORRIED_ABOUT_THE_TOWNSFOLK, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceTheTownsfolkDontKnowAboutLord) },
    { TRIGGER_TALK, ACTOR_WORRIED_ABOUT_THE_TOWNSFOLK, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceIHearHammetWasSavedBy) },
    { TRIGGER_TALK, ACTOR_ASKING_IF_HAMMET_WILL_BE_RELEASED, FLAG_DODONPA_LOCKED_UP, EVENT_SCRIPT(EventScript_AskWhetherHammetWillBeReleased) },
    { TRIGGER_TALK, ACTOR_ASKING_IF_HAMMET_WILL_BE_RELEASED, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceTheySayHammetsReturnIsSupposed) },
    { TRIGGER_TALK, ACTOR_LEFT_THE_PASSAGE_DOOR_OPEN, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceLayanaIsPuttingUpAStrong) },
    { TRIGGER_TALK, ACTOR_LEFT_THE_PASSAGE_DOOR_OPEN, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceILeftTheDoorToThe) },
    { TRIGGER_TALK, ACTOR_AFRAID_OF_DODONPAS_THUGS, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceMerchantsAreSupposedToBeProtected) },
    { TRIGGER_TALK, ACTOR_AFRAID_OF_DODONPAS_THUGS, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceIWonderIfAnyOfDodonpas) },
    { TRIGGER_TALK, ACTOR_ASKING_ABOUT_THE_SILK_TRADE, FLAG_DODONPA_LOCKED_UP, EVENT_SCRIPT(EventScript_AskWhetherSilkTradeHasEnded) },
    { TRIGGER_TALK, ACTOR_ASKING_ABOUT_THE_SILK_TRADE, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceTheSecretPassageIsLockedBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LAYANA, FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS, MSGNO(MsgHammetPalaceIsThisTheRightDecisionHammet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LAYANA, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceIDontCareAboutTheMoney) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LAYANA, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceImSoHappyHammetMadeIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMMET, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceLayanasWordsPainHerMoreThan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_ABOUT_THE_TOWNSFOLK, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceImSureTheVillagersWouldPanic) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_ABOUT_THE_TOWNSFOLK, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceImSoRelievedHammetMadeIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_IF_HAMMET_WILL_BE_RELEASED, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceEveryonesWorriedAboutLordHammetBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_IF_HAMMET_WILL_BE_RELEASED, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceICantLetOnToThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEFT_THE_PASSAGE_DOOR_OPEN, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceLadyLayanaWasCryingAgainLast) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEFT_THE_PASSAGE_DOOR_OPEN, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceLayanaOrderedTheDoorToThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFRAID_OF_DODONPAS_THUGS, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceAndToThinkIAlmostWent) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFRAID_OF_DODONPAS_THUGS, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceWeWillNeverLetDodonpasHenchmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_THE_SILK_TRADE, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceLordHammetIsALegendAmong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_THE_SILK_TRADE, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceTheKeyToTheSecretPassage) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_HAMMET_PALACE_WATER_JACKET_CHEST, FIND_ITEM(ITEM_WATER_JACKET) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 101, FLAG_HAMMET_PALACE_NUT_BARREL, FIND_ITEM(ITEM_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_WARDROBE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHammetPalaceTheresATurbanItsTheSymbol)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHammetPalaceTheresABookEntitledBusinessAnd)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHammetPalaceTheresABookEntitledTheGood)) },
    { TRIGGERS_END },
};
// clang-format on
