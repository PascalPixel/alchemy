#ifndef ALCHEMY_UNIT_WINDOW_SELECT_H
#define ALCHEMY_UNIT_WINDOW_SELECT_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

struct MenuSprite;
struct NodeChainNode;
struct NodeChainState;
struct SelectionNode;
struct SelectionScreen;

void UiGlyph_ResetWorkState(void);
void Resource_ClearOwnerListAndCounters(void);
void Resource_PushPendingPair(u32 first, u32 second);
void MenuSelection_BuildEntries(u32 unused);
void UiWork_ReservedNoOp(void);
struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind);
void Resource_ScheduleOwnerResetDelayed(void);
void Resource_ScheduleOwnerReset(void);
void MenuSelection_DrawFrame(void);
void MenuSelection_DrawSideMarker(struct SelectionScreen *state, s32 index);
void Menu_OpenSelectionWindow(s32 mode, u32 count);
void Resource_ResetOwnerEntries(void);
void Menu_SetNodeCoordinates(u32 first, u32 second);
void Menu_SetupSelectionBothSides(void);
void Menu_SetupSelectionSide(struct SelectionScreen *state, s32 index);
struct NodeChainNode *NodeChain_GetNodeAtCount(struct NodeChainState *st);
s32 Menu_SelectionLoop(s32 mode);
u32 Menu_WaitForSelectionInput(u32 mode);
void Menu_MoveSelectionForward(struct SelectionScreen *screen);
void Menu_MoveSelectionBackward(struct SelectionScreen *screen);
void Menu_StepRight(struct SelectionScreen *state);
void Menu_StepLeft(struct SelectionScreen *state);
void Menu_ReloadNodeResource(struct SelectionScreen *state, u32 index);
void Menu_LoadSelectionNodeResource(struct SelectionScreen *state, u32 index);
void Menu_SendNodeCountList(struct SelectionScreen *screen);
void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward);
s32 Resource_LoadByMode(s32 mode, s32 value);
s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option);
void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node, s32 reuse);
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
void Menu_LoadSelectedResource(void);
void Resource_ResetPendingTransfer(void);
s32 Menu_RunTopSelection(void);
void UiWindow_OpenMode1AndWaitFrame(void);
/* UiWork_CloseAndRelease is called before its definition with no prototype in scope. */
void Ui_ClearWorkStateAndWaitFrame(void);
u32 Menu_RunSelectionForValue(u32 value);

#endif
