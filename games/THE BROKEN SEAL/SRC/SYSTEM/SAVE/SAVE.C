#define OWN_AudioCommand_PlayFar
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "SAVE_STATE.H"
#include "GAME_STATE.H"
#include "RUNTIME_INTERFACES.H"
#include "MEM.H"
#include "TBS_EDITION.H"
#include "RENDER_INPUT.H"
#include "WINDOW.H"
#include "HEAP_STATE.H"
#include "IO_REG.H"
#include "text/MSG_IDS.H"

#define RenderOutput_RedrawSavedRect ((void (*)(void *))RenderOutput_RedrawSavedRect)

#define UiWindow_FillTilemapRect ((void (*)(void*,s32,s32,s32,s32))UiWindow_FillTilemapRect)
#define UiText_DrawFourNumbersInRow ((void (*)(void*,void*))UiText_DrawFourNumbersInRow)
#define StatusMenu_DrawCharacterSummary ((void (*)(void*,void*))StatusMenu_DrawCharacterSummary)
#define ObjectPlacement_CreateGroup ((void (*)(void*,s32,s32,void*))ObjectPlacement_CreateGroup)
#define Menu_SpawnFourObjectsAtOrigin ((void (*)(void*,s32,s32))Menu_SpawnFourObjectsAtOrigin)
#define RenderResource_CreatePair ((struct RenderOutput * (*)(s32,struct UiWindow*,s32,s32))RenderResource_CreatePair)
#define Ui_ApplyTableOffsetToPair ((void (*)(void*))Ui_ApplyTableOffsetToPair)

void UiText_ShowPositionedMessageAndWait(s32 msg, s32 mode);
s32 UiWork_IsComplete(void);
void UiWork_FinalizePendingCore(void);

extern s32 gLoadedStateWord;
extern u8 gOptionMirror;
extern s16 gPostLoadCounter;

#if EDITION_INTERNATIONAL
extern volatile s32 gKeyState;
extern volatile s32 gKeysRepeat;

void AudioCommand_PlayFar(s32 cue);
void PaletteGlow_Update(s32 icon, s32 icon_sub);
void UiWindow_DrawDividerLine(void *win, s32 a, s32 b, s32 c, s32 d);
void UiText_DrawResource(s32 message, void *win, s32 x, s32 y);
void UiText_DrawString(void *text, void *win, s32 x, s32 y);

s32 SaveMenu_SelectSlot(s32 slot, s32 mode)
{
    struct UiWindow *list;
    struct SaveWorkspace *save;
    struct UiWindow *status;
    struct UiWindow *djinn;
    struct UiWindow *counts;
    struct SaveSummary *summary;
    struct RenderOutput *cursor;
    s32 redraw;
    struct UiRenderWork *render;
    u32 stamp;
    void *work;
    volatile u32 zero;
    s32 result;
    s32 i;
    s32 y;

    work = Runtime_AllocateHeapBlock(55, 0xa70);
    save = *(gWorkSlot + HEAP_SLOT_SAVE);
    status = NULL;
    djinn = NULL;
    counts = NULL;
    redraw = 1;
    render = *(gWorkSlot + HEAP_SLOT_WINDOW);
    stamp = Runtime_GetBuildStampTimeFar();
    if (slot < 0)
        slot = 0;
    if (mode == 1) {
        for (i = 0; i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].send_flag != 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    } else if (mode == 4) {
        for (i = 0; i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].flag_count == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    } else if (mode == 5) {
        for (i = 0; i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].send_flag == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    } else if (mode != 0) {
        for (i = 0; i < 3 && (save->summary[slot].level == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    }

    zero = 0;
    Dma_Set(&zero, work, 0x85000000 | (0xa70 / 4), REG_DMA3);
    Scheduler_ScheduleCallbackAAfterFrames();
    list = UiWindow_Create(1, 2, 28, 7, 2);
    summary = save->summary;
    y = 0;
    for (i = 0; i < 3; i++, y += 16, summary++) {
        if (summary->level == 0) {
            UiText_DrawResource((s32)MsgSaveNoData, list, 10, y);
        } else if (summary->frames < stamp) {
            UiText_DrawResource((s32)MsgSaveContinue, list, 10, y);
        } else if (summary->play_time != *(u32 *)summary->unknown_38) {
            UiText_DrawResource((s32)MsgSaveCorrupted, list, 10, y);
        } else if (mode == 5 && (s8)summary->send_flag == 0) {
            UiText_DrawResource((s32)MsgSaveNotCleared, list, 10, y);
        } else {
            UiText_DrawString(summary->name, list, SAVE_SLOT_NAME_X, y);
            UiText_DrawResource(summary->area + (s32)MsgDebugEntryName, list, 62, y);
            render->dirty = 1;
        }
    }
    UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
    UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
    cursor = RenderResource_CreatePair(mode, list, 72, -24);

    while (1) {
        if (redraw != 0) {
            redraw = 0;
            if (save->summary[slot].level != 0) {
                PaletteGlow_Update(save->summary[slot].palette_glow, save->summary[slot].palette_glow_sub);
                if (status == NULL)
                    status = UiWindow_Create(1, 10, 14, 9, 2);
                StatusMenu_DrawCharacterSummary(status, &save->summary[slot]);
                WaitFrames(1);
                if (djinn == NULL)
                    djinn = UiWindow_Create(16, 10, 13, 3, 2);
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                ObjectPlacement_CreateGroup(djinn, 0, 0, &save->summary[slot]);
                WaitFrames(1);
                if ((s8)save->summary[slot].djinn[0] + (s8)save->summary[slot].djinn[1] + (s8)save->summary[slot].djinn[2]
                        + (s8)save->summary[slot].djinn[3] != 0) {
                    if (counts == NULL)
                        counts = UiWindow_Create(16, 14, 13, 5, 2);
                    UiText_DrawFourNumbersInRow(counts, &save->summary[slot]);
                    Menu_ClearSecondObjectRowAndScheduleUpdate();
                    Menu_SpawnFourObjectsAtOrigin(counts, 0, 0);
                } else {
                    Menu_ClearSecondObjectRowAndScheduleUpdate();
                    UiWork_Finalize(counts, 2);
                    counts = NULL;
                }
            } else {
                PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
                Menu_ClearSecondObjectRowAndScheduleUpdate();
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                UiWork_Finalize(counts, 2);
                UiWork_Finalize(djinn, 2);
                UiWork_Finalize(status, 2);
                counts = NULL;
                djinn = NULL;
                status = NULL;
            }
            RenderOutput_RedrawSavedRect(list);
            UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
            UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
            UiWindow_FillTilemapRect(list, 0, slot * 2, 26, 1);
        }
        Ui_ApplyTableOffsetToPair(cursor);
        WaitFrames(1);
        if (gKeysRepeat & 0x40) {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do {
                slot = (slot + 2) % 3;
                if (mode == 0)
                    break;
            } while ((save->summary[slot].level == 0
                || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
                || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
                || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
        } else if (gKeysRepeat & 0x80) {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do {
                slot = (slot + 4) % 3;
                if (mode == 0)
                    break;
            } while ((save->summary[slot].level == 0
                || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
                || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
                || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
        } else if (gKeyState & 2) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        } else if (gKeyState & 1) {
            AudioCommand_PlayFar(112);
            result = slot;
            break;
        }
    }
    Menu_ClearSecondObjectRowAndScheduleUpdate();
    Menu_ClearFirstObjectRowAndScheduleUpdate();
    UiWork_Finalize(counts, 2);
    UiWork_Finalize(djinn, 2);
    UiWork_Finalize(status, 2);
    UiWork_Finalize(list, 2);
    Scheduler_ScheduleCallbackA();
    Runtime_ReleaseHeapBlock(55);
    PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
    WaitFrames(1);
done:
    return result;
}
#elif defined(TBS_EDITION_JA)
#include "SHOP.H"
#include "RESOURCE.H"

#define Shop_SetCursorFar ((void (*)(void*,s32,s32,s32))Shop_SetCursorFar)

extern u8 Resource_FixedBlockBTiles[];
extern volatile s32 gKeyState;
extern volatile s32 gKeysRepeat;

void AudioCommand_PlayFar(s32 cue);
void PaletteGlow_Update(s32 icon, s32 icon_sub);
void UiWindow_DrawDividerLine(void *win, s32 a, s32 b, s32 c, s32 d);
void UiText_DrawResource(s32 message, void *win, s32 x, s32 y);
void UiText_DrawString(void *text, void *win, s32 x, s32 y);

s32 SaveMenu_SelectSlot(s32 slot, s32 mode)
{
    struct UiWindow *list;
    struct UiWindow *status;
    struct SaveWorkspace *save;
    struct UiWindow *djinn;
    struct UiWindow *counts;
    struct SaveSummary *summary;
    struct RenderOutput *cursor;
    s32 kind;
    s32 redraw;
    struct UiRenderWork *render;
    u32 stamp;
    void *work;
    struct ShopCursor marker;
    volatile u32 zero;
    s32 result;
    s32 i;
    s32 x;
    s32 y;
    s32 tile;
    struct RenderOutput *anchor;

    work = Runtime_AllocateHeapBlock(55, 0xa70);
    save = *(gWorkSlot + HEAP_SLOT_SAVE);
    status = NULL;
    djinn = NULL;
    counts = NULL;
    redraw = 1;
    render = *(gWorkSlot + HEAP_SLOT_WINDOW);
    stamp = Runtime_GetBuildStampTimeFar();
    if (slot < 0)
        slot = 0;
    if (mode != 0) {
        for (i = 0; i < 3 && (save->summary[slot].level == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    }
    if (mode == 1) {
        for (i = 0; i < 3 && ((s8)save->summary[slot].send_flag != 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    }
    if (mode == 4) {
        for (i = 0; i < 3 && ((s8)save->summary[slot].flag_count == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    }
    if (mode == 5) {
        for (i = 0; i < 3 && ((s8)save->summary[slot].send_flag == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    }

    zero = 0;
    Dma_Set(&zero, work, 0x85000000 | (0xa70 / 4), REG_DMA3);
    list = UiWindow_Create(1, 2, 28, 7, 2);

    for (i = 0, summary = save->summary; i < 3; i++, summary++) {
        if (save->summary[i].level == 0) {
            UiText_DrawResource((s32)MsgSaveNoData, list, 10, i * 16);
        } else if (save->summary[i].frames < stamp) {
            UiText_DrawResource((s32)MsgSaveContinue, list, 10, i * 16);
        } else if (save->summary[i].play_time != *(u32 *)save->summary[i].unknown_38) {
            UiText_DrawResource((s32)MsgSaveCorrupted, list, 10, i * 16);
        } else if (mode == 5 && (s8)save->summary[i].send_flag == 0) {
            UiText_DrawResource((s32)MsgSaveNotCleared, list, 10, i * 16);
        } else {
            y = i * 16;
            UiText_DrawString(summary->name, list, SAVE_SLOT_NAME_X, y);
            UiText_DrawResource(save->summary[i].area + (s32)MsgDebugEntryName, list, 72, y);
            render->dirty = 1;
        }
    }
    UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
    UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
    tile = Resource_FindFreeEntry();
    if (tile < 96) {
        VramBlock_LoadCached(tile, 128, Resource_FixedBlockBTiles);
        x = -12;
        y = slot * 16 + 4;
        anchor = RenderOutput_Create(tile, 0x40000000, (struct RenderInput *)list, x, y);
        anchor->active = 1;
        marker.anchor = anchor;
    }
    kind = 1;
    cursor = RenderResource_CreatePair(mode, list, 72, -24);

    while (1) {
        if (redraw != 0) {
            redraw = 0;
            if (save->summary[slot].level != 0) {
                PaletteGlow_Update(save->summary[slot].palette_glow, save->summary[slot].palette_glow_sub);
                if (status == NULL)
                    status = UiWindow_Create(1, 10, 14, 9, 2);
                StatusMenu_DrawCharacterSummary(status, &save->summary[slot]);
                WaitFrames(1);
                if (djinn == NULL)
                    djinn = UiWindow_Create(16, 10, 13, 3, 2);
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                ObjectPlacement_CreateGroup(djinn, 0, 0, &save->summary[slot]);
                WaitFrames(1);
                if ((s8)save->summary[slot].djinn[0] + (s8)save->summary[slot].djinn[1] + (s8)save->summary[slot].djinn[2]
                        + (s8)save->summary[slot].djinn[3] != 0) {
                    if (counts == NULL)
                        counts = UiWindow_Create(16, 14, 13, 5, 2);
                    UiText_DrawFourNumbersInRow(counts, &save->summary[slot]);
                    Menu_ClearSecondObjectRowAndScheduleUpdate();
                    Menu_SpawnFourObjectsAtOrigin(counts, 0, 0);
                } else {
                    Menu_ClearSecondObjectRowAndScheduleUpdate();
                    UiWork_Finalize(counts, 2);
                    counts = NULL;
                }
            } else {
                PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
                Menu_ClearSecondObjectRowAndScheduleUpdate();
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                UiWork_Finalize(counts, 2);
                UiWork_Finalize(djinn, 2);
                UiWork_Finalize(status, 2);
                counts = NULL;
                djinn = NULL;
                status = NULL;
            }
            x = list->x * 8 - 4;
            y = (list->y << 3) + slot * 16 + 12;
            Shop_SetCursorFar(&marker, x, y, kind);
            kind = 3;
        }
        ShopCursor_AdvanceFar(&marker);
        Ui_ApplyTableOffsetToPair(cursor);
        WaitFrames(1);
        if (gKeysRepeat & 0x40) {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do {
                slot = (slot + 2) % 3;
                if (mode == 0)
                    break;
            } while ((save->summary[slot].level == 0
                || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
                || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
                || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
        } else if (gKeysRepeat & 0x80) {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do {
                slot = (slot + 4) % 3;
                if (mode == 0)
                    break;
            } while ((save->summary[slot].level == 0
                || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
                || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
                || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
        } else if (gKeyState & 2) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        } else if (gKeyState & 1) {
            AudioCommand_PlayFar(112);
            result = slot;
            break;
        }
    }
    Menu_ClearSecondObjectRowAndScheduleUpdate();
    Menu_ClearFirstObjectRowAndScheduleUpdate();
    UiWork_Finalize(counts, 2);
    UiWork_Finalize(djinn, 2);
    UiWork_Finalize(status, 2);
    UiWork_Finalize(list, 2);
    Runtime_ReleaseHeapBlock(55);
    PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
    WaitFrames(1);
done:
    return result;
}
#endif

s32 Save_WriteSelectedSlot(void)
{
    s32 result;
    s32 slot;
    s32 flag;
    struct SaveWorkspace *work;

    result = 0;
    flag = SaveState_InitializeWorkspace();
    if (flag != 0) {
        UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
        result = -9;
    } else {
        SaveState_LoadSummaryRecords();
        work = gSaveWorkspace;
        slot = SaveMenu_SelectSlot(gSaveSlot, 0);
        if (slot == -1) {
            result = slot;
        } else {
            if (work->summary[slot].level != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgOverwriteConfirm, 13);
                while (UiWork_IsComplete() == 0) {
                    WaitFrames(1);
                }
                if (Menu_RunConfirmSelection(1, 0, 0, 1) != 0) {
                    UiWork_FinalizePendingCore();
                    goto skip;
                }
                UiWork_FinalizePendingCore();
            }
            gSaveSlot = slot;
            AudioCommand_PlayFar(85);
            UiText_ShowPositionedMessageAndWait((s32)MsgSaving, 13);
            while (UiWork_IsComplete() == 0) {
                WaitFrames(1);
            }
            SaveState_BuildSummaryHeader();
            SaveState_CaptureObjectTableFar();
            flag = SaveState_WriteRecord(slot, gSaveBuffer);
            flag |= SaveState_WriteRecord(slot + 3, gSaveBuffer + 0x1000);
            UiWork_FinalizePendingCore();
            if (flag != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
                result = -3;
            } else {
                UiText_ShowPositionedMessageAndWait((s32)MsgGameSaved, 9);
            }
        }
    }
skip:
    SaveState_ReleaseWorkspace();
    return result;
}

s32 SaveState_LoadRecordIntoWork(s32 arg)
{
    s32 ret = 0;
    s32 err = SaveState_InitializeWorkspace();

    if (err != 0) {
        UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
        ret = -9;
    } else {
        s32 value;

        SaveState_LoadSummaryRecords();
        value = SaveMenu_SelectSlot(gSaveSlot, arg);
        if (value == -1) {
            ret = value;
        } else {
            void *base = &gSaveBuffer;

            err = SaveState_ReadRecordPayload(value, base);
            base = (char *)base + 0x1000;
            err |= SaveState_ReadRecordPayload(value + 3, base);
            if (err != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgLoadFailed, 1);
                ret = -2;
            } else {
                gLoadedStateWord = gGameState.play_time;
                gOptionMirror = gGameState.saved_options;
                gPostLoadCounter = 0;
                gSaveSlot = value;
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return ret;
}
