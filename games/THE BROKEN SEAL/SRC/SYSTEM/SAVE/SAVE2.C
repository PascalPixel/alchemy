#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "SAVE_STATE.H"
#include "GAME_STATE.H"
#include "BATTLE_UNIT.H"
#include "RUNTIME_INTERFACES.H"
#include "WINDOW.H"

void Runtime_ReleaseHeapBlock(s32 slot);

u8 *UiText_FormatNumber(u8 *, s32, s32);

extern volatile u32 gKeysHeld;
extern s16 gTitleExtraOptionEnabled;

extern u8 gSaveBuffer[];
extern u8 gSceneState[];
extern u32 gLoadedStateWord;
extern u8 gOptionMirror;
extern u32 GameFlagBytes[];
u32 Runtime_GetBuildStampTimeFar(s32);
u16 BattleFx_FindConditionResourceFar(s32 map, s32 entrance);
u8 Party_SumDjinnCountsFar(s32 element);
void Party_ListActiveOwnersFar(u16 *owners);
s32 GameFlag_TestFar(s32 flag);

extern s16 gSaveSlot;
void UiText_ShowPositionedMessageAndWait(s32, s32);
extern char MsgNoBackupMemory;
extern char MsgSaveFailed;

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
            u16 *cell = (u16 *)((u32)x + (u32)work->tilemap);
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
    /* FAKEMATCH: retain the existing signed byte cursor from the summary
       party tail. Direct flag fields fold a different pool base at the
       same 156-byte extent; offsets below come from the real record. */
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

        if ((gKeysHeld & 0x120) != 0x120) {
            gTitleSendOptionEnabled = 0;
        }
    }
    SaveState_ReleaseWorkspace();
    if (ret != 0 && cnt == ret) {
        return ret + 100;
    }
    return ret;
}

/* Save: fill the summary header at the start of the save image (leader name
   and level, area, play time, coins, Djinn counts, party and progress
   counters) and its checksum over the rest of the image. */
u32 SaveState_BuildSummaryHeader(void)
{
    u16 owners[14];
    struct SaveSummary *summary;
    struct BattleUnit *owner;
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
    owner = Owner_GetStateFar(gGameState.selected_actor);
    destination = summary->name;
    source = owner->name;
    for (i = 11; i >= 0; i--)
        *destination++ = *source++;
    summary->level = owner->level;
    summary->play_time = gGameState.play_time;
    summary->area = BattleFx_FindConditionResourceFar(gGameState.scene, gGameState.entrance);
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
    summary->palette_glow[0] = gGameState.palette_glow[0];
    summary->palette_glow[1] = gGameState.palette_glow[1];
    summary->send_flag = gGameState.unknown_20f;
    summary->flag_count = 0;
    for (i = 48; i <= 127; i++) {
        if (GameFlag_TestFar(i))
            summary->flag_count++;
    }
    summary->has_flag_20 = GameFlag_TestFar(32) != 0;
    {
        /* FAKEMATCH: keeps the bound in r1 */
        register s32 n asm("r1");
        u32 *frame = &gGameState.build_stamp;
        u32 frames;

        n = 242;
        /* FAKEMATCH: issues the bound's first half before the frame load */
        asm("" : "+r"(n) : "r"(frame));
        frames = *frame;
        word = GameFlagBytes;
        i = 0;
        /* FAKEMATCH: finishes the bound after the loop setup, before the store */
        asm("lsl %0, %0, #2" : "+r"(n) : "r"(word), "r"(i), "r"(frames));
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
            UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
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
                UiText_ShowPositionedMessageAndWait((s32)&MsgSaveFailed, 1);
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
        UiText_ShowPositionedMessageAndWait((s32)&MsgNoBackupMemory, 1);
        result = -9;
    } else {
        void *base = &gSaveBuffer;

        found = SaveState_WriteRecord(arg0, base);
        base = (char *)base + 0x1000;
        found |= SaveState_WriteRecord(arg0 + 3, base);
        if (found != 0) {
            UiText_ShowPositionedMessageAndWait((s32)&MsgSaveFailed, 1);
            result = -3;
        }
    }
    SaveState_ReleaseWorkspace();
    return result;
}
