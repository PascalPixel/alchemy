#include "types.h"
#include "text/msg_ids.h"

void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
s32 Djinn_AddToLeastLoadedOwner(s32 element, s32 djinn);
void Func_02000dc0(void);

void GameFlag_SetBit(s32 flag);
void Djinn_AddToOwner(s32 owner, s32 element, s32 djinn);
void Djinn_Activate(s32 owner, s32 element, s32 djinn);

void Func_020005fc(s32 owner, s32 element, s32 djinn, s32 activate)
{
    GameFlag_SetBit(element * 20 + djinn + 48);
    Djinn_AddToOwner(owner, element, djinn);
    if (activate == 1)
        Djinn_Activate(owner, element, djinn);
}

void Func_02000638(void)
{
    s32 element;
    s32 djinn;

    UiText_ShowPositionedMessageAndWait((s32)MsgYouGotA, 1);
    for (element = 0; element < 4; element++) {
        for (djinn = 0; djinn < 18; djinn++)
            Djinn_AddToLeastLoadedOwner(element, djinn);
    }
    Func_02000dc0();
}

void Func_02000664(void)
{
}
