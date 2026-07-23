typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 *Data_03001e74;
extern u8 Data_02000240[];

s32 Func_08077148(void);
void *Func_08077008(s32);

s32 Func_080b6a60(u16 *output)
{
    s32 maximum;
    s32 count;
    s32 remaining;
    s32 index;

    maximum = 4;
    if (Data_03001e74[68] != 0)
        maximum = 3;

    count = Func_08077148();
    if (count > maximum)
        count = maximum;

    if (count > 0) {
        index = 0;
        remaining = count;
        do {
            u8 id = Data_02000240[504 + index++];
            if (output != 0)
                *output++ = id;
            remaining--;
            *(u8 *)((u8 *)Func_08077008(id) + 0x12a) = 2;
        } while (remaining != 0);
    }

    if (output != 0)
        *output = 0xff;

    return count;
}
