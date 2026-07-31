typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3ad, owner at 0x020001b0 (80 bytes, 8 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x020001b0 and the interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x020001f6.  The final popped register is
 * r0, so it holds the return address and the owner is `void`.  One pool word
 * (0x202) sits at 0x020001fc, past the return.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`:
 *   0x020001b4 -> veneer 0x02001b3c -> Func_0808a080
 *   0x020001be -> veneer 0x02001b0c -> Func_080770c8
 *   0x020001ca -> veneer 0x02001af4 -> Func_080091e0
 *   0x020001d8 -> veneer 0x02001b04 -> Func_080770c0
 *   0x020001e2 -> veneer 0x02001c2c -> Func_080f9010
 *   0x020001e6 -> 0x020000d4 (this overlay's own prologue)
 *   0x020001ec -> veneer 0x02001c2c -> Func_080f9010
 *   0x020001f2 -> veneer 0x02001b0c -> Func_080770c8
 * Per-target multiset: Func_080f9010 x2, Func_080770c8 x2, and one each of
 * Func_0808a080, Func_080091e0, Func_080770c0, Func_020000d4 - 8 sites against
 * 8 C call expressions, matching the row's calls=8.
 *
 * r5 is the only saved register and holds the entity record across the
 * Func_080770c8(0x200) call, which is why the null test happens after it.
 */

/* Old-style declarations: overlay import arities vary per call site. */
u8 *Func_0808a080();    /* scene entity record by selector */
void Func_080091e0();
s32 Func_080770c0();    /* test a story flag (used in a condition) */
void Func_080770c8();   /* set a story flag */
void Func_080f9010();
void Func_020000d4();   /* this overlay */

void Func_020001b0(void)
{
    u8 *entity;

    entity = Func_0808a080(10);

    /* 0x200 built as `movs r0,#128 / lsls r0,#2`. */
    Func_080770c8(0x200);

    if (entity != 0) {
        Func_080091e0(entity, 0);
        entity[0x23] = 1;
    }

    /* 0x202 is the pooled word at 0x020001fc, read twice. */
    if (Func_080770c0(0x202) == 0) {
        Func_080f9010(0x9d);
        Func_020000d4();
        Func_080f9010(0x50);
        Func_080770c8(0x202);
    }
}
