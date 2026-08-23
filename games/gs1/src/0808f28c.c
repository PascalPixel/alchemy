#include "types.h"
#include "fixed_math.h"

struct Values_0808f28c {
    u32 first;
    u32 second;
    u32 third;
};

struct Source_0808f28c {
    u8 padding[8];
    struct Values_0808f28c values;
};

struct Child_0808f28c {
    u8 padding[9];
    u8 flags;
};

struct Object_0808f28c {
    u8 padding[80];
    struct Child_0808f28c *child;
};

extern u32 Random16(void);
extern void RotateVectorByMagnitude(s32, s32, struct Values_0808f28c *);
extern struct Object_0808f28c *Object_Spawn(s32, u32, u32, u32);
extern void Object_SetCallback(struct Object_0808f28c *, void *);
extern void Object_SetMode(struct Object_0808f28c *, s32);

void Func_0808f28c(const struct Source_0808f28c *source)
{
    struct Values_0808f28c values;
    struct Object_0808f28c *object;
    u32 firstRandom;

    if ((100 * Random16() >> 16) > 9)
        return;

    values.first = source->values.first;
    values.second = source->values.second;
    values.third = source->values.third;
    firstRandom = Random16();
    RotateVectorByMagnitude(firstRandom << 4, Random16(), &values);
    object = Object_Spawn(
        0x11D, values.first, values.second, values.third);
    if (object != 0) {
        s32 mask;
        u8 flags;

        Object_SetCallback(object, (void *)0x0809E87C);
        Object_SetMode(object, 0);
        mask = 13;
        flags = object->child->flags;
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object->child->flags = mask;
    }
}
