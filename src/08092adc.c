#include "object_lookup.h"
#include "types.h"

void Func_08009098(u8 *, void *);
void Func_0809163c(s32 arg0);

void Func_08092adc(s32 arg0, s32 arg1, s32 arg2) {
    u8 *object = Func_0808ba1c(arg0);
    if (object != NULL) {
        *(s16 *)(object + 0x64) = arg1;
        Func_08009098(object, (void *)0x0809FC1C);
        Func_0809163c(arg2);
    }
}
