#include "types.h"

void Func_08016478(void *);
void Func_0801e74c(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern u8 Data_03001f38;
extern u8 Value_00000c71;
extern u8 Value_00000c73;

void Func_08028b80(void)
{
    u8 *state = *(u8 **)&Data_03001f38;
    s16 *shown = (s16 *)(state + 150);
    s16 *current = (s16 *)(state + 140);

    /* 値が変わった時だけ表示を更新する。 */
    if (*shown != *current) {
        *shown = (u16)*current;
        Func_08016478(*(void **)(state + 124));
        if (*current == 0) {
            Func_0801e74c((s32)&Value_00000c71,
                *(void **)(state + 124), 16, 4);
            Func_0801e74c((s32)&Value_00000c71 + 1,
                *(void **)(state + 124), 16, 16);
        } else {
            Func_0801e74c((s32)&Value_00000c73,
                *(void **)(state + 124), 0, 4);
            Func_0801e74c((s32)&Value_00000c73 + 1,
                *(void **)(state + 124), 0, 16);
            Func_0801e74c((s32)&Value_00000c73 + 2,
                *(void **)(state + 124), 0, 28);
        }
    }
}
