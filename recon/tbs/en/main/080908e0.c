/* Draft, not exact (2026-09-24): 380 of 380 bytes, 8 differing halfwords
   (was 189 at 372). Once a palette blend is running, add the step table
   into the working colours (or, on the last step, copy the targets in and
   stop), pack the working colours into the back palette buffer, flip
   buffers and queue both halves for the next frame. The green and blue
   masks are u16 values of link symbols (halfword pool constants, which
   place the pool before the pack loop), and the queue writes use the IO
   write queue idiom of SYSTEM/IO_WRITE_QUEUE.C with function-level queue
   and IME pointers. Residual: in the pack loop preheader the reference sets
   the count before the source pointer, and it computes the first queued
   address ahead of the IME pointer load and keeps the saved IME word in r1
   (here r7). */
#include "TYPES.H"
#include "IO_WRITE_QUEUE.H"

s32 Func_080770c0(s32 flag);

extern volatile u16 Data_04000208;
extern u8 Value_000003e0;
extern u8 Value_0000001f;

struct Rgb16 {
    s16 r;
    s16 g;
    s16 b;
};

typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);

#define QUEUE_WRITE(value, address, control) {                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do {                                                                \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = (control);                                       \
        }                                                                   \
        *ime = saved;                                                       \
    }

void Func_080908e0(void)
{
    u8 *p = *(u8 **)0x03001ed0;
    u16 *add = (u16 *)(p + 0x1880);
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u8 *base;
    s32 i;

    if (Func_080770c0(0x152) != 0) {
        return;
    }
    if (*(s8 *)(p + 0x2a01) == 0) {
        return;
    }
    if (++*(s8 *)(p + 0x2a02) < *(s8 *)(p + 0x2a01)) {
        u16 *sum = (u16 *)(p + 0x380);
        for (i = 0; i < 0x540; i++) {
            *sum++ += *add++;
        }
    } else {
        CopyWordsFn copy = (CopyWordsFn)0x03001388;
        copy(p + 0x380, p + 0xe00, 0xa80);
        *(s8 *)(p + 0x2a01) = 0;
    }

    {
        s16 (*in)[3] = (s16 (*)[3])(p + 0x380);
        u16 *out = (u16 *)(p + (1 ^ p[0x2a00]) * 0x380 + 0x2300);
        u16 green = (u16)(s32)&Value_000003e0;
        u16 blue = (u16)(s32)&Value_0000001f;

        for (i = 0; i < 0x1c0; i++) {
            *out++ = (in[i][0] & 0x7c00) | ((in[i][1] >> 5) & green) | ((in[i][2] >> 10) & blue);
        }
    }

    p[0x2a00] ^= 1;
    base = p + p[0x2a00] * 0x380;
    q = &gIoWriteQueue;
    ime = &Data_04000208;
    {
        u32 bg = (u32)(base + 0x2300);
        QUEUE_WRITE(bg, 0x05000000, 0x84000070);
    }
    QUEUE_WRITE((u32)(base + 0x24c0), 0x05000200, 0x84000070);
}
