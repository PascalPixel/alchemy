#include "object_lookup.h"
#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "global_cells.h"

/* effect_runtime/prepare_rising_object.c */
struct State_0808f0d8 {
    u8 pad0[0x1f4];
    s32 object_index;
};

struct Entity_0808f0d8 {
    u8 pad0[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
};

struct Object_0808f0d8 {
    u8 pad0[0x30];
    s32 field30;
    s32 field34;
    u8 pad38[0x1d];
    u8 field55;
};

extern struct State_0808f0d8 gCell;
void Object_SetPosition(struct Object_0808f0d8 *, s32, s32, s32);
void WaitFrames(s32);
void Object_SetMode(void *, s32);
void Object_SetCallback(void *, const void *);
extern const u8 RomBytes_0809e75c[];

void EffectRuntime_PrepareRisingObject(struct Object_0808f0d8 *object)
{
    struct Entity_0808f0d8 *entity;

    if (object == 0)
        return;

    entity = ObjectTable_Get(gCell.object_index);
    object->field34 = 0x10000;
    object->field30 = 0x20000;
    object->field55 = 0;
    Object_SetPosition(object, entity->x, entity->y + 0x240000, entity->z);
    WaitFrames(3);
    Object_SetMode(entity, 28);
    Object_SetCallback(object, RomBytes_0809e75c);
    entity->angle = 0x4000;
}

/* effect_runtime/run_rising_object_sequence.c */
void Sys_Apply(void *, s32);
extern const u8 gRom[];
extern u8 gRom2;

void EffectRuntime_RunRisingObjectSequence(void *object, s32 flags)
{
    void *other;

    if (object != NULL) {
        other = ObjectTable_Get(gCell.object_index);
        if (flags & 1) {
            Sys_Apply(object, 0);
            Object_SetCallback(object, gRom);
            FIELD_AT_OFFSET(object, u32 *, 0x28) = 0x20000;
            FIELD_AT_OFFSET(object, u32 *, 0x48) = 0x4000;
            FIELD_AT_OFFSET(object, s32 *, 0x6C) = (s32)&gRom2;
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
