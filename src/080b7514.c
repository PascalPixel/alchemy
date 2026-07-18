typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)

u8 *Func_08077008(s32);

s32 Func_080b7514(void) {
    s32 i = 0;
    s32 id;
    goto test;
again:
    i++;
test:
    if (i > 5) {
        goto done;
    }
    id = i + 0x80;
    if (Func_08077008(id)[0x12A] != 0) {
        goto again;
    }
done:
    if (i == 6) {
        return -1;
    }
    return id;
}
