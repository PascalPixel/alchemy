/*
 * resource_3c4 @ 0x02002040 (98 bytes).
 *
 * Sibling of 0x02001fdc against a different service set.  Same six-argument
 * placement shape: four register arguments plus [sp,#0] and [sp,#4], with r5
 * holding 49 across both calls.
 *
 * `negs r1,r1` after `movs r1,#1` materialises -1, so the two middle calls
 * take signed -1 arguments.
 *
 * The tail at 0x02002090 is a secondary entry into this owner used by
 * Func_02001fc4, which is not admitted (see this overlay's report).
 *
 * `add sp,#8 ; pop {r5} ; pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;

void Func_080091c0();
void Func_0808a0f0();
void Func_0808a3c0();

void Func_02002040(void)
{
    Func_080091c0(8, 113, 1, 1, 8, 49);
    Func_080091c0(49, 107, 1, 1, 49, 43);
    Func_0808a3c0(100, -1, -1);
    Func_0808a3c0(101, -1, -1);
    Func_0808a0f0(15, 0, 0);
    /* secondary entry 0x02002090 */
    Func_0808a0f0(16, 0, 0);
}
