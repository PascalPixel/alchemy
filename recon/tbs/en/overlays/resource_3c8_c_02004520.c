/* 2026-09-30 (Jupiter): tagged register pins (options r8, x r7, z sl,
 * phase r6, coin r5) reach the full 208 bytes and 46 differing halfwords
 * (from 99). Remaining: the reference sets the scale pair through r2 = &options
 * before mov r8, r2 and loads the pool constant before sub sp; the first
 * spawn builds x + draw in r5 (reusing the draw register) and reads z from
 * sl before the divide; the second branch computes r * 17 through r3.
 * Pinning fewer registers (no options, no phase/coin) scores 76-77. */
/* 2026-09-27 sol-venus-room bounded revalidation: full normalized diff and
 * allocator-order dump read, with the complete three-word pool accounted.
 * Retained H1 is 204/208 bytes, 99 differing halfwords / 35 aligned edits.
 * The observed named options lifetime still outranks x; the four missing
 * bytes are the consequence of the reversed carriers, not missing stores.
 * Exact siblings already reject omitted initialization and wrong call ABI.
 * Keep the existing typed options model without a declaration/order sweep.
 * No new function bytes or alignment credit; still C not yet written.
 *
 * NONMATCHING: 204 of 208 bytes, 99 halfword edits (2026-09-24). Structure
 * matches; global allocation gives the options pointer r7 and the x parameter
 * r8, the reverse of the reference (x in r7, options in r8 reloaded through
 * r2/r3), which also costs the four missing movs. Needs
 * gFrameCount=0x03001e40 bound as data.
 * 2026-09-27 complete extent 02004520..020045f0 includes three pool words.
 * Stable unit baseline: 204/208 bytes, 99 differing halfwords, 35 aligned
 * edits. Exact DUST_STEP, RISING_SPRAY, PARTICLE_WAVE and Effect_Spawn
 * confirm the 40-byte options layout and flags 0x880000 (scale and spin).
 * Both argument pairs are scalar coordinates; the second branch really
 * uses x for its z expression. No omitted initialization or wrong callee
 * interface was found. Retain the baseline; no new allocation sweep.
 * 2026-09-27 H1 transfers RISING_SPRAY's options-pointer ownership: create
 * opts before the first field writes and use it for both scale fields.
 * Prediction: replacing the anonymous stack-address carrier with the named
 * pointer could move x ahead of options in allocation. Expansion/CSE does
 * retain the named opts pseudo (34 instead of anonymous 37), but lreg still
 * reports 6 references / 58 instructions / 6 calls versus x's 4 / 47 / 6.
 * Global order remains coin, phase, opts, x, z: r5, r6, r7, r8, sl. Thus the
 * full 204-byte output is identical to baseline, including all three pool
 * values; whole-owner score stays 99 halfwords / 35 aligned edits. Frame 56,
 * random/division call sequence and both spawn shapes remain admitted, but
 * x=r7/options=r8 and the four missing bytes are not recovered. Keep this
 * exact-sibling pointer form as the saved H1; this ownership axis is closed.
 * The ROM's r5 reuse from branch draw to position draw already occurs with
 * distinct source locals; it provides no new x/options lifetime mechanism.
 * Do not turn either observation into declaration or random-local sweeps. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"


void VinasuHeya_SpawnRandomParticles(s32 x_arg, s32 z_arg)
{
    struct EffectOptions options;
    register struct EffectOptions *opts asm("r8") = &options; /* FAKEMATCH: pins the options pointer to r8 */
    register s32 x asm("r7") = x_arg; /* FAKEMATCH: pins x to r7 */
    register s32 z asm("r10") = z_arg; /* FAKEMATCH: pins z to sl */
    register s32 phase asm("r6"); /* FAKEMATCH: pins the phase to r6 */
    register u32 coin asm("r5"); /* FAKEMATCH: pins the coin to r5 */

    opts->start_scale_x = 0xb333;
    opts->start_scale_y = 0xb333;
    opts->spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
    phase = gFrameCount & 3;
    if (phase == 0) {
        coin = (u32)(Engine_RandomNext() << 1) >> 16;
        if (coin != 0) {
            s32 r = Engine_RandomNext();
            s32 vz = Engine_MathDivide((((u32)(Engine_RandomNext() * 5) >> 16) << 16) + 0x70000, 10);

            Effect_Spawn((x + (((u32)(r << 1) >> 16) << 4)) << 16, 0, z << 19, 0, phase, vz, 0x880000, opts);
        } else {
            s32 r = Engine_RandomNext();

            Effect_Spawn((x + ((u32)(r * 17) >> 16)) << 16, 0, (x << 19) - 0x40000, 0, coin, coin, 0x880000, opts);
        }
    }
}
