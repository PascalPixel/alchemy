typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/* struct at *0x03001E70 -- same object and field layout as
 * src/0801173c.c (fields "a"/"b" at 0x100/0x102) and src/08011984.c
 * (field at 0xFC). 0x03001E6C is the immediately preceding u32 global
 * (a separate pointer), read together with 0x03001E70 through one
 * struct so gcc emits the reference's ldmia/ldr pair instead of two
 * independent loads. */
struct S {
    u8 pad0[0xFC];
    u8 unk_FC;
    u8 pad1[3];
    s16 a;
    s16 b;
};

struct Ptrs {
    u8 *base;
    struct S *work;
};

struct DmaRegisters {
    const void *source;
    void *destination;
    u32 control;
};

s32 Func_080042c8(s32);
void Func_080030f8(u32);
void Func_08012388(s32, s32);

void Func_08011590(void) {
    struct Ptrs *ptrs;
    u8 *base;
    struct S *work;
    s32 flag;

    ptrs = (struct Ptrs *)0x03001E6C;
    base = ptrs->base;
    work = ptrs->work;
    work->unk_FC = 1;
    Func_080042c8(0x0801179D);

    {
        struct DmaRegisters *dma = (struct DmaRegisters *)0x040000D4;
        dma->source = (const void *)0x06004000;
        dma->destination = (void *)0x0201C000;
        dma->control = 0x84000800;
    }
    Func_080030f8(1U);

    flag = (*(s32 *)0x03001E40) & 1;
    Func_08012388((s32)(base + flag * 5120 + 3200), 0x02010000);

    work->a = 200;
    work->b = 255;

    *(s32 *)0x03001CFC = 0x08011569;
}

/* Second, unrelated leaf function occupying the remaining 40 bytes of the
 * same call-graph-unreached gap (0x0801161C..0x08011644). It is a distinct
 * compiled routine (no push/pop, no lr use) whose address (as a thumb
 * pointer, 0x0801161D) is itself installed as a handler by
 * src/0801173c.c's `*(s32 *)0x03001CFC = 0x0801161D;`. Included here for
 * completeness of the gap region; decomp_diagnose only scores the
 * Func_08011590 symbol above (see summary). */
void Func_0801161c(void) {
    struct DmaRegisters *dma;

    *(u16 *)0x0400000A = 0x0501;
    dma = (struct DmaRegisters *)0x040000D4;
    dma->source = (const void *)0x02038000;
    dma->destination = (void *)0x06008000;
    dma->control = 0x84002000;
}
