#ifndef ALCHEMY_UNIT_WINDOW_WINDOW_H
#define ALCHEMY_UNIT_WINDOW_WINDOW_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

struct AbilityListEntry;
struct BattleActionRecord;
struct BattleAct;
struct GlyphPalette;
struct MenuSprite;
struct OwnerActionSlot;
struct UiWindow;
struct RenderOutput;
struct RenderOutputList;
struct SaveSummary;
struct SelectionNode;
struct SelectionScreen;
struct SelectionWork;
struct TextReader;
struct UiChannelSlot;
struct WorkspaceWork;

/// The flags of a UI window (struct UiWindow.flags), which UiWindow_Create
/// takes as its attributes.
enum {
  UI_WINDOW_OPEN = 0x1,    ///< the window slot is in use
  UI_WINDOW_INSTANT = 0x2, ///< opens and closes at once, with no zoom
  /// text is drawn into a canvas of bitmap tiles (UiWindow_MapTextCanvasTiles)
  UI_WINDOW_TEXT_CANVAS = 0x8,
  /// the canvas has a frame and a filled background; without it the canvas is
  /// transparent and frameless
  UI_WINDOW_FRAMED_CANVAS = 0x20,
  UI_WINDOW_OUTLINED_TEXT = 0x40, ///< the canvas text gets an outline
  /// when a new page resizes the window, its bottom edge stays put
  UI_WINDOW_ANCHOR_BOTTOM = 0x80,
  /// when a new page resizes the window, its left edge stays put
  UI_WINDOW_FIXED_X = 0x100,
};

// The text canvas tiles in VRAM: 120 4bpp tiles from tile 0x128, enough for
// the interior of a full-width window six tiles tall. The tiles run down
// columns, each as tall as the window's interior.
#define UI_CANVAS_VRAM ((void *)0x06002500)
#define UI_CANVAS_SIZE 0xf00
#define UI_CANVAS_FIRST_TILE 0x128

/// The UI tilemap in VRAM (BG screen block 4), which UiWork_UploadDirtyBlocks
/// copies the work block's tilemap to.
#define UI_TILEMAP_VRAM ((u8 *)0x06002000)

/// A tilemap entry of palette 15, the UI palette.
#define UI_TILE(tile) (0xf000 | (tile))
/// Two copies of a tilemap entry, for filling a run of entries a word at a
/// time.
#define UI_TILE_PAIR(entry) ((u32)(entry) << 16 | (entry))

// The two page arrows beside a paged list's page numbers, in the menus and
// the clear menu: tiles 0x128 and 0x129 of character block 0, which
// Link_DrawShiftedTilePair slides back and forth each frame, and their
// tilemap entries.
#define UI_PAGE_ARROW_TILE 0x128
#define UI_PAGE_ARROW_VRAM                                                     \
  ((s32)BG_CHAR_BLOCK(0) + UI_PAGE_ARROW_TILE * TILE_SIZE_4BPP)
#define UI_PAGE_ARROW_LEFT UI_TILE(UI_PAGE_ARROW_TILE)
#define UI_PAGE_ARROW_RIGHT UI_TILE(UI_PAGE_ARROW_TILE + 1)

/// The window frame's tiles. While UiRenderWork.mode is set (a battle effect
/// sets it while it greys palette 15 and scrolls BG0 by scanline) the corners
/// are one rounded tile, flipped for each corner.
enum {
  UI_TILE_BLANK = UI_TILE(0x00),
  UI_TILE_FRAME_TOP_LEFT = UI_TILE(0x10),
  UI_TILE_FRAME_TOP = UI_TILE(0x11),
  UI_TILE_FRAME_TOP_RIGHT = UI_TILE(0x12),
  UI_TILE_FRAME_BOTTOM_LEFT = UI_TILE(0x13),
  UI_TILE_FRAME_BOTTOM = UI_TILE(0x14),
  UI_TILE_FRAME_BOTTOM_RIGHT = UI_TILE(0x15),
  UI_TILE_FRAME_LEFT = UI_TILE(0x16),
  UI_TILE_FRAME_RIGHT = UI_TILE(0x17),
  UI_TILE_FRAME_ROUND_CORNER = UI_TILE(0x1c),
  UI_TILE_WINDOW_FILL = UI_TILE(0x20),
  // Divider lines (UiWindow_DrawDividerLine) and their joins.
  UI_TILE_DIVIDER_TEE_DOWN = UI_TILE(0x09), ///< a vertical line leaving below
  UI_TILE_DIVIDER_TEE_UP = UI_TILE(0x0a),   ///< a vertical line leaving above
  /// a horizontal line leaving right
  UI_TILE_DIVIDER_TEE_RIGHT = UI_TILE(0x0b),
  UI_TILE_DIVIDER_TEE_LEFT = UI_TILE(0x0c), ///< a horizontal line leaving left
  UI_TILE_DIVIDER_CROSS = UI_TILE(0x0d),
  UI_TILE_DIVIDER_HORIZONTAL = UI_TILE(0x0e),
  UI_TILE_DIVIDER_VERTICAL = UI_TILE(0x0f),
  UI_TILE_FRAME_TOP_TEE = UI_TILE(0x18),    ///< the top edge meeting a line
  UI_TILE_FRAME_BOTTOM_TEE = UI_TILE(0x19), ///< the bottom edge meeting a line
  UI_TILE_FRAME_LEFT_TEE = UI_TILE(0x1a),   ///< the left edge meeting a line
  UI_TILE_FRAME_RIGHT_TEE = UI_TILE(0x1b),  ///< the right edge meeting a line
  /// In battle, the bottom frame tiles' lower rows, which UiWork_Initialize
  /// copies to tiles 128 to 130.
  UI_TILE_BATTLE_FRAME_BOTTOM_LEFT = UI_TILE(0x80),
  UI_TILE_BATTLE_FRAME_BOTTOM = UI_TILE(0x81),
  UI_TILE_BATTLE_FRAME_BOTTOM_RIGHT = UI_TILE(0x82),
};

/// The tilemap entry flip bits.
#define TILE_HFLIP 0x400
#define TILE_VFLIP 0x800

/// The DMA control word that fills count words from one fixed source word.
#define DMA_FILL32(count)                                                      \
  ((DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 | (count))
/// The DMA control word that copies count words.
#define DMA_COPY32(count) ((DMA_ENABLE | DMA_32BIT) << 16 | (count))
/// The DMA control word that copies count halfwords.
#define DMA_COPY16(count) ((DMA_ENABLE << 16) | (count))

/// How a render output's sprite moves (struct RenderOutput.active), run each
/// frame by UiWork_AnimateSpriteSlots. 0 is a free output.
enum {
  UI_SPRITE_STILL = 1,
  UI_SPRITE_PAGE_ARROW = 2,   ///< the animated next-page arrow, bobbing
  UI_SPRITE_WOBBLE = 4,       ///< swings along a small arc
  UI_SPRITE_SHAKE = 5,        ///< jitters by a pixel every other frame
  UI_SPRITE_POP = 6,          ///< shown at double size, then normal size
  UI_SPRITE_SPIN = 7,         ///< turns while circling its place
  UI_SPRITE_PULSE = 8,        ///< shown at 1.25 times, then normal size
  UI_SPRITE_SWELL = 9,        ///< pulses between 1 and 1.5 times its size
  UI_SPRITE_SWELL_HALF = 10,  ///< the same at half the scale
  UI_SPRITE_SHRINK = 11,      ///< shrinks from 1.5 times to its size, once
  UI_SPRITE_SHRINK_HALF = 12, ///< the same at half the scale
  UI_SPRITE_HIDDEN = 13,      ///< not drawn
  UI_SPRITE_HOP_DOWN = 14,    ///< 14 to 16: hops down and back
  UI_SPRITE_HOP_UP = 17,
  UI_SPRITE_HOP_DOWN_LEFT = 18,
};

/// The kinds of message argument (UiWork_PushValueSlot), which the message
/// codes such as {argument_item_name} read.
enum {
  MSG_ARG_ACTOR = 1,
  MSG_ARG_ITEM = 2,
  MSG_ARG_CLASS = 3,
  MSG_ARG_ABILITY = 4,
  MSG_ARG_NUMBER = 5,
  MSG_ARG_LOCATION = 6,
};

struct RenderOutput *RenderOutput_AcquireFree(void);
void RenderOutput_ReleaseFree(struct RenderOutput *out);
void RenderOutput_InitFreeList(void);
void UiWork_InitializeWithResourceCounters(void);
void UiWork_Initialize(s32 loadGlyphTiles);
void UiWork_UploadDirtyBlocks(void);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void UiWindow_DrawOpened(struct UiWindow *window);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 attrs);
void UiWindow_WaitZoom(struct UiWindow *window);
void UiWork_Finalize(struct UiWindow *window, s32 instant);
void RenderOutput_PrepareForRedraw(struct UiWindow *window);
void RenderOutput_RedrawSavedRect(struct UiWindow *window);
void RenderOutput_ClearList(void *outputs);
void UiWindow_ClearInteriorTiles(const struct UiWindow *window, u32 left,
                                 u32 top, u32 right, u32 bottom);
void RenderOutputList_UpdateTail(struct RenderOutputList *list);
// RenderOutput_AppendToList is called from battle_command.c with no prototype
// in scope: in one file its definition would come before RenderOutput_Create
// and DjinnMenu_ShowChangePreview, and RenderOutput_Create changes (2 of 61
// instructions in tbs-en). Split evidence for window|battle_command.
void RenderOutput_Release(struct RenderOutput *entry);
// UiText_QueueRenderEntries is called from battle_command.c with no prototype
// in scope: Party_ShowJoinedMessage, Party_ShowPairJoinedMessage and
// Djinn_ShowJoinedMessage pass five of its six arguments, and in one file,
// with its definition first, they would not compile. Split evidence for
// window|battle_command.
struct UiChannelSlot *UiText_StartBattleMessage(struct UiWindow *window,
                                                s32 entry, s32 keepPosition);
s32 Ui_ClearVramBlock(void);
s32 Ui_FillVramBlockPattern(void);
void UiWork_ResetFreeChannel(void);
void UiText_ApplyChannelStyle(struct UiChannelSlot *channel);
void UiText_StartCanvasScroll(struct UiChannelSlot *channel);
void UiText_ScrollCanvasUp(s32 step);
void UiWork_ProcessRenderChannels(void);
s32 UiWork_StepChannelScript(struct UiChannelSlot *ch);
void UiWork_ProcessDirectWork(void);
void UiWindow_DrawZoomFrame(void *windowArg, s32 savePosition);
u16 *Memory_FillHalfwordsDma(u16 *dst, s32 value, s32 count);
void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height);
void UiWindow_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height,
                                 s32 fullWidth);
s32 UiWork_IsComplete(void);
s32 UiWork_IsIdle(struct UiWindow *window);
void UiWork_ResetCounters();
void UiWork_InitCountersWithResourceAndScheduleRefresh(void);
void UiWork_InitCountersAndScheduleRefresh(s32 loadGlyphTiles);
void UiWork_FinalizeSharedSlot(void);
void UiText_PrepareMessageWork(s32 argument);
void UiText_ShowMessageAndWaitCore(s32 argument);
s32 UiText_OpenEntryMessage(struct UiWindow *window, s32 argument);
void UiWork_SetBusyFlags(s32 flags);
struct UiWindow *UiText_OpenMessageWindow(s32 message, s32 x, s32 y,
                                          u32 packed);
void UiText_ShowPositionedMessageAndWait(s32 no, s32 flags);
void UiWork_ProcessAll(void);
#if TBS_PATCH_1
s32 UiText_RenderGlyphPair(s32 code, u32 *out);
#else
s32 UiText_RenderGlyphPair(u32 code, u32 *out);
#endif
s32 UiText_GetWideStringWidth(u16 *text);
void UiText_RenderWideStringAtOffset(u16 *text, struct UiWindow *window, s32 x,
                                     s32 y);
void UiText_RenderGlyphTileAtWorkOffset(u16 *buffer,
                                        const struct UiWindow *work,
                                        s32 offsetX, s32 offsetY);
// UiText_RenderWideStringInWindow is called from battle_command.c with no
// prototype in scope: in one file its definition would come before
// DjinnMenu_ShowChangePreview's 17 calls, and that function changes by 18
// bytes in tbs-en. Split evidence for window|battle_command.
u8 *UiText_FormatNumber(u8 *buffer, s32 input, s32 width);
u32 UiText_AppendArticleName(s32 mode, u16 *name, u32 pos, u16 *entry, s32 no,
                             s32 plural, s32 *suffix);
s32 UiText_BuildRenderEntries(s32 script, s32 clear);
void UiWindow_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph,
                          u16 *spacing, u32 flags);
void UiText_MeasureResourceEntries(s32 no, s32 *x, s32 *y);
s32 UiText_GetResourceDimensions(s32 no, s32 *x, s32 *y, u32 *width,
                                 u32 *height);
s32 UiText_GetResourceDimensionsAlt(s32 no, s32 *x, s32 *y, u32 *width,
                                    u32 *height);
void UiText_MeasureEntryDimensions(s32 pos, u32 *out_width, u32 *out_height,
                                   u16 *spacing);
void UiText_MeasureAllPages(s32 start, u32 *out_width, u32 *out_height,
                            u16 *spacing);
s32 UiText_DrawGlyph(struct UiWindow *window, s32 character, s32 x, s32 y,
                     s32 mode);
// UiWindow_PutGlyph is called from battle_command.c with no prototype in
// scope, as in window.c above its definition: PartyStatusDraw,
// SummonMenu_SelectSummon and Battle_CollectPartyCommands pass four of its
// five arguments, and in one file, with its definition first, they would not
// compile. Split evidence for window|battle_command.
void UiWindow_SetTilemapEntry(struct UiWindow *window, s32 value, s32 x, s32 y,
                              u32 mode);
void RenderOutput_UpdateScaleAnimation(struct RenderOutput *output);
void UiWork_AnimateSpriteSlots(void);
void Ui_AdjustValueWithoutLimit(s32 message, u16 *buf);
s32 UiText_CopyMessageString(s32 key, u16 *destination, u32 capacity);
void UiText_DecodeMessage(s32 message, u16 *text, s32 capacity);
s32 *Runtime_ClearWordIfSet(s32 *p);
void UiWork_DrainPending(s32 unused);
void UiWindow_StepSwellEffect(struct UiChannelSlot *ch);
void UiWork_ClearValueNameTables(void);
void UiWork_PushValueSlot(u32 value, u32 kind);
u32 UiText_LookupArgument(u32 kind, u32 clear);
s32 UiText_CheckPauseSkipped(struct UiChannelSlot *ch);
s32 UiText_CheckTextDismissed(struct UiChannelSlot *ch);
void UiWork_FinalizePendingCore(void);
void UiText_ShowCenteredMessage(s32 message, s32 mode, s32 yOffset);
s32 UiText_BuildRenderEntriesMode1(s32 arg0);
void UiText_LookupMessage(struct TextReader *reader, s32 message);
s32 UiText_DecodeSymbol(struct TextReader *reader);
void UiWork_ClearPortraitSlots(void);
s32 Localization_LookupEntryId(u32 value);
struct UiWindow *UiWindow_CreateWithSideObject(s32 arg0, s32 arg1, s32 x,
                                               s32 y);
void UiWork_FinalizeEntityMatchingLocalizedId(u32 entry);

#define UI_WORK ((struct UiRenderWork *)gWindowWork[0])

#define ACTION_ID_MASK 0x3fff

/// Defines name(no, x, y, width, height): decodes message no and fits a window
/// for it on the screen near tile (x, y) (UiWindow_FitOnScreen with flags),
/// returning its place and size in tiles. Returns 0 for an empty message.
#define DEFINE_GET_RESOURCE_DIMENSIONS(name, flags)                            \
  s32 name(s32 no, s32 *x, s32 *y, u32 *width, u32 *height) {                  \
    struct UiRenderWork *work;                                                 \
    s32 idx;                                                                   \
                                                                               \
    work = UI_WORK;                                                            \
    idx = UiText_BuildRenderEntries(no, 0);                                    \
    if (work->entries[idx] == 0) {                                             \
      return 0;                                                                \
    }                                                                          \
    UiWindow_FitOnScreen(idx, x, y, width, height, 0, flags);                  \
    return 1;                                                                  \
  }

s32 UiIcon_CountItemIcons(void);
s32 UiIcon_CountPsynergyIcons(void);
void Ui_BuildPairedPatternsToSlot(s32 no0, s32 no1, s32 *slot, s32 *ret,
                                  s32 flag);
void Ability_RequestGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile,
                          s32 reuse);
void UiIcon_BuildItemIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst,
                               s32 reuse);
s32 ItemIcon_Compose(u32 code, u32 layers);
s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2);
s32 Ui_BuildPatternToSlot(s32 icon, s32 unused, s32 slot);
s32 RenderResource_LoadTableEntry(u32 value, s32 unused, u32 slot);
void Ui_PrepareTransferForItem(s32 code);
void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile,
                       s32 reuse);
void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 with_base, s32 *src, s32 *dst,
                                  s32 reuse);
void Ui_PrepareTransferFromTableEntry(u32 index);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse);
void UiGlyph_ReservedNoOp(void);
void UiIcon_Decode(GlyphTransfer *glyph, s32 outlined);

void UiGlyph_ResetWorkState(void);
void Resource_ClearOwnerListAndCounters(void);
void Resource_PushPendingPair(u32 first, u32 second);
void MenuSelection_BuildEntries(u32 unused);
void UiWork_ReservedNoOp(void);
struct SelectionNode *SelectionScreen_FindFreeIcon(s32 kind);
void Resource_ScheduleOwnerResetDelayed(void);
void Resource_ScheduleOwnerReset(void);
void SelectionScreen_Draw(void);
void SelectionScreen_DrawArrow(struct SelectionScreen *state, s32 index);
void Menu_OpenSelectionWindow(s32 mode, u32 count);
void Resource_ResetOwnerEntries(void);
void Menu_SetNodeCoordinates(u32 first, u32 second);
void Menu_SetupSelectionBothSides(void);
void SelectionScreen_SetupArrow(struct SelectionScreen *state, s32 index);
struct SelectionNode *Selection_GetCursorNode(struct SelectionScreen *screen);
s32 Menu_SelectionLoop(s32 mode);
u32 Menu_WaitForSelectionInput(u32 mode);
void Menu_MoveSelectionForward(struct SelectionScreen *screen);
void Menu_MoveSelectionBackward(struct SelectionScreen *screen);
void Menu_StepRight(struct SelectionScreen *state);
void Menu_StepLeft(struct SelectionScreen *state);
void SelectionScreen_RedrawIcon(struct SelectionScreen *state, u32 index);
void SelectionScreen_HighlightIcon(struct SelectionScreen *state, u32 index);
void Menu_SendNodeCountList(struct SelectionScreen *screen);
void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward);
s32 Resource_LoadByMode(s32 mode, s32 value);
s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option);
void SelectionScreen_SetupIcon(u32 kind, s32 base, struct SelectionNode *node,
                               s32 reuse);
u32 Menu_ConfirmSelection(struct SelectionScreen *screen);
void UiTextResource_NoOpCallback(void);
void UiTextResource_NoOpCallbackBank(void);
void UiTextResource_NoOpCallback1(void);
void UiTextResource_NoOpCallback2(void);
void UiTextResource_NoOpCallback3(void);
void UiTextResource_NoOpCallback4(void);
void UiTextResource_Initialize(struct MenuSprite *object, s32 *slot);
void UiTextResource_SetPosition(struct MenuSprite *obj, s32 x, s32 y);
void UiTextResource_Release(s32 value);
void SelectionScreen_LoadLargeIcon(void);
void Resource_ResetPendingTransfer(void);
s32 Menu_RunTopSelection(void);
void UiWindow_OpenMode1AndWaitFrame(void);
// The matching build calls this before its void definition and uses the
// result. Menu_RunTopSelection keeps that form outside UBFIX.
#ifdef UBFIX
void UiWork_CloseAndRelease(void);
#endif
void SelectionScreen_CreateEmpty(void);
u32 Menu_RunSelectionForValue(u32 value);

void UiTimedNotice_Create(void);
void UiTimedNotice_Tick(void);
void UiTimedNotice_CloseIfActive(void);
s32 Item_RemoveFromInventory(s32 unused, s32 itemId);
s32 Item_ReturnTrue(void);
void OptionMenu_StepWindowHue(s32 arg0);
void Debug_SelectAbilityPair(void);
s32 Party_ListPsynergy(struct AbilityListEntry *output);
void Menu_FindShortcutEntries(u32 *first_index, u32 *second_index,
                              const struct AbilityListEntry *entries);
void Menu_RunSelectionHook(void);

void Menu_AllocateSelectionWorkspace(void);
void Menu_ReleaseSelectionWorkspace(void);
void GraphicsPalette_ReservedNoOpC9BC(void);
void GraphicsPalette_ReservedNoOpC9C0(void);
void GraphicsPalette_ReservedNoOpC9C4(void);
void OptionMenu_ResetEntries(struct WorkspaceWork *work);
void WindowColor_ComputeTint(struct WorkspaceWork *work);
void WindowColor_WritePalette(struct WorkspaceWork *work);
u16 WindowColor_ScaleTint(struct WorkspaceWork *work, s32 scale_r, s32 scale_g,
                          s32 scale_b);
u16 Color_ScaleComponents(s16 *rgb, s32 red_scale, s32 green_scale,
                          s32 blue_scale);
void PaletteGlow_Update(s32 phase, s32 brightness);
void OptionMenu_PreviousRow(struct WorkspaceWork *work);
void OptionMenu_NextRow(struct WorkspaceWork *work);
void OptionMenu_LowerSetting(struct WorkspaceWork *work);
void OptionMenu_RaiseSetting(struct WorkspaceWork *work);
void GraphicsPalette_SelectionNoOp(void);
void OptionMenu_AnimateCursors(void);
void OptionMenu_InitializeWork(void);
void OptionMenu_Release(void);
struct UiWindow *OptionMenu_Open(void);
s32 Menu_RunWorkspaceOptions(void);
void SystemMenu_AnimateCursor(void);
void SystemMenu_Initialize(void);
void SystemMenu_Release(void);
struct UiWindow *SystemMenu_Open(void);
s32 Menu_RunWorkspaceSelectionLoop(void);

void UiWindow_OverlayTilePattern(u16 *entry, u16 *mirror, s32 pattern,
                                 u8 *remap);
s32 UiText_RenderStringTiles(u16 *script, u16 *tiles, u16 *mirror, s32 x);
// UiWindow_ClearTileAttributesInRect is called before its definition with no
// prototype in scope.
void UiWindow_MarkVisibleTileAttributes(void);
void UiWork_SetAltFlagAndClearTable(s32 flag);
void UiRender_ReservedNoOp(void);
void UiWindow_DrawDividerLine(struct UiWindow *win, u32 x1, u32 y1, u32 x2,
                              u32 y2);
void UiWork_SetParamNibble(s32 param);
void UiWork_SetRenderWord(u16 value);
void UiText_DrawResource(s32 message, struct UiWindow *window, s32 x, s32 y);
void UiText_DrawCharacterAtOffset(s32 message, const struct UiWindow *window,
                                  u32 offsetX, u32 offsetY);
void UiText_DrawString(const u8 *text, struct UiWindow *window, s32 x, s32 y);
void UiText_DrawStringAtOffset(const u8 *text, const struct UiWindow *work,
                               s32 offsetX, s32 offsetY);
void UiText_DrawStringInWindow(const u8 *text, struct UiWindow *window, u32 x,
                               u32 y);
void UiText_DrawNumber(s32 value, s32 width, struct UiWindow *window, s32 x,
                       s32 y);
void UiText_DrawNumberAtOffset(s32 value, s32 width,
                               const struct UiWindow *window, s32 x, s32 y);
void UiText_DrawNumberInWindow(s32 value, s32 width, struct UiWindow *window,
                               s32 x, s32 y);
// UiText_DrawPrefixedNumberAtOffset is called from battle_command.c with no
// prototype in scope: in one file its definition would come before
// PartyStatusDraw, and PartyStatusDraw changes (2 of 435 instructions in
// tbs-en). Split evidence for window|battle_command.

#endif
