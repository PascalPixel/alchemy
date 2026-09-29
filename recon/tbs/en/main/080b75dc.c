/* 2026-09-29 alchemy permute: score 3368 to 2135 on the permuter's scorer
   (0 is exact); remaining 41 register-only, 8 operand, 9 reordered, 4
   inserted, 8 deleted. Kept rewrites: 12x swap commutative operands, 7x
   reorder local declarations, 6x split or join a compound assignment, 5x
   introduce a temporary, 5x change loop form, 5x pointer arithmetic or
   indexing, 4x reorder independent statements, 3x remove a temporary, 2x
   test truth or compare with zero, 1x add a same-width cast, 1x drop a
   same-width cast, 1x toggle register. FAKEMATCH: the permuter's
   temporaries, register hints and swapped operand orders below only steer
   allocation and scheduling; no programmer would write them, so they stay
   tagged until a natural spelling replaces them. */
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
    register s32 x[6];
    struct PlacementWork *battle = gBattleWork;
    s32 z[6];
    s32 i = 0;
    s32 count;
    s32 n;
    u8 *p;
    s32 value;
    u16 *cursor;
    s32 pos;
    s32 tmp;

    tmp = BattleParty_PrepareActiveOwners(ids);
    count = tmp;
    for (n = 13; n >= 0; n--)
        battle->order[n] = 255;
    n = 13;
    while (n >= 8) {
        *(battle->order + n) = n;
        n--;
    }
    if (count > 0) {
        pos = 2 * i;
        n = count;
        cursor = ids;
        if (1 != 0) {
            do {
                s32 id = *cursor++;
                s32 tmp2;
                battle->order[id] = i;
                tmp2 = i + 1;
                BattlePresentation_SpawnActorObject(GetBattleObjectSlot(id), id, BattlePlacement_StepPairs[pos], BattlePlacement_StepPairs[1 + pos]);
                i = tmp2;
                n--;
                pos += 2;
                if (!n)
                    break;
            } while (1 != 0);
        }
    }
    for (i = 0; 6 > i && 0xff != battle->units[i]; ++i)
        ids[i] = battle->units[i];
    count = i;
    Summon_LayoutPositions(ids, count, x, z);
    i = 0;
    while (count > i) {
        s32 id = battle->units[i];
        if (id != 0xfe)
            BattlePresentation_SpawnActorObject(GetBattleObjectSlot(id), id, x[i], z[i]);
        i += 1;
    }
}
