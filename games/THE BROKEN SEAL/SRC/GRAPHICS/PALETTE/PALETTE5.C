#include "DMA.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"

extern u8 Data_03001ecc[];

struct DisplayTransitionState {
    u8 data[0x528];
    s16 value;
    s16 timer;
};

void DisplayTransition_FillTilemapAndSolidTile(s32);
void Scheduler_AddOrUpdateCallback(void (*)(void), s32);
void DisplayTransition_UpdateFrame(void);

s32 GameFlag_IsSet(s32 flag);
extern u8 *Data_03001ed0;
typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);

/* FAKEMATCH: SYSTEM/IO_WRITE_QUEUE.C's queue idiom, for that file's reasons:
   the loop that runs once around the IME read, and the count stored through
   a u16 pointer before the entry is written. */
#define QUEUE_WRITE(value, address, control) {                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do {                                                                \
            ime = &REG_IME;                                                 \
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

void DisplayTransition_FillTilemapAndSolidTile(s32 color)
{
    u8 *work = *(u8 **)Data_03001ecc;
    volatile u32 fill = 0xf000f000;
    Dma_Set(&fill, (void *)0x06002000, 0x85000140, (volatile u32 *)0x040000d4);
    if (color != -1) {
        u32 pattern = 0;
        s32 cnt;
        u32 *tile;
        for (cnt = 7; cnt >= 0; --cnt) pattern = (pattern << 4) | color;
        tile = (u32 *)(work + 1288);
        for (cnt = 7; cnt >= 0; --cnt) *tile++ = pattern;
        Dma_Set(work + 1288, (void *)0x06000000, 0x84000008, (volatile u32 *)0x040000d4);
    }
}

void DisplayTransition_InitializeState(s32 value)
{
    struct DisplayTransitionState *state;
    volatile u32 zero;

    state = Runtime_AllocateBlock(0x1f, 0x540);
    zero = 0;
    Dma_Set(&zero, state, 0x85000150, (volatile u32 *)0x040000d4);
    DisplayTransition_FillTilemapAndSolidTile(0);
    state->value = value;
    state->timer = 0;
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
    WaitFrames(0x78);
}

void BattleFx_InterpolateBuffers(s16 *arg0, s16 *arg1, s16 *arg2, s32 arg3)
{
    s32 index;
    s32 first;
    s32 second;
    s32 (*divide)(s32, s32);

    if (arg3 > 0) {
        divide = Iwram_SignedDivide;
        index = 0x53F;
        do {
            first = *arg0;
            second = *arg1;
            *arg2 = divide(second - first, arg3);
            index--;
            arg0++;
            arg1++;
            arg2++;
        } while (index >= 0);
    }
}

/* Battle palette blend, run each frame by the scheduler once
   BattleEffect_InitializeBuffers has set it up: add the step table into the
   working colours (on the last step, copy the targets in and stop), pack them
   into the back palette buffer, flip buffers and queue both halves.
   The packing loop follows GRAPHICS/PALETTE/TITLE_UPDATE_FADE.C. */
void BattlePalette_UpdateBlend(void)
{
    u8 *p = Data_03001ed0;
    u16 *add = (u16 *)(p + 0x1880);
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u8 *base;
    s32 i;

    if (GameFlag_IsSet(0x152) != 0) {
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
        CopyWordsFn copy = Iwram_CopyWords;

        copy(p + 0x380, p + 0xe00, 0xa80);
        *(s8 *)(p + 0x2a01) = 0;
    }

    {
        u16 *packed = (u16 *)(p + (1 ^ p[0x2a00]) * 0x380 + 0x2300);
        s32 blue = 0x7c00;
        u16 *current;

        i = 0x1c0;
        current = (u16 *)(p + 0x380);
        for (; i != 0; i--) {
            *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) | (((s16)current[2] >> 10) & 0x1f);
            current += 3;
        }
    }

    p[0x2a00] ^= 1;
    base = p + p[0x2a00] * 0x380;
    q = &gIoWriteQueue;
    {
        u32 bg = (u32)(base + 0x2300);
        QUEUE_WRITE(bg, 0x05000000, 0x84000070);
    }
    QUEUE_WRITE((u32)(base + 0x24c0), 0x05000200, 0x84000070);
}
