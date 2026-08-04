#include "types.h"

extern u8 Data_00000000[];

void Func_08015270(void *);
s32 Func_080770c0(s32);
void Func_08015070(void *, s32, s32, s32, s32);
void Func_080a1ac0(s32, s32);
s32 Func_080a7d68(void);
s32 Func_080a7a34(void);
void Func_080a17c4(void *);
void Func_080030f8(s32);

s32 Func_080a77a4(s32 index)
{
    s32 object_offset;
    s32 first_object_offset;
    s32 variant_offset;
    s32 result;
    s32 variant;
    void *object;
    void *state;

    variant_offset = index + 28;
    object_offset = index * 4;
    state = *(void **)0x03001F2C;
    first_object_offset = object_offset;
    first_object_offset += 20;
    object = *(void **)((u8 *)state + first_object_offset);
    variant = *(s8 *)((u8 *)state + variant_offset);
    result = 0;
    *((s8 *)object + 5) = 1;
    *(s16 *)((u8 *)object + 12) = result;
    Func_08015270(*(void **)((u8 *)state + 16));
    if (Func_080770c0(370) != 0)
        Func_08015070(*(void **)((u8 *)state + 16), 9, 1, 9, 3);

    if (variant == -1) {
        *(u8 *)((u8 *)state + variant_offset) = (s32)Data_00000000;
    } else {
        Func_080a1ac0(variant * 24 - 10, 16);
    }

    if (*(u16 *)((u8 *)state + 544) == 3)
        result = Func_080a7d68();
    else
        result = Func_080a7a34();

    Func_080a17c4(*(void **)((u8 *)state + (object_offset + 20)));
    Func_080030f8(1);
    return result;
}
