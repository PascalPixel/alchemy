#include "types.h"

struct BattlePartyTargetState {
    u8 padding[0x1f8];
    u8 target_ids[1];
};

extern struct BattlePartyTargetState Data_02000240;

s32 Func_08077148(s32);
void Func_08077120(s32, s32);

void BattleParty_ApplyDrain(s32 amount)
{
    s32 target_count = Func_08077148(amount);

    if (target_count > 0) {
        u8 *base = (u8 *)&Data_02000240;
        s32 offset = 252 << 1;
        u8 *target_id = base + offset;
        s32 remaining = target_count;

        do {
            Func_08077120(*target_id++, amount);
            remaining--;
        } while (remaining != 0);
    }
}
