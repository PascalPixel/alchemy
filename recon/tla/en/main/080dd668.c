/* UpdateRisingParticleBurst: lift and spin the source for 31 frames, then
   burst eight item-break fragments with random speeds and headings. */
#include "TYPES.H"
#include "OBJECT_EFX.H"
#include "SYSTEM.H"

extern void *Object_Spawn(s32, s32, s32, s32);
extern void Motion_SetTargetPositionFromMagnitudeAngle(void *object, s32 magnitude, s32 angle);
extern void Engine_ObjectSetScript(void *object, const void *script);
extern void Object_Destroy(void *object);
extern void Audio_PlayCue(s32);

void UpdateRisingParticleBurst(void *source)
{
    s32 count;
    s32 dist;
    u32 vel;
    void *child;
    /* FAKEMATCH: holds 0x10000 in sl across the burst loop as the ROM does */
    register s32 unit asm("sl");

    Audio_PlayCue(154);
    for (count = 30; count >= 0; count--) {
        *(s32 *)((s8 *)source + 12) += 0x10000;
        *(u16 *)((s8 *)source + 6) += 0x2000;
        *(s32 *)((s8 *)source + 24) += -0x800;
        *(s32 *)((s8 *)source + 28) += -0x800;
        WaitFrames(1);
    }
    count = 7;
    unit = 0x10000;
    for (; count >= 0; count--) {
        child = Object_Spawn(0x11d, *(s32 *)((s8 *)source + 8),
                             *(s32 *)((s8 *)source + 12),
                             *(s32 *)((s8 *)source + 16));
        if (child != 0) {
            Engine_ObjectSetScript(child, &BattleFx_FragmentScript);
            {
                s32 speed = Random16();

                *(s32 *)((s8 *)child + 0x34) = unit;
                *(s32 *)((s8 *)child + 0x30) = speed + unit;
            }
            *(s8 *)((s8 *)child + 0x55) = 2;
            *(s32 *)((s8 *)child + 0x48) = 0xa3d;
            vel = Random16();
            *(s32 *)((s8 *)child + 0x28) = vel - Random16();
            dist = Random16() * 0x18;
            dist += 0x80000;
            Motion_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
        }
    }
    Audio_PlayCue(131);
    Object_Destroy(source);
}
