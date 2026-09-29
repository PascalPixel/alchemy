/* NONMATCHING (number-bound): resource_3b9 at 0x02009a14 (56 bytes with its
 * pool), Scene_Initialize; twin resource_37f:0x020088f4.
 *
 * Remaining difference: the reference loads scene numbers 0x8c and 0x8e
 * from its literal pool and compares registers, as link-time scene numbers
 * do; plain constants compile to movs and cmp with immediates. The
 * handlers are the overlay's C (KORASHIAMU_IRIGUCHI). */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void FieldScene_DispatchBySelector(void);
void SceneState_ApplyRectsByFlatla384And962(void);

s32 Scene_Initialize(void)
{
    s32 scene = gGameState.scene;

    if (scene == 0x8c)
        FieldScene_DispatchBySelector();
    else if (scene == 0x8e)
        SceneState_ApplyRectsByFlatla384And962();
    return 0;
}
