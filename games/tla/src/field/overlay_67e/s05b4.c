#include "ov67e.h"

void Func_020005b4(s32 actor)
{
    const u8 *message;

    Func_02000d00();
    Func_02000dd0(0);
    message = MsgLadyUzumeAnd;
    Func_02000d60(message);
    UiText_OpenMessageAtObject(actor, 0);
    if (Inventory_PromptAndSetObjectMode(Func_02000e00(), 0) == 0) {
        Battle_WaitMode0(10);
        Func_02000d60(message + 1);
    } else {
        Battle_WaitMode0(20);
        Func_02000d60(message + 2);
    }
    Func_02000d70(actor, 0);
    Func_02000d08();
}
