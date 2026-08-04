#include "types.h"

extern u8 *Func_02001cf2(s32);
extern void Func_02001ccc(s32);
extern void Func_02001cc0(u8 *, s32);
extern s32 Func_02001cde(s32);
extern void Func_02001e10(s32);
extern void Func_020002bc(void);
extern void Func_02001e1a(s32);
extern void Func_02001d00(s32);

void Func_020001b0(void)
{
    u8 *entity;

    entity = Func_02001cf2(10);

    /* 0x200 built as `movs r0,#128 / lsls r0,#2`. */
    Func_02001ccc(0x200);

    if (entity != 0) {
        Func_02001cc0(entity, 0);
        entity[0x23] = 1;
    }

    /* 0x202 is pooled and read twice; the reference loads it fresh into r0 at
     * each site rather than caching it in a callee-saved register, which is
     * what the rerun-cse pass would otherwise do. */
    if (Func_02001cde(0x202) == 0) {
        Func_02001e10(0x9D);
        Func_020002bc();
        Func_02001e1a(0x50);
        Func_02001d00(0x202);
    }
}
