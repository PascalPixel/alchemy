#include "OWNER_STATE.H"
#include "PARTY_STATE.H"

struct LevelUpResult;
struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *result);
void Owner_RecalculateStats(s32);

void Party_AdvanceOwnerCountToTarget(s32 owner, s32 target)
{
    u8 buf[16];
    s32 count = ((struct BattleUnit *)Owner_GetState(owner))->level;

    while (count < target) {
        Owner_LevelUp(owner, (struct LevelUpResult *)buf);
        count++;
    }
    Owner_RecalculateStats(owner);
}
