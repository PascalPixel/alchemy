#include "object_lookup.h"
#include "types.h"

void Func_08092878(void *, void *);
void Func_0809163c(s32 arg0);

void Object_LinkPair(s32 arg0, s32 arg1, s32 arg2) {
    void *first = ObjectTable_Get(arg0);
    void *second = ObjectTable_Get(arg1);
    if (first != NULL && second != NULL) {
        Func_08092878(first, second);
        Func_0809163c(arg2);
    }
}
