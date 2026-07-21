typedef signed int s32;
typedef unsigned char u8;

void Func_0800c388(void *arg0, s32 arg1, s32 arg2) {
    void **items;
    void *item;
    s32 count;

    if (arg0 != 0) {
        switch (*((u8 *)arg0 + 84) & 15) {
        case 1:
            Func_0800ba30(*(s32 *)((u8 *)arg0 + 80), arg1);
            Func_0800baf8(*(s32 *)((u8 *)arg0 + 80), arg2);
            break;
        case 2:
            items = *(void ***)((u8 *)arg0 + 80);
            for (count = 3; count >= 0; count--) {
                item = *items++;
                if (item != 0) {
                    Func_0800ba30((s32)item, arg1);
                    Func_0800baf8((s32)item, arg2);
                }
            }
            break;
        }
    }
}
