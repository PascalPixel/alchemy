#ifndef ALCHEMY_MAP_TABLES_H
#define ALCHEMY_MAP_TABLES_H

/// @file
/// The scene table formats: entrances, exits, object event templates
/// and map events, and the macros that write them.

#include "static_assert.h"
#include "scene_ids.h"
#include "scene_exits.h"
#include "character_ids.h"
#include "sprite_ids.h"

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
enum { MAP_TABLE_END = -1 };

/// A table row's condition: CONDITION_ALWAYS, a flag number that must be
/// clear, or IF_FLAG_SET of a flag that must be set
/// (GameFlag_IsConditionActive).
enum {
  CONDITION_ALWAYS = -1,
  /// An object event template's condition that Scene_AssignViewFlags replaces
  /// with whether the object event lies within the map's view bounds, in the
  /// scenes that call it, so only the room entered is populated.
  CONDITION_IN_VIEW = 0,
  CONDITION_FLAG_SET = 0x1000 ///< the flag must be set, not clear
};

/// A condition that holds while flag is set.
#define IF_FLAG_SET(flag) ((flag) | CONDITION_FLAG_SET)

/// Set once the party has left Vale (WORLD_MAP_FROM_VALE_PLAZA): Vale's
/// scenes then place their later object events and lines, the Sanctum takes
/// another map event table and Healer line, and the Sol Sanctum approach moves
/// its sentry, changes metatile behaviors and stops sending its way down to
/// SceneId_ValeEruption.
/// ★ Named from its area name: Vale
enum { FLAG_VALE_PARTY_LEFT = 0x815 };

/// A scene's entrance: where and facing which way the party arrives,
/// and the camera's bounds there (-1 for none).
struct SceneEntrance {
  s16 entrance;     ///< the entrance number the save block names; -1 ends
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

/// Scene numbers above the field scenes: what runs instead of a scene
/// (SceneManager_Run). They have no row in gSceneHeaders.
enum {
  /// Runs nothing and returns 0; no ☀️ code sends the party there.
  SEQUENCE_NONE = 0x1fb,
  SEQUENCE_LUCKY_WHEELS = 0x1fc, ///< Lucky Wheels in Tolbi's game room,
                                 ///< SceneId_StuccoRoomsC (lucky_wheels.c)
  SEQUENCE_LUCKY_DICE = 0x1fd,   ///< Lucky Dice in Tolbi's game room,
                                 ///< SceneId_StuccoRoomsC (lucky_dice.c)
  /// A battle; the entrance is the formation, and the result -1 a
  /// defeat.
  SEQUENCE_BATTLE = 0x1fe
};

/// An exit into a battle: exit kind `exit` fights formation `group`.
#define EXIT_TO_BATTLE(exit, group)                                            \
  (((u32)(exit) << 20) + ((u32)(group) << 12) + SEQUENCE_BATTLE)

/// An object event a scene places: its sprite, the condition for placing it,
/// how it behaves, where it stands and which way it faces, and how it turns to
/// talk.
struct ObjectEventTemplate {
  /// The object event's sprite; 0 to 7 are the party's characters, each in its
  /// own object event slot, and -1 ends the table.
  s16 sprite;
  /// -1 always, a flag | 0x1000 set, else the flag clear. Off the world map,
  /// a Djinni's flag (from FLAG_DJINN_FIRST) also needs its
  /// FLAG_DJINN_HIDING_FIRST flag clear: the Djinni stays hidden until the
  /// scene is entered normally again.
  s16 condition;
  /// A built-in action (enum MovementType) or an action script, for
  /// ObjectEvent_SetMovementType.
  s32 movement;
  s32 x;         ///< 16.16 fixed-point pixels
  s32 y;         ///< 16.16 fixed-point pixels
  s32 z;         ///< 16.16 fixed-point pixels
  u16 facing;    ///< 1/65536 turns (enum Facing)
  u8 talkFacing; ///< enum ObjectEventTalkFacing; 3 follows the leader
                 ///< afterwards
  u8 flags;      ///< OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE
};

STATIC_ASSERT(sizeof(struct ObjectEventTemplate) == 24);
STATIC_ASSERT(offsetof(struct ObjectEventTemplate, talkFacing) == 22);

/// The built-in actions an object event template's behavior can name, which
/// ObjectEvent_SetMovementType turns into scripts.
enum MovementType {
  MOVEMENT_TYPE_STAND = 1, ///< its script only ends
  /// Wanders about where it stands (SC_WANDER_HOME), within 16 pixels.
  MOVEMENT_TYPE_WANDER = 2,
  MOVEMENT_TYPE_WANDER_NEAR = 3,   ///< as MOVEMENT_TYPE_WANDER, within 8 pixels
  MOVEMENT_TYPE_WANDER_CLOSE = 4,  ///< within 6 pixels, at its own speed
  MOVEMENT_TYPE_FOLLOW_LINKED = 5, ///< moves toward the linked object
  MOVEMENT_TYPE_FACE_LEADER = 6,   ///< links to the leader and turns toward it
  MOVEMENT_TYPE_RESET_ACTION = 7   ///< gMovementType_ResetActionScript
};

/// How a placed object event turns when the leader talks to it
/// (Scene_TalkToObjectEvent). With all but TALK_FACING_KEEP the leader stops
/// and the two are linked.
enum ObjectEventTalkFacing {
  TALK_FACING_PARTY = 0,
  /// Faces the party, then turns back to its old heading.
  TALK_FACING_PARTY_AND_BACK = 1,
  TALK_FACING_KEEP = 2,
  /// Faces the party, then keeps turning to face the leader once the talk is
  /// over (gObjectMotion_LinkedActionScript).
  TALK_FACING_PARTY_THEN_WATCH = 3
};

/// ObjectEventTemplate.flags: the object event takes the previous object
/// event's sprite resource instead of loading its own, when both are single
/// sprites.
#define OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE 0x01

/// Local ids: 0 to 7 the party's characters, the placed object events from 8.
/// A party member's object event takes its character's number
/// (character_ids.h) as its local id.
enum LocalId {
  LOCALID_PARTY_LEADER = 0,
  /// ★ Named from the game: "Garet" (MsgCharacterName)
  LOCALID_GARET = 1,
  /// ★ Named from the game: "Ivan" (MsgCharacterName)
  LOCALID_IVAN = 2,
  /// ★ Named from the game: "Mia" (MsgCharacterName)
  LOCALID_MIA = 3,
  /// ★ Named from the game: "Jenna" (MsgCharacterName)
  LOCALID_JENNA = 5,
  /// ★ Named from the game: "Sheba" (MsgCharacterName)
  LOCALID_SHEBA = 6,
  LOCALID_FIRST_PLACED = 8,
  /// The last of the 66 object event slots, as scenes loop over every placed
  /// object event.
  LOCALID_LAST_SLOT = 65
};

/// Whole pixels as 16.16 fixed point.
#define PIXELS(value) ((value) << 16)

/// A map event: its kind and detail, the trigger it answers, its condition
/// and its value (a message or a script).
struct MapEvent {
  /// The kind and its detail, read by Scene_FindEvent and the item, Psynergy
  /// and bg event lookups; -1 ends the table. The detail bits depend on the
  /// kind. bits: 30..28 Psynergy phase | 15..12 coord facing | 15..8 item or
  /// ability | 11 short push | 10 facing | 9 walking | 8..4 bg target | 7
  /// Psynergy anywhere | 5..4 exit sound | 4 on an object event | 3..0 kind
  u32 kind;
  /// What the event answers: a local id, a metatile behavior, a treasure
  /// spot id or a raised value. Item and Psynergy events read it as bits below;
  /// bit 10 also skips a bg event's checking message and a Psynergy's effect.
  /// bits: 15..12 facing | 11 needs facing | 10 own message | 7..0 target
  s16 trigger;
  s16 condition; ///< -1 always, a flag | 0x1000 set, else the flag clear
  /// A message below 0x10000, else a script function; for an exit, the exit
  /// kind its scene's exit table matches, and for a bg event, a FIND_ value.
  u32 value;
};

STATIC_ASSERT(sizeof(struct MapEvent) == 12);

/// MapEvent.trigger bit 10: a Psynergy event runs in place of the
/// Psynergy's effect, as Mind Read on someone who notices it does, and a
/// bg event shows no checking message.
#define MAP_EVENT_TRIGGER_OWN_EFFECT 0x400

/// MapEvent.kind's low four bits: what the event answers.
enum MapEventKind {
  MAP_EVENT_TALK = 0, ///< the A button on an object event; trigger is its id
  /// Stepping onto a cell whose metatile behavior is the trigger, when no coord
  /// event answers it; value is the exit kind.
  MAP_EVENT_EXIT = 1,
  MAP_EVENT_COORD = 2, ///< stepping onto or pushing against a metatile behavior
  MAP_EVENT_BG =
      3, ///< the A button on what is ahead, when it is no object event
  MAP_EVENT_ITEM = 4, ///< an item used on a cell or an object event
  MAP_EVENT_PSYNERGY = 5,
  MAP_EVENT_RAISED = 6, ///< raised by code through FieldWork.raisedEvent
  /// Talking to an object event Halt has stopped
  /// (Scene_FindObjectEventTalkEvent).
  MAP_EVENT_TALK_HALTED = 7
};

// XXX: what leaving animations 26 and 25 and arrivals 24 and 23 show;
// recordings of an exit of each style would settle it.
/// An exit event's control bits 5..4: how the leader leaves
/// (Scene_RunCoordEventOrExit).
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

/// The raised event the menus and shortcut keys raise instead of opening
/// while FLAG_MENUS_RAISE_EVENT is set, as Vault's inn sets it while the
/// party corners the suspects.
#define FIELD_RAISED_EVENT_MENUS 250

/// A coord event that needs the leader to have pushed against it for more
/// than 12 frames, or 2 with COORD_EVENT_FLAG_QUICK.
#define COORD_EVENT_FLAG_WALKING 0x200
/// With COORD_EVENT_FLAG_WALKING, 2 frames of pushing are enough.
#define COORD_EVENT_FLAG_QUICK 0x800
/// An event that needs the leader to face within an eighth of a turn of the
/// heading in control bits 15..12 (Scene_FindEvent).
#define MAP_EVENT_FLAG_FACING 0x400
#define COORD_EVENT_WALKING (MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING)
/// facing is an enum Facing; only its top four bits are compared.
#define COORD_EVENT_FACING(facing)                                             \
  (MAP_EVENT_COORD | MAP_EVENT_FLAG_FACING | (facing))
#define COORD_EVENT_WALKING_INTO(facing)                                       \
  (MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING | MAP_EVENT_FLAG_FACING |        \
   (facing))

/// When a Psynergy event runs around its effect, control bits 30..28.
enum PsynergyPhase {
  /// After the effect (Field_UsePsynergy), PSYNERGY_ON_OBJ_EVENT's phase. With
  /// PSYNERGY_SCRIPT_PLAYS_EFFECT the event plays the effect itself.
  PSYNERGY_PHASE_AFTER_CAST = 0,
  PSYNERGY_PHASE_BEFORE_CAST = 1, ///< once the PP is paid, before the effect
  /// After a Psynergy that runs out by itself has ended:
  /// Field_RunPsynergyEvents
  /// runs it for a use with ACT_PACKED, as Cloak fades or Reveal runs out.
  PSYNERGY_PHASE_AFTER_END = 2,
  PSYNERGY_PHASE_BEFORE_END = 3, ///< as PSYNERGY_PHASE_AFTER_END, before it
  /// As Reveal's colours come back (FieldEffect_EndReveal): the scenes
  /// hide again what Reveal showed.
  PSYNERGY_PHASE_EFFECT_ENDS = 4,
  /// Run by the effect itself as it acts (force.c, frost.c and reveal.c): as
  /// Force strikes, Frost freezes or Reveal shows what is hidden. An event in
  /// this phase also makes Field_UsePsynergy aim the effect at the object event
  /// ahead.
  PSYNERGY_PHASE_EFFECT_ACTS = 5,
  PSYNERGY_PHASE_ANY = 7 ///< Scene_FindPsynergyEvent matches every phase
};

/// The control word of a Psynergy event (MAP_EVENT_PSYNERGY) in the given
/// phase, as Scene_FindPsynergyEvent compares it (bits 30..28 phase
/// | 3..0 kind).
#define PSYNERGY_EVENT(phase) ((phase) << 28 | MAP_EVENT_PSYNERGY)
#define PSYNERGY_EV_ANY PSYNERGY_EVENT(PSYNERGY_PHASE_ANY)

/// A Psynergy event on the object event the leader faces.
#define MAP_EVENT_PSYNERGY_ON_OBJ_EVENT 0x10
#define PSYNERGY_ON_OBJ_EVENT(ability)                                         \
  (MAP_EVENT_PSYNERGY | MAP_EVENT_PSYNERGY_ON_OBJ_EVENT | (ability) << 8)
/// Bits of an after-cast Psynergy event's target word, beside the object
/// event's number.
enum PsynergyEventTarget {
  /// Field_UsePsynergy clears FLAG_PSYNERGY_EFFECT and plays no effect of its
  /// own, leaving it to the event's script.
  PSYNERGY_SCRIPT_PLAYS_EFFECT = 0x400
};
/// A Psynergy event on an object event at one phase of the cast, as
/// PSYNERGY_ANYWHERE is for any target.
#define PSYNERGY_PHASE_ON_OBJ_EVENT(ability, phase)                            \
  ((u32)(phase) << 28 | PSYNERGY_ON_OBJ_EVENT(ability))

/// A Psynergy event for any target, not a particular object event or cell.
#define MAP_EVENT_PSYNERGY_ANYWHERE 0x80
#define PSYNERGY_ANYWHERE(ability, phase)                                      \
  ((u32)(phase) << 28 | (ability) << 8 | MAP_EVENT_PSYNERGY_ANYWHERE |         \
   MAP_EVENT_PSYNERGY)

// Using an item (one byte) on a metatile behavior, or on an object event, as
// Field_FindItemEvent reads them.
#define MAP_EVENT_ITEM_ON_OBJ_EVENT 0x10
#define USE_ITEM(item) (MAP_EVENT_ITEM | (item) << 8)
#define USE_ITEM_ON_OBJ_EVENT(item)                                            \
  (MAP_EVENT_ITEM | MAP_EVENT_ITEM_ON_OBJ_EVENT | (item) << 8)

#define BG_EVENT(target) (MAP_EVENT_BG | (target) << 4)
/// A bg event searched while facing the given way, as a counter is searched
/// across.
#define BG_EVENT_FACING(facing)                                                \
  (MAP_EVENT_BG | MAP_EVENT_FLAG_FACING | (facing))
/// A talk event that answers only while the leader faces the given way, so one
/// object event can answer differently from each side.
#define TALK_EVENT_FACING(facing)                                              \
  (MAP_EVENT_TALK | MAP_EVENT_FLAG_FACING | (facing))

/// What a bg event is on, BG_EVENT()'s control bits 8..4. Target n shows
/// message MsgTileChecked + n as the leader checks it, unless the trigger has
/// bit 10; target 0 shows nothing.
enum BgEventTarget {
  BG_EVENT_SILENT = 0,
  BG_EVENT_CHEST = 1,
  BG_EVENT_JAR,
  BG_EVENT_BARREL,
  BG_EVENT_WALL,
  BG_EVENT_GROUND,
  BG_EVENT_ROCK,
  BG_EVENT_HOLE,
  BG_EVENT_GRAVE,
  BG_EVENT_TREE,
  BG_EVENT_UNDERBRUSH,
  BG_EVENT_DOOR,
  BG_EVENT_CHIMNEY,
  BG_EVENT_WOODEN_BOX,
  BG_EVENT_BED,
  BG_EVENT_BOOKCASE,
  BG_EVENT_STONE_COFFIN,
  BG_EVENT_FIREPLACE,
  BG_EVENT_WATER,
  BG_EVENT_STONE_PILLAR,
  BG_EVENT_STALACTITE,
  BG_EVENT_BOARDS,
  BG_EVENT_FOUNTAIN,
  BG_EVENT_OVEN,
  BG_EVENT_TABLE,
  BG_EVENT_STONE_STATUE,
  BG_EVENT_STONE_TABLET,
  BG_EVENT_SHELF,
  BG_EVENT_WARDROBE,
  BG_EVENT_FIREWOOD,
  BG_EVENT_BOOKS,
  BG_EVENT_WELL
};

/// What a search finds, a bg event's value: a function when bits 27..24
/// are set, else a type in its top twelve bits.
#define FIND_ITEM(item) (0x00100000 | (item))
#define FIND_COINS(coins) (0x00200000 | (coins))
/// A Psynergy Stone, which restores all PP.
#define FIND_PSYNERGY_STONE 0x00300000
#define FIND_MESSAGE(message) (0x00400000 | (message))
/// A mimic chest's battle, with the event's value its slot of
/// ENCOUNTER_ZONE_MIMIC.
#define FIND_MIMIC 0x00500000

/// A script function as a MapEvent value; its address is above 0x10000, so
/// it is never taken for a message.
#define EVENT_SCRIPT(script) ((u32)(script))
#define MAP_EVENTS_END 0xffffffff ///< ends a map event table

struct SceneExitRegion;

#endif
