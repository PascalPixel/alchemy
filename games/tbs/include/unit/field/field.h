#ifndef ALCHEMY_UNIT_FIELD_FIELD_H
#define ALCHEMY_UNIT_FIELD_FIELD_H

#include "render_placement.h"

#include "types.h"

struct SpriteAnim;
struct Sprite;
struct Object;
struct FieldPosition;
struct WorldMapAffinePair;
struct WorldMapScanline;
struct ScriptOperands;
struct WorldPosition;

/// How the object system runs (ObjectSystem_Initialize): which camera follows
/// the objects and which update moves them.
enum ObjectSystemMode {
  /// A town, a dungeon or the sprite viewer: the scrolling camera.
  OBJECT_SYSTEM_LAYOUT = 2,
  /// The world map: the fixed camera, which projects 3D points onto the
  /// screen.
  OBJECT_SYSTEM_WORLD_MAP = 3,
  /// A battle: the fixed camera, and the battle's own motion update.
  OBJECT_SYSTEM_BATTLE = 4
};

/// SpriteAnim.kind: the facing table a sprite anim's frames follow
/// (Sprite_ComposeFrame, facing.c). Each cuts the facing angle into
/// sectors and gives each sector a frame and whether it is mirrored.
enum AnimationFacingKind {
  /// The script's own frame whatever the facing; zero also marks a free
  /// sprite anim.
  ANIM_FACING_NONE = 0,
  ANIM_FACING_ONE_FRAME = 1,    ///< 8 sectors: one frame, mirrored to the left
  ANIM_FACING_FRONT_BACK = 2,   ///< 8 sectors: a front and a back frame
  ANIM_FACING_THREE_FRAMES = 3, ///< 16 sectors: front, side and back
  ANIM_FACING_FOUR_FRAMES = 4,  ///< 64 sectors
  ANIM_FACING_FIVE_FRAMES = 5,  ///< 16 sectors
  ANIM_FACING_SIX_FRAMES = 6,   ///< 64 sectors
  /// 8 sectors centred on the compass directions, a frame for each.
  ANIM_FACING_EIGHT_FRAMES = 8,
  /// ANIM_FACING_FRONT_BACK's table for a sprite drawn in two parts, the
  /// second the next sprite id (the sprite viewer).
  ANIM_FACING_TWO_PART = 20,
  /// As ANIM_FACING_FRONT_BACK, with the front frame mirrored the other
  /// way.
  ANIM_FACING_FRONT_BACK_SWAPPED = 22,
  // XXX: likely an older eight-direction kind, because the viewer still lists
  // it; a sprite of this kind would settle it.
  /// Ringed as eight facings by the sprite viewer, but with no facing table:
  /// Sprite_ComposeFrame draws it as ANIM_FACING_NONE.
  ANIM_FACING_EIGHT_NO_TABLE = 44,
  /// As ANIM_FACING_EIGHT_FRAMES, with three of the frames mirrored.
  ANIM_FACING_EIGHT_FRAMES_MIRRORED = 88
};

/// SpriteAnim.mode, from the metadata's frameCodec: how a sprite anim's frames
/// are stored. Any other value is raw, for the frame decoder.
enum SpriteFrameCodec {
  SPRITE_FRAME_RAW = 0,
  SPRITE_FRAME_TAGGED_LZ = 1, ///< Decompress_TaggedLz's format
  SPRITE_FRAME_LZ = 3         ///< LZ-compressed
};

/// Sprite.flags.
enum SpriteFlag {
  SPRITE_FLAG_SHADOW = 1 << 0, ///< its shadow is drawn; set when created
  /// Drawn as an outline, in ObjectSystemWork's edge and fill colours.
  SPRITE_FLAG_OUTLINED = 1 << 1
};

/// Object.spriteStorageKind, its low four bits: what the object's sprite
/// pointer holds.
enum ObjectSpriteKind {
  OBJECT_SPRITE_NONE = 0, ///< nothing drawn
  OBJECT_SPRITE_ONE = 1,  ///< one sprite
  OBJECT_SPRITE_GROUP = 2 ///< a list of up to four sprites, empty ones null
};

/// Object_CreateField's id / 4096: what sprites the object is made with.
enum ObjectCreateKind {
  OBJECT_CREATE_ONE_SPRITE = 0, ///< the id's sprite (OBJECT_SPRITE_ONE)
  /// The sprites of the id and id + 1 (OBJECT_SPRITE_GROUP).
  OBJECT_CREATE_GROUP = 2,
  /// No sprite: any kind but the two above, and the one the camera object
  /// takes.
  OBJECT_CREATE_NO_SPRITE = 8
};

/// The world map's collision values (WorldMap_GetCollision), 1 to 15; each
/// indexes a row of gWorldMap_BattleEnvironments. Values not named here
/// are walkable or not only by their range.
enum WorldMapCollision {
  /// The party walks only on WORLD_MAP_COLLISION_WALKABLE_FIRST to
  /// WORLD_MAP_COLLISION_WALKABLE_LAST (WorldMap_IsWalkable).
  WORLD_MAP_COLLISION_WALKABLE_FIRST = 5,
  /// Shallow water: a ripple follows the leader as it walks.
  WORLD_MAP_COLLISION_SHALLOW_WATER = 6,
  /// Where neither collision layer has a value.
  WORLD_MAP_COLLISION_DEFAULT = 7,
  /// Forest: the leader's sprite shows its second layer and no shadow, and
  /// encounters take a region's forest zone.
  WORLD_MAP_COLLISION_FOREST = 9,
  WORLD_MAP_COLLISION_WALKABLE_LAST = 12
};

/// A world map battle environment (gWorldMap_BattleEnvironments,
/// WorldMap_GetBattleEnvironment): what its ground means for battles, named by
/// the battle background BattleBackground_SelectForEnvironment gives it.
enum BattleEnvironment {
  BATTLE_ENVIRONMENT_NONE = 0,      ///< Sol Sanctum's background
  BATTLE_ENVIRONMENT_GRASSLAND = 1, ///< ResourceId_GrasslandBackdrop
  /// ResourceId_ForestBackdrop; encounters take a region's forest zone.
  BATTLE_ENVIRONMENT_FOREST = 2,
  BATTLE_ENVIRONMENT_DESERT = 3,    ///< ResourceId_DesertBackdrop
  BATTLE_ENVIRONMENT_BEACH = 4,     ///< ResourceId_BeachBackdrop
  BATTLE_ENVIRONMENT_SNOWFIELD = 5, ///< ResourceId_SnowfieldBackdrop
  /// ResourceId_SnowfieldBackdrop, for the snowfield's forest.
  BATTLE_ENVIRONMENT_SNOW_FOREST = 6,
  /// ResourceId_BeachBackdrop, for shallow water
  /// (WORLD_MAP_COLLISION_SHALLOW_WATER).
  BATTLE_ENVIRONMENT_SHALLOWS = 7
};

/// A world map cell's encounter region (bits 30..24 of its block word),
/// which picks its encounter zones
/// (gWorldMap_EncounterRules). Only the snowfield changes anything else.
enum WorldMapEncounterRegion {
  /// Takes the second world map graphics set (WorldMap_LoadGraphics) and the
  /// snowfield's battle environments (WorldMap_GetBattleEnvironment).
  WORLD_MAP_ENCOUNTER_REGION_SNOWFIELD = 21
};

/// A map cell's terrain kind: the low four bits of its shape's first byte,
/// which pick its height function from gMapGrid_TerrainHeightFunctions. Samples
/// are the shape's other bytes; x and y run 0 to 15 across the cell.
enum TerrainKind {
  TERRAIN_FLAT = 0,    ///< the first sample everywhere
  TERRAIN_SLOPE_X = 1, ///< from the first sample at x 0 towards the second
  TERRAIN_SLOPE_Y = 2, ///< from the first sample at y 0 towards the second
  /// A step across the diagonal x + y = 15, the higher sample on it.
  TERRAIN_STEP_SUM_DIAGONAL = 3,
  /// A step across the diagonal x = y, the higher sample on it.
  TERRAIN_STEP_DIFFERENCE_DIAGONAL = 4,
  /// Three samples, bending on the diagonal x + y = 15.
  TERRAIN_BEND_SUM_DIAGONAL = 5,
  /// Three samples, bending on the diagonal x = y.
  TERRAIN_BEND_DIFFERENCE_DIAGONAL = 6,
  /// A raised or sunken rounded square (gTerrain_LerpWeightTable).
  TERRAIN_ROUNDED_SQUARE = 7,
  TERRAIN_STEP_X = 8,  ///< the first sample for x below 8, then the second
  TERRAIN_STEP_Y = 9,  ///< the first sample for y below 8, then the second
  TERRAIN_BEND_X = 10, ///< three samples along x, bending at the middle
  TERRAIN_BEND_Y = 11, ///< three samples along y, read unsigned
  /// The sample gTerrain_SampleIndexTable picks for each pixel.
  TERRAIN_SAMPLE_TABLE = 12,
  /// As TERRAIN_SAMPLE_TABLE, mirrored left to right.
  TERRAIN_SAMPLE_TABLE_MIRRORED = 13,
  TERRAIN_FLAT_B = 14, ///< as TERRAIN_FLAT
  TERRAIN_FLAT_C = 15, ///< as TERRAIN_FLAT
  TERRAIN_KIND_COUNT = 16
};

/// What MapGrid_CheckCollision says of a step to a position.
enum Collision {
  COLLISION_TOO_LOW = -1,  ///< the ground there is more than 12 pixels lower
  COLLISION_NONE = 0,      ///< the object can step there
  COLLISION_TOO_HIGH = 1,  ///< the ground there is more than 8 pixels higher
  COLLISION_IMPASSABLE = 2 ///< the cell there is a wall (MB_WALL)
};

u8 *Decompress_Lz(const u8 *source, u8 *destination);
s32 Sprite_ComposeFrame(struct Sprite *animation, u16 dir);
void Render_PlaceSpritePartPair(struct Sprite *sprite, s32 screenX, s32 height,
                                s32 screenY, s32 groundHeight, s32 *scale);
void Render_ApplyProjectedPlacement(struct Sprite *sprite,
                                    const struct SpritePlacement *position,
                                    const struct SpriteScale *scale,
                                    s32 facing);
void Render_PlaceProjectedSprite(struct Sprite *sprite, s32 *point, s32 *scale,
                                 s32 facing, s32 depth);
void Sprite_SetAnimColorModes(void *rawSprite, u32 value);
s32 SpriteFrameCache_Load(u32 slot, u32 *buffer, s32 number, u32 variant);
s32 SpriteFrameCache_FindByTag(s32 tag);
s32 Sprite_ResetAnims(struct Sprite *sprite);
void SpriteAnim_Reset(struct SpriteAnim *layer);
s32 Sprite_AddAnim(struct Sprite *sprite, s32 id);
void Sprite_RemoveAnim(struct Sprite *sprite, s32 layer);
void Sprite_RemoveAnimAt(struct Sprite *sprite, u32 index);
void SpriteAnim_Start(struct SpriteAnim *layer, s32 animation);
s32 Sprite_StartAnim(struct Sprite *sprite, s32 animation);
s32 Sprite_SetAnimTimers(struct Sprite *sprite, s32 frames);
void Sprite_SetAnimSpeed(struct Sprite *sprite, s32 step);
void ObjectSystem_Configure(s32 mode);
struct SpriteAnim *SpriteAnim_Alloc(s32 id);
void SpriteAnim_Clear(struct SpriteAnim *record);
struct Sprite *Sprite_Create(s32 id);
void Sprite_Destroy(struct Sprite *sprite);
s32 SpriteAnim_SumCommandLengths(s32 id, u32 animation, s32 frameCount);
void SpriteDissolve_Step(u8 *obj, u32 pos);
void SpriteDissolve_Run(u8 *sprite);
void SpriteDissolve_RunMany(u8 **sprites, s32 count);
void Object_SelectAnimation(void *raw, s32 no);
void Object_SetAnimationSpeed(struct Object *object, s32 val);
s32 ObjectScript_SetFlagAndTest(struct Object *interpreter);
s32 ObjectScript_ClearFlagAndTest(struct Object *interpreter);
void ObjectScript_SetOrCompareScriptAddress(struct ScriptOperands *state,
                                            s32 operation, s32 value);
void ObjectScript_SetOrCompareFacing(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void ObjectScript_SetOrCompareX(struct ScriptOperands *state, s32 operation,
                                s32 value);
void ObjectScript_SetOrCompareY(struct ScriptOperands *state, s32 operation,
                                s32 value);
void ObjectScript_SetOrCompareZ(struct ScriptOperands *state, s32 operation,
                                s32 value);
void ObjectScript_SetOrCompareScaleX(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void ObjectScript_SetOrCompareScaleY(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void ObjectScript_SetOrCompareVelocityX(struct ScriptOperands *state,
                                        s32 operation, s32 value);
void ObjectScript_SetOrCompareVelocityY(struct ScriptOperands *state,
                                        s32 operation, s32 value);
void ObjectScript_SetOrCompareVelocityZ(struct ScriptOperands *state,
                                        s32 operation, s32 value);
void ObjectScript_SetOrCompareSpeedLimit(struct ScriptOperands *state,
                                         s32 operation, s32 value);
void ObjectScript_SetOrCompareAcceleration(struct ScriptOperands *state,
                                           s32 operation, s32 value);
void ObjectScript_SetOrCompareTargetX(struct ScriptOperands *state,
                                      s32 operation, s32 value);
void ObjectScript_SetOrCompareTargetY(struct ScriptOperands *state,
                                      s32 operation, s32 value);
void ObjectScript_SetOrCompareTargetZ(struct ScriptOperands *state,
                                      s32 operation, s32 value);
void ObjectScript_SetOrCompareVerticalMotion(struct ScriptOperands *state,
                                             s32 operation, s32 value);
void ObjectScript_SetOrCompareGravity(struct ScriptOperands *state,
                                      s32 operation, s32 value);
void ObjectScript_SetOrCompareTerrainHeight(struct ScriptOperands *state,
                                            s32 operation, s32 value);
void ObjectScript_SetOrCompareRiseBaseY(struct ScriptOperands *state,
                                        s32 operation, s32 value);
void ObjectScript_SetOrCompareSprite(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void ObjectScript_SetOrCompareSpriteStorageKind(struct ScriptOperands *state,
                                                s32 operation, s32 value);
void ObjectScript_SetOrCompareMotionFlags(struct ScriptOperands *state,
                                          s32 operation, s32 value);
void ObjectScript_SetOrCompareArrivalAxis(struct ScriptOperands *state,
                                          s32 operation, s32 value);
void ObjectScript_SetOrCompareComparisonResult(struct ScriptOperands *state,
                                               s32 operation, s32 value);
void ObjectScript_SetOrCompareSnapToTarget(struct ScriptOperands *state,
                                           s32 operation, s32 value);
void ObjectScript_SetOrCompareCollisionFlags(struct ScriptOperands *state,
                                             s32 operation, s32 value);
void ObjectScript_SetOrCompareSteeringFlags(struct ScriptOperands *state,
                                            s32 operation, s32 value);
void ObjectScript_SetOrComparePaused(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void ObjectScript_SetOrCompareRepeatCount(struct ScriptOperands *work,
                                          s32 operation, s32 value);
void ObjectScript_SetOrCompareActionParameter(struct ScriptOperands *work,
                                              s32 operation, s32 value);
void ObjectScript_SetOrCompareFootprintSide(struct ScriptOperands *work,
                                            s32 operation, s32 value);
void ObjectScript_SetOrCompareLinkedObject(struct ScriptOperands *work,
                                           s32 operation, s32 value);
void ObjectScript_SetOrCompareUpdateAddress(struct ScriptOperands *work,
                                            s32 operation, s32 value);
void ObjectScript_SetOrCompareRiseCounter(struct ScriptOperands *work,
                                          s32 operation, s32 value);
void ObjectScript_SetOrCompareRiseEnabled(struct ScriptOperands *work,
                                          s32 operation, s32 value);
void SpriteViewer_SetGridColumnColorMode(s32 slot, s32 value);
void SpriteViewer_SetGridColumnPriority(s32 slot, s32 value);

s32 MapGrid_GetScreenRelativePosition(struct Object *object,
                                      s32 *screenPosition);
void ObjectSystem_Initialize(s32 controlMode);
void nullsub_0800C0C4(void);
void nullsub_0800C0C8(void);
void *Object_FindFree(void);
void Object_Release(struct Object *object);
struct Object *Object_CreateField(s32 encodedSprite, s32 x, s32 y, s32 z);
void Object_Initialize(struct Object *object, const s32 *script);
void Object_SetAnimationAndSpeed(struct Object *object, s32 animation,
                                 s32 step);
void Object_SetSpriteRotation(struct Object *object, u32 rotation);
void Anim_SetIndexAndInitializeObjects(void *rawDispatchObject,
                                       s32 animationId);
void Object_RegisterSpriteMetadata(struct Object *object, s32 animationId);
void Object_StartApproachLink(struct Object *object, s32 linkedObjectHandle);
void Object_StartKeyCheck(struct Object *object);
void Object_StartMoveByKeys(struct Object *object);
void Object_StartPlayerControl(struct Object *object);
void Object_StartWorldMapControl(struct Object *object);
void Object_StartIdle(struct Object *object);
void Object_StartCamera(struct Object *object, s32 linkedObjectHandle);
void Object_WaitForValue16(void *rawObject);
void Object_SetSpriteFlags(struct Object *object, u32 flags);
void Object_SetSpriteBlendMode(struct Object *object, u32 mode);
void Object_SetSpriteScale(struct Object *object, u32 fixedScale);
void Anim_ApplyChildValues(struct Object *object, u32 param);
void ObjectSystem_SuspendProjectionAndShowSprites(void);
void ObjectSystem_ResumeProjectionAndHideLayers(void);
s32 Object_ReturnTrue(void);
void ObjectSystem_PlaceSprites(void);
s32 ObjectCamera_ReturnTrue(void);
void ObjectSystem_ProjectSprites(void);

s32 ObjectScript_SetBase(struct Object *interpreter);
s32 ObjectScript_HideSprites(struct Object *interpreter);
s32 ObjectScript_ShowSprite(struct Object *interpreter);
void Object_WaitForMove(void *fieldObject);
s32 Object_IsTargetUnset(struct Object *object);
void Object_UpdateAllThumb(void);
void Object_SetPositionAndResetMotion(struct Object *object, s32 x, s32 y,
                                      s32 z);
void Object_SetMoveTarget(struct Object *object, s32 x, s32 y, s32 z);
void Object_RunUpdateAllFromHeap(void);
void Object_UpdateAllMotion(void);
s32 ObjectScript_Wait(struct Object *interpreter);
s32 ObjectScript_WaitForArrival(struct Object *interpreter);
s32 ObjectScript_InvokeCallback(struct Object *interpreter);
s32 ObjectScript_FindLabel(struct Object *interpreter, u32 labelKey);
s32 ObjectScript_RepeatOrJump(struct Object *interpreter);
s32 ObjectScript_JumpToLabel(struct Object *interpreter);
s32 ObjectScript_JumpIfTrue(struct Object *interpreter);
s32 ObjectScript_JumpIfFalse(struct Object *interpreter);
s32 ObjectScript_End(struct Object *interpreter);
s32 ObjectScript_TestFlag(struct Object *interpreter);
s32 ObjectScript_ToggleFlagAndTest(struct Object *interpreter);
s32 ObjectScript_SetAnimation(struct Object *interpreter);
s32 ObjectScript_FreeObject(struct Object *interpreter);
s32 ObjectScript_Nop(struct Object *interpreter);
s32 ObjectScript_PlaySound(struct Object *interpreter);
s32 ObjectScript_CheckOverlap(struct Object *object, s32 *values);
struct Object *ObjectScript_FindOverlappingEntry(struct Object *object,
                                                 s32 *values);
s32 ObjectScript_SetPositionAndResetMotion(struct Object *interpreter);
s32 ObjectScript_ApplyAbsolutePosition(struct Object *interpreter);
s32 ObjectScript_ApplyRelativePosition(struct Object *interpreter);
s32 ObjectScript_StoreAngleToLinkedObject(struct Object *interpreter);
s32 ObjectScript_ApplyLinkedObjectPosition(struct Object *interpreter);
s32 ObjectScript_ApplyLocalOffsetPosition(struct Object *interpreter);
s32 FieldCamera_FollowLeaderInBounds(struct Object *interpreter);
s32 Object_ApproachLinkedObject(struct Object *interpreter);
s32 Object_Wander(struct Object *interpreter);
s32 ObjectScript_WanderNearHome(struct Object *interpreter);

void ObjectScript_SetOrCompareCursor(struct ScriptOperands *operands,
                                     s32 operation, s32 value);
void ObjectScript_SetOrCompareRadius(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void ObjectScript_SetOrCompareWait(struct ScriptOperands *work, s32 operation,
                                   s32 value);
s32 ObjectScript_ApplyOperandSet(struct Object *interpreter);
s32 ObjectScript_ApplyOperandAdd(struct Object *interpreter);
s32 ObjectScript_ApplyOperandCompare(struct Object *interpreter);

void Object_SetUpdateValue(void *rawObject, s32 value);
u32 Field_RequestShortcut(u32 encodedAssignment);
s32 Field_CheckConfiguredKeys(void);
s32 Field_CheckSpheresOverlap(s32 *firstPosition, s32 radiusA,
                              s32 *secondPosition, s32 radiusB);
s32 Object_UpdateLeaderControl(struct Object *interpreter);
s32 Object_MoveOnWorldMap(struct Object *interpreter);
s32 Field_CheckConfiguredKeysAndCount(struct Object *interpreter);
s32 Object_MoveByKeys(struct Object *interpreter);

void SpriteColor_RemapPixels(u8 *bytes, s32 byteCount);
void TileMap_DecodeStagedBuffer(s32 size);
void TileMap_NumberPlaceholderMetatiles(void);
void TileMap_ConvertBuffer(void);
s32 MapGrid_LoadLayout(s32 index);
void MapGrid_CenterOnFollowedPoint(void);
void MapGrid_RenderMetatileRow(u32 layer, s32 tileX, s32 tileY);
void MapGrid_RenderMetatileColumn(u32 layer, s32 tileX, s32 tileY);
void MapGrid_UpdateLayerScroll(void);
void MapGrid_SetCameraCenter(s32 x, s32 y);
void MapGrid_CopyMetatileIndicesRect(s32 sourceX, s32 sourceY, u32 dstX,
                                     u32 dstY, u32 width, u32 height);
void MapGrid_PlayMetatileCopySequence(u16 *sequence, s32 destinationX,
                                      u32 destinationY);
void MapGrid_CopyMetatileCellsRect(s32 sourceX, s32 sourceY, s32 width,
                                   s32 height, s32 dstX, s32 dstY);
void MapGrid_CopyCellAttributeRect(s32 sourceX, s32 sourceY, s32 width,
                                   s32 height, s32 dstX, s32 dstY);
void MapGrid_CopyCellIndexRect(s32 sourceX, s32 sourceY, s32 width, s32 height,
                               s32 dstX, s32 dstY);
void MapGrid_SetShownLayers(u32 layerFlags);

s32 WorldMap_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);
#ifdef UBFIX
void WorldMap_InitializePerspective(void);
#else
s32 WorldMap_InitializePerspective(void);
#endif
void WorldMap_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py);
void WorldMap_LoadGraphics(s32 x, s32 z);
void WorldMap_ApplyAffineFrame(void);
void WorldMap_RenderPaletteMappedRow(u32 tileRow);
void WorldMap_RenderPaletteMappedColumn(u32 tileColumn);
void WorldMap_UpdateView(void);
void WorldMap_UpdateCurrentTileBlock(void);
void WorldMap_PreloadCurrentTileBlock(void);
void WorldMap_PresentStillFrame(void);
void WorldMap_StartStillView(void);
void WorldMap_ShowBg1FromBuffer(void);
void WorldMap_LoadAreaGraphics(void);
void WorldMap_LoadDefaultCellsAndUpdateBlock(void);
void TilesetAnim_Update(void);
void TilesetAnim_Resume(u32 channelIndex);
void TilesetAnim_Pause(u32 channelIndex);
void TilesetAnim_StartAll(const u16 *script);
void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);
void BlendScript_Run(void);
void BlendScript_Start(u16 *script);
void BlendScript_Suspend(void);
void BlendScript_Resume(void);

void PaletteCycle_Initialize(void);
s32 PaletteCycle_Add(s16 paletteBank, s16 firstColor, s16 delay,
                     s16 colorCount);
void PaletteCycle_Stop(void);
void PaletteCycle_Start(void);
void PaletteCycle_Step(void);

s32 Terrain_GetFirstSampleA(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_LerpTwoSamples(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_LerpTwoSamplesB(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_StepAtSummedPosition(const u8 *heightBytes, s32 x, s32 y);
s32 Terrain_StepAtDifferencePosition(const u8 *heightBytes, s32 x, s32 y);
s32 Terrain_LerpThreeSamplesByRatio(const u8 *heightBytes, s32 x, s32 y);
s32 Terrain_LerpThreeSamplesAtDifference(const u8 *heightBytes, s32 x, s32 y);
s32 Terrain_LerpTwoSamplesByTable(const u8 *heightBytes, s32 x, s32 y);
s32 Terrain_StepAtHalf(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_StepAtHalfB(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_LerpThreeSamplesAtHalf(const u8 *heightBytes, s32 pixelX,
                                   s32 pixelY);
s32 Terrain_LerpThreeSamplesAtHalfB(const u8 *heightBytes, s32 pixelX,
                                    s32 pixelY);
s32 Terrain_LookupSampleByTable(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_LookupSampleByTableReversed(const u8 *heightBytes, s32 pixelX,
                                        s32 pixelY);
s32 Terrain_GetFirstSampleB(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Terrain_GetFirstSampleC(const u8 *heightBytes, s32 pixelX, s32 pixelY);

s32 MapGrid_GetTerrainHeight(s32 layer, s32 x, s32 y);
s32 MapGrid_GetCellAttributeLowNibble(s32 layer, s32 x, s32 y);
s32 MapGrid_GetMetatileBehaviorAt(s32 layer, s32 x, s32 y);
void MapGrid_SetMetatileBehaviorAt(u32 layer, s32 x, s32 y, u32 behavior);
s32 MapGrid_GetCellHighFlags(s32 x, s32 y);
s32 MapGrid_CheckCollision(struct Object *object,
                           struct FieldPosition *position);
s32 MapGrid_IsWallAt(struct WorldPosition *position);
s32 WorldMap_GetCollision(struct WorldPosition *position);
s32 WorldMap_IsWalkable(s32 unused, struct WorldPosition *position);
s32 WorldMap_GetBattleEnvironment(struct WorldPosition *position,
                                  s32 *terrainKind);
void MapGrid_SetScreenShake(s32 shakeX, s32 shakeY, s32 shakeDecay);
void MapGrid_WaitForShakeToSettle(void);
void TileMap_DrawRowsFromRam(const struct WorldMapAffinePair *lines,
                             u8 *pixels);
void WorldMap_BuildScanlineTable(s32 depth, s32 *position,
                                 struct WorldMapScanline *scanline);

void SpriteViewer_Run(void);
s32 SpriteViewer_FindNextSpriteWithImage(s32 spriteId, s32 direction);
void SpriteViewer_BuildProbeRing(s32 centerX, s32 centerY,
                                 struct SpritePlacement *positions);
void SpriteViewer_StartStaggeredAnims(s32 layerIndex, s32 animationIndex);
void SpriteViewer_SetGridColumnNumber(s32 layerIndex, s32 spriteId);
void Battle_PlaceActorsByFormationKind(void);

#endif
