#include "types.h"

struct Struct3848 {
    u8 pad00[8];
    u32 field08;
    s32 field0c;
    u32 field10;
};

extern u8 Data_02000240[];

extern struct Struct3848 *Func_02003848(s32 arg0);
extern void Func_0200176a(u32 *arg0);

void Func_02000c14(void) {
    u32 buf[3];
    s32 off = 500;
    struct Struct3848 *p = Func_02003848(*(s32 *)(Data_02000240 + off));
    u32 base = p->field08 & 0xfff00000;

    buf[0] = base + 0x80000;
    buf[1] = p->field0c;
    buf[2] = (p->field10 & 0xfff00000) + 0x80000;
    buf[0] = base + 0x280000;
    Func_0200176a(buf);
}
