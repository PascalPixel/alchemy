#include "types.h"
#include "object_effect.h"

struct Values_0808eee4 {
    s32 first;
    s32 second;
    s32 third;
};

struct Source_0808eee4 {
    u8 padding[8];
    struct Values_0808eee4 values;
    u8 padding2[20];
    s32 offset;
    u8 padding3[41];
    u8 state;
};

struct Child_0808eee4 {
    u8 padding[9];
    u8 flags;
};

struct Object_0808eee4 {
    u8 padding[80];
    struct Child_0808eee4 *child;
};

#define OBJECT_0808EEE4_OFFSET(type, field) \
    ((u32)&(((type *)0)->field))
typedef char Values_0808eee4_size[
    sizeof(struct Values_0808eee4) == 0x0c ? 1 : -1
];
typedef char Source_0808eee4_offset_offset[
    OBJECT_0808EEE4_OFFSET(struct Source_0808eee4, offset) == 0x28 ? 1 : -1
];
typedef char Source_0808eee4_state_offset[
    OBJECT_0808EEE4_OFFSET(struct Source_0808eee4, state) == 0x55 ? 1 : -1
];
typedef char Object_0808eee4_child_offset[
    OBJECT_0808EEE4_OFFSET(struct Object_0808eee4, child) == 0x50 ? 1 : -1
];
#undef OBJECT_0808EEE4_OFFSET

extern u32 Func_08004458(void);
extern void Func_0800447c(s32, s32, struct Values_0808eee4 *);
extern void Func_08009098(struct Object_0808eee4 *, void *);
extern void Func_08009080(struct Object_0808eee4 *, s32);

void Func_0808eee4(struct Source_0808eee4 *source)
{
    struct Values_0808eee4 values;
    struct Object_0808eee4 *object;
    u32 random_angle;

    if (source->offset >= -255 && source->offset <= 255)
        source->state = 0;

    if ((100 * Func_08004458() >> 16) > 9)
        return;

    values.first = source->values.first;
    values.second = source->values.second;
    values.third = source->values.third;
    random_angle = Func_08004458();
    Func_0800447c(random_angle << 4, Func_08004458(), &values);
    object = (struct Object_0808eee4 *)Func_08096c80(
        0x11D, values.first, values.second, values.third);
    if (object != 0) {
        s32 mask;
        u8 flags;

        Func_08009098(object, (void *)0x0809E87C);
        Func_08009080(object, 0);
        mask = 13;
        flags = object->child->flags;
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object->child->flags = mask;
    }
}
