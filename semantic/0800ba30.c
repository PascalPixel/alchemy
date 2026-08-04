#include "types.h"

struct Child {
    s16 id;
    s16 value;
    u8 frame;
    u8 pad5[7];
    u32 *frames;
    u32 image;
    u8 hidden;
    u8 timer;
};

struct Context {
    u8 pad0[0x22];
    u8 first_a;
    u8 first_b;
    u8 current;
    u8 pad25[2];
    u8 count;
    struct Child *children[1];
};

struct Description {
    u8 pad0[4];
    u8 frame;
    u8 count;
    u8 first_a;
    u8 first_b;
};

extern struct Description *Func_08185000(s32);

void Func_0800ba30(struct Context *context, s32 state)
{
    s32 frame = state & 0x7f;
    s32 flag = state & 0x80;
    s32 index;

    if (context->current != frame) {
        for (index = 0; index < context->count; index++) {
            struct Child *child = context->children[index];
            if (child != 0 && child->frames != 0) {
                struct Description *description = Func_08185000(child->id);
                if (frame < description->count) {
                    child->frame = description->frame;
                    child->image = child->frames[frame];
                    child->timer = 0x10;
                    if (flag == 0) {
                        child->hidden = flag;
                        child->value = flag;
                    }
                    if (index == 0) {
                        context->first_b = description->first_b;
                        context->first_a = description->first_a;
                    }
                }
            }
        }
        context->current = frame;
    }
}
