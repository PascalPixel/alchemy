#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08017364(void) {
    s32 result;
    s32 channel_no;
    void **channel;
    void *entry;

    channel = *(s32 *)0x03001E8C + 0x620;
    channel_no = 0;
loop_1:
    entry = *channel;
    if ((entry == NULL) || (result = 0, (M2C_FIELD(entry, u16 *, 0x14) != 0))) {
        channel_no += 1;
        channel += 0xA;
        if (channel_no == 3) {
            result = 1;
        } else {
            goto loop_1;
        }
    }
    return result;
}
