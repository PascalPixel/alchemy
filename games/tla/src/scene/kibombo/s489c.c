#include "kibombo.h"

void Func_0200489c(void)
{
    Func_02004524();
    Kibombo_SePlay(0x240);
    Object_SetPartAttribute(Object_GetById(12), 7);
    Object_SetPartAttribute(Object_GetById(13), 7);
    WaitFrames(8);
    Object_SetPartAttribute(Object_GetById(12), 0);
    Object_SetPartAttribute(Object_GetById(13), 0);
    WaitFrames(8);
    Func_02005b94(17, 0x100, 20);
    Func_02005b4c((s32)MsgLookLookThe);
    Func_02005b64(17, 0);
    Func_02005c9c(0x241);
    ObjectMotion_EnableActionAndSetCallback(12, gData_0200614c);
    Object_SetActionCallbackAndRefreshById(13, gData_0200614c);
    ObjectMotion_EnableActionAndSetCallback(12, gData_0200613c);
    ObjectMotion_EnableActionAndSetCallback(13, gData_0200613c);
    Object_SetPartAttribute(Object_GetById(12), 12);
    Object_SetPartAttribute(Object_GetById(13), 12);
    Battle_WaitMode0(120);
    Func_020045d0();
    GameFlag_SetBit(0x8fc);
    GameFlag_SetBit(265);
    Func_02005bc4(12);
}

void Func_02004978(void)
{
    Func_02004524();
    Kibombo_SePlay(0x240);
    Object_SetPartAttribute(Object_GetById(12), 7);
    Object_SetPartAttribute(Object_GetById(13), 7);
    WaitFrames(8);
    Object_GetById(12)->update = Func_020000a0;
    Object_GetById(13)->update = Func_020000a0;
    Func_02005b94(17, 0x100, 20);
    Func_02005b4c((s32)MsgLookTheGreat);
    Func_02005b5c(17, 0, 20);
    Kibombo_SePlay(0x240);
    Func_020036e0(1, 0);
    Battle_WaitMode0(20);
    Func_02005c9c(162);
    Scheduler_AddOrUpdateCallback(Func_02002f34, 0x480);
    Battle_WaitMode0(120);
    Func_020045d0();
    GameFlag_SetBit(0x8fc);
    GameFlag_SetBit(265);
    Func_02005bc4(12);
}
