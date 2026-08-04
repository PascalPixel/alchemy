typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

enum { DEFERRED_MMIO_CAPACITY_02003640 = 32 };

struct DeferredMmioEntry_02003640 {
    u32 value;
    volatile u32 *address;
    u32 mask;
};

struct DeferredMmioQueue_02003640 {
    u16 count;
    u16 padding02;
    struct DeferredMmioEntry_02003640 entries[DEFERRED_MMIO_CAPACITY_02003640];
};

/*
 * resource_3bc actor/blend setup owner at 0x02003640, 476 bytes including
 * its eight-word pool.  Deferred MMIO writes use the engine queue at
 * 0x02002090; the even-address IME store is preserved exactly as ordinary C.
 */

extern void Func_080000c0(s32 frames);
extern void Func_08009080(void *record, s32 mode);
extern void Func_080091e0(void *record, s32 mode);
extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a0f0(s32 actor, s32 x, s32 z);
extern void Func_0808a110(s32 actor, s32 mode);
extern void Func_0808a148(s32 actor, s32 value, s32 mode);
extern void Func_0808a1e0(s32 actor, s32 mode);
extern void Func_080f9010(s32 cue);

static void enqueue_mmio_02003640(u32 value, volatile u32 *address)
{
    struct DeferredMmioQueue_02003640 *queue =
        (struct DeferredMmioQueue_02003640 *)0x02002090;
    volatile u16 *interrupt_master = (volatile u16 *)0x04000208;
    u16 saved = *interrupt_master;

    *interrupt_master = (u16)(u32)interrupt_master;
    if (queue->count <= 31) {
        struct DeferredMmioEntry_02003640 *entry =
            &queue->entries[queue->count++];
        entry->value = value;
        entry->address = address;
        entry->mask = 0x20000;
    }
    *interrupt_master = saved;
}

void Func_02003640(s32 actor, s32 map_x, s32 map_z)
{
    u8 *workspace = *(u8 **)0x03001e68;
    u8 *record = Func_0808a080(actor);
    u8 *render;
    s32 phase;

    workspace[6] = 1;
    workspace[7] = 4;
    *(s32 *)0x0200dc44 = *(s32 *)(record + 8);
    *(s32 *)0x0200db9c = *(s32 *)(record + 16);
    *(u32 *)0x0200dbc8 = *(u16 *)(record + 6);
    render = *(u8 **)(record + 0x50);

    Func_0808a1e0(actor, 2);
    record[0x23] |= 1;
    *(u16 *)(record + 6) = 0x4000;
    Func_080091e0(record, 3);
    Func_08009080(record, 0);
    Func_08009080(record, 1);
    Func_0808a0f0(actor, map_x << 16, map_z << 16);
    Func_0808a148(0, 0x4000, 0);

    enqueue_mmio_02003640(0xf00, (volatile u32 *)0x04000050);
    render[5] = (render[5] & ~0x0c) | 4;
    render[0x11] = (render[0x11] & ~0x0c) | 4;
    Func_080f9010(0xfc);

    for (phase = 0; phase <= 15; phase += 2) {
        *(s32 *)(record + 0x18) = (phase << 12) + 0x1000;
        *(s32 *)(record + 0x1c) = 0x1f000 - (phase << 12);
        enqueue_mmio_02003640(
            ((15 - phase) << 8) | (phase + 1),
            (volatile u32 *)0x04000052);
        Func_080000c0(1);
    }

    enqueue_mmio_02003640(0x10, (volatile u32 *)0x04000052);
    *(s32 *)(record + 0x18) = 0x11000;
    *(s32 *)(record + 0x1c) = 0xf000;
    Func_0808a010(1);
    *(s32 *)(record + 0x18) = 0x10000;
    *(s32 *)(record + 0x1c) = 0x10000;
    Func_0808a010(13);

    render[5] &= ~0x0c;
    render[0x11] &= ~0x0c;
    Func_0808a110(actor, 3);
    Func_0808a010(20);
}
