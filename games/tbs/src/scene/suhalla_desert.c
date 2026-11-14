/// @file
/// Suhalla Desert, the desert of sandstorms between Suhalla and Suhalla Gate,
/// which the party crosses with Iodem after Babi's audience while Sheba is
/// missing in it.
///
/// Three scenes share this overlay and the area name Suhalla Desert:
///
/// - SuhallaDesertNorth (SceneId_SuhallaDesertNorth): entered from the world
///   map near Suhalla, with five storms.
/// - SuhallaDesertSouth (SceneId_SuhallaDesertSouth): four storms and a fifth
///   that gives chase, Flash, a Mars Djinni, a stone pillar that Reveal shows,
///   and near its way out Iodem's question whether to search on for Sheba
///   (SuhallaDesert_IodemAsksWhetherToPressOn,
///   FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA).
/// - SuhallaDesertCaves (SceneId_SuhallaDesertCaves): two caves under the
///   desert, reached from SuhallaDesertSouth. They place nothing.
///
/// One exit table and, for the first two scenes, one trigger table serve the
/// overlay. The exits in that trigger table keep their numbers, which each
/// scene's exit rows lead its own way.
///
/// The storms are the desert's mechanism, ACTOR_FIRST_SANDSTORM to
/// ACTOR_LAST_SANDSTORM in the first two scenes.
///
/// As the party nears one (cell behaviors 61 to 65 and 71 to 73) it rises,
/// growing to full size (SuhallaDesert_RaiseStorm). The first time, the sand
/// haze fades in. The haze is BG1 blended over the rest
/// (SUHALLA_DESERT_HAZE_BLEND). The first storm to rise while Iodem travels
/// with the party brings him out to warn of them.
///
/// Walking into one (cell behaviors 41 to 45 and 51 to 54) draws the leader
/// to its centre and lifts the party, spinning, for SANDSTORM_CAPTURE_FRAMES
/// (SuhallaDesert_CatchInStorm).
///
/// Douse in that time stops the storm and the monster in it attacks
/// (SuhallaDesert_DouseStorm). Otherwise the storm carries the party up and
/// drops it on the World Map outside Suhalla
/// (SuhallaDesert_StormCarriesPartyAway), which is why everyone the storms
/// catch winds up there.
///
/// In SuhallaDesertSouth, cell behavior 81 sets the fifth storm chasing the
/// leader (SuhallaDesert_ChaseLeader), and it catches a leader on the move.
///
/// A doused storm's flag is an area flag, five from
/// FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED and five from
/// FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED, so the storm stays gone
/// until the party leaves the desert's area. The chasing storm, once
/// doused, is gone for good (FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_DOUSED).
///
/// Its scene flags:
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

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor.h"
#include "field/event.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_psynergy_work.h"
#include "field/map_grid_scroll.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "scene/walk_into_leader.h"
#include "sound/sound.h"
#include "system/io_write.h"
#include "system/io_write_queue.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "system/system_call.h"
#include "text/msg_ids.h"

extern s32 gSuhallaDesertStormScript[];
extern s32 gSuhallaDesertFourthStormScript[];
extern struct SceneEntrance gSuhallaDesertEntrancesOther[];
extern struct SceneEntrance gSuhallaDesertNorthEntrances[];
extern struct SceneEntrance gSuhallaDesertSouthEntrances[];
extern struct SceneEntrance gSuhallaDesertCavesEntrances[];
extern u32 gSuhallaDesertExits[];
extern struct ActorPlacement gSuhallaDesertActorPlacementsOther[];
extern struct ActorPlacement gSuhallaDesertNorthActorPlacements[];
extern struct ActorPlacement gSuhallaDesertSouthActorPlacements[];
extern struct ActorPlacement gSuhallaDesertCavesActorPlacements[];
extern s32 gSuhallaDesertChaseScript[];
extern struct Trigger gSuhallaDesertTriggersOther[];
extern struct Trigger gSuhallaDesertCavesTriggers[];

/// Storms doused in the first and second scenes, one flag per storm from the
/// first.
///
/// Each storm stays away once doused.
enum {
  /// The first scene's storms, five from this one.
  FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED = FLAG_AREA_TEMP_01,
  /// The second scene's storms, five from this one.
  FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED = FLAG_AREA_TEMP_11,
  /// A storm has risen, so the sand haze stays faded in.
  FLAG_SUHALLA_DESERT_SANDSTORMS_RISEN = FLAG_AREA_TEMP_40,
  /// The Psynergy Stone has been found, an area flag.
  FLAG_SUHALLA_DESERT_PSYNERGY_STONE = FLAG_AREA_TEMP_51
};

/// The actors, numbering the actor placement rows of the first two
/// scenes from ACTOR_FIRST_PLACED.
///
/// The third scene places none.
enum {
  /// The storms, five in each scene.
  ACTOR_FIRST_SANDSTORM = ACTOR_FIRST_PLACED,
  ACTOR_FOURTH_SANDSTORM = ACTOR_FIRST_SANDSTORM + 3,
  ACTOR_LAST_SANDSTORM = ACTOR_FIRST_SANDSTORM + 4,
  /// In the second scene the last storm gives chase.
  ACTOR_CHASING_SANDSTORM = ACTOR_LAST_SANDSTORM,
  /// Iodem, who steps out of the party to talk and then rejoins it.
  ACTOR_IODEM,
  /// In the second scene, seen only while Reveal lasts, drawn as Goma Cave's
  /// stone pillars.
  ACTOR_REVEALED_PILLAR,
  /// In the second scene, the Mars Djinni.
  ACTOR_FLASH
};

/// Frames a caught party spins before the storm carries it away.
enum { SANDSTORM_CAPTURE_FRAMES = 180 };

/// The treasure spot of SuhallaDesertSouth's hidden Psynergy Stone, whose
/// search row finds the stone (FIND_PSYNERGY_STONE).
///
/// Its actor stands in its cell only while Reveal lasts
/// (SuhallaDesert_RevealStarts moves it to the cell's centre). It is moved
/// off the map as Reveal ends.
enum { TREASURE_SPOT_PSYNERGY_STONE = 100 };

/// The events the storms raise.
///
/// Either the spinning has run out, or the chasing storm has reached the
/// moving leader.
enum {
  RAISED_CHASING_SANDSTORM_CATCHES = 55,
  RAISED_SANDSTORM_CARRIES_PARTY_AWAY = 99
};

/// The sand haze, which is BG1 blended over every layer.
///
/// Its weight rises from 0 to 16 as the first storm rises.
#define SUHALLA_DESERT_HAZE_BLEND                                              \
  (BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

static void SuhallaDesert_SetUpFirstMapStorms(void);
static void SuhallaDesert_SetUpSecondMapStorms(void);

static u16 sSuhallaDesertSavedColor;

/// The colour Reveal pulses, the last of BG palette 12.
#define SUHALLA_REVEAL_PALETTE_COLOR BG_PLTT_COLOR(12, 15)

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/// The first sandstorm (ACTOR_FIRST_SANDSTORM) has risen on this visit.
///
/// The four flags after it are the next four sandstorms'.
#define FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN FLAG_SCENE_TEMP_06
/// The second sandstorm has risen on this visit.
#define FLAG_SUHALLA_DESERT_SECOND_SANDSTORM_RISEN FLAG_SCENE_TEMP_07
/// The third sandstorm has risen on this visit.
#define FLAG_SUHALLA_DESERT_THIRD_SANDSTORM_RISEN FLAG_SCENE_TEMP_08
/// The fourth sandstorm (ACTOR_FOURTH_SANDSTORM) has risen on this visit.
#define FLAG_SUHALLA_DESERT_FOURTH_SANDSTORM_RISEN FLAG_SCENE_TEMP_09
/// The fifth sandstorm (ACTOR_LAST_SANDSTORM) has risen on this visit.
#define FLAG_SUHALLA_DESERT_FIFTH_SANDSTORM_RISEN FLAG_SCENE_TEMP_0A
/// In SuhallaDesertSouth, the sandstorm that gives chase has set off after the
/// leader.
#define FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_SET_OFF FLAG_SCENE_TEMP_0E
/// A sandstorm has caught the leader and lifts the party, and Douse can put it
/// out.
#define FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM FLAG_SCENE_TEMP_0F
/// A byte flag that holds the frames left before the sandstorm that caught
/// the leader carries the party away.
#define FLAG_SUHALLA_DESERT_CAPTURE_FRAMES_LEFT FLAG_SCENE_TEMP_10
/// A byte flag that holds the actor id of the sandstorm that caught the
/// leader.
#define FLAG_SUHALLA_DESERT_CATCHING_SANDSTORM FLAG_SCENE_TEMP_18

/// In SuhallaDesertSouth, talking to Flash, a Mars Djinni, starts its battle.
static void SuhallaDesert_FightFlash(void) {
  Field_RequestDjinnBattle(ACTOR_FLASH, DJINNI_FLASH);
}

/// A storm's actor placement script, which clears its sprite and collision
/// flags.
static s32
SuhallaDesert_ClearActorSpriteAndCollisionFlags(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  actor->collisionFlags = 0;
  return 0;
}

/// A storm's actor placement script, which toggles its single-sprite storage.
static s32 SuhallaDesert_ToggleActorActive(struct Actor *actor) {
  actor->spriteStorageKind ^= 1;
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_SUHALLA_DESERT_NORTH) {
    return gSuhallaDesertNorthEntrances;
  }
  if (scene == SCENE_SUHALLA_DESERT_SOUTH) {
    return gSuhallaDesertSouthEntrances;
  }
  if (scene == SCENE_SUHALLA_DESERT_CAVES) {
    return gSuhallaDesertCavesEntrances;
  }
  return gSuhallaDesertEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

/// Where each exit of the three scenes leads.
const u32 *Scene_GetExits(void) { return gSuhallaDesertExits; }

/// The storms, Iodem, and in the second scene Flash and what Reveal shows.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_SUHALLA_DESERT_NORTH) {
    return gSuhallaDesertNorthActorPlacements;
  }
  if (scene == SCENE_SUHALLA_DESERT_SOUTH) {
    return gSuhallaDesertSouthActorPlacements;
  }
  if (scene == SCENE_SUHALLA_DESERT_CAVES) {
    return gSuhallaDesertCavesActorPlacements;
  }
  return gSuhallaDesertActorPlacementsOther;
}

/// Each frame while caught, spins the leader and counts down to raising event
/// 99, the storm carrying the party away.
static void SuhallaDesert_UpdateCaptureTimer(void) {
  struct Actor *leader;
  struct FieldWork *work;
  s32 remainingFrames;

  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  work = gFieldWork;
  leader->facing = gFrameCount << 12;

  remainingFrames =
      FieldGameFlag_GetByte(FLAG_SUHALLA_DESERT_CAPTURE_FRAMES_LEFT);
  if (remainingFrames != 0) {
    if (remainingFrames == 1) {
      work->raisedTrigger = RAISED_SANDSTORM_CARRIES_PARTY_AWAY;
    } else if (GameFlag_Get(FLAG_MENU_OPEN) == 0) {
      remainingFrames -= 1;
    }
  }
  FieldGameFlag_SetByte(FLAG_SUHALLA_DESERT_CAPTURE_FRAMES_LEFT,
                        remainingFrames);
}

/// The leader walks into a storm and is caught.
static void SuhallaDesert_CatchInStorm(s32 stormId) {
  s32 leaderId;
  struct Actor *leader;
  struct Actor *storm;

  leaderId = gMainRoutineWork.leaderActorId;
  leader = Actor_Get(leaderId);
  // DEAD: the storm looked up and dropped; the ROM keeps its call.
  Actor_Get(stormId);
  if (Event_GetFlag(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM) == 0) {
    Event_Start();
    // The leader is drawn to the storm's centre, then lifted.
    Actor_SetAttachedEffect(leaderId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
    Actor_SetAnimation(leaderId, ANIM_LEADER_LOOK_AROUND);
    storm = Actor_Get(stormId);
    if (storm != NULL) {
      Actor_SetDestination(leaderId, storm->x.part.pixel, storm->z.part.pixel);
    }
    Actor_WaitForMoveById(leaderId);
    Sound_Play(SE_SANDSTORM);
    Task_AddFrame(SuhallaDesert_UpdateCaptureTimer, TASK_PASS_FRAME);
    leader->motionFlags = 0;
    FieldActor_SetMoveTargetFixed(leader, leader->x.fixed,
                                  leader->y.fixed + px(32), leader->z.fixed);
    Actor_WaitForMoveById(leaderId);
    leader->velocityY = 0;
    leader->motionFlags = ACTOR_FLOATS;
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_AIRBORNE;
    GameFlag_Set(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM);
    FieldGameFlag_SetByte(FLAG_SUHALLA_DESERT_CATCHING_SANDSTORM, stormId);
    FieldGameFlag_SetByte(FLAG_SUHALLA_DESERT_CAPTURE_FRAMES_LEFT,
                          SANDSTORM_CAPTURE_FRAMES);
    Event_End();
    gFieldWork->encounter = 0; // drop any battle this step rolled
  }
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the leader walks into the scene's first storm
/// (cell behavior 41 or 51).
static void SuhallaDesert_CaughtByFirstStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_SANDSTORM);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the leader walks into the second storm
/// (cell behavior 42 or 52).
static void SuhallaDesert_CaughtBySecondStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_SANDSTORM + 1);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the leader walks into the third storm (cell
/// behavior 43 or 53).
static void SuhallaDesert_CaughtByThirdStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_SANDSTORM + 2);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the leader walks into the fourth storm
/// (cell behavior 44 or 54).
static void SuhallaDesert_CaughtByFourthStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_SANDSTORM + 3);
}

/// The fifth storm catches the leader.
static void SuhallaDesert_CaughtByFifthStorm(void) {
  SuhallaDesert_CatchInStorm(ACTOR_FIRST_SANDSTORM + 4);
}

/// As the party nears a storm it rises, growing to full size with the sand
/// haze fading in the first time.
static void SuhallaDesert_RaiseStorm(s32 stormId) {
  s32 frame;
  s32 weight;
  s32 leaderX;
  s32 leaderZ;
  struct Actor *storm;
  s32 hazeShown;
  struct Actor *leader;
  struct Actor *partyLeader;

  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  storm = Actor_Get(stormId);
  hazeShown = GameFlag_Get(FLAG_SUHALLA_DESERT_SANDSTORMS_RISEN);
  leaderX = leader->x.part.pixel;
  leaderZ = leader->z.part.pixel;
  Event_Start();
  Sound_Play(SE_SANDSTORM);
  for (frame = 0; frame < 16; frame++) {
    storm->scaleX = (frame << 11) + Q16_ONE / 32;
    storm->scaleY = (frame << 12) + Q16_ONE / 16;
    if (hazeShown == 0) {
      weight = frame + 1;
      IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(weight, 16 - weight),
                          IO_WRITE_16);
    }
    wait(1);
  }
  GameFlag_Set(FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN -
               ACTOR_FIRST_SANDSTORM + stormId);
  GameFlag_Set(FLAG_SUHALLA_DESERT_SANDSTORMS_RISEN);
  if (GameFlag_Get(FLAG_IODEM_FOLLOWING_PARTY) != 0 &&
      Event_GetFlag(FLAG_SUHALLA_DESERT_IODEM_WARNED) == 0) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_IODEM_WARNED);
    Actor_SetSpeed(ACTOR_IODEM, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
    partyLeader = Actor_Get(ACTOR_PARTY_LEADER);
    if (partyLeader != NULL) {
      Actor_SetPositionFixed(ACTOR_IODEM, partyLeader->x.fixed,
                             partyLeader->z.fixed);
    }
#ifdef BUGFIX
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
#else
    // BUG: a heading where Actor_FaceActor takes an actor id.
    Actor_FaceActor(ACTOR_IODEM, FACING_SOUTH, 0);
#endif
    Actor_Get(ACTOR_IODEM)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkTo(ACTOR_IODEM, leaderX, leaderZ - 16);
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, leaderX + 8, leaderZ - 40);
    Event_Wait(1);
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_SetAnimation(ACTOR_IODEM, ANIM_STAND);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Event_SetMessageId(MsgSuhallaSandstorm);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
    // "A sandstorm!"
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    // "There'll be trouble if we get caught up in one of those..."
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "But something is wrong... Why are there so many?"
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 30);
    Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
    // "If we don't get past here, we'll never make it to Babi..."
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    // "We must step carefully if we hope to escape the desert."
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_SetSpeed(ACTOR_IODEM, 1);
    Scene_WalkIntoLeader(ACTOR_IODEM);
  }
  Event_End();
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the party nears the scene's first storm
/// (cell behavior 61 or 71).
static void SuhallaDesert_FirstStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_SANDSTORM);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the party nears the second storm (cell
/// behavior 62 or 72).
static void SuhallaDesert_SecondStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_SANDSTORM + 1);
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the party nears the third storm (cell
/// behavior 63 or 73).
static void SuhallaDesert_ThirdStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_SANDSTORM + 2);
}

/// In SuhallaDesertNorth, the party nears the fourth storm (cell behavior
/// 64).
static void SuhallaDesert_FourthStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_SANDSTORM + 3);
}

/// In SuhallaDesertNorth, the party nears the fifth storm (cell behavior
/// 65).
static void SuhallaDesert_FifthStormRises(void) {
  SuhallaDesert_RaiseStorm(ACTOR_FIRST_SANDSTORM + 4);
}

/// Iodem asks whether to search the desert for Sheba again or press on.
static void SuhallaDesert_IodemAsksWhetherToPressOn(void) {
  struct Actor *leader;

  if (GameFlag_Get(FLAG_IODEM_FOLLOWING_PARTY) != 0 &&
      // Area 55 is Lalivero's, by the scene table: the party has not yet been
      // there.
      GameFlag_Get(FLAG_AREA_VISITED(55)) == 0 &&
      GameFlag_Get(FLAG_IODEM_MET_WOUNDED_SOLDIERS) != 0) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA);
    Event_Start();
    Event_SetMessageId(MsgSuhallaNowhereFound);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != NULL) {
      Actor_SetPositionFixed(ACTOR_IODEM, leader->x.fixed, leader->z.fixed);
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
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    // "Sheba is nowhere to be found."
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    // "I wonder what happened..."
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
    // "Should we search the desert once more or press on to Lalivero?"
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 30);
    // "What do you say, Isaac? Should we keep going?"
    // Yes: "Ah, yes. I had forgotten about your foes..."
    // No: "But I must carry out Babi's orders..."
    Event_AskYesNo(ACTOR_IODEM, 0);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
    // "Our guard gate lies ahead."
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    // "We can leave Sheba to the soldiers at the gate."
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_SetSpeed(ACTOR_IODEM, 0.7);
    Actor_WalkToAndWait(ACTOR_IODEM, 440, 1256);
    // "Well, perhaps we should head there first, then."
    Event_ShowMessage(ACTOR_IODEM, 0);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Scene_WalkIntoLeader(ACTOR_IODEM);
    Event_End();
  }
}

/// How far apart two coordinates are.
#define GAP(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

/// The chasing storm's script, which follows the leader.
static s32 SuhallaDesert_ChaseLeader(struct Actor *actor) {
  struct EffectOptions options;
  struct Actor *leader;
  s32 sparkFrame;

  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  actor->acceleration = px(1) / 4;
  actor->speedLimit = px(3) / 2;
  actor->motionFlags = 0;
  Actor_SetSpriteFlags(actor, 0);
  actor->spriteStorageKind ^= 1;
  if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS)) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
  } else {
    actor->targetX = leader->x.fixed;
    actor->targetY = leader->groundHeight;
    actor->targetZ = leader->z.fixed;
    if (GAP(actor->x.fixed, leader->x.fixed) +
            GAP(actor->z.fixed, leader->z.fixed) <
        px(8)) {
      struct FieldWork *work = gFieldWork;

      if (leader->motionFlags != 0) {
        work->raisedTrigger = RAISED_CHASING_SANDSTORM_CATCHES;
      }
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      actor->targetX = leader->x.fixed;
      actor->targetY = leader->y.fixed;
      actor->targetZ = leader->z.fixed;
    }
  }
  // Sand every eight frames, with a random spin.
  sparkFrame = gFrameCount & 7;
  if (sparkFrame == 0) {
    options.startScaleX = 4 * Q16_ONE / 5;
    options.startScaleY = 4 * Q16_ONE / 5;
    options.spin = (Random_Below(4096)) + 0xf800;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0,
                      sparkFrame, sparkFrame,
                      EFFECT_USE_SPIN | EFFECT_USE_START_SCALE |
                          EFFECT_SCRIPT_MEDIUM,
                      &options);
  }
  return 1;
}

/// In SuhallaDesertSouth, until the chasing storm has been doused, it
/// appears behind the party and gives chase.
static void SuhallaDesert_StormGivesChase(void) {
  if (GameFlag_Get(FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_DOUSED) == 0) {
    GameFlag_Set(FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_SET_OFF);
    Actor_SetPosition(ACTOR_CHASING_SANDSTORM, 120, 824);
    Actor_SetScript(ACTOR_CHASING_SANDSTORM, gSuhallaDesertChaseScript);
  }
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), Douse cast at the storm that caught the leader
/// rains on it from above.
static void SuhallaDesert_DouseCastAtStorm(void) {
  struct FieldEffectWork *work = gFieldEffectWork;

  work->childOption = 1;
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), Douse stops the storm that caught the leader.
static void SuhallaDesert_DouseStorm(s32 unused, s32 stormId) {
  // The storm no longer carries the party away.
  if (gFieldWork->raisedTrigger == RAISED_SANDSTORM_CARRIES_PARTY_AWAY) {
    gFieldWork->raisedTrigger = 0;
  }
  GameFlag_Clear(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM);
  if (gMainRoutineWork.next == SCENE_SUHALLA_DESERT_NORTH) {
    GameFlag_Set((stormId + FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED -
                  ACTOR_FIRST_SANDSTORM));
  } else {
    if (gMainRoutineWork.next == SCENE_SUHALLA_DESERT_SOUTH) {
      GameFlag_Set((stormId + FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED -
                    ACTOR_FIRST_SANDSTORM));
    }
  }
  FieldGameFlag_SetByte(FLAG_SUHALLA_DESERT_CAPTURE_FRAMES_LEFT, 0);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 5);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  if (gMainRoutineWork.next == SCENE_SUHALLA_DESERT_SOUTH) {
    if (stormId == ACTOR_FOURTH_SANDSTORM) {
      Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 7);
    } else {
      if (stormId == ACTOR_CHASING_SANDSTORM) {
        Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 6);
        Actor_Stop(ACTOR_CHASING_SANDSTORM);
        Actor_SetPosition(ACTOR_CHASING_SANDSTORM, 0, 0);
      }
    }
  }
  Actor_Get(gMainRoutineWork.leaderActorId)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
}

/// In Suhalla Desert (SceneId_SuhallaDesertNorth and
/// SceneId_SuhallaDesertSouth), the spinning runs out (event 99) and the storm
/// carries the party away.
static void SuhallaDesert_StormCarriesPartyAway(void) {
  s32 stormId;
  struct Actor *leader;
  struct Actor *storm;
  s32 frame;

  stormId = FieldGameFlag_GetByte(FLAG_SUHALLA_DESERT_CATCHING_SANDSTORM);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  storm = Actor_Get(stormId);
  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Sound_Play(SE_GUSH);
  // BUG: this passes an actor id where the sprite helper needs a pointer.
#ifdef BUGFIX
  Actor_SetSpriteFlags(leader, 0);
#else
  Actor_SetSpriteFlags((struct Actor *)gMainRoutineWork.leaderActorId, 0);
#endif
  storm->motionFlags = 0;
  leader->motionFlags = 0;
  leader->velocityY = 0;
  leader->frozen = 1;
  storm->frozen = 1;
  // Both rise faster and faster for a second.
  for (frame = 0; frame < 60; frame++) {
    leader->velocityY += px(1) / 5;
    storm->velocityY += px(1) / 5;
    wait(1);
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
  GameFlag_Set(FLAG_ARRIVE_FALLING);
  if (gMainRoutineWork.next == SCENE_SUHALLA_DESERT_SOUTH &&
      FieldGameFlag_GetByte(FLAG_SUHALLA_DESERT_CATCHING_SANDSTORM) ==
          ACTOR_FOURTH_SANDSTORM) {
    Event_SetDestination(SCENE_WORLD_MAP,
                         WORLD_MAP_FROM_SUHALLA_DESERT_SANDSTORM);
  } else {
    Event_SetDestination(SCENE_WORLD_MAP, WORLD_MAP_FROM_SUHALLA);
  }
}

/// While Reveal lasts, one palette color pulses, brightening and dimming over
/// 64 frames.
static void SuhallaDesert_PulseRevealPalette(void) {
  u16 brightnessFrame = gFrameCount & 63;
  s32 grey;

  if (brightnessFrame > 31)
    brightnessFrame = 64 - brightnessFrame;
  grey = (brightnessFrame >> 1) + 7;
  grey |= (grey << 10) | (grey << 5);
  SUHALLA_REVEAL_PALETTE_COLOR = ((u32)grey << 16) >> 16;
}

#ifdef UBFIX
/// Reveal wears off, and what it showed is hidden again.
static void SuhallaDesert_RevealEnds(void) {
#else
/// Reveal wears off, and what it showed is hidden again.
static s32 SuhallaDesert_RevealEnds(void) {
  s32 removalResult;
#endif
  if (gMainRoutineWork.next == SCENE_SUHALLA_DESERT_SOUTH) {
    Actor_Get(ACTOR_REVEALED_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_Get(ACTOR_REVEALED_PILLAR)->motionFlags =
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

/// Reveal shows a hidden stone pillar in the desert's south.
static UNREAD_RESULT SuhallaDesert_RevealStarts(void) {
  if (gMainRoutineWork.next == SCENE_SUHALLA_DESERT_SOUTH) {
    Actor_Get(ACTOR_REVEALED_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_Get(ACTOR_REVEALED_PILLAR)->motionFlags = 0;
    Actor_SetPosition(ACTOR_REVEALED_PILLAR, 248, 712);
    MapGrid_CopyCellAttributes(31, 95, 1, 1, 15, 44);
    TreasureSpot_SetPosition(TREASURE_SPOT_PSYNERGY_STONE, -1, -1);
    TreasureSpot_MoveLeaderAway();
    MapGrid_CopyCellAttributes(127, 127, 1, 1, 12, 71);
#ifdef UBFIX
    Task_AddFrame(SuhallaDesert_PulseRevealPalette, TASK_PASS_FRAME);
#else
    return Task_AddFrame(SuhallaDesert_PulseRevealPalette, TASK_PASS_FRAME);
#endif
  }
}

/// The third scene's exits, or the storms, Iodem, treasure and Reveal of the
/// first two.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_SUHALLA_DESERT_CAVES) {
    return gSuhallaDesertCavesTriggers;
  }
  return gSuhallaDesertTriggersOther;
}

/// Sets a scene up as the party enters.
s32 Scene_Initialize(void) {
  struct MapGridScrollWork *grid;

  grid = HEAP_SLOT_BLOCK(HEAP_SLOT_MAP_GRID);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  if (FieldGameFlag_GetByte(FLAG_SUHALLA_DESERT_CAPTURE_FRAMES_LEFT) != 0) {
    gMainRoutineWork.movementMode = LEADER_MOVEMENT_AIRBORNE;
    Task_AddFrame(SuhallaDesert_UpdateCaptureTimer, TASK_PASS_FRAME);
  }
  if (gMainRoutineWork.next == SCENE_SUHALLA_DESERT_NORTH ||
      gMainRoutineWork.next == SCENE_SUHALLA_DESERT_SOUTH) {
    sSuhallaDesertSavedColor = SUHALLA_REVEAL_PALETTE_COLOR;
    SuhallaDesert_RevealEnds();
  }
  if (gMainRoutineWork.next == SCENE_SUHALLA_DESERT_NORTH) {
    SuhallaDesert_SetUpFirstMapStorms();
  } else if (gMainRoutineWork.next == SCENE_SUHALLA_DESERT_SOUTH) {
    SuhallaDesert_SetUpSecondMapStorms();
  } else {
    Sound_Play(SE_STOP_ACTION_SE);
  }
  if (gMainRoutineWork.entrance == ENTRANCE_DEFAULT) {
    grid->flags &= ~DISPCNT_BG1_ON; // no haze on a fresh arrival
  }
  return 0;
}

/// The first scene's storms on entry.
static void SuhallaDesert_SetUpFirstMapStorms(void) {
  struct Actor *actor;
  s32 stormId;
  s32 hazeWeight = 0;

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
    actor = Actor_Get(stormId);
    if (actor != NULL) {
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        actor->scaleX = Q16_ONE / 32;
        actor->scaleY = Q16_ONE / 32;
      }
      actor->sprite->flags = 0;
    }
  }
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, SUHALLA_DESERT_HAZE_BLEND, IO_WRITE_16);
  if (GameFlag_Get(FLAG_SUHALLA_DESERT_SANDSTORMS_RISEN)) {
    hazeWeight = 16;
    Scene_QueueMusic(SE_SANDSTORM);
  }
  // The haze at full weight once a storm has risen, else not at all.
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - hazeWeight) << 8) | hazeWeight,
                      IO_WRITE_16);
}

/// The second scene's storms on entry, as the first scene's.
static void SuhallaDesert_SetUpSecondMapStorms(void) {
  struct Actor *actor;
  struct Sprite *sprite;
  s32 stormId;
  s32 hazeWeight = 0;

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
    actor = Actor_Get(stormId);
    if (actor != NULL) {
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        actor->scaleX = Q16_ONE / 32;
        actor->scaleY = Q16_ONE / 32;
      }
      sprite = actor->sprite;
      sprite->flags = 0;
    }
  }
  actor = Actor_Get(ACTOR_FOURTH_SANDSTORM);
  if (actor != NULL) {
    struct SpriteAnimation *part;

    sprite = actor->sprite;
    part = sprite->entries[0];
    if (part != NULL) {
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
    hazeWeight = 16;
    Scene_QueueMusic(SE_SANDSTORM);
  }
  // The haze at full weight once a storm has risen, else not at all.
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - hazeWeight) << 8) | hazeWeight,
                      IO_WRITE_16);
}

#include "shared/field_effect_duration_scripts.c"

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

struct ActorPlacement gSuhallaDesertActorPlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorPlacement gSuhallaDesertNorthActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED, (s32)gSuhallaDesertStormScript,
        56, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 1, (s32)gSuhallaDesertStormScript,
        456, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 2, (s32)gSuhallaDesertStormScript,
        120, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 3, (s32)gSuhallaDesertStormScript,
        264, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 4, (s32)gSuhallaDesertStormScript,
        440, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gSuhallaDesertSouthActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED, (s32)gSuhallaDesertStormScript,
        216, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 1, (s32)gSuhallaDesertStormScript,
        296, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 2, (s32)gSuhallaDesertStormScript,
        408, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 3, (s32)gSuhallaDesertFourthStormScript,
        376, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SANDSTORM, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 4, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FLASH), MOVEMENT_TYPE_STAND,
        72, 0, 696, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gSuhallaDesertCavesActorPlacements[] = {
    { SCENE_TABLE_END },
};

s32 gSuhallaDesertChaseScript[] = {
    SC_CALL(SuhallaDesert_ChaseLeader),
    SC_END,
};

/// The first two scenes' events, in one table.
///
/// Its exits keep their numbers, since each scene's exit table gives them its
/// own ways out. Exit 1 is SUHALLA_DESERT_NORTH_TO_WORLD_MAP in the first and
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
    { TRIGGER_STEP, 9, FLAG_SUHALLA_DESERT_TALKED_OF_SHEBA, EVENT(SuhallaDesert_IodemAsksWhetherToPressOn) },
    { TRIGGER_STEP, 41, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED, EVENT(SuhallaDesert_CaughtByFirstStorm) },
    { TRIGGER_STEP, 42, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 1, EVENT(SuhallaDesert_CaughtBySecondStorm) },
    { TRIGGER_STEP, 43, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 2, EVENT(SuhallaDesert_CaughtByThirdStorm) },
    { TRIGGER_STEP, 44, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 3, EVENT(SuhallaDesert_CaughtByFourthStorm) },
    { TRIGGER_STEP, 45, FLAG_SUHALLA_DESERT_FIRST_MAP_SANDSTORM_DOUSED + 4, EVENT(SuhallaDesert_CaughtByFifthStorm) },
    { TRIGGER_STEP, 51, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED, EVENT(SuhallaDesert_CaughtByFirstStorm) },
    { TRIGGER_STEP, 52, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 1, EVENT(SuhallaDesert_CaughtBySecondStorm) },
    { TRIGGER_STEP, 53, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 2, EVENT(SuhallaDesert_CaughtByThirdStorm) },
    { TRIGGER_STEP, 61, FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN, EVENT(SuhallaDesert_FirstStormRises) },
    { TRIGGER_STEP, 62, FLAG_SUHALLA_DESERT_SECOND_SANDSTORM_RISEN, EVENT(SuhallaDesert_SecondStormRises) },
    { TRIGGER_STEP, 63, FLAG_SUHALLA_DESERT_THIRD_SANDSTORM_RISEN, EVENT(SuhallaDesert_ThirdStormRises) },
    { TRIGGER_STEP, 64, FLAG_SUHALLA_DESERT_FOURTH_SANDSTORM_RISEN, EVENT(SuhallaDesert_FourthStormRises) },
    { TRIGGER_STEP, 65, FLAG_SUHALLA_DESERT_FIFTH_SANDSTORM_RISEN, EVENT(SuhallaDesert_FifthStormRises) },
    { TRIGGER_STEP, 71, FLAG_SUHALLA_DESERT_FIRST_SANDSTORM_RISEN, EVENT(SuhallaDesert_FirstStormRises) },
    { TRIGGER_STEP, 72, FLAG_SUHALLA_DESERT_SECOND_SANDSTORM_RISEN, EVENT(SuhallaDesert_SecondStormRises) },
    { TRIGGER_STEP, 73, FLAG_SUHALLA_DESERT_THIRD_SANDSTORM_RISEN, EVENT(SuhallaDesert_ThirdStormRises) },
    { TRIGGER_STEP, 54, FLAG_SUHALLA_DESERT_SECOND_MAP_SANDSTORM_DOUSED + 3, EVENT(SuhallaDesert_CaughtByFourthStorm) },
    { TRIGGER_STEP, 81, FLAG_SUHALLA_DESERT_CHASING_SANDSTORM_SET_OFF, EVENT(SuhallaDesert_StormGivesChase) },
    { TRIGGER_TALK, ACTOR_FLASH, CONDITION_ALWAYS, EVENT(SuhallaDesert_FightFlash) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), TREASURE_SPOT_PSYNERGY_STONE, FLAG_SUHALLA_DESERT_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_SUHALLA_DESERT_VIRTUOUS_ARMLET, FIND_ITEM(ITEM_VIRTUOUS_ARMLET) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 104, FLAG_SUHALLA_DESERT_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_SUHALLA_DESERT_COOKIE, FIND_ITEM(ITEM_COOKIE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_SUHALLA_DESERT_MIMIC_BEATEN, FIND_MIMIC | 6 },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT(SuhallaDesert_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM + 1, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT(SuhallaDesert_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM + 2, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT(SuhallaDesert_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM + 3, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT(SuhallaDesert_DouseCastAtStorm) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_SANDSTORM + 4, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT(SuhallaDesert_DouseCastAtStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT(SuhallaDesert_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM + 1, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT(SuhallaDesert_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM + 2, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT(SuhallaDesert_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM + 3, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT(SuhallaDesert_DouseStorm) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_FIRST_SANDSTORM + 4, IF_FLAG_SET(FLAG_SUHALLA_DESERT_CAUGHT_IN_SANDSTORM), EVENT(SuhallaDesert_DouseStorm) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(SuhallaDesert_RevealStarts) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(SuhallaDesert_RevealEnds) },
    { TRIGGER_RAISED, RAISED_SANDSTORM_CARRIES_PARTY_AWAY, CONDITION_ALWAYS, EVENT(SuhallaDesert_StormCarriesPartyAway) },
    { TRIGGER_RAISED, RAISED_CHASING_SANDSTORM_CATCHES, CONDITION_ALWAYS, EVENT(SuhallaDesert_CaughtByFifthStorm) },
    { TRIGGER_TABLE_END },
};
// clang-format on

struct Trigger gSuhallaDesertCavesTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     SUHALLA_DESERT_CAVES_TO_SUHALLA_DESERT_SOUTH_EAST},
    {TRIGGER_TABLE_END},
};
