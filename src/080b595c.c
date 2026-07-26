#include "types.h"

extern u8 *Data_03001e74;
extern u8 Value_00000810;

void Func_08015118(void);
void Func_08015120(s32, s32);
void Func_080151c8(s32);
void Func_08015218(void);
s32 Func_080bb65c(void);
s32 Func_080b6ae0(s16 *);

void Func_080b595c(s32 count)
{
    s16 entries[8];
    u8 *state;
    s16 *entry;
    s32 index;

    state = Data_03001e74;
    Func_08015118();
    Func_080b6ae0(entries);

    index = 0;
    if (count != 0) {
        entry = entries;
        do {
            Func_08015120((u16)*entry++, 1);
            if (index == count - 1)
                Func_080151c8(0x811);
            else
                Func_080151c8((u32)&Value_00000810);
            index++;
            Func_080bb65c();
        } while (index != count);
    }

    Func_08015218();
    if (state[69] == 1) {
        Func_08015118();
        Func_08015120(0, 1);
        Func_080151c8(0x812);
        Func_080bb65c();
    } else if (state[69] == 2) {
        Func_08015118();
        Func_08015120(0, 1);
        Func_080151c8(0x813);
        Func_080bb65c();
    }
}
