#include "runtime_1e74.h"

s32 Func_080041d8(s32, s32);

s32 Func_080bd808(s32 parameter) {
    struct Runtime1e74 *runtime;

    runtime = Runtime1e74_Get();
    runtime->value_7fc = 0;
    runtime->value_804 = 0;
    runtime->value_808 = parameter;
    runtime->phase = 2;
    runtime->flag_655 = 0;
    return Func_080041d8(0x080BD899, 0xC80);
}
