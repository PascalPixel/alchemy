/* The Suhara desert: the phase request and two object toggles. */
#include "SABAKU.H"

void SceneState_SendRequest15With45(void)
{
    BattleFx_SetPhaseRequest(15, 45);
}

s32 OverlayObject_ApplyZeroAndClearByte89(u8 *obj)
{
    Actor_SetSpriteFlags(obj, 0);
    obj[89] = 0;
    return 0;
}

s32 OverlayObject_ToggleField84Bit0(u8 *obj)
{
    obj[84] ^= 1;
    return 1;
}
