#include "types.h"

void Func_02002688();           /* scripted-scene bracket: open */
s32 Func_0200266e();            /* story-flag test; used in a condition */
void Func_02002760();           /* show a dialogue line by id */
s32 Func_02002680();
void Func_02002772();
void Func_0200277a();
s32 Func_02002698();
void Func_020027ac();           /* wait for the slot's action to finish */
void Func_020026e0();           /* scripted-scene bracket: close */

void FieldScene_RunActorSeventeenFlaggedDialogue(void)
{
    u8 *work;

    Func_02002688();

    if (Func_0200266e(0x202) != 0) {
        Func_02002760(0x174b);
    } else if (Func_02002680(0x84e) != 0) {
        Func_02002772(0x176e);
    } else {
        Func_0200277a(0x1432);
        if (Func_02002698(0x84d) != 0) {
            work = *(u8 **)0x03001ebc;
            *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + 1);
        }
    }

    Func_020027ac(17, 0);
    Func_020026e0();
}
