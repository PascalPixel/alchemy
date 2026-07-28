#include "types.h"

struct SharedWork080a23c0 {
    u8 padding_00[0x10];
    s32 resource;
};

extern s32 Func_080150a8(s32, s32, s32, s32, s32);
extern void Func_08015080(s32, s32, s32, s32);
extern struct SharedWork080a23c0 Data_02000240;

void Func_080a23c0(s32 value) {
    Func_080150a8(Data_02000240.resource, 7, value, 8, 0);
    Func_08015080(0xB0B, value, 0x40, 0);
}
