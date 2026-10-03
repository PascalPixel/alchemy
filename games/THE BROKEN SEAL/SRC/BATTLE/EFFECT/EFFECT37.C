#include "WINDOW.H"
#include "GAME_STATE.H"
#include "OBJECT_RUNTIME.H"
#include "DMA.H"
#include "OBJDISP.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "IWRAM_CALL.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

extern u8 *gBattleBgFxWork;

s32 BattleFx_HueChannelRamp(s32, s32, s32);
void BattleFx_ComputeHueChannels(s32 value, s32 *maximum, s32 *center, s32 *minimum);
s32 Fixed_Remainder(s32 value, s32 divisor);
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
extern const u8 BattleFx_UntargetedObjectScript[];


extern u8 Data_03001e8c[];
void BattleFx_ArmBg0HBlankDma(void);
extern void BattleFx_ArmBg0HBlankDma(void);
extern s32 PaletteGlow_UpdateFar(s32, s32);

void BattleFx_ArmBg0HBlankDma(void)
{
    u8 *work = gBattleBgFxWork;
    if (!work[660]) {
        u32 offset = work[650] * 324;
        volatile u16 *channel = (volatile u16 *)0x040000b0;
        channel[5] &= 0xc5ff;
        channel[5] &= 0x7fff;
        (void)channel[5];
        Dma_Set(work + offset, (void *)0x04000010, 0xa2600001, (volatile u32 *)channel);
    }
}

void BattleFx_AdvanceHueCycle(void)
{
    u8 *base = gBattleBgFxWork;
    s32 out1 = 0;
    s32 out2 = 0;
    s32 out3 = 0;
    s32 offset;
    u8 *p0;
    u8 *p1;
    u8 *p2;

    BattleFx_ComputeHueChannels((s32)(*(u16 *)(base + 0x28E)) << 16, &out1, &out2, &out3);

    offset = 0x28B;
    p0 = base + offset;
    *p0 = (u8)((out1 >> 18) + 4);
    offset += 1;
    p1 = base + offset;
    *p1 = (u8)((out2 >> 18) + 4);
    offset += 1;
    p2 = base + offset;
    *p2 = (u8)((out3 >> 18) + 4);

    *(u16 *)(base + 0x28E) += 4;

    *p0 &= 0x1F;
    *p1 &= 0x1F;
    *p2 &= 0x1F;

    if (*(u16 *)(base + 0x28E) >= 360) {
        *(u16 *)(base + 0x28E) = 0;
    }
}

void BattleFx_ComputeHueChannels(s32 value, s32 *maximum, s32 *center, s32 *minimum)
{
    *maximum = BattleFx_HueChannelRamp(value + 0x780000, 0, 0x1F0000);
    *center = BattleFx_HueChannelRamp(value, 0, 0x1F0000);
    *minimum = BattleFx_HueChannelRamp(value + 0xFF880000, 0, 0x1F0000);
}

/* One colour channel of a hue wheel: angle (Q16 degrees, taken modulo 360)
   ramps from 0 up to high over 0-60, holds high to 180, ramps back down over
   180-240 and is low beyond. */
s32 BattleFx_HueChannelRamp(s32 angle, s32 low, s32 high)
{
    s32 product;

    angle = Fixed_Remainder(angle, 360 << 16);
    if (angle < 60 << 16) {
        product = Iwram_MulQ16(high, angle);
    } else {
        if (angle >= 60 << 16 && angle < 180 << 16)
            return high;
        /* FAKEMATCH: the goto places the low return after the divide. */
        if (!(angle >= 180 << 16 && angle < 240 << 16))
            goto out;
        product = Iwram_MulQ16(high, (240 << 16) - angle);
    }
    {
        s32 (*divide)(s32, s32) = Iwram_RatioMulQ14;

        return divide(60 << 16, product);
    }
out:
    return low;
}

/* Fixed-point remainder: value less the whole multiples of |divisor|, using
   the IWRAM ratio and the IWRAM Q16 multiply. */
s32 Fixed_Remainder(s32 value, s32 divisor)
{
    s32 quotient;

    if (divisor == 0)
        return 0;
    if (divisor & 0xf0000000)
        divisor = -divisor;
    quotient = Iwram_RatioMulQ14(divisor, value);
    return value - Iwram_MulQ16(quotient & 0xffff0000, divisor);
}

void BattleFx_SetCallbackWhenTargetUnset(struct ObjectRuntime *target)
{
    s32 ty;
    s32 tx;

    tx = target->target_x;
    if (tx == 0x80000000) {
        ty = target->target_y;
        if ((ty == tx) && (target->target_z == ty)) {
            ObjectDispatch_InitializeFar((struct DispatchObject *)target, (u32)BattleFx_UntargetedObjectScript);
        }
    }
}

void Ui_FillBank15PaletteGrey(void)
{
    volatile s16 *p;

    ((struct UiRenderWork *)gWindowWork[0])->mode = 1;
    p = (s16 *)0x050001E2;
    *p = 0x739C;
    p += 2;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    p += 1;
    *p = 0x739C;
    Scheduler_AddOrUpdateCallback((s32)BattleFx_ArmBg0HBlankDma, 0x480);
}

void Ui_SetBank15PaletteAndClearRenderMode(void)
{
    void *work;

    work = gWindowWork[0];
    Scheduler_RemoveCallback((u32)((s32)BattleFx_ArmBg0HBlankDma));
    *(volatile s16 *)0x050001E2 = 0x7FFF;
    *(s16 *)0x050001E6 = 0;
    *(volatile s16 *)0x050001F6 = 0x294A;
    *(volatile s16 *)0x050001F8 = 0x5294;
    PaletteGlow_UpdateFar(gGameState.palette_glow[0], gGameState.palette_glow[1]);
    ((struct UiRenderWork *)work)->mode = 0;
}
