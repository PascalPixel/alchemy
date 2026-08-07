#include "types.h"

/*
 * Resource 372 scene setup at 0x020028a4 (1156 bytes, 109 distinct call
 * targets across 120 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x020028a4, matching
 * `pop {r5} / pop {r0} / bx r0` at 0x02002cb8.  `pop {r0} ; bx r0` — r0 is the
 * popped return address, so the owner returns nothing.
 * 0x02002cbe-0x02002d27 is alignment plus the literal pool, reached only by
 * `ldr rN, [pc, #imm]`.
 *
 * `out/decomp/overlays.json` seeds three further rows inside this span
 * (0x020028d8, 0x020028ec, 0x02002abc), all marked `contained_by` this entry.
 * None of them is a function: each is a plain `movs` in the middle of an
 * argument block with this owner's `push {r5, lr}` already executed.  They are
 * artefacts of the encoded-`bl` reading discussed below, and the owner is
 * reconstructed whole from its own prologue to its own epilogue.
 *
 * Structure: build handles 9, 10, 22, 24, 25, 26; set three actors' frame
 * counters from a queried value; then two nested two-way script branches, and
 * the usual optional position fix-up through `*(s16 *)(p + 10)` /
 * `*(s16 *)(p + 18)`.
 *
 * Three field idioms here are proven by byte-exact siblings, so they are not
 * re-derived: `*(u16 *)(obj + 100) = query(...) + 60` with the constant 90
 * (0x5a) as the second argument is exactly the `+0x64` / `0x5A` / `+60`
 * sequence of `assets/code/resource_372_c_02000434.c`; `Data_0200cec8` is the
 * table that file installs into the same kind of object; and the two-field
 * descriptor copy `place(handle, p->f08, p->f10)` is from
 * `assets/code/resource_372_c_0200173c.c`.
 *
 * Message identifiers are carried in r5 and advanced in place: 0xe9b (+4, +5)
 * for the first branch and 0xea1 (+1, +2) for the second.
 *
 * Several imports are reached with different argument counts at different
 * sites: 0x0200714e (four then two), 0x02007124 (one at both, different
 * handles), 0x02007176, 0x02007320, 0x02007328, 0x0200732e, 0x0200734a,
 * 0x02007356, 0x0200737a and 0x020073a4.  Old-style declarations are therefore
 * mandatory.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is an import identity for this overlay, not a place to
 * disassemble.
 *
 * Uncertainties: handles, message ids and the 16.16-looking constants are
 * unresolved; the two branch predicates 0x020072aa(0, 0) and 0x02007304(0, 0)
 * are tested against zero exactly as written.
 */

/* Imports, old-style: interfaces unknown, several with varying arity. */
void Func_020070ac();
void Func_020070b8();
void Func_020070c4();
void Func_020070d0();
void Func_020070dc();
void Func_0200707c();
void Func_02007084();
void Func_0200708c();
void Func_02007094();
void Func_020070a4();
void Func_02007002();
void Func_02007136();
void Func_0200714e();
void Func_02007070();
void Func_0200707a();
void Func_0200708a();
void Func_020070aa();
void Func_02007208();
void Func_02006fea();
void Func_02007058();
void Func_020070ea();
void Func_02007104();
void Func_020070fe();
void Func_02007232();
void Func_0200718e();
void Func_0200719a();
void Func_020071a6();
void Func_02007146();
void Func_02007156();
u8 *Func_020070f4();
s32 Func_02007022();
s32 Func_02007008();
u8 *Func_0200710c();
s32 Func_0200703a();
s32 Func_02007020();
u8 *Func_02007124();
s32 Func_02007052();
s32 Func_02007038();
void Func_02007158();
void Func_02007160();
void Func_02007168();
void Func_02007176();
void Func_02007124_b();
void Func_020072ea();
void Func_020070cc();
void Func_0200713a();
void Func_020072f6();
void Func_0200727e();
void Func_02007314();
void Func_020070f6();
void Func_02007164();
void Func_02007320();
void Func_020071c0();
void Func_020071ce();
void Func_02007184();
void Func_0200734a();
void Func_0200712c();
void Func_020071f2();
void Func_02007356();
void Func_020071a4();
void Func_0200728c();
void Func_020072ae();
void Func_02007266();
void Func_020072c0();
void Func_02007270();
void Func_02007280();
void Func_020071de();
void Func_02007236();
void Func_0200723e();
void Func_020071f4();
void Func_02007328();
void Func_02007340();
void Func_0200726e();
void Func_02007344();
void Func_0200732e();
void Func_020072e0();
void Func_02007350();
void Func_0200737a();
s32 Func_020072aa();
void Func_0200736c();
void Func_0200737c();
void Func_02007394();
void Func_020073b8();
void Func_020073d4();
void Func_020073a4();
void Func_020073b4();
s32 Func_02007304();
void Func_02007388();
void Func_020073c6();
void Func_020073e8();
void Func_02007404();
void Func_020073ac();
void Func_020073bc();
void Func_0200731a();
void Func_0200743e();
void Func_02007448();
void Func_020073d8();
void Func_0200741e();
void Func_02007436();
void Func_0200739e();
void Func_02007354();
void Func_0200742e();
void Func_02007364();
void Func_0200740c();
void Func_0200741c();
void Func_02007422();
u8 *Func_020073c0();
void Func_0200740a();
void Func_02007430();
void Func_02007442();

                     

                     

                     

                     

                    

                     

                     

                    

                     

/* In-image data blocks. */
extern u8 Data_0200cab4[];
extern u8 Data_0200ca78[];
extern u8 Data_0200d7a0[];
extern u8 Data_0200cec8[];
extern u8 Data_0200d7e2[];
extern u8 Data_0200d78a[];
extern u8 Data_0200cb28[];
extern u8 Data_0200cb9c[];
extern u8 Data_0200d7cc[];
extern u8 Data_0200cc0c[];
extern u8 Data_0200cc5c[];
extern u8 Data_0200cca8[];

void Func_020028a4(void)
{
    u8 *obj;
    s16 *pos;
    s32 msg;

    Func_020070ac(26, 0x3000, 0);      /* 192 << 6 */
    Func_020070b8(24, 0xd000, 0);      /* 208 << 8 */
    Func_020070c4(25, 0xb000, 0);      /* 176 << 8 */
    Func_020070d0(9, 0x3000, 0);
    Func_020070dc(10, 0xd000, 20);
    Func_0200707c(26, 3);
    Func_02007084(24, 3);
    Func_0200708c(25, 3);
    Func_02007094(9, 3);
    Func_020070a4(25, 3);
    Func_02007002(20);
    Func_02007136(0x10000, 0x2000);    /* 128<<9, 128<<6 */
    Func_0200714e(0x860000, -1, 0x4ab0000, 1);   /* 134 << 16 */
    Func_02007070(26, 0x19999, 0xcccc);
    Func_0200707a(9, 0x19999, 0xcccc);
    Func_0200708a(26, Data_0200cab4);
    Func_020070aa(9, Data_0200ca78);
    Func_02007208(158);
    Func_02006fea(Data_0200d7a0, 38, 72);
    Func_02007058(10);
    Func_020070ea(9, 149, 0x497);
    Func_02007104(9, 0, 0);
    Func_020070fe(25, 250, 0x4be);
    Func_02007232();
    Func_0200718e(10, 0x3000, 0);
    Func_0200719a(24, 0x3000, 0);
    Func_020071a6(25, 0x3000, 0);
    Func_02007146(10, 5);
    Func_0200714e(24, 6);              /* two arguments here */
    Func_02007156(25, 6);

    /* Three copies of the byte-exact "+0x64 = query(...) + 60" idiom. */
    obj = Func_020070f4(10);
    *(u16 *)(obj + 100) = (u16)(Func_02007008(Func_02007022(), 90) + 60);
    obj = Func_0200710c(24);
    *(u16 *)(obj + 100) = (u16)(Func_02007020(Func_0200703a(), 90) + 60);
    obj = Func_02007124(25);
    *(u16 *)(obj + 100) = (u16)(Func_02007038(Func_02007052(), 90) + 60);

    Func_02007158(10, Data_0200cec8);
    Func_02007160(24, Data_0200cec8);
    Func_02007168(25, Data_0200cec8);
    Func_02007176(26);
    Func_02007124_b(10);                 /* one argument here */
    Func_020072ea(159);
    Func_020070cc(Data_0200d7e2, 38, 72);
    Func_0200713a(30);
    Func_020072f6();
    Func_0200727e(0x700000, -1, 0x4c90000, 1);   /* 224 << 15 */
    Func_02007314(158);
    Func_020070f6(Data_0200d78a, 35, 73);
    Func_02007164(20);
    Func_02007320();
    Func_020071c0(9, Data_0200cb28);
    Func_02007176(20);
    Func_020071ce(26, Data_0200cb9c);
    Func_02007184(40);
    Func_0200734a(159);
    Func_0200712c(Data_0200d7cc, 35, 73);
    Func_020071f2(26);
    Func_02007356();
    Func_020071a4(40);

    msg = 0xe9b;
    Func_0200728c(msg);
    Func_020072ae(9, 0, 20);
    Func_02007266(26, 3);
    Func_020072c0(0x201a, 0, 40);
    Func_02007270(9, 3);
    Func_02007280(26, 3);
    Func_020071de(30);
    Func_02007236(9, Data_0200cc0c);
    Func_0200723e(26, Data_0200cc5c);
    Func_020071f4(40);
    Func_02007328(0x20000, 0x4000);              /* 128<<10, 128<<7 */
    Func_02007340(0x690000, -1, 0x43e0000, 1);   /* 210 << 15 */
    Func_0200726e(9);
    Func_02007328(9, 0, 0);                      /* three arguments here */
    Func_02007344(9, 0x100, 40);                 /* 128 << 1 */
    Func_0200732e(9, 0, 10);
    Func_0200734a(0, 0x8000, 0);                 /* 128 << 8 */
    Func_02007356(22, 0x8000, 10);
    Func_020072e0(9, 105, 0x43e);
    Func_02007320(9, 2);
    Func_02007350(0x8009, 0);
    Func_0200737a(22, 0, 0);

    if (Func_020072aa(0, 0) == 0) {
        Func_0200732e(9, 3);                     /* two arguments here */
        Func_0200736c(msg + 4);
    } else {
        Func_02007356(9, 2);
        Func_0200737c(msg + 5);
    }

    Func_02007394(0x8009, 0);
    Func_020073b8(22, 0x8000, 40);
    Func_020073d4(9, 0x100, 30);

    Func_020073a4(msg);
    msg = 0xea1;
    Func_020073b4(0x8009, 0);

    if (Func_02007304(0, 0) == 0) {
        Func_02007388(9, 3);
        Func_020073c6(msg + 1);
        Func_020073e8(0x8009, 0, 30);
        Func_02007404(22, 0x8000, 20);
        Func_020073a4(0, 3);                     /* two arguments here */
        Func_020073ac(22, 3);
        Func_020073bc(9, 3);
        Func_0200731a(40);
    } else {
        Func_0200743e(9, 0x105, 90);
        Func_02007448(9, 0x103, 40);
        Func_020073d8(9, 4);
        Func_0200741e(msg + 2);
        Func_02007436(0x8009, 0);
    }

    Func_0200739e(9, Data_0200cca8);
    Func_02007354(90);
    Func_0200742e(0, 22, 0);
    Func_02007364(40);
    Func_0200740c(0, 3);
    Func_0200741c(22, 3);
    Func_0200737a(20);                           /* one argument here */
    Func_02007422(22, 2);

    pos = Func_020073c0(0);
    if (pos != 0) {
        Func_0200740a(22, pos[5], pos[9]);       /* +10 and +18, signed */
    }

    Func_02007430(22);
    Func_02007442(22, 0, 0);
}
