#ifndef ALCHEMY_FIELD_SCENE_H
#define ALCHEMY_FIELD_SCENE_H

/// @file
/// The scene table formats: entrances, exits, placements and events, and the
/// macros that write them.

#include "static_assert.h"
#include "scene_ids.h"
#include "scene_links.h"
#include "character_ids.h"

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

/// A table row's condition: CONDITION_ALWAYS, a flag number that must be
/// clear, or IF_FLAG_SET of a flag that must be set
/// (GameFlag_IsConditionActive).
enum {
  SCENE_TABLE_END = -1, ///< in the first column, ends a table
  CONDITION_ALWAYS = -1,
  CONDITION_FLAG_SET = 0x1000 ///< the flag must be set, not clear
};

/// A condition that holds while flag is set.
#define IF_FLAG_SET(flag) ((flag) | CONDITION_FLAG_SET)

/// Set once the party has left Vale (the world map's entrance 1): Vale's
/// scenes then place their later actors and lines, the Sanctum takes another
/// event table and Healer line, and the Sol Sanctum approach moves its sentry,
/// changes cell collision and stops sending its way down to scene 10.
enum { FLAG_PARTY_LEFT_VALE = 0x815 };

/// A scene entrance: where and facing which way the party arrives, and the
/// camera's bounds there (-1 for none).
struct SceneEntrance {
  s16 entrance;     ///< the entrance number the game state names; -1 ends
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
/// place (`SCENE_EXITS(Bilibin)`). The table is read by Scene_TakeExit as the
/// scene ends, with the exit kind it ended on.
#define SCENE_EXITS(scene) ((u32)&SceneId_##scene)
/// One exit: exit kind `exit` leads to `entrance` of the place `scene`, both
/// named in scene_links.h. Kind 0xff matches any exit; kind 0 or the next
/// header sends the party to the first exit's scene. The fields are added
/// rather than ORed so the scene can be its SceneId_ symbol, which the linker
/// fills in; they never overlap. bits: 27..20 exit kind | 19..12 entrance |
/// 11..0 scene
#define SCENE_EXIT(exit, scene, entrance)                                      \
  (((u32)(exit) << 20) + ((u32)(entrance) << 12) + (u32)&SceneId_##scene)
/// An exit with a flag word after it (bit 28). Despite the name, the exit is
/// taken only while `flag` is clear; when it is set, the search goes on to
/// the next exit.
#define SCENE_EXIT_IF(exit, scene, entrance, flag)                             \
  (SCENE_EXIT(exit, scene, entrance) + 0x10000000), (u32)(flag)
/// The last header word: it ends the last scene's exits and the table.
#define SCENE_EXITS_END 0x1ff

/// Scene numbers above the field scenes: what runs instead of a scene
/// (Scene_Run). They have no row in gField_SceneTable.
enum {
  /// Runs nothing and returns 0; no ☀️ code sends the party there.
  SCENE_BLANK = 0x1fb,
  SCENE_REEL_GAME = 0x1fc,  ///< Lucky Wheels at Tolbi's spring (colorfx.c)
  SCENE_LUCKY_DICE = 0x1fd, ///< Lucky Dice at Tolbi's spring (lucky_dice.c)
  /// A battle; the entrance is the enemy group, and the result -1 a defeat.
  SCENE_BATTLE = 0x1fe
};

/// An exit into a battle: exit kind `exit` fights enemy group `group`.
#define SCENE_EXIT_TO_BATTLE(exit, group)                                      \
  (((u32)(exit) << 20) + ((u32)(group) << 12) + SCENE_BATTLE)

/// An actor a scene places: its sprite, the condition for placing it, how it
/// behaves, where it stands and which way it faces, and how it turns to talk.
struct ScenePlacement {
  /// The actor's sprite; 0 to 7 are the party's characters, each in its own
  /// actor slot, and -1 ends the table.
  s16 sprite;
  /// -1 always, a flag | 0x1000 set, else the flag clear. Off the world map,
  /// a Djinni's flag (48 to 127) also needs flag + 80 clear, which
  /// Djinn_ResolvePendingEvent sets after a lost or fled Djinni battle: the
  /// Djinni stays hidden until the scene is entered normally again.
  s16 condition;
  /// A built-in action (enum ActorBehavior, 1 to 7) or an action script, for
  /// ObjectMotion_SetActionCallback.
  s32 behavior;
  s32 x;         ///< 16.16 fixed-point pixels
  s32 y;         ///< 16.16 fixed-point pixels
  s32 z;         ///< 16.16 fixed-point pixels
  u16 facing;    ///< 1/65536 turns (enum Facing)
  u8 talkFacing; ///< enum ActorTalkFacing; 3 follows the leader afterwards
  u8 flags;      ///< PLACEMENT_SHARE_PREVIOUS_SPRITE
};

STATIC_ASSERT(sizeof(struct ScenePlacement) == 24);
STATIC_ASSERT(offsetof(struct ScenePlacement, talkFacing) == 22);

/// The built-in actions a placement's behavior can name, which
/// ObjectMotion_SetActionCallback turns into scripts: 1 stands, 2 to 4 wander,
/// 5 follows the linked object, 6 faces the leader, 7 resets.
enum ActorBehavior {
  ACTOR_STAND = 1, ///< its script only ends
  /// Wanders about where it stands (SC_WANDER_HOME); 3 and 4 do so at their
  /// own speed and range.
  ACTOR_WANDER = 2
};

/// How a placed actor turns when the leader talks to it (Scene_TalkToActor).
/// With 0, 1 or 3 the leader stops and the two are linked; 3, unnamed, makes
/// the actor follow the leader once the talk is over.
enum ActorTalkFacing {
  TALK_FACE_PARTY = 0,
  /// Faces the party, then turns back to its old heading.
  TALK_FACE_PARTY_AND_BACK = 1,
  TALK_KEEP_FACING = 2
};

/// ScenePlacement.flags: the actor takes the previous actor's sprite
/// resource instead of loading its own, when both are single sprites.
#define PLACEMENT_SHARE_PREVIOUS_SPRITE 0x01

/// Actor ids: 0 to 7 the party's characters, the placed actors from 8.
enum { ACTOR_PARTY_LEADER = 0, ACTOR_FIRST_PLACED = 8 };

/// Whole pixels as 16.16 fixed point.
#define PIXELS(value) ((value) << 16)

/// A scene event: its kind and detail, the trigger it answers, its condition
/// and its value (a message or an event script).
struct SceneEvent {
  /// The kind and its detail, read by Scene_FindEvent and the item, Psynergy
  /// and search lookups; -1 ends the table. The detail bits depend on the kind.
  /// bits: 30..28 Psynergy phase | 15..12 touch facing | 15..8 item or ability
  /// | 11 short push | 10 facing | 9 walking | 8..4 search target
  /// | 7 Psynergy in area | 5..4 exit sound | 4 on an actor | 3..0 kind
  u32 control;
  /// What the event answers: an actor id, a tile's collision code, a marker
  /// id or a raised value. Item and Psynergy events read it as bits below;
  /// bit 10 also skips a search's checking message and a Psynergy's effect.
  /// bits: 15..12 facing | 11 needs facing | 10 own message | 7..0 target
  s16 trigger;
  s16 condition; ///< -1 always, a flag | 0x1000 set, else the flag clear
  /// A message below 0x10000, else an event function; for an exit, the exit
  /// kind its scene's exit table matches, and for a search, a FIND_ value.
  u32 value;
};

STATIC_ASSERT(sizeof(struct SceneEvent) == 12);

/// SceneEvent.control's low four bits: what the event answers. Kind 7 is the
/// talk event of an actor Halt has stopped (Scene_FindActorTalkEvent).
enum SceneEventKind {
  EVENT_TALK = 0, ///< the A button on an actor; trigger is its id
  /// Stepping onto a cell whose collision code is the trigger, when no touch
  /// event answers it; value is the exit kind.
  EVENT_EXIT = 1,
  EVENT_TOUCH = 2,  ///< stepping onto or pushing against a collision code
  EVENT_SEARCH = 3, ///< the A button on what is ahead, when it is no actor
  EVENT_ITEM = 4,   ///< an item used on a cell or an actor
  EVENT_PSYNERGY = 5,
  EVENT_RAISED = 6 ///< raised by code through FieldWork.event
};

/// A touch event that needs the leader to have pushed against it for more
/// than 12 frames, or 2 with control bit 11.
#define EVENT_TOUCH_WALKING 0x200
/// A touch event that needs the leader to face within an eighth of a turn of
/// the heading in control bits 15..12 (Scene_FindEvent).
#define EVENT_TOUCH_FACING 0x400
#define TOUCH_WALKING (EVENT_TOUCH | EVENT_TOUCH_WALKING)
/// facing is an enum Facing; only its top four bits are compared.
#define TOUCH_FACING(facing) (EVENT_TOUCH | EVENT_TOUCH_FACING | (facing))
#define TOUCH_WALKING_INTO(facing)                                             \
  (EVENT_TOUCH | EVENT_TOUCH_WALKING | EVENT_TOUCH_FACING | (facing))

/// Psynergy events run around the effect by phase, control bits 30..28
/// (Field_UsePsynergy): 1 before it, 0 after it; 5 only aims the effect at
/// the actor ahead; 3 and 2 serve a use that pays no PP.
#define EVENT_PSYNERGY_ON_ACTOR 0x10
#define PSYNERGY_ON_ACTOR(ability)                                             \
  (EVENT_PSYNERGY | EVENT_PSYNERGY_ON_ACTOR | (ability) << 8)
/// A Psynergy event on an actor at one phase of the cast, as PSYNERGY_IN_AREA
/// is for an area.
#define PSYNERGY_PHASE_ON_ACTOR(ability, phase)                                \
  ((u32)(phase) << 28 | PSYNERGY_ON_ACTOR(ability))

/// A Psynergy event for any target, not a particular actor or cell.
#define EVENT_PSYNERGY_IN_AREA 0x80
#define PSYNERGY_IN_AREA(ability, phase)                                       \
  ((u32)(phase) << 28 | (ability) << 8 | EVENT_PSYNERGY_IN_AREA |              \
   EVENT_PSYNERGY)

// Using an item (one byte) on a tile code, or on an actor, as
// Field_FindItemEvent reads them.
#define EVENT_ITEM_ON_ACTOR 0x10
#define USE_ITEM(item) (EVENT_ITEM | (item) << 8)
#define USE_ITEM_ON_ACTOR(item) (EVENT_ITEM | EVENT_ITEM_ON_ACTOR | (item) << 8)

#define SEARCH(target) (EVENT_SEARCH | (target) << 4)

/// What a search event is on, SEARCH()'s control bits 8..4. Target n shows
/// message MsgTileChecked + n as the leader checks it, unless the trigger has
/// bit 10; target 0 shows nothing.
enum SearchTarget {
  SEARCH_UNNAMED = 0,
  SEARCH_CHEST = 1,
  SEARCH_JAR,
  SEARCH_BARREL,
  SEARCH_WALL,
  SEARCH_GROUND,
  SEARCH_ROCK,
  SEARCH_HOLE,
  SEARCH_GRAVE,
  SEARCH_TREE,
  SEARCH_UNDERBRUSH,
  SEARCH_DOOR,
  SEARCH_CHIMNEY,
  SEARCH_WOODEN_BOX,
  SEARCH_BED,
  SEARCH_BOOKCASE,
  SEARCH_STONE_COFFIN,
  SEARCH_FIREPLACE,
  SEARCH_WATER,
  SEARCH_STONE_PILLAR,
  SEARCH_STALACTITE,
  SEARCH_BOARDS,
  SEARCH_FOUNTAIN,
  SEARCH_OVEN,
  SEARCH_TABLE,
  SEARCH_STONE_STATUE,
  SEARCH_STONE_TABLET,
  SEARCH_SHELF,
  SEARCH_WARDROBE,
  SEARCH_FIREWOOD,
  SEARCH_BOOKS,
  SEARCH_WELL
};

/// What a search finds, a search event's value: a function when bits 27..24
/// are set, else a type in its top twelve bits (type 5, a mimic chest's
/// battle, has no macro).
#define FIND_ITEM(item) (0x00100000 | (item))
#define FIND_COINS(coins) (0x00200000 | (coins))
/// A Psynergy Stone, which restores all PP.
#define FIND_PSYNERGY_STONE 0x00300000
#define FIND_MESSAGE(message) (0x00400000 | (message))

/// An event function as a SceneEvent value; its address is above 0x10000, so
/// it is never taken for a message.
#define EVENT_SCRIPT(script) ((u32)(script))
#define SCENE_EVENTS_END 0xffffffff ///< ends an event table

struct SceneRegion;

#endif
