#include "TYPES.H"

extern u8 *gEventWork;

extern s32 Encounter_SelectEnemyGroup();

u16 BattleFx_GetWeightedResult(s32 arg0, s32 arg1)
{
    return Encounter_EnemyGroupTable[(arg0 * 14) + arg1 + 2];
}
