#include "types.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "ram_buffer.h"

#define WAVE_FX_SLOT 88

struct ObjectRuntime;
struct ObjectRuntime *Object_GetById(s32 id);
void BattleFx_ApplyFlickerValue(struct ObjectRuntime *object, s32 value);
void BattleFx_PrepareBufferInterpolation(void);
void Func_080dcadc(void);
void Func_080dcb44(void);

void Func_080dca50(void)
{
    u8 *work = Ram_HeapSlots->unknown_4c[3];

    Func_080dcadc();
    Scheduler_RemoveCallback((SchedulerCallback)Func_080dcb44);
    BattleFx_ApplyFlickerValue(Object_GetById(*(u16 *)(work + 0x290)), 1);
    BattleFx_PrepareBufferInterpolation();
    Runtime_ReleaseHeapBlock(WAVE_FX_SLOT);
}

#define BG_PLTT_COLOR(bank, color) (*(volatile u16 *)(0x05000000 + ((bank) * 16 + (color)) * 2))
#define GREY28 0x739c

void Func_080dcdc8(void);

// BG パレット 15 の色 1, 3〜12 を灰色にし、走査線スクロールを始める
void Func_080dca84(void)
{
    volatile u16 *p;

    Ram_HeapSlots->window_tiles[4] = 1;
    p = &BG_PLTT_COLOR(15, 1);
    *p = GREY28;
    p += 2;
    *p = GREY28;
    p += 8;
    *p = GREY28;
    p += 1;
    *p = GREY28;
    p -= 8;
    *p = GREY28;
    p += 1;
    *p = GREY28;
    p += 1;
    *p = GREY28;
    p += 1;
    *p = GREY28;
    p += 1;
    *p = GREY28;
    p += 1;
    *p = GREY28;
    p += 1;
    *p = GREY28;
    Scheduler_AddOrUpdateCallback((SchedulerCallback)Func_080dcdc8, 0xc6c);
}
