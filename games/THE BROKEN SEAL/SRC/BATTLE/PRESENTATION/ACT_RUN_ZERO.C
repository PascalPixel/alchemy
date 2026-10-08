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
#include "BATTLE_FORMATION.H"
#include "CHARACTER.H"
void BattlePresentation_SetPaletteLevel(s32 unused, s32 level);

void BattlePres_RunWithZeroArguments(void)
{
  BattlePresentation_SetPaletteLevel(0, 0);
}

typedef void (*InterruptHandler)(void);
void Runtime_SetIrqHandler(u32, s32, InterruptHandler);

void Runtime_RemoveIrqHandlerSlot2(void)
{
    Runtime_SetIrqHandler(2, 0, 0);
}


s32 Party_ComputeEligibleMemberAverage(s32 record_id)
{
    struct BattleFormationRecord *record;
    struct EnemyDefinition *enemy;
    u32 i;
    s32 id;
    s32 sum;
    s32 count;
    /* FAKEMATCH: an unread seven-word level table, the native frame's 28 bytes. */
    s32 levels[7];

    count = 0;
    sum = 0;
    record = &BattleFormation_Records[record_id];
    for (i = 0; i < 5; i++) {
        if (record->minimum_counts[i] != 0)
            break;
    }
    if (i == 5)
        return -1;
    for (i = 0; i < 5; i++) {
        if (record->maximum_counts[i] == 0)
            continue;
        id = record->member_ids[i];
        enemy = Owner_GetRecordFar(id + 8);
        if (enemy == 0)
            continue;
        if (enemy->level > 3 && !GameFlag_IsSet(372) && !GameFlag_IsSet(id + 1544))
            return -2;
        sum += enemy->level;
        count++;
    }
    if (count == 0)
        return -3;
    return sum / count;
}
