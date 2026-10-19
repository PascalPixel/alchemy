#ifndef GRAPHICS_H
#define GRAPHICS_H

/// @file
/// The screen blend's globals and the graphics routines.

#include "types.h"
#include "edition.h"
#include "farcall.h"

struct UiWindow;

extern u8 gBlendBrighten;
/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it five functions of tbs's runtime.c differ in tbs-en:
 * Fade_Start (31), Fade_FromBlack (19),
 * Fade_FromWhite, Fade_ToWhite and
 * Fade_ToBlack (7 each). */
extern volatile u8 gBlendDuration;
extern u8 gBlendFramesLeft;
extern u16 gBlendLayers;
extern u8 gBlendStartLevel;
/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it Fade_Update: differ 29 in tbs-en, 45 instructions
 * for 46. */
extern volatile u8 gBlendTargetLevel;
extern u8 gRomShiftedTilePair[];
extern const u32 RelocatedQ16DotProduct[];
extern u8 Resource_Decode2CodeSize[];
extern u8 Resource_DecodeByteLzCodeSize[];
extern const u8 gUi_PpRowString[];
extern s32 gUi_FixedTileBlocks[];
extern const u8 gUi_HpRowString[];
extern const u8 gUi_HpString[];
extern const u8 gUi_PpString[];
extern const u8 gUi_SlashString[];
extern u8 *gUiIcon_ItemIconPointers[];
extern u8 *gUiIcon_ItemIconPointersEnd[];
extern u8 *gUiIcon_MiscIconPointers[];
extern u8 *gUiIcon_PsynergyIconPointers[];
extern u8 *gUiIcon_PsynergyIconPointersEnd[];

struct AnimationObject;
struct ArticleTable;
struct BattleUnit;
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

extern u8 ColorBuffer_BackupAndBrightenCodeSize[];
extern u8 ColorBuffer_BackupAndBrightenPartialCodeSize[];
extern u8 ColorBuffer_BackupAndDarkenCodeSize[];
extern u8 ColorBuffer_BackupAndDarkenPartialCodeSize[];
extern u8 ColorBuffer_BackupAndHalveCodeSize[];
extern u8 ColorBuffer_BackupAndHalveNonzeroCodeSize[];
extern u8 ColorBuffer_BackupAndScaleNonzeroThreeQuartersCodeSize[];
extern u8 ColorBuffer_BackupAndScaleThreeQuartersCodeSize[];
extern const u8 ColorBuffer_BrightenKernel[];
extern const u8 ColorBuffer_BrightenPartialKernel[];
extern const u8 ColorBuffer_DarkenKernel[];
extern const u8 ColorBuffer_DarkenPartialKernel[];
extern const u8 ColorBuffer_HalveKernel[];
extern const u8 ColorBuffer_HalveNonzeroKernel[];
extern const u8 ColorBuffer_ScaleNonzeroThreeQuartersKernel[];
extern const u8 ColorBuffer_ScaleThreeQuartersKernel[];
#ifndef OWN_gRenderSlotHeads
extern void *gRenderSlotHeads[];
#endif
extern u8 gPartyStatusWork[];
extern u8 gSelectionWorkspace[];
extern const struct ArticleTable gUiText_ArticleTable;
struct TileMask {
  u32 word0;
  u32 word1;
};
extern const struct TileMask gMenu_BarPatternRows[];
extern const u8 gDisplayScroll_Font[];
extern const u8 gDisplayTransition_DitherTable[];
extern s8 gDisplayTransition_EdgeOffsets[];
extern u8 gDisplayTransition_NoiseWidths[];
extern const u8 FarCall_ResourceTable[];
extern u8 gDecodeFillByte;
extern struct PaletteSnapshotWork *gPaletteWork;
extern u16 gGraphics_ExpandNibbleTable[];
extern struct AnimationObject *gSpriteObjects;
#ifndef OWN_Menu_CursorObjectTiles
extern const u8 gMenu_CursorObjectTiles[];
#endif
extern const u8 gNameEntry_AlternateKeyboardTilemap[];
extern s8 gNameEntry_CaretWave[];
extern const u8 gNameEntry_VoicedMarkRanges[];
extern u8 gNameEntry_DeleteKeyLabel[];
extern u8 gNameEntry_EndKeyLabel[];
extern const u8 gNameEntry_SemivoicedMarkRanges[];
extern const u8 gNameEntry_KeyboardTilemap[];
extern const u8 gRender_DecodeFrameBuffer[];
extern const u8 Resource_Decode2[];
extern const u8 Resource_DecodeByteLzArm[];
extern const u8 Resource_DecodeHalfwordLz[];
extern const u8 Resource_DecodeType01Arm[];
extern u8 Resource_DecodeType01CodeSize[];
extern u8 Resource_DecompressHalfwordsCodeSize[];
extern u8 gResource_FixedBlockBTiles[];
extern const u8 gResource_FixedBlockATiles[];
extern const u8 BattleEffect_Expand[];
extern u8 BattleEffect_ExpandCodeSize[];
extern const u8 Text_DecodeSymbol[];
extern u8 Text_DecodeSymbolCodeSize[];
extern u8 Tile_BuildMetatilesCodeSize[];
extern const u8 Tile_ConvertMap[];
extern u8 Tile_ConvertMapCodeSize[];
extern const u8 Tile_Decompress4bpp[];
extern u8 Tile_Decompress4bppCodeSize[];
extern const u8 Tile_ExpandMasked[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern const u8 Tile_ExpandOpaque[];
extern u8 Tile_ExpandOpaqueCodeSize[];
extern const u8 gTitle_PromptTiles[];
extern s32 gUi_ObjectPulseScales[];
extern const u8 gUi_PairBobOffsets[];
extern u8 *gUiIcon_DigitPointers[];
extern u8 *gUiIcon_FramePointerTable[];
extern u8 *gUiIcon_MarkPointers[];
extern u8 *gUiIcon_OverlayPointerTable[];
extern u8 gUiIcon_ResourceTiles[];
extern const s8 gUiSprite_BounceOffsets[];
extern const u8 gUiSprite_CursorFrameTiles[];
extern const s8 gUiSprite_WobbleOffsets[][2];
extern const u8 gUiText_ColourPens[];
extern u8 UiText_DrawGlyphCodeSize[];
extern const u8 gUiText_GlyphWidths[];
extern const struct ArticleTable gUiText_IndefiniteArticles;
extern const u8 UiText_LookupMessageArm[];
extern u8 UiText_LookupMessageCodeSize[];
extern const s8 gUiWindow_PartyColumnOffsets[];
#ifndef OWN_FarCall_EffectTable
s32 FarCall_EffectTable(s32 slot, s32 width_shift, s32 height_shift, s32 flags,
                        u32 mode);
#endif

#endif
