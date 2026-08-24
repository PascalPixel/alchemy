/*
 * The reference pops the return address into r1, not r0, so r0 is live at
 * return: the wrapper hands its callee's result back. Declaring the pair
 * void compiles pop {r0}/bx r0 and misses by exactly those two halfwords.
 */
extern int Func_02001020(void);
int Func_020007d4(void)
{
    return Func_02001020();
}
