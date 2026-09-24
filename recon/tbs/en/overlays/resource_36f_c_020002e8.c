/* NONMATCHING: 364 bytes, candidate 364, 23 differing halfwords (2026-09-24).
 * Single-overlay unit binding Engine_* at their import veneers. Remaining:
 * global allocation order only: the reference gives the IME pointer r5 and
 * the fade counter r6, here the counter outranks it (9 refs over 56 insns
 * against 33 over 3712) and takes r5; the first counter reset also swaps its
 * address and pool-zero registers. The count is block-local in the queue
 * macro and the IME pointer and queue are function-level, which fixed the
 * count and saved registers. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;
extern u8 Value_00000000;
extern s16 Data_0200868c;

void Local_02000454(void);
void Title_Func020001c0(s32 mode);
void Title_RevealSpriteRow(void);

/* Queue a register write with interrupts masked. */
#define QUEUE_WRITE(address, value)                                         \
    do {                                                                    \
        s32 count;                                                          \
        q = &gIoWriteQueue;                                                 \
        do {                                                                \
            ime = &Data_04000208;                                           \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = 0x20000;                                         \
        }                                                                   \
        *ime = saved;                                                       \
    } while (0)

void Local_020002e8(void)
{
    s32 i;
    s32 zero;
    u8 *event;
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u32 saved;

    Local_02000454();
    Engine_EventWait(30);
    zero = (u16)(u32)&Value_00000000;
    Data_0200868c = zero;
    Title_Func020001c0(0);
    Engine_TaskAddCallback(Title_RevealSpriteRow, 0xc80);
    QUEUE_WRITE(0x4000000, 0x1540);
    QUEUE_WRITE(0x4000050, 0x2fce);
    QUEUE_WRITE(0x4000054, 16);
    QUEUE_WRITE(0x4000052, 0x1010);
    Engine_EventWait(120);
    for (i = 0; i <= 16; i++) {
        QUEUE_WRITE(0x4000054, 16 - i);
        Engine_TaskWait(3);
    }
    event = *(u8 **)0x03001ebc;
    *(s32 *)(event + 0x1c0) = 0;
    *(s32 *)(event + 0x1c8) = 1;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    *(s32 *)(*(u8 **)0x03001ebc + 0x1c8) = 60;
}
