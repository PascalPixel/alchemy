#ifndef ALCHEMY_SCENE_SCENE_H
#define ALCHEMY_SCENE_SCENE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "field/field_coordinate.h"
#include "field/sprite.h"
#include "graphics/render_input.h"
#include "scene/mars_lighthouse_aerie.h"
#include "field/scene_tables.h"
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

/// The actor word at 84 seen as bit fields, for one store (AirsRock_BlowGust).
struct MotionView {
  u8 pad[84];
  u32 kind : 8;
  u32 flags : 8;
};

/// A map cell, or a step between cells, kept as one 8-byte pair.
struct Cell {
  s32 x, z;
};

/// A field sprite's byte 9: the top of its first tile and its priority (OAM
/// attribute 2), read as bit-fields.
struct SpriteAttr2High {
  u8 unk00[9];
  u8 tileHigh : 2;
  u8 priority : 2;
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

struct LightOam {
  u32 link;
  u32 attr01;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 unk0a;
};

/// Draws eight sprites while flag 0x205 is clear and the level is below
/// 0x200000: each moves by the level as its kind says, is placed relative to
/// the view and is drawn when it is on screen. Then sets a word in map layer
/// 1 to the level plus a base.
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

struct MapBits {
  u8 unk00[23];
  u8 low : 2;
  u8 mode : 2;
};

/// The bytes of an actor this handler uses. unk63 is the last of the four
/// scene-owned bytes the Broken Seal's actors keep at this place.
struct TalkActor {
  u8 unk00[6];
  u16 angle; ///< 1/65536 turns
  u8 unk08[0x5b];
  u8 unk63; ///< Func_020006d0 reads it
};

/// One id of a list ended by 0xffff, as Pillars_Raise reads it.
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

struct GabombaStatueChamberLayer {
  u8 unk00[12];
  s32 y;
  u8 unk10[0x28];
};

struct GabombaStatueChamberMapWork {
  u8 unk000[0x108];
  struct GabombaStatueChamberLayer layers[3];
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

/// A coordinate as the scenes' actor views read it: struct FieldCoordinate's
/// layout as a union, whose members may alias anything.
union ViewCoordinate {
  FIXED fixed;
  struct {
    u16 decimal;
    s16 integer;
  } part[1];
};

/// struct Actor as this function reads it, its coordinates as
/// FieldCoordinate.
struct PositionedActor {
  u8 unk00[8];
  union ViewCoordinate x;
  union ViewCoordinate y;
  union ViewCoordinate z;
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

struct Probe {
  s32 unk00[6];
};

struct AnimationRecord26 {
  u8 unk00[0x1a];
  u8 frame;
};

struct SpawnedActor {
  u8 unk00[0x5e];
  u16 timer;
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

struct MadraPlacement {
  const void *data;
  u16 x;
  u16 y;
};

struct StagedActor {
  u8 unk00[6];
  u16 facing;
  union ViewCoordinate x;
  union ViewCoordinate y;
  union ViewCoordinate z;
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

struct MagmaRockInsideWork {
  s32 unk00;
  s32 x;
  s32 y;
  s32 z;
  s32 unk10;
  struct Actor *target;
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

/// One halfword read as a struct's field.
struct HalfwordField {
  s16 value;
};

struct Sprite020012e4 {
  u8 unk00[18];
  u16 unk12;
};

/// One object the beam reaches, as FlameThrowers_Start records it.
struct MarsLighthouseBeamTarget {
  u16 active;
  u16 index;
  u16 flags;
  u16 unk06;
  struct Actor *object;
  FIXED height; ///< 16.16 pixels
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

struct Spot {
  s32 x;
  s32 z;
};

struct Spots {
  struct Spot spot[6];
};

/// The 14 colours of BG palette 3 (entries 1 to 14) that the fade returns to,
/// copied from palette RAM by Scene_Initialize and rotated by Func_02001650 in
/// step with BG palette entries 49 to 62.
struct SavedColors {
  u16 colors[14];
};

struct LinkedEffectWork {
  u8 unk000[0x7c0];
  u16 vramBlock;
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

struct SceneEffectParams {
  s32 priority;      ///< the sprite's priority
  s32 palette;       ///< the sprite's palette
  FIXED startScaleX; ///< 16.16 fixed point, 0x10000 full size
  FIXED startScaleY; ///< 16.16 fixed point, 0x10000 full size
  u8 unk10[0x18];
};

enum { ENTRANCE_ALTERNATE_PLACEMENTS = 99 };

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
  union ViewCoordinate x;
  union ViewCoordinate y;
  union ViewCoordinate z;
  u8 unk14[80];
  s16 angle;
  s16 stopped;
};

/// gMainRoutineWork.restartScene and savedEntrance as this script reads them.
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

void MapGrid_CenterOnFollowedPoint(void);

extern u8 gField_CachedTilesA[];

extern u8 gField_CachedTilesD[];

extern const u8 gField_GroundTiles[];

s32 Speaker_GetVoice(s32 object);

extern const u16 Field_ExplosionPalette[];

void FieldPsynergy_StartCast(void);

void EasternAlhafraShip_CountAnswer();

extern const u8 gField_GeyserTiles[];

extern u8 gField_CachedTilesB[];

extern u8 gField_CachedTilesC[];

// The cell stampers keep their last cell and the block they copy.

void LineScroll_InitWave(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

s32 SceneEvent_FindActiveRecord(s32 mode, s32 value);

extern const u8 gWorldMapPackedTilesC[];

struct Vector3;
struct Window;

struct FieldPartBytes {
  u8 unk00[5];
  u8 frame;
};

struct FieldAnimation {
  u8 unk00[40];
  struct FieldPartBytes *part;
};

struct FieldPart {
  s16 kind;
};

#endif
