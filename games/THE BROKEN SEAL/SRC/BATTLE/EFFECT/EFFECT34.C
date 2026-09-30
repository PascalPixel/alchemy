#include "OBJECT_RUNTIME.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "OBJECT_EFX.H"
#include "FIXED_MATH.H"

extern u8 Data_03001f30[];
struct ObjectRuntime *Object_CreateFar(s32, s32, s32, s32);
void Object_Destroy(struct ObjectRuntime *);
void ObjectDispatch_SetSingleChildField26Far(struct ObjectRuntime *, s32);
void Object_SetMode(struct ObjectRuntime *, s32);
void ObjectDispatch_ApplyValueToChildrenFar(struct ObjectRuntime *, s32);
extern u8 gPlayerObjectId[];

struct ArcObject {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[4];
    s32 scale_x;
    s32 scale_y;
    u8 pad20[0x44];
    u16 step;
    u8 pad66[2];
    struct ArcObject *link;
};

struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z)
{
    u8 *base = *(u8 **)((u32)&Data_03001f30);
    struct ObjectRuntime *object;
    u8 *child;
    u8 flag;

    object = Object_CreateFar(kind, x, y, z);
    if (object != NULL) {
        if (object->animation_kind == 0) {
            Object_Destroy(object);
            return NULL;
        }
        object->terrain_height = *(s32 *)(*(u8 **)(base + 16) + 20);
        flag = 4;
        object->flags = flag;
        object->unknown_23 = flag;
        child = object->animation;
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

void BattleFx_UpdateScaledArcObjectA(struct ArcObject *obj)
{
    struct ArcObject *link;
    s32 v;

    link = obj->link;
    v = (s16)++obj->step;
    if (v > 31) {
        ObjectDispatch_InitializeFar((s32)obj, BattleFx_CommonParticleScript);
        return;
    }
    v = Trig_Sin(v << 10);
    obj->scale_x = v;
    obj->scale_y = v;
    obj->x = link->x;
    obj->y += 0x10000;
    obj->z = link->z + (0x10000 - v) * 5 + 0x90000;
}

void BattleFx_UpdateScaledArcObjectB(struct ArcObject *obj)
{
    struct ArcObject *link;
    s32 v;

    link = obj->link;
    v = (s16)++obj->step;
    if (v > 31) {
        ObjectDispatch_InitializeFar((s32)obj, BattleFx_CommonParticleScript);
        return;
    }
    v = Trig_Sin(v << 10);
    obj->scale_x = v;
    obj->scale_y = -v;
    obj->x = link->x;
    obj->y += 0x10000;
    obj->z = link->z - (0x10000 - v) * 5 + 0x100000;
}
