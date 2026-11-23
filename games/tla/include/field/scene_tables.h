#ifndef ALCHEMY_SCENE_TABLES_H
#define ALCHEMY_SCENE_TABLES_H

/// @file
/// The scene table formats: the scene table, entrances, exits, actor placements
/// and triggers.

#include "fixed_math.h"
#include "scene_exits.h"
#include "scene_ids.h"
#include "types.h"
#include "system/main_routine_work.h"

/// A scene's row in gScenes: its overlay's resource, its area, its scene type,
/// its map and its attributes.
struct Scene {
  s16 resourceId; ///< the overlay Overlay_Load loads into gOverlayArea
  /// The area: entering a new one clears the FLAG_AREA_TEMP_xx flags
  /// and the step counters and moves Retreat's destination (Scene_Arrive).
  s8 area;
  s8 sceneType; ///< an enum SceneType, copied to gMainRoutineWork.sceneType
  u16 mapIndex; ///< the map loaded for it, copied to gMainRoutineWork.mapId
  /// Two nibbles the field sequence reads on arrival
  /// (Field_GetSceneAttributes).
  s16 attributes;
};

/// Every scene's row, by scene number (field/scenes.s).
extern const struct Scene gScenes[];

/// The sixteen headings an actor faces, in turns from east, clockwise on the
/// screen: the eight between the main ones turn an actor a little past a main
/// heading, which picks the side its sprite leans to.
enum Facing {
  FACING_EAST = 0,
  FACING_EAST_SOUTHEAST = TRIG_TURN / 16,
  FACING_SOUTHEAST = DEGtoANG(45),
  FACING_SOUTH_SOUTHEAST = TRIG_TURN * 3 / 16,
  FACING_SOUTH = DEGtoANG(90),
  FACING_SOUTH_SOUTHWEST = TRIG_TURN * 5 / 16,
  FACING_SOUTHWEST = TRIG_TURN * 3 / 8,
  FACING_WEST_SOUTHWEST = TRIG_TURN * 7 / 16,
  FACING_WEST = DEGtoANG(180),
  FACING_WEST_NORTHWEST = TRIG_TURN * 9 / 16,
  FACING_NORTHWEST = TRIG_TURN * 5 / 8,
  FACING_NORTH_NORTHWEST = TRIG_TURN * 11 / 16,
  FACING_NORTH = TRIG_TURN * 3 / 4,
  FACING_NORTH_NORTHEAST = TRIG_TURN * 13 / 16,
  FACING_NORTHEAST = TRIG_TURN * 7 / 8,
  FACING_EAST_NORTHEAST = TRIG_TURN * 15 / 16
};

/// A sixteenth of a turn, half the step between two headings.
enum { FACING_STEP = TRIG_TURN / 16 };
/// A sliver of a turn off a heading: an actor turning round to face the
/// opposite way turns by the side the nudge leans to.
enum { FACING_NUDGE = 10 };
/// The quarter of a turn a facing lies in, what is past it dropped: east,
/// south, west or north.
#define FACING_QUARTER(facing) ((facing) & 0xc000)
/// The eighth of a turn a facing lies in, what is past it dropped.
#define FACING_EIGHTH(facing) ((facing) & 0xe000)
/// The eighth of a turn a facing lies in, as a number from 0 (east) to 7.
#define FACING_EIGHTH_INDEX(facing) ((facing) >> 13)
/// The sixteenth of a turn a facing lies in, what is past it dropped.
#define FACING_SIXTEENTH(facing) ((facing) & 0xf000)
/// The sixteenth of a turn a facing lies in, as a number from 0 (east) to 15,
/// which indexes the scenes' sixteen-direction step tables.
#define FACING_INDEX(facing) ((facing) >> 12)
/// The facing at the start of the sixteenth of a turn numbered index.
#define FACING_FROM_INDEX(index) ((index) << 12)

/// The cardinal heading nearest to facing: the shop counters turn a customer
/// this way.
#define FACING_NEAREST_CARDINAL(facing)                                        \
  ((u16)(((facing) + DEGtoANG(45)) & ~0x3fff))

/// In the first column of a scene table, ends the table.
enum { SCENE_TABLE_END = -1 };

/// A table row's condition: CONDITION_ALWAYS, a flag number that must be
/// clear, or IF_FLAG_SET of a flag that must be set
/// (GameFlag_IsConditionActive).
enum {
  CONDITION_ALWAYS = -1,
  CONDITION_FLAG_SET = 0x1000 ///< the flag must be set, not clear
};

/// A condition that holds while flag is set.
#define IF_FLAG_SET(flag) ((flag) | CONDITION_FLAG_SET)

/// Where the party arrives by one of a scene's entrances, which way the leader
/// faces, and the camera's bounds there.
struct SceneEntrance {
  s16 entrance;     ///< the entrance number; SCENE_TABLE_END ends the table
  s16 requiredFlag; ///< a flag that must be set, or -1 for none
  s16 x;            ///< pixels
  s16 y;            ///< pixels
  s16 z;            ///< pixels
  u16 facing;       ///< 1/65536 turns (enum Facing)
  s16 unusedAfterFacing;       ///< no code names it
  s16 cameraLeft;              ///< pixels, -1 for none
  s16 cameraTop;               ///< pixels, -1 for none
  s16 cameraRight;             ///< pixels, -1 for none
  s16 cameraBottom;            ///< pixels, -1 for none
  s16 unusedAfterCameraBottom; ///< no code names it
};

/// An exit table's header word: the scene whose exits follow, named by its
/// name (`SCENE_EXITS(Alhafra)`). The table is read by Scene_TakeExit as the
/// scene ends, with the exit kind it ended on.
#define SCENE_EXITS(scene) ((u32)&SceneId_##scene)
/// One exit: exit kind `exit` leads to `entrance` of the scene `scene`.
/// EXIT_ID_ANY matches any exit; kind 0 or the next header sends the party to
/// the first exit's scene. The fields are added rather than ORed so the scene
/// can be its SceneId_ symbol, which the linker fills in; they never overlap.
/// bits: 27..20 exit kind | 19..12 entrance | 11..0 scene
#define SCENE_EXIT(exit, scene, entrance)                                      \
  (((u32)(exit) << 20) + ((u32)(entrance) << 12) + (u32)&SceneId_##scene)
/// An exit word's scene.
#define EXIT_SCENE_MASK 0xfff
/// An exit word's entrance.
#define ENTRANCE(word) (((word) & 0xff000) >> 12)
/// An exit word's exit kind.
#define EXIT_ID(word) (((word) & 0x0ff00000) >> 20)
/// An exit word's bit 28: a flag word follows it.
#define EXIT_HAS_FLAG 0x10000000
/// The exit kind that matches whatever exit the scene ended on.
#define EXIT_ID_ANY 255
/// The exit kind that leaves for the scene and entrance already chosen (a
/// battle, Retreat, an event's destination): Scene_TakeExit keeps them.
#define EXIT_KEEP_DESTINATION 999
/// An exit with a flag word after it: the exit is taken unless `flag` is set,
/// and when it is set the search goes on to the next exit.
#define SCENE_EXIT_UNLESS(exit, scene, entrance, flag)                         \
  (SCENE_EXIT(exit, scene, entrance) + EXIT_HAS_FLAG), (u32)(flag)
/// An exit's flag word of -1 stands for no flag: Scene_TakeExit takes the
/// exit whatever the flags hold, so a chain of exits always ends somewhere.
#define EXIT_FLAG_NONE 0xffffffff
/// The last header word: it ends the last scene's exits and the table.
#define SCENE_EXITS_END 511

/// An actor a scene places: its sprite, the condition for placing it,
/// how it behaves, where it stands and which way it faces, and how it turns to
/// talk.
struct ActorPlacement {
  s16 sprite;    ///< the actor's sprite; SCENE_TABLE_END ends the table
  s16 condition; ///< CONDITION_ALWAYS, or a flag condition
  /// A movement type (enum MovementType) or an actor script, for
  /// Actor_SetMovementType.
  s32 behavior;
  FIXED x;       ///< 16.16 fixed-point pixels
  FIXED y;       ///< 16.16 fixed-point pixels
  FIXED z;       ///< 16.16 fixed-point pixels
  u16 facing;    ///< 1/65536 turns (enum Facing)
  u8 talkFacing; ///< enum ActorTalkFacing
  u8 flags;
};

/// ActorPlacement.flags: the actor takes the previous actor's sprite resource
/// instead of loading its own, when both are single sprites.
#define ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE 0x01
/// ActorPlacement.flags: x is the number of a scene point to stand on, not a
/// position (Scene_FindActorPlacement).
#define ACTOR_PLACEMENT_AT_SCENE_POINT 0x02

/// One row of a scene's actor placements, its spot in plain pixels.
#define ACTOR_PLACEMENT(sprite, condition, movement, x, y, z, facing,          \
                        talkFacing, flags)                                     \
  {sprite,     condition, movement,   toFIXED(x), toFIXED(y),                  \
   toFIXED(z), facing,    talkFacing, flags}

/// The movement types an actor placement's behavior can name, which
/// Actor_SetMovementType turns into scripts.
enum MovementType {
  MOVEMENT_TYPE_STAND = 1, ///< its script only ends
  MOVEMENT_TYPE_WANDER = 2,
  MOVEMENT_TYPE_WANDER_NEAR = 3,
  MOVEMENT_TYPE_WANDER_CLOSE = 4,
  MOVEMENT_TYPE_FOLLOW_WATCHED = 5, ///< moves toward the watched actor
  MOVEMENT_TYPE_FACE_LEADER = 6,    ///< watches the leader and turns toward it
  /// gMovementType_ClearCollisionScript: clears the actor's child and
  /// collision flags (Actor_ClearChildAndCollisionFlags).
  MOVEMENT_TYPE_CLEAR_COLLISION = 7,
  /// gMovementType_SquashScript: squashes and stretches for ever.
  MOVEMENT_TYPE_SQUASH = 8,
  MOVEMENT_TYPE_TAKE_LEADER_LAYER = 9, ///< gMovementType_TakeLeaderLayerScript
  MOVEMENT_TYPE_IGNORE_RISES = 10,     ///< gMovementType_IgnoreRisesScript
  MOVEMENT_TYPE_CLEAR_CHILD = 11       ///< gMovementType_ClearChildScript
};

/// How a placed actor turns when the leader talks to it (Scene_TalkToActor).
/// With all but TALK_FACING_KEEP the leader stops and each watches the other.
enum ActorTalkFacing {
  TALK_FACING_PARTY = 0,
  /// Faces the party, then turns back to its old heading.
  TALK_FACING_PARTY_AND_BACK = 1,
  TALK_FACING_KEEP = 2,
  /// Faces the party, then keeps turning to face the leader once the talk is
  /// over (gActor_FaceWatchedScript).
  TALK_FACING_PARTY_THEN_WATCH = 3
};

/// A row of a scene's trigger table.
struct Trigger {
  /// The kind and its detail; TRIGGER_TABLE_END ends the table. Item triggers
  /// read it as bits: 15..8 item | 4 on an actor | 3..0 kind (4)
  u32 control;
  /// What the trigger answers: an actor id or a tile code. Item triggers read
  /// it as bits: 15..12 facing | 11 needs facing | 7..0 target
  s16 subject;
  s16 condition; ///< CONDITION_ALWAYS, or a flag condition
  u32 value;     ///< a message (FIND_MESSAGE) or an event (EVENT)
};

/// Trigger.subject bit 10: a Psynergy trigger runs in place of its field
/// Psynergy, as Mind Read on someone who notices it does; an item trigger's
/// event shows its own message in place of the "uses" message
/// (MsgActorUsesItem); an examine trigger shows no checking message.
#define TRIGGER_SUBJECT_OWN_EFFECT 0x400
/// Trigger.subject bit 9 on a Psynergy trigger: the cast neither flashes the
/// caster nor announces the ability (FieldEffectWork.visible and .active).
#define TRIGGER_SUBJECT_QUIET 0x200
/// An item trigger's subject: the heading it needs in bits 15..12, whether it
/// needs one, and its target, an actor or a cell behavior.
#define TRIGGER_SUBJECT_FACING 0xf000
#define TRIGGER_SUBJECT_NEEDS_FACING 0x0800
#define TRIGGER_SUBJECT_TARGET 0xff
/// A trigger's subject that answers only while the leader faces `facing`.
#define TRIGGER_FACING_SUBJECT(facing, target)                                 \
  ((s16)((facing) | TRIGGER_SUBJECT_NEEDS_FACING | (target)))
/// A trigger's value below this is a message, from it an event.
#define TRIGGER_RESULT_MESSAGE_END 65536

/// What a search finds, an examine trigger's value: an item, coins or a
/// message.
#define FIND_ITEM(item) (0x00100000 | (item))
#define FIND_COINS(coins) (0x00200000 | (coins))
#define FIND_MESSAGE(message) (0x00400000 | (message))
/// A search result's type, its top twelve bits (FIND_ITEM to FIND_MESSAGE).
#define FIND_TYPE_MASK 0xfff00000
/// Bits 27..24 of a search result, set only in a routine's address: the result
/// is then a function to call.
#define FIND_FUNCTION_BITS 0x0f000000
#define EVENT(event) ((u32)(event))
#define TRIGGER_TABLE_END 0xffffffff

/// A trigger as the field reads it while the scene runs: the target is one
/// byte.
struct TriggerView {
  s32 flags;           ///< the low 4 bits are the kind (TRIGGER_KIND_MASK)
  u8 value;            ///< the target (an actor id or a cell behavior)
  u8 unusedAfterValue; ///< no code names it
  s16 condition;       ///< as for GameFlag_IsConditionActive
  s32 result;
};

/// A trigger's control bits 3..0, its kind.
#define TRIGGER_KIND_MASK 0xf
/// A trigger's control bits 15..12: the heading TRIGGER_FLAG_FACING compares,
/// in sixteenths of a turn.
#define TRIGGER_FACING_MASK 0xf000
/// A step trigger that needs the leader to have pushed against it for more
/// than FIELD_PUSH_FRAMES, or 2 frames with STEP_TRIGGER_FLAG_QUICK.
#define STEP_TRIGGER_FLAG_WALKING 0x200
/// With STEP_TRIGGER_FLAG_WALKING, 2 frames of pushing are enough.
#define STEP_TRIGGER_FLAG_QUICK 0x800
/// An event that needs the leader to face within an eighth of a turn of the
/// heading in control bits 15..12.
#define TRIGGER_FLAG_FACING 0x400
#define STEP_TRIGGER_WALKING (TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING)
/// facing is an enum Facing; only its top four bits are compared.
#define STEP_TRIGGER_FACING(facing)                                            \
  (TRIGGER_STEP | TRIGGER_FLAG_FACING | (facing))
#define STEP_TRIGGER_WALKING_INTO(facing)                                      \
  (TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING | TRIGGER_FLAG_FACING | (facing))
#define EXAMINE_TRIGGER(target) (TRIGGER_EXAMINE | (target) << 4)
/// An examine trigger searched while facing the given way, as a counter is
/// searched across.
#define EXAMINE_TRIGGER_FACING(facing)                                         \
  (TRIGGER_EXAMINE | TRIGGER_FLAG_FACING | (facing))
/// A talk trigger that answers only while the leader faces the given way, so
/// one actor can answer differently from each side.
#define TALK_TRIGGER_FACING(facing)                                            \
  (TRIGGER_TALK | TRIGGER_FLAG_FACING | (facing))

/// An exit trigger's control bits 6..4: how the leader leaves
/// (Scene_RunStepTriggerOrExit).
enum ExitStyle {
  EXIT_STYLE_NORMAL = 0x00, ///< with SE_MAP_EXIT
  EXIT_STYLE_SILENT = 0x10,
  /// With SE_STAIRS_UP and ANIM_LEADER_LEAVE; the next scene plays
  /// ANIM_LEADER_ARRIVE (FLAG_ARRIVAL_STAIRS_UP).
  EXIT_STYLE_STAIRS_UP = 0x20,
  /// With SE_STAIRS_DOWN and ANIM_LEADER_LEAVE_ALTERNATE; the next scene
  /// plays ANIM_LEADER_ARRIVE_ALTERNATE (FLAG_ARRIVAL_STAIRS_DOWN).
  EXIT_STYLE_STAIRS_DOWN = 0x30,
  /// With SE_MAP_EXIT and a hidden layer on the leader's sprite
  /// (Exit_AddHiddenLeaderLayer).
  EXIT_STYLE_HIDDEN_LAYER = 0x50,
  EXIT_STYLE_MASK = 0x70
};

/// What an examine trigger is on, EXAMINE_TRIGGER()'s control bits 8..4. Target
/// n shows message MsgTileChecked + n, "<leader> checked the ...", as the
/// leader checks it, unless the trigger has TRIGGER_SUBJECT_OWN_EFFECT; target
/// 0 shows nothing.
enum ExamineTarget {
  EXAMINE_SILENT = 0,
  EXAMINE_CHEST = 1,
  EXAMINE_JAR,
  EXAMINE_BARREL,
  EXAMINE_WALL,
  EXAMINE_GROUND,
  EXAMINE_ROCK,
  EXAMINE_HOLE,
  EXAMINE_GRAVE,
  EXAMINE_TREE,
  EXAMINE_UNDERBRUSH,
  EXAMINE_DOOR,
  EXAMINE_CHIMNEY,
  EXAMINE_WOODEN_BOX,
  EXAMINE_BED,
  EXAMINE_BOOKCASE,
  EXAMINE_STONE_COFFIN,
  EXAMINE_FIREPLACE,
  EXAMINE_WATER,
  EXAMINE_STONE_PILLAR,
  EXAMINE_STALACTITE,
  EXAMINE_BOARDS,
  EXAMINE_FOUNTAIN,
  EXAMINE_OVEN,
  EXAMINE_TABLE,
  EXAMINE_STONE_STATUE,
  EXAMINE_STONE_TABLET,
  EXAMINE_SHELF,
  EXAMINE_WARDROBE,
  EXAMINE_FIREWOOD,
  EXAMINE_BOOKS,
  EXAMINE_WELL,
  EXAMINE_POND
};

#include "ability_ids.h"

/// Actor ids: 0 to 7 the party's characters, the placed actors from 8. A party
/// member's actor takes its character's number as its actor id.
enum ActorId {
  ACTOR_ISAAC,
  ACTOR_GARET,
  ACTOR_IVAN,
  ACTOR_MIA,
  ACTOR_FELIX,
  ACTOR_JENNA,
  ACTOR_SHEBA,
  ACTOR_PIERS,
  ACTOR_FIRST_PLACED
};

/// Trigger.kind's low four bits: what the trigger answers.
enum TriggerKind {
  TRIGGER_TALK = 0, ///< the A button on an actor; the subject is its id
  /// Stepping onto a cell whose cell behavior is the subject, when no
  /// step trigger answers it; value is the exit kind.
  TRIGGER_EXIT = 1,
  TRIGGER_STEP = 2,    ///< stepping onto or pushing against a cell behavior
  TRIGGER_EXAMINE = 3, ///< the A button on what is ahead, when it is no actor
  TRIGGER_ITEM = 4,    ///< an item used on a cell or an actor
  TRIGGER_PSYNERGY = 5,
  TRIGGER_RAISED = 6,      ///< raised by code
  TRIGGER_TALK_HALTED = 7, ///< talking to an actor Halt has stopped
  /// Pushing an actor ahead, for any actor: it runs, with the keys and the
  /// actor's id, before the leader pushes (Field_RunPushCell).
  TRIGGER_PUSH = 8,
  TRIGGER_PUSHED = 9 ///< as TRIGGER_PUSH, once the leader has pushed
};

/// When a Psynergy trigger runs around its field Psynergy, control bits 31..28.
enum PsynergyPhase {
  /// After the field Psynergy, PSYNERGY_ON_ACTOR's phase.
  PSYNERGY_PHASE_AFTER_CAST = 0,
  PSYNERGY_PHASE_BEFORE_CAST = 1, ///< once the PP is paid, before the cast
  /// After a Psynergy that runs out by itself has ended.
  PSYNERGY_PHASE_AFTER_END = 2,
  PSYNERGY_PHASE_BEFORE_END = 3, ///< as PSYNERGY_PHASE_AFTER_END, before it
  PSYNERGY_PHASE_FADES = 4,      ///< as Reveal's colours come back
  PSYNERGY_PHASE_ACTS = 5,       ///< run by the field Psynergy as it acts
  PSYNERGY_PHASE_HITS = 6, ///< as Blaze's fire reaches what it is aimed at
  /// Before every other phase, as soon as the Psynergy is chosen and Retreat
  /// has asked its question.
  PSYNERGY_PHASE_FIRST = 8,
  PSYNERGY_PHASE_ANY = 15 ///< Scene_FindPsynergyTrigger matches every phase
};
/// The phase a field Psynergy's work is left in once it has finished, which
/// the cast waits for; its acting events get it as their stage the last time
/// they run.
#define FIELD_PSYNERGY_FINISHED 999

/// A Psynergy trigger's phase bits, 31..28.
#define PSYNERGY_PHASE_SHIFT 28
#define PSYNERGY_PHASE_MASK (15u << PSYNERGY_PHASE_SHIFT)
/// The control word of a Psynergy trigger (TRIGGER_PSYNERGY) in the given
/// phase, as Scene_FindPsynergyTrigger compares it (bits 31..28 phase
/// | 3..0 kind).
#define PSYNERGY_TRIGGER(phase)                                                \
  ((u32)(phase) << PSYNERGY_PHASE_SHIFT | TRIGGER_PSYNERGY)
#define PSYNERGY_TRIGGER_ANY PSYNERGY_TRIGGER(PSYNERGY_PHASE_ANY)

/// A Psynergy trigger on the actor the leader faces.
#define TRIGGER_PSYNERGY_ON_ACTOR 0x10
#define PSYNERGY_ON_ACTOR(ability)                                             \
  (TRIGGER_PSYNERGY | TRIGGER_PSYNERGY_ON_ACTOR | (ability) << 8)
/// A Psynergy trigger on an actor at one phase of the cast, as
/// PSYNERGY_ANYWHERE is for any target.
#define PSYNERGY_PHASE_ON_ACTOR(ability, phase)                                \
  ((u32)(phase) << PSYNERGY_PHASE_SHIFT | PSYNERGY_ON_ACTOR(ability))

/// What Scene_SelectPsynergyTarget chooses: a cell behavior as it is, an
/// actor's id with PSYNERGY_TARGET_ACTOR, or PSYNERGY_TARGET_ANYWHERE.
enum PsynergyTarget {
  PSYNERGY_TARGET_ACTOR = 0x100,
  PSYNERGY_TARGET_ANYWHERE = 0x200,
  PSYNERGY_TARGET_KIND = 0xff00, ///< which of the three
  PSYNERGY_TARGET_ID = 0xff      ///< the cell behavior or actor
};

/// A Psynergy trigger on the cell behavior ahead, at one phase of the cast.
#define PSYNERGY_PHASE_ON_CELL(ability, phase)                                 \
  ((u32)(phase) << PSYNERGY_PHASE_SHIFT | (ability) << 8 | TRIGGER_PSYNERGY)

/// Using an item on a cell behavior, or on an actor; the item is an ItemId
/// (item_ids.h) or an ITEM_FILTER_ value.
#define TRIGGER_ITEM_ON_ACTOR 0x10
#define USE_ITEM(item) (TRIGGER_ITEM | (item) << 8)
#define USE_ITEM_ON_ACTOR(item)                                                \
  (TRIGGER_ITEM | TRIGGER_ITEM_ON_ACTOR | (item) << 8)

/// A Psynergy trigger for any target, not a particular actor or cell.
#define TRIGGER_PSYNERGY_ANYWHERE 0x80
#define PSYNERGY_ANYWHERE(ability, phase)                                      \
  ((u32)(phase) << PSYNERGY_PHASE_SHIFT | (ability) << 8 |                     \
   TRIGGER_PSYNERGY_ANYWHERE | TRIGGER_PSYNERGY)

/// A scene overlay's own entrances, actor placements and triggers, which its
/// getters return.
extern struct SceneEntrance gSceneEntrances[];
extern struct ActorPlacement gActorPlacements[];
extern struct Trigger gSceneEvents[];

/// A scene's exit region: the box that, while its condition holds, takes the
/// party leader out through the exit whose number is its value
/// (Field_CheckExitRegions).
struct SceneRegion {
  /// Its lower corner in pixels, inclusive; a minX of SCENE_TABLE_END ends the
  /// table.
  s16 minX, minY, minZ;
  s16 maxX, maxY, maxZ; ///< its upper corner in pixels, exclusive
  /// CONDITION_ALWAYS, a flag | CONDITION_FLAG_SET set, else the flag clear
  s16 condition;
  s16 value; ///< the exit kind, stored in FieldWork's exit
};

/// The table a scene overlay starts with (gOverlayArea): a veneer word and an
/// entry point for each of its tables.
struct SceneEntries {
  u32 initializeVeneer;
  s32 (*initialize)(void);
  u32 entrancesVeneer;
  const struct SceneEntrance *(*entrances)(void);
  u32 exitsVeneer;
  const u32 *(*exits)(void);
  u32 placementsVeneer;
  const struct ActorPlacement *(*placements)(void);
  u32 triggersVeneer;
  const struct Trigger *(*triggers)(void);
  u32 regionsVeneer;
  const struct SceneRegion *(*regions)(void);
  u32 prepareMapVeneer;
  /// Run once the scene's actors are placed, on a map other than the World
  /// Map: the scene's own edits to its map before it is shown.
  s32 (*prepareMap)(void);
};

/// The bit that marks a song in gEncounter_BattleMusic; encounter keys have it
/// clear.
#define ENCOUNTER_BATTLE_MUSIC_MARKER 0x8000
/// A song's row in gEncounter_BattleMusic, before the encounters that take it:
/// the song (a MUS_ id) with ENCOUNTER_BATTLE_MUSIC_MARKER set.
#define ENCOUNTER_BATTLE_MUSIC(song)                                           \
  (s16)(ENCOUNTER_BATTLE_MUSIC_MARKER | (song))
/// An encounter's row in gEncounter_BattleMusic: slot of encounter zone.
#define ENCOUNTER_KEY(zone, slot) (((zone) << 4) + (slot))

#endif
