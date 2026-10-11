// near miss: differ 15 (listing 177 instructions, draft 176); Func_02005b94's 0x103: the native reloads it at each call in two argument orders; the draft keeps it in r5
#include "../../../../games/tla/src/field/overlay_670/ov670.h"

void Func_02000978(void)
{
    Ov670_ObjArm(gPartyState.current_owner, 0x8000, 20);
    Func_02005b4c((s32)MsgItLooksLikeHas);
    UiText_OpenMessageAtObject(6, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
        Ov670_ObjArm(5, 0x4000, 10);
        Object_SetModeById(5, 3);
        Func_02005b64(5, 0);
        ((struct Ov670EventWork *)Ram_HeapSlots->event_work)->field_1c4++;
    } else {
        Ov670_ObjArm(5, 0x4000, 10);
        Object_SetModeById(5, 4);
        ((struct Ov670EventWork *)Ram_HeapSlots->event_work)->field_1c4++;
        Func_02005b64(5, 0);
    }
    Func_02005b94(7, 0x101, 20);
    Func_02005b64(7, 0);
    Ov670_ObjArm(gPartyState.current_owner, 0x2000, 0);
    Ov670_ObjArm(26, 0xb000, 0);
    Ov670_ObjArm(5, 0x6000, 0);
    Ov670_ObjArm(6, 0xe000, 40);
    Ov670_ObjArm(gPartyState.current_owner, 0xe000, 0);
    Ov670_ObjArm(26, 0xd000, 0);
    Ov670_ObjArm(5, 0, 0);
    Ov670_ObjArm(6, 0xe000, 20);
    Func_02005b64(7, 0);
    Func_02005b94(26, 0x100, 0);
    Func_02005b64(26, 0);
    Func_02005b64(7, 0);
    Func_02005b64(6, 0);
    Ov670_ObjArm(5, 0x4000, 40);
    Ov670_ObjArm(5, 0, 0);
    Func_02005b64(5, 0);
    Func_02005b64(7, 0);
    Func_02005b94(6, 0x103, 0);
    ObjectMotion_Launch(6, 2, 10);
    ObjectMotion_Launch(6, 4, 10);
    Func_02005b64(6, 0);
    Func_02005b94(5, 0x103, 10);
    Func_02005b64(5, 0);
    Func_02005b94(7, 0x103, 10);
    Func_02005b64(7, 0);
}
