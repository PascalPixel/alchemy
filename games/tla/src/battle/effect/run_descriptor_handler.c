#include "types.h"

void *Func_080ccd78(s32 group, s32 id);
void EventRuntime_RunMessage(s32 actor, s32 message);

s32 Func_080cd584(s32 id)
{
    u8 *descriptor = Func_080ccd78(6, id);
    s32 result = -1;

    if (descriptor != NULL) {
        s32 handler = *(s32 *)(descriptor + 8);

        if (handler != 0) {
            if (handler < 0x10000)
                EventRuntime_RunMessage(-1, handler);
            else
                ((void (*)(s32))handler)(id);
            result = 0;
        }
    }
    return result;
}
