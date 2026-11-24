/// @file
/// Fuchin Temple's grounds, and the rooms map that shares its overlay.
///
/// The game packs rooms of several places that share a tile style onto one
/// map. Here that is the inside of Fuchin Temple with two masters' rooms in
/// other areas, Master Feh's school in Xian and Master Hama's room in Lama
/// Temple.
///
/// Two scenes hold four areas, by their area names. In each, story flags
/// pick where the people stand and which event plays on entry.
///
/// - Fuchin Temple (SceneId_FuchinTempleGrounds), outside: a path from the
///   World Map in the south (FUCHIN_TEMPLE_GROUNDS_FROM_WORLD_MAP) climbs
///   beside a pool and waterfall to the temple, a blue-roofed hall at the top,
///   with a traveling healer by its door and a Psynergy Stone to its east. A
///   defeat anywhere here returns the party to
///   FUCHIN_TEMPLE_GROUNDS_AFTER_DEFEAT, in front of the healer.
///
///   A monk stands on a tall stump in the pool, in front of the falls,
///   guarding the grotto behind them. Once Nyunpa has set his test
///   (FLAG_NYUNPA_TEST_ACCEPTED) the monk leaps aside to the next stump
///   (FLAG_FUCHIN_FALLS_GROTTO_OPENED), and the grotto leads into Fuchin Falls
///   Cave (FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE).
///
///   Back out with the Orb of Force (FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND), he
///   sends the party on to Nyunpa (FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST).
///
/// - Lacquer rooms A (SceneId_LacquerRoomsA), a rooms map drawn in red
///   lacquer. Each is a camera box of its own, chosen by the entrance the
///   party comes in by. In story order, by the exits that lead in:
///   - Fuchin Temple (by LACQUER_ROOMS_A_FROM_FUCHIN_TEMPLE_GROUNDS or the
///     default entrance), inside, where Nyunpa meditates on a dais with the
///     sign of yin and yang.
///     - Mind Read on him offers the test (FuchinTemple_NyunpaOffersTest,
///       again after FLAG_FUCHIN_TEMPLE_NYUNPA_TEST_DECLINED).
///     - Back with the Orb, Scene_Initialize plays his lesson on Ki
///       (FuchinTemple_NyunpaTeachesKi, FLAG_NYUNPA_TAUGHT_FORCE).
///     - Once Mogall Forest has set FLAG_NYUNPA_ENLIGHTENED he sits withered
///       (ANIM_NYUNPA_WITHERED) and Mind Read hears his last wish.
///   - Xian (by LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_XIAN and
///     LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_HOUSE), Master Feh's kung fu school.
///     The area-name rules give the name Xian only at
///     LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_XIAN, and Scene_Initialize hides the
///     name at both.
///     - Force from the white line on the floor fells the tree trunk the
///       students train on (FuchinTemple_ForceTree). The first time, Feizhi and
///       Feh come out and she leaves to find Hsu (FLAG_FEIZHI_WENT_TO_HSU),
///       which unlocks LACQUER_ROOMS_A_FEHS_SCHOOL_TO_HOUSE.
///     - Feh then waits to see Isaac's Chi (FLAG_FEH_WAITS_TO_SEE_CHI, or
///       FLAG_FEH_CHI_DEMONSTRATION_DECLINED after two refusals). Seeing the
///       tree fall from the line, he names it Ki (FLAG_FEH_SAW_KI).
///     - Moving the tree with Move or by hand only sets
///       FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL.
///     - Once Hsu is known to be trapped (FLAG_XIAN_HEARD_HSU_TRAPPED),
///       Scene_GetActorPlacements removes the tree and two students.
///     - LACQUER_ROOMS_A_FEHS_SCHOOL_TO_XIAN leads out into Xian.
///   - Lama Temple (by LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE), the room
///     in which Master Hama meditates.
///     - The first visit plays her lesson
///       (FuchinTemple_RunHamasRevealLesson). In it she gives Ivan Reveal and
///       leaves with Feizhi to free Hsu, setting FLAG_XIAN_HEARD_HSU_TRAPPED.
///     - Until FLAG_HSU_RESCUED, Hama and the boy monk in her room are away.
///     - LACQUER_ROOMS_A_HAMAS_ROOM_TO_LAMA_TEMPLE leads back into Lama
///       Temple.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_psynergy_state.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "sound/sound.h"
#include "system/heap_slots.h"
#include "system/io_reg.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "system/system_call.h"
#include "text/msg_ids.h"

extern s32 gFuchinTemple_JumpingStudentScript[];
extern u16 gFuchinTemple_TempleDoorOpens[];
extern u16 gFuchinTemple_FehsDoorOpens[];
extern u16 gFuchinTemple_FehsDoorCloses[];
extern struct SceneEntrance gFuchinTempleGroundsEntrances[];
extern struct SceneEntrance gFuchinTempleEntrancesOther[];
extern u32 gFuchinTempleExits[];
extern struct ActorPlacement gFuchinTempleGroundsActorPlacements[];
extern struct ActorPlacement gFuchinTempleActorPlacements[];
extern struct ActorPlacement gFuchinTempleActorPlacementsFromLamaTemple[];
extern struct Trigger gFuchinTempleGroundsTriggers[];
extern struct Trigger gFuchinTempleTriggers[];
extern struct Trigger gFuchinTempleTriggersFromLamaTemple[];
extern s32 gFuchinTemple_ShotScript[];

/// The cells outside the temple and in its rooms that run events.
enum {
  /// Outside: the road south, out to the world map.
  CELL_BEHAVIOR_FUCHIN_TEMPLE_SOUTH_ROAD = 1,
  /// Outside: the temple's door.
  CELL_BEHAVIOR_FUCHIN_TEMPLE_DOOR = 2,
  /// Outside: the falls behind the monk's stump, the way into the grotto.
  CELL_BEHAVIOR_FUCHIN_FALLS_GROTTO = 3,
  /// Outside: the stump beside the monk, set only on the way out of the
  /// grotto with the Orb of Force.
  CELL_BEHAVIOR_FUCHIN_FALLS_MONK_ASKS = 5,
  /// Outside: where the Psynergy Stone lies, cell (24, 17).
  CELL_BEHAVIOR_FUCHIN_TEMPLE_PSYNERGY_STONE = 110,
  /// Inside: the temple's south door.
  CELL_BEHAVIOR_FUCHIN_TEMPLE_SOUTH_DOOR = 1,
  /// Inside: the jar in Nyunpa's hall, cell (12, 27).
  CELL_BEHAVIOR_FUCHIN_TEMPLE_UNICORN_RING_JAR = 101,
  /// Inside: the school's south door, out to Xian.
  CELL_BEHAVIOR_FEHS_SCHOOL_SOUTH_DOOR = 2,
  /// Inside: the lattice door to Feh's house.
  CELL_BEHAVIOR_FEHS_SCHOOL_HOUSE_DOOR = 3,
  /// Inside: the cells east and south of the school's tree trunk.
  CELL_BEHAVIOR_FEHS_SCHOOL_TREE_EAST_SOUTH = 10,
  /// Inside: the cells west and north of the school's tree trunk.
  CELL_BEHAVIOR_FEHS_SCHOOL_TREE_WEST_NORTH = 11,
  /// Inside: the barrel in the school, cell (6, 14).
  CELL_BEHAVIOR_FEHS_SCHOOL_ANTIDOTE_BARREL = 100,
  /// Hama's room: the south door, out to Lama Temple.
  CELL_BEHAVIOR_HAMAS_ROOM_SOUTH_DOOR = 4,
  /// Hama's room: the jar, cell (27, 27).
  CELL_BEHAVIOR_HAMAS_ROOM_COINS_JAR = 102
};

// Fuchin Temple's area temp flags last until the party enters another area.

/// The party refused Nyunpa's test, so talking to him offers it again.
#define FLAG_FUCHIN_TEMPLE_NYUNPA_TEST_DECLINED FLAG_AREA_TEMP_00
/// Feh's students saw the training tree fall, but not to Chi from the line.
#define FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL FLAG_AREA_TEMP_01

/// The placed actors outside Fuchin Temple (FuchinTempleGrounds),
/// numbered in the order of gFuchinTempleGroundsActorPlacements.
enum FuchinTempleOutsideActorId {
  /// A young man from Kolima.
  ACTOR_YOUNG_MAN_FROM_KOLIMA = ACTOR_FIRST_PLACED,
  /// The monk beneath the falls, at the waterfall grotto.
  ACTOR_FALLS_MONK,
  /// A boy monk who reads omens.
  ACTOR_BOY_MONK_READING_OMENS,
  /// A boy monk who fears for Nyunpa.
  ACTOR_WORRIED_BOY_MONK,
  /// A boy monk who admires Nyunpa.
  ACTOR_ADMIRING_BOY_MONK,
  /// The traveling healer, a man in a tall hat and a long robe.
  ACTOR_HEALER
};

/// The placed actors inside Fuchin Temple and in Master Feh's school in
/// Xian (LacquerRoomsA), numbered in the order of
/// gFuchinTempleActorPlacements.
///
/// They are Nyunpa, Feh's students, Feizhi and Feh, and the training tree.
enum FuchinTempleInsideActorId {
  ACTOR_NYUNPA = ACTOR_FIRST_PLACED,
  /// A young man, by the school's door.
  ACTOR_WELCOMING_STUDENT,
  /// A student who practices kicks.
  ACTOR_KICKING_STUDENT,
  /// A student who practices punches.
  ACTOR_PUNCHING_STUDENT,
  ACTOR_JUMPING_STUDENT,
  /// A student proud of Feh's school.
  ACTOR_PROUD_STUDENT,
  /// A little girl, a student who learns by watching.
  ACTOR_WATCHING_STUDENT,
  /// By the training tree, which he sets up again after it falls.
  ACTOR_TREE_STUDENT,
  /// Keeps the party from Feh, and goes to Feh's door when the tree first falls
  /// from the line.
  ACTOR_MESSENGER_STUDENT,
  ACTOR_FEIZHI,
  ACTOR_FEH,
  ACTOR_TRAINING_TREE,
  /// Where Force strikes the tree.
  ACTOR_TRAINING_TREE_TARGET
};

/// The placed actors in Master Hama's room in Lama Temple (LacquerRoomsA by
/// LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE), numbered in the order of
/// gFuchinTempleActorPlacementsFromLamaTemple.
///
/// Garet, Ivan and Mia, placed first, keep their own slots.
enum HamasRoomActorId {
  ACTOR_HAMA = ACTOR_FIRST_PLACED,
  /// A boy monk in Hama's room.
  ACTOR_HAMAS_BOY_MONK,
  /// Bursts in to say that Hsu is in danger.
  ACTOR_FEIZHI_IN_LAMA_TEMPLE
};

/// Animations of these scenes' own sprites beyond enum ActorAnimation.
enum {
  /// Seated cross-legged with his eyes shut (nyunpa.png's frame 33).
  ANIM_NYUNPA_MEDITATE = 5,
  /// Seated as he meditated, shrunken and darkened, once he has died
  /// enlightened (nyunpa.png's frame 34).
  ANIM_NYUNPA_WITHERED = 6,
  /// A drill of kicks, a knee raised, a kick and a leaping kick, from a
  /// fighting stance (youth_with_headband.tsv).
  ANIM_STUDENT_KICKS = 5,
  /// A drill of quick punches with one kick among them
  /// (youth_with_tied_hair.tsv).
  ANIM_STUDENT_PUNCHES = 5
};

static void FuchinTemple_FeizhiLeavesForHsu(void);
static void FuchinTemple_FehSeesChi(void);
static void Scene_HopLeaderBy(s32 dx, s32 dz);
static void FuchinTemple_StartMindReadDemonstration(s32 caster, s32 target);
static void FuchinTemple_FinishMindReadDemonstration(void);
static void FuchinTemple_SpawnNyunpasParticles(void);
static void FuchinTemple_AnimateRevealLesson(void);

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/// The Psynergy Stone outside the temple has been taken. It refills once the
/// party enters another area.
#define FLAG_FUCHIN_TEMPLE_PSYNERGY_STONE FLAG_AREA_TEMP_50

/// A script step that takes the actor's shadow and other sprite flags away.
static s32
FuchinTemple_ClearScriptedActorSpriteFlags(struct Actor *interpreter) {
  Actor_SetSpriteFlags(interpreter, 0);
  return 0;
}

/// Turns an actor toward the party leader, each frame, while Feh's school
/// waits to see Isaac's Chi.
static UNREAD_RESULT FuchinTemple_FaceLeader(struct Actor *actor) {
  struct Actor *player = Actor_Get(ACTOR_PARTY_LEADER);
  actor->facing = Trig_Atan(player->z.fixed - actor->z.fixed,
                            player->x.fixed - actor->x.fixed);
#ifndef UBFIX
  return 0;
#endif
}

/// The entrances outside, or those of the rooms map.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_FUCHIN_TEMPLE_GROUNDS) {
    return gFuchinTempleGroundsEntrances;
  }
  return gFuchinTempleEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gFuchinTempleExits; }

/// The actors outside, in Hama's room, or in the temple and Feh's school, where
/// once Hsu is known to be trapped the tree and two students are gone.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.next == SCENE_FUCHIN_TEMPLE_GROUNDS) {
    return gFuchinTempleGroundsActorPlacements;
  }
  if (gMainRoutineState.entrance ==
      LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE) {
    return gFuchinTempleActorPlacementsFromLamaTemple;
  }
  // Once Hsu is known to be trapped, the tree and two students are gone, and
  // another student stands elsewhere.
  if (Event_GetFlag(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
    gFuchinTempleActorPlacements[ACTOR_PROUD_STUDENT - ACTOR_FIRST_PLACED]
        .condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
    gFuchinTempleActorPlacements[ACTOR_TREE_STUDENT - ACTOR_FIRST_PLACED]
        .condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
    gFuchinTempleActorPlacements[ACTOR_MESSENGER_STUDENT - ACTOR_FIRST_PLACED]
        .x = toFIXED(288);
    gFuchinTempleActorPlacements[ACTOR_MESSENGER_STUDENT - ACTOR_FIRST_PLACED]
        .z = toFIXED(248);
    gFuchinTempleActorPlacements[ACTOR_TRAINING_TREE - ACTOR_FIRST_PLACED]
        .condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
    gFuchinTempleActorPlacements[ACTOR_TRAINING_TREE_TARGET -
                                 ACTOR_FIRST_PLACED]
        .condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
  }
  Scene_AssignViewFlags(gFuchinTempleActorPlacements);
  return gFuchinTempleActorPlacements;
}

/// Isaac talks to the young man from Kolima on the path below the temple: he
/// asks how the party got here, and whether the bridge is open.
static void FuchinTemple_YoungManFromKolimaAsksAboutBridge(void) {
  Event_Start();
  // "How did you get here? Is the bridge open again?"
  Event_SetMessageId(MsgXianHowDidGetHereBridge);
  if (Event_GetFlag(FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN) != 0) {
    // "Did you know that the forest up ahead used to be a vast prairie?"
    gFieldState->message += 4;
  }
  Event_OpenMessage(ACTOR_YOUNG_MAN_FROM_KOLIMA, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    // Yes: "Really?! Then I can go home to Kolima now?...", or after the
    // prairie: "They say the prairie became a vast forest overnight..."
    GameFlag_Set(FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN);
  } else {
    // No: "Oh... Then you came through the forest? That's too bad.", or after
    // the prairie: "You don't believe me, do you? Well, it's the truth..."
    gFieldState->message++;
  }
  Event_ShowMessage(ACTOR_YOUNG_MAN_FROM_KOLIMA, 0);
  Event_End();
}

/// Isaac talks to the boy monk who admires Nyunpa, below the temple: "Isn't it
/// noble of him to try to save the world...", with up to two more questions.
/// Once Nyunpa has died enlightened, he wonders whether he is watching over
/// them.
static void FuchinTemple_BoyMonkSpeaksOfNyunpa(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_NYUNPA_ENLIGHTENED) != 0) {
    Event_SetMessageId(MsgXianNowHeTrulyBeyondWorlds);
    // "Now, he truly is beyond the world's concerns. Is he watching..."
    // Yes: "I believed that once. But now, I am no longer so sure."
    // No: "Yes... This seems very wrong. The world has lost something..."
    Event_AskYesNo(ACTOR_ADMIRING_BOY_MONK, 0);
    Event_End();
  } else {
    Event_SetMessageId(MsgXianIsntNobleHimTrySave);
    // "Isn't it noble of him to try to save the world by becoming..."
    Event_OpenMessage(ACTOR_ADMIRING_BOY_MONK, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
      gFieldState->message++;
      // "Do you know what you mean when you say that?"
      Event_OpenMessage(ACTOR_ADMIRING_BOY_MONK, 0);
      if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
        gFieldState->message++;
      }
    }
    // Yes: "However, I really do not want to see Nyunpa waste away."
    // Then yes: "Well, one may have many different opinions in this world,
    // but... I wish you could understand our beliefs."
    // Then no: "To achieve enlightenment, one fasts to cleanse body and
    // mind..."
    Event_ShowMessage(ACTOR_ADMIRING_BOY_MONK, 0);
    Event_End();
  }
}

/// Isaac talks to the monk on the stump in front of the falls once Nyunpa has
/// set the test: "I have already heard of your test from Master Nyunpa. You
/// may enter." He leaps aside onto the next stump with a splash, opening the
/// way into the grotto (FLAG_FUCHIN_FALLS_GROTTO_OPENED).
static void FuchinTemple_MonkLetsPartyIntoGrotto(void) {
  struct Actor *monk;
  struct EffectOptions options;

  monk = Actor_Get(ACTOR_FALLS_MONK);
  Event_Start();
  Event_SetMessageId(MsgXianAlreadyHeardTest);
  // "I have already heard of your test from Master Nyunpa. You..."
  Event_ShowMessageThenWait(ACTOR_FALLS_MONK, 0, 20);
  // The monk leaps aside and lands in a splash.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 392);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Sound_Play(SE_WATER);
  Actor_Get(ACTOR_FALLS_MONK)->velocityY = toFIXED(20);
  Actor_Get(ACTOR_FALLS_MONK)->gravity = toFIXED(4);
  Actor_SetSpeed(ACTOR_FALLS_MONK, 3);
  Actor_SetDestination(ACTOR_FALLS_MONK, 152, 392);
  Actor_WaitForMoveById(ACTOR_FALLS_MONK);
  Actor_Get(ACTOR_FALLS_MONK)->gravity = toFIXED(1);
  Actor_FaceDirection(ACTOR_FALLS_MONK, FACING_EAST, 0);
  Sound_Play(SE_WATER);
  // Three white puffs, with only the colour option set.
  options.palette = SPRITE_COLOR_WHITE;
  FieldEffect_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + toFIXED(4)),
                    toFIXED(1) / 2, 0, 0, EFFECT_USE_PALETTE, &options);
  FieldEffect_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + toFIXED(4)),
                    0, 0, 0, EFFECT_USE_PALETTE, &options);
  FieldEffect_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + toFIXED(4)),
                    -toFIXED(1) / 2, 0, 0, EFFECT_USE_PALETTE, &options);
  Event_Wait(30);
  Scene_PlayQueuedMusic();
  MapGrid_CopyCellAttributes(10, 24, 1, 1, 10, 22);
  GameFlag_Set(FLAG_FUCHIN_FALLS_GROTTO_OPENED);
  Event_End();
}

/// Coming back out from behind the falls with the Orb of Force, Isaac hops
/// onto the stump beside the monk, cell (10, 24), and the monk asks: "Young
/// master, did you complete the test?" Either way he sends him up to the
/// temple, where Master Nyunpa is waiting.
///
/// The cell is only set for this on the way out of the grotto
/// (Scene_Initialize), and turned back into a plain stepping stone here.
static void FuchinTemple_MonkAsksWhetherTestIsDone(void) {
  Event_Start();
  GameFlag_Set(FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST);
  Actor_FaceActor(ACTOR_FALLS_MONK, ACTOR_PARTY_LEADER, 0);
  Event_Wait(10);
  Event_SetMessageId(MsgXianYoungMasterDidCompleteTest);
  Actor_RunRepeatedMotion(ACTOR_FALLS_MONK, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  // "Young master, did you complete the test?"
  // Yes: "Why, you have mastered the secrets of Fuchin Temple!?"
  // No: "But you must have..."
  Event_AskYesNo(ACTOR_FALLS_MONK, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FALLS_MONK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  Actor_FaceDirection(ACTOR_FALLS_MONK, FACING_NORTH_NORTHEAST, 20);
  Actor_RunRepeatedMotion(ACTOR_FALLS_MONK, 2);
  Event_Wait(20);
  // "Master Nyunpa is waiting for you..."
  Event_ShowMessageThenWait(ACTOR_FALLS_MONK, 0, 20);
  Actor_FaceDirection(ACTOR_FALLS_MONK, FACING_EAST, 20);
  Actor_SetAnimationAndWait(ACTOR_FALLS_MONK, ANIM_NOD);
  Event_Wait(20);
  // "Proceed to the temple."
  Event_ShowMessageThenWait(ACTOR_FALLS_MONK, 0, 20);
  MapGrid_CopyCellAttributes(10, 26, 1, 1, 10, 24);
  Event_End();
}

/// Isaac reads Nyunpa's mind as he meditates in the temple, or talks to him
/// after turning the test down: Nyunpa offers him the test of the grotto
/// beneath the falls.
static void FuchinTemple_NyunpaOffersTest(void) {
  s32 alreadyDeclinedTrial;

  Event_Start();
  // Revisit an earlier refusal, or introduce the offer through Mind Read.
  alreadyDeclinedTrial = GameFlag_Get(FLAG_FUCHIN_TEMPLE_NYUNPA_TEST_DECLINED);
  if (alreadyDeclinedTrial != 0) {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 504);
    Event_Wait(5);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Actor_Get(ACTOR_NYUNPA)->paused = 0;
    Sound_Play(SE_JUMP);
    Actor_Get(ACTOR_NYUNPA)->velocityY = toFIXED(8);
    Actor_SetAnimation(ACTOR_NYUNPA, ANIM_STAND);
    Event_Wait(30);
    Event_SetMessageId(MsgXianMustSurviveTestGrottoBefore);
  } else {
    Event_SetMessageId(MsgXianMmmmWhoWhoSpeaksMy);
    FuchinTemple_StartMindReadDemonstration(ACTOR_PARTY_LEADER, ACTOR_NYUNPA);
    Event_Wait(30);
    // "Mmmm... Who is that? Who speaks to my mind?"
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    FuchinTemple_FinishMindReadDemonstration();
    Event_Wait(20);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 504);
    Event_Wait(5);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Sound_Play(SE_JUMP);
    Actor_Get(ACTOR_NYUNPA)->paused = alreadyDeclinedTrial;
    Actor_Get(ACTOR_NYUNPA)->velocityY = toFIXED(8);
    Actor_SetAnimation(ACTOR_NYUNPA, ANIM_STAND);
    Event_Wait(30);
    // "Young master... Was that your voice I heard in my mind just now?"
    Event_OpenMessage(ACTOR_NYUNPA, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
      Actor_RunRepeatedMotion(ACTOR_NYUNPA, 2);
      Event_Wait(20);
      // No: "Speak no lies to me!"
      Event_ShowMessage(ACTOR_NYUNPA, 0);
      Event_Wait(20);
      FuchinTemple_StartMindReadDemonstration(ACTOR_NYUNPA, ACTOR_PARTY_LEADER);
      Event_Wait(30);
      Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Event_Wait(50);
      FuchinTemple_FinishMindReadDemonstration();
      Event_Wait(30);
      Actor_SetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
      // "I knew it!"
      Event_ShowMessage(ACTOR_NYUNPA, 0);
    } else {
      gFieldState->message += 2;
      // Yes: "I knew it!"
      Event_ShowMessage(ACTOR_NYUNPA, 0);
    }
    Actor_SetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_NYUNPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    Event_SetMessageId(MsgXianFollowThemDoNot);
    // "You follow them, do you not?"
    Event_OpenMessage(ACTOR_NYUNPA, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
      Actor_ShowEmote(ACTOR_NYUNPA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      FuchinTemple_StartMindReadDemonstration(ACTOR_NYUNPA, ACTOR_PARTY_LEADER);
      Event_Wait(30);
      Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Event_Wait(50);
      FuchinTemple_FinishMindReadDemonstration();
      Event_Wait(30);
      // No: "You speak lies... You are indeed following them!"
      Event_ShowMessage(ACTOR_NYUNPA, 0);
      gFieldState->message += 1;
    } else {
      gFieldState->message += 1;
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
      Event_Wait(20);
      // "I knew it!"
      Event_ShowMessage(ACTOR_NYUNPA, 0);
    }
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_NYUNPA, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "But you must go through Mogall Forest, the endless woods..."
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    // "But perhaps I could, young master."
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_NYUNPA, FACING_NORTH, 20);
    Actor_SetSpeed(ACTOR_NYUNPA, 0.3);
    Actor_WalkToAndWait(ACTOR_NYUNPA, 168, 464);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_NYUNPA, FACING_SOUTH, 40);
    // "If you can endure the trials in the waterfall grotto, I will tell you."
    Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Actor_WalkToAndWait(ACTOR_NYUNPA, 168, 472);
  }
  // "Will you take the test?", or after a refusal, "You must survive the test
  // of the grotto before you can pass through Mogall Forest. Will you take the
  // test?"
  Event_OpenMessage(ACTOR_NYUNPA, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    Event_SetMessageId(MsgXianThenCannotTellGiveUp);
    // "Then I cannot tell you. Give up and go back from whence you..."
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    GameFlag_Set(FLAG_FUCHIN_TEMPLE_NYUNPA_TEST_DECLINED);
  } else {
    // "Do not worry. You will be permitted to enter the grotto." Nyunpa
    // glows as he calls to the monk.
    Event_SetMessageId(MsgXianDoNotWorryWillPermitted);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
    Event_Wait(20);
    FieldCamera_SetSpeed(0.5);
    FieldCamera_MoveToActorById(ACTOR_NYUNPA, 1);
    PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
    PaletteFade_StartInterpolation(30);
    Event_WaitTransitionFrames();
    FieldCamera_WaitForArrival();
    FuchinTemple_SpawnNyunpasParticles();
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_StartInterpolation(30);
    // "Do not worry. You will be permitted to enter the grotto."
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    Actor_RunRepeatedMotion(ACTOR_NYUNPA, 2);
    Event_Wait(20);
    // "Go now, Isaac!"
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(60);
    // "Just remember, you are not the only ones who can read minds."
    Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 10);
    GameFlag_Set(FLAG_NYUNPA_TEST_ACCEPTED);
  }
  Actor_SetAnimation(ACTOR_NYUNPA, ANIM_NYUNPA_MEDITATE);
  Event_End();
}

/// Isaac reads Nyunpa's mind again once the test is accepted, and Nyunpa
/// notices: "You enjoy reading the minds of others, do you not?"
static void FuchinTemple_NyunpaFeelsMindRead(void) {
  Event_Start();
  Event_SetMessageId(MsgXianEnjoyReadingMindsOthersDo);
  // "You enjoy reading the minds of others, do you not?"
  // Yes: "If you overdo it, you will soon learn to despise the ability."
  // No: "That is good. Enjoy reading the minds of others while you..."
  Event_AskYesNo(ACTOR_NYUNPA, 0);
  Event_End();
}

/// Isaac talks to the young man by the door of Master Feh's school in Xian:
/// "This is Master Feh's school... You came to watch?"
static void FuchinTemple_StudentWelcomesToSchool(void) {
  Event_Start();
  Event_SetMessageId(MsgXianMasterFehsSchoolCameWatch);
  // "This is Master Feh's school... You came to watch?"
  // Yes: "Promise me you will be quiet."
  // No: "Warriors should learn martial arts. They will help in battles."
  Event_AskYesNo(ACTOR_WELCOMING_STUDENT, 0);
  Event_End();
}

/// Isaac talks to the student jumping up and down in Master Feh's school: he
/// lands, talks about jumping (later of Lama Temple, or of Hsu), then jumps
/// on.
static void FuchinTemple_TalkToJumpingStudent(void) {
  Event_Start();

  Actor_Get(ACTOR_JUMPING_STUDENT)->paused = 0;

  while (Actor_Get(ACTOR_JUMPING_STUDENT)->y.fixed > 0) {
    wait(1);
  }

  Actor_Get(ACTOR_JUMPING_STUDENT)->y.fixed = 0;

  Actor_Get(ACTOR_JUMPING_STUDENT)->targetY = ACTOR_NO_TARGET;

  Actor_Get(ACTOR_JUMPING_STUDENT)->velocityY = 0;

  Actor_Get(ACTOR_JUMPING_STUDENT)->paused = 1;

  Actor_FaceActor(ACTOR_JUMPING_STUDENT, ACTOR_PARTY_LEADER, 0);

  if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) != 0) {
    Event_SetMessageId(MsgXianHsuDidNotPracticeJumping);
  } else if (GameFlag_Get(FLAG_FEH_SAW_KI) != 0) {
    Event_SetMessageId(MsgXianLamaTempleFarWestIn);
  } else {
    Event_SetMessageId(MsgXianFlexibilityJumpingVeryImportantIn);
  }

  Event_ShowMessage(ACTOR_JUMPING_STUDENT, 0);

  Actor_Get(ACTOR_JUMPING_STUDENT)->facing = FACING_SOUTH;

  Actor_Get(ACTOR_JUMPING_STUDENT)->paused = 0;

  Actor_SetScript(ACTOR_JUMPING_STUDENT, gFuchinTemple_JumpingStudentScript);
  Event_End();
}

/// Isaac talks to the student by the tree trunk in Master Feh's school: "Master
/// Feh is very busy. Do you still wish to see him?" Yes has him point out the
/// line to stand on and use Chi from.
static void FuchinTemple_StudentAsksAboutFeh(void) {
  Event_Start();
  Event_SetMessageId(MsgXianMasterFehVeryBusyDo);
  // "Master Feh is very busy. Do you still wish to see him?"
  // Yes: "Look there. You see that line? Stand there and use Chi. If..."
  // No: "You will not see Master Feh? That is too bad."
  Event_AskYesNo(ACTOR_TREE_STUDENT, 0);
  Event_End();
}

/// The tree trunk's tilt in sixteenths of a turn (FACING_STEP) as it falls
/// sideways: it stands at TREE_STANDING_STEP and lies a quarter turn to either
/// side.
enum {
  TREE_STANDING_STEP = DEGtoANG(180) / FACING_STEP,
  TREE_FALLEN_WEST_STEP = DEGtoANG(90) / FACING_STEP,
  TREE_FALLEN_EAST_STEP = DEGtoANG(270) / FACING_STEP
};

/// The tree trunk topples to the west, faster as it goes, and lands with a
/// thud and a puff of dust.
static void FuchinTemple_KnockTreeDownWest(void) {
  struct Actor *tree = Actor_Get(ACTOR_TRAINING_TREE);
  u32 turnStep;
  s32 angle;

  for (turnStep = TREE_STANDING_STEP; turnStep >= TREE_FALLEN_WEST_STEP;
       turnStep--) {
    angle = turnStep * FACING_STEP;
    tree->sprite->rotation = angle;
    wait((turnStep - TREE_FALLEN_WEST_STEP) * 2);
    tree->x.fixed += Trig_Cos(angle) * 6;
    tree->z.fixed += Trig_Sin(angle) * 6;
  }

  tree->y.fixed = toFIXED(18);
  tree->targetY = toFIXED(18);

  Sound_Play(SE_TOPPLE);

  FieldEffect_Spawn(tree->x.fixed - toFIXED(12), tree->y.fixed,
                    tree->z.fixed + toFIXED(8), -toFIXED(1) / 5, toFIXED(2) / 5,
                    0, 0, 0);
  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + toFIXED(8),
                    -toFIXED(4) / 5, toFIXED(3) / 10, 0, 0, 0);
  FieldEffect_Spawn(tree->x.fixed + toFIXED(10), tree->y.fixed,
                    tree->z.fixed + toFIXED(8), -toFIXED(1), toFIXED(1) / 5, 0,
                    0, 0);
}

/// The tree trunk topples to the east, faster as it goes, and lands with a
/// thud and a puff of dust.
static void FuchinTemple_KnockTreeDownEast(void) {
  struct Actor *tree;
  u32 turnStep;
  s32 angle;

  tree = Actor_Get(ACTOR_TRAINING_TREE);
  for (turnStep = TREE_STANDING_STEP; turnStep <= TREE_FALLEN_EAST_STEP;
       turnStep++) {
    angle = turnStep * FACING_STEP;
    tree->sprite->rotation = angle;
    wait((TREE_FALLEN_EAST_STEP - turnStep) * 2);
    tree->x.fixed -= Trig_Cos(angle) * 6;
    tree->z.fixed -= Trig_Sin(angle) * 6;
  }
  tree->y.fixed = toFIXED(18);
  tree->targetY = toFIXED(18);
  tree->scaleX = -4 * FIXED_ONE / 5;
  Sound_Play(SE_TOPPLE);
  FieldEffect_Spawn(tree->x.fixed - toFIXED(12), tree->y.fixed,
                    tree->z.fixed + toFIXED(8), toFIXED(1), toFIXED(1) / 5, 0,
                    0, 0);
  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + toFIXED(8),
                    toFIXED(4) / 5, toFIXED(3) / 10, 0, 0, 0);
  FieldEffect_Spawn(tree->x.fixed + toFIXED(10), tree->y.fixed,
                    tree->z.fixed + toFIXED(8), toFIXED(1) / 5, toFIXED(2) / 5,
                    0, 0, 0);
}

/// The tree trunk rocks north and falls over that way, with a thud and a puff
/// of dust.
static void FuchinTemple_KnockTreeDownNorth(void) {
  struct Actor *tree = Actor_Get(ACTOR_TRAINING_TREE);
  u32 stepIndex = 0;
  s32 delay = 8;
  do {
    wait(delay);
    tree->z.fixed -= toFIXED(1);
    tree->targetZ = ACTOR_NO_TARGET;
    stepIndex++;
    delay -= 2;
  } while (stepIndex < 4);
  {
    u8 rotation = 0;

    tree->sprite->rotation = rotation;
    Sound_Play(SE_TOPPLE);
    FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed - toFIXED(8),
                      -toFIXED(4) / 5, 0, -toFIXED(1) / 5, 0, 0);
    FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed - toFIXED(8),
                      toFIXED(4) / 5, 0, -toFIXED(1) / 5, 0, 0);
    FieldEffect_Spawn(tree->x.fixed - toFIXED(6), tree->y.fixed,
                      tree->z.fixed + toFIXED(10), toFIXED(1) / 5, 0,
                      -toFIXED(1), 0, 0);
    FieldEffect_Spawn(tree->x.fixed + toFIXED(6), tree->y.fixed,
                      tree->z.fixed + toFIXED(10), toFIXED(1) / 5, 0,
                      -toFIXED(1), 0, 0);
  }
}

/// The tree trunk rocks south and falls over that way, with a thud and a puff
/// of dust.
static void FuchinTemple_KnockTreeDownSouth(void) {
  struct Actor *tree;
  u32 stepIndex;
  FIXED particleSpeed;
  s32 delay;

  tree = Actor_Get(ACTOR_TRAINING_TREE);
  stepIndex = 0;
  delay = 8;
  do {
    wait(delay);
    tree->z.fixed += toFIXED(1);
    tree->targetZ = ACTOR_NO_TARGET;
    stepIndex++;
    delay -= 2;
  } while (stepIndex < 4);

  tree->sprite->rotation = 0;
  tree->z.fixed += toFIXED(24);
  tree->targetZ = ACTOR_NO_TARGET;
  Sound_Play(SE_TOPPLE);

  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + toFIXED(12),
                    -toFIXED(4) / 5, 0, toFIXED(1) / 5, 0, 0);
  particleSpeed = toFIXED(1) / 5;
  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + toFIXED(12),
                    toFIXED(4) / 5, 0, particleSpeed, 0, 0);
  FieldEffect_Spawn(tree->x.fixed - toFIXED(6), tree->y.fixed,
                    tree->z.fixed - toFIXED(8), particleSpeed, 0, toFIXED(1), 0,
                    0);
  FieldEffect_Spawn(tree->x.fixed + toFIXED(6), tree->y.fixed,
                    tree->z.fixed - toFIXED(8), particleSpeed, 0, toFIXED(1), 0,
                    0);
}

/// The fallen tree trunk is set up again at its place in the school.
static inline void FuchinTemple_SetTreeUpAgain(void) {
  Actor_SetPosition(ACTOR_TRAINING_TREE, 232, 168);
  Actor_SetPosition(ACTOR_TRAINING_TREE_TARGET, 232, 168);
  Actor_Get(ACTOR_TRAINING_TREE)->y.fixed = toFIXED(12);
  Actor_Get(ACTOR_TRAINING_TREE)->targetY = ACTOR_NO_TARGET;
  Actor_Get(ACTOR_TRAINING_TREE)->scaleX = 4 * FIXED_ONE / 5;
  Actor_Get(ACTOR_TRAINING_TREE)->sprite->rotation = DEGtoANG(180);
  Sound_Play(SE_CHEST_OPEN);
}

/// The student by the tree walks round to the fallen trunk, on the side away
/// from Isaac.
static void FuchinTemple_StudentWalksAroundTree(void) {
  u16 leaderFacing;

  leaderFacing = Actor_Get(ACTOR_PARTY_LEADER)->facing;
  if (leaderFacing >= FACING_SOUTHEAST && leaderFacing < FACING_SOUTHWEST) {
    Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 216, 168);
    Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 224, 168);
    Actor_FaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTHEAST, 20);
  } else if (leaderFacing >= FACING_SOUTHWEST &&
             leaderFacing < FACING_NORTHWEST) {
    Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 232, 160);
    Actor_FaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTH_SOUTHWEST, 20);
  } else if (leaderFacing >= FACING_NORTHWEST &&
             leaderFacing < FACING_NORTHEAST) {
    Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 216, 168);
    Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 224, 172);
    Actor_FaceDirection(ACTOR_TREE_STUDENT, FACING_NORTHEAST, 20);
  } else {
    Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 232, 160);
    Actor_FaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTHEAST, 20);
  }
}

/// What the student by the tree explains
/// (FuchinTemple_StudentExplainsChiRules).
enum FuchinTempleChiRule {
  /// Psynergy felled the tree, but not from the line.
  FUCHIN_TEMPLE_CHI_NOT_FROM_LINE = 0,
  FUCHIN_TEMPLE_CHI_PUSHED_BY_HAND = 1, ///< Isaac pushed it over by hand
  /// Praise for a warrior Feh has already seen.
  FUCHIN_TEMPLE_CHI_SEEN_BEFORE = 2
};

/// The student by the tree explains the rules, as `mode`, an enum
/// FuchinTempleChiRule, says, and sets the trunk up again.
static void FuchinTemple_StudentExplainsChiRules(s32 mode) {
  s32 rulesMessage;

  Actor_SetSpeed(ACTOR_TREE_STUDENT, 0.8);
  Event_Wait(60);
  rulesMessage = MsgXianCannotPushHands;
  Event_SetMessageId(rulesMessage);
  if (mode == FUCHIN_TEMPLE_CHI_NOT_FROM_LINE) {
    Event_SetMessageId(rulesMessage - 1);
    Actor_ShowEmote(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "Ya! What happened!? The tree fell on its own!"
    Event_ShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_TREE_STUDENT, 2);
    Event_SetMessageId(MsgXianWarriorsCannotUse);
    // "Warriors cannot use... Wait. This is different from Chi."
    Event_ShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_TREE_STUDENT, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "Do not step over the line, warrior."
    Event_ShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_TREE_STUDENT, ANIM_NOD);
    Event_Wait(20);
  }
  if (mode == FUCHIN_TEMPLE_CHI_SEEN_BEFORE) {
    Event_SetMessageId(MsgXianGreatWarriorTrain);
    Actor_RunRepeatedMotion(ACTOR_TREE_STUDENT, 2);
    Event_Wait(20);
  }
  // "You cannot push it with your hands. Attack it with Chi.", "You are a
  // great warrior. I must train, like you must have trained.", or after the
  // warning about the line, "Stand beyond the line. Then try your Chi."
  Event_ShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
  FuchinTemple_StudentWalksAroundTree();
  Actor_RunRepeatedMotion(ACTOR_TREE_STUDENT, 3);
  FuchinTemple_SetTreeUpAgain();
  Event_Wait(40);
  Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 216, 152);
  Actor_FaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTHEAST, 30);
}

/// The students see the tree trunk fall, Feh will not count it because it was
/// not done from the line, and the student sets the trunk up again.
static void FuchinTemple_StudentsSeeTreeFall(void) {
  Actor_FaceActor(ACTOR_PROUD_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_WATCHING_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_TREE_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_FEH, ACTOR_TRAINING_TREE, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_TREE_STUDENT, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgXianLookTreeFell);
  // "Look! The tree fell."
  Event_ShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
  // "Master, did you see? The warrior used Chi!"
  Event_ShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Actor_ShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_ShowEmote(ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "Master, what is the matter?"
  Event_ShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "He did not do it from the line, so I cannot be sure."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_ShowEmote(ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // "That is not good. Warrior, do it right! Do it from the line."
  Event_ShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Actor_TurnToFaceEachOther(ACTOR_TREE_STUDENT, ACTOR_FEH, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_TREE_STUDENT, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_TREE_STUDENT, 0.8);
  FuchinTemple_StudentWalksAroundTree();
  Actor_RunRepeatedMotion(ACTOR_TREE_STUDENT, 3);
  FuchinTemple_SetTreeUpAgain();
  Event_Wait(40);
  Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 216, 152);
  Actor_FaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTH, 30);
  GameFlag_Set(FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL);
}

/// Isaac casts Move on the tree trunk in Master Feh's school: it topples, and
/// the student by it says that was not Chi, or Feh will not count it because
/// it was not done from the line.
static void FuchinTemple_MoveTree(void) {
  Event_Start();

  if (Actor_Get(ACTOR_PARTY_LEADER)->facing > FACING_SOUTH &&
      Actor_Get(ACTOR_PARTY_LEADER)->facing < FACING_NORTH) {
    FuchinTemple_KnockTreeDownWest();
  } else {
    FuchinTemple_KnockTreeDownEast();
  }

  if (GameFlag_Get(FLAG_FEH_WAITS_TO_SEE_CHI) != 0) {
    FuchinTemple_StudentsSeeTreeFall();
  } else {
    FuchinTemple_StudentExplainsChiRules(FUCHIN_TEMPLE_CHI_NOT_FROM_LINE);
  }

  Event_End();
}

/// Isaac walks into the tree trunk in Master Feh's school and pushes it over
/// by hand: "You cannot push it with your hands. Attack it with Chi." The
/// student by it sets it up again.
static void FuchinTemple_PushTreeByHand(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  u16 facing;

  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Event_Wait(20);

  facing = leader->facing;

  if (facing >= FACING_SOUTHEAST && facing < FACING_SOUTHWEST) {
    FuchinTemple_KnockTreeDownSouth();
  } else if (facing >= FACING_SOUTHWEST && facing < FACING_NORTHWEST) {
    FuchinTemple_KnockTreeDownWest();
  } else if (facing >= FACING_NORTHWEST && facing < FACING_NORTHEAST) {
    FuchinTemple_KnockTreeDownNorth();
  } else {
    FuchinTemple_KnockTreeDownEast();
  }

  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FuchinTemple_StudentExplainsChiRules(FUCHIN_TEMPLE_CHI_PUSHED_BY_HAND);
  Event_End();
}

/// Isaac casts Force on the tree trunk in Master Feh's school and it topples.
/// Cast from behind the white line on the floor, the first time, Feizhi and
/// Feh come out; with Feh waiting to see it, he names it Ki. Cast from nearer,
/// the students will not count it.
static void FuchinTemple_ForceTree(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  u16 facing;

  Event_Start();

  facing = leader->facing;
  if (facing >= FACING_SOUTHEAST && facing < FACING_SOUTHWEST) {
    FuchinTemple_KnockTreeDownSouth();
  } else if (facing >= FACING_SOUTHWEST && facing < FACING_NORTHWEST) {
    FuchinTemple_KnockTreeDownWest();
  } else if (facing >= FACING_NORTHWEST && facing < FACING_NORTHEAST) {
    FuchinTemple_KnockTreeDownNorth();
  } else {
    FuchinTemple_KnockTreeDownEast();
  }

  FieldCamera_SetSpeed(1);
  FieldCamera_MoveToActorById(ACTOR_TRAINING_TREE_TARGET, 1);
  FieldCamera_WaitForArrival();

  if (leader->z.part[0].integer <= 209) {
    if (GameFlag_Get(FLAG_FEIZHI_WENT_TO_HSU) == 0 ||
        GameFlag_Get(FLAG_FEH_SAW_KI) != 0) {
      FuchinTemple_StudentExplainsChiRules(FUCHIN_TEMPLE_CHI_NOT_FROM_LINE);
    } else {
      FuchinTemple_StudentsSeeTreeFall();
    }
    Event_End();
    return;
  }

  if (GameFlag_Get(FLAG_FEH_SAW_KI) != 0) {
    FuchinTemple_StudentExplainsChiRules(FUCHIN_TEMPLE_CHI_SEEN_BEFORE);
  } else if (GameFlag_Get(FLAG_FEIZHI_WENT_TO_HSU) == 0) {
    FuchinTemple_FeizhiLeavesForHsu();
  } else {
    FuchinTemple_FehSeesChi();
  }
  Event_End();
}

/// The first time the trunk falls to Force from the line, a student runs to
/// Feh's door, Feh and his daughter Feizhi come out arguing over her visions,
/// and she leaves alone to find Hsu.
static void FuchinTemple_FeizhiLeavesForHsu(void) {
  struct Actor *actor;

  GameFlag_Set(FLAG_FEIZHI_WENT_TO_HSU);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_PROUD_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_TREE_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PROUD_STUDENT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  0);
  Event_Wait(60);
  Event_SetMessageId(MsgXianDidDoWarrior);
  // "What did you do, warrior?"
  Event_ShowMessageThenWait(ACTOR_PROUD_STUDENT, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_PROUD_STUDENT, 0);
  Actor_RunRepeatedMotion(ACTOR_TREE_STUDENT, 1);
  Event_Wait(20);
  // "The warrior used Chi! He did it from the line... and the..."
  Event_ShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TREE_STUDENT, 0);
  Actor_RunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_MESSENGER_STUDENT, 0);
  // "He knows kung fu! Do you know kung fu?"
  // Yes: "Your Chi must be strong if you can use it from afar. I must..."
  // No: "It is unbelievable that someone who does not know kung fu..."
  Event_AskYesNo(ACTOR_MESSENGER_STUDENT, 0);
  Event_Wait(50);
  FieldCamera_FollowActor(ACTOR_MESSENGER_STUDENT, 1);
  Actor_SetSpeed(ACTOR_MESSENGER_STUDENT, 0.8);
  Actor_WalkToAndWait(ACTOR_MESSENGER_STUDENT, 176, 248);
  Actor_WalkToAndWait(ACTOR_MESSENGER_STUDENT, 308, 248);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MESSENGER_STUDENT, FACING_NORTH, 20);
  Sound_Play(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gFuchinTemple_FehsDoorOpens, 78, 13);
  Actor_RunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_MESSENGER_STUDENT, 1.5);
  Actor_Get(ACTOR_MESSENGER_STUDENT)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_MESSENGER_STUDENT, 308, 272);
  Event_Wait(1);
  actor = Actor_Get(ACTOR_MESSENGER_STUDENT);

  actor->steeringFlags = ACTOR_FACES_MOTION | actor->steeringFlags;
  // "Ya! What happened!? The door opened on its own!"
  Event_ShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 50);
  Actor_SetPosition(ACTOR_FEIZHI, 304, 216);
  Actor_WalkToAndWait(ACTOR_FEIZHI, 304, 248);
  Actor_FaceActor(ACTOR_WELCOMING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_KICKING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_PUNCHING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_JUMPING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_PROUD_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_WATCHING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_TREE_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_FEIZHI, 0);
  Event_Wait(10);
  Actor_StartRepeatedMotion(ACTOR_WELCOMING_STUDENT, 2);
  Actor_StartRepeatedMotion(ACTOR_KICKING_STUDENT, 2);
  Actor_StartRepeatedMotion(ACTOR_PUNCHING_STUDENT, 2);
  Actor_StartRepeatedMotion(ACTOR_JUMPING_STUDENT, 2);
  Actor_StartRepeatedMotion(ACTOR_PROUD_STUDENT, 2);
  Actor_StartRepeatedMotion(ACTOR_WATCHING_STUDENT, 2);
  Actor_StartRepeatedMotion(ACTOR_TREE_STUDENT, 2);
  Actor_RunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
  Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Actor_SetPosition(ACTOR_FEH, 304, 216);
  Actor_WalkTo(ACTOR_FEH, 304, 248);
  Actor_WalkTo(ACTOR_FEIZHI, 280, 264);
  Actor_WaitForMoveById(ACTOR_FEH);
  Actor_FaceDirection(ACTOR_FEH, FACING_SOUTH_SOUTHWEST, 0);
  Actor_WaitForMoveById(ACTOR_FEIZHI);
  Sound_Play(SE_DOOR_CLOSE);
  MapGrid_AnimateCells(gFuchinTemple_FehsDoorCloses, 78, 13);
  // "Feizhi! Wait!"
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_FaceActor(ACTOR_WELCOMING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_KICKING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_PUNCHING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_JUMPING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_PROUD_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_WATCHING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_TREE_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_FEIZHI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FEIZHI, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Are you worried that Hsu is late?"
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_FEH, 1);
  Event_Wait(20);
  // "He is only a little late in returning from Lama Temple. Do..."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHEAST, 20);
  // "Something is wrong with Hsu... I sense it."
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_ShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // "Ahhh, Feizhi... Is it your power again?"
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
  Event_Wait(20);
  // "Ever since gems fell from the sky and hit me, I have sensed..."
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  Event_Wait(20);
  // "You say you sense evil ever since you received the power of..."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I do not believe it, Feizhi. Your visions are just coincidences."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_FEIZHI, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_EAST, 20);
  // "My visions came true many times, no?"
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_FaceDirection(ACTOR_MESSENGER_STUDENT, FACING_WEST, 20);
  Actor_SetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
  Event_Wait(20);
  // "I had a vision about Mogall Forest, and that came true..."
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
  Event_Wait(20);
  // "Yes, your predictions came true, Feizhi. It was very surprising!"
  Event_ShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_WEST, 20);
  // "The flood in Altin... Did I not predict that, too?"
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_WELCOMING_STUDENT, ANIM_NOD);
  Event_Wait(20);
  // "All Feizhi's predictions come true."
  Event_ShowMessageThenWait(ACTOR_WELCOMING_STUDENT, 0, 20);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHEAST, 20);
  Actor_RunRepeatedMotion(ACTOR_FEIZHI, 1);
  Event_Wait(20);
  // "See, Father?"
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_ShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // "I have heard this all. I was surprised, too, but it is still..."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "You still don't believe it?"
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  Event_Wait(20);
  // "Hsu is late, so you worry too much. But do not think bad..."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  // "You sent people to find Hsu, correct?"
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  // "Stubborn old man."
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_ShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  // "What did you say, Feizhi? You should not speak so to your..."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I will not ask anyone. I will go alone, then!"
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_FEH, 2);
  Event_Wait(20);
  // "It is too dangerous."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_FEIZHI, 2);
  Event_Wait(10);
  Actor_WalkToAndWait(ACTOR_FEIZHI, 256, 280);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_SOUTH, 20);
  Actor_SetPosition(ACTOR_FEIZHI, 0, 0);
  Actor_ReleaseById(ACTOR_FEIZHI);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_WELCOMING_STUDENT, 2);
  Event_Wait(20);
  // "Ya! Master Feh! Feizhi has gone!"
  Event_ShowMessageThenWait(ACTOR_WELCOMING_STUDENT, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_TREE_STUDENT, 2);
  Event_Wait(20);
  // "Feizhi was very angry. She was frightening!"
  Event_ShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
  Actor_FaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_FEH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
  Event_Wait(20);
  // "Hsu is in danger. Feizhi is going to help."
  Event_ShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Actor_FaceActor(ACTOR_FEH, ACTOR_MESSENGER_STUDENT, 0);
  Event_Wait(20);
  // "Feizhi knows kung fu. She will be all right."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Feizhi is alone. She cannot go too far."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  Event_Wait(20);
  // "She will give up soon enough. Until then, let her be."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_SetSpeed(ACTOR_FEH, 0.8);
  Actor_WalkToAndWait(ACTOR_FEH, 256, 248);
  Actor_FaceDirection(ACTOR_FEH, FACING_NORTH, 20);
  Actor_StartRepeatedMotion(ACTOR_FEH, 1);
  Actor_ShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_WalkToAndWait(ACTOR_FEH, 240, 184);
  Actor_RunRepeatedMotion(ACTOR_FEH, 2);
  Event_Wait(20);
  FuchinTemple_SetTreeUpAgain();
  // "The tree... It fell. What happened?"
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Actor_WalkToAndWait(ACTOR_MESSENGER_STUDENT, 256, 240);
  Actor_FaceDirection(ACTOR_MESSENGER_STUDENT, FACING_NORTH_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 1);
  // "The warrior did it. He used Chi."
  Event_ShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Actor_FaceActor(ACTOR_WELCOMING_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_KICKING_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PUNCHING_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_JUMPING_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PROUD_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_WATCHING_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_TREE_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_RunRepeatedMotion(ACTOR_FEH, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FEH, FACING_SOUTH_SOUTHWEST, 20);
  Actor_WalkToAndWait(ACTOR_FEH, 248, 208);
  Actor_FaceDirection(ACTOR_FEH, FACING_SOUTH_SOUTHWEST, 20);
  // "Warrior, you... Can you use Chi?"
  Event_OpenMessage(ACTOR_FEH, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_RunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 1);
    Event_Wait(20);
    // "Do it again, warrior. Show me your Chi."
    Event_ShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
    GameFlag_Set(FLAG_FEH_WAITS_TO_SEE_CHI);
  } else {
    Event_SkipMessages(1);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Actor_FaceDirection(ACTOR_FEH, FACING_SOUTH, 20);
    Actor_RunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
    Event_Wait(20);
    // "Warrior, you speak nonsense. You used Chi. Do it again!"
    Event_OpenMessage(ACTOR_MESSENGER_STUDENT, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Actor_SetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
      Event_Wait(20);
      Actor_FaceDirection(ACTOR_FEH, FACING_NORTH_NORTHWEST, 20);
      // "Show me your Chi, warrior."
      Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
      GameFlag_Set(FLAG_FEH_WAITS_TO_SEE_CHI);
    } else {
      Event_SkipMessages(1);
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
      Event_Wait(20);
      // "I want to see your Chi, warrior."
      Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
      Event_SetFlag(FLAG_FEH_CHI_DEMONSTRATION_DECLINED);
    }
  }
  Actor_FaceDirection(ACTOR_KICKING_STUDENT, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PUNCHING_STUDENT, FACING_WEST, 20);
  Actor_SetAnimation(ACTOR_KICKING_STUDENT, ANIM_STUDENT_KICKS);
  Actor_SetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_STUDENT_PUNCHES);
  Actor_SetScript(ACTOR_JUMPING_STUDENT, gFuchinTemple_JumpingStudentScript);
}

/// Isaac talks to Feh in his school after turning him down: "Warrior, will you
/// show me your Chi?"
static void FuchinTemple_FehAsksToSeeChi(void) {
  Event_Start();
  Event_SetMessageId(MsgXianWarriorWillShowMeYour);
  // "Warrior, will you show me your Chi?"
  Event_OpenMessage(ACTOR_FEH, 0);

  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_Wait(20);
    // "Very good. Now show me."
    Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
    GameFlag_Set(FLAG_FEH_WAITS_TO_SEE_CHI);
    Event_End();
  } else {
    gFieldState->message += 1;
    // "What you can do is an incredible thing. Hiding it is not good."
    Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
    Event_End();
  }
}

/// The trunk falls to Force from the line with Feh watching, and Feh sees that
/// it is not Chi but Fuchin's Ki: energy focused from the mind
/// (FLAG_FEH_SAW_KI).
static void FuchinTemple_FehSeesChi(void) {
  struct Actor *actor;

  Actor_Get(ACTOR_FEH)->update = 0;
  Actor_Get(ACTOR_PROUD_STUDENT)->update = 0;
  Actor_Get(ACTOR_WATCHING_STUDENT)->update = 0;
  Actor_Get(ACTOR_TREE_STUDENT)->update = 0;
  Actor_Get(ACTOR_MESSENGER_STUDENT)->update = 0;
  Actor_SetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_STAND);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveTo(232, CAMERA_KEEP, 200, 1);
  FieldCamera_WaitForArrival();
  Event_SetMessageId(MsgXianYaTreeFell);
  Actor_SetSpeed(ACTOR_KICKING_STUDENT, 0.8);
  Actor_SetSpeed(ACTOR_JUMPING_STUDENT, 0.8);
  Actor_WalkTo(ACTOR_KICKING_STUDENT, 152, 200);
  Actor_WalkToAndWait(ACTOR_JUMPING_STUDENT, 144, 248);
  Actor_WaitForMoveById(ACTOR_KICKING_STUDENT);
  Actor_FaceActor(ACTOR_WELCOMING_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_PUNCHING_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_PROUD_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_WATCHING_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_TREE_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_FEH, ACTOR_TRAINING_TREE, 0);
  Actor_SetSpeed(ACTOR_KICKING_STUDENT, 1.5);
  Actor_SetSpeed(ACTOR_JUMPING_STUDENT, 2);
  Actor_WalkTo(ACTOR_KICKING_STUDENT, 152, 200);
  Actor_WalkToAndWait(ACTOR_JUMPING_STUDENT, 144, 248);
  Actor_FaceActor(ACTOR_JUMPING_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_WaitForMoveById(ACTOR_KICKING_STUDENT);
  Actor_FaceActor(ACTOR_KICKING_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_RunRepeatedMotion(ACTOR_FEH, 2);
  Event_Wait(20);
  // "Ya! The tree fell!"
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 40);
  Actor_FaceActor(ACTOR_WELCOMING_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_KICKING_STUDENT, ACTOR_FEH, 0);
  Actor_FaceDirection(ACTOR_PUNCHING_STUDENT, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceActor(ACTOR_JUMPING_STUDENT, ACTOR_FEH, 0);
  Actor_FaceDirection(ACTOR_PROUD_STUDENT, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceActor(ACTOR_WATCHING_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_TREE_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_FEH, 0);
  Event_Wait(20);
  // "Master, did you see? The warrior used Chi."
  Event_ShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
  Event_Wait(20);
  // "The warrior's Chi is excellent! Are you surprised, Master?"
  Event_ShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Actor_ShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_ShowEmote(ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "What is wrong, Master?"
  Event_ShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  // "Hmmmm... The warrior did make the tree fall. I grant you that..."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_ShowEmote(ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_ShowEmote(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetSpeed(ACTOR_TREE_STUDENT, 0.8);
  Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 216, 176);
  Actor_FaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTH_SOUTHEAST, 20);
  // "Is something wrong with the warrior's Chi?"
  Event_ShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
  Actor_FaceDirection(ACTOR_FEH, FACING_NORTH_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "What the warrior did... It is not Chi."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_WELCOMING_STUDENT, 2);
  Actor_StartRepeatedMotion(ACTOR_KICKING_STUDENT, 2);
  Actor_StartRepeatedMotion(ACTOR_PUNCHING_STUDENT, 2);
  Actor_StartRepeatedMotion(ACTOR_JUMPING_STUDENT, 2);
  Actor_StartRepeatedMotion(ACTOR_PROUD_STUDENT, 2);
  Actor_StartRepeatedMotion(ACTOR_WATCHING_STUDENT, 2);
  Actor_StartRepeatedMotion(ACTOR_TREE_STUDENT, 2);
  Actor_StartRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_PROUD_STUDENT, 2);
  // "What? It is not Chi?"
  Event_ShowMessageThenWait(ACTOR_PROUD_STUDENT, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  Event_Wait(20);
  // "Hmmm... It looks very similar...but it is not Chi."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_FEH, FACING_SOUTH_SOUTHWEST, 20);
  // "Warrior... Have you been to Fuchin Temple?"
  // Yes: "Ah... I knew it."
  // No: "Hmmm... That is strange."
  Event_AskYesNo(ACTOR_FEH, 0);
  Actor_ShowEmote(ACTOR_WELCOMING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(5);
  Actor_ShowEmote(ACTOR_KICKING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(5);
  Actor_ShowEmote(ACTOR_PUNCHING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(5);
  Actor_ShowEmote(ACTOR_JUMPING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(5);
  Actor_ShowEmote(ACTOR_PROUD_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(5);
  Actor_ShowEmote(ACTOR_WATCHING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(5);
  Actor_ShowEmote(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(5);
  Actor_ShowEmote(ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
  // "How are Chi and Fuchin Temple related?"
  Event_ShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  Event_Wait(20);
  // "I learned my Chi from Master Hama of Lama Temple."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_ShowEmote(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "Then it is the same as Chi, is it not?"
  Event_ShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
  Actor_FaceActor(ACTOR_FEH, ACTOR_TREE_STUDENT, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Very similar... But still different..."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 40);
  Actor_TurnToFaceEachOther(ACTOR_PUNCHING_STUDENT, ACTOR_KICKING_STUDENT, 0);
  Actor_TurnToFaceEachOther(ACTOR_JUMPING_STUDENT, ACTOR_WATCHING_STUDENT, 0);
  Actor_TurnToFaceEachOther(ACTOR_PROUD_STUDENT, ACTOR_TREE_STUDENT, 0);
  Event_Wait(60);
  Actor_FaceActor(ACTOR_KICKING_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_PUNCHING_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_JUMPING_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_PROUD_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_WATCHING_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_TREE_STUDENT, ACTOR_FEH, 0);
  Event_Wait(20);
  // "Chi concentrates the energy of the body into the hand......"
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_FEH, 2);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_WELCOMING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KICKING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JUMPING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PROUD_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_WATCHING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_TREE_STUDENT, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FEH, FACING_SOUTH_SOUTHWEST, 20);
  // "You, warrior, used energy focused from the mind. Understand?"
  // Yes: "This warrior is wise. The power to will something to move..."
  // No: "Remember this... Wishing for something is the same as..."
  Event_AskYesNo(ACTOR_FEH, 0);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  Event_Wait(20);
  // "Fuchin's Ki releases the mind's energy like Chi releases the..."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_WELCOMING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KICKING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JUMPING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PROUD_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_WATCHING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_TREE_STUDENT, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_FEH, 2);
  Event_Wait(20);
  // "The body's energy has limits, but mental energy has few limits."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  // "Train your Ki. It will grow very strong. Do your best, warrior."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FEH, FACING_WEST, 20);
  Actor_RunRepeatedMotion(ACTOR_FEH, 2);
  Event_Wait(20);
  // "Practice, everyone! Chi has its limits, but anyone can use..."
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_SetAnimation(ACTOR_WELCOMING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KICKING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JUMPING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PROUD_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_WATCHING_STUDENT, ANIM_NOD);
  Actor_SetAnimation(ACTOR_TREE_STUDENT, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkTo(ACTOR_KICKING_STUDENT, 120, 200);
  Actor_WalkTo(ACTOR_JUMPING_STUDENT, 120, 248);
  Actor_WaitForMoveById(ACTOR_KICKING_STUDENT);
  Actor_FaceDirection(ACTOR_PUNCHING_STUDENT, FACING_WEST, 20);
  Actor_SetAnimation(ACTOR_KICKING_STUDENT, ANIM_STUDENT_KICKS);
  Actor_SetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_STUDENT_PUNCHES);
  Actor_WaitForMoveById(ACTOR_JUMPING_STUDENT);
  Actor_SetScript(ACTOR_JUMPING_STUDENT, gFuchinTemple_JumpingStudentScript);
  Actor_SetSpeed(ACTOR_TREE_STUDENT, 0.8);
  Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 216, 168);
  Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 232, 168);
  Actor_FaceDirection(ACTOR_TREE_STUDENT, FACING_NORTH, 20);
  Actor_RunRepeatedMotion(ACTOR_TREE_STUDENT, 3);
  Actor_SetPosition(ACTOR_TRAINING_TREE, 232, 168);
  Actor_Get(ACTOR_TRAINING_TREE)->y.fixed = toFIXED(12);
  Actor_Get(ACTOR_TRAINING_TREE)->targetY = ACTOR_NO_TARGET;
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  actor->sprite->rotation = DEGtoANG(180);
  Sound_Play(SE_CHEST_OPEN);
  Event_Wait(40);
  Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 216, 152);
  Actor_FaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTH, 30);
  GameFlag_Clear(FLAG_FEH_WAITS_TO_SEE_CHI);
  GameFlag_Set(FLAG_FEH_SAW_KI);
}

/// Isaac talks to the student practising punches in Master Feh's school once
/// Hsu is known to be trapped: "Water monsters flooded Altin. Did you defeat
/// them, warrior?"
static void FuchinTemple_StudentAsksAboutAltin(void) {
  Event_Start();
  Event_SetMessageId(MsgXianWaterMonstersFloodedAltinDid);
  // "Water monsters flooded Altin. Did you defeat them, warrior?"
  // Yes: "You are much stronger than you look, warrior. I have much..."
  // No: "Oh... You are not the strong warrior Feizhi spoke of?"
  Event_AskYesNo(ACTOR_PUNCHING_STUDENT, 0);
  Event_End();
}

/// Isaac walks north into the lattice door Feh and Feizhi came out of, in the
/// east of the school: "It is locked." until Feizhi has left; then it opens
/// and he walks through into Feh's house.
static void FuchinTemple_OpenLockedDoor(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_FEIZHI_WENT_TO_HSU) == 0 &&
      GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
    // "It is locked."
    FieldMessage_Show(MsgXianItIsLocked, FIELD_MESSAGE_ZOOM);
    Event_End();
  } else {
    Sound_Play(SE_DOOR_OPEN);
    MapGrid_AnimateCells(gFuchinTemple_FehsDoorOpens, 78, 13);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 306, 248);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 304, 216);
    Event_Wait(20);
    Import_EventSetExitRequest(LACQUER_ROOMS_A_FEHS_SCHOOL_TO_HOUSE);
    Event_End();
  }
}

/// Isaac talks to the traveling healer by the temple door: from the south he
/// offers his healing, as a Sanctum does; from any other side he says he is
/// traveling the world to spread good will.
static void FuchinTemple_TalkToHealer(void) {
  u32 facing;

  facing = Actor_Get(ACTOR_PARTY_LEADER)->facing;
  Event_Start();

  if (FACING_IS_NORTH(facing)) {
    Sanctum_Run(ACTOR_HEALER);
  } else {
    Event_SetMessageId(MsgXianImTravelingAroundWorldSpread);
    // "I'm traveling around the world to spread good will."
    Event_ShowMessage(ACTOR_HEALER, 0);
  }

  Event_End();
}

/// Isaac reads the withered Nyunpa's mind once he has died enlightened:
/// "Isaac... Is that you...? I am...counting on you... to bring..."
static void FuchinTemple_ReadNyunpasLastWish(void) {
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_NYUNPA, 2);
  Event_SetMessageId(MsgXianIsaacAmCountingOnBring);
  // "Isaac... Is that you...? I am...counting on you... to bring..."
  Event_ShowMessage(ACTOR_NYUNPA, 0);
  Event_End();
}

/// The triggers outside, in Hama's room, or in the temple and Feh's school.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineState.next == SCENE_FUCHIN_TEMPLE_GROUNDS) {
    return gFuchinTempleGroundsTriggers;
  }
  if (gMainRoutineState.entrance ==
      LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE) {
    return gFuchinTempleTriggersFromLamaTemple;
  }
  return gFuchinTempleTriggers;
}

/// Drops spray over Isaac as he walks in behind the falls, and makes him
/// flicker.
static void FuchinTemple_SpawnGrottoEntryParticle(void) {
  struct EffectOptions options;
  struct Actor *leader;
  s32 paletteDraw;
  s32 fallSpeed;

  leader = Actor_Get(ACTOR_PARTY_LEADER);

  options.palette = SPRITE_COLOR_WHITE;
  paletteDraw = Random_Below(7);
  if ((paletteDraw & 7) == 0)
    options.palette = SPRITE_COLOR_BRG;

  options.startScaleX = 7 * FIXED_ONE / 10;
  options.startScaleY = 4 * FIXED_ONE / 5;

  fallSpeed = Random_Below(8) * (toFIXED(1) / 5);

  FieldEffect_Spawn(leader->x.fixed + toFIXED(8 - (gFrameCount & 15)),
                    leader->y.fixed + toFIXED(24), leader->z.fixed, 0,
                    -fallSpeed, 0, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                    &options);

  if ((gFrameCount & 1) != 0)
    Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  else
    Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_GRB);
}

/// Unused: hops the leader two map cells north; nothing calls it.
static UNUSED void FuchinTemple_HopLeaderNorth(void) {
  Scene_HopLeaderBy(0, -32);
}

/// Unused: hops the leader two map cells south; nothing calls it.
static UNUSED void FuchinTemple_HopLeaderSouth(void) {
  Scene_HopLeaderBy(0, 32);
}

/// Unused: hops the leader two map cells west; nothing calls it.
static UNUSED void FuchinTemple_HopLeaderWest(void) {
  Scene_HopLeaderBy(-32, 0);
}

#include "shared/leader_hop.c"

/// Isaac steps from the monk's stump into the falls, cell (10, 21): spray
/// falls over him and he flickers out of sight as the screen fades, into
/// Fuchin Falls Cave.
static void FuchinTemple_EnterWaterfallGrotto(void) {
  TaskFunc particleTask;

  Event_Start();
  particleTask = FuchinTemple_SpawnGrottoEntryParticle;
  Task_AddFrame(particleTask, TASK_PASS_FRAME);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.2);
  gFieldState->transitionFrames = 60;
  Event_FinishScreenTransition();
  Sound_Play(SE_CAST);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Task_Remove(particleTask);
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE);
  Event_End();
}

/// Walking out through the temple's south door, Isaac leaves Nyunpa's hall for
/// the grounds outside.
static void FuchinTemple_LeaveTemple(void) {
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(LACQUER_ROOMS_A_TO_FUCHIN_TEMPLE_GROUNDS);
}

/// Isaac walks north into the temple's door, cell (18, 18): it opens with a
/// thump, and he walks inside, where Nyunpa meditates.
static void FuchinTemple_EnterTemple(void) {
  Event_Start();
  Sound_Play(SE_THUMP);
  MapGrid_AnimateCells(gFuchinTemple_TempleDoorOpens, 77, 8);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  Event_Wait(16);
  Import_EventSetExitRequest(FUCHIN_TEMPLE_GROUNDS_TO_LACQUER_ROOMS_A);
  Event_End();
}

/// Back in the temple with the Orb of Force, Isaac walks up to Nyunpa, who
/// praises him, names the power he has gained Ki, which the western world
/// calls Force, and tells him to use it to find his way through Mogall
/// Forest (FLAG_NYUNPA_TAUGHT_FORCE).
static void FuchinTemple_NyunpaTeachesKi(void) {
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 168, 504);
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_NYUNPA, 2);
  Actor_SetAttachedEffect(ACTOR_NYUNPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_Get(ACTOR_NYUNPA)->paused = 0;
  Sound_Play(SE_JUMP);
  Actor_Get(ACTOR_NYUNPA)->velocityY = toFIXED(8);
  Actor_SetAnimation(ACTOR_NYUNPA, ANIM_STAND);
  Event_SetMessageId(MsgXianExcellentIsaac);
  // "Excellent, Isaac!"
  Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
  // "I am glad to see you master the secrets of Fuchin Temple!"
  Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
  Event_Wait(20);
  // "We monks call the power you have obtained Ki."
  Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "You seem confused, Isaac."
  Event_OpenMessage(ACTOR_NYUNPA, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
    Event_Wait(20);
    // "This is called 'force' by the western world, but it is..."
    Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
    gFieldState->message += 2;
  } else {
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_NYUNPA, 2);
    Event_Wait(20);
    gFieldState->message++;
    // "You possess great wisdom for one so young, but I feel I must..."
    Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
    Event_Wait(20);
    // "It is called 'force' by the western world, but it is really..."
    Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
  }
  // "I will teach you, as I promised. Use this power to go..."
  Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_NYUNPA, 2);
  Event_Wait(20);
  // "What would happen if you let loose this energy in the forest?"
  Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "Ha ha! You are curious, no?"
  Event_OpenMessage(ACTOR_NYUNPA, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_NYUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Event_SetMessageId(MsgXianCanReadMindsKnowCurious);
    // "I can read minds, you know. You are curious, aren't you?"
    Event_OpenMessage(ACTOR_NYUNPA, 0);
    do {
      if (Event_AskYesNoWithActorAnimation(0, 0) != 1) {
        break;
      }
      Event_Wait(10);
      Actor_ShowEmote(ACTOR_NYUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      Event_SetMessageId(MsgXianDontTryHard);
      // "You don't have to try so hard. You are curious, are you not?"
      Event_OpenMessage(ACTOR_NYUNPA, 0);
    } while (1);
  }
  Event_SetMessageId(MsgXianMonstersWaitInHidingWould);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
  Event_Wait(20);
  // "Monsters that wait in hiding would show themselves. You..."
  Event_OpenMessage(ACTOR_NYUNPA, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    // "Sometimes, you must let go of your belief that fighting..."
    Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
    gFieldState->message++;
  } else {
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_NYUNPA, 2);
    gFieldState->message++;
    // "Ha ha... That is right. There is no good in wanton destruction."
    Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
  }
  Actor_SetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
  Event_Wait(20);
  // "Monsters are cowardly. They will run once they know you can..."
  Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_NYUNPA, 2);
  Event_Wait(20);
  // "They are creatures of the woods... So they do not get lost..."
  Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_NYUNPA, ANIM_NYUNPA_MEDITATE);
  GameFlag_Set(FLAG_NYUNPA_TAUGHT_FORCE);
  Event_End();
}

/// The first time the party enters Master Hama's room in Lama Temple, the boy
/// monk asks them to be quiet while she meditates. Hama teaches Ivan Reveal,
/// Feizhi bursts in to say Hsu is in danger, and the two leave to free him
/// (FLAG_XIAN_HEARD_HSU_TRAPPED).
static void FuchinTemple_RunHamasRevealLesson(void) {
  struct Actor *leader;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.4);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 472, 536);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_FaceDirection(ACTOR_HAMAS_BOY_MONK, FACING_EAST, 20);
  Actor_RunRepeatedMotion(ACTOR_HAMAS_BOY_MONK, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgXianMasterHamaMeditatingPleaseExtremely);
  // "Master Hama is meditating. Please be extremely quiet."
  Event_ShowMessageThenWait(ACTOR_HAMAS_BOY_MONK, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH, 30);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "So, Isaac, you and your Adept companions have finally come."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_HAMAS_BOY_MONK, FACING_NORTH_NORTHEAST, 0);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFX(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFX(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_SetSpeed(ACTOR_GARET, 0.5);
  Actor_SetSpeed(ACTOR_IVAN, 0.5);
  Actor_SetSpeed(ACTOR_MIA, 0.5);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 464, 504);
  Actor_WalkTo(ACTOR_IVAN, 480, 504);
  Actor_WalkTo(ACTOR_GARET, 496, 496);
  Actor_WalkTo(ACTOR_MIA, 448, 496);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
  // "Finally come? Was she expecting us?"
  // Yes: "But how did she know we'd be here now?"
  // No: "Then, how come she knew we were coming, Isaac?"
  Event_AskYesNo(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "I had anticipated your arrival."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  // "You knew we were coming, then?"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "I have been waiting a long while for you. You come seeking..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  // "You know about the Elemental Stars, too? Who... Who are you?"
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHEAST, 20);
  // "I am a Jupiter Adept, just like Ivan."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Event_Wait(20);
  // "I'm not a Jupiter Adept! What are you talking about?"
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "You are, Ivan, although you may not yet know it."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  // "Why do you say that?"
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  // "Do you not have the power to read minds?"
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 30);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  // "This ability is unique to the Jupiter Adepts."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "You predicted our coming... Is that also Psynergy?"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST, 20);
  // "Ho ho! Excellent question, Miss Mia."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "Yes, the ability to foresee events also belongs to the..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
  // "Then, can Ivan learn the power of prediction, too?"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_HAMA, ACTOR_IVAN, 0);
  Event_Wait(10);
  // "He already has."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_IVAN, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // "I don't think I have-"
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "You do. You merely do not yet know that you have it."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  // "The power of Jupiter must be great if it lets you see the..."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceActor(ACTOR_HAMA, ACTOR_GARET, 0);
  Event_Wait(20);
  // "Oh, but Ivan can learn to use an even greater power!"
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  // "What greater power?"
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_FaceActor(ACTOR_HAMA, ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "Reveal."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  // "Reveal?"
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "Yes. I have waited long to pass the power of true sight on..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  // "You waited here in Lama Temple just to teach Reveal to Ivan?"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "You will need Reveal if you are to achieve your goals..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Jupiter Adepts can learn Reveal on their own, over time..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_HAMA, 2);
  Event_Wait(20);
  // "But that power alone will not help you find Saturos as his..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  // "It was Saturos and his companions who blocked Silk Road."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(60);
  // "Felix and the rest of them, huh? Those cowards!"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Felix bears a terrible fate, a burden I would not wish on..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "A terrible fate? What do you mean?"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_StartRepeatedMotion(ACTOR_HAMA, 1);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I am sorry, but I can say nothing more of this matter."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  // "Now, we must focus on the power of Reveal."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "You will need the power of Reveal to cross the evil Lamakan..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
  // "An evil desert? Is that even possible?"
  // Yes: "You believe that, Isaac? Well, I just don't buy it."
  // No: "It's hard to believe, isn't it, Isaac? I feel the same way."
  Event_AskYesNo(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(20);
  // "Why will we need Reveal to cross the desert?"
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_HAMA, FACING_NORTH, 20);
  // "Crossing Lamakan Desert was hard enough with the extreme heat."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH, 20);
  // "But evil has taken hold of the desert, and its heat has..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Entering the desert now is nothing short of asking for death."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "Then how can Saturos cross such a deadly place?"
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "Saturos and Menardi are Mars Adepts."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  // "I am merely guessing... but their powers might help them..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  // "I still don't understand... How will Reveal help us in such..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST, 20);
  Event_Wait(20);
  // "Reveal will help you find oases to cool you against the heat."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(20);
  // "Why would we need Reveal for that? Won't we be able to see..."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_GARET, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  // "Do you know what a mirage is?"
  // Yes: "The desert is filled with mirages that prevent you from..."
  // No: "Intense heat creates a curtain of air that distorts the..."
  Event_AskYesNo(ACTOR_HAMA, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  // "Sounds like we should have Ivan learn Reveal, don't you think?"
  // Yes: "Yeah! I don't want to die in any evil desert!"
  // No: "Oh, come on, Isaac! If we're going to follow Felix, we'll..."
  Event_AskYesNo(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
  // "Ivan, you should let Master Hama teach you how to use Reveal."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 30);
  Actor_FaceDirection(ACTOR_HAMA, FACING_EAST, 20);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "What's wrong, Master Hama? You can pass Reveal on to me,..."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_FaceDirection(ACTOR_HAMA, FACING_NORTH, 20);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  // "I am unsure that it will work."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "In preparation for this day, I tried to pass a power on to a..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "My student was not an Adept..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_HAMA, 1);
  Event_Wait(20);
  // "But I thought anyone could hone a skill he or she already..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "However, I saw no change in the girl."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  // Feizhi runs in from the south.
  Actor_SetPosition(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 608);
  // "No! There was change!"
  Event_ShowMessageThenWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_HAMAS_BOY_MONK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_HAMAS_BOY_MONK, FACING_EAST, 0);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0.8);
  Actor_WalkToAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 536);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "Feizhi... Why are you here?"
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 40);
  Actor_WalkToAndWait(ACTOR_IVAN, 488, 512);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 30);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 512);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Actor_RunRepeatedMotion(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 2);
  Event_Wait(20);
  // "I sensed it!"
  Event_ShowMessageThenWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "Sensed it? What did you sense?"
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 2);
  Actor_SetAttachedEffect(ACTOR_FEIZHI_IN_LAMA_TEMPLE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  // "Hsu is in great danger!"
  Event_ShowMessageThenWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "Hsu left this place a long time ago."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_FEIZHI_IN_LAMA_TEMPLE,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  // "I know... But Hsu did not return to Xian."
  Event_ShowMessageThenWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_HAMA, 1);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  // "...The boulders in the mountains!"
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, ANIM_NOD);
  Event_Wait(20);
  // "I sensed it. I came through the mine. I came to ask for help."
  Event_ShowMessageThenWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I sensed nothing, as I was focused on Isaac's arrival..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "But why has the power come to you now, after all this time?"
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  // BUG: ACTOR_FEIZHI is Feizhi's number in the school's table. Hama's room
  // places no actor at that number, so Feizhi does not bounce before she
  // speaks.
#ifdef BUGFIX
  Actor_StartRepeatedMotion(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 1);
#else
  Actor_StartRepeatedMotion(ACTOR_FEIZHI, 1);
#endif
  Event_Wait(20);
  // "Gems fell from the sky. One hit me right on the head."
  Event_ShowMessageThenWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  // "Gems..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Psynergy Stones..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(2);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(1);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_Wait(5);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_FaceActor(ACTOR_HAMA, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  // "Then this may work after all."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetSpeed(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0.8);
  Actor_WalkToAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 504);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 1);
  Event_Wait(20);
  // "What about Hsu?"
  Event_ShowMessageThenWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  Actor_FaceActor(ACTOR_HAMA, ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0);
  Actor_SetSpeed(ACTOR_HAMA, 0.8);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Actor_WaitForMoveById(ACTOR_HAMA);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "Let us hurry to Hsu's aid!"
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_WalkTo(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 568);
  Actor_WalkToAndWait(ACTOR_HAMA, 472, 536);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_WaitForMoveById(ACTOR_FEIZHI_IN_LAMA_TEMPLE);
  // "Master Hama, what about us? What about Reveal?"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_HAMA, 2);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_FEIZHI_IN_LAMA_TEMPLE, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_HAMA, FACING_NORTH_NORTHEAST, 20);
  // "I had completely forgotten."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Actor_WalkToAndWait(ACTOR_HAMA, 472, 512);
  Actor_FaceDirection(ACTOR_HAMA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_HAMAS_BOY_MONK, FACING_NORTH_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "It seems I succeeded in passing my power to Feizhi. You will..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 448, 512);
  Event_Wait(1);
  {
    struct Actor *leaderActor = Actor_Get(ACTOR_PARTY_LEADER);
    leaderActor->steeringFlags =
        ACTOR_FACES_MOTION | leaderActor->steeringFlags;
  }
  Event_Wait(20);
  FuchinTemple_AnimateRevealLesson();
  Event_Wait(60);
  Party_LearnAbility(IVAN, ABILITY_REVEAL);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "This should do it. Use Reveal to cross the desert."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_HAMAS_BOY_MONK, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_WalkToAndWait(ACTOR_HAMA, 472, 552);
  Actor_RunRepeatedMotion(ACTOR_HAMA, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_HAMA, FACING_NORTH_NORTHEAST, 20);
  // "But your Reveal may not be that strong yet... The oases may not appear."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "The rest is up to Ivan's abilities."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetSpeed(ACTOR_HAMA, 0.5);
  Actor_WalkToAndWait(ACTOR_HAMA, 480, 540);
  // "Ivan, I am very glad to see you have grown to be such a strong man."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetSpeed(ACTOR_HAMA, 0.8);
  Actor_SetSpeed(ACTOR_HAMAS_BOY_MONK, 0.8);
  Actor_WalkTo(ACTOR_HAMA, 472, 608);
  Actor_WalkTo(ACTOR_HAMAS_BOY_MONK, 472, 544);
  Actor_WalkToAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 608);
  Actor_WaitForMoveById(ACTOR_HAMAS_BOY_MONK);
  Actor_WalkTo(ACTOR_HAMAS_BOY_MONK, 472, 608);
  Actor_SetPosition(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 0);
  Actor_WaitForMoveById(ACTOR_HAMA);
  Actor_SetPosition(ACTOR_HAMA, 0, 0);
  Actor_WaitForMoveById(ACTOR_HAMAS_BOY_MONK);
  Actor_SetPosition(ACTOR_HAMAS_BOY_MONK, 0, 0);
  Actor_WalkToAndWait(ACTOR_IVAN, 488, 520);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
  // "Ivan, do you and Master Hama... know each other?"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "No... But she doesn't feel like a stranger..."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  // "Felix and the others must be far ahead of us by now..."
  // Yes: "Then we'd better get moving! Let's go!"
  // No: "The desert may have slowed them down, but we can't waste a second!"
  Event_AskYesNo(ACTOR_GARET, 0);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  // "We can't just leave like this!"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Hama might need our help..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // "She only has Feizhi to assist her. Shouldn't we help, too?"
  Event_OpenMessage(ACTOR_MIA, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(20);
    // "I'm more worried about Felix and the others... But I'm not opposed to
    // rescuing Hsu."
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    gFieldState->message++;
  } else {
    Event_Wait(20);
    gFieldState->message++;
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(20);
    // "I am worried about Feizhi and Master Hama... But I understand why you
    // want to chase after Felix."
    Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  }
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  // "We'll go with you, Isaac, whatever you decide!"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkTo(ACTOR_GARET, 448, 512);
  Actor_WalkToAndWait(ACTOR_MIA, 448, 512);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 20);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 20);
  // "I wonder what Master Hama was talking about..."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Oh, I'm sorry... Let's go."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_WalkToAndWait(ACTOR_IVAN, 448, 512);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Event_End();
  GameFlag_Set(FLAG_XIAN_HEARD_HSU_TRAPPED);
}

/// Sets each room up as the story left it.
s32 Scene_Initialize(void) {
  struct Actor *trainingTree;
  s32 heardHsuTrapped;

  if (gMainRoutineState.next == SCENE_LACQUER_ROOMS_A) {
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    if (gMainRoutineState.entrance ==
        LACQUER_ROOMS_A_FROM_FUCHIN_TEMPLE_GROUNDS) {
      if (GameFlag_Get(FLAG_NYUNPA_ENLIGHTENED)) {
        Actor_SetAnimation(ACTOR_NYUNPA, ANIM_NYUNPA_WITHERED);
      } else {
        Actor_SetAnimation(ACTOR_NYUNPA, ANIM_NYUNPA_MEDITATE);
        if (GameFlag_Get(FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND) &&
            !GameFlag_Get(FLAG_NYUNPA_TAUGHT_FORCE) &&
            !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
          FuchinTemple_NyunpaTeachesKi();
        }
      }
    } else if (gMainRoutineState.entrance ==
                   LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_XIAN ||
               gMainRoutineState.entrance ==
                   LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_HOUSE) {
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
      heardHsuTrapped = GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED);
      if (heardHsuTrapped == 0) {
        trainingTree = Actor_Get(ACTOR_TRAINING_TREE);
        trainingTree->motionFlags = heardHsuTrapped;
        trainingTree->y.fixed = toFIXED(12);
        trainingTree->targetY = toFIXED(12);
        trainingTree->scaleX = 4 * FIXED_ONE / 5;
        trainingTree->scaleY = FIXED_ONE / 2;
        trainingTree->sprite->rotation = DEGtoANG(180);
        if (GameFlag_Get(FLAG_FEIZHI_WENT_TO_HSU)) {
          Actor_SetPosition(ACTOR_FEH, 248, 208);
          if (!GameFlag_Get(FLAG_FEH_SAW_KI)) {
            Actor_SetPosition(ACTOR_MESSENGER_STUDENT, 256, 240);
            Actor_Get(ACTOR_FEH)->update = FuchinTemple_FaceLeader;
            Actor_Get(ACTOR_PROUD_STUDENT)->update = FuchinTemple_FaceLeader;
            Actor_Get(ACTOR_WATCHING_STUDENT)->update = FuchinTemple_FaceLeader;
            Actor_Get(ACTOR_TREE_STUDENT)->update = FuchinTemple_FaceLeader;
            Actor_Get(ACTOR_MESSENGER_STUDENT)->update =
                FuchinTemple_FaceLeader;
          }
        }
      } else {
        trainingTree = Actor_Get(ACTOR_TRAINING_TREE);
        trainingTree->motionFlags = 0;
        trainingTree->y.fixed = toFIXED(12);
        trainingTree->targetY = toFIXED(12);
        trainingTree->scaleX = 4 * FIXED_ONE / 5;
        trainingTree->scaleY = FIXED_ONE / 2;
        trainingTree->collisionFlags |= ACTOR_NOT_TARGETED;
        trainingTree->sprite->rotation = DEGtoANG(180);
        MapGrid_CopyCellAttributes(14, 11, 1, 1, 14, 10);
      }
      // A crate painted solid black and translucent, over the lattice door to
      // Feh's house (LACQUER_ROOMS_A_FEHS_SCHOOL_TO_HOUSE). The
      // school's render is the same with and without it; drawn plain, it covers
      // the door.
      // XXX (no evidence): why it is there; nothing the player sees shows what
      // it was for.
      Actor_Prepare(304, 24, 224, SPRITE_CRATE);
      Actor_SetAnimation(ACTOR_KICKING_STUDENT, ANIM_STUDENT_KICKS);
      Actor_SetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_STUDENT_PUNCHES);
    } else if (gMainRoutineState.entrance ==
               LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE) {
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
      if (!GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
        FuchinTemple_RunHamasRevealLesson();
      } else if (!GameFlag_Get(FLAG_HSU_RESCUED)) {
        Actor_SetPosition(ACTOR_HAMA, 0, 0);
        Actor_SetPosition(ACTOR_HAMAS_BOY_MONK, 0, 0);
      }
    }
  } else {
    Scene_QueueMusic(SE_AMBIENT_WATER);
    Actor_Get(ACTOR_FALLS_MONK)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
    if (gMainRoutineState.entrance ==
            FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE &&
        GameFlag_Get(FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND) &&
        !GameFlag_Get(FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST)) {
      MapGrid_CopyCellAttributes(10, 84, 1, 1, 10, 24);
    }
    if (GameFlag_Get(FLAG_FUCHIN_FALLS_GROTTO_OPENED)) {
      Actor_SetPosition(ACTOR_FALLS_MONK, 152, 392);
      Actor_FaceDirection(ACTOR_FALLS_MONK, FACING_EAST, 0);
      MapGrid_CopyCellAttributes(10, 26, 1, 1, 10, 22);
    }
  }
  return 0;
}

/// Starts Mind Read from caster on target as part of an event, without the
/// window that announces its name.
static void FuchinTemple_StartMindReadDemonstration(s32 caster, s32 target) {
  struct FieldEffectState *effectState;

  GameFlag_Set(FLAG_PLAY_PSYNERGY_CAST);
  FieldPsynergy_LoadResources(ABILITY_MIND_READ, 1);
  effectState = gFieldEffectState;
  FieldPsynergy_SetupActorPair(caster, target);
  effectState->announceName = 0;
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(FIELD_PSYNERGY_HANDLER_ON_TARGET);
  wait(1);
}

/// Finishes the demonstration's Mind Read and lowers the caster's hands.
static void FuchinTemple_FinishMindReadDemonstration(void) {
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldPsynergy_RunHandler(FIELD_PSYNERGY_HANDLER_FINISH);
  FieldPsynergy_StopCasterUpdate();
}

/// Eight crescents of light rise from Nyunpa, ten frames apart, as he glows
/// and calls to the monk at the falls; every second one sounds.
static void FuchinTemple_SpawnNyunpasParticles(void) {
  struct EffectOptions options;
  struct Actor *nyunpa;
  u32 shotIndex;

  nyunpa = Actor_Get(ACTOR_NYUNPA);
  options.priority = 1;
  options.type = SPRITE_CRESCENT;
  options.script = gFuchinTemple_ShotScript;
  options.targetScaleX = FIXED_ONE * 7 / 2;
  options.targetScaleY = FIXED_ONE * 3 / 2;
  for (shotIndex = 0; shotIndex < 8; shotIndex++) {
    Event_Wait(10);
    if (shotIndex & 1) {
      Sound_Play(SE_PSYNERGY_START);
    }
    FieldEffect_Spawn(nyunpa->x.fixed, nyunpa->y.fixed,
                      nyunpa->z.fixed - toFIXED(24), 0, toFIXED(3) / 5, 0,
                      EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                          EFFECT_SCALE_TO_TARGET | EFFECT_USE_PRIORITY |
                          EFFECT_SCRIPT_MEDIUM,
                      &options);
  }
  Event_Wait(60);
}

/// Hama's and Ivan's update routine during the Reveal lesson: they glow, drawn
/// red and blue in turn every two frames.
static void FuchinTemple_GlowRedAndBlue(struct Actor *updated) {
  if ((gFrameCount >> 1) & 1) {
    Actor_SetSpriteColor(updated, SPRITE_COLOR_RED_TINT);
  } else {
    Actor_SetSpriteColor(updated, SPRITE_COLOR_BLUE_TINT);
  }
}

/// The sparkles that rise from Hama, two frames apart, as the Reveal lesson
/// ends.
#define REVEAL_LESSON_SPARKLES 64

/// Hama and then Ivan glow red and blue while the room darkens to purple, then
/// sparkles rise from Hama and the room's colours come back.
static void FuchinTemple_AnimateRevealLesson(void) {
  struct EffectOptions options;
  struct Actor *hama;
  u32 particleIndex;
  FIXED particleX;
  FIXED particleY;
  s32 riseSpeed;

  Sound_Play(SE_PSYNERGY_SPARKLE);
  Actor_Get(ACTOR_HAMA)->update = FuchinTemple_GlowRedAndBlue;
  Event_Wait(40);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(20, 2, 23), PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  Event_Wait(40);
  Sound_Play(SE_PSYNERGY_SPARKLE);
  Actor_Get(ACTOR_IVAN)->update = FuchinTemple_GlowRedAndBlue;
  Event_Wait(120);
  hama = Actor_Get(ACTOR_HAMA);
  options.priority = 1;
  options.palette = SPRITE_COLOR_RBG;
  options.type = SPRITE_SPARKLE;
  for (particleIndex = 0; particleIndex < REVEAL_LESSON_SPARKLES;
       particleIndex++) {
    if ((particleIndex & 3) == 0) {
      Sound_Play(SE_GLITTER);
    }
    particleX = hama->x.fixed + toFIXED(Random_Below(48)) - toFIXED(12);
    particleY = hama->y.fixed + toFIXED(Random_Below(32)) - toFIXED(16);
    riseSpeed = Random_Below(4) * (toFIXED(1) / 2) + toFIXED(1) / 2;
    FieldEffect_Spawn(
        particleX, particleY, hama->z.fixed, 0, riseSpeed, 0,
        EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
    wait(2);
  }
  Sound_Play(SE_SHIMMER);
  Event_Wait(30);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  Event_Wait(40);
  Actor_Get(ACTOR_HAMA)->update = NULL;
  Actor_Get(ACTOR_IVAN)->update = NULL;
  Actor_SetSpriteColorById(ACTOR_HAMA, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_IVAN, SPRITE_COLOR_NORMAL);
}

#include "shared/field_effect_duration_scripts.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The student who trains by jumping: three hops, each higher, spinning a full
/// turn in the third, then a second's rest.
s32 gFuchinTemple_JumpingStudentScript[] = {
    SC_ANIMATION(ANIM_STILL),
    SC_SET(SW_GRAVITY, toFIXED(3) / 5),
    SC_LABEL(1),
    SC_SET(SW_VELOCITY_Y, toFIXED(3)),
    SC_WAIT(30),
    SC_SET(SW_VELOCITY_Y, toFIXED(4)),
    SC_WAIT(30),
    SC_SET(SW_VELOCITY_Y, toFIXED(5)),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(2),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

/// The point Force strikes on the training tree: no shadow.
s32 gFuchinTemple_HiddenSpriteScript[] = {
    SC_CALL(FuchinTemple_ClearScriptedActorSpriteFlags),
    SC_WAIT(1),
    SC_END,
};

/// The doors' map cells, frame by frame: Fuchin Temple's door opening as Isaac
/// walks in (FuchinTemple_EnterTemple), and the door Feh and Feizhi come out of
/// in Master Feh's school, opening and closing behind them.
u16 gFuchinTemple_TempleDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(66, 46, 3, 2, 5),
    MAPGRID_ANIMATION_STEP(63, 46, 3, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};
u16 gFuchinTemple_FehsDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(62, 19, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(64, 19, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};
u16 gFuchinTemple_FehsDoorCloses[] = {
    MAPGRID_ANIMATION_STEP(62, 19, 2, 2, 5),
    MAPGRID_ANIMATION_STEP(60, 19, 2, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

struct SceneEntrance gFuchinTempleGroundsEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 296, 0, 488, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {FUCHIN_TEMPLE_GROUNDS_FROM_WORLD_MAP, CONDITION_ALWAYS, 296, 0, 504,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {FUCHIN_TEMPLE_GROUNDS_FROM_LACQUER_ROOMS_A, CONDITION_ALWAYS, 296, 0, 304,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS,
     168, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {FUCHIN_TEMPLE_GROUNDS_AFTER_DEFEAT, CONDITION_ALWAYS, 232, 0, 328,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gFuchinTempleEntrancesOther[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 552, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {LACQUER_ROOMS_A_FROM_FUCHIN_TEMPLE_GROUNDS, CONDITION_ALWAYS, 168, 0, 552,
     FACING_NORTH, 0, 48, 384, 288, 576, 0},
    {LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_XIAN, CONDITION_ALWAYS, 256, 0, 264,
     FACING_NORTH, 0, 48, 80, 368, 288, 0},
    {LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE, CONDITION_ALWAYS, 472, 0, 552,
     FACING_NORTH, 0, 352, 384, 592, 576, 0},
    {LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_HOUSE, CONDITION_ALWAYS, 304, 0, 248,
     FACING_SOUTH, 0, 48, 80, 368, 288, 0},
    {SCENE_TABLE_END},
};

u32 gFuchinTempleExits[] = {
    SCENE_EXITS(FuchinTempleGrounds),
    SCENE_EXIT(FUCHIN_TEMPLE_GROUNDS_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_FUCHIN_TEMPLE_GROUNDS),
    SCENE_EXIT(FUCHIN_TEMPLE_GROUNDS_TO_LACQUER_ROOMS_A, LacquerRoomsA,
               LACQUER_ROOMS_A_FROM_FUCHIN_TEMPLE_GROUNDS),
    SCENE_EXIT(FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE,
               FuchinFallsCaveEntrance,
               FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_TEMPLE_GROUNDS),
    SCENE_EXITS(LacquerRoomsA),
    SCENE_EXIT(LACQUER_ROOMS_A_TO_FUCHIN_TEMPLE_GROUNDS, FuchinTempleGrounds,
               FUCHIN_TEMPLE_GROUNDS_FROM_LACQUER_ROOMS_A),
    SCENE_EXIT(LACQUER_ROOMS_A_FEHS_SCHOOL_TO_XIAN, Xian,
               XIAN_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL),
    SCENE_EXIT(LACQUER_ROOMS_A_HAMAS_ROOM_TO_LAMA_TEMPLE, LamaTemple,
               LAMA_TEMPLE_FROM_LACQUER_ROOMS_A_HAMAS_ROOM),
    SCENE_EXIT(LACQUER_ROOMS_A_FEHS_SCHOOL_TO_HOUSE, LacquerRoomsB,
               LACQUER_ROOMS_B_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gFuchinTempleGroundsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        296, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MONK_IN_WHITE_AND_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        272, 0, 424, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        392, 0, 392, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gFuchinTempleActorPlacements[] = {
    // Nyunpa
    ACTOR_PLACEMENT(SPRITE_NYUNPA, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        168, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    // Master Feh's students
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        216, 0, 264, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_HEADBAND, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        120, 0, 200, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_TIED_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        152, 0, 152, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_TIED_HAIR, CONDITION_IN_VIEW, (s32)gFuchinTemple_JumpingStudentScript,
        120, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_TIED_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        176, 0, 168, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        168, 0, 264, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_HEADBAND, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        216, 0, 152, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_HEADBAND, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        176, 0, 216, FACING_SOUTH, TALK_FACING_PARTY, 0),
    // Feizhi and Master Feh, out of sight until they come out
    ACTOR_PLACEMENT(SPRITE_FEIZHI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FEH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    // The training tree, and where Force strikes it
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        232, 0, 168, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, (s32)gFuchinTemple_HiddenSpriteScript,
        232, 0, 168, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gFuchinTempleActorPlacementsFromLamaTemple[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    // Hama
    ACTOR_PLACEMENT(SPRITE_HAMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        472, 0, 472, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        440, 0, 536, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FEIZHI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct Trigger gFuchinTempleGroundsTriggers[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_FUCHIN_TEMPLE_SOUTH_ROAD, CONDITION_ALWAYS, FUCHIN_TEMPLE_GROUNDS_TO_WORLD_MAP },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FUCHIN_TEMPLE_DOOR, CONDITION_ALWAYS, EVENT(FuchinTemple_EnterTemple) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_GROTTO, CONDITION_ALWAYS, EVENT(FuchinTemple_EnterWaterfallGrotto) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FUCHIN_FALLS_MONK_ASKS, FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST, EVENT(FuchinTemple_MonkAsksWhetherTestIsDone) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_FROM_KOLIMA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleTheFuchinTempleIsAPleasant) },
    { TRIGGER_TALK, ACTOR_FALLS_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleOhIsaacWhatTookYouSo) },
    { TRIGGER_TALK, ACTOR_BOY_MONK_READING_OMENS, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleAhhhMasterNyunpaHeHasPassed) },
    { TRIGGER_TALK, ACTOR_WORRIED_BOY_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleMasterNyunpaHasWastedAwayAt) },
    { TRIGGER_TALK, ACTOR_ADMIRING_BOY_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), EVENT(FuchinTemple_BoyMonkSpeaksOfNyunpa) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_FROM_KOLIMA, CONDITION_ALWAYS, EVENT(FuchinTemple_YoungManFromKolimaAsksAboutBridge) },
    // Unused: a copy of the row above, which always answers first.
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_FROM_KOLIMA, CONDITION_ALWAYS, EVENT(FuchinTemple_YoungManFromKolimaAsksAboutBridge) },
    { TRIGGER_TALK, ACTOR_FALLS_MONK, FLAG_NYUNPA_TEST_ACCEPTED, MSGNO(MsgFuchinTempleOmOmBlueLightsInThe) },
    { TRIGGER_TALK, ACTOR_FALLS_MONK, FLAG_FUCHIN_FALLS_GROTTO_OPENED, EVENT(FuchinTemple_MonkLetsPartyIntoGrotto) },
    { TRIGGER_TALK, ACTOR_FALLS_MONK, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND, MSGNO(MsgFuchinTempleTheTestOfFuchinFallsIs) },
    { TRIGGER_TALK, ACTOR_FALLS_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterNyunpaIsWaitingForYou) },
    { TRIGGER_TALK, ACTOR_BOY_MONK_READING_OMENS, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleGemsFromTheSkyBlueLights) },
    { TRIGGER_TALK, ACTOR_WORRIED_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleNyunpaHasNotEatenInDays) },
    { TRIGGER_TALK, ACTOR_ADMIRING_BOY_MONK, CONDITION_ALWAYS, EVENT(FuchinTemple_BoyMonkSpeaksOfNyunpa) },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(FuchinTemple_TalkToHealer) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_FUCHIN_TEMPLE_PSYNERGY_STONE, FLAG_FUCHIN_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FROM_KOLIMA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleGemsFallingFromTheSkyMountains) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLS_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleIKnewMasterNyunpaWasFasting) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_MONK_READING_OMENS, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleMasterNyunpaReachedEnlightenmentDuringHis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_BOY_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleMasterNyunpaIsNothingButSkin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ADMIRING_BOY_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleHeCantLeaveUsYetHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FROM_KOLIMA, FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN, MSGNO(MsgFuchinTempleIBeginningToThinkIllBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FROM_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIDontFeelSafeGoingHome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLS_MONK, FLAG_NYUNPA_TEST_ACCEPTED, MSGNO(MsgFuchinTempleManyStarsFellFromTheSky) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLS_MONK, FLAG_FUCHIN_FALLS_GROTTO_OPENED, MSGNO(MsgFuchinTempleCanSuchYoungWarriorsEndureThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLS_MONK, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND, MSGNO(MsgFuchinTempleCanSuchYoungWarriorsEndureThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLS_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleTheWarriorsHaveMasteredTheSecrets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_MONK_READING_OMENS, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleThereIsNoPointInTraining) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleICantImagineFastingItGets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ADMIRING_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIWillTrainToBecomeA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleSinceImHereIdLikeTo) },
    { TRIGGER_TABLE_END },
};

struct Trigger gFuchinTempleTriggers[] = {
    { STEP_TRIGGER_FACING(FACING_SOUTH), CELL_BEHAVIOR_FUCHIN_TEMPLE_SOUTH_DOOR, CONDITION_ALWAYS, EVENT(FuchinTemple_LeaveTemple) },
    { TRIGGER_EXIT, CELL_BEHAVIOR_FEHS_SCHOOL_SOUTH_DOOR, CONDITION_ALWAYS, LACQUER_ROOMS_A_FEHS_SCHOOL_TO_XIAN },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FEHS_SCHOOL_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(FuchinTemple_OpenLockedDoor) },
    { TRIGGER_TALK, ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTemple) },
    { TRIGGER_TALK, ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TAUGHT_FORCE), MSGNO(MsgFuchinTempleUseThisEnergyToFindThe) },
    { TRIGGER_TALK, ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TEST_ACCEPTED), MSGNO(MsgFuchinTempleSpeakToTheMonkStandingBeneath) },
    { TRIGGER_TALK, ACTOR_NYUNPA, IF_FLAG_SET(FLAG_FUCHIN_TEMPLE_NYUNPA_TEST_DECLINED), EVENT(FuchinTemple_NyunpaOffersTest) },
    { TRIGGER_TALK, ACTOR_NYUNPA, CONDITION_ALWAYS, MSGNO(MsgXianNyunpaMeditates) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), EVENT(FuchinTemple_ReadNyunpasLastWish) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TAUGHT_FORCE), MSGNO(MsgFuchinTempleNowThatIHavePassedMy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_EVENT_PLAYS_CAST | ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TEST_ACCEPTED), EVENT(FuchinTemple_NyunpaFeelsMindRead) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_EVENT_PLAYS_CAST | ACTOR_NYUNPA, CONDITION_ALWAYS, EVENT(FuchinTemple_NyunpaOffersTest) },
    { TRIGGER_TALK, ACTOR_WELCOMING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleThisIsMasterFehsKungFu) },
    { TRIGGER_TALK, ACTOR_KICKING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleBouldersOvercameHsuKungFuShould) },
    { TRIGGER_TALK, ACTOR_PUNCHING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), EVENT(FuchinTemple_StudentAsksAboutAltin) },
    { TRIGGER_TALK, ACTOR_JUMPING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), EVENT(FuchinTemple_TalkToJumpingStudent) },
    { TRIGGER_TALK, ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleFeizhiPredictedHsusDangerHerProphecy) },
    { TRIGGER_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleHsuIsRestingFeizhiIsWatching) },
    { TRIGGER_TALK, ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTemplePracticeEveryoneChiHasItsLimits) },
    { TRIGGER_TALK, ACTOR_WELCOMING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleWarriorYouAreGreatYouHave) },
    { TRIGGER_TALK, ACTOR_KICKING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleEveryoneWantsToLearnTheChi) },
    { TRIGGER_TALK, ACTOR_PUNCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleConcentrateEnergyIntoTheHandsAnd) },
    { TRIGGER_TALK, ACTOR_JUMPING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), EVENT(FuchinTemple_TalkToJumpingStudent) },
    { TRIGGER_TALK, ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleIDidNotThinkMuchOf) },
    { TRIGGER_TALK, ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleWhereDidFeizhiGoIAm) },
    { TRIGGER_TALK, ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleMasterHsuIsLateICan) },
    { TRIGGER_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleIWillLearnChiIWill) },
    { TRIGGER_TALK, ACTOR_FEH, IF_FLAG_SET(FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorUseYourChiFromThe) },
    { TRIGGER_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorPleaseUseYourChiFrom) },
    { TRIGGER_TALK, ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorLetMeSeeYourChi) },
    { TRIGGER_TALK, ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleFeizhiIsGoneItIsDangerous) },
    { TRIGGER_TALK, ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleFeizhiIsStrongerThanSheLooks) },
    { TRIGGER_TALK, ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorShowUsYourChiLike) },
    { TRIGGER_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorThatWasGreatButMaster) },
    { TRIGGER_TALK, ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), EVENT(FuchinTemple_FehAsksToSeeChi) },
    { TRIGGER_TALK, ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleFeizhiIsGoneItIsDangerousOutside) },
    { TRIGGER_TALK, ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleFeizhiIsStrongerThanSheLooksShe) },
    { TRIGGER_TALK, ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleIKnowWhyYouCanUse) },
    { TRIGGER_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleWarriorChiIsAGreatPower) },
    { TRIGGER_TALK, ACTOR_WELCOMING_STUDENT, CONDITION_ALWAYS, EVENT(FuchinTemple_StudentWelcomesToSchool) },
    { TRIGGER_TALK, ACTOR_KICKING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleStrongKicksAreVeryImportantKick) },
    { TRIGGER_TALK, ACTOR_PUNCHING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIWantToLearnSoI) },
    { TRIGGER_TALK, ACTOR_JUMPING_STUDENT, CONDITION_ALWAYS, EVENT(FuchinTemple_TalkToJumpingStudent) },
    { TRIGGER_TALK, ACTOR_PROUD_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleEveryoneInXianCanDoMartial) },
    { TRIGGER_TALK, ACTOR_WATCHING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleJustListeningDoesNotMakeOne) },
    { TRIGGER_TALK, ACTOR_TREE_STUDENT, CONDITION_ALWAYS, EVENT(FuchinTemple_StudentAsksAboutFeh) },
    { TRIGGER_TALK, ACTOR_MESSENGER_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterFehIsVeryBusyHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTemplePeaceHasReturnedToXianBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KICKING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleNoMatterHowMuchYouPractice) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUNCHING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleTheMonstersAreVeryLargeStriking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JUMPING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleWhenABoulderFallsJumpRepeat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleWhereDidFeizhiGainThePower) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleMasterFehIsVeryWorriedAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleCanSuchAYoungWarriorReally) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleYourChiIsFuchinTemplesHow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KICKING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleIDontCareWhichChiIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUNCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleMasterHamasChiCanShatterBoulders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JUMPING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleFollowSilkRoadStraightPassThrough) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleWarriorYouUseWeaponsAndChi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleFeizhiIsNotInTheFeh) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleFeizhiWorriesOnlyAboutMasterHsu) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleLearningChiIsVeryHardMaster) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, IF_FLAG_SET(FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorTheRulesRequireYouTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleSoCloseWarriorUseChiFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorShowMeYourChiI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleFeizhiIKnowIKnowHow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleMyKickAndPunchAreVery) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleShowUsYourChiNowDo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleIAmShockedThatThisWarrior) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleIWantToSeeTheChi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleFeizhiIKnowHowYouFeel) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleMyKickAndPunchAreVeryLoud) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleWarriorShowMeYourChiNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleWarriorDoNotHideYourChi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleWarriorsFightWithWeaponsKungFu) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KICKING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTemplePracticingKicksHurtsMyFeetVery) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUNCHING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleThatPunchWasGoodYouSurprise) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JUMPING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIMoveTooQuicklyYouCant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PROUD_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleWarriorsCameToLearnTheFeh) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WATCHING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleGreatKickKickFromTheHip) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TREE_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleYouAreTheWarriorsWhoCame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSENGER_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterFehIsTalkingWithFeizhi) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_FEHS_SCHOOL_TREE_EAST_SOUTH, CONDITION_ALWAYS, EVENT(FuchinTemple_PushTreeByHand) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_FEHS_SCHOOL_TREE_EAST_SOUTH, CONDITION_ALWAYS, EVENT(FuchinTemple_PushTreeByHand) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_FEHS_SCHOOL_TREE_WEST_NORTH, CONDITION_ALWAYS, EVENT(FuchinTemple_PushTreeByHand) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_FEHS_SCHOOL_TREE_WEST_NORTH, CONDITION_ALWAYS, EVENT(FuchinTemple_PushTreeByHand) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TRAINING_TREE, CONDITION_ALWAYS, EVENT(FuchinTemple_MoveTree) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_TRAINING_TREE_TARGET, CONDITION_ALWAYS, EVENT(FuchinTemple_ForceTree) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_FEHS_SCHOOL_ANTIDOTE_BARREL, FLAG_FUCHIN_TEMPLE_ANTIDOTE_BARREL, FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_FUCHIN_TEMPLE_UNICORN_RING_JAR, FLAG_FUCHIN_TEMPLE_UNICORN_RING_JAR, FIND_ITEM(ITEM_UNICORN_RING) },
    { TRIGGER_TABLE_END },
};

struct Trigger gFuchinTempleTriggersFromLamaTemple[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_HAMAS_ROOM_SOUTH_DOOR, CONDITION_ALWAYS, LACQUER_ROOMS_A_HAMAS_ROOM_TO_LAMA_TEMPLE },
    { TRIGGER_TALK, ACTOR_HAMAS_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterHamaIsMeditatingAskingFor) },
    { TRIGGER_TALK, ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleTheFutureHoldsManyHardshipsBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMAS_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIDoNotThinkMasterHama) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleICannotSeeYourFutureClearly) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_HAMAS_ROOM_COINS_JAR, FLAG_HAMAS_ROOM_COINS_JAR, FIND_COINS(6) },
    { TRIGGER_TABLE_END },
};
// clang-format on

/// One of Nyunpa's crescents (FuchinTemple_SpawnNyunpasParticles): it rises
/// facing north for 45 frames, then is put away.
s32 gFuchinTemple_ShotScript[] = {
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(44),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};
