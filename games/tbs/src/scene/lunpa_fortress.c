/// @file
/// Lunpa Fortress, Dodonpa's fortress above Lunpa, where he holds Hammet for
/// ransom, and the cave below it. Four scenes share this overlay and the area
/// name Lunpa Fortress:
///
/// - LunpaFortressGroundFloor (SceneId_LunpaFortressGroundFloor): inside the
/// door
///   from the Lunpa Fortress Gates. An entrance guard, two sentries, the upper
///   hall's guard and a patrolling guard keep watch; Dodonpa's men and the
///   people of the fortress talk, and chests and a barrel hold an Elixir, a
///   Lucky Medal, a Mythril Circlet, coins and a Vial.
/// - LunpaFortressUpperFloor (SceneId_LunpaFortressUpperFloor): where Donpa
///   sleeps. A patrolling pair and a hall patrol keep watch, three guards and
///   then three more together fight the party as Brigands, the Cell Key hangs
///   here, and Dodonpa's family and the women he brought here talk. After the
///   rescue Donpa gives the party Tonic, a Mercury Djinni.
/// - LunpaFortressCave (SceneId_LunpaFortressCave): the cave whose switches and
///   passage ivy hides, with a stack of crates to push into its hole.
/// - LunpaFortressToadonpa (SceneId_LunpaFortressToadonpa): Hammet's cell,
/// where
///   Dodonpa frees Toadonpa on the party (LunpaFortress_FindHammet) and Donpa
///   judges him after the battle (LunpaFortress_DonpaJudgesDodonpa), which sets
///   FLAG_DODONPA_LOCKED_UP and changes all of Lunpa.
///
/// The getters choose by gMainRoutineWork.sceneId; LunpaFortressCave and
/// LunpaFortressToadonpa share one exit table. Every return through the Lunpa
/// Fortress Gates (lunpa.c) clears the fortress's puzzle and beaten-guard
/// flags, so the fortress starts over.
///
/// The guards are the fortress's mechanism. A guard on screen sees the leader
/// along his row or column, within six cells (LunpaFortress_CanGuardSeeLeader),
/// unless Cloak is on, and each guard has a watcher, a task that the
/// entrance the party arrives by starts
/// (LunpaFortress_InitializeGroundFloor, LunpaFortress_InitializeUpperFloor). A
/// watcher that sees the leader raises one of the RAISED_*_SPOTTED triggers,
/// whose event has the guard challenge the party and throw it out to the gates
/// (LunpaFortress_GuardThrowsPartyOut, EXIT_THROWN_OUT). A cloaked leader
/// within four cells of a patrolling guard makes the watcher request
/// CLOAK_NEAR_GUARD. The patrolling guards walk actor scripts
/// (gFirstPatrolRoute and the others), stop while a menu, Psynergy or an item
/// is in use, and drag MAPGRID_LAYER_OVERHEAD along with them, shaking it by a
/// pixel every frame: in the dark corridors that layer holds the circles of
/// light the patrols carry, which flicker as they shake
/// (https://alchemy.pascalpixel.com/places/lunpa-fortress-upper-floor/map-1.png
/// shows the pair's two circles). Each has stretches of its rounds where it
/// cannot see the leader. On the upper floor the pair walk east and west
/// between 88 and 192 pixels across, one 192 and one 288 pixels down; each sees
/// a leader in his own column up to six rows away, or within a row of his up to
/// six columns away, whatever stands between them, but never one in rows 5 to 7
/// from column 10 west, at the passage's top, or in columns 8 and 9 below row
/// 22 (LunpaFortress_IsLeaderInPatrolSafeZone). The guards who fight instead
/// pick one of four challenges, by gLunpaFortressRandomPick, drawn on every
/// arrival.
///
/// The Cell Key opens the cell doors of all four scenes, facing each door with
/// it in the party, and each opened door is remembered in an area temp flag
/// from FLAG_AREA_TEMP_50 (TOADONPA_DOOR_FLAG and the others). In
/// LunpaFortressCave, Whirlwind blows away the ivy that hides two switches and
/// a passage; the switches show in the wall only while Reveal lasts, and
/// searching one opens its passage.
///
/// Scene temp flags:
/// - FLAG_LUNPA_FORTRESS_PARTY_SPOTTED: a guard has spotted the party;
///   FLAG_SCENE_TEMP_15 is set beside it and never read.
/// - FLAG_LUNPA_FORTRESS_FIRST_SWITCH_UNDER_IVY and
///   FLAG_LUNPA_FORTRESS_SECOND_SWITCH_UNDER_IVY: LunpaFortressCave's first and
///   second switches are still hidden by their ivy.
/// - FLAG_LUNPA_FORTRESS_ENTRANCE_GUARD_CAUGHT_PARTY and
///   FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY: the entrance guard or the
///   sentries have thrown the party out.
/// - FLAG_LUNPA_FORTRESS_OLD_MAN_PROMISED_SILENCE and
///   FLAG_LUNPA_FORTRESS_OLD_MAN_STARTLED: the startled old man has promised to
///   keep quiet, and has been startled.
/// - FLAG_LUNPA_FORTRESS_FINDING_HAMMET: LunpaFortress_FindHammet is under way.
///
/// Map-section temp flags: FLAG_LUNPA_FORTRESS_DODONPA_JUDGED once Donpa has
/// judged Dodonpa, FLAG_LUNPA_FORTRESS_ATTENDANT_TOLD_OF_DONPA once his
/// attendant has told of him, FLAG_LUNPA_FORTRESS_REWARD_SHOWN once she has
/// shown his reward, and FLAG_AREA_TEMP_50 to FLAG_AREA_TEMP_5C for the
/// opened cell doors.
///
/// Unused: four item pickups that set out an Arctic Blade, a Masamune, a Silver
/// Blade and a Shamshir on LunpaFortressGroundFloor's first four rows, and
/// copies of West Lunpa Cave's exits and actor placements, which these
/// scenes never read.

#include "main_routine_work.h"
#include "event.h"
#include "module/import.h"
#include "module/scene/lunpa_fortress.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "module/field/field.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "heap_slots.h"
#include "edition.h"
#include "lunpa_fortress_staging.h"
#include "scene_ids.h"
#include "tbs_edition.h"
#include "map_grid_scroll.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "scene_tables.h"
#include "item_ids.h"
#include "actor_script.h"
#include "flags.h"
#include "djinn_ids.h"
#include "sounds.h"
#include "sound_driver_symbols.h"
#include "fixed_math.h"
#include "io_reg.h"

/// LunpaFortressGroundFloor's people, numbered in the order of
/// gLunpaFortressGroundFloorActorPlacements from ACTOR_FIRST_PLACED,
/// past its first four rows (in a chest's sprite, at the map's origin; see
/// ACTOR_FIRST_ITEM_DISPLAY). Dodonpa's men, here and upstairs, wear the
/// sprite of a man with a mop of orange hair (SPRITE_MAN_WITH_ORANGE_HAIR).
enum {
  /// ★ Named from what the player sees: one of Dodonpa's men standing watch
  /// by the way in from the Lunpa Fortress Gates, who throws out a party he
  /// sees (its render)
  ACTOR_GROUND_FLOOR_ENTRANCE_GUARD = ACTOR_FIRST_PLACED + 4,
  /// ★ Named from what the player sees: the first of the two of Dodonpa's men
  /// who stand watch together, the one who speaks when they throw the party
  /// out (its render)
  ACTOR_GROUND_FLOOR_FIRST_SENTRY,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), wondering where
  /// Dodonpa has gone.
  /// ★ Named from the game: "Where could Dodonpa be?" (MsgLunpaWhereIsDodonpa)
  ACTOR_GROUND_FLOOR_WOMAN_SEEKING_DODONPA,
  /// One of Dodonpa's men (the guards' sprite), feasting on Kalay's ransom.
  /// ★ Named from the game: "Thanks to Hammet we can enjoy food and drink all
  /// day long!" (MsgLunpaHammetBuysDrink)
  ACTOR_GROUND_FLOOR_FEASTING_MAN,
  /// One of Dodonpa's men, drinking and boasting of Hammet.
  /// ★ Named from the game: "He's a money tree for us, hic!"
  /// (MsgLunpaHammetMoneyTree)
  ACTOR_GROUND_FLOOR_BOASTING_MAN,
  /// ★ Named from what the player sees: one of Dodonpa's men standing watch
  /// deeper in the floor, who throws out a party he sees (its render)
  ACTOR_GROUND_FLOOR_UPPER_HALL_GUARD,
  /// ★ Named from what the player sees: one of Dodonpa's men walking his
  /// rounds (gGroundFloorPatrolRoute), who throws out a party he sees (its
  /// render)
  ACTOR_GROUND_FLOOR_PATROL_GUARD,
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), talking of
  /// Donpa's poor health.
  /// ★ Named from the game: "Donpa is supposed to be old. I wonder what he's
  /// fighting with" (MsgLunpaDonpaResting)
  ACTOR_GROUND_FLOOR_MAN_WONDERING_ABOUT_DONPA,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), startled when the
  /// party first speaks to him (LunpaFortress_TalkToStartledOldMan).
  /// ★ Named from the game: "Who goes there!" (MsgLunpaWhWhWho)
  ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN,
  /// The other man standing watch with ACTOR_GROUND_FLOOR_FIRST_SENTRY.
  /// ★ Named from the game: "I was told to stand guard"
  /// (MsgLunpaToldStandGuard)
  ACTOR_GROUND_FLOOR_SECOND_SENTRY
};

/// LunpaFortressUpperFloor's people and the Cell Key, numbered in the order of
/// gLunpaFortressUpperFloorActorPlacements from ACTOR_FIRST_PLACED.
enum {
  /// ★ Named from what the player sees: a golden key (SPRITE_KEY) in the
  /// south of the floor, which the party takes as the Cell Key (its render)
  ACTOR_UPPER_FLOOR_CELL_KEY = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: the first of the pair of Dodonpa's
  /// men who walk east and west together in the west passage, carrying a
  /// circle of light (gFirstPatrolRoute) (its render)
  ACTOR_UPPER_FLOOR_FIRST_PATROL,
  /// ★ Named from what the player sees: the second of the pair, a row of
  /// cells below the first (gSecondPatrolRoute) (its render)
  ACTOR_UPPER_FLOOR_SECOND_PATROL,
  /// ★ Named from what the player sees: one of Dodonpa's men standing in the
  /// hall, who challenges the party to a battle as it comes up to him
  /// (LunpaFortress_FightHallGuard) (its render)
  ACTOR_UPPER_FLOOR_HALL_GUARD,
  /// The three guards who fight together, by where they stand once beaten
  /// (LunpaFortress_PlaceThreeBeatenGuards): west, middle and east in a row.
  /// ★ Named from the game: "You guys are tougher than we thought"
  /// (MsgLunpaGuysTougherThought)
  ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST,
  /// ★ Named from the game: "All right, all right. We give up!"
  /// (MsgLunpaRightRightGive)
  ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE,
  /// ★ Named from the game: "Don't hurt me again, you thieving scoundrel!"
  /// (MsgLunpaOwwwDontHurt)
  ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST,
  /// ★ Named from what the player sees: one of Dodonpa's men standing in the
  /// west, who challenges the party to a battle as it comes up to him
  /// (LunpaFortress_FightWestGuard) (its render)
  ACTOR_UPPER_FLOOR_WEST_GUARD,
  /// ★ Named from what the player sees: one of Dodonpa's men standing in the
  /// east, who challenges the party to a battle as it comes up to him
  /// (LunpaFortress_FightEastGuard) (its render)
  ACTOR_UPPER_FLOOR_EAST_GUARD,
  /// ★ Named from what the player sees: one of Dodonpa's men walking his
  /// rounds of the east hall, carrying a circle of light (gHallPatrolRoute)
  /// (its render)
  ACTOR_UPPER_FLOOR_HALL_PATROL,
  /// Dodonpa's wife, a woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS),
  /// the "Mama" of his children, looking for him.
  /// ★ Named from the game: "Dodonpa's wife was looking for him"
  /// (MsgLunpaWifeSeeksDodonpa)
  ACTOR_UPPER_FLOOR_DODONPAS_WIFE,
  /// Donpappa, a small boy (SPRITE_BOY_WITH_FAIR_HAIR), Dodonpa's son, as he
  /// names himself to Mind Read.
  /// ★ Named from the game: "When my Papa's gone, I, Donpappa, shall protect
  /// Lunpa myself." (MsgLunpaDonpappaProtectsFortress)
  ACTOR_UPPER_FLOOR_DONPAPPA,
  /// Dodonpa's daughter, a little girl (SPRITE_GIRL_WITH_PIGTAILS).
  /// ★ Named from the game: "I'm not going to marry anyone like my Papa when I
  /// grow up!" (MsgLunpaDaughterWillNotMarryPapa)
  ACTOR_UPPER_FLOOR_DODONPAS_DAUGHTER,
  /// An old woman (SPRITE_OLD_WOMAN) who pities Donpappa.
  /// ★ Named from the game: "Dodonpa's child is so well behaved, I can't
  /// believe he's his father's son!" (MsgLunpaChildTooResponsible)
  ACTOR_UPPER_FLOOR_OLD_WOMAN_PRAISING_DONPAPPA,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who fears for Donpa.
  /// ★ Named from the game: "Poor, poor Donpa." (MsgLunpaDonpaExhausted)
  ACTOR_UPPER_FLOOR_WOMAN_FEARING_FOR_DONPA,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), one of the women
  /// Dodonpa has brought to the fortress.
  /// ★ Named from the game: "Nearly all of these women were brought here
  /// against their will, like me." (MsgLunpaWomenWantHome)
  ACTOR_UPPER_FLOOR_CAPTIVE_WOMAN,
  /// Donpa, asleep (SPRITE_DONPA).
  /// ★ Named from the game: "Shhh! Please don't wake Donpa!"
  /// (MsgLunpaShhhPleaseDont)
  ACTOR_UPPER_FLOOR_DONPA,
  /// Donpa's attendant, a woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from what the player sees: a woman standing by the sleeping
  /// Donpa, who hushes the party (LunpaFortress_TalkToAttendant) (its render)
  ACTOR_UPPER_FLOOR_ATTENDANT,
  /// Tonic, a Mercury Djinni, Donpa's reward.
  /// ★ Named from the game: "Tonic" (MsgDjinnName)
  ACTOR_UPPER_FLOOR_TONIC
};

/// LunpaFortressCave's ivy and crates, numbered in the order of
/// gLunpaFortressCaveActorPlacements from ACTOR_FIRST_PLACED.
enum {
  /// ★ Named from what the player sees: ivy (SPRITE_IVY) on the wall that
  /// hides the first switch until Whirlwind blows it away (its render)
  ACTOR_CAVE_FIRST_SWITCH_IVY = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: ivy (SPRITE_IVY) that hides a passage
  /// until Whirlwind blows it away (its render)
  ACTOR_CAVE_PASSAGE_IVY,
  /// ★ Named from what the player sees: ivy (SPRITE_IVY) on the wall that
  /// hides the second switch until Whirlwind blows it away (its render)
  ACTOR_CAVE_SECOND_SWITCH_IVY,
  /// An invisible solid actor (SPRITE_BLANK, ACTOR_SOLID) at (504,
  /// 168) that makes the jar the layout draws there, in the cell behind the
  /// cave's eastern door, stop the leader: the layout's cell behavior lets
  /// him walk onto the jar, and with this actor he stops 14 pixels short
  /// of it. A blank sprite draws only its shadow, which
  /// LunpaFortress_InitializeCave turns off.
  ACTOR_INVISIBLE_CAVE_JAR,
  /// A stack of wooden crates (the sprite of the crates at the Bilibin
  /// Barricade and on Tolbi Docks), pushed or moved with Move onto its hole.
  /// ★ Named from what the player sees: a stack of two wooden crates
  /// (SPRITE_CRATES) in the north of the cave (its render)
  ACTOR_CAVE_CRATES
};

/// LunpaFortressToadonpa's people, numbered in the order of
/// gLunpaFortressToadonpaActorPlacements from ACTOR_FIRST_PLACED; its
/// rows for Ivan, Mia and Garet keep the party's own slots.
enum {
  /// Three invisible solid actors (SPRITE_BLANK, ACTOR_SOLID) at
  /// (56, 72), (136, 72) and (216, 72), in line with the cell doors
  /// gLunpaFortressToadonpaDoorCells lists third, second and first, that make
  /// the jar, chest and jar the layout draws in those cells stop the leader.
  /// The layout's cell behaviors let him walk onto the two jars; the
  /// chest's own cells already stop him, so the middle actor changes
  /// nothing. Scene_Initialize turns off the shadows their blank sprites would
  /// draw and lays them underfoot.
  ACTOR_INVISIBLE_TOADONPA_WEST_JAR = ACTOR_FIRST_PLACED,
  ACTOR_INVISIBLE_TOADONPA_CHEST,
  ACTOR_INVISIBLE_TOADONPA_EAST_JAR,
  /// The monster Dodonpa frees on the party (LunpaFortress_FindHammet).
  /// ★ Named from the game: "Toadonpa" (MsgEnemyName)
  ACTOR_TOADONPA_BOSS,
  /// ★ Named from the game: "Sorry to interrupt your teary little reunion,
  /// Hammet!" (MsgLunpaTimeEat)
  ACTOR_TOADONPA_HAMMET,
  /// Dodonpa (SPRITE_DODONPA).
  /// ★ Named from the game: "Dodonpa pulled a lever."
  /// (MsgLunpaDodonpaPulledLever)
  ACTOR_TOADONPA_DODONPA,
  /// ★ Named from the game: "Thank you, Donpa, but you do not need to
  /// apologize." (MsgLunpaDad)
  ACTOR_TOADONPA_DONPA,
  /// A blank sprite (SPRITE_BLANK_LARGE) laid underfoot with Toadonpa and
  /// moving with it, drawn 4/5 as wide.
  ACTOR_INVISIBLE_TOADONPA_BOSS_PART,
  /// One of Dodonpa's men, a man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), placed by Dodonpa's cell after
  /// Donpa's judgment. Read with Mind Read: "Actually, Dodonpa's in here, but
  /// only a few people know that."
  /// ★ Named from the game: "The merchant from Kalay is in here somewhere."
  /// (MsgLunpaHammetStillInside)
  ACTOR_TOADONPA_CELL_GUARD,
  /// An invisible actor that stands before Dodonpa's cell after Donpa's
  /// judgment; talking to it or reading its mind reaches Dodonpa.
  ACTOR_INVISIBLE_TOADONPA_DODONPA
};

/// The cell behaviors the fortress's triggers answer: the cell doors
/// count from CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR in each scene.
enum LunpaFortressCellBehavior {
  /// ★ Named from the game: "The door is tightly locked."
  /// (MsgFieldDoorTightlyLocked)
  CELL_BEHAVIOR_LUNPA_FORTRESS_LOCKED_DOOR = 99,
  CELL_BEHAVIOR_LUNPA_FORTRESS_SEALED_DOOR = 98,
  CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR = 40
};

/// The events a watching guard raises on spotting the party.
enum LunpaFortressRaisedTrigger {
  RAISED_PATROL_PAIR_SPOTTED = 91,
  RAISED_HALL_PATROL_SPOTTED,
  RAISED_GROUND_FLOOR_PATROL_SPOTTED,
  RAISED_UPPER_HALL_SPOTTED,
  RAISED_ENTRANCE_GUARD_SPOTTED,
  RAISED_SENTRIES_SPOTTED
};

/// Reveal's phases: the hidden things show, and hide again.

/// Where a party caught by a guard is sent: the Lunpa Fortress Gates.
enum { EXIT_THROWN_OUT = 60 };

/// What a guard close to the cloaked leader requests of Cloak.
enum { CLOAK_NEAR_GUARD = 0x2000 | ABILITY_CLOAK };

/// The first of the flags that remember which cell doors the Cell Key opened,
/// per scene: one for each door trigger from
/// CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR.
enum {
  TOADONPA_DOOR_FLAG = FLAG_AREA_TEMP_50,
  CAVE_DOOR_FLAG = FLAG_AREA_TEMP_55,
  UPPER_FLOOR_DOOR_FLAG = FLAG_AREA_TEMP_58,
  GROUND_FLOOR_DOOR_FLAG = FLAG_AREA_TEMP_5A
};

/// The map grid layers the dark passages show
/// (MapGrid_SetShownLayers): BG1 to BG3, BG1 being
/// MAPGRID_LAYER_OVERHEAD and its circles of light.
#define LUNPA_FORTRESS_PASSAGE_LAYERS                                          \
  (DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)
/// The map grid layers the lit rooms show: BG2 and BG3, without the overhead
/// layer.
#define LUNPA_FORTRESS_ROOM_LAYERS (DISPCNT_BG2_ON | DISPCNT_BG3_ON)

/// The paired patrol's layer origin across: 136 pixels, in 16.16 fixed point.
// The native paired patrol computes this layer offset in floating point.
#define PAIRED_PATROL_LAYER_ORIGIN_X (136.0 * 65536)

static s32 LunpaFortress_TryCollectDisplayedItem(s32 actorId, s32 itemId);
static void LunpaFortress_CheckSecondEntranceSentry(void);
static s32 LunpaFortress_CanGuardSeeLeader(s32 actorId);
static s32 LunpaFortress_IsLeaderNearGuard(s32 actorId);
static s32 LunpaFortress_IsGuardNearViewCenter(s32 actorId);
static void LunpaFortress_GuardThrowsPartyOut(s32 actorId);
static void LunpaFortress_ActOutIntruderDialogue(s32 actorId);
static void LunpaFortress_ActOutWhosThereDialogue(s32 actorId);
static void LunpaFortress_ActOutGuardShiftDialogue(s32 actorId);
static void LunpaFortress_ActOutScoundrelDialogue(s32 actorId);
static void LunpaFortress_InitializeGroundFloor(void);
static void LunpaFortress_InitializeUpperFloor(void);
static void LunpaFortress_InitializeCave(void);
static void LunpaFortress_RestoreGroundFloorDoors(void);
static void LunpaFortress_RestoreUpperFloorDoors(void);
static void LunpaFortress_RestoreCaveDoors(void);
static void LunpaFortress_RestoreToadonpaDoors(void);

#include "shared/staged_actor.h"

/// Donpa has judged Dodonpa, so Hammet's cell no longer starts the trap.
/// ★ Named from the game: "Huh? Donpa, didn't you just say you're going to lock
/// up Dodonpa?" (MsgLunpaDad)
#define FLAG_LUNPA_FORTRESS_DODONPA_JUDGED FLAG_AREA_TEMP_01
/// Donpa's attendant has told the party of Donpa and his spoiled son.
/// ★ Named from the game: "Donpa knows that he coddled Dodonpa too much as a
/// child." (MsgLunpaDonpaKnowsCoddled)
#define FLAG_LUNPA_FORTRESS_ATTENDANT_TOLD_OF_DONPA FLAG_AREA_TEMP_13
/// Donpa's attendant has shown the party his reward, Tonic, and stands beside
/// it.
/// ★ Named from the game: "I have a little something for you, for your teaching
/// Dodonpa his lesson." (MsgLunpaThankHelpDodonpa)
#define FLAG_LUNPA_FORTRESS_REWARD_SHOWN FLAG_AREA_TEMP_14
/// A guard has spotted the party.
/// ★ Named from the game: "How did you get in here?" (MsgLunpaWho2)
#define FLAG_LUNPA_FORTRESS_PARTY_SPOTTED FLAG_SCENE_TEMP_14
/// LunpaFortressCave's first switch is still hidden by its ivy.
/// ★ Named from what the player sees: a leafy green plant against the cave
/// wall, which Whirlwind blows away (its render)
#define FLAG_LUNPA_FORTRESS_FIRST_SWITCH_UNDER_IVY FLAG_SCENE_TEMP_17
/// LunpaFortressCave's second switch is still hidden by its ivy.
/// ★ Named from what the player sees: a leafy green plant against the cave
/// wall, which Whirlwind blows away (its render)
#define FLAG_LUNPA_FORTRESS_SECOND_SWITCH_UNDER_IVY FLAG_SCENE_TEMP_18
/// The entrance guard has thrown the party out.
/// ★ Named from the game: "No matter! You're on your way out now!"
/// (MsgLunpaWho2)
#define FLAG_LUNPA_FORTRESS_ENTRANCE_GUARD_CAUGHT_PARTY FLAG_SCENE_TEMP_24
/// The sentries have spotted the party and thrown it out.
/// ★ Named from the game: "No matter! You're on your way out now!"
/// (MsgLunpaWho2)
#define FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY FLAG_SCENE_TEMP_25
/// The startled old man has promised to keep quiet.
/// ★ Named from the game: "I won't tell anyone, but you should leave before the
/// guards find you." (MsgLunpaWontTellAnyone)
#define FLAG_LUNPA_FORTRESS_OLD_MAN_PROMISED_SILENCE FLAG_SCENE_TEMP_26
/// The startled old man has been startled once.
/// ★ Named from the game: "Who goes there!" (MsgLunpaWhWhWho)
#define FLAG_LUNPA_FORTRESS_OLD_MAN_STARTLED FLAG_SCENE_TEMP_27
/// LunpaFortress_FindHammet is under way.
/// ★ Named from the game: "I am a little late, but I have come to save you."
/// (MsgLunpaTimeEat)
#define FLAG_LUNPA_FORTRESS_FINDING_HAMMET FLAG_SCENE_TEMP_70

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.sceneId;

  if (sceneId == (s32)&SceneId_LunpaFortressGroundFloor) {
    return gLunpaFortressGroundFloorEntrances;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressUpperFloor) {
    return gLunpaFortressUpperFloorEntrances;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressCave) {
    return gLunpaFortressCaveEntrances;
  }
  return gLunpaFortressEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) {
  s16 sceneId = gMainRoutineWork.sceneId;

  if (sceneId == (s32)&SceneId_LunpaFortressUpperFloor) {
    return gLunpaFortressUpperFloorExits;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressCave ||
      sceneId == (s32)&SceneId_LunpaFortressToadonpa) {
    return gLunpaFortressCaveAndToadonpaExits;
  }
  return gLunpaFortressExitsOther;
}

/// The guards, the other people and the objects of each scene.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineWork.sceneId;

  // Unused: this overlay never runs for West Lunpa Cave, which has its own.
  if (sceneId == (s32)&SceneId_WestLunpaCave) {
    return gLunpaFortressUnusedWestLunpaCaveActorPlacements;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressCave) {
    return gLunpaFortressCaveActorPlacements;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressUpperFloor) {
    return gLunpaFortressUpperFloorActorPlacements;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressGroundFloor) {
    return gLunpaFortressGroundFloorActorPlacements;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressToadonpa) {
    return gLunpaFortressToadonpaActorPlacements;
  }
  return gLunpaFortressActorPlacementsOther;
}

/// What the party can talk to, search and use Psynergy on, by scene.
const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineWork.sceneId;

  if (sceneId == (s32)&SceneId_LunpaFortressGroundFloor) {
    return gLunpaFortressGroundFloorTriggers;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressUpperFloor) {
    return gLunpaFortressUpperFloorTriggers;
  }
  if (sceneId == (s32)&SceneId_LunpaFortressCave) {
    return gLunpaFortressCaveTriggers;
  }
  return gLunpaFortressTriggersOther;
}

/// LunpaFortressCave: the crates sink into the floor, and the party can walk
/// over them.
static void LunpaFortress_LowerCrates(void) {
  struct Actor *cratesActor;

  MapGrid_CopyCellAttributes(15, 20, 1, 1, 15, 22);
  MapGrid_CopyCellAttributes(17, 23, 1, 3, 15, 23);
  cratesActor = Actor_GetById(ACTOR_CAVE_CRATES);
  if (cratesActor != 0) {
    Actor_SetSpriteFlags(cratesActor, 0);
    cratesActor->motionFlags = 0;
    cratesActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
}

/// LunpaFortressCave, step trigger 70: pushing the crates slides them one cell
/// along.
static void LunpaFortress_PushCrates(void) { PushedActor_AdvancePair(); }

/// LunpaFortressCave, step trigger 50: the crates, pushed onto their hole,
/// slide in and sink.
static void LunpaFortress_CratesSink(void) {
  Actor_SetSpeed(ACTOR_CAVE_CRATES, PIXELS(1), PIXELS(1) / 2);
  Actor_SetDestination(ACTOR_CAVE_CRATES, 248, 376);
  Actor_WaitForMoveById(ACTOR_CAVE_CRATES);
  Event_PlaySound(SE_SPLASH_SMALL);
  Event_Wait(60);
  LunpaFortress_LowerCrates();
  GameFlag_Set(FLAG_LUNPA_FORTRESS_CRATES_MOVED);
}

/// LunpaFortressCave, Move on the crates: once they are past row 22, over their
/// hole, the cave shakes and they sink.
static void LunpaFortress_MoveCrates(void) {
  struct Actor *cratesActor;

  cratesActor = Actor_Get(ACTOR_CAVE_CRATES);
  if ((cratesActor->z.fixed >> 20) > 22) {
    MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    Event_PlaySound(SE_CLATTER);
    MapGrid_CopyCellAttributes(15, 20, 1, 1, 15, 22);
    MapGrid_CopyCellAttributes(17, 23, 1, 3, 15, 23);
    cratesActor = Actor_Get(ACTOR_CAVE_CRATES);
    if (cratesActor != NULL) {
      Actor_SetSpriteFlags(cratesActor, 0);
      cratesActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    }
    GameFlag_Set(FLAG_LUNPA_FORTRESS_CRATES_MOVED);
  }
}

/// The crates stay sunk in their hole.
static void LunpaFortress_RestoreLoweredCrates(void) {
  Actor_SetPosition(ACTOR_CAVE_CRATES, PIXELS(248), PIXELS(376));
  LunpaFortress_LowerCrates();
}

/// Draws one of LunpaFortressToadonpa's cell doors open.
static void LunpaFortress_OpenToadonpaDoor(s32 doorIndex) {
  s32 doorCellX = gLunpaFortressToadonpaDoorCells[doorIndex * 2];
  s32 doorCellY = gLunpaFortressToadonpaDoorCells[doorIndex * 2 + 1];

  MapGrid_CopyCells(0, 77, 1, 3, doorCellX, doorCellY);
  MapGrid_CopyCells(1, 77, 1, 1, doorCellX + 1, doorCellY);
  MapGrid_CopyCells(doorCellX, doorCellY - 48, 1, 1, doorCellX, doorCellY - 46);
}

/// LunpaFortressToadonpa: facing a cell door with the Cell Key opens it with a
/// rumble. After Donpa's judgment the fifth, Dodonpa's cell, stays shut.
static void LunpaFortress_UnlockToadonpaDoor(void) {
  struct FieldWork *work;
  s32 doorTrigger;
  s32 doorIndex;

  work = gFieldWork;
  if (PartyInventory_FindMember(ITEM_CELL_KEY) != -1) {
    doorTrigger = work->stepTrigger;
    doorIndex = doorTrigger - CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR;
    if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) == 0 || doorIndex != 4) {
      LunpaFortress_OpenToadonpaDoor(doorIndex);
      Event_PlaySound(SE_GATE_MOVE);
      MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
      GameFlag_Set(doorTrigger + (TOADONPA_DOOR_FLAG -
                                  CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR));
    }
  }
}

/// Draws one of LunpaFortressCave's cell doors open and lets the party through;
/// the middle door opens two cells.
static void LunpaFortress_OpenCaveDoor(s32 doorIndex) {
  s32 doorCellX;
  s32 doorCellY;
  s32 attributeCellY;

  doorCellX = gLunpaFortressCaveDoorCells[doorIndex * 2];
  doorCellY = gLunpaFortressCaveDoorCells[doorIndex * 2 + 1];
  MapGrid_CopyCells(0, 77, 1, 3, doorCellX, doorCellY);
  MapGrid_CopyCells(1, 77, 1, 1, doorCellX + 1, doorCellY);
  attributeCellY = doorCellY - 44;
  MapGrid_CopyCellAttributes(doorCellX, doorCellY - 45, 1, 1, doorCellX,
                             attributeCellY);
  if (doorIndex == 1)
    MapGrid_CopyCellAttributes(doorCellX, attributeCellY, 1, 1, doorCellX,
                               doorCellY - 43);
}

/// LunpaFortressCave: facing a cell door with the Cell Key opens it with a
/// rumble.
static void LunpaFortress_UnlockCaveDoor(void) {
  struct FieldWork *work;
  s32 doorTrigger;

  work = gFieldWork;
  if (PartyInventory_FindMember(ITEM_CELL_KEY) != -1) {
    doorTrigger = work->stepTrigger;
    LunpaFortress_OpenCaveDoor(doorTrigger -
                               CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR);
    Event_PlaySound(SE_GATE_MOVE);
    MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    GameFlag_Set(doorTrigger +
                 (CAVE_DOOR_FLAG - CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR));
  }
}

/// Draws one of LunpaFortressUpperFloor's cell doors open.
static void LunpaFortress_OpenUpperFloorDoor(s32 doorIndex) {
  s32 doorCellX = gLunpaFortressUpperFloorDoorCells[doorIndex * 2];
  s32 doorCellY = gLunpaFortressUpperFloorDoorCells[doorIndex * 2 + 1];

  MapGrid_CopyCells(55, 121, 1, 3, doorCellX, doorCellY);
  MapGrid_CopyCells(56, 121, 1, 1, doorCellX + 1, doorCellY);
  MapGrid_CopyCells(doorCellX, doorCellY - 63, 1, 1, doorCellX, doorCellY - 62);
}

/// LunpaFortressUpperFloor: facing a cell door with the Cell Key opens it with
/// a rumble.
static void LunpaFortress_UnlockUpperFloorDoor(void) {
  struct FieldWork *work;
  s32 doorTrigger;

  work = gFieldWork;
  if (PartyInventory_FindMember(ITEM_CELL_KEY) != -1) {
    doorTrigger = work->stepTrigger;
    LunpaFortress_OpenUpperFloorDoor(doorTrigger -
                                     CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR);
    Event_PlaySound(SE_GATE_MOVE);
    MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    GameFlag_Set(doorTrigger + (UPPER_FLOOR_DOOR_FLAG -
                                CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR));
  }
}

/// Draws one of LunpaFortressGroundFloor's cell doors open and lets the party
/// through.
static void LunpaFortress_OpenGroundFloorDoor(s32 doorIndex) {
  // BUG: the third door's trigger and flag (GROUND_FLOOR_DOOR_FLAG + 2) ask for
  // index 2, past the two doors gLunpaFortressGroundFloorDoorCells holds.
  s32 doorCellX = gLunpaFortressGroundFloorDoorCells[doorIndex * 2];
  s32 doorCellY = gLunpaFortressGroundFloorDoorCells[doorIndex * 2 + 1];

  MapGrid_CopyCells(1, 80, 1, 3, doorCellX, doorCellY);
  MapGrid_CopyCells(2, 80, 1, 1, doorCellX + 1, doorCellY);
  MapGrid_CopyCellAttributeRect(doorCellX, doorCellY - 63, 1, 1, doorCellX,
                                doorCellY - 62);
}

/// LunpaFortressGroundFloor: facing a cell door with the Cell Key opens it with
/// a rumble.
static void LunpaFortress_UnlockGroundFloorDoor(void) {
  struct FieldWork *work;
  s32 doorTrigger;

  work = gFieldWork;
  if (PartyInventory_FindMember(ITEM_CELL_KEY) != -1) {
    doorTrigger = work->stepTrigger;
    LunpaFortress_OpenGroundFloorDoor(doorTrigger -
                                      CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR);
    Event_PlaySound(SE_GATE_MOVE);
    MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    GameFlag_Set(doorTrigger + (GROUND_FLOOR_DOOR_FLAG -
                                CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR));
  }
}

/// Lunpa Fortress, every scene: searching a cell door without the Cell Key,
/// it stays shut.
/// "The door is tightly locked."
static void LunpaFortress_SearchLockedDoor(void) {
  if (PartyInventory_FindMember(ITEM_CELL_KEY) == -1) {
    FieldMessage_Show(MsgFieldDoorTightlyLocked, 1);
  }
}

/// LunpaFortressCave: searching the door no key opens.
static void LunpaFortress_SearchSealedDoor(void) {
  FieldMessage_Show(MsgFieldDoorTightlyLocked, 1);
}

// XXX (no evidence): which scene's actors these are; the pickups are
// unused and name no scene, and LunpaFortressGroundFloor's first four rows (in
// a chest's sprite, at the map's origin) are the only rows nothing else uses.
/// The four actors from ACTOR_FIRST_ITEM_DISPLAY that the unused item
/// pickups below show their items on, in a scene they do not name.
/// ★ Named from what the player sees: four chests (SPRITE_CHEST) at the
/// origin of LunpaFortressGroundFloor, the likely display actors (its
/// render)
enum { ACTOR_FIRST_ITEM_DISPLAY = ACTOR_FIRST_PLACED };

/// Unused: nothing calls this pickup or the three after it. Together they
/// would set out an Arctic Blade, a Masamune, a Silver Blade and a Shamshir on
/// the four display actors, each taken once
/// (FLAG_LUNPA_FORTRESS_ARCTIC_BLADE to FLAG_LUNPA_FORTRESS_SHAMSHIR); this one
/// takes the Arctic Blade.
static void UNUSED LunpaFortress_UnusedTakeArcticBlade(void) {
  if (LunpaFortress_TryCollectDisplayedItem(ACTOR_FIRST_ITEM_DISPLAY,
                                            ITEM_ARCTIC_BLADE) != 0) {
    GameFlag_Set(FLAG_LUNPA_FORTRESS_ARCTIC_BLADE);
  }
}

/// Unused: the Masamune, on the second display actor.
static void UNUSED LunpaFortress_UnusedTakeMasamune(void) {
  if (LunpaFortress_TryCollectDisplayedItem(ACTOR_FIRST_ITEM_DISPLAY + 1,
                                            ITEM_MASAMUNE) != 0) {
    GameFlag_Set(FLAG_LUNPA_FORTRESS_MASAMUNE);
  }
}

/// Unused: the Silver Blade, on the third display actor.
static void UNUSED LunpaFortress_UnusedTakeSilverBlade(void) {
  if (LunpaFortress_TryCollectDisplayedItem(ACTOR_FIRST_ITEM_DISPLAY + 2,
                                            ITEM_SILVER_BLADE) != 0) {
    GameFlag_Set(FLAG_LUNPA_FORTRESS_SILVER_BLADE);
  }
}

/// Unused: the Shamshir, on the fourth display actor.
static void UNUSED LunpaFortress_UnusedTakeShamshir(void) {
  if (LunpaFortress_TryCollectDisplayedItem(ACTOR_FIRST_ITEM_DISPLAY + 3,
                                            ITEM_SHAMSHIR) != 0) {
    GameFlag_Set(FLAG_LUNPA_FORTRESS_SHAMSHIR);
  }
}

/// Unused: called only by the four pickups above. Gives the item an
/// actor shows, with its effect; with no room for it, the actor shows
/// the item is left behind. Returns whether it was given.
static s32 LunpaFortress_TryCollectDisplayedItem(s32 actorId, s32 itemId) {
  s32 itemGiven = 0;
  s32 itemEffect;

  Event_Start();
  itemEffect = FieldEffect_PlayCueAndStartEmitterOnTarget(0, actorId, itemId);
  if (PartyInventory_GiveItem(itemId, 0) != -1) {
    Import_ActorSetAnimation(actorId, 2);
    itemGiven = 1;
  } else {
    Event_PlaySound(SE_CHEST_CLOSE);
    Import_ActorSetAnimation(actorId, 5);
  }
  Actor_Release((struct Actor *)itemEffect);
  Event_End();
  return itemGiven;
}

static void UNUSED LunpaFortress_DoNothingA(void) {}

static void UNUSED LunpaFortress_DoNothingB(void) {}

static void UNUSED LunpaFortress_DoNothingC(void) {}

static void UNUSED LunpaFortress_DoNothingD(void) {}

/// LunpaFortressUpperFloor: the leader steps up and takes the Cell Key. Catch
/// on the key hanging there, or talking to it, starts this.
static void LunpaFortress_TakeCellKey(void) {
  Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 264, 792);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Event_Wait(10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  ItemDisplay_ShowItem(ITEM_CELL_KEY, 3);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  PartyInventory_GiveItem(ITEM_CELL_KEY, 0);
  GameFlag_Set(FLAG_LUNPA_FORTRESS_CELL_KEY);
  Import_ActorSetPosition(ACTOR_UPPER_FLOOR_CELL_KEY, 0, 0);
}
/// Where the patrolling pair cannot see the leader: the west end of their
/// corridor and the cells below its gap.
static s32 LunpaFortress_IsLeaderInPatrolSafeZone(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  s32 leaderFixedZ = leader->z.fixed;
  s32 leaderFixedX;
  s32 leaderTileZ, leaderTileX;

  if (leaderFixedZ < 0) {
    leaderFixedZ += PIXELS(16) - 1;
  }
  leaderFixedX = leader->x.fixed;
  leaderTileZ = leaderFixedZ >> 20;
  if (leaderFixedX < 0) {
    leaderFixedX += PIXELS(16) - 1;
  }
  leaderTileX = leaderFixedX >> 20;
  if ((u32)(leaderTileZ - 5) <= 2 && leaderTileX <= 10) {
    return 1;
  }
  if ((u32)(leaderTileX - 8) <= 1 && leaderTileZ > 22) {
    return 1;
  }
  return 0;
}

// UB: these patrols formerly reached gFieldWork through gMapGridWork[19] and
// treated its request members as a halfword array. Named owner access adds
// eight bytes to each of the three patrol tasks in the EN trial.
#ifdef UBFIX
#define PATROL_PSYNERGY_REQUEST(work) ((work)->psynergy)
#define PATROL_ITEM_REQUEST(work) ((work)->item)
#define PATROL_RAISED_TRIGGER(work) ((work)->raisedTrigger)
#else
#define PATROL_PSYNERGY_REQUEST(work) ((work)[191])
#define PATROL_ITEM_REQUEST(work) ((work)[192])
#define PATROL_RAISED_TRIGGER(work) ((work)[193])
#endif

/// Every frame in LunpaFortressUpperFloor: the pair patrolling the west
/// corridor. They stand still while FLAG_MENU_OPEN is set or Psynergy or an
/// item is in use; otherwise they walk, with MAPGRID_LAYER_OVERHEAD shaking and
/// following the first. Close to a cloaked leader they request
/// CLOAK_NEAR_GUARD; seeing an uncloaked leader, they raise
/// RAISED_PATROL_PAIR_SPOTTED.
static void LunpaFortress_UpdatePairedGuardDetection(void) {
  struct Actor *firstGuard = Actor_Get(ACTOR_UPPER_FLOOR_FIRST_PATROL);
  struct Actor *secondGuard = Actor_Get(ACTOR_UPPER_FLOOR_SECOND_PATROL);
  struct MapGridLayerScroll *patrolLayer =
      &((struct MapGridScrollWork *)gMapGridWork[0])
           ->layers[MAPGRID_LAYER_OVERHEAD];
#ifdef UBFIX
  struct FieldWork *work = gFieldWork;
#else
  s16 *work = *(s16 **)&gMapGridWork[19];
#endif

  if (gFrameCount & 1) {
    patrolLayer->speedX = 1;
    patrolLayer->speedY = 1;
  } else {
    patrolLayer->speedX = -1;
    patrolLayer->speedY = -1;
  }
  if (GameFlag_Get(FLAG_MENU_OPEN) || PATROL_PSYNERGY_REQUEST(work) != 0 ||
      PATROL_ITEM_REQUEST(work) != 0) {
    firstGuard->paused = 1;
    secondGuard->paused = 1;
  } else if (!Event_GetFlag(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED)) {
    firstGuard->paused = 0;
    secondGuard->paused = 0;
    if (!GameFlag_Get(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED) &&
        firstGuard->paused == 0) {
      patrolLayer->phaseX = PAIRED_PATROL_LAYER_ORIGIN_X - firstGuard->x.fixed;
    }
    if (!LunpaFortress_IsLeaderInPatrolSafeZone()) {
      if (gMainRoutineWork.cloaked != 0) {
        if (LunpaFortress_IsLeaderNearGuard(ACTOR_UPPER_FLOOR_FIRST_PATROL) &&
            gMainRoutineWork.cloaked != 0) {
          SetFieldWorkValue(&PATROL_PSYNERGY_REQUEST(work), CLOAK_NEAR_GUARD);
          return;
        }
        if (LunpaFortress_IsLeaderNearGuard(ACTOR_UPPER_FLOOR_SECOND_PATROL) &&
            gMainRoutineWork.cloaked != 0) {
          SetFieldWorkValue(&PATROL_PSYNERGY_REQUEST(work), CLOAK_NEAR_GUARD);
          return;
        }
      }
      if (gMainRoutineWork.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(ACTOR_UPPER_FLOOR_FIRST_PATROL)) {
          GameFlag_Set(FLAG_SCENE_TEMP_15);
          GameFlag_Set(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED);
        }
        if (LunpaFortress_CanGuardSeeLeader(ACTOR_UPPER_FLOOR_SECOND_PATROL)) {
          GameFlag_Set(FLAG_SCENE_TEMP_15);
          GameFlag_Set(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED);
        }
      }
      if (GameFlag_Get(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED)) {
        SetFieldWorkValue(&PATROL_RAISED_TRIGGER(work),
                          RAISED_PATROL_PAIR_SPOTTED);
      }
    }
  }
}

/// LunpaFortressUpperFloor, raised when the patrolling pair spots the leader:
/// the first stops and stares, and the second throws the party out.
static void LunpaFortress_PatrolPairThrowsPartyOut(void) {
  Event_Start();
  Actor_Stop(ACTOR_UPPER_FLOOR_FIRST_PATROL);
  Actor_OffsetPositionAndResetMotion(ACTOR_UPPER_FLOOR_FIRST_PATROL, 0, 0);
  Import_ActorSetAnimation(ACTOR_UPPER_FLOOR_FIRST_PATROL, 0);
  Actor_SetAngleToward(ACTOR_UPPER_FLOOR_FIRST_PATROL, ACTOR_PARTY_LEADER, 0);
  FieldEffect_SpawnAttached(ACTOR_UPPER_FLOOR_FIRST_PATROL,
                            EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  LunpaFortress_GuardThrowsPartyOut(ACTOR_UPPER_FLOOR_SECOND_PATROL);
  Event_End();
}

/// Whether the leader and the hall patrol are in the two parallel corridors,
/// where the guard passes without seeing the leader.
static s32 LunpaFortress_IsLeaderPassingPatrollingGuard(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  struct Actor *guard = Actor_GetById(ACTOR_UPPER_FLOOR_HALL_PATROL);
  s32 leaderFixedX = leader->x.fixed;
  s32 guardFixedZ;
  s32 guardFixedX;
  s32 leaderTileX, guardTileZ, guardTileX;

  if (leaderFixedX < 0) {
    leaderFixedX += PIXELS(16) - 1;
  }
  leaderTileX = leaderFixedX >> 20;
  guardFixedZ = guard->z.fixed;
  if (guardFixedZ < 0) {
    guardFixedZ += PIXELS(16) - 1;
  }
  guardFixedX = guard->x.fixed;
  guardTileZ = guardFixedZ >> 20;
  if (guardFixedX < 0) {
    guardFixedX += PIXELS(16) - 1;
  }
  guardTileX = guardFixedX >> 20;
  if (leaderTileX == 52 && guardTileX == 57 && guardTileZ > 34 &&
      guardTileZ <= 40) {
    return 1;
  }
  if (leaderTileX == 57 && guardTileX == 52 && guardTileZ > 34 &&
      guardTileZ <= 40) {
    return 1;
  }
  return 0;
}

/// Every frame in LunpaFortressUpperFloor: the patrol in the east hall, as
/// LunpaFortress_UpdatePairedGuardDetection; seeing the leader, it raises
/// RAISED_HALL_PATROL_SPOTTED.
static void LunpaFortress_UpdateHallGuardDetection(void) {

  struct Actor *guard = Actor_Get(ACTOR_UPPER_FLOOR_HALL_PATROL);
  struct MapGridLayerScroll *patrolLayer =
      &((struct MapGridScrollWork *)gMapGridWork[0])
           ->layers[MAPGRID_LAYER_OVERHEAD];
#ifdef UBFIX
  struct FieldWork *work = gFieldWork;
#else
  s16 *work = *(s16 **)&gMapGridWork[19];
#endif

  FieldCamera_GetActor();
  if (gFrameCount & 1) {
    patrolLayer->speedX = 1;
    patrolLayer->speedY = 1;
  } else {
    patrolLayer->speedX = -1;
    patrolLayer->speedY = -1;
  }
  if (GameFlag_Get(FLAG_MENU_OPEN) || PATROL_PSYNERGY_REQUEST(work) != 0 ||
      PATROL_ITEM_REQUEST(work) != 0) {
    guard->paused = 1;
  } else if (!Event_GetFlag(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED)) {
    guard->paused = 0;
    if (!GameFlag_Get(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED) &&
        guard->paused == 0) {
      patrolLayer->phaseX = PIXELS(832) - guard->x.fixed;
      patrolLayer->phaseY = PIXELS(576) - guard->z.fixed;
    }
    if (!LunpaFortress_IsLeaderPassingPatrollingGuard()) {
      LunpaFortress_IsGuardNearViewCenter(ACTOR_UPPER_FLOOR_HALL_PATROL);
      if (LunpaFortress_IsLeaderNearGuard(ACTOR_UPPER_FLOOR_HALL_PATROL) &&
          gMainRoutineWork.cloaked != 0) {
        SetFieldWorkValue(&PATROL_PSYNERGY_REQUEST(work), CLOAK_NEAR_GUARD);
        return;
      }
      if (gMainRoutineWork.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(ACTOR_UPPER_FLOOR_HALL_PATROL)) {
          GameFlag_Set(FLAG_SCENE_TEMP_15);
          GameFlag_Set(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED);
        }
      }
      if (GameFlag_Get(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED)) {
        SetFieldWorkValue(&PATROL_RAISED_TRIGGER(work),
                          RAISED_HALL_PATROL_SPOTTED);
      }
    }
  }
}

/// LunpaFortressUpperFloor, raised when the hall patrol spots the leader: it
/// throws the party out.
static void LunpaFortress_HallPatrolThrowsPartyOut(void) {
  LunpaFortress_GuardThrowsPartyOut(ACTOR_UPPER_FLOOR_HALL_PATROL);
  Event_End();
}

/// Where LunpaFortressGroundFloor's patrol cannot see the leader: three
/// stretches of its rounds.
static s32 LunpaFortress_IsLeaderInGroundFloorPatrolSafeZone(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  s32 leaderTileZ = leader->z.fixed / PIXELS(16);
  s32 leaderTileX = leader->x.fixed / PIXELS(16);

  if ((u32)(leaderTileX - 41) <= 3 && leaderTileZ > 25 && leaderTileZ <= 28) {
    return 1;
  }
  if (leaderTileX == 41 && leaderTileZ > 37 && leaderTileZ <= 41) {
    return 1;
  }
  if ((u32)(leaderTileX - 54) <= 2 && leaderTileZ > 30 && leaderTileZ <= 40) {
    return 1;
  }
  return 0;
}

/// Every frame in LunpaFortressGroundFloor: the patrolling guard, as
/// LunpaFortress_UpdatePairedGuardDetection; seeing the leader, it raises
/// RAISED_GROUND_FLOOR_PATROL_SPOTTED.
static void LunpaFortress_UpdateGroundFloorPatrolDetection(void) {

  struct Actor *guard = Actor_Get(ACTOR_GROUND_FLOOR_PATROL_GUARD);
  struct MapGridLayerScroll *patrolLayer =
      &((struct MapGridScrollWork *)gMapGridWork[0])
           ->layers[MAPGRID_LAYER_OVERHEAD];
#ifdef UBFIX
  struct FieldWork *work = gFieldWork;
#else
  s16 *work = *(s16 **)&gMapGridWork[19];
#endif

  if (gFrameCount & 1) {
    patrolLayer->speedX = 1;
    patrolLayer->speedY = 1;
  } else {
    patrolLayer->speedX = -1;
    patrolLayer->speedY = -1;
  }
  if (GameFlag_Get(FLAG_MENU_OPEN) || PATROL_PSYNERGY_REQUEST(work) != 0 ||
      PATROL_ITEM_REQUEST(work) != 0) {
    guard->paused = 1;
  } else if (!Event_GetFlag(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED)) {
    guard->paused = 0;
    if (!GameFlag_Get(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED) &&
        guard->paused == 0) {
      patrolLayer->phaseX = PIXELS(752) - guard->x.fixed;
      patrolLayer->phaseY = PIXELS(496) - guard->z.fixed;
    }
    if (!LunpaFortress_IsLeaderInGroundFloorPatrolSafeZone()) {
      if (LunpaFortress_IsLeaderNearGuard(ACTOR_GROUND_FLOOR_PATROL_GUARD) &&
          gMainRoutineWork.cloaked != 0) {
        SetFieldWorkValue(&PATROL_PSYNERGY_REQUEST(work), CLOAK_NEAR_GUARD);
        return;
      }
      if (gMainRoutineWork.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(ACTOR_GROUND_FLOOR_PATROL_GUARD)) {
          GameFlag_Set(FLAG_SCENE_TEMP_15);
          GameFlag_Set(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED);
        }
      }
      if (GameFlag_Get(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED)) {
        SetFieldWorkValue(&PATROL_RAISED_TRIGGER(work),
                          RAISED_GROUND_FLOOR_PATROL_SPOTTED);
      }
    }
  }
}

/// LunpaFortressGroundFloor, raised when the patrol spots the leader: it throws
/// the party out.
static void LunpaFortress_GroundFloorPatrolThrowsPartyOut(void) {
  LunpaFortress_GuardThrowsPartyOut(ACTOR_GROUND_FLOOR_PATROL_GUARD);
  Event_End();
}

/// Whether the leader is outside the upper hall the guard there watches.
static s32 LunpaFortress_IsLeaderOutsideUpperHall(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  s32 leaderTileZ = leader->z.fixed / PIXELS(16);
  s32 leaderTileX = leader->x.fixed / PIXELS(16);

  if (leaderTileX > 45 && leaderTileZ > 14 && leaderTileX <= 64 &&
      leaderTileZ <= 16) {
    return 0;
  }
  return 1;
}

/// Every frame in LunpaFortressGroundFloor's upper hall: the guard who sees the
/// uncloaked leader raises RAISED_UPPER_HALL_SPOTTED.
static void LunpaFortress_CheckUpperHallGuard(void) {

  struct FieldWork *work;

  work = gFieldWork;
  if (Event_GetFlag(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED) == 0) {
    if (LunpaFortress_IsLeaderOutsideUpperHall() == 0) {
      if (gMainRoutineWork.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(
                ACTOR_GROUND_FLOOR_UPPER_HALL_GUARD) != 0) {
          GameFlag_Set(FLAG_SCENE_TEMP_15);
          GameFlag_Set(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED);
        }
      }
      if (GameFlag_Get(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED) != 0) {
        work->raisedTrigger = RAISED_UPPER_HALL_SPOTTED;
      }
    }
  }
}

/// LunpaFortressGroundFloor, raised when the upper hall's guard spots the
/// leader: he throws the party out.
static void LunpaFortress_UpperHallGuardThrowsPartyOut(void) {
  LunpaFortress_GuardThrowsPartyOut(ACTOR_GROUND_FLOOR_UPPER_HALL_GUARD);
  Event_End();
}

/// Every frame after the party arrives in LunpaFortressGroundFloor's entrance
/// or main passage: the guard who sees the uncloaked leader raises
/// RAISED_ENTRANCE_GUARD_SPOTTED, once.
static void LunpaFortress_CheckEntranceGuard(void) {

  struct FieldWork *work = gFieldWork;

  if (LunpaFortress_CanGuardSeeLeader(ACTOR_GROUND_FLOOR_ENTRANCE_GUARD) != 0 &&
      gMainRoutineWork.cloaked == 0) {
    s16 *raisedTrigger;
    s32 caughtTrigger;

    Task_Remove(LunpaFortress_CheckEntranceGuard);
    raisedTrigger = &work->raisedTrigger;
    caughtTrigger = RAISED_ENTRANCE_GUARD_SPOTTED;
    *raisedTrigger = caughtTrigger;
  }
}

/// Every frame after the party arrives in LunpaFortressGroundFloor's entrance
/// or main passage: either sentry who sees the uncloaked leader raises
/// RAISED_SENTRIES_SPOTTED, once for both.
static void LunpaFortress_CheckFirstEntranceSentry(void) {

  struct FieldWork *work;

  work = gFieldWork;
  if (Event_GetFlag(FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY) == 0) {
    if (LunpaFortress_CanGuardSeeLeader(ACTOR_GROUND_FLOOR_FIRST_SENTRY) != 0) {
      if (gMainRoutineWork.cloaked == 0) {
        GameFlag_Set(FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY);
        Task_Remove(LunpaFortress_CheckFirstEntranceSentry);
        Task_Remove(LunpaFortress_CheckSecondEntranceSentry);
        work->raisedTrigger = RAISED_SENTRIES_SPOTTED;
      }
    }
  }
}

/// The second sentry, as the first.
static void LunpaFortress_CheckSecondEntranceSentry(void) {

  struct FieldWork *work;

  work = gFieldWork;
  if (Event_GetFlag(FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY) == 0) {
    if (LunpaFortress_CanGuardSeeLeader(ACTOR_GROUND_FLOOR_SECOND_SENTRY) !=
        0) {
      if (gMainRoutineWork.cloaked == 0) {
        GameFlag_Set(FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY);
        Task_Remove(LunpaFortress_CheckSecondEntranceSentry);
        Task_Remove(LunpaFortress_CheckFirstEntranceSentry);
        work->raisedTrigger = RAISED_SENTRIES_SPOTTED;
      }
    }
  }
}

/// Whether the leader is in the guard's column, up to six cells away.
static s32 LunpaFortress_IsLeaderInGuardVerticalSight(s32 actorId) {
  struct Actor *guard = Actor_GetById(actorId);
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  s32 guardTileZ = guard->z.fixed / PIXELS(16);
  s32 guardTileX = guard->x.fixed / PIXELS(16);
  s32 leaderTileZ = leader->z.fixed / PIXELS(16);
  s32 leaderTileX = leader->x.fixed / PIXELS(16);
  s32 zDistance = guardTileZ - leaderTileZ;

  if (zDistance >= -6 && zDistance <= 6 && guardTileX - 1 < leaderTileX &&
      guardTileX + 1 > leaderTileX) {
    return 1;
  }
  return 0;
}

/// Whether the leader is within a row of the guard, up to six cells away.
static s32 LunpaFortress_IsLeaderInGuardHorizontalSight(s32 actorId) {
  struct Actor *guard = Actor_GetById(actorId);
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  s32 guardTileZ = guard->z.fixed / PIXELS(16);
  s32 guardTileX = guard->x.fixed / PIXELS(16);
  s32 leaderTileZ = leader->z.fixed / PIXELS(16);
  s32 leaderTileX = leader->x.fixed / PIXELS(16);
  s32 xDistance = guardTileX - leaderTileX;

  if (xDistance < -6 || xDistance > 6) {
    return 0;
  }
  if (guardTileZ - 2 < leaderTileZ && guardTileZ + 2 > leaderTileZ) {
    return 1;
  }
  return 0;
}

/// Whether a guard on screen sees the leader along his row or column.
static s32 LunpaFortress_CanGuardSeeLeader(s32 actorId) {
  if (LunpaFortress_IsGuardNearViewCenter(actorId) == 0) {
    return 0;
  }
  if (LunpaFortress_IsLeaderInGuardVerticalSight(actorId) != 0) {
    return 1;
  }
  {
    s32 inHorizontalSight =
        LunpaFortress_IsLeaderInGuardHorizontalSight(actorId);

    return (u32)(inHorizontalSight | -inHorizontalSight) >> 31;
  }
}

/// Whether the leader is within four cells, walking, of the cell below the
/// guard.
static s32 LunpaFortress_IsLeaderNearGuard(s32 actorId) {
  struct Actor *guard = Actor_GetById(actorId);
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  s32 guardTileZ = guard->z.fixed / PIXELS(16);
  s32 guardTileX = guard->x.fixed / PIXELS(16);
  s32 leaderTileZ = leader->z.fixed / PIXELS(16);
  s32 leaderTileX = leader->x.fixed / PIXELS(16);
  s32 xDistance = guardTileX - leaderTileX;
  s32 zDistance;

  guardTileZ += 1;
  if (xDistance < 0) {
    xDistance = -xDistance;
  }
  zDistance = guardTileZ - leaderTileZ;
  if (zDistance < 0) {
    zDistance = -zDistance;
  }
  if (xDistance + zDistance <= 4) {
    return 1;
  }
  return 0;
}

/// Whether the guard is on screen: within seven columns and five rows of the
/// view's center.
static s32 LunpaFortress_IsGuardNearViewCenter(s32 actorId) {
  struct Actor *guard = Actor_GetById(actorId);
  struct Actor *viewCenter = FieldCamera_GetActor();
  s32 guardTileX = guard->x.fixed / PIXELS(16);
  s32 guardTileZ = guard->z.fixed / PIXELS(16);
  s32 viewTileX = viewCenter->x.fixed / PIXELS(16);
  s32 viewTileZ = viewCenter->z.fixed / PIXELS(16);
  s32 xDistance = guardTileX - viewTileX;
  s32 zDistance;

  if (xDistance < 0) {
    xDistance = -xDistance;
  }
  zDistance = guardTileZ - viewTileZ;
  if (zDistance < 0) {
    zDistance = -zDistance;
  }
  if (xDistance > 7 || zDistance > 5) {
    return 0;
  }
  return 1;
}

/// Every frame in LunpaFortressGroundFloor: walking into the second cell door's
/// column with the Cell Key unlocks it.
static void LunpaFortress_CheckGroundFloorDoorApproach(void) {

  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

  if (GameFlag_Get(GROUND_FLOOR_DOOR_FLAG + 1) == 0) {
    s32 playerX = leader->x.fixed / PIXELS(16);
    s32 playerZ = leader->z.fixed / PIXELS(16);

    if (playerX == 43 && playerZ > 28 && playerZ <= 31) {
      s16 *touchedTrigger = &gFieldWork->stepTrigger;
      s32 doorTrigger = CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 1;

      *touchedTrigger = doorTrigger;
      LunpaFortress_UnlockGroundFloorDoor();
    }
  }
}

/// Every frame in LunpaFortressUpperFloor: walking east or west into the first
/// cell door's column with the Cell Key unlocks it.
static void LunpaFortress_CheckUpperFloorDoorApproach(void) {

  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

  if (GameFlag_Get(UPPER_FLOOR_DOOR_FLAG) == 0) {
    s32 playerX = leader->x.fixed / PIXELS(16);
    s32 playerZ = leader->z.fixed / PIXELS(16);

    if (playerX == 16 && playerZ > 55 && playerZ <= 58 &&
        (leader->facing == FACING_NORTH || leader->facing == FACING_SOUTH)) {
      s16 *touchedTrigger = &gFieldWork->stepTrigger;
      s32 doorTrigger = CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR;

      *touchedTrigger = doorTrigger;
      LunpaFortress_UnlockUpperFloorDoor();
    }
  }
}

/// Unused: nothing calls it; LunpaFortressUpperFloor's patrolling pair throws
/// the party out through LunpaFortress_PatrolPairThrowsPartyOut instead. The
/// first patrol would stop, ask who the party is and send it out.
static void UNUSED LunpaFortress_UnusedFirstPatrolThrowsPartyOut(void) {
  Event_Start();
  Actor_OffsetPositionAndResetMotion(ACTOR_UPPER_FLOOR_FIRST_PATROL, 0, 0);
  Actor_SetMovementTypeById(ACTOR_UPPER_FLOOR_FIRST_PATROL,
                            MOVEMENT_TYPE_STAND);
  Actor_Stop(ACTOR_UPPER_FLOOR_FIRST_PATROL);
  Import_ActorSetAnimation(ACTOR_UPPER_FLOOR_FIRST_PATROL, 0);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  {
    s32 challengeMessage = MsgLunpaWho2;

    Event_SetMessageId(challengeMessage);
    Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_FIRST_PATROL, 0);
    FieldEffect_SpawnAttached(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                              60);
    Event_SetMessageId(challengeMessage + 1);
  }
  Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_FIRST_PATROL, 0);
  Import_EventSetExitRequest(EXIT_THROWN_OUT);
  Event_FinishScreenTransition();
  Event_End();
}

/// A guard who caught the party sends it back to the Lunpa Fortress Gates.
/// "Who are you? How did you get in here?"
static void LunpaFortress_GuardThrowsPartyOut(s32 actorId) {
  Event_Start();
  Event_Start();
  FieldEffect_SpawnAttached(actorId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 1);
  Actor_OffsetPositionAndResetMotion(actorId, 0, 0);
  Actor_SetMovementTypeById(actorId, MOVEMENT_TYPE_STAND);
  Import_ActorSetAnimation(actorId, 0);
  Actor_SetAngleToward(actorId, ACTOR_PARTY_LEADER, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_OffsetPositionAndResetMotion(actorId, 0, 0);
  Actor_SetMovementTypeById(actorId, MOVEMENT_TYPE_STAND);
  Actor_Stop(actorId);
  Import_ActorSetAnimation(actorId, 0);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  {
    s32 challengeMessage = MsgLunpaWho2;

    Event_SetMessageId(challengeMessage);
    Event_ShowMessageAndWait(actorId, 0);
    Actor_SetAngleToward(ACTOR_PARTY_LEADER, actorId, 0);
    FieldEffect_SpawnAttached(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                              60);
    Event_SetMessageId(challengeMessage + 1);
  }
  Event_ShowMessageAndWait(actorId, 0);
  Event_FinishScreenTransition();
  Event_Wait(60);
  Import_EventSetExitRequest(EXIT_THROWN_OUT);
  Event_End();
}

/// A guard faces the leader with one of four challenges, chosen at random
/// when the scene was entered.
static void LunpaFortress_GuardChallengesParty(s32 actorId) {

  Actor_SetAngleToward(actorId, ACTOR_PARTY_LEADER, 0);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, actorId, 0);
  switch (gLunpaFortressRandomPick & 3) {
  case 0:
    LunpaFortress_ActOutIntruderDialogue(actorId);
    break;
  case 1:
    LunpaFortress_ActOutWhosThereDialogue(actorId);
    break;
  case 2:
    LunpaFortress_ActOutGuardShiftDialogue(actorId);
    break;
  case 3:
    LunpaFortress_ActOutScoundrelDialogue(actorId);
    break;
  default:
    LunpaFortress_ActOutWhosThereDialogue(actorId);
    break;
  }
}

/// A guard spots an intruder. "Hey! Intruder!"
static void LunpaFortress_ActOutIntruderDialogue(s32 actorId) {
  s32 messageBase;

  Actor_RunRepeatedMotion(actorId, 1);
  messageBase = MsgLunpaIntruder;
  Event_SetMessageId(messageBase);
  Event_ShowMessageAndWait(actorId, 0);
  Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_SetMessageId(messageBase + 1);
  Event_ShowMessageAndWait(actorId, 0);
  messageBase += 2;
  Actor_SetAnimationAndWait(actorId, 4);
  Event_SetMessageId(messageBase);
  Event_ShowMessageAndWait(actorId, 0);
}

/// A guard asks who is there. "Huh? Who's that?"
static void LunpaFortress_ActOutWhosThereDialogue(s32 actorId) {
  s32 messageBase = MsgLunpaGuardWhosThat;

  Event_SetMessageId(messageBase);
  Event_ShowMessageAndWait(actorId, 0);
  Actor_RunRepeatedMotion(actorId, 1);
  Event_SetMessageId(messageBase + 1);
  Event_ShowMessageAndWait(actorId, 0);
  Actor_SetAnimationAndWait(actorId, 4);
  Event_SetMessageId(messageBase + 2);
  Event_ShowMessageAndWait(actorId, 0);
}

/// A guard expects his relief, then sees who it is. "Is my shift up already?"
static void LunpaFortress_ActOutGuardShiftDialogue(s32 actorId) {
  s32 messageBase;

  messageBase = MsgLunpaShiftAlready;
  Event_SetMessageId(messageBase);
  Event_ShowMessageAndWait(actorId, 0);
  Event_Wait(120);
  Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_SetMessageId((messageBase + 1));
  Event_ShowMessageAndWait(actorId, 0);
  Actor_RunRepeatedMotion(actorId, 1);
  Event_SetMessageId((messageBase + 2));
  Event_ShowMessageAndWait(actorId, 0);
  Actor_SetAnimationAndWait(actorId, 4);
  Event_SetMessageId((messageBase + 3));
  Event_ShowMessageAndWait(actorId, 0);
}

/// A guard calls the party a scoundrel. "You scoundrel!"
static void LunpaFortress_ActOutScoundrelDialogue(s32 actorId) {
  s32 messageBase = MsgLunpaScoundrel;

  Event_SetMessageId(messageBase);
  Event_ShowMessageAndWait(actorId, 0);
  Actor_SetAnimationAndWait(actorId, 4);
  Event_SetMessageId(messageBase + 1);
  Event_ShowMessageAndWait(actorId, 0);
  Actor_RunRepeatedMotion(actorId, 1);
  Event_SetMessageId(messageBase + 2);
  Event_ShowMessageAndWait(actorId, 0);
  Actor_SetAnimationAndWait(actorId, 3);
  Event_SetMessageId(messageBase + 3);
  Event_ShowMessageAndWait(actorId, 0);
}

/// LunpaFortressUpperFloor, step trigger 33: walking up to the west guard, he
/// challenges the party to a battle with a Brigand, and is gone after it.
static void LunpaFortress_FightWestGuard(void) {

  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_PlaySound(SE_MENU_CANCEL);
  FieldEffect_SpawnAttached(ACTOR_UPPER_FLOOR_WEST_GUARD,
                            EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  LunpaFortress_GuardChallengesParty(ACTOR_UPPER_FLOOR_WEST_GUARD);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 2);
  Import_ActorSetPosition(ACTOR_UPPER_FLOOR_WEST_GUARD, 0, 0);
  Event_End();
  GameFlag_Set(FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN);
}

/// LunpaFortressUpperFloor, step trigger 30: walking up to the hall guard, he
/// challenges the party to a battle with a Brigand, and is gone after it.
static void LunpaFortress_FightHallGuard(void) {

  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_PlaySound(SE_MENU_CANCEL);
  FieldEffect_SpawnAttached(ACTOR_UPPER_FLOOR_HALL_GUARD,
                            EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  LunpaFortress_GuardChallengesParty(ACTOR_UPPER_FLOOR_HALL_GUARD);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 2);
  Import_ActorSetPosition(ACTOR_UPPER_FLOOR_HALL_GUARD, 0, 0);
  Event_End();
  GameFlag_Set(FLAG_LUNPA_FORTRESS_MIDDLE_GUARD_BEATEN);
}

/// LunpaFortressUpperFloor, step trigger 31: walking up to the three guards,
/// they argue, then fight the party together as three Brigands. "Who are you!?"
/// After the battle the party comes back here.
static void LunpaFortress_FightThreeGuards(void) {
  s32 messageBase;

  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 1);
  Import_ActorSetAnimation(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 1);
  Import_ActorSetAnimation(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 1);
  Event_PlaySound(SE_MENU_CANCEL);
  Actor_ShowEmote(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(30);
  Actor_SetAngleToward(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, ACTOR_PARTY_LEADER,
                       0);
  messageBase = MsgLunpaWho3;
  Event_SetMessageId(messageBase);
  Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 0);
  Actor_ShowEmote(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_SetAngleToward(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE,
                       ACTOR_PARTY_LEADER, 0);
  Actor_SetAngleToward(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, ACTOR_PARTY_LEADER,
                       0);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER,
                       ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 0);
  Event_Wait(65);
  Actor_FaceDirection(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE,
                      FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST,
                      FACING_NORTH + FACING_STEP, 0);
  Event_SetMessageId((messageBase + 1));
  Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 0);
  Actor_SetAnimationAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 3);
  Event_SetMessageId((messageBase + 2));
  Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 0);
  Event_SetMessageId((messageBase + 3));
  Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 0);
  Actor_RunRepeatedMotion(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 1);
  Event_SetMessageId((messageBase + 4));
  Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 0);
  Actor_SetAnimationAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 3);
  Event_SetMessageId((messageBase + 5));
  Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 0);
  Actor_SetAnimationAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 3);
  Event_Wait(60);
  Actor_SetAngleToward(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE,
                       ACTOR_PARTY_LEADER, 0);
  Actor_SetAngleToward(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, ACTOR_PARTY_LEADER,
                       0);
  Event_Wait(70);
  Actor_WalkTo(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 672, 88);
  Actor_WaitForMoveById(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST);
#if TBS_PATCH_1
  Actor_SetAngleToward(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, ACTOR_PARTY_LEADER,
                       0);
#endif
  Actor_SetAnimationAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 3);
  Event_Wait(30);
  Event_SetMessageId((messageBase + 6));
  Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Event_SetAfterBattleDestination(
      (s32)&SceneId_LunpaFortressUpperFloor,
      LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_AFTER_BATTLE);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 3);
  Actor_SetPosition(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, PIXELS(0), PIXELS(0));
  Actor_SetPosition(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, PIXELS(0),
                    PIXELS(0));
  Actor_SetPosition(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, PIXELS(0), PIXELS(0));
  Event_End();
  GameFlag_Set(FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN);
}

/// The three guards lie beaten where they fought.
static void LunpaFortress_PlaceThreeBeatenGuards(void) {
  Event_Start();
  Actor_SetPosition(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, PIXELS(688),
                    PIXELS(88));
  Actor_SetPosition(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, PIXELS(704),
                    PIXELS(88));
  Actor_SetPosition(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, PIXELS(720),
                    PIXELS(96));
  Import_ActorSetAnimation(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 5);
  Import_ActorSetAnimation(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 5);
  Import_ActorSetAnimation(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 5);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER,
                       ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 0);
  Event_End();
  Event_StartScreenTransition();
}

/// LunpaFortressUpperFloor, step trigger 32: walking up to the east guard, he
/// challenges the party to a battle with a Brigand, and is gone after it.
static void LunpaFortress_FightEastGuard(void) {

  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_PlaySound(SE_MENU_CANCEL);
  FieldEffect_SpawnAttached(ACTOR_UPPER_FLOOR_EAST_GUARD,
                            EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  LunpaFortress_GuardChallengesParty(ACTOR_UPPER_FLOOR_EAST_GUARD);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 2);
  Import_ActorSetPosition(ACTOR_UPPER_FLOOR_EAST_GUARD, 0, 0);
  Event_End();
  GameFlag_Set(FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN);
}

/// LunpaFortressGroundFloor, step trigger 50 or seen by the entrance guard: the
/// party is sent back to the Lunpa Fortress Gates. "Who are you?"
static void LunpaFortress_EntranceGuardThrowsPartyOut(void) {
  Event_Start();
  Actor_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_SetAngleToward(ACTOR_GROUND_FLOOR_ENTRANCE_GUARD, ACTOR_PARTY_LEADER,
                       0);
  Event_PlaySound(SE_MENU_CANCEL);
  FieldEffect_SpawnAttached(ACTOR_GROUND_FLOOR_ENTRANCE_GUARD,
                            EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  {
    s32 challengeMessage = MsgLunpaWho2;

    Event_SetMessageId(challengeMessage);
    Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_ENTRANCE_GUARD, 0);
    FieldEffect_SpawnAttached(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                              50);
    Event_SetMessageId(challengeMessage + 1);
  }
  Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_ENTRANCE_GUARD, 0);
  Event_FinishScreenTransition();
  Event_Wait(60);
  Import_EventSetExitRequest(EXIT_THROWN_OUT);
  Event_End();
  GameFlag_Set(FLAG_LUNPA_FORTRESS_ENTRANCE_GUARD_CAUGHT_PARTY);
}

/// LunpaFortressGroundFloor, step trigger 51 between the sentries or seen by
/// them: the party is sent back to the Lunpa Fortress Gates. "Who are you?"
static void LunpaFortress_SentriesThrowPartyOut(void) {
  s32 challengeMessage;

  Event_Start();
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_PlaySound(SE_MENU_CANCEL);
  Actor_ShowEmote(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GROUND_FLOOR_FIRST_SENTRY,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_SetAngleToward(ACTOR_GROUND_FLOOR_SECOND_SENTRY, ACTOR_PARTY_LEADER, 0);
  Actor_SetAngleToward(ACTOR_GROUND_FLOOR_FIRST_SENTRY, ACTOR_PARTY_LEADER, 0);
  challengeMessage = MsgLunpaWho2;
  Event_SetMessageId(challengeMessage);
  Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_FIRST_SENTRY, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_SetMessageId(challengeMessage + 1);
  Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_FIRST_SENTRY, 0);
  Event_FinishScreenTransition();
  Event_Wait(60);
  Import_EventSetExitRequest(EXIT_THROWN_OUT);
  Event_End();
  GameFlag_Set(FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY);
}

/// The first switch's passage opens.
static void LunpaFortress_OpenFirstSwitchPassage(void) {
  MapGrid_CopyCells(2, 82, 1, 2, 21, 81);
  MapGrid_CopyCellAttributes(21, 32, 1, 1, 21, 34);
}

/// The second switch's passage opens.
static void LunpaFortress_OpenSecondSwitchPassage(void) {
  MapGrid_CopyCells(2, 84, 1, 2, 6, 55);
  MapGrid_CopyCellAttributes(5, 9, 1, 1, 6, 10);
}

/// The passage behind the ivy Whirlwind blew away opens.
static void LunpaFortress_OpenIvyPassage(void) {
  MapGrid_CopyCells(2, 86, 1, 2, 27, 62);
  MapGrid_CopyCellAttributes(26, 16, 1, 1, 27, 17);
}

/// A switch flips in two frames, then its passage opens.
static __inline__ void
LunpaFortress_FlipHiddenSwitch(s32 cellX, s32 cellY,
                               void (*openPassage)(void)) {
  MapGrid_CopyCells(6, 77, 1, 2, cellX, cellY);
  Event_Wait(5);
  MapGrid_CopyCells(7, 77, 1, 2, cellX, cellY);
  Event_Wait(1);
  openPassage();
}

/// LunpaFortressCave, examine trigger 52 once uncovered: searching the first
/// switch while Reveal shows it flips it, and its passage opens. "You flipped
/// the switch."
static void LunpaFortress_FlipFirstSwitch(void) {
  if (gFieldWork->revealActive != 0) {
    if (Event_GetFlag(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED) == 0) {
      FieldMessage_Show(MsgFieldFlippedSwitch, 1);
      Event_PlaySound(SE_THUMP);
      Event_Wait(1);
      LunpaFortress_FlipHiddenSwitch(17, 82,
                                     LunpaFortress_OpenFirstSwitchPassage);
      GameFlag_Set(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED);
    }
  }
}

/// LunpaFortressCave, examine trigger 51 once uncovered: searching the second
/// switch while Reveal shows it flips it, and its passage opens. "You flipped
/// the switch."
static void LunpaFortress_FlipSecondSwitch(void) {
  if (gFieldWork->revealActive != 0) {
    if (Event_GetFlag(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED) == 0) {
      FieldMessage_Show(MsgFieldFlippedSwitch, 1);
      Event_PlaySound(SE_THUMP);
      Event_Wait(1);
      LunpaFortress_FlipHiddenSwitch(3, 55,
                                     LunpaFortress_OpenSecondSwitchPassage);
      GameFlag_Set(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED);
    }
  }
}

/// LunpaFortressCave, casting Reveal: both switches show in the wall and can be
/// searched.
static void LunpaFortress_RevealSwitches(void) {
  MapGrid_CopyCells(5, 77, 1, 2, 17, 82);
  MapGrid_CopyCells(5, 77, 1, 2, 3, 55);
  MapGrid_CopyCellAttributes(15, 33, 1, 1, 17, 35);
  MapGrid_CopyCellAttributes(3, 8, 1, 1, 3, 10);
}

/// LunpaFortressCave, as Reveal ends: both switches hide again.
static void LunpaFortress_HideSwitches(void) {
  MapGrid_CopyCells(8, 77, 1, 2, 17, 82);
  MapGrid_CopyCells(8, 77, 1, 2, 3, 55);
  MapGrid_CopyCellAttributes(18, 35, 1, 1, 17, 35);
  MapGrid_CopyCellAttributes(2, 10, 1, 1, 3, 10);
}

/// LunpaFortressCave, Whirlwind on the ivy over the first switch: it blows
/// away.
static void LunpaFortress_BlowAwayFirstSwitchIvy(void) {
  GameFlag_Set(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_IVY_BLOWN_AWAY);
  GameFlag_Clear(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_UNDER_IVY);
  Import_ActorSetPosition(ACTOR_CAVE_FIRST_SWITCH_IVY, 0, 0);
}

/// LunpaFortressCave, Whirlwind on the ivy over the passage: it blows away and
/// the passage opens.
static void LunpaFortress_BlowAwayPassageIvy(void) {
  GameFlag_Set(FLAG_LUNPA_FORTRESS_PASSAGE_IVY_BLOWN_AWAY);
  LunpaFortress_OpenIvyPassage();
  Import_ActorSetPosition(ACTOR_CAVE_PASSAGE_IVY, 0, 0);
}

/// LunpaFortressCave, Whirlwind on the ivy over the second switch: it blows
/// away.
static void LunpaFortress_BlowAwaySecondSwitchIvy(void) {
  GameFlag_Set(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_IVY_BLOWN_AWAY);
  GameFlag_Clear(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_UNDER_IVY);
  Import_ActorSetPosition(ACTOR_CAVE_SECOND_SWITCH_IVY, 0, 0);
}

static void UNUSED LunpaFortress_DoNothingE(void) {}

/// Garet, Ivan and Mia step out of the leader and gather around Hammet.
static void LunpaFortress_GatherPartyAroundHammet(void) {
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(7) / 10, PIXELS(7) / 20);
  Actor_WalkTo(ACTOR_IVAN, 456, 192);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(7) / 10, PIXELS(7) / 20);
  Actor_WalkTo(ACTOR_MIA, 440, 184);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(7) / 10, PIXELS(7) / 20);
  Actor_WalkTo(ACTOR_GARET, 448, 240);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_TOADONPA_HAMMET, 0);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_HAMMET, 0);
  Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(15);
}

/// Dodonpa's lever: the room shakes, the floor opens and everyone startles.
static void LunpaFortress_ActivateLeverTrap(void) {
  MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
  Event_PlaySound(SE_RUMBLE);
  Event_Wait(80);
  Event_PlaySound(SE_STOP_ACTION_SE);
  Event_Wait(5);
  Event_PlaySound(SE_CRASH);
  MapGrid_CopyCells(16, 75, 7, 4, 26, 55);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
}

/// The camera finds Toadonpa, the monster Dodonpa has freed; it lumbers
/// forward.
static void LunpaFortress_RevealMonster(void) {
  Actor_PlaceCurrentWithinCameraBounds(ACTOR_TOADONPA_BOSS, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(60);
  Event_SetMessageId(MsgLunpaPrepareBecomeMonster);
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetSpeed(ACTOR_TOADONPA_BOSS, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_INVISIBLE_TOADONPA_BOSS_PART, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_TOADONPA_BOSS, 472, 180);
  Actor_WalkTo(ACTOR_INVISIBLE_TOADONPA_BOSS_PART, 472, 180);
  FieldCamera_FollowActor(ACTOR_TOADONPA_BOSS, 1);
  Actor_WaitForMoveById(ACTOR_TOADONPA_BOSS);
  Import_ActorSetAnimation(ACTOR_TOADONPA_BOSS, 4);
  Event_Wait(30);
}

/// The party spreads out and faces Toadonpa.
static void LunpaFortress_PositionPartyForMonster(void) {
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_IVAN, 504, 216);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_MIA, 440, 232);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(ACTOR_GARET, 480, 224);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
}

/// LunpaFortressToadonpa, step trigger 20: walking into Hammet's cell. The
/// first time, Hammet hears the party out and recognizes Ivan, then Dodonpa
/// arrives, gloats, pulls a lever and frees Toadonpa on them; after a lost
/// battle, the event goes straight to the trap. Either way, the battle follows.
static void LunpaFortress_FindHammet(void) {
  s32 messageBase;

  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_DODONPA_JUDGED) != 0)
    return;
  GameFlag_Set(FLAG_LUNPA_FORTRESS_FINDING_HAMMET);
  Event_Start();
  // A retry skips the introduction but repeats Dodonpa's trap and battle.
  if (Event_GetFlag(FLAG_LUNPA_FORTRESS_TRAP_SEEN) != 0) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
    Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_HAMMET, 0);
    LunpaFortress_GatherPartyAroundHammet();
    Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    60);
    Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_WEST, 0);
    Actor_Jump(ACTOR_TOADONPA_HAMMET, 4, 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_TOADONPA_HAMMET), 1);
    Event_Wait(30);
    Actor_SetSpeed(ACTOR_IVAN, PIXELS(7) / 10, PIXELS(7) / 20);
    Actor_WalkTo(ACTOR_IVAN, 464, 192);
    Actor_WaitForMoveById(ACTOR_IVAN);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    // Dodonpa approaches from the south, then springs the lever trap.
    Actor_SetPosition(ACTOR_TOADONPA_DODONPA, PIXELS(456), PIXELS(320));
    FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
    messageBase = MsgLunpaBackMoreGuess;
    Event_SetMessageId(messageBase);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
    Actor_WalkTo(ACTOR_TOADONPA_DODONPA, 458, 272);
    Actor_WaitForMoveById(ACTOR_TOADONPA_DODONPA);
    Actor_FaceDirection(ACTOR_TOADONPA_DODONPA, FACING_SOUTH + FACING_STEP, 0);
    Event_Wait(40);
    Actor_SetDestinationOffset(ACTOR_TOADONPA_DODONPA, -8, 8);
    Actor_WaitForMoveById(ACTOR_TOADONPA_DODONPA);
    Event_Wait(60);
    Event_PlaySound(SE_SWITCH);
    FieldMessage_Show(messageBase + 1, 1);
    Actor_SetDestinationOffset(ACTOR_TOADONPA_DODONPA, 8, -8);
    LunpaFortress_ActivateLeverTrap();
    Event_Wait(120);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Actor_StartRepeatedMotion(ACTOR_GARET, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    messageBase += 2;
    LunpaFortress_PositionPartyForMonster();
    Actor_SetAngleToward(ACTOR_TOADONPA_DODONPA, ACTOR_PARTY_LEADER, 0);
    Event_SetMessageId(messageBase);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
    LunpaFortress_RevealMonster();
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Event_Wait(1);
    Battle_SetType(BATTLE_TYPE_SCRIPTED);
    Event_SetAfterBattleDestination((s32)&SceneId_LunpaFortressToadonpa,
                                    LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 4);
  } else {
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    Event_Wait(30);
    // Ivan recognizes Hammet before Dodonpa interrupts the reunion.
    messageBase = MsgLunpaTimeEat;
    Event_SetMessageId(messageBase);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
    Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_HAMMET, 0);
    Event_Wait(140);
    Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_WEST, 0);
    Actor_Jump(ACTOR_TOADONPA_HAMMET, 4, 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_TOADONPA_HAMMET), 1);
    Event_SetMessageId(messageBase + 1);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
    Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_HAMMET, 0);
    LunpaFortress_GatherPartyAroundHammet();
    Actor_SetSpeed(ACTOR_IVAN, PIXELS(7) / 10, PIXELS(7) / 20);
    Actor_WalkTo(ACTOR_IVAN, 464, 192);
    Actor_WaitForMoveById(ACTOR_IVAN);
    Event_Wait(30);
    Event_SetMessageId(messageBase + 2);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    0);
    Event_Wait(110);
    Event_PlaySound(MUS_IVAN);
    Event_SetMessageId(messageBase + 3);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
    Actor_StartRepeatedMotion(ACTOR_TOADONPA_HAMMET, 1);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_TOADONPA_HAMMET, PIXELS(2) / 5, PIXELS(1) / 5);
    Actor_WalkTo(ACTOR_TOADONPA_HAMMET, 520, 208);
    Actor_WaitForMoveById(ACTOR_TOADONPA_HAMMET);
    Import_ActorSetAnimation(ACTOR_TOADONPA_HAMMET, 1);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_NORTHWEST + FACING_STEP,
                        0);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_SOUTH + FACING_STEP, 0);
    Event_Wait(30);
    Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_IVAN, 0);
    Event_Wait(20);
    Event_SetMessageId(messageBase + 4);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Event_Wait(120);
    Event_SetMessageId(messageBase + 5);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
    Event_Wait(25);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_TOADONPA_HAMMET, 3);
    Event_Wait(40);
    Actor_WalkTo(ACTOR_IVAN, 480, 200);
    Actor_WaitForMoveById(ACTOR_IVAN);
    Actor_TurnToFaceEachOther(ACTOR_IVAN, ACTOR_TOADONPA_HAMMET, 0);
    Event_Wait(60);
    Event_SetMessageId(messageBase + 6);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
    Event_Wait(20);
    Import_ActorSetAnimation(ACTOR_TOADONPA_HAMMET, 4);
    Event_Wait(80);
    Event_SetMessageId(messageBase + 7);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
    // Dodonpa approaches from the south, then springs the lever trap.
    Actor_SetPosition(ACTOR_TOADONPA_DODONPA, PIXELS(456), PIXELS(320));
    Event_PlaySound(MUS_SILENCE);
    Event_SetMessageId(messageBase + 8);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
    Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DODONPA, 0);
    Actor_SetAngleToward(ACTOR_IVAN, ACTOR_TOADONPA_DODONPA, 0);
    Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_DODONPA, 0);
    Event_Wait(5);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DODONPA, 0);
    Event_Wait(30);
    Event_PlaySound(MUS_OMINOUS);
    FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
    Actor_PlaceCurrentWithinCameraBounds(ACTOR_TOADONPA_DODONPA, 1);
    FieldCamera_WaitForArrival();
    Actor_SetSpeed(ACTOR_TOADONPA_DODONPA, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkTo(ACTOR_TOADONPA_DODONPA, 456, 304);
    FieldCamera_FollowActor(ACTOR_TOADONPA_DODONPA, 1);
    Actor_WaitForMoveById(ACTOR_TOADONPA_DODONPA);
    FieldCamera_FollowActor(ACTOR_GARET, 1);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Event_Wait(60);
    Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DODONPA, 0);
    Actor_StartRepeatedMotion(ACTOR_TOADONPA_HAMMET, 2);
    Event_Wait(60);
    Event_SetMessageId(messageBase + 9);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
    Actor_SetAngleToward(ACTOR_TOADONPA_DODONPA, ACTOR_TOADONPA_HAMMET, 0);
    Event_SetMessageId(messageBase + 10);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
    Event_Wait(60);
    Actor_SetAngleToward(ACTOR_TOADONPA_DODONPA, ACTOR_IVAN, 0);
    Event_Wait(30);
    Event_SetMessageId(messageBase + 11);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
    Actor_TurnToFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    Event_Wait(60);
    Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DODONPA, 0);
    Actor_SetAngleToward(ACTOR_IVAN, ACTOR_TOADONPA_DODONPA, 0);
    Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_DODONPA, 0);
    Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
    Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 1);
    Event_Wait(60);
    Event_SetMessageId(messageBase + 12);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Event_Wait(60);
    Import_ActorSetAnimation(ACTOR_TOADONPA_DODONPA, 4);
    Event_SetMessageId(messageBase + 13);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
    Actor_WalkTo(ACTOR_GARET, 456, 248);
    Actor_WaitForMoveById(ACTOR_GARET);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    Event_SetMessageId(messageBase + 14);
    Event_ShowMessageAndWait(ACTOR_GARET, 0);
    Actor_WalkTo(ACTOR_IVAN, 472, 216);
    Actor_WaitForMoveById(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Event_Wait(10);
    Event_SetMessageId(messageBase + 15);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Event_Wait(60);
    Event_SetMessageId(messageBase + 16);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
    Actor_WalkTo(ACTOR_MIA, 440, 216);
    Actor_WaitForMoveById(ACTOR_MIA);
    Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(10);
    Event_SetMessageId(messageBase + 17);
    Event_ShowMessageAndWait(ACTOR_MIA, 0);
    Import_ActorSetAnimation(ACTOR_TOADONPA_DODONPA, 4);
    Event_SetMessageId(messageBase + 18);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Event_Wait(60);
    Event_SetMessageId(messageBase + 19);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Event_Wait(60);
    Event_SetMessageId(messageBase + 20);
    Event_ShowMessageAndWait(ACTOR_GARET, 0);
    Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 1);
    Event_Wait(60);
    Event_SetMessageId(messageBase + 21);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
    Actor_WalkTo(ACTOR_TOADONPA_DODONPA, 456, 280);
    Actor_WaitForMoveById(ACTOR_TOADONPA_DODONPA);
    Actor_FaceDirection(ACTOR_TOADONPA_DODONPA, FACING_SOUTH + FACING_STEP, 0);
    Event_Wait(80);
    Actor_SetDestinationOffset(ACTOR_TOADONPA_DODONPA, -8, 8);
    Actor_WaitForMoveById(ACTOR_TOADONPA_DODONPA);
    Event_Wait(60);
    Event_PlaySound(SE_SWITCH);
    FieldMessage_Show(MsgLunpaDodonpaPulledLever, 1);
    Actor_SetDestinationOffset(ACTOR_TOADONPA_DODONPA, 8, -8);
    Actor_SetAngleToward(ACTOR_TOADONPA_DODONPA, ACTOR_TOADONPA_BOSS, 0);
    LunpaFortress_ActivateLeverTrap();
    Event_PlaySound(MUS_BOSS_BATTLE);
    Event_SetMessageId(messageBase + 23);
    Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
    Event_Wait(60);
    Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_BOSS, 0);
    Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_BOSS, 0);
    Actor_SetAngleToward(ACTOR_IVAN, ACTOR_TOADONPA_BOSS, 0);
    Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_BOSS, 0);
    Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_BOSS, 0);
    LunpaFortress_PositionPartyForMonster();
    LunpaFortress_RevealMonster();
    GameFlag_Set(FLAG_LUNPA_FORTRESS_TRAP_SEEN);
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Event_Wait(1);
    Battle_SetType(BATTLE_TYPE_SCRIPTED);
    Event_SetAfterBattleDestination((s32)&SceneId_LunpaFortressToadonpa,
                                    LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 4);
  }
  Event_End();
}

/// LunpaFortressToadonpa, LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE after the battle
/// with Toadonpa: the monster has fallen on Dodonpa. The party may pull it off
/// him; they argue over his punishment until his father Donpa arrives, who
/// orders him locked up, thanks the party, and lets them leave with Hammet.
/// Sets FLAG_DODONPA_LOCKED_UP.
static void LunpaFortress_DonpaJudgesDodonpa(void) {
  struct Actor *leader;
  s32 aftermathMessage;
  s32 punishmentMessage;
  s32 donpaMessage;

  GameFlag_Set(FLAG_LUNPA_FORTRESS_DODONPA_JUDGED);
  GameFlag_Set(FLAG_DODONPA_LOCKED_UP);
  MapGrid_CopyCells(16, 75, 7, 4, 26, 55);
  LunpaFortress_OpenToadonpaDoor(4);
  Event_Start();
  leader = Actor_Get(ACTOR_TOADONPA_HAMMET);
  Actor_SetSpriteFlags(leader, 1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(456), PIXELS(184));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_SetPosition(ACTOR_GARET, PIXELS(440), PIXELS(192));
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_SetPosition(ACTOR_MIA, PIXELS(488), PIXELS(184));
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_SetPosition(ACTOR_IVAN, PIXELS(472), PIXELS(184));
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_SetPosition(ACTOR_TOADONPA_HAMMET, PIXELS(520), PIXELS(224));
  Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_SetPosition(ACTOR_TOADONPA_BOSS, PIXELS(448), PIXELS(237));
  Actor_FaceDirection(ACTOR_TOADONPA_BOSS, FACING_WEST, 0);
  Actor_SetPosition(ACTOR_INVISIBLE_TOADONPA_BOSS_PART, PIXELS(448),
                    PIXELS(238));
  Actor_SetSpritePriority(ACTOR_INVISIBLE_TOADONPA_BOSS_PART, 3);
  Import_ActorSetAnimation(ACTOR_TOADONPA_BOSS, 3);
  Actor_SetPosition(ACTOR_TOADONPA_DODONPA, PIXELS(458), PIXELS(243));
  Actor_FaceDirection(ACTOR_TOADONPA_DODONPA, FACING_SOUTH, 0);
  Import_ActorSetAnimation(ACTOR_TOADONPA_DODONPA, 5);
  Event_StartScreenTransition();
  Event_Wait(120);

  // The fallen monster has pinned Dodonpa; decide whether to help him.
  aftermathMessage = MsgLunpaUhnnGetOff;
  Event_SetMessageId(aftermathMessage);
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_SetMessageId((aftermathMessage + 1));
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 1);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 1);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 1);
  Event_Wait(60);
  Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(60);
  Event_SetMessageId((aftermathMessage + 2));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_HAMMET, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_HAMMET, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_TOADONPA_HAMMET, 0);
  Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(60);
  Actor_WalkTo(ACTOR_TOADONPA_HAMMET, 512, 232);
  Import_ActorSetAnimation(ACTOR_TOADONPA_HAMMET, 4);
  Event_Wait(60);
  Event_SetMessageId((aftermathMessage + 3));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(15);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 2);
  Event_SetMessageId((aftermathMessage + 4));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_DODONPA, 0);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Event_SetMessageId((aftermathMessage + 5));
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(80);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_STAND);
  Event_SetMessageId((aftermathMessage + 6));
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_WINCE, 65);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_HAMMET, 2);
  Event_Wait(100);
  Event_SetMessageId((aftermathMessage + 7));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_Wait(10);
  Event_SetMessageId((aftermathMessage + 8));
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(30);
  Event_SetMessageId((aftermathMessage + 9));
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Actor_SetAngleToward(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
  Event_SetMessageId((aftermathMessage + 10));
  FieldMessage_ShowAtActor(ACTOR_MIA, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DODONPA, 0);
    Event_Wait(60);
  } else {
    Event_SetMessageId((aftermathMessage + 11));
    Event_ShowMessageAndWait(ACTOR_MIA, 0);
    Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_HAMMET, 0);
    Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_HAMMET, 0);
    Actor_SetAngleToward(ACTOR_IVAN, ACTOR_TOADONPA_HAMMET, 0);
    Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_HAMMET, 0);
    Event_Wait(20);
    Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_PARTY_LEADER, 0);
    Event_Wait(60);
    Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DODONPA, 0);
    Event_Wait(80);
    Actor_SetAnimationAndWait(ACTOR_TOADONPA_HAMMET, 3);
    Event_Wait(30);
    Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_PARTY_LEADER, 0);
    Event_SetMessageId((aftermathMessage + 12));
    Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
    Event_Wait(60);
    Actor_SetAngleToward(ACTOR_IVAN, ACTOR_TOADONPA_HAMMET, 0);
    Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_HAMMET, 0);
    Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_HAMMET, 0);
    Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_HAMMET, 0);
    Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(80);
    Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DODONPA, 0);
    Actor_SetAngleToward(ACTOR_IVAN, ACTOR_TOADONPA_DODONPA, 0);
    Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_DODONPA, 0);
    Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
    Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DODONPA, 0);
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(ACTOR_GARET, 416, 216);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_WalkTo(ACTOR_GARET, 416, 248);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_WalkTo(ACTOR_GARET, 440, 248);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 440, 216);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_WalkTo(ACTOR_MIA, 488, 248);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_WalkTo(ACTOR_MIA, 456, 248);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_WalkTo(ACTOR_IVAN, 456, 216);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_TurnToFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_TurnToFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_Wait(100);
  Event_PlaySound(SE_LOG_PUSH);
  Import_ActorSetAnimation(ACTOR_TOADONPA_DODONPA, 7);
  // Move the party and both monster parts together to free Dodonpa.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 10, PIXELS(1) / 20);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, -24, 0);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1) / 10, PIXELS(1) / 20);
  Actor_SetDestinationOffset(ACTOR_GARET, -24, 0);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1) / 10, PIXELS(1) / 20);
  Actor_SetDestinationOffset(ACTOR_MIA, -24, 0);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1) / 10, PIXELS(1) / 20);
  Actor_SetDestinationOffset(ACTOR_IVAN, -24, 0);
  Actor_SetSpeed(ACTOR_TOADONPA_BOSS, PIXELS(1) / 10, PIXELS(1) / 20);
  Actor_SetSpeed(ACTOR_INVISIBLE_TOADONPA_BOSS_PART, PIXELS(1) / 10,
                 PIXELS(1) / 20);
  Actor_SetDestinationOffset(ACTOR_TOADONPA_BOSS, -24, 0);
  Actor_SetDestinationOffset(ACTOR_INVISIBLE_TOADONPA_BOSS_PART, -24, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Event_PlaySound(SE_STOP_ACTION_SE);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Event_Wait(60);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_Wait(100);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(7) / 10, PIXELS(7) / 20);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(7) / 10, PIXELS(7) / 20);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(7) / 10, PIXELS(7) / 20);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(7) / 10, PIXELS(7) / 20);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 456, 184);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_WalkTo(ACTOR_GARET, 464, 256);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_WalkTo(ACTOR_GARET, 480, 248);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_WalkTo(ACTOR_GARET, 440, 192);
  Actor_WaitForMoveById(ACTOR_GARET);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_WalkTo(ACTOR_MIA, 488, 248);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_WalkTo(ACTOR_MIA, 488, 184);
  Actor_WalkTo(ACTOR_IVAN, 472, 184);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Event_Wait(30);
  Event_Wait(60);
  // Debate Dodonpa's punishment before his father arrives.
  punishmentMessage = MsgLunpaAbleGet;
  Event_SetMessageId(punishmentMessage);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Import_ActorSetAnimation(ACTOR_TOADONPA_DODONPA, 6);
  Event_Wait(120);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 2);
  Event_Wait(60);
  Import_ActorSetAnimation(ACTOR_TOADONPA_DODONPA, 7);
  Event_SetMessageId((punishmentMessage + 1));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(20);
  Actor_TurnToFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_TOADONPA_DODONPA, 0);
  Event_SetMessageId((punishmentMessage + 2));
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 2);
  Event_Wait(60);
  Import_ActorSetAnimation(ACTOR_TOADONPA_DODONPA, 5);
  Event_Wait(70);
  Event_SetMessageId((punishmentMessage + 3));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
  Import_ActorSetAnimation(ACTOR_TOADONPA_DODONPA, 7);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_SetMessageId((punishmentMessage + 4));
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_HAMMET, 3);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Actor_WalkTo(ACTOR_GARET, 440, 208);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_SetMessageId((punishmentMessage + 5));
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(70);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 75);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_SetMessageId((punishmentMessage + 6));
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Event_SetMessageId((punishmentMessage + 7));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
  Actor_WalkTo(ACTOR_GARET, 448, 208);
  Actor_WaitForMoveById(ACTOR_GARET);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_HAMMET, 0);
  Event_SetMessageId((punishmentMessage + 8));
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_HAMMET, 1);
  Event_Wait(60);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(60);
  Event_SetMessageId((punishmentMessage + 9));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  Event_SetMessageId((punishmentMessage + 10));
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(30);
  Event_SetMessageId((punishmentMessage + 11));
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_MIA, 0);
  Event_Wait(20);
  Event_SetMessageId((punishmentMessage + 12));
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  Actor_SetAngleToward(ACTOR_MIA, ACTOR_GARET, 0);
  Event_SetMessageId((punishmentMessage + 13));
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
  Event_SetMessageId((punishmentMessage + 14));
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(120);
  Actor_ShowEmote(ACTOR_TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 1);
  Event_Wait(120);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_HAMMET, 1);
  Event_Wait(60);
  Event_SetMessageId((punishmentMessage + 15));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 110);
  Actor_WalkTo(ACTOR_GARET, 456, 212);
  Actor_WaitForMoveById(ACTOR_GARET);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Event_SetMessageId((punishmentMessage + 16));
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_GARET, 0);
  Event_Wait(60);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Event_SetMessageId((punishmentMessage + 17));
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(80);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_IVAN, 0);
  Event_Wait(60);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(30);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetAngleToward(ACTOR_TOADONPA_DODONPA, ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(80);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  Event_SetMessageId((punishmentMessage + 18));
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 2);
  Event_Wait(70);
  Event_SetMessageId((punishmentMessage + 19));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_SetMessageId((punishmentMessage + 20));
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Event_Wait(80);
  Event_SetMessageId((punishmentMessage + 21));
  FieldMessage_ShowAtActor(ACTOR_IVAN, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_SetAngleToward(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_PARTY_LEADER, 0);
  Actor_SetPosition(ACTOR_TOADONPA_DONPA, PIXELS(456), PIXELS(304));
  Actor_SetSpeed(ACTOR_TOADONPA_DONPA, PIXELS(1) / 2, PIXELS(1) / 4);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_PlaySound(MUS_SILENCE);
    Event_SetMessageId((punishmentMessage + 22));
    Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  } else {
    Event_PlaySound(MUS_SILENCE);
    Event_SetMessageId((punishmentMessage + 23));
    Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  }
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAngleToward(ACTOR_TOADONPA_DODONPA, ACTOR_TOADONPA_DONPA, 0);
  FieldCamera_MoveTo(PIXELS(456), -1, PIXELS(240), 1);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 1);
  Event_Wait(60);
  Event_PlaySound(MUS_COLOSSO);
  // Donpa takes charge and orders his son to remain imprisoned.
  donpaMessage = MsgLunpaDad;
  Event_SetMessageId(donpaMessage);
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
  Actor_WalkTo(ACTOR_TOADONPA_DONPA, 456, 280);
  Actor_WaitForMoveById(ACTOR_TOADONPA_DONPA);
  Actor_WalkTo(ACTOR_TOADONPA_DONPA, 440, 256);
  FieldCamera_MoveTo(PIXELS(456), -1, PIXELS(224), 1);
  Event_SetMessageId((donpaMessage + 1));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_WaitForMoveById(ACTOR_TOADONPA_DONPA);
  Actor_FaceDirection(ACTOR_TOADONPA_DONPA, FACING_NORTH + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Import_ActorSetAnimation(ACTOR_TOADONPA_DONPA, 4);
  Event_Wait(89);
  Event_SetMessageId((donpaMessage + 2));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_RunRepeatedMotion(ACTOR_TOADONPA_DODONPA, 2);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_DONPA, 3);
  Event_SetMessageId((donpaMessage + 3));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  80);
  Import_ActorSetAnimation(ACTOR_TOADONPA_DONPA, 4);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_TOADONPA_DONPA, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_Wait(20);
  Event_SetMessageId((donpaMessage + 4));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 1);
  Event_SetMessageId((donpaMessage + 5));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_DONPA, 3);
  Actor_SetAngleToward(ACTOR_TOADONPA_DONPA, ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(20);
  Event_SetMessageId((donpaMessage + 6));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 1);
  Actor_ShowEmote(ACTOR_TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Event_SetMessageId((donpaMessage + 7));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_DONPA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Event_SetMessageId((donpaMessage + 8));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_DONPA, 3);
  Event_SetMessageId((donpaMessage + 9));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_TOADONPA_DODONPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Event_Wait(80);
  Actor_SetAttachedEffect(ACTOR_TOADONPA_DODONPA, 0);
  Actor_WalkTo(ACTOR_TOADONPA_DONPA, 496, 240);
  Actor_WaitForMoveById(ACTOR_TOADONPA_DONPA);
  Actor_SetAngleToward(ACTOR_TOADONPA_DONPA, ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(20);
  Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DONPA, 0);
  Event_SetMessageId((donpaMessage + 10));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_HAMMET, 1);
  Event_SetMessageId((donpaMessage + 11));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_DONPA, 3);
  Event_Wait(20);
  Event_SetMessageId((donpaMessage + 12));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Import_ActorSetAnimation(ACTOR_TOADONPA_DONPA, 3);
  Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_SetMessageId((donpaMessage + 13));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_DONPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  Event_SetMessageId((donpaMessage + 14));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Import_ActorSetAnimation(ACTOR_TOADONPA_HAMMET, 3);
  Event_Wait(140);
  Import_ActorSetAnimation(ACTOR_TOADONPA_DONPA, 3);
  Event_Wait(120);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DONPA, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_Wait(120);
  Actor_ShowEmote(ACTOR_TOADONPA_DONPA, EMOTE_IN_FRONT | EMOTE_SMILE, 180);
  Event_SetMessageId((donpaMessage + 15));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Event_SetMessageId((donpaMessage + 16));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Actor_TurnToFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
  Event_Wait(80);
  Actor_SetAngleToward(ACTOR_TOADONPA_DONPA, ACTOR_PARTY_LEADER, 0);
  Event_Wait(60);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAngleToward(ACTOR_MIA, ACTOR_TOADONPA_DONPA, 0);
  Event_Wait(60);
  Event_SetMessageId((donpaMessage + 17));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(100);
  Event_SetMessageId((donpaMessage + 18));
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(60);
  Event_SetMessageId((donpaMessage + 19));
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Actor_SetAngleToward(ACTOR_TOADONPA_DONPA, ACTOR_IVAN, 0);
  Event_Wait(20);
  Event_SetMessageId((donpaMessage + 20));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(120);
  Import_ActorSetAnimation(ACTOR_TOADONPA_DONPA, 4);
  Event_Wait(120);
  Event_SetMessageId((donpaMessage + 21));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_DONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 90);
  Event_SetMessageId((donpaMessage + 22));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_TOADONPA_HAMMET, 3);
  Event_Wait(80);
  Import_ActorSetAnimation(ACTOR_TOADONPA_DONPA, 4);
  Event_Wait(120);
  Event_SetMessageId((donpaMessage + 23));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  60);
  Event_SetMessageId((donpaMessage + 24));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
  Actor_SetAngleToward(ACTOR_TOADONPA_DONPA, ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DONPA, 1);
  Event_Wait(50);
  Event_SetMessageId((donpaMessage + 25));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Event_Wait(70);
  Event_SetMessageId((donpaMessage + 26));
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_SetMessageId((donpaMessage + 27));
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_SetAngleToward(ACTOR_TOADONPA_DONPA, ACTOR_PARTY_LEADER, 0);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_DONPA, 3);
  Event_Wait(20);
  Event_SetMessageId((donpaMessage + 28));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 90);
  Event_SetMessageId((donpaMessage + 29));
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_TOADONPA_DONPA, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(20);
  Event_SetMessageId((donpaMessage + 30));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_HAMMET, 1);
  Event_Wait(60);
  Event_SetMessageId((donpaMessage + 31));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_HAMMET, 0);
  Actor_SetAngleToward(ACTOR_TOADONPA_DONPA, ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_DONPA, 3);
  Event_Wait(20);
  Event_SetMessageId((donpaMessage + 32));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  Event_SetMessageId((donpaMessage + 33));
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Actor_SetAngleToward(ACTOR_TOADONPA_DONPA, ACTOR_PARTY_LEADER, 0);
  Event_Wait(30);
  Event_SetMessageId((donpaMessage + 34));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  Event_SetMessageId((donpaMessage + 35));
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_DONPA, 3);
  Event_SetMessageId((donpaMessage + 36));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Event_Wait(30);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(100);
  Event_SetMessageId((donpaMessage + 37));
  FieldMessage_ShowAtActor(ACTOR_IVAN, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    Event_SetMessageId((donpaMessage + 38));
    Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  }
  Event_Wait(30);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_Wait(100);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_HAMMET, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_TOADONPA_HAMMET, 3);
  Event_Wait(100);
  // Rejoin the companions, then escort Hammet out of the fortress.
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, PIXELS(0), PIXELS(0));
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_IVAN, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, PIXELS(0), PIXELS(0));
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(ACTOR_MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, PIXELS(0), PIXELS(0));
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_TOADONPA_HAMMET, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_WalkTo(ACTOR_TOADONPA_HAMMET, 472, 184);
  Actor_WaitForMoveById(ACTOR_TOADONPA_HAMMET);
  Import_ActorSetAnimation(ACTOR_TOADONPA_HAMMET, 1);
  Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetAngleToward(ACTOR_TOADONPA_DONPA, ACTOR_PARTY_LEADER, 0);
  Import_ActorSetAnimation(ACTOR_TOADONPA_DONPA, 3);
  Event_Wait(20);
  Actor_SetAngleToward(ACTOR_TOADONPA_DONPA, ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(20);
  Event_SetMessageId((donpaMessage + 39));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DONPA, 0);
  Actor_RunRepeatedMotion(ACTOR_TOADONPA_DODONPA, 2);
  Event_SetMessageId((donpaMessage + 40));
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_TOADONPA_HAMMET, PIXELS(1), PIXELS(1) / 2);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 480, 248);
  Event_Wait(40);
  Actor_SetSpritePriority(ACTOR_TOADONPA_HAMMET, 0);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 0);
  Actor_WalkTo(ACTOR_TOADONPA_HAMMET, 480, 216);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DONPA, 0);
  Actor_WaitForMoveById(ACTOR_TOADONPA_HAMMET);
  Actor_SetAngleToward(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DONPA, 0);
  Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_SOUTHEAST + FACING_STEP, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(5);
  Import_ActorSetAnimation(ACTOR_TOADONPA_HAMMET, 3);
  Event_Wait(100);
  Import_ActorSetAnimation(ACTOR_TOADONPA_DONPA, 3);
  Event_Wait(100);
  Actor_WalkTo(ACTOR_TOADONPA_HAMMET, 480, 248);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 456, 248);
  Actor_WaitForMoveById(ACTOR_TOADONPA_HAMMET);
  Actor_WalkTo(ACTOR_TOADONPA_HAMMET, 456, 248);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 456, 360);
  Actor_WaitForMoveById(ACTOR_TOADONPA_HAMMET);
  Actor_WalkTo(ACTOR_TOADONPA_HAMMET, 456, 360);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 352, 360);
  Actor_WaitForMoveById(ACTOR_TOADONPA_HAMMET);
  Actor_WalkTo(ACTOR_TOADONPA_HAMMET, 352, 360);
  Event_Wait(20);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Event_FinishScreenTransition();
  Event_Wait(1);
  Event_Wait(210);
  Import_EventSetExitRequest(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_GATES);
  Event_End();
}

/// LunpaFortressGroundFloor: Mind Read on the second sentry reads his thoughts.
/// He has one of eight thoughts, chosen at random when the scene was entered;
/// in one he looks around first.
static void LunpaFortress_ReadSentrysThoughts(void) {

  switch (gLunpaFortressRandomPick) {
  case 0:
    Event_SetMessageId(MsgLunpaHammetGreatMerchant);
    Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 1:
    Event_SetMessageId(MsgLunpaZZZ);
    Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 2:
    Event_SetMessageId(MsgLunpaDodonpasOrdersAbsolute);
    Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 3:
    Event_SetMessageId(MsgLunpaSighBadCouldnt);
    Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 4:
    Event_SetMessageId(MsgLunpaStrangeSwearSomeone);
    Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 6:
    Event_SetMessageId(MsgLunpaToldStandGuard);
    Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 7:
    Event_SetMessageId(MsgLunpaWhoDisruptingSleep);
    Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 5:
    Actor_ArmCallback(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                      FACING_NORTH + FACING_STEP, 0);
    Event_Wait(50);
    Actor_ArmCallback(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                      FACING_NORTHWEST + FACING_STEP, 0);
    Event_Wait(50);
    Actor_ArmCallback(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                      FACING_SOUTH + FACING_STEP, 0);
    Event_Wait(50);
    Event_SetMessageId(MsgLunpaTakeCareAnybody);
    Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  }
}

/// LunpaFortressGroundFloor: the second sentry looks around, hearing someone he
/// cannot see.
static void LunpaFortress_TalkToSentry(void) {

  s32 messageBase;

  Actor_ShowEmote(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                      FACING_NORTH + FACING_STEP, 0);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                      FACING_NORTHWEST + FACING_STEP, 0);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                      FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(50);
  messageBase = MsgLunpaLeftGuardHearsSomeone;
  Event_SetMessageId(messageBase);
  Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
  Import_ActorSetAnimation(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 4);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                      FACING_NORTHWEST + FACING_STEP, 0);
  Event_Wait(40);
  Event_SetMessageId(messageBase + 1);
  Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
}

/// LunpaFortressUpperFloor: Donpa's attendant. Before the rescue the attendant
/// hushes the party, glances at the sleeping Donpa, asks a question and tells
/// of Donpa and his spoiled son; once told, only a line. After the rescue,
/// Donpa is grateful.
static void LunpaFortress_TalkToAttendant(void) {
  s32 whisperMessage;
  s32 familyMessage;

  Event_Start();
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    Event_SetMessageId(MsgLunpaDonpaGrateful);
    // BUG: the line is the attendant's, but it names Dodonpa's wife, in the
    // family room at the far side of the fortress, as the speaker: the window
    // opens at the screen's lower left, away from the attendant, where her
    // other lines open beside her.
#ifdef BUGFIX
    Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
#else
    Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_DODONPAS_WIFE, 0);
#endif
    Event_End();
  } else {
    if (GameFlag_Get(FLAG_LUNPA_FORTRESS_ATTENDANT_TOLD_OF_DONPA) != 0) {
      Event_SetMessageId(MsgLunpaMaybeDodonpasEyes);
      FieldMessage_ShowAtActor(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      Event_End();
    } else {
      Actor_ShowEmote(ACTOR_UPPER_FLOOR_ATTENDANT, EMOTE_IN_FRONT | EMOTE_WINCE,
                      30);
      Actor_SetAngleToward(ACTOR_UPPER_FLOOR_ATTENDANT, ACTOR_PARTY_LEADER, 0);
      whisperMessage = MsgLunpaShhhPleaseDont;
      Event_SetMessageId(whisperMessage);
      Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      Actor_SetAngleToward(ACTOR_UPPER_FLOOR_ATTENDANT, ACTOR_UPPER_FLOOR_DONPA,
                           0);
      Actor_PlaceCurrentWithinCameraBounds(ACTOR_UPPER_FLOOR_DONPA, 1);
      FieldCamera_WaitForArrival();
      Event_Wait(60);
      Actor_PlaceCurrentWithinCameraBounds(ACTOR_PARTY_LEADER, 1);
      Event_Wait(20);
      Actor_ShowEmote(ACTOR_UPPER_FLOOR_ATTENDANT,
                      EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      Event_SetMessageId(whisperMessage + 1);
      Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      Actor_ShowEmote(ACTOR_UPPER_FLOOR_ATTENDANT, EMOTE_IN_FRONT | EMOTE_SCOWL,
                      60);
      Event_SetMessageId(whisperMessage + 2);
      Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      Event_Wait(70);
      Actor_ShowEmote(ACTOR_UPPER_FLOOR_ATTENDANT,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
      Actor_SetAngleToward(ACTOR_UPPER_FLOOR_ATTENDANT, ACTOR_PARTY_LEADER, 0);
      Event_SetMessageId(whisperMessage + 3);
      FieldMessage_ShowAtActor(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
        Event_SetMessageId(whisperMessage + 4);
        FieldMessage_ShowAtActor(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      } else {
        Event_SetMessageId(whisperMessage + 5);
        FieldMessage_ShowAtActor(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      }
      Event_Wait(60);
      Actor_ShowEmote(ACTOR_UPPER_FLOOR_ATTENDANT,
                      EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      familyMessage = MsgLunpaDonpaKnowsCoddled;
      Event_SetMessageId(familyMessage);
      FieldMessage_ShowAtActor(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      Actor_RunRepeatedMotion(ACTOR_UPPER_FLOOR_ATTENDANT, 1);
      Event_SetMessageId(familyMessage + 1);
      FieldMessage_ShowAtActor(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      Actor_SetAnimationAndWait(ACTOR_UPPER_FLOOR_ATTENDANT, 3);
      Event_SetMessageId(familyMessage + 2);
      FieldMessage_ShowAtActor(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      GameFlag_Set(FLAG_LUNPA_FORTRESS_ATTENDANT_TOLD_OF_DONPA);
      Event_End();
    }
  }
}

/// LunpaFortressUpperFloor: Mind Read on Donpa's attendant.
static void LunpaFortress_ReadAttendantsThoughts(void) {
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP)) {
    Event_SetMessageId(MsgLunpaDifficultDonpaRight);
    Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
  } else {
    Event_SetMessageId(MsgLunpaSomeonePunishDodonpa);
    Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
  }
}

/// LunpaFortressUpperFloor: Mind Read on Donpa.
static void LunpaFortress_ReadDonpasThoughts(void) {
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP)) {
    Event_SetMessageId(MsgLunpaFatherStayAngry);
    Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_DONPA, 0);
  } else {
    Event_SetMessageId(MsgLunpaFatherSorryDodonpa);
    Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_DONPA, 0);
  }
}

/// LunpaFortressUpperFloor: Donpa, asleep before the rescue. "ZZZZ..." After
/// it, the first time, he thanks the party and has his attendant show them his
/// reward, Tonic.
static void LunpaFortress_TalkToDonpa(void) {
  s32 rewardMessage;

  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    if (GameFlag_Get(FLAG_LUNPA_FORTRESS_REWARD_TAKEN) == 0 &&
        Event_GetFlag(FLAG_LUNPA_FORTRESS_REWARD_SHOWN) == 0) {
      rewardMessage = MsgLunpaThankHelpDodonpa;
      Event_SetMessageId(rewardMessage);
      Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_DONPA, 0);
      Actor_RunRepeatedMotion(ACTOR_UPPER_FLOOR_DONPA, 1);
      Event_Wait(30);
      Actor_SetSpeed(ACTOR_UPPER_FLOOR_DONPA, PIXELS(1) / 10, PIXELS(1) / 20);
      Actor_SetDestinationOffset(ACTOR_UPPER_FLOOR_DONPA, -4, 0);
      Actor_WaitForMoveById(ACTOR_UPPER_FLOOR_DONPA);
      Import_ActorSetAnimation(ACTOR_UPPER_FLOOR_DONPA, 3);
      Event_Wait(60);
      Actor_SetSpeed(ACTOR_UPPER_FLOOR_DONPA, PIXELS(1) / 5, PIXELS(1) / 10);
      Actor_SetDestinationOffset(ACTOR_UPPER_FLOOR_DONPA, -6, 0);
      Actor_SetAngleToward(ACTOR_UPPER_FLOOR_DONPA, ACTOR_PARTY_LEADER, 0);
      Actor_WaitForMoveById(ACTOR_UPPER_FLOOR_DONPA);
      Event_SetMessageId(rewardMessage + 1);
      Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_DONPA, 0);
      Actor_RunRepeatedMotion(ACTOR_UPPER_FLOOR_DONPA, 1);
      Actor_SetAngleToward(ACTOR_UPPER_FLOOR_ATTENDANT, ACTOR_UPPER_FLOOR_DONPA,
                           0);
      Event_SetMessageId(rewardMessage + 2);
      Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_DONPA, 0);
      Event_Wait(70);
      Import_ActorSetAnimation(ACTOR_UPPER_FLOOR_ATTENDANT, 3);
      Event_Wait(60);
      Actor_SetSpeed(ACTOR_UPPER_FLOOR_ATTENDANT, PIXELS(2) / 5, PIXELS(1) / 5);
      Actor_WalkTo(ACTOR_UPPER_FLOOR_ATTENDANT, 880, 112);
      Actor_WaitForMoveById(ACTOR_UPPER_FLOOR_ATTENDANT);
      Actor_FaceDirection(ACTOR_UPPER_FLOOR_ATTENDANT,
                          FACING_NORTH + FACING_STEP, 0);
      Event_SetMessageId(rewardMessage + 3);
      Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_DONPA, 0);
      Import_ActorSetAnimation(ACTOR_UPPER_FLOOR_DONPA, 3);
      Event_Wait(70);
      Actor_SetDestinationOffset(ACTOR_UPPER_FLOOR_DONPA, 8, 0);
      Actor_WaitForMoveById(ACTOR_UPPER_FLOOR_DONPA);
      Import_ActorSetAnimation(ACTOR_UPPER_FLOOR_DONPA, 5);
      Event_SetMessageId(rewardMessage + 4);
      Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_DONPA, 0);
      Actor_WalkTo(ACTOR_PARTY_LEADER, 896, 120);
      Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
      Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_UPPER_FLOOR_ATTENDANT,
                                0);
      Event_Wait(60);
      Import_ActorSetAnimation(ACTOR_UPPER_FLOOR_ATTENDANT, 3);
      Event_Wait(30);
      GameFlag_Set(FLAG_LUNPA_FORTRESS_REWARD_SHOWN);
    } else {
      Event_SetMessageId(MsgLunpaWellWorriedDodonpa);
      Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_DONPA, 0);
    }
  } else {
    Event_SetMessageId(MsgLunpaZZZZ);
    Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_DONPA, 0);
  }
}

/// LunpaFortressUpperFloor: talking to Tonic, a Mercury Djinni and Donpa's
/// reward, it joins the party.
static void LunpaFortress_DjinniJoins(void) {
  Djinn_JoinParty(ACTOR_UPPER_FLOOR_TONIC, 1, 5);
  GameFlag_Set(FLAG_LUNPA_FORTRESS_REWARD_TAKEN);
}

/// LunpaFortressUpperFloor: the east of the three beaten guards begs. "Owww!
/// Hey! Don't hurt me again..."
static void LunpaFortress_TalkToBeatenEastGuardOfThree(void) {
  Actor_RunRepeatedMotion(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 2);
  Event_SetMessageId(MsgLunpaOwwwDontHurt);
  Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 0);
}

/// LunpaFortressUpperFloor: the middle of the three beaten guards gives up, and
/// Hammet is in the cave. "We give up!"
static void LunpaFortress_TalkToBeatenMiddleGuardOfThree(void) {
  Actor_RunRepeatedMotion(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 2);
  Event_SetMessageId(MsgLunpaRightRightGive);
  Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 0);
}

/// LunpaFortressUpperFloor: the west of the three beaten guards marvels at the
/// party. "Who'd have thought they'd be so tough?"
static void LunpaFortress_TalkToBeatenWestGuardOfThree(void) {
  Actor_RunRepeatedMotion(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 2);
  Event_SetMessageId(MsgLunpaGuysTougherThought);
  Event_ShowMessageAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 0);
}

/// LunpaFortressUpperFloor: one of Dodonpa's family asks where he is. "Do you
/// know where Dodonpa is?"
static void LunpaFortress_AskWhereDodonpaIs(void) {
  Event_SetMessageId(MsgLunpaKnowWhereDodonpa);
  Event_AskYesNoAndShowResponse(ACTOR_UPPER_FLOOR_DODONPAS_WIFE, 0);
}

/// LunpaFortressGroundFloor: a bent old man asks whether the party sneaked in.
/// He's startled the first time, and wants to know whether it came in search of
/// something; on yes, he promises to keep quiet. "Wh-Wh-Who goes there!"
static void LunpaFortress_TalkToStartledOldMan(void) {
  s32 frightMessage;
  s32 purposeMessage;

  if (Event_GetFlag(FLAG_LUNPA_FORTRESS_OLD_MAN_PROMISED_SILENCE) != 0) {
    Event_SetMessageId(MsgLunpaWontTellAnyone);
    Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
  } else {
    Event_Start();
    Actor_SetAngleToward(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN,
                         ACTOR_PARTY_LEADER, 0);
    if (GameFlag_Get(FLAG_LUNPA_FORTRESS_OLD_MAN_STARTLED) == 0) {
      Actor_Jump(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 4, 0);
      Actor_Stop(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN);
      Actor_RefreshSelectorById(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN);
      Event_Wait(20);
      frightMessage = MsgLunpaWhWhWho;
      Event_SetMessageId(frightMessage);
      Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
      Actor_ShowEmote(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN,
                      EMOTE_IN_FRONT | EMOTE_WINCE, 30);
      Event_SetMessageId(frightMessage + 1);
      Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
      Event_Wait(30);
      Import_ActorSetAnimation(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 4);
      Event_Wait(30);
    }
    purposeMessage = MsgLunpaDontLookNearly;
    Event_SetMessageId(purposeMessage);
    Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
    Actor_ShowEmote(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN,
                    EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Event_SetMessageId(purposeMessage + 1);
    FieldMessage_ShowAtActor(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
      Event_SetMessageId(purposeMessage + 2);
      FieldMessage_ShowAtActor(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
      GameFlag_Set(FLAG_LUNPA_FORTRESS_OLD_MAN_PROMISED_SILENCE);
    } else {
      Event_SetMessageId(purposeMessage + 3);
      FieldMessage_ShowAtActor(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
    }
    GameFlag_Set(FLAG_LUNPA_FORTRESS_OLD_MAN_STARTLED);
    Event_End();
  }
}

/// LunpaFortressGroundFloor: Mind Read on the startled resident. Once they have
/// promised, a thought about Hammet; before, Mind Read stops and they talk
/// instead.
static void LunpaFortress_ReadStartledResidentsThoughts(void) {

  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_OLD_MAN_PROMISED_SILENCE)) {
    Event_SetMessageId(MsgLunpaMaybeMerchantReason);
    Event_ShowMessageAndWait(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
  } else {
    s16 *psynergyRequest = &gFieldWork->psynergy;

    *psynergyRequest = 0;
    FieldPsynergy_FinishWaveAndReleaseHeapBlock();
    LunpaFortress_TalkToStartledOldMan();
  }
}

static void UNUSED LunpaFortress_DoNothingF(void) {}

/// LunpaFortressToadonpa, after Donpa's judgment: Dodonpa behind bars begs his
/// father's forgiveness; with the Cell Key, a further line shows.
static void LunpaFortress_TalkToJailedDodonpa(void) {
  s32 pleaMessage = MsgLunpaWrongTurnOver;

  Event_SetMessageId(pleaMessage);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_TOADONPA_DODONPA, 0);
  if (PartyInventory_FindMember(ITEM_CELL_KEY) != -1) {
    FieldMessage_Show(pleaMessage + 2, 1);
  }
}

/// LunpaFortressToadonpa: Mind Read on the jailed Dodonpa.
static void LunpaFortress_ReadJailedDodonpasThoughts(void) {
  Event_SetMessageId(MsgLunpaCantBelieveWhen);
  Event_ShowMessageAndWait(ACTOR_TOADONPA_DODONPA, 0);
}

/// Sets each scene up: a random pick for the guards' lines, the doors already
/// opened, the watching guards and, in LunpaFortressToadonpa, the room after
/// the battle with Toadonpa or Dodonpa jailed.
s32 Scene_Initialize(void) {
  struct Actor *mapActor;

  gLunpaFortressRandomPick = (u32)Import_RandomNext() * 7 >> 16;
  if (gMainRoutineWork.sceneId == (s32)&SceneId_LunpaFortressGroundFloor) {
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    LunpaFortress_InitializeGroundFloor();
  }
  if (gMainRoutineWork.sceneId == (s32)&SceneId_LunpaFortressUpperFloor)
    LunpaFortress_InitializeUpperFloor();
  if (gMainRoutineWork.sceneId == (s32)&SceneId_LunpaFortressCave)
    LunpaFortress_InitializeCave();
  if (gMainRoutineWork.sceneId == (s32)&SceneId_LunpaFortressToadonpa) {
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_TOADONPA_HAMMET), 0);
    Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_EAST, 0);
    Import_ActorSetAnimation(ACTOR_TOADONPA_HAMMET, 0);
    LunpaFortress_RestoreToadonpaDoors();
    mapActor = Actor_Get(ACTOR_INVISIBLE_TOADONPA_WEST_JAR);
    if (mapActor != 0)
      Actor_SetSpriteFlags(mapActor, 0);
    mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    mapActor = Actor_Get(ACTOR_INVISIBLE_TOADONPA_CHEST);
    if (mapActor != 0)
      Actor_SetSpriteFlags(mapActor, 0);
    mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    mapActor = Actor_Get(ACTOR_INVISIBLE_TOADONPA_EAST_JAR);
    if (mapActor != 0)
      Actor_SetSpriteFlags(mapActor, 0);
    mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    if (gMainRoutineWork.entrance == LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE) {
      MapGrid_SetShownLayers(LUNPA_FORTRESS_ROOM_LAYERS);
      LunpaFortress_DonpaJudgesDodonpa();
    }
    if (gMainRoutineWork.entrance ==
        LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHWEST) {
      MapGrid_SetShownLayers(LUNPA_FORTRESS_ROOM_LAYERS);
      if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
        Actor_SetPosition(ACTOR_TOADONPA_HAMMET, PIXELS(0), PIXELS(0));
        Actor_SetPosition(ACTOR_TOADONPA_CELL_GUARD, PIXELS(432), PIXELS(344));
        Actor_FaceDirection(ACTOR_TOADONPA_CELL_GUARD,
                            FACING_SOUTH + FACING_STEP, 0);
        Actor_SetPosition(ACTOR_TOADONPA_DODONPA, PIXELS(456), PIXELS(288));
        Actor_FaceDirection(ACTOR_TOADONPA_DODONPA, FACING_SOUTH + FACING_STEP,
                            0);
        Actor_SetPosition(ACTOR_INVISIBLE_TOADONPA_DODONPA, PIXELS(456),
                          PIXELS(320));
        Actor_SetSpriteFlags(Actor_Get(ACTOR_INVISIBLE_TOADONPA_DODONPA), 0);
      }
    }
    mapActor = Actor_Get(ACTOR_INVISIBLE_TOADONPA_BOSS_PART);
    if (mapActor != 0)
      Actor_SetSpriteFlags(mapActor, 0);
    mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    mapActor->scaleX = 4 * Q16_ONE / 5;
  }
  return 0;
}

/// Sets LunpaFortressGroundFloor up by entrance: the guards that watch
/// each part, and the cell doors already opened.
static void LunpaFortress_InitializeGroundFloor(void) {
  LunpaFortress_RestoreGroundFloorDoors();
  switch (gMainRoutineWork.entrance) {
  case LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_FROM_MAIN_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_ENTRANCE:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTH_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_FEAST_ROOM:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTHEAST_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_EAST_PASSAGE:
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Task_AddFrame(LunpaFortress_CheckEntranceGuard, TASK_PASS_FRAME);
    Task_AddFrame(LunpaFortress_CheckFirstEntranceSentry, TASK_PASS_FRAME);
    Task_AddFrame(LunpaFortress_CheckSecondEntranceSentry, TASK_PASS_FRAME);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    break;
  case LUNPA_FORTRESS_GROUND_FLOOR_FEAST_ROOM_FROM_MAIN_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_BARREL_ROOM_FROM_NORTHEAST_PASSAGE:
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_ROOM_LAYERS);
    break;
  case LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_MAIN_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_NORTHEAST_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_TREASURE_ROOM:
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Task_AddFrame(LunpaFortress_UpdateGroundFloorPatrolDetection,
                  TASK_PASS_FRAME);
    Task_AddFrame(LunpaFortress_CheckGroundFloorDoorApproach, TASK_PASS_FRAME);
    wait(1);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
    MapGrid_CopyCells(101, 9, 10, 8, 110, 9);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    break;
  case LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_MAIN_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_EAST_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_BARREL_ROOM:
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Task_AddFrame(LunpaFortress_CheckUpperHallGuard, TASK_PASS_FRAME);
    break;
  default:
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    break;
  }
  Actor_ConfigureSpriteColor(ACTOR_GROUND_FLOOR_PATROL_GUARD, SPRITE_COLOR_GRB);
  Actor_ConfigureSpriteColor(ACTOR_GROUND_FLOOR_UPPER_HALL_GUARD,
                             SPRITE_COLOR_GRB);
  Actor_ConfigureSpriteColor(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                             SPRITE_COLOR_GRB);
  Actor_ConfigureSpriteColor(ACTOR_GROUND_FLOOR_ENTRANCE_GUARD,
                             SPRITE_COLOR_GRB);
  Actor_ConfigureSpriteColor(ACTOR_GROUND_FLOOR_FIRST_SENTRY, SPRITE_COLOR_GRB);
  wait(1);
}

/// Sets LunpaFortressUpperFloor up: beaten guards gone or lying where they
/// fell, the Cell Key gone once taken, the patrols by entrance and
/// Donpa's attendant by his reward's place.
static void LunpaFortress_InitializeUpperFloor(void) {
  struct Actor *keyActor;

  LunpaFortress_RestoreUpperFloorDoors();
  Actor_ConfigureSpriteColor(ACTOR_UPPER_FLOOR_FIRST_PATROL, SPRITE_COLOR_GRB);
  Actor_ConfigureSpriteColor(ACTOR_UPPER_FLOOR_SECOND_PATROL, SPRITE_COLOR_GRB);
  Actor_ConfigureSpriteColor(ACTOR_UPPER_FLOOR_HALL_PATROL, SPRITE_COLOR_GRB);
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN)) {
    Actor_SetPosition(ACTOR_UPPER_FLOOR_WEST_GUARD, PIXELS(0), PIXELS(0));
  }
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_MIDDLE_GUARD_BEATEN)) {
    Actor_SetPosition(ACTOR_UPPER_FLOOR_HALL_GUARD, PIXELS(0), PIXELS(0));
  }
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN)) {
    Actor_SetPosition(ACTOR_UPPER_FLOOR_EAST_GUARD, PIXELS(0), PIXELS(0));
  }
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_CELL_KEY)) {
    Actor_SetPosition(ACTOR_UPPER_FLOOR_CELL_KEY, PIXELS(0), PIXELS(0));
  }
  switch (gMainRoutineWork.entrance) {
  case LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH:
  case LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_NORTH_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_MIDDLE_PASSAGE:
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    Task_AddFrame(LunpaFortress_UpdatePairedGuardDetection, TASK_PASS_FRAME);
    wait(1);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_FROM_MIDDLE_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_FROM_NORTH_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_SOUTH_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_FROM_DONPAS_ROOM:
  case LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_TONIC_ROOM:
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_ROOM_LAYERS);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_UPPER_FLOOR_DONPA), 0);
    if (GameFlag_Get(FLAG_LUNPA_FORTRESS_REWARD_SHOWN)) {
      Actor_SetPosition(ACTOR_UPPER_FLOOR_ATTENDANT, PIXELS(872), PIXELS(120));
    }
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_CELL_KEY_ROOM:
  case LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_SOUTH_PASSAGE:
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    Task_AddFrame(LunpaFortress_UpdateHallGuardDetection, TASK_PASS_FRAME);
    wait(1);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_NORTH_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_SOUTH_PASSAGE:
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    if (GameFlag_Get(FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN)) {
      LunpaFortress_PlaceThreeBeatenGuards();
    }
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_AFTER_BATTLE:
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, LUNPA_RETURN_TRANSITION_VARIANT);
    LunpaFortress_PlaceThreeBeatenGuards();
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_MIDDLE_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_EAST_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_NORTH:
    Task_AddFrame(LunpaFortress_CheckUpperFloorDoorApproach, TASK_PASS_FRAME);
    break;
  default:
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    break;
  }
  keyActor = Actor_Get(ACTOR_UPPER_FLOOR_CELL_KEY);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_UPPER_FLOOR_CELL_KEY), 0);
  Actor_SetSpritePriority(ACTOR_UPPER_FLOOR_CELL_KEY, 1);
  keyActor->scaleX = 3 * Q16_ONE / 4;
  keyActor->scaleY = 3 * Q16_ONE / 4;
}

/// Sets LunpaFortressCave up: the doors, the crates, the ivy Whirlwind blows
/// away and the switches as the party left them.
static void LunpaFortress_InitializeCave(void) {
  struct Actor *mapActor;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  LunpaFortress_RestoreCaveDoors();
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_CRATES_MOVED)) {
    LunpaFortress_RestoreLoweredCrates();
  }
  Event_SetFlag(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_UNDER_IVY);
  Event_SetFlag(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_UNDER_IVY);
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_IVY_BLOWN_AWAY)) {
    Actor_SetPosition(ACTOR_CAVE_FIRST_SWITCH_IVY, PIXELS(0), PIXELS(0));
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_UNDER_IVY);
  }
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_PASSAGE_IVY_BLOWN_AWAY)) {
    Actor_SetPosition(ACTOR_CAVE_PASSAGE_IVY, PIXELS(0), PIXELS(0));
    LunpaFortress_OpenIvyPassage();
  }
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_IVY_BLOWN_AWAY)) {
    Actor_SetPosition(ACTOR_CAVE_SECOND_SWITCH_IVY, PIXELS(0), PIXELS(0));
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_UNDER_IVY);
  }
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED)) {
    LunpaFortress_OpenFirstSwitchPassage();
  }
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED)) {
    LunpaFortress_OpenSecondSwitchPassage();
  }
  Event_Start();
  mapActor = Actor_Get(ACTOR_CAVE_FIRST_SWITCH_IVY);
  if (mapActor != 0) {
    mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  mapActor = Actor_Get(ACTOR_CAVE_PASSAGE_IVY);
  if (mapActor != 0) {
    mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  mapActor = Actor_Get(ACTOR_CAVE_SECOND_SWITCH_IVY);
  if (mapActor != 0) {
    mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  mapActor = Actor_Get(ACTOR_INVISIBLE_CAVE_JAR);
  if (mapActor != 0) {
    Actor_SetSpriteFlags(mapActor, 0);
  }
  mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  mapActor = Actor_Get(ACTOR_CAVE_CRATES);
  if (mapActor != 0) {
    mapActor->collisionFlags |= ACTOR_NEAR_PSYNERGY;
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_INVISIBLE_CAVE_JAR), 0);
  Event_End();
  MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
}

/// LunpaFortressGroundFloor's opened cell doors stay open.
static void LunpaFortress_RestoreGroundFloorDoors(void) {
  if (GameFlag_Get(GROUND_FLOOR_DOOR_FLAG)) {
    LunpaFortress_OpenGroundFloorDoor(0);
  }
  if (GameFlag_Get(GROUND_FLOOR_DOOR_FLAG + 1)) {
    LunpaFortress_OpenGroundFloorDoor(1);
  }
  if (GameFlag_Get(GROUND_FLOOR_DOOR_FLAG + 2)) {
    LunpaFortress_OpenGroundFloorDoor(2);
  }
}

/// LunpaFortressUpperFloor's opened cell doors stay open.
static void LunpaFortress_RestoreUpperFloorDoors(void) {
  if (GameFlag_Get(UPPER_FLOOR_DOOR_FLAG)) {
    LunpaFortress_OpenUpperFloorDoor(0);
  }
  if (GameFlag_Get(UPPER_FLOOR_DOOR_FLAG + 1)) {
    LunpaFortress_OpenUpperFloorDoor(1);
  }
}

/// LunpaFortressCave's opened cell doors stay open.
static void LunpaFortress_RestoreCaveDoors(void) {
  if (GameFlag_Get(CAVE_DOOR_FLAG)) {
    LunpaFortress_OpenCaveDoor(0);
  }
  if (GameFlag_Get(CAVE_DOOR_FLAG + 1)) {
    LunpaFortress_OpenCaveDoor(1);
  }
  if (GameFlag_Get(CAVE_DOOR_FLAG + 2)) {
    LunpaFortress_OpenCaveDoor(2);
  }
}

/// LunpaFortressToadonpa's opened cell doors stay open.
static void LunpaFortress_RestoreToadonpaDoors(void) {
  if (GameFlag_Get(TOADONPA_DOOR_FLAG)) {
    LunpaFortress_OpenToadonpaDoor(0);
  }
  if (GameFlag_Get(TOADONPA_DOOR_FLAG + 1)) {
    LunpaFortress_OpenToadonpaDoor(1);
  }
  if (GameFlag_Get(TOADONPA_DOOR_FLAG + 2)) {
    LunpaFortress_OpenToadonpaDoor(2);
  }
  if (GameFlag_Get(TOADONPA_DOOR_FLAG + 3)) {
    LunpaFortress_OpenToadonpaDoor(3);
  }
  if (GameFlag_Get(TOADONPA_DOOR_FLAG + 4)) {
    LunpaFortress_OpenToadonpaDoor(4);
  }
}

/// Pixel steps pack X above Z in two halfwords. Unsigned packing also defines
/// the entries with negative X steps.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((u32)(dz) & 0xffff))

// The ROM keeps these three tables in .rodata, but the shared declarations in
// field.h and staged_actor.h lack const, so a section attribute
// places each one.
s32 gPushedActor_DirectionSteps[] __attribute__((section(".rodata"))) = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedActor_FootprintKinds[] __attribute__((section(".rodata"))) = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedActor_FootprintBounds[] __attribute__((section(".rodata"))) = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gLunpaFortressRandomPick = 0;

s32 gSecondPatrolRoute[] = {
    SC_SET(SW_ACCELERATION, PIXELS(11) / 10),
    SC_SET(SW_SPEED, PIXELS(11) / 10),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(192), 0, PIXELS(288)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(88), 0, PIXELS(288)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gFirstPatrolRoute[] = {
    SC_SET(SW_ACCELERATION, PIXELS(11) / 10),
    SC_SET(SW_SPEED, PIXELS(11) / 10),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(192), 0, PIXELS(192)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(88), 0, PIXELS(192)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gDonpaSleeps[] = {
    SC_ANIMATION(5),
    SC_END,
};

s32 gGroundFloorPatrolRoute[] = {
    SC_SET(SW_ACCELERATION, PIXELS(1) / 5),
    SC_SET(SW_SPEED, PIXELS(3) / 5),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(25),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(520)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(712), 0, PIXELS(520)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(25),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(520)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(40),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(744), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gHallPatrolRoute[] = {
    SC_SET(SW_ACCELERATION, PIXELS(1) / 5),
    SC_SET(SW_SPEED, PIXELS(3) / 5),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(920), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(592)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(920), 0, PIXELS(648)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(920), 0, PIXELS(592)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gLunpaFortressGroundFloorEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_GATES, CONDITION_ALWAYS, 160, 0, 168, FACING_NORTH, 0, 40, 24, 296, 280, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_FROM_MAIN_PASSAGE, CONDITION_ALWAYS, 160, 0, 104, FACING_SOUTH, 0, 40, 24, 296, 280, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_ENTRANCE, CONDITION_ALWAYS, 272, 0, 896, FACING_NORTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTH_PASSAGE, CONDITION_ALWAYS, 376, 0, 896, FACING_NORTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_FEAST_ROOM, CONDITION_ALWAYS, 248, 0, 856, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTHEAST_PASSAGE, CONDITION_ALWAYS, 488, 0, 680, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_EAST_PASSAGE, CONDITION_ALWAYS, 72, 0, 696, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH, CONDITION_ALWAYS, 96, 0, 856, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_NORTH_PASSAGE_FROM_MAIN_PASSAGE, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 328, 8, 568, 232, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_UPPER_FLOOR_NORTH, CONDITION_ALWAYS, 504, 0, 88, FACING_SOUTH, 0, 328, 8, 568, 232, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_CAVE, CONDITION_ALWAYS, 440, 0, 104, FACING_SOUTH, 0, 328, 8, 568, 232, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_FEAST_ROOM_FROM_MAIN_PASSAGE, CONDITION_ALWAYS, 136, 0, 512, FACING_NORTH, 0, 40, 328, 280, 552, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_MAIN_PASSAGE, CONDITION_ALWAYS, 888, 0, 280, FACING_NORTH, 0, 632, 40, 952, 328, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_EAST_PASSAGE, CONDITION_ALWAYS, 696, 0, 264, FACING_SOUTH, 0, 632, 40, 952, 328, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_BARREL_ROOM, CONDITION_ALWAYS, 824, 0, 120, FACING_SOUTH, 0, 632, 40, 952, 328, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_MAIN_PASSAGE, CONDITION_ALWAYS, 664, 0, 632, FACING_NORTH, 0, 616, 344, 936, 696, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_NORTHEAST_PASSAGE, CONDITION_ALWAYS, 888, 0, 520, FACING_NORTH, 0, 616, 344, 936, 696, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_TREASURE_ROOM, CONDITION_ALWAYS, 696, 0, 432, FACING_SOUTH, 0, 616, 344, 936, 696, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_BARREL_ROOM_FROM_NORTHEAST_PASSAGE, CONDITION_ALWAYS, 440, 0, 448, FACING_NORTH, 0, 320, 312, 560, 504, 0 },
    { LUNPA_FORTRESS_GROUND_FLOOR_TREASURE_ROOM_FROM_EAST_PASSAGE, CONDITION_ALWAYS, 696, 0, 824, FACING_NORTH, 0, 616, 744, 856, 952, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gLunpaFortressUpperFloorEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH, CONDITION_ALWAYS, 136, 0, 392, FACING_SOUTH, 0, 40, 8, 308, 448, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_NORTH_PASSAGE, CONDITION_ALWAYS, 136, 0, 80, FACING_SOUTH, 0, 40, 8, 308, 448, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_MIDDLE_PASSAGE, CONDITION_ALWAYS, 264, 0, 416, FACING_SOUTH, 0, 40, 8, 308, 448, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_WEST_PASSAGE, CONDITION_ALWAYS, 376, 0, 232, FACING_NORTH, 0, 328, 8, 568, 272, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_GUARD_ROOM, CONDITION_ALWAYS, 504, 0, 104, FACING_NORTH, 0, 328, 8, 568, 272, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_WOMENS_ROOM, CONDITION_ALWAYS, 376, 0, 176, FACING_SOUTH, 0, 328, 8, 568, 272, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_WEST_PASSAGE, CONDITION_ALWAYS, 408, 0, 456, FACING_NORTH, 0, 328, 272, 584, 512, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_FAMILY_ROOM, CONDITION_ALWAYS, 456, 0, 352, FACING_SOUTH, 0, 328, 272, 584, 512, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_CELL_KEY_ROOM, CONDITION_ALWAYS, 504, 0, 456, FACING_NORTH, 0, 328, 272, 584, 512, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_FROM_MIDDLE_PASSAGE, CONDITION_ALWAYS, 136, 0, 640, FACING_NORTH, 0, 16, 472, 256, 688, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_NORTH_PASSAGE, CONDITION_ALWAYS, 648, 0, 72, FACING_SOUTH, 0, 568, 16, 808, 176, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_SOUTH_PASSAGE, CONDITION_ALWAYS, 744, 0, 80, FACING_SOUTH, 0, 568, 16, 808, 176, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_FROM_NORTH_PASSAGE, CONDITION_ALWAYS, 696, 0, 376, FACING_NORTH, 0, 576, 264, 816, 432, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_MIDDLE_PASSAGE, CONDITION_ALWAYS, 200, 0, 848, FACING_SOUTH, 0, 152, 680, 424, 992, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_EAST_PASSAGE, CONDITION_ALWAYS, 264, 0, 776, FACING_SOUTH, 0, 152, 680, 424, 992, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_NORTH, CONDITION_ALWAYS, 376, 0, 824, FACING_SOUTH, 0, 152, 680, 424, 992, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_GUARD_ROOM, CONDITION_ALWAYS, 536, 0, 696, FACING_NORTH, 0, 488, 584, 760, 896, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_DONPAS_ROOM, CONDITION_ALWAYS, 680, 0, 832, FACING_SOUTH, 0, 488, 584, 760, 896, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_EAST_PASSAGE, CONDITION_ALWAYS, 712, 0, 856, FACING_NORTH, 0, 488, 584, 760, 896, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_SOUTH_PASSAGE, CONDITION_ALWAYS, 912, 0, 192, FACING_NORTH, 0, 784, 24, 1024, 240, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_CELL_KEY_ROOM, CONDITION_ALWAYS, 840, 0, 664, FACING_NORTH, 0, 768, 440, 1008, 704, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_SOUTH_PASSAGE, CONDITION_ALWAYS, 952, 0, 528, FACING_SOUTH, 0, 768, 440, 1008, 704, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_FROM_DONPAS_ROOM, CONDITION_ALWAYS, 872, 0, 376, FACING_NORTH, 0, 768, 264, 1016, 432, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_TONIC_ROOM, CONDITION_ALWAYS, 904, 0, 112, FACING_SOUTH, 0, 784, 24, 1024, 240, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_UNUSED, CONDITION_ALWAYS, 408, 0, 456, FACING_NORTH, 0, 328, 272, 584, 512, 0 },
    { LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_AFTER_BATTLE, CONDITION_ALWAYS, 648, 0, 88, FACING_NORTH, 0, 568, 16, 808, 176, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gLunpaFortressCaveEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_GROUND_FLOOR, CONDITION_ALWAYS, 424, 0, 584, FACING_SOUTH, 0, 200, 488, 480, 648, 0 },
    { LUNPA_FORTRESS_CAVE_ENTRANCE_FROM_LAKE, CONDITION_ALWAYS, 344, 0, 568, FACING_SOUTH, 0, 200, 488, 480, 648, 0 },
    { LUNPA_FORTRESS_CAVE_LAKE_FROM_ENTRANCE, CONDITION_ALWAYS, 312, 0, 448, FACING_NORTH, 0, 8, 8, 568, 480, 0 },
    { LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTHEAST, CONDITION_ALWAYS, 440, 0, 296, FACING_SOUTH, 0, 8, 8, 568, 480, 0 },
    { LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTH, CONDITION_ALWAYS, 312, 0, 248, FACING_SOUTH, 0, 8, 8, 568, 480, 0 },
    { LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTHWEST, CONDITION_ALWAYS, 104, 0, 184, FACING_SOUTH, 0, 8, 8, 568, 480, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gLunpaFortressEntrancesOther[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTH, CONDITION_ALWAYS, 56, 0, 328, FACING_NORTH, 0, 8, 8, 264, 424, 0 },
    { LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHEAST, CONDITION_ALWAYS, 184, 0, 368, FACING_NORTH, 0, 8, 8, 264, 424, 0 },
    { LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHWEST, CONDITION_ALWAYS, 328, 0, 344, FACING_SOUTH, 0, 288, 32, 576, 432, 0 },
    { LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE, CONDITION_ALWAYS, 440, 0, 224, FACING_NORTH, 0, 288, 32, 576, 432, 0 },
    { SCENE_TABLE_END },
};

/// LunpaFortressGroundFloor's exits. Unused: its rows first copy West Lunpa
/// Cave's, which no scene of this overlay reads.
u32 gLunpaFortressExitsOther[] = {
    SCENE_EXITS(WestLunpaCave),
    SCENE_EXIT(WEST_LUNPA_CAVE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_WEST_LUNPA_CAVE),
    SCENE_EXIT(WEST_LUNPA_CAVE_TO_LUNPA, Lunpa, LUNPA_FROM_WEST_LUNPA_CAVE),
    SCENE_EXIT(WEST_LUNPA_CAVE_BY_WAGON, Kalay, KALAY_FROM_HAMMET_PALACE_GROUNDS),
    SCENE_EXITS(LunpaFortressGroundFloor),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_GATES, LunpaFortressGates, LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_GROUND_FLOOR),
    SCENE_EXIT(EXIT_THROWN_OUT, LunpaFortressGates, LUNPA_FORTRESS_GATES_AFTER_CAUGHT),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_TO_MAIN_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_ENTRANCE),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_ENTRANCE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_FROM_MAIN_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_NORTH_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_NORTH_PASSAGE_FROM_MAIN_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_FEAST_ROOM, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_FEAST_ROOM_FROM_MAIN_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_NORTHEAST_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_MAIN_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_EAST_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_MAIN_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_NORTH_PASSAGE_TO_MAIN_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTH_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_NORTH, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_NORTH),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_CAVE, LunpaFortressCave, LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_GROUND_FLOOR),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_FEAST_ROOM_TO_MAIN_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_FEAST_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_MAIN_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTHEAST_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_EAST_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_NORTHEAST_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_BARREL_ROOM, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_BARREL_ROOM_FROM_NORTHEAST_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_MAIN_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_EAST_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_NORTHEAST_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_EAST_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_TREASURE_ROOM, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_TREASURE_ROOM_FROM_EAST_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_BARREL_ROOM_TO_NORTHEAST_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_FROM_BARREL_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_GROUND_FLOOR_TREASURE_ROOM_TO_EAST_PASSAGE, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_TREASURE_ROOM),
    SCENE_EXITS_END,
};

u32 gLunpaFortressUpperFloorExits[] = {
    SCENE_EXITS(LunpaFortressUpperFloor),
    SCENE_EXIT(EXIT_THROWN_OUT, LunpaFortressGates, LUNPA_FORTRESS_GATES_AFTER_CAUGHT),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_TO_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_TO_NORTH_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_WEST_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_TO_MIDDLE_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_WEST_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_WEST_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_NORTH_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_GUARD_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_NORTH_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_WOMENS_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_FROM_NORTH_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_WEST_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_MIDDLE_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_FAMILY_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_FROM_MIDDLE_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_CELL_KEY_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_MIDDLE_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_TO_MIDDLE_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_FAMILY_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_TO_NORTH_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_GUARD_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_TO_SOUTH_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_GUARD_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_TO_NORTH_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_FROM_WOMENS_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_TO_MIDDLE_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_FROM_CELL_KEY_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_TO_EAST_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_CELL_KEY_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_TO_LUNPA_FORTRESS_GROUND_FLOOR_NORTH, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_UPPER_FLOOR_NORTH),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_GUARD_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_SOUTH_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_DONPAS_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_SOUTH_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_EAST_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_SOUTH_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_TO_SOUTH_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_DONPAS_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_TO_CELL_KEY_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_EAST_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_TO_SOUTH_PASSAGE, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_FROM_EAST_PASSAGE),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_TO_DONPAS_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_TONIC_ROOM),
    SCENE_EXIT(LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_TO_TONIC_ROOM, LunpaFortressUpperFloor, LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_FROM_DONPAS_ROOM),
    SCENE_EXITS_END,
};

u32 gLunpaFortressCaveAndToadonpaExits[] = {
    SCENE_EXITS(LunpaFortressCave),
    SCENE_EXIT(LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_GROUND_FLOOR, LunpaFortressGroundFloor, LUNPA_FORTRESS_GROUND_FLOOR_FROM_LUNPA_FORTRESS_CAVE),
    SCENE_EXIT(LUNPA_FORTRESS_CAVE_ENTRANCE_TO_LAKE, LunpaFortressCave, LUNPA_FORTRESS_CAVE_LAKE_FROM_ENTRANCE),
    SCENE_EXIT(LUNPA_FORTRESS_CAVE_LAKE_TO_ENTRANCE, LunpaFortressCave, LUNPA_FORTRESS_CAVE_ENTRANCE_FROM_LAKE),
    SCENE_EXIT(LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHEAST, LunpaFortressToadonpa, LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHEAST),
    SCENE_EXIT(LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTH, LunpaFortressToadonpa, LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTH),
    SCENE_EXIT(LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHWEST, LunpaFortressToadonpa, LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHWEST),
    SCENE_EXIT(EXIT_THROWN_OUT, LunpaFortressGates, LUNPA_FORTRESS_GATES_AFTER_CAUGHT),
    SCENE_EXITS(LunpaFortressToadonpa),
    SCENE_EXIT(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTH, LunpaFortressCave, LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTH),
    SCENE_EXIT(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTHEAST, LunpaFortressCave, LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTHEAST),
    SCENE_EXIT(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTHWEST, LunpaFortressCave, LUNPA_FORTRESS_CAVE_FROM_LUNPA_FORTRESS_TOADONPA_NORTHWEST),
    SCENE_EXIT(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_GATES, LunpaFortressGates, LUNPA_FORTRESS_GATES_FROM_LUNPA_FORTRESS_TOADONPA),
    SCENE_EXIT(EXIT_THROWN_OUT, LunpaFortressGates, LUNPA_FORTRESS_GATES_AFTER_CAUGHT),
    SCENE_EXITS_END,
};

struct ActorPlacement gLunpaFortressActorPlacementsOther[] = {
    { SCENE_TABLE_END },
};

/// Unused: a copy of West Lunpa Cave's actor placements, taken only by
/// a scene this overlay never runs for.
struct ActorPlacement gLunpaFortressUnusedWestLunpaCaveActorPlacements[] = {
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(252), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(552), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BUNZA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gLunpaFortressGroundFloorActorPlacements[] = {
    // Unused: four chests at the map's origin, the display actors of the
    // unused sword pickups (ACTOR_FIRST_ITEM_DISPLAY).
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(760), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(392), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(440), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(464), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(192), 0, PIXELS(432), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(808), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gGroundFloorPatrolRoute,
        PIXELS(736), 0, PIXELS(648), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(192), 0, PIXELS(464), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(424), 0, PIXELS(392), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(432), 0, PIXELS(768), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gLunpaFortressUpperFloorActorPlacements[] = {
    { SPRITE_KEY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(856), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gFirstPatrolRoute,
        PIXELS(88), 0, PIXELS(192), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gSecondPatrolRoute,
        PIXELS(88), 0, PIXELS(288), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(680), 0, PIXELS(88), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(704), 0, PIXELS(104), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(680), 0, PIXELS(128), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(840), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(680), 0, PIXELS(824), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gHallPatrolRoute,
        PIXELS(920), 0, PIXELS(600), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(616), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(560), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(148), 0, PIXELS(616), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(560), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(664), 0, PIXELS(360), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(712), 0, PIXELS(344), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_DONPA, CONDITION_ALWAYS, (s32)gDonpaSleeps,
        PIXELS(944), 0, PIXELS(122), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(904), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MERCURY_DJINNI, FLAG_DJINNI(DJINNI_TONIC), MOVEMENT_TYPE_STAND,
        PIXELS(872), 0, PIXELS(336), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gLunpaFortressCaveActorPlacements[] = {
    { SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(576), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(440), 0, PIXELS(282), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(56), 0, PIXELS(176), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(504), 0, PIXELS(168), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(136), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gLunpaFortressToadonpaActorPlacements[] = {
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(56), 0, PIXELS(72), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(72), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(72), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { 187, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(472), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(528), 0, PIXELS(216), FACING_WEST, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_DODONPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_DONPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(464), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

/// Unused: an empty trigger table nothing returns.
struct Trigger gLunpaFortressNoTriggers[] = {
    { TRIGGERS_END },
};

struct Trigger gLunpaFortressGroundFloorTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_GATES },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_TO_MAIN_PASSAGE },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_ENTRANCE },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_NORTH_PASSAGE },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_FEAST_ROOM },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_NORTHEAST_PASSAGE },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_TO_EAST_PASSAGE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_SOUTH },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_NORTH_PASSAGE_TO_MAIN_PASSAGE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_UPPER_FLOOR_NORTH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TO_LUNPA_FORTRESS_CAVE },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_FEAST_ROOM_TO_MAIN_PASSAGE },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_MAIN_PASSAGE },
    { TRIGGER_EXIT, 14, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_EAST_PASSAGE },
    { TRIGGER_EXIT, 15, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_NORTHEAST_PASSAGE_TO_BARREL_ROOM },
    { TRIGGER_EXIT, 16, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_MAIN_PASSAGE },
    { TRIGGER_EXIT, 17, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_NORTHEAST_PASSAGE },
    { TRIGGER_EXIT, 18, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_TO_TREASURE_ROOM },
    { TRIGGER_EXIT, 19, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_BARREL_ROOM_TO_NORTHEAST_PASSAGE },
    { TRIGGER_EXIT, 20, CONDITION_ALWAYS, LUNPA_FORTRESS_GROUND_FLOOR_TREASURE_ROOM_TO_EAST_PASSAGE },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_FORTRESS_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(LunpaFortress_SearchLockedDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR, GROUND_FLOOR_DOOR_FLAG, EVENT(LunpaFortress_UnlockGroundFloorDoor) },
    { STEP_TRIGGER_FACING(FACING_SOUTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR, GROUND_FLOOR_DOOR_FLAG, EVENT(LunpaFortress_UnlockGroundFloorDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 1, GROUND_FLOOR_DOOR_FLAG + 1, EVENT(LunpaFortress_UnlockGroundFloorDoor) },
    { STEP_TRIGGER_FACING(FACING_SOUTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 1, GROUND_FLOOR_DOOR_FLAG + 1, EVENT(LunpaFortress_UnlockGroundFloorDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 2, GROUND_FLOOR_DOOR_FLAG + 2, EVENT(LunpaFortress_UnlockGroundFloorDoor) },
    { STEP_TRIGGER_FACING(FACING_SOUTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 2, GROUND_FLOOR_DOOR_FLAG + 2, EVENT(LunpaFortress_UnlockGroundFloorDoor) },
    { TRIGGER_STEP, 50, FLAG_LUNPA_FORTRESS_ENTRANCE_GUARD_CAUGHT_PARTY, EVENT(LunpaFortress_EntranceGuardThrowsPartyOut) },
    { TRIGGER_STEP, 51, FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY, EVENT(LunpaFortress_SentriesThrowPartyOut) },
    { TRIGGER_TALK, ACTOR_GROUND_FLOOR_MAN_WONDERING_ABOUT_DONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpaResting) },
    { TRIGGER_TALK, ACTOR_GROUND_FLOOR_WOMAN_SEEKING_DODONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaWhereIsDodonpa) },
    { TRIGGER_TALK, ACTOR_GROUND_FLOOR_FEASTING_MAN, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetBuysDrink) },
    { TRIGGER_TALK, ACTOR_GROUND_FLOOR_BOASTING_MAN, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetMoneyTree) },
    { TRIGGER_TALK, ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, CONDITION_ALWAYS, EVENT(LunpaFortress_TalkToStartledOldMan) },
    { TRIGGER_TALK, ACTOR_GROUND_FLOOR_SECOND_SENTRY, CONDITION_ALWAYS, EVENT(LunpaFortress_TalkToSentry) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GROUND_FLOOR_MAN_WONDERING_ABOUT_DONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaWhoFoughtDonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GROUND_FLOOR_WOMAN_SEEKING_DODONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaWifeSeeksDodonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GROUND_FLOOR_FEASTING_MAN, CONDITION_ALWAYS, MSGNO(MsgLunpaKalayBuysDrink) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GROUND_FLOOR_BOASTING_MAN, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetKeepsEarnings) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, CONDITION_ALWAYS, EVENT(LunpaFortress_ReadStartledResidentsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GROUND_FLOOR_SECOND_SENTRY, CONDITION_ALWAYS, EVENT(LunpaFortress_ReadSentrysThoughts) },
    { TRIGGER_RAISED, RAISED_GROUND_FLOOR_PATROL_SPOTTED, CONDITION_ALWAYS, EVENT(LunpaFortress_GroundFloorPatrolThrowsPartyOut) },
    { TRIGGER_RAISED, RAISED_UPPER_HALL_SPOTTED, CONDITION_ALWAYS, EVENT(LunpaFortress_UpperHallGuardThrowsPartyOut) },
    { TRIGGER_RAISED, RAISED_ENTRANCE_GUARD_SPOTTED, CONDITION_ALWAYS, EVENT(LunpaFortress_EntranceGuardThrowsPartyOut) },
    { TRIGGER_RAISED, RAISED_SENTRIES_SPOTTED, CONDITION_ALWAYS, EVENT(LunpaFortress_SentriesThrowPartyOut) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_LUNPA_FORTRESS_CHEST_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_LUNPA_FORTRESS_CHEST_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_LUNPA_FORTRESS_CHEST_MYTHRIL_CIRCLET, FIND_ITEM(ITEM_MYTHRIL_CIRCLET) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_LUNPA_FORTRESS_CHEST_COINS, FIND_COINS(100) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 104, FLAG_LUNPA_FORTRESS_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { TRIGGERS_END },
};

struct Trigger gLunpaFortressUpperFloorTriggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_TO_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_TO_NORTH_PASSAGE },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_TO_MIDDLE_PASSAGE },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_WEST_PASSAGE },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_GUARD_ROOM },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_NORTH_PASSAGE_TO_WOMENS_ROOM },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_WEST_PASSAGE },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_FAMILY_ROOM },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_MIDDLE_PASSAGE_TO_CELL_KEY_ROOM },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_TO_MIDDLE_PASSAGE },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_TO_NORTH_PASSAGE },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_TO_SOUTH_PASSAGE },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_TO_NORTH_PASSAGE },
    { TRIGGER_EXIT, 14, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_TO_MIDDLE_PASSAGE },
    { TRIGGER_EXIT, 15, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_TO_EAST_PASSAGE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 16, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_TO_LUNPA_FORTRESS_GROUND_FLOOR_NORTH },
    { TRIGGER_EXIT, 17, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_GUARD_ROOM },
    { TRIGGER_EXIT, 18, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_DONPAS_ROOM },
    { TRIGGER_EXIT, 19, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_SOUTH_PASSAGE_TO_EAST_PASSAGE },
    { TRIGGER_EXIT, 20, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_TO_SOUTH_PASSAGE },
    { TRIGGER_EXIT, 21, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_TO_CELL_KEY_ROOM },
    { TRIGGER_EXIT, 22, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_TO_SOUTH_PASSAGE },
    { TRIGGER_EXIT, 23, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_TO_DONPAS_ROOM },
    { TRIGGER_EXIT, 24, CONDITION_ALWAYS, LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_TO_TONIC_ROOM },
    { TRIGGER_STEP, 30, FLAG_LUNPA_FORTRESS_MIDDLE_GUARD_BEATEN, EVENT(LunpaFortress_FightHallGuard) },
    { TRIGGER_STEP, 31, FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN, EVENT(LunpaFortress_FightThreeGuards) },
    { TRIGGER_STEP, 32, FLAG_LUNPA_FORTRESS_EAST_GUARD_BEATEN, EVENT(LunpaFortress_FightEastGuard) },
    { TRIGGER_STEP, 33, FLAG_LUNPA_FORTRESS_WEST_GUARD_BEATEN, EVENT(LunpaFortress_FightWestGuard) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_FORTRESS_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(LunpaFortress_SearchLockedDoor) },
    { STEP_TRIGGER_FACING(FACING_SOUTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR, UPPER_FLOOR_DOOR_FLAG, EVENT(LunpaFortress_UnlockUpperFloorDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 1, UPPER_FLOOR_DOOR_FLAG + 1, EVENT(LunpaFortress_UnlockUpperFloorDoor) },
    { STEP_TRIGGER_FACING(FACING_SOUTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 1, UPPER_FLOOR_DOOR_FLAG + 1, EVENT(LunpaFortress_UnlockUpperFloorDoor) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_UPPER_FLOOR_CELL_KEY, CONDITION_ALWAYS, EVENT(LunpaFortress_TakeCellKey) },
    { TRIGGER_TALK, ACTOR_UPPER_FLOOR_CELL_KEY, CONDITION_ALWAYS, EVENT(LunpaFortress_TakeCellKey) },
    { TRIGGER_TALK, ACTOR_UPPER_FLOOR_DODONPAS_WIFE, CONDITION_ALWAYS, EVENT(LunpaFortress_AskWhereDodonpaIs) },
    { TRIGGER_TALK, ACTOR_UPPER_FLOOR_DONPAPPA, CONDITION_ALWAYS, MSGNO(MsgLunpaChildMustGrowUp) },
    { TRIGGER_TALK, ACTOR_UPPER_FLOOR_DODONPAS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgLunpaPapaWatchesLadies) },
    { TRIGGER_TALK, ACTOR_UPPER_FLOOR_OLD_WOMAN_PRAISING_DONPAPPA, CONDITION_ALWAYS, MSGNO(MsgLunpaChildTooResponsible) },
    { TRIGGER_TALK, ACTOR_UPPER_FLOOR_WOMAN_FEARING_FOR_DONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpaExhausted) },
    { TRIGGER_TALK, ACTOR_UPPER_FLOOR_CAPTIVE_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLunpaWomenWantHome) },
    { TRIGGER_TALK, ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, CONDITION_ALWAYS, EVENT(LunpaFortress_TalkToBeatenWestGuardOfThree) },
    { TRIGGER_TALK, ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, CONDITION_ALWAYS, EVENT(LunpaFortress_TalkToBeatenMiddleGuardOfThree) },
    { TRIGGER_TALK, ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, CONDITION_ALWAYS, EVENT(LunpaFortress_TalkToBeatenEastGuardOfThree) },
    { TRIGGER_TALK, ACTOR_UPPER_FLOOR_ATTENDANT, CONDITION_ALWAYS, EVENT(LunpaFortress_TalkToAttendant) },
    { TRIGGER_TALK, ACTOR_UPPER_FLOOR_DONPA, CONDITION_ALWAYS, EVENT(LunpaFortress_TalkToDonpa) },
    { TRIGGER_TALK, ACTOR_UPPER_FLOOR_TONIC, CONDITION_ALWAYS, EVENT(LunpaFortress_DjinniJoins) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UPPER_FLOOR_DODONPAS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaSuccessor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UPPER_FLOOR_DONPAPPA, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaProtectsFortress) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UPPER_FLOOR_DODONPAS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgLunpaDaughterWillNotMarryPapa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UPPER_FLOOR_OLD_WOMAN_PRAISING_DONPAPPA, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaJudges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UPPER_FLOOR_WOMAN_FEARING_FOR_DONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaFearWithoutDonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UPPER_FLOOR_CAPTIVE_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLunpaHopeDonpaRecovers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, CONDITION_ALWAYS, MSGNO(MsgLunpaShouldNotHaveFought) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetInCave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, CONDITION_ALWAYS, MSGNO(MsgLunpaHiddenDoorSwitch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UPPER_FLOOR_DONPA, CONDITION_ALWAYS, EVENT(LunpaFortress_ReadDonpasThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UPPER_FLOOR_ATTENDANT, CONDITION_ALWAYS, EVENT(LunpaFortress_ReadAttendantsThoughts) },
    { TRIGGER_RAISED, RAISED_PATROL_PAIR_SPOTTED, CONDITION_ALWAYS, EVENT(LunpaFortress_PatrolPairThrowsPartyOut) },
    { TRIGGER_RAISED, RAISED_HALL_PATROL_SPOTTED, CONDITION_ALWAYS, EVENT(LunpaFortress_HallPatrolThrowsPartyOut) },
    { TRIGGERS_END },
};

struct Trigger gLunpaFortressCaveTriggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_GROUND_FLOOR },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_ENTRANCE_TO_LAKE },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_LAKE_TO_ENTRANCE },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHEAST },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, LUNPA_FORTRESS_CAVE_TO_LUNPA_FORTRESS_TOADONPA_NORTHWEST },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_FORTRESS_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(LunpaFortress_SearchLockedDoor) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_FORTRESS_SEALED_DOOR, CONDITION_ALWAYS, EVENT(LunpaFortress_SearchSealedDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR, CAVE_DOOR_FLAG, EVENT(LunpaFortress_UnlockCaveDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 1, CAVE_DOOR_FLAG + 1, EVENT(LunpaFortress_UnlockCaveDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 2, CAVE_DOOR_FLAG + 2, EVENT(LunpaFortress_UnlockCaveDoor) },
    { STEP_TRIGGER_FACING(FACING_SOUTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 2, CAVE_DOOR_FLAG + 2, EVENT(LunpaFortress_UnlockCaveDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 50, CONDITION_ALWAYS, EVENT(LunpaFortress_CratesSink) },
    { STEP_TRIGGER_WALKING, 70, CONDITION_ALWAYS, EVENT(LunpaFortress_PushCrates) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 51, FLAG_LUNPA_FORTRESS_SECOND_SWITCH_UNDER_IVY, EVENT(LunpaFortress_FlipSecondSwitch) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 52, FLAG_LUNPA_FORTRESS_FIRST_SWITCH_UNDER_IVY, EVENT(LunpaFortress_FlipFirstSwitch) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(LunpaFortress_RevealSwitches) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(LunpaFortress_HideSwitches) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_CAVE_FIRST_SWITCH_IVY, CONDITION_ALWAYS, EVENT(LunpaFortress_BlowAwayFirstSwitchIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_CAVE_PASSAGE_IVY, CONDITION_ALWAYS, EVENT(LunpaFortress_BlowAwayPassageIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_CAVE_SECOND_SWITCH_IVY, CONDITION_ALWAYS, EVENT(LunpaFortress_BlowAwaySecondSwitchIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CAVE_CRATES, FLAG_LUNPA_FORTRESS_CRATES_MOVED, EVENT(LunpaFortress_MoveCrates) },
    { TRIGGERS_END },
};

struct Trigger gLunpaFortressTriggersOther[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTHEAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_CAVE_NORTHWEST },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_FORTRESS_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(LunpaFortress_SearchLockedDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR, TOADONPA_DOOR_FLAG, EVENT(LunpaFortress_UnlockToadonpaDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 1, TOADONPA_DOOR_FLAG + 1, EVENT(LunpaFortress_UnlockToadonpaDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 2, TOADONPA_DOOR_FLAG + 2, EVENT(LunpaFortress_UnlockToadonpaDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 3, TOADONPA_DOOR_FLAG + 3, EVENT(LunpaFortress_UnlockToadonpaDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 4, TOADONPA_DOOR_FLAG + 4, EVENT(LunpaFortress_UnlockToadonpaDoor) },
    { TRIGGER_STEP, 20, FLAG_LUNPA_FORTRESS_FINDING_HAMMET, EVENT(LunpaFortress_FindHammet) },
    { TRIGGER_TALK, ACTOR_TOADONPA_CELL_GUARD, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetStillInside) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOADONPA_CELL_GUARD, CONDITION_ALWAYS, MSGNO(MsgLunpaDodonpaSecretlyInside) },
    // These two rows give ACTOR_TOADONPA_HAMMET Dodonpa's lines from his
    // cell, though Hammet is put out of sight at (0, 0) once Dodonpa is
    // jailed; before then he stands where the party finds him.
    // XXX: whether the party can talk to Hammet before step trigger 20 starts
    // LunpaFortress_FindHammet; a recording walking up to him would settle it.
    { TRIGGER_TALK, ACTOR_TOADONPA_HAMMET, CONDITION_ALWAYS, MSGNO(MsgLunpaWrongTurnOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOADONPA_HAMMET, CONDITION_ALWAYS, MSGNO(MsgLunpaCantBelieveWhen) },
    { TRIGGER_TALK, ACTOR_INVISIBLE_TOADONPA_DODONPA, CONDITION_ALWAYS, EVENT(LunpaFortress_TalkToJailedDodonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INVISIBLE_TOADONPA_DODONPA, CONDITION_ALWAYS, EVENT(LunpaFortress_ReadJailedDodonpasThoughts) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_LUNPA_FORTRESS_CHEST_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { TRIGGERS_END },
};

u32 gLunpaFortressToadonpaDoorCells[] = {
    13, 56, 8, 56, 3, 56, 28, 85,
    28, 65,
};

u32 gLunpaFortressCaveDoorCells[] = {
    31, 61, 15, 57, 6, 61,
};

u32 gLunpaFortressUpperFloorDoorCells[] = {
    16, 119, 16, 113,
};

u32 gLunpaFortressGroundFloorDoorCells[] = {
    28, 72, 43, 92,
};
// clang-format on
