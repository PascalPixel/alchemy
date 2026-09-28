/* resource_3b8:0200bfc4..0200c00c (72 bytes), still linked from the
 * listing. Remaining difference: the answer messages are derived from one
 * loaded base 0x22a3 (r + 1, r + 2); integers become three pool constants. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
/* Declarations and helpers: games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/KYUDEN.H. */

void SceneDialogue_ShowMessage22a3Branch(s32 a)
{
    s32 k = 0x22a3;

    Event_SetMessage(k);
    Event_OpenMessage(a, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(k + 1);
        Event_ShowMessage(a, 0);
    } else {
        Event_SetMessage(k + 2);
        Event_ShowMessage(a, 0);
    }
}
