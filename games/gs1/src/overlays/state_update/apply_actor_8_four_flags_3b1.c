/*
 * Actor 8 flag setup for overlay resource_3b1. Each callee name refers to
 * its own call word rather than to a shared runtime address.
 */

void Func_0200a044();
void Func_0200a04c();
void Func_0200a054();
void Func_0200a05e();

/*
 * A flat setter sequence, no branches. The 52-byte owner at 0x02005004
 * includes its three pool words, which are plain numeric arguments and not
 * addresses of Value_ globals.
 */
void SceneState_ApplyActor8FourFlags(void)
{
    Func_0200a044(8, 0x92c);
    Func_0200a04c(8, 0x935);
    Func_0200a054(8, 0x917);
    Func_0200a05e(8, 0x990);
}
