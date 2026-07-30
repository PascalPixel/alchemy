typedef signed int s32;

/*
 * Resource 3c8 at 0x02002ee4: cutscene step guarded by a one-shot query.
 *
 * The owner is complete: `push {lr} ; sub sp, #8` at 0x02002ee4 and the
 * matching `add sp, #8 ; pop {r0} ; bx r0` at 0x02002f28..0x02002f2c.  No
 * literal pool sits inside the span and no live frame or register state
 * escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * The eight reserved stack bytes carry arguments five and six of this
 * package's six-argument placement ABI: r0-r3, then [sp,#0], then [sp,#4].
 * The byte-exact sibling 0x020010cc fixes that order
 * (`Func_02005f1c(36, 8, 1, 3, 23, 8)`).
 *
 * All seven `bl` sites are accounted for: 0x02007d7e, 0x02003bb6,
 * 0x02007d3a, 0x02007d4e, 0x02003292, 0x02007dbe, 0x02005a3a.  Import naming
 * follows the note in resource_3c8_c_020002f0.c - each name is the address
 * its call site computes, and none of them names a local continuation owned
 * by this C file.
 */

/* Old-style declarations: the imports' real interfaces are not known here. */
void Func_02007d7e();
void Func_02007d3a();
void Func_02007d4e();
void Func_02003292();
void Func_02007dbe();
void Func_02005a3a();

/* Used for its return value. */
s32 Func_02003bb6();

void Func_02002ee4(void)
{
    Func_02007d7e();
    if (Func_02003bb6() == 0) {
        Func_02007d3a(69, 48, 4, 2, 5, 48);
        Func_02007d4e(73, 37, 9, 13, 9, 37);
        Func_02003292();
    }
    Func_02007dbe();
    Func_02005a3a();
}
