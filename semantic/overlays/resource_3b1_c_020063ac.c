typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b1 unindexed helper at 0x020063ac (116 bytes, 2 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}` plus `fp`/`r9`/`r8` spill at 0x020063ac, epilogue
 * `pop {r3,r5,r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 / pop {r5,r6,r7} /
 * pop {r0} / bx r0` at 0x02006412-0x0200641e. No literal pool for this
 * owner itself -- the bytes immediately after (0x02006420 onward) are a
 * `ldr r4,[pc,#0] / bx r4` ARM-interworking veneer table into main ROM
 * (targets like 0x080000c1), a recognized structural category the
 * item-28 scan's prologue test correctly does not flag, so the span is
 * exactly 0x020063ac-0x02006420, 116 bytes.
 *
 * Two first-match linear scans (index 0..8 inclusive, each breaking out
 * on the first hit and calling a per-element handler), then two more
 * calls that deliberately cross-pair each scan's *miss count* against
 * the *other* array: `count_a` (from scanning `a`) indexes into `b`, and
 * `count_b` (from scanning `b`) indexes into `a`.
 *
 * Raw callee naming.
 */

s32 Func_0200c876();
void Func_0200c890();
s32 Func_0200c896();
void Func_0200c8b0();
void Func_0200c8b8();

void Func_020063ac(u8 *a, u8 *b)
{
    s32 count_a = 0;
    s32 count_b = 0;
    s32 i;

    for (i = 0; i <= 8; i++) {
        u8 *elem = a + i;
        if (Func_0200c876(elem) != 0) {
            Func_0200c890(elem);
            break;
        }
        count_a++;
    }

    for (i = 0; i <= 8; i++) {
        u8 *elem = b + i;
        if (Func_0200c896(elem) != 0) {
            Func_0200c8b0(elem);
            break;
        }
        count_b++;
    }

    Func_0200c8b0(b + count_a);
    Func_0200c8b8(a + count_b);
}
