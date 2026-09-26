/* Draft, not exact (2026-09-26): 412 of 412 bytes, 2 differing halfwords.
   A separate inline checksum helper, both with a local accumulator and
   with the initial accumulator passed in, sinks the zero initialization
   and removes the saved r8 accumulator. The prior in-function loop in Git
   remains the best 412-byte / two-halfword draft, restored below. The helper
   hypothesis does not recover the frame-count/bound scheduling and is
   preserved in commit 11637ce59, not adopted.
   Hand-written from the assembly. Residual: the reference schedules the load
   of the frame count after the first half of the 968 loop bound (movs r1);
   every order of the four loop-setup statements, barriers around each, and a
   symbol-bound game state leave it directly after its address load. */
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

extern u8 Data_02000000[];
extern u8 Data_02001000[];
#define gSaveSummary (*(struct SaveSummary *)(Data_02000000 - 16))
#define gSaveGameState (*(struct SaveGameState *)0x02000240)

u32 Func_08077300(s32);
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

    summary = &gSaveSummary;
    gSaveGameState.unknown_000 = Func_08077300(0);
    gSaveGameState.play_time = *(u32 *)0x03001c9c;
    {
        u32 *copy = (u32 *)Data_02001000;
        copy[64] = *(u32 *)0x03001c9c;
    }
    gSaveGameState.unknown_22a = *(u8 *)0x03001d08;
    owner = Owner_GetStateFar(gSaveGameState.leader);
    destination = summary->name;
    source = owner->name;
    for (i = 11; i >= 0; i--)
        *destination++ = *source++;
    summary->level = owner->level;
    summary->play_time = gSaveGameState.play_time;
    summary->area = BattleFx_FindConditionResourceFar(gSaveGameState.map, gSaveGameState.entrance);
    summary->class_id = owner->class_id;
    summary->coins = gSaveGameState.coins;
    summary->djinn[0] = Party_SumDjinnCountsFar(0);
    summary->djinn[1] = Party_SumDjinnCountsFar(1);
    summary->djinn[2] = Party_SumDjinnCountsFar(2);
    summary->djinn[3] = Party_SumDjinnCountsFar(3);
    Party_ListActiveOwnersFar(owners);
    for (i = 0; i <= 3 && owners[i] != 255; i++)
        summary->party[i] = owners[i];
    summary->party[i] = -1;
    summary->unknown_24 = gSaveGameState.unknown_205;
    summary->unknown_25 = gSaveGameState.unknown_206;
    summary->unknown_21 = gSaveGameState.unknown_20f;
    summary->flag_count = 0;
    for (i = 48; i <= 127; i++) {
        if (GameFlag_TestFar(i))
            summary->flag_count++;
    }
    summary->has_flag_20 = GameFlag_TestFar(32) != 0;
    {
        s32 n;

        word = (u32 *)0x02000040;
        i = 0;
        n = 968;
        summary->frames = gSaveGameState.unknown_000;
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
