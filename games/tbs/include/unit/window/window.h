#ifndef ALCHEMY_UNIT_WINDOW_WINDOW_H
#define ALCHEMY_UNIT_WINDOW_WINDOW_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

struct AbilityListEntry;
struct BattleActionRecord;
struct BtlAct;
struct GlyphPalette;
struct MenuSprite;
struct NodeChainNode;
struct NodeChainState;
struct OwnerActionSlot;
struct RenderInput;
struct RenderOutput;
struct RenderOutputList;
struct SaveSummary;
struct SelectionNode;
struct SelectionScreen;
struct SelectionWork;
struct TextReader;
struct UiChannelSlot;
struct UiWindow;
struct WorkspaceWork;

struct RenderOutput *RenderOutput_AcquireFree(void);
void RenderOutput_ReleaseFree(struct RenderOutput *out);
void UiWork_InitFreeList(void);
void UiWork_InitializeWithResourceCounters(void);
void UiWork_Initialize(s32 kind);
void UiWork_UploadDirtyBlocks(void);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);
void UiWork_DrawByAttributes(struct UiWindow *window);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs);
void UiWork_WaitWindowDuration(struct UiWindow *window);
void UiWork_Finalize(struct UiWindow *work, s32 release);
void RenderOutput_PrepareForRedraw(struct UiWindow *window);
void RenderOutput_RedrawSavedRect(struct UiWindow *window);
void RenderOutput_ClearList(void *work);
void UiWindow_ClearInteriorTiles(const struct RenderInput *window, u32 left, u32 top, u32 right, u32 bottom);
void UiWork_UpdateListTail(struct RenderOutputList *list);
/* RenderOutput_AppendToList is called before its definition with no prototype in scope. */
void RenderOutput_Release(struct RenderOutput *entry);
/* UiText_QueueRenderEntries is called before its definition with no prototype in scope. */
struct UiChannelSlot *UiWork_ActivateChannel(struct UiWindow *work, s32 value, s32 preserve);
s32 Ui_ClearVramBlock(void);
s32 Ui_FillVramBlockPattern(void);
void UiWork_ResetFreeChannel(void);
void UiWork_CopyParamsToRenderWork(struct UiChannelSlot *channel);
void UiWork_ResetChannelTransition(struct UiChannelSlot *channel);
void UiWork_ShiftPanelRowsLeft(s32 step);
void UiWork_ProcessRenderChannels(void);
s32 UiWork_StepChannelScript(struct UiChannelSlot *ch);
void UiWork_ProcessDirectWork(void);
void UiWindow_UpdateInterpolatedGeometry(void *arg, s32 save_position);
u16 *Memory_FillHalfwordsDma(u16 *dst, s32 value, s32 count);
void UiWindow_DrawFrame(s32 x, s32 y, u32 width, u32 height);
void UiWindow_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32 full_width);
s32 UiWork_IsComplete(void);
s32 UiWork_IsIdle(struct UiWindow *window);
void UiWork_ResetCounters();
void UiWork_InitCountersWithResourceAndScheduleRefresh(void);
void UiWork_InitCountersAndScheduleRefresh(s32 initialize);
void UiWork_FinalizeSharedSlot(void);
void UiText_PrepareMessageWork(s32 argument);
void UiText_ShowMessageAndWaitCore(s32 argument);
s32 UiText_OpenEntryMessage(s32 window, s32 argument);
void UiWork_SetBusyFlags(s32 flags);
struct UiWindow *UiText_OpenMessageWindow(s32 message, s32 x, s32 y, u32 packed);
void UiText_ShowPositionedMessageAndWait(s32 no, s32 flags);
void UiWork_ProcessAll(void);
#if EDITION_INTERNATIONAL
s32 UiText_RenderGlyphPair(s32 code, u32 *out);
#endif
#if !(EDITION_INTERNATIONAL)
s32 UiText_RenderGlyphPair(u32 code, u32 *out);
#endif
s32 UiText_GetWideStringWidth(u16 *text);
void UiText_RenderWideStringAtOffset(u16 *text, struct UiWindow *window, s32 x, s32 y);
void UiText_RenderGlyphTileAtWorkOffset(u16 *buffer, const struct RenderInput *work, s32 offset_x, s32 offset_y);
/* UiText_RenderWideStringInWindow is called before its definition with no prototype in scope. */
u8 *UiText_FormatNumber(u8 *buffer, s32 input, s32 width);
u32 UiText_AppendArticleName(s32 mode, u16 *name, u32 pos, u16 *entry, s32 no, s32 plural, s32 *suffix);
s32 UiText_BuildRenderEntries(s32 script, s32 clear);
void UiWindow_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph, u16 *spacing, u32 flags);
void UiText_MeasureResourceEntries(s32 no, s32 *x, s32 *y);
s32 UiText_GetResourceDimensions(s32 no, s32 *x, s32 *y, u32 *width, u32 *height);
s32 UiText_GetResourceDimensionsAlt(s32 no, s32 *x, s32 *y, u32 *width, u32 *height);
void UiText_MeasureEntryDimensions(s32 pos, u32 *out_width, u32 *out_height, u16 *spacing);
void UiText_MeasureStringVariant(s32 start, u32 *out_width, u32 *out_height, u16 *spacing);
s32 UiText_DrawGlyph(struct UiWindow *window, s32 character, s32 x, s32 y, s32 mode);
/* UiWindow_PutGlyph is called before its definition with no prototype in scope. */
void UiWindow_SetTilemapEntry(struct UiWindow *window, s32 value, s32 x, s32 y, u32 mode);
void RenderOutput_UpdateScaleAnimation(struct RenderOutput *output);
void UiWork_AnimateSpriteSlots(void);
void Ui_AdjustValueWithoutLimit(s32 mes, s32 buf);
s32 UiText_CopyMessageString(s32 key, u16 *destination, u32 capacity);
void UiText_DecodeMessage(s32 message, u16 *text, s32 capacity);
s32 *Runtime_ClearWordIfSet(s32 *p);
void UiWork_DrainPending(s32 unused);
void UiWork_AdvanceChannelTransition(struct UiChannelSlot *ch);
void UiWork_ClearValueNameTables(void);
void UiWork_PushValueSlot(u32 value, u32 kind);
u32 UiRender_LookupNamedValue(u32 kind, u32 clear);
s32 UiWork_CheckCancelByInput(struct UiChannelSlot *ch);
s32 UiWork_CheckCancelByModeInput(struct UiChannelSlot *ch);
void UiWork_FinalizePendingCore(void);
void UiText_ShowCenteredMessage(s32 message, s32 mode, s32 y_offset);
s32 UiText_BuildRenderEntriesMode1(s32 arg0);
void UiText_LookupMessage(struct TextReader *reader, s32 message);
s32 UiText_DecodeSymbol(struct TextReader *reader);
void UiWork_SetTwoEntriesTo999(void);
s32 Localization_LookupEntryId(u32 value);
s32 UiWindow_CreateWithSideObject(s32 arg0, s32 arg1, s32 x, s32 y);
void UiWork_FinalizeEntityMatchingLocalizedId(u32 entry);

#define UI_WORK ((struct UiRenderWork *)gWindowWork[0])

#define ACTION_ID_MASK 0x3fff

// no の表示エントリを作り、UiWindow_FitOnScreen に flags をつけてわたす。
// エントリがなければ 0 を返す
#define DEFINE_GET_RESOURCE_DIMENSIONS(name, flags)               \
	s32 name(s32 no, s32 *x, s32 *y, u32 *width, u32 *height)     \
	{                                                             \
		struct UiRenderWork *work;                                \
		s32					 idx;                                 \
                                                                  \
		work = UI_WORK;                                           \
		idx = UiText_BuildRenderEntries(no, 0);                   \
		if (work->entries[idx] == 0)                              \
		{                                                         \
			return 0;                                             \
		}                                                         \
		UiWindow_FitOnScreen(idx, x, y, width, height, 0, flags); \
		return 1;                                                 \
	}

#endif
