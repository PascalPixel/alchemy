typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

struct State_080796c4 {
    u8 padding[0x1f8];
    u8 active[8];
};

extern struct State_080796c4 Data_02000240;
s32 Func_080795fc(void);

s32 Func_080796c4(s16 *output)
{
    s32 count = 0;

    if (output != 0) {
        s32 index;

        count = Func_080795fc();
        index = 0;
        if (count != 0) {
            do {
                *output++ = Data_02000240.active[index];
                index++;
            } while (index != count);
        }
        *output = 0xff;
    }
    return count;
}
