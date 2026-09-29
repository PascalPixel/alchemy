#include "RAMAKAN.H"
#include "IWRAM_CALL.H"

/* The whole-pixel distance across the ground between two 16.16 positions of
   three words each, x and z apart; the resident square root takes the sum
   of the squares. */
s32 RamakanSabaku_CalculatePlanarDistance(s32 *position_a, s32 *position_b)
{
    s32 dx = (*position_b++ - *position_a++) >> 16;
    s32 dz = (*position_b - position_a[1]) >> 16;
    s32 dz_squared = dz * dz;
    s32 dx_squared = dx * dx;

    return Iwram_Sqrt(dx_squared + dz_squared);
}
