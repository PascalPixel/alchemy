#include "types.h"

extern u8 *Data_03001f38;
extern u8 Data_0000001f;
extern volatile u32 Data_03001c94;
extern volatile u32 Data_03001b04;

void Func_08016478(s32);
void Func_0801e7c0(s32, s32, s32, s32);
void Func_080030f8(s32);
void Func_080f9010(s32);

s32 Func_08028574(s32 initialSelection)
{
    u8 *base = Data_03001f38;
    s16 *selection = (s16 *)(base + 140);
    s16 *count = (s16 *)(base + 146);

    *selection = (s16)initialSelection;

    for (;;) {
        s32 resourceId;

        Func_08016478(*(s32 *)(base + 120));

        if (*count != 0) {
            resourceId = *count + *selection;
        } else {
            resourceId = base[*selection + 132] + (s32)&Data_0000001f;
        }

        Func_0801e7c0(resourceId, *(s32 *)(base + 120), 0, 0);

        for (;;) {
            u32 keys;

            Func_080030f8(1);
            keys = Data_03001c94;

            if (keys & 1) {
                Func_080f9010(112);
                return *selection;
            }
            if (keys & 2) {
                Func_080f9010(113);
                return -1;
            }
            if (keys & 8) {
                Func_080f9010(113);
                return -1;
            }

            if ((Data_03001b04 & 32) != 0 || (Data_03001b04 & 64) != 0) {
                Func_080f9010(111);
                *selection = *selection - 1;
                if (*selection < 0) {
                    *selection = *(s16 *)(base + 142) - 1;
                }
                break;
            }

            if ((Data_03001b04 & 16) != 0 || (Data_03001b04 & 128) != 0) {
                Func_080f9010(111);
                *selection = *selection + 1;
                if (*selection >= *(s16 *)(base + 142)) {
                    *selection = 0;
                }
                break;
            }
        }
    }
}
