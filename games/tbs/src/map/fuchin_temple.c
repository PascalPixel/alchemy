/// @file
/// Fuchin Temple, and two masters' rooms in other map sections that share its
/// overlay: Master Feh's school in Xian and Master Hama's room in Lama Temple.
/// Two maps hold four map sections, by their area names; in each, story flags
/// pick where the people stand and which script plays on entry.
///
/// - Fuchin Temple (MapId_FuchinTempleGrounds), outside the temple, reached
///   from the world map (FUCHIN_TEMPLE_GROUNDS_FROM_WORLD_MAP). A defeat
///   anywhere here returns the party to FUCHIN_TEMPLE_GROUNDS_AFTER_DEFEAT, in
///   front of the traveling healer. The monk beneath the falls guards the
///   waterfall grotto: once Nyunpa has set his test
///   (FLAG_NYUNPA_TEST_ACCEPTED) he leaps aside
///   (FLAG_FUCHIN_FALLS_GROTTO_OPENED) and the grotto leads into Fuchin Falls
///   Cave (FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE); back out with
///   the Orb of Force (FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND), he sends the
///   party on to Nyunpa (FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST).
/// - Fuchin Temple (MapId_FuchinTempleInteriors, by
///   FUCHIN_TEMPLE_INTERIORS_FROM_FUCHIN_TEMPLE_GROUNDS or the default warp
///   destination), inside, where Nyunpa meditates. Mind Read on him offers the
///   test (EventScript_NyunpaOffersTest, again after
///   FLAG_NYUNPA_TEST_DECLINED); back with the Orb, Map_Initialize plays his
///   lesson on Ki (FuchinTemple_NyunpaTeachesKi, FLAG_NYUNPA_TAUGHT_FORCE).
///   Once Mogall Forest has set FLAG_NYUNPA_ENLIGHTENED he sits withered
///   (ANIMATION_NYUNPA_WITHERED) and Mind Read hears his last wish.
/// - Xian (FuchinTempleInteriors, by FUCHIN_TEMPLE_INTERIORS_FROM_XIAN and
///   FUCHIN_TEMPLE_INTERIORS_FROM_HOUSE), Master Feh's kung fu school; the
///   area-name rules give the name Xian only at
///   FUCHIN_TEMPLE_INTERIORS_FROM_XIAN, and Map_Initialize hides the name at
///   both. Force from the line fells the training tree (EventScript_ForceTree):
///   the first time, Feizhi and Feh come out and she leaves to find Hsu
///   (FLAG_FEIZHI_WENT_TO_HSU), which unlocks FUCHIN_TEMPLE_INTERIORS_TO_HOUSE;
///   Feh then waits to see Isaac's Chi (FLAG_FEH_WAITS_TO_SEE_CHI, or
///   FLAG_FEH_CHI_DEMONSTRATION_DECLINED after two refusals) and, seeing the
///   tree fall from the line, names it Ki (FLAG_FEH_SAW_KI). Moving the tree
///   with Move or by hand only sets FLAG_FEH_SCHOOL_SAW_TREE_FALL. Once Hsu
///   is known to be trapped (FLAG_XIAN_HEARD_HSU_TRAPPED),
///   Map_GetObjectEventTemplates removes the tree and two students.
///   FUCHIN_TEMPLE_INTERIORS_TO_XIAN leads out into Xian.
/// - Lama Temple (FuchinTempleInteriors, by
///   FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE), the room in which Master Hama
///   meditates. The first visit plays her lesson
///   (FuchinTemple_RunHamasRevealLesson), in which she gives Ivan Reveal and
///   leaves with Feizhi to free Hsu, setting FLAG_XIAN_HEARD_HSU_TRAPPED; until
///   FLAG_HSU_RESCUED, Hama and the boy monk in her room are away.
///   FUCHIN_TEMPLE_INTERIORS_TO_LAMA_TEMPLE leads back into Lama Temple.
///
/// Open questions:
///
/// - XXX (no evidence): why Map_Initialize places a crate painted solid
///   black and translucent (ObjectEvent_PrepareObject) over the lattice door
///   to Feh's house in his school; the school's renders are the same with and
///   without it, so nothing the player sees shows what it was for.
#include "game_state.h"
#include "map_ids.h"
#include "unit/map/fuchin_temple.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "object_event.h"
#include "action_script.h"
#include "types.h"
#include "script_layer.h"
#include "engine_call.h"
#include "map_tables.h"
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
#include "script.h"
#include "item_ids.h"
#include "objcmd.h"
#include "global_cells.h"
#include "fx_work.h"
#include "palbuf.h"
#include "io_reg.h"

// Fuchin Temple's map-section temp flags, which last until the party enters
// another map section; FLAG_MAPSEC_TEMP_50 is the Psynergy Stone outside,
// which then refills.

/// The party refused Nyunpa's test, so talking to him offers it again.
/// ★ Named from the game: "Then I cannot tell you. Give up and go back from
/// whence you came." (MsgXianThenCannotTellGiveUp)
#define FLAG_NYUNPA_TEST_DECLINED FLAG_MAPSEC_TEMP_00
/// Feh's students saw the training tree fall, but not to Chi from the line.
/// ★ Named from the game: "Look! The tree fell." (MsgXianLookTreeFell)
#define FLAG_FEH_SCHOOL_SAW_TREE_FALL FLAG_MAPSEC_TEMP_01

/// The placed object events outside Fuchin Temple (FuchinTempleGrounds),
/// numbered in the order of gFuchinTempleGroundsObjectEventTemplates.
enum FuchinTempleOutsideLocalId {
  /// A young man from Kolima.
  /// ★ Named from the game: "How did you get here? Is the bridge open again?"
  /// (MsgXianHowDidGetHereBridge)
  LOCALID_YOUNG_MAN_FROM_KOLIMA = LOCALID_FIRST_PLACED,
  /// The monk beneath the falls, at the waterfall grotto.
  /// ★ Named from what the player sees: a bald monk in an orange robe standing
  /// on a stump beneath the waterfall (its render)
  LOCALID_FALLS_MONK,
  /// A boy monk who reads omens.
  /// ★ Named from the game: "Gems from the sky, blue lights in the north... All
  /// omens of disaster!" (MsgFuchinTempleGemsFromTheSkyBlueLights)
  LOCALID_BOY_MONK_READING_OMENS,
  /// A boy monk who fears for Nyunpa.
  /// ★ Named from the game: "Nyunpa has not eaten in days... He is going to
  /// waste away to nothing." (MsgFuchinTempleNyunpaHasNotEatenInDays)
  LOCALID_WORRIED_BOY_MONK,
  /// A boy monk who admires Nyunpa.
  /// ★ Named from the game: "Isn't it noble of him to try to save the world by
  /// becoming enlightened?" (MsgXianIsntNobleHimTrySave)
  LOCALID_ADMIRING_BOY_MONK,
  /// The traveling healer, a man in a tall hat and a long robe.
  /// ★ Named from the game: "I'm traveling around the world to spread good
  /// will." (MsgXianImTravelingAroundWorldSpread)
  LOCALID_HEALER
};

/// The placed object events inside Fuchin Temple and in Master Feh's school in
/// Xian (FuchinTempleInteriors), numbered in the order of
/// gFuchinTempleObjectEventTemplates: Nyunpa, Feh's students, Feizhi and Feh,
/// and the training tree.
enum FuchinTempleInsideLocalId {
  /// ★ Named from the game: "I have already heard of your test from Master
  /// Nyunpa." (MsgXianAlreadyHeardTest)
  LOCALID_NYUNPA = LOCALID_FIRST_PLACED,
  /// A young man, by the school's door.
  /// ★ Named from the game: "This is Master Feh's school... You came to watch?"
  /// (MsgXianMasterFehsSchoolCameWatch)
  LOCALID_WELCOMING_STUDENT,
  /// A student who practices kicks.
  /// ★ Named from the game: "Strong kicks are very important. Kick, kick,
  /// kick!" (MsgFuchinTempleStrongKicksAreVeryImportantKick)
  LOCALID_KICKING_STUDENT,
  /// A student who practices punches.
  /// ★ Named from the game: "I want to learn, so I train. Quick punches are
  /// very good." (MsgFuchinTempleIWantToLearnSoI)
  LOCALID_PUNCHING_STUDENT,
  /// ★ Named from the game: "Flexibility and jumping are very important in
  /// kung fu." (MsgXianFlexibilityJumpingVeryImportantIn)
  LOCALID_JUMPING_STUDENT,
  /// A student proud of Feh's school.
  /// ★ Named from the game: "Everyone in Xian can do martial arts. But the Feh
  /// School of martial arts is the most elite."
  /// (MsgFuchinTempleEveryoneInXianCanDoMartial)
  LOCALID_PROUD_STUDENT,
  /// A little girl, a student who learns by watching.
  /// ★ Named from the game: "Just listening does not make one strong. I learn
  /// by watching." (MsgFuchinTempleJustListeningDoesNotMakeOne)
  LOCALID_WATCHING_STUDENT,
  /// By the training tree, which he sets up again after it falls.
  /// ★ Named from the game: "Master Feh is very busy. Do you still wish to see
  /// him?" (MsgXianMasterFehVeryBusyDo)
  LOCALID_TREE_STUDENT,
  /// Keeps the party from Feh, and goes to Feh's door when the tree first falls
  /// from the line.
  /// ★ Named from the game: "Master Feh is very busy. He is speaking with his
  /// daughter, Feizhi. You cannot enter now."
  /// (MsgFuchinTempleMasterFehIsVeryBusyHe)
  LOCALID_MESSENGER_STUDENT,
  /// ★ Named from the game: "He is speaking with his daughter, Feizhi."
  /// (MsgFuchinTempleMasterFehIsVeryBusyHe)
  LOCALID_FEIZHI,
  /// ★ Named from the game: "Master Feh is talking with Feizhi"
  /// (MsgFuchinTempleMasterFehIsTalkingWithFeizhi)
  LOCALID_FEH,
  /// ★ Named from the game: "Look! The tree fell." (MsgXianLookTreeFell)
  LOCALID_TRAINING_TREE,
  /// Where Force strikes the tree.
  /// ★ Named from the game: "You cannot push it with your hands. Attack it with
  /// Chi." (MsgXianCannotPushHands)
  LOCALID_TRAINING_TREE_TARGET
};

/// The placed object events in Master Hama's room in Lama Temple
/// (FuchinTempleInteriors by FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE),
/// numbered in the order of gFuchinTempleObjectEventTemplatesFromLamaTemple;
/// Garet, Ivan and Mia, placed first, keep their own slots.
enum HamasRoomLocalId {
  /// ★ Named from the game: "Master Hama is meditating."
  /// (MsgXianMasterHamaMeditatingPleaseExtremely)
  LOCALID_HAMA = LOCALID_FIRST_PLACED,
  /// A boy monk in Hama's room.
  /// ★ Named from the game: "Master Hama is meditating. Please be extremely
  /// quiet." (MsgXianMasterHamaMeditatingPleaseExtremely)
  LOCALID_HAMAS_BOY_MONK,
  /// Bursts in to say that Hsu is in danger.
  /// ★ Named from the game: "Feizhi... Why are you here?"
  /// (MsgXianMasterHamaMeditatingPleaseExtremely)
  LOCALID_FEIZHI_IN_LAMA_TEMPLE
};

/// Animations of these maps' own sprites beyond enum ObjectEventAnimation.
enum {
  /// Seated cross-legged with his eyes shut (spr_051.png's frame 33).
  ANIMATION_NYUNPA_MEDITATE = 5,
  /// Seated as he meditated, shrunken and darkened, once he has died
  /// enlightened (spr_051.png's frame 34).
  ANIMATION_NYUNPA_WITHERED = 6,
  /// A drill of kicks, a knee raised, a kick and a leaping kick, from a
  /// fighting stance (spr_130.tsv).
  ANIMATION_STUDENT_KICKS = 5,
  /// A drill of quick punches with one kick among them (spr_131.tsv).
  ANIMATION_STUDENT_PUNCHES = 5
};

static void FuchinTemple_FeizhiLeavesForHsu(void);
static void FuchinTemple_FehSeesChi(void);
static UNUSED void FuchinTemple_HopLeaderByOffset(s32 offsetX, s32 offsetZ);
static void FuchinTemple_StartMindReadDemonstration(s32 caster, s32 target);
static void FuchinTemple_FinishMindReadDemonstration(void);
static void FuchinTemple_SpawnNyunpasParticles(void);
static void FuchinTemple_AnimateRevealLesson(void);

// Shared source, included in link order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Clears the scripted object event's sprite flags and completes the script
/// command.
static s32 FuchinTemple_ClearScriptedObjectEventSpriteFlags(
    struct ObjectScriptContext *interpreter) {
  Engine_ObjectEventSetSpriteFlags((struct ObjectEvent *)interpreter, 0);
  return 0;
}

// Keep an object event facing the party leader while the conversation is
// staged.
// UB: update hooks return void. The native produces an unused zero result;
// plain void changes five instructions in the English build.
#ifdef UBFIX
static void FuchinTemple_FaceLeader(union FieldObject *fieldObject) {
#else
static s32 FuchinTemple_FaceLeader(union FieldObject *fieldObject) {
#endif
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *player = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->facing = ArcTan2(player->z.fixed - objectEvent->z.fixed,
                                player->x.fixed - objectEvent->x.fixed);
#ifndef UBFIX
  return 0;
#endif
}

/// Chooses the outside warp destination table or the shared interior warp
/// destinations.
const struct MapWarpDest *Map_GetWarpDests(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_FuchinTempleGrounds) {
    return gFuchinTempleGroundsWarpDests;
  }
  return gFuchinTempleWarpDestsOther;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) { return gFuchinTempleWarps; }

/// Selects the room's object events and applies the post-lesson object event
/// template changes.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  if (gGameState.mapId == (s32)&MapId_FuchinTempleGrounds) {
    return gFuchinTempleGroundsObjectEventTemplates;
  }
  if (gGameState.warpDest == FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE) {
    return gFuchinTempleObjectEventTemplatesFromLamaTemple;
  }
  // Once Hsu is known to be trapped, the tree and two students are gone, and
  // another student stands elsewhere.
  if (FLAG_CHECK(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
    gFuchinTempleObjectEventTemplates[LOCALID_PROUD_STUDENT -
                                      LOCALID_FIRST_PLACED]
        .condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
    gFuchinTempleObjectEventTemplates[LOCALID_TREE_STUDENT -
                                      LOCALID_FIRST_PLACED]
        .condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
    gFuchinTempleObjectEventTemplates[LOCALID_MESSENGER_STUDENT -
                                      LOCALID_FIRST_PLACED]
        .x = 0x1200000;
    gFuchinTempleObjectEventTemplates[LOCALID_MESSENGER_STUDENT -
                                      LOCALID_FIRST_PLACED]
        .z = 0xf80000;
    gFuchinTempleObjectEventTemplates[LOCALID_TRAINING_TREE -
                                      LOCALID_FIRST_PLACED]
        .condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
    gFuchinTempleObjectEventTemplates[LOCALID_TRAINING_TREE_TARGET -
                                      LOCALID_FIRST_PLACED]
        .condition = FLAG_XIAN_HEARD_HSU_TRAPPED;
  }
  Map_PrepareObjectEvents(gFuchinTempleObjectEventTemplates);
  return gFuchinTempleObjectEventTemplates;
}

/// Talking to the young man from Kolima at Fuchin Temple
/// (FuchinTempleGrounds): he asks how the party got here. "How did you get
/// here? Is the bridge open again?" Yes cheers him ("Then I can go home to
/// Kolima now?") and is remembered; no means the party came through the forest.
/// Later talks go on to the tale of the forest.
static void EventScript_YoungManFromKolimaAsksAboutBridge(void) {
  SCRIPT_START();
  MESSAGE_SET(MsgXianHowDidGetHereBridge);
  if (FLAG_CHECK(FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN) != 0) {
    gFieldWork->message += 4;
  }
  Script_OpenMessage(LOCALID_YOUNG_MAN_FROM_KOLIMA, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    FLAG_SET(FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN);
  } else {
    gFieldWork->message++;
  }
  Script_ShowMessage(LOCALID_YOUNG_MAN_FROM_KOLIMA, 0);
  SCRIPT_END();
}

/// Talking to the boy monk (LOCALID_ADMIRING_BOY_MONK) at Fuchin Temple
/// (FuchinTempleGrounds): he speaks of the one seeking enlightenment, with up
/// to two more questions. "Isn't it noble of him to try to save the world by
/// becoming enlightened?" Once Nyunpa is gone, "Now, he truly is beyond the
/// world's concerns. Is he watching over us?"
static void EventScript_BoyMonkSpeaksOfNyunpa(void) {

  SCRIPT_START();
  if (Engine_GameFlagIsSet(FLAG_NYUNPA_ENLIGHTENED) != 0) {
    MESSAGE_SET(MsgXianNowHeTrulyBeyondWorlds);
    Engine_ScriptAskYesNo(LOCALID_ADMIRING_BOY_MONK, 0);
    SCRIPT_END();
  } else {
    MESSAGE_SET(MsgXianIsntNobleHimTrySave);
    MESSAGE_OPEN(LOCALID_ADMIRING_BOY_MONK, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == 1) {
      gFieldWork->message++;
      MESSAGE_OPEN(LOCALID_ADMIRING_BOY_MONK, 0);
      if (Engine_ScriptChooseYesNo(0, 0) == 1) {
        gFieldWork->message++;
      }
    }
    MESSAGE_PUT(LOCALID_ADMIRING_BOY_MONK, 0);
    SCRIPT_END();
  }
}

/// Talking to the monk beneath the falls at Fuchin Temple (FuchinTempleGrounds)
/// once Nyunpa has set the test (FLAG_NYUNPA_TEST_ACCEPTED): he lets the party
/// pass (FLAG_FUCHIN_FALLS_GROTTO_OPENED). "I have already heard of your test
/// from Master Nyunpa. You may enter." He leaps aside, and the way into the
/// waterfall grotto opens.
static void EventScript_MonkLetsPartyIntoGrotto(void) {
  struct ObjectEvent *monk;
  struct ObjectEvent *movingMonk;
  struct EffectOptions options;

  monk = ObjectEvent_GetById(LOCALID_FALLS_MONK);
  SCRIPT_START();
  MESSAGE_SET(MsgXianAlreadyHeardTest);
  Engine_ScriptShowMessageAndWait(LOCALID_FALLS_MONK, 0, 20);
  // The monk leaps aside and lands in a puff of dust.
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 168, 392);
  Engine_ObjectEventFaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
  Engine_AudioPlayCue(SE_132);
  movingMonk = ObjectEvent_GetById(LOCALID_FALLS_MONK);
  movingMonk->velocityY = 0x140000;
  movingMonk = ObjectEvent_GetById(LOCALID_FALLS_MONK);
  movingMonk->gravity = 0x40000;
  OBJECT_SPEED(LOCALID_FALLS_MONK, PIXELS(3), 3 * PIXELS(1) / 2);
  ObjectEvent_SetDestination(LOCALID_FALLS_MONK, 152, 392);
  Engine_ObjectEventWaitForMove(LOCALID_FALLS_MONK);
  movingMonk = ObjectEvent_GetById(LOCALID_FALLS_MONK);
  movingMonk->gravity = 0x10000;
  Engine_ObjectEventFaceDirection(LOCALID_FALLS_MONK, FACING_EAST, 0);
  Engine_AudioPlayCue(SE_132);
  // Three white puffs, with only the colour option set.
  options.palette = SPRITE_COLOR_WHITE;
  FieldEffect_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + 0x40000),
                    0x8000, 0, 0, EFFECT_USE_PALETTE, &options);
  FieldEffect_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + 0x40000), 0,
                    0, 0, EFFECT_USE_PALETTE, &options);
  FieldEffect_Spawn(monk->x.fixed, monk->y.fixed, (monk->z.fixed + 0x40000),
                    -0x8000, 0, 0, EFFECT_USE_PALETTE, &options);
  WAIT(30);
  Map_PlayQueuedMusic();
  MapGrid_CopyCellAttributes(10, 24, 1, 1, 10, 22);
  Engine_GameFlagSet(FLAG_FUCHIN_FALLS_GROTTO_OPENED);
  SCRIPT_END();
}

/// Isaac comes back out of the grotto with the Orb of Force. At Fuchin Temple
/// (FuchinTempleGrounds, coord event 5) the monk sends him on to Nyunpa, and
/// the way back is open again. "Young master, did you complete the test?" Then
/// "Master Nyunpa is waiting for you... Proceed to the temple."
static void EventScript_MonkAsksWhetherTestIsDone(void) {
  SCRIPT_START();
  Engine_GameFlagSet(FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST);
  OBJECT_FACE(LOCALID_FALLS_MONK, LOCALID_PARTY_LEADER, 0);
  WAIT(10);
  MESSAGE_SET(MsgXianYoungMasterDidCompleteTest);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FALLS_MONK, 2);
  WAIT(20);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_WEST, 20);
  Engine_ScriptAskYesNo(LOCALID_FALLS_MONK, 0);
  WAIT(10);
  OBJECT_EMOTE(LOCALID_FALLS_MONK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  OBJECT_DIR(LOCALID_FALLS_MONK, FACING_NORTH + FACING_STEP, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FALLS_MONK, 2);
  WAIT(20);
  Engine_ScriptShowMessageAndWait(LOCALID_FALLS_MONK, 0, 20);
  Engine_ObjectEventFaceDirection(LOCALID_FALLS_MONK, FACING_EAST, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FALLS_MONK, ANIMATION_NOD);
  WAIT(20);
  Engine_ScriptShowMessageAndWait(LOCALID_FALLS_MONK, 0, 20);
  MapGrid_CopyCellAttributes(10, 26, 1, 1, 10, 24);
  SCRIPT_END();
}

/// Reading Nyunpa's mind inside Fuchin Temple (FuchinTempleInteriors), or
/// talking to him after a refusal: he hears the party, reads their minds in
/// turn, guesses that they are following someone, warns of Mogall Forest and
/// offers the test of the grotto. "Mmmm... Who is that? Who speaks to my mind?"
/// Accepting it (FLAG_NYUNPA_TEST_ACCEPTED), he sends Isaac to the monk beneath
/// the falls; refusing is remembered (FLAG_NYUNPA_TEST_DECLINED), and the next
/// talk asks again.
static void EventScript_NyunpaOffersTest(void) {
  s32 alreadyDeclinedTrial;
  struct ObjectEvent *nyunpa;

  SCRIPT_START();
  // Revisit an earlier refusal, or introduce the offer through Mind Read.
  alreadyDeclinedTrial = FLAG_CHECK(FLAG_NYUNPA_TEST_DECLINED);
  if (alreadyDeclinedTrial != 0) {
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 168, 504);
    WAIT(5);
    OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
    ObjectEvent_GetById(LOCALID_NYUNPA)->paused = 0;
    SE(SE_JUMP);
    nyunpa = ObjectEvent_Get(LOCALID_NYUNPA);
    nyunpa->velocityY = 0x80000;
    OBJECT_ANIMATION(LOCALID_NYUNPA, ANIMATION_STAND);
    WAIT(30);
    MESSAGE_SET(MsgXianMustSurviveTestGrottoBefore);
  } else {
    MESSAGE_SET(MsgXianMmmmWhoWhoSpeaksMy);
    FuchinTemple_StartMindReadDemonstration(LOCALID_PARTY_LEADER,
                                            LOCALID_NYUNPA);
    WAIT(30);
    Script_ShowMessage(LOCALID_NYUNPA, 0);
    FuchinTemple_FinishMindReadDemonstration();
    WAIT(20);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 168, 504);
    WAIT(5);
    OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
    SE(SE_JUMP);
    ObjectEvent_GetById(LOCALID_NYUNPA)->paused = alreadyDeclinedTrial;
    nyunpa = ObjectEvent_Get(LOCALID_NYUNPA);
    nyunpa->velocityY = 0x80000;
    OBJECT_ANIMATION(LOCALID_NYUNPA, ANIMATION_STAND);
    WAIT(30);
    Script_OpenMessage(LOCALID_NYUNPA, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == 1) {
      Engine_ObjectEventRunRepeatedMotion(LOCALID_NYUNPA, 2);
      WAIT(20);
      Script_ShowMessage(LOCALID_NYUNPA, 0);
      WAIT(20);
      FuchinTemple_StartMindReadDemonstration(LOCALID_NYUNPA,
                                              LOCALID_PARTY_LEADER);
      WAIT(30);
      Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
      WAIT(50);
      FuchinTemple_FinishMindReadDemonstration();
      WAIT(30);
      Engine_ObjectEventSetAnimationAndWait(LOCALID_NYUNPA, ANIMATION_NOD);
      Script_ShowMessage(LOCALID_NYUNPA, 0);
    } else {
      gFieldWork->message += 2;
      Script_ShowMessage(LOCALID_NYUNPA, 0);
    }
    Engine_ObjectEventSetAnimationAndWait(LOCALID_NYUNPA, ANIMATION_NOD);
    WAIT(30);
    OBJECT_EMOTE(LOCALID_NYUNPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    MESSAGE_SET(MsgXianFollowThemDoNot);
    Script_OpenMessage(LOCALID_NYUNPA, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == 1) {
      OBJECT_EMOTE(LOCALID_NYUNPA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      FuchinTemple_StartMindReadDemonstration(LOCALID_NYUNPA,
                                              LOCALID_PARTY_LEADER);
      WAIT(30);
      Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
      WAIT(50);
      FuchinTemple_FinishMindReadDemonstration();
      WAIT(30);
      Script_ShowMessage(LOCALID_NYUNPA, 0);
      gFieldWork->message += 1;
    } else {
      gFieldWork->message += 1;
      WAIT(20);
      Engine_ObjectEventSetAnimationAndWait(LOCALID_NYUNPA, ANIMATION_NOD);
      WAIT(20);
      Script_ShowMessage(LOCALID_NYUNPA, 0);
    }
    WAIT(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_NYUNPA, ANIMATION_SHAKE_HEAD);
    WAIT(20);
    Script_ShowMessage(LOCALID_NYUNPA, 0);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
    WAIT(20);
    Script_ShowMessage(LOCALID_NYUNPA, 0);
    WAIT(20);
    OBJECT_DIR(LOCALID_NYUNPA, FACING_NORTH, 20);
    OBJECT_SPEED(LOCALID_NYUNPA, 3 * PIXELS(1) / 10, 3 * PIXELS(1) / 20);
    ObjectEvent_WalkToAndWait(LOCALID_NYUNPA, 168, 464);
    WAIT(60);
    OBJECT_DIR(LOCALID_NYUNPA, FACING_SOUTH, 40);
    Script_ShowMessageAndWait(LOCALID_NYUNPA, 0, 10);
    OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    ObjectEvent_WalkToAndWait(LOCALID_NYUNPA, 168, 472);
  }
  Script_OpenMessage(LOCALID_NYUNPA, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 1) {
    MESSAGE_SET(MsgXianThenCannotTellGiveUp);
    Script_ShowMessage(LOCALID_NYUNPA, 0);
    FLAG_SET(FLAG_NYUNPA_TEST_DECLINED);
  } else {
    // "Do not worry. You will be permitted to enter the grotto." Nyunpa
    // glows as he calls to the monk.
    MESSAGE_SET(MsgXianDoNotWorryWillPermitted);
    WAIT(30);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_NYUNPA, ANIMATION_NOD);
    WAIT(20);
    FieldCamera_SetSpeed(0x8000, 0x1000);
    Engine_CameraMoveToObjectEvent(LOCALID_NYUNPA, 1);
    PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
    Engine_ColorBufferInterpolate(30);
    Engine_ScriptWaitForScreen();
    Engine_CameraWaitForMove();
    FuchinTemple_SpawnNyunpasParticles();
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    Engine_ColorBufferInterpolate(30);
    Script_ShowMessage(LOCALID_NYUNPA, 0);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_NYUNPA, 2);
    WAIT(20);
    Script_ShowMessage(LOCALID_NYUNPA, 0);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
    ObjectEvent_SetAttachedEffect(
        LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    WAIT(60);
    Script_ShowMessageAndWait(LOCALID_NYUNPA, 0, 10);
    FLAG_SET(FLAG_NYUNPA_TEST_ACCEPTED);
  }
  OBJECT_ANIMATION(LOCALID_NYUNPA, ANIMATION_NYUNPA_MEDITATE);
  SCRIPT_END();
}

/// Reading Nyunpa's mind inside Fuchin Temple once the test is accepted: he
/// notices.
/// "You enjoy reading the minds of others, do you not?"
static void EventScript_NyunpaFeelsMindRead(void) {
  SCRIPT_START();
  MESSAGE_SET(MsgXianEnjoyReadingMindsOthersDo);
  Engine_ScriptAskYesNo(LOCALID_NYUNPA, 0);
  SCRIPT_END();
}

/// Talking to the student at the door of Master Feh's school in Xian
/// (LOCALID_WELCOMING_STUDENT): the student asks whether the party came to
/// watch. "This is Master Feh's school... You came to watch?"
static void EventScript_StudentWelcomesToSchool(void) {
  SCRIPT_START();
  MESSAGE_SET(MsgXianMasterFehsSchoolCameWatch);
  Engine_ScriptAskYesNo(LOCALID_WELCOMING_STUDENT, 0);
  SCRIPT_END();
}

/// Talking to the student who practices jumping in Master Feh's school in Xian:
/// he lands, talks about jumping (or Lama Temple, or Hsu), then jumps on.
static void EventScript_TalkToJumpingStudent(void) {
  SCRIPT_START();

  ObjectEvent_GetById(LOCALID_JUMPING_STUDENT)->paused = 0;

  while (ObjectEvent_GetById(LOCALID_JUMPING_STUDENT)->y.fixed > 0) {
    Engine_TaskWait(1);
  }

  ObjectEvent_GetById(LOCALID_JUMPING_STUDENT)->y.fixed = 0;

  ObjectEvent_GetById(LOCALID_JUMPING_STUDENT)->targetY = OBJ_EVENT_NO_TARGET;

  ObjectEvent_GetById(LOCALID_JUMPING_STUDENT)->velocityY = 0;

  ObjectEvent_GetById(LOCALID_JUMPING_STUDENT)->paused = 1;

  OBJECT_FACE(LOCALID_JUMPING_STUDENT, LOCALID_PARTY_LEADER, 0);

  if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) != 0) {
    MESSAGE_SET(MsgXianHsuDidNotPracticeJumping);
  } else if (Engine_GameFlagIsSet(FLAG_FEH_SAW_KI) != 0) {
    MESSAGE_SET(MsgXianLamaTempleFarWestIn);
  } else {
    MESSAGE_SET(MsgXianFlexibilityJumpingVeryImportantIn);
  }

  MESSAGE_PUT(LOCALID_JUMPING_STUDENT, 0);

  ObjectEvent_GetById(LOCALID_JUMPING_STUDENT)->facing = FACING_SOUTH;

  ObjectEvent_GetById(LOCALID_JUMPING_STUDENT)->paused = 0;

  ObjectEvent_SetActionScript(LOCALID_JUMPING_STUDENT,
                              gFuchinTemple_JumpingStudentScript);
  SCRIPT_END();
}

/// Talking to the student by the training tree in Master Feh's school in Xian
/// (LOCALID_TREE_STUDENT): the student asks whether the party still wishes to
/// see Master Feh. "Master Feh is very busy. Do you still wish to see him?"
/// Yes: if the tree falls to Chi from the line, he will take the party to Feh.
static void EventScript_StudentAsksAboutFeh(void) {
  SCRIPT_START();
  MESSAGE_SET(MsgXianMasterFehVeryBusyDo);
  Engine_ScriptAskYesNo(LOCALID_TREE_STUDENT, 0);
  SCRIPT_END();
}

/// Rotates the training tree through its westward arc and scatters particles.
static void FuchinTemple_ToppleTreeWest(void) {
  struct ObjectEvent *tree = ObjectEvent_GetById(LOCALID_TRAINING_TREE);
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

  Engine_AudioPlayCue(SE_TOPPLE);

  FieldEffect_Spawn(tree->x.fixed - 0xc0000, tree->y.fixed,
                    tree->z.fixed + 0x80000, 0xffffcccd, 0x6666, 0, 0, 0);
  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0x80000,
                    0xffff3334, 0x4ccc, 0, 0, 0);
  FieldEffect_Spawn(tree->x.fixed + 0xa0000, tree->y.fixed,
                    tree->z.fixed + 0x80000, 0xffff0000, 0x3333, 0, 0, 0);
}

/// Rotates the training tree through the opposite arc and reverses its
/// horizontal scale.
static void FuchinTemple_ToppleTreeEast(void) {
  struct ObjectEvent *tree;
  u32 turnStep;
  s32 angle;

  tree = ObjectEvent_GetById(LOCALID_TRAINING_TREE);
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
  Engine_AudioPlayCue(SE_TOPPLE);
  FieldEffect_Spawn(tree->x.fixed - 0xc0000, tree->y.fixed,
                    tree->z.fixed + 0x80000, 0x10000, 0x3333, 0, 0, 0);
  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0x80000,
                    0xcccc, 0x4ccc, 0, 0, 0);
  FieldEffect_Spawn(tree->x.fixed + 0xa0000, tree->y.fixed,
                    tree->z.fixed + 0x80000, 0x3333, 0x6666, 0, 0, 0);
}

/// Slides the training tree north in four steps, then scatters particles.
static void FuchinTemple_ToppleTreeNorth(void) {
  struct ObjectEvent *tree = ObjectEvent_GetById(LOCALID_TRAINING_TREE);
  u32 stepIndex = 0;
  s32 delay = 8;
  u8 rotation;
  do {
    Engine_TaskWait(delay);
    tree->z.fixed += 0xffff0000;
    tree->targetZ = OBJ_EVENT_NO_TARGET;
    stepIndex++;
    delay -= 2;
  } while (stepIndex < 4);
  rotation = 0;
  tree->sprite->rotation = rotation;
  Engine_AudioPlayCue(SE_TOPPLE);
  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0xfff80000,
                    0xffff3334, 0, 0xffffcccd, 0, 0);
  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0xfff80000,
                    0x0000cccc, 0, 0xffffcccd, 0, 0);
  FieldEffect_Spawn(tree->x.fixed + 0xfffa0000, tree->y.fixed,
                    tree->z.fixed + (160 << 12), 0x00003333, 0, 0xffff0000, 0,
                    0);
  FieldEffect_Spawn(tree->x.fixed + (192 << 11), tree->y.fixed,
                    tree->z.fixed + (160 << 12), 0x00003333, 0, 0xffff0000, 0,
                    0);
}

/// Slides the training tree south in four steps before the final displacement.
static void FuchinTemple_ToppleTreeSouth(void) {
  struct ObjectEvent *tree;
  u32 stepIndex;
  s32 particleSpeed;
  s32 delay;

  tree = ObjectEvent_GetById(LOCALID_TRAINING_TREE);
  stepIndex = 0;
  delay = 8;
  do {
    Engine_TaskWait(delay);
    tree->z.fixed += 0x10000;
    tree->targetZ = OBJ_EVENT_NO_TARGET;
    stepIndex++;
    delay -= 2;
  } while (stepIndex < 4);

  tree->sprite->rotation = 0;
  tree->z.fixed += 0x180000;
  tree->targetZ = OBJ_EVENT_NO_TARGET;
  Engine_AudioPlayCue(SE_TOPPLE);

  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0xc0000,
                    -0xcccc, 0, 0x3333, 0, 0);
  particleSpeed = 0x3333;
  FieldEffect_Spawn(tree->x.fixed, tree->y.fixed, tree->z.fixed + 0xc0000,
                    0xcccc, 0, particleSpeed, 0, 0);
  FieldEffect_Spawn(tree->x.fixed - 0x60000, tree->y.fixed,
                    tree->z.fixed - 0x80000, particleSpeed, 0, 0x10000, 0, 0);
  FieldEffect_Spawn(tree->x.fixed + 0x60000, tree->y.fixed,
                    tree->z.fixed - 0x80000, particleSpeed, 0, 0x10000, 0, 0);
}

/// Routes the student around the tree according to the leader's facing.
static void FuchinTemple_StudentWalksAroundTree(void) {
  u32 leaderFacing;
  u32 quadrantLimit;

  leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;
  quadrantLimit = 0x3fff;
  if (((leaderFacing - FACING_SOUTHEAST) << 16) <= 0x3fff0000) {
    Engine_ObjectEventWalkToAndWait(LOCALID_TREE_STUDENT, 216, 168);
    Engine_ObjectEventWalkToAndWait(LOCALID_TREE_STUDENT, 224, 168);
    OBJECT_DIR(LOCALID_TREE_STUDENT, FACING_SOUTHEAST, 20);
  } else if ((u16)(leaderFacing - FACING_SOUTHWEST) <= quadrantLimit) {
    Engine_ObjectEventWalkToAndWait(LOCALID_TREE_STUDENT, 232, 160);
    OBJECT_DIR(LOCALID_TREE_STUDENT, FACING_SOUTH + FACING_STEP, 20);
  } else if ((u16)(leaderFacing - FACING_NORTHWEST) <= quadrantLimit) {
    Engine_ObjectEventWalkToAndWait(LOCALID_TREE_STUDENT, 216, 168);
    Engine_ObjectEventWalkToAndWait(LOCALID_TREE_STUDENT, 224, 172);
    OBJECT_DIR(LOCALID_TREE_STUDENT, FACING_NORTHEAST, 20);
  } else {
    Engine_ObjectEventWalkToAndWait(LOCALID_TREE_STUDENT, 232, 160);
    OBJECT_DIR(LOCALID_TREE_STUDENT, FACING_SOUTHEAST, 20);
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

  OBJECT_SPEED(LOCALID_TREE_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  WAIT(60);
  rulesMessage = MsgXianCannotPushHands;
  MESSAGE_SET(rulesMessage);
  if (mode == FUCHIN_TEMPLE_CHI_NOT_FROM_LINE) {
    MESSAGE_SET(rulesMessage - 1);
    OBJECT_EMOTE(LOCALID_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Engine_ScriptShowMessageAndWait(LOCALID_TREE_STUDENT, 0, 20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_TREE_STUDENT, 2);
    MESSAGE_SET(MsgXianWarriorsCannotUse);
    Engine_ScriptShowMessageAndWait(LOCALID_TREE_STUDENT, 0, 20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_TREE_STUDENT,
                                          ANIMATION_SHAKE_HEAD);
    WAIT(20);
    Engine_ScriptShowMessageAndWait(LOCALID_TREE_STUDENT, 0, 20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_TREE_STUDENT, ANIMATION_NOD);
    WAIT(20);
  }
  if (mode == FUCHIN_TEMPLE_CHI_SEEN_BEFORE) {
    MESSAGE_SET(MsgXianGreatWarriorTrain);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_TREE_STUDENT, 2);
    WAIT(20);
  }
  Engine_ScriptShowMessageAndWait(LOCALID_TREE_STUDENT, 0, 20);
  // Restore the tree and let the student return to the demonstration spot.
  FuchinTemple_StudentWalksAroundTree();
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TREE_STUDENT, 3);
  OBJECT_POSITION_SET(LOCALID_TRAINING_TREE, 232, 168);
  Engine_ObjectEventSetPosition(LOCALID_TRAINING_TREE_TARGET, PIXELS(232),
                                PIXELS(168));
  ObjectEvent_GetById(LOCALID_TRAINING_TREE)->y.fixed = 0xc0000;
  ObjectEvent_GetById(LOCALID_TRAINING_TREE)->targetY = OBJ_EVENT_NO_TARGET;
  ObjectEvent_GetById(LOCALID_TRAINING_TREE)->scaleX = 0xcccc;
  ObjectEvent_GetById(LOCALID_TRAINING_TREE)->sprite->rotation = 0x8000;
  Engine_AudioPlayCue(SE_124);
  WAIT(40);
  Engine_ObjectEventWalkToAndWait(LOCALID_TREE_STUDENT, 216, 152);
  OBJECT_DIR(LOCALID_TREE_STUDENT, FACING_SOUTHEAST, 30);
}

/// The students see the tree fall, Feh doubts it came from the line, and the
/// student sets the tree up again. "Look! The tree fell."
static void FuchinTemple_StudentsSeeTreeFall(void) {
  struct ObjectEvent *tree;

  OBJECT_FACE(LOCALID_PROUD_STUDENT, LOCALID_TRAINING_TREE, 0);
  OBJECT_FACE(LOCALID_WATCHING_STUDENT, LOCALID_TRAINING_TREE, 0);
  OBJECT_FACE(LOCALID_TREE_STUDENT, LOCALID_TRAINING_TREE, 0);
  OBJECT_FACE(LOCALID_MESSENGER_STUDENT, LOCALID_TRAINING_TREE, 0);
  OBJECT_FACE(LOCALID_FEH, LOCALID_TRAINING_TREE, 0);
  WAIT(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TREE_STUDENT, 2);
  WAIT(20);
  MESSAGE_SET(MsgXianLookTreeFell);
  Engine_ScriptShowMessageAndWait(LOCALID_TREE_STUDENT, 0, 20);
  Engine_ScriptShowMessageAndWait(LOCALID_MESSENGER_STUDENT, 0, 20);
  OBJECT_EMOTE(LOCALID_FEH, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  OBJECT_EMOTE(LOCALID_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ScriptShowMessageAndWait(LOCALID_MESSENGER_STUDENT, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Engine_ScriptShowMessageAndWait(LOCALID_FEH, 0, 20);
  OBJECT_EMOTE(LOCALID_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ScriptShowMessageAndWait(LOCALID_MESSENGER_STUDENT, 0, 20);
  Engine_ObjectEventFaceEachOther(LOCALID_TREE_STUDENT, LOCALID_FEH, 0);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_NOD);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_TREE_STUDENT, ANIMATION_NOD);
  WAIT(20);
  // Reset the tree after the group has reacted to the demonstration.
  OBJECT_SPEED(LOCALID_TREE_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  FuchinTemple_StudentWalksAroundTree();
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TREE_STUDENT, 3);
  OBJECT_POSITION_SET(LOCALID_TRAINING_TREE, 232, 168);
  Engine_ObjectEventSetPosition(LOCALID_TRAINING_TREE_TARGET, PIXELS(232),
                                PIXELS(168));
  tree = ObjectEvent_GetById(LOCALID_TRAINING_TREE);
  tree->y.fixed = 0xc0000;
  tree = ObjectEvent_GetById(LOCALID_TRAINING_TREE);
  tree->targetY = OBJ_EVENT_NO_TARGET;
  tree = ObjectEvent_GetById(LOCALID_TRAINING_TREE);
  tree->scaleX = 0xcccc;
  tree = ObjectEvent_GetById(LOCALID_TRAINING_TREE);
  {
    struct Sprite *sprite = tree->sprite;
    s32 rotation = 0x8000;

    sprite->rotation = rotation;
  }
  Engine_AudioPlayCue(SE_124);
  WAIT(40);
  Engine_ObjectEventWalkToAndWait(LOCALID_TREE_STUDENT, 216, 152);
  OBJECT_DIR(LOCALID_TREE_STUDENT, FACING_SOUTH, 30);
  Engine_GameFlagSet(FLAG_FEH_SCHOOL_SAW_TREE_FALL);
}

/// Move on the training tree in Master Feh's school in Xian: it topples, and
/// the students react; it does not count as Chi.
static void EventScript_MoveTree(void) {
  SCRIPT_START();

  if (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing > FACING_SOUTH &&
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing < FACING_NORTH) {
    FuchinTemple_ToppleTreeWest();
  } else {
    FuchinTemple_ToppleTreeEast();
  }

  if (Engine_GameFlagIsSet(FLAG_FEH_WAITS_TO_SEE_CHI) != 0) {
    FuchinTemple_StudentsSeeTreeFall();
  } else {
    FuchinTemple_StudentExplainsChiRules(FUCHIN_TEMPLE_CHI_NOT_FROM_LINE);
  }

  SCRIPT_END();
}

/// Walking into the training tree in Master Feh's school in Xian: Isaac pushes
/// it over by hand. "You cannot push it with your hands. Attack it with Chi."
static void EventScript_PushTreeByHand(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  u16 facing;

  SCRIPT_START();
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_PUSH);
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

  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  FuchinTemple_StudentExplainsChiRules(FUCHIN_TEMPLE_CHI_PUSHED_BY_HAND);
  SCRIPT_END();
}

/// Force on the training tree in Master Feh's school in Xian: it topples. From
/// too close it does not count;
/// from the line, the students call Feh out the first time, and later Feh
/// himself sees it.
static void EventScript_ForceTree(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  u16 facing;

  SCRIPT_START();

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
  Engine_CameraMoveToObjectEvent(LOCALID_TRAINING_TREE_TARGET, 1);
  Engine_CameraWaitForMove();

  if (leader->z.part.pixel <= 209) {
    if (Engine_GameFlagIsSet(FLAG_FEIZHI_WENT_TO_HSU) == 0 ||
        Engine_GameFlagIsSet(FLAG_FEH_SAW_KI) != 0) {
      FuchinTemple_StudentExplainsChiRules(FUCHIN_TEMPLE_CHI_NOT_FROM_LINE);
    } else {
      FuchinTemple_StudentsSeeTreeFall();
    }
    SCRIPT_END();
    return;
  }

  if (Engine_GameFlagIsSet(FLAG_FEH_SAW_KI) != 0) {
    FuchinTemple_StudentExplainsChiRules(FUCHIN_TEMPLE_CHI_SEEN_BEFORE);
  } else if (Engine_GameFlagIsSet(FLAG_FEIZHI_WENT_TO_HSU) == 0) {
    FuchinTemple_FeizhiLeavesForHsu();
  } else {
    FuchinTemple_FehSeesChi();
  }
  SCRIPT_END();
}

/// The first time the tree falls from the line, a student runs for Master Feh.
/// "What did you do, warrior?" The door opens on its own and Feizhi storms
/// out, sure that Hsu is in danger. Feh will not believe her visions, and she
/// leaves alone to find Hsu. Feh then asks Isaac to show him his Chi.
static void FuchinTemple_FeizhiLeavesForHsu(void) {
  struct ObjectEvent *objectEvent;

  FLAG_SET(FLAG_FEIZHI_WENT_TO_HSU);
  WAIT(30);
  ObjectEvent_FaceObjectEvent(LOCALID_PROUD_STUDENT, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_TREE_STUDENT, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_MESSENGER_STUDENT, LOCALID_PARTY_LEADER,
                              0);
  WAIT(20);
  OBJECT_EMOTE(LOCALID_PROUD_STUDENT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(LOCALID_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(LOCALID_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
               0);
  WAIT(60);
  MESSAGE_SET(MsgXianDidDoWarrior);
  Script_ShowMessageAndWait(LOCALID_PROUD_STUDENT, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_PROUD_STUDENT, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TREE_STUDENT, 1);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_TREE_STUDENT, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_TREE_STUDENT, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_MESSENGER_STUDENT, 2);
  WAIT(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_MESSENGER_STUDENT,
                              0);
  Script_AskYesNo(LOCALID_MESSENGER_STUDENT, 0);
  WAIT(50);
  // Bring the observers out through the school door.
  Engine_CameraFollowObjectEvent(LOCALID_MESSENGER_STUDENT, 1);
  OBJECT_SPEED(LOCALID_MESSENGER_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_MESSENGER_STUDENT, 176, 248);
  ObjectEvent_WalkToAndWait(LOCALID_MESSENGER_STUDENT, 308, 248);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  OBJECT_DIR(LOCALID_MESSENGER_STUDENT, FACING_NORTH, 20);
  SE(SE_DOOR_OPEN);
  MapGrid_AnimateCells(gFuchinTemple_FehsDoorOpens, 78, 13);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_MESSENGER_STUDENT, 2);
  WAIT(20);
  OBJECT_SPEED(LOCALID_MESSENGER_STUDENT, 3 * PIXELS(1) / 2, 3 * PIXELS(1) / 4);
  objectEvent = ObjectEvent_GetById(LOCALID_MESSENGER_STUDENT);
  objectEvent->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_MESSENGER_STUDENT, 308, 272);
  WAIT(1);
  objectEvent = ObjectEvent_Get(LOCALID_MESSENGER_STUDENT);

  objectEvent->steeringFlags =
      OBJ_EVENT_FACES_MOTION | objectEvent->steeringFlags;
  Script_ShowMessageAndWait(LOCALID_MESSENGER_STUDENT, 0, 50);
  OBJECT_POSITION_SET(LOCALID_FEIZHI, 304, 216);
  ObjectEvent_WalkToAndWait(LOCALID_FEIZHI, 304, 248);
  ObjectEvent_FaceObjectEvent(LOCALID_WELCOMING_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_KICKING_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PUNCHING_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_JUMPING_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PROUD_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WATCHING_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_TREE_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_MESSENGER_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_FEIZHI, 0);
  WAIT(10);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_WELCOMING_STUDENT, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_KICKING_STUDENT, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PUNCHING_STUDENT, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_JUMPING_STUDENT, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PROUD_STUDENT, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_WATCHING_STUDENT, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_TREE_STUDENT, 2);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_MESSENGER_STUDENT, 2);
  OBJECT_EMOTE(LOCALID_FEIZHI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  OBJECT_POSITION_SET(LOCALID_FEH, 304, 216);
  OBJECT_WALK(LOCALID_FEH, 304, 248);
  OBJECT_WALK(LOCALID_FEIZHI, 280, 264);
  Engine_ObjectEventWaitForMove(LOCALID_FEH);
  OBJECT_DIR(LOCALID_FEH, FACING_SOUTH + FACING_STEP, 0);
  Engine_ObjectEventWaitForMove(LOCALID_FEIZHI);
  SE(SE_159);
  MapGrid_AnimateCells(gFuchinTemple_FehsDoorCloses, 78, 13);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_WELCOMING_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_KICKING_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PUNCHING_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_JUMPING_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PROUD_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WATCHING_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_TREE_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_MESSENGER_STUDENT, LOCALID_FEIZHI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_FEIZHI, 0);
  WAIT(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FEIZHI, 2);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEIZHI, ANIMATION_NOD);
  WAIT(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FEH, 1);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  OBJECT_DIR(LOCALID_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  OBJECT_EMOTE(LOCALID_FEH, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEIZHI, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEIZHI, ANIMATION_NOD);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FEIZHI, 2);
  WAIT(20);
  OBJECT_DIR(LOCALID_FEIZHI, FACING_EAST, 20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  OBJECT_DIR(LOCALID_MESSENGER_STUDENT, FACING_WEST, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_MESSENGER_STUDENT,
                                        ANIMATION_NOD);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEIZHI, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_MESSENGER_STUDENT,
                                        ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_MESSENGER_STUDENT, 0, 20);
  OBJECT_DIR(LOCALID_FEIZHI, FACING_WEST, 20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_WELCOMING_STUDENT,
                                        ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_WELCOMING_STUDENT, 0, 20);
  OBJECT_DIR(LOCALID_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FEIZHI, 1);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  OBJECT_EMOTE(LOCALID_FEH, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  OBJECT_EMOTE(LOCALID_FEIZHI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  OBJECT_EMOTE(LOCALID_FEIZHI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  OBJECT_EMOTE(LOCALID_FEIZHI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  OBJECT_EMOTE(LOCALID_FEH, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEIZHI, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FEH, 2);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FEIZHI, 2);
  WAIT(10);
  ObjectEvent_WalkToAndWait(LOCALID_FEIZHI, 256, 280);
  OBJECT_DIR(LOCALID_FEIZHI, FACING_SOUTH, 20);
  OBJECT_POSITION_SET(LOCALID_FEIZHI, 0, 0);
  Engine_ObjectEventDestroy(LOCALID_FEIZHI);
  WAIT(30);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_WELCOMING_STUDENT, 2);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_WELCOMING_STUDENT, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TREE_STUDENT, 2);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_TREE_STUDENT, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_MESSENGER_STUDENT, LOCALID_FEH, 0);
  WAIT(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_MESSENGER_STUDENT, 2);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_MESSENGER_STUDENT, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_FEH, LOCALID_MESSENGER_STUDENT, 0);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  OBJECT_SPEED(LOCALID_FEH, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_FEH, 256, 248);
  OBJECT_DIR(LOCALID_FEH, FACING_NORTH, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_FEH, 1);
  OBJECT_EMOTE(LOCALID_FEH, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  ObjectEvent_WalkToAndWait(LOCALID_FEH, 240, 184);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FEH, 2);
  WAIT(20);
  OBJECT_POSITION_SET(LOCALID_TRAINING_TREE, 232, 168);
  OBJECT_POSITION_SET(LOCALID_TRAINING_TREE_TARGET, 232, 168);
  // Restore the tree before leaving the group ready for another demonstration.
  objectEvent = ObjectEvent_Get(LOCALID_TRAINING_TREE);
  objectEvent->y.fixed = 0xc0000;
  objectEvent = ObjectEvent_Get(LOCALID_TRAINING_TREE);
  objectEvent->targetY = OBJ_EVENT_NO_TARGET;
  objectEvent = ObjectEvent_Get(LOCALID_TRAINING_TREE);
  objectEvent->scaleX = 0xcccc;
  objectEvent = ObjectEvent_Get(LOCALID_TRAINING_TREE);
  {
    struct Sprite *sprite = objectEvent->sprite;
    s32 rotation = 0x8000;

    sprite->rotation = rotation;
  }
  SE(SE_124);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
  ObjectEvent_WalkToAndWait(LOCALID_MESSENGER_STUDENT, 256, 240);
  OBJECT_DIR(LOCALID_MESSENGER_STUDENT, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_MESSENGER_STUDENT, 1);
  Script_ShowMessageAndWait(LOCALID_MESSENGER_STUDENT, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_WELCOMING_STUDENT, LOCALID_PARTY_LEADER,
                              0);
  ObjectEvent_FaceObjectEvent(LOCALID_KICKING_STUDENT, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PUNCHING_STUDENT, LOCALID_PARTY_LEADER,
                              0);
  ObjectEvent_FaceObjectEvent(LOCALID_JUMPING_STUDENT, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PROUD_STUDENT, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WATCHING_STUDENT, LOCALID_PARTY_LEADER,
                              0);
  ObjectEvent_FaceObjectEvent(LOCALID_TREE_STUDENT, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_MESSENGER_STUDENT, LOCALID_PARTY_LEADER,
                              0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FEH, 2);
  WAIT(20);
  OBJECT_DIR(LOCALID_FEH, FACING_SOUTH + FACING_STEP, 20);
  ObjectEvent_WalkToAndWait(LOCALID_FEH, 248, 208);
  OBJECT_DIR(LOCALID_FEH, FACING_SOUTH + FACING_STEP, 20);
  Script_OpenMessage(LOCALID_FEH, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ObjectEventRunRepeatedMotion(LOCALID_MESSENGER_STUDENT, 1);
    WAIT(20);
    Script_ShowMessageAndWait(LOCALID_MESSENGER_STUDENT, 0, 20);
    FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI);
  } else {
    Script_SkipMessages(1);
    WAIT(20);
    OBJECT_EMOTE(LOCALID_FEH, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    OBJECT_DIR(LOCALID_FEH, FACING_SOUTH, 20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_MESSENGER_STUDENT, 2);
    WAIT(20);
    Script_OpenMessage(LOCALID_MESSENGER_STUDENT, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == 0) {
      Engine_ObjectEventSetAnimationAndWait(LOCALID_MESSENGER_STUDENT,
                                            ANIMATION_NOD);
      WAIT(20);
      OBJECT_DIR(LOCALID_FEH, FACING_NORTHWEST + FACING_STEP, 20);
      Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
      FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI);
    } else {
      Script_SkipMessages(1);
      WAIT(20);
      Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_SHAKE_HEAD);
      WAIT(20);
      Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
      FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED);
    }
  }
  OBJECT_DIR(LOCALID_KICKING_STUDENT, FACING_WEST, 0);
  OBJECT_DIR(LOCALID_PUNCHING_STUDENT, FACING_WEST, 20);
  OBJECT_ANIMATION(LOCALID_KICKING_STUDENT, ANIMATION_STUDENT_KICKS);
  OBJECT_ANIMATION(LOCALID_PUNCHING_STUDENT, ANIMATION_STUDENT_PUNCHES);
  ObjectEvent_SetActionScript(LOCALID_JUMPING_STUDENT,
                              gFuchinTemple_JumpingStudentScript);
}

/// Talking to Feh in his school in Xian after the party declined: he asks again
/// to see Isaac's Chi.
/// "Warrior, will you show me your Chi?"
static void EventScript_FehAsksToSeeChi(void) {

  SCRIPT_START();
  MESSAGE_SET(MsgXianWarriorWillShowMeYour);
  MESSAGE_OPEN(LOCALID_FEH, 0);

  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    WAIT(20);
    Engine_ScriptShowMessageAndWait(LOCALID_FEH, 0, 20);
    Engine_GameFlagSet(FLAG_FEH_WAITS_TO_SEE_CHI);
    SCRIPT_END();
  } else {
    gFieldWork->message += 1;
    Engine_ScriptShowMessageAndWait(LOCALID_FEH, 0, 20);
    SCRIPT_END();
  }
}

/// The tree falls from the line with Feh watching, but Feh sees that it is not
/// Chi. "Ya! The tree fell!" He asks whether the party has been to Fuchin
/// Temple, and the school goes back to its training.
static void FuchinTemple_FehSeesChi(void) {
  struct ObjectEvent *objectEvent;

  // Stop the temporary facing hooks while the group reacts to the tree.
  objectEvent = ObjectEvent_Get(LOCALID_FEH);
  objectEvent->update = 0;
  objectEvent = ObjectEvent_GetById(LOCALID_PROUD_STUDENT);
  objectEvent->update = 0;
  objectEvent = ObjectEvent_GetById(LOCALID_WATCHING_STUDENT);
  objectEvent->update = 0;
  objectEvent = ObjectEvent_GetById(LOCALID_TREE_STUDENT);
  objectEvent->update = 0;
  objectEvent = ObjectEvent_Get(LOCALID_MESSENGER_STUDENT);
  objectEvent->update = 0;
  OBJECT_ANIMATION(LOCALID_PUNCHING_STUDENT, ANIMATION_STAND);
  FieldCamera_SetSpeed(0x8000, 0x1000);
  FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(200), 1);
  Engine_CameraWaitForMove();
  // Continue the Chi discussion with the students gathered around the tree.
  MESSAGE_SET(MsgXianYaTreeFell);
  OBJECT_SPEED(LOCALID_KICKING_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJECT_SPEED(LOCALID_JUMPING_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJECT_WALK(LOCALID_KICKING_STUDENT, 152, 200);
  ObjectEvent_WalkToAndWait(LOCALID_JUMPING_STUDENT, 144, 248);
  Engine_ObjectEventWaitForMove(LOCALID_KICKING_STUDENT);
  ObjectEvent_FaceObjectEvent(LOCALID_WELCOMING_STUDENT, LOCALID_TRAINING_TREE,
                              0);
  ObjectEvent_FaceObjectEvent(LOCALID_PUNCHING_STUDENT, LOCALID_TRAINING_TREE,
                              0);
  ObjectEvent_FaceObjectEvent(LOCALID_PROUD_STUDENT, LOCALID_TRAINING_TREE, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WATCHING_STUDENT, LOCALID_TRAINING_TREE,
                              0);
  ObjectEvent_FaceObjectEvent(LOCALID_TREE_STUDENT, LOCALID_TRAINING_TREE, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_MESSENGER_STUDENT, LOCALID_TRAINING_TREE,
                              0);
  ObjectEvent_FaceObjectEvent(LOCALID_FEH, LOCALID_TRAINING_TREE, 0);
  OBJECT_SPEED(LOCALID_KICKING_STUDENT, 3 * PIXELS(1) / 2, 3 * PIXELS(1) / 4);
  OBJECT_SPEED(LOCALID_JUMPING_STUDENT, PIXELS(2), PIXELS(1));
  OBJECT_WALK(LOCALID_KICKING_STUDENT, 152, 200);
  ObjectEvent_WalkToAndWait(LOCALID_JUMPING_STUDENT, 144, 248);
  ObjectEvent_FaceObjectEvent(LOCALID_JUMPING_STUDENT, LOCALID_TRAINING_TREE,
                              0);
  Engine_ObjectEventWaitForMove(LOCALID_KICKING_STUDENT);
  ObjectEvent_FaceObjectEvent(LOCALID_KICKING_STUDENT, LOCALID_TRAINING_TREE,
                              0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FEH, 2);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 40);
  ObjectEvent_FaceObjectEvent(LOCALID_WELCOMING_STUDENT, LOCALID_FEH, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_KICKING_STUDENT, LOCALID_FEH, 0);
  OBJECT_DIR(LOCALID_PUNCHING_STUDENT, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_JUMPING_STUDENT, LOCALID_FEH, 0);
  OBJECT_DIR(LOCALID_PROUD_STUDENT, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WATCHING_STUDENT, LOCALID_FEH, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_TREE_STUDENT, LOCALID_FEH, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_MESSENGER_STUDENT, LOCALID_FEH, 0);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_MESSENGER_STUDENT, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_NOD);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_MESSENGER_STUDENT,
                                        ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_MESSENGER_STUDENT, 0, 20);
  OBJECT_EMOTE(LOCALID_FEH, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  OBJECT_EMOTE(LOCALID_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(LOCALID_MESSENGER_STUDENT, 0, 20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  OBJECT_EMOTE(LOCALID_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  OBJECT_EMOTE(LOCALID_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  OBJECT_SPEED(LOCALID_TREE_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_TREE_STUDENT, 216, 176);
  OBJECT_DIR(LOCALID_TREE_STUDENT, FACING_SOUTHEAST + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_TREE_STUDENT, 0, 20);
  OBJECT_DIR(LOCALID_FEH, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_WELCOMING_STUDENT, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_KICKING_STUDENT, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PUNCHING_STUDENT, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_JUMPING_STUDENT, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PROUD_STUDENT, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_WATCHING_STUDENT, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_TREE_STUDENT, 2);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_MESSENGER_STUDENT, 2);
  WAIT(40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PROUD_STUDENT, 2);
  Script_ShowMessageAndWait(LOCALID_PROUD_STUDENT, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 20);
  OBJECT_DIR(LOCALID_FEH, FACING_SOUTH + FACING_STEP, 20);
  Script_AskYesNo(LOCALID_FEH, 0);
  OBJECT_EMOTE(LOCALID_WELCOMING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(5);
  OBJECT_EMOTE(LOCALID_KICKING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(5);
  OBJECT_EMOTE(LOCALID_PUNCHING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(5);
  OBJECT_EMOTE(LOCALID_JUMPING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(5);
  OBJECT_EMOTE(LOCALID_PROUD_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(5);
  OBJECT_EMOTE(LOCALID_WATCHING_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(5);
  OBJECT_EMOTE(LOCALID_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(5);
  OBJECT_EMOTE(LOCALID_MESSENGER_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  WAIT(60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_MESSENGER_STUDENT, 2);
  Script_ShowMessageAndWait(LOCALID_MESSENGER_STUDENT, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  OBJECT_EMOTE(LOCALID_TREE_STUDENT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(LOCALID_TREE_STUDENT, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_FEH, LOCALID_TREE_STUDENT, 0);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 40);
  Engine_ObjectEventFaceEachOther(LOCALID_PUNCHING_STUDENT,
                                  LOCALID_KICKING_STUDENT, 0);
  Engine_ObjectEventFaceEachOther(LOCALID_JUMPING_STUDENT,
                                  LOCALID_WATCHING_STUDENT, 0);
  Engine_ObjectEventFaceEachOther(LOCALID_PROUD_STUDENT, LOCALID_TREE_STUDENT,
                                  0);
  WAIT(60);
  ObjectEvent_FaceObjectEvent(LOCALID_KICKING_STUDENT, LOCALID_FEH, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PUNCHING_STUDENT, LOCALID_FEH, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_JUMPING_STUDENT, LOCALID_FEH, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PROUD_STUDENT, LOCALID_FEH, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_WATCHING_STUDENT, LOCALID_FEH, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_TREE_STUDENT, LOCALID_FEH, 0);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FEH, 2);
  WAIT(20);
  OBJECT_ANIMATION(LOCALID_WELCOMING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_KICKING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_PUNCHING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_JUMPING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_PROUD_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_WATCHING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_TREE_STUDENT, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_MESSENGER_STUDENT,
                                        ANIMATION_NOD);
  WAIT(20);
  OBJECT_DIR(LOCALID_FEH, FACING_SOUTH + FACING_STEP, 20);
  Script_AskYesNo(LOCALID_FEH, 0);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_WELCOMING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_KICKING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_PUNCHING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_JUMPING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_PROUD_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_WATCHING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_TREE_STUDENT, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_MESSENGER_STUDENT,
                                        ANIMATION_NOD);
  WAIT(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FEH, 2);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEH, ANIMATION_NOD);
  WAIT(20);
  OBJECT_DIR(LOCALID_FEH, FACING_WEST, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FEH, 2);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEH, 0, 20);
  OBJECT_ANIMATION(LOCALID_WELCOMING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_KICKING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_PUNCHING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_JUMPING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_PROUD_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_WATCHING_STUDENT, ANIMATION_NOD);
  OBJECT_ANIMATION(LOCALID_TREE_STUDENT, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_MESSENGER_STUDENT,
                                        ANIMATION_NOD);
  WAIT(20);
  OBJECT_WALK(LOCALID_KICKING_STUDENT, 120, 200);
  OBJECT_WALK(LOCALID_JUMPING_STUDENT, 120, 248);
  Engine_ObjectEventWaitForMove(LOCALID_KICKING_STUDENT);
  OBJECT_DIR(LOCALID_PUNCHING_STUDENT, FACING_WEST, 20);
  OBJECT_ANIMATION(LOCALID_KICKING_STUDENT, ANIMATION_STUDENT_KICKS);
  OBJECT_ANIMATION(LOCALID_PUNCHING_STUDENT, ANIMATION_STUDENT_PUNCHES);
  Engine_ObjectEventWaitForMove(LOCALID_JUMPING_STUDENT);
  ObjectEvent_SetActionScript(LOCALID_JUMPING_STUDENT,
                              gFuchinTemple_JumpingStudentScript);
  OBJECT_SPEED(LOCALID_TREE_STUDENT, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_TREE_STUDENT, 216, 168);
  ObjectEvent_WalkToAndWait(LOCALID_TREE_STUDENT, 232, 168);
  OBJECT_DIR(LOCALID_TREE_STUDENT, FACING_NORTH, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TREE_STUDENT, 3);
  OBJECT_POSITION_SET(LOCALID_TRAINING_TREE, 232, 168);
  objectEvent = ObjectEvent_Get(LOCALID_TRAINING_TREE);
  objectEvent->y.fixed = 0xc0000;
  objectEvent = ObjectEvent_Get(LOCALID_TRAINING_TREE);
  objectEvent->targetY = OBJ_EVENT_NO_TARGET;
  objectEvent = ObjectEvent_Get(LOCALID_TRAINING_TREE);
  {
    struct Sprite *sprite = objectEvent->sprite;
    s32 rotation = 0x8000;

    sprite->rotation = rotation;
  }
  SE(SE_124);
  WAIT(40);
  ObjectEvent_WalkToAndWait(LOCALID_TREE_STUDENT, 216, 152);
  OBJECT_DIR(LOCALID_TREE_STUDENT, FACING_SOUTH, 30);
  GameFlag_Clear(FLAG_FEH_WAITS_TO_SEE_CHI);
  FLAG_SET(FLAG_FEH_SAW_KI);
}

/// Talking to a student (LOCALID_PUNCHING_STUDENT) in Master Feh's school in
/// Xian once Hsu is known to be trapped: the student asks about the flood.
/// "Water monsters flooded Altin. Did you defeat them, warrior?"
static void EventScript_StudentAsksAboutAltin(void) {
  SCRIPT_START();
  MESSAGE_SET(MsgXianWaterMonstersFloodedAltinDid);
  Engine_ScriptAskYesNo(LOCALID_PUNCHING_STUDENT, 0);
  SCRIPT_END();
}

/// Walking north into the door Feh and Feizhi came out of, in Master Feh's
/// school in Xian (coord event 3): it is locked until Feizhi has left; then it
/// opens and Isaac walks through (FUCHIN_TEMPLE_INTERIORS_TO_HOUSE). "It is
/// locked."
static void EventScript_OpenLockedDoor(void) {
  SCRIPT_START();
  if (FLAG_CHECK(FLAG_FEIZHI_WENT_TO_HSU) == 0 &&
      FLAG_CHECK(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
    Engine_MessageShowCentered(MsgXianItIsLocked, 1);
    SCRIPT_END();
  } else {
    SE(SE_DOOR_OPEN);
    MapGrid_AnimateCells(gFuchinTemple_FehsDoorOpens, 78, 13);
    OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 306, 248);
    OBJECT_WALK(LOCALID_PARTY_LEADER, 304, 216);
    WAIT(20);
    Engine_ScriptRequestWarp(FUCHIN_TEMPLE_INTERIORS_TO_HOUSE);
    SCRIPT_END();
  }
}

/// Talking to the healer at Fuchin Temple (FuchinTempleGrounds): across the
/// counter from the south, the Sanctum's services; from elsewhere, a word on
/// the healer's travels. "I'm traveling around the world to spread good will."
static void EventScript_TalkToHealer(void) {
  struct ObjectEvent *leader;
  u32 facing;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  facing = leader->facing;
  SCRIPT_START();

  if (FACING_IS_NORTH(facing)) {
    Engine_SanctumOpen(LOCALID_HEALER);
  } else {
    MESSAGE_SET(MsgXianImTravelingAroundWorldSpread);
    MESSAGE_PUT(LOCALID_HEALER, 0);
  }

  SCRIPT_END();
}

/// Reading Nyunpa's mind inside Fuchin Temple once he is enlightened: he
/// entrusts the world to Isaac. "Isaac... Is that you...? I am... counting on
/// you... to bring peace... to the world..."
static void EventScript_ReadNyunpasLastWish(void) {
  SCRIPT_START();
  Engine_ObjectEventRunRepeatedMotion(LOCALID_NYUNPA, 2);
  MESSAGE_SET(MsgXianIsaacAmCountingOnBring);
  MESSAGE_PUT(LOCALID_NYUNPA, 0);
  SCRIPT_END();
}

/// Selects the outdoor, school, or lesson map event table.
const struct MapEvent *Map_GetEvents(void) {
  if (gGameState.mapId == (s32)&MapId_FuchinTempleGrounds) {
    return gFuchinTempleGroundsEvents;
  }
  if (gGameState.warpDest == FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE) {
    return gFuchinTempleEventsFromLamaTemple;
  }
  return gFuchinTempleEvents;
}

/// Emits a falling drop of spray beside the leader during the grotto entrance
/// fade, white or, one drop in seven, with its colours rotated, and flickers
/// the leader, hidden and red and green swapped on alternate frames, as he
/// walks in behind the falls.
static void FuchinTemple_SpawnGrottoEntryParticle(void) {
  struct EffectOptions options;
  struct ObjectEvent *leader;
  s32 paletteDraw;
  s32 fallSpeed;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  options.palette = SPRITE_COLOR_WHITE;
  paletteDraw = (u32)(Engine_RandomNext() * 7) >> 16;
  if ((paletteDraw & 7) == 0)
    options.palette = SPRITE_COLOR_BRG;

  options.startScaleX = 0xb333;
  options.startScaleY = 0xcccc;

  fallSpeed = ((u32)(Engine_RandomNext() * 8) >> 16) * 13107;

  FieldEffect_Spawn(leader->x.fixed + ((8 - (gFrameCount & 15)) << 16),
                    leader->y.fixed + (192 << 13), leader->z.fixed, 0,
                    -fallSpeed, 0, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                    &options);

  if ((gFrameCount & 1) != 0)
    Engine_ObjectEventSetChildValue(LOCALID_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  else
    Engine_ObjectEventSetChildValue(LOCALID_PARTY_LEADER, SPRITE_COLOR_GRB);
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
  SCRIPT_START();
  OBJECT_SPEED(LOCALID_PARTY_LEADER, 5 * PIXELS(1) / 2, 5 * PIXELS(1) / 4);
  Engine_ObjectEventSetDestinationOffset(LOCALID_PARTY_LEADER, offsetX,
                                         offsetZ);
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 4, 0);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_LEADER_LEAP);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_LEADER_SPRING);
  SCRIPT_END();
}

/// Stepping into the waterfall grotto's entrance at Fuchin Temple
/// (FuchinTempleGrounds, coord event 3): spray falls over Isaac as the screen
/// fades, and he enters Fuchin Falls Cave.
static void EventScript_EnterWaterfallGrotto(void) {
  struct ObjectEvent *leader;
  TaskFunc particleTask;

  SCRIPT_START();
  // Keep the entry effect alive until the leader has moved into the fade.
  particleTask = FuchinTemple_SpawnGrottoEntryParticle;
  FieldScheduler_AddCallback(particleTask, TASK_PRIORITY_MAP);
  Engine_ObjectEventSetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 5,
                             PIXELS(1) / 10);
  gFieldWork->transitionFrames = 60;
  Engine_ScriptCloseScreen();
  Engine_AudioPlayCue(SE_CAST);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_WALK);
  Engine_ObjectEventSetDestinationOffset(LOCALID_PARTY_LEADER, 0, -6);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetChildValue(LOCALID_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetSpriteFlags(leader, 0);
  Task_Remove(particleTask);
  Engine_ScriptWaitForScreen();
  Engine_ScriptRequestWarp(FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE);
  SCRIPT_END();
}

/// Stepping south onto coord event 1 inside Fuchin Temple
/// (FuchinTempleInteriors): the party leaves by
/// FUCHIN_TEMPLE_INTERIORS_TO_FUCHIN_TEMPLE_GROUNDS, out to
/// FuchinTempleGrounds.
static void EventScript_LeaveTemple(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_ScriptRequestWarp(FUCHIN_TEMPLE_INTERIORS_TO_FUCHIN_TEMPLE_GROUNDS);
}

/// Walking north into the temple door at Fuchin Temple (FuchinTempleGrounds,
/// coord event 2): it opens, and Isaac walks inside, where Nyunpa meditates.
static void EventScript_EnterTemple(void) {
  SCRIPT_START();
  SE(SE_188);
  MapGrid_AnimateCells(gFuchinTemple_TempleDoorOpens, 77, 8);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 0;
  OBJECT_SPEED(LOCALID_PARTY_LEADER, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_WALK);
  ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0, -16);
  WAIT(16);
  Engine_ScriptRequestWarp(FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_TEMPLE_INTERIORS);
  SCRIPT_END();
}

/// Back inside Fuchin Temple from the grotto with the Orb of Force
/// (FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND), Nyunpa praises Isaac. "Excellent,
/// Isaac! I am glad to see you master the secrets of Fuchin Temple!" Nyunpa
/// names the power Ki, "force" in the western world, and says it will show the
/// monsters hiding in Mogall Forest.
static void FuchinTemple_NyunpaTeachesKi(void) {
  struct ObjectEvent *nyunpa;

  SCRIPT_START();
  OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJECT_WALK(LOCALID_PARTY_LEADER, 168, 504);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  WAIT(20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_NYUNPA, 2);
  ObjectEvent_SetAttachedEffect(LOCALID_NYUNPA, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
  WAIT(60);
  ObjectEvent_GetById(LOCALID_NYUNPA)->paused = 0;
  SE(SE_JUMP);
  nyunpa = ObjectEvent_Get(LOCALID_NYUNPA);
  nyunpa->velocityY = 0x80000;
  OBJECT_ANIMATION(LOCALID_NYUNPA, ANIMATION_STAND);
  // Praise the completed test before answering the follow-up questions.
  MESSAGE_SET(MsgXianExcellentIsaac);
  Script_ShowMessageAndWait(LOCALID_NYUNPA, 0, 20);
  Script_ShowMessageAndWait(LOCALID_NYUNPA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_NYUNPA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_NYUNPA, 0, 20);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_OpenMessage(LOCALID_NYUNPA, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    WAIT(10);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_NYUNPA, ANIMATION_NOD);
    WAIT(20);
    Script_ShowMessageAndWait(LOCALID_NYUNPA, 0, 20);
    gFieldWork->message += 2;
  } else {
    WAIT(10);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_NYUNPA, 2);
    WAIT(20);
    gFieldWork->message++;
    Script_ShowMessageAndWait(LOCALID_NYUNPA, 0, 20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_NYUNPA, ANIMATION_NOD);
    WAIT(20);
    Script_ShowMessageAndWait(LOCALID_NYUNPA, 0, 20);
  }
  Script_ShowMessageAndWait(LOCALID_NYUNPA, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_NYUNPA, 2);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_NYUNPA, 0, 20);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_OpenMessage(LOCALID_NYUNPA, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 1) {
    WAIT(10);
    OBJECT_EMOTE(LOCALID_NYUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    MESSAGE_SET(MsgXianCanReadMindsKnowCurious);
    Script_OpenMessage(LOCALID_NYUNPA, 0);
    do {
      if (Engine_ScriptChooseYesNo(0, 0) != 1) {
        break;
      }
      WAIT(10);
      OBJECT_EMOTE(LOCALID_NYUNPA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      MESSAGE_SET(MsgXianDontTryHard);
      Script_OpenMessage(LOCALID_NYUNPA, 0);
    } while (1);
  }
  // Explain why hidden monsters make the new ability useful.
  MESSAGE_SET(MsgXianMonstersWaitInHidingWould);
  WAIT(10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_NYUNPA, ANIMATION_NOD);
  WAIT(20);
  Script_OpenMessage(LOCALID_NYUNPA, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    WAIT(10);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    WAIT(20);
    Script_ShowMessageAndWait(LOCALID_NYUNPA, 0, 20);
    gFieldWork->message++;
  } else {
    WAIT(10);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_NYUNPA, 2);
    gFieldWork->message++;
    Script_ShowMessageAndWait(LOCALID_NYUNPA, 0, 20);
  }
  Engine_ObjectEventSetAnimationAndWait(LOCALID_NYUNPA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_NYUNPA, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_NYUNPA, 2);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_NYUNPA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_NYUNPA, ANIMATION_NOD);
  WAIT(20);
  OBJECT_ANIMATION(LOCALID_NYUNPA, ANIMATION_NYUNPA_MEDITATE);
  FLAG_SET(FLAG_NYUNPA_TAUGHT_FORCE);
  SCRIPT_END();
}

/// Entering Master Hama's room in Lama Temple the first time, the party is
/// greeted. "So, Isaac, you and your Adept companions have finally come." Hama
/// tells Ivan he is a Jupiter Adept like her and means to teach him Reveal for
/// crossing the Lamakan Desert. Feizhi bursts in: Hsu is in great danger among
/// the boulders in the mountains. Hama gives Ivan Reveal and hurries with her
/// to his aid, and the party is left to choose between helping Hsu and
/// following the people it chases (FLAG_XIAN_HEARD_HSU_TRAPPED).
static void FuchinTemple_RunHamasRevealLesson(void) {
  struct ObjectEvent *leader;

  SCRIPT_START();
  OBJECT_SPEED(LOCALID_PARTY_LEADER, 2 * PIXELS(1) / 5, PIXELS(1) / 5);
  OBJECT_WALK(LOCALID_PARTY_LEADER, 472, 536);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  OBJECT_DIR(LOCALID_HAMAS_BOY_MONK, FACING_EAST, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_HAMAS_BOY_MONK, 2);
  WAIT(20);
  MESSAGE_SET(MsgXianMasterHamaMeditatingPleaseExtremely);
  Script_ShowMessageAndWait(LOCALID_HAMAS_BOY_MONK, 0, 20);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_WEST, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  WAIT(30);
  OBJECT_DIR(LOCALID_HAMA, FACING_SOUTH, 30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  WAIT(20);
  OBJECT_DIR(LOCALID_HAMAS_BOY_MONK, FACING_NORTH + FACING_STEP, 0);
  // Bring the other party members out from the leader for the lesson.
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
  }
  OBJECT_SPEED(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJECT_SPEED(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJECT_SPEED(IVAN, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJECT_SPEED(MIA, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJECT_WALK(LOCALID_PARTY_LEADER, 464, 504);
  OBJECT_WALK(IVAN, 480, 504);
  OBJECT_WALK(GARET, 496, 496);
  OBJECT_WALK(MIA, 448, 496);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  Engine_ObjectEventWaitForMove(IVAN);
  Engine_ObjectEventWaitForMove(MIA);
  OBJECT_DIR(MIA, FACING_NORTHEAST, 0);
  Engine_ObjectEventWaitForMove(GARET);
  OBJECT_DIR(GARET, FACING_NORTHWEST, 0);
  WAIT(20);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  OBJECT_DIR(GARET, FACING_SOUTH + FACING_STEP, 20);
  Script_AskYesNo(GARET, 0);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_DIR(GARET, FACING_NORTHWEST, 20);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Script_ShowMessageAndWait(MIA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  OBJECT_DIR(LOCALID_HAMA, FACING_SOUTHEAST + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  OBJECT_DIR(GARET, FACING_SOUTHWEST, 0);
  OBJECT_DIR(MIA, FACING_SOUTHEAST, 0);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 2);
  ObjectEvent_SetAttachedEffect(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                          ATTACHED_EFFECT_ONCE);
  WAIT(60);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  OBJECT_DIR(GARET, FACING_NORTHWEST, 0);
  OBJECT_DIR(MIA, FACING_NORTHEAST, 0);
  WAIT(20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  WAIT(20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, IVAN, 50);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  OBJECT_DIR(IVAN, FACING_NORTH, 30);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(MIA, 0, 20);
  OBJECT_DIR(LOCALID_HAMA, FACING_SOUTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(MIA, 1);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  OBJECT_DIR(GARET, FACING_SOUTHWEST, 0);
  WAIT(30);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  OBJECT_DIR(GARET, FACING_NORTHWEST, 20);
  Script_ShowMessageAndWait(GARET, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  ObjectEvent_FaceObjectEvent(LOCALID_HAMA, IVAN, 0);
  WAIT(10);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 0);
  ObjectEvent_FaceObjectEvent(GARET, IVAN, 0);
  ObjectEvent_FaceObjectEvent(MIA, IVAN, 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
  WAIT(20);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  WAIT(20);
  Script_ShowMessageAndWait(GARET, 0, 20);
  OBJECT_EMOTE(LOCALID_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  ObjectEvent_FaceObjectEvent(LOCALID_HAMA, GARET, 0);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_HAMA, GARET, 0);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  OBJECT_DIR(GARET, FACING_NORTHWEST, 0);
  OBJECT_DIR(MIA, FACING_NORTHEAST, 0);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 2);
  WAIT(20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(MIA, 2);
  WAIT(20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  OBJECT_DIR(LOCALID_HAMA, FACING_SOUTH + FACING_STEP, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 2);
  Engine_ObjectEventRunRepeatedMotion(MIA, 2);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_HAMA, 2);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  OBJECT_ANIMATION(GARET, ANIMATION_NOD);
  OBJECT_ANIMATION(IVAN, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  WAIT(60);
  Script_ShowMessageAndWait(GARET, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(GARET, 0, 20);
  OBJECT_EMOTE(LOCALID_HAMA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_HAMA, 1);
  WAIT(60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  Engine_ObjectEventStartRepeatedMotion(GARET, 1);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 1);
  Engine_ObjectEventRunRepeatedMotion(MIA, 1);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  OBJECT_DIR(GARET, FACING_SOUTH + FACING_STEP, 20);
  Script_AskYesNo(GARET, 0);
  WAIT(20);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
  WAIT(20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  OBJECT_DIR(GARET, FACING_NORTHWEST, 20);
  OBJECT_DIR(LOCALID_HAMA, FACING_NORTH, 20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  OBJECT_ANIMATION(GARET, ANIMATION_NOD);
  OBJECT_ANIMATION(IVAN, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
  WAIT(20);
  OBJECT_DIR(LOCALID_HAMA, FACING_SOUTH, 20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 2);
  Engine_ObjectEventRunRepeatedMotion(MIA, 2);
  WAIT(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT |
                                         ATTACHED_EFFECT_ONCE);
  WAIT(60);
  Script_ShowMessageAndWait(MIA, 0, 20);
  OBJECT_DIR(LOCALID_HAMA, FACING_SOUTH + FACING_STEP, 20);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  WAIT(20);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, GARET, 0);
  ObjectEvent_FaceObjectEvent(IVAN, GARET, 0);
  ObjectEvent_FaceObjectEvent(MIA, GARET, 0);
  WAIT(20);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  OBJECT_ANIMATION(IVAN, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
  WAIT(60);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  OBJECT_DIR(IVAN, FACING_NORTH, 0);
  OBJECT_DIR(MIA, FACING_NORTH + FACING_STEP, 0);
  WAIT(20);
  Script_AskYesNo(LOCALID_HAMA, 0);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  WAIT(60);
  OBJECT_DIR(GARET, FACING_SOUTHWEST, 20);
  Script_AskYesNo(GARET, 0);
  WAIT(20);
  OBJECT_DIR(MIA, FACING_SOUTHEAST, 20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  OBJECT_DIR(GARET, FACING_NORTHWEST, 0);
  OBJECT_DIR(MIA, FACING_NORTHEAST, 30);
  OBJECT_DIR(LOCALID_HAMA, FACING_EAST, 20);
  OBJECT_EMOTE(LOCALID_HAMA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  OBJECT_DIR(LOCALID_HAMA, FACING_NORTH, 20);
  OBJECT_EMOTE(LOCALID_HAMA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  OBJECT_ANIMATION(GARET, ANIMATION_NOD);
  OBJECT_ANIMATION(IVAN, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
  WAIT(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_HAMA, 1);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  OBJECT_DIR(GARET, FACING_SOUTHWEST, 0);
  OBJECT_DIR(IVAN, FACING_WEST, 0);
  OBJECT_DIR(MIA, FACING_SOUTHEAST, 0);
  WAIT(60);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  OBJECT_DIR(GARET, FACING_NORTHWEST, 0);
  OBJECT_DIR(IVAN, FACING_NORTH, 0);
  OBJECT_DIR(MIA, FACING_NORTHEAST, 0);
  WAIT(30);
  OBJECT_DIR(LOCALID_HAMA, FACING_SOUTH, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  // Interrupt the discussion with the arrival from the southern path.
  OBJECT_POSITION_SET(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 472, 608);
  Script_ShowMessageAndWait(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(LOCALID_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  OBJECT_EMOTE(LOCALID_HAMAS_BOY_MONK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  WAIT(60);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  OBJECT_DIR(GARET, FACING_SOUTH, 0);
  OBJECT_DIR(IVAN, FACING_SOUTH, 0);
  OBJECT_DIR(MIA, FACING_SOUTH, 0);
  OBJECT_DIR(LOCALID_HAMAS_BOY_MONK, FACING_EAST, 0);
  WAIT(30);
  OBJECT_SPEED(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 4 * PIXELS(1) / 5,
               2 * PIXELS(1) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 472, 536);
  OBJECT_EMOTE(LOCALID_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_DIR(IVAN, FACING_NORTH, 40);
  ObjectEvent_WalkToAndWait(IVAN, 488, 512);
  WAIT(10);
  OBJECT_DIR(IVAN, FACING_SOUTHWEST, 20);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_EAST, 30);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 456, 512);
  WAIT(10);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 2);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  OBJECT_EMOTE(LOCALID_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 2);
  ObjectEvent_SetAttachedEffect(LOCALID_FEIZHI_IN_LAMA_TEMPLE,
                                ATTACHED_EFFECT_IN_FRONT |
                                    ATTACHED_EFFECT_ONCE);
  WAIT(60);
  Script_ShowMessageAndWait(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  OBJECT_EMOTE(LOCALID_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_EMOTE(LOCALID_FEIZHI_IN_LAMA_TEMPLE,
               EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Script_ShowMessageAndWait(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_HAMA, 1);
  OBJECT_EMOTE(LOCALID_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
  OBJECT_DIR(GARET, FACING_NORTHWEST, 0);
  OBJECT_DIR(IVAN, FACING_NORTHWEST, 0);
  OBJECT_DIR(MIA, FACING_NORTHEAST, 0);
  WAIT(40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEIZHI_IN_LAMA_TEMPLE,
                                        ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_EMOTE(LOCALID_HAMA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  // BUG: LOCALID_FEIZHI is Feizhi's number in the school's table. Hama's room
  // places no object event at that number, so Feizhi does not bounce before she
  // speaks.
#ifdef BUGFIX
  Engine_ObjectEventStartRepeatedMotion(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 1);
#else
  Engine_ObjectEventStartRepeatedMotion(LOCALID_FEIZHI, 1);
#endif
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  OBJECT_EMOTE(LOCALID_HAMA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_EMOTE(LOCALID_HAMA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  WAIT(2);
  OBJECT_ANIMATION(IVAN, ANIMATION_NOD);
  WAIT(1);
  OBJECT_ANIMATION(MIA, ANIMATION_NOD);
  WAIT(5);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  ObjectEvent_FaceObjectEvent(LOCALID_HAMA, LOCALID_PARTY_LEADER, 0);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_SPEED(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 4 * PIXELS(1) / 5,
               2 * PIXELS(1) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 472, 504);
  WAIT(20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 1);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_HAMA, LOCALID_FEIZHI_IN_LAMA_TEMPLE, 0);
  OBJECT_SPEED(LOCALID_HAMA, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  OBJECT_DIR(GARET, FACING_SOUTHWEST, 0);
  OBJECT_DIR(IVAN, FACING_SOUTHWEST, 0);
  OBJECT_DIR(MIA, FACING_SOUTHEAST, 0);
  Engine_ObjectEventWaitForMove(LOCALID_HAMA);
  WAIT(30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_WALK(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 472, 568);
  ObjectEvent_WalkToAndWait(LOCALID_HAMA, 472, 536);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ObjectEventWaitForMove(LOCALID_FEIZHI_IN_LAMA_TEMPLE);
  Script_ShowMessageAndWait(GARET, 0, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_HAMA, 2);
  OBJECT_EMOTE(LOCALID_HAMA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  OBJECT_DIR(LOCALID_FEIZHI_IN_LAMA_TEMPLE, FACING_NORTH + FACING_STEP, 0);
  OBJECT_DIR(LOCALID_HAMA, FACING_NORTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 2);
  Engine_ObjectEventRunRepeatedMotion(MIA, 2);
  ObjectEvent_WalkToAndWait(LOCALID_HAMA, 472, 512);
  OBJECT_DIR(LOCALID_HAMA, FACING_EAST, 0);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  OBJECT_DIR(IVAN, FACING_WEST, 0);
  OBJECT_DIR(LOCALID_HAMAS_BOY_MONK, FACING_NORTH + FACING_STEP, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
  WAIT(20);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 448, 512);
  WAIT(1);
  {
    struct ObjectEvent *leaderObjectEvent =
        ObjectEvent_Get(LOCALID_PARTY_LEADER);
    leaderObjectEvent->steeringFlags =
        OBJ_EVENT_FACES_MOTION | leaderObjectEvent->steeringFlags;
  }
  WAIT(20);
  // Finish Ivan's lesson before adding Reveal to his ability list.
  FuchinTemple_AnimateRevealLesson();
  WAIT(60);
  Party_LearnAbility(IVAN, ABILITY_REVEAL);
  // Let Hama and her monk leave, then gather the party into the leader.
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  OBJECT_ANIMATION(GARET, ANIMATION_NOD);
  OBJECT_ANIMATION(IVAN, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
  WAIT(20);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  OBJECT_DIR(GARET, FACING_SOUTHWEST, 0);
  OBJECT_DIR(IVAN, FACING_SOUTHWEST, 0);
  OBJECT_DIR(MIA, FACING_SOUTHEAST, 0);
  OBJECT_DIR(LOCALID_HAMAS_BOY_MONK, FACING_SOUTHEAST + FACING_STEP, 0);
  WAIT(20);
  ObjectEvent_WalkToAndWait(LOCALID_HAMA, 472, 552);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_HAMA, 2);
  WAIT(20);
  OBJECT_DIR(LOCALID_HAMA, FACING_NORTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  WAIT(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_SPEED(LOCALID_HAMA, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkToAndWait(LOCALID_HAMA, 480, 540);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  OBJECT_SPEED(LOCALID_HAMA, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJECT_SPEED(LOCALID_HAMAS_BOY_MONK, 4 * PIXELS(1) / 5, 2 * PIXELS(1) / 5);
  OBJECT_WALK(LOCALID_HAMA, 472, 608);
  OBJECT_WALK(LOCALID_HAMAS_BOY_MONK, 472, 544);
  ObjectEvent_WalkToAndWait(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 472, 608);
  Engine_ObjectEventWaitForMove(LOCALID_HAMAS_BOY_MONK);
  OBJECT_WALK(LOCALID_HAMAS_BOY_MONK, 472, 608);
  OBJECT_POSITION_SET(LOCALID_FEIZHI_IN_LAMA_TEMPLE, 0, 0);
  Engine_ObjectEventWaitForMove(LOCALID_HAMA);
  OBJECT_POSITION_SET(LOCALID_HAMA, 0, 0);
  Engine_ObjectEventWaitForMove(LOCALID_HAMAS_BOY_MONK);
  OBJECT_POSITION_SET(LOCALID_HAMAS_BOY_MONK, 0, 0);
  ObjectEvent_WalkToAndWait(IVAN, 488, 520);
  OBJECT_EMOTE(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ObjectEventRunRepeatedMotion(MIA, 1);
  WAIT(20);
  OBJECT_DIR(MIA, FACING_SOUTHEAST, 20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  OBJECT_DIR(IVAN, FACING_NORTHWEST, 20);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  OBJECT_EMOTE(MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  OBJECT_DIR(GARET, FACING_SOUTHWEST, 20);
  Script_AskYesNo(GARET, 0);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  WAIT(20);
  Engine_ObjectEventRunRepeatedMotion(MIA, 2);
  WAIT(20);
  OBJECT_DIR(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  OBJECT_DIR(GARET, FACING_WEST, 0);
  Script_ShowMessageAndWait(MIA, 0, 20);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  OBJECT_EMOTE(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Script_OpenMessage(MIA, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    WAIT(20);
    Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
    WAIT(20);
    Script_ShowMessageAndWait(GARET, 0, 20);
    gFieldWork->message++;
  } else {
    WAIT(20);
    gFieldWork->message++;
    Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
    WAIT(20);
    Script_ShowMessageAndWait(MIA, 0, 20);
  }
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Script_ShowMessageAndWait(GARET, 0, 20);
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
  WAIT(20);
  OBJECT_WALK(GARET, 448, 512);
  ObjectEvent_WalkToAndWait(MIA, 448, 512);
  OBJECT_POSITION_SET(MIA, 0, 0);
  Engine_ObjectEventWaitForMove(GARET);
  OBJECT_POSITION_SET(GARET, 0, 0);
  WAIT(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, IVAN, 20);
  WAIT(30);
  ObjectEvent_FaceObjectEvent(IVAN, LOCALID_PARTY_LEADER, 20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  OBJECT_EMOTE(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 2);
  ObjectEvent_SetAttachedEffect(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                          ATTACHED_EFFECT_ONCE);
  WAIT(60);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
  WAIT(20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  ObjectEvent_WalkToAndWait(IVAN, 448, 512);
  OBJECT_POSITION_SET(IVAN, 0, 0);
  SCRIPT_END();
  FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED);
}

/// Inside Fuchin Temple, Nyunpa meditates, or teaches Ki after the test; in
/// Master Feh's school in Xian the tree and students stand as the story left
/// them; in Lama Temple, Hama teaches Reveal on the first visit and is away
/// until Hsu is rescued. At Fuchin Temple (FuchinTempleGrounds) the monk
/// beneath the falls stands where the story left him.
s32 Map_Initialize(void) {
  struct ObjectEvent *trainingTree;
  s32 heardHsuTrapped;

  if (gGameState.mapId == (s32)&MapId_FuchinTempleInteriors) {
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    if (gGameState.warpDest ==
        FUCHIN_TEMPLE_INTERIORS_FROM_FUCHIN_TEMPLE_GROUNDS) {
      if (Engine_GameFlagIsSet(FLAG_NYUNPA_ENLIGHTENED)) {
        OBJECT_ANIMATION(LOCALID_NYUNPA, ANIMATION_NYUNPA_WITHERED);
      } else {
        OBJECT_ANIMATION(LOCALID_NYUNPA, ANIMATION_NYUNPA_MEDITATE);
        if (Engine_GameFlagIsSet(FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND) &&
            !Engine_GameFlagIsSet(FLAG_NYUNPA_TAUGHT_FORCE) &&
            !Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
          FuchinTemple_NyunpaTeachesKi();
        }
      }
    } else if (gGameState.warpDest == FUCHIN_TEMPLE_INTERIORS_FROM_XIAN ||
               gGameState.warpDest == FUCHIN_TEMPLE_INTERIORS_FROM_HOUSE) {
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
      heardHsuTrapped = Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED);
      if (heardHsuTrapped == 0) {
        trainingTree = ObjectEvent_GetById(LOCALID_TRAINING_TREE);
        trainingTree->motionFlags = heardHsuTrapped;
        trainingTree->y.fixed = 0xc0000;
        trainingTree->targetY = 0xc0000;
        trainingTree->scaleX = 0xcccc;
        trainingTree->scaleY = 0x8000;
        trainingTree->sprite->rotation = 0x8000;
        if (Engine_GameFlagIsSet(FLAG_FEIZHI_WENT_TO_HSU)) {
          Engine_ObjectEventSetPosition(LOCALID_FEH, PIXELS(248), PIXELS(208));
          if (!Engine_GameFlagIsSet(FLAG_FEH_SAW_KI)) {
            Engine_ObjectEventSetPosition(LOCALID_MESSENGER_STUDENT,
                                          PIXELS(256), PIXELS(240));
            ObjectEvent_GetById(LOCALID_FEH)->update = FuchinTemple_FaceLeader;
            ObjectEvent_GetById(LOCALID_PROUD_STUDENT)->update =
                FuchinTemple_FaceLeader;
            ObjectEvent_GetById(LOCALID_WATCHING_STUDENT)->update =
                FuchinTemple_FaceLeader;
            ObjectEvent_GetById(LOCALID_TREE_STUDENT)->update =
                FuchinTemple_FaceLeader;
            ObjectEvent_GetById(LOCALID_MESSENGER_STUDENT)->update =
                FuchinTemple_FaceLeader;
          }
        }
      } else {
        trainingTree = ObjectEvent_GetById(LOCALID_TRAINING_TREE);
        trainingTree->motionFlags = 0;
        trainingTree->y.fixed = 0xc0000;
        trainingTree->targetY = 0xc0000;
        trainingTree->scaleX = 0xcccc;
        trainingTree->scaleY = 0x8000;
        trainingTree->collisionFlags |= OBJ_EVENT_NOT_TARGETED;
        trainingTree->sprite->rotation = 0x8000;
        MapGrid_CopyCellAttributes(14, 11, 1, 1, 14, 10);
      }
      // A crate painted solid black and translucent, over the lattice door to
      // Feh's house (FUCHIN_TEMPLE_INTERIORS_TO_HOUSE). The school's render
      // is the same with and without it; drawn plain, it covers the door.
      ObjectEvent_PrepareObject(PIXELS(304), PIXELS(24), PIXELS(224),
                                SPRITE_CRATE);
      OBJECT_ANIMATION(LOCALID_KICKING_STUDENT, ANIMATION_STUDENT_KICKS);
      OBJECT_ANIMATION(LOCALID_PUNCHING_STUDENT, ANIMATION_STUDENT_PUNCHES);
    } else if (gGameState.warpDest ==
               FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE) {
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
      if (!Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
        FuchinTemple_RunHamasRevealLesson();
      } else if (!Engine_GameFlagIsSet(FLAG_HSU_RESCUED)) {
        Engine_ObjectEventSetPosition(LOCALID_HAMA, 0, 0);
        Engine_ObjectEventSetPosition(LOCALID_HAMAS_BOY_MONK, 0, 0);
      }
    }
  } else {
    Map_QueueMusic(SE_170);
    ObjectEvent_GetById(LOCALID_FALLS_MONK)->collisionFlags |=
        OBJ_EVENT_NEAR_PSYNERGY;
    if (gGameState.warpDest ==
            FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE &&
        Engine_GameFlagIsSet(FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND) &&
        !Engine_GameFlagIsSet(FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST)) {
      MapGrid_CopyCellAttributes(10, 84, 1, 1, 10, 24);
    }
    if (Engine_GameFlagIsSet(FLAG_FUCHIN_FALLS_GROTTO_OPENED)) {
      OBJECT_POSITION_SET(LOCALID_FALLS_MONK, 152, 392);
      Engine_ObjectEventFaceDirection(LOCALID_FALLS_MONK, FACING_EAST, 0);
      MapGrid_CopyCellAttributes(10, 26, 1, 1, 10, 22);
    }
  }
  return 0;
}

/// Starts Mind Read from caster on target as part of a cutscene, without the
/// window that announces its name. Nyunpa's Mind Read map events carry
/// PSYNERGY_SCRIPT_PLAYS_EFFECT, so the field plays nothing of its own.
static void FuchinTemple_StartMindReadDemonstration(s32 caster, s32 target) {
  struct FieldEffectWork *effectWork;

  Engine_GameFlagSet(FLAG_PSYNERGY_EFFECT);
  Engine_PsynergyStart(ABILITY_MIND_READ, 1);
  effectWork = gFieldEffectWork;
  Engine_PsynergySetTarget(caster, target);
  effectWork->active = 0;
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(PSYNERGY_STEP_ON_TARGET);
  Engine_TaskWait(1);
}

/// Finishes the demonstration's effect and lowers the caster's hands.
static void FuchinTemple_FinishMindReadDemonstration(void) {
  OBJECT_ANIMATION(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_PsynergyPlayEffect(PSYNERGY_STEP_FINISH);
  Engine_PsynergyLowerHands();
}

/// Emits eight rising particles from Nyunpa, sounding every other shot.
static void FuchinTemple_SpawnNyunpasParticles(void) {
  struct EffectOptions options;
  struct ObjectEvent *nyunpa;
  u32 shotIndex;

  nyunpa = ObjectEvent_GetById(LOCALID_NYUNPA);
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
    FieldEffect_Spawn(nyunpa->x.fixed, nyunpa->y.fixed,
                      nyunpa->z.fixed + 0xffe80000, 0, 0x9999, 0,
                      EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                          EFFECT_SCALE_TO_TARGET | EFFECT_USE_PRIORITY |
                          EFFECT_SCRIPT_MEDIUM,
                      &options);
  }
  WAIT(60);
}

/// Hama's and Ivan's update hook during the Reveal lesson: they glow, drawn red
/// and blue in turn every two frames.
static void
FuchinTemple_AlternateLessonChildEffect(union FieldObject *fieldObject) {
  if ((*(u32 *)&gFrameCount >> 1) & 1) {
    ObjectGroup_SetChildValue((struct ObjectEvent *)fieldObject,
                              SPRITE_COLOR_RED_TINT);
  } else {
    ObjectGroup_SetChildValue((struct ObjectEvent *)fieldObject,
                              SPRITE_COLOR_BLUE_TINT);
  }
}

/// Animates Hama and Ivan's lesson effects, fades the palette and clears the
/// callbacks.
static void FuchinTemple_AnimateRevealLesson(void) {
  struct EffectOptions options;
  struct ObjectEvent *hama;
  u32 particleIndex;
  s32 particleX;
  s32 particleY;
  s32 riseSpeed;

  Engine_AudioPlayCue(SE_131);
  ObjectEvent_GetById(LOCALID_HAMA)->update =
      FuchinTemple_AlternateLessonChildEffect;
  WAIT(40);
  Engine_ColorBufferApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  Engine_ColorBufferApplyTarget(PALETTE_TINT_CUSTOM | RGB(20, 2, 23),
                                PALETTES_BG);
  Engine_ColorBufferInterpolate(60);
  WAIT(40);
  Engine_AudioPlayCue(SE_131);
  ObjectEvent_GetById(IVAN)->update = FuchinTemple_AlternateLessonChildEffect;
  WAIT(120);
  hama = ObjectEvent_GetById(LOCALID_HAMA);
  // Emit the lesson particles while both temporary child effects run.
  options.priority = 1;
  options.palette = SPRITE_COLOR_RBG;
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
    FieldEffect_Spawn(
        particleX, particleY, hama->z.fixed, 0, riseSpeed, 0,
        EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
    Engine_TaskWait(2);
  }
  Engine_AudioPlayCue(SE_220);
  WAIT(30);
  // Fade back and remove both hooks before the conversation resumes.
  Engine_ColorBufferApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  Engine_ColorBufferInterpolate(60);
  WAIT(40);
  ObjectEvent_GetById(LOCALID_HAMA)->update = NULL;
  ObjectEvent_GetById(IVAN)->update = NULL;
  Engine_ObjectEventSetChildValue(LOCALID_HAMA, SPRITE_COLOR_NORMAL);
  Engine_ObjectEventSetChildValue(IVAN, SPRITE_COLOR_NORMAL);
}

s32 gShortEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gMediumEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gLongEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gShortEffectScript,
    gMediumEffectScript,
    gLongEffectScript,
};

s32 gFuchinTemple_JumpingStudentScript[] = {
    SC_ANIMATION(0),
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
    SC_CALL(FuchinTemple_ClearScriptedObjectEventSpriteFlags),
    SC_WAIT(1),
    SC_END,
};

// The doors' map cells, frame by frame: Fuchin Temple's door opening as Isaac
// walks in (EventScript_EnterTemple), and the door Feh and Feizhi come out of
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

struct MapWarpDest gFuchinTempleGroundsWarpDests[] = {
    {0, CONDITION_ALWAYS, 296, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {FUCHIN_TEMPLE_GROUNDS_FROM_WORLD_MAP, CONDITION_ALWAYS, 296, 0, 504,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_TEMPLE_INTERIORS, CONDITION_ALWAYS, 296,
     0, 304, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS,
     168, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {FUCHIN_TEMPLE_GROUNDS_AFTER_DEFEAT, CONDITION_ALWAYS, 232, 0, 328,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gFuchinTempleWarpDestsOther[] = {
    {0, CONDITION_ALWAYS, 168, 0, 552, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {FUCHIN_TEMPLE_INTERIORS_FROM_FUCHIN_TEMPLE_GROUNDS, CONDITION_ALWAYS, 168,
     0, 552, FACING_NORTH, 0, 48, 384, 288, 576, 0},
    {FUCHIN_TEMPLE_INTERIORS_FROM_XIAN, CONDITION_ALWAYS, 256, 0, 264,
     FACING_NORTH, 0, 48, 80, 368, 288, 0},
    {FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE, CONDITION_ALWAYS, 472, 0, 552,
     FACING_NORTH, 0, 352, 384, 592, 576, 0},
    {FUCHIN_TEMPLE_INTERIORS_FROM_HOUSE, CONDITION_ALWAYS, 304, 0, 248,
     FACING_SOUTH, 0, 48, 80, 368, 288, 0},
    {MAP_TABLE_END},
};

u32 gFuchinTempleWarps[] = {
    MAP_WARPS(FuchinTempleGrounds),
    MAP_WARP(FUCHIN_TEMPLE_GROUNDS_TO_WORLD_MAP, WorldMap,
             WORLD_MAP_FROM_FUCHIN_TEMPLE_GROUNDS),
    MAP_WARP(FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_TEMPLE_INTERIORS,
             FuchinTempleInteriors,
             FUCHIN_TEMPLE_INTERIORS_FROM_FUCHIN_TEMPLE_GROUNDS),
    MAP_WARP(FUCHIN_TEMPLE_GROUNDS_TO_FUCHIN_FALLS_CAVE_ENTRANCE,
             FuchinFallsCaveEntrance,
             FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_TEMPLE_GROUNDS),
    MAP_WARPS(FuchinTempleInteriors),
    MAP_WARP(FUCHIN_TEMPLE_INTERIORS_TO_FUCHIN_TEMPLE_GROUNDS,
             FuchinTempleGrounds,
             FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_TEMPLE_INTERIORS),
    MAP_WARP(FUCHIN_TEMPLE_INTERIORS_TO_XIAN, Xian,
             XIAN_FROM_FUCHIN_TEMPLE_INTERIORS),
    MAP_WARP(FUCHIN_TEMPLE_INTERIORS_TO_LAMA_TEMPLE, LamaTemple,
             LAMA_TEMPLE_FROM_FUCHIN_TEMPLE_INTERIORS),
    MAP_WARP(FUCHIN_TEMPLE_INTERIORS_TO_HOUSE, XianInteriors,
             XIAN_INTERIORS_FROM_FUCHIN_TEMPLE_INTERIORS),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gFuchinTempleGroundsObjectEventTemplates[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(296), 0, PIXELS(440), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MONK_IN_WHITE_AND_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(272), 0, PIXELS(424), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(392), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(312), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gFuchinTempleObjectEventTemplates[] = {
    // Nyunpa
    { SPRITE_NYUNPA, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(472), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    // Master Feh's students
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(264), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUTH_WITH_HEADBAND, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(200), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUTH_WITH_TIED_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUTH_WITH_TIED_HAIR, 0x0, (s32)gFuchinTemple_JumpingStudentScript,
        PIXELS(120), 0, PIXELS(248), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUTH_WITH_TIED_HAIR, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(176), 0, PIXELS(168), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(264), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUTH_WITH_HEADBAND, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(152), FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUTH_WITH_HEADBAND, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(176), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    // Feizhi and Master Feh, out of sight until they come out
    { SPRITE_FEIZHI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_FEH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    // The training tree, and where Force strikes it
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(168), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, (s32)gFuchinTemple_HiddenSpriteScript,
        PIXELS(232), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gFuchinTempleObjectEventTemplatesFromLamaTemple[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    // Hama
    { SPRITE_HAMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(472), 0, PIXELS(472), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BALD_BOY_IN_ORANGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(440), 0, PIXELS(536), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_FEIZHI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gFuchinTempleGroundsEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, FUCHIN_TEMPLE_GROUNDS_TO_WORLD_MAP },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterTemple) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterWaterfallGrotto) },
    { MAP_EVENT_COORD, 5, FLAG_FUCHIN_FALLS_MONK_ASKED_ABOUT_TEST, EVENT_SCRIPT(EventScript_MonkAsksWhetherTestIsDone) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_FROM_KOLIMA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleTheFuchinTempleIsAPleasant) },
    { MAP_EVENT_TALK, LOCALID_FALLS_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleOhIsaacWhatTookYouSo) },
    { MAP_EVENT_TALK, LOCALID_BOY_MONK_READING_OMENS, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleAhhhMasterNyunpaHeHasPassed) },
    { MAP_EVENT_TALK, LOCALID_WORRIED_BOY_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleMasterNyunpaHasWastedAwayAt) },
    { MAP_EVENT_TALK, LOCALID_ADMIRING_BOY_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), EVENT_SCRIPT(EventScript_BoyMonkSpeaksOfNyunpa) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_FROM_KOLIMA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_YoungManFromKolimaAsksAboutBridge) },
    // Unused: a copy of the row above, which always answers first.
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_FROM_KOLIMA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_YoungManFromKolimaAsksAboutBridge) },
    { MAP_EVENT_TALK, LOCALID_FALLS_MONK, FLAG_NYUNPA_TEST_ACCEPTED, MSGNO(MsgFuchinTempleOmOmBlueLightsInThe) },
    { MAP_EVENT_TALK, LOCALID_FALLS_MONK, FLAG_FUCHIN_FALLS_GROTTO_OPENED, EVENT_SCRIPT(EventScript_MonkLetsPartyIntoGrotto) },
    { MAP_EVENT_TALK, LOCALID_FALLS_MONK, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND, MSGNO(MsgFuchinTempleTheTestOfFuchinFallsIs) },
    { MAP_EVENT_TALK, LOCALID_FALLS_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterNyunpaIsWaitingForYou) },
    { MAP_EVENT_TALK, LOCALID_BOY_MONK_READING_OMENS, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleGemsFromTheSkyBlueLights) },
    { MAP_EVENT_TALK, LOCALID_WORRIED_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleNyunpaHasNotEatenInDays) },
    { MAP_EVENT_TALK, LOCALID_ADMIRING_BOY_MONK, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_BoyMonkSpeaksOfNyunpa) },
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToHealer) },
    { BG_EVENT(BG_EVENT_SILENT), 110, FLAG_MAPSEC_TEMP_50, FIND_PSYNERGY_STONE },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_FROM_KOLIMA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleGemsFallingFromTheSkyMountains) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FALLS_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleIKnewMasterNyunpaWasFasting) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_MONK_READING_OMENS, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleMasterNyunpaReachedEnlightenmentDuringHis) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WORRIED_BOY_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleMasterNyunpaIsNothingButSkin) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ADMIRING_BOY_MONK, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTempleHeCantLeaveUsYetHe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_FROM_KOLIMA, FLAG_FUCHIN_TEMPLE_VISITOR_HEARD_BRIDGE_OPEN, MSGNO(MsgFuchinTempleIBeginningToThinkIllBe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_FROM_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIDontFeelSafeGoingHome) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FALLS_MONK, FLAG_NYUNPA_TEST_ACCEPTED, MSGNO(MsgFuchinTempleManyStarsFellFromTheSky) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FALLS_MONK, FLAG_FUCHIN_FALLS_GROTTO_OPENED, MSGNO(MsgFuchinTempleCanSuchYoungWarriorsEndureThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FALLS_MONK, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND, MSGNO(MsgFuchinTempleCanSuchYoungWarriorsEndureThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FALLS_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleTheWarriorsHaveMasteredTheSecrets) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_MONK_READING_OMENS, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleThereIsNoPointInTraining) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WORRIED_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleICantImagineFastingItGets) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ADMIRING_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIWillTrainToBecomeA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleSinceImHereIdLikeTo) },
    { MAP_EVENTS_END },
};

struct MapEvent gFuchinTempleEvents[] = {
    { COORD_EVENT_FACING(FACING_SOUTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveTemple) },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, FUCHIN_TEMPLE_INTERIORS_TO_XIAN },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenLockedDoor) },
    { MAP_EVENT_TALK, LOCALID_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), MSGNO(MsgFuchinTemple) },
    { MAP_EVENT_TALK, LOCALID_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TAUGHT_FORCE), MSGNO(MsgFuchinTempleUseThisEnergyToFindThe) },
    { MAP_EVENT_TALK, LOCALID_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TEST_ACCEPTED), MSGNO(MsgFuchinTempleSpeakToTheMonkStandingBeneath) },
    { MAP_EVENT_TALK, LOCALID_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TEST_DECLINED), EVENT_SCRIPT(EventScript_NyunpaOffersTest) },
    { MAP_EVENT_TALK, LOCALID_NYUNPA, CONDITION_ALWAYS, MSGNO(MsgXianNyunpaMeditates) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_ENLIGHTENED), EVENT_SCRIPT(EventScript_ReadNyunpasLastWish) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TAUGHT_FORCE), MSGNO(MsgFuchinTempleNowThatIHavePassedMy) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), PSYNERGY_SCRIPT_PLAYS_EFFECT | LOCALID_NYUNPA, IF_FLAG_SET(FLAG_NYUNPA_TEST_ACCEPTED), EVENT_SCRIPT(EventScript_NyunpaFeelsMindRead) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), PSYNERGY_SCRIPT_PLAYS_EFFECT | LOCALID_NYUNPA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_NyunpaOffersTest) },
    { MAP_EVENT_TALK, LOCALID_WELCOMING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleThisIsMasterFehsKungFu) },
    { MAP_EVENT_TALK, LOCALID_KICKING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleBouldersOvercameHsuKungFuShould) },
    { MAP_EVENT_TALK, LOCALID_PUNCHING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), EVENT_SCRIPT(EventScript_StudentAsksAboutAltin) },
    { MAP_EVENT_TALK, LOCALID_JUMPING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), EVENT_SCRIPT(EventScript_TalkToJumpingStudent) },
    { MAP_EVENT_TALK, LOCALID_WATCHING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleFeizhiPredictedHsusDangerHerProphecy) },
    { MAP_EVENT_TALK, LOCALID_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleHsuIsRestingFeizhiIsWatching) },
    { MAP_EVENT_TALK, LOCALID_FEH, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTemplePracticeEveryoneChiHasItsLimits) },
    { MAP_EVENT_TALK, LOCALID_WELCOMING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleWarriorYouAreGreatYouHave) },
    { MAP_EVENT_TALK, LOCALID_KICKING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleEveryoneWantsToLearnTheChi) },
    { MAP_EVENT_TALK, LOCALID_PUNCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleConcentrateEnergyIntoTheHandsAnd) },
    { MAP_EVENT_TALK, LOCALID_JUMPING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), EVENT_SCRIPT(EventScript_TalkToJumpingStudent) },
    { MAP_EVENT_TALK, LOCALID_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleIDidNotThinkMuchOf) },
    { MAP_EVENT_TALK, LOCALID_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleWhereDidFeizhiGoIAm) },
    { MAP_EVENT_TALK, LOCALID_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleMasterHsuIsLateICan) },
    { MAP_EVENT_TALK, LOCALID_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleIWillLearnChiIWill) },
    { MAP_EVENT_TALK, LOCALID_FEH, IF_FLAG_SET(FLAG_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorUseYourChiFromThe) },
    { MAP_EVENT_TALK, LOCALID_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorPleaseUseYourChiFrom) },
    { MAP_EVENT_TALK, LOCALID_FEH, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorLetMeSeeYourChi) },
    { MAP_EVENT_TALK, LOCALID_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleFeizhiIsGoneItIsDangerous) },
    { MAP_EVENT_TALK, LOCALID_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleFeizhiIsStrongerThanSheLooks) },
    { MAP_EVENT_TALK, LOCALID_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorShowUsYourChiLike) },
    { MAP_EVENT_TALK, LOCALID_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorThatWasGreatButMaster) },
    { MAP_EVENT_TALK, LOCALID_FEH, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), EVENT_SCRIPT(EventScript_FehAsksToSeeChi) },
    { MAP_EVENT_TALK, LOCALID_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleFeizhiIsGoneItIsDangerousOutside) },
    { MAP_EVENT_TALK, LOCALID_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleFeizhiIsStrongerThanSheLooksShe) },
    { MAP_EVENT_TALK, LOCALID_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleIKnowWhyYouCanUse) },
    { MAP_EVENT_TALK, LOCALID_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleWarriorChiIsAGreatPower) },
    { MAP_EVENT_TALK, LOCALID_WELCOMING_STUDENT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StudentWelcomesToSchool) },
    { MAP_EVENT_TALK, LOCALID_KICKING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleStrongKicksAreVeryImportantKick) },
    { MAP_EVENT_TALK, LOCALID_PUNCHING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIWantToLearnSoI) },
    { MAP_EVENT_TALK, LOCALID_JUMPING_STUDENT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToJumpingStudent) },
    { MAP_EVENT_TALK, LOCALID_PROUD_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleEveryoneInXianCanDoMartial) },
    { MAP_EVENT_TALK, LOCALID_WATCHING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleJustListeningDoesNotMakeOne) },
    { MAP_EVENT_TALK, LOCALID_TREE_STUDENT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StudentAsksAboutFeh) },
    { MAP_EVENT_TALK, LOCALID_MESSENGER_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterFehIsVeryBusyHe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WELCOMING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTemplePeaceHasReturnedToXianBut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_KICKING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleNoMatterHowMuchYouPractice) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_PUNCHING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleTheMonstersAreVeryLargeStriking) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_JUMPING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleWhenABoulderFallsJumpRepeat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WATCHING_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleWhereDidFeizhiGainThePower) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MSGNO(MsgFuchinTempleMasterFehIsVeryWorriedAbout) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FEH, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleCanSuchAYoungWarriorReally) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WELCOMING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleYourChiIsFuchinTemplesHow) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_KICKING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleIDontCareWhichChiIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_PUNCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleMasterHamasChiCanShatterBoulders) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_JUMPING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleFollowSilkRoadStraightPassThrough) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleWarriorYouUseWeaponsAndChi) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleFeizhiIsNotInTheFeh) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleFeizhiWorriesOnlyAboutMasterHsu) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_SAW_KI), MSGNO(MsgFuchinTempleLearningChiIsVeryHardMaster) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FEH, IF_FLAG_SET(FLAG_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleWarriorTheRulesRequireYouTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_SCHOOL_SAW_TREE_FALL), MSGNO(MsgFuchinTempleSoCloseWarriorUseChiFrom) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FEH, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleWarriorShowMeYourChiI) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleFeizhiIKnowIKnowHow) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleMyKickAndPunchAreVery) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleShowUsYourChiNowDo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_WAITS_TO_SEE_CHI), MSGNO(MsgFuchinTempleIAmShockedThatThisWarrior) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FEH, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleIWantToSeeTheChi) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_PROUD_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleFeizhiIKnowHowYouFeel) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WATCHING_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleMyKickAndPunchAreVeryLoud) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TREE_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleWarriorShowMeYourChiNow) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MESSENGER_STUDENT, IF_FLAG_SET(FLAG_FEH_CHI_DEMONSTRATION_DECLINED), MSGNO(MsgFuchinTempleWarriorDoNotHideYourChi) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WELCOMING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleWarriorsFightWithWeaponsKungFu) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_KICKING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTemplePracticingKicksHurtsMyFeetVery) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_PUNCHING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleThatPunchWasGoodYouSurprise) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_JUMPING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIMoveTooQuicklyYouCant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_PROUD_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleWarriorsCameToLearnTheFeh) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WATCHING_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleGreatKickKickFromTheHip) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TREE_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleYouAreTheWarriorsWhoCame) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MESSENGER_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterFehIsTalkingWithFeizhi) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushTreeByHand) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushTreeByHand) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushTreeByHand) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushTreeByHand) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_TRAINING_TREE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MoveTree) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_TRAINING_TREE_TARGET, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ForceTree) },
    { BG_EVENT(BG_EVENT_BARREL), 100, FLAG_FUCHIN_TEMPLE_ANTIDOTE_BARREL, FIND_ITEM(ITEM_ANTIDOTE) },
    { BG_EVENT(BG_EVENT_JAR), 101, FLAG_FUCHIN_TEMPLE_UNICORN_RING_JAR, FIND_ITEM(ITEM_UNICORN_RING) },
    { MAP_EVENTS_END },
};

struct MapEvent gFuchinTempleEventsFromLamaTemple[] = {
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, FUCHIN_TEMPLE_INTERIORS_TO_LAMA_TEMPLE },
    { MAP_EVENT_TALK, LOCALID_HAMAS_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleMasterHamaIsMeditatingAskingFor) },
    { MAP_EVENT_TALK, LOCALID_HAMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleTheFutureHoldsManyHardshipsBut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HAMAS_BOY_MONK, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleIDoNotThinkMasterHama) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HAMA, CONDITION_ALWAYS, MSGNO(MsgFuchinTempleICannotSeeYourFutureClearly) },
    { BG_EVENT(BG_EVENT_JAR), 102, FLAG_HAMAS_ROOM_COINS_JAR, FIND_COINS(6) },
    { MAP_EVENTS_END },
};
// clang-format on

s32 gFuchinTemple_ShotScript[] = {
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(44),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};
