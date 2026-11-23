#ifndef ALCHEMY_FIELD_H
#define ALCHEMY_FIELD_H

/// @file
/// The field's routines and tables other banks call, and the field Psynergy's
/// reach.

#include "fixed_math.h"
#include "types.h"
#include "edition.h"
#include "field/actor.h"
#include "system/vector3.h"
#include "system/io_reg.h"
#include "resource_ids.h"
#include "system/bg_scroll.h"
#include "field/particle.h"
#include "field/sprite.h"
#include "field/event_commands.h"
#include "system/sprite_metadata.h"
#include "tla_edition.h"

struct Actor;
struct ScreenPoint;

extern const FIXED gCamera_FixedViewMatrix[];
extern u8 gSpriteDissolve_PixelOrder[];
extern const u8 gWorldMap_TerrainBehaviorTable[];
extern u8 gSpriteColorRemapTables[];
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);
s32 Party_CheckShortcut(u32 packed_id);
void SpriteDissolve_Step(u8 *object, u32 position);
void FieldCamera_FollowActor(s32 id, s32 flag);
void Actor_UpdateAllMotion(void);
void Actor_UpdateAllThumb(void);
void Actor_SetSpriteFlags(struct Actor *actor, u32 flags);
void ActorSystem_ProjectSprites(void);
s32 Actor_FindLastActiveId(void);
void BattleMotion_ProjectPositionFar(s32, s32 *);
s32 Unit_KnowsPsynergyFar(s32 group, s32 entry);
s32 Party_ShowJoinedMessageFar(s32 event_id, s32 value);
s32 Party_ShowPairJoinedMessageFar(s32 first, s32 second);
s32 Field_CheckSpheresOverlap(FIXED *a, s32 radius_a, FIXED *b, s32 radius_b);

/// SpriteDissolve_Step clears one pixel of each 8-bit tile a step, in
/// gSpriteDissolve_PixelOrder's order, so a tile is gone after this many.
#define DISSOLVE_TILE_PIXELS 64
/// The step the first tile starts to dissolve at; each tile after it starts a
/// step sooner.
#define DISSOLVE_FIRST_STEP 64
/// The steps a dissolve takes, four a frame: in the field, SpriteDissolve_Run
/// and SpriteDissolve_RunMany's, and in battle a felled enemy's (PLAY_FELLED).
#define DISSOLVE_STEPS (DISSOLVE_FIRST_STEP + DISSOLVE_TILE_PIXELS)

struct Sprite;
struct Particle;
struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneRegion;

extern u32 gDriftScene_SpawnScript[];
extern u32 gSceneExits[];
/// The actors ObjectTable_Snapshot saved, up to 32, after their slot numbers.
extern struct Actor gSnapshotObjects[];
extern u8 gLinkLobby_SlotColumns[];
extern s32 gLinkLobby_SlotValues[];
extern const u8 gActor_BlinkSpriteColors[4];
extern const u8 gActor_StepAngleScript[];
extern u16 gScript_MainScript[];
void Actor_SetBlendMode(struct Actor *actor, u32 value);
s32 PaletteFade_SetTargetFar(s32, s32);
void FieldPsynergy_FinishMindRead(void);
void FieldPsynergy_Dispatch(void);
void FieldPsynergy_LoadResources(s32 action, s32 flags);
void FieldPsynergy_Run(void);
void Encounter_SelectBattleMusic(s32 row, s32 column);
void FieldPsynergy_SetupActorPair(s32 actor, s32 target);
s32 PaletteFade_StartInterpolationFar(s32);
s32 BattleMotion_ProjectScaledPositionFar(s32, s32 *);
struct BattleEffectParams;
void BattleEffect_RunFlurry(struct BattleEffectParams *effect, s32 variant);
void BattleEffect_StopMainActor(void);
void Event_SpawnObjectTable(struct ActorPlacement *entry, s32 slot);
void Actor_TurnPairToFaceEachOther(struct Actor *first, struct Actor *second);
void Field_StartPaletteTransition(s32 frames);
void FieldPsynergy_RunHandler(s32 kind);
void FieldPsynergy_RunExtraHandler(void);

s32 Actor_PlaySoundInViewFar(struct Actor *interpreter, s32 value);
void Actor_ReleaseAtIndex(u32 actorIndex);
void Actor_SetSquareSprite(u32 objectId);
void BattleEffect_DropObjectAtAnchor(s32 actorId);
void SuperLuckyDice_Run(void);
void GameFlag_RefreshLureCapFar(void);
void MapGrid_CenterOnFollowedPointFar(void);
FIXED MapGrid_GetTerrainHeightFar(s32 layer, FIXED x, FIXED z);
void Actor_RunRepeatedMotion(u32 objectId, s32 variant);
s32 Actor_IsTargetUnset(struct Actor *actor);
#ifndef OWN_Actor_SetPartAttribute
void Actor_SetSpriteColor(struct Actor *actor, s32 attribute);
#endif
void Scene_SetActorSpritePriority(struct Actor *actor, s32 priority);
void Actor_StartCameraFar(void *actor, void *effect);
void Entrance_EndArrivalAnimation(s32 effect);
void Exit_PrepareAnimation(s32 effect);
void Actor_CenterOnColumn(u32 objectId, s32 action, s32 z_offset);
void Actor_WalkByAndWaitById(u32 objectId, s32 x_offset, s32 z_offset);
void Actor_WalkByById(u32 objectId, s32 x_offset, s32 z_offset);
void Actor_SetDestinationById(u32 objectId, s32 x, s32 z);
void Actor_MoveToAndWaitById(u32 objectId, s32 x, s32 z);
/// Puts the actor at (x, y, z), 16.16 pixels, at rest, raised by the ground
/// height there if it follows the ground.
void Actor_SetPositionWithTerrain(u32 objectId, FIXED x, FIXED y, FIXED z);
void Actor_StartRepeatedMotion(u32 objectId, s32 variant);
void ActorSystem_Configure(s32 mode);
s32 Party_AddMemberFar(s32 event_id);
void Party_RemoveMemberFar(s32 owner);
void Render_ApplyProjectedPlacement(struct Sprite *sprite, s32 *position,
                                    s32 *scale, s32 facing);

/// The depth slot Render_ApplyProjectedPlacement gives an actor's sprite on a
/// scrolling map: its screen row on from this slot, so a sprite lower on the
/// screen draws in front and those up to this many rows above it keep their
/// order; its shadow takes two slots behind.
#define PLACEMENT_IMAGE_DEPTH 62

s32 Matrix_ProjectPoint(const void *point, s32 *screen);
void Scene_SetArrivalFlags(s32 unused);
s32 Message_OpenWindowFar(s32 message, s32 x, s32 y, s32 packed);
void AreaNameWindow_HideFar(void);
s32 Window_IsClosedFar(s32);

/// How far a field Psynergy reaches from its caster, one row a kind in
/// gFieldPsynergy_Reach, ended by -1: Field_MeasurePsynergyHeight and
/// Field_MeasurePsynergyReach read it.
struct FieldPsynergyReach {
  s16 fieldPsynergy; ///< enum FieldPsynergy; -1 ends the table
  s16 range;         ///< pixels over the ground
  s16 height;        ///< pixels above or below the caster
  s16 width;         ///< either side of the caster's facing, 1/65536 turns
};

extern const struct FieldPsynergyReach gFieldPsynergy_Reach[];
/// Added to a field Psynergy's kind, the gFieldPsynergy_Reach row for the
/// objects its shaking reaches (Tremor's, 275).
#define REACH_SHAKEN_OBJECTS 256

/// Actor.spriteStorageKind, its low four bits: what the actor's sprite
/// pointer holds.
enum ActorSpriteKind {
  ACTOR_SPRITE_NONE = 0, ///< nothing drawn
  ACTOR_SPRITE_ONE = 1,
  ACTOR_SPRITE_GROUP = 2 ///< a list of up to four sprites, empty ones null
};

/// Actor.spriteStorageKind's low four bits, its enum ActorSpriteKind.
#define ACTOR_SPRITE_KIND_MASK 15

/// Actor_Create's id / 4096: what sprites the actor is made with.
enum ActorCreateKind {
  ACTOR_CREATE_ONE_SPRITE = 0, ///< the id's sprite (ACTOR_SPRITE_ONE)
  /// The sprites of the id and id + 1 (ACTOR_SPRITE_GROUP).
  ACTOR_CREATE_GROUP = 2,
  /// No sprite: any kind but the two above, and the one the camera actor
  /// takes.
  ACTOR_CREATE_NO_SPRITE = 8
};

/// The sprite id in Actor_Create's id, below its enum ActorCreateKind (id /
/// 4096).
#define ACTOR_CREATE_SPRITE_MASK 0xfff
/// An Actor_Create id of enum ActorCreateKind `kind`, above its sprite.
#define ACTOR_CREATE_KIND(kind) ((kind) << 12)

/// The world map's still image (story.s): its tiles, packed (tagged LZ).
extern u8 gWorldMapPackedTiles[];
/// The still image's tile map, packed (tagged LZ), decoded after its tiles.
extern u8 gWorldMapPackedPictures[];
/// The still image's palettes, one for each of its modes.
extern u8 gWorldMapPalettes[];

struct Window;
struct ScriptOperands;
struct SpritePlacement;
struct TriggerView;

typedef void (*DrawFn)(const void *src, void *dst, s32 param);

typedef void (*UploadFn)(const void *src, u32 w, u32 h, void *vram);

typedef void (*OutlineFn)(u32 w, u32 h, void *pixels, u32 edge, u32 fill);

/// An actor script command's handler: it takes the actor and returns nonzero
/// to go on with the script this frame, zero to stop. The handlers take
/// different views of the actor, so the type leaves the parameters
/// unspecified.
typedef s32 (*ScriptCommandFn)();

typedef void (*TargetHook)(struct Actor *, s32 *, s32 *);

/// One sprite's two OAM words, volatile because they are built in place for the
/// DMA to copy to OAM.
struct OamWords {
  volatile u32 attr01; ///< attr0 | attr1 << 16
  volatile u32 attr2;  ///< attr2 | affine value << 16
};

/// An affine sprite's matrix, of the 32, in its attr01 word.
#define OBJ_AFFINE_MATRIX(n) ((n) << 25)

/// A die of the dice game: its place and velocity, 16.16, and its variant.
struct DiceParticle {
  FIXED x;
  FIXED y;
  FIXED z;
  FIXED velocityX;
  FIXED velocityY;
  FIXED velocityZ;
  s32 variant;
};

/// The dice game's four dice.
struct DiceWork {
  struct DiceParticle particles[4];
};

/// The dice game's work as its drawing reads it: 128 OAM entries and its
/// coins, bet and help windows.
struct DiceSprites {
  u8 filler[196];
  struct OamWords oam[128];
  struct Window *coinsWindow;
  struct Window *betWindow;
  struct Window *helpWindow;
};

/// The dice games' camera: the eye and the point it looks at, 16.16.
struct DiceCamera {
  FIXED eye[3];
  FIXED focus[3];
};

/// The dice games' numbers, Lucky Dice's and Super Lucky Dice's alike: a die's
/// faces.
#define DICE_FACE_COUNT 6
/// A throw costs ten coins for each level of the party's average.
#define DICE_BET_PER_LEVEL 10
/// The coins, and Super Lucky Dice's bet, are drawn with six digits.
#define DICE_COIN_DIGITS 6
/// The dice screen fades in and out over 16 frames, a sixteenth of full
/// brightness a frame.
#define DICE_FADE_FRAMES 16
/// The depths from the camera a die or its shadow is drawn between: nearer or
/// further is drawn as at the limit, and the nearer, the larger.
#define DICE_NEAREST_DEPTH 350
#define DICE_FARTHEST_DEPTH 650
/// Where the dice's shadow starts in OBJ VRAM, in 16-colour tiles (32 bytes,
/// half a 256-colour tile): after the rolling die's eighteen 256-colour views.
#define DICE_SHADOW_TILE 144
/// A thrown die starts spinning at this rate, its roll angle a frame; each
/// bounce takes DICE_BOUNCE_SPIN_LOSS off it.
#define DICE_THROW_SPIN 300
#define DICE_BOUNCE_SPIN_LOSS 20
/// A die's roll angle (its particle's variant) crosses a face in
/// DICE_TILES_PER_FACE tiles: the tile is angle / DICE_ROLL_PER_TILE and the
/// face angle / DICE_ROLL_PER_FACE. It is the die's own unit, a whole roll
/// DICE_ROLL_TURN, not a TRIG_TURN angle.
#define DICE_TILES_PER_FACE 3
#define DICE_ROLL_PER_TILE 1024
#define DICE_ROLL_PER_FACE (DICE_TILES_PER_FACE * DICE_ROLL_PER_TILE)
#define DICE_ROLL_TURN (DICE_FACE_COUNT * DICE_ROLL_PER_FACE) ///< six faces
/// A die's or a shadow's affine scale at DICE_NEAREST_DEPTH, in 256ths: drawn
/// at twice its size, and smaller the further past it.
#define DICE_NEAREST_SCALE 128

/// A party member's unit as the field reads it: its gauges and its HP and PP.
struct FieldUnit {
  u8 unused[20]; ///< no code names it
  s16 hpGauge;
  s16 ppGauge;
  u8 unusedAfterPpGauge[28]; ///< no code names it
  s16 hpMax;
  s16 ppMax;
  s16 hp;
  s16 pp;
};

/// Lucky Dice's work as its drawing reads it: 128 OAM entries and its coins,
/// winnings and help windows.
struct LuckyDiceSprites {
  u8 filler[196];
  struct OamWords oam[128];
  struct Window *coinsWindow;
  struct Window *winningsWindow;
  struct Window *helpWindow;
};

/// A Lucky Wheels reel: its turn, its 21 symbols, whether it is held and
/// where it stops.
struct Reel {
  s32 position;
  u8 symbols[21];
  u8 held;
  s8 stop;
};

/// Lucky Wheels' effect work: the title letters' particles, the BG and OBJ
/// shades, and its transfer to the screen.
struct ReelEffectWork {
  struct Particle particles[LUCKY_WHEELS_TITLE_LETTERS];
  /// No code names it.
  u8 unusedAfterParticles[1792 -
                          LUCKY_WHEELS_TITLE_LETTERS * sizeof(struct Particle)];
  u16 bgShade[256];
  u16 spriteShade[256];
  u8 unusedAfterSpriteShade[27776]; ///< no code names it
  s32 transferMode;
  s32 transferValue;
  u8 unusedAfterTransferValue[4]; ///< no code names it
  s32 frame;
  u8 unusedAfterFrame[344]; ///< no code names it
  s32 transferPending;
};

/// The main routine work from gPartyDjinnRecovery on, as Lucky Wheels reads it:
/// the prizes won, kept after the spring game's prize weights.
struct ReelSave {
  u8 unused[320]; ///< summons, coins, Djinn recovery, shops, prize weights
  s8 wonPrizes[16];
};

/// One 32-pixel cell of the World Map: its tile and its encounter region;
/// Ram_MapBlocks holds 32 by 32 of them.
struct WorldMapCell {
  u32 tile : 24; ///< low halfword: the cell picture
  /// Passed to WorldMap_UpdateRegionTint; region / 10 picks the graphics set.
  u32 region : 6;
  u32 unused : 1; ///< no code names it
  /// A beach: WorldMap_GetBattleEnvironment fights on it from
  /// gWorldMap_TerrainBehaviorTable's beach bank.
  u32 beach : 1;
};

/// A map cell's terrain kind: the low four bits of its shape's first byte,
/// which pick its height function.
enum TerrainKind {
  TERRAIN_FLAT = 0,
  TERRAIN_SLOPE_X = 1,
  TERRAIN_SLOPE_Y = 2,
  TERRAIN_STEP_SUM_DIAGONAL = 3,
  TERRAIN_STEP_DIFFERENCE_DIAGONAL = 4,
  TERRAIN_BEND_SUM_DIAGONAL = 5,
  TERRAIN_BEND_DIFFERENCE_DIAGONAL = 6,
  TERRAIN_ROUNDED_SQUARE = 7,
  TERRAIN_STEP_X = 8,
  TERRAIN_STEP_Y = 9,
  /// As TERRAIN_SAMPLE_TABLE with x and y swapped.
  TERRAIN_SAMPLE_TABLE_TRANSPOSED = 10,
  /// As TERRAIN_SAMPLE_TABLE_MIRRORED with x and y swapped.
  TERRAIN_SAMPLE_TABLE_TRANSPOSED_MIRRORED = 11,
  /// The sample gTerrain_SampleIndexTable picks for each pixel.
  TERRAIN_SAMPLE_TABLE = 12,
  /// As TERRAIN_SAMPLE_TABLE, mirrored left to right.
  TERRAIN_SAMPLE_TABLE_MIRRORED = 13,
  /// Above y 8, the first sample left of x 8 and the second right of it; below,
  /// the third.
  TERRAIN_SPLIT_TOP_STEP = 14,
  /// Above y 8, the first sample; below, the second left of x 8 and the third
  /// right of it.
  TERRAIN_SPLIT_BOTTOM_STEP = 15,
  TERRAIN_KIND_COUNT = 16,
  /// Not a shape's kind: what MapGrid_GetCellAttributeLowNibbleOrBump gives
  /// for a TERRAIN_ROUNDED_SQUARE whose two samples differ, a bump or a hollow.
  TERRAIN_BUMP = 99
};

/// What MapGrid_CheckCollision says of a step to a position.
enum Collision {
  COLLISION_TOO_LOW = -1,  ///< the ground there is more than 12 pixels lower
  COLLISION_NONE = 0,      ///< the actor can step there
  COLLISION_TOO_HIGH = 1,  ///< the ground there is more than 8 pixels higher
  COLLISION_IMPASSABLE = 2 ///< the cell there is a wall (CELL_BEHAVIOR_WALL)
};

/// One node of a tightrope (Tightrope_Initialize's work): a point on the rope,
/// and the sprite Tightrope_Draw puts between it and the next node.
struct TightropeNode {
  struct TightropeNode *next;
  s32 x;
  s32 y;
  s32 z;
  s8 kind; ///< 0 while free; 1 a node the leader can step onto; 2 coiled
  s8 slot; ///< the tightrope's index plus one while it is thrown, else 0
  u8 unusedAfterSlot[2]; ///< no code names it
  struct AnimationSpritePart sprite;
};

/// One of the eight tightropes: the two posts it runs between and its wobble.
struct TightropeSlot {
  s16 first;
  s16 second;
  u32 direction;              ///< the sprites' turn, along the tightrope
  s32 amplitude;              ///< the wobble's size, decaying each frame
  u8 unusedAfterAmplitude[4]; ///< no code names it
  s32 phase;
  s32 wobbling;
  struct TightropeNode *head;
};

/// The ropes', tightropes' and hanging vines' works each keep eight slots,
/// each a chain of nodes from one pool of 128.
#define ROPE_SLOT_COUNT 8
#define ROPE_NODE_COUNT 128
/// A squared distance in pixels beyond any rope node, to start a search for
/// the nearest.
#define ROPE_NODE_DISTANCE_NONE (1 << 28)

/// The tightropes' work (HEAP_SLOT_TIGHTROPE).
struct TightropeWork {
  struct Actor
      *recoilFrom; ///< the posts Tightrope_Untie gives Tightrope_Recoil
  struct Actor *recoilTo;
  s32 vramEntry; ///< the nodes' picture
  struct TightropeSlot slots[ROPE_SLOT_COUNT];
  struct TightropeNode nodes[ROPE_NODE_COUNT];
};

#define TIGHTROPE_WORK ((struct TightropeWork *)Ram_HeapSlots->tightrope)

/// The point the map view follows (MapWork.origin), as the depth sort reads it.
struct DepthOrigin {
  FIXED x; ///< 16.16 fixed-point pixels
  FIXED y; ///< its height, 16.16 fixed-point pixels
};

/// The map view's place (MapWork.viewX and viewZ), read as a pair.
struct DepthScroll {
  FIXED x; ///< 16.16 fixed-point pixels
  FIXED z; ///< 16.16 fixed-point pixels
};

/// MapWork as BattleEffect_GetObjectDepth reads it: its followed point and its
/// view, through a pointer to the pair.
struct DepthMap {
  struct DepthOrigin *origin;
  u8 unusedAfterOrigin[224]; ///< no code names it
  struct DepthScroll view;
};

/// One rising particle: its sprite, world position, drift and frames left.
struct RisingParticle {
  /// The sprite: set as three words by the start, drawn field by field.
  union {
    u32 words[3];
    struct {
      u8 unusedAfterWords[4]; ///< no code names it
      /// OAM attributes 0 to 2.
      ///@{
      u16 y : 8; ///< screen pixels
      u16 attr0 : 2;
      u16 blendMode : 2;
      u16 attr0b : 2;
      u16 shape : 2;
      u16 x : 9; ///< screen pixels
      u16 attr1 : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      ///@}
      u8 unusedAfterPalette[2]; ///< no code names it
    } attributes;
  } sprite;
  s32 posX;
  s32 posY;
  s32 posZ;
  s32 drift;
  u16 timer;
  u8 unusedAfterTimer[2]; ///< no code names it
};

/// HEAP_SLOT_WEATHER while the rising particles run: their tiles' VRAM cache
/// entry and first tile, and the particles.
struct RisingParticleWork {
  s32 vramEntry;
  s32 tile;
  struct RisingParticle particles[64];
};

/// An actor (struct Actor) as the Mars Djinni's sparks read the leader: its
/// position and its sprite, typed.
struct EffectOwner {
  u8 unused[8]; ///< no code names it
  struct Vector3 position;
  u8 unusedAfterPosition[60]; ///< no code names it
  struct Sprite *sprite;
};

/// The line scroll's work: two pages of BG1 to BG3 scroll offsets for each of
/// the 160 scanlines, and the sine wave that bends them on each axis.
struct LineScrollWork {
  struct BgScroll rows[2][160][3]; ///< per page and scanline: BG3, BG2, BG1
  u8 page;      ///< the page the HBlank DMA shows; the other is built
  u8 mode;      ///< set at the start; not read by name
  u16 frame;    ///< frames built so far, added to BG3's y for the wave's phase
  u8 unused[4]; ///< zeroed with the block; nothing reads or writes it
  FIXED freqX;  ///< phase per frame and per BG3 row, 16.16 wave table entries
  FIXED freqY;  ///< phase per frame and per BG3 row, 16.16 wave table entries
  FIXED stepX;  ///< 16.16 wave table entries a scanline, 256 a cycle
  FIXED stepY;  ///< 16.16 wave table entries a scanline, 256 a cycle
  FIXED ampX;   ///< 16.16 scale on the sine entry, which is then divided by 256
  FIXED ampY;   ///< 16.16 scale on the sine entry, which is then divided by 256
};

/// One node of a rope Lash throws and ties (Rope_Initialize's work): a point on
/// the rope, and the sprite Rope_Draw puts between it and the next node.
struct RopeNode {
  struct RopeNode *next;
  s32 x;
  s32 y;
  s32 z;
  u16 angle; ///< the sprite's turn, along the rope
  s8 kind;   ///< 0 while free; 1 a node the leader can step onto; 2 coiled
  s8 slot;   ///< the rope's index plus one while it is thrown, else 0
  struct AnimationSpritePart sprite;
};

/// One of the eight ropes: the two posts it runs between and its swing.
struct RopeSlot {
  s16 first;    ///< the post it coils at
  s16 second;   ///< the post it is tied to
  s16 segments; ///< its nodes, less one
  s16 priority; ///< the sprites', the first post's
  s16 flag;     ///< set while the rope is tied
  s16 state;    ///< 1 once thrown or tied
  s32 amplitude;
  u8 unusedAfterAmplitude[4]; ///< no code names it
  s32 phase;
  s32 swinging;
  struct RopeNode *head;
};

/// The ropes' work (HEAP_SLOT_ROPE).
struct RopeWork {
  struct Actor *recoilFrom; ///< the posts Rope_Untie gives Rope_Recoil
  struct Actor *recoilTo;
  s32 vramEntry; ///< the nodes' picture
  struct RopeSlot slots[ROPE_SLOT_COUNT];
  struct RopeNode nodes[ROPE_NODE_COUNT];
};

#define ROPE_WORK ((struct RopeWork *)Ram_HeapSlots->rope)

extern const u8 gSuperLuckyDice_FaceTiles[];
s32 SuperLuckyDice_ScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale,
                                     s32 count);
void SuperLuckyDice_BrightenColors(u16 *source, u16 *destination, u32 count,
                                   s32 angle);
s32 SuperLuckyDice_ScoreDice(s32 *dice);
s32 SuperLuckyDice_DrawNumber(s32 sprite, s32 x, s32 y, s32 value, s32 palette,
                              s32 digits);
s32 SuperLuckyDice_DrawLargeNumber(s32 sprite, s32 x, s32 y, s32 value,
                                   s32 palette, s32 digits);

void Actor_ResetForLeader(s32 table);

void Portrait_CloseWindowFar(s32 sprite);

extern const s32 gFieldEffect_HeavyImpactScript[];
extern const s32 gFieldEffect_ShortEffectScript[];
extern const s32 gFieldEffect_MediumEffectScript[];
extern const s32 gFieldEffect_LongEffectScript[];

extern const s32 gFieldEffect_FragmentScript[];

s32 MapGrid_CheckCollisionFar(struct Actor *actor, s32 *position);

s32 Field_CheckSpheresOverlapFar(s32 *a, s32 radiusA, s32 *b, s32 radiusB);

void MapGrid_SetScreenShakeFar(s32 x, s32 y, s32 decay);

s32 Field_MeasurePsynergyHeight(s32 id, struct Actor *actor, s32 *position);

u32 MapGrid_GetTerrainKindFar(s32 terrain, s32 x, s32 z);

void WorldMap_Freeze(void);
void WorldMap_ReloadPendingAreaGraphics(void);

void Event_NoOp(void);

void Actor_UpdateBlink(struct Actor *actor);

/// The world map camera's yaw, in 1/65536 turns, the scanline parameters were
/// built for; the vertex update rebuilds them when it changes.
extern u32 gScanlineAngle;

/// Every sprite animation's metadata, by number (graphics/sprite/sprites.s).
extern struct AnimationMetadata gSprite_AnimationMetadata[];

/// Render_DecodePicture's Thumb entry and ARM code (picture_decode.s), which
/// Render_LoadDecoder copies into the frame scratch slot.
extern u8 Render_DecodePictureCode[];

/// The end of the colour tables kept after Render_DecodePicture's code, where
/// its copy stops (picture_decode_data.s).
extern u8 Render_DecodePictureCopyEnd[];

/// The size of the frame scratch block for Render_DecodePicture and its tables,
/// an address equal to the size (ld_script.ld).
extern u8 Render_DecodePictureBlockSize[];

/// The grid outliner's Thumb entry and ARM code (grid_outline_entry.s), which
/// RamCode_Load copies to RAM.
extern const u8 Grid_DrawOutlineCode[];

/// The size of Grid_DrawOutlineCode, an address equal to the size
/// (ld_script.ld).
extern u8 Grid_DrawOutlineCodeSize[];

extern const u8 gSpriteColorShadeTables[];

void SpriteAnimation_Start(struct SpriteAnimation *work, s32 no);

u8 *Decompress_Lz(const u8 *source, u8 *destination);

s32 Party_CheckItemLimit(s32 item);

s32 WorldMap_GetCollisionFar(s32 *position);

/// The map's scene data in EWRAM, loaded from its resource: its entrance
/// stream, then from the halfword offsets after it its edits and its bounds.
extern u8 gMapScene[];

void Party_ClearUnusableShortcuts(void);

void BattleBackground_SelectForMap(void);

s32 ScenePoint_GetPosition(s32 id, FIXED *position);
void ObjectTable_Clear(void);

/// Party_AddPp's amount that fills every member's PP: "Everyone's PP was
/// replenished!"
#define PARTY_PP_REPLENISH 999
void Party_AddPp(s32 amount, s32 unused);
void Party_AddHp(s32 amount, s32 scaled);
s32 Party_ApplyPoisonDamage(void);
struct Actor *Field_GetPartyActor(void);

s32 Actor_GetTriggerCellAheadOfCurrent(void);
u32 Stream_ReadU16(u8 **cursor);
s16 Stream_ReadS16(u8 **cursor);
void Field_ApplyEntrance(void);

void PaletteEditor_Run(void);

struct TriggerView *Scene_FindTrigger(s32 kind, s32 target);
s32 Scene_TalkToActor(s32 id);
s32 Scene_RunRaisedTrigger(s32 id);
s32 Scene_RunStepTriggerOrExit(s32 value);
s32 Field_RunPushCell(s32 keys);
s32 Field_ExamineCell(s32 id);

s32 Leader_CanPushObjectAhead(void);
void Leader_PushObjectAhead(void);

s32 Field_UsePsynergy(u32 code);

s16 *SceneItem_GetRows(void);
void Field_PlaceTreasureSpots(void);
s32 Actor_GetSpriteShape(u32 objectId);
void TreasureSpot_CloseChest(u32 objectId);
void TreasureSpot_LaunchMimic(u32 objectId);
void TreasureSpot_OpenChest(u32 objectId);
void TreasureSpot_ShowOpened(u32 objectId);
void ObjectEvent_SetMode6WithCue(u32 objectId);
struct Actor *TreasureSpot_GetActor(u32 objectId);
void ObjectEvent_Remove(u32 objectId);

void Camera_ResetViewDefaults(void);
s32 LuckyDice_ScaleRgb555Buffer(u16 *source, u16 *destination, FIXED scale,
                                s32 count);

/// Copies size bytes of 8-bit pixels from source to destination, then halves
/// each colour number left in source, so a drawing fades as it is shown: Lucky
/// Wheels' trails (CANVAS_FADE_HALF).
void ColorBuffer_HalveNonzero(u8 *source, u8 *destination, s32 size);
/// ColorBuffer_HalveNonzero, taking each colour number to three quarters, for
/// the trails that fade more slowly.
void ColorBuffer_ScaleNonzeroThreeQuarters(u8 *source, u8 *destination,
                                           s32 size);

/// gMainRoutineWork from its summonMask on, under a label of its own: the
/// party's DjinnRecoveryTable, which Djinn_GetRecoveryTable returns. Lucky
/// Wheels reads its won prizes 320 bytes in (ReelSave).
extern u8 gPartyDjinnRecovery[];

extern const u16 gLuckyWheels_SparkCellOffsets[];

s32 BattleEffect_BuildBlitterFar(s32 slot, s32 widthShift, s32 heightShift,
                                 s32 flags, u32 mode);

/// The tileset animations' tiles, 16 KB of EWRAM.
extern u8 gMapGridAnimTiles[];

s32 WorldMap_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void PaletteFade_LoadPaletteFar(void *palette);

void TilesetAnim_StartAll(const u16 *script);

void WorldMap_UpdateCurrentTileBlock(void);

extern const u16 gSpriteViewer_BackdropColors[8];

void ActorSystem_Initialize(s32 mode);

s32 SpritePictureCache_Load(u32 slot, u32 *buffer, s32 number, u32 variant);

void SpriteViewer_SetGridColumnPriority(s32 slot, s32 value);
void SpriteViewer_SetGridColumnColorMode(s32 slot, s32 value);
void SpriteViewer_StartStaggeredAnimations(s32 slot, s32 value);
void SpriteViewer_SetGridColumnNumber(s32 slot, s32 value);
void SpriteViewer_BuildProbeRing(s32 x, s32 z, struct SpritePlacement *out);

/// The work value handlers of gActorScript_OperandHandlerTable, one an SW_
/// value: each sets, adds to or compares (into conditionResult) the actor field
/// it names; positions, targets, velocities and gravity are 16.16 pixels.
void ActorScript_SetOrCompareScriptAddress(struct ScriptOperands *state,
                                           s32 operation, s32 value);
void ActorScript_SetOrCompareCursor(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void ActorScript_SetOrCompareFacing(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void ActorScript_SetOrCompareX(struct ScriptOperands *state, s32 operation,
                               s32 value);
void ActorScript_SetOrCompareY(struct ScriptOperands *state, s32 operation,
                               s32 value);
void ActorScript_SetOrCompareZ(struct ScriptOperands *state, s32 operation,
                               s32 value);
void ActorScript_SetOrCompareRadius(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void ActorScript_SetOrCompareScaleX(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void ActorScript_SetOrCompareScaleY(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void ActorScript_SetOrCompareVelocityX(struct ScriptOperands *state,
                                       s32 operation, s32 value);
void ActorScript_SetOrCompareVelocityY(struct ScriptOperands *state,
                                       s32 operation, s32 value);
void ActorScript_SetOrCompareVelocityZ(struct ScriptOperands *state,
                                       s32 operation, s32 value);
void ActorScript_SetOrCompareSpeedLimit(struct ScriptOperands *state,
                                        s32 operation, s32 value);
void ActorScript_SetOrCompareAcceleration(struct ScriptOperands *state,
                                          s32 operation, s32 value);
void ActorScript_SetOrCompareTargetX(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void ActorScript_SetOrCompareTargetY(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void ActorScript_SetOrCompareTargetZ(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void ActorScript_SetOrCompareVerticalMotion(struct ScriptOperands *state,
                                            s32 operation, s32 value);
void ActorScript_SetOrCompareGravity(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void ActorScript_SetOrCompareTerrainHeight(struct ScriptOperands *state,
                                           s32 operation, s32 value);
void ActorScript_SetOrCompareRiseBaseY(struct ScriptOperands *state,
                                       s32 operation, s32 value);
void ActorScript_SetOrCompareSprite(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void ActorScript_SetOrCompareSpriteStorageKind(struct ScriptOperands *state,
                                               s32 operation, s32 value);
void ActorScript_SetOrCompareMotionFlags(struct ScriptOperands *state,
                                         s32 operation, s32 value);
void ActorScript_SetOrCompareArrivalAxis(struct ScriptOperands *state,
                                         s32 operation, s32 value);
void ActorScript_SetOrCompareComparisonResult(struct ScriptOperands *state,
                                              s32 operation, s32 value);
void ActorScript_SetOrCompareSnapToTarget(struct ScriptOperands *state,
                                          s32 operation, s32 value);
void ActorScript_SetOrCompareCollisionFlags(struct ScriptOperands *state,
                                            s32 operation, s32 value);
void ActorScript_SetOrCompareSteeringFlags(struct ScriptOperands *state,
                                           s32 operation, s32 value);
void ActorScript_SetOrComparePaused(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void ActorScript_SetOrCompareRepeatCount(struct ScriptOperands *work,
                                         s32 operation, s32 value);
void ActorScript_SetOrCompareWait(struct ScriptOperands *work, s32 operation,
                                  s32 value);
void ActorScript_SetOrCompareTimer(struct ScriptOperands *work, s32 operation,
                                   s32 value);
void ActorScript_SetOrCompareUserValue(struct ScriptOperands *work,
                                       s32 operation, s32 value);
void ActorScript_SetOrCompareWatchedActor(struct ScriptOperands *work,
                                          s32 operation, s32 value);
void ActorScript_SetOrCompareUpdateAddress(struct ScriptOperands *work,
                                           s32 operation, s32 value);
void ActorScript_SetOrCompareRiseCounter(struct ScriptOperands *work,
                                         s32 operation, s32 value);
void ActorScript_SetOrCompareRiseEnabled(struct ScriptOperands *work,
                                         s32 operation, s32 value);

void Icon_DrawItemFar(s32 item);

s32 SceneItem_FindPosition(s32 value, s32 *out);

void MapGrid_SetCellBehaviorAtFar(s32 terrain, s32 x, s32 z, s32 value);

void SpriteAnimation_ClearFar(void *record);

void Djinn_ShowJoinedMessageFar(s32 owner, s32 element, s32 djinni);

/// The rope's tiles, tagged LZ (rope_vine_pictures.s).
extern const u8 gRope_Tiles[];
/// The bottom link of a hanging vine's chain, tagged LZ (rope_vine_pictures.s).
extern const u8 gHangingVine_ChainBottomPicture[];

s32 Field_FindNearbyPsynergyTarget(s32 self, s32 effect);

s32 MapGrid_GetCellAttributeLowNibbleFar(s32 layer, s32 x, s32 z);

/// The ground particles' tiles, byte LZ (field_effect_particles.s).
extern const u8 gFieldEffect_GroundParticleTiles[];
/// The motes' tiles, byte LZ (field_effect_particles.s).
extern const u8 gFieldEffect_MoteTiles[];

extern const u32 gBattleEffect_PulseScales[];

extern const s16 gLineScroll_WaveSine[];

extern const s16 gFieldEffect_GroundParticleFrames[];

/// The streaks' tiles, byte LZ (field_effect_particles.s).
extern const u8 gFieldEffect_StreakTiles[];

extern const struct SparklePicture gFieldEffect_SparklePictures[];

/// The sparkles' tiles, byte LZ (field_effect_particles.s).
extern const u8 gFieldEffect_SparkleTiles[];

void Actor_SpawnWaterSplashFar(struct Actor *actor, s32 kind);

s32 MapGrid_GetCellBehaviorAtFar(s32 layer, s32 x, s32 z);

s32 MapGrid_IsWallAtFar(s32 *position);

extern const u16 gRope_KeyHeadings[];
extern const u16 gFieldEffect_CyclePatternWords[];

/// The world map camera's pitch the scanline parameters were built for.
extern u32 gScanlinePitch;

extern const s16 gGroundParticles_Offsets[];

void Actor_MoveWithCarriers(void);

void ActorSystem_PlaceSprites(void);

extern const s32 gActor_KeyCheckScript[];
extern const s32 gActor_KeysScript[];
extern const s32 gActor_RopeScript[];
extern const s32 gActor_TightropeScript[];
extern const s32 gActor_SandScript[];
extern const s32 gActor_HoverScript[];
extern const s32 gActor_WalkingScript[];
extern const s32 gActor_PlayerScript[];
extern const s32 gActor_WorldMapScript[];
extern const s32 gActor_SailOnMapScript[];
extern const s32 gActor_SailScript[];
extern const s32 gActor_FlyScript[];
extern const s32 gActor_SandOnWorldMapScript[];
extern const s32 gActor_ApproachWatchedScript[];

FIXED MapGrid_GetTerrainHeight(s32 layer, FIXED x, FIXED z);

s32 ActorScript_CheckOverlap(struct Actor *actor, s32 *values);

void Actor_SelectAnimation(struct Actor *actor, s32 argument);
struct Actor *Actor_Create(s32 kind, FIXED x, FIXED y, FIXED z);

void Actor_SpawnWaterSplash(struct Actor *actor, s32 kind);
s32 Field_CheckConfiguredKeys(void);

void TileMap_DecodeStagedBuffer(s32 size);
void TileMap_ConvertBuffer(void);

void MapGrid_UpdateLayerScroll(void);

void WorldMap_UpdateView(void);
void WorldMap_ApplyAffinePage(void);

void MapGrid_SetCameraCenter(FIXED x, FIXED y);
u32 Map_ReadEditU16(u8 **cursor);
void Map_ApplyFlagEdits(s32 flag, s32 attributes);

s32 MapGrid_IsWallAt(s32 *position);

void Actor_SetAnimationSpeed(struct Actor *actor, s32 value);

void ObjectTable_Snapshot(void);

void Actor_ResetTargetsAndVelocity(u32 objectId);

void Actor_SetFirstSpriteAnimation(struct Actor *actor, s32 index);

void Scene_ChooseMusic(void);
void WorldMap_MarkPlaceVisited(s32 id, s32 variant);
void WorldMap_SetReturnLeavingScene(s32 scene, s32 entrance);

#endif
