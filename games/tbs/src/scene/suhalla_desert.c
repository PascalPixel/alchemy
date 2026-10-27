/// @file
/// Suhalla Desert, the desert of sandstorms between Suhalla and Suhalla Gate,
/// which the party crosses with Iodem after Babi's audience while Sheba is
/// missing in it. Three maps share this overlay and the area name Suhalla
/// Desert:
///
/// - SuhallaDesertNorth (SceneId_SuhallaDesertNorth): entered from the world
/// map
///   near Suhalla, with five storms.
/// - SuhallaDesertSouth (SceneId_SuhallaDesertSouth): four storms and a fifth
///   that gives chase, Flash, a Mars Djinni, a stone pillar that Reveal shows,
///   and near its way out Iodem's question whether to search on for Sheba
///   (EventScript_IodemAsksWhetherToPressOn,
///   FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA).
/// - SuhallaDesertCaves (SceneId_SuhallaDesertCaves): two caves under the
/// desert,
///   reached from SuhallaDesertSouth; they place nothing.
///
/// One exit table and, for the first two maps, one trigger table serve the
/// overlay; the exits in that trigger table keep their numbers, which each
/// map's exit rows lead its own way.
///
/// The storms are the desert's mechanism, ACTOR_FIRST_SANDSTORM to
/// ACTOR_LAST_SANDSTORM in the first two maps. As the party nears one
/// (metatile behaviors 61 to 65 and 71 to 73) it rises, growing to full size,
/// and the sand haze, BG1 blended over the rest (SUHALLA_DESERT_HAZE_BLEND),
/// fades in the first time (SuhallaDesert_RaiseStorm); the first storm to rise
/// while Iodem travels with the party brings him out to warn of them. Walking
/// into one (metatile behaviors 41 to 45 and 51 to 54) draws the leader to its
/// centre and lifts the party, spinning, for SANDSTORM_CAPTURE_FRAMES
/// (SuhallaDesert_CatchInStorm). Douse in that time stops the storm and the
/// monster in it attacks (EventScript_DouseStorm); otherwise the storm carries
/// the party up and drops it on the world map outside Suhalla
/// (EventScript_StormCarriesPartyAway), which is why everyone the storms catch
/// winds up there. In SuhallaDesertSouth, metatile behavior 81 sets the fifth
/// storm chasing the leader (SuhallaDesert_ChaseLeader), and it catches a
/// leader on the move.
///
/// A doused storm's flag is an area flag, five from
/// FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED and five from
/// FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED, so the storm stays gone
/// until the party leaves the desert's area; the chasing storm, once
/// doused, is gone for good (FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_DOUSED). Its
/// map flags:
///
/// - FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN to
///   FLAG_SUHALLA_DESERT_FIFTH_SANDSTORM_RISEN: storms ACTOR_FIRST_SANDSTORM
///   to ACTOR_LAST_SANDSTORM have risen on this visit.
/// - FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_SET_OFF: the chasing storm has set
///   off.
/// - FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM: a storm has caught the leader.
/// - FLAG_SUHALLA_DESERT_CAPTURE_FRAMES_LEFT: a byte, the frames left before
///   the storm carries the party away.
/// - FLAG_SUHALLA_DESERT_CATCHING_SANDSTORM: a byte, the storm that caught the
///   leader.

#include "action_script.h"
#include "flags.h"
#include "version.h"
#include "save_block.h"
#include "engine_call.h"
#include "unit/scene/suhalla_desert.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/game/game.h"
#include "actor.h"
#include "types.h"
#include "script_layer.h"
#include "scene_tables.h"
#include "djinn_ids.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fx_work.h"
#include "heap_slots.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "call.h"
#include "field_effect.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "objcmd.h"
#include "iowrite.h"
#include "songs.h"
#include "heap.h"
#include "map_grid.h"
#include "fixed_math.h"

/// Storms doused in the first and second maps, one flag per storm from the
/// first; each storm stays away once doused.
enum {
  /// The first map's storms, five from this one.
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED = FLAG_AREA_TEMP_01,
  /// The second map's storms, five from this one.
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED = FLAG_AREA_TEMP_11,
  /// A storm has risen: the sand haze stays faded in.
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  FLAG_SUHALLA_DESERT_SANDSTORMS_RISEN = FLAG_AREA_TEMP_40,
  /// The Psynergy Stone has been found, an area flag.
  /// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
  FLAG_SUHALLA_DESERT_PSYNERGY_STONE = FLAG_AREA_TEMP_51
};

/// The actors, numbering the actor template rows of the first two
/// maps from ACTOR_FIRST_PLACED; the third map places none.
enum {
  /// The storms, five in each map.
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  ACTOR_FIRST_SANDSTORM = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  ACTOR_FOURTH_SANDSTORM = ACTOR_FIRST_SANDSTORM + 3,
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  ACTOR_LAST_SANDSTORM = ACTOR_FIRST_SANDSTORM + 4,
  /// In the second map the last storm gives chase.
  /// ★ Named from the game: "A sandstorm!" (MsgSuhallaSandstorm)
  ACTOR_CHASING_SANDSTORM = ACTOR_LAST_SANDSTORM,
  /// Iodem, who steps out of the party to talk and then rejoins it.
  /// ★ Named from the game: "Iodem..." (MsgSuhallaIodem)
  ACTOR_IODEM,
  /// In the second map, seen only while Reveal lasts, drawn as Goma Cave's
  /// stone pillars.
  /// ★ Named from what the player sees: its sprite, SPRITE_STONE_PILLAR, a
  /// stone pillar (its render)
  ACTOR_REVEALED_PILLAR,
  /// In the second map, the Mars Djinni.
  /// ★ Named from the game: "Flash" (MsgDjinnName)
  ACTOR_FLASH
};

/// Frames a caught party spins before the storm carries it away.
enum { SANDSTORM_CAPTURE_FRAMES = 180 };

/// The treasure spot of SuhallaDesertSouth's hidden Psynergy Stone, whose
/// search row finds the stone (FIND_PSYNERGY_STONE). Its actor stands in
/// its cell only while Reveal lasts (EventScript_RevealStarts moves it to the
/// cell's centre) and is moved off the map as Reveal ends.
enum { TREASURE_SPOT_PSYNERGY_STONE = 100 };

/// The events the storms raise: the spinning has run out, or the chasing storm
/// has reached the moving leader.
enum {
  RAISED_CHASING_SANDSTORM_CATCHES = 55,
  RAISED_SANDSTORM_CARRIES_PARTY_AWAY = 99
};

/// The sand haze: BG1 blended over every layer, its weight rising from 0 to
/// 16 as the first storm rises.
#define SUHALLA_DESERT_HAZE_BLEND                                              \
  (BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

static void SuhallaDesert_SetUpFirstMapStorms(void);
static void SuhallaDesert_SetUpSecondMapStorms(void);

static u16 sSuhallaDesertSavedColor;

#define SUHALLA_REVEAL_PALETTE_COLOR (*(volatile u16 *)0x0500019e)

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// The first sandstorm (ACTOR_FIRST_SANDSTORM) has risen on this visit; the
/// four flags after it are the next four sandstorms'.
/// ★ Named from the game: "The sandstorms suddenly increased... I hope no one
/// gets caught in one and gets injured."
/// (MsgSuhallaTheSandstormsSuddenlyIncreasedIHope)
#define FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN FLAG_SCENE_TEMP_06
/// The second sandstorm has risen on this visit.
/// ★ Named from the game: "The sandstorms suddenly increased... I hope no one
/// gets caught in one and gets injured."
/// (MsgSuhallaTheSandstormsSuddenlyIncreasedIHope)
#define FLAG_SUHALLA_DESERT_SECOND_SANDSTORM_RISEN FLAG_SCENE_TEMP_07
/// The third sandstorm has risen on this visit.
/// ★ Named from the game: "The sandstorms suddenly increased... I hope no one
/// gets caught in one and gets injured."
/// (MsgSuhallaTheSandstormsSuddenlyIncreasedIHope)
#define FLAG_SUHALLA_DESERT_THIRD_SANDSTORM_RISEN FLAG_SCENE_TEMP_08
/// The fourth sandstorm (ACTOR_FOURTH_SANDSTORM) has risen on this visit.
/// ★ Named from the game: "The sandstorms suddenly increased... I hope no one
/// gets caught in one and gets injured."
/// (MsgSuhallaTheSandstormsSuddenlyIncreasedIHope)
#define FLAG_SUHALLA_DESERT_FOURTH_SANDSTORM_RISEN FLAG_SCENE_TEMP_09
/// The fifth sandstorm (ACTOR_LAST_SANDSTORM) has risen on this visit.
/// ★ Named from the game: "The sandstorms suddenly increased... I hope no one
/// gets caught in one and gets injured."
/// (MsgSuhallaTheSandstormsSuddenlyIncreasedIHope)
#define FLAG_SUHALLA_DESERT_FIFTH_SANDSTORM_RISEN FLAG_SCENE_TEMP_0A
/// In SuhallaDesertSouth, the sandstorm that gives chase has set off after the
/// leader.
/// ★ Named from the game: "The sandstorms suddenly increased... I hope no one
/// gets caught in one and gets injured."
/// (MsgSuhallaTheSandstormsSuddenlyIncreasedIHope)
#define FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_SET_OFF FLAG_SCENE_TEMP_0E
/// A sandstorm has caught the leader and lifts the party, and Douse can put it
/// out.
/// ★ Named from the game: "For some reason, anyone who gets caught in a
/// sandstorm winds up here." (MsgSuhallaInteriorsForSomeReasonAnyoneWhoGets)
#define FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM FLAG_SCENE_TEMP_0F
/// A byte flag: the frames left before the sandstorm that caught the leader
/// carries the party away.
/// ★ Named from the game: "For some reason, anyone who gets caught in a
/// sandstorm winds up here." (MsgSuhallaInteriorsForSomeReasonAnyoneWhoGets)
#define FLAG_SUHALLA_DESERT_CAPTURE_FRAMES_LEFT FLAG_SCENE_TEMP_10
/// A byte flag: the actor id of the sandstorm that caught the leader.
/// ★ Named from the game: "For some reason, anyone who gets caught in a
/// sandstorm winds up here." (MsgSuhallaInteriorsForSomeReasonAnyoneWhoGets)
#define FLAG_SUHALLA_DESERT_CATCHING_SANDSTORM FLAG_SCENE_TEMP_18

void Scene_QueueMusic(s32 cue);

/// In SuhallaDesertSouth, talking to Flash, a Mars Djinni, starts its battle.
static void EventScript_FightFlash(void) {
  Field_RequestDjinnBattle(ACTOR_FLASH, DJINNI_FLASH);
}

/// A storm's actor template script: clears its sprite and collision
/// flags.
static s32
SuhallaDesert_ClearActorSpriteAndCollisionFlags(struct Object *interpreter) {
  struct Actor *actor = (struct Actor *)interpreter;
  Actor_SetSpriteFlags(actor, 0);
  actor->collisionFlags = 0;
  return 0;
}

/// A storm's actor template script: toggles its single-sprite storage.
static s32 SuhallaDesert_ToggleActorActive(struct Object *interpreter) {
  struct Actor *actor = (struct Actor *)interpreter;
  actor->spriteStorageKind ^= 1;
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 selector = gSaveBlock.sceneId;

  if (selector == (s32)&SceneId_SuhallaDesertNorth) {
    return gSuhallaDesertNorthEntrances;
  }
  if (selector == (s32)&SceneId_SuhallaDesertSouth) {
    return gSuhallaDesertSouthEntrances;
  }
  if (selector == (s32)&SceneId_SuhallaDesertCaves) {
    return gSuhallaDesertCavesEntrances;
  }
  return gSuhallaDesertEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

/// Where each exit of the three maps leads.
const u32 *Scene_GetExits(void) { return gSuhallaDesertExits; }

/// The storms, Iodem, and in the second map Flash and what Reveal shows.
const struct ActorTemplate *Scene_GetActorTemplates(void) {
  s16 selector = gSaveBlock.sceneId;

  if (selector == (s32)&SceneId_SuhallaDesertNorth) {
    return gSuhallaDesertNorthActorTemplates;
  }
  if (selector == (s32)&SceneId_SuhallaDesertSouth) {
    return gSuhallaDesertSouthActorTemplates;
  }
  if (selector == (s32)&SceneId_SuhallaDesertCaves) {
    return gSuhallaDesertCavesActorTemplates;
  }
  return gSuhallaDesertActorTemplatesOther;
}

/// Each frame while caught: spins the leader, and counts down to raising event
/// 99, the storm carrying the party away.
static void SuhallaDesert_UpdateCaptureTimer(void) {
  struct Actor *selectedActor;
  struct FieldWork *requests;
  s32 remainingFrames;

  selectedActor = Actor_GetById(gSaveBlock.leaderActorId);
  requests = gFieldWork;
  selectedActor->facing = (gFrameCount << 12);

  remainingFrames =
      FieldGameFlag_GetByte(FLAG_SUHALLA_DESERT_CAPTURE_FRAMES_LEFT);
  if (remainingFrames != 0) {
    if (remainingFrames == 1) {
      requests->raisedEvent = RAISED_SANDSTORM_CARRIES_PARTY_AWAY;
    } else if (GameFlag_Get(FLAG_MENU_OPEN) == 0) {
      remainingFrames -= 1;
    }
  }
  FieldGameFlag_SetByte(FLAG_SUHALLA_DESERT_CAPTURE_FRAMES_LEFT,
                        remainingFrames);
}

/// The leader walks into a storm: drawn to its centre and lifted, the party
/// spins until Douse stops the storm or it carries them away.
static void SuhallaDesert_CatchInStorm(s32 stormId) {
  s32 stormIdCopy;
  s32 captureActive;
  struct Actor *selectedActor;
  struct Actor *storm;
  s32 leaderActorId;

  // The event-selected actor is pulled toward the storm before being
  // lifted.
  leaderActorId = gSaveBlock.leaderActorId;
  stormIdCopy = stormId;
  selectedActor = Actor_GetById(leaderActorId);
  Actor_Get(stormIdCopy);
  captureActive = Script_GetFlag(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM);
  if (captureActive == 0) {
    FieldScript_Start();
    Actor_SetAttachedEffect(leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    Import_ActorSetAnimation(leaderActorId, ANIM_LEADER_LOOK_AROUND);
    storm = Actor_GetById(stormIdCopy);
    if (storm != 0) {
      Actor_SetDestination(leaderActorId, storm->x.part.pixel,
                           storm->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(leaderActorId);
    // Start the capture countdown once the approach finishes.
    Script_PlaySong(SE_SANDSTORM);
    FieldScheduler_AddCallback(SuhallaDesert_UpdateCaptureTimer,
                               TASK_PASS_FRAME);
    selectedActor->motionFlags = captureActive;
    Object_SetPosition(selectedActor, selectedActor->x.fixed,
                       selectedActor->y.fixed + PIXELS(32),
                       selectedActor->z.fixed);
    ObjectMotion_CommitCurrentPositionAndActivate(leaderActorId);
    selectedActor->velocityY = captureActive;
    selectedActor->motionFlags = ACTOR_FLOATS;
    gSaveBlock.movementMode = LEADER_MOVEMENT_AIRBORNE;
    GameFlag_Set(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM);
    FieldGameFlag_SetByte(FLAG_SUHALLA_DESERT_CATCHING_SANDSTORM, stormIdCopy);
    FieldGameFlag_SetByte(FLAG_SUHALLA_DESERT_CAPTURE_FRAMES_LEFT,
                          SANDSTORM_CAPTURE_FRAMES);
    FieldScript_End();
    gFieldWork->battle = 0; // drop any battle this step rolled
  }
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the leader walks into the map's first storm
/// (metatile behavior 41 or 51).
static void EventScript_CaughtByFirstStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_SANDSTORM);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the leader walks into the second storm
/// (metatile behavior 42 or 52).
static void EventScript_CaughtBySecondStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_SANDSTORM + 1);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the leader walks into the third storm (metatile
/// behavior 43 or 53).
static void EventScript_CaughtByThirdStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_SANDSTORM + 2);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the leader walks into the fourth storm
/// (metatile behavior 44 or 54).
static void EventScript_CaughtByFourthStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_SANDSTORM + 3);
}

/// The fifth storm catches the leader. In SuhallaDesertNorth the leader walks
/// into it (metatile behavior 45); in SuhallaDesertSouth it's the chasing
/// storm, which reaches the moving leader (event 55).
static void EventScript_CaughtByFifthStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_SANDSTORM + 4);
}

/// As the party nears a storm it rises, growing to full size with the sand
/// haze fading in the first time. The first time a storm rises with Iodem in
/// the party, he steps out and warns of the sandstorms: "If we don't get past
/// here, we'll never make it to Babi Lighthouse."
static void SuhallaDesert_RaiseStorm(s32 stormId) {
  s32 frame;
  s32 nextFrame;
  s32 stormIdCopy;
  s32 leaderX;
  s32 leaderZ;
  struct Actor *storm;
  s32 appearanceRestored;
  struct Actor *selectedActor;
  struct Actor *leader;

  stormIdCopy = stormId;
  selectedActor = Actor_GetById(gSaveBlock.leaderActorId);
  storm = Actor_GetById(stormIdCopy);
  appearanceRestored = GameFlag_Get(FLAG_SUHALLA_DESERT_SANDSTORMS_RISEN);
  leaderX = selectedActor->x.part.pixel;
  leaderZ = selectedActor->z.part.pixel;
  FieldScript_Start();
  Sound_PlaySong(SE_SANDSTORM);
  // Grow over 16 frames and, the first time, fade the haze in with it.
  frame = 0;
  do {
    storm->scaleX = ((frame << 11) + 0x800);
    storm->scaleY = ((frame << 12) + 0x1000);
    if (appearanceRestored == 0) {
      nextFrame = frame + 1;
      IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                          BLEND_ALPHA(nextFrame, 16 - nextFrame), IO_WRITE_16);
    } else {
      nextFrame = frame + 1;
    }
    WaitFrames(1);
    frame = nextFrame;
  } while (frame <= 15);
  GameFlag_Set((FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN -
                ACTOR_FIRST_SANDSTORM + stormIdCopy));
  GameFlag_Set(FLAG_SUHALLA_DESERT_SANDSTORMS_RISEN);
  // XXX (no evidence): what flag 0x1b7 holds; nothing else in the game sets
  // or reads it, so the test always passes.
  if (GameFlag_Get(FLAG_IODEM_FOLLOWING_PARTY) != 0 &&
      Script_GetFlag(FLAG_SUHALLA_DESERT_IODEM_WARNED) == 0) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_IODEM_WARNED);
    Actor_SetSpeed(ACTOR_IODEM, PIXELS(1), PIXELS(1) / 2);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if ((s32)leader != 0) {
      Import_ActorSetPosition(ACTOR_IODEM, leader->x.fixed, leader->z.fixed);
    }
#ifdef BUGFIX
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
#else
    // BUG: a heading where Actor_FaceActor takes an actor id.
    Actor_FaceActor(ACTOR_IODEM, FACING_SOUTH, 0);
#endif
    Actor_GetById(ACTOR_IODEM)->steeringFlags &= ~ACTOR_FACES_MOTION;
    ObjectMotion_ResetAndSetPositionInMode2(ACTOR_IODEM, leaderX,
                                            (leaderZ - 16));
    Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    ObjectMotion_SetPositionAndReset(ACTOR_PARTY_LEADER, (leaderX + 8),
                                     (leaderZ - 40));
    FieldScript_WaitFrames(1);
    Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    Import_ActorSetAnimation(ACTOR_IODEM, ANIM_STAND);
    ObjectMotion_ArmCallback(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    FieldScript_SetMessageId(MsgSuhallaSandstorm);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
    FieldScript_ShowMessageAndWait(ACTOR_IODEM, 0);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    FieldScript_ShowMessageAndWait(ACTOR_IODEM, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    FieldScript_ShowMessageAndWait(ACTOR_IODEM, 0);
    FieldScript_WaitFrames(10);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 30);
    ObjectMotion_SetVarCbAndRefresh(ACTOR_IODEM, 2);
    FieldScript_ShowMessageAndWait(ACTOR_IODEM, 0);
    ObjectMotion_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    FieldScript_ShowMessageAndWait(ACTOR_IODEM, 0);
    Actor_SetSpeed(ACTOR_IODEM, PIXELS(1), PIXELS(1) / 2);
    Import_ActorSetAnimation(ACTOR_IODEM, ANIM_WALK);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if ((s32)leader != 0) {
      ObjectMotion_ResetAndSetPosition(ACTOR_IODEM, leader->x.part.pixel,
                                       leader->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IODEM);
    Import_ActorSetPosition(ACTOR_IODEM, 0, 0);
  }
  FieldScript_End();
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the party nears the map's first storm (metatile
/// behavior 61 or 71).
static void EventScript_FirstStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_SANDSTORM);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the party nears the second storm (metatile
/// behavior 62 or 72).
static void EventScript_SecondStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_SANDSTORM + 1);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the party nears the third storm (metatile
/// behavior 63 or 73).
static void EventScript_ThirdStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_SANDSTORM + 2);
}

/// In SuhallaDesertNorth, the party nears the fourth storm (metatile behavior
/// 64).
static void EventScript_FourthStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_SANDSTORM + 3);
}

/// In SuhallaDesertNorth, the party nears the fifth storm (metatile behavior
/// 65).
static void EventScript_FifthStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_SANDSTORM + 4);
}

/// Iodem asks whether to search the desert for Sheba again or press on. It
/// plays in SuhallaDesertSouth, near its way out to the world map (metatile
/// behavior 9), once Iodem has heard the wounded soldiers in Suhalla: he steps
/// out of the party. "Sheba is nowhere to be found. Should we search the desert
/// once more or press on to Lalivero?" Whatever the answer, Iodem decides to
/// head first for the guard gate ahead, Suhalla Gate, and leave Sheba to the
/// soldiers there, and rejoins the party. Sets
/// FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA.
static void EventScript_IodemAsksWhetherToPressOn(void) {
  struct Actor *leader;

  if (GameFlag_Get(FLAG_IODEM_FOLLOWING_PARTY) != 0 &&
      GameFlag_Get(0x1b7) == 0 &&
      GameFlag_Get(FLAG_IODEM_MET_WOUNDED_SOLDIERS) != 0) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA);
    FieldScript_Start();
    FieldScript_SetMessageId(MsgSuhallaNowhereFound);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetPosition(ACTOR_IODEM, leader->x.fixed, leader->z.fixed);
    }
#ifdef BUGFIX
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
#else
    // BUG: a heading where Actor_FaceActor takes an actor id.
    Actor_FaceActor(ACTOR_IODEM, FACING_NORTH, 0);
#endif
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 440, 1256);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 444, 1240);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 30);
    ObjectMotion_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    Script_ShowMessage(ACTOR_IODEM, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Script_ShowMessage(ACTOR_IODEM, 0);
    FieldScript_WaitFrames(30);
    ObjectMotion_SetVarCbAndRefresh(ACTOR_IODEM, 2);
    Script_ShowMessage(ACTOR_IODEM, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 30);
    Script_AskYesNo(ACTOR_IODEM, 0);
    FieldScript_WaitFrames(30);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
    Script_ShowMessage(ACTOR_IODEM, 0);
    ObjectMotion_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Script_ShowMessage(ACTOR_IODEM, 0);
    Actor_SetSpeed(ACTOR_IODEM, PIXELS(7) / 10, PIXELS(7) / 20);
    Actor_WalkToAndWait(ACTOR_IODEM, 440, 1256);
    Script_ShowMessage(ACTOR_IODEM, 0);
    ObjectMotion_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_IODEM, ANIM_WALK);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetDestination(ACTOR_IODEM, leader->x.part.pixel,
                           leader->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_IODEM);
    Actor_SetPosition(ACTOR_IODEM, 0, 0);
    FieldScript_End();
  }
}

/// The chasing storm's script: it follows the leader, throwing up sand every
/// eight frames, and catches a moving leader it comes within 8 pixels of
/// (event 55).
static s32 SuhallaDesert_ChaseLeader(struct Object *interpreter) {
  struct Actor *actor = (struct Actor *)interpreter;
  struct EffectOptions options;
  struct Actor *selectedActor;
  s32 dx;
  s32 dz;
  s32 sparkFrame;

  selectedActor = Actor_GetById(gSaveBlock.leaderActorId);
  actor->acceleration = PIXELS(1) / 4;
  actor->speed = PIXELS(3) / 2;
  actor->motionFlags = 0;
  Actor_SetSpriteFlags(actor, 0);
  actor->spriteStorageKind ^= 1;
  if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS)) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
  } else {
    actor->targetX = selectedActor->x.fixed;
    actor->targetY = selectedActor->groundHeight;
    actor->targetZ = selectedActor->z.fixed;
    dx = actor->x.fixed - selectedActor->x.fixed;
    if (dx < 0) {
      dx = selectedActor->x.fixed - actor->x.fixed;
    }
    dz = actor->z.fixed - selectedActor->z.fixed;
    if (dx + (dz >= 0 ? actor->z.fixed - selectedActor->z.fixed
                      : selectedActor->z.fixed - actor->z.fixed) <
        PIXELS(8)) {
      struct FieldWork *work = gFieldWork;

      if (selectedActor->motionFlags != 0) {
        work->raisedEvent = RAISED_CHASING_SANDSTORM_CATCHES;
      }
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      actor->targetX = selectedActor->x.fixed;
      actor->targetY = selectedActor->y.fixed;
      actor->targetZ = selectedActor->z.fixed;
    }
  }
  // Sand every eight frames, with a random spin.
  sparkFrame = gFrameCount & 7;
  if (sparkFrame == 0) {
    options.startScaleX = 4 * Q16_ONE / 5;
    options.startScaleY = 4 * Q16_ONE / 5;
    options.spin = ((u32)(Import_RandomNext() << 12) >> 16) + 0xf800;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0,
                      sparkFrame, sparkFrame,
                      EFFECT_USE_SPIN | EFFECT_USE_START_SCALE |
                          EFFECT_SCRIPT_MEDIUM,
                      &options);
  }
  return 1;
}

/// In SuhallaDesertSouth, until the chasing storm has been doused: it
/// appears behind the party and gives chase.
static void EventScript_StormGivesChase(void) {
  if (GameFlag_Get(FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_DOUSED) == 0) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_SET_OFF);
    Actor_SetPosition(ACTOR_CHASING_SANDSTORM, PIXELS(120), PIXELS(824));
    Actor_SetActionScript(ACTOR_CHASING_SANDSTORM, gSuhallaDesertChaseScript);
  }
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), Douse cast at the storm that caught the leader:
/// sets the Psynergy effect's child option, so that Douse's orb rises to 80
/// pixels above the storm rather than 32 and rains on it from above
/// (FieldEffect_RunDouse).
static void EventScript_DouseCastAtStorm(void) {
  u8 *state = gFieldEffectWork;
  ((struct FieldEffectWork *)state)->childOption = 1;
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), Douse stops the storm that caught the leader:
/// it is doused for good, and the monster in it attacks.
/// SceneId_SuhallaDesertSouth's fourth and chasing storms hold other monsters.
static void EventScript_DouseStorm(s32 unused, s32 stormId) {
  struct SaveBlock *state;

  // The storm no longer carries the party away.
  if (gFieldWork->raisedEvent == RAISED_SANDSTORM_CARRIES_PARTY_AWAY) {
    gFieldWork->raisedEvent = 0;
  }
  GameFlag_Clear(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM);
  if (gSaveBlock.sceneId == (s32)&SceneId_SuhallaDesertNorth) {
    GameFlag_Set((stormId + FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED -
                  ACTOR_FIRST_SANDSTORM));
  } else {
    if (gSaveBlock.sceneId == (s32)&SceneId_SuhallaDesertSouth) {
      GameFlag_Set((stormId + FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED -
                    ACTOR_FIRST_SANDSTORM));
    }
  }
  FieldGameFlag_SetByte(FLAG_SUHALLA_DESERT_CAPTURE_FRAMES_LEFT, 0);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 5);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  state = &gSaveBlock;
  if (state->sceneId == (s32)&SceneId_SuhallaDesertSouth) {
    if (stormId == ACTOR_FOURTH_SANDSTORM) {
      Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 7);
    } else {
      if (stormId == ACTOR_CHASING_SANDSTORM) {
        Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 6);
        ObjectMotion_EnableActionAndResetMotion(ACTOR_CHASING_SANDSTORM);
        Import_ActorSetPosition(ACTOR_CHASING_SANDSTORM, 0, 0);
      }
    }
  }
  Actor_GetById(gSaveBlock.leaderActorId)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the spinning runs out (event 99): the storm
/// rises into the sky with the party, the screen closes, and the party lands
/// falling (FLAG_ARRIVE_FALLING) on the world map outside Suhalla, or where the
/// second map's fourth storm drops it.
static void EventScript_StormCarriesPartyAway(void) {
  s32 stormId;
  struct Actor *selectedActor;
  struct Actor *storm;
  s32 frame;

  stormId = FieldGameFlag_GetByte(FLAG_SUHALLA_DESERT_CATCHING_SANDSTORM);
  selectedActor = Actor_GetById(gSaveBlock.leaderActorId);
  storm = Actor_GetById(stormId);
  FieldScript_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Sound_PlaySong(SE_GUSH);
  // BUG: the native passes an actor id where the sprite helper needs a pointer.
#ifdef BUGFIX
  Object_SetSpriteFlags(selectedActor, 0);
#else
  Actor_SetSpriteFlags((struct Actor *)gSaveBlock.leaderActorId, 0);
#endif
  storm->motionFlags = 0;
  selectedActor->motionFlags = 0;
  selectedActor->velocityY = 0;
  selectedActor->frozen = 1;
  storm->frozen = 1;
  // Both rise faster and faster for a second.
  for (frame = 0; frame < 60; frame++) {
    selectedActor->velocityY += PIXELS(1) / 5;
    storm->velocityY += PIXELS(1) / 5;
    WaitFrames(1);
  }
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_End();
  GameFlag_Set(FLAG_ARRIVE_FALLING);
  if (gSaveBlock.sceneId == (s32)&SceneId_SuhallaDesertSouth &&
      FieldGameFlag_GetByte(FLAG_SUHALLA_DESERT_CATCHING_SANDSTORM) ==
          ACTOR_FOURTH_SANDSTORM) {
    FieldScript_SetDestination((s32)&SceneId_WorldMap,
                               WORLD_MAP_FROM_SUHALLA_DESERT_SANDSTORM);
  } else {
    FieldScript_SetDestination((s32)&SceneId_WorldMap, WORLD_MAP_FROM_SUHALLA);
  }
}

/// While Reveal lasts: one palette color pulses, brightening and dimming over
/// 64 frames.
static void SuhallaDesert_PulseRevealPalette(void) {
  u16 brightnessFrame = gFrameCount & 63;
  s32 color;

  if (brightnessFrame > 31)
    brightnessFrame = 64 - brightnessFrame;
  color = (brightnessFrame >> 1) + 7;
  color |= (color << 10) | (color << 5);
  SUHALLA_REVEAL_PALETTE_COLOR = ((u32)color << 16) >> 16;
}

// Native event return forms: see MATCHING.md.
#ifdef UBFIX
/// Reveal wears off, and what it showed is hidden again. It runs as Reveal
/// ends and whenever a map is entered, and only in SuhallaDesertSouth does it
/// hide what Reveal showed and put back the pulsing color.
static void EventScript_RevealEnds(void) {
#else
/// Reveal wears off, and what it showed is hidden again. It runs as Reveal
/// ends and whenever a map is entered, and only in SuhallaDesertSouth does it
/// hide what Reveal showed and put back the pulsing color.
static s32 EventScript_RevealEnds(void) {
  s32 removalResult;
#endif
  if (gSaveBlock.sceneId == (s32)&SceneId_SuhallaDesertSouth) {
    Actor_GetById(ACTOR_REVEALED_PILLAR)->priorityFlags =
        ACTOR_PRIORITY_UNDERFOOT;
    Actor_GetById(ACTOR_REVEALED_PILLAR)->motionFlags =
        ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    Actor_SetPosition(ACTOR_REVEALED_PILLAR, 0, 0);
    MapGrid_CopyCellAttributes(16, 44, 1, 1, 15, 44);
    TreasureSpot_SetPosition(TREASURE_SPOT_PSYNERGY_STONE, 0, 0);
    MapGrid_CopyCellAttributes(12, 71, 1, 1, 127, 127);
    MapGrid_CopyCellAttributes(11, 71, 1, 1, 12, 71);
#ifdef UBFIX
    Task_Remove(SuhallaDesert_PulseRevealPalette);
#else
    removalResult = Task_Remove(SuhallaDesert_PulseRevealPalette);
#endif
    Io_Write16(sSuhallaDesertSavedColor, &SUHALLA_REVEAL_PALETTE_COLOR);
#ifndef UBFIX
    return removalResult;
#endif
  }
}

#ifdef UBFIX
/// Reveal shows a hidden stone pillar in the desert's south. In
/// SuhallaDesertSouth the stone pillar appears at 248, 712, map cells and the
/// Psynergy Stone's map object change, and a palette color pulses.
static void EventScript_RevealStarts(void) {
#else
/// Reveal shows a hidden stone pillar in the desert's south. In
/// SuhallaDesertSouth the stone pillar appears at 248, 712, map cells and the
/// Psynergy Stone's map object change, and a palette color pulses.
static s32 EventScript_RevealStarts(void) {
#endif
  if (gSaveBlock.sceneId == (s32)&SceneId_SuhallaDesertSouth) {
    Actor_GetById(ACTOR_REVEALED_PILLAR)->priorityFlags =
        ACTOR_PRIORITY_UNDERFOOT;
    Actor_GetById(ACTOR_REVEALED_PILLAR)->motionFlags = 0;
    Actor_SetPosition(ACTOR_REVEALED_PILLAR, PIXELS(248), PIXELS(712));
    MapGrid_CopyCellAttributes(31, 95, 1, 1, 15, 44);
    TreasureSpot_SetPosition(TREASURE_SPOT_PSYNERGY_STONE, -1, -1);
    TreasureSpot_MoveLeaderAway();
    MapGrid_CopyCellAttributes(127, 127, 1, 1, 12, 71);
#ifdef UBFIX
    FieldScheduler_AddCallback(SuhallaDesert_PulseRevealPalette,
                               TASK_PASS_FRAME);
#else
    return FieldScheduler_AddCallback(SuhallaDesert_PulseRevealPalette,
                                      TASK_PASS_FRAME);
#endif
  }
}

/// The third map's exits, or the storms, Iodem, treasure and Reveal of the
/// first two.
const struct Trigger *Scene_GetTriggers(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_SuhallaDesertCaves) {
    return gSuhallaDesertCavesTriggers;
  }
  return gSuhallaDesertTriggersOther;
}

/// Sets a map up as the party enters: a capture goes on spinning, Reveal's
/// color is saved, and the first two maps set up their storms.
s32 Scene_Initialize(void) {
  struct MapGridScrollAnimView *gridState;

  gridState = HEAP_SLOT_BLOCK(HEAP_SLOT_MAP_GRID);
  ((struct FieldWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FIELD))->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  if (FieldGameFlag_GetByte(FLAG_SUHALLA_DESERT_CAPTURE_FRAMES_LEFT) != 0) {
    gSaveBlock.movementMode = LEADER_MOVEMENT_AIRBORNE;
    FieldScheduler_AddCallback(SuhallaDesert_UpdateCaptureTimer,
                               TASK_PASS_FRAME);
  }
  if (gSaveBlock.sceneId == (s32)&SceneId_SuhallaDesertNorth ||
      gSaveBlock.sceneId == (s32)&SceneId_SuhallaDesertSouth) {
    sSuhallaDesertSavedColor = SUHALLA_REVEAL_PALETTE_COLOR;
    EventScript_RevealEnds();
  }
  if (gSaveBlock.sceneId == (s32)&SceneId_SuhallaDesertNorth) {
    SuhallaDesert_SetUpFirstMapStorms();
  } else if (gSaveBlock.sceneId == (s32)&SceneId_SuhallaDesertSouth) {
    SuhallaDesert_SetUpSecondMapStorms();
  } else {
    Sound_PlaySong(SE_STOP_ACTION_SE);
  }
  if (gSaveBlock.entrance == ENTRANCE_DEFAULT) {
    gridState->flags &= ~DISPCNT_BG1_ON;
  }
  return 0;
}

/// The first map's storms on entry: a doused storm counts as risen, the
/// others start small (unless the party is coming back to where it stood), and
/// the sand haze is faded in if a storm has risen before.
static void SuhallaDesert_SetUpFirstMapStorms(void) {
  // Trial: sharing the blend tail through an ordinary inline shrinks this
  // function from 0x144 to 0x138 bytes in tbs-en; retain the original body.
  struct Actor *actor;
  s32 stormId;
  s32 stormAlpha = 0;

  if (GameFlag_Get(FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED)) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN);
  }
  if (GameFlag_Get(FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 1)) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_SECOND_SANDSTORM_RISEN);
  }
  if (GameFlag_Get(FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 2)) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_THIRD_SANDSTORM_RISEN);
  }
  if (GameFlag_Get(FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 3)) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_FOURTH_SANDSTORM_RISEN);
  }
  if (GameFlag_Get(FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 4)) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_FIFTH_SANDSTORM_RISEN);
  }
  for (stormId = ACTOR_FIRST_SANDSTORM; stormId <= ACTOR_LAST_SANDSTORM;
       stormId++) {
    actor = Actor_GetById(stormId);
    if (actor != 0) {
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        actor->scaleX = Q16_ONE / 32;
        actor->scaleY = Q16_ONE / 32;
      }
      actor->sprite->flags = 0;
    }
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, SUHALLA_DESERT_HAZE_BLEND, IO_WRITE_16);
  if (GameFlag_Get(FLAG_SUHALLA_DESERT_SANDSTORMS_RISEN)) {
    stormAlpha = 16;
    Scene_QueueMusic(SE_SANDSTORM);
  }
  // BLEND_ALPHA(stormAlpha, 16 - stormAlpha), whose operand order compiles
  // differently.
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - stormAlpha) << 8) | stormAlpha,
                      IO_WRITE_16);
}

/// The second map's storms on entry, as the first map's; the fourth storm's
/// first sprite part takes parameter 10, and once the chasing storm has been
/// doused it is gone for good.
static void SuhallaDesert_SetUpSecondMapStorms(void) {
  // Trial: the same blend-tail inline shrinks this function from 0x158 to 0x14c
  // bytes in tbs-en; retain the original body.
  struct Actor *actor;
  struct Sprite *sprite;
  s32 stormId;
  s32 stormAlpha = 0;

  if (GameFlag_Get(FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED)) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN);
  }
  if (GameFlag_Get(FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 1)) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_SECOND_SANDSTORM_RISEN);
  }
  if (GameFlag_Get(FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 2)) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_THIRD_SANDSTORM_RISEN);
  }
  for (stormId = ACTOR_FIRST_SANDSTORM; stormId < ACTOR_FOURTH_SANDSTORM;
       stormId++) {
    actor = Actor_GetById(stormId);
    if (actor != 0) {
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        actor->scaleX = Q16_ONE / 32;
        actor->scaleY = Q16_ONE / 32;
      }
      sprite = actor->sprite;
      sprite->flags = 0;
    }
  }
  actor = Actor_GetById(ACTOR_FOURTH_SANDSTORM);
  if (actor != 0) {
    struct SpriteAnim *part;

    sprite = actor->sprite;
    part = sprite->entries[0];
    if (part != 0) {
      part->colorMode = 10;
    }
    sprite->dirty = 1;
    sprite->flags = 0;
  }
  if (GameFlag_Get(FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 4)) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_DOUSED);
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, SUHALLA_DESERT_HAZE_BLEND, IO_WRITE_16);
  if (GameFlag_Get(FLAG_SUHALLA_DESERT_SANDSTORMS_RISEN)) {
    stormAlpha = 16;
    Scene_QueueMusic(SE_SANDSTORM);
  }
  // BLEND_ALPHA(stormAlpha, 16 - stormAlpha), whose operand order compiles
  // differently.
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - stormAlpha) << 8) | stormAlpha,
                      IO_WRITE_16);
}

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gSuhallaDesertStormScript[] = {
    SC_CALL(SuhallaDesert_ClearActorSpriteAndCollisionFlags),
    SC_CALL(SuhallaDesert_ToggleActorActive),
    SC_END,
};

s32 gSuhallaDesertFourthStormScript[] = {
    SC_CALL(SuhallaDesert_ClearActorSpriteAndCollisionFlags),
    SC_CALL(SuhallaDesert_ToggleActorActive),
    SC_END,
};

struct SceneEntrance gSuhallaDesertEntrancesOther[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaDesertNorthEntrances[] = {
    {SUHALLA_DESERT_NORTH_FROM_WORLD_MAP, CONDITION_ALWAYS, 264, 0, 56,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_NORTH_FROM_SUHALLA_DESERT_SOUTH, CONDITION_ALWAYS, 24, 0,
     936, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaDesertSouthEntrances[] = {
    {SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_NORTH, CONDITION_ALWAYS, 488, 0,
     456, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_SOUTH_FROM_WORLD_MAP, CONDITION_ALWAYS, 488, 0, 1208,
     FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_EAST, CONDITION_ALWAYS, 440,
     0, 624, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_WEST, CONDITION_ALWAYS, 56,
     0, 624, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSuhallaDesertCavesEntrances[] = {
    {SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_WEST, CONDITION_ALWAYS, 72,
     0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_CAVES_WEST_FROM_EAST, CONDITION_ALWAYS, 440, 0, 152,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_CAVES_EAST_FROM_WEST, CONDITION_ALWAYS, 632, 0, 152,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_EAST, CONDITION_ALWAYS, 648,
     0, 280, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSuhallaDesertExits[] = {
    SCENE_EXITS(SuhallaDesertNorth),
    SCENE_EXIT(SUHALLA_DESERT_NORTH_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_SUHALLA_DESERT_NORTH),
    SCENE_EXIT(SUHALLA_DESERT_NORTH_TO_SUHALLA_DESERT_SOUTH, SuhallaDesertSouth,
               SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_NORTH),
    SCENE_EXITS(SuhallaDesertSouth),
    SCENE_EXIT(SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_NORTH, SuhallaDesertNorth,
               SUHALLA_DESERT_NORTH_FROM_SUHALLA_DESERT_SOUTH),
    SCENE_EXIT(SUHALLA_DESERT_SOUTH_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_SUHALLA_DESERT_SOUTH),
    SCENE_EXIT(SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_CAVES_EAST,
               SuhallaDesertCaves,
               SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_EAST),
    SCENE_EXIT(SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_CAVES_WEST,
               SuhallaDesertCaves,
               SUHALLA_DESERT_CAVES_FROM_SUHALLA_DESERT_SOUTH_WEST),
    SCENE_EXITS(SuhallaDesertCaves),
    SCENE_EXIT(SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_WEST,
               SuhallaDesertSouth,
               SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_WEST),
    SCENE_EXIT(SUHALLA_DESERT_CAVES_WEST_TO_EAST, SuhallaDesertCaves,
               SUHALLA_DESERT_CAVES_EAST_FROM_WEST),
    SCENE_EXIT(SUHALLA_DESERT_CAVES_EAST_TO_WEST, SuhallaDesertCaves,
               SUHALLA_DESERT_CAVES_WEST_FROM_EAST),
    SCENE_EXIT(SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_EAST,
               SuhallaDesertSouth,
               SUHALLA_DESERT_SOUTH_FROM_SUHALLA_DESERT_CAVES_EAST),
    SCENE_EXITS_END,
};

struct ActorTemplate gSuhallaDesertActorTemplatesOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorTemplate gSuhallaDesertNorthActorTemplates[] = {
    { SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED, (s32)gSuhallaDesertStormScript,
        PIXELS(56), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 1, (s32)gSuhallaDesertStormScript,
        PIXELS(456), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 2, (s32)gSuhallaDesertStormScript,
        PIXELS(120), 0, PIXELS(712), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 3, (s32)gSuhallaDesertStormScript,
        PIXELS(264), 0, PIXELS(712), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 4, (s32)gSuhallaDesertStormScript,
        PIXELS(440), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gSuhallaDesertSouthActorTemplates[] = {
    { SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED, (s32)gSuhallaDesertStormScript,
        PIXELS(216), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 1, (s32)gSuhallaDesertStormScript,
        PIXELS(296), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 2, (s32)gSuhallaDesertStormScript,
        PIXELS(408), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 3, (s32)gSuhallaDesertFourthStormScript,
        PIXELS(376), 0, PIXELS(632), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 4, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MARS_DJINNI, FLAG_DJINNI(DJINNI_FLASH), MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(696), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gSuhallaDesertCavesActorTemplates[] = {
    { SCENE_TABLE_END },
};

s32 gSuhallaDesertChaseScript[] = {
    SC_CALL(SuhallaDesert_ChaseLeader),
    SC_END,
};

/// The first two maps' events, in one table. Its exits keep their numbers,
/// since each map's exit table gives them its own ways out: exit 1 is
/// SUHALLA_DESERT_NORTH_TO_WORLD_MAP in the first and
/// SUHALLA_DESERT_SOUTH_TO_SUHALLA_DESERT_NORTH in the second.
struct Trigger gSuhallaDesertTriggersOther[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3 },
#if !TBS_PATCH_1
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4 },
#else
    { TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 4, CONDITION_ALWAYS, 4 },
#endif
    { TRIGGER_STEP, 9, FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA, EVENT_SCRIPT(EventScript_IodemAsksWhetherToPressOn) },
    { TRIGGER_STEP, 41, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED, EVENT_SCRIPT(EventScript_CaughtByFirstStorm) },
    { TRIGGER_STEP, 42, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 1, EVENT_SCRIPT(EventScript_CaughtBySecondStorm) },
    { TRIGGER_STEP, 43, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 2, EVENT_SCRIPT(EventScript_CaughtByThirdStorm) },
    { TRIGGER_STEP, 44, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 3, EVENT_SCRIPT(EventScript_CaughtByFourthStorm) },
    { TRIGGER_STEP, 45, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 4, EVENT_SCRIPT(EventScript_CaughtByFifthStorm) },
    { TRIGGER_STEP, 51, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED, EVENT_SCRIPT(EventScript_CaughtByFirstStorm) },
    { TRIGGER_STEP, 52, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 1, EVENT_SCRIPT(EventScript_CaughtBySecondStorm) },
    { TRIGGER_STEP, 53, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 2, EVENT_SCRIPT(EventScript_CaughtByThirdStorm) },
    { TRIGGER_STEP, 61, FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN, EVENT_SCRIPT(EventScript_FirstStormRises) },
    { TRIGGER_STEP, 62, FLAG_SUHALLA_DESERT_SECOND_SANDSTORM_RISEN, EVENT_SCRIPT(EventScript_SecondStormRises) },
    { TRIGGER_STEP, 63, FLAG_SUHALLA_DESERT_THIRD_SANDSTORM_RISEN, EVENT_SCRIPT(EventScript_ThirdStormRises) },
    { TRIGGER_STEP, 64, FLAG_SUHALLA_DESERT_FOURTH_SANDSTORM_RISEN, EVENT_SCRIPT(EventScript_FourthStormRises) },
    { TRIGGER_STEP, 65, FLAG_SUHALLA_DESERT_FIFTH_SANDSTORM_RISEN, EVENT_SCRIPT(EventScript_FifthStormRises) },
    { TRIGGER_STEP, 71, FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN, EVENT_SCRIPT(EventScript_FirstStormRises) },
    { TRIGGER_STEP, 72, FLAG_SUHALLA_DESERT_SECOND_SANDSTORM_RISEN, EVENT_SCRIPT(EventScript_SecondStormRises) },
    { TRIGGER_STEP, 73, FLAG_SUHALLA_DESERT_THIRD_SANDSTORM_RISEN, EVENT_SCRIPT(EventScript_ThirdStormRises) },
    { TRIGGER_STEP, 54, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 3, EVENT_SCRIPT(EventScript_CaughtByFourthStorm) },
    { TRIGGER_STEP, 81, FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_SET_OFF, EVENT_SCRIPT(EventScript_StormGivesChase) },
    { TRIGGER_TALK, ACTOR_FLASH, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_FightFlash) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), TREASURE_SPOT_PSYNERGY_STONE, FLAG_SUHALLA_DESERT_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_SUHALLA_DESERT_VIRTUOUS_ARMLET, FIND_ITEM(ITEM_VIRTUOUS_ARMLET) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 104, FLAG_SUHALLA_DESERT_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_SUHALLA_DESERT_COOKIE, FIND_ITEM(ITEM_COOKIE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_SUHALLA_DESERT_MIMIC_BEATEN, FIND_MIMIC | 6 },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT_SCRIPT(EventScript_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM + 1, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT_SCRIPT(EventScript_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM + 2, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT_SCRIPT(EventScript_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM + 3, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT_SCRIPT(EventScript_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM + 4, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT_SCRIPT(EventScript_DouseCastAtStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT_SCRIPT(EventScript_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM + 1, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT_SCRIPT(EventScript_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM + 2, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT_SCRIPT(EventScript_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM + 3, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT_SCRIPT(EventScript_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM + 4, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT_SCRIPT(EventScript_DouseStorm) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RevealStarts) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RevealEnds) },
    { TRIGGER_RAISED, RAISED_SANDSTORM_CARRIES_PARTY_AWAY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StormCarriesPartyAway) },
    { TRIGGER_RAISED, RAISED_CHASING_SANDSTORM_CATCHES, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_CaughtByFifthStorm) },
    { TRIGGERS_END },
};
// clang-format on

struct Trigger gSuhallaDesertCavesTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_EAST},
    {TRIGGERS_END},
};
