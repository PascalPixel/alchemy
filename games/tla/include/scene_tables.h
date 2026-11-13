#ifndef ALCHEMY_SCENE_TABLES_H
#define ALCHEMY_SCENE_TABLES_H

/// @file
/// The scene table formats: the scene table, entrances, exits, actor
/// placements and triggers, as The Broken Seal's scene_tables.h has them.

#include "fixed_math.h"
#include "scene_ids.h"
#include "types.h"
#include "system/main_routine_work.h"

struct Scene {
  s16 resourceId;
  s8 group;
  s8 variant;
  u16 mapIndex;
  s16 attributes;
};

extern const struct Scene gScenes[];

/// The sixteen headings an actor faces, in turns from east, clockwise on the
/// screen, as The Broken Seal's: the eight between the main ones turn an actor
/// a little past a main heading, which picks the side its sprite leans to.
enum Facing {
  FACING_EAST = 0,
  FACING_EAST_SOUTHEAST = TRIG_TURN / 16,
  FACING_SOUTHEAST = TRIG_TURN / 8,
  FACING_SOUTH_SOUTHEAST = TRIG_TURN * 3 / 16,
  FACING_SOUTH = TRIG_TURN / 4,
  FACING_SOUTH_SOUTHWEST = TRIG_TURN * 5 / 16,
  FACING_SOUTHWEST = TRIG_TURN * 3 / 8,
  FACING_WEST_SOUTHWEST = TRIG_TURN * 7 / 16,
  FACING_WEST = TRIG_TURN / 2,
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

enum { SCENE_TABLE_END = -1, CONDITION_ALWAYS = -1 };

/// A table row's condition: CONDITION_ALWAYS, a flag number that must be
/// clear, or IF_FLAG_SET of a flag that must be set, as The Broken Seal's.
enum { CONDITION_FLAG_SET = 0x1000 };

/// A condition that holds while flag is set.
#define IF_FLAG_SET(flag) ((flag) | CONDITION_FLAG_SET)

/// Entrance positions.
struct SceneEntrance {
  s16 entrance;     ///< the entrance number; SCENE_TABLE_END ends the table
  s16 requiredFlag; ///< a flag that must be set, or -1 for none
  s16 x;            ///< pixels
  s16 y;            ///< pixels
  s16 z;            ///< pixels
  u16 facing;       ///< 1/65536 turns (enum Facing)
  s16 unused1;
  s16 cameraLeft;   ///< pixels, -1 for none
  s16 cameraTop;    ///< pixels, -1 for none
  s16 cameraRight;  ///< pixels, -1 for none
  s16 cameraBottom; ///< pixels, -1 for none
  s16 unused2;
};

/// An exit table's header word: the scene whose exits follow, named by its
/// name (`SCENE_EXITS(Alhafra1)`). The table is read by Scene_TakeExit as the
/// scene ends, with the exit kind it ended on.
#define SCENE_EXITS(scene) ((u32)&SceneId_##scene)
/// One exit: exit kind `exit` leads to `entrance` of the scene `scene`.
/// Kind 0xff matches any exit; kind 0 or the next header sends the party to
/// the first exit's scene. The fields are added rather than ORed so the scene
/// can be its SceneId_ symbol, which the linker fills in; they never overlap.
/// bits: 27..20 exit kind | 19..12 entrance | 11..0 scene
#define SCENE_EXIT(exit, scene, entrance)                                      \
  (((u32)(exit) << 20) + ((u32)(entrance) << 12) + (u32)&SceneId_##scene)
/// An exit with a flag word after it (bit 28): the exit is taken unless `flag`
/// is set, and when it is set the search goes on to the next exit.
#define SCENE_EXIT_UNLESS(exit, scene, entrance, flag)                         \
  (SCENE_EXIT(exit, scene, entrance) + 0x10000000), (u32)(flag)
/// An exit's flag word of -1 stands for no flag: Scene_TakeExit takes the
/// exit whatever the flags hold, so a chain of exits always ends somewhere.
#define EXIT_FLAG_NONE 0xffffffff
/// The last header word: it ends the last scene's exits and the table.
#define SCENE_EXITS_END 0x1ff

/// Actor placements.
/// An actor a scene places: its sprite, the condition for placing it,
/// how it behaves, where it stands and which way it faces, and how it turns to
/// talk.
struct ActorPlacement {
  s16 sprite;    ///< the actor's sprite; -1 ends the table
  s16 condition; ///< CONDITION_ALWAYS (-1), or a flag condition
  /// A movement type (enum MovementType) or an actor script, for
  /// Actor_SetActionCallback.
  s32 behavior;
  s32 x;         ///< 16.16 fixed-point pixels
  s32 y;         ///< 16.16 fixed-point pixels
  s32 z;         ///< 16.16 fixed-point pixels
  u16 facing;    ///< 1/65536 turns (enum Facing)
  u8 talkFacing; ///< enum ActorTalkFacing
  u8 flags;
};

/// One row of a scene's actor placements, its spot in plain pixels.
#define ACTOR_PLACEMENT(sprite, condition, movement, x, y, z, facing,          \
                        talkFacing, flags)                                     \
  {sprite, condition, movement,   Q16(x), Q16(y),                              \
   Q16(z), facing,    talkFacing, flags}

/// The movement types an actor placement's behavior can name, which
/// Actor_SetActionCallback turns into scripts: The Broken Seal's first six,
/// and its reset script moved to 10. Types 7, 8, 9 and 11 are scripts of
/// The Lost Age's own.
enum MovementType {
  MOVEMENT_TYPE_STAND = 1, ///< its script only ends
  MOVEMENT_TYPE_WANDER = 2,
  MOVEMENT_TYPE_WANDER_NEAR = 3,
  MOVEMENT_TYPE_WANDER_CLOSE = 4,
  MOVEMENT_TYPE_FOLLOW_WATCHED = 5, ///< moves toward the watched actor
  MOVEMENT_TYPE_FACE_LEADER = 6,    ///< watches the leader and turns toward it
  MOVEMENT_TYPE_RESET_ACTION = 10   ///< gMovementType_ResetActionScript
};

/// How a placed actor turns when the leader talks to it
/// (Scene_TalkToActor), as The Broken Seal's. With all but TALK_FACING_KEEP
/// the leader stops and each watches the other.
enum ActorTalkFacing {
  TALK_FACING_PARTY = 0,
  /// Faces the party, then turns back to its old heading.
  TALK_FACING_PARTY_AND_BACK = 1,
  TALK_FACING_KEEP = 2,
  /// Faces the party, then keeps turning to face the leader once the talk is
  /// over (gActor_FaceWatchedScript).
  TALK_FACING_PARTY_THEN_WATCH = 3
};

#define PIXELS(value) ((value) << 16)

/// A row of a scene's trigger table.
struct Trigger {
  /// The kind and its detail; TRIGGER_TABLE_END ends the table. Item triggers
  /// read it as bits: 15..8 item | 4 on an actor | 3..0 kind (4)
  u32 control;
  /// What the trigger answers: an actor id or a tile code. Item triggers read
  /// it as bits: 15..12 facing | 11 needs facing | 7..0 target
  s16 subject;
  s16 condition; ///< CONDITION_ALWAYS (-1), or a flag condition
  u32 value;     ///< a message (FIND_MESSAGE) or an event (EVENT)
};

#define FIND_MESSAGE(message) (0x00400000 | (message))
#define EVENT(event) ((u32)(event))
#define TRIGGER_TABLE_END 0xffffffff

/// A trigger as the field reads it while the scene runs: the target is one
/// byte.
struct TriggerView {
  s32 flags; ///< the low 4 bits are the kind (TRIGGER_KIND_MASK)
  u8 value;  ///< the target (an actor id or a cell behavior)
  u8 unused05;
  s16 condition; ///< as for GameFlag_IsConditionActive
  s32 result;
};

/// A trigger's control bits 3..0, its kind.
#define TRIGGER_KIND_MASK 0xf
/// A trigger's control bits 15..12: the heading TRIGGER_FLAG_FACING compares,
/// in sixteenths of a turn.
#define TRIGGER_FACING_MASK 0xf000
/// A step trigger that needs the leader to have pushed against it for more
/// than 12 frames, or 2 with STEP_TRIGGER_FLAG_QUICK.
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
/// (Scene_RunStepTriggerOrExit). Style 0x50 plays the plain exit sound and
/// runs Func_080d5fd4 on the leader as it goes.
enum ExitStyle {
  EXIT_STYLE_NORMAL = 0x00, ///< with SE_MAP_EXIT
  EXIT_STYLE_SILENT = 0x10,
  EXIT_STYLE_STAIRS_UP = 0x20,   ///< with the stairs-up sound and walk
  EXIT_STYLE_STAIRS_DOWN = 0x30, ///< with the stairs-down sound and walk
  EXIT_STYLE_MASK = 0x70
};

/// What an examine trigger is on, EXAMINE_TRIGGER()'s control bits 8..4,
/// as The Broken Seal's, with the pond added. Target n shows message
/// MsgKey0dc4 + n, "<leader> checked the ...", as the leader checks it, unless
/// the trigger has subject bit 10; target 0 shows nothing.
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

/// Actor ids: 0 to 7 the party's characters, the placed actors from 8. A
/// party member's actor takes its character's number as its actor id, as in
/// The Broken Seal.
enum ActorId {
  ACTOR_FELIX = 4,
  ACTOR_JENNA = 5,
  ACTOR_SHEBA = 6,
  ACTOR_PIERS = 7,
  ACTOR_FIRST_PLACED = 8
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
  PSYNERGY_PHASE_FIRST = 8
};

/// A Psynergy trigger on the actor the leader faces.
#define TRIGGER_PSYNERGY_ON_ACTOR 0x10
#define PSYNERGY_ON_ACTOR(ability)                                             \
  (TRIGGER_PSYNERGY | TRIGGER_PSYNERGY_ON_ACTOR | (ability) << 8)
/// A Psynergy trigger on an actor at one phase of the cast, as
/// PSYNERGY_ANYWHERE is for any target.
#define PSYNERGY_PHASE_ON_ACTOR(ability, phase)                                \
  ((u32)(phase) << 28 | PSYNERGY_ON_ACTOR(ability))

/// A Psynergy trigger on the cell behavior ahead, at one phase of the cast.
#define PSYNERGY_PHASE_ON_CELL(ability, phase)                                 \
  ((u32)(phase) << 28 | (ability) << 8 | TRIGGER_PSYNERGY)

/// Using an item on a cell behavior, or on an actor, as The Broken Seal's
/// USE_ITEM; the item is an ItemId (item_ids.h) or an ITEM_FILTER_ value.
#define TRIGGER_ITEM_ON_ACTOR 0x10
#define USE_ITEM(item) (TRIGGER_ITEM | (item) << 8)
#define USE_ITEM_ON_ACTOR(item)                                                \
  (TRIGGER_ITEM | TRIGGER_ITEM_ON_ACTOR | (item) << 8)

/// A Psynergy trigger for any target, not a particular actor or cell.
#define TRIGGER_PSYNERGY_ANYWHERE 0x80
#define PSYNERGY_ANYWHERE(ability, phase)                                      \
  ((u32)(phase) << 28 | (ability) << 8 | TRIGGER_PSYNERGY_ANYWHERE |           \
   TRIGGER_PSYNERGY)

/// A scene overlay's own entrances, actor placements and triggers, which its
/// getters return.
extern struct SceneEntrance gSceneEntrances[];
extern struct ActorPlacement gActorPlacements[];
extern struct Trigger gSceneEvents[];

struct SceneRegion;

/// The table a scene overlay starts with (gOverlayArea): a veneer word and an
/// entry point for each of its tables.
// As ☀️'s.
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
};

#endif
