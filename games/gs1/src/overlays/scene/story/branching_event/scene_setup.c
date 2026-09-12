#include "types.h"

#define FieldScene_RunFourCallSequence Func_020010f4
#define ResetSceneParametersAndFinishSetup Func_02001158
#define SceneState_ConfigureRegion82_7AndApply768 Func_020027f4
void Func_02003f30();
void Func_020011c0();
void Func_02002130();
void Func_02003f44();
void Func_0200408a();
void Func_0200402c();
void Func_020040d0();
void Func_020040dc();
void Func_020055e2(s32, s32, s32, s32, s32, s32);
void Func_02005590(s32);
void Func_02005638(s32);

/*
 * Configure this overlay's scene parameters, enable the configuration, and
 * apply the final magnitude.  The complete owner is the saved-link function
 * at 0x020027f4-0x02002820; its stack frame holds arguments five and six for
 * the first call.
 */

static __inline__ void ResetSceneParameters(s32 a, s32 b, s32 c, s32 mode)
{
    Func_0200408a(a, b, c, mode);
}

void FieldScene_RunFourCallSequence(void)
{
    Func_02003f30();
    Func_020011c0();
    Func_02002130();
    Func_02003f44();
}

/* resource_3c5 owner at 0x02001158, 42 bytes. */
void ResetSceneParametersAndFinishSetup(void)
{
    ResetSceneParameters(-1, -1, -1, 0);
    Func_0200402c(0, 6, 0);
    Func_020040d0();
    Func_020040dc();
}

void SceneState_ConfigureRegion82_7AndApply768(void)
{
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a = 18;
    s32 b = 7;

    Func_020055e2(82, 7, 1, 2, a, b);
    Func_02005590(1);
    Func_02005638(768);
}
