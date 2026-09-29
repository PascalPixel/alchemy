/* Draft of resource_3a5 0x02008e00 (CalculatePlanarDistance), from
 * games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SELECTED_ACTOR_SCENE_ID_SELECTED_ACTOR_SCENE.C.
 * Remaining difference: none in its instructions, but the ROM reaches the
 * IWRAM square root (IwramSqrt, out of Thumb branch range) by loading its
 * address and branching through r3. C spells that only with a literal
 * address or a long_call attribute, and both are refused, so the listing
 * keeps these rows. */
#include "RAMAKAN.H"

typedef s32 (*IwramSquareRoot)(s32);

s32 CalculatePlanarDistance(s32 *position_a, s32 *position_b)
{
    s32 dx = (*position_b++ - *position_a++) >> 16;
    s32 dz = (*position_b - position_a[1]) >> 16;
    s32 dz_squared = dz *dz;
    s32 dx_squared = dx *dx;

    return ((IwramSquareRoot)0x030001d8)(dx_squared + dz_squared);
}
