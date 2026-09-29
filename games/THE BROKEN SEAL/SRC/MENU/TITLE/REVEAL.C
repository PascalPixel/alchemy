/* The title screen's reveal: the background fades in and a row of eighteen
 * sprites appears one by one. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
#include "VRAM_BLOCK.H"
#include "TITLE.H"

void Runtime_PushSlotEntry(void *entry, s32 slot);

static __inline__ void RestoreInterrupts(u32 saved)
{
    /* FAKEMATCH: keep the final hardware address local to restoration. */
    do { REG_IME = saved; } while (0);
}

/* FAKEMATCH: the one-pass IME read keeps the saved copy before masking;
 * the count cast preserves the queue's original publication order. */
#define QUEUE_WRITE(address, value)                                         \
    do {                                                                    \
        volatile u16 *ime;                                                  \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
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
            *destination = 0x20000;                                         \
        }                                                                   \
        RestoreInterrupts(saved);                                           \
    } while (0)

/* FAKEMATCH: the one-halfword record keeps the short-range pool zero, as
 * PALETTE_START.C does. */
static __inline__ void ResetCounter(s16 *destination)
{
    struct { u16 value; } zero;

    zero.value = 0;
    *destination = zero.value;
}

/* Rebuild the row of eighteen title sprites; one more shows every two
 * frames, and the newest two blink with the frame counter. */
void Title_RevealSpriteRow(void)
{
    u32 *w;
    struct TitleSprite *p;
    s32 tile;
    s32 i;
    s32 n;
    s32 y;
    s32 x;

    p = gTitleSprites;
    w = gTitleSprites[0].attr;
    tile = gVramBlockCache[gTitleVramBlock].offset >> 5;
    i = 0;
    y = 0x88;
loop:
    {
        x = 232 - (18 - i) * 8;
        *w++ = 0;
        *w++ = (x << 16) | y | 0x8400;
        *w++ = 0xf000 | tile;
        n = gTitleRevealFrame / 2 - i;
        if (n < 0)
            n = 0;
        if (n <= 2 && (gFrameCount & 1))
            n = 0;
        if (n != 0)
            Runtime_PushSlotEntry(p++, 255);
        tile += 2;
    }
    if (++i <= 17)
        goto loop;
    gTitleRevealFrame++;
}

/* Fade the title in: the background, then the sprite row under a blend
 * that clears over sixteen steps. */
void Title_RevealScreen(s32 unused)
{
    s32 i;
    struct IoWriteQueue *q;

    Title_LoadBackground();
    Engine_EventWait(30);
    ResetCounter(&gTitleRevealFrame);
    Title_LoadSprites(0);
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
    gEventWork->start_transition = 0;
    gEventWork->transition_frames = 1;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    gEventWork->transition_frames = 60;
}
