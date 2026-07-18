typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 *Data_03001e60;
void *Func_08185000(s32);
void Func_0800b868(void *);

void Func_08012de8(s32 arg0, s32 arg1)
{
    u8 *base = Data_03001e60;
    s32 offset;
    s32 count;

    Func_08185000(arg1);
    offset = (arg0 & 3) * 4 + 40;
    count = 9;
    do {
        u8 *entry = *(u8 **)(base + offset);

        count--;
        *(u16 *)entry = arg1;
        Func_0800b868(entry);
        base += 56;
    } while (count >= 0);
}
