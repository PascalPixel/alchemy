#include "types.h"

/*
 * Flag-driven actor reset owner at 0x02005324, complete 332-byte span through
 * its sole return, alignment and eight-word pool before 0x02005470.  All 29
 * call sites, six flag gates and the original nullable/non-nullable actor
 * assumptions remain explicit ordinary C.
 */

extern void Func_02000c78(void);
extern void Func_0200226c(void);
extern void Func_020022a0(void);
extern void Func_020022d4(void);
extern void Func_020054dc(void);
extern void Func_080091e0(void *record, s32 mode);
extern void Func_08009258(s32 value);
extern s32 Func_080770c0(s32 flag);
extern void Func_080770c8(s32 flag);
extern void Func_080770d0(s32 flag);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a0f0(s32 actor, s32 x, s32 z);

void Func_02005324(void)
{
    u8 *state = *(u8 **)0x03001ebc;
    u8 *record;

    *(s32 *)(state + 0x1c0) = 0x200;
    Func_020054dc();

    if (Func_080770c0(0x943) != 0)
        Func_02000c78();

    Func_080770c8(0x217);
    Func_080770c8(0x218);

    if (Func_080770c0(0x944) != 0) {
        Func_0808a0f0(8, 0, 0);
        Func_080770d0(0x217);
    }
    if (Func_080770c0(0x945) != 0) {
        Func_0808a0f0(9, 0, 0);
        Func_020022d4();
    }
    if (Func_080770c0(0x946) != 0) {
        Func_0808a0f0(10, 0, 0);
        Func_080770d0(0x218);
    }
    if (Func_080770c0(0x947) != 0)
        Func_0200226c();
    if (Func_080770c0(0x948) != 0)
        Func_020022a0();

    Func_0808a018();

    record = Func_0808a080(8);
    if (record != 0)
        record[0x23] = 2;

    record = Func_0808a080(9);
    if (record != 0)
        record[0x23] = 2;

    record = Func_0808a080(10);
    if (record != 0)
        record[0x23] = 2;

    record = Func_0808a080(11);
    if (record != 0)
        Func_080091e0(record, 0);
    record[0x23] = 2;

    record = Func_0808a080(12);
    if (record != 0)
        record[0x59] |= 0x10;

    record = Func_0808a080(11);
    Func_080091e0(record, 0);
    Func_0808a020();
    Func_08009258(224 << 4);
}
