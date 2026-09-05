#include "types.h"

#define NULL ((void *)0)

s32 Func_02002f6a(s32);
void Func_0200314c(s32);
void Func_0200101a(s32);

void SceneState_ApplyFlag815Branch(void) {
    if (Func_02002f6a(0x815) != 0) {
        Func_0200314c(123);
        Func_0200101a(10);
    }
}
