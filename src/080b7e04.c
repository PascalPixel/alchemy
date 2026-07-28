#include "types.h"

struct Fields_080b7e04 {
    u8 filler[0x10];
    s32 field10;
};

void Func_080b7e04(void *owner) {
    s32 remaining;
    s32 zero;
    struct Fields_080b7e04 **items;

    if (owner != NULL) {
        zero = 0;
        items = (struct Fields_080b7e04 **)((u8 *)owner + 0x28);
        for (remaining = 3; remaining >= 0; remaining--) {
            struct Fields_080b7e04 *item = *items++;
            if (item != NULL) {
                item->field10 = zero;
            }
        }
    }
}
