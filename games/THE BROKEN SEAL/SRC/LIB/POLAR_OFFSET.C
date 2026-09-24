#include "IWRAM_CALL.H"

s32 Func_08002322(s32 angle);

/* Moves an (x, y, z) position radius along angle in the x-z plane. */
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position)
{
    *position++ += Iwram_MulQ16(radius, Func_08002322(angle + 0x4000));
    position++;
    *position += Iwram_MulQ16(radius, Func_08002322(angle));
}
