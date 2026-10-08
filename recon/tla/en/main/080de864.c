#include "types.h"
#include "objdisp.h"
#include "ram_buffer.h"
#include "object_efx.h"

struct Triple08099340 {
    s32 x;
    s32 y;
    s32 z;
};

void Vector_AddPolarOffset(s32, s32, struct Triple08099340 *);

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
            ObjectDispatch_InitializeFar((struct DispatchObject *)arg, (u32)BattleFx_CommonParticleScript);
        }
    }
}
