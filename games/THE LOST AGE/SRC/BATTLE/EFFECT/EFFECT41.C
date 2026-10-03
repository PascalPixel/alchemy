#include "TYPES.H"
#include "OBJECT_RUNTIME.H"
#include "OBJDISP.H"
#include "OBJECT_EFX.H"

u32 Random16(void);

void BattleFx_UpdateDriftingFallObject(struct ObjectRuntime *object)
{
    s32 r;

    object->y = (s32)((u32)object->y - 0x4ccc);
    r = Random16();
    object->x += r - Random16();
    if (object->y <= object->terrain_height) {
        ObjectDispatch_InitializeFar((struct DispatchObject *)object,
            (u32)BattleFx_CommonParticleScript);
    }
}
