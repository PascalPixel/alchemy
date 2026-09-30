#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"

void WaitFrames(s32);

struct EffectObject_0808f1c0;
void Object_Destroy(struct EffectObject_0808f1c0 *);

void EffectRuntime_RunRisingObjectSequence(void *object, s32 flags)
{
    void *other;

    if (object != NULL) {
        other = ObjectTable_Get(gGameState.object_index);
        if (flags & 1) {
            ObjectDispatch_SetSingleChildField26Far(object, 0);
            ObjectDispatch_InitializeFar(object, BattleFx_ParticleEmitterScript);
            FIELD_AT_OFFSET(object, u32 *, 0x28) = 0x20000;
            FIELD_AT_OFFSET(object, u32 *, 0x48) = 0x4000;
            FIELD_AT_OFFSET(object, s32 *, 0x6C) = (s32)&BattleFx_EmitRandomParticleFromEmitter;
        }
        if (flags == 3) {
            WaitFrames(60);
        }
        if (flags & 2) {
            EffectRuntime_PrepareRisingObject(object);
        }
        if (flags == 3) {
            WaitFrames(80);
        }
        Object_SetMode(other, 1);
    }
}
