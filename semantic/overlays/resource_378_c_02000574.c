typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 378 owner at 0x02000574 (88 bytes, through 0x020005cb).
 *
 * Eight imported calls and one conditional branch are present.  The two
 * literal words at 0x020005c4--0x020005cb are part of this owner; the next
 * entry-driver prologue begins at 0x020005cc.  The sole state write is the
 * same workspace +472 halfword increment used by the preceding owner.
 *
 * STILL-OPEN residual, 4 bytes at 0x02000590: the reference sets the fifth
 * call's arguments as `movs r1,#0` before `movs r0,#10`; this source emits
 * them the other way.  candidate_explain.ts shows the pair ties through
 * every rank_for_schedule tier (priority, class, dependent count) and is
 * decided by insn creation order, with arg0's setter created first -- so no
 * -fsched-* flag can flip it, -fthumb-call-arg1-before-arg0 provably cannot
 * fire (it only undoes inversions where arg1 has the LOWER uid), and
 * -fsched-high-dest-first would flip it but demonstrably breaks the three
 * other argument pairs in this function that are also creation-order
 * decided and currently match.  A local temporary for either argument (or
 * for the shared 10) is folded by constant propagation and re-materialized
 * in arg order, changing nothing.  Whatever source shape the original used
 * to create arg1's setter first at this one site is not yet identified.
 */

extern u8 *Data_03001ebc;

extern void Func_02003b68();
extern void Func_02003c12();
extern void Func_02003b70();
extern void Func_02003c36();
extern void Func_02003c46();
extern s32 Func_02003ba6();
extern void Func_02003c72();
extern void Func_02003bb6();

void Func_02000574(void)
{
    Func_02003b68();
    Func_02003c12(10, 0, 0);
    Func_02003b70(10);
    Func_02003c36(0x119f);
    Func_02003c46(10, 0);

    if (Func_02003ba6(0, 0) == 1) {
        (*(u16 *)(Data_03001ebc + 472))++;
    }

    Func_02003c72(10, 0);
    Func_02003bb6();
}
