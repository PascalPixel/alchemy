#include "ANIMSPR.H"
#include "FIELD_EVENT.H"
#include "OBJECT_RUNTIME.H"
#include "OBJDISP.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "FX_SCENE.H"
#include "OBJECT_EFX.H"
#include "FIXED_MATH.H"

extern struct BattleFxScene *gEffectWork;
struct ObjectRuntime *Object_CreateFar(s32, s32, s32, s32);
void Object_Destroy(struct ObjectRuntime *);
void ObjectDispatch_SetSingleChildField26Far(struct ObjectRuntime *, s32);
void Object_SetMode(struct ObjectRuntime *, s32);
void ObjectDispatch_ApplyValueToChildrenFar(struct ObjectRuntime *, s32);
extern u8 gPlayerObjectId[];


struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z)
{
    struct BattleFxScene *scene = gEffectWork;
    struct ObjectRuntime *object;
    struct AnimationObject *child;
    u8 flag;

    object = Object_CreateFar(kind, x, y, z);
    if (object != NULL) {
        if (object->animation_kind == 0) {
            Object_Destroy(object);
            return NULL;
        }
        object->terrain_height = ((struct ObjectRuntime *)scene->main_object)->terrain_height;
        flag = 4;
        object->flags = flag;
        object->unknown_23 = flag;
        child = object->animation;
        child->part[0].attr &= ~((flag + 8) >> 2);
        ObjectDispatch_SetSingleChildField26Far(object, 0);
        Object_SetMode(object, 1);
    }
    return object;
}

void ObjectGroup_SetActionForOthers(struct ObjectRuntime *excluded_object,
                                  s32 group_mode, s32 action)
{
    s16 *active_object_id;
    struct ObjectRuntime *object;
    s32 object_id;

    object_id = 0;
    active_object_id = (s16 *)gPlayerObjectId;
    do {
        object = ObjectTable_Get(object_id);
        if (object_id != *active_object_id && object != NULL && object != excluded_object) {
            object->movement_state = group_mode;
            ObjectDispatch_ApplyValueToChildrenFar(object, action);
        }
        object_id++;
    } while (object_id <= 0x42);
}

void BattleFx_UpdateScaledArcObjectA(struct FieldActor *obj)
{
    struct FieldActor *link;
    s32 v;

    link = *(struct FieldActor **)&obj->unknown_68[0];
    v = (s16)++obj->unknown_64;
    if (v > 31) {
        ObjectDispatch_InitializeFar((struct DispatchObject *)obj, (u32)BattleFx_CommonParticleScript);
        return;
    }
    v = Trig_Sin(v << 10);
    obj->scale_x = v;
    obj->scale_y = v;
    obj->x.fixed = link->x.fixed;
    obj->y.fixed += 0x10000;
    obj->z.fixed = link->z.fixed + (0x10000 - v) * 5 + 0x90000;
}

void BattleFx_UpdateScaledArcObjectB(struct FieldActor *obj)
{
    struct FieldActor *link;
    s32 v;

    link = *(struct FieldActor **)&obj->unknown_68[0];
    v = (s16)++obj->unknown_64;
    if (v > 31) {
        ObjectDispatch_InitializeFar((struct DispatchObject *)obj, (u32)BattleFx_CommonParticleScript);
        return;
    }
    v = Trig_Sin(v << 10);
    obj->scale_x = v;
    obj->scale_y = -v;
    obj->x.fixed = link->x.fixed;
    obj->y.fixed += 0x10000;
    obj->z.fixed = link->z.fixed - (0x10000 - v) * 5 + 0x100000;
}
