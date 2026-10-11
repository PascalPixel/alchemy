#ifndef GRAPHICS_H
#define GRAPHICS_H

/* GRAPHICS */

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

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
#if GAME_TLA
void			   Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile, s32 reuse);
#endif
#ifndef OWN_BattleLayout_HighlightPartyPanels
#if GAME_TLA
s32 BattleLayout_HighlightPartyPanels(u16 *ids);
#endif
#endif
#if GAME_TLA
void PaletteGlow_UpdateSine(void);
void Render_ResetTransformState(void);
s32	 Ui_ClearVramBlock(void);
#endif
#if GAME_TLA
s32	 BattleParty_ListActorIdsFar(s32 side, s32 group);
void Ui_LoadCharacterEntryForSlotFar(s32 a, s32 b, s32 c);
#endif
#if GAME_TLA
void Ui_PrepareTransferFromTableEntry(u32 index);
s32	 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);
void UiText_DrawNumberRightAlignedFar(s32 number, s32 layer, s32 x, s32 y);
void UiTextResource_Release(s32 value);
void PtyStDraw(s32 flags);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
#endif
#ifndef OWN_UiWindow_SetTilemapEntry
#if GAME_TLA
void UiWindow_SetTilemapEntry(struct UiWindow *window, s32 value, s32 x, s32 y, u32 mode);
#endif
#endif
#if GAME_TLA
void UiWork_InitCountersWithResourceAndScheduleRefresh(void);
void UiWork_ProcessDirectWork(void);
void UiWork_ProcessRenderChannels(void);
void UiWork_SetTwoEntriesTo999(void);
void UiWork_UploadDirtyBlocks(void);
#endif

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
extern u8						 UiText_DrawGlyphCodeSize[];
extern const u8					 UiText_GlyphWidths[];
extern const struct ArticleTable UiText_IndefiniteArticles;
extern const u8					 UiText_LookupMessageArm[];
extern u8						 UiText_LookupMessageCodeSize[];
extern const u8					 UiText_SpeedCueGaps[3];
extern const u8					 UiText_SpeedDelays[3];
extern const u8					 UiText_SpeedSteps[3];
extern const s8					 UiWindow_PartyColumnOffsets[];
#ifndef OWN_BattleFx_PlayUnitElementEffect
#endif
#ifndef OWN_Camera_StoreSceneParameters
#endif
#ifndef OWN_ColorBuffer_BackupAndBrighten
#endif
#ifndef OWN_ColorBuffer_BackupAndDarken
#endif
#ifndef OWN_ColorBuffer_BackupAndHalve
#endif
#ifndef OWN_ColorBuffer_BackupAndScaleThreeQuarters
#endif
#ifndef OWN_DisplayBlend_StartScript
#endif
#ifndef OWN_DisplayScroll_InitializeHBlankDma
#endif
#ifndef OWN_FarCall_EffectTable
s32 FarCall_EffectTable(s32 slot, s32 width_shift, s32 height_shift, s32 flags, u32 mode);
#endif
#ifndef OWN_Graphics_BuildSequentialTileTable
#endif
#ifndef OWN_Graphics_TransformSmallPalette
#endif
#ifndef OWN_Graphics_TransposeCopy
#endif
#ifndef OWN_MenuSelection_BuildEntries
#endif
#ifndef OWN_PsynergyMenu_CallIconRoutineWithValue
#endif
#ifndef OWN_RenderOutput_Create
#endif
#ifndef OWN_RenderResource_CreateFrame
#endif
#ifndef OWN_RenderResource_CreatePair
#endif
#ifndef OWN_SceneTransform_ApplyPosition
#endif
#ifndef OWN_SceneTransform_ApplyScale
#endif
#ifndef OWN_Text_FormatHex
#endif
#ifndef OWN_Tilemap_DecodeStagedBuffer
#endif
#ifndef OWN_Title_ShowSplashScreen
#endif
#ifndef OWN_Ui_AdjustValueWithoutLimit
#endif
#ifndef OWN_Ui_ApplyTableOffsetToPair
#endif
#ifndef OWN_Ui_ApplyTableScaleToObject
#endif
#ifndef OWN_UiGlyph_LoadEntryWithPalette
#endif
#ifndef OWN_UiIcon_BuildAbilityIconTiles
#endif
#ifndef OWN_UiIcon_BuildItemIconTiles
#endif
#ifndef OWN_UiIcon_CreateWithLoadedResource
#endif
#ifndef OWN_UiIcon_DrawVariantWithTileOffset
#endif
#ifndef OWN_UiText_DrawFourNumbersInRow
#endif
#ifndef OWN_UiText_GetResourceDimensions
#endif
#ifndef OWN_UiText_RenderWideStringAtOffset
#endif
#ifndef OWN_UiWindow_ClearInteriorTiles
#endif
#ifndef OWN_UiWindow_DrawThreeTileColumn
#endif
#ifndef OWN_UiWindow_FillTilemapRect
#endif
#ifndef OWN_UiWindow_MapTextCanvasTiles
#endif
#ifndef OWN_UiWork_CloseAndRelease
#endif
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
