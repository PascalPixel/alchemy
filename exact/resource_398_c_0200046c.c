#include "types.h"

/*
 * Resource 398 map-variant selector at 0x0200046c.
 *
 * Complete owner: `push {lr}` at 0x0200046c and the matching
 * `pop {r1} / bx r1` at 0x0200049e.  72-byte row: 54 bytes of code, an
 * alignment halfword at 0x020004a2, and four pool words (0x02000240,
 * 0x00000031, 0x00000030, 0x0000002f) filling 0x020004a4-0x020004b3.  The pool
 * map is from a control-flow walk: every branch in the body targets
 * 0x02000486, 0x02000492 or 0x0200049c, and nothing reaches 0x020004a2.
 *
 * Signature.  The return address is popped into r1, not r0, so r0 survives and
 * is the result — and `movs r0,#0` immediately precedes the pop, so the owner
 * returns 0.  Same shape as the tracked byte-exact assets/code/
 * resource_398_c_02000030.c, which is written `s32 Func_02000030(...)
 * { ...; return 0; }`.
 *
 * ROOT.  This is entry 0 of the exported-entry veneer table at image offset 0:
 * `ldr r4,[pc,#0] / bx r4 / .word 0x0200846d`, which is 0x0200_046c + the Thumb
 * bit under the 0x02008000 link base.  Working the call graph root-down from
 * here reaches 0x020004b4, 0x020004e8 and 0x02000538 and nothing else.
 *
 * The selector itself is the cross-overlay Data_02000240 idiom: the signed
 * halfword at byte offset 448 (element 224), branched on.  The tracked
 * byte-exact assets/code/resource_398_c_02000040.c reads the same halfword and
 * tests it against the same three values, spelling them `(s32)&Value_00000031`
 * and so on — that is the exact reconstruction's constant-pooling device, and the plain
 * integers below are the same numbers.
 *
 * Three call sites, matching the row's advertised count.  All three are
 * overlay-local prologues, not veneers.
 */

extern s16 Data_02000240[];
extern unsigned char Value_00000031;
extern unsigned char Value_00000030;
extern unsigned char Value_0000002f;

void Func_02000936(void);
void Func_02000976(void);
void Func_020009d2(void);

s32 Func_0200046c(void)
{
    s16 variant = Data_02000240[224];

    if (variant == (s32)&Value_00000031) {
        Func_02000936();
    } else if (variant == (s32)&Value_00000030) {
        Func_02000976();
    } else if (variant == (s32)&Value_0000002f) {
        Func_020009d2();
    }
    return 0;
}
