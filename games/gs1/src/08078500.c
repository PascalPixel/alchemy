#include "types.h"

struct Global_08078500 {
    u8 padding[500];
    s32 owner;
};

s32 Func_080784d8(s32 owner);
s32 Func_080796c4(s16 *);
extern struct Global_08078500 Data_02000240;

s32 Func_08078500(void)
{
    s16 owners[10];
    s32 count;
    s32 index;
    s16 *owner_cursor;

    if (Func_080784d8(Data_02000240.owner) != 15)
        return 1;
    count = Func_080796c4(owners);
    owner_cursor = owners;
    index = 0;
    if (index < count) {
        do {
            if (Func_080784d8(*owner_cursor++) != 15)
                return 1;
            index++;
        } while (index < count);
    }
    return 0;
}
