#include "RUNTIME_MEM.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "DISPTRAN.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"

extern struct DisplayTransitionState *Data_03001ecc;

s32 GameFlag_IsSet(s32 flag);
typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);

/* FAKEMATCH: SYSTEM/IO_WRITE_QUEUE.C's queue idiom, for that file's reasons:
   the loop that runs once around the IME read, and the count stored through
   a u16 pointer before the entry is written. */
#define QUEUE_WRITE(value, address, control) {                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do { \
            /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
            ime = &REG_IME;                                                 \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = q->entries[count];                            \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = (control);                                       \
        }                                                                   \
        *ime = saved;                                                       \
    }

void DisplayTransition_FillTilemapAndSolidTile(s32 color)
{
    struct DisplayTransitionState *work = Data_03001ecc;
    volatile u32 fill = 0xf000f000;
    Dma_Set(&fill, (void *)0x06002000, 0x85000140, (volatile u32 *)0x040000d4);
    if (color != -1) {
        u32 pattern = 0;
        s32 cnt;
        u32 *tile;
        for (cnt = 7; cnt >= 0; --cnt) pattern = (pattern << 4) | color;
        tile = work->solid_tile;
        for (cnt = 7; cnt >= 0; --cnt) *tile++ = pattern;
        Dma_Set(work->solid_tile, (void *)0x06000000, 0x84000008, (volatile u32 *)0x040000d4);
    }
}

void DisplayTransition_InitializeState(s32 value)
{
    struct DisplayTransitionState *state;
    volatile u32 zero;

    state = Runtime_AllocateBlock(0x1f, sizeof(*state));
    zero = 0;
    Dma_Set(&zero, state, 0x85000000 | (sizeof(*state) / 4), (volatile u32 *)0x040000d4);
    DisplayTransition_FillTilemapAndSolidTile(0);
    state->mode = value;
    state->value = 0;
    Scheduler_AddOrUpdateCallback((s32)(DisplayTransition_UpdateFrame), 0xc80);
    WaitFrames(0x78);
}

void BattleFx_InterpolateBuffers(s16 *source, s16 *target, s16 *step, s32 frames)
{
    s32 index;
    s32 first;
    s32 second;
    s32 (*divide)(s32, s32);

    if (frames > 0) {
        divide = Iwram_SignedDivide;
        index = 0x53F;
        do {
            first = *source;
            second = *target;
            *step = divide(second - first, frames);
            index--;
            source++;
            target++;
            step++;
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
    /* FAKEMATCH: retain the existing one-pass IME scopes and halfword
       queue-count stores used by the other palette queue writers.
       Directly selecting the packed bank swaps the queue/IME registers
       in all six editions; retain the existing byte-address boundary. */
    struct BattleEffectBuffers *work = Data_03001ed0;
    u16 *add = work->delta;
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u8 *base;
    s32 i;

    if (GameFlag_IsSet(0x152) != 0) {
        return;
    }
    if (work->duration == 0) {
        return;
    }
    if (++work->step < work->duration) {
        u16 *sum = work->current;
        for (i = 0; i < 0x540; i++) {
            *sum++ += *add++;
        }
    } else {
        CopyWordsFn copy = Iwram_CopyWords;

        copy(work->current, work->target, sizeof(work->current));
        work->duration = 0;
    }

    {
        u16 *packed = work->packed[1 ^ work->page];
        s32 blue = 0x7c00;
        u16 *current;

        i = 0x1c0;
        current = work->current;
        for (; i != 0; i--) {
            *packed++ = (current[0] & blue) | (((s16)current[1] >> 5) & 0x3e0) | (((s16)current[2] >> 10) & 0x1f);
            current += 3;
        }
    }

    work->page ^= 1;
    base = (u8 *)work + work->page * sizeof(work->packed[0]);
    q = &gIoWriteQueue;
    {
        u32 bg = (u32)(base + 0x2300);
        QUEUE_WRITE(bg, 0x05000000, 0x84000070);
    }
    QUEUE_WRITE((u32)(base + 0x24c0), 0x05000200, 0x84000070);
}
