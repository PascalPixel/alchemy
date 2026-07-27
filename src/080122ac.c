#include "types.h"

struct Position_080122c8;

s32 Func_08012204(struct Position_080122c8 *);

s32 Func_080122ac(s32 arg0, struct Position_080122c8 *position) {
    if ((u32)(Func_08012204(position) - 5) <= 7U) {
        return 0;
    }
    return -1;
}
