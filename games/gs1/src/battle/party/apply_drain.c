#include "types.h"
#include "scene.h"
#include "abi/battle/party/apply_drain.h"

struct BattlePartyTargetState {
    u8 padding[0x1f8];
    u8 target_ids[1];
};

extern struct BattlePartyTargetState gCell;

void BattleParty_ApplyDrain(s32 amount)
{
    s32 target_count = Battle_Check(amount);

    if (target_count > 0) {
        u8 *base = (u8 *)&gCell;
        s32 offset = 252 << 1;
        u8 *target_id = base + offset;
        s32 remaining = target_count;

        do {
            Battle_Apply(*target_id++, amount);
            remaining--;
        } while (remaining != 0);
    }
}
