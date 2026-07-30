typedef unsigned char u8;
typedef signed int s32;

struct State_0808f1c0 {
    u8 padding000[0x1F4];
    s32 object_index;
};

struct Entity_0808f1c0 {
    u8 padding00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Child_0808f1c0 {
    u8 padding00[5];
    u8 flags5;
    u8 padding06[3];
    u8 flags9;
    u8 padding0a[0x12];
    u8 transfer_slot;
    u8 padding1d[9];
    u8 field26;
    u8 field27;
};

struct Object_0808f1c0 {
    u8 padding00[0x50];
    struct Child_0808f1c0 *child;
    u8 padding54[0x18];
    void (*callback)(void *);
};

typedef char State_0808f1c0_size[
    sizeof(struct State_0808f1c0) == 0x1F8 ? 1 : -1
];
typedef char Entity_0808f1c0_size[
    sizeof(struct Entity_0808f1c0) == 0x14 ? 1 : -1
];
typedef char Child_0808f1c0_size[
    sizeof(struct Child_0808f1c0) == 0x28 ? 1 : -1
];
typedef char Object_0808f1c0_size[
    sizeof(struct Object_0808f1c0) == 0x70 ? 1 : -1
];

void Func_08002dd8(s32);
void Func_080030f8(s32);
s32 Func_08003fa4(s32, s32, const void *);
void *Func_080048b0(s32, s32);
void Func_08009080(struct Entity_0808f1c0 *, s32);
struct Object_0808f1c0 *Func_080090c8(s32, s32, s32, s32);
void Func_080090d0(struct Object_0808f1c0 *);
void Func_08015250(s32);
struct Entity_0808f1c0 *Func_0808ba1c(s32);
void Func_0808eee4(void *);
void Func_0808f0d8(struct Object_0808f1c0 *);

void Func_0808f1c0(s32 value, s32 options)
{
    struct State_0808f1c0 *state;
    struct Entity_0808f1c0 *entity;
    struct Object_0808f1c0 *object;
    struct Child_0808f1c0 *child;
    u8 *allocation;

    state = (struct State_0808f1c0 *)0x02000240;
    entity = Func_0808ba1c(state->object_index);
    allocation = Func_080048b0(0x11, 0x608);
    object = Func_080090c8(
        0x16,
        entity->x,
        entity->y + 0x240000,
        entity->z);

    if (object != 0) {
        child = object->child;
        child->field26 = 0;
        child->field27 = 0;
        child->flags5 &= 0xDF;
        child->flags9 = (child->flags9 & 3) | 4;

        Func_08015250(value);
        Func_08003fa4(
            child->transfer_slot,
            0x80,
            allocation + 0x400);
        Func_08002dd8(0x11);

        if (options & 1)
            object->callback = Func_0808eee4;
        if (options & 2)
            Func_0808f0d8(object);

        Func_080030f8(0x50);
        Func_08009080(entity, 1);
        Func_080090d0(object);
    }
}
