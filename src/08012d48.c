typedef unsigned char u8;
typedef signed int s32;

extern u8 *Data_03001e60;

void Func_08012d48(s32 arg0, s32 arg1)
{
    u8 *base = Data_03001e60;
    s32 offset = (arg0 & 3) * 4 + 40;
    s32 count = 9;

    do {
        u8 *entry = *(u8 **)(base + offset);

        count--;
        entry[6] = arg1;
        base += 56;
    } while (count >= 0);
}
