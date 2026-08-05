#include "types.h"

/*
 * Resource 3bc, owner at 0x02000da4 — the overlay's scene entry point.
 * 1480 strict bytes.  Layout inside the row:
 *   0x0da4-0x1141  code
 *   0x1142         alignment halfword
 *   0x1144-0x116b  ten-word literal pool
 *   0x116c-0x121d  code
 *   0x121e         alignment halfword
 *   0x1220-0x1233  the switch's jump table (five words, DATA)
 *   0x1234-0x1361  code
 *   0x1362         alignment halfword
 *   0x1364-0x136b  two-word literal pool
 * The pool map above came from a control-flow walk from the prologue, not
 * from the listing's "pool words referenced" line.  Three further pool words
 * (0x136c/0x1370/0x1374/0x1378) are referenced by this owner but sit past the
 * row's 1480-byte end, in the following inventory row.
 *
 * LINK BASE.  Four independent witnesses here, all consistent with the
 * 0x02008000 base:
 *   - jump-table base pool word 0x02009220 against the table physically at
 *     file offset 0x1220, and its five entries 0x02009234 / 0x020092d6 /
 *     0x02009308 / 0x0200931e / 0x0200933a are file offsets 0x1234 / 0x12d6 /
 *     0x1308 / 0x131e / 0x133a — all EVEN, correct for `mov pc,r3` which does
 *     not interwork;
 *   - 0x02008659 = Func_02000658 + Thumb (that owner is byte-exact in
 *     assets/code/resource_3bc_c_02000658.c);
 *   - 0x0200804d = Func_0200004c + Thumb (the per-frame task, converted as
 *     semantic/overlays/resource_3bc_c_0200004c.c);
 *   - 0x0200a711 = Func_02002710 + Thumb (byte-exact in assets/code).
 * 0x0200cbec is even, hence in-image data at file offset 0x4bec.
 *
 * RETURN TYPE.  The epilogue is `movs r0,#0 / ... / pop {r1} / bx r1` — the
 * popped register is r1, not r0, so r0 survives and IS the result.  The owner
 * returns s32 0 on every path.
 *
 * CALL ACCOUNTING.  104 sites resolved with tools/overlay_call_targets.ts
 * (90 veneers, 14 internal prologues).  The inventory row says calls=64,
 * which is the documented lower bound — the jump table inside the span is
 * exactly the case it undercounts.  Per-target multiset, reproduced by the C
 * below:
 *   080770c8 x4, 080000d0 x3, 080091c0 x15, 0808a080 x21, 080091e0 x5,
 *   080770c0 x10, 0808a100 x4, 080770e0 x1, 080091a8 x1, 08009080 x4,
 *   0808a590 x1, 08009190 x2, 08009098 x1, 0808a158 x1, 02003bd0 x2,
 *   02004494 x1, 080091c8 x1, 0808a088 x9, 080f9010 x1, 02002640 x2,
 *   020025c8 x2, 0808a588 x4, 02003468 x2, 0200457c x1, 02001a0c x1,
 *   02002a50 x1, 02001474 x2, 0808a248 x2  =  104.
 *
 * SHARED CALL SITE.  0x02000e84 is one `bl Func_080091c0` reached from two
 * arms that differ only in registers the arms left set.  Writing a call per
 * arm would inject a phantom into the multiset, so the arguments are hoisted
 * into locals and the site is spelled once, behind `sharedPanel:`.
 *
 * DISPLACEMENT/VALUE TRAP at the very top: `movs r2,#224 / lsls #1` builds the
 * offset 448, and `subs r2,#192` then turns the SAME register into the stored
 * value 32.  The store is workspace+448 = 32, not workspace+256.
 *
 * The 15..17 loop is the same routine the byte-exact
 * assets/code/resource_3bc_c_020005e0.c contains (there Func_02004f26 is this
 * file's Func_080091a8, and the two `Func_080091c0(83, 13, ...)` calls are its
 * Func_02004f60/Func_02004f7a pair with the +52 row offset).  That file also
 * supplies the field names used here.
 *
 * Shift-built constants written as values: 162<<1 = 324, 128<<14 = 0x200000,
 * 128<<11 = 0x40000, 192<<9 = 0x18000, 218<<2 = 872, 128<<10 = 0x20000,
 * 220<<2 = 880, 128<<12 = 0x80000, 216<<2 = 864, 184<<16 = 0xB80000,
 * 249*2 = 498, 225*2 = 450, 192<<16 = 0xC00000, 189<<19 = 0x5E80000,
 * 149<<4 = 2384.
 *
 * Uncertainties:
 *  - Func_02004494 is given seven arguments (r0-r3 plus three stack words at
 *    sp+0/4/8); the stack block is written immediately before the call and
 *    read by nothing else here, but a wider callee frame is not excluded.
 *  - Func_080770e0(880) returning 0 is replaced by 19; whether 19 is a
 *    default row or a default id is not established.
 */

typedef struct Object {
    u8 filler00[8];
    s32 x;              /* 16.16 */
    s32 field0C;
    s32 z;              /* 16.16 */
    u8 filler14[4];
    s32 field18;
    s32 field1C;
    u8 filler20[3];
    u8 mode;            /* +0x23 */
    u8 filler24[0x31];
    u8 state;           /* +0x55 */
} Object;

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;   /* pointer cell holding the scene workspace */
extern u8 Data_0200cbec[];  /* in-image data at file offset 0x4bec */

void Func_080770c8();
void Func_080000d0();
void Func_080091c0();
Object *Func_0808a080();
void Func_080091e0();
s32 Func_080770c0();
void Func_0808a100();
s32 Func_080770e0();
s32 Func_080091a8();
void Func_08009080();
void Func_0808a590();
void Func_08009190();
void Func_08009098();
void Func_0808a158();
void Func_02003bd0();
void Func_02004494();
void Func_080091c8();
void Func_0808a088();
void Func_080f9010();
void Func_02002640();
void Func_020025c8();
void Func_0808a588();
void Func_02003468();
void Func_0200457c();
void Func_02001a0c();
void Func_02002a50();
void Func_02001474();
void Func_0808a248();

s32 Func_02000da4(void)
{
    u8 *workspace;
    s16 *table;
    Object *object;
    s32 slot;
    s32 column;
    s32 row;
    s32 arg0;
    s32 arg1;
    s32 arg4;
    s32 arg5;

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 32;

    Func_080770c8(324);
    Func_080000d0((void *)0x02008659, 0xC80);   /* Func_02000658 + Thumb */
    Func_080091c0(74, 60, 8, 6, 120, 60);

    Func_080091e0(Func_0808a080(9), 0);

    object = Func_0808a080(10);
    Func_080091e0(object, 0);
    object->state = 0;
    object->field0C = 0x200000;

    object = Func_0808a080(11);
    Func_080091e0(object, 0);
    object->field0C = 0x40000;
    object->state = 0;

    if (Func_080770c0(0x362) != 0) {
        Func_0808a100(9, 5);
        object = Func_0808a080(10);
        object->field0C = 0x40000;
        object = Func_0808a080(11);
        object->field0C = 0x200000;
        arg0 = 15;
        arg1 = 12;
        arg4 = 13;
        arg5 = 12;
        goto sharedPanel;
    }

    object = Func_0808a080(9);
    object->field1C = 0x18000;
    if (Func_080770c0(0x367) != 0) {
        arg0 = 0;
        arg1 = 24;
        arg4 = 9;
        arg5 = 12;
        goto sharedPanel;
    }
    object->field18 = 0x18000;
    Func_080091c0(0, 25, 1, 1, 9, 12);
    goto afterPanel;

sharedPanel:
    Func_080091c0(arg0, arg1, 1, 1, arg4, arg5);
afterPanel:

    if (Func_080770c0(872) != 0) {
        Func_080091c0(15, 12, 1, 1, 13, 12);
        Func_080091c0(1, 25, 1, 1, 9, 12);

        object = Func_0808a080(12);
        Func_080091e0(object, 0);
        object->state = 0;
        object->field0C = 0x20000;
        object->mode = 2;

        object = Func_0808a080(10);
        object->field0C = 0x40000;
        object->mode = 2;

        object = Func_0808a080(11);
        object->field0C = 0x200000;
    }

    row = Func_080770e0(880);
    if (row == 0) {
        row = 19;
    }

    object = Func_0808a080(13);
    object->x = (row << 20) + 0x80000;
    object->state = 0;
    object->mode = 2;

    Func_080091c0(18, 10, 3, 1, 18, 11);
    Func_080091c0(17, 11, 1, 1, row, 11);

    for (slot = 15; slot <= 17; slot++) {
        object = Func_0808a080(slot);
        if (object->field0C == 0
         && Func_080091a8(0, object->x, object->z) == 0) {
            object->mode = 2;
            object->state = 0;
            Func_080091c0(83, 13, 1, 1, object->x >> 20, object->z >> 20);
            Func_080091c0(83, 13, 1, 1,
                          object->x >> 20, (object->z >> 20) + 52);
        }
    }

    if (Func_080770c0(0x361) != 0) {
        column = 33;
        for (slot = 18; slot <= 22; slot++) {
            object = Func_0808a080(slot);
            object->mode = 2;
            Func_08009080(object, 2);

            object = Func_0808a080(slot + 5);
            object->mode = 2;
            object->state = 0;
            object->field0C = 0x200000;
            Func_08009080(object, 10);

            Func_080091c0(74, 12, 1, 1, column, 11);
            column += 2;
        }
        Func_0808a100(28, 10);
        Func_0808a590(28);
    } else {
        for (slot = 18; slot <= 22; slot++) {
            object = Func_0808a080(slot);
            object->mode = 2;

            object = Func_0808a080(slot + 5);
            object->mode = 2;
            object->state = 0;
            object->field0C = 0x200000;
        }
        Func_080000d0((void *)0x0200804D, 0xC80);   /* Func_0200004c + Thumb */
    }

    if (Func_080770c0(864) != 0) {
        Func_0808a100(29, 4);
        Func_080091c0(47, 61, 1, 4, 49, 61);
    }

    if (Func_080770c0(0x363) != 0) {
        Func_08009190(1);
        object = Func_0808a080(30);
        object->state = 0;
        object->x = 0x046A0000;
        object->z = 0xB80000;
        Func_080091e0(object, 0);
        Func_08009080(object, 3);
        Func_08009098(object, Data_0200cbec);
    } else {
        Func_08009190(2);
    }

    if (Func_080770c0(0x369) != 0) {
        object = Func_0808a080(31);
        Func_08009080(object, 8);
        object->mode = 2;
        Func_080091c0(86, 10, 1, 2, 84, 10);
        Func_080091c0(86, 9, 1, 1, 84, 12);
    } else {
        object = Func_0808a080(31);
        Func_080091c0(85, 9, 1, 4, object->x >> 20, 9);
        Func_080091c0(85, 9, 1, 4, object->x >> 20, 61);
    }

    object->state = 0;
    object->mode = 2;
    object = Func_0808a080(9);
    object = Func_0808a080(10);
    object->state = 0;
    object->mode = 2;
    object = Func_0808a080(11);
    object->state = 0;
    object->mode = 2;

    Func_0808a100(8, 9);

    table = Data_02000240;
    ((u8 *)table)[498] = 0;

    Func_02003bd0(39, 3);
    Func_02003bd0(40, 17);
    Func_0808a158(8, 2);

    switch ((u32)(table[225] - 1)) {
    case 0:
        Func_02004494(0, 8, 6, 0x5E80000, 0xC00000, 39, 40);
        Func_080091c8(127, 0, 1, 2, 5, 2);
        Func_0808a088(32);
        Func_0808a088(33);
        Func_0808a088(34);
        Func_0808a088(35);
        Func_0808a088(36);
        Func_0808a088(37);
        Func_0808a088(38);
        if (Func_080770c0(0x109) == 0) {
            Func_080f9010(17);
            Func_02002640(0);
            Func_020025c8();
            Func_0808a588(1, 0);
            Func_02003468(3);
        }
        Func_0808a588(1, 0);
        Func_0808a588(2, 0);
        Func_0808a588(3, 0);
        Func_0200457c(0xE6);
        break;

    case 1:
        Func_080000d0((void *)0x0200A711, 0xC80);   /* Func_02002710 + Thumb */
        Func_0808a088(39);
        Func_0808a088(40);
        if (Func_080770c0(0x109) != 0) {
            break;
        }
        Func_020025c8();
        Func_02002640(1);
        Func_02003468(0);
        break;

    case 2:
        if (Func_080770c0(0x109) != 0) {
            break;
        }
        Func_02001a0c(32);
        Func_02002a50();
        break;

    case 3:
        Func_02001474(1);
        Func_0808a248(4);
        Func_080770c8(2384);
        Func_080770c8(0x951);
        break;

    case 4:
        Func_02001474(-1);
        Func_0808a248(5);
        Func_080770c8(2384);
        break;

    default:
        break;
    }

    return 0;
}
