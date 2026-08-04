#include "types.h"

struct Inner_0808ba38 {
    u8 pad00[9];
    u8 flags09;
    u8 pad0a[0x1a];
    u8 value24;
    u8 pad25;
    u8 value26;
};

struct Object_0808ba38 {
    u8 pad00[0x50];
    struct Inner_0808ba38 *inner;
    u8 active;
};

struct Dma_0808ba38 {
    const void *source;
    void *destination;
    u32 control;
};

extern u8 *Data_03001ebc;
struct Object_0808ba38 *Func_0808ba1c(u32 index);

void Func_0808ba38(void) {
    u8 *base;
    u8 *values24;
    u8 *values26;
    u8 *flags;
    u8 *indices;
    s32 count;
    s32 limit;
    s32 index;

    base = (u8 *)0x02001124;
    values24 = base + 0xe00;
    values26 = base + 0xe20;
    flags = base + 0xe40;
    count = 0;
    limit = 0x42;

    if (*(s16 *)(Data_03001ebc + 0x19e) == 3)
        limit = 8;

    indices = base - 0x20;
    for (index = 0; index < limit; index++) {
        struct Object_0808ba38 *object = Func_0808ba1c(index);
        u32 value24;
        u32 value26;
        u32 flag;

        if (object == 0)
            continue;

        *indices++ = index;
        *(volatile struct Dma_0808ba38 *)0x040000d4 =
            (struct Dma_0808ba38){ object, base, 0x8400001c };

        if (object->active == 1) {
            struct Inner_0808ba38 *inner = object->inner;
            value24 = inner->value24;
            value26 = inner->value26;
            flag = ((u32)inner->flags09 << 28) >> 30;
        } else {
            value24 = 0;
            value26 = 0;
            flag = 0;
        }

        *values24++ = value24;
        *values26++ = value26;
        *flags++ = flag;
        count++;
        base += 0x70;
        if (count > 31)
            break;
    }

    if (count <= 31) {
        s32 left = 32 - count;
        do {
            *indices++ = 0xff;
            left--;
        } while (left != 0);
    }
}
