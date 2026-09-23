#include "IWRAM_CALL.H"

s32 Fixed_Remainder(s32 value, s32 divisor);

#define Iwram_DivQ16 ((s32 (*)(s32, s32))0x0300013c)

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
        s32 (*divide)(s32, s32) = Iwram_DivQ16;

        return divide(60 << 16, product);
    }
out:
    return low;
}
