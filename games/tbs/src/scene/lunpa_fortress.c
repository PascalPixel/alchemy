/// @file
/// Lunpa Fortress, the stronghold of the thief Dodonpa at the top of Lunpa,
/// where he holds Hammet, the great merchant of Kalay, for ransom; and the
/// cave and cells under it, where Hammet is kept.
///
/// The party slips in by the door at the back of the Lunpa Fortress Gates
/// (lunpa.c) and sneaks through the dark halls under Cloak. The halls are lit
/// only in circles of light, round the torches and round each guard. A guard
/// who sees Isaac stops him ("Who are you? How did you get in here?") and
/// throws the party back out to the gates. The rooms off the halls are lit,
/// and their people only talk. In story order:
///
/// - The ground floor (`LunpaFortressGroundFloor`). A small room inside the
///   door opens on a long winding hall, where a guard at its west end and two
///   sentries at its east end keep watch; stairs at its west end lead up.
///   Off it are a feast room where Dodonpa's men eat and drink on Hammet's
///   ransom; a winding hall in the north-east, with one guard, and beyond it
///   a room of barrels where a startled old man hides and a barrel holds a
///   Vial; and a hall in the east walked by a patrolling guard, where a
///   locked cell door at its north-west keeps a room of four chests (an
///   Elixir, a Lucky Medal, a Mythril Circlet and 100 coins). In the hall at
///   the top of the map a second cell door shuts off the stairs down to the
///   cave from the rest of the floor.
/// - The upper floor (`LunpaFortressUpperFloor`). Two guards walk to and fro
///   across the tall west hall in step, one above the other. Past it, three
///   red-capped guards in a room in the north fight the party together as
///   three Brigands, and a guard in the doorway of Donpa's room, another in
///   the middle hall and another in the Cell Key room each fight it as one
///   Brigand. Donpa, Dodonpa's old father, lies asleep in his room; a guard
///   walks the east hall; the Cell Key lies on a stool just inside a cell
///   door of the Cell Key room, where Catch reaches it from the passage
///   outside. Dodonpa's wife and children, and the women he brought to the
///   fortress, talk in their rooms. Stairs from the Cell Key room lead down
///   to the north end of the ground floor, by the stairs to the cave.
/// - The cave (`LunpaFortressCave`), a ring of ledges round an underground
///   lake. Whirlwind blows ivy off two switches hidden in its walls, Reveal
///   shows them, and each opens a sealed doorway. More ivy hides a passage to
///   the cells, and a stack of crates pushed down a corridor fills a gap at its
///   foot.
/// - The cells (`LunpaFortressToadonpa`): a row of three cells, the middle
///   one holding a Power Bread, and the room where Hammet is held. Walking
///   in to him, Dodonpa traps the party and sets his monster Toadonpa on it
///   (LunpaFortress_FindHammet). After the battle Dodonpa lies pinned under
///   the fallen monster, and his father Donpa comes down and locks him in
///   Hammet's cell (LunpaFortress_DonpaJudgesDodonpa), which sets
///   FLAG_DODONPA_LOCKED_UP and changes all of Lunpa. The party leaves with
///   Hammet, and may come back to Donpa for his reward, Tonic, a Mercury
///   Djinni.
///
/// The four scenes share this overlay and the area name Lunpa Fortress. The
/// getters choose by `gMainRoutineState.next`; the cave and the cells share
/// one exit table. Every return to the Lunpa Fortress Gates clears the
/// fortress's puzzle and beaten-guard flags, so the fortress starts over.
///
/// A guard on screen sees the leader in his own column up to six rows away,
/// or within a row of his up to six columns away, whatever stands between
/// them, unless Cloak is on (LunpaFortress_CanGuardSeeLeader). Each watching
/// guard has a task, started by the entrance the party arrives by
/// (LunpaFortress_InitializeGroundFloor,
/// LunpaFortress_InitializeUpperFloor), that raises one of the
/// RAISED_*_SPOTTED triggers when he sees the leader; its event throws the
/// party out (LunpaFortress_GuardThrowsPartyOut, EXIT_THROWN_OUT). A
/// cloaked Isaac who comes within four cells of a patrolling guard loses
/// Cloak (CLOAK_END).
///
/// The patrolling guards walk actor scripts (gLunpaFortressFirstPatrolRoute
/// and the others) and stand still while a menu, Psynergy or an item is in
/// use. Each carries his circle of light, drawn on MAPGRID_LAYER_OVERHEAD,
/// which his task moves with him and shakes by a pixel every frame, so the
/// light flickers. Each has stretches of its rounds where it
/// cannot see the leader. The pair in the upper floor's west hall walk east
/// and west across columns 5 to 12, one in row 12 and one in row 18, and
/// never see Isaac in rows 5 to 7 west of column 11, at the hall's top, or in
/// columns 8 and 9 below row 22, at its foot
/// (LunpaFortress_IsLeaderHiddenFromPatrolPair).
///
/// The guards who fight greet the party with one of four challenges, drawn
/// at random on every arrival (gLunpaFortressRandomPick).
///
/// With the Cell Key in the party, walking up to a cell door opens it, in
/// all four scenes. Each opened door is remembered in an area flag from
/// FLAG_AREA_TEMP_50 (TOADONPA_DOOR_FLAG and the others).
///
/// Unused: four item pickups that would give an Arctic Blade, a Masamune, a
/// Silver Blade and a Shamshir from chests the ground floor places out of
/// sight, and copies of West Lunpa Cave's exits and actor placements, which
/// these scenes never read.

#include "ability_ids.h"
#include "djinn_ids.h"
#include "edition.h"
#include "flags.h"
#include "item_ids.h"
#include "static_assert.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/map_grid_scroll.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/import.h"
#include "scene/walk_into_leader.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/heap_slots.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern s32 gLunpaFortressRandomPick;
extern struct SceneEntrance gLunpaFortressGroundFloorEntrances[];
extern struct SceneEntrance gLunpaFortressUpperFloorEntrances[];
extern struct SceneEntrance gLunpaFortressCaveEntrances[];
extern struct SceneEntrance gLunpaFortressEntrancesOther[];
extern u32 gLunpaFortressExitsOther[];
extern u32 gLunpaFortressUpperFloorExits[];
extern u32 gLunpaFortressCaveAndToadonpaExits[];
extern struct ActorPlacement gLunpaFortressActorPlacementsOther[];
extern struct ActorPlacement gLunpaFortressUnusedWestLunpaCaveActorPlacements[];
extern struct ActorPlacement gLunpaFortressGroundFloorActorPlacements[];
extern struct ActorPlacement gLunpaFortressUpperFloorActorPlacements[];
extern struct ActorPlacement gLunpaFortressCaveActorPlacements[];
extern struct ActorPlacement gLunpaFortressToadonpaActorPlacements[];
extern struct Trigger gLunpaFortressGroundFloorTriggers[];
extern struct Trigger gLunpaFortressUpperFloorTriggers[];
extern struct Trigger gLunpaFortressCaveTriggers[];
extern struct Trigger gLunpaFortressTriggersOther[];
extern u32 gLunpaFortressToadonpaDoorCells[];
extern u32 gLunpaFortressCaveDoorCells[];
extern u32 gLunpaFortressUpperFloorDoorCells[];
extern u32 gLunpaFortressGroundFloorDoorCells[];

/// The ground floor's people, numbered in the order of
/// gLunpaFortressGroundFloorActorPlacements from ACTOR_FIRST_PLACED, past its
/// first four rows. Those four are chests placed out of sight at the map's
/// origin (see ACTOR_FIRST_SWORD_CHEST).
/// Dodonpa's men, here and upstairs, wear the sprite of a man with a mop of
/// orange hair (SPRITE_MAN_WITH_ORANGE_HAIR). The guards who watch the dark
/// halls are recoloured green (SPRITE_COLOR_GRB).
enum {
  /// The guard at the west end of the long hall, by the stairs up.
  ACTOR_GROUND_FLOOR_ENTRANCE_GUARD = ACTOR_FIRST_PLACED + 4,
  /// The first of the two sentries at the long hall's east end, the one who
  /// speaks when they catch the party.
  ACTOR_GROUND_FLOOR_FIRST_SENTRY,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), wondering where
  /// Dodonpa has gone.
  ACTOR_GROUND_FLOOR_WOMAN_SEEKING_DODONPA,
  /// One of Dodonpa's men (the guards' sprite), feasting on Kalay's ransom.
  ACTOR_GROUND_FLOOR_FEASTING_MAN,
  /// One of Dodonpa's men, drinking and boasting of Hammet.
  ACTOR_GROUND_FLOOR_BOASTING_MAN,
  /// The guard in the winding north-east hall.
  ACTOR_GROUND_FLOOR_NORTHEAST_HALL_GUARD,
  /// The guard who walks his rounds of the east hall.
  ACTOR_GROUND_FLOOR_PATROL_GUARD,
  /// A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), talking of
  /// Donpa's poor health.
  ACTOR_GROUND_FLOOR_MAN_WONDERING_ABOUT_DONPA,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), startled when the
  /// party first speaks to him (LunpaFortress_TalkToStartledOldMan).
  ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN,
  /// The other man standing watch with ACTOR_GROUND_FLOOR_FIRST_SENTRY.
  ACTOR_GROUND_FLOOR_SECOND_SENTRY
};

/// The upper floor's people and the Cell Key, numbered in the order of
/// gLunpaFortressUpperFloorActorPlacements from ACTOR_FIRST_PLACED.
enum {
  ACTOR_UPPER_FLOOR_CELL_KEY = ACTOR_FIRST_PLACED,
  /// The pair who walk to and fro across the west hall in step, the first
  /// to the north of the second.
  ACTOR_UPPER_FLOOR_FIRST_PATROL,
  ACTOR_UPPER_FLOOR_SECOND_PATROL,
  /// The guard in the narrow way north out of the middle hall, to the
  /// family's room.
  ACTOR_UPPER_FLOOR_MIDDLE_HALL_GUARD,
  /// The three guards who fight together, by where they stand once beaten
  /// (LunpaFortress_PlaceThreeBeatenGuards): west, middle and east in a row.
  ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST,
  ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE,
  ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST,
  /// The guard beside the Cell Key in the Cell Key room.
  ACTOR_UPPER_FLOOR_CELL_KEY_ROOM_GUARD,
  /// The guard in the doorway up to Donpa's room.
  ACTOR_UPPER_FLOOR_DONPAS_DOOR_GUARD,
  /// The guard who walks his rounds of the east hall.
  ACTOR_UPPER_FLOOR_HALL_PATROL,
  /// Dodonpa's wife, a woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS),
  /// the "Mama" of his children, looking for him.
  ACTOR_UPPER_FLOOR_DODONPAS_WIFE,
  /// Donpappa, a small boy (SPRITE_BOY_WITH_FAIR_HAIR), Dodonpa's son, as he
  /// names himself to Mind Read.
  ACTOR_UPPER_FLOOR_DONPAPPA,
  /// Dodonpa's daughter, a little girl (SPRITE_GIRL_WITH_PIGTAILS).
  ACTOR_UPPER_FLOOR_DODONPAS_DAUGHTER,
  /// An old woman (SPRITE_OLD_WOMAN) who pities Donpappa.
  ACTOR_UPPER_FLOOR_OLD_WOMAN_PRAISING_DONPAPPA,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who fears for Donpa.
  ACTOR_UPPER_FLOOR_WOMAN_FEARING_FOR_DONPA,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), one of the women
  /// Dodonpa has brought to the fortress.
  ACTOR_UPPER_FLOOR_CAPTIVE_WOMAN,
  /// Donpa, asleep (SPRITE_DONPA).
  ACTOR_UPPER_FLOOR_DONPA,
  /// Donpa's attendant, a woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  ACTOR_UPPER_FLOOR_ATTENDANT,
  /// Tonic, a Mercury Djinni, Donpa's reward.
  ACTOR_UPPER_FLOOR_TONIC
};

/// The cave's ivy and crates, numbered in the order of
/// gLunpaFortressCaveActorPlacements from ACTOR_FIRST_PLACED.
enum {
  /// Ivy on the north wall of the room at the foot of the stairs, west of
  /// its sealed doorway, hiding the first switch.
  ACTOR_CAVE_FIRST_SWITCH_IVY = ACTOR_FIRST_PLACED,
  /// Ivy on the north wall of the cave's east ledge, hiding the passage to
  /// the cells.
  ACTOR_CAVE_PASSAGE_IVY,
  /// Ivy on the wall of the small north-west chamber, west of its sealed
  /// doorway, hiding the second switch.
  ACTOR_CAVE_SECOND_SWITCH_IVY,
  /// An invisible solid actor (SPRITE_BLANK, ACTOR_SOLID) at (504, 168).
  /// It makes the jar the layout draws there, in the cell behind the cave's
  /// eastern door, stop the leader. The layout's cell behavior lets him walk
  /// onto the jar, and with this actor he stops 14 pixels short of it.
  /// A blank sprite draws only its shadow, which LunpaFortress_InitializeCave
  /// turns off.
  ACTOR_INVISIBLE_CAVE_JAR,
  /// A stack of wooden crates (the sprite of the crates at the Bilibin
  /// Barricade and on Tolbi Docks) in a nook behind the cave's middle cell
  /// door, pushed or moved with Move down the corridor into its gap.
  ACTOR_CAVE_CRATES
};

/// The cells' people, numbered in the order of
/// gLunpaFortressToadonpaActorPlacements from ACTOR_FIRST_PLACED.
/// Its rows for Ivan, Mia and Garet keep the party's own slots.
enum {
  /// Three invisible solid actors (SPRITE_BLANK, ACTOR_SOLID) at
  /// (56, 72), (136, 72) and (216, 72).
  /// They stand in line with the cell doors gLunpaFortressToadonpaDoorCells
  /// lists third, second and first. They make the jar, chest and jar the layout
  /// draws in those cells stop the leader.
  /// The layout's cell behaviors let him walk onto the two jars. The chest's
  /// own cells already stop him, so the middle actor changes nothing.
  /// Scene_Initialize turns off the shadows their blank sprites would draw and
  /// lays them underfoot.
  ACTOR_INVISIBLE_TOADONPA_WEST_JAR = ACTOR_FIRST_PLACED,
  ACTOR_INVISIBLE_TOADONPA_CHEST,
  ACTOR_INVISIBLE_TOADONPA_EAST_JAR,
  /// The monster Dodonpa frees on the party (LunpaFortress_FindHammet).
  ACTOR_TOADONPA_BOSS,
  ACTOR_TOADONPA_HAMMET,
  /// Dodonpa (SPRITE_DODONPA).
  ACTOR_TOADONPA_DODONPA,
  ACTOR_TOADONPA_DONPA,
  /// A blank sprite (SPRITE_BLANK_LARGE) laid underfoot with Toadonpa and
  /// moving with it, drawn 4/5 as wide.
  ACTOR_INVISIBLE_TOADONPA_BOSS_PART,
  /// One of Dodonpa's men, a man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), placed by Dodonpa's cell after
  /// Donpa's judgment. Read with Mind Read: "Actually, Dodonpa's in here, but
  /// only a few people know that."
  ACTOR_TOADONPA_CELL_GUARD,
  /// An invisible actor that stands before Dodonpa's cell after Donpa's
  /// judgment. Talking to it or reading its mind reaches Dodonpa.
  ACTOR_INVISIBLE_TOADONPA_DODONPA
};

/// Animations of the fortress's sprites beyond enum ActorAnimation, as their
/// animation tables and frames under graphics/sprite draw them.
enum LunpaFortressAnimation {
  /// One of Dodonpa's men lying beaten on the floor (man_with_orange_hair.png),
  /// as the three guards lie after their fight.
  ANIM_GUARD_BEATEN = 5,
  /// Donpa lying in bed, only his head on the pillow showing (donpa.png).
  ANIM_DONPA_IN_BED = 5,
  ANIM_TOADONPA_FALLEN = 3,  ///< sprawled on its side (toadonpa.png)
  ANIM_TOADONPA_REAR_UP = 4, ///< reared up, its mouth open (toadonpa.png)
  /// Dodonpa lying on his front, propped on his hands, his head raised
  /// (dodonpa.png).
  ANIM_DODONPA_PRONE = 7,
  /// ANIM_DODONPA_PRONE with his head sinking to the floor and lifting again,
  /// over and over.
  ANIM_DODONPA_PRONE_SLUMP = 5,
  /// ANIM_DODONPA_PRONE dropping his head, then ANIM_DODONPA_PRONE_SLUMP.
  ANIM_DODONPA_PRONE_HEAD_DOWN = 6
};

/// The cell behaviors the fortress's triggers answer. Each scene numbers its
/// own, so a number can mean one thing on one floor and another below.
enum LunpaFortressCellBehavior {
  /// A cell door, in every scene.
  CELL_BEHAVIOR_LUNPA_FORTRESS_LOCKED_DOOR = 99,
  /// The cave's two sealed doorways, which only their hidden switches open.
  CELL_BEHAVIOR_LUNPA_FORTRESS_SEALED_DOOR = 98,
  /// The cells either side of a scene's first cell door; its other doors
  /// count on from here.
  CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR = 40,
  /// Unused: no cell of the ground floor has these. Walking onto them would
  /// have had the entrance guard, or the sentries, throw the party out.
  CELL_BEHAVIOR_LUNPA_FORTRESS_ENTRANCE_GUARD_POST = 50,
  CELL_BEHAVIOR_LUNPA_FORTRESS_SENTRIES_POST = 51,
  /// The upper floor: the floor in front of each guard who fights.
  CELL_BEHAVIOR_LUNPA_FORTRESS_BEFORE_MIDDLE_HALL_GUARD = 30,
  CELL_BEHAVIOR_LUNPA_FORTRESS_BEFORE_THREE_GUARDS = 31,
  CELL_BEHAVIOR_LUNPA_FORTRESS_BEFORE_DONPAS_DOOR_GUARD = 32,
  CELL_BEHAVIOR_LUNPA_FORTRESS_BEFORE_CELL_KEY_ROOM_GUARD = 33,
  /// The cave: the floor Isaac can push the crates over, from their nook
  /// down the corridor.
  CELL_BEHAVIOR_LUNPA_FORTRESS_CRATES_PATH = 70,
  /// The cave: the end of the crates' corridor, at the edge of the gap.
  CELL_BEHAVIOR_LUNPA_FORTRESS_CRATES_GAP = 50,
  /// The cave: where each hidden switch is searched while Reveal shows it.
  CELL_BEHAVIOR_LUNPA_FORTRESS_SECOND_SWITCH = 51,
  CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_SWITCH = 52,
  /// The cells: the row just inside the door of Hammet's cell.
  CELL_BEHAVIOR_LUNPA_FORTRESS_HAMMETS_CELL = 20,
  /// The ground floor's room of four chests, and the barrel room's barrel.
  CELL_BEHAVIOR_LUNPA_FORTRESS_CHEST_ELIXIR = 100,
  CELL_BEHAVIOR_LUNPA_FORTRESS_CHEST_LUCKY_MEDAL = 101,
  CELL_BEHAVIOR_LUNPA_FORTRESS_CHEST_MYTHRIL_CIRCLET = 102,
  CELL_BEHAVIOR_LUNPA_FORTRESS_CHEST_COINS = 103,
  CELL_BEHAVIOR_LUNPA_FORTRESS_BARREL_VIAL = 104,
  /// The chest in the middle one of the three cells.
  CELL_BEHAVIOR_LUNPA_FORTRESS_CHEST_POWER_BREAD = 101
};

/// The events a watching guard raises on spotting the party.
enum LunpaFortressRaisedTrigger {
  RAISED_PATROL_PAIR_SPOTTED = 91,
  RAISED_HALL_PATROL_SPOTTED,
  RAISED_GROUND_FLOOR_PATROL_SPOTTED,
  RAISED_NORTHEAST_HALL_SPOTTED,
  RAISED_ENTRANCE_GUARD_SPOTTED,
  RAISED_SENTRIES_SPOTTED
};

/// Where a party caught by a guard is sent: the Lunpa Fortress Gates.
enum { EXIT_THROWN_OUT = 60 };

/// The first of the flags that remember which cell doors the Cell Key opened,
/// per scene: one for each door trigger from
/// CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR.
enum {
  TOADONPA_DOOR_FLAG = FLAG_AREA_TEMP_50,
  CAVE_DOOR_FLAG = FLAG_AREA_TEMP_55,
  UPPER_FLOOR_DOOR_FLAG = FLAG_AREA_TEMP_58,
  GROUND_FLOOR_DOOR_FLAG = FLAG_AREA_TEMP_5A
};

/// Hammet's reunion with Ivan in his cell and Dodonpa's interruption
/// (LunpaFortress_FindHammet), in order. The scene shows its own
/// MsgLunpaDodonpaPulledLever in place of REUNION_DODONPA_PULLS_LEVER.
enum ReunionMessage {
  REUNION_HAMMET_ASKS_IF_TIME_TO_EAT,
  REUNION_HAMMET_ASKS_WHO,
  REUNION_IVAN_NAMES_HAMMET,
  REUNION_HAMMET_KNOWS_IVAN,
  REUNION_HAMMET_ASKS_ABOUT_FRIENDS,
  REUNION_HAMMET_SEES_JOURNEY_BEGUN,
  REUNION_IVAN_COMES_TO_SAVE_HAMMET,
  REUNION_HAMMET_WARNS_OF_RISK,
  REUNION_DODONPA_HALTS_PARTY,
  REUNION_HAMMET_NAMES_DODONPA,
  REUNION_DODONPA_INTERRUPTS,
  REUNION_DODONPA_MOCKS_IVAN,
  REUNION_DODONPA_SCORNS_CHILD,
  REUNION_DODONPA_MARVELS_AT_KIDS,
  REUNION_GARET_TAUNTS_DODONPA,
  REUNION_IVAN_ACCUSES_DODONPA,
  REUNION_HAMMET_LEARNS_OF_EXTORTION,
  REUNION_MIA_TAKES_HAMMET_HOME,
  REUNION_DODONPA_REFUSES,
  REUNION_DODONPA_CALLS_HAMMET_MONEY_TREE,
  REUNION_GARET_CALLS_DODONPA_SCOUNDREL,
  REUNION_DODONPA_KEEPS_HAMMET,
  REUNION_DODONPA_PULLS_LEVER,
  REUNION_DODONPA_SPRINGS_TRAP
};

/// The talk over Dodonpa pinned under his beaten monster
/// (LunpaFortress_DonpaJudgesDodonpa), in order.
enum AftermathMessage {
  AFTERMATH_DODONPA_CRIES_OUT,
  AFTERMATH_GARET_ASKS_HOW_DODONPA_WAS_TRAPPED,
  AFTERMATH_HAMMET_TELLS_OF_TRICK,
  AFTERMATH_HAMMET_EXPLAINS_TRICK,
  AFTERMATH_DODONPA_BOASTS,
  AFTERMATH_IVAN_CALLS_DODONPA_SNAKE,
  AFTERMATH_MIA_CALLS_DODONPA_ROTTEN,
  AFTERMATH_DODONPA_APOLOGIZES,
  AFTERMATH_GARET_REPEATS_DEMAND,
  AFTERMATH_IVAN_DOUBTS_STRENGTH,
  AFTERMATH_MIA_ASKS_TO_MOVE_MONSTER,
  AFTERMATH_MIA_ASKS_HAMMET,
  AFTERMATH_HAMMET_WILL_TRY
};

/// The party's debate over Dodonpa's punishment, until Donpa arrives
/// (LunpaFortress_DonpaJudgesDodonpa), in order.
enum PunishmentMessage {
  PUNISHMENT_GARET_FREES_DODONPA,
  PUNISHMENT_DODONPA_CANNOT_STAND,
  PUNISHMENT_MIA_ASKS_ABOUT_LEG,
  PUNISHMENT_DODONPA_WILL_STAND,
  PUNISHMENT_IVAN_IS_RELIEVED,
  PUNISHMENT_GARET_QUESTIONS_RELIEF,
  PUNISHMENT_GARET_RECALLS_ATTACK,
  PUNISHMENT_HAMMET_FORGIVES,
  PUNISHMENT_GARET_RECALLS_KIDNAPPING,
  PUNISHMENT_HAMMET_REMEMBERS,
  PUNISHMENT_GARET_LOSES_PATIENCE,
  PUNISHMENT_IVAN_POINTS_TO_SAFETY,
  PUNISHMENT_GARET_DOUBTS_REFORM,
  PUNISHMENT_MIA_WONDERS,
  PUNISHMENT_MIA_ASKS_DODONPA,
  PUNISHMENT_HAMMET_DEFENDS_DODONPA,
  PUNISHMENT_GARET_DISTRUSTS_DODONPA,
  PUNISHMENT_IVAN_ASKS_GARET,
  PUNISHMENT_GARET_WANTS_DODONPA_LOCKED_UP,
  PUNISHMENT_DODONPA_PLEADS,
  PUNISHMENT_MIA_SCOLDS_DODONPA,
  PUNISHMENT_IVAN_ASKS_TO_LOCK_DODONPA_UP,
  PUNISHMENT_DONPA_APPROVES,
  PUNISHMENT_DONPA_LEAVES_DODONPA
};

/// Donpa's judgment of his son and his request to the party
/// (LunpaFortress_DonpaJudgesDodonpa), in order.
enum JudgmentMessage {
  JUDGMENT_DODONPA_CALLS_FATHER,
  JUDGMENT_DONPA_PITIES_DODONPA,
  JUDGMENT_DONPA_RECALLS_WARNING,
  JUDGMENT_DONPA_DEMANDS_AMENDS,
  JUDGMENT_DONPA_REGRETS_SPOILING,
  JUDGMENT_DODONPA_BLAMES_DONPA,
  JUDGMENT_DONPA_WILL_PUNISH,
  JUDGMENT_DODONPA_DOUBTS_DONPA,
  JUDGMENT_DONPA_INSISTS,
  JUDGMENT_DONPA_SENTENCES_DODONPA,
  JUDGMENT_DONPA_ASKS_ABOUT_TROUBLE,
  JUDGMENT_HAMMET_DENIES_TROUBLE,
  JUDGMENT_DONPA_APOLOGIZES,
  JUDGMENT_HAMMET_DECLINES_APOLOGY,
  JUDGMENT_DONPA_ASKS_FORGIVENESS,
  JUDGMENT_DONPA_HAS_REQUEST,
  JUDGMENT_HAMMET_ASKS_ABOUT_REQUEST,
  JUDGMENT_DONPA_ASKS_PARTY_TO_LEAVE,
  JUDGMENT_GARET_QUESTIONS_SECRECY,
  JUDGMENT_IVAN_QUESTIONS_SNEAKING,
  JUDGMENT_DONPA_WARNS_OF_HENCHMEN,
  JUDGMENT_DONPA_IS_ASHAMED,
  JUDGMENT_DONPA_WARNS_OF_ATTACK,
  JUDGMENT_DONPA_FEARS_CELL_CHECK,
  JUDGMENT_HAMMET_FORESEES_RELEASE,
  JUDGMENT_DONPA_FORESEES_REVENGE,
  JUDGMENT_GARET_AGREES,
  JUDGMENT_MIA_FEARS_FOR_KALAY,
  JUDGMENT_DONPA_URGES_HASTE,
  JUDGMENT_IVAN_ASKS_IF_NOW,
  JUDGMENT_DONPA_SPREAD_RUMORS,
  JUDGMENT_HAMMET_ASKS_ABOUT_RUMORS,
  JUDGMENT_DONPA_TELLS_RUMOR,
  JUDGMENT_GARET_ASKS_WHY,
  JUDGMENT_DONPA_EXPLAINS_RUMOR,
  JUDGMENT_MIA_PRAISES_DONPA,
  JUDGMENT_DONPA_OFFERS_HELP,
  JUDGMENT_IVAN_URGES_ESCAPE,
  JUDGMENT_DONPA_HURRIES_PARTY,
  JUDGMENT_DONPA_PROMISES_CHANGE,
  JUDGMENT_DODONPA_PROMISES_TO_BEHAVE
};

/// The map layers the dark halls show (MapGrid_SetShownLayers): the ground,
/// the walls, and MAPGRID_LAYER_OVERHEAD with the circles of light over them.
#define LUNPA_FORTRESS_PASSAGE_LAYERS                                          \
  (DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON)
/// The map layers the lit rooms show: the ground and the walls, without the
/// circles of light.
#define LUNPA_FORTRESS_ROOM_LAYERS (DISPCNT_BG2_ON | DISPCNT_BG3_ON)

/// Where the west hall's pair have their circles of light on the overhead
/// layer, 136 pixels across; the layer scrolls by the first guard's distance
/// from it, so the circles walk with the pair.
#define PAIRED_PATROL_LAYER_ORIGIN_X (136.0 * FIXED_ONE)

static s32 LunpaFortress_TryTakeChestItem(s32 actorId, s32 itemId);
static void LunpaFortress_CheckSecondEntranceSentry(void);
static s32 LunpaFortress_CanGuardSeeLeader(s32 actorId);
static s32 LunpaFortress_IsLeaderNearGuard(s32 actorId);
static s32 LunpaFortress_IsGuardNearViewCenter(s32 actorId);
static void LunpaFortress_GuardThrowsPartyOut(s32 actorId);
static void LunpaFortress_CryIntruder(s32 actorId);
static void LunpaFortress_AskWhosThat(s32 actorId);
static void LunpaFortress_AskIfShiftIsUp(s32 actorId);
static void LunpaFortress_CallPartyScoundrel(s32 actorId);
static void LunpaFortress_InitializeGroundFloor(void);
static void LunpaFortress_InitializeUpperFloor(void);
static void LunpaFortress_InitializeCave(void);
static void LunpaFortress_RestoreGroundFloorDoors(void);
static void LunpaFortress_RestoreUpperFloorDoors(void);
static void LunpaFortress_RestoreCaveDoors(void);
static void LunpaFortress_RestoreToadonpaDoors(void);

#include "shared/staged_actor.c"

/// Donpa has judged Dodonpa, so Hammet's cell no longer starts the trap.
#define FLAG_LUNPA_FORTRESS_DODONPA_JUDGED FLAG_AREA_TEMP_01
/// Donpa's attendant has told the party of Donpa and his spoiled son.
#define FLAG_LUNPA_FORTRESS_ATTENDANT_TOLD_OF_DONPA FLAG_AREA_TEMP_13
/// Donpa's attendant has shown the party his reward, Tonic, and stands at the
/// door to its room.
#define FLAG_LUNPA_FORTRESS_REWARD_SHOWN FLAG_AREA_TEMP_14
/// A guard has spotted the party.
#define FLAG_LUNPA_FORTRESS_PARTY_SPOTTED FLAG_SCENE_TEMP_14
/// Unused: set beside FLAG_LUNPA_FORTRESS_PARTY_SPOTTED whenever a guard sees
/// the leader, and never read.
#define FLAG_LUNPA_FORTRESS_UNUSED_SPOTTED FLAG_SCENE_TEMP_15
/// The cave's first switch is still hidden by its ivy.
#define FLAG_LUNPA_FORTRESS_FIRST_SWITCH_UNDER_IVY FLAG_SCENE_TEMP_17
/// The cave's second switch is still hidden by its ivy.
#define FLAG_LUNPA_FORTRESS_SECOND_SWITCH_UNDER_IVY FLAG_SCENE_TEMP_18
/// The entrance guard has thrown the party out.
#define FLAG_LUNPA_FORTRESS_ENTRANCE_GUARD_CAUGHT_PARTY FLAG_SCENE_TEMP_24
/// The sentries have spotted the party and thrown it out.
#define FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY FLAG_SCENE_TEMP_25
/// The startled old man has promised to keep quiet.
#define FLAG_LUNPA_FORTRESS_OLD_MAN_PROMISED_SILENCE FLAG_SCENE_TEMP_26
/// The startled old man has been startled once.
#define FLAG_LUNPA_FORTRESS_OLD_MAN_STARTLED FLAG_SCENE_TEMP_27
/// LunpaFortress_FindHammet is under way.
#define FLAG_LUNPA_FORTRESS_FINDING_HAMMET FLAG_SCENE_TEMP_70

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_LUNPA_FORTRESS_GROUND_FLOOR) {
    return gLunpaFortressGroundFloorEntrances;
  }
  if (sceneId == SCENE_LUNPA_FORTRESS_UPPER_FLOOR) {
    return gLunpaFortressUpperFloorEntrances;
  }
  if (sceneId == SCENE_LUNPA_FORTRESS_CAVE) {
    return gLunpaFortressCaveEntrances;
  }
  return gLunpaFortressEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_LUNPA_FORTRESS_UPPER_FLOOR) {
    return gLunpaFortressUpperFloorExits;
  }
  if (sceneId == SCENE_LUNPA_FORTRESS_CAVE ||
      sceneId == SCENE_LUNPA_FORTRESS_TOADONPA) {
    return gLunpaFortressCaveAndToadonpaExits;
  }
  return gLunpaFortressExitsOther;
}

/// The guards, the other people and the objects of each scene.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineState.next;

  // Unused: this overlay never runs for West Lunpa Cave, which has its own.
  if (sceneId == SCENE_WEST_LUNPA_CAVE) {
    return gLunpaFortressUnusedWestLunpaCaveActorPlacements;
  }
  if (sceneId == SCENE_LUNPA_FORTRESS_CAVE) {
    return gLunpaFortressCaveActorPlacements;
  }
  if (sceneId == SCENE_LUNPA_FORTRESS_UPPER_FLOOR) {
    return gLunpaFortressUpperFloorActorPlacements;
  }
  if (sceneId == SCENE_LUNPA_FORTRESS_GROUND_FLOOR) {
    return gLunpaFortressGroundFloorActorPlacements;
  }
  if (sceneId == SCENE_LUNPA_FORTRESS_TOADONPA) {
    return gLunpaFortressToadonpaActorPlacements;
  }
  return gLunpaFortressActorPlacementsOther;
}

/// What the party can talk to, search and use Psynergy on, by scene.
const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_LUNPA_FORTRESS_GROUND_FLOOR) {
    return gLunpaFortressGroundFloorTriggers;
  }
  if (sceneId == SCENE_LUNPA_FORTRESS_UPPER_FLOOR) {
    return gLunpaFortressUpperFloorTriggers;
  }
  if (sceneId == SCENE_LUNPA_FORTRESS_CAVE) {
    return gLunpaFortressCaveTriggers;
  }
  return gLunpaFortressTriggersOther;
}

/// The crates settle in the gap at the foot of the cave's middle corridor, at
/// cell (15, 23), as a step the party can walk across down to the ledge below.
///
/// The ledge cell at the corridor's end, (15, 22), takes the walkable floor of
/// (15, 20), and (15, 23) to (15, 25) take the cells of (17, 23) to (17, 25);
/// the crates are drawn underfoot from then on.
static void LunpaFortress_LowerCrates(void) {
  struct Actor *cratesActor;

  MapGrid_CopyCellAttributes(15, 20, 1, 1, 15, 22);
  MapGrid_CopyCellAttributes(17, 23, 1, 3, 15, 23);
  cratesActor = Actor_Get(ACTOR_CAVE_CRATES);
  if (cratesActor != NULL) {
    Actor_SetSpriteFlags(cratesActor, 0);
    cratesActor->motionFlags = 0;
    cratesActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
}

/// Walking into the stack of crates in the cave pushes it one cell along: out
/// of its nook behind the middle cell door, through the room below and down the
/// corridor to the gap at its foot.
///
/// The floor of the nook, the doorway, the room and the corridor is
/// CELL_BEHAVIOR_LUNPA_FORTRESS_CRATES_PATH; standing there and pushing runs
/// PushedActor_AdvancePair, which slides the crates and Isaac together.
static void LunpaFortress_PushCrates(void) { PushedActor_AdvancePair(); }

/// Pushing the crates on from the end of the cave's middle corridor, at
/// cell (15, 21), drops them into the gap at its foot, where they settle as a
/// step and Isaac can walk on down over them.
static void LunpaFortress_CratesSink(void) {
  Actor_SetSpeed(ACTOR_CAVE_CRATES, 1);
  Actor_SetDestination(ACTOR_CAVE_CRATES, 248, 376);
  Actor_WaitForMoveById(ACTOR_CAVE_CRATES);
  Sound_Play(SE_SPLASH_SMALL);
  Event_Wait(60);
  LunpaFortress_LowerCrates();
  GameFlag_Set(FLAG_LUNPA_FORTRESS_CRATES_MOVED);
}

/// Casting Move on the stack of crates in the cave: once they stand over the
/// gap at the foot of the middle corridor, the cave shakes, they clatter down
/// and settle in it as when pushed. Anywhere else nothing happens but Move's
/// hand pushing at them.
///
/// They count as over the gap once they stand south of row 22.
static void LunpaFortress_MoveCrates(void) {
  struct Actor *cratesActor;

  if (FIELD_CELL(Actor_Get(ACTOR_CAVE_CRATES)->z.fixed) > 22) {
    MapGrid_SetShake(4, 4, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    Sound_Play(SE_CLATTER);
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

/// On a return to the cave, the crates are back in the gap where they settled.
static void LunpaFortress_RestoreLoweredCrates(void) {
  Actor_SetPosition(ACTOR_CAVE_CRATES, 248, 376);
  LunpaFortress_LowerCrates();
}

/// Draws one of the cells' doors open.
static void LunpaFortress_OpenToadonpaDoor(s32 doorIndex) {
  s32 doorCellX = gLunpaFortressToadonpaDoorCells[doorIndex * 2];
  s32 doorCellY = gLunpaFortressToadonpaDoorCells[doorIndex * 2 + 1];

  MapGrid_CopyCells(0, 77, 1, 3, doorCellX, doorCellY);
  MapGrid_CopyCells(1, 77, 1, 1, doorCellX + 1, doorCellY);
  MapGrid_CopyCells(doorCellX, doorCellY - 48, 1, 1, doorCellX, doorCellY - 46);
}

/// Walking up to one of the cells' doors with the Cell Key in the party opens
/// it, and the screen shakes. These are the doors of the three cells in a row
/// and the door of Hammet's cell; after Donpa's judgment Dodonpa is behind that
/// last one, and it stays shut.
///
/// The doors are listed in gLunpaFortressToadonpaDoorCells, east cell first;
/// the fourth, at cell (28, 39), lies outside both rooms, where the party never
/// goes.
static void LunpaFortress_UnlockToadonpaDoor(void) {
  struct FieldState *state;
  s32 doorTrigger;
  s32 doorIndex;

  state = gFieldState;
  if (PartyInventory_FindMember(ITEM_CELL_KEY) != -1) {
    doorTrigger = state->stepTrigger;
    doorIndex = doorTrigger - CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR;
    if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) == 0 || doorIndex != 4) {
      LunpaFortress_OpenToadonpaDoor(doorIndex);
      Sound_Play(SE_GATE_MOVE);
      MapGrid_SetShake(3, 3, 1);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      GameFlag_Set(doorTrigger + (TOADONPA_DOOR_FLAG -
                                  CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR));
    }
  }
}

/// Draws one of the cave's cell doors open and lets the party through. The
/// middle door, to the crates' nook, opens two cells.
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

/// Walking up to one of the cave's three cell doors with the Cell Key in the
/// party opens it, and the screen shakes. The doors stand in the north wall of
/// the ledge above the lake: the east one at cell (31, 17), the middle one, to
/// the crates, at cell (15, 13), and the west one, to the chamber of the second
/// switch, at cell (6, 17).
static void LunpaFortress_UnlockCaveDoor(void) {
  struct FieldState *state;
  s32 doorTrigger;

  state = gFieldState;
  if (PartyInventory_FindMember(ITEM_CELL_KEY) != -1) {
    doorTrigger = state->stepTrigger;
    LunpaFortress_OpenCaveDoor(doorTrigger -
                               CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR);
    Sound_Play(SE_GATE_MOVE);
    MapGrid_SetShake(3, 3, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    GameFlag_Set(doorTrigger +
                 (CAVE_DOOR_FLAG - CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR));
  }
}

/// Draws one of the upper floor's two cell doors open.
static void LunpaFortress_OpenUpperFloorDoor(s32 doorIndex) {
  s32 doorCellX = gLunpaFortressUpperFloorDoorCells[doorIndex * 2];
  s32 doorCellY = gLunpaFortressUpperFloorDoorCells[doorIndex * 2 + 1];

  MapGrid_CopyCells(55, 121, 1, 3, doorCellX, doorCellY);
  MapGrid_CopyCells(56, 121, 1, 1, doorCellX + 1, doorCellY);
  MapGrid_CopyCells(doorCellX, doorCellY - 63, 1, 1, doorCellX, doorCellY - 62);
}

/// Walking up to the north cell door of the Cell Key room, at cell (16, 51),
/// with the Cell Key in the party opens it, and the screen shakes.
/// LunpaFortress_CheckUpperFloorDoorApproach opens the room's south door, at
/// cell (16, 57), the same way.
static void LunpaFortress_UnlockUpperFloorDoor(void) {
  struct FieldState *state;
  s32 doorTrigger;

  state = gFieldState;
  if (PartyInventory_FindMember(ITEM_CELL_KEY) != -1) {
    doorTrigger = state->stepTrigger;
    LunpaFortress_OpenUpperFloorDoor(doorTrigger -
                                     CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR);
    Sound_Play(SE_GATE_MOVE);
    MapGrid_SetShake(3, 3, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    GameFlag_Set(doorTrigger + (UPPER_FLOOR_DOOR_FLAG -
                                CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR));
  }
}

/// Draws one of the ground floor's cell doors open and lets the party through.
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

/// Walking up to the cell door in the hall at the top of the ground floor, at
/// cell (28, 10), from either side with the Cell Key in the party opens it, and
/// the screen shakes. LunpaFortress_CheckGroundFloorDoorApproach opens the east
/// hall's door to the room of chests, at cell (43, 30), the same way.
static void LunpaFortress_UnlockGroundFloorDoor(void) {
  struct FieldState *state;
  s32 doorTrigger;

  state = gFieldState;
  if (PartyInventory_FindMember(ITEM_CELL_KEY) != -1) {
    doorTrigger = state->stepTrigger;
    LunpaFortress_OpenGroundFloorDoor(doorTrigger -
                                      CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR);
    Sound_Play(SE_GATE_MOVE);
    MapGrid_SetShake(3, 3, 1);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    GameFlag_Set(doorTrigger + (GROUND_FLOOR_DOOR_FLAG -
                                CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR));
  }
}

/// Searching any cell door of the fortress without the Cell Key shows "The door
/// is tightly locked."
static void LunpaFortress_SearchLockedDoor(void) {
  if (PartyInventory_FindMember(ITEM_CELL_KEY) == -1) {
    FieldMessage_Show(MsgFieldDoorTightlyLocked, 1);
  }
}

/// Searching one of the cave's two sealed doorways shows "The door is tightly
/// locked." No key opens them: one is in the middle of the north wall of the
/// room at the foot of the stairs, at cell (21, 34), the other in the small
/// north-west chamber, at cell (6, 10), and each opens to its hidden switch.
static void LunpaFortress_SearchSealedDoor(void) {
  FieldMessage_Show(MsgFieldDoorTightlyLocked, 1);
}

// XXX (no evidence): which scene's actors these are; the pickups are
// unused and name no scene, and LunpaFortressGroundFloor's first four rows (in
// a chest's sprite, at the map's origin) are the only rows nothing else uses.
/// The four chests from ACTOR_FIRST_SWORD_CHEST that the unused sword pickups
/// open, in a scene they do not name.
enum { ACTOR_FIRST_SWORD_CHEST = ACTOR_FIRST_PLACED };

/// Unused: nothing calls this pickup or the three after it.
///
/// Each opens one of four chests (ACTOR_FIRST_SWORD_CHEST on) and gives its
/// sword, showing the picture before the leader. With no room the chest peeks
/// and shuts. Each sets a flag nothing tests.
///
/// This one gives the Arctic Blade, from the first chest.
static void UNUSED LunpaFortress_UnusedTakeArcticBlade(void) {
  if (LunpaFortress_TryTakeChestItem(ACTOR_FIRST_SWORD_CHEST,
                                     ITEM_ARCTIC_BLADE) != 0) {
    GameFlag_Set(FLAG_LUNPA_FORTRESS_ARCTIC_BLADE);
  }
}

/// Unused: the Masamune, from the second chest.
static void UNUSED LunpaFortress_UnusedTakeMasamune(void) {
  if (LunpaFortress_TryTakeChestItem(ACTOR_FIRST_SWORD_CHEST + 1,
                                     ITEM_MASAMUNE) != 0) {
    GameFlag_Set(FLAG_LUNPA_FORTRESS_MASAMUNE);
  }
}

/// Unused: the Silver Blade, from the third chest.
static void UNUSED LunpaFortress_UnusedTakeSilverBlade(void) {
  if (LunpaFortress_TryTakeChestItem(ACTOR_FIRST_SWORD_CHEST + 2,
                                     ITEM_SILVER_BLADE) != 0) {
    GameFlag_Set(FLAG_LUNPA_FORTRESS_SILVER_BLADE);
  }
}

/// Unused: the Shamshir, from the fourth chest.
static void UNUSED LunpaFortress_UnusedTakeShamshir(void) {
  if (LunpaFortress_TryTakeChestItem(ACTOR_FIRST_SWORD_CHEST + 3,
                                     ITEM_SHAMSHIR) != 0) {
    GameFlag_Set(FLAG_LUNPA_FORTRESS_SHAMSHIR);
  }
}

/// Unused: called only by the four pickups above.
static s32 LunpaFortress_TryTakeChestItem(s32 actorId, s32 itemId) {
  s32 itemGiven = 0;
  s32 itemEffect;

  Event_Start();
  itemEffect = Chest_OpenAndShowItem(0, actorId, itemId);
  if (Party_GiveItem(itemId, 0) != -1) {
    Actor_SetAnimation(actorId, CHEST_ANIMATION_OPEN_EMPTY);
    itemGiven = 1;
  } else {
    Sound_Play(SE_CHEST_CLOSE);
    Actor_SetAnimation(actorId, CHEST_ANIMATION_PEEK_AND_SHUT);
  }
  Actor_Release((struct Actor *)itemEffect);
  Event_End();
  return itemGiven;
}

static void UNUSED LunpaFortress_DoNothingA(void) {}

static void UNUSED LunpaFortress_DoNothingB(void) {}

static void UNUSED LunpaFortress_DoNothingC(void) {}

static void UNUSED LunpaFortress_DoNothingD(void) {}

/// Isaac takes the Cell Key, which lies on a stool just inside the north cell
/// door of the Cell Key room, on the upper floor. Catch on it from the passage
/// outside the door, or talking to it, walks him up to the door at
/// cell (16, 49), turns him to face the key, and he holds it up while a window
/// says he got the Cell Key.
///
/// Talked to from inside the room, the same walk takes him north into the shut
/// door, where he stands on top of it while he holds the key up. The key is put
/// out of sight once taken, and FLAG_LUNPA_FORTRESS_CELL_KEY keeps it gone.
static void LunpaFortress_TakeCellKey(void) {
  Actor_WalkTo(ACTOR_PARTY_LEADER, 264, 792);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Item_ShowFound(ITEM_CELL_KEY, 3);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Party_GiveItem(ITEM_CELL_KEY, 0);
  GameFlag_Set(FLAG_LUNPA_FORTRESS_CELL_KEY);
  Actor_SetPosition(ACTOR_UPPER_FLOOR_CELL_KEY, 0, 0);
}

/// Where the pair in the upper floor's west hall cannot see Isaac: the top of
/// the hall, rows 5 to 7 west of column 11, and the foot of it by the stairs,
/// columns 8 and 9 below row 22.
static s32 LunpaFortress_IsLeaderHiddenFromPatrolPair(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 leaderRow = leader->z.fixed / toFIXED(16);
  s32 leaderColumn = leader->x.fixed / toFIXED(16);

  if (leaderRow >= 5 && leaderRow <= 7 && leaderColumn <= 10) {
    return 1;
  }
  if (leaderColumn >= 8 && leaderColumn <= 9 && leaderRow > 22) {
    return 1;
  }
  return 0;
}

/// Every frame in the upper floor's west hall: walks the two patrolling guards'
/// circles of light with them, and raises RAISED_PATROL_PAIR_SPOTTED when
/// either sees an uncloaked Isaac.
///
/// A cloaked Isaac who comes within four cells of either loses Cloak instead.
static void LunpaFortress_CheckPatrolPair(void) {
  struct Actor *firstGuard = Actor_Get(ACTOR_UPPER_FLOOR_FIRST_PATROL);
  struct Actor *secondGuard = Actor_Get(ACTOR_UPPER_FLOOR_SECOND_PATROL);
  struct MapGridLayerScroll *patrolLayer =
      &((struct MapGridScrollState *)gMapGridState[0])
           ->layers[MAPGRID_LAYER_OVERHEAD];
  struct FieldState *state = gFieldState;

  if (gFrameCount & 1) {
    patrolLayer->speedX = 1;
    patrolLayer->speedY = 1;
  } else {
    patrolLayer->speedX = -1;
    patrolLayer->speedY = -1;
  }
  if (GameFlag_Get(FLAG_MENU_OPEN) || state->psynergy != 0 ||
      state->item != 0) {
    firstGuard->paused = 1;
    secondGuard->paused = 1;
  } else if (!Event_GetFlag(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED)) {
    firstGuard->paused = 0;
    secondGuard->paused = 0;
    if (!GameFlag_Get(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED) &&
        firstGuard->paused == 0) {
      patrolLayer->phaseX = PAIRED_PATROL_LAYER_ORIGIN_X - firstGuard->x.fixed;
    }
    if (!LunpaFortress_IsLeaderHiddenFromPatrolPair()) {
      if (gMainRoutineState.cloaked != 0) {
        if (LunpaFortress_IsLeaderNearGuard(ACTOR_UPPER_FLOOR_FIRST_PATROL) &&
            gMainRoutineState.cloaked != 0) {
          state->psynergy = CLOAK_END;
          return;
        }
        if (LunpaFortress_IsLeaderNearGuard(ACTOR_UPPER_FLOOR_SECOND_PATROL) &&
            gMainRoutineState.cloaked != 0) {
          state->psynergy = CLOAK_END;
          return;
        }
      }
      if (gMainRoutineState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(ACTOR_UPPER_FLOOR_FIRST_PATROL)) {
          GameFlag_Set(FLAG_LUNPA_FORTRESS_UNUSED_SPOTTED);
          GameFlag_Set(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED);
        }
        if (LunpaFortress_CanGuardSeeLeader(ACTOR_UPPER_FLOOR_SECOND_PATROL)) {
          GameFlag_Set(FLAG_LUNPA_FORTRESS_UNUSED_SPOTTED);
          GameFlag_Set(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED);
        }
      }
      if (GameFlag_Get(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED)) {
        state->raisedTrigger = RAISED_PATROL_PAIR_SPOTTED;
      }
    }
  }
}

/// When the pair patrolling the upper floor's west hall see Isaac, both stop;
/// the northern one stares, and the southern one throws the party out to the
/// Lunpa Fortress Gates.
static void LunpaFortress_PatrolPairThrowsPartyOut(void) {
  Event_Start();
  Actor_Stop(ACTOR_UPPER_FLOOR_FIRST_PATROL);
  Actor_SetDestinationOffset(ACTOR_UPPER_FLOOR_FIRST_PATROL, 0, 0);
  Actor_SetAnimation(ACTOR_UPPER_FLOOR_FIRST_PATROL, ANIM_STILL);
  Actor_FaceActor(ACTOR_UPPER_FLOOR_FIRST_PATROL, ACTOR_PARTY_LEADER, 0);
  Actor_ShowEmote(ACTOR_UPPER_FLOOR_FIRST_PATROL,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  LunpaFortress_GuardThrowsPartyOut(ACTOR_UPPER_FLOOR_SECOND_PATROL);
  Event_End();
}

/// Whether Isaac and the east hall's patrol are in its two parallel north-south
/// corridors, columns 52 and 57, one each, where the guard passes without
/// seeing him.
static s32 LunpaFortress_IsLeaderPassingHallPatrol(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  struct Actor *guard = Actor_Get(ACTOR_UPPER_FLOOR_HALL_PATROL);
  s32 leaderColumn = leader->x.fixed / toFIXED(16);
  s32 guardRow = guard->z.fixed / toFIXED(16);
  s32 guardColumn = guard->x.fixed / toFIXED(16);

  if (leaderColumn == 52 && guardColumn == 57 && guardRow > 34 &&
      guardRow <= 40) {
    return 1;
  }
  if (leaderColumn == 57 && guardColumn == 52 && guardRow > 34 &&
      guardRow <= 40) {
    return 1;
  }
  return 0;
}

/// Every frame in the upper floor's east hall: as
/// LunpaFortress_CheckPatrolPair, for the guard who walks its rounds.
static void LunpaFortress_CheckHallPatrol(void) {
  struct Actor *guard = Actor_Get(ACTOR_UPPER_FLOOR_HALL_PATROL);
  struct MapGridLayerScroll *patrolLayer =
      &((struct MapGridScrollState *)gMapGridState[0])
           ->layers[MAPGRID_LAYER_OVERHEAD];
  struct FieldState *state = gFieldState;

  FieldCamera_GetActor();
  if (gFrameCount & 1) {
    patrolLayer->speedX = 1;
    patrolLayer->speedY = 1;
  } else {
    patrolLayer->speedX = -1;
    patrolLayer->speedY = -1;
  }
  if (GameFlag_Get(FLAG_MENU_OPEN) || state->psynergy != 0 ||
      state->item != 0) {
    guard->paused = 1;
  } else if (!Event_GetFlag(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED)) {
    guard->paused = 0;
    if (!GameFlag_Get(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED) &&
        guard->paused == 0) {
      patrolLayer->phaseX = toFIXED(832) - guard->x.fixed;
      patrolLayer->phaseY = toFIXED(576) - guard->z.fixed;
    }
    if (!LunpaFortress_IsLeaderPassingHallPatrol()) {
      LunpaFortress_IsGuardNearViewCenter(ACTOR_UPPER_FLOOR_HALL_PATROL);
      if (LunpaFortress_IsLeaderNearGuard(ACTOR_UPPER_FLOOR_HALL_PATROL) &&
          gMainRoutineState.cloaked != 0) {
        state->psynergy = CLOAK_END;
        return;
      }
      if (gMainRoutineState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(ACTOR_UPPER_FLOOR_HALL_PATROL)) {
          GameFlag_Set(FLAG_LUNPA_FORTRESS_UNUSED_SPOTTED);
          GameFlag_Set(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED);
        }
      }
      if (GameFlag_Get(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED)) {
        state->raisedTrigger = RAISED_HALL_PATROL_SPOTTED;
      }
    }
  }
}

/// When the guard walking the upper floor's east hall sees Isaac, he stops and
/// throws the party out to the Lunpa Fortress Gates.
static void LunpaFortress_HallPatrolThrowsPartyOut(void) {
  LunpaFortress_GuardThrowsPartyOut(ACTOR_UPPER_FLOOR_HALL_PATROL);
  Event_End();
}

/// Where the guard walking the ground floor's east hall cannot see Isaac: three
/// stretches of his rounds, columns 41 to 44 in rows 26 to 28, column 41 in
/// rows 38 to 41, and columns 54 to 56 in rows 31 to 40.
static s32 LunpaFortress_IsLeaderHiddenFromGroundFloorPatrol(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 leaderRow = leader->z.fixed / toFIXED(16);
  s32 leaderColumn = leader->x.fixed / toFIXED(16);

  if (leaderColumn >= 41 && leaderColumn <= 44 && leaderRow > 25 &&
      leaderRow <= 28) {
    return 1;
  }
  if (leaderColumn == 41 && leaderRow > 37 && leaderRow <= 41) {
    return 1;
  }
  if (leaderColumn >= 54 && leaderColumn <= 56 && leaderRow > 30 &&
      leaderRow <= 40) {
    return 1;
  }
  return 0;
}

/// Every frame in the ground floor's east hall: as
/// LunpaFortress_CheckPatrolPair, for the guard who walks its rounds.
static void LunpaFortress_CheckGroundFloorPatrol(void) {
  struct Actor *guard = Actor_Get(ACTOR_GROUND_FLOOR_PATROL_GUARD);
  struct MapGridLayerScroll *patrolLayer =
      &((struct MapGridScrollState *)gMapGridState[0])
           ->layers[MAPGRID_LAYER_OVERHEAD];
  struct FieldState *state = gFieldState;

  if (gFrameCount & 1) {
    patrolLayer->speedX = 1;
    patrolLayer->speedY = 1;
  } else {
    patrolLayer->speedX = -1;
    patrolLayer->speedY = -1;
  }
  if (GameFlag_Get(FLAG_MENU_OPEN) || state->psynergy != 0 ||
      state->item != 0) {
    guard->paused = 1;
  } else if (!Event_GetFlag(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED)) {
    guard->paused = 0;
    if (!GameFlag_Get(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED) &&
        guard->paused == 0) {
      patrolLayer->phaseX = toFIXED(752) - guard->x.fixed;
      patrolLayer->phaseY = toFIXED(496) - guard->z.fixed;
    }
    if (!LunpaFortress_IsLeaderHiddenFromGroundFloorPatrol()) {
      if (LunpaFortress_IsLeaderNearGuard(ACTOR_GROUND_FLOOR_PATROL_GUARD) &&
          gMainRoutineState.cloaked != 0) {
        state->psynergy = CLOAK_END;
        return;
      }
      if (gMainRoutineState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(ACTOR_GROUND_FLOOR_PATROL_GUARD)) {
          GameFlag_Set(FLAG_LUNPA_FORTRESS_UNUSED_SPOTTED);
          GameFlag_Set(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED);
        }
      }
      if (GameFlag_Get(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED)) {
        state->raisedTrigger = RAISED_GROUND_FLOOR_PATROL_SPOTTED;
      }
    }
  }
}

/// When the guard walking the ground floor's east hall sees Isaac, he stops and
/// throws the party out to the Lunpa Fortress Gates.
static void LunpaFortress_GroundFloorPatrolThrowsPartyOut(void) {
  LunpaFortress_GuardThrowsPartyOut(ACTOR_GROUND_FLOOR_PATROL_GUARD);
  Event_End();
}

/// Whether Isaac is anywhere but rows 15 and 16 east of column 45, the stretch
/// of the ground floor's north-east hall its guard watches.
static s32 LunpaFortress_IsLeaderOutsideNortheastHall(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 leaderRow = leader->z.fixed / toFIXED(16);
  s32 leaderColumn = leader->x.fixed / toFIXED(16);

  if (leaderColumn > 45 && leaderRow > 14 && leaderColumn <= 64 &&
      leaderRow <= 16) {
    return 0;
  }
  return 1;
}

/// Every frame in the ground floor's north-east hall: raises
/// RAISED_NORTHEAST_HALL_SPOTTED when its guard sees an uncloaked Isaac in the
/// stretch he watches.
static void LunpaFortress_CheckNortheastHallGuard(void) {
  struct FieldState *state;

  state = gFieldState;
  if (Event_GetFlag(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED) == 0) {
    if (LunpaFortress_IsLeaderOutsideNortheastHall() == 0) {
      if (gMainRoutineState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(
                ACTOR_GROUND_FLOOR_NORTHEAST_HALL_GUARD) != 0) {
          GameFlag_Set(FLAG_LUNPA_FORTRESS_UNUSED_SPOTTED);
          GameFlag_Set(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED);
        }
      }
      if (GameFlag_Get(FLAG_LUNPA_FORTRESS_PARTY_SPOTTED) != 0) {
        state->raisedTrigger = RAISED_NORTHEAST_HALL_SPOTTED;
      }
    }
  }
}

/// When the guard in the ground floor's winding north-east hall sees Isaac, he
/// throws the party out to the Lunpa Fortress Gates.
static void LunpaFortress_NortheastHallGuardThrowsPartyOut(void) {
  LunpaFortress_GuardThrowsPartyOut(ACTOR_GROUND_FLOOR_NORTHEAST_HALL_GUARD);
  Event_End();
}

/// Every frame after the party comes into the ground floor's long hall, or the
/// small room inside the door: raises RAISED_ENTRANCE_GUARD_SPOTTED once, when
/// the guard at the hall's west end sees an uncloaked Isaac.
static void LunpaFortress_CheckEntranceGuard(void) {
  struct FieldState *state = gFieldState;

  if (LunpaFortress_CanGuardSeeLeader(ACTOR_GROUND_FLOOR_ENTRANCE_GUARD) != 0 &&
      gMainRoutineState.cloaked == 0) {
    s16 *raisedTrigger;

    Task_Remove(LunpaFortress_CheckEntranceGuard);
    raisedTrigger = &state->raisedTrigger;
    Io_Write16(RAISED_ENTRANCE_GUARD_SPOTTED, raisedTrigger);
  }
}

/// Every frame after the party comes into the ground floor's long hall, or the
/// small room inside the door: raises RAISED_SENTRIES_SPOTTED once for both
/// sentries, when the first of them sees an uncloaked Isaac.
static void LunpaFortress_CheckFirstEntranceSentry(void) {
  struct FieldState *state;

  state = gFieldState;
  if (Event_GetFlag(FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY) == 0) {
    if (LunpaFortress_CanGuardSeeLeader(ACTOR_GROUND_FLOOR_FIRST_SENTRY) != 0) {
      if (gMainRoutineState.cloaked == 0) {
        GameFlag_Set(FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY);
        Task_Remove(LunpaFortress_CheckFirstEntranceSentry);
        Task_Remove(LunpaFortress_CheckSecondEntranceSentry);
        state->raisedTrigger = RAISED_SENTRIES_SPOTTED;
      }
    }
  }
}

/// The second sentry, as the first.
static void LunpaFortress_CheckSecondEntranceSentry(void) {
  struct FieldState *state;

  state = gFieldState;
  if (Event_GetFlag(FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY) == 0) {
    if (LunpaFortress_CanGuardSeeLeader(ACTOR_GROUND_FLOOR_SECOND_SENTRY) !=
        0) {
      if (gMainRoutineState.cloaked == 0) {
        GameFlag_Set(FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY);
        Task_Remove(LunpaFortress_CheckSecondEntranceSentry);
        Task_Remove(LunpaFortress_CheckFirstEntranceSentry);
        state->raisedTrigger = RAISED_SENTRIES_SPOTTED;
      }
    }
  }
}

/// Whether the leader is in the guard's column, up to six cells away.
static s32 LunpaFortress_IsLeaderAlongGuardsColumn(s32 actorId) {
  struct Actor *guard = Actor_Get(actorId);
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 guardRow = guard->z.fixed / toFIXED(16);
  s32 guardColumn = guard->x.fixed / toFIXED(16);
  s32 leaderRow = leader->z.fixed / toFIXED(16);
  s32 leaderColumn = leader->x.fixed / toFIXED(16);
  s32 zDistance = guardRow - leaderRow;

  if (zDistance >= -6 && zDistance <= 6 && guardColumn - 1 < leaderColumn &&
      guardColumn + 1 > leaderColumn) {
    return 1;
  }
  return 0;
}

/// Whether the leader is within a row of the guard, up to six cells away.
static s32 LunpaFortress_IsLeaderAlongGuardsRow(s32 actorId) {
  struct Actor *guard = Actor_Get(actorId);
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 guardRow = guard->z.fixed / toFIXED(16);
  s32 guardColumn = guard->x.fixed / toFIXED(16);
  s32 leaderRow = leader->z.fixed / toFIXED(16);
  s32 leaderColumn = leader->x.fixed / toFIXED(16);
  s32 xDistance = guardColumn - leaderColumn;

  if (xDistance < -6 || xDistance > 6) {
    return 0;
  }
  if (guardRow - 2 < leaderRow && guardRow + 2 > leaderRow) {
    return 1;
  }
  return 0;
}

/// Whether a guard on screen sees the leader along his row or column.
static s32 LunpaFortress_CanGuardSeeLeader(s32 actorId) {
  if (LunpaFortress_IsGuardNearViewCenter(actorId) == 0) {
    return 0;
  }
  if (LunpaFortress_IsLeaderAlongGuardsColumn(actorId) != 0) {
    return 1;
  }
  {
    s32 alongRow = LunpaFortress_IsLeaderAlongGuardsRow(actorId);

    return NONZERO(alongRow);
  }
}

/// Whether the leader is within four cells, walking, of the cell below the
/// guard.
static s32 LunpaFortress_IsLeaderNearGuard(s32 actorId) {
  struct Actor *guard = Actor_Get(actorId);
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 guardRow = guard->z.fixed / toFIXED(16);
  s32 guardColumn = guard->x.fixed / toFIXED(16);
  s32 leaderRow = leader->z.fixed / toFIXED(16);
  s32 leaderColumn = leader->x.fixed / toFIXED(16);
  s32 xDistance = guardColumn - leaderColumn;
  s32 zDistance;

  guardRow += 1;
  if (xDistance < 0) {
    xDistance = -xDistance;
  }
  zDistance = guardRow - leaderRow;
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
  struct Actor *guard = Actor_Get(actorId);
  struct Actor *viewCenter = FieldCamera_GetActor();
  s32 guardColumn = guard->x.fixed / toFIXED(16);
  s32 guardRow = guard->z.fixed / toFIXED(16);
  s32 viewColumn = viewCenter->x.fixed / toFIXED(16);
  s32 viewRow = viewCenter->z.fixed / toFIXED(16);
  s32 xDistance = guardColumn - viewColumn;
  s32 zDistance;

  if (xDistance < 0) {
    xDistance = -xDistance;
  }
  zDistance = guardRow - viewRow;
  if (zDistance < 0) {
    zDistance = -zDistance;
  }
  if (xDistance > 7 || zDistance > 5) {
    return 0;
  }
  return 1;
}

/// Every frame in the ground floor's east hall: walking into the column of its
/// cell door, at cell (43, 30), with the Cell Key in the party opens it.
static void LunpaFortress_CheckGroundFloorDoorApproach(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  if (GameFlag_Get(GROUND_FLOOR_DOOR_FLAG + 1) == 0) {
    s32 leaderColumn = leader->x.fixed / toFIXED(16);
    s32 leaderRow = leader->z.fixed / toFIXED(16);

    if (leaderColumn == 43 && leaderRow > 28 && leaderRow <= 31) {
      s16 *touchedTrigger = &gFieldState->stepTrigger;
      s32 doorTrigger = CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR + 1;

      *touchedTrigger = doorTrigger;
      LunpaFortress_UnlockGroundFloorDoor();
    }
  }
}

/// Every frame in the Cell Key room: walking north or south into the column of
/// its south cell door, at cell (16, 57), with the Cell Key in the party opens
/// it.
static void LunpaFortress_CheckUpperFloorDoorApproach(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  if (GameFlag_Get(UPPER_FLOOR_DOOR_FLAG) == 0) {
    s32 leaderColumn = leader->x.fixed / toFIXED(16);
    s32 leaderRow = leader->z.fixed / toFIXED(16);

    if (leaderColumn == 16 && leaderRow > 55 && leaderRow <= 58 &&
        (leader->facing == FACING_NORTH || leader->facing == FACING_SOUTH)) {
      s16 *touchedTrigger = &gFieldState->stepTrigger;
      s32 doorTrigger = CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_DOOR;

      *touchedTrigger = doorTrigger;
      LunpaFortress_UnlockUpperFloorDoor();
    }
  }
}

/// Unused: nothing calls it. The northern guard of the pair patrolling the
/// upper floor's west hall would stop, ask who the party is and send it out;
/// the pair throws the party out through LunpaFortress_PatrolPairThrowsPartyOut
/// instead.
static void UNUSED LunpaFortress_UnusedFirstPatrolThrowsPartyOut(void) {
  Event_Start();
  Actor_SetDestinationOffset(ACTOR_UPPER_FLOOR_FIRST_PATROL, 0, 0);
  Actor_SetMovementTypeById(ACTOR_UPPER_FLOOR_FIRST_PATROL,
                            MOVEMENT_TYPE_STAND);
  Actor_Stop(ACTOR_UPPER_FLOOR_FIRST_PATROL);
  Actor_SetAnimation(ACTOR_UPPER_FLOOR_FIRST_PATROL, ANIM_STILL);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  {
    s32 challengeMessage = MsgLunpaWhoAreYou;

    Event_SetMessageId(challengeMessage);
    // "Who are you?"
    Event_ShowMessage(ACTOR_UPPER_FLOOR_FIRST_PATROL, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Event_SetMessageId(challengeMessage + 1);
  }
  Event_ShowMessage(ACTOR_UPPER_FLOOR_FIRST_PATROL, 0);
  Import_EventSetExitRequest(EXIT_THROWN_OUT);
  Event_FinishScreenTransition();
  Event_End();
}

/// A guard who has seen Isaac stops with a start, turns to him and asks "Who
/// are you? How did you get in here?"; Isaac winces, the screen fades, and the
/// party is back outside at the Lunpa Fortress Gates.
static void LunpaFortress_GuardThrowsPartyOut(s32 actorId) {
  Event_Start();
  Event_Start();
  Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 1);
  Actor_SetDestinationOffset(actorId, 0, 0);
  Actor_SetMovementTypeById(actorId, MOVEMENT_TYPE_STAND);
  Actor_SetAnimation(actorId, ANIM_STILL);
  Actor_FaceActor(actorId, ACTOR_PARTY_LEADER, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_SetDestinationOffset(actorId, 0, 0);
  Actor_SetMovementTypeById(actorId, MOVEMENT_TYPE_STAND);
  Actor_Stop(actorId);
  Actor_SetAnimation(actorId, ANIM_STILL);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  {
    s32 challengeMessage = MsgLunpaWhoAreYou;

    Event_SetMessageId(challengeMessage);
    // "Who are you?"
    Event_ShowMessage(actorId, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, actorId, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Event_SetMessageId(challengeMessage + 1);
  }
  Event_ShowMessage(actorId, 0);
  Event_FinishScreenTransition();
  Event_Wait(60);
  Import_EventSetExitRequest(EXIT_THROWN_OUT);
  Event_End();
}

/// A guard who fights turns to Isaac with one of four challenges, drawn at
/// random when the party came in.
static void LunpaFortress_GuardChallengesParty(s32 actorId) {
  Actor_FaceActor(actorId, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, actorId, 0);
  switch (gLunpaFortressRandomPick & 3) {
  case 0:
    LunpaFortress_CryIntruder(actorId);
    break;
  case 1:
    LunpaFortress_AskWhosThat(actorId);
    break;
  case 2:
    LunpaFortress_AskIfShiftIsUp(actorId);
    break;
  case 3:
    LunpaFortress_CallPartyScoundrel(actorId);
    break;
  default:
    LunpaFortress_AskWhosThat(actorId);
    break;
  }
}

/// A guard spots an intruder. "Hey! Intruder!"
static void LunpaFortress_CryIntruder(s32 actorId) {
  s32 messageBase;

  Actor_RunRepeatedMotion(actorId, 1);
  messageBase = MsgLunpaIntruder;
  Event_SetMessageId(messageBase);
  // "Hey! Intruder!"
  Event_ShowMessage(actorId, 0);
  Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_SetMessageId(messageBase + 1);
  // "You think you're going any farther?"
  Event_ShowMessage(actorId, 0);
  messageBase += 2;
  Actor_SetAnimationAndWait(actorId, ANIM_SHAKE_HEAD);
  Event_SetMessageId(messageBase);
  // "Hey! Intruder!"
  Event_ShowMessage(actorId, 0);
}

/// A guard asks who is there. "Huh? Who's that?"
static void LunpaFortress_AskWhosThat(s32 actorId) {
  s32 messageBase = MsgLunpaGuardWhosThat;

  Event_SetMessageId(messageBase);
  // "Huh? Who's that?"
  Event_ShowMessage(actorId, 0);
  Actor_RunRepeatedMotion(actorId, 1);
  Event_SetMessageId(messageBase + 1);
  // "You're pretty bold to come charging into a den of thieves."
  Event_ShowMessage(actorId, 0);
  Actor_SetAnimationAndWait(actorId, ANIM_SHAKE_HEAD);
  Event_SetMessageId(messageBase + 2);
  // "But your luck ran out when you ran into me! Show me your stuff!"
  Event_ShowMessage(actorId, 0);
}

/// A guard expects his relief, then sees who it is. "Is my shift up already?"
static void LunpaFortress_AskIfShiftIsUp(s32 actorId) {
  s32 messageBase;

  messageBase = MsgLunpaShiftAlready;
  Event_SetMessageId(messageBase);
  // "Is my shift up already?"
  Event_ShowMessage(actorId, 0);
  Event_Wait(120);
  Actor_ShowEmote(actorId, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_SetMessageId(messageBase + 1);
  // "...Wait a minute! Who are you?"
  Event_ShowMessage(actorId, 0);
  Actor_RunRepeatedMotion(actorId, 1);
  Event_SetMessageId(messageBase + 2);
  // "I've never seen you before! Intruder!"
  Event_ShowMessage(actorId, 0);
  Actor_SetAnimationAndWait(actorId, ANIM_SHAKE_HEAD);
  Event_SetMessageId(messageBase + 3);
  // "Come on! You'll have to deal with me!"
  Event_ShowMessage(actorId, 0);
}

/// A guard calls the party a scoundrel. "You scoundrel!"
static void LunpaFortress_CallPartyScoundrel(s32 actorId) {
  s32 messageBase = MsgLunpaScoundrel;

  Event_SetMessageId(messageBase);
  // "You scoundrel!"
  Event_ShowMessage(actorId, 0);
  Actor_SetAnimationAndWait(actorId, ANIM_SHAKE_HEAD);
  Event_SetMessageId(messageBase + 1);
  // "Dodonpa always said you'd come, and here you are!"
  Event_ShowMessage(actorId, 0);
  Actor_RunRepeatedMotion(actorId, 1);
  Event_SetMessageId(messageBase + 2);
  // "If I stop you here, Dodonpa will finally reward me!"
  Event_ShowMessage(actorId, 0);
  Actor_SetAnimationAndWait(actorId, ANIM_NOD);
  Event_SetMessageId(messageBase + 3);
  // "What a stroke of luck! My time has finally come!"
  Event_ShowMessage(actorId, 0);
}

/// Walking up to the guard beside the Cell Key, on the floor in front of him in
/// the Cell Key room (cells (17, 52) to (24, 53)), he starts, challenges the
/// party and fights it as a Brigand, and is gone after the battle.
static void LunpaFortress_FightCellKeyRoomGuard(void) {
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Sound_Play(SE_MENU_CANCEL);
  Actor_ShowEmote(ACTOR_UPPER_FLOOR_CELL_KEY_ROOM_GUARD,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  LunpaFortress_GuardChallengesParty(ACTOR_UPPER_FLOOR_CELL_KEY_ROOM_GUARD);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 2);
  Actor_SetPosition(ACTOR_UPPER_FLOOR_CELL_KEY_ROOM_GUARD, 0, 0);
  Event_End();
  GameFlag_Set(FLAG_LUNPA_FORTRESS_CELL_KEY_ROOM_GUARD_BEATEN);
}

/// Walking up the narrow way north out of the upper floor's middle hall, cells
/// (28, 25) to (28, 28), the guard standing in it starts, challenges the party
/// and fights it as a Brigand, and is gone after the battle.
static void LunpaFortress_FightMiddleHallGuard(void) {
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Sound_Play(SE_MENU_CANCEL);
  Actor_ShowEmote(ACTOR_UPPER_FLOOR_MIDDLE_HALL_GUARD,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  LunpaFortress_GuardChallengesParty(ACTOR_UPPER_FLOOR_MIDDLE_HALL_GUARD);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 2);
  Actor_SetPosition(ACTOR_UPPER_FLOOR_MIDDLE_HALL_GUARD, 0, 0);
  Event_End();
  GameFlag_Set(FLAG_LUNPA_FORTRESS_MIDDLE_HALL_GUARD_BEATEN);
}

/// Walking up to the three red-capped guards in the guard room at the north of
/// the upper floor, onto the floor round them (cells (40, 4) to (47, 6)), they
/// turn on Isaac, argue over whether he has come for Hammet, and fight the
/// party together as three Brigands. "Who are you!?"
///
/// After the battle the party is back in the room, and the three lie beaten on
/// the floor (LunpaFortress_PlaceThreeBeatenGuards).
static void LunpaFortress_FightThreeGuards(void) {
  s32 messageBase;

  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_SetAnimation(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, ANIM_STAND);
  Actor_SetAnimation(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, ANIM_STAND);
  Actor_SetAnimation(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, ANIM_STAND);
  Sound_Play(SE_MENU_CANCEL);
  Actor_ShowEmote(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, ACTOR_PARTY_LEADER, 0);
  messageBase = MsgLunpaWhoAreYouShouted;
  Event_SetMessageId(messageBase);
  // "Who are you!?"
  Event_ShowMessage(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 0);
  Actor_ShowEmote(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceActor(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 0);
  Event_Wait(65);
  Actor_FaceDirection(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE,
                      FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST,
                      FACING_NORTH_NORTHEAST, 0);
  Event_SetMessageId(messageBase + 1);
  // "Could this be the guy who's come to get Hammet back?"
  Event_ShowMessage(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 0);
  Actor_SetAnimationAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, ANIM_NOD);
  Event_SetMessageId(messageBase + 2);
  // "Yeah! I bet he is!"
  Event_ShowMessage(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 0);
  Event_SetMessageId(messageBase + 3);
  // "What a bold rascal!"
  Event_ShowMessage(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 0);
  Actor_RunRepeatedMotion(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 1);
  Event_SetMessageId(messageBase + 4);
  // "Dodonpa will be livid if we lose Hammet!"
  Event_ShowMessage(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 0);
  Actor_SetAnimationAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, ANIM_NOD);
  Event_SetMessageId(messageBase + 5);
  // "We're really going to be in for it if we let these kids get..."
  Event_ShowMessage(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 0);
  Actor_SetAnimationAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, ANIM_NOD);
  Event_Wait(60);
  Actor_FaceActor(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, ACTOR_PARTY_LEADER, 0);
  Event_Wait(70);
  Actor_WalkTo(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 672, 88);
  Actor_WaitForMoveById(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST);
#if TBS_PATCH_1
  Actor_FaceActor(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, ACTOR_PARTY_LEADER, 0);
#endif
  Actor_SetAnimationAndWait(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, ANIM_NOD);
  Event_Wait(30);
  Event_SetMessageId(messageBase + 6);
  // "You're not going any farther! Not without some serious..."
  Event_ShowMessage(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 0);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Event_SetAfterBattleDestination(
      SCENE_LUNPA_FORTRESS_UPPER_FLOOR,
      LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_AFTER_BATTLE);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 3);
  Actor_SetPosition(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 0, 0);
  Actor_SetPosition(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 0, 0);
  Actor_SetPosition(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 0, 0);
  Event_End();
  GameFlag_Set(FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN);
}

/// The three guards lie beaten on the guard room's floor, side by side, where
/// they fought.
static void LunpaFortress_PlaceThreeBeatenGuards(void) {
  Event_Start();
  Actor_SetPosition(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 688, 88);
  Actor_SetPosition(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 704, 88);
  Actor_SetPosition(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 720, 96);
  Actor_SetAnimation(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, ANIM_GUARD_BEATEN);
  Actor_SetAnimation(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, ANIM_GUARD_BEATEN);
  Actor_SetAnimation(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, ANIM_GUARD_BEATEN);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 0);
  Event_End();
  Event_StartScreenTransition();
}

/// Walking up to the guard in the doorway up to Donpa's room, in the upper
/// floor's south hall (cells (41, 52) to (43, 53)), he starts, challenges the
/// party and fights it as a Brigand, and is gone after the battle.
static void LunpaFortress_FightDonpasDoorGuard(void) {
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Sound_Play(SE_MENU_CANCEL);
  Actor_ShowEmote(ACTOR_UPPER_FLOOR_DONPAS_DOOR_GUARD,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  LunpaFortress_GuardChallengesParty(ACTOR_UPPER_FLOOR_DONPAS_DOOR_GUARD);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 2);
  Actor_SetPosition(ACTOR_UPPER_FLOOR_DONPAS_DOOR_GUARD, 0, 0);
  Event_End();
  GameFlag_Set(FLAG_LUNPA_FORTRESS_DONPAS_DOOR_GUARD_BEATEN);
}

/// When the guard at the west end of the ground floor's long hall sees Isaac,
/// he stops him, "Who are you? How did you get in here?", and the party is back
/// outside at the Lunpa Fortress Gates.
///
/// Its step row, CELL_BEHAVIOR_LUNPA_FORTRESS_ENTRANCE_GUARD_POST, is on no
/// cell of the map: only LunpaFortress_CheckEntranceGuard starts it.
static void LunpaFortress_EntranceGuardThrowsPartyOut(void) {
  Event_Start();
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceActor(ACTOR_GROUND_FLOOR_ENTRANCE_GUARD, ACTOR_PARTY_LEADER, 0);
  Sound_Play(SE_MENU_CANCEL);
  Actor_ShowEmote(ACTOR_GROUND_FLOOR_ENTRANCE_GUARD,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  {
    s32 challengeMessage = MsgLunpaWhoAreYou;

    Event_SetMessageId(challengeMessage);
    // "Who are you?"
    Event_ShowMessage(ACTOR_GROUND_FLOOR_ENTRANCE_GUARD, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    Event_SetMessageId(challengeMessage + 1);
  }
  Event_ShowMessage(ACTOR_GROUND_FLOOR_ENTRANCE_GUARD, 0);
  Event_FinishScreenTransition();
  Event_Wait(60);
  Import_EventSetExitRequest(EXIT_THROWN_OUT);
  Event_End();
  GameFlag_Set(FLAG_LUNPA_FORTRESS_ENTRANCE_GUARD_CAUGHT_PARTY);
}

/// When the two sentries at the east end of the ground floor's long hall see
/// Isaac, both start and turn to him, the first asks "Who are you? How did you
/// get in here?", and the party is back outside at the Lunpa Fortress Gates.
///
/// Its step row, CELL_BEHAVIOR_LUNPA_FORTRESS_SENTRIES_POST, is on no cell of
/// the map: only the sentries' tasks start it.
static void LunpaFortress_SentriesThrowPartyOut(void) {
  s32 challengeMessage;

  Event_Start();
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetMovementTypeById(ACTOR_PARTY_LEADER, MOVEMENT_TYPE_STAND);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Sound_Play(SE_MENU_CANCEL);
  Actor_ShowEmote(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GROUND_FLOOR_FIRST_SENTRY,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceActor(ACTOR_GROUND_FLOOR_SECOND_SENTRY, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_GROUND_FLOOR_FIRST_SENTRY, ACTOR_PARTY_LEADER, 0);
  challengeMessage = MsgLunpaWhoAreYou;
  Event_SetMessageId(challengeMessage);
  // "Who are you?"
  Event_ShowMessage(ACTOR_GROUND_FLOOR_FIRST_SENTRY, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_SetMessageId(challengeMessage + 1);
  // "How did you get in here?"
  Event_ShowMessage(ACTOR_GROUND_FLOOR_FIRST_SENTRY, 0);
  Event_FinishScreenTransition();
  Event_Wait(60);
  Import_EventSetExitRequest(EXIT_THROWN_OUT);
  Event_End();
  GameFlag_Set(FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY);
}

/// The sealed doorway in the north wall of the room at the foot of the stairs,
/// at cell (21, 34), opens on the way to the lake.
static void LunpaFortress_OpenFirstSwitchPassage(void) {
  MapGrid_CopyCells(2, 82, 1, 2, 21, 81);
  MapGrid_CopyCellAttributes(21, 32, 1, 1, 21, 34);
}

/// The sealed doorway of the small north-west chamber, at cell (6, 10), opens
/// on the stairs down to Hammet's cell.
static void LunpaFortress_OpenSecondSwitchPassage(void) {
  MapGrid_CopyCells(2, 84, 1, 2, 6, 55);
  MapGrid_CopyCellAttributes(5, 9, 1, 1, 6, 10);
}

/// The passage behind the ivy on the north wall of the cave's east ledge, at
/// cell (27, 17), opens on the way to the cells.
static void LunpaFortress_OpenIvyPassage(void) {
  MapGrid_CopyCells(2, 86, 1, 2, 27, 62);
  MapGrid_CopyCellAttributes(26, 16, 1, 1, 27, 17);
}

/// The switch is drawn flipping, two pictures five frames apart, and then its
/// doorway opens.
static inline void LunpaFortress_FlipHiddenSwitch(s32 cellX, s32 cellY,
                                                  void (*openPassage)(void)) {
  MapGrid_CopyCells(6, 77, 1, 2, cellX, cellY);
  Event_Wait(5);
  MapGrid_CopyCells(7, 77, 1, 2, cellX, cellY);
  Event_Wait(1);
  openPassage();
}

/// Searching the wall behind where the ivy grew, west of the sealed doorway in
/// the room at the foot of the stairs (cell (17, 35)), while Reveal shows the
/// switch there: "Isaac flipped the switch.", and the doorway opens.
///
/// The switch can be searched only once Whirlwind has blown the ivy away, and
/// only while Reveal lasts (LunpaFortress_RevealSwitches).
static void LunpaFortress_FlipFirstSwitch(void) {
  if (gFieldState->revealActive != 0) {
    if (Event_GetFlag(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED) == 0) {
      FieldMessage_Show(MsgFieldFlippedSwitch, 1);
      Sound_Play(SE_THUMP);
      Event_Wait(1);
      LunpaFortress_FlipHiddenSwitch(17, 82,
                                     LunpaFortress_OpenFirstSwitchPassage);
      GameFlag_Set(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_FLIPPED);
    }
  }
}

/// Searching the wall behind where the ivy grew in the small north-west chamber
/// (cell (3, 10)), while Reveal shows the switch there: "Isaac flipped the
/// switch.", and the chamber's sealed doorway opens on the stairs down to
/// Hammet's cell.
static void LunpaFortress_FlipSecondSwitch(void) {
  if (gFieldState->revealActive != 0) {
    if (Event_GetFlag(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED) == 0) {
      FieldMessage_Show(MsgFieldFlippedSwitch, 1);
      Sound_Play(SE_THUMP);
      Event_Wait(1);
      LunpaFortress_FlipHiddenSwitch(3, 55,
                                     LunpaFortress_OpenSecondSwitchPassage);
      GameFlag_Set(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_FLIPPED);
    }
  }
}

/// Casting Reveal in the cave shows the two switches hidden in its walls,
/// behind where the ivy grew, and lets them be searched.
///
/// The cave turns grey but for Reveal's circle round Isaac. Each switch's
/// picture is drawn into its wall, and the cell in front of it, (17, 35) or
/// (3, 10), takes the switch's cell behavior from the spare copy at (15, 33) or
/// (3, 8).
static void LunpaFortress_RevealSwitches(void) {
  MapGrid_CopyCells(5, 77, 1, 2, 17, 82);
  MapGrid_CopyCells(5, 77, 1, 2, 3, 55);
  MapGrid_CopyCellAttributes(15, 33, 1, 1, 17, 35);
  MapGrid_CopyCellAttributes(3, 8, 1, 1, 3, 10);
}

/// As Reveal wears off in the cave, the two switches hide in the wall again and
/// can no longer be searched.
static void LunpaFortress_HideSwitches(void) {
  MapGrid_CopyCells(8, 77, 1, 2, 17, 82);
  MapGrid_CopyCells(8, 77, 1, 2, 3, 55);
  MapGrid_CopyCellAttributes(18, 35, 1, 1, 17, 35);
  MapGrid_CopyCellAttributes(2, 10, 1, 1, 3, 10);
}

/// Whirlwind on the ivy west of the sealed doorway, in the room at the foot of
/// the stairs, blows it away from the wall where the first switch hides.
static void LunpaFortress_BlowAwayFirstSwitchIvy(void) {
  GameFlag_Set(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_IVY_BLOWN_AWAY);
  GameFlag_Clear(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_UNDER_IVY);
  Actor_SetPosition(ACTOR_CAVE_FIRST_SWITCH_IVY, 0, 0);
}

/// Whirlwind on the ivy on the north wall of the cave's east ledge blows it
/// away, and the passage behind it to the cells opens.
static void LunpaFortress_BlowAwayPassageIvy(void) {
  GameFlag_Set(FLAG_LUNPA_FORTRESS_PASSAGE_IVY_BLOWN_AWAY);
  LunpaFortress_OpenIvyPassage();
  Actor_SetPosition(ACTOR_CAVE_PASSAGE_IVY, 0, 0);
}

/// Whirlwind on the ivy in the small north-west chamber, west of its sealed
/// doorway, blows it away from the wall where the second switch hides.
static void LunpaFortress_BlowAwaySecondSwitchIvy(void) {
  GameFlag_Set(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_IVY_BLOWN_AWAY);
  GameFlag_Clear(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_UNDER_IVY);
  Actor_SetPosition(ACTOR_CAVE_SECOND_SWITCH_IVY, 0, 0);
}

static void UNUSED LunpaFortress_DoNothingE(void) {}

/// Garet, Ivan and Mia step out from Isaac and gather round Hammet.
static void LunpaFortress_GatherPartyAroundHammet(void) {
  Scene_PlaceAtLeader(ACTOR_IVAN);
  Scene_PlaceAtLeader(ACTOR_MIA);
  Scene_PlaceAtLeader(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_SetSpeed(ACTOR_IVAN, 0.7);
  Actor_WalkTo(ACTOR_IVAN, 456, 192);
  Actor_SetSpeed(ACTOR_MIA, 0.7);
  Actor_WalkTo(ACTOR_MIA, 440, 184);
  Actor_SetSpeed(ACTOR_GARET, 0.7);
  Actor_WalkTo(ACTOR_GARET, 448, 240);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_TOADONPA_HAMMET, 0);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_HAMMET, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(15);
}

/// Dodonpa's lever: the cell rumbles and shakes, its north wall crashes open on
/// the den behind it, and everyone starts.
static void LunpaFortress_ActivateLeverTrap(void) {
  MapGrid_SetShake(4, 4, 1);
  Sound_Play(SE_RUMBLE);
  Event_Wait(80);
  Sound_Play(SE_STOP_ACTION_SE);
  Event_Wait(5);
  Sound_Play(SE_CRASH);
  MapGrid_CopyCells(16, 75, 7, 4, 26, 55);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_Wait(60);
}

/// The camera goes up through the opened wall to Toadonpa in its den; Dodonpa
/// cries "Prepare to become monster fodder!", and the monster lumbers down into
/// the cell and rears up.
static void LunpaFortress_RevealMonster(void) {
  FieldCamera_MoveToActorById(ACTOR_TOADONPA_BOSS, 1);
  FieldCamera_WaitForMove();
  Event_Wait(60);
  Event_SetMessageId(MsgLunpaPrepareBecomeMonster);
  // "Prepare to become monster fodder!"
  Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetSpeed(ACTOR_TOADONPA_BOSS, 1);
  Actor_SetSpeed(ACTOR_INVISIBLE_TOADONPA_BOSS_PART, 1);
  Actor_WalkTo(ACTOR_TOADONPA_BOSS, 472, 180);
  Actor_WalkTo(ACTOR_INVISIBLE_TOADONPA_BOSS_PART, 472, 180);
  FieldCamera_FollowActor(ACTOR_TOADONPA_BOSS, 1);
  Actor_WaitForMoveById(ACTOR_TOADONPA_BOSS);
  Actor_SetAnimation(ACTOR_TOADONPA_BOSS, ANIM_TOADONPA_REAR_UP);
  Event_Wait(30);
}

/// The party spreads out and faces Toadonpa.
static void LunpaFortress_PositionPartyForMonster(void) {
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_WalkTo(ACTOR_IVAN, 504, 216);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_WalkTo(ACTOR_MIA, 440, 232);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_WalkTo(ACTOR_GARET, 480, 224);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
}

/// Walking into Hammet's cell, through its cell door and up to the row at
/// cell (26, 15) to (30, 15), the party finds Hammet on his straw mat; Ivan and
/// Hammet are reunited, then Dodonpa appears in the doorway behind them, pulls
/// a lever that opens the cell's north wall on his monster Toadonpa, and the
/// battle with Toadonpa begins.
///
/// After a lost battle, coming back into the cell skips the reunion and goes
/// straight to Dodonpa's "Back for more? I guess you didn't learn your lesson!"
/// and his trap. Once Donpa has judged Dodonpa, the cell starts nothing.
static void LunpaFortress_FindHammet(void) {
  s32 messageBase;

  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_DODONPA_JUDGED) != 0)
    return;
  GameFlag_Set(FLAG_LUNPA_FORTRESS_FINDING_HAMMET);
  Event_Start();
  // A retry skips the introduction but repeats Dodonpa's trap and battle.
  if (Event_GetFlag(FLAG_LUNPA_FORTRESS_TRAP_SEEN) != 0) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_HAMMET, 0);
    LunpaFortress_GatherPartyAroundHammet();
    Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    60);
    Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_WEST, 0);
    Actor_Jump(ACTOR_TOADONPA_HAMMET, 4, 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_TOADONPA_HAMMET), SPRITE_FLAG_SHADOW);
    Event_Wait(30);
    Actor_SetSpeed(ACTOR_IVAN, 0.7);
    Actor_WalkTo(ACTOR_IVAN, 464, 192);
    Actor_WaitForMoveById(ACTOR_IVAN);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    // Dodonpa approaches from the south, then springs the lever trap.
    Actor_SetPosition(ACTOR_TOADONPA_DODONPA, 456, 320);
    FieldCamera_SetSpeed(2);
    messageBase = MsgLunpaBackMoreGuess;
    Event_SetMessageId(messageBase);
    // "Back for more? I guess you didn't learn your lesson!"
    Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
    Actor_WalkTo(ACTOR_TOADONPA_DODONPA, 458, 272);
    Actor_WaitForMoveById(ACTOR_TOADONPA_DODONPA);
    Actor_FaceDirection(ACTOR_TOADONPA_DODONPA, FACING_SOUTH_SOUTHWEST, 0);
    Event_Wait(40);
    Actor_SetDestinationOffset(ACTOR_TOADONPA_DODONPA, -8, 8);
    Actor_WaitForMoveById(ACTOR_TOADONPA_DODONPA);
    Event_Wait(60);
    Sound_Play(SE_SWITCH);
    // "Dodonpa pulled a lever."
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
    Actor_FaceActor(ACTOR_TOADONPA_DODONPA, ACTOR_PARTY_LEADER, 0);
    Event_SetMessageId(messageBase);
    // "Go ahead-keep trying! You'll never win!"
    Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
    LunpaFortress_RevealMonster();
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Event_Wait(1);
    Battle_SetType(BATTLE_TYPE_SCRIPTED);
    Event_SetAfterBattleDestination(SCENE_LUNPA_FORTRESS_TOADONPA,
                                    LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 4);
  } else {
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Sound_Play(SOUND_FADE_OUT_BGM);
    Event_Wait(30);
    // Ivan recognizes Hammet before Dodonpa interrupts the reunion.
    messageBase = MsgLunpaTimeEat;
    Event_SetMessageId(messageBase + REUNION_HAMMET_ASKS_IF_TIME_TO_EAT);
    // "Is it time to eat?"
    Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_HAMMET, 0);
    Event_Wait(140);
    Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_WEST, 0);
    Actor_Jump(ACTOR_TOADONPA_HAMMET, 4, 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_TOADONPA_HAMMET), SPRITE_FLAG_SHADOW);
    Event_SetMessageId(messageBase + REUNION_HAMMET_ASKS_WHO);
    // "...Who are you?"
    Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_HAMMET, 0);
    LunpaFortress_GatherPartyAroundHammet();
    Actor_SetSpeed(ACTOR_IVAN, 0.7);
    Actor_WalkTo(ACTOR_IVAN, 464, 192);
    Actor_WaitForMoveById(ACTOR_IVAN);
    Event_Wait(30);
    Event_SetMessageId(messageBase + REUNION_IVAN_NAMES_HAMMET);
    // "Hammet..."
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                    0);
    Event_Wait(110);
    Sound_Play(MUS_WIND_ADEPTS);
    Event_SetMessageId(messageBase + REUNION_HAMMET_KNOWS_IVAN);
    // "Ivan, is that you?"
    Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
    Actor_StartRepeatedMotion(ACTOR_TOADONPA_HAMMET, 1);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_TOADONPA_HAMMET, 0.4);
    Actor_WalkTo(ACTOR_TOADONPA_HAMMET, 520, 208);
    Actor_WaitForMoveById(ACTOR_TOADONPA_HAMMET);
    Actor_SetAnimation(ACTOR_TOADONPA_HAMMET, ANIM_STAND);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_NORTH_NORTHWEST, 0);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_SOUTH_SOUTHWEST, 0);
    Event_Wait(30);
    Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_IVAN, 0);
    Event_Wait(20);
    Event_SetMessageId(messageBase + REUNION_HAMMET_ASKS_ABOUT_FRIENDS);
    // "These people... Are they friends of yours?"
    Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Event_Wait(120);
    Event_SetMessageId(messageBase + REUNION_HAMMET_SEES_JOURNEY_BEGUN);
    // "Then it would appear your journey has begun, Ivan."
    Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
    Event_Wait(25);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_TOADONPA_HAMMET, ANIM_NOD);
    Event_Wait(40);
    Actor_WalkTo(ACTOR_IVAN, 480, 200);
    Actor_WaitForMoveById(ACTOR_IVAN);
    Actor_FaceEachOther(ACTOR_IVAN, ACTOR_TOADONPA_HAMMET, 0);
    Event_Wait(60);
    Event_SetMessageId(messageBase + REUNION_IVAN_COMES_TO_SAVE_HAMMET);
    // "I am a little late, but I have come to save you."
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_TOADONPA_HAMMET, ANIM_SHAKE_HEAD);
    Event_Wait(80);
    Event_SetMessageId(messageBase + REUNION_HAMMET_WARNS_OF_RISK);
    // "You shouldn't have risked yourself, Ivan."
    Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
    // Dodonpa approaches from the south, then springs the lever trap.
    Actor_SetPosition(ACTOR_TOADONPA_DODONPA, 456, 320);
    Sound_Play(MUS_SILENCE);
    Event_SetMessageId(messageBase + REUNION_DODONPA_HALTS_PARTY);
    // "Hold it right there!"
    Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DODONPA, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_TOADONPA_DODONPA, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_DODONPA, 0);
    Event_Wait(5);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DODONPA, 0);
    Event_Wait(30);
    Sound_Play(MUS_CHANCE_MEETING_WITH_SWORN_ENEMIES);
    FieldCamera_SetSpeed(2);
    FieldCamera_MoveToActorById(ACTOR_TOADONPA_DODONPA, 1);
    FieldCamera_WaitForMove();
    Actor_SetSpeed(ACTOR_TOADONPA_DODONPA, 0.8);
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
    Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DODONPA, 0);
    Actor_StartRepeatedMotion(ACTOR_TOADONPA_HAMMET, 2);
    Event_Wait(60);
    Event_SetMessageId(messageBase + REUNION_HAMMET_NAMES_DODONPA);
    // "Dodonpa!"
    Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
    Actor_FaceActor(ACTOR_TOADONPA_DODONPA, ACTOR_TOADONPA_HAMMET, 0);
    Event_SetMessageId(messageBase + REUNION_DODONPA_INTERRUPTS);
    // "Sorry to interrupt your teary little reunion, Hammet!"
    Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_TOADONPA_DODONPA, ACTOR_IVAN, 0);
    Event_Wait(30);
    Event_SetMessageId(messageBase + REUNION_DODONPA_MOCKS_IVAN);
    // "This must be your little rat, Ivan-you always did claim he'd..."
    Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DODONPA, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_TOADONPA_DODONPA, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_DODONPA, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
    Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 1);
    Event_Wait(60);
    Event_SetMessageId(messageBase + REUNION_DODONPA_SCORNS_CHILD);
    // "Look at him. He's just a child!"
    Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Event_Wait(60);
    Actor_SetAnimation(ACTOR_TOADONPA_DODONPA, ANIM_SHAKE_HEAD);
    Event_SetMessageId(messageBase + REUNION_DODONPA_MARVELS_AT_KIDS);
    // "But to think these kids could get this far!"
    Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
    Actor_WalkTo(ACTOR_GARET, 456, 248);
    Actor_WaitForMoveById(ACTOR_GARET);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    Event_SetMessageId(messageBase + REUNION_GARET_TAUNTS_DODONPA);
    // "Hey, yeah! If we're just kids, what does that make your..."
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_WalkTo(ACTOR_IVAN, 472, 216);
    Actor_WaitForMoveById(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Event_Wait(10);
    Event_SetMessageId(messageBase + REUNION_IVAN_ACCUSES_DODONPA);
    // "What kind of scum takes ransom money and then doesn't free..."
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Event_Wait(60);
    Event_SetMessageId(messageBase + REUNION_HAMMET_LEARNS_OF_EXTORTION);
    // "I see. So, you've been extorting money from Layana, have you?"
    Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
    Actor_WalkTo(ACTOR_MIA, 440, 216);
    Actor_WaitForMoveById(ACTOR_MIA);
    Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(10);
    Event_SetMessageId(messageBase + REUNION_MIA_TAKES_HAMMET_HOME);
    // "Now that we've found Hammet, we'll be taking him home!"
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_SetAnimation(ACTOR_TOADONPA_DODONPA, ANIM_SHAKE_HEAD);
    Event_SetMessageId(messageBase + REUNION_DODONPA_REFUSES);
    // "I'm afraid I can't let you do that."
    Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Event_Wait(60);
    Event_SetMessageId(messageBase + REUNION_DODONPA_CALLS_HAMMET_MONEY_TREE);
    // "Hammet is what I like to call my little money tree."
    Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Event_Wait(60);
    Event_SetMessageId(messageBase + REUNION_GARET_CALLS_DODONPA_SCOUNDREL);
    // "You dirty scoundrel!"
    Event_ShowMessage(ACTOR_GARET, 0);
    Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 1);
    Event_Wait(60);
    Event_SetMessageId(messageBase + REUNION_DODONPA_KEEPS_HAMMET);
    // "Name-calling won't help you! I'm not giving Hammet up!"
    Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
    Actor_WalkTo(ACTOR_TOADONPA_DODONPA, 456, 280);
    Actor_WaitForMoveById(ACTOR_TOADONPA_DODONPA);
    Actor_FaceDirection(ACTOR_TOADONPA_DODONPA, FACING_SOUTH_SOUTHWEST, 0);
    Event_Wait(80);
    Actor_SetDestinationOffset(ACTOR_TOADONPA_DODONPA, -8, 8);
    Actor_WaitForMoveById(ACTOR_TOADONPA_DODONPA);
    Event_Wait(60);
    Sound_Play(SE_SWITCH);
    FieldMessage_Show(MsgLunpaDodonpaPulledLever, 1);
    Actor_SetDestinationOffset(ACTOR_TOADONPA_DODONPA, 8, -8);
    Actor_FaceActor(ACTOR_TOADONPA_DODONPA, ACTOR_TOADONPA_BOSS, 0);
    LunpaFortress_ActivateLeverTrap();
    Sound_Play(MUS_BATTLING_A_POWERFUL_FOE);
    Event_SetMessageId(messageBase + REUNION_DODONPA_SPRINGS_TRAP);
    // "This little trap here marks the end of your good fortune!"
    Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_BOSS, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_BOSS, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_TOADONPA_BOSS, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_BOSS, 0);
    Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_BOSS, 0);
    LunpaFortress_PositionPartyForMonster();
    LunpaFortress_RevealMonster();
    GameFlag_Set(FLAG_LUNPA_FORTRESS_TRAP_SEEN);
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Event_Wait(1);
    Battle_SetType(BATTLE_TYPE_SCRIPTED);
    Event_SetAfterBattleDestination(SCENE_LUNPA_FORTRESS_TOADONPA,
                                    LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED, 4);
  }
  Event_End();
}

/// After the battle with Toadonpa, the monster has fallen on Dodonpa and pinned
/// him; Isaac is asked whether to lift it off him and they lift it either way,
/// argue over what to do with him, and his father Donpa comes down, locks him
/// in Hammet's cell to think over what he has done, and asks the party to slip
/// out with Hammet before Dodonpa's loyal men find them.
///
/// It plays as the party comes back from the battle
/// (LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE). It sets FLAG_DODONPA_LOCKED_UP,
/// which changes all of Lunpa, and ends with the party and Hammet leaving for
/// the Lunpa Fortress Gates.
static void LunpaFortress_DonpaJudgesDodonpa(void) {
  struct Actor *actor;
  s32 aftermathMessage;
  s32 punishmentMessage;
  s32 donpaMessage;

  GameFlag_Set(FLAG_LUNPA_FORTRESS_DODONPA_JUDGED);
  GameFlag_Set(FLAG_DODONPA_LOCKED_UP);
  MapGrid_CopyCells(16, 75, 7, 4, 26, 55);
  LunpaFortress_OpenToadonpaDoor(4);
  Event_Start();
  Actor_SetSpriteFlags(Actor_Get(ACTOR_TOADONPA_HAMMET), SPRITE_FLAG_SHADOW);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 456, 184);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_SetPosition(ACTOR_GARET, 440, 192);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_SetPosition(ACTOR_MIA, 488, 184);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_SetPosition(ACTOR_IVAN, 472, 184);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_SetPosition(ACTOR_TOADONPA_HAMMET, 520, 224);
  Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_NORTH_NORTHWEST, 0);
  Actor_SetPosition(ACTOR_TOADONPA_BOSS, 448, 237);
  Actor_FaceDirection(ACTOR_TOADONPA_BOSS, FACING_WEST, 0);
  Actor_SetPosition(ACTOR_INVISIBLE_TOADONPA_BOSS_PART, 448, 238);
  Actor_SetSpritePriority(ACTOR_INVISIBLE_TOADONPA_BOSS_PART, 3);
  Actor_SetAnimation(ACTOR_TOADONPA_BOSS, ANIM_TOADONPA_FALLEN);
  Actor_SetPosition(ACTOR_TOADONPA_DODONPA, 458, 243);
  Actor_FaceDirection(ACTOR_TOADONPA_DODONPA, FACING_SOUTH, 0);
  Actor_SetAnimation(ACTOR_TOADONPA_DODONPA, ANIM_DODONPA_PRONE_SLUMP);
  Event_StartScreenTransition();
  Event_Wait(120);

  // The fallen monster has pinned Dodonpa; decide whether to help him.
  aftermathMessage = MsgLunpaUhnnGetOff;
  Event_SetMessageId(aftermathMessage + AFTERMATH_DODONPA_CRIES_OUT);
  // "Uhnn! Get it off me!"
  Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_SetMessageId(aftermathMessage +
                     AFTERMATH_GARET_ASKS_HOW_DODONPA_WAS_TRAPPED);
  // "How did Dodonpa get trapped under the monster?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 1);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 1);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 1);
  Event_Wait(60);
  Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(60);
  Event_SetMessageId(aftermathMessage + AFTERMATH_HAMMET_TELLS_OF_TRICK);
  // "He tried to pull another dirty trick on you!"
  Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_HAMMET, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_HAMMET, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_TOADONPA_HAMMET, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(60);
  Actor_WalkTo(ACTOR_TOADONPA_HAMMET, 512, 232);
  Actor_SetAnimation(ACTOR_TOADONPA_HAMMET, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  Event_SetMessageId(aftermathMessage + AFTERMATH_HAMMET_EXPLAINS_TRICK);
  // "When Dodonpa saw his monster was losing, he tried to get..."
  Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(15);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 2);
  Event_SetMessageId(aftermathMessage + AFTERMATH_DODONPA_BOASTS);
  // "Just a second longer, and you'd have been history!"
  Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DODONPA, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_TOADONPA_DODONPA, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_DODONPA, 0);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Event_SetMessageId(aftermathMessage + AFTERMATH_IVAN_CALLS_DODONPA_SNAKE);
  // "You dirty snake!"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(80);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Event_SetMessageId(aftermathMessage + AFTERMATH_MIA_CALLS_DODONPA_ROTTEN);
  // "Dodonpa, you are rotten to the core!"
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_WINCE, 65);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_HAMMET, 2);
  Event_Wait(100);
  Event_SetMessageId(aftermathMessage + AFTERMATH_DODONPA_APOLOGIZES);
  // "Yeah, OK! I'm sorry! Now, get this thing off of me!"
  Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_Wait(10);
  Event_SetMessageId(aftermathMessage + AFTERMATH_GARET_REPEATS_DEMAND);
  // "'Get this thing off of me'!?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(30);
  Event_SetMessageId(aftermathMessage + AFTERMATH_IVAN_DOUBTS_STRENGTH);
  // "I don't know. That thing looks awfully heavy for us kids to..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
  Event_SetMessageId(aftermathMessage + AFTERMATH_MIA_ASKS_TO_MOVE_MONSTER);
  // "What should we do, Isaac? Should we try to move it?"
  Event_OpenMessage(ACTOR_MIA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DODONPA, 0);
    Event_Wait(60);
  } else {
    Event_SetMessageId(aftermathMessage + AFTERMATH_MIA_ASKS_HAMMET);
    // "I agree. What do you think, Hammet?"
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_HAMMET, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_HAMMET, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_TOADONPA_HAMMET, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_HAMMET, 0);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_PARTY_LEADER, 0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DODONPA, 0);
    Event_Wait(80);
    Actor_SetAnimationAndWait(ACTOR_TOADONPA_HAMMET, ANIM_NOD);
    Event_Wait(30);
    Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_PARTY_LEADER, 0);
    Event_SetMessageId(aftermathMessage + AFTERMATH_HAMMET_WILL_TRY);
    // "I think we have to try, at least."
    Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_TOADONPA_HAMMET, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_HAMMET, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_HAMMET, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_HAMMET, 0);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(80);
    Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DODONPA, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_TOADONPA_DODONPA, 0);
    Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_DODONPA, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DODONPA, 0);
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_SetSpeed(ACTOR_GARET, 0.5);
  Actor_SetSpeed(ACTOR_MIA, 0.5);
  Actor_SetSpeed(ACTOR_IVAN, 0.5);
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
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_Wait(100);
  Sound_Play(SE_LOG_PUSH);
  Actor_SetAnimation(ACTOR_TOADONPA_DODONPA, ANIM_DODONPA_PRONE);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.1);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, -24, 0);
  Actor_SetSpeed(ACTOR_GARET, 0.1);
  Actor_SetDestinationOffset(ACTOR_GARET, -24, 0);
  Actor_SetSpeed(ACTOR_MIA, 0.1);
  Actor_SetDestinationOffset(ACTOR_MIA, -24, 0);
  Actor_SetSpeed(ACTOR_IVAN, 0.1);
  Actor_SetDestinationOffset(ACTOR_IVAN, -24, 0);
  Actor_SetSpeed(ACTOR_TOADONPA_BOSS, 0.1);
  Actor_SetSpeed(ACTOR_INVISIBLE_TOADONPA_BOSS_PART, 0.1);
  Actor_SetDestinationOffset(ACTOR_TOADONPA_BOSS, -24, 0);
  Actor_SetDestinationOffset(ACTOR_INVISIBLE_TOADONPA_BOSS_PART, -24, 0);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Sound_Play(SE_STOP_ACTION_SE);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Event_Wait(60);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_Wait(100);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.7);
  Actor_SetSpeed(ACTOR_GARET, 0.7);
  Actor_SetSpeed(ACTOR_IVAN, 0.7);
  Actor_SetSpeed(ACTOR_MIA, 0.7);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 456, 184);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_WalkTo(ACTOR_GARET, 464, 256);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_WalkTo(ACTOR_GARET, 480, 248);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_WalkTo(ACTOR_GARET, 440, 192);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_WalkTo(ACTOR_MIA, 488, 248);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_WalkTo(ACTOR_MIA, 488, 184);
  Actor_WalkTo(ACTOR_IVAN, 472, 184);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Event_Wait(30);
  Event_Wait(60);
  // Debate Dodonpa's punishment before his father arrives.
  punishmentMessage = MsgLunpaAbleGet;
  Event_SetMessageId(punishmentMessage + PUNISHMENT_GARET_FREES_DODONPA);
  // "There. You should be able to get up now!"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_SetAnimation(ACTOR_TOADONPA_DODONPA, ANIM_DODONPA_PRONE_HEAD_DOWN);
  Event_Wait(120);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 2);
  Event_Wait(60);
  Actor_SetAnimation(ACTOR_TOADONPA_DODONPA, ANIM_DODONPA_PRONE);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_DODONPA_CANNOT_STAND);
  // "I... I can't."
  Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_TOADONPA_DODONPA, 0);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_MIA_ASKS_ABOUT_LEG);
  // "Did you break your leg or something?"
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 2);
  Event_Wait(60);
  Actor_SetAnimation(ACTOR_TOADONPA_DODONPA, ANIM_DODONPA_PRONE_SLUMP);
  Event_Wait(70);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_DODONPA_WILL_STAND);
  // "It's not broken... I should be able to stand after a bit."
  Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetAnimation(ACTOR_TOADONPA_DODONPA, ANIM_DODONPA_PRONE);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_IVAN_IS_RELIEVED);
  // "Thank goodness..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_HAMMET, ANIM_NOD);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Actor_WalkTo(ACTOR_GARET, 440, 208);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_GARET_QUESTIONS_RELIEF);
  // "What do you mean, 'thank goodness'!?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(70);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 75);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_GARET_RECALLS_ATTACK);
  // "Didn't he just try to kill us?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_HAMMET_FORGIVES);
  // "Can't we just forgive and forget?"
  Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
  Actor_WalkTo(ACTOR_GARET, 448, 208);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_HAMMET, 0);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_GARET_RECALLS_KIDNAPPING);
  // "Dodonpa kidnapped you, remember!?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_HAMMET, 1);
  Event_Wait(60);
  Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(60);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_HAMMET_REMEMBERS);
  // "Oh, that's right..."
  Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_GARET_LOSES_PATIENCE);
  // "Get a grip, you guys!"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(30);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_IVAN_POINTS_TO_SAFETY);
  // "But Hammet is safe now, isn't he?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_MIA, 0);
  Event_Wait(20);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_GARET_DOUBTS_REFORM);
  // "Yeah, but do you really think Dodonpa will reform if Hammet..."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  Actor_FaceActor(ACTOR_MIA, ACTOR_GARET, 0);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_MIA_WONDERS);
  // "Hmm... I wonder..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_MIA_ASKS_DODONPA);
  // "How about it, Dodonpa?"
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DODONPA, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_TOADONPA_DODONPA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_DODONPA, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(120);
  Actor_ShowEmote(ACTOR_TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 1);
  Event_Wait(120);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_HAMMET, 1);
  Event_Wait(60);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_HAMMET_DEFENDS_DODONPA);
  // "He already said he wouldn't do anything!"
  Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 110);
  Actor_WalkTo(ACTOR_GARET, 456, 212);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_GARET_DISTRUSTS_DODONPA);
  // "Of course, he's going to say that! He wants us to forgive him!"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_GARET, 0);
  Event_Wait(60);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_IVAN_ASKS_GARET);
  // "What do you think we should do with him, Garet?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(80);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 0);
  Event_Wait(60);
  Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DODONPA, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_DODONPA, 0);
  Actor_FaceActor(ACTOR_TOADONPA_DODONPA, ACTOR_TOADONPA_DODONPA, 0);
  Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(80);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  Event_SetMessageId(punishmentMessage +
                     PUNISHMENT_GARET_WANTS_DODONPA_LOCKED_UP);
  // "We should lock him up here, just like he did to Hammet."
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 2);
  Event_Wait(70);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_DODONPA_PLEADS);
  // "No! How can you be so cruel!"
  Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Event_SetMessageId(punishmentMessage + PUNISHMENT_MIA_SCOLDS_DODONPA);
  // "Oh? And yet you have no problem being that cruel to Hammet!"
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Event_Wait(80);
  Event_SetMessageId(punishmentMessage +
                     PUNISHMENT_IVAN_ASKS_TO_LOCK_DODONPA_UP);
  // "What should we do, Isaac? Should we lock Dodonpa up?"
  Event_OpenMessage(ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_PARTY_LEADER, 0);
  Actor_SetPosition(ACTOR_TOADONPA_DONPA, 456, 304);
  Actor_SetSpeed(ACTOR_TOADONPA_DONPA, 0.5);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Sound_Play(MUS_SILENCE);
    Event_SetMessageId(punishmentMessage + PUNISHMENT_DONPA_APPROVES);
    // "Serves the little punk right!"
    Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  } else {
    Sound_Play(MUS_SILENCE);
    Event_SetMessageId(punishmentMessage + PUNISHMENT_DONPA_LEAVES_DODONPA);
    // "Just leave him here like this for a while!"
    Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  }
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DONPA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_DONPA, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_DONPA, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_TOADONPA_DONPA, 0);
  Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DONPA, 0);
  Actor_FaceActor(ACTOR_TOADONPA_DODONPA, ACTOR_TOADONPA_DONPA, 0);
  FieldCamera_MoveTo(456, CAMERA_KEEP, 240, 1);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 1);
  Event_Wait(60);
  Sound_Play(MUS_COLOSSEUM);
  // Donpa takes charge and orders his son to remain imprisoned.
  donpaMessage = MsgLunpaDad;
  Event_SetMessageId(donpaMessage + JUDGMENT_DODONPA_CALLS_FATHER);
  // "Dad!"
  Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
  Actor_WalkTo(ACTOR_TOADONPA_DONPA, 456, 280);
  Actor_WaitForMoveById(ACTOR_TOADONPA_DONPA);
  Actor_WalkTo(ACTOR_TOADONPA_DONPA, 440, 256);
  FieldCamera_MoveTo(456, CAMERA_KEEP, 224, 1);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_PITIES_DODONPA);
  // "Dodonpa... You pitiful wretch."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_WaitForMoveById(ACTOR_TOADONPA_DONPA);
  Actor_FaceDirection(ACTOR_TOADONPA_DONPA, FACING_NORTH_NORTHEAST, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Actor_SetAnimation(ACTOR_TOADONPA_DONPA, ANIM_SHAKE_HEAD);
  Event_Wait(89);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_RECALLS_WARNING);
  // "I always told you that if you did evil to others... others..."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_RunRepeatedMotion(ACTOR_TOADONPA_DODONPA, 2);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_DONPA, ANIM_NOD);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_DEMANDS_AMENDS);
  // "You must right all of the misdeeds you've done!"
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  80);
  Actor_SetAnimation(ACTOR_TOADONPA_DONPA, ANIM_SHAKE_HEAD);
  Event_Wait(80);
  Actor_FaceDirection(ACTOR_TOADONPA_DONPA, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_REGRETS_SPOILING);
  // "I feel bad for spoiling you. Look at the trouble it has..."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 1);
  Event_SetMessageId(donpaMessage + JUDGMENT_DODONPA_BLAMES_DONPA);
  // "See! It's all your fault that all this happened!"
  Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_DONPA, ANIM_NOD);
  Actor_FaceActor(ACTOR_TOADONPA_DONPA, ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(20);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_WILL_PUNISH);
  // "Yes, which is why this is going to hurt you more than it..."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DODONPA, 1);
  Actor_ShowEmote(ACTOR_TOADONPA_DODONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Event_SetMessageId(donpaMessage + JUDGMENT_DODONPA_DOUBTS_DONPA);
  // "You're not serious, are you, Dad?"
  Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_DONPA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_INSISTS);
  // "I most certainly am!"
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_DONPA, ANIM_NOD);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_SENTENCES_DODONPA);
  // "You're going to spend some time here thinking about all that..."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_TOADONPA_DODONPA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Event_Wait(80);
  Actor_SetAttachedEffect(ACTOR_TOADONPA_DODONPA, 0);
  Actor_WalkTo(ACTOR_TOADONPA_DONPA, 496, 240);
  Actor_WaitForMoveById(ACTOR_TOADONPA_DONPA);
  Actor_FaceActor(ACTOR_TOADONPA_DONPA, ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DONPA, 0);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_ASKS_ABOUT_TROUBLE);
  // "Dodonpa has caused you all sorts of trouble, hasn't he?"
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_HAMMET, 1);
  Event_SetMessageId(donpaMessage + JUDGMENT_HAMMET_DENIES_TROUBLE);
  // "No, not at all!"
  Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_DONPA, ANIM_NOD);
  Event_Wait(20);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_APOLOGIZES);
  // "I must apologize deeply for all of this."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAnimation(ACTOR_TOADONPA_DONPA, ANIM_NOD);
  Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_SetMessageId(donpaMessage + JUDGMENT_HAMMET_DECLINES_APOLOGY);
  // "Thank you, Donpa, but you do not need to apologize."
  Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_DONPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_ASKS_FORGIVENESS);
  // "Then you forgive me?"
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAnimation(ACTOR_TOADONPA_HAMMET, ANIM_NOD);
  Event_Wait(140);
  Actor_SetAnimation(ACTOR_TOADONPA_DONPA, ANIM_NOD);
  Event_Wait(120);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_Wait(120);
  Actor_ShowEmote(ACTOR_TOADONPA_DONPA, EMOTE_IN_FRONT | EMOTE_SMILE, 180);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_HAS_REQUEST);
  // "I am sorry about the trouble, but I do have one request."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Event_SetMessageId(donpaMessage + JUDGMENT_HAMMET_ASKS_ABOUT_REQUEST);
  // "A request?"
  Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
  Event_Wait(80);
  Actor_FaceActor(ACTOR_TOADONPA_DONPA, ACTOR_PARTY_LEADER, 0);
  Event_Wait(60);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DONPA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_TOADONPA_DONPA, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_TOADONPA_DONPA, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_TOADONPA_DONPA, 0);
  Event_Wait(60);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_ASKS_PARTY_TO_LEAVE);
  // "I don't know how you got in here, but please leave by the..."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(100);
  Event_SetMessageId(donpaMessage + JUDGMENT_GARET_QUESTIONS_SECRECY);
  // "Huh? Donpa, didn't you just say you're going to lock up Dodonpa?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Event_Wait(60);
  Event_SetMessageId(donpaMessage + JUDGMENT_IVAN_QUESTIONS_SNEAKING);
  // "So we shouldn't have to sneak around, should we?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_TOADONPA_DONPA, ACTOR_IVAN, 0);
  Event_Wait(20);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_WARNS_OF_HENCHMEN);
  // "Some of Dodonpa's henchmen are still loyal to him."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(120);
  Actor_SetAnimation(ACTOR_TOADONPA_DONPA, ANIM_SHAKE_HEAD);
  Event_Wait(120);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_IS_ASHAMED);
  // "I'm ashamed to say it, but they won't listen to any of my..."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_DONPA, EMOTE_IN_FRONT | EMOTE_WINCE, 90);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_WARNS_OF_ATTACK);
  // "This new gang of thieves will attack Hammet if they find him..."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_TOADONPA_HAMMET, ANIM_NOD);
  Event_Wait(80);
  Actor_SetAnimation(ACTOR_TOADONPA_DONPA, ANIM_SHAKE_HEAD);
  Event_Wait(120);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_FEARS_CELL_CHECK);
  // "And they may come to check his cell, right?"
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_TOADONPA_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                  60);
  Event_SetMessageId(donpaMessage + JUDGMENT_HAMMET_FORESEES_RELEASE);
  // "I see. Then they'd free Dodonpa, wouldn't they?"
  Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
  Actor_FaceActor(ACTOR_TOADONPA_DONPA, ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_DONPA, 1);
  Event_Wait(50);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_FORESEES_REVENGE);
  // "And then he'd head straight for Kalay seeking his revenge!"
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Event_Wait(70);
  Event_SetMessageId(donpaMessage + JUDGMENT_GARET_AGREES);
  // "I see. Yes, you're right."
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_SetMessageId(donpaMessage + JUDGMENT_MIA_FEARS_FOR_KALAY);
  // "It would be terrible if Kalay were attacked."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_FaceActor(ACTOR_TOADONPA_DONPA, ACTOR_PARTY_LEADER, 0);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_DONPA, ANIM_NOD);
  Event_Wait(20);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_URGES_HASTE);
  // "That's why you must go while you still can!"
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 90);
  Event_SetMessageId(donpaMessage + JUDGMENT_IVAN_ASKS_IF_NOW);
  // "You mean immediately?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_TOADONPA_DONPA, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_SPREAD_RUMORS);
  // "I heard of your intrusion, so I spread some rumors through..."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_StartRepeatedMotion(ACTOR_TOADONPA_HAMMET, 1);
  Event_Wait(60);
  Event_SetMessageId(donpaMessage + JUDGMENT_HAMMET_ASKS_ABOUT_RUMORS);
  // "Rumors?"
  Event_ShowMessage(ACTOR_TOADONPA_HAMMET, 0);
  Actor_FaceActor(ACTOR_TOADONPA_DONPA, ACTOR_TOADONPA_HAMMET, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_DONPA, ANIM_NOD);
  Event_Wait(20);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_TELLS_RUMOR);
  // "I said brigands had snuck in to kidnap Hammet, but failed..."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 70);
  Event_SetMessageId(donpaMessage + JUDGMENT_GARET_ASKS_WHY);
  // "Why in the world did you do that?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceActor(ACTOR_TOADONPA_DONPA, ACTOR_PARTY_LEADER, 0);
  Event_Wait(30);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_EXPLAINS_RUMOR);
  // "Now, they won't let anyone down here at all. Plus, many of..."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  Event_SetMessageId(donpaMessage + JUDGMENT_MIA_PRAISES_DONPA);
  // "Donpa, you are a shrewd one!"
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetAnimationAndWait(ACTOR_TOADONPA_DONPA, ANIM_NOD);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_OFFERS_HELP);
  // "I just thought I'd try to help however I could..."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(100);
  Event_SetMessageId(donpaMessage + JUDGMENT_IVAN_URGES_ESCAPE);
  // "With Dodonpa's henchmen gone, this is our chance to escape,..."
  Event_OpenMessage(ACTOR_IVAN, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
    Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_HURRIES_PARTY);
    // "Why did I even bother asking? Let's get a move on!"
    Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  }
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_Wait(100);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_HAMMET, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_TOADONPA_HAMMET, ANIM_NOD);
  Event_Wait(100);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_IVAN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_MIA, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_TOADONPA_HAMMET, 0.4);
  Actor_WalkTo(ACTOR_TOADONPA_HAMMET, 472, 184);
  Actor_WaitForMoveById(ACTOR_TOADONPA_HAMMET);
  Actor_SetAnimation(ACTOR_TOADONPA_HAMMET, ANIM_STAND);
  Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceActor(ACTOR_TOADONPA_DONPA, ACTOR_PARTY_LEADER, 0);
  Actor_SetAnimation(ACTOR_TOADONPA_DONPA, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_TOADONPA_DONPA, ACTOR_TOADONPA_DODONPA, 0);
  Event_Wait(20);
  Event_SetMessageId(donpaMessage + JUDGMENT_DONPA_PROMISES_CHANGE);
  // "The next time you meet Dodonpa, he will be a changed man."
  Event_ShowMessage(ACTOR_TOADONPA_DONPA, 0);
  Actor_RunRepeatedMotion(ACTOR_TOADONPA_DODONPA, 2);
  Event_SetMessageId(donpaMessage + JUDGMENT_DODONPA_PROMISES_TO_BEHAVE);
  // "I get it already! I won't do anything wrong! Come on, let me go!"
  Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(ACTOR_TOADONPA_HAMMET, 1);
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 480, 248);
  Event_Wait(40);
  Actor_SetSpritePriority(ACTOR_TOADONPA_HAMMET, 0);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 0);
  Actor_WalkTo(ACTOR_TOADONPA_HAMMET, 480, 216);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_TOADONPA_DONPA, 0);
  Actor_WaitForMoveById(ACTOR_TOADONPA_HAMMET);
  Actor_FaceActor(ACTOR_TOADONPA_HAMMET, ACTOR_TOADONPA_DONPA, 0);
  Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(5);
  Actor_SetAnimation(ACTOR_TOADONPA_HAMMET, ANIM_NOD);
  Event_Wait(100);
  Actor_SetAnimation(ACTOR_TOADONPA_DONPA, ANIM_NOD);
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
  Sound_Play(SOUND_FADE_OUT_BGM);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
  Event_FinishScreenTransition();
  Event_Wait(1);
  Event_Wait(210);
  Import_EventSetExitRequest(LUNPA_FORTRESS_TOADONPA_TO_LUNPA_FORTRESS_GATES);
  Event_End();
}

/// Mind Read on the second sentry at the east end of the ground floor's long
/// hall shows one of seven thoughts, drawn at random when the party came in:
/// Hammet as their best source of income, dozing in the warmth, Dodonpa's
/// orders, missing Colosso in Tolbi, someone he could swear is right there,
/// being a coward told to stand guard; or he looks round and vows "I'll take
/// care of anybody that might show up!"
static void LunpaFortress_ReadSentryMind(void) {
  switch (gLunpaFortressRandomPick) {
  case 0:
    Event_SetMessageId(MsgLunpaHammetGreatMerchant);
    // "Hammet, the great merchant of Kalay, is our best source of..."
    Event_ShowMessage(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 1:
    Event_SetMessageId(MsgLunpaZZZ);
    // "Z-z-z-z Z-z-z-z-z... It's so warm today! My eyes are getting..."
    Event_ShowMessage(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 2:
    Event_SetMessageId(MsgLunpaDodonpasOrdersAbsolute);
    // "Dodonpa's orders are absolute! No one will ever get by me..."
    Event_ShowMessage(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 3:
    Event_SetMessageId(MsgLunpaSighBadCouldnt);
    // "Sigh. It's too bad I couldn't go to Tolbi this year. I..."
    Event_ShowMessage(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 4:
    Event_SetMessageId(MsgLunpaStrangeSwearSomeone);
    // "That's strange!? I could swear there's someone right..."
    Event_ShowMessage(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 6:
    Event_SetMessageId(MsgLunpaToldStandGuard);
    // "I was told to stand guard, so I am, but I'm actually a..."
    Event_ShowMessage(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 7:
    // Unused: Random_Below(7) never draws 7.
    Event_SetMessageId(MsgLunpaWhoDisruptingSleep);
    // "Who is disrupting my sleep???"
    Event_ShowMessage(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  case 5:
    Actor_FaceDirection(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                        FACING_NORTH_NORTHEAST, 0);
    Event_Wait(50);
    Actor_FaceDirection(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                        FACING_NORTH_NORTHWEST, 0);
    Event_Wait(50);
    Actor_FaceDirection(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                        FACING_SOUTH_SOUTHWEST, 0);
    Event_Wait(50);
    Event_SetMessageId(MsgLunpaTakeCareAnybody);
    // "I'll take care of anybody that might show up!"
    Event_ShowMessage(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
    break;
  }
}

/// Talking to the second sentry at the east end of the ground floor's long
/// hall, he looks round for whoever he heard: "Huh? I could swear I just heard
/// someone!"
static void LunpaFortress_TalkToSentry(void) {
  s32 messageBase;

  Actor_ShowEmote(ACTOR_GROUND_FLOOR_SECOND_SENTRY,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_GROUND_FLOOR_SECOND_SENTRY, FACING_NORTH_NORTHEAST,
                      0);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_GROUND_FLOOR_SECOND_SENTRY, FACING_NORTH_NORTHWEST,
                      0);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_GROUND_FLOOR_SECOND_SENTRY, FACING_SOUTH_SOUTHWEST,
                      0);
  Event_Wait(50);
  messageBase = MsgLunpaLeftGuardHearsSomeone;
  Event_SetMessageId(messageBase);
  // "Huh? I could swear I just heard someone!"
  Event_ShowMessage(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
  Actor_SetAnimation(ACTOR_GROUND_FLOOR_SECOND_SENTRY, ANIM_SHAKE_HEAD);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_GROUND_FLOOR_SECOND_SENTRY, FACING_NORTH_NORTHWEST,
                      0);
  Event_Wait(40);
  Event_SetMessageId(messageBase + 1);
  // "You're crazy! Still, this place is kind of creepy. I never..."
  Event_ShowMessage(ACTOR_GROUND_FLOOR_SECOND_SENTRY, 0);
}

/// Talking to the woman at Donpa's bedside, in his room on the upper floor:
/// before the rescue she hushes the party ("Shhh! Please don't wake Donpa!"),
/// tells of Donpa's despair over Dodonpa and asks whether Isaac is the one
/// Donpa said would come to punish him; after it she says Donpa is grateful.
///
/// She tells it all once; after that she only hopes Dodonpa's eyes will be
/// opened.
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
    // "Donpa is very grateful for what you have done."
#ifdef BUGFIX
    Event_ShowMessage(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
#else
    Event_ShowMessage(ACTOR_UPPER_FLOOR_DODONPAS_WIFE, 0);
#endif
    Event_End();
  } else {
    if (GameFlag_Get(FLAG_LUNPA_FORTRESS_ATTENDANT_TOLD_OF_DONPA) != 0) {
      Event_SetMessageId(MsgLunpaMaybeDodonpasEyes);
      // "Maybe if Dodonpa's eyes are opened, Lunpa will be a good..."
      Event_OpenMessage(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      Event_End();
    } else {
      Actor_ShowEmote(ACTOR_UPPER_FLOOR_ATTENDANT, EMOTE_IN_FRONT | EMOTE_WINCE,
                      30);
      Actor_FaceActor(ACTOR_UPPER_FLOOR_ATTENDANT, ACTOR_PARTY_LEADER, 0);
      whisperMessage = MsgLunpaShhhPleaseDont;
      Event_SetMessageId(whisperMessage);
      // "Shhh! Please don't wake Donpa!"
      Event_ShowMessage(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      Actor_FaceActor(ACTOR_UPPER_FLOOR_ATTENDANT, ACTOR_UPPER_FLOOR_DONPA, 0);
      FieldCamera_MoveToActorById(ACTOR_UPPER_FLOOR_DONPA, 1);
      FieldCamera_WaitForMove();
      Event_Wait(60);
      FieldCamera_MoveToActorById(ACTOR_PARTY_LEADER, 1);
      Event_Wait(20);
      Actor_ShowEmote(ACTOR_UPPER_FLOOR_ATTENDANT,
                      EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      Event_SetMessageId(whisperMessage + 1);
      // "He's been driven to despair after battling a terrible foe."
      Event_ShowMessage(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      Actor_ShowEmote(ACTOR_UPPER_FLOOR_ATTENDANT, EMOTE_IN_FRONT | EMOTE_SCOWL,
                      60);
      Event_SetMessageId(whisperMessage + 2);
      // "It is all because of that Dodonpa!"
      Event_ShowMessage(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      Event_Wait(70);
      Actor_ShowEmote(ACTOR_UPPER_FLOOR_ATTENDANT,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
      Actor_FaceActor(ACTOR_UPPER_FLOOR_ATTENDANT, ACTOR_PARTY_LEADER, 0);
      Event_SetMessageId(whisperMessage + 3);
      // "Are you the one who Donpa said would come to punish Dodonpa?"
      Event_OpenMessage(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
        Event_SetMessageId(whisperMessage + 4);
        // "Donpa has waited a very long time for you."
        Event_OpenMessage(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      } else {
        Event_SetMessageId(whisperMessage + 5);
        // "Oh, you're not? Well...can you do it anyway? Someone has to..."
        Event_OpenMessage(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      }
      Event_Wait(60);
      Actor_ShowEmote(ACTOR_UPPER_FLOOR_ATTENDANT,
                      EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      familyMessage = MsgLunpaDonpaKnowsCoddled;
      Event_SetMessageId(familyMessage);
      // "Donpa knows that he coddled Dodonpa too much as a child."
      Event_OpenMessage(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      Actor_RunRepeatedMotion(ACTOR_UPPER_FLOOR_ATTENDANT, 1);
      Event_SetMessageId(familyMessage + 1);
      // "It pains him so much. He can't die with things the way they are."
      Event_OpenMessage(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      Actor_SetAnimationAndWait(ACTOR_UPPER_FLOOR_ATTENDANT, ANIM_NOD);
      Event_SetMessageId(familyMessage + 2);
      // "Maybe if Dodonpa's eyes are opened, Lunpa will be a good..."
      Event_OpenMessage(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      GameFlag_Set(FLAG_LUNPA_FORTRESS_ATTENDANT_TOLD_OF_DONPA);
      Event_End();
    }
  }
}

/// Mind Read on the woman at Donpa's bedside: before the rescue she hopes
/// someone will punish Dodonpa, for which Donpa would surely give a reward;
/// after it she thinks how hard it is for Donpa to teach his child a lesson.
static void LunpaFortress_ReadAttendantMind(void) {
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP)) {
    Event_SetMessageId(MsgLunpaDifficultDonpaRight);
    // "It must be difficult for Donpa right now. It's so hard to..."
    Event_ShowMessage(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
  } else {
    Event_SetMessageId(MsgLunpaSomeonePunishDodonpa);
    // "If someone would punish Dodonpa, I'm sure Donpa would offer..."
    Event_ShowMessage(ACTOR_UPPER_FLOOR_ATTENDANT, 0);
  }
}

/// Mind Read on Donpa, asleep in his bed on the upper floor: before the rescue
/// he is sorry about Dodonpa and sure someone will teach him a lesson someday;
/// after it he would like to let Dodonpa out of his cell already.
static void LunpaFortress_ReadDonpaMind(void) {
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP)) {
    Event_SetMessageId(MsgLunpaFatherStayAngry);
    // "What father can stay angry at his children for long?..."
    Event_ShowMessage(ACTOR_UPPER_FLOOR_DONPA, 0);
  } else {
    Event_SetMessageId(MsgLunpaFatherSorryDodonpa);
    // "Father...I'm so sorry about Dodonpa. Someone will teach him..."
    Event_ShowMessage(ACTOR_UPPER_FLOOR_DONPA, 0);
  }
}

/// Talking to Donpa in his bed on the upper floor: before the rescue he only
/// snores, "Z-z-z-z... Z-z-z-z..."; after it he thanks the party for teaching
/// Dodonpa a lesson and has his attendant show it his reward, a creature he
/// once fought, in the room behind his.
///
/// Once the reward has been shown, or Tonic has joined, he only worries whether
/// Dodonpa will really turn over a new leaf.
static void LunpaFortress_TalkToDonpa(void) {
  s32 rewardMessage;

  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    if (GameFlag_Get(FLAG_LUNPA_FORTRESS_REWARD_TAKEN) == 0 &&
        Event_GetFlag(FLAG_LUNPA_FORTRESS_REWARD_SHOWN) == 0) {
      rewardMessage = MsgLunpaThankHelpDodonpa;
      Event_SetMessageId(rewardMessage);
      // "Thank you for your help with Dodonpa."
      Event_ShowMessage(ACTOR_UPPER_FLOOR_DONPA, 0);
      Actor_RunRepeatedMotion(ACTOR_UPPER_FLOOR_DONPA, 1);
      Event_Wait(30);
      Actor_SetSpeed(ACTOR_UPPER_FLOOR_DONPA, 0.1);
      Actor_SetDestinationOffset(ACTOR_UPPER_FLOOR_DONPA, -4, 0);
      Actor_WaitForMoveById(ACTOR_UPPER_FLOOR_DONPA);
      Actor_SetAnimation(ACTOR_UPPER_FLOOR_DONPA, ANIM_NOD);
      Event_Wait(60);
      Actor_SetSpeed(ACTOR_UPPER_FLOOR_DONPA, 0.2);
      Actor_SetDestinationOffset(ACTOR_UPPER_FLOOR_DONPA, -6, 0);
      Actor_FaceActor(ACTOR_UPPER_FLOOR_DONPA, ACTOR_PARTY_LEADER, 0);
      Actor_WaitForMoveById(ACTOR_UPPER_FLOOR_DONPA);
      Event_SetMessageId(rewardMessage + 1);
      // "I have a little something for you, for your teaching Dodonpa..."
      Event_ShowMessage(ACTOR_UPPER_FLOOR_DONPA, 0);
      Actor_RunRepeatedMotion(ACTOR_UPPER_FLOOR_DONPA, 1);
      Actor_FaceActor(ACTOR_UPPER_FLOOR_ATTENDANT, ACTOR_UPPER_FLOOR_DONPA, 0);
      Event_SetMessageId(rewardMessage + 2);
      // "Give these people a token of my gratitude."
      Event_ShowMessage(ACTOR_UPPER_FLOOR_DONPA, 0);
      Event_Wait(70);
      Actor_SetAnimation(ACTOR_UPPER_FLOOR_ATTENDANT, ANIM_NOD);
      Event_Wait(60);
      Actor_SetSpeed(ACTOR_UPPER_FLOOR_ATTENDANT, 0.4);
      Actor_WalkTo(ACTOR_UPPER_FLOOR_ATTENDANT, 880, 112);
      Actor_WaitForMoveById(ACTOR_UPPER_FLOOR_ATTENDANT);
      Actor_FaceDirection(ACTOR_UPPER_FLOOR_ATTENDANT, FACING_NORTH_NORTHEAST,
                          0);
      Event_SetMessageId(rewardMessage + 3);
      // "Deep within there is an interesting creature that I fought..."
      Event_ShowMessage(ACTOR_UPPER_FLOOR_DONPA, 0);
      Actor_SetAnimation(ACTOR_UPPER_FLOOR_DONPA, ANIM_NOD);
      Event_Wait(70);
      Actor_SetDestinationOffset(ACTOR_UPPER_FLOOR_DONPA, 8, 0);
      Actor_WaitForMoveById(ACTOR_UPPER_FLOOR_DONPA);
      Actor_SetAnimation(ACTOR_UPPER_FLOOR_DONPA, ANIM_DONPA_IN_BED);
      Event_SetMessageId(rewardMessage + 4);
      // "I give this to you. Please take good care of it."
      Event_ShowMessage(ACTOR_UPPER_FLOOR_DONPA, 0);
      Actor_WalkTo(ACTOR_PARTY_LEADER, 896, 120);
      Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
      Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_UPPER_FLOOR_ATTENDANT, 0);
      Event_Wait(60);
      Actor_SetAnimation(ACTOR_UPPER_FLOOR_ATTENDANT, ANIM_NOD);
      Event_Wait(30);
      GameFlag_Set(FLAG_LUNPA_FORTRESS_REWARD_SHOWN);
    } else {
      Event_SetMessageId(MsgLunpaWellWorriedDodonpa);
      // "Well, now I'm worried about Dodonpa really turning over a..."
      Event_ShowMessage(ACTOR_UPPER_FLOOR_DONPA, 0);
    }
  } else {
    Event_SetMessageId(MsgLunpaZZZZ);
    // "Z-z-z-z... Z-z-z-z..."
    Event_ShowMessage(ACTOR_UPPER_FLOOR_DONPA, 0);
  }
}

/// Talking to Tonic, the Mercury Djinni in the room behind Donpa's, it joins
/// the party.
static void LunpaFortress_DjinniJoins(void) {
  Djinn_JoinParty(ACTOR_UPPER_FLOOR_TONIC, ELEMENT_MERCURY,
                  DJINNI_INDEX(DJINNI_TONIC));
  GameFlag_Set(FLAG_LUNPA_FORTRESS_REWARD_TAKEN);
}

/// Talking to the east one of the three beaten guards in the upper floor's
/// guard room, he whimpers and tells the party to go down to the cave and save
/// Hammet. "Owww! Hey! Don't hurt me again..."
static void LunpaFortress_TalkToBeatenEastGuardOfThree(void) {
  Actor_RunRepeatedMotion(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 2);
  Event_SetMessageId(MsgLunpaOwwwDontHurt);
  // "Owww! Hey! Don't hurt me again, you thieving scoundrel! I..."
  Event_ShowMessage(ACTOR_UPPER_FLOOR_THREE_GUARDS_EAST, 0);
}

/// Talking to the middle one of the three beaten guards in the upper floor's
/// guard room, he gives up and says Hammet is down in the cave. "We give up!"
static void LunpaFortress_TalkToBeatenMiddleGuardOfThree(void) {
  Actor_RunRepeatedMotion(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 2);
  Event_SetMessageId(MsgLunpaRightRightGive);
  // "All right, all right. We give up! If you're here for Hammet,..."
  Event_ShowMessage(ACTOR_UPPER_FLOOR_THREE_GUARDS_MIDDLE, 0);
}

/// Talking to the west one of the three beaten guards in the upper floor's
/// guard room, he says the party is tougher than they thought and lets it go.
/// "Who'd have thought they'd be so tough?"
static void LunpaFortress_TalkToBeatenWestGuardOfThree(void) {
  Actor_RunRepeatedMotion(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 2);
  Event_SetMessageId(MsgLunpaGuysTougherThought);
  // "You guys are tougher than we thought-we'll let you go this..."
  Event_ShowMessage(ACTOR_UPPER_FLOOR_THREE_GUARDS_WEST, 0);
}

/// Talking to Dodonpa's wife in the family's room on the upper floor, she asks
/// "Do you know where Dodonpa is?"
static void LunpaFortress_AskWhereDodonpaIs(void) {
  Event_SetMessageId(MsgLunpaKnowWhereDodonpa);
  // "Do you know where Dodonpa is?"
  // Yes: "Really? That's fine, as long as he's in the fortress."
  // No: "Sigh. Don't tell me he's gone off, trying to relive his..."
  Event_AskYesNo(ACTOR_UPPER_FLOOR_DODONPAS_WIFE, 0);
}

/// Talking to the old man hiding in the ground floor's room of barrels, he
/// jumps the first time ("Wh-Wh-Who goes there!"), then asks whether the party
/// sneaked into the fortress in search of something; told yes, he promises to
/// keep quiet.
static void LunpaFortress_TalkToStartledOldMan(void) {
  s32 frightMessage;
  s32 purposeMessage;

  if (Event_GetFlag(FLAG_LUNPA_FORTRESS_OLD_MAN_PROMISED_SILENCE) != 0) {
    Event_SetMessageId(MsgLunpaWontTellAnyone);
    // "I won't tell anyone, but you should leave before the guards..."
    Event_ShowMessage(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
  } else {
    Event_Start();
    Actor_FaceActor(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, ACTOR_PARTY_LEADER, 0);
    if (GameFlag_Get(FLAG_LUNPA_FORTRESS_OLD_MAN_STARTLED) == 0) {
      Actor_Jump(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 4, 0);
      Actor_Stop(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN);
      Actor_WaitForScriptEndById(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN);
      Event_Wait(20);
      frightMessage = MsgLunpaWhWhWho;
      Event_SetMessageId(frightMessage);
      // "Wh-Wh-Who goes there!"
      Event_ShowMessage(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
      Actor_ShowEmote(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN,
                      EMOTE_IN_FRONT | EMOTE_WINCE, 30);
      Event_SetMessageId(frightMessage + 1);
      // "Wh-Who are you?"
      Event_ShowMessage(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
      Event_Wait(30);
      Actor_SetAnimation(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, ANIM_SHAKE_HEAD);
      Event_Wait(30);
    }
    purposeMessage = MsgLunpaDontLookNearly;
    Event_SetMessageId(purposeMessage);
    // "You don't look nearly as mean as the thieves in the fortress."
    Event_ShowMessage(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
    Actor_ShowEmote(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN,
                    EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Event_SetMessageId(purposeMessage + 1);
    // "Did you sneak into Lunpa Fortress in search of something?"
    Event_OpenMessage(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_SetMessageId(purposeMessage + 2);
      // "The great merchant Hammet? I'd tell you if I knew, but I don't."
      Event_OpenMessage(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
      GameFlag_Set(FLAG_LUNPA_FORTRESS_OLD_MAN_PROMISED_SILENCE);
    } else {
      Event_SetMessageId(purposeMessage + 3);
      // "Who would sneak into such a dangerous place just for fun?"
      Event_OpenMessage(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
    }
    GameFlag_Set(FLAG_LUNPA_FORTRESS_OLD_MAN_STARTLED);
    Event_End();
  }
}

/// Mind Read on the old man in the room of barrels: once he has promised to
/// keep quiet he wonders whether Hammet is why the lower caves are closed off;
/// before that he starts and talks as LunpaFortress_TalkToStartledOldMan.
static void LunpaFortress_ReadStartledOldManMind(void) {
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_OLD_MAN_PROMISED_SILENCE)) {
    Event_SetMessageId(MsgLunpaMaybeMerchantReason);
    // "Maybe that merchant is the reason they've closed off the..."
    Event_ShowMessage(ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, 0);
  } else {
    gFieldState->psynergy = 0;
    FieldPsynergy_StopMindReadWave();
    LunpaFortress_TalkToStartledOldMan();
  }
}

static void UNUSED LunpaFortress_DoNothingF(void) {}

/// Talking to Dodonpa behind the door of Hammet's old cell, after Donpa's
/// judgment, he begs his father's forgiveness: "I was wrong! I'll turn over a
/// new leaf, Dad! Please forgive me!" With the Cell Key in the party, "The key
/// doesn't work. Maybe Donpa changed the locks."
///
/// The party talks to an invisible actor standing in front of the door
/// (ACTOR_INVISIBLE_TOADONPA_DODONPA).
static void LunpaFortress_TalkToJailedDodonpa(void) {
  s32 pleaMessage = MsgLunpaWrongTurnOver;

  Event_SetMessageId(pleaMessage);
  // "I was wrong! I'll turn over a new leaf, Dad! Please forgive me!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TOADONPA_DODONPA, 0);
  if (PartyInventory_FindMember(ITEM_CELL_KEY) != -1) {
    // "The key doesn't work. Maybe Donpa changed the locks."
    FieldMessage_Show(pleaMessage + 2, 1);
  }
}

/// Mind Read on Dodonpa in his cell after Donpa's judgment: "I can't believe
/// what you did to me! When I get out of here, I'll get you all!"
static void LunpaFortress_ReadJailedDodonpaMind(void) {
  Event_SetMessageId(MsgLunpaCantBelieveWhen);
  // "I can't believe what you did to me! When I get out of here,..."
  Event_ShowMessage(ACTOR_TOADONPA_DODONPA, 0);
}

/// Sets each of the four scenes up. In the cells, coming back by the stairs
/// after Donpa's judgment, Hammet is gone, one of Dodonpa's men stands by the
/// door of the cell where Dodonpa is locked up, and Dodonpa is behind it;
/// coming back from the battle with Toadonpa plays the judgment
/// (LunpaFortress_DonpaJudgesDodonpa).
s32 Scene_Initialize(void) {
  struct Actor *mapActor;

  gLunpaFortressRandomPick = Random_Below(7);
  if (gMainRoutineState.next == SCENE_LUNPA_FORTRESS_GROUND_FLOOR) {
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    LunpaFortress_InitializeGroundFloor();
  }
  if (gMainRoutineState.next == SCENE_LUNPA_FORTRESS_UPPER_FLOOR)
    LunpaFortress_InitializeUpperFloor();
  if (gMainRoutineState.next == SCENE_LUNPA_FORTRESS_CAVE)
    LunpaFortress_InitializeCave();
  if (gMainRoutineState.next == SCENE_LUNPA_FORTRESS_TOADONPA) {
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_TOADONPA_HAMMET), 0);
    Actor_FaceDirection(ACTOR_TOADONPA_HAMMET, FACING_EAST, 0);
    Actor_SetAnimation(ACTOR_TOADONPA_HAMMET, ANIM_STILL);
    LunpaFortress_RestoreToadonpaDoors();
    mapActor = Actor_Get(ACTOR_INVISIBLE_TOADONPA_WEST_JAR);
    if (mapActor != NULL)
      Actor_SetSpriteFlags(mapActor, 0);
    mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    mapActor = Actor_Get(ACTOR_INVISIBLE_TOADONPA_CHEST);
    if (mapActor != NULL)
      Actor_SetSpriteFlags(mapActor, 0);
    mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    mapActor = Actor_Get(ACTOR_INVISIBLE_TOADONPA_EAST_JAR);
    if (mapActor != NULL)
      Actor_SetSpriteFlags(mapActor, 0);
    mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    if (gMainRoutineState.entrance == LUNPA_FORTRESS_TOADONPA_AFTER_BATTLE) {
      MapGrid_SetShownLayers(LUNPA_FORTRESS_ROOM_LAYERS);
      LunpaFortress_DonpaJudgesDodonpa();
    }
    if (gMainRoutineState.entrance ==
        LUNPA_FORTRESS_TOADONPA_FROM_LUNPA_FORTRESS_CAVE_NORTHWEST) {
      MapGrid_SetShownLayers(LUNPA_FORTRESS_ROOM_LAYERS);
      if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
        Actor_SetPosition(ACTOR_TOADONPA_HAMMET, 0, 0);
        Actor_SetPosition(ACTOR_TOADONPA_CELL_GUARD, 432, 344);
        Actor_FaceDirection(ACTOR_TOADONPA_CELL_GUARD, FACING_SOUTH_SOUTHWEST,
                            0);
        Actor_SetPosition(ACTOR_TOADONPA_DODONPA, 456, 288);
        Actor_FaceDirection(ACTOR_TOADONPA_DODONPA, FACING_SOUTH_SOUTHWEST, 0);
        Actor_SetPosition(ACTOR_INVISIBLE_TOADONPA_DODONPA, 456, 320);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_INVISIBLE_TOADONPA_DODONPA), 0);
      }
    }
    mapActor = Actor_Get(ACTOR_INVISIBLE_TOADONPA_BOSS_PART);
    if (mapActor != NULL)
      Actor_SetSpriteFlags(mapActor, 0);
    mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    mapActor->scaleX = 4 * FIXED_ONE / 5;
  }
  return 0;
}

/// Sets the ground floor up by the way the party came in: the guards who watch
/// that part of the floor, and the cell doors already opened.
static void LunpaFortress_InitializeGroundFloor(void) {
  LunpaFortress_RestoreGroundFloorDoors();
  switch (gMainRoutineState.entrance) {
  case LUNPA_FORTRESS_GROUND_FLOOR_ENTRANCE_FROM_MAIN_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_ENTRANCE:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTH_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_FEAST_ROOM:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_NORTHEAST_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_MAIN_PASSAGE_FROM_EAST_PASSAGE:
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Task_AddFrame(LunpaFortress_CheckEntranceGuard, TASK_PASS_FRAME);
    Task_AddFrame(LunpaFortress_CheckFirstEntranceSentry, TASK_PASS_FRAME);
    Task_AddFrame(LunpaFortress_CheckSecondEntranceSentry, TASK_PASS_FRAME);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    break;
  case LUNPA_FORTRESS_GROUND_FLOOR_FEAST_ROOM_FROM_MAIN_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_BARREL_ROOM_FROM_NORTHEAST_PASSAGE:
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_ROOM_LAYERS);
    break;
  case LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_MAIN_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_NORTHEAST_PASSAGE:
  case LUNPA_FORTRESS_GROUND_FLOOR_EAST_PASSAGE_FROM_TREASURE_ROOM:
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Task_AddFrame(LunpaFortress_CheckGroundFloorPatrol, TASK_PASS_FRAME);
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
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    Task_AddFrame(LunpaFortress_CheckNortheastHallGuard, TASK_PASS_FRAME);
    break;
  default:
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    break;
  }
  Actor_SetSpriteColorById(ACTOR_GROUND_FLOOR_PATROL_GUARD, SPRITE_COLOR_GRB);
  Actor_SetSpriteColorById(ACTOR_GROUND_FLOOR_NORTHEAST_HALL_GUARD,
                           SPRITE_COLOR_GRB);
  Actor_SetSpriteColorById(ACTOR_GROUND_FLOOR_SECOND_SENTRY, SPRITE_COLOR_GRB);
  Actor_SetSpriteColorById(ACTOR_GROUND_FLOOR_ENTRANCE_GUARD, SPRITE_COLOR_GRB);
  Actor_SetSpriteColorById(ACTOR_GROUND_FLOOR_FIRST_SENTRY, SPRITE_COLOR_GRB);
  wait(1);
}

/// Sets the upper floor up by the way the party came in: the guards who watch
/// that part, the guards already beaten, the Cell Key once taken, and Donpa's
/// attendant at the door to Tonic's room once the reward has been shown.
static void LunpaFortress_InitializeUpperFloor(void) {
  struct Actor *keyActor;

  LunpaFortress_RestoreUpperFloorDoors();
  Actor_SetSpriteColorById(ACTOR_UPPER_FLOOR_FIRST_PATROL, SPRITE_COLOR_GRB);
  Actor_SetSpriteColorById(ACTOR_UPPER_FLOOR_SECOND_PATROL, SPRITE_COLOR_GRB);
  Actor_SetSpriteColorById(ACTOR_UPPER_FLOOR_HALL_PATROL, SPRITE_COLOR_GRB);
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_CELL_KEY_ROOM_GUARD_BEATEN)) {
    Actor_SetPosition(ACTOR_UPPER_FLOOR_CELL_KEY_ROOM_GUARD, 0, 0);
  }
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_MIDDLE_HALL_GUARD_BEATEN)) {
    Actor_SetPosition(ACTOR_UPPER_FLOOR_MIDDLE_HALL_GUARD, 0, 0);
  }
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_DONPAS_DOOR_GUARD_BEATEN)) {
    Actor_SetPosition(ACTOR_UPPER_FLOOR_DONPAS_DOOR_GUARD, 0, 0);
  }
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_CELL_KEY)) {
    Actor_SetPosition(ACTOR_UPPER_FLOOR_CELL_KEY, 0, 0);
  }
  switch (gMainRoutineState.entrance) {
  case LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_SOUTH:
  case LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_NORTH_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_WEST_PASSAGE_FROM_MIDDLE_PASSAGE:
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    Task_AddFrame(LunpaFortress_CheckPatrolPair, TASK_PASS_FRAME);
    wait(1);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_FAMILY_ROOM_FROM_MIDDLE_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_WOMENS_ROOM_FROM_NORTH_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_SOUTH_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_TONIC_ROOM_FROM_DONPAS_ROOM:
  case LUNPA_FORTRESS_UPPER_FLOOR_DONPAS_ROOM_FROM_TONIC_ROOM:
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_ROOM_LAYERS);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_UPPER_FLOOR_DONPA), 0);
    if (GameFlag_Get(FLAG_LUNPA_FORTRESS_REWARD_SHOWN)) {
      Actor_SetPosition(ACTOR_UPPER_FLOOR_ATTENDANT, 872, 120);
    }
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_CELL_KEY_ROOM:
  case LUNPA_FORTRESS_UPPER_FLOOR_EAST_PASSAGE_FROM_SOUTH_PASSAGE:
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    Task_AddFrame(LunpaFortress_CheckHallPatrol, TASK_PASS_FRAME);
    wait(1);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_NORTH_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_FROM_SOUTH_PASSAGE:
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    if (GameFlag_Get(FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN)) {
      LunpaFortress_PlaceThreeBeatenGuards();
    }
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_GUARD_ROOM_AFTER_BATTLE:
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, LUNPA_RETURN_TRANSITION_VARIANT);
    LunpaFortress_PlaceThreeBeatenGuards();
    break;
  case LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_MIDDLE_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_CELL_KEY_ROOM_FROM_EAST_PASSAGE:
  case LUNPA_FORTRESS_UPPER_FLOOR_FROM_LUNPA_FORTRESS_GROUND_FLOOR_NORTH:
    Task_AddFrame(LunpaFortress_CheckUpperFloorDoorApproach, TASK_PASS_FRAME);
    break;
  default:
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
    break;
  }
  keyActor = Actor_Get(ACTOR_UPPER_FLOOR_CELL_KEY);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_UPPER_FLOOR_CELL_KEY), 0);
  Actor_SetSpritePriority(ACTOR_UPPER_FLOOR_CELL_KEY, 1);
  keyActor->scaleX = 3 * FIXED_ONE / 4;
  keyActor->scaleY = 3 * FIXED_ONE / 4;
}

/// Sets the cave up: the crates and ivy as the party left them, and the
/// doorways already opened.
static void LunpaFortress_InitializeCave(void) {
  struct Actor *mapActor;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  LunpaFortress_RestoreCaveDoors();
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_CRATES_MOVED)) {
    LunpaFortress_RestoreLoweredCrates();
  }
  Event_SetFlag(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_UNDER_IVY);
  Event_SetFlag(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_UNDER_IVY);
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_IVY_BLOWN_AWAY)) {
    Actor_SetPosition(ACTOR_CAVE_FIRST_SWITCH_IVY, 0, 0);
    GameFlag_Clear(FLAG_LUNPA_FORTRESS_FIRST_SWITCH_UNDER_IVY);
  }
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_PASSAGE_IVY_BLOWN_AWAY)) {
    Actor_SetPosition(ACTOR_CAVE_PASSAGE_IVY, 0, 0);
    LunpaFortress_OpenIvyPassage();
  }
  if (GameFlag_Get(FLAG_LUNPA_FORTRESS_SECOND_SWITCH_IVY_BLOWN_AWAY)) {
    Actor_SetPosition(ACTOR_CAVE_SECOND_SWITCH_IVY, 0, 0);
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
  if (mapActor != NULL) {
    mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  mapActor = Actor_Get(ACTOR_CAVE_PASSAGE_IVY);
  if (mapActor != NULL) {
    mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  mapActor = Actor_Get(ACTOR_CAVE_SECOND_SWITCH_IVY);
  if (mapActor != NULL) {
    mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  mapActor = Actor_Get(ACTOR_INVISIBLE_CAVE_JAR);
  if (mapActor != NULL) {
    Actor_SetSpriteFlags(mapActor, 0);
  }
  mapActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  mapActor = Actor_Get(ACTOR_CAVE_CRATES);
  if (mapActor != NULL) {
    mapActor->collisionFlags |= ACTOR_NEAR_PSYNERGY;
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_INVISIBLE_CAVE_JAR), 0);
  Event_End();
  MapGrid_SetShownLayers(LUNPA_FORTRESS_PASSAGE_LAYERS);
}

/// The ground floor's opened cell doors stay open.
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

/// The upper floor's opened cell doors stay open.
static void LunpaFortress_RestoreUpperFloorDoors(void) {
  if (GameFlag_Get(UPPER_FLOOR_DOOR_FLAG)) {
    LunpaFortress_OpenUpperFloorDoor(0);
  }
  if (GameFlag_Get(UPPER_FLOOR_DOOR_FLAG + 1)) {
    LunpaFortress_OpenUpperFloorDoor(1);
  }
}

/// The cave's opened cell doors stay open.
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

/// The cells' opened doors stay open.
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

/// Lunpa Fortress keeps its pushed-actor tables with its read-only data, though
/// the shared declarations in field.h and staged_actor.c are not const.
#define PUSHED_ACTOR_TABLES_READ_ONLY
#include "shared/pushed_actor_tables.c"
// clang-format off

/// Which of the four challenges the guards who fight greet the party with,
/// drawn on every arrival.
s32 gLunpaFortressRandomPick = 0;

// The patrolling guards' rounds: the west hall's pair walk east and west
// across it in step, and the east halls' guards walk theirs to and fro, the
// ground floor's pausing at each end.
s32 gLunpaFortressSecondPatrolRoute[] = {
    SC_ACCELERATION(1.1),
    SC_SPEED(1.1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(192, 0, 288),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(88, 0, 288),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gLunpaFortressFirstPatrolRoute[] = {
    SC_ACCELERATION(1.1),
    SC_SPEED(1.1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(192, 0, 192),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(88, 0, 192),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// Donpa lies asleep in his bed.
s32 gLunpaFortressDonpaSleeps[] = {
    SC_ANIMATION(ANIM_DONPA_IN_BED),
    SC_END,
};

s32 gLunpaFortressGroundFloorPatrolRoute[] = {
    SC_ACCELERATION(0.2),
    SC_SPEED(0.6),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(808, 0, 648),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(25),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(808, 0, 520),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(20),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(712, 0, 520),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(25),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(808, 0, 520),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(40),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(808, 0, 648),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(20),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(744, 0, 648),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gLunpaFortressHallPatrolRoute[] = {
    SC_ACCELERATION(0.2),
    SC_SPEED(0.6),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(920, 0, 648),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(840, 0, 648),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(840, 0, 592),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(840, 0, 648),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(920, 0, 648),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(920, 0, 592),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
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

/// The ground floor's exits. Unused: its rows first copy West Lunpa Cave's,
/// which no scene of this overlay reads.
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
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 296, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 252, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        552, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BUNZA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gLunpaFortressGroundFloorActorPlacements[] = {
    // Unused: four chests at the map's origin, likely those the unused sword
    // pickups open (ACTOR_FIRST_SWORD_CHEST).
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 760, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        392, 0, 728, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 440, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 464, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        192, 0, 432, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        808, 0, 232, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gLunpaFortressGroundFloorPatrolRoute,
        736, 0, 648, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        192, 0, 464, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        424, 0, 392, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        432, 0, 768, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gLunpaFortressUpperFloorActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KEY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 856, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gLunpaFortressFirstPatrolRoute,
        88, 0, 192, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gLunpaFortressSecondPatrolRoute,
        88, 0, 288, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        456, 0, 392, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        680, 0, 88, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        704, 0, 104, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        680, 0, 128, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        680, 0, 824, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, (s32)gLunpaFortressHallPatrolRoute,
        920, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        88, 0, 616, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 560, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        148, 0, 616, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        72, 0, 560, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        664, 0, 360, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        712, 0, 344, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_DONPA, CONDITION_ALWAYS, (s32)gLunpaFortressDonpaSleeps,
        944, 0, 122, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        904, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_TONIC), MOVEMENT_TYPE_STAND,
        872, 0, 336, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gLunpaFortressCaveActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 576, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        440, 0, 282, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        56, 0, 176, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        504, 0, 168, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 136, FACING_NORTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gLunpaFortressToadonpaActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        56, 0, 72, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 72, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 72, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TOADONPA, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        472, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_HAMMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        528, 0, 216, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DODONPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DONPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        464, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

/// Unused: an empty trigger table nothing returns.
struct Trigger gLunpaFortressNoTriggers[] = {
    { TRIGGER_TABLE_END },
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
    { TRIGGER_STEP, CELL_BEHAVIOR_LUNPA_FORTRESS_ENTRANCE_GUARD_POST, FLAG_LUNPA_FORTRESS_ENTRANCE_GUARD_CAUGHT_PARTY, EVENT(LunpaFortress_EntranceGuardThrowsPartyOut) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LUNPA_FORTRESS_SENTRIES_POST, FLAG_LUNPA_FORTRESS_SENTRIES_CAUGHT_PARTY, EVENT(LunpaFortress_SentriesThrowPartyOut) },
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
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GROUND_FLOOR_STARTLED_OLD_MAN, CONDITION_ALWAYS, EVENT(LunpaFortress_ReadStartledOldManMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GROUND_FLOOR_SECOND_SENTRY, CONDITION_ALWAYS, EVENT(LunpaFortress_ReadSentryMind) },
    { TRIGGER_RAISED, RAISED_GROUND_FLOOR_PATROL_SPOTTED, CONDITION_ALWAYS, EVENT(LunpaFortress_GroundFloorPatrolThrowsPartyOut) },
    { TRIGGER_RAISED, RAISED_NORTHEAST_HALL_SPOTTED, CONDITION_ALWAYS, EVENT(LunpaFortress_NortheastHallGuardThrowsPartyOut) },
    { TRIGGER_RAISED, RAISED_ENTRANCE_GUARD_SPOTTED, CONDITION_ALWAYS, EVENT(LunpaFortress_EntranceGuardThrowsPartyOut) },
    { TRIGGER_RAISED, RAISED_SENTRIES_SPOTTED, CONDITION_ALWAYS, EVENT(LunpaFortress_SentriesThrowPartyOut) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_LUNPA_FORTRESS_CHEST_ELIXIR, FLAG_LUNPA_FORTRESS_CHEST_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_LUNPA_FORTRESS_CHEST_LUCKY_MEDAL, FLAG_LUNPA_FORTRESS_CHEST_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_LUNPA_FORTRESS_CHEST_MYTHRIL_CIRCLET, FLAG_LUNPA_FORTRESS_CHEST_MYTHRIL_CIRCLET, FIND_ITEM(ITEM_MYTHRIL_CIRCLET) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_LUNPA_FORTRESS_CHEST_COINS, FLAG_LUNPA_FORTRESS_CHEST_COINS, FIND_COINS(100) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_LUNPA_FORTRESS_BARREL_VIAL, FLAG_LUNPA_FORTRESS_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { TRIGGER_TABLE_END },
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
    { TRIGGER_STEP, CELL_BEHAVIOR_LUNPA_FORTRESS_BEFORE_MIDDLE_HALL_GUARD, FLAG_LUNPA_FORTRESS_MIDDLE_HALL_GUARD_BEATEN, EVENT(LunpaFortress_FightMiddleHallGuard) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LUNPA_FORTRESS_BEFORE_THREE_GUARDS, FLAG_LUNPA_FORTRESS_THREE_GUARDS_BEATEN, EVENT(LunpaFortress_FightThreeGuards) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LUNPA_FORTRESS_BEFORE_DONPAS_DOOR_GUARD, FLAG_LUNPA_FORTRESS_DONPAS_DOOR_GUARD_BEATEN, EVENT(LunpaFortress_FightDonpasDoorGuard) },
    { TRIGGER_STEP, CELL_BEHAVIOR_LUNPA_FORTRESS_BEFORE_CELL_KEY_ROOM_GUARD, FLAG_LUNPA_FORTRESS_CELL_KEY_ROOM_GUARD_BEATEN, EVENT(LunpaFortress_FightCellKeyRoomGuard) },
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
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UPPER_FLOOR_DONPA, CONDITION_ALWAYS, EVENT(LunpaFortress_ReadDonpaMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_UPPER_FLOOR_ATTENDANT, CONDITION_ALWAYS, EVENT(LunpaFortress_ReadAttendantMind) },
    { TRIGGER_RAISED, RAISED_PATROL_PAIR_SPOTTED, CONDITION_ALWAYS, EVENT(LunpaFortress_PatrolPairThrowsPartyOut) },
    { TRIGGER_RAISED, RAISED_HALL_PATROL_SPOTTED, CONDITION_ALWAYS, EVENT(LunpaFortress_HallPatrolThrowsPartyOut) },
    { TRIGGER_TABLE_END },
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
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_LUNPA_FORTRESS_CRATES_GAP, CONDITION_ALWAYS, EVENT(LunpaFortress_CratesSink) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_LUNPA_FORTRESS_CRATES_PATH, CONDITION_ALWAYS, EVENT(LunpaFortress_PushCrates) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_FORTRESS_SECOND_SWITCH, FLAG_LUNPA_FORTRESS_SECOND_SWITCH_UNDER_IVY, EVENT(LunpaFortress_FlipSecondSwitch) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LUNPA_FORTRESS_FIRST_SWITCH, FLAG_LUNPA_FORTRESS_FIRST_SWITCH_UNDER_IVY, EVENT(LunpaFortress_FlipFirstSwitch) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(LunpaFortress_RevealSwitches) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(LunpaFortress_HideSwitches) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_CAVE_FIRST_SWITCH_IVY, CONDITION_ALWAYS, EVENT(LunpaFortress_BlowAwayFirstSwitchIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_CAVE_PASSAGE_IVY, CONDITION_ALWAYS, EVENT(LunpaFortress_BlowAwayPassageIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_CAVE_SECOND_SWITCH_IVY, CONDITION_ALWAYS, EVENT(LunpaFortress_BlowAwaySecondSwitchIvy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CAVE_CRATES, FLAG_LUNPA_FORTRESS_CRATES_MOVED, EVENT(LunpaFortress_MoveCrates) },
    { TRIGGER_TABLE_END },
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
    { TRIGGER_STEP, CELL_BEHAVIOR_LUNPA_FORTRESS_HAMMETS_CELL, FLAG_LUNPA_FORTRESS_FINDING_HAMMET, EVENT(LunpaFortress_FindHammet) },
    { TRIGGER_TALK, ACTOR_TOADONPA_CELL_GUARD, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetStillInside) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOADONPA_CELL_GUARD, CONDITION_ALWAYS, MSGNO(MsgLunpaDodonpaSecretlyInside) },
    // Unused: these two rows give ACTOR_TOADONPA_HAMMET Dodonpa's lines from
    // his cell, but the player never reaches them. Before Dodonpa is jailed
    // Hammet stands in his cell, whose only way in is the row of five cells
    // of CELL_BEHAVIOR_LUNPA_FORTRESS_HAMMETS_CELL, so LunpaFortress_FindHammet
    // starts before the leader is beside him, and no ground outside the cell
    // is within Mind Read's reach of him. After the battle
    // LunpaFortress_DonpaJudgesDodonpa ends with the party leading him out,
    // and once Dodonpa is jailed he is put out of sight at (0, 0). Talking to
    // him would have shown Dodonpa's "I was wrong! I'll turn over a new leaf,
    // Dad!" (MsgLunpaWrongTurnOver).
    { TRIGGER_TALK, ACTOR_TOADONPA_HAMMET, CONDITION_ALWAYS, MSGNO(MsgLunpaWrongTurnOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOADONPA_HAMMET, CONDITION_ALWAYS, MSGNO(MsgLunpaCantBelieveWhen) },
    { TRIGGER_TALK, ACTOR_INVISIBLE_TOADONPA_DODONPA, CONDITION_ALWAYS, EVENT(LunpaFortress_TalkToJailedDodonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INVISIBLE_TOADONPA_DODONPA, CONDITION_ALWAYS, EVENT(LunpaFortress_ReadJailedDodonpaMind) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_LUNPA_FORTRESS_CHEST_POWER_BREAD, FLAG_LUNPA_FORTRESS_CHEST_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { TRIGGER_TABLE_END },
};

/// The cell doors' map cells, (x, y) pairs: Toadonpa's (east cell first), the
/// cave's, the upper floor's and the ground floor's.
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
