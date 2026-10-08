#include "types.h"
#define FIELD_AT(base, type, off) (((struct { u8 pad[off]; type v; } *)(base))->v)

#include "call.h"
s32 Random16Far();

void Func_0200062c(s32 a0)
{
    s32 t0;
    s32 t1;

    t0 = Random16Far();
    FIELD_AT(a0, s32, 24) = ((FIELD_AT(a0, s32, 24) + ((u32)(t0 << 12) >> 16)) + -2048);
    t1 = Random16Far();
    FIELD_AT(a0, s32, 28) = ((FIELD_AT(a0, s32, 28) + ((u32)(t1 << 12) >> 16)) + -2048);
    if (FIELD_AT(a0, s32, 24) <= 0xfff) {
        FIELD_AT(a0, s32, 24) = 0x2000;
    }
    if (FIELD_AT(a0, s32, 28) <= 0xfff) {
        FIELD_AT(a0, s32, 28) = 0x2000;
    }
}

