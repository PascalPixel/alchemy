#include "ANIMSPR.H"
#include "FIELDOBJ.H"
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


/* The existing arc-mode counter/link alias view. The callbacks otherwise
   use the shared FieldActor geometry and true object API transports. */
struct ArcObject {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[4];
    s32 scale_x;
    s32 scale_y;
    u8 pad20[0x44];
    u16 counter;
    u8 pad66[2];
    struct ArcObject *linked_object;
};

struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z)
{
    struct BattleFxScene *scene = gEffectWork;
    struct ObjectRuntime *object;
    u8 *child;
    u8 flag;

    /* FAKEMATCH: the existing packed OAM byte clear is retained over the
       real animation part; a bitfield clear adds six native instructions. */
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
        child = (u8 *)&((struct AnimationObject *)object->animation)->part[0];
        child[9] &= ~(flag + 8);
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
    /* FAKEMATCH: retain the existing unsigned arc-counter/link view here;
       canonical mixed views move the link load after the counter store
       and sign extension in all six editions. */
    struct ArcObject *arc = (struct ArcObject *)obj;
    struct FieldActor *link;
    s32 v;

    link = (struct FieldActor *)arc->linked_object;
    v = (s16)++arc->counter;
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
    /* FAKEMATCH: retain the existing unsigned arc-counter/link view here;
       canonical mixed views move the link load after the counter store
       and sign extension in all six editions. */
    struct ArcObject *arc = (struct ArcObject *)obj;
    struct FieldActor *link;
    s32 v;

    link = (struct FieldActor *)arc->linked_object;
    v = (s16)++arc->counter;
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
