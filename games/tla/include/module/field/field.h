#ifndef ALCHEMY_MODULE_FIELD_FIELD_H
#define ALCHEMY_MODULE_FIELD_FIELD_H

/// @file
/// What games/tla/src/field/ declares for the files that use it, written once.

#include "types.h"
#include "bg_scroll.h"
#include "field/effect_step.h"
#include "field/sprite.h"
#include "module/field/event_commands.h"
#include "system/sprite_metadata.h"
#include "tla_edition.h"

struct AnimationObject;
struct RenderInput;
struct ScriptOperands;
struct SpritePlacement;
struct TouchOwner;
struct TriggerView;

typedef void (*DrawFn)(const void *src, void *dst, s32 param);

typedef void (*UploadFn)(const void *src, u32 w, u32 h, void *vram);

typedef void (*OutlineFn)(u32 w, u32 h, void *pixels, u32 edge, u32 fill);

/// A command handler takes the object and returns nonzero to go on with
/// the script this frame, zero to stop. The handlers take different views
/// of the object, so the type leaves the parameters unspecified.
typedef s32 (*ScriptCommandFn)();

typedef void (*TargetHook)(struct Actor *, s32 *, s32 *);

/// A sprite's attribute words. They are volatile, as LuckyWheels_Update keeps
/// its OAM shadow in both games; as plain words, the two offset
/// counters, the multiply on each pass and the stack slots all differ.
struct ReelSprite {
  volatile u32 attr01;
  volatile u32 attr2;
};

// The casino's four-dice game, built
// like LuckyDice_Run (four dice in a round dome, Slots_DomePicture): A throws
// for a bet of ten coins a level. A pair, two pairs, a triple or four of a kind
// (Func_081b834c, banners Slots_OnePairBanner to Slots_PerfectBanner) pays 1,
// 2, 3 or 8 times the bet; a win offers a double-up round (state 5,
// Slots_DoubleUpBanner and Slots_CashOutBanner): guess whether the next total
// is higher or lower than this one (state 1, Slots_HigherBanner and
// Slots_LowerBanner) for twice the winnings, up to five rounds, the last the
// final game; a miss loses everything. Winnings are counted into the coins a
// coin a frame (state 4). The dice bounce off the dome's round wall, settle to
// a face and are drawn back to front with their shadows; palettes of the
// banners pulse with Func_081b810c.
struct DiceParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 variant;
};

struct DiceWork {
  struct DiceParticle particles[4];
};

struct DiceOam {
  volatile u32 attr01;
  volatile u32 attr2;
};

struct DiceSprites {
  u8 filler[196];
  struct DiceOam oam[128];
  struct RenderInput *coinsWindow;
  struct RenderInput *betWindow;
  struct RenderInput *helpWindow;
};

struct DiceCamera {
  s32 eye[3];
  s32 focus[3];
};

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

struct TouchWork {
  u8 unk00[16];
  struct TouchOwner *owner;
  u8 unk14[4];
  s16 argument0;
  s16 argument1;
  u8 unk1c[2];
  s16 kind;
};

struct ScriptChannel {
  u8 unk00[2];
  u16 unk02;
  u8 unk04;
  u8 unk05[11];
  u8 *script;
  u8 position;
  u8 unk15[2];
  u8 unk17;
};

struct ScriptPlayer {
  u8 unk00[22];
  u8 unk16;
  u8 unk17;
  u8 unk18;
  u8 unk19;
  u8 unk1a;
  u8 channelCount;
  u8 unk1c[12];
  struct ScriptChannel *channels[1];
};

struct ObjectSystemColours {
  u16 used;
  u8 unk02[4];
  u8 edge;
  u8 fill;
};

// A party member's unit as the parts read it.
struct FieldUnit {
  u8 unk00[20];
  s16 hpGauge;
  s16 ppGauge;
  u8 unk18[28];
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
};

struct LuckyDiceSprites {
  u8 filler[196];
  struct DiceOam oam[128];
  struct RenderInput *coinsWindow;
  struct RenderInput *winningsWindow;
  struct RenderInput *helpWindow;
};

struct BattleFxTransferWork {
  u8 unk0000[0x7780];
  s32 transferMode;
  s32 transferValue;
  u8 unk_7788[0x78e4 - 0x7788];
  s32 vblanksSinceTransfer;
  s32 transferPending;
};

struct Reel {
  s32 position;
  u8 symbols[21];
  u8 held;
  s8 stop;
};

struct ReelEffectWork {
  struct EffectStep particles[REEL_TITLE_LETTERS];
  u8 unkAfterParticles[0x700 - REEL_TITLE_LETTERS * sizeof(struct EffectStep)];
  u16 bgShade[0x100];
  u16 objShade[0x100];
  u8 unk0b00[0x7780 - 0xb00];
  s32 transferMode;
  s32 transferValue;
  u8 unk7788[4];
  s32 frame;
  u8 unk7790[0x78e8 - 0x7790];
  s32 transferPending;
};

struct ReelSave {
  u8 unk000[0x140];
  s8 wonPrizes[16];
};

/// One word of the table at Ram_MapBlocks, 32 words to a row.
struct Data02020000 {
  u32 unk0 : 24;
  /// Passed to Func_0802d088; region / 10 picks the set. The Broken Seal's
  /// WorldMapCell keeps the encounter region at these bits.
  u32 region : 6;
  u32 unk1e : 2;
};

struct MapTileWindow {
  s32 *origin;
  u8 unk004[0x11c];
  const u32 *resources;
  u8 unk124[0x24];
  u16 tileIds[32 * 32];
  u8 unk948[0x10];
  s32 reloadSteps;
};

/// A map cell's terrain kind, as The Broken Seal's enum TerrainKind: the low
/// four bits of its shape's first byte, which pick its height function. This
/// game has no bends along an axis; its sample tables take their kinds.
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
  TERRAIN_KIND_COUNT = 16
};

struct ObjectSprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affineMode : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 palette256 : 1;
  u16 shape : 2;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[6];
  u8 vramBlock;
  u8 unk11;
  u16 rotation;
  u8 unk14[2];
  s8 offsetX;
  s8 offsetY;
  u8 frame;
  u8 frameTimer;
  u8 nextFrame;
  u8 unk1b;
  u8 unk1c[0xc];
  void *record;
};

struct EventActors {
  u8 unk00[20];
  void *actors[64];
};

struct ActorDescriptor {
  s16 action;
  s16 scene;
  u8 unk04[4];
  s32 x;
  s32 y;
  s32 z;
};

struct MapActor {
  s16 id;
  s16 event;
};

struct ContextAnimation {
  u8 unk00[0x1a];
  u8 flags;
};

struct ContextEffect {
  u8 unk00[5];
  u8 param;
};

struct LinkAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

struct LinkEnds {
  s16 first;
  s16 second;
  s16 unk04;
  s16 layer;
};

struct LinkEntry {
  u8 unk00[12];
  struct LinkEnds ends;
  u8 unk14[12];
};

struct WobbleEvent {
  u8 unk000[362];
  u16 cue;
  u8 unk16c[24];
  s16 counter;
};

struct LineNode {
  struct LineNode *next;
  s32 x;
  s32 y;
  s32 z;
  u8 kind;
  u8 slot;
};

struct LineSlot {
  s16 first;
  s16 second;
  s32 direction;
  s32 amplitude;
  u8 unk0c[4];
  s32 angle;
  s32 active;
  struct LineNode *head;
};

struct LineWork {
  u8 unk00[12];
  struct LineSlot slots[8];
};

struct TileEffectWork {
  s32 vramEntry;
  u8 unk04[0x10e0];
};

struct ChainLink {
  struct ChainLink *next;
  s32 x;
  s32 y;
  s32 z;
  u16 angle;
  s8 kind; ///< 0 or 1: not drawn; else its picture, from 2
  u8 unk13;
  struct AnimationSpritePart sprite;
};

struct ChainObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x3c];
  struct {
    u8 unk00[0x28];
    s16 *entries[4];
  } *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
};

struct Unk_080db0b0Sprite {
  u8 unk00[18];
  u16 unk12;
};

struct CompositeAnimation {
  u8 unk00[0x11];
  u8 displayFlags;
  u8 unk12[7];
  u8 dirty;
  u8 unk1a;
  u8 count;
  u8 unk1c[0x0c];
  struct AnimationEntry *entries[4];
};

/// unk26a from main_routine_work.h.
struct PartyPauseView {
  u8 unk000[0x26a];
  s16 pausedObject;
};

struct DepthPart {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 unk09_4 : 4;
};

struct DepthMapBase {
  u8 unk00[4];
  s32 base;
};

struct DepthScroll {
  s32 unk00;
  s32 offset;
};

struct DepthMap {
  struct DepthMapBase *base;
  u8 unk04[0xe0];
  struct DepthScroll scroll;
};

struct DepthSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 hidden : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 layer : 4;
  u8 unk0a[2];
  s32 x;
  s32 y;
  u8 unk14[4];
  s32 scale;
  u8 unk1c[2];
  u16 frame;
  u8 unk20[8];
};

struct DepthScene {
  u16 mode;
  u8 unk02[2];
  s32 x;
  u8 unk08[8];
  struct Actor *mainObject;
  u8 unk14[0x7b4];
  struct DepthSprite sprite;
  s16 vramSlot;
  s16 active;
};

struct MoveScene {
  u16 mode;
  u8 unk02[14];
  struct Actor *mainObject;
};

struct SpawnScene {
  u8 unk00[0x10];
  struct Actor *mainObject;
};

/// One rising particle: its sprite, world position, drift and frames left.
struct RisingParticle {
  /// The sprite: set as three words by the start, drawn field by field.
  union {
    u32 words[3];
    struct {
      u8 unk00[4];
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
      u8 unk0a[2];
    } attributes;
  } sprite;
  s32 posX;
  s32 posY;
  s32 posZ;
  s32 drift;
  u16 timer;
  u8 unk1e[2];
};

/// Heap slot 0x74 while the rising particles run.
struct RisingParticleWork {
  s32 vramEntry;
  s32 tile;
  struct RisingParticle particles[64];
};

struct MoteParticle {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 attr3[2];
  s32 posX;
  s32 posY;
  s32 posZ;
  s32 delay;
  u16 timer;
};

struct EffectOwner {
  u8 unk00[8];
  struct EffectVector position;
  u8 unk14[0x3c];
  struct AnimationObject *sprite;
};

struct OrbitPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct DescendingOrbitObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[80];
  u16 timer;
  s16 angle;
};

struct SpreadPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct CaptureObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 baseY;
  s32 scaleX;
  s32 scaleY;
  u8 unk20[8];
  s32 gravity;
  u8 unk2c[4];
  s32 speed;
  u8 unk34[33];
  u8 mode;
  u8 unk56[5];
  u8 visible;
  u8 unk5c[2];
  u16 lifetime;
  u8 unk60[4];
  u16 timer;
  u16 angle;
  u8 unk68[4];
  void *callback;
};

struct LinkedCaptureObject {
  u8 padding000[8];
  s32 x;
  s32 y;
  s32 z;
  u8 padding014[84];
  struct LinkedCaptureObject *target;
};

struct ContextSprite {
  u8 unk00[0xc];
  s32 scale;
  u8 unk10[0xa];
  u8 flags;
  u8 count;
  u8 unk1c[0xc];
  void *entries[4];
};

struct ContextEffectEntry {
  u8 unk00[5];
  u8 param;
};

struct DmaFxPage {
  u32 lines[0x1e0];
};

struct DmaFxPages {
  struct DmaFxPage pages[2];
  u8 page;
};

struct DisplayScrollWork {
  struct BgScroll rows[2][160][3];
  u8 page;
  u8 mode;
  u16 frame;
  u8 unkf04[4];
  s32 freqX;
  s32 freqY;
  s32 stepX;
  s32 stepY;
  s32 ampX;
  s32 ampY;
};

struct DmaFxWork {
  u8 unk000[0xf01];
  u8 kind;
  u8 unkf02[6];
  s32 unkf08;
  s32 unkf0c;
  s32 unkf10;
  s32 unkf14;
  s32 unkf18;
  s32 unkf1c;
};

struct DjinniObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[4];
  s32 scaleX;
  s32 scaleY;
  u8 unk20[68];
  u16 timer;
  u8 unk66[6];
  void *update;
};

struct CaptureSprite {
  u8 unk00[40];
  s16 *resource;
};

struct CaptureResource {
  u8 unk00[16];
  u8 id;
};

struct ChainTransform {
  u32 scaleX : 16;
  u32 scaleY : 16;
  u32 angle : 16;
  u32 unk06 : 16;
};

struct ChainNode {
  struct ChainNode *next;
  s32 x;
  s32 y;
  s32 z;
  u16 angle;
  s8 used;
  u8 unk13;
  struct AnimationSpritePart sprite;
};

struct ChainSlot {
  s16 id;
  s16 value;
  s16 depth;
  s16 priority;
  s16 flag;
  s16 state;
  u8 unk0c[16];
  struct ChainNode *head;
};

struct SwingSlot {
  s16 id;
  s16 value;
  u8 unk04[8];
  s32 amplitude;
  u8 unk10[4];
  s32 phase;
  s32 active;
  u8 unk1c[4];
};

struct SwingEvent {
  u8 unk000[362];
  s16 prompt;
  u8 unk16c[24];
  s16 timer;
};

struct RopeNode {
  struct RopeNode *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  u8 kind;
  u8 slot;
};

struct RopeSlot {
  s16 id;
  s16 value;
  s16 count;
  u8 unk06[2];
  s16 flag;
  s16 state;
  s32 amplitude;
  u8 unk10[4];
  s32 phase;
  s32 active;
  struct RopeNode *head;
};

struct RopeWork {
  u8 unk000[12];
  struct RopeSlot slots[8];
};

union MoteWord {
  u32 value;
  void *link;
};

/// One floating mote: its sprite's attribute words and its world position.
struct Mote {
  union MoteWord link;
  u32 attr01;
  u32 attr2;
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 pad18[4];
  u16 timer;
  u8 pad1e[2];
};

/// One of Weather_UpdateRain's particles: its sprite's attribute words, its
/// world position and its frames left.
struct SparkParticle {
  u32 sprite[3];
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 unk18[4];
  u16 timer;
  u8 unk1e[2];
};

/// Heap slot 0x74 while the particles run.
struct SparkParticleWork {
  s32 vramEntry;
  s32 tile;
  struct SparkParticle particles[64];
};

struct Sparkle {
  u8 padding00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 matrix : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 padding0a[2];
  s32 posX;
  s32 height;
  s32 posZ;
  s32 delay;
  u16 timer;
  u8 padding1e[2];
};

union DustWord {
  u32 value;
  void *link;
};

struct LinkedEffectVisual {
  u8 unk00[9];
  u8 flags;
  u8 unk0a[16];
  u8 value26;
};

struct LinkedEffectObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[60];
  struct LinkedEffectVisual *visual;
  u8 value54;
  u8 value55;
  u8 unk56[14];
  u16 counter;
  u16 resourceId;
  struct LinkedEffectObject *resource;
  void (*callback)(void);
};

struct ObjectFlags23 {
  u8 unk00[0x23];
  u8 flags23;
};

struct ObjectLayerAnimation {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
  u8 unk0a[27];
  u8 unused25 : 2;
  u8 shadowLayer : 2;
};

struct ObjectAnimationEntry {
  s16 animId;
};

struct ObjectAnimation {
  u8 unk00[0x28];
  struct ObjectAnimationEntry *entries[1];
};

struct JumpSprite {
  u8 unk00[26];
  u8 flags;
};

struct JumpEventWork {
  u8 unk000[0x19c];
  s32 walkingDistanceScale;
  s32 walkingDistance;
};

struct ClimbSprite {
  u8 unk00[26];
  u8 flags;
};

struct Work9cItemPosition {
  u8 unk00[4];
  s32 x;
  s32 y;
  s32 z;
};

struct GrabSave {
  u8 unk00[98];
  u8 priority;
};

struct EventWorkGrab {
  u8 unk000[388];
  s16 grabState;
};

struct BattleEffectVisual {
  u8 unk00[9];
  u8 flags;
  u8 unk0a[16];
  u8 value26;
};

struct BattleEffectResource {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[60];
  struct BattleEffectVisual *visual;
};

struct BattleEffectWatchedObject {
  u8 unk00[80];
  struct BattleEffectVisual *visual;
  u8 value54;
  u8 value55;
  u8 unk56[14];
  u16 counter;
  u16 resourceId;
  struct BattleEffectResource *resource;
  void (*callback)(void);
};

struct AnimationGroup {
  u8 unk00[0x16];
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 unk19[2];
  u8 count;
  u8 unk1c[0x0c];
  struct AnimationEntry *entries[4];
};

extern const u8 gData_081ba30c[];
s32 Graphics_ScaleRgb555BufferB(u16 *source, u16 *destination, s32 scale,
                                s32 count);
void Func_081b810c(u16 *source, u16 *destination, u32 count, s32 angle);
s32 Func_081b834c(s32 *dice);
s32 Func_081b8180(s32 sprite, s32 x, s32 y, s32 value, s32 palette, s32 digits);
s32 Func_081b8274(s32 sprite, s32 x, s32 y, s32 value, s32 palette, s32 digits);

void Func_080caa4c(s32 table);

void Portrait_CloseWindowFar(s32 sprite);

extern const s32 gFieldEffect_HeavyImpactScript[];

extern const s32 gFieldEffect_FragmentScript[];

s32 MapGrid_CheckCollisionFar(struct Actor *object, s32 *position);

s32 Field_CheckSpheresOverlapFar(s32 *a, s32 radiusA, s32 *b, s32 radiusB);

void MapGrid_SetScreenShakeFar(s32 x, s32 y, s32 decay);

s32 Func_080cda84(s32 id, struct Actor *actor, s32 *position);

u32 MapGrid_GetTerrainKindFar(s32 terrain, s32 x, s32 z);

void WorldMap_Freeze(void);
void WorldMap_ReloadPendingAreaGraphics(void);

void Event_NoOp(void);

void Actor_ApplyIndexedChildValue(struct Actor *object);

extern u32 gScanlineAngle;

extern struct AnimationMetadata gResource_Data012[];

extern u8 Render_DecodePictureCode[];

extern u8 Render_DecodePictureCopyEnd[];

extern u8 Render_DecodePictureBlockSize[];

extern const u8 Grid_DrawOutlineCode[];

extern u8 Grid_DrawOutlineCodeSize[];

extern const u8 Data_080209a8[];

void SpriteAnimation_Start(struct AnimationEntry *work, s32 no);

u8 *Decompress_Lz(const u8 *source, u8 *destination);

s32 Party_CheckItemLimit(s32 item);

s32 WorldMap_GetCollisionFar(s32 *position);

extern u8 gData_0202e000[];

void Party_ClearUnusableShortcuts(void);

void BattleBackground_SelectForMap(void);

s32 Func_080ca9cc(s32 id, s32 *position);
void ObjectTable_Clear(void);

void Party_AddPp(s32 amount, s32 unused);
void Party_AddHp(s32 amount, s32 scaled);
s32 Party_ApplyPoisonDamage(void);
void *WorldMap_Unfreeze(void);

s32 Actor_GetTriggerCellAheadOfCurrent(void);
u32 Stream_ReadU16(u8 **cursor);
s16 Stream_ReadS16(u8 **cursor);
void BattleMap_ApplySceneEntranceView(void);

void PaletteEditor_Run(void);

struct TriggerView *Scene_FindTrigger(s32 kind, s32 target);
s32 Scene_TalkToActor(s32 id);
s32 Scene_RunRaisedEvent(s32 id);
s32 Scene_RunStepTriggerOrExit(s32 value);
s32 Field_RunPushCell(s32 keys);
s32 Field_ExamineCell(s32 id);

s32 Func_080cdc74(void);
void Func_080cdd80(void);

s32 Field_UsePsynergy(u32 code);

s16 *Func_080ceba8(void);
void Field_PlaceTreasureSpots(void);
s32 Actor_GetSpriteShape(u32 objectId);
void ObjectEvent_SetMode5WithCue(u32 objectId);
void ObjectEvent_Spring(u32 objectId);
void ObjectEvent_SetMode4WithCue(u32 objectId);
void ObjectEvent_SetMode2(u32 objectId);
void ObjectEvent_SetMode6WithCue(u32 objectId);
struct Actor *ObjectEvent_GetObject(u32 objectId);
void ObjectEvent_Remove(u32 objectId);

void Camera_ResetViewDefaults(void);
s32 LuckyDice_ScaleRgb555Buffer(u16 *source, u16 *destination, s32 scale,
                                s32 count);
extern char ResourceId_FarCall00C;

void ColorBuffer_HalveNonzero(u8 *source, u8 *destination, s32 size);
void ColorBuffer_ScaleNonzeroThreeQuarters(u8 *source, u8 *destination,
                                           s32 size);

extern u8 Data_0200024c[];

extern const u16 gLuckyWheels_SparkCellOffsets[];

s32 Resource_FarCall00C(s32 slot, s32 a, s32 b, s32 c, s32 d);

extern char ResourceId_SlotsMachinePicture;
extern char ResourceId_SlotsIcons;
extern char ResourceId_SlotsJackpotBanner;

extern u8 gData_02028000[];

s32 WorldMap_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void BattleEffect_LoadBufferPaletteFar(void *palette);

void TilesetAnim_StartAll(const u16 *script);
extern char ResourceId_DefaultMapCells;

void WorldMap_UpdateCurrentTileBlock(void);

extern const u16 gData_0802f054[8];

void ActorSystem_Initialize(s32 mode);

s32 SpritePictureCache_Load(u32 slot, u32 *buffer, s32 number, u32 variant);

void SpriteViewer_SetGridColumnPriority(s32 slot, s32 value);
void SpriteViewer_SetGridColumnColorMode(s32 slot, s32 value);
void SpriteViewer_StartStaggeredAnimations(s32 slot, s32 value);
void SpriteViewer_SetGridColumnNumber(s32 slot, s32 value);
void SpriteViewer_BuildProbeRing(s32 x, s32 z, struct SpritePlacement *out);

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

s32 Func_080cec24(s32 value, s32 *out);

void MapGrid_SetCellBehaviorAtFar(s32 terrain, s32 x, s32 z, s32 value);

void SpriteAnimation_ClearFar(void *record);

void Djinn_ShowJoinedMessageFar(s32 owner, s32 element, s32 djinni);

extern const u8 gRope_Tiles[];
extern const u8 Data_080f0e00[];

s32 Field_FindNearbyPsynergyTarget(s32 self, s32 effect);

s32 MapGrid_GetCellAttributeLowNibbleFar(s32 layer, s32 x, s32 z);

extern const u8 gFieldEffect_GroundParticleTiles[];
extern const u8 gData_080f385e[];

extern u32 gData_080f0bfc[];

void BattleEffect_UpdateDescendingOrbitObject(
    struct DescendingOrbitObject *arg);

extern const s16 gLineScroll_WaveSine[];

extern const s16 gFieldEffect_GroundParticleFrames[];

extern const u8 Data_080f3901[];

extern const struct SparklePicture gFieldEffect_SparklePictures[];

extern const u8 gFieldEffect_SparkleTiles[];

void Func_080203a0(struct Actor *object, s32 kind);

s32 MapGrid_GetCellBehaviorAtFar(s32 layer, s32 x, s32 z);

s32 MapGrid_IsWallAtFar(s32 *position);

extern const u16 Data_080f0890[];

extern u32 gScanlinePitch;

extern const s16 Data_080f0b50[];

void Actor_UpdateAllMotion(void);

void Actor_UpdateAllThumb(void);
void Func_080246b8(void);

void ActorSystem_PlaceSprites(void);

extern const s32 gData_0802f098[];
extern const s32 gData_0802f0b0[];
extern const s32 gData_0802f0c8[];
extern const s32 gData_0802f0e0[];
extern const s32 gData_0802f0f8[];
extern const s32 gData_0802f110[];
extern const s32 gData_0802f128[];
extern const s32 gData_0802f140[];
extern const s32 gData_0802f158[];
extern const s32 gActor_KeyCheckScript[];
extern const s32 gActor_KeysScript[];
extern const s32 gActor_PlayerScript[];
extern const s32 gActor_WorldMapScript[];
extern const s32 gData_0802f1e8[];

s32 MapGrid_GetTerrainHeight(s32 layer, s32 x, s32 z);

s32 ActorScript_CheckOverlap(struct Actor *object, s32 *values);

void Actor_SelectAnimation(struct Actor *object, s32 argument);
struct Actor *Actor_Create(s32 kind, s32 x, s32 y, s32 z);

void Func_08026fc8(struct Actor *object, s32 kind);
s32 Field_CheckConfiguredKeys(void);

void TileMap_DecodeStagedBuffer(s32 size);
void Tilemap_ConvertBuffer(void);

void MapGrid_UpdateLayerScroll(void);

void WorldMap_UpdateView(void);
void WorldMap_ApplyAffinePage(void);

void MapGrid_SetCameraCenter(s32 x, s32 y);
u32 Map_ReadEditU16(u8 **cursor);
void Map_ApplyFlagEdits(s32 flag, s32 attributes);

s32 MapGrid_IsWallAt(s32 *position);

void Actor_SetAnimationSpeed(struct Actor *object, s32 value);

void ObjectTable_Snapshot(void);

void Actor_ResetTargetsAndVelocity(u32 objectId);

void Actor_SetFirstSpriteAnimation(struct Actor *object, s32 index);

void Scene_ChooseMusic(void);
void Func_080c9de0(s32 id, s32 variant);
void Func_080ca3f4(s32 scene, s32 entrance);

#endif
