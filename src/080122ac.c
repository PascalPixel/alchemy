#include "types.h"
#include "map.h"

struct WorldPosition;

s32 Func_08012204(struct WorldPosition *);

s32 Func_080122ac(s32 arg0, struct WorldPosition *position) {
    if ((u32)(Func_08012204(position) - 5) <= 7U) {
        return 0;
    }
    return -1;
}
