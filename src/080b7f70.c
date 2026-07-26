#include "types.h"

struct Fields_080b7f70 {
    u8 filler[0x50];
    void *field50;
    u8 field54;
};

void *Func_080b7f70(struct Fields_080b7f70 *arg0, s32 arg1) {
    s32 type = arg0->field54 & 0xF;
    if (type == 1) {
        if (arg1 == 0) {
            return arg0->field50;
        }
    } else if (type == 2) {
        return ((void **)arg0->field50)[arg1];
    }
    return NULL;
}
