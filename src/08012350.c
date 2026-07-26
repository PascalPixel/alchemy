#include "types.h"

typedef int bool;
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_080030f8(s32);

void Func_08012350(void) {
    s32 *sp;
    s32 count;

    sp = *(s32 **)0x03001E70;
    count = 0;
    if (sp[1] > 255 || sp[2] > 255) {
        goto body;
body:
        Func_080030f8(1);
        ++count;
        if (count >= 300) {
            goto done;
        }
        if (sp[1] > 255) {
            goto body;
        }
        if (sp[2] > 255) {
            goto body;
        }
    }
done:
    sp[3] = 0;
}
