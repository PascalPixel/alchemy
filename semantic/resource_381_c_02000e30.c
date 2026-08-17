/*
 * Two differing halfwords at the second Func_020043fc call, and the whole
 * residual is one instruction's position:
 *
 *   ours       movs r1,#128 / lsls r1,r1,#1 / mov r0,sl / bl
 *   reference  movs r1,#128 / mov r0,sl / lsls r1,r1,#1 / bl
 *
 * NOT the allocator, and not this call site. The reorder is controlled by what
 * follows in and after the basic block, which the probes below isolate. Same
 * compiler, same flags, only the tail of the function varies:
 *
 *   TARGET(a, 0x100);                       -> ours
 *   TARGET(a, 0x100); store;                -> ours
 *   TARGET(a, 0x100); store; store;         -> ours
 *   TARGET(a, 0x100); call;                 -> REFERENCE ORDER
 *   TARGET(a, 0x100); store; call;          -> REFERENCE ORDER
 *   TARGET(a, 0x100); call; store;          -> REFERENCE ORDER
 *   TARGET(a, 0x100); store; call; loop;    -> ours
 *
 * A later CALL in the same block produces the reference order; a store does
 * not; a loop after the block takes it away again. That is INSN_PRIORITY in
 * haifa-sched.c being the longest path to the END of the block: the two
 * candidates tie on priority, on the last-scheduled-insn class and on
 * dependent count, so `rank_for_schedule` falls through to INSN_LUID, and what
 * follows decides which of them ties first. haifa-sched.c is unmodified stock
 * 2.96, so the reference order is reachable here -- our own exact corpus emits
 * this interleave 112 times in 400 owners.
 *
 * This owner has both the later call AND the loop, so it lands on ours. Tried
 * and rejected against the bytes: hoisting `i = 0` above the call (2 -> 64
 * halfwords), the first loop as `for`, a prototyped 2-arg alias for the
 * unprototyped import, `0x100` spelled `128 << 1`, moving the p-store before
 * the call (2 -> 9), and a local temporary for either argument. shape-sweep,
 * the permuter at 999 candidates, and mode-cohort at 153 configurations all
 * hold at the baseline.
 *
 * So it is a source question, not an allocation one -- the register-blind
 * verdict would call this a reorder and pass over it -- and the thing to find
 * is the block tail the original had, not a different spelling of the call.
 */
#include "types.h"

typedef struct {
    u8 filler0[12];
    s32 unkC;
    u8 filler10[8];
    s32 unk18;
    s32 unk1C;
} Ent;

extern Ent *Func_0200432a(s32);
extern void Func_02004378(s32, s32, s32);
extern void Func_020043fc();
extern void Func_02004332(s32);
extern void Func_020043a6(s32, s32, s32);
extern Ent *Func_02004364(s32);
extern s32 Func_0200436c(s32);
extern void Func_0200430a(s32, s32);
extern void Func_020044c2(s32);
extern void Func_0200437c(s32);
extern void Func_020044e4(s32);
extern void Func_020043b8(s32);
extern void Func_02004434(s32, s32, s32);
extern void Func_0200443e(s32, s32, s32);

void Func_02000e30(s32 a)
{
    Ent *b;
    Ent *p;
    s32 step;
    u8 i;

    b = Func_0200432a(8);
    b->unk18 = 0x10000;
    b->unk1C = 0x10000;
    Func_02004378(a, 0x1d7, 0x122);
    Func_020043fc(a, 0xc000, 0);
    Func_02004332(10);
    Func_020043a6(8, 0x1d70000, 0x1220000);
    p = Func_02004364(a);
    Func_0200430a(Func_0200436c(a), 0);
    Func_020043fc(a, 0x100);
    *((u8 *)p + 0x55) = 0;
    Func_020044c2(0xc9);
    i = 0;
    do {
        p->unkC += 0x8000;
        Func_0200437c(1);
        i++;
    } while (i != 60);
    Func_020044e4(0xbe);

    i = 0;
    do {
        p->unkC += 0x1999;
        p->unk18 -= 0x28f;
        p->unk1C -= 0x28f;
        b->unk18 -= 0x28f;
        b->unk1C -= 0x28f;
        Func_020043b8(1);
        i++;
    } while (i != 90);
    Func_02004434(a, 0, 0);
    Func_0200443e(8, 0, 0);
}
