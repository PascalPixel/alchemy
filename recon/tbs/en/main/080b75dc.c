/* alchemy permute: BattleUnit_RefreshPlacement against recon/tbs/raw/080b75dc.s: score 1970 (32 register-only, 10 operand, 8 reordered, 5 inserted, 6 deleted).
   Job 11, iteration 5500; rewrites: 9x swap commutative operands, 6x reorder independent statements, 4x introduce a temporary, 4x share one temporary between two statements, 4x add a same-width cast, 4x move an assignment into or out of a condition or call, 3x reorder local declarations, 3x remove a temporary, 3x drop a same-width cast, 3x change loop form, 3x split or join a compound assignment, 2x pointer arithmetic or indexing, 2x toggle register. */
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
#include "BATTLE_WORK.H"
extern const s8 BattlePlacement_StepPairs[];
s32 BattleParty_PrepareActiveOwners(u16 *ids);
void *GetBattleObjectSlot(s32 owner);
void BattlePresentation_SpawnActorObject(void *object, s32 owner, s32 x, s32 z);
void Summon_LayoutPositions(u16 *ids, s32 count, s32 *x, s32 *z);

void BattleUnit_RefreshPlacement(void)
{
    u16 ids[14];
    struct BattleSession *battle = gBattleWork;
    register s32 x[6];
    s32 z[6];
    s32 i = 0;
    s32 count;
    s32 n;
    s32 value;
    u8 *p;
    s32 pos;
    u16 *cursor;
    s32 tmp6;

    tmp6 = BattleParty_PrepareActiveOwners(ids);
    count = tmp6;
    n = 13;
    if (n >= 0) {
        while (1) {
            u8 *tmp4;
            tmp4 = battle->placement;
            tmp4[n] = 255;
            n--;
            if (n < 0)
                break;
        }
    }
    n = 13;
    while (8 <= n) {
        u8 *tmp7;
        tmp7 = battle->placement;
        tmp7 = tmp7 + n;
        *tmp7 = n;
        n--;
    }
    if (count > 0) {
        s32 tmp;
        s32 tmp5;
        tmp5 = 2 * i;
        tmp = tmp5;
        n = count;
        cursor = ids;
        pos = tmp;
        tmp5 = 0 != 1;
        if (tmp = tmp5) {
            do {
                s32 id = *cursor++;
                s32 tmp2;
                s32 tmp3;
                battle->placement[id] = i;
                tmp3 = (u32)(i + 1);
                tmp2 = tmp3;
                n--;
                BattlePresentation_SpawnActorObject(GetBattleObjectSlot(id), id, *(pos + BattlePlacement_StepPairs), *(BattlePlacement_StepPairs + (tmp3 = 1 + pos)));
                i = tmp2;
                pos += 2;
                if (!n)
                    break;
            } while (1 != 0);
        }
    }
    for (i = 0; 6 > i && 0xff != battle->enemy_units[i]; ++i)
        ids[i] = battle->enemy_units[i];
    count = i;
    i = 0;
    Summon_LayoutPositions(ids, count, x, z);
    for (; i < count; i += 1) {
        s32 id = battle->enemy_units[i];
        if (id != 0xfe)
            BattlePresentation_SpawnActorObject(GetBattleObjectSlot(id), id, x[i], z[i]);
    }
}
