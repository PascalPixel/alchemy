#include "object_lookup.h"
#include "types.h"

void ObjectLink_RotatePairToward(void *, void *);
void BattleRuntime_WaitIfModeZero(s32 arg0);

void Object_LinkPair(s32 arg0, s32 arg1, s32 arg2) {
    void *first = ObjectTable_Get(arg0);
    void *second = ObjectTable_Get(arg1);
    if (first != NULL && second != NULL) {
        ObjectLink_RotatePairToward(first, second);
        BattleRuntime_WaitIfModeZero(arg2);
    }
}
