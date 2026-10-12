#ifndef ALCHEMY_UNIT_WINDOW_CONFIG_H
#define ALCHEMY_UNIT_WINDOW_CONFIG_H

#include "types.h"
#include "edition.h"
#include "glyph.h"

struct RenderInput;
struct WorkspaceWork;

void Menu_AllocateSelectionWorkspace(void);
void Ui_FinalizeWorkAndReleaseHeap13(void);
void GraphicsPalette_ReservedNoOpC9BC(void);
void GraphicsPalette_ReservedNoOpC9C0(void);
void GraphicsPalette_ReservedNoOpC9C4(void);
void Menu_ResetWorkspaceSelection(struct WorkspaceWork *work);
void GraphicsPalette_SetTintChannelsFromCounters(struct WorkspaceWork *work);
void Palette_WriteBlendedBank15Entries(struct WorkspaceWork *work);
u16 Palette_ScaleTintChannels(struct WorkspaceWork *work, s32 scale_r, s32 scale_g, s32 scale_b);
u16 Color_ScaleComponents(s16 *rgb, s32 red_scale, s32 green_scale, s32 blue_scale);
void PaletteGlow_Update(s32 phase, s32 brightness);
void GraphicsPalette_DecrementSelectionWrap(struct WorkspaceWork *work);
void Menu_AdvanceWorkspaceIndexModulo3(struct WorkspaceWork *work);
void GraphicsPalette_DecrementSelectedCounter(struct WorkspaceWork *work);
void GraphicsPalette_AdjustSelectionCounter(struct WorkspaceWork *work);
void GraphicsPalette_SelectionNoOp(void);
void GraphicsPalette_LoadSelectionResourcesAndAdvance(void);
void OptionMenu_InitializeWork(void);
void Runtime_ScheduleCallbackAndReleaseBlock20B(void);
struct RenderInput *Menu_OpenWorkspaceOptions(void);
s32 Menu_RunWorkspaceOptions(void);
void Menu_RunSelectedWorkspaceEntry(void);
void Menu_InitializeSelectedWorkspace(void);
void Runtime_ScheduleCallbackAndReleaseBlock20A(void);
struct RenderInput *Menu_CreateWorkspaceWindows(void);
s32 Menu_RunWorkspaceSelectionLoop(void);

#endif
