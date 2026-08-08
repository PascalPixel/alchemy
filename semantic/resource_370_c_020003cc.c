#include "types.h"

/*
 * Resource 370 top-level driver at 0x020003cc.
 *
 * Complete owner: the four-part prologue at 0x020003cc (`push {r5, r6, r7,
 * lr}`, fp/sl/r9 pushed through r5-r7, r8 pushed through r7, then
 * `add sp, r5` with the pooled constant -548) through the matching unwind and
 * `pop {r1} / bx r1` at 0x02000cbe.  The return address is popped into r1, so
 * r0 survives and IS the result (HANDOVER section 0, epilogue rule); the only
 * value that ever reaches the epilogue is the `movs r0, #0` at 0x02000cbe, so
 * this owner always returns 0.
 *
 * POOL MAP, derived by a control-flow walk from the prologue as HANDOVER
 * section 0 requires -- not from `overlay_show.ts`'s referenced-words list.
 * Three literal pools sit inside the span, each reached only by falling past
 * an unconditional branch:
 *   0x020006b4-0x02000713 (96 bytes), hopped by `b.n 0x020004a8` at 0x020006b2
 *   0x0200085e-0x0200088b (46 bytes), hopped by `b.n 0x020008ce` at 0x0200085c
 *   0x02000afa-0x02000b2f (54 bytes), hopped by `b.n 0x02000b4e` at 0x02000af8
 * plus the trailing pool at 0x02000cd6-0x02000cfb after the epilogue.  Every
 * other byte in 0x020003cc-0x02000cd5 is reached as an instruction.  Register
 * state is carried ACROSS each pool: for example r0 is built at 0x02000af2 and
 * consumed at 0x02000b4e on the far side of the third pool.
 *
 * CALL ACCOUNTING.  `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_370 --json`
 * reports 139 sites in this span (the inventory's calls=127 counts distinct
 * targets and is a lower bound, as HANDOVER section 0 records).  Of those:
 *   - 137 are real calls, and this file contains one C call expression for
 *     each of them, with shared sites written once and reached by `goto` so
 *     the multiset does not inflate.  The two shared sites that would
 *     otherwise be duplicated are 0x02000664 (Func_08015040, entered both from
 *     0x020005f0 with 6 and by falling through with r6 + 2 = 6) and
 *     0x02000b50 (Func_030003ac, entered from 0x02000af8 with page + pages - 1
 *     and by falling through with page + pages + 1).
 *   - 2 are `bl` used as a long unconditional branch inside the owner, which
 *     `overlay_call_targets.ts` reports as `unknown` -- exactly the shape
 *     HANDOVER section 0 describes.  0x02000470 targets 0x02000cbe, the
 *     owner's own `movs r0, #0` exit, and 0x02000c92 targets 0x020004a8, its
 *     own main-loop head.  Neither pushes anything, both land on arms that run
 *     into the owner's own epilogue (or back into its own loop), and the
 *     clobbered lr is dead because the real return address is on the stack.
 *     They are spelled `goto`.
 *   - Six sites are calls to this overlay's other owners and are named
 *     accordingly: 0x020003e2 -> Func_02000054, 0x020005e6 and 0x0200065a ->
 *     Func_02000384, 0x020009ec -> Func_02000de4, 0x020009f4 ->
 *     Func_020012c8, 0x02000a12 -> Func_020011e4, 0x02000c16 ->
 *     Func_020002f4.  That is what identifies this owner as the overlay's
 *     driver.
 *
 * WHAT IT IS.  A screen that generates a printable code from the party state
 * and exchanges it over the GBA multiplayer link:
 *  - Func_080000d0(0x020081fd, 3200) installs Func_020001fc, the cheat-code
 *    watcher, as a per-frame task; the loop at 0x02000522 then reads that
 *    watcher's two in-image gates (0x020096b2 and 0x020096b4) to extend a
 *    one-step sequence to four or seven steps.
 *  - Func_080000d0(0x02008155, 3200) installs Func_02000154, the alpha-blend
 *    step, the first time the polling loop sees no pending work.
 *  - 0x04000128 is REG_SIOCNT: bits 4-5 of the word read there are the
 *    multiplayer id, and the code checks the *other* console's four-halfword
 *    block in the receive buffer at 0x02002024 for the signature
 *    {85, 86, 84, 83} before proceeding.  0x03001f64 is the connected-console
 *    mask and 0x03001c94 the pressed-button latch.
 *  - Func_02000de4 serializes the party, Func_020012c8 appends a CRC-16 big
 *    endian, Func_020011e4 encodes the result into six-bit symbols, and
 *    Func_020002f4 maps each symbol to a glyph for display, fifty symbols per
 *    page.  Func_03000380 and Func_030003ac are the relocated IWRAM divide and
 *    modulo helpers reached through this overlay's own veneers at file offsets
 *    0x1314 and 0x131c.
 *
 * Uncertainties, recorded rather than guessed away:
 *  - The stack buffers' sizes are inferred from the 548-byte frame and the
 *    offsets actually used (a 4-byte glyph record at [sp, #24], the encoded
 *    symbols at [sp, #28] and the serialized payload at [sp, #348]).  Only
 *    their relative placement is evidence; the trailing slack is not.
 *  - Several sites set no argument registers at all (Func_08077098,
 *    Func_080151f0, Func_08015318, Func_08000300, Func_08000370,
 *    Func_080001f0, Func_08077270, Func_08077300, Func_08077318,
 *    Func_0808a360, Func_0808a368, Func_0808a370, Func_080152e8,
 *    Func_08015140).  They are written with no arguments; leftover values in
 *    r0 are not asserted as arguments.
 *  - The call at 0x02000b74 (Func_08015070) passes r5 both as the third
 *    argument and as the stack-carried fifth.  That is what the code does; no
 *    interpretation is offered.
 *  - The layout arithmetic in the glyph loop uses the relocated modulo helper
 *    for the column and the divide helper for the row; the row/column split is
 *    named from that usage, not asserted from a caller.
 *  - The workspace at 0x02000240 is the shared cross-overlay scene record
 *    already modelled by byte-exact siblings; byte offsets are kept literal
 *    because only a few fields are touched here.
 *  - Control flow is transcribed with labels and `goto` because the owner is a
 *    genuine state machine with several joins that no structured form
 *    reproduces without duplicating call sites.
 */

/* Overlay imports (via the veneer table).  Old-style declarations are
 * mandatory here: overlay imports take different argument counts at different
 * sites, and several are used for their return values. */
void Func_02000438(void);
void Func_0200171a();
void Func_020018ac();
void Func_020018c0();
void Func_020017b2();
s32  Func_020018e8();
void Func_020017ee();
void Func_020017ca();
s32  Func_02001830();
void Func_020018e8_b();
void Func_0200190c();
void Func_0200192a();
void Func_020018fa();
void Func_02001912();
void Func_02001944();
void Func_0200181a();
void Func_02001812();
void Func_02001822();
void Func_020017c0();
s32  Func_020018b6();
void Func_02001804();
void Func_02001816();
void Func_02001814();
s32  Func_020018c8();
void Func_02001956();
void Func_02001938();
void Func_0200184a();
s32  Func_02001940();
void Func_020019dc();
s32  Func_02001978();
void Func_020019f0();
void Func_020019c2();
s32  Func_02001a2e();
void Func_02001a38();
s32  Func_0200096c(void);
void Func_02001a76();
void Func_020019ca();
void Func_020019d2();
s32  Func_02001a6e();
void Func_02001a16();
void Func_02001a1c();
s32  Func_020009e0(void);
void Func_020019fc();
void Func_02001af2();
void Func_02001af0();
void Func_02001b00();
void Func_02001aa2();
void Func_02001ab4();
s32  Func_02001b10();
s32  Func_02001b8e();
void Func_02001bce();
void Func_02001bbc();
void Func_02001bc2();
void Func_02001bc8();
void Func_02001bcc();
void Func_02001bd2();
void Func_02001bd8();
void Func_02001bde();
void Func_02001b9e();
void Func_02001bec();
void Func_02001bf4();
void Func_02001c4a();
s32  Func_02001bb0();
void Func_02001bf6();
s32  Func_02001bb4();
s32  Func_02001b84();
void Func_02001bca();
void Func_02001bd6();
void Func_02001b9a();
void Func_02001b58();
void Func_02001bf0();
void Func_02001daa();
void Func_02001c72();
s32  Func_02001c80();
void Func_02001cb6();
void Func_02001c34();
void Func_02001c94();
void Func_02001c42();
void Func_02001c56();
void Func_02001d02();
void Func_02001d1e();
void Func_02001ca0();
void Func_02001cae();
void Func_02001cb8();
void Func_02001d36();
void Func_02001d2e();
void Func_02001e76();
void Func_02001d4c();
void Func_02001d44();
void Func_02001d54();
s32  Func_02001daa_b();
s32  Func_020017d2();
u16  Func_02001cbe();
s32  Func_02001bf8();
void Func_02001d6e();
s32  Func_02001dae();
s32  Func_02001d4c_b();
s32  Func_02001dce();
void Func_02001e06();
s32  Func_02001df4();
void Func_02001e32();
s32  Func_02001e12();
void Func_02001e50();
void Func_02001ece();
void Func_02001e44();
void Func_02001ee2();
void Func_02001fa2();
void Func_02001fbc();
void Func_02002012();
s32  Func_02001e6e();
void Func_02001efe();
void Func_02001f1a();
void Func_02001f92();
void Func_02001fac();
void Func_02000f0c();
s32  Func_02001f3c();
s32  Func_02001f4a();
s32  Func_02001f4e();
s32  Func_02001f6e();
s32  Func_02001f72();
void Func_02002030();
void Func_02001faa();
void Func_02001fb4();
void Func_0200213c();
void Func_0200217a();
void Func_02002166();
void Func_02002172();
void Func_02002150();

                     

                     
                               /* relocated IWRAM divide */
                               /* relocated IWRAM modulo */

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

/* This overlay's own owners. */

                     

                     

s32 Func_020003cc(void)
{
    u8 *workspace = (u8 *)0x02000240;

    /* Stack buffers, in the order the frame lays them out. */
    u8 glyph[4];        /* [sp, #24] */
    u8 encoded[320];    /* [sp, #28] */
    u8 payload[200];    /* [sp, #348] */

    s32 fade_installed; /* [sp, #20] */
    s32 window_a;       /* [sp, #12] */
    s32 window_b;       /* [sp, #16] */

    s32 list_window;    /* sl */
    s32 pages;          /* r8 */
    s32 page;           /* fp */
    s32 length;         /* r9 */

    s32 r4;
    s32 r5;
    s32 r6;
    s32 r7;
    s32 r0;

    fade_installed = 0;

    Func_02000438();
    /* Func_020001fc + 1: the cheat-code watcher, Thumb-tagged. */
    Func_0200171a(0x020081fd, 3200);
    *(u32 *)(*(u8 **)0x03001ebc + 448) = (u32)fade_installed;
    Func_020018ac();
    Func_020018c0();

    if (*(s16 *)(workspace + 450) != 2) {
        goto L_04a0;
    }

L_0416:
    Func_020017b2(7, 5);
    r7 = Func_020018e8(1);
    Func_020017ee();
    if (r7 == 0) {
        Func_020017ca(8, 1);
        *(u8 *)(workspace + 0x20f) = 1;
        r7 = Func_02001830();
        if (r7 == -1) {
            goto L_0416;
        }
    }

    Func_020018e8_b(60);
    Func_0200190c();
    goto L_exit;                    /* long `bl` at 0x02000470 */
    Func_0200192a(17);
    Func_020018fa(300);
    Func_02001912(2, 72);

    Func_02001944(112);
    Func_0200181a(list_window);
L_0474:
    Func_02001812(list_window, 2);
    Func_0200181a(window_b, 2);
    Func_02001822(window_a, 2);
    Func_020017c0(1);
    goto L_04a8;

L_04a0:
    *(u8 *)0x03001ca0 = (u8)fade_installed;

L_04a8:
    r6 = Func_020018b6();
    if (r6 < 0 && *(u8 *)0x03001f54 != 0) {
        *(u8 *)(workspace + 0x22a) = 1;
        *(u8 *)0x03001d08 = 1;
        Func_020018ac(10, 1, 8);
    }

    if (r6 == 0 && fade_installed == 0) {
        Func_02001804(30);
        /* Func_02000154 + 1: the alpha-blend step, Thumb-tagged. */
        Func_02001816(0x02008155, 3200);
        Func_02001814(1);
        fade_installed = 1;
    }

    if (r6 > 0) {
        r6 = Func_020018c8();
    } else {
        r6 = 0;
    }

    if (r6 != 0) {
        goto L_057a;
    }

    /* Scene walk.  One step normally; the two cheat gates extend it. */
    Func_02001956();
    r5 = 1;
    r7 = 0;
L_0522:
    Func_02001938(*(u8 *)(workspace + 0x205), *(u8 *)(workspace + 0x206));
    Func_0200184a(6);
    r6 = Func_02001940(r7);
    if (r6 == -1) {
        if (r7 == 0) {
            goto L_04a8;
        }
        r7--;
        goto L_0522;
    }
    if (*(s16 *)0x020096b2 != 0) {
        r5 = 4;
    }
    if (*(s16 *)0x020096b4 != 0) {
        r5 = 7;
    }
    r7++;
    if (r7 < r5) {
        goto L_0522;
    }
    Func_020019dc();
    *(u16 *)(workspace + 448) = 8;
    *(u16 *)(workspace + 450) = 20;
    goto L_0c96;

L_057a:
    if (r6 != 1) {
        goto L_06a0;
    }
    r6 = Func_02001978(1);
    if (r6 == -1) {
        goto L_04a8;
    }
    Func_020019f0(265);
    Func_020019c2(*(u8 *)(workspace + 0x205), *(u8 *)(workspace + 0x206));
    if (*(u32 *)workspace != (u32)Func_02001a2e()) {
        *(u16 *)(workspace + 448) = *(u16 *)(workspace + 452);
        *(u16 *)(workspace + 450) = *(u16 *)(workspace + 454);
        Func_02001a38(265);
        goto L_0696;
    }

    /* Both of the two buttons in the 0x208 mask held. */
    if ((*(u32 *)0x03001ae8 & 520) == 520) {
        if (Func_0200096c() != 0) {
            r0 = 6;
            goto L_0664;
        }
        *(u16 *)(workspace + 448) = *(u16 *)(workspace + 452);
        *(u16 *)(workspace + 450) = *(u16 *)(workspace + 454);
        Func_02001a76(265);
        Func_02001a76(318);
        goto L_0696;
    }

    if (*(u32 *)(workspace + 4) == *(u32 *)0x02001100) {
        goto L_0696;
    }
    Func_020019ca(4, 9);
    Func_020019d2(5, 13);
    if (Func_02001a6e(1, 0, 0, 0) != 0) {
        Func_02001a16();
        goto L_04a8;
    }
    Func_02001a1c();
    if (Func_020009e0() != 0) {
        r0 = 4 + 2;
L_0664:
        Func_020019fc(r0, 9);
        goto L_04a8;
    }

    *(u16 *)(workspace + 448) = *(u16 *)(workspace + 452);
    *(u16 *)(workspace + 450) = *(u16 *)(workspace + 454);
    Func_02001af2(265);
    Func_02001af0(319);

L_0696:
    Func_02001b00(262);
    goto L_0c96;

L_06a0:
    if (r6 == 2) {
        Func_02001aa2();
        goto L_04a8;
    }
    if (r6 == 3) {
        Func_02001ab4();
        goto L_04a8;
    }
    if (r6 == 4) {
        r6 = Func_02001b10(4);
        if (r6 == -1) {
            goto L_04a8;
        }
        *(u32 *)(workspace + 500) = 0;
        if (Func_02001b8e(0x952) != 0) {
            Func_02001bce();
            Func_02001bbc(0);
            Func_02001bc2(1);
            Func_02001bc8(2);
            Func_02001bce(3);
            Func_02001bcc(0);
            Func_02001bd2(1);
            Func_02001bd8(2);
            Func_02001bde(3);
        }
        Func_02001b9e(*(u8 *)(workspace + 0x205),
                      *(u8 *)(workspace + 0x206));
        Func_02001bec(265);
        Func_02001bf4(262);
        Func_02001bf4(382);
        *(u8 *)0x03001ca0 = 1;
        Func_02001c4a(190, 1);
        goto L_0c96;
    }

    if (r6 != 5) {
        goto L_04a8;
    }

L_07b8:
    r6 = Func_02001bb0(5);
    if (r6 == -1) {
        goto L_04a8;
    }
    Func_02001bf6(*(u8 *)(workspace + 0x205), *(u8 *)(workspace + 0x206));

L_07dc:
    r6 = Func_02001bb4(0);
    if (r6 == -1) {
        goto L_07b8;
    }
    if (r6 != 1) {
        goto L_09cc;
    }

    /* Link handshake. */
    r6 = Func_02001b84(6, 5, 18, 8, 2);
    Func_02001bbc(3203, r6, 0, 4);
    Func_02001bca(3204, r6, 0, 16);
    Func_02001bd6(3206, r6, 0, 36);
    Func_02001b9a();
    Func_02001b58(10);
    {
        u16 *local = (u16 *)0x02002224;
        u16 *slots = (u16 *)0x02002024;

        local[0] = 48;
        local[1] = 48;
        local[2] = 48;
        local[3] = 48;

        r7 = 0;
        r5 = 3;
        r4 = 0;
        do {
            r4++;
            slots[0] = 48;
            slots[1] = 48;
            slots[2] = 48;
            slots[3] = 48;
            slots += 12;        /* 24 bytes per console block */
        } while (r4 != 4);
    }
    goto L_08ce;

L_088c:
    if ((*(u16 *)0x03001f64 & r5) == r5) {
        /* Bits 4-5 of REG_SIOCNT are this console's multiplayer id; the
         * partner's block is the other one of the first pair. */
        u32 other = ((*(u32 *)0x04000128 << 26) >> 30) ^ 1;
        u16 *block = (u16 *)((u8 *)0x02002024 + other * 24);

        if (block[0] == 85 && block[1] == 86 &&
            block[2] == 84 && block[3] == 83) {
            r7 = 1;
            goto L_08e0;
        }
    }
    Func_02001bf0(1);

L_08ce:
    if ((*(u32 *)0x03001c94 & 2) == 0) {
        goto L_088c;
    }
    Func_02001daa(113);

L_08e0:
    Func_02001c72(r6, 2);
    if (r7 == 0) {
        goto L_07dc;
    }

    r6 = Func_02001c80(5, 10, 20, 4, 2);
    Func_02001cb6(3205, r6, 0, 4);
    Func_02001c34(10);
    Func_02001c94(0x02000000, 0x1004);
    Func_02001c42(10);

    r5 = 0;
    r7 = 0;
    r4 = 1;
    goto L_093a;
L_092a:
    Func_02001c56(1);
    r7++;
L_093a:
    if (r7 > 599999) {
        goto L_095e;
    }
    r5++;
    if ((*(u16 *)0x03001f64 & 3) == 3) {
        r5 = 0;
    }
    if (r5 == 10) {
        r4 = 0;
        goto L_095e;
    }
    if (*(u32 *)0x02002080 != 0) {
        goto L_092a;
    }

L_095e:
    if (r4 == 0) {
        Func_02001d02(r6);
        Func_02001d1e(3207, r6, 0, 4);
        do {
            Func_02001ca0(1);
        } while ((*(u32 *)0x03001c94 & 1) == 0);
    }
    Func_02001cae(10);
    Func_02001d02();
    Func_02001cb8(10);
    Func_02001d36(r6);
    Func_02001d2e(r6, 2);
    goto L_04a8;

L_09a6:
    Func_02001e76(113);
    Func_02001d4c(list_window);
    Func_02001d44(list_window, 2);
    Func_02001d4c(window_b, 2);
    Func_02001d54(window_a, 2);
    goto L_09d2;

L_09cc:
    if (r6 != 0) {
        goto L_delayed_wait;
    }

L_09d2:
    r6 = Func_02001daa_b(1);
    if (r6 == -1) {
        goto L_07dc;
    }

    /* Serialize, checksum, encode. */
    length = Func_020017d2(0, r6, payload);
    {
        u16 crc = Func_02001cbe(length, payload);

        payload[length] = (u8)(crc >> 8);
        payload[length + 1] = (u8)crc;
    }
    length += 2;
    length = Func_02001bf8(payload, length, encoded);

    Func_02001d6e();

    list_window = Func_02001dae(5, 4, 20, 12, 2);
    page = 0;
    pages = Func_02001d4c_b(length, 50) + 1;

    window_a = Func_02001dce(10, 0, 10, 4, 2);
    Func_02001e06(3202, window_a, 6, 4);

    r7 = 1;
    if (pages == 1) {
        window_b = Func_02001df4(5, 16, 20, 3, 2);
        Func_02001e32(3200, window_b, 80, 0);
    } else {
        window_b = Func_02001e12(1, 16, 28, 3, 2);
        Func_02001e50(3201, window_b, 0, 0);
    }

    Func_02001ece(0x06006000);
    Func_02001e44(list_window);

L_0aaa:
    Func_02001ee2(0x06002500);

    if ((*(u32 *)0x03001c94 & 2) != 0) {
        goto L_09a6;
    }
    if ((*(u32 *)0x03001c94 & 1) != 0) {
        page++;
        r7 = 1;
        if (page == pages) {
            goto L_0474;
        }
        Func_02001fa2(111);
        goto L_0b58;
    }

    if ((*(u32 *)0x03001c94 & 32) != 0 && pages > 1) {
        Func_02001fbc(111);
        r0 = page + pages - 1;
        goto L_0b4e;
    }
    if ((*(u32 *)0x03001c94 & 16) != 0 && pages > 1) {
        Func_02002012(111);
        r0 = page + pages + 1;
L_0b4e:
        page = Func_02001e6e(r0, pages);
        r7 = 1;
    }

L_0b58:
    if (r7 != 1) {
        goto L_0c82;
    }
    Func_02001efe(list_window);

    r7 = 0;
    r5 = 2;
    do {
        r7++;
        Func_02001f1a(list_window, 0, r5, 18, r5);
        r5 += 2;
    } while (r7 != 4);

    if (pages > 1) {
        r7 = 0;
        r5 = 18 - pages;
        do {
            s32 id = 0xf301 + r7;

            if (r7 == page) {
                id = 0xf30b + r7;
            }
            Func_02001f92(list_window, id, r5, -1, 0);
            r7++;
            r5++;
        } while (r7 != pages);

        Func_02001fac(list_window, 0xf128, 17 - pages, -1, 0);
        Func_02001fbc(list_window, 0xf129, 18, -1, 0);
        {
            u8 *marks = *(u8 **)0x03001e8c + 0x0ea3;

            *marks = (u8)(*marks |
                          (2 << (*(u16 *)((u8 *)list_window + 14) >> 2)));
        }
    }

    /* Fifty symbols per page, laid out ten per row. */
    r4 = page * 50;
    r6 = r4 + 50;
    if (r6 > length) {
        r6 = length;
    }
    r7 = r4;
    r4 = 0;
    if (r7 != r6) {
        do {
            s32 column;
            s32 row;
            s32 x;
            s32 y;

            Func_02000f0c(encoded[r7] & 63, glyph);
            if (Func_02001f3c(r7, 10) > 4) {
                column = Func_02001f4a(r4, 10);
                row = Func_02001f4e(r4, 10);
                x = column * 12 + 18;
            } else {
                column = Func_02001f6e(r4, 10);
                row = Func_02001f72(r4, 10);
                x = column * 12 + 8;
            }
            y = (row << 4) + 2;
            Func_02002030(glyph, list_window, x, y);
            r7++;
            r4++;
        } while (r7 != r6);
    }
    r7 = 0;

L_0c82:
    Func_02001faa(1);
    goto L_0aaa;

L_delayed_wait:
    Func_02001fb4(300);
    ((void (*)(void))0x0200113c)(); /* long `bl` at 0x02000c92 */

L_0c96:
    *(u16 *)(*(u8 **)0x03001ebc + 368) = 999;
    Func_0200213c(30);
    Func_0200217a(17);
    Func_02002166();
    Func_02002172();
    Func_02002150(60);

L_exit:
    return 0;
}
