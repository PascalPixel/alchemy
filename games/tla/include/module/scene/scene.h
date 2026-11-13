#ifndef ALCHEMY_MODULE_SCENE_SCENE_H
#define ALCHEMY_MODULE_SCENE_SCENE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "types.h"
#include "field/field_coordinate.h"
#include "graphics/render_input.h"
#include "module/scene/mars_lighthouse_aerie.h"
#include "scene_tables.h"
#include "system/vector3.h"

struct Actor;
struct FieldEffect;
struct FieldPosition;
struct FieldSprite;
struct Particle;
struct ParticleSprite;
struct Position;
struct Vector3;

typedef s32 (*HandlerFn)(void);

/// The actor word at 84 seen as bit fields, for one store (see Func_0200095c).
struct MotionView {
  u8 pad[84];
  u32 kind : 8;
  u32 flags : 8;
};

/// A map cell, or a step between cells, kept as one 8-byte pair: written by the
/// word, it keeps the native's store before the travel loop.
struct Cell {
  s32 x, z;
};

struct ObjBits {
  u8 unk00[35];
  u8 flags : 8;
};

struct Shot {
  u8 unk00[0x62];
  u8 unk62; ///< set once it has hit the leader
  u8 unk63; ///< set with unk62, cleared when it ends
};

struct AirsRockEventWork {
  u8 unk000[0xcbc];
  s32 valueCbc;
};

/// A field sprite's byte 9: the top of its first tile and its priority (OAM
/// attribute 2), read as bit-fields.
struct SpriteAttr2High {
  u8 unk00[9];
  u8 tileHigh : 2;
  u8 priority : 2;
};

/// An object's priority flags read as a whole-byte bit-field.
struct ObjectPriorityByte {
  u8 unk00[35];
  u8 priorityFlags : 8;
};

struct GabombaStatueEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

struct Orbiter {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x38];
  s32 radius;
  u8 unk50[0x12];
  u8 countdown;
  u8 unk63;
  s16 angle;
  u16 speed;
  struct Actor *anchor;
};

struct Orbit {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x30];
  s32 centreX;
  s32 centreZ;
  s32 radius;
  u8 unk50[0x12];
  u8 phase;
  u8 growth;
  s16 angle;
  s16 speed;
};

struct DoorStep {
  s32 script;
  u16 x;
  u16 z;
};

struct AlhafraEventWork {
  u8 unk000[0x15a];
  s16 value15a;
  u8 unk15c[0x68];
  u16 value1c4;
};

struct EventWork1c4 {
  u8 unk000[0x1c4];
  u16 count;
};

struct CellMapCell {
  u8 unk00[2];
  u8 event;
  u8 unk03;
};

struct CellMapLayer {
  s32 unk00[2];
  s32 originX;
  s32 originZ;
  u8 unk10[0x20];
  struct CellMapCell *cells;
};

struct CellMapWork {
  u8 unk000[0xec];
  s32 minX;
  s32 minZ;
  s32 maxX;
  s32 maxZ;
  u8 unk0fc[0xc];
  struct CellMapLayer layers[4];
};

struct PushEvent {
  u8 unk00[6];
  s16 x;
  s16 z;
  u8 unk0a[4];
  u16 busy;
};

struct JupiterLighthouseRoom8EventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
  u8 unk1b0[0x1c];
  struct Actor *marker;
};

struct AnemosInnerSanctumEntranceEventWork {
  u8 unk000[0x1ac];
  s32 route;
};

struct LightOam {
  u32 link;
  u32 attr01;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
};

// Draws eight sprites while flag 0x205 is clear and the level is below
// 0x200000: each moves by the level as its kind says, is placed relative to
// the view and is drawn when it is on screen. Then sets a word in map layer
// 1 to the level plus a base.
struct AnkohlRuinsSprite {
  s32 x;
  s32 z;
  s32 y;
  u32 kind;
  u32 oam[3];
};

struct AnkohlRuinsSpriteWork {
  s16 unk00;
  s16 tile;
  s32 level;
  s32 unk08;
  s32 unk0c;
  struct AnkohlRuinsSprite sprites[8];
};

struct AnkohlRuinsWaterRow {
  u8 level;
  u8 width;
  u8 height;
  u8 x;
  u8 z;
  u8 unk05[3];
};

struct ActorTail {
  u8 head[0x5e];
  u16 unk5e;
};

struct Unk02003450 {
  s16 unk00;
  s16 unk02;
  s16 unk04;
  s16 unk06;
  s16 unk08;
  s16 unk0A;
  s16 unk0C;
  s16 unk0E;
  s16 unk10;
  s16 unk12;
  s16 unk14;
  s16 unk16[2];
  s16 unk1A[2];
  s16 unk1E;
};

struct AquaRockEventWork {
  u8 unk000[0x16a];
  s16 field16a;
};

struct MapBits {
  u8 unk00[23];
  u8 low : 2;
  u8 mode : 2;
};

struct SlideWork {
  u8 unk000[0x1cc];
  struct Actor *camera;
};

struct CurrentUnit {
  u8 unk00[0x14];
  struct Actor *object;
  u8 unk18[4];
  s16 unk1c;
};

struct ChampaEventWork {
  u8 unk000[0x1c4];
  s16 message;
};

struct EventTransition {
  u8 unk000[0x1ac];
  s32 type;
};

/// The byte at 0x5a of an actor, as the field its bit 0 is set through.
struct ActorSolid {
  u8 unk00[0x5a];
  u8 solid : 8;
};

struct EventCounters {
  u8 unk000[0x1c4];
  u16 count;
};

struct ThrowAnimations {
  s32 animations[3];
};

struct EventFlags685 {
  u8 unk000[0x15a];
  u16 field15a;
};

struct FlashSprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 x;
  s32 z;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  u16 unk1e;
};

struct FlashWork {
  u8 tiles[0x400];
  struct FlashSprite sprite;
};

// ---- games/tla/src/scene/champa_cliff_caves/s2c88.c ----
struct ChampaCliffCavesSprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x14];
  u16 unk1e;
  u8 unk20[8];
};

struct ChampaCliffCavesWork {
  u8 tiles[0x1000];
  s32 originX;
  s32 originY;
  s32 originZ;
  struct ChampaCliffCavesSprite spriteA;
  struct ChampaCliffCavesSprite spriteB;
  u16 entryA;
  u16 vramA;
  u16 entryB;
  u16 vramB;
  // A union: the two stores below keep their place after the sprite's.
  union {
    s32 value;
    u16 half;
  } field1064, field1068;
  s32 field106c;
  s32 field1070;
  u16 field1074;
  u8 unk1076[2];
};

/// The bytes of an actor this handler uses. unk63 is the last of the four
/// scene-owned bytes the Broken Seal's actors keep at this place.
struct TalkActor {
  u8 unk00[6];
  u16 angle; ///< 1/65536 turns
  u8 unk08[0x5b];
  u8 unk63; ///< Func_020006d0 reads it
};

struct ContigoEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

struct DailaActor {
  u8 unk00[6];
  u16 facing;
};

struct DailaEventWork {
  u8 unk000[0x1c4];
  s16 message;
};

struct DailaStartActor {
  u8 unk00[0x18];
  s32 speed18;
  s32 speed1c;
  u8 unk20[3];
  u8 priorityFlags;
  u8 unk24[0x35];
  u8 collisionFlags;
};

struct DailaMapWork {
  u8 unk000[0x17];
  u8 low17 : 2;
  u8 mode17 : 2;
  u8 high17 : 4;
};

struct CellRecord {
  u16 present;
  u16 x;
  u16 y;
  u16 unk06[3];
};

struct EffectWork7c4 {
  u8 unk000[0x7c4];
  s16 active;
  s16 timer;
};

/// The effect fields of an Actor, as field_effect.h names them.
struct EffectView {
  u8 unk00[0x30];
  s32 scaleRateX;
  s32 scaleRateY;
  u8 unk38[0x0c];
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  struct FieldSprite *sprite;
  u8 unk54[0x10];
  u16 spin;
  u8 unk66[6];
  void (*update)(struct Actor *object);
};

struct MapCells {
  u8 unk000[0x138];
  u8 *cells;
};

struct EventWorkCbc {
  u8 unk000[0xcbc];
  struct Actor *active;
};

// One-field views: the terminator's read and the shape store are memory of
// their own to sched2, which sinks them past the cell stores; plain reads and
// stores keep their order. A struct is padded to 4 bytes here, so the pointers
// stay plain.
struct ListedActor {
  u16 id;
};

struct DoorPlan {
  s32 value[6];
};

struct SpawnObject {
  u8 unk00[0x50];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[8];
  u16 unk5e;
};

/// The heap slots read as words: the map slot comes through u32, not its
/// pointer type, so its load stays behind the stores of the block.
struct HeapSlotWords {
  u32 slots[8];
  u32 mapWork;
};

struct Sprite020003e8 {
  u8 unk00[18];
  s16 rotation;
  u8 unk14[6];
  u8 nextFrame;
};

struct Actor020003e8 {
  u8 unk00[0x50];
  struct Sprite020003e8 *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[8];
  s16 unk5e;
};

struct EpilogueSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 unk05_5 : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 unk09_2 : 2;
  u8 unk09_4 : 4;
  u8 unk0a[2];
  s32 unk0c;
  s32 unk10;
  u8 unk14[10];
  u16 unk1e;
};

struct EpilogueWindow {
  u8 unk00[8];
  u16 unk08;
  u16 unk0a;
  u8 unk0c[2];
  u16 unk0e;
};

union EventLine {
  s32 word;
  u16 line;
};

struct PuzzleCell {
  u8 kind;
  u8 blocked;
  u8 state;
  u8 occupied;
};

struct GroupWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
  u8 unk1b0[0x1c4 - 0x1b0];
  u16 unk1c4;
};

struct GabombaStatueChamberLayer {
  u8 unk00[12];
  s32 y;
  u8 unk10[0x28];
};

struct GabombaStatueChamberMapWork {
  u8 unk000[0x108];
  struct GabombaStatueChamberLayer layers[3];
};

struct GarohPicture {
  const void *data;
  u16 a;
  u16 b;
};

struct GarohEventWork {
  u8 unk000[0xca4];
  s8 fadeLock;
};

struct Vec {
  s32 x;
  s32 y;
  s32 z;
};

struct TileSprite {
  u8 unk00[18];
  u16 tile;
  u8 unk14[6];
  u8 frame;
};

/// The hideout's placed actors: two not yet understood, then the four
/// Kibombo warriors, named for what each says when spoken to.
enum GondowanCliffsHideoutActorId {
  ACTOR_WARRIOR_FROM_KIBOMBO = ACTOR_FIRST_PLACED + 2,
  ACTOR_WARRIOR_BLAMING_THE_COLLISION,
  ACTOR_WARRIOR_CLAIMING_TO_CAMP,
  ACTOR_WARRIOR_AWAITING_FRIENDS
};

enum { ENTRANCE_WAKE = 2, ENTRANCE_WAKE_AGAIN = 4 };

struct ActorSprite {
  u8 unk00[18];
  u16 rotation;
};

struct FadePalette {
  u16 colors[256];
};

struct IsletCaveEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

struct IsletCaveEntry {
  s32 value;
  u16 position[2];
};

struct TrapEntry {
  u16 actor;
  u16 flag;
};

union ActorCoordinate {
  s32 value;
  u16 half;
};

struct JupiterLighthouseAerieObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct GustObject {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x0f];
  u8 priorityFlags;
  u8 unk24[0x0c];
  s32 unk30;
  u8 unk34[0x21];
  u8 motionFlags;
  u8 unk56[0x0c];
  u8 unk62;
  u8 unk63;
  u16 unk64;
  u8 unk66[6];
  void (*update)(struct GustObject *object);
};

/// struct Actor as this function reads it, its coordinates as
/// FieldCoordinate.
struct PositionedActor {
  u8 unk00[8];
  union FieldCoordinate x;
  union FieldCoordinate y;
  union FieldCoordinate z;
  s32 terrainHeight;
  s32 scaleX;
  u8 unk1c[0x34];
  struct FieldSprite *animation;
  u8 animationKind;
  u8 motionFlags;
};

struct BeamSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 bits : 6;
  u8 unk0a[2];
  s32 x;
  s32 z;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[0x0c];
};

struct BeamSpark {
  s32 position[3];
  s32 unk0c;
  s32 drift;
  s32 unk14;
  u32 age;
};

struct SceneEffectSource {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct EventWorkLock {
  u8 unk000[0x15a];
  u16 locked;
};

struct SparkActor {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct EffectParams {
  s32 count;
  s32 kind;
  s32 spread;
  s32 rise;
  u8 unk10[8];
  u16 tile;
  u16 unk1a;
  u8 unk1c[12];
};

struct JupiterLighthouseRoom5EventWork {
  u8 unk000[0x1c4];
  u16 message;
};

struct JupiterLighthouseRoom5EventWork1ac {
  u8 unk000[0x1ac];
  s32 value;
};

struct Probe {
  s32 unk00[6];
};

struct EventWorkRoute {
  u8 unk000[0x1ac];
  s32 route;
  u8 unk1b0[4];
  s32 countdown;
};

struct AnimationRecord26 {
  u8 unk00[0x1a];
  u8 frame;
};

struct SpawnedActor {
  u8 unk00[0x5e];
  u16 timer;
};

struct Unk_020003a0Sprite {
  u8 unk00[5];
  u8 unk05_0 : 2;
  u8 shape : 2;
  u8 unk05_4 : 1;
  u8 blend : 1;
  u8 unk05_6 : 2;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
  // XXX (no evidence): what these hold; read as s32, whose alias set orders
  // the stores as the listing does.
  s32 unk0c[4];
  u16 unk1c;
  u16 unk1e;
};

struct Half {
  u16 v;
};

struct SpringTossMotion {
  u8 unused[2];
  s16 launchFrame;
  struct Vector3 trail[4];
  struct Vector3 shadow;
  struct Vector3 velocity;
  s32 attractionFrames;
};

struct SpringObstacle {
  struct Vector3 position;
  s16 heading;
  s16 pauseOrPhase;
  s16 speedMode;
  s16 reactionFrames;
  s16 hitCooldownFrames;
};

struct SpringPoint {
  s32 x;
  s32 y;
  s32 z;
};

struct SpringRide {
  u16 unk00;
  u16 hold;
  struct SpringPoint trail[5];
  struct SpringPoint velocity;
  s32 frames;
};

struct LemurianShipWarp {
  u16 exit;
  s32 value;
  u16 x;
  u16 z;
};

struct Unk16 {
  u8 unk00[4];
  union {
    u32 words[2];
    u16 halves[4];
    u8 bytes[8];
    struct {
      u16 y : 8;
      u16 attr0 : 8;
      u16 x : 9;
      u16 attr1 : 7;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
    } oam;
  } sprite;
  s32 unk_0c;
};

struct LohoPicture {
  const void *data;
  u16 a;
  u16 b;
};

struct MadraEventWork {
  u8 unk000[0x154];
  s16 value154;
};

struct MadraPlacement {
  const void *data;
  u16 x;
  u16 y;
};

/// A map edit and where it applies (Map_PlayMapEdit forwards
/// to Map_PlayMapEdit).
struct CellPictureCopyEntry {
  const s16 *steps;
  u16 unk04;
  u16 unk06;
};

struct EventWork154 {
  u8 unk000[0x154];
  s16 returnEntrance;
};

struct AnimationRecord18 {
  u8 unk00[0x12];
  u16 frame;
};

struct EventObjects {
  u8 unk00[20];
  struct Actor *objects[64];
};

struct Object0200074c {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x4f];
  u8 marked;
};

struct StagedActor {
  u8 unk00[6];
  u16 facing;
  union FieldCoordinate x;
  union FieldCoordinate y;
  union FieldCoordinate z;
  u8 unk14[0x0e];
  u8 transitionMode;
  u8 unk23;
  s32 velocityX;
  u8 unk28[4];
  s32 velocityZ;
  s32 moveRateX;
  s32 moveRateZ;
  s32 targetX;
  u8 unk3c[4];
  s32 targetZ;
  u8 unk44[0x15];
  u8 collisionFlags;
  u8 unk5a[8];
  u8 transitionBusy;
};

/// The cell watch's work: the object found on the leader's cell.
struct CellWatch {
  u8 unk00[4];
  s32 x;
  u8 unk08[4];
  s32 z;
  u8 unk10[4];
  struct Actor *object;
};

struct ActorTable {
  u8 unk_0[20];
  struct Actor *actors[64];
};

struct MagmaRockInsideWork {
  s32 unk00;
  s32 x;
  s32 y;
  s32 z;
  s32 unk10;
  struct Actor *target;
};

struct EffectConfig {
  s32 kind;
  s32 variant;
  u8 unk08[16];
  u16 id;
  u8 unk1a[2];
  const u8 *data;
  u8 unk20[8];
};

struct MarsLighthouseAerieEventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

struct MarsLighthouseAerieSprite {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 layer : 2;
  u8 unk09_4 : 4;
  u8 unk0a[0x10];
  u8 unk1a;
};

/// The event work's step counter.
struct AerieEventWork {
  u8 unk000[452];
  union {
    union StoredHalfword step;
    u16 stepRead;
  };
};

struct EffectHolder {
  u8 unk00[0x1a];
  s16 objectId;
};

struct Overlay6aaEventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

// ---- games/tla/src/scene/mars_lighthouse_entrance/s1254.c ----
/// One halfword read as a struct's field.
struct HalfwordField {
  s16 value;
};

struct Sprite020012e4 {
  u8 unk00[18];
  u16 unk12;
};

/// One object the beam reaches, as Func_020022e4 records it.
struct MarsLighthouseBeamTarget {
  u16 active;
  u16 index;
  u16 flags;
  u16 unk06;
  struct Actor *object;
  s32 height; ///< 16.16 pixels
};

struct MarsLighthouseBeam {
  u8 unk00[10];
  u16 unk0a;
  u8 unk0c[4];
  s32 unk10;
  struct MarsLighthouseBeamTarget targets[4];
  struct {
    u16 unk00;
    u16 unk02;
  } stops[4];
};

struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x68 - 6];
  s16 unk68;
  s16 unk6a;
};

// ---- games/tla/src/scene/mars_lighthouse_room_f/s25b0.c ----
struct MarsLighthouseRoomFAnimation {
  u8 unk00[0x10];
  u8 value10;
  u8 flag11 : 1;
};

struct MarsLighthouseRoomFRideWork {
  u8 unk00[0x0c];
  u16 value0c;
  u8 unk0e[0x62];
  struct Actor *handle;
};

struct WarpEntry {
  s16 x;
  s16 z;
  s16 flag;
  s16 unk6;
};

struct Overlay6acEventWork {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0xb3c];
  u8 unkca4;
};

struct UnitData {
  u8 filler[216];
  u16 items[15];
};

/// The sprite words as one stored pair, or the attributes field by field.
union SparkOam {
  struct {
    u32 first;
    u32 second;
  } words;
  struct OamData oam;
};

struct Sprite6a7 {
  u32 unk_00;
  struct OamData oam;
  u8 unk_0c[18];
  u16 unk_1e;
  u8 unk_20[8];
};

struct Particle6a7 {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 unk_14;
  u32 age;
};

struct Spark6a7 {
  u32 unk_00;
  union SparkOam oam;
  s32 timer;
};

struct Work6a7 {
  u8 sheet[0x1000];
  struct Sprite6a7 sprites[54];
  struct Particle6a7 particles[54];
};

struct SeaOfTimeEventWork0154 {
  u8 unk000[0x154];
  s16 unk154;
};

struct Spot {
  s32 x;
  s32 z;
};

struct Spots {
  struct Spot spot[6];
};

struct EventWork687 {
  u8 unk000[0x16a];
  u16 unk16a;
};

/// The 14 colours of BG palette 3 (entries 1 to 14) that the fade returns to,
/// copied from palette RAM by Scene_Initialize and rotated by Func_02001650 in
/// step with BG palette entries 49 to 62.
struct SavedColors {
  u16 colors[14];
};

struct ShamanVillageCaveSceneWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

struct LinkedEffectWork {
  u8 unk000[0x7c0];
  u16 vramBlock;
};

struct LinkedSpriteView {
  u8 unk00[6];
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u8 unk08[9];
  u8 retainCacheEntry : 1;
  u8 unk11 : 7;
};

struct Charge {
  u8 pad[17];
  u8 low : 2;
  u8 level : 6;
};

struct MapFocus {
  s32 scrolledX;
  s32 scrolledZ;
  s32 x;
  s32 z;
  s32 scaleX;
  s32 scaleZ;
  s32 unk18[4];
  u16 unk28;
  u16 unk2a;
  u16 cellX;
  u16 row;
  u8 *cell;
  u8 *shape;
};

struct SwingPoint {
  u16 unk00;
  u16 index;
  s32 x;
  s32 y;
  s32 z;
  s32 w;
};

struct TitleWork {
  u8 unk00[20];
  u16 mode;
};

/// One entry of the cell lists Func_020005ac walks.
struct TreasureIsleCell {
  s32 x; ///< in 16-pixel cells
  s32 z; ///< in 16-pixel cells
  s32 unk08;
  u16 flag; ///< set once the cell has been opened (Func_020003fc)
  u16 unk0e;
};

struct TreasureIsleEventWork {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0xb3c];
  u8 unkca4;
};

struct SceneEffectParams {
  s32 priority;    ///< the sprite's priority
  s32 palette;     ///< the sprite's palette
  s32 startScaleX; ///< 16.16 fixed point, 0x10000 full size
  s32 startScaleY; ///< 16.16 fixed point, 0x10000 full size
  u8 unk10[0x18];
};

enum { ENTRANCE_ALTERNATE_PLACEMENTS = 99 };

struct SparkRecord {
  u8 unk00[100];
  u16 delay;
  u8 unk66[6];
  s32 update;
};

struct GlowState {
  s32 x; ///< 16.16 pixels
  s32 y; ///< 16.16 pixels
  s32 z; ///< 16.16 pixels
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 frame;
};

struct GlowSprite {
  u8 unk00[4];
  u16 attr0 : 13;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct GlowAnimation {
  u32 unk00[4];
  u32 unk10 : 16;
  u32 angle : 16;
};

struct Unk_0200258cState {
  u8 unk00[24];
  s32 frame;
};

struct Unk_0200258cSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 attributes : 6;
  u8 unk0a[30];
};

struct Unk_0200258cWork {
  u8 unk0000[0x800];
  struct Unk_0200258cState states[96];
  struct Unk_0200258cSprite sprites[96];
  u16 unk2180;
  u16 unk2182;
};

struct SparkState {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 frame;
};

struct BobbingMarker {
  u8 unk00[6];
  u16 facing;
  union FieldCoordinate x;
  union FieldCoordinate y;
  union FieldCoordinate z;
  u8 unk14[80];
  s16 angle;
  s16 stopped;
};

struct EventDestination {
  u8 unk000[0x154];
  s16 scene;
};

/// gMainRoutineWork.savedScene and savedEntrance as this script reads them.
struct SavedDestination {
  u8 unk000[0x1e4];
  s16 scene;
  s16 entrance;
};

struct RescueMap {
  u8 unk000[0x11c];
  u16 unk11c;
  u16 unk11e;
};

struct LinkedDetail {
  u8 unk00[22];
  u8 field16;
};

// The layout Func_02001904 reads: state and timer sit where
// struct Actor has its callback and action.
struct YallamActor {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[78];
  u8 state;
  u8 kind;
  s16 timer;
  s16 count;
  u8 unk68[4];
  void (*update)(struct YallamActor *actor);
};

struct YampiDesertVisual {
  u8 unk00[5];
  u8 flags05;
  u8 unk06[3];
  u8 flags09;
  u8 unk0a[6];
  u8 tileSlot;
  u8 unk11[9];
  u8 value1a;
  u8 value1b;
};

struct DustSprite {
  u8 unk00[9];
  u8 unk09 : 2;
  u8 mode : 2;
  u8 unk09High : 4;
};

struct YampiDesertRideWork {
  s16 entry;
  u16 value;
  u8 unk004[0x1c0];
  void *block;
};

struct HeapView {
  s32 pad[8];
  s32 word;
};

struct YampiDesertCaveEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

extern char SceneId_Ov663Room1;
extern char SceneId_Ov663Room2;

s32 Math_Cosine(s32 angle);
void Map_CenterOnFollowedPoint(void);

void Func_02003b48(struct Actor *object);
void Func_02003c38(s32 actor, s32 a);
void Func_02003c30(void);

extern char ResourceId_WhirlwindMap;
extern char ResourceId_WhirlwindTiles;

void Func_02003cd0(struct Actor *object, s32 cue);
void Func_02003cb8(const void *data);

extern char ResourceId_LightningMap;
extern char ResourceId_LightningTiles;
extern char SceneId_Ov663Room3;

void Func_02003cb0(const u8 *table);
void Func_02003ca0(const u8 *table);
void Func_02003c80(s32 a);

void Func_02005df4(s32 x, s32 z, s32 terrain, s32 height);
void Func_02005efc(s32 a);
void Func_02005ee4(void);
struct Actor *Func_02005ef4(void);
void Func_02005d8c(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02005d94(struct Actor *object);
void Func_02005f8c(struct Actor *object, s32 cue);
void Func_02005d7c(struct Actor *object);
s32 Func_02005dec(struct Actor *object);
void Func_02005eec(s32 actor, s32 a);

s32 Func_02001cec();

extern u8 gData_02001c90[];
extern u8 gData_02001c60[];

void Func_02001578(const void *data);
void Func_02001570(s32 a, s32 b, s32 c, s32 d);

extern s32 gData_02001c08;

void Func_020019e4();

extern s16 gData_02001cde;
extern struct Actor *gData_02001ce0[];
extern u16 gData_02001de2;
extern u16 gData_02001de4;
extern s16 gData_02001cdc;
extern s16 gData_02001de0;

s32 Func_02000d5c(struct Actor *actor);

extern u8 gField_CachedTilesA[];

extern u8 gField_CachedTilesD[];

s32 Func_02002e0c(struct Actor *object, s32 *position);

extern struct AnkohlRuinsSpriteWork Data_02004030;

void Func_02002dac(u32 *oam, s32 b);

extern u16 gData_02002a58[16];

extern u8 Data_0202c001[];

extern struct Unk02003450 Data_02003450[];

void Func_02002940(s32 layer);
void Func_02002a10(void);

extern const u8 gField_Tiles679[];

void Func_020028b0(const void *source, void *destination);

s32 Func_02003d2c(struct Actor *object, struct Vector3 *target);
void Func_02003cc4(struct Actor *object, s32 mode);
void Func_02003ea4();
void Func_02003ee4();
void Func_02003c94(u32 *sprite, s32 depth);

void Func_020010e0(void);

s32 Func_02002b3c();
s32 Func_020029fc();
s32 Func_02002bac();

s32 BattleEffect_GetResourceId(s32 object);

s32 Window_CloseMatchingId();

void Func_020035ac(s32 a);
void Func_0200360c(s32 a, s32 b);

extern const u16 Field_Palette685[];

extern char ResourceId_FieldEffectExplosion;

void Func_020035fc(struct FlashSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tile);

extern char ResourceId_Data1F7;
extern char ResourceId_GlowDisc;
extern char ResourceId_FireColumn;

void Func_02000180(s32 a, s32 b);

extern char ResourceId_LogoOnBlack;

void Func_02000d58(s16 *data);

void Func_02000ddc(s32 slot, s32 actor);
s32 Func_02000dbc(s32 mode);
void Func_02000d9c(void);

void Func_02003dc0(const void *data);
void Func_02003dc8(void);
void Func_02003de8(void);
void Func_02003cf0(void);

void EventObject_Initialize(void);

void Func_02003cc0(s32 id);
void Func_02003ba0(struct Actor *object, s32 mode);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
s32 Func_02003d20(void);
s32 Func_02003db0(struct Actor *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);
struct Actor *Func_02003dd8(s32 *position, s32 mode);

extern s32 gData_0200567c;

s32 Func_02003de0(void);
s32 Func_02003d10(s32 owner);

s32 Func_02004c24();
s32 Func_02004c8c();
s32 Func_02004a24();
s32 Func_02004c44();
s32 Func_02004c4c();

extern u16 gData_020056e0[2][160];

void Func_02004c5c();
void Func_02004c14(void);
void Func_02004c1c(void);
void Func_02004c84();
void Func_02004b94();
void Func_02004c6c(struct ParticleSprite *sprite, struct Particle *particle);
void Func_02004a0c(struct Actor *object, s32 mode);

void Func_02003148();
void Func_02003268();
void Func_02003218();
void Func_020032a0();

extern s8 gData_02000a34;
extern s16 gData_02000a38;
extern s16 gData_02000a2c;
extern struct EpilogueSprite gData_02000a00;

extern const u16 Data_02000874[];

extern s32 Data_020008f0;
extern s32 Data_02000a30;

extern char ResourceId_GreenPanel;

extern s32 gData_02000a28;

s32 Func_0200084c();

void Func_0200126c(s32 id);

void Func_020055cc(const u8 *data, void *buffer);
void Func_02005824(void);
void Func_0200582c(s32 index, s32 a, s32 b);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);

void Func_020057b4(void);

void Func_020056b4(void);
void Func_02005624(struct Actor *object, s32 mode);

void Func_02001d78(void);

extern char SceneId_Ov681Room5;

void Func_02004e64(void);

extern u16 Data_02006340[];

extern char SceneId_Ov65FRoom1;
extern char SceneId_Ov65FRoom2;
extern char SceneId_Ov65FRoom3;
extern char SceneId_Ov65FRoom4;
extern char SceneId_Ov65FRoom5;

void Func_020052b4(void);
void Func_020052f4(s32 value);
void Func_0200535c();

extern const u8 gField_Tiles64D[];

void Import_ActorPlaceAtActor();
void Func_02003320();

extern s16 gData_02003c14;
extern s16 gData_02003990[2][160];

extern u16 *gData_02001a24;
extern u16 *Data_02001a2c;
extern u16 gData_02001a14;
extern u16 gData_02001a20;
extern u16 gData_02001a30;
extern u16 gData_02001a34;

void Func_02001334(void);
void Func_0200131c(s32 mode);
void Func_0200133c(void);

void Func_0200230c(s32 actor);

void Func_02002a78(s32 x, s32 z, s32 a, s32 b);
void Func_02002af0(void);

extern u16 gData_02003360[];
extern u16 gData_02003364[];

void Func_02002b38(struct BeamSprite *sprite);
void Func_02002b40(struct BeamSprite *sprite, struct BeamSpark *spark);

extern u8 gField_CachedTilesB[];

extern u8 gField_CachedTilesC[];

void Func_020029f0(struct FieldEffect *effect, s32 mode);

void Func_02001d1c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

extern u16 gData_020023b0[];
extern u16 gData_020023d0[];
extern u16 gData_020023d4[];
extern s32 Data_020023ac;

s32 Func_02001d2c(s32 x, s32 z, s32 layer);

extern u16 gData_020021c0[];
extern u16 gData_020021e0[];

void Func_02001b34();

extern u16 gData_02002258[];
extern u16 gData_02002278[];
extern u16 gData_0200227c[];

s32 Func_02001bb4(struct Actor *actor);

s32 Func_0200231c();

extern s32 gData_02002b88[3];
extern const u8 gData_02002b60[];

void Func_02002324(const void *script, s32 a, s32 b, s32 c);

extern u16 gData_02002b98[];
extern u16 gData_02002bbc[];

void Func_02001f00(s32 id);

extern u16 gData_0200273c[];
extern u16 gData_02002740[];

void Func_02001e28();

void Func_02001ef8();

extern s32 gData_02002718;

void Func_020029d8(void);
void Func_020028a0(s32 value);

extern u16 gData_02003404[];
extern u16 gData_02003424[];
extern u16 gData_02003428[];

void Func_02002a30();
void Func_02002810(struct FieldEffect *effect, s32 mode);

extern s16 gData_0200242a;
extern u16 gData_0200252e;
extern u16 gData_020023e8[];

void Func_02001be4();

extern u16 gData_02002408[];
extern u16 gData_0200240c[];
extern s32 Data_020023e4;

extern s16 gData_020035fe;

void Func_02002b54(s32 x, s32 z, s32 a, s32 b);
void Func_02002c54(s32 x, s32 z, s32 a, s32 b);
void Func_02002bec(void);

void Func_020021e0(s32 x, s32 z, s32 a, s32 b);

extern u16 gData_02002ab0[];
extern u16 gData_02002ad4[];
extern s32 Data_02002aac;

s32 Func_020022b0(struct Actor *actor);

extern s32 gData_02002618;
extern u16 gData_0200261c[];
extern u16 gData_0200263c[];
extern u16 gData_02002640[];

void Func_02001bd8();
void Func_02001c98();

s32 Event_ClearInvalidPackedValues();
void Func_02004994(s32 id, s32 mode);
void Func_02004984(void);
void Func_0200498c(s32 a, s32 b, s32 c, s32 d);

extern u16 gData_02005a60[];
extern u16 gData_02005a64[];

void Func_020049b4();
s32 Func_020049a4(s32 x, s32 z, s32 layer);

extern s32 gData_020062a0;
extern u16 gData_020062a8;
extern s32 gData_020062a4;
extern u16 gData_020062aa;

void Func_02005330(struct Actor *object, s32 mode);
void Func_02005148(struct Actor *object);
void Func_02005398(void);
void Func_020053b8(const void *data);

void Func_0200103c(struct Actor *object, s32 value);
void Func_0200117c(s32 a, s32 b, s32 c, s32 d);
void Func_02001134(void);

// The scene's setup: places its actors, then plays what the entrance it was
// reached by calls for.
extern char SceneId_Ov688;

extern u8 Data_02003098[];

extern u8 Data_02002fd7[];

extern u8 Data_02003090[];
extern u8 Data_02003024[];
extern u8 Data_02003060[];
extern u8 Data_02003030[];

s32 Func_0200210c();

extern const u8 Data_02002620[];
extern const u8 Data_02002af8[];

extern s32 gData_02003094;
extern s32 Data_02003088;
extern s32 Data_02003058;

void Func_02002104(struct Unk_020003a0Sprite *sprite, s32 a, s32 b, u32 attr,
                   s32 tile);

s32 Func_02001358();
s32 Func_02001370();

extern struct SpringTossMotion gData_02001760;
extern struct SpringObstacle gData_020017c0[];
extern s32 gData_02001824;
extern s32 gData_02001828;

void Func_020012e8(struct Actor *object, s32 mode);

extern s32 gData_02001820;

extern s32 gData_020024b4;
extern s32 gData_020024b0;

void Func_02001440(s32 a, s32 b);

extern char SceneId_Ov6A6Room2;

s32 Func_02001268();
void Func_02001438(s32 id);

void Func_02002c28(void);

struct Actor *Func_0200120c(void);

void Func_02003ba8(struct Actor *object, const void *script);

extern char SceneId_Ov65aRoom1;
extern char SceneId_Ov65aRoom2;
extern char SceneId_Ov65aRoom3;
extern char SceneId_Ov65aRoom4;
extern char SceneId_Ov65aRoom5;
extern char SceneId_Ov65aRoom6;

void Func_020011ec(void);
void Func_02001204(void);

void Func_02001e80(s32 value);

s32 Func_02004cf8(struct StagedActor *object, struct FieldPosition *position);
void Func_02004ee0(s16 *cells);
void Func_02004ee8(void);
void Func_02004cb0(struct Actor *object);
void Func_02004e78(s32 owner, u16 value);
void Func_02004f28(struct Actor *object, s32 sound);
void Func_02004e58(s32 item, s32 mode);

// The cell stampers keep their last cell and the block they copy.
extern s32 Data_020067ec;
extern s32 Data_020067f0;
extern s32 Data_020067f4;
extern s32 Data_020067f8;

void Func_02004ec0(void);
void Func_02004c98(struct Actor *object, s32 a);

extern u32 Data_0200eb60;

extern char SceneId_Ov6ACRoom3;

void Func_020055dc(s32 scene, s32 a1);
void Func_02005734();

extern u8 gData_02006c20[];
extern u8 gData_02006b70[];

void Func_020055fc();
void Func_020057c4(void);

void Func_02004c80();

extern char SceneId_Ov6aaRoom1;
extern char SceneId_Ov6aaRoom2;
extern char SceneId_Ov6aaRoom3;
extern char SceneId_Ov6aaRoom4;
extern char SceneId_Ov6aaRoom5;
extern char SceneId_Ov6aaRoom6;
extern char SceneId_Ov6aaRoom7;

void Func_02004dc8(void);
void Func_02004dd0(void);

extern u16 gData_020068a2;
extern u32 Data_020068a4[3];
extern s16 gData_020068a0;

void Func_02004c88(s32 scene, s32 a1);
s32 Func_02004d88();
s32 Func_02004dd8();

extern u8 Data_02006878[];

void Func_02004bc0(struct Actor *object, s32 value);

extern char SceneId_Ov6adRoom1;
extern char SceneId_Ov6adRoom2;
extern char SceneId_Ov6adRoom3;
extern char SceneId_Ov6adRoom4;

void Func_020055b8(s32 id);
void Func_02005638(s32 a0);
void Func_02005670(void);
void Func_02005678(s32 a0, s32 a1, s32 a2);
void Func_020056b0(s32 id);
void Func_020056b8(s32 id);
void Func_02005548(s32 scene, s32 a1);

extern u16 gData_02007088[];
extern u16 gData_0200708c[];

void Func_02005540();
void Func_020056a8();

extern s32 Data_02007064;

s32 Func_020056a0(s32 x, s32 z, s32 layer);
void Func_02005480(struct Actor *object, s32 mode);

extern u16 gData_02004cf0[];

void Func_020054a0(void *handle);

void Func_02003130(void *handle);
void Func_02003118(struct Actor *object, s32 value);
void Func_02003230(s32 a, s32 b);
void Func_02003228(void);
void Func_020032a8(void);

extern u16 Data_020039bc[];
extern volatile u16 Data_020039dc[];
extern u16 Data_02003a1c[];
extern const u16 Data_0200ba70[];
extern s16 Data_02003a7c;
extern s16 gData_02003a7e;
extern s16 gData_02003a80;
extern s16 gData_02003a82;

extern char SceneId_Ov6ACRoom6;
extern char SceneId_Ov6ACRoom1;

void Func_020031b8(s32 scene, s32 a1);
void DisplayScroll_InitializeHBlankDma(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                       s32 g);

s32 Func_0200428c(s32 *a, s32 radiusA, struct Position *b, s32 radiusB);

extern u32 gData_020007e4;

void Func_0200064c();

extern char SceneId_Ov64c;

void Func_02001198(s32 slot, s32 actor);
const u8 *Func_020010b8(s32 item);

extern char SceneId_Ov6a7Room1;
extern char ResourceId_FieldEffectIceShard;

s32 Func_02001f9c(struct Actor *actor);

void Func_02001f4c(struct Actor *object, s32 value);

extern u16 Data_02002580[160];

void Func_02001f64();

void Func_02001180();
void Func_02001208();

extern u16 gData_02003dbc;
extern u16 gData_02003dbe;

extern u8 Data_020020bc[];
extern u8 Data_0200213c[];
extern u8 Data_0200217c[];
extern u8 Data_02002469[];

void Func_0200207c(void);

extern char ResourceId_FarCall00A;

void Func_020020ac(s32 value);

s32 Func_02002004(s32 *position);
s32 SceneEvent_FindActiveRecord(s32 mode, s32 value);

extern s16 gData_02003fc6;
extern u8 gData_020040d8[][32];
extern u32 Data_02003fcc[3];

extern const u8 gWorldMapPackedTilesC[];

extern char SceneId_SuhallaGate2;
extern char SceneId_SuhallaGate3;
extern char SceneId_SuhallaGate1;

s32 Func_02002850(struct Actor *object, struct Vector3 *position);
void Func_02002960(s32 a);
void Func_02002988(s32 a, s32 flag, s32 b, s32 c);

extern char SceneId_Ov650;
extern char ResourceId_FarCall00B;

extern s16 gData_020006dc;

extern u16 Data_02001aa8[];
extern volatile u16 Data_02001ae8[];
extern u16 Data_02001b08[];

s32 Func_02003180(s32 layer, s32 shape, s32 height);
u32 Func_02003150(struct FieldSprite *sprite, u32 frame);

void Func_02001ccc(s32 actor, s32 value);

s32 Func_020028a4();
s32 Func_0200299c();
s32 Func_02002a04();
void Func_02002994(s32 id, s32 useSetter);
void Func_02002824(void *source, void *destination);
void Func_02002a0c(struct GlowSprite *sprite, s32 width, s32 height, s32 attr,
                   s32 tile);
void Func_0200285c(struct Actor *actor, s32 mode);
void Func_02002874(struct Actor *actor);

extern u8 gData_02006c08[];
extern u8 gData_02006f20[];
extern u8 gData_02006e0e[];
extern u8 gData_02006e0c[];

s32 Func_02004c38(s32 a, s32 b, s32 c);

void Func_02004b28(void);
void Func_02004da8(void);
void Func_02004db0(void);
s32 Func_02004d68(s32 owner);
s32 Func_02004b88(struct Actor *object, s32 *position);
void Func_02004d70(s32 value);
void Func_02004c00(void);
void Func_02004e00(void);
void Func_02004e28(void);
void Func_02004bf8(s32 value);
void Func_02004d30(void);

extern s32 Data_02006be8[8];

extern char SceneId_Ov676Room1;
extern char SceneId_Ov676Room2;

void Func_02002598(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);

void Func_02000a40(s32 item, s32 mode);

void Func_02004100(s32 actor, s32 a, s32 b);
void Func_02004108(s32 actor, s32 a);
s32 Func_02004130();
void Func_02004128();
void Func_02004030(s32 item);
void Func_02003fa8(struct Actor *object, s32 mode);
void Func_02003f88(u32 *oam, s32 depth);

s32 Func_02001850(struct Actor *object, s32 *position);
struct Actor *Func_02001948(s32 *position, s32 mode);

extern u16 *gData_02001fdc;
extern u16 *gData_02001fe0;
extern u16 *gData_02001fcc;

extern char SceneId_Ov664Room4;

void Func_020018c8(s32 id);

struct Actor;
struct SparkSprite__jupiter_lighthouse_room_5;
struct Vector3;
struct Window;

struct ClearWork {
  u8 unk00[20];
  u16 mode;
};

struct PasswordWork {
  void *glyphs[50];
  void *cells[50];
  u8 shown[50];
  u8 unk1c2[26];
};

struct FieldPartBytes {
  u8 unk00[5];
  u8 frame;
};

struct FieldAnimation {
  u8 unk00[40];
  struct FieldPartBytes *part;
};

struct GateEventWork {
  u8 unk000[362];
  u16 gateTimer;
};

struct FieldPart {
  s16 kind;
};

extern char SceneId_Ov6b5;

void Func_020022e8(void);
s32 Func_020022f0(void);
void Func_02002350(void);
void Func_02002370(void *data, s32 a, s32 b);
void Func_02002398(void);
void Func_020023a0(void *data);
void Func_020023e0(s32 scene, s32 entrance);

extern u8 gData_02002440[];
extern u8 gData_02002454[];
extern u8 gData_02002468[];

extern u8 gData_0200265e[];

extern u8 Data_0200274a[];
extern u8 Data_0200288a[];
extern u8 Data_0200284e[];

extern u8 Data_02012f94[];

void *Func_02002330(struct Window *window, void *glyph, s32 x, s32 y);

void Func_0200561c(void *animation, s32 a);
s32 Func_02005834(s32 value);

void Func_020057d4(struct Actor *object);

s32 Func_02005684(struct Actor *object, struct Vector3 *position);
void Func_02005784(s32 id);

void Func_020029e8(struct SparkSprite__jupiter_lighthouse_room_5 *sprite);

#endif
