/* Near miss: listing 37 instructions, draft 33, common 13, differ 44; the rest differs in instruction order. */
#include "types.h"
#include "iwram_call.h"

s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleFx_PlotCanvasPixel(s32 x, s32 y, s32 color);

void Func_080e7f94(s32 unused, s32 radius, s32 angle, s32 color)
{
    s32 (*mul)(s32 a, s32 b) = Iwram_MulQ16;
    s32 x = mul(Trig_Cos(angle), radius);
    s32 y = mul(Trig_Sin(angle), radius);

    BattleFx_PlotCanvasPixel(63 - (x >> 16), 63 - (y >> 16), color);
}
