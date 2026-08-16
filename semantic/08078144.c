#include "types.h"
#include "party_state.h"

extern u8 Data_0807a828[];

u8 *Func_08077394(s32 owner);
s32 Func_08079338(s32 value);
s32 Func_080022ec(s32 numerator, s32 denominator);

void Func_08078144(void)
{
    s32 count;
    s32 index;

    count = Func_080795fc();
    index = 0;
    while (index < count) {
        s32 owner = Data_02000240.active_owners[index];
        s32 enabled = 0;

        if (Data_0807a828[owner] == 0) {
            if (Func_08079338(0x110) != 0
                || Func_08079338(0x112) != 0)
                enabled = 1;
        } else {
            if (Func_08079338(0x111) != 0
                || Func_08079338(0x113) != 0)
                enabled = 1;
        }

        if (enabled != 0) {
            u8 *state = Func_08077394(owner);
            s32 t;
            s32 v14;
            s32 v16;
            s32 one;
            s32 numerator;
            s32 denominator;

            *(u16 *)(state + 58) = *(u16 *)(state + 54);

            numerator = *(s16 *)(state + 56);
            denominator = *(s16 *)(state + 52);
            t = Func_080022ec(numerator << 14, denominator);
            v14 = 0x4000;
            if (t <= 0x4000) {
                v14 = 0;
                if (t >= 0)
                    v14 = t;
            }
            *(s16 *)(state + 20) = (s16)v14;
            if (((v14 << 16) == 0) && (*(s16 *)(state + 56) != 0)) {
                one = 1;
                *(s16 *)(state + 20) = (s16)one;
            }

            t = Func_080022ec(*(s16 *)(state + 58) << 14,
                              *(s16 *)(state + 54));
            v16 = 0x4000;
            if (t <= 0x4000) {
                v16 = 0;
                if (t >= 0)
                    v16 = t;
            }
            *(s16 *)(state + 22) = (s16)v16;
            if (((v16 << 16) == 0) && (*(s16 *)(state + 58) != 0)) {
                one = 1;
                *(s16 *)(state + 22) = (s16)one;
            }
        }

        index++;
    }
}
