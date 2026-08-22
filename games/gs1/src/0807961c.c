#include "types.h"
#include "party_state.h"
s32 Func_08079358(s32 arg0);

s32 Func_0807961c(s32 value) {
    s32 count = Func_080795fc();
    s32 index;

    Func_08079358(value);
    index = 0;
    while (index < count) {
        if (Data_02000240.active_owners[index] == value) {
            return count;
        }
        index++;
    }
    Data_02000240.active_owners[index] = value;
    return count + 1;
}
