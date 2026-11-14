#ifndef ALCHEMY_FIELD_SEQUENCE_INTERNAL_H
#define ALCHEMY_FIELD_SEQUENCE_INTERNAL_H

/// @file
/// What every part of field_sequence.c shares: its includes, and the
/// declarations its parts call before their definitions or once declared
/// differently. field_sequence.c includes it before the parts, and each part
/// includes it too, so that a part read on its own resolves. It places no
/// bytes.

#include "types.h"
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

// A row of the field Psynergy target table (gData_080eff28).
struct DescriptorEntry {
  s16 id;
  u8 unk02[2];
  s16 range;
  u8 unk06[2];
};

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
#include "field/facing_object.h"
#include "actor_view.h"

typedef void (*ItemTriggerCallback)(s32 item, s32 actor, s32 slot);

// The field's event work (heap slot 0x6c): the placement tables spawned so
// far, the actors by index and the field mode.
struct FieldStepWork {
  struct ActorPlacement *tables[4];
  const struct Trigger *triggers;
  struct Actor *actors[66];
  u8 unk11c[0x6e];
  s8 mode;
};

struct SpawnSpritePart {
  u8 unk00[2];
  u16 frame;
};

// The Lost Age's sprite object as this function reads it.
struct SpawnSprite {
  u8 unk00[12];
  s32 scale;
  u8 slot;
  u8 displayFlags;
  u8 unk12[7];
  u8 unk19;
  u8 unk1a;
  u8 partCount;
  u8 unk1c[12];
  struct SpawnSpritePart *parts[1];
};

// The object fields this function sets as an actor's home cell.
struct SpawnActor {
  u8 unk00[100];
  s16 homeX;
  s16 homeZ;
};

// The event work as this function reads it: the four event tables it clears
// and the leader's two-row placement table at 0x1ec.
struct EventWorkPlacements {
  const struct ActorPlacement *tables[4];
  u8 unk010[0x1dc];
  struct ActorPlacement leader[2];
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

struct MapViewBounds {
  u8 unk000[0xec];
  s32 minX;
  s32 minY;
  s32 maxX;
  s32 maxY;
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

struct ItemUnit {
  u8 unk000[0xd8];
  u16 inventory[15];
};

struct ItemTrigger {
  u8 unk00[4];
  u16 trigger;
  u8 unk06[2];
  u32 value;
};

struct ItemDefinitionView {
  u8 unk00[12];
  u8 useType;
  u8 unk0d[27];
  u16 actionId;
};

struct ItemFieldWork {
  u8 unk000[0x158];
  s16 exit;
  u8 unk15a[0xcb6 - 0x15a];
  u8 resolvingAction;
};

struct TriggerWork {
  u8 unk000[0x10];
  struct TriggerEvent *events;
};

struct PendingAction {
  u8 unk00[6];
  u8 eventKind;
};

struct ArrivalView {
  u8 unk000[0x2be];
  s16 scene;
  s16 entrance;
  s8 state;
};

struct HazardView {
  u8 unk000[0x24c];
  u16 limit;
  u16 mode;
  u16 damage;
};

union PolarOffsetPosition {
  struct EffectVector vector;
  s32 coordinates[3];
};

// How far, how high and how wide of the caster's facing an animation reaches
// (gData_080eff28, one row per animation, ended by -1).
struct NearbyRule {
  s16 animation;
  s16 range;  // pixels, inclusive
  s16 height; // pixels above or below
  s16 width;  // of the facing, in 1/65536 turns
};

struct SceneItemWork {
  u8 unk000[0x114];
  struct Actor *objects[16];
  u8 unk154[0x18a - 0x154];
  s8 mode;
};

struct ActionDescriptorTables {
  struct ActionDescriptor *tables[4];
};

struct ContainerRow {
  s16 type;
  s16 noHint;
  s16 flag;
  s16 contents;
};

struct TalkEvent {
  u8 unk00[8];
  s32 script;
};

/// A scene event as the event finder (Scene_FindTrigger) returns it.
struct TouchEvent {
  s32 flags;
  u8 target;
  u8 unk05;
  s16 condition;
  /// A message number below 0x10000, else a function; read as a union
  /// member, so a store to the field work does not let it stay in a
  /// register.
  union {
    s32 result;
  } u;
};

struct SceneEventEntry {
  s32 flags;
  u8 value;
  u8 unk05;
  s16 condition;
  s32 result;
};

struct BattleEffectCounterState {
  u8 unk000[0x154];
  s16 counters[12];
};

struct DisplayBlock {
  u8 unk000[0x18a];
  s8 mode;
  s8 pending;
  u8 unk18c[0x40];
  u8 *object;
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

struct MapFocusObject {
  u8 padding00[6];
  u16 kind;
  s32 x;
  s32 unk0c;
  s32 y;
};

struct MapFocusPosition {
  s32 x;
  s32 unk04;
  s32 y;
};

// The event work as this function reads it.
struct StepWork {
  u8 unk000[0x154];
  s16 exit;     ///< the cell's exit code (1 to 230)
  s16 pushCell; ///< the cell's attributes when any of 0xe2 is set
  u8 unk158[0xc];
  s16 encounterEnemyGroup;
  s16 psynergy; ///< a pending Psynergy request
  u8 unk168[2];
  s16 raisedEvent;
  s16 fallenCount;
  s16 survivorCount;
  s16 fallenOwners[8];
  u8 unk180[0x1c];
  s32 walkingDistanceScale;
  s32 walkingDistance;
  struct MapCell *currentCell;
  struct MapCell *previousCell;
};

// A unit as this function reads it.
struct UnitStepView {
  u8 unk000[0x38];
  s16 hp;
  u8 unk03a[0xf7];
  s8 poison;
};

struct PartyStepView {
  u8 unk000[0x212];
  u8 renderMode;
  u8 unk213[5];
  u8 activeOwners[8];
  u8 unk220[0x2c];
  s16 hazardLimit;
  s16 hazardMode;
  s16 hazardDamage;
  u16 hazardSteps;
  u8 unk254[0xa];
  s16 sceneVariant;
  u8 unk260[4];
  s32 avoidDistance;
};

struct FxWork {
  u8 unk000[394];
  u8 state;
  u8 busy;
  u8 unk18c[64];
  struct FxSprite *sprite;
};

struct FadeWork {
  u8 unk000[0x53a];
  u8 level;
  u8 step;
  u8 active;
  s8 pending;
};

extern const struct SceneMusicEntry gScene_EncounterRules[];

extern struct TerrainRule gData_080eedbc[];

extern struct TerrainRule gData_080eef34[];

extern const struct EncounterZone gEncounter_EnemyGroupTable[];

extern const struct SceneCueRule gScene_CueRuleTable[];

extern const struct SceneAreaNameRule gScene_AreaNameRules[];

extern const s16 gParty_PairResolveRules[];

extern u8 Data_020004b8[];

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
extern const s16 Data_080efd1c[];

extern u8 Data_0202e004[];

extern s32 Data_02000448;

extern u16 Data_0200044c;

extern s16 Data_080f2204[];

extern const u8 gField_ExamineTriggerMessageOffsets[];

#endif
