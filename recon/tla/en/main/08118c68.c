/* Draft, differ 6 of 89 (2026-10-15), ported from The Broken Seal's BattleParty_CollectUnitList:
 * the three halfword 0xff end markers load from the literal pool one or two rows earlier in the
 * native than here (ldr r2, =0xff before lsls r3, r6, #1, and so on). */
#include "types.h"
#include "battle_work.h"
#include "battle_party.h"

s32 BattleEnemyList(s16 *unit_ids);
extern const s8 gData_081287c8[];

void Func_08118c68(void)
{
    u16 buf[14];
    struct BattleWork *battle;
    s32 count;
    s32 i;
    s32 index;
    s32 kind;

    battle = Ram_HeapSlots->battleWork;
    count = BattleParty_PrepareActiveOwners(buf);
    for (i = 0; i < count; i++)
        battle->partyUnits[i] = buf[i];
    battle->partyUnits[count] = 0xff;

    count = BattleEnemyList(buf);
    kind = battle->unk042;
    if (kind >= 0) {
        if (kind <= 1) {
            for (i = 0; i < count; i++)
                battle->enemyIds[i] = buf[i];
            goto done;
        }
    }
    for (i = 0; i < count; i++) {
        index = gData_081287c8[i] + count / 2;
        battle->enemyIds[index] = buf[i];
    }
done:
    battle->enemyIds[count] = 0xff;
}
