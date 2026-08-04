#include "types.h"
#include "party_state.h"

s32 Func_080796c4(s16 *output)
{
    s32 count = 0;

    if (output != 0) {
        s32 index;

        count = Func_080795fc();
        index = 0;
        if (count != 0) {
            do {
                *output++ = Data_02000240.active_owners[index];
                index++;
            } while (index != count);
        }
        *output = 0xff;
    }
    return count;
}
