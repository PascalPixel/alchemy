#ifndef ALCHEMY_UNIT_FIELD_FIELD_H
#define ALCHEMY_UNIT_FIELD_FIELD_H

#include "render_placement.h"

#include "types.h"

struct AnimationEntry;
struct AnimationObject;
struct DispatchObject;
struct FieldPosition;
struct MapScanline;
struct ObjectRuntime;
struct ScriptInterpreter;
struct ScriptObjectEntry;
struct ScriptObjectRuntime;
struct ScriptOperands;
struct WorldPosition;

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
void Tilemap_DecodeStagedBuffer(s32 size);
void Tilemap_NumberPlaceholderBlocks(void);
void Tilemap_ConvertBuffer(void);
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
void TileMap_DrawRowsFromRam(s32 lines, s32 pixels);
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
