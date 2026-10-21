#ifndef ALCHEMY_UNIT_FIELD_FIELD_H
#define ALCHEMY_UNIT_FIELD_FIELD_H

#include "render_placement.h"

#include "types.h"

struct AnimationEntry;
struct AnimationObject;
struct DispatchObject;
struct FieldPosition;
struct MapAffinePair;
struct MapScanline;
struct ObjectRuntime;
struct ScriptInterpreter;
struct ScriptObjectEntry;
struct ScriptObjectRuntime;
struct ScriptOperands;
struct WorldPosition;

/// How the object system runs (ObjectSystem_Initialize): which camera follows
/// the objects and which update moves them.
enum ObjectSystemMode {
  /// A town, a dungeon or the sprite viewer: the scrolling camera.
  MODE_LAYERED_MAP = 2,
  /// The world map: the fixed camera, which projects 3D points onto the
  /// screen.
  MODE_WORLD_MAP = 3,
  /// A battle: the fixed camera, and the battle's own motion update.
  MODE_BATTLE = 4
};

/// AnimationEntry.kind: the facing table a layer's frames follow
/// (Sprite_ComposeAnimationFrame, facing.c). Each cuts the facing angle into
/// sectors and gives each sector a frame and whether it is mirrored.
enum AnimationFacingKind {
  /// The script's own frame whatever the facing; zero also marks a free
  /// layer record.
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
  /// As ANIM_FACING_FRONT_BACK, with the front frame mirrored the other way.
  ANIM_FACING_FRONT_BACK_SWAPPED = 22,
  /// Ringed as eight facings by the sprite viewer, but with no facing table:
  /// Sprite_ComposeAnimationFrame draws it as ANIM_FACING_NONE.
  /// XXX: likely an older eight-direction kind, because the viewer still
  /// lists it; a sprite of this kind would settle it.
  ANIM_FACING_EIGHT_NO_TABLE = 44,
  /// As ANIM_FACING_EIGHT_FRAMES, with three of the frames mirrored.
  ANIM_FACING_EIGHT_FRAMES_MIRRORED = 88
};

/// AnimationEntry.mode, from the metadata's frameCodec: how a layer's frames
/// are stored. Any other value is raw, for the frame decoder.
enum SpriteFrameCodec {
  SPRITE_FRAME_RAW = 0,
  SPRITE_FRAME_TYPE01 = 1, ///< Resource_DecodeType01's format
  SPRITE_FRAME_LZ = 3      ///< LZ-compressed
};

/// AnimationObject.flags.
enum AnimationObjectFlag {
  SPRITE_FLAG_SHADOW = 1 << 0, ///< its shadow is drawn; set when created
  /// Drawn as an outline, in ObjectSystemWork's edge and fill colours.
  SPRITE_FLAG_OUTLINED = 1 << 1
};

/// ObjectRuntime.animationKind (DispatchObject.kind), its low four bits: what
/// the object's animation pointer holds.
enum ObjectSpriteKind {
  OBJECT_SPRITE_NONE = 0, ///< nothing drawn
  OBJECT_SPRITE_ONE = 1,  ///< one sprite
  OBJECT_SPRITE_GROUP = 2 ///< a list of up to four sprites, empty ones null
};

/// FieldObject_Create's id / 4096: what sprites the object is made with.
enum ObjectCreateKind {
  OBJECT_CREATE_ONE_SPRITE = 0, ///< the id's sprite (OBJECT_SPRITE_ONE)
  /// The sprites of the id and id + 1 (OBJECT_SPRITE_GROUP).
  OBJECT_CREATE_GROUP = 2,
  /// No sprite: any kind but the two above, and the one the camera object
  /// takes.
  OBJECT_CREATE_NO_SPRITE = 8
};

/// The world map's collision values (GetWorldMapCollision), 1 to 15; each
/// indexes a row of gWorldMap_TerrainBehaviorTable. Values not named here
/// are walkable or not only by their range.
enum WorldMapCollision {
  /// The party walks only on WORLD_MAP_COLLISION_WALKABLE_FIRST to
  /// WORLD_MAP_COLLISION_WALKABLE_LAST (CheckWorldMapWalkable).
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

/// A world map terrain behaviour (gWorldMap_TerrainBehaviorTable,
/// GetWorldMapTerrainBehavior): what its ground means for battles, named by
/// the battle backdrop Battle_SelectTerrainBackdrop gives it.
enum WorldMapTerrainBehavior {
  WORLD_MAP_TERRAIN_NONE = 0,      ///< Sol Sanctum's backdrop
  WORLD_MAP_TERRAIN_GRASSLAND = 1, ///< ResourceId_GrasslandBackdrop
  /// ResourceId_ForestBackdrop; encounters take a region's forest zone.
  WORLD_MAP_TERRAIN_FOREST = 2,
  WORLD_MAP_TERRAIN_DESERT = 3,    ///< ResourceId_DesertBackdrop
  WORLD_MAP_TERRAIN_BEACH = 4,     ///< ResourceId_BeachBackdrop
  WORLD_MAP_TERRAIN_SNOWFIELD = 5, ///< ResourceId_SnowfieldBackdrop
  /// ResourceId_SnowfieldBackdrop, for the snowfield's forest.
  WORLD_MAP_TERRAIN_SNOW_FOREST = 6,
  /// ResourceId_BeachBackdrop, for shallow water
  /// (WORLD_MAP_COLLISION_SHALLOW_WATER).
  WORLD_MAP_TERRAIN_SHALLOWS = 7
};

/// A world map cell's terrain kind (bits 30..24 of its block word): the
/// region it belongs to, which picks its encounter zones
/// (gWorldMap_EncounterRules). Only the snowfield changes anything else.
enum WorldMapRegion {
  /// Takes the second world map graphics set (WorldMap_LoadGraphics) and the
  /// snowfield's terrain behaviours (GetWorldMapTerrainBehavior).
  WORLD_MAP_REGION_SNOWFIELD = 21
};

/// A map cell's terrain kind: the low four bits of its shape's first byte,
/// which pick its height function from gMap_TerrainHeightFunctions. Samples
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
  /// A raised or sunken rounded square (gCurve_LerpWeightTable).
  TERRAIN_ROUNDED_SQUARE = 7,
  TERRAIN_STEP_X = 8,  ///< the first sample for x below 8, then the second
  TERRAIN_STEP_Y = 9,  ///< the first sample for y below 8, then the second
  TERRAIN_BEND_X = 10, ///< three samples along x, bending at the middle
  TERRAIN_BEND_Y = 11, ///< three samples along y, read unsigned
  /// The sample gCurve_SampleIndexTable picks for each pixel.
  TERRAIN_SAMPLE_TABLE = 12,
  /// As TERRAIN_SAMPLE_TABLE, mirrored left to right.
  TERRAIN_SAMPLE_TABLE_MIRRORED = 13,
  TERRAIN_FLAT_B = 14, ///< as TERRAIN_FLAT
  TERRAIN_FLAT_C = 15, ///< as TERRAIN_FLAT
  TERRAIN_KIND_COUNT = 16
};

/// What Map_CheckObjectStep says of a step to a position.
enum MapStepCheck {
  MAP_STEP_TOO_LOW = -1, ///< the ground there is more than 12 pixels lower
  MAP_STEP_OPEN = 0,     ///< the object can step there
  MAP_STEP_TOO_HIGH = 1, ///< the ground there is more than 8 pixels higher
  MAP_STEP_WALL = 2      ///< the cell there is a wall (MAP_CELL_WALL)
};

u8 *Resource_DecompressLz(const u8 *source, u8 *destination);
s32 Sprite_ComposeAnimationFrame(struct AnimationObject *obj, u16 dir);
void Render_PlaceSpritePartPair(struct AnimationObject *sprite, s32 screenX,
                                s32 height, s32 screenY, s32 groundHeight,
                                s32 *scale);
void Render_ApplyProjectedPlacement(struct AnimationObject *sprite,
                                    const struct SpritePlacement *position,
                                    const struct SpriteScale *scale,
                                    s32 facing);
void Render_PlaceProjectedSprite(struct AnimationObject *sprite, s32 *point,
                                 s32 *scale, s32 facing, s32 depth);
void ObjectGroup_SetChildValueUnlessFifteen(void *rawSprite, u32 value);
s32 ResourceSlot_Load(u32 slot, u32 *buffer, s32 number, u32 variant);
s32 Animation_LookupValueByKey(s32 tag);
s32 Animation_InitializeObjects(struct AnimationObject *sprite);
void Animation_InitWorkFromMetadata(struct AnimationEntry *layer);
s32 ResourceMetadata_Register(struct AnimationObject *sprite, s32 id);
void ResourceMetadata_Unregister(struct AnimationObject *sprite, s32 layer);
void ResourceMetadata_ReleaseSlot(struct AnimationObject *sprite, u32 index);
void Animation_SetWorkEntry(struct AnimationEntry *layer, s32 animation);
s32 AnimationObjects_SelectAnimation(struct AnimationObject *sprite,
                                     s32 animation);
s32 AnimationObjects_SetActiveFrameTimers(struct AnimationObject *sprite,
                                          s32 frames);
void AnimationObjects_SetActiveTimerSteps(struct AnimationObject *sprite,
                                          s32 step);
void ObjectSystem_Configure(s32 mode);
struct AnimationEntry *AnimationObject_Allocate(s32 id);
void ResourceMetadata_ClearRecord(struct AnimationEntry *record);
struct AnimationObject *ResourceObject_Create(s32 id);
void ResourceObject_Release(struct AnimationObject *sprite);
s32 ResourceMetadata_SumCommandLengths(s32 id, u32 animation, s32 frameCount);
void Map_RenderAnimatedTileFrame(u8 *obj, u32 pos);
void Map_RenderAnimatedTileFramesForObject(u8 *sprite);
void Map_RenderAllAnimatedTileFrames(u8 **sprites, s32 count);
void ObjectDispatch_SelectAnimation(void *raw, s32 no);
void ObjectDispatch_SetAnimationSpeed(struct DispatchObject *obj, s32 val);
s32 Script_SetFlagAndTest(struct ScriptInterpreter *interpreter);
s32 Script_ClearFlagAndTest(struct ScriptInterpreter *interpreter);
void Script_SetOrCompareAddress(struct ScriptOperands *state, s32 operation,
                                s32 value);
void Script_SetOrCompareUnsignedHalfword(struct ScriptOperands *state,
                                         s32 operation, s32 value);
void Script_SetOrCompareX(struct ScriptOperands *state, s32 operation,
                          s32 value);
void Script_SetOrCompareY(struct ScriptOperands *state, s32 operation,
                          s32 value);
void Script_SetOrCompareZ(struct ScriptOperands *state, s32 operation,
                          s32 value);
void Script_SetOrCompareScaleX(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareScaleY(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareVelocityX(struct ScriptOperands *state, s32 operation,
                                  s32 value);
void Script_SetOrCompareVelocityY(struct ScriptOperands *state, s32 operation,
                                  s32 value);
void Script_SetOrCompareVelocityZ(struct ScriptOperands *state, s32 operation,
                                  s32 value);
void Script_SetOrCompareSpeedLimit(struct ScriptOperands *state, s32 operation,
                                   s32 value);
void Script_SetOrCompareAcceleration(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void Script_SetOrCompareTargetX(struct ScriptOperands *state, s32 operation,
                                s32 value);
void Script_SetOrCompareTargetY(struct ScriptOperands *state, s32 operation,
                                s32 value);
void Script_SetOrCompareTargetZ(struct ScriptOperands *state, s32 operation,
                                s32 value);
void Script_SetOrCompareVerticalMotion(struct ScriptOperands *state,
                                       s32 operation, s32 value);
void Script_SetOrCompareGravity(struct ScriptOperands *state, s32 operation,
                                s32 value);
void Script_SetOrCompareTerrainHeight(struct ScriptOperands *state,
                                      s32 operation, s32 value);
void Script_SetOrCompareWord4c(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareAddress50(struct ScriptOperands *state, s32 operation,
                                  s32 value);
void Script_SetOrCompareByte54(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareMotionFlags(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void Script_SetOrCompareArrivalAxis(struct ScriptOperands *state, s32 operation,
                                    s32 value);
void Script_SetOrCompareComparisonResult(struct ScriptOperands *state,
                                         s32 operation, s32 value);
void Script_SetOrCompareSnapToTarget(struct ScriptOperands *state,
                                     s32 operation, s32 value);
void Script_SetOrCompareCollisionFlags(struct ScriptOperands *state,
                                       s32 operation, s32 value);
void Script_SetOrCompareSteeringFlags(struct ScriptOperands *state,
                                      s32 operation, s32 value);
void Script_SetOrComparePaused(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareRepeatCount(struct ScriptOperands *work, s32 operation,
                                    s32 value);
void Script_SetOrCompareSignedHalfword64(struct ScriptOperands *work,
                                         s32 operation, s32 value);
void Script_SetOrCompareSignedHalfword66(struct ScriptOperands *work,
                                         s32 operation, s32 value);
void Script_SetOrCompareWord68(struct ScriptOperands *work, s32 operation,
                               s32 value);
void Script_SetOrCompareUpdateAddress(struct ScriptOperands *work,
                                      s32 operation, s32 value);
void Script_SetOrCompareByte62(struct ScriptOperands *work, s32 operation,
                               s32 value);
void Script_SetOrCompareByte63(struct ScriptOperands *work, s32 operation,
                               s32 value);
void Ui_SetGridColumnParameter(s32 slot, s32 value);
void Ui_SetGridColumnPriority(s32 slot, s32 value);

s32 Map_GetScreenRelativePosition(struct ObjectRuntime *object,
                                  s32 *screenPosition);
void ObjectSystem_Initialize(s32 controlMode);
void ResourceTable_ReservedNoOpC0C4(void);
void ResourceTable_ReservedNoOp2(void);
void *ObjectDispatch_FindFreeObject(void);
void ObjectDispatch_Release(struct DispatchObject *object);
struct ObjectRuntime *FieldObject_Create(s32 encodedSprite, s32 x, s32 y,
                                         s32 z);
void ObjectDispatch_Initialize(struct DispatchObject *object,
                               const s32 *script);
void ObjectDispatch_SetAnimationAndSpeed(struct DispatchObject *object,
                                         s32 animation, s32 step);
void ObjectDispatch_SetChildRotation(struct DispatchObject *object,
                                     u32 rotation);
void Animation_SetIndexAndInitObjects(void *rawDispatchObject, s32 animationId);
void ObjectDispatch_RegisterChildMetadata(struct DispatchObject *object,
                                          s32 animationId);
void ObjectDispatch_StartApproachLink(struct DispatchObject *object,
                                      s32 linkedObjectHandle);
void ObjectDispatch_StartKeyCheck(struct DispatchObject *object);
void ObjectDispatch_StartMoveByKeys(struct DispatchObject *object);
void ObjectDispatch_StartPlayerControl(struct DispatchObject *object);
void ObjectDispatch_StartWorldMapControl(struct DispatchObject *object);
void ObjectDispatch_StartIdle(struct DispatchObject *object);
void ObjectDispatch_StartCamera(struct DispatchObject *object,
                                s32 linkedObjectHandle);
void ObjectDispatch_WaitForValue16(void *rawObject);
void ObjectDispatch_SetSingleChildFlags(struct DispatchObject *object,
                                        u32 flags);
void ObjectDispatch_SetChildBlendMode(struct DispatchObject *object, u32 mode);
void ObjectDispatch_SetChildFixedScale(struct DispatchObject *object,
                                       u32 fixedScale);
void Animation_ApplyChildValues(struct DispatchObject *object, u32 param);
void ObjectSystem_SuspendProjectionAndShowSprites(void);
void ObjectSystem_ResumeProjectionAndHideLayers(void);
s32 ObjectDispatch_ReturnTrue(void);
void ObjectSystem_UpdateCamera(void);
s32 ObjectCamera_ReturnTrue(void);
void ObjectSystem_UpdateCameraFixed(void);

s32 Script_SetBase(struct ScriptInterpreter *interpreter);
s32 Script_ClearByte54(struct ScriptInterpreter *interpreter);
s32 Script_SetByte54(struct ScriptInterpreter *interpreter);
void FieldObject_WaitForMove(void *fieldObject);
s32 Object_IsTargetUnset(struct ObjectRuntime *object);
void Object_UpdateAllThumb(void);
void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x,
                                      s32 y, s32 z);
void Object_SetMoveTarget(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Object_RunUpdateAllFromHeap(void);
void Object_UpdateAllMotion(void);
s32 Script_Wait(struct ScriptInterpreter *interpreter);
s32 Script_WaitForArrival(struct ScriptInterpreter *interpreter);
s32 Script_InvokeCallback(struct ScriptInterpreter *interpreter);
s32 Script_FindLabel(struct ScriptInterpreter *interpreter, u32 labelKey);
s32 Script_RepeatOrJump(struct ScriptInterpreter *interpreter);
s32 Script_JumpToLabel(struct ScriptInterpreter *interpreter);
s32 Script_JumpIfTrue(struct ScriptInterpreter *interpreter);
s32 Script_JumpIfFalse(struct ScriptInterpreter *interpreter);
s32 Script_End(struct ScriptInterpreter *interpreter);
s32 Script_TestFlag(struct ScriptInterpreter *interpreter);
s32 Script_ToggleFlagAndTest(struct ScriptInterpreter *interpreter);
s32 Script_SetAnimation(struct ScriptInterpreter *interpreter);
s32 Script_FreeObject(struct ScriptInterpreter *interpreter);
s32 Script_Nop(struct ScriptInterpreter *interpreter);
s32 Script_PlaySound(struct ScriptInterpreter *interpreter);
s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *values);
struct ScriptObjectEntry *
ScriptObject_FindOverlappingEntry(struct ScriptObjectEntry *object,
                                  s32 *values);
s32 Script_SetPositionAndResetMotion(struct ScriptInterpreter *interpreter);
s32 Script_ApplyAbsolutePosition(struct ScriptInterpreter *interpreter);
s32 Script_ApplyRelativePosition(struct ScriptInterpreter *interpreter);
s32 Script_StoreAngleToLinkedObject(struct ScriptInterpreter *interpreter);
s32 Script_ApplyLinkedObjectPosition(struct ScriptInterpreter *interpreter);
s32 Script_ApplyLocalOffsetPosition(struct ScriptInterpreter *interpreter);
s32 Camera_FollowLeaderInBounds(struct ScriptInterpreter *interpreter);
s32 Object_ApproachLinkedObject(struct ScriptInterpreter *interpreter);
s32 Object_Wander(struct ScriptInterpreter *interpreter);
s32 ScriptObject_WanderNearHome(struct ScriptInterpreter *interpreter);

void Script_SetOrCompareCursor(struct ScriptOperands *operands, s32 operation,
                               s32 value);
void Script_SetOrCompareRadius(struct ScriptOperands *state, s32 operation,
                               s32 value);
void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation,
                                   s32 value);
s32 Script_ApplyOperandSet(struct ScriptInterpreter *interpreter);
s32 Script_ApplyOperandAdd(struct ScriptInterpreter *interpreter);
s32 Script_ApplyOperandCompare(struct ScriptInterpreter *interpreter);

void ObjectDispatch_SetUpdateValue(void *rawObject, s32 value);
u32 Field_RequestShortcut(u32 encodedAssignment);
s32 Field_CheckConfiguredKeys(void);
s32 Runtime_CheckRadiusOverlap(s32 *firstPosition, s32 radiusA,
                               s32 *secondPosition, s32 radiusB);
s32 FieldObject_UpdatePlayerControl(struct ScriptInterpreter *interpreter);
s32 Object_MoveOnWorldMap(struct ScriptInterpreter *interpreter);
s32 Field_CheckConfiguredKeysAndCount(struct ScriptInterpreter *interpreter);
s32 Object_MoveByKeys(struct ScriptInterpreter *interpreter);

void Runtime_RemapBytesByTable(u8 *bytes, s32 byteCount);
void TileMap_DecodeStagedBuffer(s32 size);
void TileMap_NumberPlaceholderBlocks(void);
void TileMap_ConvertBuffer(void);
s32 Map_LoadLayeredScene(s32 index);
void Map_CenterOnFollowedPoint(void);
void Map_RenderMetatileRow(u32 layer, s32 tileX, s32 tileY);
void Map_RenderMetatileColumn(u32 layer, s32 tileX, s32 tileY);
void Map_UpdateLayerScroll(void);
void Map_SetCameraCenter(s32 x, s32 y);
void Map_CopyMetatileIndicesRect(s32 srcX, s32 srcY, u32 dstX, u32 dstY,
                                 u32 width, u32 height);
void Map_PlayMetatileCopySequence(u16 *sequence, s32 destinationX,
                                  u32 destinationY);
void Map_CopyMetatileCellsRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dstX, s32 dstY);
void Map_CopyCellAttributeRect(s32 srcX, s32 srcY, s32 width, s32 height,
                               s32 dstX, s32 dstY);
void Map_CopyCellIndexRect(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                           s32 dstY);
void Map_SetShownLayers(u32 layerFlags);

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);
#ifdef UBFIX
void Map_InitializePerspectiveScene(void);
#else
s32 Map_InitializePerspectiveScene(void);
#endif
void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py);
void WorldMap_LoadGraphics(s32 x, s32 z);
void MapAnimation_ApplyAffineFrame(void);
void Map_RenderPaletteMappedRow(u32 tileRow);
void Map_RenderPaletteMappedColumn(u32 tileColumn);
void WorldMap_UpdateView(void);
void Map_UpdateCurrentTileBlock(void);
void Map_PreloadCurrentTileBlock(void);
void MapAnimation_PresentFrame(void);
void WorldMap_BeginStillView(void);
void Map_ShowBg1FromBuffer(void);
void Map_LoadAreaGraphics(void);
void Map_LoadDefaultCellsAndUpdateBlock(void);
void MapAnimation_Update(void);
void Map_ClearLayerEntryFlag(u32 channelIndex);
void Map_SetLayerEntryFlag(u32 channelIndex);
void MapAnimation_StartChannels(const u16 *script);
void MapAnimation_Suspend(void);
void MapAnimation_Resume(void);
void DisplayBlend_RunScript(void);
void DisplayBlend_StartScript(u16 *script);
void DisplayBlend_SuspendScript(void);
void DisplayBlend_ResumeScript(void);

void PaletteQueue_Initialize(void);
s32 PaletteQueue_Add(s16 paletteBank, s16 firstColor, s16 delay,
                     s16 colorCount);
void PaletteQueue_Stop(void);
void PaletteQueue_Start(void);
void PaletteQueue_StepCycles(void);

s32 Curve_GetFirstSampleA(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Curve_LerpTwoSamples(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Curve_LerpTwoSamplesB(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Curve_StepAtSummedPosition(const u8 *heightBytes, s32 x, s32 y);
s32 Curve_StepAtDifferencePosition(const u8 *heightBytes, s32 x, s32 y);
s32 Curve_LerpThreeSamplesByRatio(const u8 *heightBytes, s32 x, s32 y);
s32 Curve_LerpThreeSamplesAtDifference(const u8 *heightBytes, s32 x, s32 y);
s32 Curve_LerpTwoSamplesByTable(const u8 *heightBytes, s32 x, s32 y);
s32 Curve_StepAtHalf(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Curve_StepAtHalfB(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Curve_LerpThreeSamplesAtHalf(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Curve_LerpThreeSamplesAtHalfB(const u8 *heightBytes, s32 pixelX,
                                  s32 pixelY);
s32 Curve_LookupSampleByTable(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Curve_LookupSampleByTableReversed(const u8 *heightBytes, s32 pixelX,
                                      s32 pixelY);
s32 Curve_GetFirstSampleB(const u8 *heightBytes, s32 pixelX, s32 pixelY);
s32 Curve_GetFirstSampleC(const u8 *heightBytes, s32 pixelX, s32 pixelY);

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 y);
s32 Map_GetCellAttributeLowNibble(s32 layer, s32 x, s32 y);
s32 GetMapCellCollision(s32 layer, s32 x, s32 y);
void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collisionCode);
s32 Map_GetCellHighFlags(s32 x, s32 y);
s32 Map_CheckObjectStep(struct ObjectRuntime *object,
                        struct FieldPosition *position);
s32 CheckMapPositionCellOccupied(struct WorldPosition *position);
s32 GetWorldMapCollision(struct WorldPosition *position);
s32 CheckWorldMapWalkable(s32 unused, struct WorldPosition *position);
s32 GetWorldMapTerrainBehavior(struct WorldPosition *position,
                               s32 *terrainKind);
void Map_SetScreenShake(s32 shakeX, s32 shakeY, s32 shakeDecay);
void Map_WaitForShakeToSettle(void);
void TileMap_DrawRowsFromRam(const struct MapAffinePair *lines, u8 *pixels);
void WorldMap_BuildScanlineTable(s32 depth, s32 *position,
                                 struct MapScanline *scanline);

void Ui_RunIconMonitor(void);
s32 Ui_FindNextSpriteWithImage(s32 spriteId, s32 direction);
void Map_BuildProbeRing(s32 centerX, s32 centerY,
                        struct SpritePlacement *positions);
void Ui_FillGridColumnFromMetadata(s32 layerIndex, s32 animationIndex);
void Ui_SetGridColumnNumber(s32 layerIndex, s32 spriteId);
void Battle_PlaceActorsByFormationKind(void);

#endif
