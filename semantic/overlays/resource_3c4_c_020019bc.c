/*
 * resource_3c4 @ 0x020019bc (14 bytes).
 *
 * A bracketing pair of resident services with no argument setup between them.
 * Whether Func_02004ac0 consumes the value Func_02004ab4 leaves in r0 cannot
 * be decided from this overlay: both targets live outside the overlay image.
 * Modelled as two argument-less calls, which is what the body encodes.
 * `pop {r0} ; bx r0` return: void.
 */
void Func_02004ab4(void);
void Func_02004ac0(void);

void Func_020019bc(void)
{
    Func_02004ab4();
    Func_02004ac0();
}
