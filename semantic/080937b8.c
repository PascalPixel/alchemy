#include "object_lookup.h"
#include "types.h"

struct Child_080937b8 {
    u8 unknown_00[9];
    u8 low_09 : 2;
    u8 copied_09 : 2;
    u8 high_09 : 4;
    u8 unknown_0a[0x1c];
    u8 value_26;
};

struct Object_080937b8 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x3c];
    struct Child_080937b8 *child;
    u8 unknown_54;
    u8 mode_55;
    u8 unknown_56[0x0e];
    u16 unknown_64;
    u16 source_id;
    struct Object_080937b8 *source;
    void (*callback)(void);
};

struct Object_080937b8 *Func_080090c8(s32, s32, s32, s32);
void Func_08009098(struct Object_080937b8 *, const void *);
void Func_08009080(struct Object_080937b8 *, s32);
void Func_0809163c(s32);
void Func_080f9010(s32);
extern u8 Data_00000000[];

void Func_080937b8(u32 source_id, s32 flags, s32 effect)
{
    struct Object_080937b8 *source;
    struct Object_080937b8 *object;
    struct Child_080937b8 *child;

    if ((flags & 0xff) == 6)
        Func_080f9010(110);

    source = Func_0808ba1c(source_id);
    if (source != 0) {
        object = Func_080090c8(21, source->x, source->y, source->z);
        if (object != 0) {
            Func_08009098(object, (const void *)0x0809fc2c);
            Func_08009080(object, flags & 0xf);
            object->mode_55 = 0;
            object->unknown_64 = 0;
            object->source_id = source_id;
            child = object->child;
            object->callback = (void (*)(void))0x0809376d;
            child->value_26 = (u8)(u32)Data_00000000;
            object->source = source;
            if ((flags & 0x100) != 0) {
                child->copied_09 = 1;
            } else {
                child->copied_09 = source->child->copied_09;
            }
        }
        Func_0809163c(effect);
    }
}
