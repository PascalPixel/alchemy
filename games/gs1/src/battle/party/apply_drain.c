#include "types.h"
#include "scene.h"

/* battle/party/apply_drain.c */
extern s32 gCell[];
s32 FunctionHead_08077148(s32);
void FunctionHead_08077120(s32, s32);

void BattleParty_ApplyDrain(s32 amount)
{
    s32 target_count = FunctionHead_08077148(amount);

    if (target_count > 0) {
        u8 *base = (u8 *)&gCell;
        s32 offset = 252 << 1;
        u8 *target_id = base + offset;
        s32 remaining = target_count;

        do {
            FunctionHead_08077120(*target_id++, amount);
            remaining--;
        } while (remaining != 0);
    }
}
