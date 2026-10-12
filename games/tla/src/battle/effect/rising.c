#include "types.h"
#include "object_runtime.h"
#include "mem.h"

s32 EventRuntime_GetControlledOwner(void);
void WaitFrames(s32);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object, s32 flags);
void ObjectDispatch_InitializeFar(struct ObjectRuntime *object, const u8 *script);
void BattleFx_EmitRandomParticleFromEmitter(void);
void Func_080cf350(struct ObjectRuntime *object);
extern const u8 Data_080effd8[];

void Func_080cf3b4(struct ObjectRuntime *object, s32 flags)
{
    struct ObjectRuntime *owner;

    if (object != 0) {
        owner = ObjectTable_Get(EventRuntime_GetControlledOwner());
        if (flags & 1) {
            ObjectDispatch_SetSingleChildFlagsFar(object, 0);
            ObjectDispatch_InitializeFar(object, Data_080effd8);
            object->velocity_y = 0x20000;
            object->gravity = 0x4000;
            object->update = BattleFx_EmitRandomParticleFromEmitter;
        }
        if (flags == 3)
            WaitFrames(60);
        if (flags & 2)
            Func_080cf350(object);
        if (flags == 3)
            WaitFrames(80);
        Object_SetMode(owner, 1);
    }
}

struct EffectVisual {
    u8 unknown_00[5];
    u8 flags_05;
    u8 unknown_06[3];
    u8 flags_09;
    u8 unknown_0a[6];
    u8 tile_slot;
    u8 unknown_11[9];
    u8 value_1a;
    u8 value_1b;
};
struct ObjectRuntime *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Func_08038248(s32 item);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
void Object_Destroy(struct ObjectRuntime *object);

void Func_080cf424(s32 item, s32 flags)
{
    struct ObjectRuntime *owner = ObjectTable_Get(EventRuntime_GetControlledOwner());
    u8 *buffer = Runtime_AllocateHeapBlock(68, 0x608);
    struct ObjectRuntime *object = Func_080200c0(489, owner->x, owner->y + 0x240000, owner->z);

    if (object != 0) {
        struct EffectVisual *visual = object->animation;

        s32 mask;
        s32 zero = 0;

        visual->value_1a = zero;
        visual->value_1b = zero;
        visual->flags_05 &= zero - 33;

        mask = visual->flags_09 & 0x0f;
        {
            s32 clear = 13;

            clear = -clear;
            mask &= clear;
        }
        mask |= 4;
        visual->flags_09 = mask;

        Func_08038248(item);
        VramBlock_LoadCached(visual->tile_slot, 128, buffer + 0x400);
        Runtime_ReleaseHeapBlock(68);

        if (flags & 1)
            object->update = BattleFx_EmitRandomParticleFromEmitter;
        if (flags & 2)
            Func_080cf350(object);

        WaitFrames(80);
        Object_SetMode(owner, 1);
        Object_Destroy(object);
    }
}
