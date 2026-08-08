#include "types.h"

/*
 * resource_3bb owner at 0x02000bd4, 1276 bytes (0x02000bd4-0x020010cf):
 * code 0x02000bd4-0x02000f85, two alignment bytes, the five-word jump table
 * at 0x02000f88, the eleven-word literal pool at 0x02000f9c, and the case
 * bodies 0x02000fc8-0x020010cf with the shared epilogue at 0x020010c0.  The
 * pool words at 0x020010d0-0x020010db that the last cases load sit just past
 * the row.
 *
 * Prologue `push {r5, r6, r7, lr} / mov r7, sl / mov r6, r8 / push {r6, r7} /
 * sub sp, #12`; epilogue `movs r0, #0 / add sp, #12 / pop {r3, r5} /
 * mov r8, r3 / mov sl, r5 / pop {r5, r6, r7} / pop {r1} / bx r1`.  The
 * interworking return pops into r1, not r0, so r0 survives the epilogue and
 * IS the result - and it is set to 0 immediately before, on every path.  The
 * owner therefore returns a constant 0.
 *
 * The jump table's base pool word is 0x02008f88 and the table physically sits
 * at file offset 0x0f88, which is the cheapest confirmation that this overlay
 * is linked at 0x02008000.  Two more witnesses appear in the body:
 * 0x02008715 = Func_02000714 + 1, the task Func_020008ec also installs, and
 * 0x02009c79 = Func_02001c78 + 1.
 *
 * All 88 `bl` sites are placed and reach 22 distinct callees.  The inventory
 * row records calls=54; that count predates the corrected branch decoding.
 * Targets come from cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- (target offset = stored
 * displacement + 2), never from the disassembler's annotations, which print
 * every repeated callee as a separate function.  The imports reached through
 * the veneer table are Func_080770c8, Func_080091c0, Func_0808a080,
 * Func_08009080, Func_080770c0, Func_0808a100, Func_080770e0, Func_0808a590,
 * Func_080000d0, Func_080003b8, Func_0808a158, Func_080091c8, Func_0808a088,
 * Func_080f9010, Func_0808a588 and Func_0808a248; the calls into this
 * overlay's own code go to Func_0200062c, Func_02003138, Func_020039fc,
 * Func_02001ba8, Func_02001b30, Func_020029d0, Func_02003ae4, Func_020010dc,
 * Func_02001fb8 and Func_02000b38.
 *
 * Shape: the map-entry routine.  It clears the workspace word at +448, draws
 * the two header lines, resets participants 26-30, then reads five story
 * flags (816, 0x335, 0x333, 0x331, 0x332) to decide which optional lines and
 * participants are present.  Participants 12, 13 and 14 are placed from the
 * counters 832, 840 and 848, each defaulting to 73, 76 and 79 when the
 * counter is zero, and the summary block Func_0200062c is drawn.  Flag 820
 * then selects between a labelled arrangement of participants 22-25 and a
 * moving one that installs the per-frame task Func_02000714 under key 0xc85.
 * Finally the byte at 0x02000432 is cleared, two pairings are made, and the
 * halfword at 0x02000402 selects one of five scene entries through the jump
 * table; a value outside 1..5 falls straight through to the return.
 *
 * Uncertainties: only the record fields at +8, +12, +16, +35, +48, +52 and
 * +85 are asserted.  0x02000240 is below the link base and is a RAM global,
 * so its +450 selector and +498 byte are RAM, unlike the 0x0200_8xxx pool
 * words above.  The three counter defaults (73, 76, 79) are applied when
 * Func_080770e0 returns zero, which is a "not yet recorded" reading rather
 * than a proven one.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_02004ae2();
void Func_02004a8e();
void Func_02004aa2();
u8 *Func_02004b78();
void Func_02004a78();
u8 *Func_02004b9a();
s32 Func_02004b40();
u8 *Func_02004bb2();
void Func_02004b10();
void Func_02004b24();
u8 *Func_02004bf4();
void Func_02004af4();
u8 *Func_02004c08();
s32 Func_02004bb4();
void Func_02004b6c();
s32 Func_02004bd2();
void Func_02004c8e();
void Func_02004b92();
s32 Func_02004bf8();
u8 *Func_02004c6a();
u8 *Func_02004c76();
void Func_02004ccc();
void Func_02004bd0();
s32 Func_02004c36();
u8 *Func_02004ca8();
u8 *Func_02004cb4();
void Func_02004d0a();
void Func_02004c0e();
u8 *Func_02004cdc();
void Func_02004c3a();
u8 *Func_02004d08();
void Func_02004c66();
s32 Func_02004ce6();
u8 *Func_02004d44();
void Func_02004ca8_b();
s32 Func_02004d28();
u8 *Func_02004d86();
void Func_02004ce8();
s32 Func_02004d68();
u8 *Func_02004dc6();
void Func_02004d28_b();
void Func_020014d0(void);
void Func_02004e44();
s32 Func_02004d9c();
u8 *Func_02004e16();
void Func_02004d1e();
void Func_02004d72();
void Func_02004e90();
void Func_02004f9e();
u8 *Func_02004e60();
void Func_02004d68_b();
void Func_02004cde();
void Func_02004d76();
void Func_02004ede();
void Func_02004092();
void Func_0200409a();
void Func_02004f32();
void Func_020049de();
void Func_02004e86();
void Func_02004f54();
void Func_02004f5a();
void Func_02004f60();
void Func_02004f66();
void Func_02004f6c();
void Func_02004f72();
s32 Func_02004f08();
void Func_020050da();
void Func_02002bd8();
void Func_02002b64(void);
void Func_02003a0a();
void Func_020050e2();
void Func_020050ea();
void Func_02004b3c();
void Func_02004e0c();
void Func_02004fc2();
void Func_02004fc8();
s32 Func_02004f5e();
void Func_02002bae(void);
void Func_02002c2c();
void Func_02003a5a();
s32 Func_02004f7a();
void Func_02002178();
void Func_02003058(void);
void Func_02001be0(s32);
void Func_020050de();
void Func_02001bf0(s32);
void Func_020050ee();

                    

                     

                     

                     

                     

/* This overlay's own routines. */

                     

                     

                         
                        
void Func_02000714();
void Func_02001c78();

s32 Func_02000bd4(void)
{
    u8 *workspace;
    u8 *record;
    s32 id;
    s32 label;
    s32 value;

    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 448) = 0;              /* 224 << 1 */

    Func_02004ae2(324);                         /* 162 << 1 */
    Func_02004a8e(14, 11, 12, 4, 100, 11);
    Func_02004aa2(48, 10, 5, 6, 120, 10);

    do {
        record = Func_02004b78(id);
        Func_02004a78(record, 4);
        record[85] = 0;
        *(s32 *)(record + 12) = 0;
        record[35] = 2;
        id = id + 1;
    } while (id <= 30);
    id = 26;

    record = Func_02004b9a(18);
    record[35] = 2;

    if (Func_02004b40(816) != 0) {              /* 204 << 2 */
        record = Func_02004bb2(30);
        *(s32 *)(record + 8) = 0x2d00000;       /* 168 << 17 */
        *(s32 *)(record + 12) = 0xfff80000;
        *(s32 *)(record + 16) = 0x2280000;      /* 132 << 17 */
        Func_02004b10(19, 16, 1, 1, 20, 16);
        Func_02004b24(20, 80, 1, 1, 21, 80);
    } else {
        record = Func_02004bf4(30);
        Func_02004af4(record, 3);
        *(s32 *)(record + 12) = 0x100000;       /* 128 << 13 */
    }

    record = Func_02004c08(11);
    record[35] = 2;

    if (Func_02004bb4(0x335) != 0) {
        Func_02004b6c(35, 78, 1, 1, 35, 77);
    }

    if (Func_02004bd2(0x333) != 0) {
        Func_02004c8e(19, 4);
        Func_02004b92(32, 37, 1, 4, 32, 77);
    }

    if (Func_02004bf8(0x331) != 0) {
        Func_02004c6a(20)[85] = 0;
        Func_02004c76(20)[35] = 2;
        Func_02004ccc(20, 5);
        Func_02004bd0(46, 17, 1, 1, 44, 17);
    }

    if (Func_02004c36(0x332) != 0) {
        Func_02004ca8(21)[85] = 0;
        Func_02004cb4(21)[35] = 2;
        Func_02004d0a(21, 5);
        Func_02004c0e(46, 17, 1, 1, 50, 17);
    }

    record = Func_02004cdc(32);
    record[35] = 2;
    record[85] = 0;
    Func_02004c3a(52, 28, 1, 3, value, 10);
    value = *(s32 *)(record + 8) >> 20;

    value = *(s32 *)(record + 8) >> 20;
    record = Func_02004d08(33);
    record[85] = 0;
    record[35] = 2;
    Func_02004c66(52, 28, 1, 3, value, 13);

    value = Func_02004ce6(832);                 /* 208 << 2 */
    if (value == 0) value = 73;
    record = Func_02004d44(12);
    *(s32 *)(record + 8) = (value << 20) + 0x80000;
    record[85] = 0;
    record[35] = 2;
    Func_02004ca8_b(71, 16, 1, 1, value, 16);

    value = Func_02004d28(840);                 /* 210 << 2 */
    if (value == 0) value = 76;
    record = Func_02004d86(13);
    *(s32 *)(record + 8) = (value << 20) + 0x80000;
    record[85] = 0;
    record[35] = 2;
    Func_02004ce8(71, 16, 1, 1, value, 16);

    value = Func_02004d68(848);                 /* 212 << 2 */
    if (value == 0) value = 79;
    record = Func_02004dc6(14);
    *(s32 *)(record + 8) = (value << 20) + 0x80000;
    record[85] = 0;
    record[35] = 2;
    Func_02004d28_b(71, 16, 1, 1, value, 16);

    Func_020014d0();
    Func_02004e44(31, 10);

    if (Func_02004d9c(820) != 0) {              /* 205 << 2 */
        id = 22;
        label = 58;
        do {
            record = Func_02004e16(id);
            record[35] = 2;
            Func_02004d1e(record, 4);
            Func_02004d72(56, 13, 1, 1, label, 13);
            id = id + 1;
            label = label + 2;
        } while (id <= 25);
        Func_02004e90(31, 10);
        Func_02004f9e(31);
    } else {
        id = 22;
        do {
            record = Func_02004e60(id);
            record[35] = 2;
            Func_02004d68_b(record, 4);
            *(s32 *)(record + 48) = 0x8000;     /* 128 << 8 */
            *(s32 *)(record + 52) = 0x3333;
            id = id + 1;
        } while (id <= 25);

        /* Tasks are published as their entry address plus the Thumb bit. */
        Func_02004cde((s32)Func_02000714 + 1, 0xc85);
        Func_02004d76((s32)Func_02000714 + 1, 1);
    }

    Func_02004ede(8, 9);

    *(u8 *)(0x02000240 + 498) = 0;              /* 249 << 1 */

    Func_02004092(41, 89);
    Func_0200409a(40, 77);
    Func_02004f32(8, 1);

    /* The selector is 1-based; anything outside 1..5 falls through. */
    value = *(s16 *)(0x02000240 + 450) - 1;     /* 225 << 1 */
    if ((u32)value > 4) return 0;

    switch (value) {
    case 0:
        Func_020049de(0, 8, 5, 0x680000, 0x1000000, 40, 41);  /* 208<<15, 128<<17 */
        Func_02004e86(127, 0, 1, 2, 79, 6);
        Func_02004f54(34);
        Func_02004f5a(35);
        Func_02004f60(36);
        Func_02004f66(37);
        Func_02004f6c(38);
        Func_02004f72(39);
        if (Func_02004f08(0x109) == 0) {
            Func_020050da(17);
            Func_02002bd8(0);
            Func_02002b64();
            Func_02003a0a(2);
        }
        Func_020050da(1, 0);
        Func_020050e2(2, 0);
        Func_020050ea(3, 0);
        Func_02004b3c(0xe5);
        break;

    case 1:
        Func_02004e0c((s32)Func_02001c78 + 1, 3200);          /* 200 << 4 */
        Func_02004fc2(40);
        Func_02004fc8(41);
        if (Func_02004f5e(0x109) != 0) break;
        Func_02002bae();
        Func_02002c2c(1);
        Func_02003a5a(0);
        break;

    case 2:
        if (Func_02004f7a(0x109) != 0) break;
        Func_02002178(34);
        Func_02003058();
        break;

    case 3:
        Func_02001be0(2);
        Func_020050de(4);
        break;

    case 4:
        Func_02001bf0(-2);
        Func_020050ee(5);
        break;
    }

    return 0;
}
