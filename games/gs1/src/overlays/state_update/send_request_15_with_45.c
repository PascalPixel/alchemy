/* Named shorthand for one fixed state request, in overlay resource_3c0. */

void Func_02001660();

/*
 * The 14-byte owner at 0x02000314 loads no literal and has no pool word.
 * Both arguments are immediates, and Func_02001660 spells the overlay's own
 * relocated call word rather than a runtime address.
 */
void SceneState_SendRequest15With45(void)
{
    Func_02001660(15, 45);
}
