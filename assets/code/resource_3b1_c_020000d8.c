typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

void Func_020000d8(u8 *o) {
    u8 *p = o + 98;
    s32 n = *p + 1;

    *p = n;
    if ((u8)n > 80) {
        *(u16 *)(o + 102) += 1;
    }
}
