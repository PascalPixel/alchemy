#include "types.h"
#define NULL ((void *)0)

/*
 * Resource 374 cutscene step at 0x02001084 (496 bytes, 50 call sites).
 *
 * Complete owner and entirely straight-line: prologue
 * `push {r5, r6, lr} ; mov r6, r8 ; push {r6}` at 0x02001084 and the
 * interworking return `pop {r3} ; mov r8, r3 ; pop {r5, r6} ; pop {r0} ;
 * bx r0` at 0x02001240.  r0 holds the popped return address, so the owner is
 * `void`.  Bytes 0x0200124a-0x02001273 are the literal pool (alignment,
 * 0x010b0000, 0x00013333, 0x00009999, 0x00000157, 0x00000ed6, 0x01190000,
 * 0x0000cccc, 0x00006666, 0x00008017, 0x00002018); they are reached only by
 * `ldr rN, [pc, #imm]`.  The next prologue at 0x02001274 is a separate owner,
 * and it is this owner's own last real call.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`, not from the
 * disassembler's `bl` annotations (an overlay `bl` stores target_offset - 2):
 * 41 import veneers plus 9 prologues in this overlay.  Of the prologues,
 * 0x020022c8 and 0x020022e0 are already byte-exact in `assets/code`, and
 * 0x02001274 is the neighbouring owner.
 *
 * Everything the owner does is a fixed sequence of scripted camera, actor and
 * caption commands; the numeric arguments are the shift chains the assembly
 * builds (e.g. `movs r1,#210 ; lsls r1,#15` is 0x00690000) and read as 16.16
 * fixed-point coordinates.
 *
 * Uncertainties:
 *  - the two pool words 0x00008017 and 0x00002018 are handles used both as
 *    Func_0808a180's and Func_020022c8's first argument, the same position in
 *    which the small integers 23 and 24 appear elsewhere in this owner.  Their
 *    namespace is not established here, so they are passed through as
 *    constants;
 *  - Func_0808a210 is reached three times with four arguments and
 *    Func_0808a180 five times with two; one main-image name may still carry
 *    more than one interface, so old-style declarations are used throughout.
 */

/* Overlay-local callees (byte-exact siblings in assets/code). */



/* Neighbouring owner in this overlay. */


/* Main-image imports reached through this overlay's veneer table. */
          /* frame wait */
























extern u8 * Func_02003850();
extern void Func_0200372e();
extern void Func_02003858();
extern void Func_02003696();
extern void Func_02003870();
extern void Func_020036f4();
extern void Func_020036b2();
extern void Func_0200377a();
extern void Func_020038e6();
extern void Func_02003924();
extern void Func_020038e8();
extern void Func_020037fc();
extern void Func_020036da();
extern void Func_020037c4();
extern void Func_02003806();
extern void Func_0200386c();
extern void Func_02003884();
extern void Func_02003962();
extern void Func_020038d4();
extern void Func_020038ec();
extern void Func_020038f8();
extern void Func_020037d6();
extern void Func_02003864();
extern void Func_02003826();
extern void Func_02003862();
extern void Func_02003800();
extern void Func_02003904();
extern void Func_0200388a();
extern void Func_020038a2();
extern void Func_02003472(s32, s32, s32);
extern void Func_020038be();
extern void Func_020038c6();
extern void Func_02003920();
extern void Func_020038f2();
extern void Func_02003488(s32, s32);
extern void Func_020034aa(s32, s32, s32);
extern void Func_0200349e(s32, s32);
extern void Func_020034c0(s32, s32, s32);
extern void Func_0200395c();
extern void Func_02003914();
extern void Func_0200396c();
extern void Func_020034e4(s32, s32, s32);
extern void Func_02003948();
extern void Func_02003988();
extern void Func_02003500(s32, s32, s32);
extern void Func_0200394c();
extern void Func_020038c2();
extern void Func_020034fe(s32, s32);
extern void Func_020024ae(void);
extern void Func_020038e2();
void Func_02001084(void)
{
    u8 *record;
    s32 captionA;
    s32 captionB;

    record = Func_02003850();
    Func_0200372e();

    Func_02003858(-1, -1, -1, 0);
    Func_02003696(1);
    Func_02003870(0x400000, 0x900000, 0x15e00000, 0);
    Func_020036f4();
    Func_020036b2(1);
    Func_0200377a(1, 0);
    Func_020038e6();
    Func_02003924(17);
    Func_020038e8();
    Func_020037fc(23, 0x690000, 0x010b0000);
    Func_020036da(1);
    Func_020037c4(0, 0x00013333, 0x00009999);
    Func_02003806(0, 93, 0x157);
    Func_0200386c(0x0ed6);
    Func_02003884(23, 0);
    Func_02003962(61);

    record[0x55] = 0;

    Func_020038d4(0x30000, 0x6000);
    Func_020038ec(0x6d0000, 0xb00000, 0x01190000, 1);
    Func_020038f8();
    Func_020037d6(40);
    Func_02003864(24, 0x870000, 0xb10000);
    Func_02003826(24, 0x0000cccc, 0x00006666);
    Func_02003862(24, 126, 258);
    Func_02003800(40);
    Func_02003904(23, 0xd000, 0);
    Func_0200388a(24);
    Func_020038a2(24, 1);
    Func_02003472(24, 0x7000, 10);
    Func_020038be(23, 3);
    Func_020038c6(24, 4);

    captionA = 0x8017;
    captionB = 0x2018;

    Func_02003920(captionB, 0);
    Func_020038f2(23, 2);
    Func_02003488(captionA, 30);
    Func_020034aa(24, 0xb000, 20);
    Func_0200349e(captionB, 10);
    Func_020034c0(23, 0xb000, 40);
    Func_0200395c(captionA, 0);
    Func_02003914(24, 4);
    Func_0200396c(captionB, 0);
    Func_020034e4(23, 0xf000, 10);
    Func_02003948(23, 2);
    Func_02003988(captionA, 0);
    Func_02003500(24, 0x6000, 20);
    Func_0200394c(24, 3);
    Func_020038c2(20);
    Func_020034fe(captionB, 20);

    Func_020024ae();
    Func_020038e2();
}
