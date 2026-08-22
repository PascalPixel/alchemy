#include "types.h"

#define SIGNATURE_080FA280 0x68736D53

struct Track_080fa280 {
    u8 unknown_00[36];
    u16 target;
    u16 current;
    u16 state;
    u8 unknown_2a[10];
    u32 signature;
};

void Func_080fa280(struct Track_080fa280 *track, u32 value)
{
    u16 narrowed = (u16)value;

    if (track->signature != SIGNATURE_080FA280)
        return;

    track->current = narrowed;
    track->target = narrowed;
    track->state = 0x100;
}
