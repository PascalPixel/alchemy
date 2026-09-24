#include "IWRAM_CALL.H"

/* Fixed-point remainder: value less the whole multiples of |divisor|, using
   the IWRAM divide at 0x0300013c and the IWRAM Q16 multiply. */
s32 Fixed_Remainder(s32 value, s32 divisor)
{
    s32 quotient;

    if (divisor == 0)
        return 0;
    if (divisor & 0xf0000000)
        divisor = -divisor;
    quotient = ((s32 (*)(s32, s32))0x0300013c)(divisor, value);
    return value - Iwram_MulQ16(quotient & 0xffff0000, divisor);
}
