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

struct Fields_080b7e04 {
    u8 filler[0x10];
    s32 field10;
};

void Func_080b7e04(void *arg0) {
    s32 i;
    s32 zero;
    struct Fields_080b7e04 **items;

    if (arg0 != NULL) {
        zero = 0;
        items = (struct Fields_080b7e04 **)((u8 *)arg0 + 0x28);
        for (i = 3; i >= 0; i--) {
            struct Fields_080b7e04 *item = *items++;
            if (item != NULL) {
                item->field10 = zero;
            }
        }
    }
}
