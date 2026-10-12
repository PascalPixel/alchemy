#ifndef ALCHEMY_UNIT_GRAPHICS_WINDOW_SAVE_H
#define ALCHEMY_UNIT_GRAPHICS_WINDOW_SAVE_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

struct RenderInput;
struct RenderOutput;
struct SaveSummary;
struct UiWindow;

void *Text_FormatPlayTime(s32 value, u8 *out);
u32 SaveState_FindFreeSummarySlot(void);
s32 SaveState_CountRecordsExcludingFlagged(s32 flag);
s32 SaveState_ScanRecordFlags(void);
u32 SaveState_BuildSummaryHeader(void);
s16 SaveState_WriteCurrentSlotPair(void);
s32 SaveState_WriteSlotPair(s32 arg0);
s32 SaveState_ProcessSelectedSlot(void);
s32 SaveState_ConfirmAndProcessSelectedSlot(void);
s32 SaveState_CopySlotPair(void);
s32 SaveState_DeleteSelectedSlot(void);
void PaletteGlow_UpdateSine(void);
void Scheduler_ScheduleCallbackAAfterFrames(void);
void Scheduler_ScheduleCallbackA(void);
void UiWindow_FillTilemapRect(struct UiWindow *window, s32 x, s32 y, s32 width, s32 height);
void ObjectPlacement_CreateGroup(struct RenderInput *origin, s32 x, s32 y, struct SaveSummary *descriptor);
void Menu_ClearFirstObjectRowAndScheduleUpdate(void);
void Menu_UpdateFirstObjectRowPositions(void);
void Menu_SpawnFourObjectsAtOrigin(struct RenderInput *origin, s32 x, s32 y);
void Menu_ClearSecondObjectRowAndScheduleUpdate(void);
void Menu_UpdateSecondObjectRowPositions(void);
void UiText_DrawFourNumbersInRow(struct RenderInput *win, struct SaveSummary *row);
void StatusMenu_DrawCharacterSummary(s32 win, struct SaveSummary *rec);
s32 SaveMenu_SelectSlot(s32 slot, s32 mode);
s32 Save_WriteSelectedSlot(void);
s32 SaveState_LoadRecordIntoWork(s32 arg);
s32 Resource_LoadFixedBlockBIntoFreeSlot(void);
s32 Resource_ReturnTrue(void);
s32 NameEntry_AcceptsMark(s32 mark, s32 letter);
s32 NameEntry_CountLetters(const u8 *name);
void UiWindow_CopyTilemapRegion(const struct RenderInput *window, const void *source);
void UiWindow_SetTileAttributeRect(const struct RenderInput *window, s32 x, s32 y, s32 width, s32 height, u32 field);
void Resource_CopyFixedBlockB(s32 arg0);
void Resource_CopyFixedBlockA(s32 arg0);
s32 UiText_SetRenderString(const u8 *str);
void UiText_DrawPaddedLabel(s32 output, u8 *input);
s32 NameEntry_EditOwnerName(s32 entry);
s32 PartyTalkMenu_Choose(s32 unused);
void Djinn_ShowJoinedMessage(s32 pc, s32 element, s32 djinn);
s32 Party_LookupCharacterValueByFlag32(u32 index);
void Party_ShowJoinedMessage(s32 member);
void Party_ShowPairJoinedMessage(s32 first, s32 second);
void RenderResource_LoadPair(s32 group_index, s32 resource_index);
struct RenderOutput *RenderResource_CreatePair(s32 group, struct RenderInput *input, s32 x, s32 y);
void Ui_ApplyTableOffsetToPair(struct RenderOutput *obj);
void RenderResource_LoadFrame(s32 index, s32 value, s32 flag);
struct RenderOutput *RenderResource_CreateFrame(s32 arg0, s32 arg1, struct RenderInput *arg2, s32 arg3, s32 arg4);
void Ui_ApplyTableScaleToObject(struct RenderOutput *obj);

#endif
