#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "callback_scheduler.h"
#include "btlwork.h"
#include "bg_scroll.h"

void BattleFx_BeginCanvasLayer(s32 bg_control);

void BattleFx_BeginTiledCanvas(s32 bg_control)
{
    s32 row;
    s32 col;
    s32 offset;

    BattleFx_BeginCanvasLayer(bg_control);
    REG_BG2CNT = bg_control | 0x4784;
    offset = 0;
    for (row = 0; row != 16; row++) {
        for (col = 0; col != 8; col++, offset += 2) {
            s32 tile;
            s16 pair;

            tile = row * 16 + col * 2;
            pair = ((tile + 1) << 8) | tile;
            *(volatile s16 *)(0x06003800 + offset) = pair;
        }
        for (col = 0; col != 8; col++, offset += 2) {
            *(volatile u16 *)(0x06003800 + offset) = 0;
        }
    }
}

void BattleFx_BeginTiledCanvasFilled(s32 bg_control)
{
    s32 row;
    s32 col;
    s32 offset;

    BattleFx_BeginCanvasLayer(bg_control);
    REG_BG2CNT = bg_control | 0x4784;
    offset = 0;
    for (row = 0; row != 16; row++) {
        for (col = 0; col != 8; col++, offset += 2) {
            s32 tile;
            s16 pair;

            tile = row * 16 + col * 2;
            pair = ((tile + 1) << 8) | tile;
            *(volatile s16 *)(0x06003800 + offset) = pair;
        }
        for (col = 0; col != 8; col++, offset += 2) {
            *(volatile u16 *)(0x06003800 + offset) = 0xf0f;
        }
    }
}

// The battle effect work, as far as the canvas layer reads it.
struct CanvasLayerWork
{
    u8 unknown_0000[0x77a0];
    s32 savedBg1X;
    s32 savedBg1Y;
};

extern u32 gCameraSceneParameters[];

void Audio_PlayCue(s32 cue);
void Func_08143488(void);
void BattlePresentation_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 value);
void BattlePresentation_SetPaletteLevelFar(s32 background, s32 level);

// Closes the effect canvas layer: BG1 back where it was, BG2 and its
// characters cleared, then the background palette faded back in.
void Func_08143bb8(void)
{
    struct CanvasLayerWork *work = (struct CanvasLayerWork *)Ram_HeapSlots->battle_fx_work[0];
    struct BtlWork *btl = Ram_HeapSlots->battle_work;
    s32 i;

    Audio_PlayCue(390);
    gBgScroll[1].x = work->savedBg1X;
    gBgScroll[1].y = work->savedBg1Y;
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    {
        /* FAKEMATCH: the native loads the control word from the pool before it starts the register address; the plain store starts the address first (differ 2), as does a pin without the barrier. Pinned to the r3 it takes, as in f18284c.c. */
        register u16 control asm("r3") = 0x787;

        /* FAKEMATCH: the barrier of the pin above: it holds r2 until the value is loaded. */
        __asm__("" : "+l"(control) : : "r2");
        REG_BG2CNT = control;
    }
    Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
    Scheduler_RemoveCallback(Func_08143488);
    gBgScroll[1].y = 32;
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
    /* FAKEMATCH: the native loads the zero from the pool between the first two instructions of the register address; after the push (whose IME restore is a one-pass block) the plain store loads it one instruction later (differ 2), as does a pinned zero with or without its own barrier. With the push written out and a plain restore, the load falls right but the restore sinks below the address. */
    __asm__ volatile("");
    REG_BLDCNT = 0;
    WaitFrames(1);

    BattlePresentation_ConfigurePaletteFadeFar(2, *(u16 *)btl->unknown_670, 7);
    WaitFrames(1);
    for (i = 0; i != 8; i++)
    {
        BattlePresentation_SetPaletteLevelFar(*(u16 *)btl->unknown_670, 21 - i * 3);
        WaitFrames(1);
    }

    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7541, 0x20000);
    WaitFrames(1);
}
