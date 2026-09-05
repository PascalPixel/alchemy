#include "types.h"

#define ObjectDispatch_SetField6c Func_0800ea54

void ObjectDispatch_SetField6c(void *arg0, s32 arg1) {
    if (arg0 != NULL) {
        *(s32 *)((u8 *)arg0 + 0x6C) = arg1;
    }
}
