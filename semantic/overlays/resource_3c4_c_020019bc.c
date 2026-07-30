/*
 * resource_3c4 @ 0x020019bc (14 bytes).
 *
 * A bracketing pair of resident services with no argument setup between them.
 * Whether Func_02004ac0 consumes the value Func_02004ab4 leaves in r0 cannot
 * be decided from this overlay: both targets live outside the overlay image.
 * Modelled as two argument-less calls, which is what the body encodes.
 * Note that Func_02001970 calls Func_02004ac0 with two arguments (0, 1), so
 * the service does take arguments; this owner simply establishes none, leaving
 * r0 as whatever Func_02004ab4 returned and r1 stale.  Recheck once the
 * resident service is reconstructed.
 * `pop {r0} ; bx r0` return: void.
 */
void Func_02004ab4(void);
void Func_02004ac0(void);

void Func_020019bc(void)
{
    Func_02004ab4();
    Func_02004ac0();
}
