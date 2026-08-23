#include "types.h"
#include "runtime_interfaces.h"

s32 UiText_BuildRenderEntries(s32, s32);

void Func_08018790(s32 no, s32 *x, s32 *y) {
    Func_08018850(UiText_BuildRenderEntries(no, 0), x, y, 0);
}
