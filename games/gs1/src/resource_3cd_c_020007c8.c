/*
 * The reference bl's raw decode is 0x0200100c -- beyond this overlay stream's
 * own extent, so the call is named at its decoded site address rather than at
 * the main-image symbol it semantically reaches (Func_08015340): a direct far
 * bl cannot encode that displacement, and only the RAM-resident address
 * reproduces the reference bytes. The reference also pops the return address
 * into r1, so r0 is live at return and the wrapper hands its callee's result
 * back.
 */
extern int Func_0200100c(void);
int Func_020007c8(void)
{
    return Func_0200100c();
}
