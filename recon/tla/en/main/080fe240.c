/* Draft, differ 28 of 24: the native keeps the result in r2 beside r0
 * (adds r2, r0, #0 before the -1 test, adds r0, r2, #0 at the end) and
 * builds the zero before the field offset; this coalesces the result into
 * r0. Not yet tried: a narrower result type, or the test inside an inline. */
#include "types.h"
#include "ram_buffer.h"

struct MenuRuntimeSelect {
    u8 unknown_000[0x168];
    u16 unknown_168;
    u8 unknown_16a[0xac];
    u8 selected;
};

s32 Func_080fe580(s32 mode);

s32 Func_080fe240(void)
{
    struct MenuRuntimeSelect *work = Ram_HeapSlots->menu_runtime;
    s32 result;

    work->unknown_168 = 0;
    result = Func_080fe580(0);
    if (result != -1)
        result = work->selected;
    return result;
}
