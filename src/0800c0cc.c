typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

extern u8 *Data_03001e64;

void *Func_0800c0cc(void)
{
    u8 *entry = Data_03001e64;
    void *result = 0;
    s32 index = 0;

    while (index <= 63) {
        if (*(u32 *)entry == 0) {
            result = entry;
            break;
        }
        index++;
        entry += 112;
    }
    return result;
}
