#include "runtime_1e74.h"

void Func_080bd7dc(s32 parameter) {
    struct Runtime1e74 *runtime;

    runtime = Runtime1e74_Get();
    if (runtime->phase == 0) {
        runtime->phase = 1;
        if (parameter != 0) {
            runtime->parameter = parameter;
        }
    }
}
