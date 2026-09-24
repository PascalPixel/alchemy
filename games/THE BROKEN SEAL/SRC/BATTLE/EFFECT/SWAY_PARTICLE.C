#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"

/* Sways the object four units either side of its anchor x, stepping a
   128-step phase each frame. */
void BattleFx_SwayParticle(u8 *object)
{
    s16 *phase;

    phase = (s16 *)(object + 100);
    *(s32 *)(object + 8) = *(s32 *)(object + 56) + Iwram_MulQ16(0x40000, Trig_Sin(*phase << 9));
    (*phase)++;
    *phase = (*phase + 128) % 128;
}
