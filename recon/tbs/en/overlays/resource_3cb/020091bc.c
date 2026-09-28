/* Draft of Scene_ShowDialoguePair292a, resource_3cb at 0x020091bc, built with
 * games/THE BROKEN SEAL/SRC/MENU/LINK_LOBBY/LOBBY.H.
 * Remaining difference: four bytes, two argument moves in the opposite order.
 * The listing keeps these rows. */
#include "LOBBY.H"

s32 Scene_ShowDialoguePair292a(void)
{
    s32 handle;
    Engine_AudioPlayCue(85);
    handle = UiText_OpenMessageWindow(0x292a, 5, 4, 1);
    while (UiWork_IsComplete() == 0)
        Engine_TaskWait(1);
    SaveState_ProcessSelectedSlot();
    UiWork_Finalize(handle, 1);
    Engine_TaskWait(1);
    handle = UiText_OpenMessageWindow(0x292b, 5, 4, 1);
    while (UiWork_IsComplete() == 0)
        Engine_TaskWait(1);
    return UiWork_Finalize(handle, 1);
}
