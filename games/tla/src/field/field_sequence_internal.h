#ifndef ALCHEMY_FIELD_SEQUENCE_INTERNAL_H
#define ALCHEMY_FIELD_SEQUENCE_INTERNAL_H

/// @file
/// What every part of field_sequence.c shares: its includes, and the
/// declarations its parts call before their definitions or once declared
/// differently. field_sequence.c includes it before the parts, and each part
/// includes it too, so that a part read on its own resolves. It places no
/// bytes.

#include "types.h"
#include "window/message_argument.h"
#include "field/world_map.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "system/iwram_call.h"
#include "battle/battle.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_ids.h"
#include "scene_tables.h"
#include "actor.h"
#include "field/field_sequence_event.h"
#include "map.h"
#include "field_sprite.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/vector3.h"
#include "game/owner_value.h"
#include "battle/battle_unit.h"
#include "owner_state.h"
#include "fixed_math.h"
#include "battle/range_falloff.h"
#include "field_event.h"
#include "system/input.h"
#include "macros.h"
#include "version.h"
#include "field/field_sequence_event.h"
#include "system/scene.h"
#include "system/object_effect.h"
#include "game/item.h"
#include "battle/battle_system.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "field/actor_script_view.h"
#include "field/objdisp.h"
#include "system/heap_release.h"
#include "flag_call.h"
#include "graphics/display_transition.h"
#include "system/task.h"
#include "system/io_write_queue.h"
#include "system/irq.h"
#include "system/frame.h"
#include "graphics/palette_buffers.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/sprite.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"
#include "system/io_write.h"
#include "gamepak/far_call/window.h"
#include "module/field/field.h"
#include "module/field/field_sequence.h"
#include "module/field/object.h"
#include "module/system/system.h"
#include "gamepak/far_call/field.h"

extern const struct FieldPsynergyReach gData_080eff28[];

// The object's functions its parts call before their definitions.

// Outside routines and data the parts had declared each in its own way, with
// the one type they share.
struct Actor *Actor_GetById(u32 objectId);
/* VIEW(field_sequence): routine MapGrid_GetTerrainHeightAboveFar as s32
 * MapGrid_GetTerrainHeightAboveFar(s32 x, s32 z, s32 layer, s32 height);
 * module/tla.h has s32 MapGrid_GetTerrainHeightAboveFar(s32 x, s32 z, s32
 * layer); tried as the rest of the object declares it, it does not compile in
 * tla-ja: games/tla/src/field/field_sequence/targets.c:102: too many arguments
 * to function `MapGrid_GetTerrainHeightAboveFar'. */
extern s32 MapGrid_GetTerrainHeightAboveFar__field_sequence(
    s32 x, s32 z, s32 layer,
    s32 height) __asm__("MapGrid_GetTerrainHeightAboveFar");

#include "field/encounter_zone.h"
#include "module/field/field.h"
#include "scene_tables.h"
#include "inventory.h"
#include "graphics/uiwindow.h"
#include "actor_view.h"

/// Trigger.subject of an item trigger: the event shows its own message in
/// place of the "uses" message (MsgActorUsesItem).
#define TRIGGER_SUBJECT_OWN_EFFECT 0x400

typedef void (*ItemTriggerCallback)(s32 item, s32 actor, s32 slot);

// The object fields this function sets as an actor's home cell.
struct SpawnActor {
  u8 unk00[100];
  s16 homeX;
  s16 homeZ;
};

struct SceneRegionEntry {
  s16 id;
  s16 flag;
  u8 unk04[4];
  s32 x;
  u8 unk0c[4];
  s32 z;
  u8 unk14[4];
};

struct SceneMusicEntry {
  s16 scene;
  s16 entrance; // -1 any
  s16 flag;     // -1 none; the entry applies while the flag is clear
  u8 first;
  u8 second;
};

/// A world map encounter rule, ended by kind -1.
struct TerrainRule {
  s16 kind;     ///< the terrain kind of the map block, 0 to 19
  s16 behavior; ///< the terrain's behavior, or -1 for any
  s16 flag;     ///< -1, or a flag the rule needs set
  s16 zone;
};

struct ArrivalView {
  u8 unk000[0x2be];
  s16 scene;
  s16 entrance;
  s8 state;
};

union PolarOffsetPosition {
  struct EffectVector vector;
  s32 coordinates[3];
};

/// What a scene's container actor is, ContainerRow.type, named by what the
/// leader checks when talking to it.
enum ContainerKind {
  CONTAINER_CHEST = 128,          ///< "checked the chest..."
  CONTAINER_MIMIC = 129,          ///< a chest that is "really a Mimic!"
  CONTAINER_PSYNERGY_STONE = 130, ///< restores the party's PP
  /// "There's something buried here!": the Scoop spot, actor 79.
  CONTAINER_BURIED = 131,
  CONTAINER_STONE_TABLET = 132, ///< a summon tablet
  CONTAINER_POND = 133          ///< "There's something in the shallows!"
};

/// A row of the scene's container table (SceneItem_GetRows), numbered from
/// actor 64.
struct ContainerRow {
  s16 type;   ///< enum ContainerKind
  s16 noHint; ///< 0 lets Venus Lighthouse hint at a chest already opened
  s16 flag;   ///< set once it has been opened; -1 for none
  /// The item inside, 0 for none; a Mimic's formation slot; a Psynergy
  /// Stone's number; a tablet's summon.
  s16 contents;
};

// A box of the map in whole cells (16.16 when compared) and the value it
// selects while its condition holds; a row with minX -1 ends the list.
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

extern const struct SceneMusicEntry gScene_EncounterRules[];

extern struct TerrainRule gData_080eedbc[];

extern struct TerrainRule gData_080eef34[];

extern const struct EncounterZone gEncounter_EnemyGroupTable[];

extern const struct SceneCueRule gScene_CueRuleTable[];

extern const struct SceneAreaNameRule gScene_AreaNameRules[];

extern const s16 gParty_PairResolveRules[];

extern s32 gScene_BattleBackgroundRules[];

extern char ResourceId_DragonHallABackdrop;

extern char ResourceId_DragonHallBBackdrop;

extern char ResourceId_GrasslandBBackdrop;

extern char ResourceId_RockyBeachBackdrop;

extern char ResourceId_DenseForestBackdrop;

extern char ResourceId_ShipDeckBackdrop;

extern char ResourceId_SnowfieldBBackdrop;

extern char ResourceId_BlizzardBackdrop;

/// The battle music table: each song as 0x8000 | song, followed by the
/// encounters that use it as (zone << 4) + slot, ended by 0.
extern const s16 gEncounter_BattleMusic[];

extern u8 Data_0202e004[];

extern s16 Data_080f2204[];

extern const u8 gField_ExamineTriggerMessageOffsets[];

#endif
