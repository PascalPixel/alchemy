/*
 * Draft: BattleFx_UpdateShrinkingOrbitObject, ported from its ☀️ twin with
 * the effect work from its heap slot. Score 60: the listing shifts the count
 * for the angle (lsls r0, r2, #16) after storing z, where this C shifts it
 * earlier; 30 seconds of permuting found nothing better.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "OBJECT_EFX.H"

struct Triple08099340 {
    s32 x;
    s32 y;
    s32 z;
};

void Vector_AddPolarOffset(s32, s32, struct Triple08099340 *);
void Object_SetCallback(void *, const void *);

/* Counts the object's orbit down, setting it at the shrinking polar offset
   from the effect work's centre, and hands it the common particle script
   when the count runs out. */
void BattleFx_UpdateShrinkingOrbitObject(u8 *arg)
{
    s32 *global = Ram_HeapSlots->effect_work;
    struct Triple08099340 local;
    s16 value;
    s32 raw;

    if (arg != 0) {
        raw = *(u16 *)(arg + 100) - 1;
        *(u16 *)(arg + 100) = raw;
        value = (s16)raw;
        if (value != 0) {
            local.x = global[1];
            local.y = global[2] + 0xA0000;
            local.z = global[3];
            Vector_AddPolarOffset(value << 16,
                          *(s16 *)(arg + 102) + (value << 11),
                          &local);
            *(s32 *)(arg + 8) = local.x;
            *(s32 *)(arg + 12) = local.y;
            *(s32 *)(arg + 16) = local.z;
        } else {
            Object_SetCallback(arg, BattleFx_CommonParticleScript);
        }
    }
}
