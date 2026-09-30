#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "SAVE_STATE.H"
#include "RUNTIME_INTERFACES.H"

extern u8 Data_03001e90[];
s32 Runtime_ReleaseHeapBlock(s32);
void UiWork_Finalize(struct Work *work, s32 release);
extern u8 Data_03001e8c[];

u32 Math_DivU(s32, s32);
s32 Math_ModU(s32, s32);
u8 *UiText_FormatNumber(u8 *, s32, s32);

extern u8 Data_03001f1c[];
extern u8 Data_03001ae8[];
s32 SaveState_InitializeWorkspace(void);
s32 SaveState_LoadSummaryRecords(void);
extern s16 gTitleExtraOptionEnabled;

struct SaveSummary {
    u8 slot_header[16];
    u8 name[12];
    u8 level;
    u8 class_id;
    u16 area;
    u32 play_time;
    u32 coins;
    u8 djinn[4];
    s8 party[5];
    u8 unknown_21;
    u8 flag_count;
    u8 has_flag_20;
    u8 unknown_24;
    u8 unknown_25;
    u16 frames;
    u8 padding28[4];
    u32 checksum;
};

struct SaveGameState {
    u32 unknown_000;
    u32 play_time;
    u8 padding008[8];
    u32 coins;
    u8 padding014[0x1ac];
    s16 map;
    s16 entrance;
    u8 padding1c4[0x30];
    s32 leader;
    u8 padding1f8[0x0d];
    u8 unknown_205;
    u8 unknown_206;
    u8 padding207[8];
    u8 unknown_20f;
    u8 padding210[0x1a];
    u8 unknown_22a;
};

struct OwnerState {
    u8 name[15];
    u8 level;
    u8 padding10[0x119];
    u8 class_id;
};

extern u8 gSaveBuffer[];
extern u8 gSceneState[];
extern struct SaveGameState gGameState;
extern u32 gLoadedStateWord;
extern u8 gOptionMirror;
extern u32 GameFlagBytes[];
u32 Runtime_GetBuildStampTimeFar(s32);
struct OwnerState *Owner_GetStateFar(s32 owner);
u16 BattleFx_FindConditionResourceFar(s32 map, s32 entrance);
u8 Party_SumDjinnCountsFar(s32 element);
void Party_ListActiveOwnersFar(u16 *owners);
s32 GameFlag_TestFar(s32 flag);

extern u8 gSaveSlot[];
s32 SaveState_WriteRecord(s32, void *);
void UiText_ShowPositionedMessageAndWait(s32, s32);
extern char MsgNoBackupMemory;
extern char MsgSaveFailed;

u32 SaveState_BuildSummaryHeader(void);

void UiWork_FinalizeAndReleaseBlock16(void)
{
    UiWork_Finalize(**(s32 **)((u32)&Data_03001e90), 1);
    Runtime_ReleaseHeapBlock(0x10);
}

void UiWindow_SetTileAttributeBitRect(
    const u8 *window, s32 x, s32 y, s32 width, s32 height, u32 field)
{
    u8 *base = *(u8 **)((u32)&Data_03001e8c);

    x += *(u16 *)(window + 12) + 1;
    y += *(u16 *)(window + 14) + 1;
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
            u16 *cell = (u16 *)((u32)x + (u32)base);
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
        base[RENDER_DIRTY_OFS] = 1;
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

    time = Math_DivU(value, 0xe10);
    if (time > 0xea5f)
        time = 0xea5f;

    minutes = Math_DivU(time, 60);
    seconds = Math_ModU(time, 60);

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
    u8 *p;

    p = (u8 *)(*(s32 *)((u32)&Data_03001f1c) + 0x1040);
    for (i = 0; i < 3; i++, p += 0x40) {
        if (p[0x1c] == 0)
            return i;
    }
    return 0x3E7;
}

s32 SaveState_CountRecordsExcludingFlagged(s32 flag)
{
    s32 i;
    s32 cnt;
    s8 *p;

    if (SaveState_InitializeWorkspace() != 0) {
        cnt = -9;
    } else {
        cnt = SaveState_LoadSummaryRecords();
        if (flag != 0) {
            p = (s8 *)(*(s32 *)((u32)&Data_03001f1c) + 0x1070);
            for (i = 0; i < 3; i++) {
                if (p[i * 0x40 + 1] != 0)
                    cnt--;
            }
        }
    }
    SaveState_ReleaseWorkspace();
    return cnt;
}

s32 SaveState_ScanRecordFlags(void)
{
    s32 err;
    s32 cnt;
    s32 ret;

    err = SaveState_InitializeWorkspace();
    cnt = 0;
    ret = -9;
    if (err == 0) {
        s32 i;
        s8 *p;

        ret = SaveState_LoadSummaryRecords();
        p = (s8 *)(*(s32 *)((u32)&Data_03001f1c) + 0x1070);
        gTitleSendOptionEnabled = 0;
        gTitleExtraOptionEnabled = 0;
        for (i = 0; i < 3; i++) {
            if (p[i * 0x40 + 1] != 0) {
                gTitleSendOptionEnabled = 1;
                cnt++;
            }
            if (p[i * 0x40 + 2] != 0) {
                gTitleExtraOptionEnabled = 1;
            }
        }

        if ((*(volatile s32 *)((u32)&Data_03001ae8) & 0x120) != 0x120) {
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
    struct OwnerState *owner;
    u8 *source;
    u8 *destination;
    u32 *word;
    s32 i;
    u32 sum = 0;

    summary = (struct SaveSummary *)(gSaveBuffer - 16);
    gGameState.unknown_000 = Runtime_GetBuildStampTimeFar(0);
    gGameState.play_time = gLoadedStateWord;
    {
        u32 *copy = (u32 *)gSceneState;
        copy[64] = gLoadedStateWord;
    }
    gGameState.unknown_22a = gOptionMirror;
    owner = Owner_GetStateFar(gGameState.leader);
    destination = summary->name;
    source = owner->name;
    for (i = 11; i >= 0; i--)
        *destination++ = *source++;
    summary->level = owner->level;
    summary->play_time = gGameState.play_time;
    summary->area = BattleFx_FindConditionResourceFar(gGameState.map, gGameState.entrance);
    summary->class_id = owner->class_id;
    summary->coins = gGameState.coins;
    summary->djinn[0] = Party_SumDjinnCountsFar(0);
    summary->djinn[1] = Party_SumDjinnCountsFar(1);
    summary->djinn[2] = Party_SumDjinnCountsFar(2);
    summary->djinn[3] = Party_SumDjinnCountsFar(3);
    Party_ListActiveOwnersFar(owners);
    for (i = 0; i <= 3 && owners[i] != 255; i++)
        summary->party[i] = owners[i];
    summary->party[i] = -1;
    summary->unknown_24 = gGameState.unknown_205;
    summary->unknown_25 = gGameState.unknown_206;
    summary->unknown_21 = gGameState.unknown_20f;
    summary->flag_count = 0;
    for (i = 48; i <= 127; i++) {
        if (GameFlag_TestFar(i))
            summary->flag_count++;
    }
    summary->has_flag_20 = GameFlag_TestFar(32) != 0;
    {
        /* FAKEMATCH: keeps the bound in r1 */
        register s32 n asm("r1");
        u32 *frame = &gGameState.unknown_000;
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
    value = *(s16 *)gSaveSlot;
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

            found = SaveState_WriteRecord(*(s16 *)gSaveSlot, base);
            next = *(s16 *)gSaveSlot;
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
