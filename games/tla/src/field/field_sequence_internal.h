#ifndef ALCHEMY_FIELD_SEQUENCE_INTERNAL_H
#define ALCHEMY_FIELD_SEQUENCE_INTERNAL_H

/// @file
/// What every part of field_sequence.c shares: its includes, and the
/// declarations its parts call before their definitions or once declared
/// differently. field_sequence.c includes it before the parts, and each part
/// includes it too, so that a part read on its own resolves. It places no
/// bytes.

#include "types.h"
#include "resource_ids.h"
#include "window/message_argument.h"
#include "window/window.h"
#include "field/world_map.h"
#include "system/ram_buffer.h"
#include "system/main_routine_state.h"
#include "system/iwram_call.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_ids.h"
#include "field/scene_tables.h"
#include "field/actor.h"
#include "field/field_sequence_event.h"
#include "field/map_grid.h"
#include "field/field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/vector3.h"
#include "game/unit_hp.h"
#include "battle/battle_unit.h"
#include "game/class_djinn.h"
#include "fixed_math.h"
#include "battle/range_falloff.h"
#include "field/field_event.h"
#include "system/input.h"
#include "macros.h"
#include "version.h"
#include "system/scene.h"
#include "game/item.h"
#include "battle/battle_system.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "field/objdisp.h"
#include "system/heap_release.h"
#include "graphics/display_transition.h"
#include "system/task.h"
#include "system/io_write_queue.h"
#include "system/irq.h"
#include "system/frame.h"
#include "graphics/palette_buffers.h"
#include "field/sprite.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"
#include "system/io_write.h"
#include "gamepak/far_call/window.h"
#include "field/field_sequence.h"
#include "field/object.h"
#include "gamepak/far_call/field.h"
#include "field/encounter_zone.h"
#include "game/inventory.h"
#include "graphics/uiwindow.h"
#include "field/actor_view.h"

/// No user named: Field_UsePsynergy takes the first party member, and
/// Field_UseItem whoever holds the item in the most inventory slots.
#define UNIT_NONE 15

/// The running scene's entry table, at the start of gOverlayArea.
#define SCENE_ENTRIES ((struct SceneEntries *)gOverlayArea)

// Outside routines and data the parts had declared each in its own way, with
// the one type they share.
struct Actor *Actor_GetById(u32 actorId);

typedef void (*ItemTriggerCallback)(s32 item, s32 actor, s32 slot);

/// An actor's home, as an actor placement sets it (Actor.homeX and homeZ).
struct SpawnActor {
  u8 unused[100]; ///< no code names it
  s16 homeX;
  s16 homeZ;
};

/// An actor placement as Scene_AssignViewFlags reads it: its sprite, its
/// condition and its place.
struct ViewedPlacement {
  s16 sprite; ///< -1 ends the table
  s16 condition;
  u8 unusedAfterFlag[4]; ///< no code names it
  s32 x;
  u8 unusedAfterX[4]; ///< no code names it
  s32 z;
  u8 unusedAfterZ[4]; ///< no code names it
};

/// A row of the scenes' encounter rules (gScene_EncounterRules): a scene and
/// entrance and, while a flag is clear, the encounter zones of its two kinds of
/// ground.
struct SceneEncounterRule {
  s16 scene;
  s16 entrance;  ///< -1 for any
  s16 flag;      ///< -1 for none; the rule applies while the flag is clear
  u8 groundZone; ///< ordinary ground
  u8 secondZone; ///< MAP_CELL_SECOND_ZONE cells
};

/// A point that Vector_AddPolarOffset moves as three coordinates and the item
/// display reads as a Vector3.
union PolarOffsetPosition {
  struct Vector3 vector;
  s32 coordinates[3];
};

/// What a scene's container actor is, ContainerRow.type, named by what the
/// leader checks when talking to it.
enum ContainerKind {
  CONTAINER_CHEST = 128,          ///< "checked the chest..."
  CONTAINER_MIMIC = 129,          ///< a chest that is "really a Mimic!"
  CONTAINER_PSYNERGY_STONE = 130, ///< restores the party's PP
  /// "There's something buried here!": the Scoop spot, actor 79.
  CONTAINER_GROUND = 131,
  CONTAINER_STONE_TABLET = 132, ///< a summon tablet
  CONTAINER_POND = 133          ///< "...gazed into the pond..."
};

/// A row of the scene's container table (SceneItem_GetRows), numbered from
/// actor 64.
struct ContainerRow {
  s16 type; ///< enum ContainerKind
  /// The scene point it stands on (SceneItem_FindPosition); in Venus
  /// Lighthouse the chest on point 0 is "already open" until its flag is set.
  s16 point;
  s16 flag; ///< set once it has been opened; -1 for none
  /// The item inside, 0 for none; a Mimic's formation slot; a Psynergy
  /// Stone's number; a tablet's summon.
  s16 contents;
};
/// ContainerRow.contents and FieldState.scoopedItem: coins rather than an item,
/// their count in the bits under it.
#define CONTAINER_COINS (1 << 15)
/// The most treasure spots a scene places, each an actor from
/// FIELD_TREASURE_ACTOR on.
#define TREASURE_SPOT_MAX 16
/// Actor.sceneWord of a treasure spot that stands for a map cell rather than an
/// actor: its cell is in timer and userValue, and it stands at x and z 0.
#define TREASURE_SPOT_ON_CELL 0x31415927
/// gSceneItems: bit 14 marks a scene's header (SCENE_ITEMS), its scene number
/// in the bits under it.
#define SCENE_ITEMS_HEADER (1 << 14)

/// A box of the map in whole cells (16.16 when compared) and the value it
/// selects while its condition holds; a row with minX -1 ends the list.
struct MapRegion {
  s16 minX;
  s16 minY;
  s16 minZ;
  s16 maxX;
  s16 maxY;
  s16 maxZ;
  s16 condition;
  s16 value;
};

/// Each scene's encounters by scene, entrance and flag, ended by scene -1
/// (encounter_rules.s).
extern const struct SceneEncounterRule gScene_EncounterRules[];

extern const struct EncounterZone gEncounterZones[];

/// Each scene's music by scene or group, entrance and flag
/// (music_rules.s).
extern const struct MapMusicRule gScene_MusicRules[];

/// Each scene's area name by scene or group and entrance
/// (area_name_rules.s).
extern const struct AreaNameRule gArea_NameRules[];

extern const s16 gScene_ReturnRules[];

extern const s32 gScene_BattleBackgroundRules[];

/// The battle music table: each song as ENCOUNTER_BATTLE_MUSIC(song), followed
/// by the encounters that use it as ENCOUNTER_KEY(zone, slot), ended by 0.
extern const s16 gEncounter_BattleMusic[];

/// The scene data's halfword offset of its bounds, from gMapScene.
extern u8 gMapSceneBoundsOffset[];

extern s16 gSceneItems[];

extern const u8 gField_ExamineTriggerMessageOffsets[];

/// The chest sprite's animations, for the targets and treasure spot parts.
enum {
  TREASURE_SPOT_CHEST_OPEN = 2,    ///< standing open
  TREASURE_SPOT_CHEST_OPENING = 4, ///< its lid lifting
  TREASURE_SPOT_CHEST_CLOSING = 5, ///< closing again on what nobody could carry
  TREASURE_SPOT_CHEST_OPENING_EMPTY = 6, ///< its lid lifting on nothing inside
  TREASURE_SPOT_CHEST_MIMIC = 7          ///< jumping up as a mimic
};

#endif
