#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080b7e24(void *object) {
    s32 object_type;
    s32 remaining;
    void **items;
    void *item;

    if (object != NULL) {
        object_type = 0xF & M2C_FIELD(object, u8 *, 0x54);
        switch (object_type) {
        case 1:
            Func_080b7e04(M2C_FIELD(object, void ***, 0x50));
            return;
        case 2:
            items = M2C_FIELD(object, void ***, 0x50);
            remaining = 3;
            do {
                item = *items++;
                if (item != NULL) {
                    Func_080b7e04(item);
                }
                remaining -= 1;
            } while (remaining >= 0);
            break;
        }
    }
}
