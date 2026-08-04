typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 38b scene entry at 0x020008f0 (220 bytes, 16 call sites over 8
 * distinct callees).
 *
 * Complete owner: `push {lr}` at 0x020008f0 and `pop {r0} / bx r0` at
 * 0x020009b0 — the popped r0 is the return address, so the owner is `void`.
 * 0x020009b4-0x020009cb is the literal pool; note that every one of its six
 * words is a small integer flag id or a pointer, and the pool is reached only
 * by `ldr rN,[pc,...]`, never as code.
 *
 * Reached from the dispatcher at 0x02000890 for scene id 0x1e.
 *
 * COMPLETENESS.  Sites placed = 16, against the inventory's `calls=15`.  That
 * is the documented direction of error: HANDOVER section 0 records the
 * `calls` field as systematically LOW because it predates the corrected `bl`
 * decoding, and states the check as "placed >= row count", not equality.  The
 * per-target multiset is: Func_080770c0 x4, Func_0808a0f0 x4,
 * Func_0808a080 x2, Func_0808a1b8 x2, Func_080091e0 x1, Func_080770d0 x1,
 * Func_080770c8 x1, Func_020009cc x1 = 16, and each is written exactly that
 * many times below.  No arm shares a `bl` with another, so none of the five
 * inflating shared-call-site shapes applies here.
 *
 * Fifteen sites are import veneers; the sixteenth, 0x02000990, is this
 * overlay's own prologue at 0x020009cc — a real function start reached as an
 * ordinary call, not a hidden-context target and not a long branch to this
 * owner's epilogue (it is forward of the epilogue and returns).
 *
 * The scene-state halfword at byte offset 450 of Data_02000240 (index 225) is
 * the same sub-scene selector that 0x02000d10 tests.
 */

/* Cross-overlay scene-state halfword array. */
extern s16 Data_02000240[];

/* Progress-flag accessors: c0 tests, c8 sets, d0 clears. */
extern s32 Func_080770c0();
extern void Func_080770c8();
extern void Func_080770d0();

/* Imports. */
extern u8 *Func_0808a080();
extern void Func_0808a0f0();
extern void Func_0808a1b8();
extern void Func_080091e0();

/* This overlay's own follow-on step. */
extern void Func_020009cc(void);

void Func_020008f0(void)
{
    u8 *record;
    s16 sub_scene;

    if (Func_080770c0(0x845) != 0) {
        Func_0808a0f0(9, 0, 0);
        /* 192 << 6 = 0x3000, 160 << 7 = 0x5000. */
        Func_0808a1b8(14, 0x3000, 0);
        Func_0808a1b8(15, 0x5000, 0);
    } else {
        record = Func_0808a080(9);
        Func_080091e0(record, 0);
        Func_0808a0f0(21, 0, 0);
    }

    /* 192 << 9 = 0x18000. */
    record = Func_0808a080(8);
    *(s32 *)(record + 28) = 0x18000;

    sub_scene = *(s16 *)((u8 *)Data_02000240 + 450);
    if (sub_scene == 10) {
        Func_0808a0f0(8, 0, 0);
    } else if (sub_scene == 9) {
        Func_080770d0(0x12f);
    }

    if (Func_080770c0(0x109) == 0) {
        /* Reloaded from memory rather than reused — a side effect of the
         * calls above would be observed here. */
        sub_scene = *(s16 *)((u8 *)Data_02000240 + 450);
        if (sub_scene == 11) {
            /* 248 << 16 and 216 << 16: 16.16 coordinates. */
            Func_0808a0f0(20, 0x00f80000, 0x00d80000);
        }
    }

    Func_020009cc();

    if (Func_080770c0(0x84a) != 0) {
        if (Func_080770c0(0x84b) == 0) {
            /* 193 << 2 = 772. */
            Func_080770c8(772);
        }
    }
}
