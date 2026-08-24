#include "types.h"

s32 Func_08015100();
s32 ObjectTable_ReadActiveValue();

void Func_080931d4(void) {
    if (ObjectTable_ReadActiveValue() != -1) {
        Func_08015100();
    }
}
