#include "far_runtime.h"
#include "types.h"

s32 Func_08015010(s32 first, s32 second, s32 third, s32 fourth, s32 flags);

s32 Func_080a10d0(s32 *handle, s32 first, s32 second, s32 third, s32 fourth, s32 flags)
{
    s32 current;
    s32 masked;

    current = *handle;
    if (current != 0) {
        if (0x100 & flags) {
            return 0;
        }
        Func_08015270(current);
        return 0;
    }
    masked = flags & 0xff;
    *handle = Func_08015010(first, second, third, fourth, masked);
    return 1;
}
