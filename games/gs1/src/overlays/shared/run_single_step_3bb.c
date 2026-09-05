#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_02003be4();

void FieldScene_RunSingleStep(void) {
    Func_02003be4();
}
