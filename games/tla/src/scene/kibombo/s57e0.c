#include "kibombo.h"

void Func_02003c14(void);
void Func_02003c84(void);

#define PALETTE_WORD(index) (*(u16 *)(0x05000000 + (index) * 2))

void Func_020057e0(void)
{
    Func_02005a9c();
    Func_02005c3c(0);
    GameFlag_SetBit(514);
    Data_02007948[0] = PALETTE_WORD(169);
    Data_02007948[1] = PALETTE_WORD(167);
    Data_02007948[2] = PALETTE_WORD(166);
    if (gPartyState.entrance == 10) {
        ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
        Object_SetPartAttribute(Object_GetById(gPartyState.current_owner), 15);
        Func_02003c84();
    } else if (gPartyState.entrance == 11) {
        ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
        Object_SetPartAttribute(Object_GetById(gPartyState.current_owner), 15);
        Func_02003c14();
    } else {
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        KIBOMBO_TASK_ADD(Func_02002d64, 1152);
        Func_02005aa4();
    }
}
