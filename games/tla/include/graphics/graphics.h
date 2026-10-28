#ifndef GRAPHICS_H
#define GRAPHICS_H

/// @file
/// GRAPHICS

#include "types.h"
#include "edition.h"

struct Window;

extern u8 gBlendBrighten;
/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it five functions of tbs's system.c differ in tbs-en:
 * Fade_Start (31), Fade_FromBlack (19),
 * Fade_FromWhite, Fade_ToWhite and
 * Fade_ToBlack (7 each). */
extern volatile u8 gBlendDuration;
extern u8 gBlendVBlanksLeft;
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
extern const u8 gStatusPanel_PpRowString[];
extern s32 gWindow_FixedTileBlocks[];
extern const u8 gStatusPanel_HpRowString[];
extern const u8 gStatusPanel_HpString[];
extern const u8 gStatusPanel_PpString[];
extern const u8 gStatusPanel_SlashString[];
extern u8 *gItemIcons[];
extern u8 *gItemIconsEnd[];
extern u8 *gIcon_MiscIconPointers[];
extern u8 *gPsynergyIcons[];
extern u8 *gPsynergyIconsEnd[];
void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile,
                       s32 reuse);
#ifndef OWN_BattleLayout_HighlightPartyPanels
s32 BattleLayout_HighlightPartyPanels(u16 *ids);
#endif
void PaletteGlow_UpdateSine(void);
void Render_ResetTransformState(void);
s32 Vram_ClearWindowBlock(void);
s32 BattleParty_ListActorIdsFar(s32 side, s32 group);
void Portrait_ReloadFar(s32 a, s32 b, s32 c);
void Icon_DrawMisc(u32 index);
s32 Icon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);
void Text_DrawNumberRightAligned(s32 number, s32 window, s32 x, s32 y);
void UiTextResource_Release(s32 value);
void PartyStatus_Draw(s32 flags);
void Window_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
#ifndef OWN_Window_SetTilemapEntry
void Window_SetTilemapEntry(struct Window *window, s32 value, s32 x, s32 y,
                            u32 mode);
#endif
void WindowSystem_StartFieldUpdate(void);
void Window_UpdateZooms(void);
void Message_UpdateAll(void);
void Portrait_ClearSlots(void);
void WindowSystem_UploadTilemap(void);

struct WindowSprite;

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
void WindowSprite_ClearList(void *work);
void WindowSprite_Release(struct WindowSprite *node);
#ifndef OWN_Resource_DecodeByteLzInRam
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
#endif
s32 Resource_LoadByMode(s32 mode, s32 value);
void SpriteColor_RemapPixelsFar(void *buffer, s32 size);
void NumberText_FormatDecimal(s32);
void Icon_BuildCommandIconTiles(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3,
                                s32 arg4);
#ifndef OWN_Icon_BuildAbilityIconTiles
void Icon_BuildAbilityIconTiles(s32 icon, s32 with_base, s32 *slot, s32 *tile,
                                s32 reuse);
#endif
void Text_DrawNumberAtOffsetFar(s32 value, s32 width, s32 window, s32 x, s32 y);
void Text_DrawNumberInWindowFar(s32 value, s32 width, s32 window, s32 x, s32 y);
void Text_DrawString(u8 *text, s32 window, s32 x, s32 y);
void Text_DrawStringAtOffsetFar(const u8 *text, s32 window, s32 offsetX,
                                s32 offsetY);
#ifndef OWN_Text_DrawStringInWindowFar
void Text_DrawStringInWindowFar(u8 *text, s32 window, u32 x, u32 y);
#endif
void Text_RenderWideStringInWindow(u16 *text, s32 window, s32 x, s32 y);
void Window_ClearTextTilesInRect(s32 x, s32 y, u32 width, u32 height);
void Window_DrawDividerLineFar(s32, s32, s32, s32, s32);
void WindowSprite_UpdateAll(void);
void WindowSystem_StartBattleUpdate(s32 kind);
s32 Text_SetColorFar(s32 color);
void VramBlock_LoadResourceFar(s32 entry_id, s32 mode, s32 resourceId);

#endif
