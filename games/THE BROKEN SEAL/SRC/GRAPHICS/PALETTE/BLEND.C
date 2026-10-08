#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
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

extern struct DisplayTransitionState *gDisplayTransition;

typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);

/* FAKEMATCH: needed to match: SYSTEM/IO_WRITE_QUEUE.C's queue idiom: a one-pass loop around the IME read, and the count stored through a u16 pointer before the entry. */
#define QUEUE_WRITE(value, address, control) {                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do { \
            /* FAKEMATCH: needed to match: removing this one-pass boundary changes measured instruction scheduling. */ \
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
    struct DisplayTransitionState *work = gDisplayTransition;
    volatile u32 fill = 0xf000f000;
    Dma_Set(&fill, BG_SCREEN_BLOCK(4), 0x85000140, REG_DMA3);
    if (color != -1) {
        u32 pattern = 0;
        s32 cnt;
        u32 *tile;
        for (cnt = 7; cnt >= 0; --cnt) pattern = (pattern << 4) | color;
        tile = work->solid_tile;
        for (cnt = 7; cnt >= 0; --cnt) *tile++ = pattern;
        Dma_Set(work->solid_tile, BG_CHAR_BLOCK(0), 0x84000008, REG_DMA3);
    }
}

void DisplayTransition_InitializeState(s32 value)
{
    struct DisplayTransitionState *state;
    volatile u32 zero;

    state = Runtime_AllocateBlock(0x1f, sizeof(*state));
    zero = 0;
    Dma_Set(&zero, state, 0x85000000 | (sizeof(*state) / 4), REG_DMA3);
    DisplayTransition_FillTilemapAndSolidTile(0);
    state->mode = value;
    state->value = 0;
    Scheduler_AddOrUpdateCallback(DisplayTransition_UpdateFrame, 0xc80);
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

void BattlePalette_UpdateBlend(void)
{
    /* FAKEMATCH: needed to match: IME scopes and halfword count stores preserve queue order; direct packed-bank selection swaps queue/IME registers. */
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
        QUEUE_WRITE(bg, (u32)BG_PLTT, 0x84000070);
    }
    QUEUE_WRITE((u32)(base + 0x24c0), (u32)OBJ_PLTT, 0x84000070);
}
