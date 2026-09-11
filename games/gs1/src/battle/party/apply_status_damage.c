#include "types.h"
#include "scene.h"
#include "abi/battle/party/apply_status_damage.h"

extern u8 gCell[];

u8 *Runtime_GetObject(s32);
s32 FixedPoint_Ratio(s32, s32);

void BattleFx_ApplyColorToSourceBuffer(void *, s32);
void BattleFx_StartBufferInterpolation(s32);
void Audio_PlayCue(s32);

s32 BattleParty_ApplyStatusDamage(void)
{
    s32 result = 0;
    s32 count = Battle_Check();

    if (result < count) {
        s32 offset = 252;
        u8 *entry;
        s32 remaining;

        offset <<= 1;
        entry = gCell + offset;
        remaining = count;

        do {
            u8 *object = Runtime_GetObject(*entry);
            s32 amount;

            switch ((s8)object[0x131]) {
            case 1:
                amount = -FixedPoint_Ratio(*(s16 *)(object + 0x34) + 10, 20);
                if (amount == 0)
                    amount = -1;
                if (result <= 0)
                    result = 1;
                break;
            case 2:
                amount = -FixedPoint_Ratio(*(s16 *)(object + 0x34) + 5, 10);
                if (amount == 0)
                    amount = -1;
                if (result <= 1)
                    result = 2;
                break;
            default:
                amount = 0;
                break;
            }

            remaining--;
            Battle_Apply(*entry, amount);
            entry++;
        } while (remaining != 0);
    }

    if (result != 0) {
        BattleFx_ApplyColorToSourceBuffer((void *)0x1ff, 0);
        BattleFx_StartBufferInterpolation(4);
        Audio_PlayCue(133);
    }

    return result;
}
