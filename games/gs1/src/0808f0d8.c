#include "object_lookup.h"
#include "types.h"

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

extern struct State_0808f0d8 Data_02000240;
void Object_SetPosition(struct Object_0808f0d8 *, s32, s32, s32);
void WaitFrames(s32);
void Object_SetMode(struct Entity_0808f0d8 *, s32);
void Object_SetCallback(struct Object_0808f0d8 *, const void *);
extern const u8 Data_0809e75c[];

void Func_0808f0d8(struct Object_0808f0d8 *object)
{
    struct Entity_0808f0d8 *entity;

    if (object == 0)
        return;

    entity = GetObject(Data_02000240.object_index);
    object->field34 = 0x10000;
    object->field30 = 0x20000;
    object->field55 = 0;
    Object_SetPosition(object, entity->x, entity->y + 0x240000, entity->z);
    WaitFrames(3);
    Object_SetMode(entity, 28);
    Object_SetCallback(object, Data_0809e75c);
    entity->angle = 0x4000;
}
