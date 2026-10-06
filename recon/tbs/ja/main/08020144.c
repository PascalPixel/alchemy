/* Draft: Japanese SaveMenu_SelectSlot, 2026-10-06. Native 0x5f8 bytes.
   Thumb scaffold diff: 663 native instructions, 653 draft, common 421,
   differ 474 (was 593). Japanese eligibility scans are independent; its
   cursor is a ShopCursor local and it does not install the international
   scheduler callbacks. The slot list now indexes save->summary[i] and sets
   y = i * 16: the counter stays ascending in r4 with the native caller-save
   stores and unmerged message calls, and marker sits above zero as native.
   With kind declared before redraw, stamp, render, redraw, kind, cursor
   and &marker take their native slots (16, 20, 24, 28, 32, 12).
   Remaining: native reduces y to its own stack giv (sp+8, the missing 4 of
   the 64-byte frame versus 68) and keeps a summary pointer at sp+36, so
   djinn and status sit 4 lower and the loop registers differ. Native slot
   order suggests a summary pointer declared between counts and cursor.
   Trials: the shared MenuCursor union emitted the same 0x5c8-byte body.
   summary pointer plus y += 16 (old draft): descending counter, 593.
   summary = &save->summary[i] plus y += 16: still descending, 588.
   summary pointer plus y = i * 16: 489. i * 16 written in each call: the
   counter descends again, 585. y = i << 4: 489. Declaring marker before
   zero moves zero below marker as native, differ unchanged. kind declared
   before redraw: 474. Adding that summary pointer, set from
   &save->summary[i] each pass: 474; stepped with summary++: 495. Moving the
   declaration of y or i to the top or the end: unchanged (480 before the
   kind move). y = ++i * 16 as the loop step: descending, 588. A fixed r4
   index gave 544 but loses the native caller-save stores; rejected. No
   device retained. */
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
#include "RUNTIME_MEM.H"
#include "TBS_EDITION.H"
#include "RENDER_INPUT.H"
#include "WINDOW.H"
#include "HEAP_STATE.H"
#include "IO_REG.H"
#include "text/MSG_IDS.H"
#include "SHOP.H"
#include "RESOURCE.H"

extern u8 Resource_FixedBlockBTiles[];

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
void Audio_PlayCue(u8 mode);

extern s32 gLoadedStateWord;
extern u8 gOptionMirror;
extern s16 gPostLoadCounter;

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
    s32 y;
    s32 tile;

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

    for (i = 0; i < 3; i++) {
        y = i * 16;
        if (save->summary[i].level == 0) {
            UiText_DrawResource((s32)MsgSaveNoData, list, 10, y);
        } else if (save->summary[i].frames < stamp) {
            UiText_DrawResource((s32)MsgSaveContinue, list, 10, y);
        } else if (save->summary[i].play_time != *(u32 *)save->summary[i].unknown_38) {
            UiText_DrawResource((s32)MsgSaveCorrupted, list, 10, y);
        } else if (mode == 5 && (s8)save->summary[i].send_flag == 0) {
            UiText_DrawResource((s32)MsgSaveNotCleared, list, 10, y);
        } else {
            UiText_DrawString(save->summary[i].name, list, SAVE_SLOT_NAME_X, y);
            UiText_DrawResource(save->summary[i].area + (s32)MsgDebugEntryName, list, 72, y);
            render->dirty = 1;
        }
    }
    UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
    UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
    tile = Resource_FindFreeEntry();
    if (tile < 96) {
        VramBlock_LoadCached(tile, 128, Resource_FixedBlockBTiles);
        marker.anchor = RenderOutput_Create(tile, 0x40000000, (struct RenderInput *)list, -12, slot * 16 + 4);
        marker.anchor->active = 1;
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
            Shop_SetCursorFar(&marker, list->x * 8 - 4, list->y * 8 + slot * 16 + 12, kind);
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
