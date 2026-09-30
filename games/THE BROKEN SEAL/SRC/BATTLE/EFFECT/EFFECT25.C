#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "BATTLE_RUNTIME.H"

extern u16 gGameState[];

/*
 * Groups of battle keys, each group led by its cue with bit 15 set, ended by
 * zero. A key is (first << 4) + second; a key missing from the table takes
 * the last group's cue.
 */
extern const u16 gBattleCueTable[];

s32 GameFlag_TestFar(s32);

/* Picks the next battle's backdrop for a kind of terrain: grassland, forest,
   desert, beach or snowfield, else the Sol Sanctum's. Halfword 235 of the
   game state is the backdrop, which battle setup hands to
   BattleBackground_LoadFar. */
void BattleFx_SelectResultPointer(s32 arg0)
{
    u16 value;

    switch (arg0 - 1) {
    case 0:
        value = (u16)(u32)&ResourceId_GrasslandBackdrop;
        break;
    case 1:
        value = (u16)(u32)&ResourceId_ForestBackdrop;
        break;
    case 2:
        value = (u16)(u32)&ResourceId_DesertBackdrop;
        break;
    case 3:
    case 6:
        value = (u16)(u32)&ResourceId_BeachBackdrop;
        break;
    case 4:
    case 5:
        value = (u16)(u32)&ResourceId_SnowfieldBackdrop;
        break;
    default:
        value = (u16)(u32)&ResourceId_SoruShindenBackdrop;
        break;
    }
    gGameState[235] = value;
}

void BattleFx_SelectBattleCue(s32 first, s32 second)
{
    u16 key = (first << 4) + second;
    const u16 *tbl = gBattleCueTable;
    u16 cue;
    u16 value;

    if (GameFlag_TestFar(0x16c)) {
        cue = 18;
    } else {
        for (;;) {
            value = *tbl++;
            if (value == 0)
                break;
            if (value == key)
                break;
            if (value & 0x8000)
                cue = value & 0xfff;
        }
    }
    Data_02000240.battle_cue = cue;
}

s32 Party_RemapCharacterIdByFlags(s32 arg0)
{
    s32 result;

    result = arg0;
    if (result <= 8) {
        if (GameFlag_TestFar(0x20) != 0) {
            if (result == 0) {
                result = 0x12;
            }
            if (result == 1) {
                result = 0x13;
            }
        } else if ((GameFlag_TestFar(0x21) != 0) && (result == 0)) {
            result = 0x11;
        }
    }
    return result;
}

s32 Party_RemapCharacterIdIfEnabled(s32 arg0, s32 arg1)
{
    s32 result;

    result = arg0;
    if ((result <= 8) && (arg1 != 0)) {
        if (result == 0) {
            result = 0x12;
        }
        if (result == 1) {
            result = 0x13;
        }
    }
    return result;
}
