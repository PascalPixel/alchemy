#ifndef ALCHEMY_SCENE_SCENE_H
#define ALCHEMY_SCENE_SCENE_H

/// @file
/// What the scenes share: the views of actors, sprites and state they read in
/// their own way, and the constants of the mechanisms several scenes keep
/// copies of (geysers, vents, smoke, sparks, cracked tiles).

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "field/field_coordinate.h"
#include "field/sprite.h"
#include "field/field_psynergy_state.h"
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
  FIXED orbitRadius; ///< 16.16 fixed-point pixels, in sceneWord
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
  u8 sceneByte;
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
  FIXED orbitRadius; ///< 16.16 fixed-point pixels, in sceneWord
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

/// Half a turn of an Orbit's phase, 64 to the turn: the sparks that rise round
/// one who learns a Psynergy take every other phase short of it, one each 2
/// pixels up a column round him, so their circles widen and narrow again up
/// the column.
#define ORBIT_HALF_TURN 32

/// The sparks that rise from a Psynergy's tablet as it lifts, flashing.
#define TABLET_RISING_SPARKS 16

/// The LEARN_SPARKS that circle the one who learns a Psynergy start
/// LEARN_SPARK_INTERVAL frames apart, 122 pixels out, and close 8 pixels a
/// frame (Orbiter_Spiral) to 2 pixels in LEARN_SPIRAL_FRAMES; each one's
/// countdown ends with the last's.
///@{
#define LEARN_SPARKS 16
#define LEARN_SPARK_INTERVAL 8
#define LEARN_SPIRAL_FRAMES 15
///@}

/// How far a pillar sinks, its top then level with the floor: into the floor
/// as Blaze at a torch lowers it in Magma Rock, and into its gap as Move drops
/// it in Jupiter Lighthouse.
#define PILLAR_SINK_DEPTH 32

/// SPRITE_PARTICLE's swells, from a speck to its full glow, held 32 frames,
/// and back: slowly, 12 frames a step, or quickly, 6. Jupiter Lighthouse's
/// sparks and the motes of light inside Air's Rock take turns at them.
enum { ANIM_PARTICLE_SLOW_SWELL = 9, ANIM_PARTICLE_QUICK_SWELL };
/// How long such a particle lives before it starts again: a slow swell and the
/// 12 frames it stays gone after, or a quick one.
enum { PARTICLE_SLOW_LIFETIME = 212, PARTICLE_QUICK_LIFETIME = 116 };

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
struct AnkohlRuinsSpriteState {
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

/// The map state (struct MapState) with its ground marks as bit-fields, for
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

/// The palette fade's state (heap slot paletteFade) as 256 colours.
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

/// The field effect state as the Golden Rings read it: the VRAM cache entry
/// their sprites share, in bytes struct FieldEffectState leaves unnamed.
struct LinkedEffectState {
  u8 beforeVramBlock[1984];
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
  s32 sceneWord;
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
  u8 sceneCounter;
  u8 sceneByte;
  s16 angle;   ///< its place on the circle, 1/65536 turns, in timer
  s16 stopped; ///< nonzero holds it still, in userValue
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
  u8 size[4];       ///< width, height and the x and y offsets
  u8 lastAnimation; ///< the animation number last selected
  u8 dirty;         ///< nonzero redraws its pixels
  u8 flags;         ///< the sprite flags (enum SpriteFlag)
  u8 layerCount;
};

void MapGrid_CenterOnFollowedPoint(void);

/// Compressed tiles the scenes unpack into the VRAM cache
/// (graphics/field/cached_a.png to cached_d.png).
extern u8 gField_CachedTilesA[];

/// See gField_CachedTilesA (graphics/field/cached_d.png).
extern u8 gField_CachedTilesD[];

/// Compressed ground tiles (graphics/field/ground_tiles.png).
extern const u8 gField_GroundTiles[];

s32 Speaker_GetVoice(s32 object);

/// An explosion's 16 sprite colours (graphics/field/explosion_palette.png).
extern const u16 Field_ExplosionPalette[];
/// The sparks of the explosion that Loho's cannon blast and the Northern
/// Reaches' shattered ice wall throw over the screen: how many, a spark's life
/// in frames (the explosion sprite sheet's eight pictures, four frames each),
/// the sprite palette their colours are loaded into (Field_ExplosionPalette),
/// and their depth slot among the field's sprites.
#define EXPLOSION_SPARK_COUNT 96
#define EXPLOSION_SPARK_FRAMES 32
#define EXPLOSION_SPARK_PALETTE 15
#define EXPLOSION_SPARK_DEPTH 100

void FieldPsynergy_StartCast(void);

/// Counts one more weird answer Felix gives (Event_CountAnswer, through the
/// overlay's own import row).
void EasternAlhafraShip_CountAnswer();

/// A geyser's compressed tiles (graphics/field/geyser.png).
extern const u8 gField_GeyserTiles[];

/// The geysers Scoop digs up in Gondowan Cliffs and in Yampi Desert's scorpion
/// pit (each scene's struct GeyserState): the most sprites Geyser_Draw queues
/// in a frame, and the geysers a room can have.
#define GEYSER_SPRITES 80
#define GEYSER_ENTRIES 16
/// A geyser's six-second round, and the frames of it at which it starts to rise
/// and to fall.
#define GEYSER_PERIOD_FRAMES 360
#define GEYSER_RISE_FRAME 60
#define GEYSER_FALL_FRAME 240
/// The pixels a geyser rises to.
#define GEYSER_MAX_HEIGHT 64
/// The terrain id and collision slot of the first geyser: the geysers take the
/// last GEYSER_ENTRIES of the 256 terrain ids.
#define GEYSER_FIRST_TERRAIN (256 - GEYSER_ENTRIES)
/// The geysers' state (each scene's struct GeyserState), kept in the inventory
/// snapshot's spare space.
#define GEYSER_STATE                                                           \
  ((struct GeyserState *)(gInventorySnapshot + INVENTORY_SNAPSHOT_SPARE))

/// See gField_CachedTilesA (graphics/field/cached_b.png).
extern u8 gField_CachedTilesB[];

/// See gField_CachedTilesA (graphics/field/cached_c.png).
extern u8 gField_CachedTilesC[];

/// The cell stampers of Air's Rock's and Magma Rock's insides keep the last
/// cell they stamped under the leader in one word, x in the high half and z in
/// the low (AirsRockInside_StampCellUnderLeader,
/// MagmaRockInside_StampStepCell): the word for cell (x, z), and the x and z a
/// word holds.
#define CELL_KEY(x, z) (((x) << 16) + (z))
#define CELL_KEY_X(key) ((key) >> 16)
#define CELL_KEY_Z(key) ((key) & 0xffff)

void LineScroll_InitWave(s32 mode, s32 freqX, s32 stepX, s32 ampX, s32 freqY,
                         s32 stepY, s32 ampY);

/// The scene's trigger of a kind for a target whose condition holds, or NULL
/// (Scene_FindTrigger, through the overlay's own import row).
s32 SceneEvent_FindActiveRecord(s32 mode, s32 value);

/// The map screen's third set of compressed tiles
/// (graphics/world_map/storyc.png).
extern const u8 gWorldMapPackedTilesC[];

struct Vector3;
struct Window;

/// The lightning bolt's picture (RESOURCE_LIGHTNING_TILES and
/// RESOURCE_LIGHTNING_MAP), shown on BG0 for the strike: its tiles at the start
/// of character block 0, and its map, LIGHTNING_MAP_ROWS rows of
/// LIGHTNING_MAP_COLUMNS numbered from tile 512, on screen block
/// LIGHTNING_SCREEN_BLOCK.
#define LIGHTNING_TILES 64
#define LIGHTNING_MAP_ROWS 16
#define LIGHTNING_MAP_COLUMNS 4
#define LIGHTNING_SCREEN_BLOCK 4

/// Plays an actor's animation and lets frames pass before the event goes on.
#define Actor_AnimateThenWait(id, animation, frames)                           \
  do {                                                                         \
    Actor_SetAnimation(id, animation);                                         \
    FieldEvent_Wait(frames);                                                   \
  } while (0)

/// gMaps holds Mars Lighthouse's nineteen maps twice, from its outside to its
/// Venus ledge: frozen from row 270, then again from row 289 in the colours the
/// lighthouse takes once it is thawed (FLAG_MARS_LIGHTHOUSE_THAWED). A frozen
/// map's thawed copy is MARS_LIGHTHOUSE_THAWED_MAPS rows on, the distance the
/// linker gives between the two first rows: they are named here for the scene
/// ids that share their numbers, Prox (270) and Jupiter Ledge (289).
extern char MapRow_MarsLighthouseOutside __asm__("SceneId_Prox");
extern char MapRow_MarsLighthouseOutsideThawed __asm__(
    "SceneId_MarsLighthouseJupiterLedge");
#define MARS_LIGHTHOUSE_THAWED_MAPS                                            \
  ((s32)&MapRow_MarsLighthouseOutsideThawed -                                  \
   (s32)&MapRow_MarsLighthouseOutside)

/// The Lemurian Ship's background palette, the blues of the band along its hull
/// and the surf, in the scenes that show the ship: Atteka Inlet, Lemuria's dock
/// and the ship's deck.
#define LEMURIAN_SHIP_PALETTE 11
/// The runs of a background palette's colours the shore scenes turn a step at
/// a time: 12 to 15, the surf, and on LEMURIAN_SHIP_PALETTE also 4 to 9, the
/// blues of the band along its hull.
#define HULL_BAND_FIRST_COLOR 4
#define HULL_BAND_LAST_COLOR 9
#define SURF_FIRST_COLOR 12
#define SURF_LAST_COLOR 15
/// The same run of four as Gondowan Cliffs (background palette 0) and SW Atteka
/// Islet (palette 3) turn their water's colours in place
/// (GondowanCliffs_CycleWaterColors, Settlements_CycleWaterColors): its first
/// colour and how many.
#define FIRST_WATER_COLOR 12
#define WATER_COLORS 4

/// The frames a puff of smoke from a chimney or a cooking fire rises and
/// swells before it is released: Prox's, Kalt Island's (the Settlements),
/// Shaman Village's and Kibombo's, each scene's own copy of the same drift.
#define SMOKE_FRAMES 60

/// The raised trigger a whirlwind raises as it catches the leader, which each
/// scene with whirlwinds answers by carrying him off on it: the Jupiter
/// Lighthouse rooms with Whirlwind_CarryLeader (shared/whirlwind_update.h
/// raises it), Air's Rock with AirsRock_RideWhirlwind and its inside with
/// AirsRockInside_RideWhirlwind.
#define RAISED_RIDE_WHIRLWIND 200

/// The cell behavior of a cracked tile in the Jupiter Lighthouse rooms with
/// cracked floors (the blue key's, the cracked floor's and the Phaeton's Blade
/// chest's), the kind CrackedTile_Find looks for; a tile takes the next one up
/// as it cracks.
enum { CELL_BEHAVIOR_CRACKED_TILE = 33 };
/// The fall through such a tile to the floor below, on the room's own map: the
/// exit it takes and the entrance it lands at share the number.
enum { THROUGH_CRACKED_TILE = 13 };

/// How near, in pixels, the two walking children of Lemuria and of Contigo
/// heed: the other child they face, the leader they face, and the leader come
/// so near that they walk on (Lemuria_NoticeLeader, Contigo_PauseWhenFaced).
enum {
  CHILD_SEES_CHILD_RANGE = 18,
  CHILD_SEES_LEADER_RANGE = 26,
  CHILD_WALKS_ON_RANGE = 11
};

/// The vents of Aqua Rock and Taopo Swamp (struct VentState, shared/vent_*.h):
/// the most vents a room has, and the most actors besides the leader their
/// puffs land on.
#define VENT_MAX 16
#define VENT_IDS_MAX 4

/// One vent: whether it blows, the frames to its next puff, its position and
/// its gust's speed, in 16.16 pixels a frame.
struct VentEntry {
  s16 active;
  s16 timer;
  FIXED x;
  FIXED y;
  FIXED z;
  FIXED value;
};

/// The vents of the room, in the inventory snapshot's spare space (VENT_STATE):
/// their frame counter, the room's hook run each frame, its vents and the
/// actors their puffs land on, besides the leader.
struct VentState {
  s32 counter;
  s32 (*hook)(void);
  struct VentEntry entries[VENT_MAX];
  u16 ids[VENT_IDS_MAX];
};

/// The vents' state, kept in the inventory snapshot's spare space.
#define VENT_STATE                                                             \
  ((struct VentState *)(gInventorySnapshot + INVENTORY_SNAPSHOT_SPARE))
/// How far apart a and b are, never negative, as a vent's thrown object tests
/// whether it rests on another in x and z (field/actor_vent_bounce.c). Each is
/// read twice.
#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))
/// The names field/actor_vent_bounce.c's two routines take in both rooms'
/// copies: whether a thrown object rests on another, and its motion.
#define VENT_TOUCHING Vent_IsTouching
#define VENT_BOUNCE Vent_Bounce
/// Vent index's entry: `state` moved on by index entries and read as the first,
/// as the vents' accessors (Vent_SetBlowing, Vent_SetGust) read it.
#define VENT_AT(state, index)                                                  \
  ((struct VentState *)((u8 *)(state) + (index) * sizeof(struct VentEntry)))

#endif
