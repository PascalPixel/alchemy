#include "types.h"
#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

#include "call.h"
s32 Func_02001054();

void Func_02000870(void)
{
    Call3(Func_02001054, 9, 0, 0x13);
}
