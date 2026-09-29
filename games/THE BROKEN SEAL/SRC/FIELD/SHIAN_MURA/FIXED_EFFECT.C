#include "SHIAN.H"

/* Plays page effect 22 for slot 1 with mode 2; all three arguments are
 * immediates, so the function carries no literal pool. */
void SceneEffect_RequestFixedEffect(void)
{
    BattleFx_RunPageEffectForSlot(22, 1, 2);
}
