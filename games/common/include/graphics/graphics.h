#ifndef GRAPHICS_H
#define GRAPHICS_H

/* GRAPHICS */

#include "types.h"
#include "edition.h"

struct UiWindow;

extern u8		   gBlendBrighten;
extern volatile u8 gBlendDuration;
extern u8		   gBlendFramesLeft;
extern u16		   gBlendLayers;
extern u8		   gBlendStartLevel;
extern volatile u8 gBlendTargetLevel;
extern u8		   gRomShiftedTilePair[];
extern const u32   RelocatedQ16DotProduct[];
extern u8		   Resource_Decode2CodeSize[];
extern u8		   Resource_DecodeByteLzCodeSize[];
extern const u8	   Ui_EpRowString[];
extern s32		   Ui_FixedTileBlocks[];
extern const u8	   Ui_HpRowString[];
extern const u8	   Ui_HpString[];
extern const u8	   Ui_PpString[];
extern const u8	   Ui_SlashString[];
extern u8		  *UiIcon_ItemIconPointers[];
extern u8		  *UiIcon_ItemIconPointersEnd[];
extern u8		  *UiIcon_MiscIconPointers[];
extern u8		  *UiIcon_PsynergyIconPointers[];
extern u8		  *UiIcon_PsynergyIconPointersEnd[];
void			   Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse);
#ifndef OWN_BattleLayout_HighlightPartyPanels
s32 BattleLayout_HighlightPartyPanels(u16 *ids);
#endif
s32	 BattleParty_ListActorIdsFar(s32 side, s32 group);
void PaletteGlow_UpdateSine(void);
void Render_ResetTransformState(void);
s32	 Ui_ClearVramBlock(void);
void Ui_LoadCharacterEntryForSlotFar(s32 a, s32 b, s32 c);
void Ui_PrepareTransferFromTableEntry(u32 index);
s32	 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);
void UiText_DrawNumberRightAlignedFar(s32 number, s32 layer, s32 x, s32 y);
void UiTextResource_Release(s32 value);
void PtyStDraw(s32 flags);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
#ifndef OWN_UiWindow_SetTilemapEntry
void UiWindow_SetTilemapEntry(struct UiWindow *window, s32 value, s32 x, s32 y, u32 mode);
#endif
void UiWork_InitCountersWithResourceAndScheduleRefresh(void);
void UiWork_ProcessDirectWork(void);
void UiWork_ProcessRenderChannels(void);
void UiWork_SetTwoEntriesTo999(void);
void UiWork_UploadDirtyBlocks(void);

#if !GAME_TLA
struct AnimationObject;
struct ArticleTable;
struct BtlUnit;
struct EffectLaunch;
struct GlyphPalette;
struct NumberRow;
struct PaletteSnapshotWork;
struct RenderInput;
struct RenderOutput;
struct ShopCursor;
struct TileMask;
struct TransitionShape;
struct UiBobPairOutput;
struct UiChannelSlot;
struct UiScaleSprite;
struct UiWindow;
struct Vec2;
struct WorkspaceWork;

extern const u8	   AnimationFacing_Kind1[];
extern const u8	   AnimationFacing_Kind2[];
extern const u8	   AnimationFacing_Kind22[];
extern const u8	   AnimationFacing_Kind3[];
extern const u8	   AnimationFacing_Kind4[];
extern const u8	   AnimationFacing_Kind5[];
extern const u8	   AnimationFacing_Kind6[];
extern const u8	   AnimationFacing_Kind8[];
extern const u8	   AnimationFacing_Kind88[];
extern struct Vec2 Battle_FormationPlacementScale;
extern u8		   ColorBuffer_BackupAndBrightenCodeSize[];
extern u8		   ColorBuffer_BackupAndBrightenPartialCodeSize[];
extern u8		   ColorBuffer_BackupAndDarkenCodeSize[];
extern u8		   ColorBuffer_BackupAndDarkenPartialCodeSize[];
extern u8		   ColorBuffer_BackupAndHalveCodeSize[];
extern u8		   ColorBuffer_BackupAndHalveNonzeroCodeSize[];
extern u8		   ColorBuffer_BackupAndScaleNonzeroThreeQuartersCodeSize[];
extern u8		   ColorBuffer_BackupAndScaleThreeQuartersCodeSize[];
extern const u8	   ColorBuffer_BrightenKernel[];
extern const u8	   ColorBuffer_BrightenPartialKernel[];
extern const u8	   ColorBuffer_DarkenKernel[];
extern const u8	   ColorBuffer_DarkenPartialKernel[];
extern const u8	   ColorBuffer_HalveKernel[];
extern const u8	   ColorBuffer_HalveNonzeroKernel[];
extern const u8	   ColorBuffer_ScaleNonzeroThreeQuartersKernel[];
extern const u8	   ColorBuffer_ScaleThreeQuartersKernel[];
#ifndef OWN_gRenderSlotHeads
extern u8 gRenderSlotHeads[];
#endif
extern u8						 gPartyStatusWork[];
extern u8						 gSelectionWorkspace[];
extern const struct ArticleTable UiText_ArticleTable;
struct TileMask
{
	u32 word0;
	u32 word1;
};
extern const struct TileMask	   Menu_BarPatternRows[];
extern const u8					   Title_IntroSpriteColumns[];
extern const u8					   DisplayScroll_Font[];
extern const u8					   DisplayScroll_GlyphWidths[];
extern const void				  *DisplayScroll_LineTable[];
extern const u32				   DisplayScroll_SlideResources[];
extern const u8					   DisplayTransition_DitherTable[];
extern s8						   DisplayTransition_EdgeOffsets[];
extern u8						   DisplayTransition_NoiseWidths[];
extern const u8					   FarCall_ResourceTable[];
extern u8						   gDecodeFillByte;
extern struct PaletteSnapshotWork *gPaletteWork;
extern u16						   Graphics_ExpandNibbleTable[];
extern struct AnimationObject	  *gSpriteObjects;
#ifndef OWN_Menu_CursorObjectTiles
extern const u8 Menu_CursorObjectTiles[];
#endif
extern const u8					 NameEntry_AlternateKeyboardTilemap[];
extern s8						 NameEntry_CaretWave[];
extern const u8					 NameEntry_DakutenRanges[];
extern u8						 NameEntry_DeleteKeyLabel[];
extern u8						 NameEntry_EndKeyLabel[];
extern const u8					 NameEntry_HandakutenRanges[];
extern const u8					 NameEntry_KeyboardTilemap[];
extern const u8					 PaletteGlow_WaveTable[];
extern const u8					 Render_DecodeFrameBuffer[];
extern const u8					 Resource_Decode2[];
extern const u8					 Resource_DecodeByteLzArm[];
extern const u8					 Resource_DecodeHalfwordLz[];
extern const u8					 Resource_DecodeType01Arm[];
extern u8						 Resource_DecodeType01CodeSize[];
extern u8						 Resource_DecompressHalfwordsCodeSize[];
extern u8						 Resource_FixedBlockBTiles[];
extern const u8					 Resource_FixedBlockATiles[];
extern const u8					 SentouKouka_Tenkai[];
extern u8						 SentouKouka_TenkaiCodeSize[];
extern const u8					 Text_DecodeSymbol[];
extern u8						 Text_DecodeSymbolCodeSize[];
extern const u8					 Tile_BuildMetatiles[];
extern u8						 Tile_BuildMetatilesCodeSize[];
extern const u8					 Tile_ConvertMap[];
extern u8						 Tile_ConvertMapCodeSize[];
extern const u8					 Tile_Decompress4bpp[];
extern u8						 Tile_Decompress4bppCodeSize[];
extern const u8					 Tile_ExpandMasked[];
extern u8						 Tile_ExpandMaskedCodeSize[];
extern const u8					 Tile_ExpandOpaque[];
extern u8						 Tile_ExpandOpaqueCodeSize[];
extern const u8					 Title_PromptBlendLevels[];
extern const u8					 Title_PromptTiles[];
extern u16						 Ui_BounceScales[];
extern s32						 Ui_ObjectPulseScales[];
extern const u8					 Ui_PairBobOffsets[];
extern u8						*UiIcon_DigitPointers[];
extern u8						*UiIcon_FramePointerTable[];
extern u8						*UiIcon_MarkPointers[];
extern u8						*UiIcon_OverlayPointerTable[];
extern u8						 UiIcon_ResourceTiles[];
extern const s8					 UiSprite_BounceOffsets[];
extern const u8					 UiSprite_CursorBobOffsets[];
extern const u8					 UiSprite_CursorFrameTiles[];
extern const s8					 UiSprite_WobbleOffsets[][2];
extern const u8					 UiText_ColourPens[];
extern const u8					 UiText_DrawGlyphArm[];
extern u8						 UiText_DrawGlyphCodeSize[];
extern const u8					 UiText_GlyphWidths[];
extern const struct ArticleTable UiText_IndefiniteArticles;
extern const u8					 UiText_LookupMessageArm[];
extern u8						 UiText_LookupMessageCodeSize[];
extern const u8					 UiText_SpeedCueGaps[3];
extern const u8					 UiText_SpeedDelays[3];
extern const u8					 UiText_SpeedSteps[3];
extern const s8					 UiWindow_PartyColumnOffsets[];
void							 Battle_PlaceActorsByFormationKind(void);
void							 BattleFx_ClearActorTimers(void);
s32								 BattleFx_InitializeStarField(s32 mode);
void BattleFx_InterpolateBuffers(s16 *source, s16 *target, s16 *step, s32 frames);
#ifndef OWN_BattleFx_PlayUnitElementEffect
void BattleFx_PlayUnitElementEffect(s32 unit, s32 kind, s32 mode, s32 variant);
#endif
void BattleFx_RunGatheringBurstFar(struct EffectLaunch *launch);
void BattleFx_ScheduleCallbacksAndReleaseBlocksFar(void);
void BattlePalette_UpdateBlend(void);
s32	 BattleParty_PrepareActiveOwnersFar(s32);
s32	 BattlePlacement_CountValidEntriesFar(s32, u8 *);
void BattlePresentation_PrepareSceneFar(s32 kind);
void Blend_SetBrightenTarget0(s32 duration);
void Blend_SetBrightenTarget16(s32 duration);
void Blend_SetDarkenTarget0(s32 duration);
void Blend_SetDarkenTarget16(s32 duration);
void Blend_WaitForTransition(void);
void BlendTransition_Update(void);
#ifndef OWN_Camera_StoreSceneParameters
void Camera_StoreSceneParameters(u32 value0, u32 value1, u32 value2);
#endif
u16 Color_ScaleComponents(s16 *rgb, s32 red_scale, s32 green_scale, s32 blue_scale);
#ifndef OWN_ColorBuffer_BackupAndBrighten
void ColorBuffer_BackupAndBrighten(u8 *buffer, u32 amount, u8 *backup, u32 bytes);
#endif
#ifndef OWN_ColorBuffer_BackupAndDarken
void ColorBuffer_BackupAndDarken(u8 *buffer, u32 amount, u8 *backup, u32 bytes);
#endif
#ifndef OWN_ColorBuffer_BackupAndHalve
void ColorBuffer_BackupAndHalve(u8 *buffer, u8 *backup, u32 bytes);
#endif
void ColorBuffer_BackupAndHalveNonzero(u8 *buffer, u8 *backup, u32 bytes);
void ColorBuffer_BackupAndScaleNonzeroThreeQuarters(u8 *buffer, u8 *backup, u32 bytes);
#ifndef OWN_ColorBuffer_BackupAndScaleThreeQuarters
void ColorBuffer_BackupAndScaleThreeQuarters(u8 *buffer, u8 *backup, u32 bytes);
#endif
void DisplayBlend_DisableRunScript(void);
#ifndef OWN_DisplayBlend_StartScript
void DisplayBlend_StartScript(u16 *script);
#endif
s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align);
#ifndef OWN_DisplayScroll_InitializeHBlankDma
void DisplayScroll_InitializeHBlankDma(
	s32 mode, s32 freq_x, s32 step_x, s32 amp_x, s32 freq_y, s32 step_y, s32 amp_y);
#endif
void DisplayScroll_InitObjectTable(void);
void DisplayScroll_RenderEnteringLine(void);
void DisplayScroll_UpdateObjects(void);
s32	 DjinnMenu_ShowHelp(void);
#ifndef OWN_FarCall_EffectTable
s32 FarCall_EffectTable(s32 slot, s32 width_shift, s32 height_shift, s32 flags, u32 mode);
#endif
#ifndef OWN_Graphics_BuildSequentialTileTable
void Graphics_BuildSequentialTileTable(s32 *destination);
#endif
s32	 Graphics_ClampRgb555Channel(s32 val);
s32	 Graphics_ClampRgb555Component(s32 val);
void Graphics_ClearBg1ControlBit2(void);
void Graphics_ClearCharacterBlockAndPalette(s32 alternate);
void Graphics_RestoreTransferWork(void);
s32	 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count);
void Graphics_ScaleRgb555ClampedFar(void *, void *, s32, s32);
#ifndef OWN_Graphics_TransformSmallPalette
void Graphics_TransformSmallPalette(s32 index, s32 transform);
#endif
#ifndef OWN_Graphics_TransposeCopy
void Graphics_TransposeCopy(
	const u8 *source_base, u8 *destination_base, s32 row_size, s32 column_count);
#endif
void Graphics_UpdatePhasePalette(s32 frame, s32 red_phase, s32 green_phase, s32 blue_phase);
void Link_DrawShiftedTilePair(s32 offset);
u16 *Memory_FillHalfwordsDma(u16 *destination, s32 value, s32 count);
s32	 Menu_DrawAtWindowOffset(void *win, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
#ifndef OWN_MenuSelection_BuildEntries
void MenuSelection_BuildEntries(void);
#endif
s32	 Object_GetScreenPositionFar(s32 object, s32 *out);
void ObjectGroup_TickMemberTimers(void);
void Palette_BrightenBgEntries(s32 blue_delta, s32 green_delta, s32 red_delta);
u16	 Palette_ScaleTintChannels(struct WorkspaceWork *work, s32 scale_r, s32 scale_g, s32 scale_b);
#ifndef OWN_PsynergyMenu_CallIconRoutineWithValue
void PsynergyMenu_CallIconRoutineWithValue(s32 arg0, s32 arg1);
#endif
#ifndef OWN_RenderOutput_Create
struct RenderOutput *RenderOutput_Create(
	s32 slot, s32 flags, struct RenderInput *input, s32 offset_x, s32 offset_y);
#endif
void RenderOutput_Release(struct RenderOutput *entry);
#ifndef OWN_RenderResource_CreateFrame
struct RenderOutput *RenderResource_CreateFrame(
	s32 arg0, s32 arg1, struct RenderInput *arg2, s32 arg3, s32 arg4);
#endif
#ifndef OWN_RenderResource_CreatePair
struct RenderOutput *RenderResource_CreatePair(s32 group, struct RenderInput *input, s32 x, s32 y);
#endif
void RenderResource_LoadFrame(s32 index, s32 value, s32 flag);
void Resource_ClearOwnerListAndCounters(void);
s32	 Resource_DecodeType2(const void *source, void *destination);
void Resource_DecompressHalfwords(const void *source, void *destination);
s32	 Resource_LoadIndexedEntryToBuffer(s32 resource, s32 index);
s32	 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1);
s32	 Resource_LoadKind26EntryToBuffer(s32 resource, s32 index);
void Runtime_RemapBytesByTableFar(void *, s32);
void SceneTransform_ApplyPitch(s32 angle);
#ifndef OWN_SceneTransform_ApplyPosition
void SceneTransform_ApplyPosition(const s32 *position);
#endif
void SceneTransform_ApplyRoll(s32 angle);
#ifndef OWN_SceneTransform_ApplyScale
void SceneTransform_ApplyScale(const s32 *scale);
#endif
void SceneTransform_ApplyYaw(s32 angle);
void Scheduler_ScheduleCallbackA(void);
void Scheduler_ScheduleCallbackAAfterFrames(void);
#ifndef OWN_ShopCursor_MoveTowardTargetFar
void ShopCursor_MoveTowardTargetFar(struct ShopCursor *cursor);
#endif
s32 Sprite_ComposeAnimationFrame(struct AnimationObject *obj, u16 dir);
#ifndef OWN_Text_FormatHex
void Text_FormatHex(u32 value, u32 count, u8 *output);
#endif
void Tilemap_ConvertBuffer(void);
#ifndef OWN_Tilemap_DecodeStagedBuffer
void Tilemap_DecodeStagedBuffer(s32 size);
#endif
void Title_PlaceIntroSprites(void);
#ifndef OWN_Title_ShowSplashScreen
s32 Title_ShowSplashScreen(s32 mode);
#endif
#ifndef OWN_Ui_AdjustValueWithoutLimit
void Ui_AdjustValueWithoutLimit(s32 no, s32 value);
#endif
#ifndef OWN_Ui_ApplyTableOffsetToPair
void Ui_ApplyTableOffsetToPair(struct UiBobPairOutput *obj);
#endif
#ifndef OWN_Ui_ApplyTableScaleToObject
void Ui_ApplyTableScaleToObject(struct UiScaleSprite *obj);
#endif
void Ui_BuildPairedPatternsToSlot(s32 no0, s32 no1, s32 *slot, s32 *ret, s32 flag);
s32	 Ui_BuildPatternToSlot(s32 icon, s32 unused, s32 slot);
void Ui_DrawValuePairRows(struct BtlUnit *unit, s32 layer);
void Ui_FillGridColumnFromMetadata(s32 slot, s32 value);
s32	 Ui_FindNextNumberWithMetadata(s32 start, s32 step);
void Ui_SetGridColumnNumber(s32 slot, s32 no);
void Ui_SetGridColumnParameter(s32 slot, s32 value);
void Ui_SetGridColumnPriority(s32 slot, s32 value);
#ifndef OWN_UiGlyph_LoadEntryWithPalette
void UiGlyph_LoadEntryWithPalette(
	u32 icon, s32 unused, s32 *slot, s32 *tile, s32 palette, s32 reuse);
#endif
#ifndef OWN_UiIcon_BuildAbilityIconTiles
void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst, s32 reuse);
#endif
#ifndef OWN_UiIcon_BuildItemIconTiles
void UiIcon_BuildItemIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst, s32 reuse);
#endif
#ifndef OWN_UiIcon_CreateWithLoadedResource
s32 UiIcon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y, s32 resource_id);
#endif
s32 UiIcon_CreateWithResourceVariant(struct RenderInput *window, s32 x, s32 y);
#ifndef OWN_UiIcon_DrawVariantWithTileOffset
s32 UiIcon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y, s32 variant);
#endif
void UiIcon_PrepareObject(struct RenderOutput *object);
s32	 UiText_BuildRenderEntriesMode1(s32 arg0);
#ifndef OWN_UiText_DrawFourNumbersInRow
void UiText_DrawFourNumbersInRow(struct RenderInput *work, struct NumberRow *row);
#endif
void UiText_DrawStatComparison(const struct BtlUnit *alt, const struct BtlUnit *base, s32 work);
void UiText_DrawWorkValueWithLabel(s32 work);
#ifndef OWN_UiText_GetResourceDimensions
s32 UiText_GetResourceDimensions(s32 no, s32 *x, s32 *y, u32 *width, u32 *height);
#endif
void UiText_LoadRemappedGlyph(struct GlyphPalette *palette, s32 glyph, s32 tile);
s32	 UiText_OpenEntryMessageFar(s32 window, s32 message);
#ifndef OWN_UiText_RenderWideStringAtOffset
void UiText_RenderWideStringAtOffset(u16 *text, struct UiWindow *window, s32 x, s32 y);
#endif
void UiTimedNotice_Tick(void);
void UiWindow_BuildLayoutBounds(s32 flags);
#ifndef OWN_UiWindow_ClearInteriorTiles
void UiWindow_ClearInteriorTiles(
	const struct RenderInput *window, u32 left, u32 top, u32 right, u32 bottom);
#endif
void UiWindow_CreateWithLayoutBounds(s32 flags);
#ifndef OWN_UiWindow_DrawThreeTileColumn
int UiWindow_DrawThreeTileColumn(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
#endif
void UiWindow_FillFromSceneFar(u16 *lines);
#ifndef OWN_UiWindow_FillTilemapRect
void UiWindow_FillTilemapRect(struct UiWindow *window, s32 x, s32 y, s32 width, s32 height);
#endif
void UiWindow_InitializeWork(s32 unused);
#ifndef OWN_UiWindow_MapTextCanvasTiles
void UiWindow_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32 full_width);
#endif
void UiWindow_SetPaletteBitRectFar(s32, s32, s32, s32, s32);
void UiWindow_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette);
void UiWindow_UpdateInterpolatedGeometry(void *window, s32 save_position);
s32	 UiWindow_UpdateOrCreate(s32 *handle, s32 first, s32 second, s32 third, s32 fourth, s32 flags);
struct UiChannelSlot *UiWork_ActivateChannel(struct UiWindow *work, s32 value, s32 preserve);
void				  UiWork_AdvanceChannelTransition(struct UiChannelSlot *channel);
s32					  UiWork_CheckCancelByInput(struct UiChannelSlot *channel);
s32					  UiWork_CheckCancelByModeInput(struct UiChannelSlot *channel);
#ifndef OWN_UiWork_CloseAndRelease
void UiWork_CloseAndRelease(void);
#endif
void UiWork_CopyParamsToRenderWork(struct UiChannelSlot *channel);
void UiWork_DrawByAttributes(struct UiWindow *window);
void UiWork_FinalizeEntityMatchingLocalizedId(u32 entry);
s32	 Localization_LookupEntryId(u32 entry);
void UiWork_InitCountersAndScheduleRefresh(s32 initialize);
s32	 UiWork_IsIdle(struct UiWindow *window);
void UiWork_ProcessAll(void);
void UiWork_ResetChannelTransition(struct UiChannelSlot *channel);
s32	 UiWork_StepChannelScript(struct UiChannelSlot *ch);
void UiWork_WaitWindowDuration(struct UiWindow *window);
#endif

#if GAME_TLA
struct RenderOutput;

extern s16		DisplayScroll_Busy;
extern s16		DisplayScroll_LastPosition;
extern u16		DisplayScroll_Position;
extern u32		gCameraSceneParameters[3];
extern u8		gNumberTextBuffer[];
extern s32		RenderResource_PairSourceTable[];
extern const u8 Resource_Decode01Code[];
extern u8		Resource_Decode01CodeSize[];
extern const u8 Resource_Decode2Code[];
extern const u8 Resource_DecodeByteLzCode[];
extern char		ResourceId_UiFixedBlockATiles;
extern char		ResourceId_UiIconTiles;
extern const u8 Text_LookupSymbolCode[];
extern u8		Text_LookupSymbolCodeSize[];
void			Ability_LoadGlyphFar(s32, s32, s32 *, s32 *, s32);
s32				BattleParty_PrepareActiveOwnersFar(u16 *owners);
s32				DisplayScroll_DrawMessageLine(s32 message, s32 slot, s32 align);
void			Resource_ClearOwnerListAndCounters(void);
void			Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output, s32 shift);
s32				ItemIcon_Compose(s32, s32);
void			OwnerAction_AddFar(void);
void			RenderOutput_ClearList(void *work);
void			RenderOutput_Release(struct RenderOutput *node);
#ifndef OWN_Resource_DecodeByteLzInRam
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
#endif
s32	 Resource_LoadByMode(s32 mode, s32 value);
void Runtime_RemapBytesByTableFar(void *buffer, s32 size);
void Text_FormatSignedDecimalToWork(s32);
void Ui_BuildPairedPatternsToSlot(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 arg4);
#ifndef OWN_UiIcon_BuildAbilityIconTiles
void UiIcon_BuildAbilityIconTiles(s32 icon, s32 with_base, s32 *slot, s32 *tile, s32 reuse);
#endif
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 layer, s32 x, s32 y);
void UiText_DrawString(u8 *text, s32 output, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(s32 image, s32 layer, s32 x, s32 y);
#ifndef OWN_UiText_DrawStringInWindowFar
void UiText_DrawStringInWindowFar(u8 *text, s32 arg1, u32 x, u32 y);
#endif
void UiText_RenderWideStringInWindow(u16 *text, s32 work, s32 x, s32 y);
void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);
void UiWindow_DrawDividerLineFar(s32, s32, s32, s32, s32);
void UiWork_AnimateSpriteSlots(void);
void UiWork_InitCountersAndScheduleRefresh(s32 kind);
s32	 UiWork_SetParamNibbleFar(s32 color);
void VramBlock_LoadResourceFar(s32 entry_no, s32 mode, s32 resource_id);
#endif

#endif
