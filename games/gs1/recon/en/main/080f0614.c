#include "types.h"

s32 Func_080f07f0(void *resource, s32 offset, s32 mode);

void Func_080f0614(void)
{
    s16 current;
    s16 previous;
    s32 current_group;
    s32 previous_group;

    if (*(s16 *)0x02004c04 != 0)
        return;

    current = *(s16 *)0x02004c00;
    current_group = current;
    if (current_group < 0)
        current_group += 7;
    current_group >>= 3;

    previous = *(s16 *)0x02004c08;
    previous_group = previous;
    if (previous_group < 0)
        previous_group += 7;
    previous_group >>= 3;

    if (current_group != previous_group) {
        *(s16 *)0x02004c08 = current;
        *(s16 *)0x02004c04 = Func_080f07f0(
            *(void **)(0x080f1220 + current_group * 4),
            ((current_group + 16) & 31) * 24,
            1);
    }
}
