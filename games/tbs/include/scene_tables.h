#ifndef ALCHEMY_SCENE_TABLES_H
#define ALCHEMY_SCENE_TABLES_H

/// @file
/// The scene table formats: entrances, exits, actor templates
/// and triggers, and the macros that write them.

#include "static_assert.h"
#include "scene_ids.h"
#include "scene_exits.h"
#include "character_ids.h"
#include "sprite_ids.h"
#include "field_work.h"

/// The eight compass headings in 1/65536 turns, from east through south: a
/// quarter turn is 0x4000, and the screen's y axis points south.
enum Facing {
  FACING_EAST = 0x0000,
  FACING_SOUTHEAST = 0x2000,
  FACING_SOUTH = 0x4000,
  FACING_SOUTHWEST = 0x6000,
  FACING_WEST = 0x8000,
  FACING_NORTHWEST = 0xa000,
  FACING_NORTH = 0xc000,
  FACING_NORTHEAST = 0xe000
};

/// A sixteenth of a turn, half the step between two headings.
enum { FACING_STEP = 0x1000 };

/// True for a heading strictly between FACING_NORTHWEST and FACING_NORTHEAST.
#define FACING_IS_NORTH(facing)                                                \
  ((u32)((facing) - (FACING_NORTHWEST + 1)) <=                                 \
   FACING_NORTHEAST - FACING_NORTHWEST - 2)

/// In the first column of a scene table, ends the table.
enum { SCENE_TABLE_END = -1 };

/// A table row's condition: CONDITION_ALWAYS, a flag number that must be
/// clear, or IF_FLAG_SET of a flag that must be set
/// (GameFlag_IsConditionActive).
enum {
  CONDITION_ALWAYS = -1,
  /// An actor template's condition that Scene_AssignViewFlags replaces
  /// with whether the actor lies within the map's view bounds, in the
  /// scenes that call it, so only the room entered is populated.
  CONDITION_IN_VIEW = 0,
  CONDITION_FLAG_SET = 0x1000 ///< the flag must be set, not clear
};

/// A condition that holds while flag is set.
#define IF_FLAG_SET(flag) ((flag) | CONDITION_FLAG_SET)

/// Set once the party has left Vale (WORLD_MAP_FROM_VALE_PLAZA): Vale's
/// scenes then place their later actors and lines, the Sanctum takes
/// another trigger table and Healer line, and the Sol Sanctum approach moves
/// its sentry, changes metatile behaviors and stops sending its way down to
/// SceneId_ValeEruption.
/// ★ Named from its area name: Vale
enum { FLAG_VALE_PARTY_LEFT = 0x815 };

/// A scene's entrance: where and facing which way the party arrives,
/// and the camera's bounds there (-1 for none).
struct SceneEntrance {
  s16 entrance;     ///< the entrance number common names; -1 ends
  s16 requiredFlag; ///< a flag that must be set, or -1 for none
  s16 x;            ///< pixels
  s16 y;            ///< pixels
  s16 z;            ///< pixels
  u16 facing;       ///< 1/65536 turns (enum Facing)
  s16 unused1;      ///< 0 in every table; not read
  s16 cameraLeft;   ///< pixels
  s16 cameraTop;    ///< pixels
  s16 cameraRight;  ///< pixels
  s16 cameraBottom; ///< pixels
  s16 unused2;      ///< 0 in every table; not read
};

STATIC_ASSERT(sizeof(struct SceneEntrance) == 24);
STATIC_ASSERT(offsetof(struct SceneEntrance, cameraLeft) == 14);

/// An exit table's header word: the scene whose exits follow, named by its
/// name (`SCENE_EXITS(Bilibin)`). The table is read by Scene_TakeExit as the
/// scene ends, with the exit kind it ended on.
#define SCENE_EXITS(map) ((u32)&SceneId_##map)
/// One exit: exit kind `exit` leads to `entrance` of the scene `scene`, both
/// named in scene_exits.h. Kind 0xff matches any exit; kind 0 or the next
/// header sends the party to the first exit's scene. The fields are added
/// rather than ORed so the scene can be its SceneId_ symbol, which the linker
/// fills in; they never overlap. bits: 27..20 exit kind | 19..12
/// entrance | 11..0 scene
#define SCENE_EXIT(exit, map, entrance)                                        \
  (((u32)(exit) << 20) + ((u32)(entrance) << 12) + (u32)&SceneId_##map)
/// An exit with a flag word after it (bit 28): the exit is taken unless `flag`
/// is set, and when it is set the search goes on to the next exit.
#define SCENE_EXIT_UNLESS(exit, map, entrance, flag)                           \
  (SCENE_EXIT(exit, map, entrance) + 0x10000000), (u32)(flag)
/// The last header word: it ends the last scene's exits and the table.
#define SCENE_EXITS_END 0x1ff

/// Sequence ids above the scene table's rows: the sequences other than the
/// scene sequence, which ParentSequence_Run runs in its place. They have no row
/// in gScenes.
enum {
  /// Runs nothing and returns 0; no ☀️ code sends the party there.
  SEQUENCE_NONE = 0x1fb,
  SEQUENCE_SLOTS = 0x1fc, ///< Lucky Wheels in Tolbi's game room,
                          ///< SceneId_StuccoRoomsC (lucky_wheels.c)
  SEQUENCE_DICE = 0x1fd,  ///< Lucky Dice in Tolbi's game room,
                          ///< SceneId_StuccoRoomsC (lucky_dice.c)
  /// A battle; the entrance is the formation, and the result -1 a
  /// defeat.
  SEQUENCE_BATTLE = 0x1fe
};

/// An exit into a battle: exit kind `exit` fights formation `group`.
#define EXIT_TO_BATTLE(exit, group)                                            \
  (((u32)(exit) << 20) + ((u32)(group) << 12) + SEQUENCE_BATTLE)

/// An actor a scene places: its sprite, the condition for placing it,
/// how it behaves, where it stands and which way it faces, and how it turns to
/// talk.
struct ActorTemplate {
  /// The actor's sprite; 0 to 7 are the party's characters, each in its
  /// own actor slot, and -1 ends the table.
  s16 sprite;
  /// -1 always, a flag | 0x1000 set, else the flag clear. Off the world map,
  /// a Djinni's flag (from FLAG_DJINN_FIRST) also needs its
  /// FLAG_DJINN_HIDING_FIRST flag clear: the Djinni stays hidden until the
  /// scene is entered normally again.
  s16 condition;
  /// A movement type (enum MovementType) or an actor script, for
  /// Actor_SetMovementType.
  s32 movement;
  s32 x;         ///< 16.16 fixed-point pixels
  s32 y;         ///< 16.16 fixed-point pixels
  s32 z;         ///< 16.16 fixed-point pixels
  u16 facing;    ///< 1/65536 turns (enum Facing)
  u8 talkFacing; ///< enum ActorTalkFacing; 3 follows the leader
                 ///< afterwards
  u8 flags;      ///< ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE
};

STATIC_ASSERT(sizeof(struct ActorTemplate) == 24);
STATIC_ASSERT(offsetof(struct ActorTemplate, talkFacing) == 22);

/// The movement types an actor template's behavior can name, which
/// Actor_SetMovementType turns into scripts.
enum MovementType {
  MOVEMENT_TYPE_STAND = 1, ///< its script only ends
  /// Wanders about where it stands (SC_WANDER_HOME), within 16 pixels.
  MOVEMENT_TYPE_WANDER = 2,
  MOVEMENT_TYPE_WANDER_NEAR = 3,   ///< as MOVEMENT_TYPE_WANDER, within 8 pixels
  MOVEMENT_TYPE_WANDER_CLOSE = 4,  ///< within 6 pixels, at its own speed
  MOVEMENT_TYPE_FOLLOW_LINKED = 5, ///< moves toward the linked actor
  MOVEMENT_TYPE_FACE_LEADER = 6,   ///< links to the leader and turns toward it
  MOVEMENT_TYPE_RESET_ACTION = 7   ///< gMovementType_ResetActionScript
};

/// How a placed actor turns when the leader talks to it
/// (Scene_TalkToActor). With all but TALK_FACING_KEEP the leader stops
/// and the two are linked.
enum ActorTalkFacing {
  TALK_FACING_PARTY = 0,
  /// Faces the party, then turns back to its old heading.
  TALK_FACING_PARTY_AND_BACK = 1,
  TALK_FACING_KEEP = 2,
  /// Faces the party, then keeps turning to face the leader once the talk is
  /// over (gActor_LinkedScript).
  TALK_FACING_PARTY_THEN_WATCH = 3
};

/// ActorTemplate.flags: the actor takes the previous
/// actor's sprite resource instead of loading its own, when both are single
/// sprites.
#define ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE 0x01

/// Actor ids: 0 to 7 the party's characters, the placed actors from 8.
/// A party member's actor takes its character's number
/// (character_ids.h) as its actor id.
enum ActorId {
  ACTOR_PARTY_LEADER = 0,
  /// ★ Named from the game: "Garet" (MsgCharacterName)
  ACTOR_GARET = 1,
  /// ★ Named from the game: "Ivan" (MsgCharacterName)
  ACTOR_IVAN = 2,
  /// ★ Named from the game: "Mia" (MsgCharacterName)
  ACTOR_MIA = 3,
  /// ★ Named from the game: "Jenna" (MsgCharacterName)
  ACTOR_JENNA = 5,
  /// ★ Named from the game: "Sheba" (MsgCharacterName)
  ACTOR_SHEBA = 6,
  ACTOR_FIRST_PLACED = 8,
  /// The last actor id, as scenes loop over every placed actor.
  ACTOR_LAST_ID = ACTOR_ID_COUNT - 1
};

/// Whole pixels as 16.16 fixed point.
#define PIXELS(value) ((value) << 16)

/// A trigger: its kind and detail, the subject it answers, its condition
/// and its value (a message or an event).
struct Trigger {
  /// The kind and its detail, read by Scene_FindTrigger and the item, Psynergy
  /// and examine trigger lookups; -1 ends the table. The detail bits depend on
  /// the kind. bits: 30..28 Psynergy phase | 15..12 step facing | 15..8 item or
  /// ability | 11 short push | 10 facing | 9 walking | 8..4 examine target | 7
  /// Psynergy anywhere | 5..4 exit sound | 4 on an actor | 3..0 kind
  u32 kind;
  /// What the trigger answers: an actor id, a metatile behavior, a treasure
  /// spot id or a raised value. Item and Psynergy triggers read it as bits
  /// below; bit 10 also skips an examine trigger's checking message and its
  /// field Psynergy. bits: 15..12 facing | 11 needs facing | 10 own message
  /// | 7..0 target
  s16 subject;
  s16 condition; ///< -1 always, a flag | 0x1000 set, else the flag clear
  /// A message below 0x10000, else an event; for an exit, the exit
  /// kind its scene's exit table matches, and for an examine trigger, a FIND_
  /// value.
  u32 value;
};

STATIC_ASSERT(sizeof(struct Trigger) == 12);

/// Trigger.subject bit 10: a Psynergy trigger runs in place of its
/// field Psynergy, as Mind Read on someone who notices it does, and an
/// examine trigger shows no checking message.
#define TRIGGER_SUBJECT_OWN_EFFECT 0x400

/// Trigger.kind's low four bits: what the trigger answers.
enum TriggerKind {
  TRIGGER_TALK = 0, ///< the A button on an actor; the subject is its id
  /// Stepping onto a cell whose metatile behavior is the subject, when no
  /// step trigger answers it; value is the exit kind.
  TRIGGER_EXIT = 1,
  TRIGGER_STEP = 2,    ///< stepping onto or pushing against a metatile behavior
  TRIGGER_EXAMINE = 3, ///< the A button on what is ahead, when it is no actor
  TRIGGER_ITEM = 4,    ///< an item used on a cell or an actor
  TRIGGER_PSYNERGY = 5,
  TRIGGER_RAISED = 6, ///< raised by code through FieldWork.raisedTrigger
  /// Talking to an actor Halt has stopped
  /// (Scene_FindActorTalkTrigger).
  TRIGGER_TALK_HALTED = 7
};

// XXX: what leaving animations 26 and 25 and arrivals 24 and 23 show;
// recordings of an exit of each style would settle it.
/// An exit trigger's control bits 5..4: how the leader leaves
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
  EXIT_STYLE_MASK = 0x30
};

/// The raised trigger the menus and shortcut keys raise instead of opening
/// while FLAG_MENUS_RAISE_TRIGGER is set, as Vault's inn sets it while the
/// party corners the suspects.
#define FIELD_RAISED_TRIGGER_MENUS 250

/// A step trigger that needs the leader to have pushed against it for more
/// than 12 frames, or 2 with STEP_TRIGGER_FLAG_QUICK.
#define STEP_TRIGGER_FLAG_WALKING 0x200
/// With STEP_TRIGGER_FLAG_WALKING, 2 frames of pushing are enough.
#define STEP_TRIGGER_FLAG_QUICK 0x800
/// An event that needs the leader to face within an eighth of a turn of the
/// heading in control bits 15..12 (Scene_FindTrigger).
#define TRIGGER_FLAG_FACING 0x400
#define STEP_TRIGGER_WALKING (TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING)
/// facing is an enum Facing; only its top four bits are compared.
#define STEP_TRIGGER_FACING(facing)                                            \
  (TRIGGER_STEP | TRIGGER_FLAG_FACING | (facing))
#define STEP_TRIGGER_WALKING_INTO(facing)                                      \
  (TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING | TRIGGER_FLAG_FACING | (facing))

/// When a Psynergy trigger runs around its field Psynergy, control bits 30..28.
enum PsynergyPhase {
  /// After the field Psynergy (Field_UsePsynergy), PSYNERGY_ON_ACTOR's phase.
  /// With
  /// PSYNERGY_EVENT_PLAYS_CAST the event plays the cast itself.
  PSYNERGY_PHASE_AFTER_CAST = 0,
  PSYNERGY_PHASE_BEFORE_CAST =
      1, ///< once the PP is paid, before the field Psynergy
  /// After a Psynergy that runs out by itself has ended:
  /// Field_RunPsynergyEndEvents
  /// runs it for a use with ACT_PACKED, as Cloak fades or Reveal runs out.
  PSYNERGY_PHASE_AFTER_END = 2,
  PSYNERGY_PHASE_BEFORE_END = 3, ///< as PSYNERGY_PHASE_AFTER_END, before it
  /// As Reveal's colours come back (FieldPsynergy_EndReveal): the scenes
  /// hide again what Reveal showed.
  PSYNERGY_PHASE_FADES = 4,
  /// Run by the field Psynergy itself as it acts (force.c, frost.c and
  /// reveal.c): as
  /// Force strikes, Frost freezes or Reveal shows what is hidden. An event in
  /// this phase also makes Field_UsePsynergy aim the field Psynergy at the
  /// actor
  /// ahead.
  PSYNERGY_PHASE_ACTS = 5,
  PSYNERGY_PHASE_ANY = 7 ///< Scene_FindPsynergyTrigger matches every phase
};

/// The control word of a Psynergy trigger (TRIGGER_PSYNERGY) in the given
/// phase, as Scene_FindPsynergyTrigger compares it (bits 30..28 phase
/// | 3..0 kind).
#define PSYNERGY_TRIGGER(phase) ((phase) << 28 | TRIGGER_PSYNERGY)
#define PSYNERGY_TRIGGER_ANY PSYNERGY_TRIGGER(PSYNERGY_PHASE_ANY)

/// A Psynergy trigger on the actor the leader faces.
#define TRIGGER_PSYNERGY_ON_ACTOR 0x10
#define PSYNERGY_ON_ACTOR(ability)                                             \
  (TRIGGER_PSYNERGY | TRIGGER_PSYNERGY_ON_ACTOR | (ability) << 8)
/// Bits of an after-cast Psynergy trigger's target word, beside the
/// actor's number.
enum PsynergyEventTarget {
  /// Field_UsePsynergy clears FLAG_PLAY_PSYNERGY_CAST and plays no cast of
  /// its
  /// own, leaving it to the event.
  PSYNERGY_EVENT_PLAYS_CAST = 0x400
};
/// A Psynergy trigger on an actor at one phase of the cast, as
/// PSYNERGY_ANYWHERE is for any target.
#define PSYNERGY_PHASE_ON_ACTOR(ability, phase)                                \
  ((u32)(phase) << 28 | PSYNERGY_ON_ACTOR(ability))

/// A Psynergy trigger for any target, not a particular actor or cell.
#define TRIGGER_PSYNERGY_ANYWHERE 0x80
#define PSYNERGY_ANYWHERE(ability, phase)                                      \
  ((u32)(phase) << 28 | (ability) << 8 | TRIGGER_PSYNERGY_ANYWHERE |           \
   TRIGGER_PSYNERGY)

// Using an item (one byte) on a metatile behavior, or on an actor, as
// Field_FindItemTrigger reads them.
#define TRIGGER_ITEM_ON_ACTOR 0x10
#define USE_ITEM(item) (TRIGGER_ITEM | (item) << 8)
#define USE_ITEM_ON_ACTOR(item)                                                \
  (TRIGGER_ITEM | TRIGGER_ITEM_ON_ACTOR | (item) << 8)

#define EXAMINE_TRIGGER(target) (TRIGGER_EXAMINE | (target) << 4)
/// An examine trigger searched while facing the given way, as a counter is
/// searched across.
#define EXAMINE_TRIGGER_FACING(facing)                                         \
  (TRIGGER_EXAMINE | TRIGGER_FLAG_FACING | (facing))
/// A talk trigger that answers only while the leader faces the given way, so
/// one actor can answer differently from each side.
#define TALK_TRIGGER_FACING(facing)                                            \
  (TRIGGER_TALK | TRIGGER_FLAG_FACING | (facing))

/// What an examine trigger is on, EXAMINE_TRIGGER()'s control bits 8..4. Target
/// n shows message MsgTileChecked + n as the leader checks it, unless the
/// trigger has bit 10; target 0 shows nothing.
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
  EXAMINE_WELL
};

/// What a search finds, an examine trigger's value: a function when bits 27..24
/// are set, else a type in its top twelve bits.
#define FIND_ITEM(item) (0x00100000 | (item))
#define FIND_COINS(coins) (0x00200000 | (coins))
/// A Psynergy Stone, which restores all PP.
#define FIND_PSYNERGY_STONE 0x00300000
#define FIND_MESSAGE(message) (0x00400000 | (message))
/// A mimic chest's battle, with the event's value its slot of
/// ENCOUNTER_ZONE_MIMIC.
#define FIND_MIMIC 0x00500000

/// An event as a Trigger value; its address is above 0x10000, so
/// it is never taken for a message.
#define EVENT(script) ((u32)(script))
#define TRIGGERS_END 0xffffffff ///< ends a trigger table

struct SceneExitRegion;

#endif
