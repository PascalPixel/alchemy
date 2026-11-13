/// @file
/// Fuchin Temple's grounds, and the sheet of rooms that shares its overlay.
///
/// The game packs rooms of several places that share a tile style onto one
/// map. Here that is the inside of Fuchin Temple with two masters' rooms in
/// other areas, Master Feh's school in Xian and Master Hama's room in Lama
/// Temple.
///
/// Two scenes hold four areas, by their area names. In each, story flags
/// pick where the people stand and which event plays on entry.
///
/// - Fuchin Temple (SceneId_FuchinTempleGrounds), outside the temple, reached
///   from the World Map (FUCHIN_TEMPLE_GROUNDS_FROM_WORLD_MAP). A defeat
///   anywhere here returns the party to FUCHIN_TEMPLE_GROUNDS_AFTER_DEFEAT, in
///   front of the traveling healer.
///
///   The monk beneath the falls guards the waterfall grotto. Once Nyunpa has
///   set his test (FLAG_NYUNPA_TEST_ACCEPTED) he leaps aside
///   (FLAG_FUCHIN_FALLS_GROTTO_OPENED), and the grotto leads into Fuchin Falls
///   Cave (FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE).
///
///   Back out with the Orb of Force (FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND), he
///   sends the party on to Nyunpa (FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST).
///
/// - Lacquer rooms A (SceneId_LacquerRoomsA), a sheet of rooms drawn in red
///   lacquer. Each is a camera box of its own, chosen by the entrance the
///   party comes in by. In story order, by the exits that lead in:
///   - Fuchin Temple (by LACQUER_ROOMS_A_FROM_FUCHIN_TEMPLE_GROUNDS or the
///     default entrance), inside, where Nyunpa meditates.
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
///     - Force from the line fells the training tree
///       (FuchinTemple_ForceTree). The first time, Feizhi and Feh come out
///       and she leaves to find Hsu (FLAG_FEIZHI_WENT_TO_HSU), which unlocks
///       LACQUER_ROOMS_A_FEHS_SCHOOL_TO_HOUSE.
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
#include "main_routine_work.h"
#include "scene_ids.h"
#include "module/scene/fuchin_temple.h"
#include "module/scene/shared/field_effect_spawn.h"
#include "module/field/field_sequence.h"
#include "module/field/event_commands.h"
#include "module/system/system.h"
#include "actor.h"
#include "script_operands.h"
#include "types.h"
#include "event.h"
#include "system_call.h"
#include "scene_tables.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "flags.h"
#include "sounds.h"
#include "item_ids.h"
#include "actor_script.h"
#include "heap_slots.h"
#include "field_psynergy_work.h"
#include "palette_buffers.h"
#include "io_reg.h"

// Fuchin Temple's area temp flags, which last until the party enters
// another area; FLAG_FUCHIN_TEMPLE_PSYNERGY_STONE is the Psynergy Stone
// outside, which then refills.

/// The party refused Nyunpa's test, so talking to him offers it again.
/// ★ Named from the game: "Then I cannot tell you. Give up and go back from
/// whence you came." (MsgXianThenCannotTellGiveUp)
#define FLAG_FUCHIN_TEMPLE_NYUNPA_TEST_DECLINED FLAG_AREA_TEMP_00
/// Feh's students saw the training tree fall, but not to Chi from the line.
/// ★ Named from the game: "Look! The tree fell." (MsgXianLookTreeFell)
#define FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL FLAG_AREA_TEMP_01

/// The placed actors outside Fuchin Temple (FuchinTempleGrounds),
/// numbered in the order of gFuchinTempleGroundsActorPlacements.
enum FuchinTempleOutsideActorId {
  /// A young man from Kolima.
  /// ★ Named from the game: "How did you get here? Is the bridge open again?"
  /// (MsgXianHowDidGetHereBridge)
  ACTOR_YOUNG_MAN_FROM_KOLIMA = ACTOR_FIRST_PLACED,
  /// The monk beneath the falls, at the waterfall grotto.
  /// ★ Named from what the player sees: a bald monk in an orange robe standing
  /// on a stump beneath the waterfall (its render)
  ACTOR_FALLS_MONK,
  /// A boy monk who reads omens.
  /// ★ Named from the game: "Gems from the sky, blue lights in the north... All
  /// omens of disaster!" (MsgFuchinTempleGemsFromTheSkyBlueLights)
  ACTOR_BOY_MONK_READING_OMENS,
  /// A boy monk who fears for Nyunpa.
  /// ★ Named from the game: "Nyunpa has not eaten in days... He is going to
  /// waste away to nothing." (MsgFuchinTempleNyunpaHasNotEatenInDays)
  ACTOR_WORRIED_BOY_MONK,
  /// A boy monk who admires Nyunpa.
  /// ★ Named from the game: "Isn't it noble of him to try to save the world by
  /// becoming enlightened?" (MsgXianIsntNobleHimTrySave)
  ACTOR_ADMIRING_BOY_MONK,
  /// The traveling healer, a man in a tall hat and a long robe.
  /// ★ Named from the game: "I'm traveling around the world to spread good
  /// will." (MsgXianImTravelingAroundWorldSpread)
  ACTOR_HEALER
};

/// The placed actors inside Fuchin Temple and in Master Feh's school in
/// Xian (LacquerRoomsA), numbered in the order of
/// gFuchinTempleActorPlacements.
///
/// They are Nyunpa, Feh's students, Feizhi and Feh, and the training tree.
enum FuchinTempleInsideActorId {
  /// ★ Named from the game: "I have already heard of your test from Master
  /// Nyunpa." (MsgXianAlreadyHeardTest)
  ACTOR_NYUNPA = ACTOR_FIRST_PLACED,
  /// A young man, by the school's door.
  /// ★ Named from the game: "This is Master Feh's school... You came to watch?"
  /// (MsgXianMasterFehsSchoolCameWatch)
  ACTOR_WELCOMING_STUDENT,
  /// A student who practices kicks.
  /// ★ Named from the game: "Strong kicks are very important. Kick, kick,
  /// kick!" (MsgFuchinTempleStrongKicksAreVeryImportantKick)
  ACTOR_KICKING_STUDENT,
  /// A student who practices punches.
  /// ★ Named from the game: "I want to learn, so I train. Quick punches are
  /// very good." (MsgFuchinTempleIWantToLearnSoI)
  ACTOR_PUNCHING_STUDENT,
  /// ★ Named from the game: "Flexibility and jumping are very important in
  /// kung fu." (MsgXianFlexibilityJumpingVeryImportantIn)
  ACTOR_JUMPING_STUDENT,
  /// A student proud of Feh's school.
  /// ★ Named from the game: "Everyone in Xian can do martial arts. But the Feh
  /// School of martial arts is the most elite."
  /// (MsgFuchinTempleEveryoneInXianCanDoMartial)
  ACTOR_PROUD_STUDENT,
  /// A little girl, a student who learns by watching.
  /// ★ Named from the game: "Just listening does not make one strong. I learn
  /// by watching." (MsgFuchinTempleJustListeningDoesNotMakeOne)
  ACTOR_WATCHING_STUDENT,
  /// By the training tree, which he sets up again after it falls.
  /// ★ Named from the game: "Master Feh is very busy. Do you still wish to see
  /// him?" (MsgXianMasterFehVeryBusyDo)
  ACTOR_TREE_STUDENT,
  /// Keeps the party from Feh, and goes to Feh's door when the tree first falls
  /// from the line.
  /// ★ Named from the game: "Master Feh is very busy. He is speaking with his
  /// daughter, Feizhi. You cannot enter now."
  /// (MsgFuchinTempleMasterFehIsVeryBusyHe)
  ACTOR_MESSENGER_STUDENT,
  /// ★ Named from the game: "He is speaking with his daughter, Feizhi."
  /// (MsgFuchinTempleMasterFehIsVeryBusyHe)
  ACTOR_FEIZHI,
  /// ★ Named from the game: "Master Feh is talking with Feizhi"
  /// (MsgFuchinTempleMasterFehIsTalkingWithFeizhi)
  ACTOR_FEH,
  /// ★ Named from the game: "Look! The tree fell." (MsgXianLookTreeFell)
  ACTOR_TRAINING_TREE,
  /// Where Force strikes the tree.
  /// ★ Named from the game: "You cannot push it with your hands. Attack it with
  /// Chi." (MsgXianCannotPushHands)
  ACTOR_TRAINING_TREE_TARGET
};

/// The placed actors in Master Hama's room in Lama Temple (LacquerRoomsA by
/// LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE), numbered in the order of
/// gFuchinTempleActorPlacementsFromLamaTemple.
///
/// Garet, Ivan and Mia, placed first, keep their own slots.
enum HamasRoomActorId {
  /// ★ Named from the game: "Master Hama is meditating."
  /// (MsgXianMasterHamaMeditatingPleaseExtremely)
  ACTOR_HAMA = ACTOR_FIRST_PLACED,
  /// A boy monk in Hama's room.
  /// ★ Named from the game: "Master Hama is meditating. Please be extremely
  /// quiet." (MsgXianMasterHamaMeditatingPleaseExtremely)
  ACTOR_HAMAS_BOY_MONK,
  /// Bursts in to say that Hsu is in danger.
  /// ★ Named from the game: "Feizhi... Why are you here?"
  /// (MsgXianMasterHamaMeditatingPleaseExtremely)
  ACTOR_FEIZHI_IN_LAMA_TEMPLE
};

/// Animations of these scenes' own sprites beyond enum ActorAnimation.
enum {
  /// Seated cross-legged with his eyes shut (spr_051.png's frame 33).
  ANIM_NYUNPA_MEDITATE = 5,
  /// Seated as he meditated, shrunken and darkened, once he has died
  /// enlightened (spr_051.png's frame 34).
  ANIM_NYUNPA_WITHERED = 6,
  /// A drill of kicks, a knee raised, a kick and a leaping kick, from a
  /// fighting stance (spr_130.tsv).
  ANIM_STUDENT_KICKS = 5,
  /// A drill of quick punches with one kick among them (spr_131.tsv).
  ANIM_STUDENT_PUNCHES = 5
};

static void FuchinTemple_FeizhiLeavesForHsu(void);
static void FuchinTemple_FehSeesChi(void);
static UNUSED void FuchinTemple_HopLeaderByOffset(s32 offsetX, s32 offsetZ);
static void FuchinTemple_StartMindReadDemonstration(s32 caster, s32 target);
static void FuchinTemple_FinishMindReadDemonstration(void);
static void FuchinTemple_SpawnNyunpasParticles(void);
static void FuchinTemple_AnimateRevealLesson(void);

// Shared source, included in link order.
#include "shared/field_effect_create.h"
#include "shared/field_effect_spawn.h"

/// The Psynergy Stone outside the temple has been taken. It refills once the
/// party enters another area.
/// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
#define FLAG_FUCHIN_TEMPLE_PSYNERGY_STONE FLAG_AREA_TEMP_50

/// Clears the scripted actor's sprite flags and completes the script
/// command.
static s32
FuchinTemple_ClearScriptedActorSpriteFlags(struct Actor *interpreter) {
  Actor_SetSpriteFlags(interpreter, 0);
  return 0;
}

// Keep an actor facing the party leader while the conversation is
// staged.
// UB: update routines return void. The native produces an unused zero result;
// plain void changes five instructions in the English build.
#ifdef UBFIX
static void FuchinTemple_FaceLeader(struct Actor *updated) {
#else
static s32 FuchinTemple_FaceLeader(struct Actor *updated) {
#endif
  struct Actor *actor = (struct Actor *)updated;
  struct Actor *player = Actor_Get(ACTOR_PARTY_LEADER);
  actor->facing = ArcTan2(player->z.fixed - actor->z.fixed,
                          player->x.fixed - actor->x.fixed);
#ifndef UBFIX
  return 0;
#endif
}

/// Chooses the outside entrance table or the shared rooms'
/// entrances.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == (s32)&SceneId_FuchinTempleGrounds) {
    return gFuchinTempleGroundsEntrances;
  }
  return gFuchinTempleEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gFuchinTempleExits; }

/// Selects the room's actors and applies the post-lesson actor
/// placement changes.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == (s32)&SceneId_FuchinTempleGrounds) {
    return gFuchinTempleGroundsActorPlacements;
  }
  if (gMainRoutineWork.entrance ==
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
        .x = px(288);
    gFuchinTempleActorPlacements[ACTOR_MESSENGER_STUDENT - ACTOR_FIRST_PLACED]
        .z = px(248);
    gFuchinTempleActorPlacements[ACTOR_TRAINING_TREE - ACTOR_FIRST_PLACED]
        .condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
    gFuchinTempleActorPlacements[ACTOR_TRAINING_TREE_TARGET -
                                 ACTOR_FIRST_PLACED]
        .condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
  }
  Scene_AssignViewFlags(gFuchinTempleActorPlacements);
  return gFuchinTempleActorPlacements;
}

/// Talking to the young man from Kolima at Fuchin Temple
/// (FuchinTempleGrounds): he asks how the party got here.
///
/// "How did you get here? Is the bridge open again?"
///
/// Yes cheers him ("Then I can go home to Kolima now?") and is remembered. No
/// means the party came through the forest. Later talks go on to the tale of
/// the forest.
static void FuchinTemple_YoungManFromKolimaAsksAboutBridge(void) {
  Event_Start();
  Event_SetMessageId(MsgXianHowDidGetHereBridge);
  if (Event_GetFlag(FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN) != 0) {
    gFieldWork->message += 4;
  }
  Event_OpenMessage(ACTOR_YOUNG_MAN_FROM_KOLIMA, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    GameFlag_Set(FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN);
  } else {
    gFieldWork->message++;
  }
  Event_ShowMessage(ACTOR_YOUNG_MAN_FROM_KOLIMA, 0);
  Event_End();
}

/// Talking to the boy monk (ACTOR_ADMIRING_BOY_MONK) at Fuchin Temple
/// (FuchinTempleGrounds): he speaks of the one seeking enlightenment, with up
/// to two more questions.
///
/// "Isn't it noble of him to try to save the world by becoming enlightened?"
///
/// Once Nyunpa is gone, "Now, he truly is beyond the world's concerns. Is he
/// watching over us?"
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
      gFieldWork->message++;
      // "Do you know what you mean when you say that?"
      Event_OpenMessage(ACTOR_ADMIRING_BOY_MONK, 0);
      if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
        gFieldWork->message++;
      }
    }
    Event_ShowMessage(ACTOR_ADMIRING_BOY_MONK, 0);
    Event_End();
  }
}

/// Talking to the monk beneath the falls at Fuchin Temple (FuchinTempleGrounds)
/// once Nyunpa has set the test (FLAG_NYUNPA_TEST_ACCEPTED): he lets the party
/// pass (FLAG_FUCHIN_FALLS_GROTTO_OPENED).
///
/// "I have already heard of your test from Master Nyunpa. You may enter."
///
/// He leaps aside, and the way into the waterfall grotto opens.
static void FuchinTemple_MonkLetsPartyIntoGrotto(void) {
  struct Actor *monk;
  struct Actor *movingMonk;
  struct EffectOptions options;

  monk = Actor_Get(ACTOR_FALLS_MONK);
  Event_Start();
  Event_SetMessageId(MsgXianAlreadyHeardTest);
  // "I have already heard of your test from Master Nyunpa. You..."
  Event_ShowMessageThenWait(ACTOR_FALLS_MONK, 0, 20);
  // The monk leaps aside and lands in a puff of dust.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 392);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Sound_Play(SE_WATER);
  movingMonk = Actor_Get(ACTOR_FALLS_MONK);
  movingMonk->velocityY = px(20);
  movingMonk = Actor_Get(ACTOR_FALLS_MONK);
  movingMonk->gravity = px(4);
  Actor_SetSpeed(ACTOR_FALLS_MONK, 3);
  Actor_SetDestination(ACTOR_FALLS_MONK, 152, 392);
  Actor_WaitForMoveById(ACTOR_FALLS_MONK);
  movingMonk = Actor_Get(ACTOR_FALLS_MONK);
  movingMonk->gravity = px(1);
  Actor_FaceDirection(ACTOR_FALLS_MONK, FACING_EAST, 0);
  Sound_Play(SE_WATER);
  // Three white puffs, with only the colour option set.
  options.palette = SPRITE_COLOR_WHITE;
  FieldEffect_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + px(4)),
                    px(1) / 2, 0, 0, EFFECT_USE_PALETTE, &options);
  FieldEffect_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + px(4)), 0, 0,
                    0, EFFECT_USE_PALETTE, &options);
  FieldEffect_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + px(4)),
                    -px(1) / 2, 0, 0, EFFECT_USE_PALETTE, &options);
  Event_Wait(30);
  Scene_PlayQueuedMusic();
  MapGrid_CopyCellAttributes(10, 24, 1, 1, 10, 22);
  GameFlag_Set(FLAG_FUCHIN_FALLS_GROTTO_OPENED);
  Event_End();
}

/// Isaac comes back out of the grotto with the Orb of Force.
///
/// At Fuchin Temple (FuchinTempleGrounds, step trigger 5) the monk sends him
/// on to Nyunpa, and the way back is open again.
///
/// "Young master, did you complete the test?" Then "Master Nyunpa is waiting
/// for you... Proceed to the temple."
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

/// Reading Nyunpa's mind inside Fuchin Temple (LacquerRoomsA), or
/// talking to him after a refusal: he offers the test of the grotto.
///
/// "Mmmm... Who is that? Who speaks to my mind?"
///
/// He hears the party, reads their minds in turn, guesses that they are
/// following someone, warns of Mogall Forest and offers the test.
///
/// Accepting it (FLAG_NYUNPA_TEST_ACCEPTED), he sends Isaac to the monk beneath
/// the falls. Refusing is remembered
/// (FLAG_FUCHIN_TEMPLE_NYUNPA_TEST_DECLINED), and the next talk asks again.
static void FuchinTemple_NyunpaOffersTest(void) {
  s32 alreadyDeclinedTrial;
  struct Actor *nyunpa;

  Event_Start();
  // Revisit an earlier refusal, or introduce the offer through Mind Read.
  alreadyDeclinedTrial = GameFlag_Get(FLAG_FUCHIN_TEMPLE_NYUNPA_TEST_DECLINED);
  if (alreadyDeclinedTrial != 0) {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 504);
    Event_Wait(5);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Actor_Get(ACTOR_NYUNPA)->paused = 0;
    Sound_Play(SE_JUMP);
    nyunpa = Actor_Get(ACTOR_NYUNPA);
    nyunpa->velocityY = px(8);
    Actor_SetAnimation(ACTOR_NYUNPA, ANIM_STAND);
    Event_Wait(30);
    Event_SetMessageId(MsgXianMustSurviveTestGrottoBefore);
  } else {
    Event_SetMessageId(MsgXianMmmmWhoWhoSpeaksMy);
    FuchinTemple_StartMindReadDemonstration(ACTOR_PARTY_LEADER, ACTOR_NYUNPA);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    FuchinTemple_FinishMindReadDemonstration();
    Event_Wait(20);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 504);
    Event_Wait(5);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Sound_Play(SE_JUMP);
    Actor_Get(ACTOR_NYUNPA)->paused = alreadyDeclinedTrial;
    nyunpa = Actor_Get(ACTOR_NYUNPA);
    nyunpa->velocityY = px(8);
    Actor_SetAnimation(ACTOR_NYUNPA, ANIM_STAND);
    Event_Wait(30);
    Event_OpenMessage(ACTOR_NYUNPA, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
      Actor_RunRepeatedMotion(ACTOR_NYUNPA, 2);
      Event_Wait(20);
      Event_ShowMessage(ACTOR_NYUNPA, 0);
      Event_Wait(20);
      FuchinTemple_StartMindReadDemonstration(ACTOR_NYUNPA, ACTOR_PARTY_LEADER);
      Event_Wait(30);
      Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Event_Wait(50);
      FuchinTemple_FinishMindReadDemonstration();
      Event_Wait(30);
      Actor_SetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
      Event_ShowMessage(ACTOR_NYUNPA, 0);
    } else {
      gFieldWork->message += 2;
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
      Event_ShowMessage(ACTOR_NYUNPA, 0);
      gFieldWork->message += 1;
    } else {
      gFieldWork->message += 1;
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
      Event_Wait(20);
      // "I knew it!"
      Event_ShowMessage(ACTOR_NYUNPA, 0);
    }
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_NYUNPA, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_NYUNPA, FACING_NORTH, 20);
    Actor_SetSpeed(ACTOR_NYUNPA, 0.3);
    Actor_WalkToAndWait(ACTOR_NYUNPA, 168, 464);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_NYUNPA, FACING_SOUTH, 40);
    Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Actor_WalkToAndWait(ACTOR_NYUNPA, 168, 472);
  }
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
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    Actor_RunRepeatedMotion(ACTOR_NYUNPA, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(60);
    Event_ShowMessageThenWait(ACTOR_NYUNPA, 0, 10);
    GameFlag_Set(FLAG_NYUNPA_TEST_ACCEPTED);
  }
  Actor_SetAnimation(ACTOR_NYUNPA, ANIM_NYUNPA_MEDITATE);
  Event_End();
}

/// Reading Nyunpa's mind inside Fuchin Temple once the test is accepted: he
/// notices.
///
/// "You enjoy reading the minds of others, do you not?"
static void FuchinTemple_NyunpaFeelsMindRead(void) {
  Event_Start();
  Event_SetMessageId(MsgXianEnjoyReadingMindsOthersDo);
  // "You enjoy reading the minds of others, do you not?"
  // Yes: "If you overdo it, you will soon learn to despise the ability."
  // No: "That is good. Enjoy reading the minds of others while you..."
  Event_AskYesNo(ACTOR_NYUNPA, 0);
  Event_End();
}

/// Talking to the student at the door of Master Feh's school in Xian
/// (ACTOR_WELCOMING_STUDENT): the student asks whether the party came to
/// watch.
///
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

/// Talking to the student who practices jumping in Master Feh's school in Xian:
/// he lands, talks about jumping (or Lama Temple, or Hsu), then jumps on.
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

/// Talking to the student by the training tree in Master Feh's school in Xian
/// (ACTOR_TREE_STUDENT): the student asks whether the party still wishes to
/// see Master Feh.
///
/// "Master Feh is very busy. Do you still wish to see him?"
///
/// Yes: if the tree falls to Chi from the line, he will take the party to Feh.
static void FuchinTemple_StudentAsksAboutFeh(void) {
  Event_Start();
  Event_SetMessageId(MsgXianMasterFehVeryBusyDo);
  // "Master Feh is very busy. Do you still wish to see him?"
  // Yes: "Look there. You see that line? Stand there and use Chi. If..."
  // No: "You will not see Master Feh? That is too bad."
  Event_AskYesNo(ACTOR_TREE_STUDENT, 0);
  Event_End();
}

/// Rotates the training tree through its westward arc and scatters particles.
static void FuchinTemple_ToppleTreeWest(void) {
  struct Actor *tree = Actor_Get(ACTOR_TRAINING_TREE);
  u32 turnStep;
  s32 angle;

  for (turnStep = 8; turnStep > 3; turnStep--) {
    angle = turnStep << 12;
    tree->sprite->rotation = angle;
    wait((turnStep - 4) * 2);
    tree->x.fixed += Trig_Cos(angle) * 6;
    tree->z.fixed += Trig_Sin(angle) * 6;
  }

  tree->y.fixed = px(18);
  tree->targetY = px(18);

  Sound_Play(SE_TOPPLE);

  FieldEffect_Spawn(tree->x.fixed - px(12), tree->y.fixed,
                    tree->z.fixed + px(8), -px(1) / 5, px(2) / 5, 0, 0, 0);
  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + px(8),
                    -px(4) / 5, px(3) / 10, 0, 0, 0);
  FieldEffect_Spawn(tree->x.fixed + px(10), tree->y.fixed,
                    tree->z.fixed + px(8), -px(1), px(1) / 5, 0, 0, 0);
}

/// Rotates the training tree through the opposite arc and reverses its
/// horizontal scale.
static void FuchinTemple_ToppleTreeEast(void) {
  struct Actor *tree;
  u32 turnStep;
  s32 angle;

  tree = Actor_Get(ACTOR_TRAINING_TREE);
  for (turnStep = 8; turnStep <= 12; turnStep++) {
    angle = turnStep << 12;
    tree->sprite->rotation = angle;
    wait((12 - turnStep) * 2);
    tree->x.fixed -= Trig_Cos(angle) * 6;
    tree->z.fixed -= Trig_Sin(angle) * 6;
  }
  tree->y.fixed = px(18);
  tree->targetY = px(18);
  tree->scaleX = -4 * Q16_ONE / 5;
  Sound_Play(SE_TOPPLE);
  FieldEffect_Spawn(tree->x.fixed - px(12), tree->y.fixed,
                    tree->z.fixed + px(8), px(1), px(1) / 5, 0, 0, 0);
  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + px(8),
                    px(4) / 5, px(3) / 10, 0, 0, 0);
  FieldEffect_Spawn(tree->x.fixed + px(10), tree->y.fixed,
                    tree->z.fixed + px(8), px(1) / 5, px(2) / 5, 0, 0, 0);
}

/// Slides the training tree north in four steps, then scatters particles.
static void FuchinTemple_ToppleTreeNorth(void) {
  struct Actor *tree = Actor_Get(ACTOR_TRAINING_TREE);
  u32 stepIndex = 0;
  s32 delay = 8;
  u8 rotation;
  do {
    wait(delay);
    tree->z.fixed -= px(1);
    tree->targetZ = ACTOR_NO_TARGET;
    stepIndex++;
    delay -= 2;
  } while (stepIndex < 4);
  rotation = 0;
  tree->sprite->rotation = rotation;
  Sound_Play(SE_TOPPLE);
  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed - px(8),
                    -px(4) / 5, 0, -px(1) / 5, 0, 0);
  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed - px(8),
                    px(4) / 5, 0, -px(1) / 5, 0, 0);
  FieldEffect_Spawn(tree->x.fixed - px(6), tree->y.fixed,
                    tree->z.fixed + px(10), px(1) / 5, 0, -px(1), 0, 0);
  FieldEffect_Spawn(tree->x.fixed + px(6), tree->y.fixed,
                    tree->z.fixed + px(10), px(1) / 5, 0, -px(1), 0, 0);
}

/// Slides the training tree south in four steps before the final displacement.
static void FuchinTemple_ToppleTreeSouth(void) {
  struct Actor *tree;
  u32 stepIndex;
  s32 particleSpeed;
  s32 delay;

  tree = Actor_Get(ACTOR_TRAINING_TREE);
  stepIndex = 0;
  delay = 8;
  do {
    wait(delay);
    tree->z.fixed += px(1);
    tree->targetZ = ACTOR_NO_TARGET;
    stepIndex++;
    delay -= 2;
  } while (stepIndex < 4);

  tree->sprite->rotation = 0;
  tree->z.fixed += px(24);
  tree->targetZ = ACTOR_NO_TARGET;
  Sound_Play(SE_TOPPLE);

  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + px(12),
                    -px(4) / 5, 0, px(1) / 5, 0, 0);
  particleSpeed = px(1) / 5;
  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + px(12),
                    px(4) / 5, 0, particleSpeed, 0, 0);
  FieldEffect_Spawn(tree->x.fixed - px(6), tree->y.fixed, tree->z.fixed - px(8),
                    particleSpeed, 0, px(1), 0, 0);
  FieldEffect_Spawn(tree->x.fixed + px(6), tree->y.fixed, tree->z.fixed - px(8),
                    particleSpeed, 0, px(1), 0, 0);
}

/// Routes the student around the tree according to the leader's facing.
static void FuchinTemple_StudentWalksAroundTree(void) {
  u32 leaderFacing;
  u32 quadrantLimit;

  leaderFacing = Actor_Get(ACTOR_PARTY_LEADER)->facing;
  quadrantLimit = 0x3fff;
  if (((leaderFacing - FACING_SOUTHEAST) << 16) <= 0x3fff0000) {
    Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 216, 168);
    Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 224, 168);
    Actor_FaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTHEAST, 20);
  } else if ((u16)(leaderFacing - FACING_SOUTHWEST) <= quadrantLimit) {
    Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 232, 160);
    Actor_FaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTH_SOUTHWEST, 20);
  } else if ((u16)(leaderFacing - FACING_NORTHWEST) <= quadrantLimit) {
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
/// FuchinTempleChiRule, says.
///
/// Then he sets the tree up again.
static void FuchinTemple_StudentExplainsChiRules(s32 mode) {
  s32 rulesMessage;

  Actor_SetSpeed(ACTOR_TREE_STUDENT, 0.8);
  Event_Wait(60);
  rulesMessage = MsgXianCannotPushHands;
  Event_SetMessageId(rulesMessage);
  if (mode == FUCHIN_TEMPLE_CHI_NOT_FROM_LINE) {
    Event_SetMessageId(rulesMessage - 1);
    Actor_ShowEmote(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
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
  Event_ShowMessageThenWait(ACTOR_TREE_STUDENT, 0, 20);
  // Restore the tree and let the student return to the demonstration spot.
  FuchinTemple_StudentWalksAroundTree();
  Actor_RunRepeatedMotion(ACTOR_TREE_STUDENT, 3);
  Actor_SetPosition(ACTOR_TRAINING_TREE, 232, 168);
  Actor_SetPosition(ACTOR_TRAINING_TREE_TARGET, 232, 168);
  Actor_Get(ACTOR_TRAINING_TREE)->y.fixed = px(12);
  Actor_Get(ACTOR_TRAINING_TREE)->targetY = ACTOR_NO_TARGET;
  Actor_Get(ACTOR_TRAINING_TREE)->scaleX = 4 * Q16_ONE / 5;
  Actor_Get(ACTOR_TRAINING_TREE)->sprite->rotation = TRIG_TURN / 2;
  Sound_Play(SE_CHEST_OPEN);
  Event_Wait(40);
  Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 216, 152);
  Actor_FaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTHEAST, 30);
}

/// The students see the tree fall, Feh doubts it came from the line, and the
/// student sets the tree up again.
///
/// "Look! The tree fell."
static void FuchinTemple_StudentsSeeTreeFall(void) {
  struct Actor *tree;

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
  // Reset the tree after the group has reacted to the demonstration.
  Actor_SetSpeed(ACTOR_TREE_STUDENT, 0.8);
  FuchinTemple_StudentWalksAroundTree();
  Actor_RunRepeatedMotion(ACTOR_TREE_STUDENT, 3);
  Actor_SetPosition(ACTOR_TRAINING_TREE, 232, 168);
  Actor_SetPosition(ACTOR_TRAINING_TREE_TARGET, 232, 168);
  tree = Actor_Get(ACTOR_TRAINING_TREE);
  tree->y.fixed = px(12);
  tree = Actor_Get(ACTOR_TRAINING_TREE);
  tree->targetY = ACTOR_NO_TARGET;
  tree = Actor_Get(ACTOR_TRAINING_TREE);
  tree->scaleX = 4 * Q16_ONE / 5;
  tree = Actor_Get(ACTOR_TRAINING_TREE);
  {
    struct Sprite *sprite = tree->sprite;
    s32 rotation = TRIG_TURN / 2;

    sprite->rotation = rotation;
  }
  Sound_Play(SE_CHEST_OPEN);
  Event_Wait(40);
  Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 216, 152);
  Actor_FaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTH, 30);
  GameFlag_Set(FLAG_FUCHIN_TEMPLE_FEH_SCHOOL_SAW_TREE_FALL);
}

/// Move on the training tree in Master Feh's school in Xian: it topples, and
/// the students react.
///
/// It does not count as Chi.
static void FuchinTemple_MoveTree(void) {
  Event_Start();

  if (Actor_Get(ACTOR_PARTY_LEADER)->facing > FACING_SOUTH &&
      Actor_Get(ACTOR_PARTY_LEADER)->facing < FACING_NORTH) {
    FuchinTemple_ToppleTreeWest();
  } else {
    FuchinTemple_ToppleTreeEast();
  }

  if (GameFlag_Get(FLAG_FEH_WAITS_TO_SEE_CHI) != 0) {
    FuchinTemple_StudentsSeeTreeFall();
  } else {
    FuchinTemple_StudentExplainsChiRules(FUCHIN_TEMPLE_CHI_NOT_FROM_LINE);
  }

  Event_End();
}

/// Walking into the training tree in Master Feh's school in Xian: Isaac pushes
/// it over by hand.
///
/// "You cannot push it with your hands. Attack it with Chi."
static void FuchinTemple_PushTreeByHand(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  u16 facing;

  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Event_Wait(20);

  facing = leader->facing;

  if ((u16)(facing - FACING_SOUTHEAST) <= 0x3fffu) {
    FuchinTemple_ToppleTreeSouth();
  } else if ((u16)(facing - FACING_SOUTHWEST) <= 0x3fffu) {
    FuchinTemple_ToppleTreeWest();
  } else if ((u16)(facing - FACING_NORTHWEST) <= 0x3fffu) {
    FuchinTemple_ToppleTreeNorth();
  } else {
    FuchinTemple_ToppleTreeEast();
  }

  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FuchinTemple_StudentExplainsChiRules(FUCHIN_TEMPLE_CHI_PUSHED_BY_HAND);
  Event_End();
}

/// Force on the training tree in Master Feh's school in Xian: it topples.
///
/// From too close it does not count. From the line, the students call Feh out
/// the first time, and later Feh himself sees it.
static void FuchinTemple_ForceTree(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  u16 facing;

  Event_Start();

  facing = leader->facing;
  if ((u16)(facing - FACING_SOUTHEAST) <= 0x3fff) {
    FuchinTemple_ToppleTreeSouth();
  } else if ((u16)(facing - FACING_SOUTHWEST) <= 0x3fff) {
    FuchinTemple_ToppleTreeWest();
  } else if ((u16)(facing - FACING_NORTHWEST) <= 0x3fff) {
    FuchinTemple_ToppleTreeNorth();
  } else {
    FuchinTemple_ToppleTreeEast();
  }

  FieldCamera_SetSpeed(1);
  FieldCamera_MoveToActorById(ACTOR_TRAINING_TREE_TARGET, 1);
  FieldCamera_WaitForArrival();

  if (leader->z.part.pixel <= 209) {
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

/// The first time the tree falls from the line, a student runs for Master Feh.
///
/// "What did you do, warrior?"
///
/// The door opens on its own and Feizhi storms out, sure that Hsu is in
/// danger. Feh will not believe her visions, and she leaves alone to find Hsu.
///
/// Feh then asks Isaac to show him his Chi.
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
  // Bring the observers out through the school door.
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
  actor = Actor_Get(ACTOR_MESSENGER_STUDENT);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
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
  Actor_SetPosition(ACTOR_TRAINING_TREE, 232, 168);
  Actor_SetPosition(ACTOR_TRAINING_TREE_TARGET, 232, 168);
  // Restore the tree before leaving the group ready for another demonstration.
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  actor->y.fixed = px(12);
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  actor->targetY = ACTOR_NO_TARGET;
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  actor->scaleX = 4 * Q16_ONE / 5;
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  {
    struct Sprite *sprite = actor->sprite;
    s32 rotation = TRIG_TURN / 2;

    sprite->rotation = rotation;
  }
  Sound_Play(SE_CHEST_OPEN);
  Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Actor_WalkToAndWait(ACTOR_MESSENGER_STUDENT, 256, 240);
  Actor_FaceDirection(ACTOR_MESSENGER_STUDENT, FACING_NORTH_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 1);
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
  Event_OpenMessage(ACTOR_FEH, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_RunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 1);
    Event_Wait(20);
    Event_ShowMessageThenWait(ACTOR_MESSENGER_STUDENT, 0, 20);
    GameFlag_Set(FLAG_FEH_WAITS_TO_SEE_CHI);
  } else {
    Event_SkipMessages(1);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Actor_FaceDirection(ACTOR_FEH, FACING_SOUTH, 20);
    Actor_RunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
    Event_Wait(20);
    Event_OpenMessage(ACTOR_MESSENGER_STUDENT, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Actor_SetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
      Event_Wait(20);
      Actor_FaceDirection(ACTOR_FEH, FACING_NORTH_NORTHWEST, 20);
      Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
      GameFlag_Set(FLAG_FEH_WAITS_TO_SEE_CHI);
    } else {
      Event_SkipMessages(1);
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
      Event_Wait(20);
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

/// Talking to Feh in his school in Xian after the party declined: he asks again
/// to see Isaac's Chi.
///
/// "Warrior, will you show me your Chi?"
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
    gFieldWork->message += 1;
    // "What you can do is an incredible thing. Hiding it is not good."
    Event_ShowMessageThenWait(ACTOR_FEH, 0, 20);
    Event_End();
  }
}

/// The tree falls from the line with Feh watching, but Feh sees that it is not
/// Chi.
///
/// "Ya! The tree fell!"
///
/// He asks whether the party has been to Fuchin Temple, and the school goes
/// back to its training.
static void FuchinTemple_FehSeesChi(void) {
  struct Actor *actor;

  // Stop the temporary facing update routines while the group reacts to the
  // tree.
  actor = Actor_Get(ACTOR_FEH);
  actor->update = 0;
  actor = Actor_Get(ACTOR_PROUD_STUDENT);
  actor->update = 0;
  actor = Actor_Get(ACTOR_WATCHING_STUDENT);
  actor->update = 0;
  actor = Actor_Get(ACTOR_TREE_STUDENT);
  actor->update = 0;
  actor = Actor_Get(ACTOR_MESSENGER_STUDENT);
  actor->update = 0;
  Actor_SetAnimation(ACTOR_PUNCHING_STUDENT, ANIM_STAND);
  FieldCamera_SetSpeed(0.5);
  FieldCamera_MoveTo(232, CAMERA_KEEP, 200, 1);
  FieldCamera_WaitForArrival();
  // Continue the Chi discussion with the students gathered around the tree.
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
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  actor->y.fixed = px(12);
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  actor->targetY = ACTOR_NO_TARGET;
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  {
    struct Sprite *sprite = actor->sprite;
    s32 rotation = TRIG_TURN / 2;

    sprite->rotation = rotation;
  }
  Sound_Play(SE_CHEST_OPEN);
  Event_Wait(40);
  Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 216, 152);
  Actor_FaceDirection(ACTOR_TREE_STUDENT, FACING_SOUTH, 30);
  GameFlag_Clear(FLAG_FEH_WAITS_TO_SEE_CHI);
  GameFlag_Set(FLAG_FEH_SAW_KI);
}

/// Talking to a student (ACTOR_PUNCHING_STUDENT) in Master Feh's school in
/// Xian once Hsu is known to be trapped: the student asks about the flood.
///
/// "Water monsters flooded Altin. Did you defeat them, warrior?"
static void FuchinTemple_StudentAsksAboutAltin(void) {
  Event_Start();
  Event_SetMessageId(MsgXianWaterMonstersFloodedAltinDid);
  // "Water monsters flooded Altin. Did you defeat them, warrior?"
  // Yes: "You are much stronger than you look, warrior. I have much..."
  // No: "Oh... You are not the strong warrior Feizhi spoke of?"
  Event_AskYesNo(ACTOR_PUNCHING_STUDENT, 0);
  Event_End();
}

/// Walking north into the door Feh and Feizhi came out of, in Master Feh's
/// school in Xian (step trigger 3): it is locked until Feizhi has left.
///
/// "It is locked."
///
/// After that it opens and Isaac walks through
/// (LACQUER_ROOMS_A_FEHS_SCHOOL_TO_HOUSE).
static void FuchinTemple_OpenLockedDoor(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_FEIZHI_WENT_TO_HSU) == 0 &&
      GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
    FieldMessage_Show(MsgXianItIsLocked, 1);
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

/// Talking to the healer at Fuchin Temple (FuchinTempleGrounds): he heals, or
/// speaks of his travels.
///
/// Across the counter from the south, he offers the Sanctum's services. From
/// elsewhere, he has a word on the healer's travels.
///
/// "I'm traveling around the world to spread good will."
static void FuchinTemple_TalkToHealer(void) {
  struct Actor *leader;
  u32 facing;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  facing = leader->facing;
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

/// Reading Nyunpa's mind inside Fuchin Temple once he is enlightened: he
/// entrusts the world to Isaac.
///
/// "Isaac... Is that you...? I am... counting on you... to bring peace... to
/// the world..."
static void FuchinTemple_ReadNyunpasLastWish(void) {
  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_NYUNPA, 2);
  Event_SetMessageId(MsgXianIsaacAmCountingOnBring);
  // "Isaac... Is that you...? I am...counting on you... to bring..."
  Event_ShowMessage(ACTOR_NYUNPA, 0);
  Event_End();
}

/// Selects the outdoor, school, or lesson trigger table.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == (s32)&SceneId_FuchinTempleGrounds) {
    return gFuchinTempleGroundsTriggers;
  }
  if (gMainRoutineWork.entrance ==
      LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE) {
    return gFuchinTempleTriggersFromLamaTemple;
  }
  return gFuchinTempleTriggers;
}

/// Emits a falling drop of spray beside the leader during the grotto entrance
/// fade, and flickers the leader as he walks in behind the falls.
///
/// A drop is white or, one drop in seven, has its colours rotated. The leader
/// is hidden and has red and green swapped on alternate frames.
static void FuchinTemple_SpawnGrottoEntryParticle(void) {
  struct EffectOptions options;
  struct Actor *leader;
  s32 paletteDraw;
  s32 fallSpeed;

  leader = Actor_Get(ACTOR_PARTY_LEADER);

  options.palette = SPRITE_COLOR_WHITE;
  paletteDraw = (u32)(Import_RandomNext() * 7) >> 16;
  if ((paletteDraw & 7) == 0)
    options.palette = SPRITE_COLOR_BRG;

  options.startScaleX = 7 * Q16_ONE / 10;
  options.startScaleY = 4 * Q16_ONE / 5;

  fallSpeed = ((u32)(Import_RandomNext() * 8) >> 16) * 13107;

  FieldEffect_Spawn(leader->x.fixed + px(8 - (gFrameCount & 15)),
                    leader->y.fixed + (192 << 13), leader->z.fixed, 0,
                    -fallSpeed, 0, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                    &options);

  if ((gFrameCount & 1) != 0)
    Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  else
    Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_GRB);
}

/// Unused: hops the leader one map cell north; nothing calls it.
static UNUSED void FuchinTemple_HopLeaderNorth(void) {
  FuchinTemple_HopLeaderByOffset(0, -32);
}

/// Unused: hops the leader one map cell south; nothing calls it.
static UNUSED void FuchinTemple_HopLeaderSouth(void) {
  FuchinTemple_HopLeaderByOffset(0, 32);
}

/// Unused: hops the leader one map cell west; nothing calls it.
static UNUSED void FuchinTemple_HopLeaderWest(void) {
  FuchinTemple_HopLeaderByOffset(-32, 0);
}

/// Unused: hops the leader by a pixel offset, waiting for the movement to
/// finish.
///
/// Only the three unused hops above call it.
static UNUSED void FuchinTemple_HopLeaderByOffset(s32 offsetX, s32 offsetZ) {
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2.5);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, offsetX, offsetZ);
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAP);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
  Event_End();
}

/// Stepping into the waterfall grotto's entrance at Fuchin Temple
/// (FuchinTempleGrounds, step trigger 3): spray falls over Isaac as the screen
/// fades, and he enters Fuchin Falls Cave.
static void FuchinTemple_EnterWaterfallGrotto(void) {
  struct Actor *leader;
  TaskFunc particleTask;

  Event_Start();
  // Keep the entry effect alive until the leader has moved into the fade.
  particleTask = FuchinTemple_SpawnGrottoEntryParticle;
  Task_AddFrame(particleTask, TASK_PASS_FRAME);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.2);
  gFieldWork->transitionFrames = 60;
  Event_FinishScreenTransition();
  Sound_Play(SE_CAST);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(leader, 0);
  Task_Remove(particleTask);
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE);
  Event_End();
}

/// Stepping south onto step trigger 1 inside Fuchin Temple (LacquerRoomsA):
/// the party leaves by LACQUER_ROOMS_A_TO_FUCHIN_TEMPLE_GROUNDS, out to
/// FuchinTempleGrounds.
static void FuchinTemple_LeaveTemple(void) {
  Sound_Play(SE_MAP_EXIT);
  Import_EventSetExitRequest(LACQUER_ROOMS_A_TO_FUCHIN_TEMPLE_GROUNDS);
}

/// Walking north into the temple door at Fuchin Temple (FuchinTempleGrounds,
/// step trigger 2): it opens, and Isaac walks inside, where Nyunpa meditates.
static void FuchinTemple_EnterTemple(void) {
  Event_Start();
  Sound_Play(SE_THUMP);
  MapGrid_AnimateCells(gFuchinTemple_TempleDoorOpens, 77, 8);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  Event_Wait(16);
  Import_EventSetExitRequest(FUCHIN_TEMPLE_GROUNDS_TO_LACQUER_ROOMS_A);
  Event_End();
}

/// Back inside Fuchin Temple from the grotto with the Orb of Force
/// (FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND), Nyunpa praises Isaac.
///
/// "Excellent, Isaac! I am glad to see you master the secrets of Fuchin
/// Temple!"
///
/// Nyunpa names the power Ki, "force" in the western world, and says it will
/// show the monsters hiding in Mogall Forest.
static void FuchinTemple_NyunpaTeachesKi(void) {
  struct Actor *nyunpa;

  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 168, 504);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
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
  nyunpa = Actor_Get(ACTOR_NYUNPA);
  nyunpa->velocityY = px(8);
  Actor_SetAnimation(ACTOR_NYUNPA, ANIM_STAND);
  // Praise the completed test before answering the follow-up questions.
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
    gFieldWork->message += 2;
  } else {
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_NYUNPA, 2);
    Event_Wait(20);
    gFieldWork->message++;
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
  // Explain why hidden monsters make the new ability useful.
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
    gFieldWork->message++;
  } else {
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_NYUNPA, 2);
    gFieldWork->message++;
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

/// Entering Master Hama's room in Lama Temple the first time, the party is
/// greeted.
///
/// "So, Isaac, you and your Adept companions have finally come."
///
/// - Hama tells Ivan he is a Jupiter Adept like her and means to teach him
///   Reveal for crossing the Lamakan Desert.
/// - Feizhi bursts in: Hsu is in great danger among the boulders in the
///   mountains.
/// - Hama gives Ivan Reveal and hurries with her to his aid.
/// - The party is left to choose between helping Hsu and following the people
///   it chases (FLAG_XIAN_HEARD_HSU_TRAPPED).
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
  // Bring the other party members out from the leader for the lesson.
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPositionFixed(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPositionFixed(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPositionFixed(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
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
  // Interrupt the discussion with the arrival from the southern path.
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
  // Finish Ivan's lesson before adding Reveal to his ability list.
  FuchinTemple_AnimateRevealLesson();
  Event_Wait(60);
  Party_LearnAbility(IVAN, ABILITY_REVEAL);
  // Let Hama and her monk leave, then gather the party into the leader.
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
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
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetSpeed(ACTOR_HAMA, 0.5);
  Actor_WalkToAndWait(ACTOR_HAMA, 480, 540);
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
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  Event_AskYesNo(ACTOR_GARET, 0);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_OpenMessage(ACTOR_MIA, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    gFieldWork->message++;
  } else {
    Event_Wait(20);
    gFieldWork->message++;
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  }
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
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
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_WalkToAndWait(ACTOR_IVAN, 448, 512);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Event_End();
  GameFlag_Set(FLAG_XIAN_HEARD_HSU_TRAPPED);
}

/// Sets each room up as the story left it.
///
/// - Inside Fuchin Temple, Nyunpa meditates, or teaches Ki after the test.
/// - In Master Feh's school in Xian the tree and students stand as the story
///   left them.
/// - In Lama Temple, Hama teaches Reveal on the first visit and is away until
///   Hsu is rescued.
/// - At Fuchin Temple (FuchinTempleGrounds) the monk beneath the falls stands
///   where the story left him.
s32 Scene_Initialize(void) {
  struct Actor *trainingTree;
  s32 heardHsuTrapped;

  if (gMainRoutineWork.next == (s32)&SceneId_LacquerRoomsA) {
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    if (gMainRoutineWork.entrance ==
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
    } else if (gMainRoutineWork.entrance ==
                   LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_XIAN ||
               gMainRoutineWork.entrance ==
                   LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_HOUSE) {
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
      heardHsuTrapped = GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED);
      if (heardHsuTrapped == 0) {
        trainingTree = Actor_Get(ACTOR_TRAINING_TREE);
        trainingTree->motionFlags = heardHsuTrapped;
        trainingTree->y.fixed = px(12);
        trainingTree->targetY = px(12);
        trainingTree->scaleX = 4 * Q16_ONE / 5;
        trainingTree->scaleY = Q16_ONE / 2;
        trainingTree->sprite->rotation = TRIG_TURN / 2;
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
        trainingTree->y.fixed = px(12);
        trainingTree->targetY = px(12);
        trainingTree->scaleX = 4 * Q16_ONE / 5;
        trainingTree->scaleY = Q16_ONE / 2;
        trainingTree->collisionFlags |= ACTOR_NOT_TARGETED;
        trainingTree->sprite->rotation = TRIG_TURN / 2;
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
    } else if (gMainRoutineWork.entrance ==
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
    if (gMainRoutineWork.entrance ==
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
///
/// Nyunpa's Mind Read triggers carry PSYNERGY_EVENT_PLAYS_CAST, so the field
/// plays nothing of its own.
static void FuchinTemple_StartMindReadDemonstration(s32 caster, s32 target) {
  struct FieldEffectWork *effectWork;

  GameFlag_Set(FLAG_PLAY_PSYNERGY_CAST);
  FieldPsynergy_LoadResources(ABILITY_MIND_READ, 1);
  effectWork = gFieldEffectWork;
  FieldPsynergy_SetupActorPair(caster, target);
  effectWork->active = 0;
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

/// Emits eight rising particles from Nyunpa, sounding every other shot.
static void FuchinTemple_SpawnNyunpasParticles(void) {
  struct EffectOptions options;
  struct Actor *nyunpa;
  u32 shotIndex;

  nyunpa = Actor_Get(ACTOR_NYUNPA);
  options.priority = 1;
  options.type = SPRITE_CRESCENT;
  options.script = gFuchinTemple_ShotScript;
  options.targetScaleX = 224 << 10;
  options.targetScaleY = 192 << 9;
  for (shotIndex = 0; shotIndex < 8; shotIndex++) {
    Event_Wait(10);
    if (shotIndex & 1) {
      Sound_Play(SE_PSYNERGY_START);
    }
    FieldEffect_Spawn(nyunpa->x.fixed, nyunpa->y.fixed,
                      nyunpa->z.fixed - px(24), 0, px(3) / 5, 0,
                      EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                          EFFECT_SCALE_TO_TARGET | EFFECT_USE_PRIORITY |
                          EFFECT_SCRIPT_MEDIUM,
                      &options);
  }
  Event_Wait(60);
}

/// Hama's and Ivan's update routine during the Reveal lesson: they glow, drawn
/// red and blue in turn every two frames.
static void FuchinTemple_AlternateLessonChildEffect(struct Actor *updated) {
  if ((*(u32 *)&gFrameCount >> 1) & 1) {
    Actor_SetSpriteColor((struct Actor *)updated, SPRITE_COLOR_RED_TINT);
  } else {
    Actor_SetSpriteColor((struct Actor *)updated, SPRITE_COLOR_BLUE_TINT);
  }
}

/// Animates Hama and Ivan's lesson effects, fades the palette and clears the
/// update routines.
static void FuchinTemple_AnimateRevealLesson(void) {
  struct EffectOptions options;
  struct Actor *hama;
  u32 particleIndex;
  s32 particleX;
  s32 particleY;
  s32 riseSpeed;

  Sound_Play(SE_PSYNERGY_SPARKLE);
  Actor_Get(ACTOR_HAMA)->update = FuchinTemple_AlternateLessonChildEffect;
  Event_Wait(40);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(20, 2, 23), PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  Event_Wait(40);
  Sound_Play(SE_PSYNERGY_SPARKLE);
  Actor_Get(ACTOR_IVAN)->update = FuchinTemple_AlternateLessonChildEffect;
  Event_Wait(120);
  hama = Actor_Get(ACTOR_HAMA);
  // Emit the lesson particles while both temporary child effects run.
  options.priority = 1;
  options.palette = SPRITE_COLOR_RBG;
  options.type = SPRITE_SPARKLE;
  for (particleIndex = 0; particleIndex < 64; particleIndex++) {
    if ((particleIndex & 3) == 0) {
      Sound_Play(SE_GLITTER);
    }
    particleX = hama->x.fixed +
                ((((u32)(Import_RandomNext() * 3) << 4) >> 16) << 16) - px(12);
    particleY = hama->y.fixed +
                ((((u32)Import_RandomNext() << 5) >> 16) << 16) - px(16);
    riseSpeed = ((((u32)Import_RandomNext() << 2) >> 16) << 15) + (128 << 8);
    FieldEffect_Spawn(
        particleX, particleY, hama->z.fixed, 0, riseSpeed, 0,
        EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
    wait(2);
  }
  Sound_Play(SE_SHIMMER);
  Event_Wait(30);
  // Fade back and remove both update routines before the conversation resumes.
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  Event_Wait(40);
  Actor_Get(ACTOR_HAMA)->update = NULL;
  Actor_Get(ACTOR_IVAN)->update = NULL;
  Actor_SetSpriteColorById(ACTOR_HAMA, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteColorById(ACTOR_IVAN, SPRITE_COLOR_NORMAL);
}

#include "shared/field_effect_duration_scripts.h"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gFuchinTemple_JumpingStudentScript[] = {
    SC_ANIMATION(0),
    SC_SET(SW_GRAVITY, px(3) / 5),
    SC_LABEL(1),
    SC_SET(SW_VELOCITY_Y, px(3)),
    SC_WAIT(30),
    SC_SET(SW_VELOCITY_Y, px(4)),
    SC_WAIT(30),
    SC_SET(SW_VELOCITY_Y, px(5)),
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

s32 gFuchinTemple_HiddenSpriteScript[] = {
    SC_CALL(FuchinTemple_ClearScriptedActorSpriteFlags),
    SC_WAIT(1),
    SC_END,
};

// The doors' map cells, frame by frame: Fuchin Temple's door opening as Isaac
// walks in (FuchinTemple_EnterTemple), and the door Feh and Feizhi come out of
// in Master Feh's school, opening and closing behind them.
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
    {0, CONDITION_ALWAYS, 296, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0},
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
    {0, CONDITION_ALWAYS, 168, 0, 552, FACING_NORTH, 0, -1, -1, -1, -1, 0},
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
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
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
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, FUCHIN_TEMPLE_GROUNDS_TO_WORLD_MAP },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(FuchinTemple_EnterTemple) },
    { TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(FuchinTemple_EnterWaterfallGrotto) },
    { TRIGGER_STEP, 5, FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST, EVENT(FuchinTemple_MonkAsksWhetherTestIsDone) },
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
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 110, FLAG_FUCHIN_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
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
    { STEP_TRIGGER_FACING(FACING_SOUTH), 1, CONDITION_ALWAYS, EVENT(FuchinTemple_LeaveTemple) },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, LACQUER_ROOMS_A_FEHS_SCHOOL_TO_XIAN },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT(FuchinTemple_OpenLockedDoor) },
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
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT(FuchinTemple_PushTreeByHand) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(FuchinTemple_PushTreeByHand) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT(FuchinTemple_PushTreeByHand) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT(FuchinTemple_PushTreeByHand) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TRAINING_TREE, CONDITION_ALWAYS, EVENT(FuchinTemple_MoveTree) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_TRAINING_TREE_TARGET, CONDITION_ALWAYS, EVENT(FuchinTemple_ForceTree) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 100, FLAG_FUCHIN_TEMPLE_ANTIDOTE_BARREL, FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 101, FLAG_FUCHIN_TEMPLE_UNICORN_RING_JAR, FIND_ITEM(ITEM_UNICORN_RING) },
    { TRIGGER_TABLE_END },
};

struct Trigger gFuchinTempleTriggersFromLamaTemple[] = {
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, LACQUER_ROOMS_A_HAMAS_ROOM_TO_LAMA_TEMPLE },
    { TRIGGER_TALK, ACTOR_HAMAS_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterHamaIsMeditatingAskingFor) },
    { TRIGGER_TALK, ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleTheFutureHoldsManyHardshipsBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMAS_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIDoNotThinkMasterHama) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleICannotSeeYourFutureClearly) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), 102, FLAG_HAMAS_ROOM_COINS_JAR, FIND_COINS(6) },
    { TRIGGER_TABLE_END },
};
// clang-format on

s32 gFuchinTemple_ShotScript[] = {
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(44),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};
