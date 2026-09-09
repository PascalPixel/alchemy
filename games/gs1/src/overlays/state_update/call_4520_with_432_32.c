/* Named shorthand for one fixed argument pair, in overlay resource_3c8. */

void Func_02008b1a();

/*
 * Func_02008b1a is the relocated call word for the in-overlay routine at
 * image offset 0x4520, not a runtime address.  The 16-byte owner loads no
 * literal, so it carries no pool word and no alignment halfword.  432 is
 * built from a shifted immediate and passed straight to the callee as a
 * value, not used as a displacement.
 */
void SceneState_Call4520With432And32(void)
{
    Func_02008b1a(432, 32);
}
