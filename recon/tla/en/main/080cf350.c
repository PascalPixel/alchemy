#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"

void WaitFrames(s32);

struct EffectObject_0808f1c0;
void Object_Destroy(struct EffectObject_0808f1c0 *);

void EffectRuntime_PrepareRisingObject(struct Object_0808f0d8 *object)
{
    struct Entity_0808f0d8 *entity;

    if (object == 0)
        return;

    entity = ObjectTable_Get(gGameState.object_index);
    object->field34 = 0x10000;
    object->field30 = 0x20000;
    object->field55 = 0;
    Object_SetPosition(object, entity->x, entity->y + 0x240000, entity->z);
    WaitFrames(3);
    Object_SetMode(entity, 28);
    ObjectDispatch_InitializeFar(object, RomBytes_0809e75c);
    entity->angle = 0x4000;
}
