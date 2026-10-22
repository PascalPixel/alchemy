/// @file
/// Fuchin Temple, and two masters' rooms in other places that share its
/// overlay: Master Feh's school in Xian and the room in Lama Temple where
/// Master Hama meditates. Two scenes hold four places, by their area names:
///
/// - Fuchin Temple (SceneId_FuchinTempleGrounds), outside the temple, reached
///   from the world map (FUCHIN_TEMPLE_GROUNDS_FROM_WORLD_MAP). Its monks read
///   omens in the gems that fell from the sky and the blue lights in the north,
///   and tell of Master Nyunpa, who is fasting to save the world by becoming
///   enlightened. A young man from Kolima asks whether the bridge is open again
///   (FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN), a traveling healer offers
///   the Sanctum's services, and a search finds a Psynergy Stone. The monk
///   beneath the falls guards the waterfall grotto: once Nyunpa has set the
///   test (FLAG_NYUNPA_TEST_ACCEPTED), he leaps aside
///   (FLAG_FUCHIN_FALLS_GROTTO_OPENED) and the party enters Fuchin Falls Cave
///   (FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE). Coming back out
///   with the Orb of Force (FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND), Isaac is
///   sent on to Nyunpa (FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST). The temple
///   door leads inside (FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_TEMPLE_INTERIORS).
/// - Fuchin Temple (SceneId_FuchinTempleInteriors, by
///   FUCHIN_TEMPLE_INTERIORS_FROM_FUCHIN_TEMPLE_GROUNDS or the default
///   entrance), inside, where Nyunpa meditates and does not answer. When the
///   party reads his mind (Event_NyunpaOffersTest), he hears them, reads their
///   minds in turn, guesses that they are following someone, warns them that no
///   ordinary man can cross Mogall Forest and offers the test of the grotto;
///   after a refusal (FLAG_NYUNPA_TEST_DECLINED), talking to him asks again.
///   Back from the grotto with the Orb, Scene_Initialize plays his lesson
///   (FuchinTemple_NyunpaTeachesKi): the power the monks call Ki, "force" in
///   the western world, will make the monsters hiding in the forest show
///   themselves (FLAG_NYUNPA_TAUGHT_FORCE). Once the party has been to Mogall
///   Forest, Nyunpa has died, enlightened (FLAG_NYUNPA_ENLIGHTENED, set there):
///   he no longer answers, the monks grieve, and Mind Read hears his last wish,
///   that Isaac bring peace to the world.
/// - Xian (FuchinTempleInteriors, by FUCHIN_TEMPLE_INTERIORS_FROM_XIAN and
///   FUCHIN_TEMPLE_INTERIORS_FROM_HOUSE), Master Feh's kung fu school; the
///   area-name rules give the name Xian only at
///   FUCHIN_TEMPLE_INTERIORS_FROM_XIAN, and Scene_Initialize hides the name at
///   both. A student promises to take the party to Feh if the tree by the line
///   falls to Chi. Force from the line fells it (Event_ForceTree): the first
///   time, the students run for Feh, and Feizhi, Feh's daughter, storms out
///   with him. Since gems fell from the sky and one hit her she has had
///   visions, and she is sure that Hsu, who has not come back from Lama Temple,
///   is in danger; Feh will not believe her, and she leaves alone to find him
///   (FLAG_FEIZHI_WENT_TO_HSU), which unlocks the door she came out of
///   (FUCHIN_TEMPLE_INTERIORS_TO_HOUSE). Feh asks to see Isaac's Chi
///   (FLAG_FEH_WAITS_TO_SEE_CHI, or FLAG_FEH_CHI_DEMONSTRATION_DECLINED if
///   Isaac twice says no); when he sees the tree fall from the line, he knows
///   it is not Chi, which he learned from Master Hama of Lama Temple, but
///   Fuchin's Ki, which releases the mind's energy as Chi releases the body's
///   (FLAG_FEH_SAW_KI). Moving the tree with Move or pushing it by hand does
///   not count, and FLAG_FEH_SCHOOL_SAW_TREE_FALL keeps the students asking for
///   Chi from the line. Each of these flags picks the students' lines and Feh's
///   place; once Hsu is known to be trapped (FLAG_XIAN_HEARD_HSU_TRAPPED),
///   Scene_GetPlacements removes the tree and two students, and the school
///   talks of the boulders that overcame him. FUCHIN_TEMPLE_INTERIORS_TO_XIAN
///   leads out into Xian.
/// - Lama Temple (FuchinTempleInteriors, by
///   FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE), the room in which Master Hama
///   meditates, entered from Lama Temple (LamaTemple). On the first visit
///   (FuchinTemple_RunHamasRevealLesson) Hama greets Isaac and his Adept
///   companions, whom she foresaw, and tells Ivan that he is a Jupiter Adept
///   like her. She means to teach him Reveal, without which the party cannot
///   find the oases of the Lamakan Desert, which Saturos and Menardi are
///   crossing after blocking Silk Road. Feizhi bursts in, having sensed that
///   Hsu is in great danger among the boulders in the mountains; seeing that
///   her power has passed to Feizhi, Hama gives Ivan Reveal and hurries with
///   her to Hsu's aid, and FLAG_XIAN_HEARD_HSU_TRAPPED is set. Until Hsu is
///   rescued (FLAG_HSU_RESCUED) Hama and the boy monk in her room are away;
///   afterwards they are back. FUCHIN_TEMPLE_INTERIORS_TO_LAMA_TEMPLE leads
///   back into Lama Temple (LamaTemple).
///
/// Open questions:
///
/// - XXX: what Nyunpa's animations 5 (as he meditates) and 6 (once he is
///   enlightened), the kicking and punching students' animation 5 and the
///   leader's 7 and 6 (the unused hops) show; the frames each picks in its
///   sprite's sheet (graphics/sprite/spr_051.tsv for Nyunpa) would settle
///   their names.
/// - XXX: why Scene_Initialize places a translucent crate (SPRITE_CRATE,
///   through OverlayObject_PrepareObject) 24 pixels up by Feh's door in Master
///   Feh's school; a render of the school with and without it would show what
///   it covers.
/// - XXX: what reaches FuchinTempleGrounds' entrance 5, in front of the
///   traveling healer, which no exit names; likely where the party wakes after
///   it falls in battle, as Lama Temple's healer has one in the same place,
///   and a battle lost after speaking to the healer would settle it.
/// - XXX: what the leader's child values 15 and 1, alternated as spray falls
///   over him at the grotto (FuchinTemple_SpawnGrottoEntryParticle), and Hama's
///   and Ivan's 10 and 9 during the Reveal lesson change; the sprite group's
///   child objects in a render of those frames would settle it.
#include "game_state.h"
#include "scene_ids.h"
#include "unit/scene/fuchin_temple.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "field_obj.h"
#include "script.h"
#include "types.h"
#include "field_event.h"
#include "engine_call.h"
#include "field_scene.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "flags.h"
#include "songs.h"
#include "event.h"
#include "item_ids.h"
#include "objcmd.h"
#include "global_cells.h"
#include "fx_scene.h"
#include "palbuf.h"
#include "io_reg.h"

// Fuchin Temple's place flags, which last until the party enters another place;
// FLAG_PLACE_TEMP_50 is the Psynergy Stone outside, which then refills.

/// The party refused Nyunpa's test, so talking to him offers it again.
/// ★ Named from the game: "Then I cannot tell you. Give up and go back from
/// whence you came." (MsgXianThenCannotTellGiveUp)
#define FLAG_NYUNPA_TEST_DECLINED FLAG_PLACE_TEMP_00
/// Feh's students saw the training tree fall, but not to Chi from the line.
/// ★ Named from the game: "Look! The tree fell." (MsgXianLookTreeFell)
#define FLAG_FEH_SCHOOL_SAW_TREE_FALL FLAG_PLACE_TEMP_01

/// The placed actors outside Fuchin Temple (FuchinTempleGrounds), numbered in
/// the order of gFuchinTempleGroundsPlacements.
enum FuchinTempleOutsideActor {
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

/// The placed actors inside Fuchin Temple and in Master Feh's school in Xian
/// (FuchinTempleInteriors), numbered in the order of gFuchinTemplePlacements:
/// Nyunpa, Feh's students, Feizhi and Feh, and the training tree.
enum FuchinTempleInsideActor {
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

/// The placed actors in Master Hama's room in Lama Temple
/// (FuchinTempleInteriors by FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE),
/// numbered in the order of gFuchinTemplePlacementsFromLamaTemple; Garet, Ivan
/// and Mia, placed first, keep their own slots.
enum HamasRoomActor {
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

static void FuchinTemple_FeizhiLeavesForHsu(void);
static void FuchinTemple_FehSeesChi(void);
static UNUSED void FuchinTemple_HopLeaderByOffset(s32 offsetX, s32 offsetZ);
static void FuchinTemple_BeginMindReadDemonstration(s32 caster, s32 target);
static void FuchinTemple_FinishMindReadDemonstration(void);
static void FuchinTemple_SpawnNyunpasParticles(void);
static void FuchinTemple_AnimateRevealLesson(void);

// Shared source, included in link order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Clears the scripted actor's sprite flags and completes the script command.
static s32 FuchinTemple_ClearScriptedActorSpriteFlags(
    struct ScriptInterpreter *interpreter) {
  Engine_ActorSetSpriteFlags((struct FieldActor *)interpreter, 0);
  return 0;
}

// Keep an actor facing the party leader while the conversation is staged.
// UB: update hooks return void. The native produces an unused zero result;
// plain void changes five instructions in the English build.
#ifdef UBFIX
static void FuchinTemple_FaceLeader(union FieldObject *object) {
#else
static s32 FuchinTemple_FaceLeader(union FieldObject *object) {
#endif
  struct FieldActor *actor = (struct FieldActor *)object;
  struct FieldActor *player = Object_GetById(ACTOR_PARTY_LEADER);
  actor->facing = ArcTan2(player->z.fixed - actor->z.fixed,
                          player->x.fixed - actor->x.fixed);
#ifndef UBFIX
  return 0;
#endif
}

/// Chooses the outside entrance table or the shared interior entrances.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_FuchinTempleGrounds) {
    return gFuchinTempleGroundsEntrances;
  }
  return gFuchinTempleEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gFuchinTempleExits; }

/// Selects the room's actors and applies the post-lesson placement changes.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_FuchinTempleGrounds) {
    return gFuchinTempleGroundsPlacements;
  }
  if (gGameState.entrance == FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE) {
    return gFuchinTemplePlacementsFromLamaTemple;
  }
  // Once Hsu is known to be trapped, the tree and two students are gone, and
  // another student stands elsewhere.
  if (FLAG_CHECK(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
    gFuchinTemplePlacements[ACTOR_PROUD_STUDENT - ACTOR_FIRST_PLACED]
        .condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
    gFuchinTemplePlacements[ACTOR_TREE_STUDENT - ACTOR_FIRST_PLACED].condition =
        FLAG_XIAN_HEARD_HSU_TRAPPED;
    gFuchinTemplePlacements[ACTOR_MESSENGER_STUDENT - ACTOR_FIRST_PLACED].x =
        0x1200000;
    gFuchinTemplePlacements[ACTOR_MESSENGER_STUDENT - ACTOR_FIRST_PLACED].z =
        0xf80000;
    gFuchinTemplePlacements[ACTOR_TRAINING_TREE - ACTOR_FIRST_PLACED]
        .condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
    gFuchinTemplePlacements[ACTOR_TRAINING_TREE_TARGET - ACTOR_FIRST_PLACED]
        .condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
  }
  FieldScene_PrepareActors(gFuchinTemplePlacements);
  return gFuchinTemplePlacements;
}

/// Talking to the young man from Kolima at Fuchin Temple
/// (FuchinTempleGrounds): he asks how the party got here. "How did you get
/// here? Is the bridge open again?" Yes cheers him ("Then I can go home to
/// Kolima now?") and is remembered; no means the party came through the forest.
/// Later talks go on to the tale of the forest.
static void Event_YoungManFromKolimaAsksAboutBridge(void) {
  EV_BEGIN();
  MESSAGE_SET(MsgXianHowDidGetHereBridge);
  if (FLAG_CHECK(FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN) != 0) {
    gEventWork->message += 4;
  }
  Event_OpenMessage(ACTOR_YOUNG_MAN_FROM_KOLIMA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    FLAG_SET(FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN);
  } else {
    gEventWork->message++;
  }
  Event_ShowMessage(ACTOR_YOUNG_MAN_FROM_KOLIMA, 0);
  EV_END();
}

/// Talking to the boy monk (ACTOR_ADMIRING_BOY_MONK) at Fuchin Temple
/// (FuchinTempleGrounds): he speaks of the one seeking enlightenment, with up
/// to two more questions. "Isn't it noble of him to try to save the world by
/// becoming enlightened?" Once Nyunpa is gone, "Now, he truly is beyond the
/// world's concerns. Is he watching over us?"
static void Event_BoyMonkSpeaksOfNyunpa(void) {

  EV_BEGIN();
  if (Engine_GameFlagIsSet(FLAG_NYUNPA_ENLIGHTENED) != 0) {
    MESSAGE_SET(MsgXianNowHeTrulyBeyondWorlds);
    Engine_EventAskYesNo(ACTOR_ADMIRING_BOY_MONK, 0);
    EV_END();
  } else {
    MESSAGE_SET(MsgXianIsntNobleHimTrySave);
    MESSAGE_OPEN(ACTOR_ADMIRING_BOY_MONK, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      gEventWork->message++;
      MESSAGE_OPEN(ACTOR_ADMIRING_BOY_MONK, 0);
      if (Engine_EventChooseYesNo(0, 0) == 1) {
        gEventWork->message++;
      }
    }
    MESSAGE_PUT(ACTOR_ADMIRING_BOY_MONK, 0);
    EV_END();
  }
}

/// Talking to the monk beneath the falls at Fuchin Temple (FuchinTempleGrounds)
/// once Nyunpa has set the test (FLAG_NYUNPA_TEST_ACCEPTED): he lets the party
/// pass (FLAG_FUCHIN_FALLS_GROTTO_OPENED). "I have already heard of your test
/// from Master Nyunpa. You may enter." He leaps aside, and the way into the
/// waterfall grotto opens.
static void Event_MonkLetsPartyIntoGrotto(void) {
  struct FieldActor *monk;
  struct FieldActor *movingMonk;
  struct EffectOptions options;

  monk = Object_GetById(ACTOR_FALLS_MONK);
  EV_BEGIN();
  MESSAGE_SET(MsgXianAlreadyHeardTest);
  Engine_EventShowMessageAndWait(ACTOR_FALLS_MONK, 0, 20);
  // The monk leaps aside and lands in a puff of dust.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 392);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Engine_AudioPlayCue(SE_132);
  movingMonk = Object_GetById(ACTOR_FALLS_MONK);
  movingMonk->velocityY = 0x140000;
  movingMonk = Object_GetById(ACTOR_FALLS_MONK);
  movingMonk->gravity = 0x40000;
  OBJ_SPEED(ACTOR_FALLS_MONK, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetDestination(ACTOR_FALLS_MONK, 152, 392);
  Engine_ActorWaitForMove(ACTOR_FALLS_MONK);
  movingMonk = Object_GetById(ACTOR_FALLS_MONK);
  movingMonk->gravity = 0x10000;
  Engine_ActorFaceDirection(ACTOR_FALLS_MONK, FACING_EAST, 0);
  Engine_AudioPlayCue(SE_132);
  // Only the palette option is enabled for these three particles.
  options.palette = 7;
  FieldFx_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + 0x40000), 0x8000,
                0, 0, EFFECT_USE_PALETTE, &options);
  FieldFx_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + 0x40000), 0, 0,
                0, EFFECT_USE_PALETTE, &options);
  FieldFx_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + 0x40000),
                -0x8000, 0, 0, EFFECT_USE_PALETTE, &options);
  WAIT(30);
  FieldFx_PlayQueuedSound();
  Map_CopyCellAttributes(10, 24, 1, 1, 10, 22);
  Engine_GameFlagSet(FLAG_FUCHIN_FALLS_GROTTO_OPENED);
  EV_END();
}

/// Isaac comes back out of the grotto with the Orb of Force. At Fuchin Temple
/// (FuchinTempleGrounds, trigger 5) the monk sends him on to Nyunpa, and the
/// way back is open again. "Young master, did you complete the test?" Then
/// "Master Nyunpa is waiting for you... Proceed to the temple."
static void Event_MonkAsksWhetherTestIsDone(void) {
  EV_BEGIN();
  Engine_GameFlagSet(FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST);
  OBJ_FACE(ACTOR_FALLS_MONK, ACTOR_PARTY_LEADER, 0);
  WAIT(10);
  MESSAGE_SET(MsgXianYoungMasterDidCompleteTest);
  Engine_ActorRunRepeatedMotion(ACTOR_FALLS_MONK, 2);
  WAIT(20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Engine_EventAskYesNo(ACTOR_FALLS_MONK, 0);
  WAIT(10);
  OBJ_EMOTE(ACTOR_FALLS_MONK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  OBJ_DIR(ACTOR_FALLS_MONK, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FALLS_MONK, 2);
  WAIT(20);
  Engine_EventShowMessageAndWait(ACTOR_FALLS_MONK, 0, 20);
  Engine_ActorFaceDirection(ACTOR_FALLS_MONK, FACING_EAST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FALLS_MONK, ANIM_NOD);
  WAIT(20);
  Engine_EventShowMessageAndWait(ACTOR_FALLS_MONK, 0, 20);
  Map_CopyCellAttributes(10, 26, 1, 1, 10, 24);
  EV_END();
}

/// Reading Nyunpa's mind inside Fuchin Temple (FuchinTempleInteriors), or
/// talking to him after a refusal: he hears the party, reads their minds in
/// turn, guesses that they are following someone, warns of Mogall Forest and
/// offers the test of the grotto. "Mmmm... Who is that? Who speaks to my mind?"
/// Accepting it (FLAG_NYUNPA_TEST_ACCEPTED), he sends Isaac to the monk beneath
/// the falls; refusing is remembered (FLAG_NYUNPA_TEST_DECLINED), and the next
/// talk asks again.
static void Event_NyunpaOffersTest(void) {
  s32 alreadyDeclinedTrial;
  struct FieldActor *nyunpa;

  EV_BEGIN();
  // Revisit an earlier refusal, or introduce the offer through Mind Read.
  alreadyDeclinedTrial = FLAG_CHECK(FLAG_NYUNPA_TEST_DECLINED);
  if (alreadyDeclinedTrial != 0) {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 504);
    WAIT(5);
    OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Object_GetById(ACTOR_NYUNPA)->paused = 0;
    SE(SE_JUMP);
    nyunpa = Actor_Get(ACTOR_NYUNPA);
    nyunpa->velocityY = 0x80000;
    OBJ_MODE(ACTOR_NYUNPA, ANIM_STAND);
    WAIT(30);
    MESSAGE_SET(MsgXianMustSurviveTestGrottoBefore);
  } else {
    MESSAGE_SET(MsgXianMmmmWhoWhoSpeaksMy);
    FuchinTemple_BeginMindReadDemonstration(ACTOR_PARTY_LEADER, ACTOR_NYUNPA);
    WAIT(30);
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    FuchinTemple_FinishMindReadDemonstration();
    WAIT(20);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 168, 504);
    WAIT(5);
    OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    SE(SE_JUMP);
    Object_GetById(ACTOR_NYUNPA)->paused = alreadyDeclinedTrial;
    nyunpa = Actor_Get(ACTOR_NYUNPA);
    nyunpa->velocityY = 0x80000;
    OBJ_MODE(ACTOR_NYUNPA, ANIM_STAND);
    WAIT(30);
    Event_OpenMessage(ACTOR_NYUNPA, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      Engine_ActorRunRepeatedMotion(ACTOR_NYUNPA, 2);
      WAIT(20);
      Event_ShowMessage(ACTOR_NYUNPA, 0);
      WAIT(20);
      FuchinTemple_BeginMindReadDemonstration(ACTOR_NYUNPA, ACTOR_PARTY_LEADER);
      WAIT(30);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      WAIT(50);
      FuchinTemple_FinishMindReadDemonstration();
      WAIT(30);
      Engine_ActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
      Event_ShowMessage(ACTOR_NYUNPA, 0);
    } else {
      gEventWork->message += 2;
      Event_ShowMessage(ACTOR_NYUNPA, 0);
    }
    Engine_ActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
    WAIT(30);
    OBJ_EMOTE(ACTOR_NYUNPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    MESSAGE_SET(MsgXianFollowThemDoNot);
    Event_OpenMessage(ACTOR_NYUNPA, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      OBJ_EMOTE(ACTOR_NYUNPA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      FuchinTemple_BeginMindReadDemonstration(ACTOR_NYUNPA, ACTOR_PARTY_LEADER);
      WAIT(30);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      WAIT(50);
      FuchinTemple_FinishMindReadDemonstration();
      WAIT(30);
      Event_ShowMessage(ACTOR_NYUNPA, 0);
      gEventWork->message += 1;
    } else {
      gEventWork->message += 1;
      WAIT(20);
      Engine_ActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
      WAIT(20);
      Event_ShowMessage(ACTOR_NYUNPA, 0);
    }
    WAIT(20);
    Engine_ActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_SHAKE_HEAD);
    WAIT(20);
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    WAIT(20);
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    WAIT(20);
    OBJ_DIR(ACTOR_NYUNPA, FACING_NORTH, 20);
    OBJ_SPEED(ACTOR_NYUNPA, 3 * PIXELS(1) / 10, 3 * PIXELS(1) / 20);
    Actor_WalkToAndWait(ACTOR_NYUNPA, 168, 464);
    WAIT(60);
    OBJ_DIR(ACTOR_NYUNPA, FACING_SOUTH, 40);
    Event_ShowMessageAndWait(ACTOR_NYUNPA, 0, 10);
    OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Actor_WalkToAndWait(ACTOR_NYUNPA, 168, 472);
  }
  Event_OpenMessage(ACTOR_NYUNPA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    MESSAGE_SET(MsgXianThenCannotTellGiveUp);
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    FLAG_SET(FLAG_NYUNPA_TEST_DECLINED);
  } else {
    // "Do not worry. You will be permitted to enter the grotto." Nyunpa
    // glows as he calls to the monk.
    MESSAGE_SET(MsgXianDoNotWorryWillPermitted);
    WAIT(30);
    Engine_ActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
    WAIT(20);
    Camera_SetSpeed(0x8000, 0x1000);
    Engine_CameraMoveToActor(ACTOR_NYUNPA, 1);
    ColorBuffer_ApplySource(PALETTE_TRANSFORM_PLAIN, PALETTE_HALVES_BOTH);
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_WARM, PALETTE_HALF_BG);
    Engine_ColorBufferInterpolate(30);
    Engine_EventWaitForScreen();
    Engine_CameraWaitForMove();
    FuchinTemple_SpawnNyunpasParticles();
    ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, PALETTE_HALVES_BOTH);
    Engine_ColorBufferInterpolate(30);
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_NYUNPA, 2);
    WAIT(20);
    Event_ShowMessage(ACTOR_NYUNPA, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    WAIT(60);
    Event_ShowMessageAndWait(ACTOR_NYUNPA, 0, 10);
    FLAG_SET(FLAG_NYUNPA_TEST_ACCEPTED);
  }
  OBJ_MODE(ACTOR_NYUNPA, 5);
  EV_END();
}

/// Reading Nyunpa's mind inside Fuchin Temple once the test is accepted: he
/// notices.
/// "You enjoy reading the minds of others, do you not?"
static void Event_NyunpaFeelsMindRead(void) {
  EV_BEGIN();
  MESSAGE_SET(MsgXianEnjoyReadingMindsOthersDo);
  Engine_EventAskYesNo(ACTOR_NYUNPA, 0);
  EV_END();
}

/// Talking to the student at the door of Master Feh's school in Xian
/// (ACTOR_WELCOMING_STUDENT): the student asks whether the party came to watch.
/// "This is Master Feh's school... You came to watch?"
static void Event_StudentWelcomesToSchool(void) {
  EV_BEGIN();
  MESSAGE_SET(MsgXianMasterFehsSchoolCameWatch);
  Engine_EventAskYesNo(ACTOR_WELCOMING_STUDENT, 0);
  EV_END();
}

/// Talking to the student who practices jumping in Master Feh's school in Xian:
/// he lands, talks about jumping (or Lama Temple, or Hsu), then jumps on.
static void Event_TalkToJumpingStudent(void) {
  EV_BEGIN();

  Object_GetById(ACTOR_JUMPING_STUDENT)->paused = 0;

  while (Object_GetById(ACTOR_JUMPING_STUDENT)->y.fixed > 0) {
    Engine_TaskWait(1);
  }

  Object_GetById(ACTOR_JUMPING_STUDENT)->y.fixed = 0;

  Object_GetById(ACTOR_JUMPING_STUDENT)->targetY = ACTOR_NO_TARGET;

  Object_GetById(ACTOR_JUMPING_STUDENT)->velocityY = 0;

  Object_GetById(ACTOR_JUMPING_STUDENT)->paused = 1;

  OBJ_FACE(ACTOR_JUMPING_STUDENT, ACTOR_PARTY_LEADER, 0);

  if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) != 0) {
    MESSAGE_SET(MsgXianHsuDidNotPracticeJumping);
  } else if (Engine_GameFlagIsSet(FLAG_FEH_SAW_KI) != 0) {
    MESSAGE_SET(MsgXianLamaTempleFarWestIn);
  } else {
    MESSAGE_SET(MsgXianFlexibilityJumpingVeryImportantIn);
  }

  MESSAGE_PUT(ACTOR_JUMPING_STUDENT, 0);

  Object_GetById(ACTOR_JUMPING_STUDENT)->facing = FACING_SOUTH;

  Object_GetById(ACTOR_JUMPING_STUDENT)->paused = 0;

  Actor_SetActionScript(ACTOR_JUMPING_STUDENT,
                        gFuchinTemple_JumpingStudentScript);
  EV_END();
}

/// Talking to the student by the training tree in Master Feh's school in Xian
/// (ACTOR_TREE_STUDENT): the student asks whether the party still wishes to see
/// Master Feh. "Master Feh is very busy. Do you still wish to see him?" Yes: if
/// the tree falls to Chi from the line, he will take the party to Feh.
static void Event_StudentAsksAboutFeh(void) {
  EV_BEGIN();
  MESSAGE_SET(MsgXianMasterFehVeryBusyDo);
  Engine_EventAskYesNo(ACTOR_TREE_STUDENT, 0);
  EV_END();
}

/// Rotates the training tree through its westward arc and scatters particles.
static void FuchinTemple_ToppleTreeWest(void) {
  struct FieldActor *tree = Object_GetById(ACTOR_TRAINING_TREE);
  u32 turnStep;
  s32 angle;

  for (turnStep = 8; turnStep > 3; turnStep--) {
    angle = turnStep << 12;
    tree->sprite->rotation = angle;
    Engine_TaskWait((turnStep - 4) * 2);
    tree->x.fixed += Engine_MathCos(angle) * 6;
    tree->z.fixed += Engine_MathSin(angle) * 6;
  }

  tree->y.fixed = 0x120000;
  tree->targetY = 0x120000;

  Engine_AudioPlayCue(SE_227);

  FieldFx_Spawn(tree->x.fixed - 0xc0000, tree->y.fixed, tree->z.fixed + 0x80000,
                0xffffcccd, 0x6666, 0, 0, 0);
  FieldFx_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0x80000,
                0xffff3334, 0x4ccc, 0, 0, 0);
  FieldFx_Spawn(tree->x.fixed + 0xa0000, tree->y.fixed, tree->z.fixed + 0x80000,
                0xffff0000, 0x3333, 0, 0, 0);
}

/// Rotates the training tree through the opposite arc and reverses its
/// horizontal scale.
static void FuchinTemple_ToppleTreeEast(void) {
  struct FieldActor *tree;
  u32 turnStep;
  s32 angle;

  tree = Object_GetById(ACTOR_TRAINING_TREE);
  for (turnStep = 8; turnStep <= 12; turnStep++) {
    angle = turnStep << 12;
    tree->sprite->rotation = angle;
    Engine_TaskWait((12 - turnStep) * 2);
    tree->x.fixed -= Engine_MathCos(angle) * 6;
    tree->z.fixed -= Engine_MathSin(angle) * 6;
  }
  tree->y.fixed = 0x120000;
  tree->targetY = 0x120000;
  tree->scaleX = 0xffff3334;
  Engine_AudioPlayCue(SE_227);
  FieldFx_Spawn(tree->x.fixed - 0xc0000, tree->y.fixed, tree->z.fixed + 0x80000,
                0x10000, 0x3333, 0, 0, 0);
  FieldFx_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0x80000, 0xcccc,
                0x4ccc, 0, 0, 0);
  FieldFx_Spawn(tree->x.fixed + 0xa0000, tree->y.fixed, tree->z.fixed + 0x80000,
                0x3333, 0x6666, 0, 0, 0);
}

/// Slides the training tree north in four steps, then scatters particles.
static void FuchinTemple_ToppleTreeNorth(void) {
  struct FieldActor *tree = Object_GetById(ACTOR_TRAINING_TREE);
  u32 stepIndex = 0;
  s32 delay = 8;
  u8 rotation;
  do {
    Engine_TaskWait(delay);
    tree->z.fixed += 0xffff0000;
    tree->targetZ = ACTOR_NO_TARGET;
    stepIndex++;
    delay -= 2;
  } while (stepIndex < 4);
  rotation = 0;
  tree->sprite->rotation = rotation;
  Engine_AudioPlayCue(SE_227);
  FieldFx_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0xfff80000,
                0xffff3334, 0, 0xffffcccd, 0, 0);
  FieldFx_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0xfff80000,
                0x0000cccc, 0, 0xffffcccd, 0, 0);
  FieldFx_Spawn(tree->x.fixed + 0xfffa0000, tree->y.fixed,
                tree->z.fixed + (160 << 12), 0x00003333, 0, 0xffff0000, 0, 0);
  FieldFx_Spawn(tree->x.fixed + (192 << 11), tree->y.fixed,
                tree->z.fixed + (160 << 12), 0x00003333, 0, 0xffff0000, 0, 0);
}

/// Slides the training tree south in four steps before the final displacement.
static void FuchinTemple_ToppleTreeSouth(void) {
  struct FieldActor *tree;
  u32 stepIndex;
  s32 particleSpeed;
  s32 delay;

  tree = Object_GetById(ACTOR_TRAINING_TREE);
  stepIndex = 0;
  delay = 8;
  do {
    Engine_TaskWait(delay);
    tree->z.fixed += 0x10000;
    tree->targetZ = ACTOR_NO_TARGET;
    stepIndex++;
    delay -= 2;
  } while (stepIndex < 4);

  tree->sprite->rotation = 0;
  tree->z.fixed += 0x180000;
  tree->targetZ = ACTOR_NO_TARGET;
  Engine_AudioPlayCue(SE_227);

  FieldFx_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0xc0000, -0xcccc,
                0, 0x3333, 0, 0);
  particleSpeed = 0x3333;
  FieldFx_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0xc0000, 0xcccc,
                0, particleSpeed, 0, 0);
  FieldFx_Spawn(tree->x.fixed - 0x60000, tree->y.fixed, tree->z.fixed - 0x80000,
                particleSpeed, 0, 0x10000, 0, 0);
  FieldFx_Spawn(tree->x.fixed + 0x60000, tree->y.fixed, tree->z.fixed - 0x80000,
                particleSpeed, 0, 0x10000, 0, 0);
}

/// Routes the student around the tree according to the leader's facing.
static void FuchinTemple_StudentWalksAroundTree(void) {
  u32 leaderFacing;
  u32 quadrantLimit;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  quadrantLimit = 0x3fff;
  if (((leaderFacing - FACING_SOUTHEAST) << 16) <= 0x3fff0000) {
    Engine_ActorWalkToAndWait(ACTOR_TREE_STUDENT, 216, 168);
    Engine_ActorWalkToAndWait(ACTOR_TREE_STUDENT, 224, 168);
    OBJ_DIR(ACTOR_TREE_STUDENT, FACING_SOUTHEAST, 20);
  } else if ((u16)(leaderFacing - FACING_SOUTHWEST) <= quadrantLimit) {
    Engine_ActorWalkToAndWait(ACTOR_TREE_STUDENT, 232, 160);
    OBJ_DIR(ACTOR_TREE_STUDENT, FACING_SOUTH + FACING_STEP, 20);
  } else if ((u16)(leaderFacing - FACING_NORTHWEST) <= quadrantLimit) {
    Engine_ActorWalkToAndWait(ACTOR_TREE_STUDENT, 216, 168);
    Engine_ActorWalkToAndWait(ACTOR_TREE_STUDENT, 224, 172);
    OBJ_DIR(ACTOR_TREE_STUDENT, FACING_NORTHEAST, 20);
  } else {
    Engine_ActorWalkToAndWait(ACTOR_TREE_STUDENT, 232, 160);
    OBJ_DIR(ACTOR_TREE_STUDENT, FACING_SOUTHEAST, 20);
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
/// FuchinTempleChiRule, says. Then he sets the tree up again.
static void FuchinTemple_StudentExplainsChiRules(s32 mode) {
  s32 rulesMessage;

  OBJ_SPEED(ACTOR_TREE_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  WAIT(60);
  rulesMessage = MsgXianCannotPushHands;
  MESSAGE_SET(rulesMessage);
  if (mode == FUCHIN_TEMPLE_CHI_NOT_FROM_LINE) {
    MESSAGE_SET(rulesMessage - 1);
    OBJ_EMOTE(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Engine_EventShowMessageAndWait(ACTOR_TREE_STUDENT, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 2);
    MESSAGE_SET(MsgXianWarriorsCannotUse);
    Engine_EventShowMessageAndWait(ACTOR_TREE_STUDENT, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_TREE_STUDENT, ANIM_SHAKE_HEAD);
    WAIT(20);
    Engine_EventShowMessageAndWait(ACTOR_TREE_STUDENT, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_TREE_STUDENT, ANIM_NOD);
    WAIT(20);
  }
  if (mode == FUCHIN_TEMPLE_CHI_SEEN_BEFORE) {
    MESSAGE_SET(MsgXianGreatWarriorTrain);
    Engine_ActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 2);
    WAIT(20);
  }
  Engine_EventShowMessageAndWait(ACTOR_TREE_STUDENT, 0, 20);
  // Restore the tree and let the student return to the demonstration spot.
  FuchinTemple_StudentWalksAroundTree();
  Engine_ActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 3);
  OBJ_POS_SET(ACTOR_TRAINING_TREE, 232, 168);
  Engine_ActorSetPosition(ACTOR_TRAINING_TREE_TARGET, PIXELS(232), PIXELS(168));
  Object_GetById(ACTOR_TRAINING_TREE)->y.fixed = 0xc0000;
  Object_GetById(ACTOR_TRAINING_TREE)->targetY = ACTOR_NO_TARGET;
  Object_GetById(ACTOR_TRAINING_TREE)->scaleX = 0xcccc;
  Object_GetById(ACTOR_TRAINING_TREE)->sprite->rotation = 0x8000;
  Engine_AudioPlayCue(SE_124);
  WAIT(40);
  Engine_ActorWalkToAndWait(ACTOR_TREE_STUDENT, 216, 152);
  OBJ_DIR(ACTOR_TREE_STUDENT, FACING_SOUTHEAST, 30);
}

/// The students see the tree fall, Feh doubts it came from the line, and the
/// student sets the tree up again. "Look! The tree fell."
static void FuchinTemple_StudentsSeeTreeFall(void) {
  struct FieldActor *tree;

  OBJ_FACE(ACTOR_PROUD_STUDENT, ACTOR_TRAINING_TREE, 0);
  OBJ_FACE(ACTOR_WATCHING_STUDENT, ACTOR_TRAINING_TREE, 0);
  OBJ_FACE(ACTOR_TREE_STUDENT, ACTOR_TRAINING_TREE, 0);
  OBJ_FACE(ACTOR_MESSENGER_STUDENT, ACTOR_TRAINING_TREE, 0);
  OBJ_FACE(ACTOR_FEH, ACTOR_TRAINING_TREE, 0);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 2);
  WAIT(20);
  MESSAGE_SET(MsgXianLookTreeFell);
  Engine_EventShowMessageAndWait(ACTOR_TREE_STUDENT, 0, 20);
  Engine_EventShowMessageAndWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  OBJ_EMOTE(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  OBJ_EMOTE(ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_EventShowMessageAndWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
  WAIT(20);
  Engine_EventShowMessageAndWait(ACTOR_FEH, 0, 20);
  OBJ_EMOTE(ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_EventShowMessageAndWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Engine_ActorFaceEachOther(ACTOR_TREE_STUDENT, ACTOR_FEH, 0);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_TREE_STUDENT, ANIM_NOD);
  WAIT(20);
  // Reset the tree after the group has reacted to the demonstration.
  OBJ_SPEED(ACTOR_TREE_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  FuchinTemple_StudentWalksAroundTree();
  Engine_ActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 3);
  OBJ_POS_SET(ACTOR_TRAINING_TREE, 232, 168);
  Engine_ActorSetPosition(ACTOR_TRAINING_TREE_TARGET, PIXELS(232), PIXELS(168));
  tree = Object_GetById(ACTOR_TRAINING_TREE);
  tree->y.fixed = 0xc0000;
  tree = Object_GetById(ACTOR_TRAINING_TREE);
  tree->targetY = ACTOR_NO_TARGET;
  tree = Object_GetById(ACTOR_TRAINING_TREE);
  tree->scaleX = 0xcccc;
  tree = Object_GetById(ACTOR_TRAINING_TREE);
  {
    struct AnimationObject *sprite = tree->sprite;
    s32 rotation = 0x8000;

    sprite->rotation = rotation;
  }
  Engine_AudioPlayCue(SE_124);
  WAIT(40);
  Engine_ActorWalkToAndWait(ACTOR_TREE_STUDENT, 216, 152);
  OBJ_DIR(ACTOR_TREE_STUDENT, FACING_SOUTH, 30);
  Engine_GameFlagSet(FLAG_FEH_SCHOOL_SAW_TREE_FALL);
}

/// Move on the training tree in Master Feh's school in Xian: it topples, and
/// the students react; it does not count as Chi.
static void Event_MoveTree(void) {
  EV_BEGIN();

  if (Object_GetById(ACTOR_PARTY_LEADER)->facing > FACING_SOUTH &&
      Object_GetById(ACTOR_PARTY_LEADER)->facing < FACING_NORTH) {
    FuchinTemple_ToppleTreeWest();
  } else {
    FuchinTemple_ToppleTreeEast();
  }

  if (Engine_GameFlagIsSet(FLAG_FEH_WAITS_TO_SEE_CHI) != 0) {
    FuchinTemple_StudentsSeeTreeFall();
  } else {
    FuchinTemple_StudentExplainsChiRules(FUCHIN_TEMPLE_CHI_NOT_FROM_LINE);
  }

  EV_END();
}

/// Walking into the training tree in Master Feh's school in Xian: Isaac pushes
/// it over by hand. "You cannot push it with your hands. Attack it with Chi."
static void Event_PushTreeByHand(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  u16 facing;

  EV_BEGIN();
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_PUSH);
  WAIT(20);

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

  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  FuchinTemple_StudentExplainsChiRules(FUCHIN_TEMPLE_CHI_PUSHED_BY_HAND);
  EV_END();
}

/// Force on the training tree in Master Feh's school in Xian: it topples. From
/// too close it does not count;
/// from the line, the students call Feh out the first time, and later Feh
/// himself sees it.
static void Event_ForceTree(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  u16 facing;

  EV_BEGIN();

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

  Engine_CameraSetSpeed(0x10000, 0x2000);
  Engine_CameraMoveToActor(ACTOR_TRAINING_TREE_TARGET, 1);
  Engine_CameraWaitForMove();

  if (leader->z.part.pixel <= 209) {
    if (Engine_GameFlagIsSet(FLAG_FEIZHI_WENT_TO_HSU) == 0 ||
        Engine_GameFlagIsSet(FLAG_FEH_SAW_KI) != 0) {
      FuchinTemple_StudentExplainsChiRules(FUCHIN_TEMPLE_CHI_NOT_FROM_LINE);
    } else {
      FuchinTemple_StudentsSeeTreeFall();
    }
    EV_END();
    return;
  }

  if (Engine_GameFlagIsSet(FLAG_FEH_SAW_KI) != 0) {
    FuchinTemple_StudentExplainsChiRules(FUCHIN_TEMPLE_CHI_SEEN_BEFORE);
  } else if (Engine_GameFlagIsSet(FLAG_FEIZHI_WENT_TO_HSU) == 0) {
    FuchinTemple_FeizhiLeavesForHsu();
  } else {
    FuchinTemple_FehSeesChi();
  }
  EV_END();
}

/// The first time the tree falls from the line, a student runs for Master Feh.
/// "What did you do, warrior?" The door opens on its own and Feizhi storms
/// out, sure that Hsu is in danger. Feh will not believe her visions, and she
/// leaves alone to find Hsu. Feh then asks Isaac to show him his Chi.
static void FuchinTemple_FeizhiLeavesForHsu(void) {
  struct FieldActor *actor;

  FLAG_SET(FLAG_FEIZHI_WENT_TO_HSU);
  WAIT(30);
  Actor_FaceActor(ACTOR_PROUD_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_TREE_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_PARTY_LEADER, 0);
  WAIT(20);
  OBJ_EMOTE(ACTOR_PROUD_STUDENT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJ_EMOTE(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJ_EMOTE(ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  WAIT(60);
  MESSAGE_SET(MsgXianDidDoWarrior);
  Event_ShowMessageAndWait(ACTOR_PROUD_STUDENT, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_PROUD_STUDENT, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 1);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_TREE_STUDENT, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TREE_STUDENT, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
  WAIT(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_MESSENGER_STUDENT, 0);
  Event_AskYesNo(ACTOR_MESSENGER_STUDENT, 0);
  WAIT(50);
  // Bring the observers out through the school door.
  Engine_CameraFollowActor(ACTOR_MESSENGER_STUDENT, 1);
  OBJ_SPEED(ACTOR_MESSENGER_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_MESSENGER_STUDENT, 176, 248);
  Actor_WalkToAndWait(ACTOR_MESSENGER_STUDENT, 308, 248);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  OBJ_DIR(ACTOR_MESSENGER_STUDENT, FACING_NORTH, 20);
  SE(SE_DOOR_OPEN);
  Map_AnimateCells(gFuchinTemple_FehsDoorOpens, 78, 13);
  Engine_ActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
  WAIT(20);
  OBJ_SPEED(ACTOR_MESSENGER_STUDENT, 3 * PIXELS(1) / 2, 3 * PIXELS(1) / 4);
  actor = Object_GetById(ACTOR_MESSENGER_STUDENT);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_MESSENGER_STUDENT, 308, 272);
  WAIT(1);
  actor = Actor_Get(ACTOR_MESSENGER_STUDENT);

  actor->steeringFlags = ACTOR_FACES_MOTION | actor->steeringFlags;
  Event_ShowMessageAndWait(ACTOR_MESSENGER_STUDENT, 0, 50);
  OBJ_POS_SET(ACTOR_FEIZHI, 304, 216);
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
  WAIT(10);
  Engine_ActorStartRepeatedMotion(ACTOR_WELCOMING_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_KICKING_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_PUNCHING_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_JUMPING_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_PROUD_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_WATCHING_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_TREE_STUDENT, 2);
  Engine_ActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
  OBJ_EMOTE(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  OBJ_POS_SET(ACTOR_FEH, 304, 216);
  OBJ_WALK(ACTOR_FEH, 304, 248);
  OBJ_WALK(ACTOR_FEIZHI, 280, 264);
  Engine_ActorWaitForMove(ACTOR_FEH);
  OBJ_DIR(ACTOR_FEH, FACING_SOUTH + FACING_STEP, 0);
  Engine_ActorWaitForMove(ACTOR_FEIZHI);
  SE(SE_159);
  Map_AnimateCells(gFuchinTemple_FehsDoorCloses, 78, 13);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  Actor_FaceActor(ACTOR_WELCOMING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_KICKING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_PUNCHING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_JUMPING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_PROUD_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_WATCHING_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_TREE_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_FEIZHI, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_FEIZHI, 0);
  WAIT(10);
  Engine_ActorRunRepeatedMotion(ACTOR_FEIZHI, 2);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(ACTOR_FEH, 1);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  OBJ_DIR(ACTOR_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  OBJ_EMOTE(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FEIZHI, 2);
  WAIT(20);
  OBJ_DIR(ACTOR_FEIZHI, FACING_EAST, 20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  OBJ_DIR(ACTOR_MESSENGER_STUDENT, FACING_WEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  OBJ_DIR(ACTOR_FEIZHI, FACING_WEST, 20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_WELCOMING_STUDENT, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_WELCOMING_STUDENT, 0, 20);
  OBJ_DIR(ACTOR_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FEIZHI, 1);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  OBJ_EMOTE(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  OBJ_EMOTE(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  OBJ_EMOTE(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
  WAIT(20);
  OBJ_EMOTE(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  OBJ_EMOTE(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FEH, 2);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FEIZHI, 2);
  WAIT(10);
  Actor_WalkToAndWait(ACTOR_FEIZHI, 256, 280);
  OBJ_DIR(ACTOR_FEIZHI, FACING_SOUTH, 20);
  OBJ_POS_SET(ACTOR_FEIZHI, 0, 0);
  Engine_ActorDestroy(ACTOR_FEIZHI);
  WAIT(30);
  Engine_ActorRunRepeatedMotion(ACTOR_WELCOMING_STUDENT, 2);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_WELCOMING_STUDENT, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 2);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_TREE_STUDENT, 0, 20);
  Actor_FaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_FEH, 0);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Actor_FaceActor(ACTOR_FEH, ACTOR_MESSENGER_STUDENT, 0);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  OBJ_SPEED(ACTOR_FEH, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_FEH, 256, 248);
  OBJ_DIR(ACTOR_FEH, FACING_NORTH, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_FEH, 1);
  OBJ_EMOTE(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_WalkToAndWait(ACTOR_FEH, 240, 184);
  Engine_ActorRunRepeatedMotion(ACTOR_FEH, 2);
  WAIT(20);
  OBJ_POS_SET(ACTOR_TRAINING_TREE, 232, 168);
  OBJ_POS_SET(ACTOR_TRAINING_TREE_TARGET, 232, 168);
  // Restore the tree before leaving the group ready for another demonstration.
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  actor->y.fixed = 0xc0000;
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  actor->targetY = ACTOR_NO_TARGET;
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  actor->scaleX = 0xcccc;
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  {
    struct AnimationObject *sprite = actor->sprite;
    s32 rotation = 0x8000;

    sprite->rotation = rotation;
  }
  SE(SE_124);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Actor_WalkToAndWait(ACTOR_MESSENGER_STUDENT, 256, 240);
  OBJ_DIR(ACTOR_MESSENGER_STUDENT, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 1);
  Event_ShowMessageAndWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Actor_FaceActor(ACTOR_WELCOMING_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_KICKING_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PUNCHING_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_JUMPING_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PROUD_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_WATCHING_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_TREE_STUDENT, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_PARTY_LEADER, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_FEH, 2);
  WAIT(20);
  OBJ_DIR(ACTOR_FEH, FACING_SOUTH + FACING_STEP, 20);
  Actor_WalkToAndWait(ACTOR_FEH, 248, 208);
  OBJ_DIR(ACTOR_FEH, FACING_SOUTH + FACING_STEP, 20);
  Event_OpenMessage(ACTOR_FEH, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 1);
    WAIT(20);
    Event_ShowMessageAndWait(ACTOR_MESSENGER_STUDENT, 0, 20);
    FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI);
  } else {
    Event_SkipMessages(1);
    WAIT(20);
    OBJ_EMOTE(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    OBJ_DIR(ACTOR_FEH, FACING_SOUTH, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
    WAIT(20);
    Event_OpenMessage(ACTOR_MESSENGER_STUDENT, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_ActorSetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
      WAIT(20);
      OBJ_DIR(ACTOR_FEH, FACING_NORTHWEST + FACING_STEP, 20);
      Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
      FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI);
    } else {
      Event_SkipMessages(1);
      WAIT(20);
      Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
      WAIT(20);
      Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
      FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED);
    }
  }
  OBJ_DIR(ACTOR_KICKING_STUDENT, FACING_WEST, 0);
  OBJ_DIR(ACTOR_PUNCHING_STUDENT, FACING_WEST, 20);
  OBJ_MODE(ACTOR_KICKING_STUDENT, 5);
  OBJ_MODE(ACTOR_PUNCHING_STUDENT, 5);
  Actor_SetActionScript(ACTOR_JUMPING_STUDENT,
                        gFuchinTemple_JumpingStudentScript);
}

/// Talking to Feh in his school in Xian after the party declined: he asks again
/// to see Isaac's Chi.
/// "Warrior, will you show me your Chi?"
static void Event_FehAsksToSeeChi(void) {

  EV_BEGIN();
  MESSAGE_SET(MsgXianWarriorWillShowMeYour);
  MESSAGE_OPEN(ACTOR_FEH, 0);

  if (Engine_EventChooseYesNo(0, 0) == 0) {
    WAIT(20);
    Engine_EventShowMessageAndWait(ACTOR_FEH, 0, 20);
    Engine_GameFlagSet(FLAG_FEH_WAITS_TO_SEE_CHI);
    EV_END();
  } else {
    gEventWork->message += 1;
    Engine_EventShowMessageAndWait(ACTOR_FEH, 0, 20);
    EV_END();
  }
}

/// The tree falls from the line with Feh watching, but Feh sees that it is not
/// Chi. "Ya! The tree fell!" He asks whether the party has been to Fuchin
/// Temple, and the school goes back to its training.
static void FuchinTemple_FehSeesChi(void) {
  struct FieldActor *actor;

  // Stop the temporary facing hooks while the group reacts to the tree.
  actor = Actor_Get(ACTOR_FEH);
  actor->update = 0;
  actor = Object_GetById(ACTOR_PROUD_STUDENT);
  actor->update = 0;
  actor = Object_GetById(ACTOR_WATCHING_STUDENT);
  actor->update = 0;
  actor = Object_GetById(ACTOR_TREE_STUDENT);
  actor->update = 0;
  actor = Actor_Get(ACTOR_MESSENGER_STUDENT);
  actor->update = 0;
  OBJ_MODE(ACTOR_PUNCHING_STUDENT, ANIM_STAND);
  Camera_SetSpeed(0x8000, 0x1000);
  Camera_MoveTo(PIXELS(232), -1, PIXELS(200), 1);
  Engine_CameraWaitForMove();
  // Continue the Chi discussion with the students gathered around the tree.
  MESSAGE_SET(MsgXianYaTreeFell);
  OBJ_SPEED(ACTOR_KICKING_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_SPEED(ACTOR_JUMPING_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_WALK(ACTOR_KICKING_STUDENT, 152, 200);
  Actor_WalkToAndWait(ACTOR_JUMPING_STUDENT, 144, 248);
  Engine_ActorWaitForMove(ACTOR_KICKING_STUDENT);
  Actor_FaceActor(ACTOR_WELCOMING_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_PUNCHING_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_PROUD_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_WATCHING_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_TREE_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_TRAINING_TREE, 0);
  Actor_FaceActor(ACTOR_FEH, ACTOR_TRAINING_TREE, 0);
  OBJ_SPEED(ACTOR_KICKING_STUDENT, 3 * PIXELS(1) / 2, 3 * PIXELS(1) / 4);
  OBJ_SPEED(ACTOR_JUMPING_STUDENT, PIXELS(2), PIXELS(1));
  OBJ_WALK(ACTOR_KICKING_STUDENT, 152, 200);
  Actor_WalkToAndWait(ACTOR_JUMPING_STUDENT, 144, 248);
  Actor_FaceActor(ACTOR_JUMPING_STUDENT, ACTOR_TRAINING_TREE, 0);
  Engine_ActorWaitForMove(ACTOR_KICKING_STUDENT);
  Actor_FaceActor(ACTOR_KICKING_STUDENT, ACTOR_TRAINING_TREE, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_FEH, 2);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 40);
  Actor_FaceActor(ACTOR_WELCOMING_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_KICKING_STUDENT, ACTOR_FEH, 0);
  OBJ_DIR(ACTOR_PUNCHING_STUDENT, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceActor(ACTOR_JUMPING_STUDENT, ACTOR_FEH, 0);
  OBJ_DIR(ACTOR_PROUD_STUDENT, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceActor(ACTOR_WATCHING_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_TREE_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_MESSENGER_STUDENT, ACTOR_FEH, 0);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  OBJ_EMOTE(ACTOR_FEH, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  OBJ_EMOTE(ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  OBJ_EMOTE(ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  OBJ_EMOTE(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  OBJ_SPEED(ACTOR_TREE_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 216, 176);
  OBJ_DIR(ACTOR_TREE_STUDENT, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_TREE_STUDENT, 0, 20);
  OBJ_DIR(ACTOR_FEH, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_WELCOMING_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_KICKING_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_PUNCHING_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_JUMPING_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_PROUD_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_WATCHING_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_TREE_STUDENT, 2);
  Engine_ActorStartRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
  WAIT(40);
  Engine_ActorRunRepeatedMotion(ACTOR_PROUD_STUDENT, 2);
  Event_ShowMessageAndWait(ACTOR_PROUD_STUDENT, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  OBJ_DIR(ACTOR_FEH, FACING_SOUTH + FACING_STEP, 20);
  Event_AskYesNo(ACTOR_FEH, 0);
  OBJ_EMOTE(ACTOR_WELCOMING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(5);
  OBJ_EMOTE(ACTOR_KICKING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(5);
  OBJ_EMOTE(ACTOR_PUNCHING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(5);
  OBJ_EMOTE(ACTOR_JUMPING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(5);
  OBJ_EMOTE(ACTOR_PROUD_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(5);
  OBJ_EMOTE(ACTOR_WATCHING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(5);
  OBJ_EMOTE(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(5);
  OBJ_EMOTE(ACTOR_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(60);
  Engine_ActorRunRepeatedMotion(ACTOR_MESSENGER_STUDENT, 2);
  Event_ShowMessageAndWait(ACTOR_MESSENGER_STUDENT, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  OBJ_EMOTE(ACTOR_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(ACTOR_TREE_STUDENT, 0, 20);
  Actor_FaceActor(ACTOR_FEH, ACTOR_TREE_STUDENT, 0);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 40);
  Engine_ActorFaceEachOther(ACTOR_PUNCHING_STUDENT, ACTOR_KICKING_STUDENT, 0);
  Engine_ActorFaceEachOther(ACTOR_JUMPING_STUDENT, ACTOR_WATCHING_STUDENT, 0);
  Engine_ActorFaceEachOther(ACTOR_PROUD_STUDENT, ACTOR_TREE_STUDENT, 0);
  WAIT(60);
  Actor_FaceActor(ACTOR_KICKING_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_PUNCHING_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_JUMPING_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_PROUD_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_WATCHING_STUDENT, ACTOR_FEH, 0);
  Actor_FaceActor(ACTOR_TREE_STUDENT, ACTOR_FEH, 0);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FEH, 2);
  WAIT(20);
  OBJ_MODE(ACTOR_WELCOMING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_KICKING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_PUNCHING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_JUMPING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_PROUD_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_WATCHING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_TREE_STUDENT, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
  WAIT(20);
  OBJ_DIR(ACTOR_FEH, FACING_SOUTH + FACING_STEP, 20);
  Event_AskYesNo(ACTOR_FEH, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(ACTOR_WELCOMING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_KICKING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_PUNCHING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_JUMPING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_PROUD_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_WATCHING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_TREE_STUDENT, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(ACTOR_FEH, 2);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_FEH, ANIM_NOD);
  WAIT(20);
  OBJ_DIR(ACTOR_FEH, FACING_WEST, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FEH, 2);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEH, 0, 20);
  OBJ_MODE(ACTOR_WELCOMING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_KICKING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_PUNCHING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_JUMPING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_PROUD_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_WATCHING_STUDENT, ANIM_NOD);
  OBJ_MODE(ACTOR_TREE_STUDENT, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_MESSENGER_STUDENT, ANIM_NOD);
  WAIT(20);
  OBJ_WALK(ACTOR_KICKING_STUDENT, 120, 200);
  OBJ_WALK(ACTOR_JUMPING_STUDENT, 120, 248);
  Engine_ActorWaitForMove(ACTOR_KICKING_STUDENT);
  OBJ_DIR(ACTOR_PUNCHING_STUDENT, FACING_WEST, 20);
  OBJ_MODE(ACTOR_KICKING_STUDENT, 5);
  OBJ_MODE(ACTOR_PUNCHING_STUDENT, 5);
  Engine_ActorWaitForMove(ACTOR_JUMPING_STUDENT);
  Actor_SetActionScript(ACTOR_JUMPING_STUDENT,
                        gFuchinTemple_JumpingStudentScript);
  OBJ_SPEED(ACTOR_TREE_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 216, 168);
  Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 232, 168);
  OBJ_DIR(ACTOR_TREE_STUDENT, FACING_NORTH, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_TREE_STUDENT, 3);
  OBJ_POS_SET(ACTOR_TRAINING_TREE, 232, 168);
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  actor->y.fixed = 0xc0000;
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  actor->targetY = ACTOR_NO_TARGET;
  actor = Actor_Get(ACTOR_TRAINING_TREE);
  {
    struct AnimationObject *sprite = actor->sprite;
    s32 rotation = 0x8000;

    sprite->rotation = rotation;
  }
  SE(SE_124);
  WAIT(40);
  Actor_WalkToAndWait(ACTOR_TREE_STUDENT, 216, 152);
  OBJ_DIR(ACTOR_TREE_STUDENT, FACING_SOUTH, 30);
  GameFlag_Clear(FLAG_FEH_WAITS_TO_SEE_CHI);
  FLAG_SET(FLAG_FEH_SAW_KI);
}

/// Talking to a student (ACTOR_PUNCHING_STUDENT) in Master Feh's school in Xian
/// once Hsu is known to be trapped: the student asks about the flood. "Water
/// monsters flooded Altin. Did you defeat them, warrior?"
static void Event_StudentAsksAboutAltin(void) {
  EV_BEGIN();
  MESSAGE_SET(MsgXianWaterMonstersFloodedAltinDid);
  Engine_EventAskYesNo(ACTOR_PUNCHING_STUDENT, 0);
  EV_END();
}

/// Walking north into the door Feh and Feizhi came out of, in Master Feh's
/// school in Xian (trigger 3): it is locked until Feizhi has left; then it
/// opens and Isaac walks through (FUCHIN_TEMPLE_INTERIORS_TO_HOUSE). "It is
/// locked."
static void Event_OpenLockedDoor(void) {
  EV_BEGIN();
  if (FLAG_CHECK(FLAG_FEIZHI_WENT_TO_HSU) == 0 &&
      FLAG_CHECK(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
    Engine_MessageShowCentered(MsgXianItIsLocked, 1);
    EV_END();
  } else {
    SE(SE_DOOR_OPEN);
    Map_AnimateCells(gFuchinTemple_FehsDoorOpens, 78, 13);
    OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 306, 248);
    OBJ_WALK(ACTOR_PARTY_LEADER, 304, 216);
    WAIT(20);
    Engine_EventRequestExit(FUCHIN_TEMPLE_INTERIORS_TO_HOUSE);
    EV_END();
  }
}

/// Talking to the healer at Fuchin Temple (FuchinTempleGrounds): across the
/// counter from the south, the Sanctum's services; from elsewhere, a word on
/// the healer's travels. "I'm traveling around the world to spread good will."
static void Event_TalkToHealer(void) {
  struct FieldActor *leader;
  u32 facing;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  facing = leader->facing;
  EV_BEGIN();

  if (FACING_IS_NORTH(facing)) {
    Engine_SanctumOpen(ACTOR_HEALER);
  } else {
    MESSAGE_SET(MsgXianImTravelingAroundWorldSpread);
    MESSAGE_PUT(ACTOR_HEALER, 0);
  }

  EV_END();
}

/// Reading Nyunpa's mind inside Fuchin Temple once he is enlightened: he
/// entrusts the world to Isaac. "Isaac... Is that you...? I am... counting on
/// you... to bring peace... to the world..."
static void Event_ReadNyunpasLastWish(void) {
  EV_BEGIN();
  Engine_ActorRunRepeatedMotion(ACTOR_NYUNPA, 2);
  MESSAGE_SET(MsgXianIsaacAmCountingOnBring);
  MESSAGE_PUT(ACTOR_NYUNPA, 0);
  EV_END();
}

/// Selects the outdoor, school, or lesson event table.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_FuchinTempleGrounds) {
    return gFuchinTempleGroundsEvents;
  }
  if (gGameState.entrance == FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE) {
    return gFuchinTempleEventsFromLamaTemple;
  }
  return gFuchinTempleEvents;
}

/// Emits a falling particle beside the leader during the grotto entrance fade.
static void FuchinTemple_SpawnGrottoEntryParticle(void) {
  struct EffectOptions options;
  struct FieldActor *leader;
  s32 paletteDraw;
  s32 fallSpeed;

  leader = Object_GetById(ACTOR_PARTY_LEADER);

  options.palette = 7;
  paletteDraw = (u32)(Engine_RandomNext() * 7) >> 16;
  if ((paletteDraw & 7) == 0)
    options.palette = 5;

  options.startScaleX = 0xb333;
  options.startScaleY = 0xcccc;

  fallSpeed = ((u32)(Engine_RandomNext() * 8) >> 16) * 13107;

  FieldFx_Spawn(leader->x.fixed + ((8 - (gFrameCount & 15)) << 16),
                leader->y.fixed + (192 << 13), leader->z.fixed, 0, -fallSpeed,
                0, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);

  if ((gFrameCount & 1) != 0)
    Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  else
    Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 1);
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
/// finish; only the three unused hops above call it.
static UNUSED void FuchinTemple_HopLeaderByOffset(s32 offsetX, s32 offsetZ) {
  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, 5 * PIXELS(1) / 2, 5 * PIXELS(1) / 4);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, offsetX, offsetZ);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, 7);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_MODE(ACTOR_PARTY_LEADER, 6);
  EV_END();
}

/// Stepping into the waterfall grotto's entrance at Fuchin Temple
/// (FuchinTempleGrounds, trigger 3): spray falls over Isaac as the screen
/// fades, and he enters Fuchin Falls Cave.
static void Event_EnterWaterfallGrotto(void) {
  struct FieldActor *leader;
  SchedulerCallback particleTask;

  EV_BEGIN();
  // Keep the entry effect alive until the leader has moved into the fade.
  particleTask = FuchinTemple_SpawnGrottoEntryParticle;
  FieldScheduler_AddCallback(particleTask, TASK_PRIORITY_SCENE);
  Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
  gEventWork->transitionFrames = 60;
  Engine_EventCloseScreen();
  Engine_AudioPlayCue(SE_CAST);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_WALK);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(leader, 0);
  Scheduler_RemoveCallback(particleTask);
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE);
  EV_END();
}

/// Stepping south onto trigger 1 inside Fuchin Temple (FuchinTempleInteriors):
/// the party leaves by FUCHIN_TEMPLE_INTERIORS_TO_FUCHIN_TEMPLE_GROUNDS, out to
/// FuchinTempleGrounds.
static void Event_LeaveTemple(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(FUCHIN_TEMPLE_INTERIORS_TO_FUCHIN_TEMPLE_GROUNDS);
}

/// Walking north into the temple door at Fuchin Temple (FuchinTempleGrounds,
/// trigger 2): it opens, and Isaac walks inside, where Nyunpa meditates.
static void Event_EnterTemple(void) {
  EV_BEGIN();
  SE(SE_188);
  Map_AnimateCells(gFuchinTemple_TempleDoorOpens, 77, 8);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  OBJ_SPEED(ACTOR_PARTY_LEADER, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  WAIT(16);
  Engine_EventRequestExit(FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_TEMPLE_INTERIORS);
  EV_END();
}

/// Back inside Fuchin Temple from the grotto with the Orb of Force
/// (FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND), Nyunpa praises Isaac. "Excellent,
/// Isaac! I am glad to see you master the secrets of Fuchin Temple!" Nyunpa
/// names the power Ki, "force" in the western world, and says it will show the
/// monsters hiding in Mogall Forest.
static void FuchinTemple_NyunpaTeachesKi(void) {
  struct FieldActor *nyunpa;

  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJ_WALK(ACTOR_PARTY_LEADER, 168, 504);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  WAIT(20);
  Engine_ActorStartRepeatedMotion(ACTOR_NYUNPA, 2);
  Actor_SetAttachedEffect(ACTOR_NYUNPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  WAIT(60);
  Object_GetById(ACTOR_NYUNPA)->paused = 0;
  SE(SE_JUMP);
  nyunpa = Actor_Get(ACTOR_NYUNPA);
  nyunpa->velocityY = 0x80000;
  OBJ_MODE(ACTOR_NYUNPA, ANIM_STAND);
  // Praise the completed test before answering the follow-up questions.
  MESSAGE_SET(MsgXianExcellentIsaac);
  Event_ShowMessageAndWait(ACTOR_NYUNPA, 0, 20);
  Event_ShowMessageAndWait(ACTOR_NYUNPA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_NYUNPA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_OpenMessage(ACTOR_NYUNPA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    WAIT(10);
    Engine_ActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
    WAIT(20);
    Event_ShowMessageAndWait(ACTOR_NYUNPA, 0, 20);
    gEventWork->message += 2;
  } else {
    WAIT(10);
    Engine_ActorRunRepeatedMotion(ACTOR_NYUNPA, 2);
    WAIT(20);
    gEventWork->message++;
    Event_ShowMessageAndWait(ACTOR_NYUNPA, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
    WAIT(20);
    Event_ShowMessageAndWait(ACTOR_NYUNPA, 0, 20);
  }
  Event_ShowMessageAndWait(ACTOR_NYUNPA, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_NYUNPA, 2);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_NYUNPA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_OpenMessage(ACTOR_NYUNPA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    WAIT(10);
    OBJ_EMOTE(ACTOR_NYUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    MESSAGE_SET(MsgXianCanReadMindsKnowCurious);
    Event_OpenMessage(ACTOR_NYUNPA, 0);
    do {
      if (Engine_EventChooseYesNo(0, 0) != 1) {
        break;
      }
      WAIT(10);
      OBJ_EMOTE(ACTOR_NYUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      MESSAGE_SET(MsgXianDontTryHard);
      Event_OpenMessage(ACTOR_NYUNPA, 0);
    } while (1);
  }
  // Explain why hidden monsters make the new ability useful.
  MESSAGE_SET(MsgXianMonstersWaitInHidingWould);
  WAIT(10);
  Engine_ActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
  WAIT(20);
  Event_OpenMessage(ACTOR_NYUNPA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    WAIT(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    WAIT(20);
    Event_ShowMessageAndWait(ACTOR_NYUNPA, 0, 20);
    gEventWork->message++;
  } else {
    WAIT(10);
    Engine_ActorRunRepeatedMotion(ACTOR_NYUNPA, 2);
    gEventWork->message++;
    Event_ShowMessageAndWait(ACTOR_NYUNPA, 0, 20);
  }
  Engine_ActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_NYUNPA, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_NYUNPA, 2);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_NYUNPA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_NYUNPA, ANIM_NOD);
  WAIT(20);
  OBJ_MODE(ACTOR_NYUNPA, 5);
  FLAG_SET(FLAG_NYUNPA_TAUGHT_FORCE);
  EV_END();
}

/// Entering Master Hama's room in Lama Temple the first time, the party is
/// greeted. "So, Isaac, you and your Adept companions have finally come." Hama
/// tells Ivan he is a Jupiter Adept like her and means to teach him Reveal for
/// crossing the Lamakan Desert. Feizhi bursts in: Hsu is in great danger among
/// the boulders in the mountains. Hama gives Ivan Reveal and hurries with her
/// to his aid, and the party is left to choose between helping Hsu and
/// following the people it chases (FLAG_XIAN_HEARD_HSU_TRAPPED).
static void FuchinTemple_RunHamasRevealLesson(void) {
  struct FieldActor *leader;

  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, 2 * PIXELS(1) / 5, PIXELS(1) / 5);
  OBJ_WALK(ACTOR_PARTY_LEADER, 472, 536);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_DIR(ACTOR_HAMAS_BOY_MONK, FACING_EAST, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_HAMAS_BOY_MONK, 2);
  WAIT(20);
  MESSAGE_SET(MsgXianMasterHamaMeditatingPleaseExtremely);
  Event_ShowMessageAndWait(ACTOR_HAMAS_BOY_MONK, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(30);
  OBJ_DIR(ACTOR_HAMA, FACING_SOUTH, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  WAIT(20);
  OBJ_DIR(ACTOR_HAMAS_BOY_MONK, FACING_NORTH + FACING_STEP, 0);
  // Bring the other party members out from the leader for the lesson.
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
  }
  OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJ_SPEED(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJ_SPEED(IVAN, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJ_SPEED(MIA, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJ_WALK(ACTOR_PARTY_LEADER, 464, 504);
  OBJ_WALK(IVAN, 480, 504);
  OBJ_WALK(GARET, 496, 496);
  OBJ_WALK(MIA, 448, 496);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorWaitForMove(IVAN);
  Engine_ActorWaitForMove(MIA);
  OBJ_DIR(MIA, FACING_NORTHEAST, 0);
  Engine_ActorWaitForMove(GARET);
  OBJ_DIR(GARET, FACING_NORTHWEST, 0);
  WAIT(20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  OBJ_DIR(GARET, FACING_SOUTH + FACING_STEP, 20);
  Event_AskYesNo(GARET, 0);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_DIR(GARET, FACING_NORTHWEST, 20);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  OBJ_DIR(ACTOR_HAMA, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 0);
  OBJ_DIR(MIA, FACING_SOUTHEAST, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Actor_SetAttachedEffect(IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  WAIT(60);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(GARET, FACING_NORTHWEST, 0);
  OBJ_DIR(MIA, FACING_NORTHEAST, 0);
  WAIT(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  WAIT(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IVAN, 50);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(IVAN, FACING_NORTH, 30);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_DIR(ACTOR_HAMA, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 0);
  WAIT(30);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(GARET, FACING_NORTHWEST, 20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Actor_FaceActor(ACTOR_HAMA, IVAN, 0);
  WAIT(10);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_FaceActor(MIA, IVAN, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  WAIT(20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  WAIT(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  OBJ_EMOTE(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceActor(ACTOR_HAMA, GARET, 0);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_FaceActor(ACTOR_HAMA, GARET, 0);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(GARET, FACING_NORTHWEST, 0);
  OBJ_DIR(MIA, FACING_NORTHEAST, 0);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  WAIT(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  WAIT(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_DIR(ACTOR_HAMA, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_HAMA, 2);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  WAIT(60);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(GARET, 0, 20);
  OBJ_EMOTE(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_HAMA, 1);
  WAIT(60);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  OBJ_DIR(GARET, FACING_SOUTH + FACING_STEP, 20);
  Event_AskYesNo(GARET, 0);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  WAIT(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  OBJ_DIR(GARET, FACING_NORTHWEST, 20);
  OBJ_DIR(ACTOR_HAMA, FACING_NORTH, 20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(20);
  OBJ_DIR(ACTOR_HAMA, FACING_SOUTH, 20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  WAIT(60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_DIR(ACTOR_HAMA, FACING_SOUTH + FACING_STEP, 20);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  WAIT(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, GARET, 0);
  Actor_FaceActor(IVAN, GARET, 0);
  Actor_FaceActor(MIA, GARET, 0);
  WAIT(20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(60);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(IVAN, FACING_NORTH, 0);
  OBJ_DIR(MIA, FACING_NORTH + FACING_STEP, 0);
  WAIT(20);
  Event_AskYesNo(ACTOR_HAMA, 0);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  WAIT(60);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 20);
  Event_AskYesNo(GARET, 0);
  WAIT(20);
  OBJ_DIR(MIA, FACING_SOUTHEAST, 20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_DIR(GARET, FACING_NORTHWEST, 0);
  OBJ_DIR(MIA, FACING_NORTHEAST, 30);
  OBJ_DIR(ACTOR_HAMA, FACING_EAST, 20);
  OBJ_EMOTE(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  OBJ_DIR(ACTOR_HAMA, FACING_NORTH, 20);
  OBJ_EMOTE(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(ACTOR_HAMA, 1);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 0);
  OBJ_DIR(IVAN, FACING_WEST, 0);
  OBJ_DIR(MIA, FACING_SOUTHEAST, 0);
  WAIT(60);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(GARET, FACING_NORTHWEST, 0);
  OBJ_DIR(IVAN, FACING_NORTH, 0);
  OBJ_DIR(MIA, FACING_NORTHEAST, 0);
  WAIT(30);
  OBJ_DIR(ACTOR_HAMA, FACING_SOUTH, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  // Interrupt the discussion with the arrival from the southern path.
  OBJ_POS_SET(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 608);
  Event_ShowMessageAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJ_EMOTE(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJ_EMOTE(ACTOR_HAMAS_BOY_MONK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  WAIT(60);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  OBJ_DIR(GARET, FACING_SOUTH, 0);
  OBJ_DIR(IVAN, FACING_SOUTH, 0);
  OBJ_DIR(MIA, FACING_SOUTH, 0);
  OBJ_DIR(ACTOR_HAMAS_BOY_MONK, FACING_EAST, 0);
  WAIT(30);
  OBJ_SPEED(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 536);
  OBJ_EMOTE(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_DIR(IVAN, FACING_NORTH, 40);
  Actor_WalkToAndWait(IVAN, 488, 512);
  WAIT(10);
  OBJ_DIR(IVAN, FACING_SOUTHWEST, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 30);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 512);
  WAIT(10);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 2);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  OBJ_EMOTE(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 2);
  Actor_SetAttachedEffect(ACTOR_FEIZHI_IN_LAMA_TEMPLE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  WAIT(60);
  Event_ShowMessageAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  OBJ_EMOTE(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_EMOTE(ACTOR_FEIZHI_IN_LAMA_TEMPLE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
            60);
  Event_ShowMessageAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_HAMA, 1);
  OBJ_EMOTE(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  OBJ_DIR(GARET, FACING_NORTHWEST, 0);
  OBJ_DIR(IVAN, FACING_NORTHWEST, 0);
  OBJ_DIR(MIA, FACING_NORTHEAST, 0);
  WAIT(40);
  Engine_ActorSetAnimationAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_EMOTE(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  // BUG: ACTOR_FEIZHI is Feizhi's number in the school's table. Hama's room
  // places no actor at that number, so Feizhi does not bounce before she
  // speaks.
#ifdef BUGFIX
  Engine_ActorStartRepeatedMotion(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 1);
#else
  Engine_ActorStartRepeatedMotion(ACTOR_FEIZHI, 1);
#endif
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  OBJ_EMOTE(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_EMOTE(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(2);
  OBJ_MODE(IVAN, ANIM_NOD);
  WAIT(1);
  OBJ_MODE(MIA, ANIM_NOD);
  WAIT(5);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Actor_FaceActor(ACTOR_HAMA, ACTOR_PARTY_LEADER, 0);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_SPEED(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  Actor_WalkToAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 504);
  WAIT(20);
  Engine_ActorStartRepeatedMotion(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 1);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  Actor_FaceActor(ACTOR_HAMA, ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0);
  OBJ_SPEED(ACTOR_HAMA, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 0);
  OBJ_DIR(IVAN, FACING_SOUTHWEST, 0);
  OBJ_DIR(MIA, FACING_SOUTHEAST, 0);
  Engine_ActorWaitForMove(ACTOR_HAMA);
  WAIT(30);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_WALK(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 568);
  Actor_WalkToAndWait(ACTOR_HAMA, 472, 536);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_ActorWaitForMove(ACTOR_FEIZHI_IN_LAMA_TEMPLE);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_HAMA, 2);
  OBJ_EMOTE(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  OBJ_DIR(ACTOR_FEIZHI_IN_LAMA_TEMPLE, FACING_NORTH + FACING_STEP, 0);
  OBJ_DIR(ACTOR_HAMA, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Actor_WalkToAndWait(ACTOR_HAMA, 472, 512);
  OBJ_DIR(ACTOR_HAMA, FACING_EAST, 0);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  OBJ_DIR(IVAN, FACING_WEST, 0);
  OBJ_DIR(ACTOR_HAMAS_BOY_MONK, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  WAIT(20);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 448, 512);
  WAIT(1);
  {
    struct FieldActor *leaderActor = Actor_Get(ACTOR_PARTY_LEADER);
    leaderActor->steeringFlags =
        ACTOR_FACES_MOTION | leaderActor->steeringFlags;
  }
  WAIT(20);
  // Finish Ivan's lesson before adding Reveal to his ability list.
  FuchinTemple_AnimateRevealLesson();
  WAIT(60);
  Party_LearnAbility(IVAN, ABILITY_REVEAL);
  // Let Hama and her monk leave, then gather the party into the leader.
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 0);
  OBJ_DIR(IVAN, FACING_SOUTHWEST, 0);
  OBJ_DIR(MIA, FACING_SOUTHEAST, 0);
  OBJ_DIR(ACTOR_HAMAS_BOY_MONK, FACING_SOUTHEAST + FACING_STEP, 0);
  WAIT(20);
  Actor_WalkToAndWait(ACTOR_HAMA, 472, 552);
  Engine_ActorRunRepeatedMotion(ACTOR_HAMA, 2);
  WAIT(20);
  OBJ_DIR(ACTOR_HAMA, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  WAIT(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_SPEED(ACTOR_HAMA, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkToAndWait(ACTOR_HAMA, 480, 540);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  OBJ_SPEED(ACTOR_HAMA, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_SPEED(ACTOR_HAMAS_BOY_MONK, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJ_WALK(ACTOR_HAMA, 472, 608);
  OBJ_WALK(ACTOR_HAMAS_BOY_MONK, 472, 544);
  Actor_WalkToAndWait(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 472, 608);
  Engine_ActorWaitForMove(ACTOR_HAMAS_BOY_MONK);
  OBJ_WALK(ACTOR_HAMAS_BOY_MONK, 472, 608);
  OBJ_POS_SET(ACTOR_FEIZHI_IN_LAMA_TEMPLE, 0, 0);
  Engine_ActorWaitForMove(ACTOR_HAMA);
  OBJ_POS_SET(ACTOR_HAMA, 0, 0);
  Engine_ActorWaitForMove(ACTOR_HAMAS_BOY_MONK);
  OBJ_POS_SET(ACTOR_HAMAS_BOY_MONK, 0, 0);
  Actor_WalkToAndWait(IVAN, 488, 520);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  WAIT(20);
  OBJ_DIR(MIA, FACING_SOUTHEAST, 20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  OBJ_DIR(IVAN, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  OBJ_DIR(GARET, FACING_SOUTHWEST, 20);
  Event_AskYesNo(GARET, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(20);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  WAIT(20);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  OBJ_DIR(GARET, FACING_WEST, 0);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_OpenMessage(MIA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    WAIT(20);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    WAIT(20);
    Event_ShowMessageAndWait(GARET, 0, 20);
    gEventWork->message++;
  } else {
    WAIT(20);
    gEventWork->message++;
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    WAIT(20);
    Event_ShowMessageAndWait(MIA, 0, 20);
  }
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Event_ShowMessageAndWait(GARET, 0, 20);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  WAIT(20);
  OBJ_WALK(GARET, 448, 512);
  Actor_WalkToAndWait(MIA, 448, 512);
  OBJ_POS_SET(MIA, 0, 0);
  Engine_ActorWaitForMove(GARET);
  OBJ_POS_SET(GARET, 0, 0);
  WAIT(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 20);
  WAIT(30);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Actor_SetAttachedEffect(IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  WAIT(60);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  WAIT(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_WalkToAndWait(IVAN, 448, 512);
  OBJ_POS_SET(IVAN, 0, 0);
  EV_END();
  FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED);
}

/// Inside Fuchin Temple, Nyunpa meditates, or teaches Ki after the test; in
/// Master Feh's school in Xian the tree and students stand as the story left
/// them; in Lama Temple, Hama teaches Reveal on the first visit and is away
/// until Hsu is rescued. At Fuchin Temple (FuchinTempleGrounds) the monk
/// beneath the falls stands where the story left him.
s32 Scene_Initialize(void) {
  struct FieldActor *trainingTree;
  s32 heardHsuTrapped;

  if (gGameState.scene == (s32)&SceneId_FuchinTempleInteriors) {
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    if (gGameState.entrance ==
        FUCHIN_TEMPLE_INTERIORS_FROM_FUCHIN_TEMPLE_GROUNDS) {
      if (Engine_GameFlagIsSet(FLAG_NYUNPA_ENLIGHTENED)) {
        OBJ_MODE(ACTOR_NYUNPA, 6);
      } else {
        OBJ_MODE(ACTOR_NYUNPA, 5);
        if (Engine_GameFlagIsSet(FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND) &&
            !Engine_GameFlagIsSet(FLAG_NYUNPA_TAUGHT_FORCE) &&
            !Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
          FuchinTemple_NyunpaTeachesKi();
        }
      }
    } else if (gGameState.entrance == FUCHIN_TEMPLE_INTERIORS_FROM_XIAN ||
               gGameState.entrance == FUCHIN_TEMPLE_INTERIORS_FROM_HOUSE) {
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
      heardHsuTrapped = Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED);
      if (heardHsuTrapped == 0) {
        trainingTree = Object_GetById(ACTOR_TRAINING_TREE);
        trainingTree->motionFlags = heardHsuTrapped;
        trainingTree->y.fixed = 0xc0000;
        trainingTree->targetY = 0xc0000;
        trainingTree->scaleX = 0xcccc;
        trainingTree->scaleY = 0x8000;
        trainingTree->sprite->rotation = 0x8000;
        if (Engine_GameFlagIsSet(FLAG_FEIZHI_WENT_TO_HSU)) {
          Engine_ActorSetPosition(ACTOR_FEH, PIXELS(248), PIXELS(208));
          if (!Engine_GameFlagIsSet(FLAG_FEH_SAW_KI)) {
            Engine_ActorSetPosition(ACTOR_MESSENGER_STUDENT, PIXELS(256),
                                    PIXELS(240));
            Object_GetById(ACTOR_FEH)->update = FuchinTemple_FaceLeader;
            Object_GetById(ACTOR_PROUD_STUDENT)->update =
                FuchinTemple_FaceLeader;
            Object_GetById(ACTOR_WATCHING_STUDENT)->update =
                FuchinTemple_FaceLeader;
            Object_GetById(ACTOR_TREE_STUDENT)->update =
                FuchinTemple_FaceLeader;
            Object_GetById(ACTOR_MESSENGER_STUDENT)->update =
                FuchinTemple_FaceLeader;
          }
        }
      } else {
        trainingTree = Object_GetById(ACTOR_TRAINING_TREE);
        trainingTree->motionFlags = 0;
        trainingTree->y.fixed = 0xc0000;
        trainingTree->targetY = 0xc0000;
        trainingTree->scaleX = 0xcccc;
        trainingTree->scaleY = 0x8000;
        trainingTree->collisionFlags |= ACTOR_NOT_TARGETED;
        trainingTree->sprite->rotation = 0x8000;
        Map_CopyCellAttributes(14, 11, 1, 1, 14, 10);
      }
      OverlayObject_PrepareObject(PIXELS(304), PIXELS(24), PIXELS(224),
                                  SPRITE_CRATE);
      OBJ_MODE(ACTOR_KICKING_STUDENT, 5);
      OBJ_MODE(ACTOR_PUNCHING_STUDENT, 5);
    } else if (gGameState.entrance ==
               FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE) {
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
      if (!Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
        FuchinTemple_RunHamasRevealLesson();
      } else if (!Engine_GameFlagIsSet(FLAG_HSU_RESCUED)) {
        Engine_ActorSetPosition(ACTOR_HAMA, 0, 0);
        Engine_ActorSetPosition(ACTOR_HAMAS_BOY_MONK, 0, 0);
      }
    }
  } else {
    FieldFx_SetQueuedSoundAndPlay(SE_170);
    Object_GetById(ACTOR_FALLS_MONK)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
    if (gGameState.entrance ==
            FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE &&
        Engine_GameFlagIsSet(FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND) &&
        !Engine_GameFlagIsSet(FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST)) {
      Map_CopyCellAttributes(10, 84, 1, 1, 10, 24);
    }
    if (Engine_GameFlagIsSet(FLAG_FUCHIN_FALLS_GROTTO_OPENED)) {
      OBJ_POS_SET(ACTOR_FALLS_MONK, 152, 392);
      Engine_ActorFaceDirection(ACTOR_FALLS_MONK, FACING_EAST, 0);
      Map_CopyCellAttributes(10, 26, 1, 1, 10, 22);
    }
  }
  return 0;
}

/// Starts Mind Read from caster on target as part of a cutscene, without the
/// window that announces its name. Nyunpa's Mind Read events carry
/// PSYNERGY_SCRIPT_PLAYS_EFFECT, so the field plays nothing of its own.
static void FuchinTemple_BeginMindReadDemonstration(s32 caster, s32 target) {
  struct FieldFxScene *effect;

  Engine_GameFlagSet(FLAG_PSYNERGY_EFFECT);
  Engine_PsynergyBegin(ABILITY_MIND_READ, 1);
  effect = gEffectWork;
  Engine_PsynergySetTarget(caster, target);
  effect->active = 0;
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(PSYNERGY_STEP_ON_TARGET);
  Engine_TaskWait(1);
}

/// Finishes the demonstration's effect and lowers the caster's hands.
static void FuchinTemple_FinishMindReadDemonstration(void) {
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_PsynergyPlayEffect(PSYNERGY_STEP_FINISH);
  Engine_PsynergyLowerHands();
}

/// Emits eight rising particles from Nyunpa, sounding every other shot.
static void FuchinTemple_SpawnNyunpasParticles(void) {
  struct EffectOptions options;
  struct FieldActor *nyunpa;
  u32 shotIndex;

  nyunpa = Object_GetById(ACTOR_NYUNPA);
  options.priority = 1;
  options.type = 0x0119;
  options.script = gFuchinTemple_ShotScript;
  options.targetScaleX = 224 << 10;
  options.targetScaleY = 192 << 9;
  for (shotIndex = 0; shotIndex < 8; shotIndex++) {
    WAIT(10);
    if (shotIndex & 1) {
      Engine_AudioPlayCue(SE_130);
    }
    FieldFx_Spawn(nyunpa->x.fixed, nyunpa->y.fixed,
                  nyunpa->z.fixed + 0xffe80000, 0, 0x9999, 0,
                  EFFECT_USE_SCRIPT | EFFECT_USE_TYPE | EFFECT_SCALE_TO_TARGET |
                      EFFECT_USE_PRIORITY | 1,
                  &options);
  }
  WAIT(60);
}

/// Hama's and Ivan's update hook during the Reveal lesson: their child value
/// alternates between 10 and 9 every two frames.
static void FuchinTemple_AlternateLessonChildEffect(union FieldObject *object) {
  if ((*(u32 *)&gFrameCount >> 1) & 1) {
    ObjectGroup_SetChildValue((struct FieldActor *)object, 10);
  } else {
    ObjectGroup_SetChildValue((struct FieldActor *)object, 9);
  }
}

/// Animates Hama and Ivan's lesson effects, fades the palette and clears the
/// callbacks.
static void FuchinTemple_AnimateRevealLesson(void) {
  struct EffectOptions options;
  struct FieldActor *hama;
  u32 particleIndex;
  s32 particleX;
  s32 particleY;
  s32 riseSpeed;

  Engine_AudioPlayCue(SE_131);
  Object_GetById(ACTOR_HAMA)->update = FuchinTemple_AlternateLessonChildEffect;
  WAIT(40);
  Engine_ColorBufferApplySource(PALETTE_TRANSFORM_PLAIN, PALETTE_HALVES_BOTH);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_TINT | RGB(20, 2, 23),
                                PALETTE_HALF_BG);
  Engine_ColorBufferInterpolate(60);
  WAIT(40);
  Engine_AudioPlayCue(SE_131);
  Object_GetById(IVAN)->update = FuchinTemple_AlternateLessonChildEffect;
  WAIT(120);
  hama = Object_GetById(ACTOR_HAMA);
  // Emit the lesson particles while both temporary child effects run.
  options.priority = 1;
  options.palette = 2;
  options.type = 0x011d;
  for (particleIndex = 0; particleIndex < 64; particleIndex++) {
    if ((particleIndex & 3) == 0) {
      Engine_AudioPlayCue(SE_246);
    }
    particleX = hama->x.fixed +
                ((((u32)(Engine_RandomNext() * 3) << 4) >> 16) << 16) +
                0xfff40000;
    particleY = hama->y.fixed +
                ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + 0xfff00000;
    riseSpeed = ((((u32)Engine_RandomNext() << 2) >> 16) << 15) + (128 << 8);
    FieldFx_Spawn(particleX, particleY, hama->z.fixed, 0, riseSpeed, 0,
                  EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                  &options);
    Engine_TaskWait(2);
  }
  Engine_AudioPlayCue(SE_220);
  WAIT(30);
  // Fade back and remove both hooks before the conversation resumes.
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, PALETTE_HALF_BG);
  Engine_ColorBufferInterpolate(60);
  WAIT(40);
  Object_GetById(ACTOR_HAMA)->update = NULL;
  Object_GetById(IVAN)->update = NULL;
  Engine_ActorSetChildValue(ACTOR_HAMA, 0);
  Engine_ActorSetChildValue(IVAN, 0);
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gFuchinTemple_JumpingStudentScript[] = {
    SC_ARG(0),
    SC_SET(SW_GRAVITY, 0x9999),
    SC_LABEL(1),
    SC_SET(SW_VELOCITY_Y, 0x30000),
    SC_WAIT(30),
    SC_SET(SW_VELOCITY_Y, 0x40000),
    SC_WAIT(30),
    SC_SET(SW_VELOCITY_Y, 0x50000),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_SOUTH + FACING_STEP),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(2),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
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
// walks in (Event_EnterTemple), and the door Feh and Feizhi come out of in
// Master Feh's school, opening and closing behind them.
u16 gFuchinTemple_TempleDoorOpens[] = {
    MAP_CELL_STEP(66, 46, 3, 2, 5),
    MAP_CELL_STEP(63, 46, 3, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gFuchinTemple_FehsDoorOpens[] = {
    MAP_CELL_STEP(62, 19, 2, 2, 5),
    MAP_CELL_STEP(64, 19, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gFuchinTemple_FehsDoorCloses[] = {
    MAP_CELL_STEP(62, 19, 2, 2, 5),
    MAP_CELL_STEP(60, 19, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

struct SceneEntrance gFuchinTempleGroundsEntrances[] = {
    {0, CONDITION_ALWAYS, 296, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {FUCHIN_TEMPLE_GROUNDS_FROM_WORLD_MAP, CONDITION_ALWAYS, 296, 0, 504,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_TEMPLE_INTERIORS, CONDITION_ALWAYS, 296,
     0, 304, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS,
     168, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 232, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gFuchinTempleEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 168, 0, 552, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {FUCHIN_TEMPLE_INTERIORS_FROM_FUCHIN_TEMPLE_GROUNDS, CONDITION_ALWAYS, 168,
     0, 552, FACING_NORTH, 0, 48, 384, 288, 576, 0},
    {FUCHIN_TEMPLE_INTERIORS_FROM_XIAN, CONDITION_ALWAYS, 256, 0, 264,
     FACING_NORTH, 0, 48, 80, 368, 288, 0},
    {FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE, CONDITION_ALWAYS, 472, 0, 552,
     FACING_NORTH, 0, 352, 384, 592, 576, 0},
    {FUCHIN_TEMPLE_INTERIORS_FROM_HOUSE, CONDITION_ALWAYS, 304, 0, 248,
     FACING_SOUTH, 0, 48, 80, 368, 288, 0},
    {SCENE_TABLE_END},
};

u32 gFuchinTempleExits[] = {
    SCENE_EXITS(FuchinTempleGrounds),
    SCENE_EXIT(FUCHIN_TEMPLE_GROUNDS_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_FUCHIN_TEMPLE_GROUNDS),
    SCENE_EXIT(FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_TEMPLE_INTERIORS,
               FuchinTempleInteriors,
               FUCHIN_TEMPLE_INTERIORS_FROM_FUCHIN_TEMPLE_GROUNDS),
    SCENE_EXIT(FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE,
               FuchinFallsCaveEntrance,
               FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_TEMPLE_GROUNDS),
    SCENE_EXITS(FuchinTempleInteriors),
    SCENE_EXIT(FUCHIN_TEMPLE_INTERIORS_TO_FUCHIN_TEMPLE_GROUNDS,
               FuchinTempleGrounds,
               FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_TEMPLE_INTERIORS),
    SCENE_EXIT(FUCHIN_TEMPLE_INTERIORS_TO_XIAN, Xian,
               XIAN_FROM_FUCHIN_TEMPLE_INTERIORS),
    SCENE_EXIT(FUCHIN_TEMPLE_INTERIORS_TO_LAMA_TEMPLE, LamaTemple,
               LAMA_TEMPLE_FROM_FUCHIN_TEMPLE_INTERIORS),
    SCENE_EXIT(FUCHIN_TEMPLE_INTERIORS_TO_HOUSE, XianInteriors,
               XIAN_INTERIORS_FROM_FUCHIN_TEMPLE_INTERIORS),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gFuchinTempleGroundsPlacements[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(296), 0, PIXELS(440), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MONK_IN_WHITE_AND_ORANGE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(392), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gFuchinTemplePlacements[] = {
    // Nyunpa
    { SPRITE_NYUNPA, 0x0, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(472), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    // Master Feh's students
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(264), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUTH_WITH_HEADBAND, 0x0, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(200), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUTH_WITH_TIED_HAIR, 0x0, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUTH_WITH_TIED_HAIR, 0x0, (s32)gFuchinTemple_JumpingStudentScript,
        PIXELS(120), 0, PIXELS(248), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUTH_WITH_TIED_HAIR, 0x0, ACTOR_STAND,
        PIXELS(176), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, 0x0, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(264), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUTH_WITH_HEADBAND, 0x0, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(152), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUTH_WITH_HEADBAND, 0x0, ACTOR_STAND,
        PIXELS(176), 0, PIXELS(216), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    // Feizhi and Master Feh, out of sight until they come out
    { SPRITE_FEIZHI, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_FEH, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    // The training tree, and where Force strikes it
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, (s32)gFuchinTemple_HiddenSpriteScript,
        PIXELS(232), 0, PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gFuchinTemplePlacementsFromLamaTemple[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    // Hama
    { SPRITE_HAMA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(472), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(536), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_FEIZHI, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gFuchinTempleGroundsEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, FUCHIN_TEMPLE_GROUNDS_TO_WORLD_MAP },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterTemple) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterWaterfallGrotto) },
    { EVENT_TOUCH, 5, FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST, EVENT_SCRIPT(Event_MonkAsksWhetherTestIsDone) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_FROM_KOLIMA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleTheFuchinTempleIsAPleasant) },
    { EVENT_TALK, ACTOR_FALLS_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleOhIsaacWhatTookYouSo) },
    { EVENT_TALK, ACTOR_BOY_MONK_READING_OMENS, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleAhhhMasterNyunpaHeHasPassed) },
    { EVENT_TALK, ACTOR_WORRIED_BOY_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleMasterNyunpaHasWastedAwayAt) },
    { EVENT_TALK, ACTOR_ADMIRING_BOY_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), EVENT_SCRIPT(Event_BoyMonkSpeaksOfNyunpa) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_FROM_KOLIMA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_YoungManFromKolimaAsksAboutBridge) },
    // Unused: a copy of the row above, which always answers first.
    { EVENT_TALK, ACTOR_YOUNG_MAN_FROM_KOLIMA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_YoungManFromKolimaAsksAboutBridge) },
    { EVENT_TALK, ACTOR_FALLS_MONK, FLAG_NYUNPA_TEST_ACCEPTED, MSGNO(MsgFuchinTempleOmOmBlueLightsInThe) },
    { EVENT_TALK, ACTOR_FALLS_MONK, FLAG_FUCHIN_FALLS_GROTTO_OPENED, EVENT_SCRIPT(Event_MonkLetsPartyIntoGrotto) },
    { EVENT_TALK, ACTOR_FALLS_MONK, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND, MSGNO(MsgFuchinTempleTheTestOfFuchinFallsIs) },
    { EVENT_TALK, ACTOR_FALLS_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterNyunpaIsWaitingForYou) },
    { EVENT_TALK, ACTOR_BOY_MONK_READING_OMENS, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleGemsFromTheSkyBlueLights) },
    { EVENT_TALK, ACTOR_WORRIED_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleNyunpaHasNotEatenInDays) },
    { EVENT_TALK, ACTOR_ADMIRING_BOY_MONK, CONDITION_ALWAYS, EVENT_SCRIPT(Event_BoyMonkSpeaksOfNyunpa) },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealer) },
    { SEARCH(SEARCH_UNNAMED), 110, FLAG_PLACE_TEMP_50, FIND_PSYNERGY_STONE },
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
    { SCENE_EVENTS_END },
};

struct SceneEvent gFuchinTempleEvents[] = {
    { TOUCH_FACING(FACING_SOUTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveTemple) },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, FUCHIN_TEMPLE_INTERIORS_TO_XIAN },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenLockedDoor) },
    { EVENT_TALK, ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTemple) },
    { EVENT_TALK, ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TAUGHT_FORCE), MSGNO(MsgFuchinTempleUseThisEnergyToFindThe) },
    { EVENT_TALK, ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TEST_ACCEPTED), MSGNO(MsgFuchinTempleSpeakToTheMonkStandingBeneath) },
    { EVENT_TALK, ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TEST_DECLINED), EVENT_SCRIPT(Event_NyunpaOffersTest) },
    { EVENT_TALK, ACTOR_NYUNPA, CONDITION_ALWAYS, MSGNO(MsgXianNyunpaMeditates) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), EVENT_SCRIPT(Event_ReadNyunpasLastWish) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TAUGHT_FORCE), MSGNO(MsgFuchinTempleNowThatIHavePassedMy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_SCRIPT_PLAYS_EFFECT | ACTOR_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TEST_ACCEPTED), EVENT_SCRIPT(Event_NyunpaFeelsMindRead) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_SCRIPT_PLAYS_EFFECT | ACTOR_NYUNPA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_NyunpaOffersTest) },
    { EVENT_TALK, ACTOR_WELCOMING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleThisIsMasterFehsKungFu) },
    { EVENT_TALK, ACTOR_KICKING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleBouldersOvercameHsuKungFuShould) },
    { EVENT_TALK, ACTOR_PUNCHING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), EVENT_SCRIPT(Event_StudentAsksAboutAltin) },
    { EVENT_TALK, ACTOR_JUMPING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), EVENT_SCRIPT(Event_TalkToJumpingStudent) },
    { EVENT_TALK, ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleFeizhiPredictedHsusDangerHerProphecy) },
    { EVENT_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleHsuIsRestingFeizhiIsWatching) },
    { EVENT_TALK, ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTemplePracticeEveryoneChiHasItsLimits) },
    { EVENT_TALK, ACTOR_WELCOMING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleWarriorYouAreGreatYouHave) },
    { EVENT_TALK, ACTOR_KICKING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleEveryoneWantsToLearnTheChi) },
    { EVENT_TALK, ACTOR_PUNCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleConcentrateEnergyIntoTheHandsAnd) },
    { EVENT_TALK, ACTOR_JUMPING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), EVENT_SCRIPT(Event_TalkToJumpingStudent) },
    { EVENT_TALK, ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleIDidNotThinkMuchOf) },
    { EVENT_TALK, ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleWhereDidFeizhiGoIAm) },
    { EVENT_TALK, ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleMasterHsuIsLateICan) },
    { EVENT_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleIWillLearnChiIWill) },
    { EVENT_TALK, ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorUseYourChiFromThe) },
    { EVENT_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorPleaseUseYourChiFrom) },
    { EVENT_TALK, ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorLetMeSeeYourChi) },
    { EVENT_TALK, ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleFeizhiIsGoneItIsDangerous) },
    { EVENT_TALK, ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleFeizhiIsStrongerThanSheLooks) },
    { EVENT_TALK, ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorShowUsYourChiLike) },
    { EVENT_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorThatWasGreatButMaster) },
    { EVENT_TALK, ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), EVENT_SCRIPT(Event_FehAsksToSeeChi) },
    { EVENT_TALK, ACTOR_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleFeizhiIsGoneItIsDangerousOutside) },
    { EVENT_TALK, ACTOR_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleFeizhiIsStrongerThanSheLooksShe) },
    { EVENT_TALK, ACTOR_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleIKnowWhyYouCanUse) },
    { EVENT_TALK, ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleWarriorChiIsAGreatPower) },
    { EVENT_TALK, ACTOR_WELCOMING_STUDENT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StudentWelcomesToSchool) },
    { EVENT_TALK, ACTOR_KICKING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleStrongKicksAreVeryImportantKick) },
    { EVENT_TALK, ACTOR_PUNCHING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIWantToLearnSoI) },
    { EVENT_TALK, ACTOR_JUMPING_STUDENT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToJumpingStudent) },
    { EVENT_TALK, ACTOR_PROUD_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleEveryoneInXianCanDoMartial) },
    { EVENT_TALK, ACTOR_WATCHING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleJustListeningDoesNotMakeOne) },
    { EVENT_TALK, ACTOR_TREE_STUDENT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StudentAsksAboutFeh) },
    { EVENT_TALK, ACTOR_MESSENGER_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterFehIsVeryBusyHe) },
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
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, IF_FLAG_SET(FLAG_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorTheRulesRequireYouTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleSoCloseWarriorUseChiFrom) },
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
    { TOUCH_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushTreeByHand) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushTreeByHand) },
    { TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushTreeByHand) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushTreeByHand) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TRAINING_TREE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveTree) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_TRAINING_TREE_TARGET, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ForceTree) },
    { SEARCH(SEARCH_BARREL), 100, FLAG_FUCHIN_TEMPLE_ANTIDOTE_BARREL, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_JAR), 101, FLAG_FUCHIN_TEMPLE_UNICORN_RING_JAR, FIND_ITEM(ITEM_UNICORN_RING) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gFuchinTempleEventsFromLamaTemple[] = {
    { EVENT_EXIT, 4, CONDITION_ALWAYS, FUCHIN_TEMPLE_INTERIORS_TO_LAMA_TEMPLE },
    { EVENT_TALK, ACTOR_HAMAS_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterHamaIsMeditatingAskingFor) },
    { EVENT_TALK, ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleTheFutureHoldsManyHardshipsBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMAS_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIDoNotThinkMasterHama) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleICannotSeeYourFutureClearly) },
    { SEARCH(SEARCH_JAR), 102, FLAG_HAMAS_ROOM_COINS_JAR, FIND_COINS(6) },
    { SCENE_EVENTS_END },
};
// clang-format on

s32 gFuchinTemple_ShotScript[] = {
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(44),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};
