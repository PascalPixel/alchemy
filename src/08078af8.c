typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 *Func_08077394(s32);
extern u8 *Func_08078414(s32);

s32 Func_08078af8(s32 arg0, s32 value)
{
    u8 *base = Func_08077394(arg0);
    s32 count = 0;
    s32 target = value & 0x1FF;
    s32 index = 0;
    s32 offset = 216;

    do {
        if ((*(u16 *)((u8 *)offset + (s32)base) & 0x1FF) == target) {
            u8 *data = Func_08078414(target);

            if (data[3] & 0x10) {
                count = (*(u16 *)((u8 *)offset + (s32)base) >> 11) + 1;
                break;
            }
            count++;
        }
        index++;
        offset += 2;
    } while (index <= 14);
    return count;
}
