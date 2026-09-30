#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"

void WaitFrames(s32);

struct EffectObject_0808f1c0;
void Object_Destroy(struct EffectObject_0808f1c0 *);

void BattleFx_StartEffectObject22(s32 value, s32 flags)
{
    struct EffectResource_0808f1c0 *resource =
        ObjectTable_Get(gGameState.object_index);
    void *handle = Runtime_AllocateHeapBlock(17, 0x608);
    struct EffectObject_0808f1c0 *object = Object_CreateFar(
        22, resource->x, resource->y + 0x240000, resource->z);

    if (object != 0) {
        struct EffectVisual_0808f1c0 *visual = object->visual;
        s32 mask;

        s32 zero = 0;
        visual->value_26 = zero;
        visual->value_27 = zero;

        visual->flags_a &= zero - 33;

        mask = visual->flags_b & 0x0f;
        {
            s32 clear = 13;
            clear = -clear;
            mask &= clear;
        }
        mask |= 4;
        visual->flags_b = mask;

        ItemIcon_LoadTilesFar(value);
        VramBlock_LoadCached(visual->value_1c, 128, (u8 *)handle + 0x400);
        Runtime_ReleaseHeapBlock(17);

        if (flags & 1)
            object->callback = (void (*)(void))BattleFx_EmitRandomParticleFromEmitter;
        if (flags & 2)
            EffectRuntime_PrepareRisingObject((struct Object_0808f0d8 *)object);

        WaitFrames(80);
        Object_SetMode(resource, 1);
        Object_Destroy(object);
    }
}
