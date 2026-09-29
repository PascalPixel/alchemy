/* Save: fill the summary header at the start of the save image (leader name
   and level, area, play time, coins, Djinn counts, party and progress
   counters) and its checksum over the rest of the image. */
#include "TYPES.H"

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
