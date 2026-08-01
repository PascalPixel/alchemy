/*
 * Correctness fix, veneer audit (2026-08-01).
 *
 * `Func_080072f0` is not a function.  0x080072e4 begins the GCC
 * `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop` entries,
 * r0..lr, ending at 0x08007320 -- so 0x080072f0 is `__call_via_r3` and
 * `bl 0x80072f0` calls whatever r3 holds.
 *
 * At every site in this file the ROM loads r3 from the literal pool with
 * the constant 0x03001388, so the callee is the relocated IWRAM word copy
 * at that address.  Its signature is not guessed: the EXACT source
 * src/080d40ec.c declares it as
 * `void *(*)(void *destination, const void *source, s32 size)` and
 * src/080e0524.c casts the same address to the same shape.
 *
 * Note what the previous draft had already half-seen: it passed
 * 0x03001388 as a fourth ARGUMENT.  That value was never an argument --
 * it is the callee, and the register load that produced it is the call
 * target, not a parameter.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *(*WordCopy)(void *destination, const void *source, s32 size);

struct PaletteDisplay_080908e0 {
    u8 unknown_0000[0x380];
    u16 accumulated[1344];
    u16 target[1344];
    u16 increments[1344];
    u16 palettes[2][448];
    u8 active_palette;
    s8 fade_frames;
    u8 fade_progress;
};

struct QueuedTransfer_080908e0 {
    const void *source;
    void *destination;
    u32 control;
};

struct TransferQueue_080908e0 {
    u16 count;
    u16 padding;
    struct QueuedTransfer_080908e0 entries[32];
};

s32 Func_080770c0(s32);

/*
 * Advance the display palette fade, render its 448 RGB555 colors into the
 * inactive buffer, swap buffers, and queue the two palette-RAM transfers.
 */
void Func_080908e0(void) {
    struct PaletteDisplay_080908e0 *display =
        *(struct PaletteDisplay_080908e0 **)0x03001ED0;
    volatile struct TransferQueue_080908e0 *queue =
        (volatile struct TransferQueue_080908e0 *)0x02002090;
    volatile u16 *interrupt_master =
        (volatile u16 *)0x04000208;
    u16 *rendered;
    s32 index;

    if (Func_080770c0(0x152) != 0) {
        return;
    }
    if (display->fade_frames == 0) {
        return;
    }

    display->fade_progress++;
    if ((s8)display->fade_progress < display->fade_frames) {
        for (index = 0; index < 1344; index++) {
            display->accumulated[index] += display->increments[index];
        }
    } else {
        ((WordCopy)0x03001388)(
            display->accumulated,
            display->target,
            0xA80);
        display->fade_frames = 0;
    }

    rendered = display->palettes[display->active_palette ^ 1];
    for (index = 0; index < 448; index++) {
        s32 component = index * 3;
        u16 first = display->accumulated[component];
        s16 second = (s16)display->accumulated[component + 1];
        s16 third = (s16)display->accumulated[component + 2];

        rendered[index] =
            (first & 0x7C00) |
            ((second >> 5) & 0x03E0) |
            ((third >> 10) & 0x001F);
    }

    display->active_palette ^= 1;
    rendered = display->palettes[display->active_palette];

    {
        u16 saved_interrupt_master = *interrupt_master;
        u16 queue_index;

        *interrupt_master = (u16)(u32)interrupt_master;
        queue_index = queue->count;
        if (queue_index <= 31) {
            volatile struct QueuedTransfer_080908e0 *entry =
                &queue->entries[queue_index];

            entry->source = rendered;
            queue->count = queue_index + 1;
            entry->destination = (void *)0x05000000;
            entry->control = 0x84000070;
        }
        *interrupt_master = saved_interrupt_master;
    }

    {
        u16 saved_interrupt_master = *interrupt_master;
        u16 queue_index;

        *interrupt_master = (u16)(u32)interrupt_master;
        queue_index = queue->count;
        if (queue_index <= 31) {
            volatile struct QueuedTransfer_080908e0 *entry =
                &queue->entries[queue_index];

            queue->count = queue_index + 1;
            entry->source = rendered + 224;
            entry->destination = (void *)0x05000200;
            entry->control = 0x84000070;
        }
        *interrupt_master = saved_interrupt_master;
    }
}
