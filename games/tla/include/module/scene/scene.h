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

/// A block of cells the sand fills in Ankohl Ruins once its level reaches
/// `level`: `width` by `height` cells from (`x`, `z`).
struct AnkohlRuinsSandRow {
  u8 level;
  u8 width;
  u8 height;
  u8 x;
  u8 z;
  u8 unused[3];
};

struct ActorTail {
  u8 head[0x5e];
  u16 unk5e;
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

struct DailaEventWork {
  u8 unk000[0x1c4];
  s16 message;
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
  s32 speedLimit;
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

/// A map edit and where it applies (MapGrid_PlayMapEdit forwards
/// to MapGrid_PlayMapEdit).
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

void Map_CenterOnFollowedPoint(void);

extern char ResourceId_WhirlwindMap;
extern char ResourceId_WhirlwindTiles;

extern char ResourceId_LightningMap;
extern char ResourceId_LightningTiles;

extern u8 gField_CachedTilesA[];

extern u8 gField_CachedTilesD[];

extern const u8 gField_Tiles679[];

s32 Speaker_GetVoice(s32 object);

extern const u16 Field_Palette685[];

extern char ResourceId_FieldEffectExplosion;

extern char ResourceId_GlowDisc;

extern char ResourceId_LogoOnBlack;

void EventObject_Initialize(void);

void Func_020032a0();

extern const u8 gField_Tiles64D[];

extern u8 gField_CachedTilesB[];

extern u8 gField_CachedTilesC[];

// The scene's setup: places its actors, then plays what the entrance it was
// reached by calls for.
extern char SceneId_LemuriaEntrance;

extern char SceneId_Ov6A6Room2;

extern char SceneId_MadraCatacombs1;
extern char SceneId_MadraCatacombs2;
extern char SceneId_MadraCatacombs3;
extern char SceneId_MadraCatacombs4;
extern char SceneId_MadraCatacombs5;
extern char SceneId_MadraCatacombs6;

void Func_02001e80(s32 value);

// The cell stampers keep their last cell and the block they copy.

extern char SceneId_Ov6ACRoom3;

extern char SceneId_MarsLighthouseEntrance1;
extern char SceneId_MarsLighthouseEntrance2;
extern char SceneId_MarsLighthouseEntrance3;
extern char SceneId_MarsLighthouseEntrance4;
extern char SceneId_MarsLighthouseEntrance5;
extern char SceneId_MarsLighthouseEntrance6;
extern char SceneId_MarsLighthouseEntrance7;

extern char SceneId_Ov6adRoom1;
extern char SceneId_Ov6adRoom2;
extern char SceneId_Ov6adRoom3;
extern char SceneId_Ov6adRoom4;

extern char SceneId_Ov6ACRoom6;
extern char SceneId_Ov6ACRoom1;

void LineScroll_InitWave(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

extern char SceneId_Ov64c;

extern char SceneId_Ov6a7Room1;
extern char ResourceId_FieldEffectIceShard;

extern char ResourceId_FarCall00A;

s32 SceneEvent_FindActiveRecord(s32 mode, s32 value);

extern const u8 gWorldMapPackedTilesC[];

extern char SceneId_SuhallaGate2;
extern char SceneId_SuhallaGate3;
extern char SceneId_SuhallaGate1;

extern char SceneId_Idejima;
extern char ResourceId_FarCall00B;

extern char SceneId_Yallam1;
extern char SceneId_Yallam2;

extern char SceneId_AirsRockInside4;

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

#endif
