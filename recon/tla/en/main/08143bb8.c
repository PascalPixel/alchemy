/* Near miss, differ 2 of 134: The Broken Seal's BattleFx_EndCanvasLayer with ⚓️'s
   cue 390 and Func_08143488 as the fade callback; the clear goes through an
   IWRAM forwarder and REG_BG2CNT's value is pinned to r3 behind a barrier that
   holds r2, as in f18284c.c. Left: before REG_BLDCNT = 0 the native loads the
   zero between the two halves of the register address (movs r2, #128; ldr r3,
   =0; lsls r2, r2, #19); here it loads after the address is complete. A pinned
   zero through a pointer local, with or without an operand barrier, changes
   nothing. */
#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "callback_scheduler.h"
#include "btlwork.h"
#include "bg_scroll.h"

extern u32 gCameraSceneParameters[];

void Audio_PlayCue(s32 cue);
void Func_08143488(void);
void BattlePresentation_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 value);
void BattlePresentation_SetPaletteLevelFar(s32 background, s32 level);

static __inline__ s32 ClearWords_(void *destination, s32 size)
{
    return Iwram_ClearWords(destination, size);
}

void Func_08143bb8(void)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];
    struct BtlWork *btl = Ram_HeapSlots->battle_work;
    s32 i;

    Audio_PlayCue(390);
    gBgScroll[1].x = *(s32 *)(work + 0x77a0);
    gBgScroll[1].y = *(s32 *)(work + 0x77a4);
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    {
        register u16 control asm("r3") = 0x787;

        __asm__("" : "+l"(control) : : "r2");
        REG_BG2CNT = control;
    }
    ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
    Scheduler_RemoveCallback(Func_08143488);
    gBgScroll[1].y = 32;
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
    {
        volatile u16 *reg = &REG_BLDCNT;
        register u16 control asm("r3") = 0;

        __asm__("" : "+l"(control));
        *reg = control;
    }
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
