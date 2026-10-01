/*
 * Draft: Trade_ListFlaggedEntries, ported from its ☀️ twin with ⚓️'s 29
 * summons. Score 60: the listing loads the offer flags (ldr r3, [r0])
 * before it copies the hoisted 1 (mov r2, r8); this copies first. A local
 * for the flags and swapped mask operands both score worse.
 */
#include "TYPES.H"
#include "BATTLE_SUMMON.H"

struct State_0807977c {
    u32 flags;
};

extern u8 Summon_OrderList[29];
struct State_0807977c *Trade_GetOfferState(s32);

s32 Trade_ListFlaggedEntries(u8 *output)
{
    u8 *source = Summon_OrderList;
    u8 *end = Summon_OrderList + 28;
    s32 count = 0;

    do {
        u8 value = *source++;
        if ((Trade_GetOfferState(0)->flags & (1 << value)) != 0) {
            *output++ = value;
            count++;
        }
    } while (source <= end);
    *output = 32;
    return count;
}
