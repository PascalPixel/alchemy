#include "types.h"

/*
 * Resource 3a3 flag-sync / scene dispatcher at 0x02000874 (144 bytes,
 * 0x02000874 .. 0x02000903, of which 0x020008e4 .. 0x02000903 is the pool).
 *
 * Complete owner: `push {lr}` at 0x02000874 and the interworking return
 * `pop {r1} / bx r1` at 0x020008e0.  The popped register is r1, not r0, so r0
 * survives and IS the result -- and r0 is set to 0 immediately before the
 * epilogue, so the owner always returns 0.
 *
 * All 10 call sites are placed (row reports calls=10): Func_080770c0 x5,
 * Func_080770c8 x3, Func_02000904 x1, Func_02000b2c x1.
 *
 * SHARED CALL SITE.  The Func_080770c8(0x241) site at 0x0200089e is reached
 * from two arms (the 0x8fe test's true edge and the 0x907 test's true edge).
 * It is one site, so it is spelled once as a short-circuit `||` rather than
 * duplicated per arm.
 *
 * `Data_02000240[224]` -- the signed halfword at byte offset 448 -- is the
 * cross-overlay scene selector; the byte-exact siblings
 * `assets/code/resource_3a3_c_02000030.c` and `_0200011c.c` read the same
 * cell.  Those sources spell the compared constants as `(s32)&Value_0000004b`
 * and `(s32)&Value_0000004c`, which is a literal-pooling device for the
 * byte-exact reconstruction; the integers 0x4b and 0x4c are written directly here.
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
        /* tests a flag id; nonzero when set */
       /* sets a flag id */

/* Overlay-local scene bodies. */



extern s16 Data_02000240[];

extern s32 Func_02001702();
extern void Func_02001716();
extern s32 Func_02001714();
extern s32 Func_0200171e();
extern void Func_02001730();
extern s32 Func_0200172e();
extern s32 Func_02001738();
extern void Func_0200174a();
extern void Func_020011d4(void);
extern void Func_02001408(void);
s32 Func_02000874(void)
{
    s16 scene;

    if (Func_02001702(0x8fd) != 0) {
        /* `movs r0,#144 / lsls r0,#2` = 576 = 0x240. */
        Func_02001716(0x240);
    }

    if (Func_02001714(0x8fe) != 0 || Func_0200171e(0x907) != 0) {
        Func_02001730(0x241);
    }

    if (Func_0200172e(0x8fe) != 0 && Func_02001738(0x907) != 0) {
        Func_0200174a(0x242);
    }

    scene = Data_02000240[224];
    if (scene == 0x4b) {
        Func_020011d4();
    } else if (scene == 0x4c) {
        Func_02001408();
    }

    return 0;
}
