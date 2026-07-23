typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

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
struct Entity_0808f0d8 *Func_0808ba1c(s32);
void Func_08009150(struct Object_0808f0d8 *, s32, s32, s32);
void Func_080030f8(s32);
void Func_08009080(struct Entity_0808f0d8 *, s32);
void Func_08009098(struct Object_0808f0d8 *, const void *);

void Func_0808f0d8(struct Object_0808f0d8 *object)
{
    struct Entity_0808f0d8 *entity;

    if (object == 0)
        return;

    entity = Func_0808ba1c(Data_02000240.object_index);
    object->field34 = 0x10000;
    object->field30 = 0x20000;
    object->field55 = 0;
    Func_08009150(object, entity->x, entity->y + 0x240000, entity->z);
    Func_080030f8(3);
    Func_08009080(entity, 28);
    Func_08009098(object, (const void *)0x0809e75c);
    entity->angle = 0x4000;
}
