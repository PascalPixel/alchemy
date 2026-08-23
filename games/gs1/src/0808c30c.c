#include "types.h"
#include "sound_ids.h"

extern u8 Data_02000240[];

s32 Func_08077148(void);
u8 *Runtime_GetObject(s32);
s32 Func_080022ec(s32, s32);
void Func_08077118(s32, s32);
void Func_08091220(s32, s32);
void Func_08091254(s32);
void Audio_PlayCue(s32);

void Func_0808c30c(s32 amount, s32 scaled)
{
    s32 count;
    s32 found;
    s32 value;
    s32 offset;
    u8 *base;
    u8 *id;
    u8 *object;

    if (amount < 0) {
        Func_08091220(0x1ff, 0);
        Func_08091254(4);
        if (amount < -10)
            Audio_PlayCue(SOUND_HEAVY_IMPACT);
        else
            Audio_PlayCue(133);
    } else {
        Audio_PlayCue(SOUND_RECOVERY);
    }

    found = Func_08077148();
    if (found > 0) {
        base = Data_02000240;
        offset = 0x1f8;
        id = base + offset;
        count = found;
        do {
            object = Runtime_GetObject(*id);
            value = amount;
            if (scaled) {
                value = Func_080022ec(*(s16 *)(object + 0x34) * amount, 100);
                if (value == 0) {
                    value = amount;
                    if (value < 0) {
                        amount = value;
                        value = -value;
                    }
                }
            }
            count--;
            Func_08077118(*id, value);
            id++;
        } while (count != 0);
    }
}
