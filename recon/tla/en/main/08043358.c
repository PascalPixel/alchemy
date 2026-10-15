#include "types.h"
#include "ram_buffer.h"
#include "save_state.h"
#include "party_state.h"
#include "battle_unit.h"
#include "owner_state.h"
#include "frame.h"
#include "scene.h"
#include "menu.h"

struct SaveGameWords {
    u32 buildStamp;
    u32 playTime;
};

#define SAVE_GAME_WORDS ((struct SaveGameWords *)&gPartyState)

extern u8 gData_02000000[];
extern u32 gData_02001000;
extern u32 gGameFlagBytes[];

u32 Runtime_GetBuildStampTimeFar(s32);
s32 GameFlag_Test(s32 flag);

// セーブの見出しを作る
s32 SaveState_BuildSummaryHeader(void)
{
    u16 owners[14];
    struct SaveSummary *summary;
    struct BattleUnit *owner;
    u8 *source;
    u8 *destination;
    u32 *word;
    s32 i;
    u32 sum = 0;

    summary = (struct SaveSummary *)(gData_02000000 - 16);
    SAVE_GAME_WORDS->buildStamp = Runtime_GetBuildStampTimeFar(0);
    SAVE_GAME_WORDS->playTime = gFrameWaitCount;
    gData_02001000 = gFrameWaitCount;
    gPartyState.unk244[6] = gAutoSleepEnabled;
    owner = UnitGet(gPartyState.currentOwner);
    destination = summary->name;
    source = owner->name;
    for (i = 11; i >= 0; i--)
        *destination++ = *source++;
    summary->level = owner->level;
    summary->playTime = SAVE_GAME_WORDS->playTime;
    summary->area = Scene_GetAreaNameIndexFar(gPartyState.scene, gPartyState.entrance);
    summary->classId = owner->classIndex;
    summary->coins = gPartyState.coins;
    summary->djinn[0] = Party_SumDjinnCountsFar(0);
    summary->djinn[1] = Party_SumDjinnCountsFar(1);
    summary->djinn[2] = Party_SumDjinnCountsFar(2);
    summary->djinn[3] = Party_SumDjinnCountsFar(3);
    Party_ListActiveOwnersFar((s16 *)owners);
    for (i = 0; i <= 3 && owners[i] != 255; i++)
        summary->party[i] = owners[i];
    summary->party[i] = -1;
    summary->paletteGlow = gPartyState.unk220[5];
    summary->paletteGlowSub = gPartyState.unk220[6];
    summary->sendFlag = gPartyState.unk22d[2];
    summary->flagCount = 0;
    for (i = 48; i <= 127; i++) {
        if (GameFlag_Test(i))
            summary->flagCount++;
    }
    summary->hasFlag34 = GameFlag_Test(34) != 0;
    {
        s32 n;
        u32 frames;

        frames = SAVE_GAME_WORDS->buildStamp;
        word = gGameFlagBytes;
        /* FAKEMATCH: keeps the two loads before the bound (differ 4 more without) */
        __asm__("" : : "r"(frames), "r"(word));
        n = 242;
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
    return 0;
}
