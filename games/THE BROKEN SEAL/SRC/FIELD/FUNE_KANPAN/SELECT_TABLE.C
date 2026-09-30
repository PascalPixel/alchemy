#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"

union Slot {
    s32 w;
    s16 h[2];
};

extern s32 FuneKanpan_FlagGroupEntries[];
extern u8 LinkedMessage_TheresNothingWeCanDo[];

s32 BuildMotionCountdown(s32, s16);

s32 SceneData_SelectTableEntryByFlagGroup(u32 sel)
{
    s32 base = 0;
    u32 i;

    switch (sel) {
    case 0:
        base = 0x92C;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (GameFlag_IsSet(base + i) != 0) return FuneKanpan_FlagGroupEntries[i];
    }
    return 0;
}
