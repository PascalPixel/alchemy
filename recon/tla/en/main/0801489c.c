#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"

extern u8 Data_03001cb4[];

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position)
{
    *position++ += Iwram_MulQ16(radius, Trig_Sin(angle + 0x4000));
    position++;
    *position += Iwram_MulQ16(radius, Trig_Sin(angle));
}
