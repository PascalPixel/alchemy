#include "types.h"
#include "text/msg_ids.h"

void Battle_WaitMode0(s32 frames);
void UiText_OpenMessageAtObject(s32 id, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 id, s32 flags);
void Func_02002adc(s32 message);
void Func_02002af4(s32 id, s32 flags);
s32 Func_02002bb4(void);

void Func_02000590(s32 actor)
{
    s32 msg = (s32)&MsgScene682No11;

    Func_02002adc(msg);
    UiText_OpenMessageAtObject(actor, 0);
    if (Inventory_PromptAndSetObjectMode(Func_02002bb4(), 0) == 0) {
        Battle_WaitMode0(10);
        Func_02002adc(msg + 1);
    } else {
        Battle_WaitMode0(20);
        Func_02002adc(msg + 2);
    }
    Func_02002af4(actor, 0);
}
