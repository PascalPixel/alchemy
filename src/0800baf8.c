typedef unsigned char u8;
typedef signed int s32;

void Func_0800baf8(u8 *arg0, s32 arg1)
{
    u8 initial = arg0[0x27];
    if (initial != 0) {
        void **cursor = (void **)(arg0 + 0x28);
        s32 count = initial;
        do {
            u8 *item = *cursor++;
            if (item != 0 && *(s32 *)(item + 0xc) != 0)
                item[0x15] = arg1;
            count--;
        } while (count != 0);
    }
}
