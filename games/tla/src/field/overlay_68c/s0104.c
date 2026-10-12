#include "types.h"
#include "flag_call.h"

extern u8 MsgScene68CNo06[];
extern u8 MsgScene68CNo09[];
extern u8 MsgScene68CNo12[];

void Func_020028ac(s32 message);
void Func_020028c4(s32 id, s32 flags);

void Func_02000104(s32 unused, s32 id)
{
    if (GameFlag_Test(0x966)) {
        Func_020028ac((s32)MsgScene68CNo12);
        Func_020028c4(id, 0);
    } else if (GameFlag_Test(0x965)) {
        Func_020028ac((s32)MsgScene68CNo09);
        Func_020028c4(id, 0);
    } else {
        Func_020028ac((s32)MsgScene68CNo06);
        Func_020028c4(id, 0);
    }
}
