#include "types.h"
#define NULL ((void *)0)

/*
 * Resource 374 scene dispatcher at 0x02000940 (636 bytes, 53 call sites).
 *
 * Complete owner: prologue `push {r5, lr} ; sub sp, #8` at 0x02000940 and the
 * interworking return `movs r0, #0 ; add sp, #8 ; pop {r5} ; pop {r1} ;
 * bx r1` at 0x02000b80.  The popped return register is r1, not r0, so by the
 * epilogue rule r0 survives and IS the result: the owner returns the constant
 * 0.  Bytes 0x02000b8a-0x02000bbb are the literal pool (alignment,
 * 0x0000090b, 0x0000090c, 0x0000090d, 0x02000240, 0x0000087a, 0x00000834,
 * 0x0200ac00, 0x00000815, 0x00004ccc, 0x00009999, 0x014b0000, 0x00000842),
 * reached only by `ldr rN, [pc, #imm]`.  The next prologue at 0x02000bbc is a
 * separate owner.
 *
 * The eight-byte stack frame is not a local variable area: it carries the
 * fifth and sixth arguments of the two six-argument imports Func_08009180 and
 * Func_080091c0 (`str r3, [sp, #0]` / `str r2, [sp, #4]` immediately before
 * each call), which is why they are spelled with six arguments here.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`, never from the
 * disassembler's `bl` annotations: 49 import veneers and 4 prologues in this
 * overlay.  The four prologues are the scene bodies this dispatcher selects —
 * 0x020017c8, 0x020015b4, 0x02001084 (all in this reconstruction) and the byte-exact
 * 0x020025b0 — and they are the reason the printed targets (0x02002284,
 * 0x02002100, 0x02001bea, 0x02002f62) look like unrelated addresses.
 *
 * Link base 0x02008000: the pool word 0x0200ac00 is file offset 0x2c00, an
 * even in-image address, so it is data (a table passed to Func_0808a168) and
 * not a Thumb entry point.  0x02000240 is below the base and is the same
 * EWRAM progress table `semantic/overlays/resource_371_c_0200008c.c` reads;
 * this owner reads its signed halfword element 225 (byte offset 450) three
 * separate times, reloading it after each block that can change it.
 *
 * Control flow note: the `case 97` arm of the first dispatch is a direct
 * `b.n 0x02000b4a` into the middle of the later `state == 19` arm, so both
 * reach Func_020015b4 and then the common return.  That join is kept as a
 * `goto` rather than duplicated, because the two paths are literally the same
 * instructions.
 *
 * Uncertainties: Func_080770c0 is the story-flag query used the same way by
 * the byte-exact siblings and by 0x02000248; Func_0808a0f0 is reached nine
 * times with three arguments and Func_0808a080 seven times with one, so
 * old-style declarations are used for every import.
 */

/* EWRAM progress table (signed halfword elements). */
extern s16 Data_02000240[];

/* In-image data under the 0x02008000 link base. */
extern u8 Data_02002c00[];

/* Scene bodies selected by this dispatcher. */
s32 Func_02002fbc();
void Func_0200306a();
s32 Func_02002fd0();
void Func_0200307e();
s32 Func_02002fe4();
void Func_02003092();
void Func_02003022();
void Func_02003178();
void Func_02002f62(void);
u8 *Func_0200307e_b();
u8 *Func_0200308a();
u8 *Func_02003092_b();
s32 Func_0200304a();
void Func_02003030();
void Func_02003044();
void Func_02003038();
void Func_02002ff6();
s32 Func_02003086();
void Func_02003206();
void Func_02003212();
s32 Func_020030ae();
void Func_0200315c();
void Func_02003166();
void Func_020031ba();
s32 Func_020030da();
void Func_0200318c();
void Func_020030ce();
void Func_020030e2();
void Func_020030d6();
void Func_02003094();
void Func_02002284(void);
s32 Func_02003138();
u8 *Func_02003192();
u8 *Func_0200319e();
void Func_02003134();
u8 *Func_020031aa();
void Func_02003210();
s32 Func_02003170();
void Func_02003220();
u8 *Func_020031d6();
void Func_0200316c();
s32 Func_02003194();
void Func_02003242();
void Func_0200324c();
void Func_02002100(void);
s32 Func_020031c8();
s32 Func_020031d2();
void Func_02001bea(void);
s32 Func_020031e2();
void Func_0200338a();
void Func_0200339e();

                         

/* Main-image imports reached through this overlay's veneer table. */
                               /* frame wait */
                     
                               /* six arguments, last two stack-carried */
                               /* six arguments, last two stack-carried */
                     
                               /* story-flag query */

                     

                     

                     

s32 Func_02000940(void)
{
    s16 state;
    u8 *record;

    s32 permuted_7;
    if (Func_02002fbc(0x90b) != 0) Func_0200306a(8, 0, 0);
    if (Func_02002fd0(0x90c) != 0) Func_0200307e(9, 0, 0);
    if (Func_02002fe4(0x90d) != 0) Func_02003092(10, 0, 0);

    permuted_7 = Data_02000240[225];
    if (state == 98) {
        Func_02003022(32);
        Func_02003178(50);
        return 0;
    }
    state  = permuted_7;
    if (state == 99) {
        Func_02002f62();
        return 0;
    }
    if (state == 97) goto scene_15b4;

    record = Func_0200307e_b(8);
    *(s32 *)(record + 28) = 0x18000;
    record = Func_0200308a(9);
    *(s32 *)(record + 28) = 0x18000;
    record = Func_02003092_b(10);
    *(s32 *)(record + 28) = 0x18000;

    if (Func_0200304a(0x87a) != 0) {
        Func_02003030(97, 2, 80, 5, 2, 2);
        Func_02003044(42, 53, 42, 54, 3, 1);
        Func_02003038();
        Func_02002ff6(1);
        return 0;
    }

    if (Func_02003086(0x834) != 0) {
        Func_02003206();
        Func_02003212();
        Func_0200307e(21, 38, 1, 1, 18, 41);
        if (Func_020030ae(0x840) != 0) {
            Func_0200315c(17, 0, 0);
            Func_02003166(18, 0, 0);
            Func_020031ba(19, 0x10000, Data_02002c00);
        }
    } else if (Func_020030da(0x815) != 0) {
        Func_0200318c(16, 0x00b40000, 0x02380000);
        Func_020030ce(92, 2, 80, 5, 2, 2);
        Func_020030e2(42, 53, 42, 54, 3, 1);
        Func_020030d6();
        Func_02003094(1);
    }

    state = Data_02000240[225];
    if (state == 12) {
        Func_02002284();
        return 0;
    }

    if (Func_02003138(0x834) != 0) {
        record = Func_02003192(20);
        *(s32 *)(record + 28) = 0x4ccc;
        Func_02003134(Func_0200319e(20), 0);
        *(s32 *)(record + 24) = 0x4ccc;
        *(s32 *)(record + 24) = 0x9999;
        *(s32 *)(record + 28) = 0x9999;
        record = Func_020031aa(21);
        Func_02003210(13, 5);
    } else if (Func_02003170(0x815) != 0) {
        Func_02003220(21, 0x014b0000, 0x00f90000);
        Func_0200316c(Func_020031d6(21), 0);
    }

    if (Func_02003194(0x840) != 0) {
        Func_02003242(26, 0, 0);
        Func_0200324c(22, 0, 0);
    }

    state = Data_02000240[225];
    if (state == 19) {
scene_15b4:
        Func_02002100();
        return 0;
    }

    if (Func_020031c8(0x834) != 0 && Func_020031d2(0x842) != 0) {
        Func_02001bea();
        return 0;
    }

    if (Func_020031e2(0x834) != 0) {
        Func_0200338a();
        Func_0200339e();
        Func_0200338a();
    }

    return 0;
}
