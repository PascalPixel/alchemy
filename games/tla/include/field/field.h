#ifndef FIELD_H
#define FIELD_H

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
extern const s32 gActor_DefaultScript[];
extern const u8 gWorldMap_TerrainBehaviorTable[];
extern u8 Render_DecodePictureCodeSize[];
extern u8 gSpriteColorRemapTables[];
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);
s32 Party_CheckShortcut(u32 packed_id);
void SpriteDissolve_Step(u8 *object, u32 position);
void FieldCamera_FollowActor(s32 id, s32 flag);
void Actor_UpdateAllMotion(void);
void Actor_UpdateAllThumb(void);
void Actor_SetSpriteFlags(struct Actor *object, u32 flags);
void ObjectSystem_UpdateCamera(void);
void ActorSystem_ProjectSprites(void);
s32 Actor_FindLastActiveId(void);
void BattleMotion_ProjectPositionFar(s32, s32 *);
s32 Unit_KnowsPsynergyFar(s32 group, s32 entry);
s32 Party_ShowJoinedMessageFar(s32 event_id, s32 value);
s32 Party_ShowPairJoinedMessageFar(s32 first, s32 second);
s32 Field_CheckSpheresOverlap(FIXED *a, s32 radius_a, FIXED *b, s32 radius_b);

struct Sprite;
struct ScreenPoint;
struct Particle;
struct Actor;
struct Actor;
struct SceneEntrance;
struct Trigger;
struct ActorPlacement;
struct SceneRegion;
struct Actor;

extern u32 gDriftScene_SpawnScript[];
extern u32 gSceneExits[];
extern struct Actor gSnapshotObjects[];
extern u8 gLinkLobby_SlotColumns[];
extern s32 gLinkLobby_SlotValues[];
extern const u8 gActor_BlinkSpriteColors[4];
extern const u8 gActor_StepAngleScript[];
extern u16 gScript_MainScript[];
s32 Animation_InitializeObjects(struct Sprite *);
void Actor_SetBlendMode(struct Actor *object, u32 value);
void AnimationObject_SetActiveTimerSteps(struct Sprite *, s32);
s32 PaletteFade_SetTargetFar(s32, s32);
void FieldPsynergy_FinishMindRead(void);
void FieldPsynergy_Dispatch(void);
void FieldPsynergy_LoadResources(s32 action, s32 flags);
void FieldPsynergy_Run(void);
void BattleEffect_SelectBattleCue(s32 row, s32 column);
void FieldPsynergy_SetupActorPair(s32 actor, s32 target);
s32 PaletteFade_StartInterpolationFar(s32);
s32 BattleMotion_ProjectScaledPositionFar(s32, s32 *);
struct BattleEffectParams;
void BattleEffect_RunFlurry(struct BattleEffectParams *effect, s32 variant);
void BattleEffect_StopMainActor(void);
struct ActorPlacement;
void Event_SpawnObjectTable(struct ActorPlacement *entry, s32 slot);
void Actor_TurnPairToFaceEachOther(struct Actor *first, struct Actor *second);
void Field_StartPaletteTransition(s32 mode);
void FieldPsynergy_RunHandler(s32 kind);
void FieldPsynergy_RunExtraHandler(void);

s32 Actor_PlaySoundInViewFar(struct Actor *interpreter, s32 value);
void Actor_ReleaseAtIndex(u32 actorIndex);
void Actor_SetSquareSprite(u32 objectId);
void BattleEffect_DropObjectAtAnchor(s32 actorId);
void SuperLuckyDice_Run(void);
void GameFlag_RefreshLureCapFar(void);
void MapGrid_CenterOnFollowedPointFar(void);
s32 MapGrid_GetTerrainHeightFar(u8, s32, s32);
void Actor_RunRepeatedMotion(u32 objectId, s32 variant);
s32 Actor_IsTargetUnset(struct Actor *object);
#ifndef OWN_Actor_SetPartAttribute
void Actor_SetSpriteColor(struct Actor *object, s32 attribute);
#endif
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_StartCameraFar(void *object, void *effect);
void Entrance_EndArrivalAnimation(s32 effect);
void Exit_PrepareAnimation(s32 effect);
void Actor_CenterOnColumn(u32 objectId, s32 action, s32 z_offset);
void Actor_WalkByAndWaitById(u32 objectId, s32 x_offset, s32 z_offset);
void Actor_WalkByById(u32 objectId, s32 x_offset, s32 z_offset);
void Actor_SetDestinationById(u32 objectId, s32 x, s32 z);
void Actor_MoveToAndWaitById(u32 objectId, s32 x, s32 z);
void Actor_SetPositionWithTerrain(u32 objectId, FIXED x, FIXED y, FIXED z);
void Actor_StartRepeatedMotion(u32 objectId, s32 variant);
void ActorSystem_Configure(s32 mode);
s32 Party_AddMemberFar(s32 event_id);
void Party_RemoveMemberFar(s32 owner);
void Render_ApplyProjectedPlacement(struct Sprite *sprite, s32 *position,
                                    s32 *scale, s32 facing);
s32 Render_ProjectPoint(const void *point, s32 *screen);
void Scene_SetArrivalFlags(s32 unused);
s32 Message_OpenWindowFar(s32, s32, s32, s32);
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

extern u8 gWorldMapPackedTiles[];
extern u8 gWorldMapPackedPictures[];
extern u8 gWorldMapPalettes[];

struct Sprite;
struct RenderInput;
struct ScriptOperands;
struct SpritePlacement;
struct TriggerView;

typedef void (*DrawFn)(const void *src, void *dst, s32 param);

typedef void (*UploadFn)(const void *src, u32 w, u32 h, void *vram);

typedef void (*OutlineFn)(u32 w, u32 h, void *pixels, u32 edge, u32 fill);

/// A command handler takes the object and returns nonzero to go on with
/// the script this frame, zero to stop. The handlers take different views
/// of the object, so the type leaves the parameters unspecified.
typedef s32 (*ScriptCommandFn)();

typedef void (*TargetHook)(struct Actor *, s32 *, s32 *);

/// One sprite's two OAM words.
// volatile: the words are built in place for the DMA to copy to OAM.
struct OamWords {
  volatile u32 attr01; ///< attr0 | attr1 << 16
  volatile u32 attr2;  ///< attr2 | affine value << 16
};

/// A sprite's place in its attr01 word: x in attribute 1's nine bits and y in
/// attribute 0's eight, each wrapped as the screen wraps them.
#define OBJ_POSITION(x, y) (OBJ_X((x) & 0x1ff) | ((y) & 0xff))
/// An affine sprite's matrix, of the 32, in its attr01 word.
#define OBJ_AFFINE_MATRIX(n) ((n) << 25)

// The casino's four-dice game, built
// like LuckyDice_Run (four dice in a round dome, Slots_DomePicture): A throws
// for a bet of ten coins a level. A pair, two pairs, a triple or four of a kind
// (SuperLuckyDice_ScoreDice, banners Slots_OnePairBanner to
// Slots_PerfectBanner) pays 1, 2, 3 or 8 times the bet; a win offers a
// double-up round (state 5, Slots_DoubleUpBanner and Slots_CashOutBanner):
// guess whether the next total is higher or lower than this one (state 1,
// Slots_HigherBanner and Slots_LowerBanner) for twice the winnings, up to five
// rounds, the last the final game; a miss loses everything. Winnings are
// counted into the coins a coin a frame (state 4). The dice bounce off the
// dome's round wall, settle to a face and are drawn back to front with their
// shadows; palettes of the banners pulse with SuperLuckyDice_BrightenColors.

/// A die of the dice game: its place and velocity, 16.16, and its variant.
struct DiceParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
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
  struct RenderInput *coinsWindow;
  struct RenderInput *betWindow;
  struct RenderInput *helpWindow;
};

/// The dice games' camera: the eye and the point it looks at, 16.16.
struct DiceCamera {
  s32 eye[3];
  s32 focus[3];
};

struct ObjectSystemColours {
  u16 used;
  u8 unusedAfterUsed[4]; ///< no code names it
  u8 edge;
  u8 fill;
};

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
  struct RenderInput *coinsWindow;
  struct RenderInput *winningsWindow;
  struct RenderInput *helpWindow;
};

struct BattleFxTransferWork {
  u8 unused[0x7780]; ///< no code names it
  s32 transferMode;
  s32 transferValue;
  u8 unusedAfterTransferValue[0x78e4 - 0x7788]; ///< no code names it
  s32 vblanksSinceTransfer;
  s32 transferPending;
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
  u8 unusedAfterParticles[0x700 -
                          LUCKY_WHEELS_TITLE_LETTERS * sizeof(struct Particle)];
  u16 bgShade[0x100];
  u16 objShade[0x100];
  u8 unusedAfterObjShade[0x7780 - 0xb00]; ///< no code names it
  s32 transferMode;
  s32 transferValue;
  u8 unusedAfterTransferValue[4]; ///< no code names it
  s32 frame;
  u8 unusedAfterFrame[0x78e8 - 0x7790]; ///< no code names it
  s32 transferPending;
};

/// The save as Lucky Wheels reads it: the prizes won.
struct ReelSave {
  u8 unused[0x140]; ///< no code names it
  s8 wonPrizes[16];
};

/// One 32-pixel cell of the World Map, as The Broken Seal's: its tile and its
/// encounter region; Ram_MapBlocks holds 32 by 32 of them.
struct WorldMapCell {
  u32 tile : 24; ///< low halfword: the cell picture
  /// Passed to WorldMap_UpdateRegionTint; region / 10 picks the graphics set.
  u32 region : 6;
  u32 unusedHigh : 2; ///< no code names them
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

/// The tightropes' work, heap slot 160.
struct TightropeWork {
  struct Actor
      *recoilFrom; ///< the posts Tightrope_Untie gives Tightrope_Recoil
  struct Actor *recoilTo;
  s32 vramEntry; ///< the nodes' picture
  struct TightropeSlot slots[ROPE_SLOT_COUNT];
  struct TightropeNode nodes[ROPE_NODE_COUNT];
};

#define TIGHTROPE_WORK ((struct TightropeWork *)Ram_HeapSlots->tightrope)

/// unk26a from main_routine_work.h.
struct PartyPauseView {
  u8 unused[0x26a]; ///< no code names it
  s16 pausedObject;
};

/// The point the map view follows (MapWork.origin), as the depth sort reads it.
struct DepthOrigin {
  s32 x;
  FIXED y; ///< its height, 16.16 fixed-point pixels
};

/// The map view's place (MapWork.viewX and viewZ), read as a pair.
struct DepthScroll {
  s32 x;
  s32 z;
};

/// MapWork as BattleEffect_GetObjectDepth reads it: its followed point and its
/// view, through a pointer to the pair.
struct DepthMap {
  struct DepthOrigin *origin;
  u8 unusedAfterOrigin[0xe0]; ///< no code names it
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

/// Heap slot 0x74 while the rising particles run.
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
  u8 unusedAfterPosition[0x3c]; ///< no code names it
  struct Sprite *sprite;
};

struct OrbitPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct LineScrollWork {
  struct BgScroll rows[2][160][3];
  u8 page;
  u8 mode;
  u16 frame;
  u8 unusedAfterFrame[4]; ///< no code names it
  s32 freqX;
  s32 freqY;
  s32 stepX;
  s32 stepY;
  s32 ampX;
  s32 ampY;
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

/// The ropes' work, heap slot 156.
struct RopeWork {
  struct Actor *recoilFrom; ///< the posts Rope_Untie gives Rope_Recoil
  struct Actor *recoilTo;
  s32 vramEntry; ///< the nodes' picture
  struct RopeSlot slots[ROPE_SLOT_COUNT];
  struct RopeNode nodes[ROPE_NODE_COUNT];
};

#define ROPE_WORK ((struct RopeWork *)Ram_HeapSlots->rope)

struct AnimationGroup {
  u8 unused[0x16]; ///< no code names it
  s8 offsetX;
  s8 offsetY;
  u8 lastNo;
  u8 unusedAfterLastNo[2]; ///< no code names it
  u8 count;
  u8 unusedAfterCount[0x0c]; ///< no code names it
  struct SpriteAnimation *entries[4];
};

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

extern const s32 gFieldEffect_FragmentScript[];

s32 MapGrid_CheckCollisionFar(struct Actor *object, s32 *position);

s32 Field_CheckSpheresOverlapFar(s32 *a, s32 radiusA, s32 *b, s32 radiusB);

void MapGrid_SetScreenShakeFar(s32 x, s32 y, s32 decay);

s32 Field_MeasurePsynergyHeight(s32 id, struct Actor *actor, s32 *position);

u32 MapGrid_GetTerrainKindFar(s32 terrain, s32 x, s32 z);

void WorldMap_Freeze(void);
void WorldMap_ReloadPendingAreaGraphics(void);

void Event_NoOp(void);

void Actor_UpdateBlink(struct Actor *object);

extern u32 gScanlineAngle;

extern struct AnimationMetadata gSprite_AnimationMetadata[];

extern u8 Render_DecodePictureCode[];

extern u8 Render_DecodePictureCopyEnd[];

extern u8 Render_DecodePictureBlockSize[];

extern const u8 Grid_DrawOutlineCode[];

extern u8 Grid_DrawOutlineCodeSize[];

extern const u8 gSpriteColorShadeTables[];

void SpriteAnimation_Start(struct SpriteAnimation *work, s32 no);

u8 *Decompress_Lz(const u8 *source, u8 *destination);

s32 Party_CheckItemLimit(s32 item);

s32 WorldMap_GetCollisionFar(s32 *position);

extern u8 gMapScene[];

void Party_ClearUnusableShortcuts(void);

void BattleBackground_SelectForMap(void);

s32 ScenePoint_GetPosition(s32 id, FIXED *position);
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

void ColorBuffer_HalveNonzero(u8 *source, u8 *destination, s32 size);
void ColorBuffer_ScaleNonzeroThreeQuarters(u8 *source, u8 *destination,
                                           s32 size);

extern u8 Data_0200024c[];

extern const u16 gLuckyWheels_SparkCellOffsets[];

s32 BattleEffect_BuildBlitterFar(s32 slot, s32 widthShift, s32 heightShift,
                                 s32 flags, u32 mode);

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

extern const u8 gRope_Tiles[];
extern const u8 gHangingVine_ChainBottomPicture[];

s32 Field_FindNearbyPsynergyTarget(s32 self, s32 effect);

s32 MapGrid_GetCellAttributeLowNibbleFar(s32 layer, s32 x, s32 z);

extern const u8 gFieldEffect_GroundParticleTiles[];
extern const u8 gFieldEffect_MoteTiles[];

extern u32 gBattleEffect_PulseScales[];

extern const s16 gLineScroll_WaveSine[];

extern const s16 gFieldEffect_GroundParticleFrames[];

extern const u8 gFieldEffect_StreakTiles[];

extern const struct SparklePicture gFieldEffect_SparklePictures[];

extern const u8 gFieldEffect_SparkleTiles[];

void Actor_SpawnWaterSplashFar(struct Actor *object, s32 kind);

s32 MapGrid_GetCellBehaviorAtFar(s32 layer, s32 x, s32 z);

s32 MapGrid_IsWallAtFar(s32 *position);

extern const u16 gRope_KeyHeadings[];
extern const u16 gFieldEffect_CyclePatternWords[];

extern u32 gScanlinePitch;

extern const s16 gGroundParticle_Offsets[];

void Actor_UpdateAllMotion(void);

void Actor_UpdateAllThumb(void);
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

s32 ActorScript_CheckOverlap(struct Actor *object, s32 *values);

void Actor_SelectAnimation(struct Actor *object, s32 argument);
struct Actor *Actor_Create(s32 kind, FIXED x, FIXED y, FIXED z);

void Actor_SpawnWaterSplash(struct Actor *object, s32 kind);
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

void Actor_SetAnimationSpeed(struct Actor *object, s32 value);

void ObjectTable_Snapshot(void);

void Actor_ResetTargetsAndVelocity(u32 objectId);

void Actor_SetFirstSpriteAnimation(struct Actor *object, s32 index);

void Scene_ChooseMusic(void);
void WorldMap_MarkPlaceVisited(s32 id, s32 variant);
void WorldMap_SetReturnLeavingScene(s32 scene, s32 entrance);

#endif
