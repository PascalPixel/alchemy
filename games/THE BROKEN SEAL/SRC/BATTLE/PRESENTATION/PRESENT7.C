/* Battle presentation: set up the effect display. Windows 0 and 1 cover the
   screen, blending starts from a clean slate, and the display control write
   (mode 1, BG0/BG1/BG2 and objects) is queued for the next frame; then wait
   one frame for it to land.

   FAKEMATCH: the queued write is QueueIoWriteDelay2 (SYSTEM/IO_WRITE_QUEUE.C)
   written out inline with that function's two odd constructs: the loop that
   runs once around the IME read and the count stored through an explicit u16
   pointer. */
#include "TYPES.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "DMA.H"

void WaitFrames(s32 frames);

struct Position {
    u8 unknown[4];
    u16 x;
    u16 y;
};

extern u32 gBattleFxWork;
extern struct Position gBgScroll;

extern u8 gMapCellBuffer[];

void BattlePres_ConfigureEffectDisplay(void)
{
    volatile u16 *ime;
    struct IoWriteQueue *q;
    u32 saved;
    s32 count;

    *(volatile u16 *)0x04000050 = 0;
    *(volatile u16 *)0x04000052 = 0x100e;
    *(volatile u16 *)0x04000040 = 0x00f0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000042 = 0x00f0;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;

    q = &gIoWriteQueue;
    /* FAKEMATCH: removing this one-pass block changes instruction scheduling; see its retained draft. */
    do {
        ime = &REG_IME;
        saved = *ime;
    } while (0);
    *ime = (u16)ime;
    count = q->count;
    if (count <= 31) {
        u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
        *(u16 *)&q->count = count + 1;
        *destination++ = 0x7741;
        *destination++ = 0x04000000;
        *destination = 0x20000;
    }
    *ime = saved;

    WaitFrames(1);
}

void BattleFx_AdvanceScrollOnInterval(void)
{
    u8 *base = (u8 *)gBattleFxWork;
    u32 *counter = (u32 *)(base + 0x7790);

    (*counter)++;
    if (*counter == *(u32 *)(base + 0x7794)) {
        gBgScroll.x += *(s32 *)(base + 0x7798);
        gBgScroll.y += *(s32 *)(base + 0x779C);
        *counter = 0;
    }
}

void Camera_AdvanceBg2Reference(void)
{
    s32 cnt;
    void *state;

    state = *(void **)((u32)&gBattleFxWork);
    cnt = FIELD_AT_OFFSET(state, s32 *, 0x7790) + 1;
    FIELD_AT_OFFSET(state, s32 *, 0x7790) = cnt;
    if (cnt == FIELD_AT_OFFSET(state, s32 *, 0x7794)) {
        FIELD_AT_OFFSET((void *)0x04000028, s32 *, 0) = (s32)FIELD_AT_OFFSET(state, s32 *, 0x77D0);
        FIELD_AT_OFFSET((void *)0x04000028, s32 *, 4) = (s32)FIELD_AT_OFFSET(state, s32 *, 0x77D4);
        FIELD_AT_OFFSET(state, s32 *, 0x77D0) = (s32)(FIELD_AT_OFFSET(state, s32 *, 0x77D0) + FIELD_AT_OFFSET(state, s32 *, 0x7798));
        FIELD_AT_OFFSET(state, s32 *, 0x77D4) = (s32)(FIELD_AT_OFFSET(state, s32 *, 0x77D4) + FIELD_AT_OFFSET(state, s32 *, 0x779C));
        FIELD_AT_OFFSET(state, s32 *, 0x7790) = 0;
    }
}

/* H-blank callback: feed the per-line WIN0H table at 0x02010000 to WIN0H. */
void BattleFx_ArmWin0HBlankDma(void)
{
    volatile u16 *channel = (volatile u16 *)0x040000b0;
    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    Dma_Set((void *)gMapCellBuffer, (void *)0x04000040, 0xa2600001, (volatile u32 *)channel);
}
