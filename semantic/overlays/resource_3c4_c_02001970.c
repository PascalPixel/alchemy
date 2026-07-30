/*
 * resource_3c4 @ 0x02001970 (76 bytes: 60 code + four pool words).
 *
 * Publishes one of two selectors depending on whether either of two flags is
 * set.  All four pool words (0x982, 0x983, 0x268a, 0x2689) are small selector
 * numbers, not addresses.  The reference short-circuits: the second query is
 * only made when the first returns zero.
 *
 * `pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;

void Func_02004a68(void);
void Func_02004ac0(s32 arg0, s32 arg1);
s32 Func_02004a56(s32 selector);
s32 Func_02004a60(s32 selector);
void Func_02004a64(s32 selector, s32 flag);
void Func_02004a6e(s32 selector, s32 flag);
void Func_02004aa2(void);

void Func_02001970(void)
{
    Func_02004a68();
    Func_02004ac0(0, 1);
    if (Func_02004a56(0x982) != 0 || Func_02004a60(0x983) != 0) {
        Func_02004a64(0x268a, 1);
    } else {
        Func_02004a6e(0x2689, 1);
    }
    Func_02004aa2();
}
