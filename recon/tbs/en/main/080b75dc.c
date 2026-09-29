/* 2026-09-28: 304 of 304 bytes, 112 differing halfwords (was 320 / 136).
 * The unit half is written as Summon_Refresh (080b7548, exact): the ids
 * gathered by index, the count taken from it, each id read once. The
 * order fills stay descending loops; the party pass keeps the separate
 * down-counter and table offset. Remaining: the running index is not kept
 * in r9 from entry, and the frame is 84 bytes instead of 80. */
#include "TYPES.H"

struct PlacementWork {
    u8 reserved_000[0x66];
    s16 units[6];                   /* 0x66 */
    u8 reserved_072[0x26a];
    u8 order[14];                   /* 0x2dc */
};

extern struct PlacementWork *gBattleWork;
extern const s8 BattlePlacement_StepPairs[];
s32 BattleParty_PrepareActiveOwners(u16 *ids);
void *GetBattleObjectSlot(s32 owner);
void BattlePresentation_SpawnActorObject(void *object, s32 owner, s32 x, s32 z);
void Summon_LayoutPositions(u16 *ids, s32 count, s32 *x, s32 *z);

void BattleUnit_RefreshPlacement(void)
{
    u16 ids[14];
    s32 x[6];
    s32 z[6];
    struct PlacementWork *battle = gBattleWork;
    s32 i = 0;
    s32 count;
    s32 n;
    u8 *p;
    s32 value;
    u16 *cursor;
    s32 pos;

    count = BattleParty_PrepareActiveOwners(ids);
    for (n = 13; n >= 0; n--)
        battle->order[n] = 255;
    for (n = 13; n >= 8; n--)
        battle->order[n] = n;
    if (count > 0) {
        cursor = ids;
        pos = i * 2;
        n = count;
        do {
            s32 id = *cursor++;
            battle->order[id] = i;
            BattlePresentation_SpawnActorObject(GetBattleObjectSlot(id), id, BattlePlacement_StepPairs[pos], BattlePlacement_StepPairs[pos + 1]);
            n--;
            pos += 2;
            i++;
        } while (n != 0);
    }
    for (i = 0; i < 6 && battle->units[i] != 0xff; i++)
        ids[i] = battle->units[i];
    count = i;
    Summon_LayoutPositions(ids, count, x, z);
    for (i = 0; i < count; i++) {
        s32 id = battle->units[i];

        if (id != 0xfe)
            BattlePresentation_SpawnActorObject(GetBattleObjectSlot(id), id, x[i], z[i]);
    }
}
