/* Near miss: differ 6 of 143 (the opening only). The native loads the battle work before it takes
 * the id list address (movs r3, #192; sub sp, #80; lsls; ldr r3, [r3, #36]; mov r2, sp; adds r2, #52); the
 * draft interleaves the two. A barrier after the load, or the load as the first statement, swaps r9 and r10.
 * The Broken Seal source plus the reserve owners; the second fill loop needs its pointer set inside the
 * for initialiser. */
#include "types.h"
#include "ram_buffer.h"
#include "btlwork.h"
#include "battle_session.h"

extern const s8 BattlePlacement_StepPairs[];
void BtlObjMake(struct BtlObj *actor, s32 unit, s32 x, s32 y);
s32 BattleParty_PrepareActiveOwners(u16 *ids);
s32 BattleParty_PrepareReserveOwners(u16 *ids);
void Func_0811b180(u16 *unit_ids, s32 count, s32 *x, s32 *z);
struct BtlObj *BtlObjGet(s32 id);

void BattleUnit_RefreshPlacement(void)
{
    u16 ids[14];
    s32 x[6];
    s32 z[6];
    s32 count;
    s32 i;
    s32 place;
    s32 stand;
    s32 n;
    s32 value;
    u8 *p;
    struct BtlWork *work;

    place = 0;
    stand = 0;
    work = Ram_HeapSlots->battle_work;
    count = BattleParty_PrepareActiveOwners(ids);
    count += BattleParty_PrepareReserveOwners(ids + count);
    value = 0xff;
    for (i = 13; i >= 0; i--)
        work->placement[i] = value;
    for (n = 5, p = &work->placement[13], value = 13; n >= 0; n--, value--)
        *p-- = value;
    for (i = 0; i < count; place++, i++) {
        s32 id = ids[i];

        work->placement[id] = place;
        BtlObjMake(BtlObjGet(id), id, BattlePlacement_StepPairs[place * 2],
            BattlePlacement_StepPairs[place * 2 + 1]);
    }
    for (i = 0; i < 6 && work->enm_no[i] != 0xff; i++)
        ids[i] = work->enm_no[i];
    count = i;
    Func_0811b180(ids, count, x, z);
    for (i = 0; i < count; i++, stand++) {
        s32 id = work->enm_no[i];

        if (id != 0xfe)
            BtlObjMake(BtlObjGet(id), id, x[stand], z[stand]);
    }
}
