// Near miss, differ 7 of 61 (tla-en, 2026-10-31): overlay 671's
// Func_02005428 (finds the current target's row in a list and shrinks it),
// which this overlay has instruction for instruction with Func_02000c44 for
// Func_020054ac; its draft is resource_671_c_02005428.c and its remaining
// difference is described there. Also measured here: a plain while loop, 38;
// a first test before a goto loop with the work read through a pointer to its
// heap slot each pass keeps the id load inside the loop (gcse's one pass
// hoists only the slot read) but places the work load after the first test,
// so its entry is not cross-jumped, 42.
#define Func_02005428 Func_02000bc0
#define Func_020054ac Func_02000c44
#include "resource_671_c_02005428.c"
