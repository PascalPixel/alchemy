typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

s32 Func_080b6c08(s32, u16 *);

u32 Func_080be070(u32 arg0)
{
    u16 values[8];
    s32 count;
    s32 total;
    s32 i;

    count = 1;
    if (arg0 > 7) {
        count = 2;
    }
    total = Func_080b6c08(count, values);

    for (i = 0; i < total; i++) {
        if (values[i] == arg0) {
            break;
        }
    }

    return i != total;
}
