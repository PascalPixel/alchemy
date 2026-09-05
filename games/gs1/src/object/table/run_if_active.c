#include "types.h"

#define ObjectTable_RunIfActive Func_080931d4

s32 Func_08015100();
s32 ObjectTable_ReadActiveValue();

void ObjectTable_RunIfActive(void) {
    if (ObjectTable_ReadActiveValue() != -1) {
        Func_08015100();
    }
}
