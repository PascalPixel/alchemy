typedef int s32;

/*
 * resource_3b9 owner at 0x02002668, 440 bytes: one long, fully linear
 * setup sequence (about 40 calls, no branches) -- three near-identical
 * calls with shifted-constant position/size pairs, several id-tagged
 * calls (0-11) with a shared shifted-constant pair, three calls sharing
 * a pool address, and a closing run touching ids 0/11/20/60/67.
 *
 * Complete owner: `push {r5, lr}` at 0x02002668 through `pop {r5} / pop
 * {r0} / bx r0` at 0x02002804-0x02002808, followed by the five-word
 * literal pool 0x0200280c-0x0200281f; the next owner's prologue is at
 * 0x02002820 (this overlay's own resource_3b9_c_02002820.c candidate,
 * not yet drafted). No incoming arguments are read before being
 * overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2`
 * rule.
 *
 * `Func_02005348` is called twice with different arities (3 args at
 * 0x2002732, 2 at 0x20026ea) -- declared old-style rather than
 * miscredited either shape, per this project's "arities vary per site"
 * convention (resource_3cb_c_02000580.c).
 *
 * Uncertainty: none of the thirty-odd callees are identified beyond
 * call shape; every id/constant is recorded as a passed value, not
 * decoded further.
 */

extern void Func_02005260(void);
extern void Func_020052ce(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020052dc(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020052ea(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020053b6(void);
extern void Func_020053ca(void);
extern void Func_02005290(s32 arg0);
extern void Func_02005328(s32 arg0, s32 arg1);
extern void Func_02005310(s32 arg0, s32 arg1);
extern void Func_0200534e(s32 arg0);
extern void Func_02004308(s32 arg0);
extern void Func_02005344(s32 arg0, s32 arg1);
extern void Func_02004316(s32 arg0);
extern void Func_02005352(s32 arg0, s32 arg1);
extern void Func_02004324(s32 arg0);
extern void Func_02005360(s32 arg0, s32 arg1);
extern void Func_02005348();
extern void Func_0200433a(s32 arg0);
extern void Func_020053ba(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020053c6(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200532c(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200533a(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200535a(s32 arg0, s32 arg1);
extern void Func_02005362(s32 arg0, s32 arg1);
extern void Func_0200537a(s32 arg0, s32 arg1);
extern void Func_02005340(s32 arg0);
extern void Func_020043b8(s32 arg0, s32 arg1);
extern void Func_020053d0(s32 arg0, s32 arg1);
extern void Func_020053d8(s32 arg0, s32 arg1);
extern void Func_0200538e(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200539c(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020053ec(s32 arg0, s32 arg1);
extern void Func_020053d6(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020053e2(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020053e8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020053a6(s32 arg0);
extern void Func_0200549e(s32 arg0, s32 arg1);
extern void Func_020054b8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0200542c(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005432(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020053e0(s32 arg0);
extern void Func_0200551c(void);
extern void Func_02005528(void);
extern void Func_020054fe(s32 arg0);

void Func_02002668(void)
{
    s32 shared = 0x0200adac;

    Func_02005260();
    Func_020052ce(1, 0x3180000, 0x880000);
    Func_020052dc(2, 0x3380000, 0x880000);
    Func_020052ea(3, 0x3280000, 0x980000);
    Func_020053b6();
    Func_020053ca();
    Func_02005290(40);
    Func_02005328(8, 1);
    Func_02005310(8, 3);
    Func_0200534e(0x2134);
    Func_02004308(8);
    Func_02005344(9, 1);
    Func_02004316(9);
    Func_02005352(10, 1);
    Func_02004324(10);
    Func_02005360(11, 1);
    Func_02005348(11, 3);
    Func_0200433a(11);
    Func_020053ba(1, 0xe000, 0);
    Func_020053c6(2, 0xa000, 20);
    Func_0200532c(1, 0x10000, 0x8000);
    Func_0200533a(2, 0x10000, 0x8000);
    Func_02005348(3, 0x10000, 0x8000);
    Func_0200535a(1, shared);
    Func_02005362(2, shared);
    Func_0200537a(3, shared);
    Func_02005340(20);
    Func_020043b8(0, 0);
    Func_020053d0(0, 3);
    Func_020053d8(11, 3);
    Func_0200538e(11, 0x10000, 0x8000);
    Func_0200539c(0, 0x10000, 0x8000);
    Func_020053ec(11, 2);
    Func_020053d6(11, 0x33e, 152);
    Func_020053e2(11, 808, 164);
    Func_020053e8(11, 808, 312);
    Func_020053a6(20);
    Func_0200549e(0x6666, 0xccc);
    Func_020054b8(0x3280000, -1, 0x1380000, 1);
    Func_0200542c(0, 808, 164);
    Func_02005432(0, 808, 312);
    Func_020053e0(60);
    Func_0200551c();
    Func_02005528();
    Func_020054fe(67);
}
