#include "fixed_math.h"
#include "types.h"
#include "battle_msg.h"
#include "battle_party.h"
#include "battle_escape.h"

u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void WaitFrames(u32);
void Func_080b8064(s32);
void Func_080151c8(s32);

s32 BattleEscape_PlayRun(s16 *action)
{
    s16 party_members[14];
    s32 party_size;
    s32 animated;
    s32 member_slot;

    (void)action;
    if (((u32)(Rand() << 4) >> 16) != 0) {
        party_size = BattleParty_ListLivingUnits(
            BATTLE_SIDE_PARTY,
            party_members);
        animated = 0;
        if (party_size != 0) {
            member_slot = 0;
            do {
                Func_080b8064(party_members[member_slot]);
                animated++;
                WaitFrames(8);
                member_slot++;
            } while (animated != party_size);
        }
        WaitFrames(22);
        return 1;
    }
    Func_080151c8((s32)&Value_00000844);
    return 0;
}
