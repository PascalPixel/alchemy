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
#include "SCENE.H"
#include "GAME_STATE.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "BTLSYS.H"




void Party_ResolveTablePair(void)
{
    s16 first = gGameState.scene;
    s16 second = gGameState.entrance;
    const s16 *entry = Party_PairResolveRules;

    while (entry[0] != -1) {
        if (entry[0] == first &&
            (entry[1] == -1 || entry[1] == second)) {
            gGameState.saved_scene = entry[2];
            gGameState.saved_entrance = entry[3];
            return;
        }
        entry += 4;
    }
}

s16 Event_GetSpecialValue(void)
{
    return gGameState.special;
}

void BattleFx_SetSpecialFromTable(s32 arg0, s32 arg1)
{
    s32 target = gGameState.scene;
    s32 *table = Battle_BackdropScenes;
    s32 entry = *table++;
    s32 result = arg1;

    if (entry != 0 && entry != target) {
        do {
            if (entry & 0x80000000) {
                result = entry & 0xFFFF;
            }
            entry = *table++;
        } while (entry != 0 && entry != target);
    }
    gGameState.special = result;
}

void BattleFx_SelectResultPointer(s32 arg0)
{
    u16 value;

    switch (arg0 - 1) {
    case 0:
        value = (u32)&ResourceId_GrasslandBackdrop;
        break;
    case 1:
        value = (u32)&ResourceId_ForestBackdrop;
        break;
    case 2:
        value = (u32)&ResourceId_DesertBackdrop;
        break;
    case 3:
    case 6:
        value = (u32)&ResourceId_BeachBackdrop;
        break;
    case 4:
    case 5:
        value = (u32)&ResourceId_SnowfieldBackdrop;
        break;
    default:
        value = (u32)&ResourceId_SoruShindenBackdrop;
        break;
    }
    gGameState.special = value;
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
    gGameState.scene_cue = cue;
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
