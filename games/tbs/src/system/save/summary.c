#include "types.h"
#include "render_input.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "window.h"
#include "btlwork.h"
#include "dma.h"
#include "global_cells.h"
#include "tbs_edition.h"
#include "io_reg.h"
#include "btltype.h"
#include "party_state.h"
#include "mem.h"
#include "btlpty.h"
#include "b5_context.h"
#include "menu_list.h"
#include "btlsys.h"
#include "scene.h"
#include "save_state.h"
#include "game_state.h"
#include "battle_unit.h"
#include "runtime_interfaces.h"
#include "text/msg_ids.h"
#include "iwram_call.h"
#include "irq.h"

void RenderOutput_SetInvertedSentinel(void *arg0, s32 arg1)
{
    if (arg0 != 0)
        ((struct RenderOutput *)arg0)->sentinel = ~arg1;
}

#define TILE_ROW_FROM_BOTTOM(tile, up) ((u32 *)((tile) * 32 - (up) * 4 + 0x0600001c))

typedef void (*RamRoutine)(void *dst, void *scene);

void UiWindow_FillFromScene(void *dst)
{
    void *scene = gWindowWork[0];

    if (scene == 0) {
        volatile u16 fill = 0xe0e0;

        Dma_Set((void *)&fill, dst, 0x810000a0, REG_DMA3);
    } else {
        u32 size = (u32)Tile_BuildMetatilesCodeSize;
        RamRoutine code = (RamRoutine)MemAlloc(size);

        Dma_Set((void *)Tile_BuildMetatiles, code, 0x84000000 | (size >> 2), REG_DMA3);
        code(dst, scene);
        MemFree(code);
    }
}

void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width, u32 height,
                   s32 value)
{
    u32 row = 0;
    s16 *dst = (s16 *)0x06002000;

    if (row < height) {
        do {
            u32 column = 0;
            if (column < width) {
                do {
                    column++;
                    *dst = value;
                    dst++;
                } while (column < width);
            }
            row++;
            dst += 32 - width;
        } while (row < height);
    }
}

void UiWindow_BuildLayoutBounds(s32 flags)
{
    void **slot = (void **)&gPartyStatusWork;
    struct PartyStatusWindow *state = *slot;
    struct UiRenderWork *work = slot[-1];
    s32 height = 4;
    s32 n;
    s32 right;
    s32 left;

    if (work->menu_state != 0) {
        n = BattleParty_PrepareActiveOwnersFar(0);
        height = 3;
    } else {
        n = Party_CountActiveOwnersFar();
    }
    if (flags & 1)
        height++;
    else
        flags &= -3;

    n *= 6;
    right = n + 1;
    if (flags & 2)
        right += 5;

    left = 30;
    left -= right;
    state->x = left;
    state->y = 0;
    state->width = right;
    state->height = height;
    state->flags = flags;
}

void UiWindow_CreateWithLayoutBounds(s32 flags)
{
    s32 zero;
    struct PartyStatusWindow *window;
    s8 *busy;

    window = Runtime_AllocateBlock(16, sizeof(*window));
    busy = (s8 *)&((struct UiRenderWork *)gWindowWork[0])->menu_busy;
    zero = 0;
    *busy = 1;
    UiWindow_BuildLayoutBounds(flags);
    window->window = UiWindow_Create(
        window->x, window->y, window->width, window->height, 6);
    PtyStDraw(flags);
    *busy = zero;
}

void UiWindow_DrawColumnBorders(struct RenderInput *window, u32 flags)
{
    struct UiRenderWork *canvas = (struct UiRenderWork *)gWindowWork[0];
    s32 first = 1;
    s32 bias = 0;
    u32 max = window->width - 1;
    s32 i;
    u32 rows = window->height;
    u32 col;
    u32 row;
    u16 *dest;

    if ((flags & 1) == 0)
        flags &= ~2;
    if (flags & 2) {
        bias = 5;
        first = 0;
    }
    i = first;
    while (UiWindow_PartyColumnOffsets[i] >= 0) {
        col = UiWindow_PartyColumnOffsets[i] + bias;
        if (col < max) {
            for (row = 0; row != rows; row++) {
                dest = (u16 *)(((window->y + row) * 32 + (window->x + col)) * 2 + (u32)canvas->tilemap);
                if (row == 0)
                    *dest = 0xf018;
                else if (row == rows - 1)
                    *dest = 0xf019;
                else
                    *dest = 0xf00f;
            }
        }
        i++;
    }
    if (canvas->menu_state) {
        dest = canvas->tilemap + (window->y + window->height - 1) * 32 + window->x;
        col = 1;
        *dest++ = 0xf080;
        while (col < max) {
            col++;
            *dest++ = 0xf081;
        }
        *dest = 0xf082;
    }
    canvas->dirty = 1;
}

s32 UiWindow_DrawStatusBarTiles(struct RenderInput *window, s32 x, s32 y, s32 value)
{
    /* FAKEMATCH: needed to match: the ignored integer result keeps the return-address pop in r1; void selects r0. */
    s32 i;
    struct UiRenderWork *canvas = (struct UiRenderWork *)gWindowWork[0];
    s32 filled = value;
    s32 row;
    s32 col;
    u32 id;
    u32 light;
    u32 dark;
    u32 pixels;
    u32 result;

    if (canvas->menu_state == 0) {
        Dma_Set((const void *)Runtime_GetLowTableAddress(), (void *)&BG_PLTT_COLOR(14, 0),
            0x80000010, REG_DMA3);
        BG_PLTT_COLOR(14, 14) = BG_PLTT_COLOR(15, 4);
    }
    x += window->x;
    y += window->y;
    for (i = 0; i < 5; i++, x++) {
        id = canvas->tilemap[y * 32 + x];
        light = 0x22222222;
        dark = 0xcccccccc;
        id &= 0x3ff;
        if (value > 7) {
            light = 0x88888888;
            dark = 0xdddddddd;
        } else if (value >= 0) {
            light <<= value * 4;
            light |= 0x88888888U >> (32 - value * 4);
            dark <<= value * 4;
            dark |= 0xddddddddU >> (32 - value * 4);
        }
        for (row = 0; row <= (filled != 0 ? 2 : 0); row++) {
            pixels = *TILE_ROW_FROM_BOTTOM(id, row);
            result = 0;
            for (col = 0; col <= 7; col++) {
                u32 color = pixels & 15;

                if (color == 14)
                    result |= light & (15U << (col * 4));
                else if (color == 1)
                    result |= dark & (15U << (col * 4));
                else
                    result |= color << (col * 4);
                pixels >>= 4;
            }
            *TILE_ROW_FROM_BOTTOM(id, row) = result;
        }
        value -= 8;
    }
}

void UiWindow_DrawFrame(s32, s32, u32, u32);
void UiText_DrawStringAtOffset(u8 *, struct RenderInput *, s32, s32);
s32 __divsi3(s32, s32);
s32 UiText_DrawPrefixedNumberAtOffset();
s32 UiWindow_PutGlyph();

void PtyStDraw(s32 flags)
{
    void **slot = (void **)gPartyStatusWork;
    struct PartyStatusWindow *layout = slot[0];
    struct BtlWork *party = slot[-7];
    struct RenderInput *window = (struct RenderInput *)layout->window;
    struct UiRenderWork *work = slot[-1];
    s32 x = 0;
    u32 count;
    s32 y = 0;
    u32 i;
    u16 owners[5];
    u8 djinn[4];

    if (work->menu_state != 0) {
        count = BattleParty_PrepareActiveOwnersFar(0);
        y = -1;
        for (i = 0; i < count; i++) {
            owners[i] = party->party_units[i];
            if (owners[i] == 255)
                break;
        }
    } else {
        count = Party_CountActiveOwnersFar();
        for (i = 0; i < count; i++)
            owners[i] = gGameState.active_owners[i];
        owners[i] = 255;
    }
    count = i;
    if (flags == -1)
        flags = layout->flags;
    if (!(flags & 1))
        flags &= ~2;
    if (work->menu_state == 0 || BattlePlacement_CountValidEntriesFar(0, 0) == 0)
        flags &= ~2;
    if (flags == 9) {
        UiWindow_EraseBorderRect(layout->x, layout->y, layout->width, layout->height);
        return;
    }
    work->menu_busy = 1;
    if (layout->flags == flags) {
        RenderOutput_RedrawSavedRect((struct UiWindow *)window);
        UiWindow_DrawColumnBorders(window, flags);
    } else {
        UiWindow_EraseBorderRect(layout->x, layout->y, layout->width, layout->height);
        UiWindow_BuildLayoutBounds(flags);
        window->width = layout->width;
        window->height = layout->height;
        window->x = layout->x;
        UiWindow_DrawFrame(layout->x, layout->y, layout->width, layout->height);
        UiWindow_DrawColumnBorders(window, flags);
    }
    if (flags & 2)
        x = 5;
    for (i = 0; i != count; i++) {
        struct BtlUnit *unit = UnitGetFar(owners[i]);
        s32 hp = unit->hp;
        s32 maximum = unit->max_hp;
        s32 value;
        s32 current;

        if (hp == 0)
            UiWork_SetParamNibble(2);
        else if (hp <= maximum / 4)
            UiWork_SetParamNibble(4);
        else
            UiWork_SetParamNibble(15);
        work->level = 14;
        if (work->menu_state != 0)
            work->level = 5;
        UiText_DrawPrefixedNumberAtOffset(hp, window, (x + i * 6) * 8, y * 8 + 8, 0);
        work->level = 15;
        UiText_DrawStringAtOffset(unit->name, window, (x + i * 6) * 8, y * 8);
        UiWork_SetParamNibble(15);
        if (unit->max_hp != 0) {
            current = unit->hp;
            value = __divsi3(current * 40, unit->max_hp);
            if (value == 0 && current != 0)
                value = 1;
            UiWindow_DrawStatusBarTiles(window, x + i * 6 + 1, y + 2, value);
        }
        if (flags & 1) {
            work->level = 14;
            if (work->menu_state != 0)
                work->level = 5;
            UiText_DrawPrefixedNumberAtOffset(unit->ep, window, (x + i * 6) * 8, y * 8 + 16, 1);
            if (unit->max_ep != 0) {
                current = unit->ep;
                value = __divsi3(current * 40, unit->max_ep);
                if (value == 0 && current != 0)
                    value = 1;
                UiWindow_DrawStatusBarTiles(window, x + i * 6 + 1, y + 3, value);
            }
        }
    }
    work->level = 15;
    if (work->menu_state != 0 && (flags & 2)) {
        s32 row = y;

        if (flags & 1)
            row++;
        BattlePlacement_CountValidEntriesFar(0, djinn);
        UiWindow_SetTilemapEntry((struct UiWindow *)window, 0x5001, 0, row, 0);
        UiWindow_SetTilemapEntry((struct UiWindow *)window, 0x5002, 2, row, 0);
        UiWindow_SetTilemapEntry((struct UiWindow *)window, 0x5003, 0, row + 1, 0);
        UiWindow_SetTilemapEntry((struct UiWindow *)window, 0x5004, 2, row + 1, 0);
        UiWindow_PutGlyph(window, djinn[0] + '0', 1, row);
        UiWindow_PutGlyph(window, djinn[1] + '0', 3, row);
        UiWindow_PutGlyph(window, djinn[2] + '0', 1, row + 1);
        UiWindow_PutGlyph(window, djinn[3] + '0', 3, row + 1);
    }
    work->menu_busy = 0;
}

#define Runtime_GetBuildStampTimeFar ((u32 (*)(s32))Runtime_GetBuildStampTimeFar)

#define gTitleExtraOptionEnabled (*(s16 *)&gTitleExtraOptionEnabled)

extern volatile u32 gKeysHeld;

extern u8 gSceneState[];
extern u32 gLoadedStateWord;
extern u8 gOptionMirror;
extern u32 GameFlagBytes[];

void UiText_ShowPositionedMessageAndWait(s32, s32);

void UiWork_FinalizeAndReleaseBlock16(void)
{
    UiWork_Finalize((struct UiWindow *)**(s32 **)(u32)&gWindowWork[1], 1);
    Runtime_ReleaseHeapBlock(0x10);
}

void UiWindow_SetTileAttributeBitRect(
    const struct RenderInput *window, s32 x, s32 y, s32 width, s32 height, u32 field)
{
    struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];

    x += window->x + 1;
    y += window->y + 1;
    field &= 1;
    field <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29) {
        width = 30 - x;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29) {
        height = 20 - y;
    }
    if (width > 0 && height > 0) {
        y <<= 6;
        x = y + (x << 1);
        do {
            u16 *cell = (u16 *)(x + (u32)work->tilemap);
            s32 remaining = width;
            while (remaining != 0) {
                u32 value = *cell;
                value &= 0xFFFFEFFF;
                value |= field;
                remaining--;
                *cell = value;
                cell++;
            }
            height--;
            x += 64;
        } while (height != 0);
        work->dirty = 1;
    }
}

void *Text_FormatPlayTime(s32 value, u8 *out)
{
    u8 buf[64];
    u32 time;
    u32 minutes;
    s32 seconds;
    u8 *s;
    u8 *p;

    time = (u32)value / 0xe10;
    if (time > 0xea5f)
        time = 0xea5f;

    minutes = time / 60;
    seconds = time % 60;

    s = UiText_FormatNumber(buf, minutes, 3);
    *out = *s;
    s++;
    p = out + 1;
    *p = *s;
    p++;
    *p = s[1];
    seconds += 100;
    p++;
    *p = ':';

    s = UiText_FormatNumber(buf, seconds, 2);
    p++;
    *p = s[0];
    p++;
    *p = s[1];
    p[1] = 0;

    return out;
}

u32 SaveState_FindFreeSummarySlot(void)
{
    u32 i;
    struct SaveSummary *p;

    p = gSaveWorkspace->summary;
    for (i = 0; i < 3; i++, p++) {
        if (p->level == 0)
            return i;
    }
    return 0x3E7;
}

s32 SaveState_CountRecordsExcludingFlagged(s32 flag)
{
    s32 i;
    s32 cnt;
    struct SaveSummary *summary;

    if (SaveState_InitializeWorkspace() != 0) {
        cnt = -9;
    } else {
        cnt = SaveState_LoadSummaryRecords();
        if (flag != 0) {
            summary = gSaveWorkspace->summary;
            for (i = 0; i < 3; i++) {
                if ((s8)summary[i].send_flag != 0)
                    cnt--;
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return cnt;
}

s32 SaveState_ScanRecordFlags(void)
{
    /* FAKEMATCH: the signed cursor from the party tail preserves the literal base changed by direct flag fields. */
    s32 err;
    s32 cnt;
    s32 ret;

    err = SaveState_InitializeWorkspace();
    cnt = 0;
    ret = -9;
    if (err == 0) {
        s32 i;
        s8 *flags;

        ret = SaveState_LoadSummaryRecords();
        flags = (s8 *)&gSaveWorkspace->summary[0].party[4];
        gTitleSendOptionEnabled = 0;
        gTitleExtraOptionEnabled = 0;
        for (i = 0; i < 3; i++) {
            if (flags[i * sizeof(struct SaveSummary) + ((u32)&((struct SaveSummary *)0)->send_flag - (u32)&((struct SaveSummary *)0)->party[4])] != 0) {
                gTitleSendOptionEnabled = 1;
                cnt++;
            }
            if (flags[i * sizeof(struct SaveSummary) + ((u32)&((struct SaveSummary *)0)->flag_count - (u32)&((struct SaveSummary *)0)->party[4])] != 0) {
                gTitleExtraOptionEnabled = 1;
            }
        }

        if ((gKeysHeld & (KEY_R | KEY_LEFT)) != (KEY_R | KEY_LEFT)) {
            gTitleSendOptionEnabled = 0;
        }
    }
    SaveState_ReleaseWorkspace();
    if (ret != 0 && cnt == ret) {
        return ret + 100;
    }
    return ret;
}

u32 SaveState_BuildSummaryHeader(void)
{
    u16 owners[14];
    struct SaveSummary *summary;
    struct BtlUnit *owner;
    u8 *source;
    u8 *destination;
    u32 *word;
    s32 i;
    u32 sum = 0;

    summary = (struct SaveSummary *)(gSaveBuffer - 16);
    gGameState.build_stamp = Runtime_GetBuildStampTimeFar(0);
    gGameState.play_time = gLoadedStateWord;
    {
        u32 *copy = (u32 *)gSceneState;
        copy[64] = gLoadedStateWord;
    }
    gGameState.saved_options = gOptionMirror;
    owner = UnitGetFar(gGameState.selected_actor);
    destination = summary->name;
    source = owner->name;
    for (i = 11; i >= 0; i--)
        *destination++ = *source++;
    summary->level = owner->level;
    summary->play_time = gGameState.play_time;
    summary->area = Scene_GetAreaNameIndexFar(gGameState.scene, gGameState.entrance);
    summary->class_id = owner->class_index;
    summary->coins = gGameState.coins;
    summary->djinn[0] = Party_SumDjinnCountsFar(0);
    summary->djinn[1] = Party_SumDjinnCountsFar(1);
    summary->djinn[2] = Party_SumDjinnCountsFar(2);
    summary->djinn[3] = Party_SumDjinnCountsFar(3);
    Party_ListActiveOwnersFar(owners);
    for (i = 0; i <= 3 && owners[i] != 255; i++)
        summary->party[i] = owners[i];
    summary->party[i] = -1;
    summary->palette_glow = gGameState.palette_glow[0];
    summary->palette_glow_sub = gGameState.palette_glow[1];
    summary->send_flag = gGameState.unknown_20f;
    summary->flag_count = 0;
    for (i = 48; i <= 127; i++) {
        if (GameFlag_TestFar(i))
            summary->flag_count++;
    }
    summary->has_flag_20 = GameFlag_TestFar(32) != 0;
    {
        /* FAKEMATCH: keeps the bound in r1 (5 of 185 rows differ without) */
        register s32 n asm("r1");
        u32 *frame = &gGameState.build_stamp;
        u32 frames;

        n = 242;
        /* FAKEMATCH: empty asm barrier: issues the bound's first half before the frame load (2 of 185 rows differ without) */
        asm("" : "+r"(n) : "r"(frame));
        frames = *frame;
        word = GameFlagBytes;
        i = 0;
        n <<= 2;
        summary->frames = frames;
        goto test;
        do {
            sum += *word++;
            i++;
test:;
        } while (i < n);
    }
    summary->checksum = sum;
    return sum;
}

s16 SaveState_WriteCurrentSlotPair(void)
{
    s16 value;
    s16 result;
    s32 found;
    s32 error;

    result = 0;
    value = gSaveSlot;
    if (value != -1) {
        found = SaveState_InitializeWorkspace();
        if (found != 0) {
            UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
            error = 9;
            goto set_error;
        }
        SaveState_BuildSummaryHeader();
        {
            void *base = &gSaveBuffer;
            s32 next;

            found = SaveState_WriteRecord(gSaveSlot, base);
            next = gSaveSlot;
            base = (char *)base + 0x1000;
            found |= SaveState_WriteRecord(next + 3, base);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
                error = 3;
set_error:
                result = 0 - error;
            }
        }
        SaveState_ReleaseWorkspace();
        value = result;
    }
    return value;
}

s32 SaveState_WriteSlotPair(s32 arg0)
{
    s32 found;
    s16 result = 0;

    found = SaveState_InitializeWorkspace();
    if (found != 0) {
        UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
        result = -9;
    } else {
        void *base = &gSaveBuffer;

        found = SaveState_WriteRecord(arg0, base);
        base = (char *)base + 0x1000;
        found |= SaveState_WriteRecord(arg0 + 3, base);
        if (found != 0) {
            UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
            result = -3;
        }
    }
    SaveState_ReleaseWorkspace();
    return result;
}

s32 UiText_OpenMessageWindow(s32, s32, s32, s32);

s32 UiWork_IsComplete(void);
void UiWork_FinalizePendingCore(void);
void AudioCommand_PlayFar(s32);

s32 SaveState_ProcessSelectedSlot(void)
{
    void *buffer;
    s32 value;
    s32 result;
    s32 found;
    /* FAKEMATCH: keeps the error code in r7. Removing the pin gives
     * 57 reference / 57 built / 54 common instructions (differ 6):
     * movs r6, #9 and movs r6, #3 replace the r7 moves, and
     * negs r7, r6 replaces negs r7, r7. Scoping error inside the slot
     * branch, using u8 *buffer plus s16 *slot = &gSaveSlot and *slot
     * for the three reads, or returning -error through an inline helper
     * leaves these same three rows. Reusing result for error swaps it
     * with the slot pointer (differ 22), including with explicit slot
     * and earlier result initialization; s16 error adds truncation (28).
     * Duplicating the first negate tail leaves differ 4: movs r7, #9
     * before the call / shared negate become subs r7, #9 after the call
     * and a branch beyond negate. All trials use the stock game flags. */
    register s32 error asm("r7");

    buffer = Runtime_BumpAllocateAlternatePool(0x1000);
    result = 0;
    value = gSaveSlot;
    if (value != -1) {
        found = SaveState_InitializeWorkspace();
        if (found != 0) {
            error = 9;
            UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
            goto negate;

        } else {
            u8 *dst;

            found = SaveState_ReadRecordPayload(gSaveSlot, buffer);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
                result = -2;
            }
            dst = (u8 *)buffer + (s32)gSaveStamp;
            dst -= (s32)gSaveBuffer;
            Iwram_CopyWords(dst, gSaveStamp, 16);
            found = SaveState_WriteRecord(gSaveSlot, buffer);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgSaveFailed, 1);
                error = 3;
negate:
                result = -error;
            }
        }
        SaveState_ReleaseWorkspace();
        MemFree(buffer);
        value = result;
    }
    return value;
}

s32 SaveState_ConfirmAndProcessSelectedSlot(void)
{
    s32 result;

    UiText_OpenMessageWindow((s32)MsgOverwriteConfirm, 8, 12, 2);
    while (UiWork_IsComplete() == 0) {
        WaitFrames(1);
    }
    if (Menu_RunConfirmSelection(1, 0, 0, 1)) {
        UiWork_FinalizePendingCore();
    } else {
        UiWork_FinalizePendingCore();
        AudioCommand_PlayFar(85);
        result = SaveState_ProcessSelectedSlot();
        if (result >= 0) {
            UiText_ShowPositionedMessageAndWait((s32)MsgGameSaved, 1);
        }
    }
    return result;
}

s32 SaveState_CopySlotPair(void)
{
    u32 found;
    s32 value;
    s32 result;

    result = 0;
    found = SaveState_InitializeWorkspace();
    if (found != 0) {
        UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
        result = -9;
    } else {
        SaveState_LoadSummaryRecords();
        value = SaveMenu_SelectSlot(0, 2);
        if (value == -1) {
            result = value;
        } else {
            void *lower = &gSaveBuffer;
            void *upper;

            found = SaveState_ReadRecordPayload(value, lower);
            upper = (u8 *)lower + 0x1000;
            found |= SaveState_ReadRecordPayload(value + 3, upper);
            if (found != 0) {
                UiText_ShowPositionedMessageAndWait((s32)MsgLoadFailed, 1);
                result = -2;
            } else {
                value = SaveState_FindFreeSummarySlot();
                if (value == 999) {
                    UiText_ShowPositionedMessageAndWait((s32)MsgEraseFailed, 1);
                    result = -5;
                } else {
                    found = SaveState_WriteRecord(value, lower);
                    found |= SaveState_WriteRecord(value + 3, upper);
                    if (found != 0) {
                        UiText_ShowPositionedMessageAndWait((s32)MsgEraseFailed, 1);
                        result = -3;
                    } else {
                        UiText_ShowPositionedMessageAndWait((s32)MsgFileCopied, 1);
                    }
                }
            }
        }
    }

    SaveState_ReleaseWorkspace();
    return result;
}

s32 SaveState_DeleteSelectedSlot(void)
{
    s32 found;
    s32 value;
    s32 result = 0;

    found = SaveState_InitializeWorkspace();
    if (found != 0) {
        UiText_ShowPositionedMessageAndWait((s32)MsgNoBackupMemory, 1);
        result = -9;
    } else {
        SaveState_LoadSummaryRecords();
        value = SaveMenu_SelectSlot(0, 3);
        if (value == -1) {
            result = value;
        } else {
            UiText_OpenMessageWindow((s32)MsgEraseConfirm, 8, 1, 2);
            while (UiWork_IsComplete() == 0) {
                WaitFrames(1);
            }
            if (Menu_RunConfirmSelection(1, 0, 3, 1) != 0) {
                UiWork_FinalizePendingCore();
            } else {
                UiWork_FinalizePendingCore();
                found = SaveState_DeleteRecord(value);
                found |= SaveState_DeleteRecord(value + 3);
                if (found != 0) {
                    UiText_ShowPositionedMessageAndWait((s32)MsgEraseFailed, 1);
                    result = -4;
                } else {
                    UiText_ShowPositionedMessageAndWait((s32)MsgFileErased, 1);
                }
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return result;
}
