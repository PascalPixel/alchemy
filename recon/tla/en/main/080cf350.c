/*
 * Canonical draft API context; no match or adoption is claimed.
 * API context measured 2026-10-02 with ordinary target flags:
 * all six original incomplete-context compile failures remain.
 * No missing view, declaration or physical symbol was supplied.
 * The shared void DispatchObject/u32 contract uses ordinary data casts;
 * every original matching-body and trial annotation is retained.
 */
#include "OBJECT_LOOKUP.H"
#include "OBJDISP.H"
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
    ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)RomBytes_0809e75c);
    entity->angle = 0x4000;
}
