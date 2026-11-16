#ifndef ALCHEMY_SCENE_SCENE_H
#define ALCHEMY_SCENE_SCENE_H

/// @file
/// What games/tla/src/scene/ declares for the files that use it, written once.

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "field/field_coordinate.h"
#include "field/sprite.h"
#include "field/field_psynergy_work.h"
#include "field/map_grid.h"
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
  u8 link[4];
  u8 y;         ///< attribute 0's y
  u8 attr0High; ///< attribute 0's high byte
  u8 attr1[2];
  u8 tileLow; ///< attribute 2's low byte
  u8 tileHigh : 2;
  u8 priority : 2;
};

/// An actor spiralling round its anchor (Orbiter_Spiral): the circle it keeps
/// in the actor's scene-owned words, and the frames it has left.
struct Orbiter {
  const s32 *script;
  s16 scriptCursor;
  u16 facing;
  FIXED x; ///< 16.16 fixed-point pixels
  FIXED y; ///< 16.16 fixed-point pixels
  FIXED z; ///< 16.16 fixed-point pixels
  FIXED groundHeight;
  FIXED scaleX;
  FIXED scaleY;
  u16 radius;
  u8 layer;
  u8 priorityFlags;
  FIXED velocityX;
  FIXED velocityY;
  FIXED velocityZ;
  FIXED speedLimit;
  FIXED acceleration;
  FIXED targetX;
  FIXED targetY;
  FIXED targetZ;
  FIXED bounce;
  FIXED gravity;
  FIXED orbitRadius; ///< 16.16 fixed-point pixels, in riseBaseY
  void *sprite;
  u8 spriteStorageKind;
  u8 motionFlags;
  u8 arrivalAxis;
  u8 conditionResult;
  u8 snapToTarget;
  u8 collisionFlags;
  u8 steeringFlags;
  u8 paused;
  u8 retainSpriteCache;
  u8 repeatCount;
  s16 wait;
  u8 refused;
  u8 frozen;
  u8 countdown; ///< frames left of the spiral
  u8 riseEnabled;
  s16 angle;            ///< its place on the circle, 1/65536 turns
  u16 speed;            ///< twice its turn a frame, 1/65536 turns
  struct Actor *anchor; ///< the actor it circles, in watchedActor
};

/// An actor circling a centre on a growing circle (Orbit_Update), its
/// centre, radius and turn kept in the actor's scene-owned words.
struct Orbit {
  const s32 *script;
  s16 scriptCursor;
  u16 facing;
  FIXED x; ///< 16.16 fixed-point pixels
  FIXED y; ///< 16.16 fixed-point pixels
  FIXED z; ///< 16.16 fixed-point pixels
  FIXED groundHeight;
  FIXED scaleX;
  FIXED scaleY;
  u16 radius;
  u8 layer;
  u8 priorityFlags;
  FIXED velocityX;
  FIXED velocityY;
  FIXED velocityZ;
  FIXED speedLimit;
  FIXED acceleration;
  FIXED targetX;
  FIXED targetY;
  FIXED targetZ;
  FIXED centreX;     ///< 16.16 fixed-point pixels, in bounce
  FIXED centreZ;     ///< 16.16 fixed-point pixels, in gravity
  FIXED orbitRadius; ///< 16.16 fixed-point pixels, in riseBaseY
  void *sprite;
  u8 spriteStorageKind;
  u8 motionFlags;
  u8 arrivalAxis;
  u8 conditionResult;
  u8 snapToTarget;
  u8 collisionFlags;
  u8 steeringFlags;
  u8 paused;
  u8 retainSpriteCache;
  u8 repeatCount;
  s16 wait;
  u8 refused;
  u8 frozen;
  u8 phase;  ///< where the circle's growth starts, 1/64 turns
  u8 growth; ///< grows by 10 every other frame up to 141
  s16 angle; ///< its place on the circle, 1/65536 turns
  s16 speed; ///< its turn a frame, slowed by 128 a frame once grown
};

/// A glow sprite queued for the OAM: the link at its depth, attributes 0 and 1
/// as one word, and attribute 2's fields.
struct LightOam {
  u32 link;
  u32 attr01;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u16 attr3; ///< no code names it
};

/// One of the eight sprites the sand draws: where it starts, how the level
/// moves it, and its OAM entry as Oam_AddSpriteAtDepthClamped takes it.
struct AnkohlRuinsSprite {
  s32 x;
  s32 z;
  s32 y;
  u32 kind; ///< which way the level moves it
  u32 oam[3];
};

/// Ankohl Ruins' sand (gAnkohlRuins_Sand): its sprites' VRAM cache entry, its
/// level, map layer 1's offset before it rose, and its eight sprites.
struct AnkohlRuinsSpriteWork {
  /// No code names it as the sand's; the entrance tests read through the type
  /// so that their load stays behind a store to the sand.
  s16 unused;
  s16 tile;  ///< the VRAM cache entry of the sprites' tiles
  s32 level; ///< 16.16 pixels the sand has risen
  /// Map layer 1's offset when the sand started, which the level is added to.
  ///@{
  FIXED baseOffsetX;
  FIXED baseOffsetY;
  ///@}
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

/// The map work (struct MapWork) with its ground marks as bit-fields, for
/// rooms that set the kind of wading.
struct MapBits {
  FIXED *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;
  u16 dispcnt;
  u8 wideTiles;
  u8 footprints : 2;
  u8 wading : 2; ///< the kind of wading; rooms with a second level set 1
};

/// One id of a list ended by 0xffff, as Pillars_Raise reads it.
struct ListedActor {
  u16 id;
};

/// The heap slots read as words: the map slot comes through u32, not its
/// pointer type, so its load stays behind the stores of the block.
struct HeapSlotWords {
  u32 slots[8];
  u32 mapWork;
};

/// An event's message counter, read as a word or as its halfword (Gabomba
/// Catacombs' centred lines).
union EventLine {
  s32 word;
  u16 line;
};

/// A cell of Gabomba Statue's channel puzzle: its kind, whether it is
/// blocked, its shape and what stands on it.
struct PuzzleCell {
  u8 kind;
  u8 blocked;
  u8 state;
  u8 occupied;
};

/// The palette fade's work (heap slot paletteFade) as 256 colours.
struct FadePalette {
  u16 colors[256];
};

/// A row of a scene's trap table, ended by an actor of 0xffff: an actor and
/// its flag.
struct TrapEntry {
  u16 actor;
  u16 flag;
};

/// An actor's coordinate read as its word or its low halfword, through a
/// union so that a store to it may alias a read after it.
union ActorCoordinate {
  s32 value;
  u16 half;
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

/// A spark of the aerie's beam: where it is, how fast it drifts up and its
/// age in frames.
struct BeamSpark {
  s32 position[3];
  s32 unusedBeforeDrift; ///< no code names it
  s32 drift;             ///< taken off its height each frame
  s32 unusedAfterDrift;  ///< no code names it
  u32 age;
};

/// Lemuria spring's toss (gLemuriaSpring_Toss): the frame the coin is let
/// go, its trail of positions and its shadow, its velocity, and the frames the
/// spring still draws it in.
struct SpringTossMotion {
  u8 unused[2];
  s16 launchFrame;
  struct Vector3 trail[4];
  struct Vector3 shadow;
  struct Vector3 velocity;
  s32 attractionFrames;
};

/// One of the spring's moving obstacles: its position and heading, and the
/// timers its motion and its hits keep.
struct SpringObstacle {
  struct Vector3 position;
  s16 heading;
  s16 pauseOrPhase;
  s16 speedMode;
  s16 reactionFrames;
  s16 hitCooldownFrames;
};

/// A door of the Lemurian ship: the step trigger it answers, its opening's
/// cell animation and the cell it opens at.
struct LemurianShipWarp {
  u16 exit;
  s32 value;
  u16 x;
  u16 z;
};

/// A field sprite (struct FieldSprite) read a byte at a time, as the Mars
/// Lighthouse aerie's flames take the beacon fire's priority.
struct MarsLighthouseAerieSprite {
  u8 link[4];
  u8 y;         ///< attribute 0's y
  u8 attr0High; ///< attribute 0's high byte
  u8 attr1[2];
  u8 tileLow; ///< attribute 2's low byte
  /// Attribute 2's high byte.
  ///@{
  u8 tileHigh : 2; ///< tile number bits 9..8
  u8 priority : 2;
  u8 palette : 4;
  ///@}
  u8 attr3[2];
  u8 scale[4];
  u8 vramBlock; ///< VRAM cache entry holding its tiles
  u8 cacheFlags;
  u8 rotation[2];
  u8 size[4]; ///< width, height and the x and y offsets
  u8 frame;
  u8 dirty;
  u8 nextFrame;
};

/// One halfword read as a struct's field.
struct HalfwordField {
  s16 value;
};

/// The sprite words as one stored pair, or the attributes field by field.
union SparkOam {
  struct {
    u32 first;
    u32 second;
  } words;
  struct OamData oam;
};

/// A place on the Sea of Time's lap, in 16.16 pixels.
struct Spot {
  s32 x;
  s32 z;
};

/// The six places of the Sea of Time's lap where spray rises once the lap has
/// been run more than three times.
struct Spots {
  struct Spot spot[6];
};

/// The 14 colours of BG palette 3 (entries 1 to 14) that the fade returns to,
/// copied from palette RAM by Scene_Initialize and rotated by
/// SeaOfTime_AnimateSea in step with BG palette entries 49 to 62.
struct SavedColors {
  u16 colors[14];
};

/// The field effect work as the Golden Rings read it: the VRAM cache entry
/// their sprites share, in bytes struct FieldEffectWork leaves unnamed.
struct LinkedEffectWork {
  u8 beforeVramBlock[0x7c0];
  u16 vramBlock;
};

/// A spiky rock's cell on Treasure Isle (TreasureIsle_RaiseSpikyRock).
struct TreasureIsleCell {
  s32 x;            ///< in 16-pixel cells
  s32 z;            ///< in 16-pixel cells
  s32 unusedAfterZ; ///< no code names it; 12 onwards, one a rock
  u16 flag;         ///< set once the rock has risen (TreasureIsle_SetSpikyRock)
  u16 unusedAfterFlag; ///< no code names it
};

/// A spark of Venus Lighthouse's entrance: its position and velocity in
/// 16.16 pixels, and its frame.
struct SparkState {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 frame;
};

/// The World Map's Echo (WorldMap_CircleEcho), which circles and bobs where it
/// waits: struct Actor with its timer and userValue words named.
struct BobbingMarker {
  const s32 *script;
  s16 scriptCursor;
  u16 facing;
  union ViewCoordinate x;
  union ViewCoordinate y;
  union ViewCoordinate z;
  FIXED groundHeight;
  FIXED scaleX;
  FIXED scaleY;
  u16 radius;
  u8 layer;
  u8 priorityFlags;
  FIXED velocityX;
  FIXED velocityY;
  FIXED velocityZ;
  FIXED speedLimit;
  FIXED acceleration;
  FIXED targetX;
  FIXED targetY;
  FIXED targetZ;
  FIXED bounce;
  FIXED gravity;
  s32 riseBaseY;
  void *sprite;
  u8 spriteStorageKind;
  u8 motionFlags;
  u8 arrivalAxis;
  u8 conditionResult;
  u8 snapToTarget;
  u8 collisionFlags;
  u8 steeringFlags;
  u8 paused;
  u8 retainSpriteCache;
  u8 repeatCount;
  s16 wait;
  u8 refused;
  u8 frozen;
  u8 riseCounter;
  u8 riseEnabled;
  s16 angle;   ///< its place on the circle, 1/65536 turns, in timer
  s16 stopped; ///< nonzero holds it still, in userValue
};

/// The map work as the World Map's sinking of Idejima reads it: two
/// halfwords of map layer 0's scale, stepped against each other.
/// XXX: what the two halfwords are on the World Map; the World Map's own
/// reading of layer 0's scale would settle it.
struct RescueMap {
  u8 unk000[0x11c];
  u16 unk11c;
  u16 unk11e;
};

/// A field sprite (struct FieldSprite) read a byte at a time, as Yampi Desert's
/// scorpion sets its colour mode and priority and loads its tiles.
struct YampiDesertVisual {
  u8 link[4];
  u8 y;         ///< attribute 0's y
  u8 attr0High; ///< attribute 0's high byte: bit 5 is full colour
  u8 attr1[2];
  u8 tileLow; ///< attribute 2's low byte
  /// Attribute 2's high byte.
  /// bits: 7..4 palette | 3..2 priority | 1..0 tile bits 9..8
  u8 attr2High;
  u8 attr3[2];
  u8 scale[4];
  u8 vramBlock; ///< VRAM cache entry holding its tiles
  u8 cacheFlags;
  u8 rotation[2];
  u8 size[4]; ///< width, height and the x and y offsets
  u8 frame;
  u8 dirty;
  u8 nextFrame;
  u8 layerCount;
};

void MapGrid_CenterOnFollowedPoint(void);

extern u8 gField_CachedTilesA[];

extern u8 gField_CachedTilesD[];

extern const u8 gField_GroundTiles[];

s32 Speaker_GetVoice(s32 object);

/// An explosion's 16 sprite colours (graphics/field/explosion_palette.png).
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

#endif
