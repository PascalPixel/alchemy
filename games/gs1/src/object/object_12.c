#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "object_lookup.h"

/* object/replace_resource_entry.c */
s32 Resource_ResetEntry(u32 index);

void *Object_ReplaceResourceEntry(void *src, void *alt)
{
    void *ret;
    void *obj;

    obj = src;
    ret = NULL;
    if (obj != NULL) {
        if (alt == NULL) {
            FIELD_AT_OFFSET(obj, u8 *, 0x1D) = (u8)(FIELD_AT_OFFSET(obj, u8 *, 0x1D) | 1);
        } else {
            Resource_ResetEntry(FIELD_AT_OFFSET(obj, u8 *, 0x1C));
            FIELD_AT_OFFSET(obj, u8 *, 0x1C) = (u8)FIELD_AT_OFFSET(alt, u8 *, 0x1C);
            FIELD_AT_OFFSET(obj, u8 *, 0x1D) = (u8)(FIELD_AT_OFFSET(obj, u8 *, 0x1D) | 1);
            obj = alt;
        }
        ret = obj;
    }
    return ret;
}

/* object/spawn.c */
u8 *Obj_Run(s32, s32, s32, s32);
void Object_Destroy(void);

void Object_SetMode(u8 *, s32);

u8 *Object_Spawn(s32 kind, s32 x, s32 y, s32 z)
{
    u8 *base = *(u8 **)ADDR_03001F30;
    u8 *object;
    u8 *child;
    u8 flag;

    object = Obj_Run(kind, x, y, z);
    if (object != 0) {
        if (object[84] == 0) {
            Object_Destroy();
            return 0;
        }
        *(u32 *)(object + 20) = *(u32 *)(*(u8 **)(base + 16) + 20);
        flag = 4;
        object[85] = flag;
        object[35] = flag;
        child = *(u8 **)(object + 80);
        child[9] &= ~(flag + 8);
        Obj_Apply(object, 0);
        Object_SetMode(object, 1);
    }
    return object;
}

/* object/group/set_action_for_others.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Object_SetAction(s32, s32);
extern u8 gOv[];

void ObjectGroup_SetActionForOthers(s32 excluded_object, s32 group_mode, s32 action)
{
    s16 *active_object_id;
    s32 object;
    s32 object_id;

    object_id = 0;
    active_object_id = (s16 *)gOv;
    do {
        object = ObjectTable_Get(object_id);
        if ((object_id != *active_object_id) && (object != 0) && (object != excluded_object)) {
            FIELD_AT_OFFSET(object, s8, 0x5B) = group_mode;
            Object_SetAction(object, action);
        }
        object_id += 1;
    } while (object_id <= 0x42);
}
