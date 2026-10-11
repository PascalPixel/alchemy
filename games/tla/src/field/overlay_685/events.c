#include "scene.h"

void Func_02000304(s32 actor)
{
    s32 msg = (s32)MsgWhatYouWant;

    Func_0200350c(msg);
    UiText_OpenMessageAtObject(actor, 0);
    if (Inventory_PromptAndSetObjectMode(Func_020035f4(), 0) == 0) {
        Battle_WaitMode0(10);
        Func_0200350c(msg + 1);
    } else {
        Battle_WaitMode0(20);
        Func_0200350c(msg + 2);
    }
    Func_02003524(actor, 0);
}

void Func_02000350(s32 actor)
{
    struct FieldActor *object;

    Func_0200350c((s32)MsgAllRightI);
    Func_02003524(actor, 0);
    object = Object_GetById(actor);
    Func_02003624(object, Object_GetById(gPartyState.current_owner));
}

void Func_0200038c(s32 actor)
{
    Func_0200350c((s32)MsgAwComeOn);
    Func_02003524(actor, 0);
    Func_02003534_(actor, 0xd000);
    Object_SetActionCallback((struct ObjectRuntime *)Object_GetById(actor), 8);
}

void Func_020003bc(void)
{
    Func_02003424(41, 97, 1, 4, 41, 77);
    Func_0200341c(41, 33, 1, 4, 41, 13);
}
