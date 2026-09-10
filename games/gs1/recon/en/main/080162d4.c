#include "types.h"

struct UiWork {
    s32 state;
    struct UiWork *self;
    u16 x;
    u16 y;
    u16 kind;
    u16 id;
    u16 f16;
    u16 f18;
    u16 f20;
    u16 flags;
    u16 f24;
    s16 timer;
    u8 pad[8];
};

extern u8 *Data_03001e8c;
extern u8 Data_00000002[];
extern u8 Data_00000008[];
extern u8 Data_00000020[];
extern u8 Data_00000040[];
extern u8 Data_00000080[];

void Func_080030f8(s32 frames);
void Func_08016230(struct UiWork *);
void Func_080163ec(struct UiWork *);
void Func_080173ac(s32);

struct UiWork *Func_080162d4(s32 kind, s32 id, s32 x, s32 y, s32 attrs)
{
    struct UiWork *slot;
    struct UiWork *found;
    s32 i;

    slot = (struct UiWork *)(Data_03001e8c + 0x500);
    found = 0;
    i = 0;
    while ((slot->flags & 1) != 0 || slot->timer != 0) {
        i++;
        slot++;
        if (i == 8) {
            goto done;
        }
    }
    found = slot;
done:
    if (found != 0) {
        found->id = id;
        found->x = x;
        found->y = y;
        found->kind = kind;
        found->state = 0;
        found->f20 = 0;
        found->self = slot;
        found->f16 = 1;
        found->flags = 1;
        Func_080173ac(kind);
        if (attrs & 8) {
            found->flags |= (s32)Data_00000008;
        }
        if (attrs & 32) {
            found->flags |= (s32)Data_00000020;
        }
        if (attrs & 64) {
            found->flags |= (s32)Data_00000040;
        }
        if (attrs & 128) {
            found->flags |= (s32)Data_00000080;
        }
        if (attrs & 0x100) {
            found->flags |= 0x100;
        }
        if (attrs & 2) {
            found->flags |= (s32)Data_00000002;
            found->f24 = 0;
            found->timer = 1;
            Func_08016230(found);
        } else {
            found->timer = 8;
            found->f24 = 7;
            Func_080163ec(found);
            Func_080030f8(1);
        }
    }
    return found;
}
