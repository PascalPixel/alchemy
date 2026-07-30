typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

u32 Func_08004458(void);

extern u8 Data_080c5c38;

u32 Func_080c1fa8(u32 id)
{
    u32 buffer[5];
    u8 *entry;
    u8 *flag;
    u8 *value;
    u32 *out;
    s32 remaining;
    s32 count;

    count = 0;
    if (id >= 380) {
        id = 1;
    }
    entry = &Data_080c5c38 + id * 16;
    out = buffer;
    value = entry + 1;
    flag = entry + 11;
    remaining = 4;
    do {
        if (*flag++ != 0) {
            *out++ = *value + 8;
            count++;
        }
        remaining--;
        value++;
    } while (remaining >= 0);
    return buffer[count * Func_08004458() >> 16];
}
