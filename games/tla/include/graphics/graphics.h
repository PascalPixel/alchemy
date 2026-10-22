#ifndef GRAPHICS_H
#define GRAPHICS_H

/// @file
/// GRAPHICS

#include "types.h"
#include "edition.h"

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
void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile,
                       s32 reuse);
#ifndef OWN_BattleLayout_HighlightPartyPanels
s32 BattleLayout_HighlightPartyPanels(u16 *ids);
#endif
void PaletteGlow_UpdateSine(void);
void Render_ResetTransformState(void);
s32 Ui_ClearVramBlock(void);
s32 BattleParty_ListActorIdsFar(s32 side, s32 group);
void Ui_LoadCharacterEntryForSlotFar(s32 a, s32 b, s32 c);
void Ui_PrepareTransferFromTableEntry(u32 index);
s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);
void UiText_DrawNumberRightAligned(s32 number, s32 window, s32 x, s32 y);
void UiTextResource_Release(s32 value);
void PartyStatus_Draw(s32 flags);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
#ifndef OWN_UiWindow_SetTilemapEntry
void UiWindow_SetTilemapEntry(struct UiWindow *window, s32 value, s32 x, s32 y,
                              u32 mode);
#endif
void UiWork_InitCountersWithResourceAndScheduleRefresh(void);
void UiWork_ProcessDirectWork(void);
void UiWork_ProcessRenderChannels(void);
void UiWork_ClearPortraitSlots(void);
void UiWork_UploadDirtyBlocks(void);

struct RenderOutput;

extern s16 gDisplayScroll_Busy;
extern s16 gDisplayScroll_LastPosition;
extern u16 gDisplayScroll_Position;
extern u32 gCameraSceneParameters[3];
extern u8 gNumberTextBuffer[];
extern s32 gRenderResource_PairSourceTable[];
extern const u8 Resource_Decode01Code[];
extern u8 Resource_Decode01CodeSize[];
extern const u8 Resource_Decode2Code[];
extern const u8 Resource_DecodeByteLzCode[];
extern char ResourceId_UiFixedBlockATiles;
extern char ResourceId_UiIconTiles;
extern const u8 Text_LookupSymbolCode[];
extern u8 Text_LookupSymbolCodeSize[];
void Ability_LoadGlyphFar(s32, s32, s32 *, s32 *, s32);
s32 BattleParty_PrepareActiveOwnersFar(u16 *owners);
s32 DisplayScroll_DrawMessageLine(s32 message, s32 slot, s32 align);
void Resource_ClearOwnerListAndCounters(void);
void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output,
                                   s32 shift);
s32 ItemIcon_Compose(s32, s32);
void Unit_LearnPsynergyFar(void);
void RenderOutput_ClearList(void *work);
void RenderOutput_Release(struct RenderOutput *node);
#ifndef OWN_Resource_DecodeByteLzInRam
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
#endif
s32 Resource_LoadByMode(s32 mode, s32 value);
void Runtime_RemapBytesByTableFar(void *buffer, s32 size);
void NumberText_FormatDecimal(s32);
void Ui_BuildPairedPatternsToSlot(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3,
                                  s32 arg4);
#ifndef OWN_UiIcon_BuildAbilityIconTiles
void UiIcon_BuildAbilityIconTiles(s32 icon, s32 with_base, s32 *slot, s32 *tile,
                                  s32 reuse);
#endif
void UiText_DrawNumberAtOffsetFar(s32 value, s32 width, s32 window, s32 x,
                                  s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 width, s32 window, s32 x,
                                  s32 y);
void UiText_DrawString(u8 *text, s32 window, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(const u8 *text, s32 window, s32 offsetX,
                                  s32 offsetY);
#ifndef OWN_UiText_DrawStringInWindowFar
void UiText_DrawStringInWindowFar(u8 *text, s32 window, u32 x, u32 y);
#endif
void UiText_RenderWideStringInWindow(u16 *text, s32 window, s32 x, s32 y);
void UiWindow_ClearTileAttributesInRect(s32 x, s32 y, u32 width, u32 height);
void UiWindow_DrawDividerLineFar(s32, s32, s32, s32, s32);
void UiWork_AnimateSpriteSlots(void);
void UiWork_InitCountersAndScheduleRefresh(s32 kind);
s32 UiWork_SetParamNibbleFar(s32 color);
void VramBlock_LoadResourceFar(s32 entry_id, s32 mode, s32 resourceId);

#endif
