/* Draft of SceneActor_GetPositionDistance, resource_3b5 at 0x02008040, built with
 * games/THE BROKEN SEAL/SRC/FIELD/TOREBI_MACHI/MACHI.H.
 * Remaining difference: the ROM loads the IWRAM square root's address from its
 * literal pool and enters it through _call_via_r3; called by its name, GCC emits
 * a direct bl (and a long call is not ordinary C), so the last eight bytes differ.
 * The listing keeps these rows. */
#include "MACHI.H"

s32 IwramSqrt(s32 value);

s32 SceneActor_GetPositionDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx *dx;
    s32 dysq = dy *dy;
    s32 dzsq = dz *dz;

    return IwramSqrt(dxsq + dysq + dzsq);
}
