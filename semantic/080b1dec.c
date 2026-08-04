#include "types.h"

struct OwnerState_080b1dec {
    u8 padding_000[0xd8];
    u16 entries[15];
};

struct DisplayObject_080b1dec {
    u8 padding_00[15];
    u8 flags;
};

struct OwnerState_080b1dec *Func_08077008(s32 owner);
void Func_08015060(void *target);
s32 Func_08077248(s32 owner);
void Func_08015078(s32 message, void *target, s32 x, s32 y);
struct DisplayObject_080b1dec *Func_080152d0(
    u16 entry,
    s32 kind,
    void *target,
    s32 x,
    s32 y
);

void Func_080b1dec(void *target, s32 owner)
{
    struct OwnerState_080b1dec *state = Func_08077008(owner);
    s32 x = 8;
    s32 y = 0;
    s32 index;
    u16 entry;

    if (target == NULL)
        return;

    Func_08015060(target);
    if (Func_08077248(owner) == 0) {
        Func_08015078(0xc91, target, 8, 20);
        return;
    }

    index = 0;
    do {
        entry = state->entries[index];

        if (entry != 0) {
            struct DisplayObject_080b1dec *object;

            object = Func_080152d0(entry, 27, target, x, y);
            object->flags = 0xfc;
        }

        x += 16;
        if (index == 4 || index == 9) {
            x = 8;
            y += 16;
        }
        index++;
    } while (index <= 14);
}
