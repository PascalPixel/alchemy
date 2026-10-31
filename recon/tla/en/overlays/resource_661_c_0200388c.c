// Near miss, differ 5 of 61 (tla-en, 2026-10-31): overlay 671's
// Func_02005428 (finds the current target's row in a list and shrinks it),
// which this overlay has instruction for instruction with Func_02003910 for
// Func_020054ac; its draft is resource_671_c_02005428.c and its remaining
// difference (the entry block only) is described there.
// The native's id load can stay inside the loop only if gcse saw no first test:
// any first test present at gcse (source if, while, for, or for (;;) with
// if (...) break, all copied by the first jump pass) gives the loop's top a
// second predecessor, and PRE inserts the id load on that edge. A copy made by
// the jump pass after loop (the other pass that duplicates exit tests) would
// leave it inside; no source form that defers the copy to that pass was found.
// Reading the work pointer both before the loop and again inside it leaves the
// load inside (64).
#define Func_02005428 Func_0200388c
#define Func_020054ac Func_02003910
#include "resource_671_c_02005428.c"
