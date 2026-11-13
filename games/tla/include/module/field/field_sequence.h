#ifndef ALCHEMY_MODULE_FIELD_FIELD_SEQUENCE_H
#define ALCHEMY_MODULE_FIELD_FIELD_SEQUENCE_H

/// @file
/// What games/tla/src/field/ declares for the files that use it, written once.

#include "types.h"
#include "field/encounter_zone.h"
#include "module/field/field.h"
#include "scene_tables.h"

struct ActorPlacement;
struct MapCell;
struct Trigger;

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

struct SceneCueRule {
  s16 id;
  s16 entrance : 15;
  u16 byScene : 1;
  s16 flag;
  s16 cue;
};

struct SceneAreaNameRule {
  s16 id;
  s16 entrance : 15;
  u16 useSceneId : 1;
  s32 areaNameIndex;
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

struct TriggerEvent {
  s32 control;
  s16 trigger;
  s16 condition;
  u32 value;
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

struct ActionDescriptor {
  u16 sprite;
  s16 condition;
  s32 behavior;
  s32 x;
  s32 y;
  s32 z;
  u16 facing;
  u8 talkFacing;
  u8 flags;
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

/// A scene event as the event finder (Func_080ccd78) returns it.
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

// gMainRoutineWork as this function reads it: struct MainRoutineWork's
// unk24c[6] holds the hazard's limit, mode (0 counts steps on hazard cells, 1
// counts frames) and damage, which adoption names in main_routine_work.h; read
// through one struct, as the native's alias order needs (through struct
// MainRoutineWork and a view of the hazard fields, differ 103).
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

struct FxSprite {
  u8 unk00[91];
  u8 visible;
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

extern const struct SceneMusicEntry Data_080ee6d4[];

extern struct TerrainRule gData_080eedbc[];
extern struct TerrainRule gData_080eef34[];

s32 GetWorldMapTerrainBehaviorFar(void *position, s32 *kind);

extern const struct EncounterZone gEncounter_EnemyGroupTable[];

extern const struct SceneCueRule gScene_CueRuleTable[];

extern const struct SceneAreaNameRule gScene_AreaNameRules[];

extern const s16 gParty_PairResolveRules[];

extern u8 Data_020004b8[];

extern s32 gData_080ef984[];

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

void Func_08020110(void);
void MapGrid_LoadMapFar(s32 map);

void Map_UpdateCurrentTileBlockFar(void);

void Map_ApplySetFlagEditsFar(void);

void AreaNameWindow_ShowFar(s32 scene);

s32 WorldPicture_Show(s32 mode);
void WorldMap_ZoomOutWhileLHeld(void);

void PauseMenu_RunFar(s32 mode);

void Func_080200b0(void);

extern u8 Data_0202e004[];
extern s32 Data_02000448;
extern u16 Data_0200044c;

extern s16 Data_080f2204[];

void Func_08038340(s32 value);

void Class_GetSeriesFar(s32 value);

extern const u8 Data_080eff22[];

void MapAnimation_StartFar();
void Func_08020270(void);

void BattleBackground_SelectForEnvironment(s32 terrain);

s32 GameFlag_IsConditionActive(s32 condition);
void *Scene_FindActorTalkTrigger(s32 id);

s32 Field_FindNearbyTalkTarget(s32 id);

s32 Field_UseItem(s32 arg, s32 slot);

void Actor_ReleaseIfPresent(struct Actor *object);
void ItemDisplay_RaiseAboveLeader(struct Actor *actor);

void FieldPsynergy_Lift(void);

s32 Speaker_GetVoice(s32 id);

#endif
