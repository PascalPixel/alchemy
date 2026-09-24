/* NONMATCHING: 204 of 208 bytes, 99 halfword edits (2026-09-24). Structure
 * matches; global allocation gives the options pointer r7 and the x parameter
 * r8, the reverse of the reference (x in r7, options in r8 reloaded through
 * r2/r3), which also costs the four missing movs. Needs
 * gFrameCount=0x03001e40 bound as data. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"


void Local_02004520(s32 x, s32 z)
{
    struct EffectOptions options;
    struct EffectOptions *opts;
    s32 phase;
    u32 coin;

    options.start_scale_x = 0xb333;
    options.start_scale_y = 0xb333;
    opts = &options;
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
