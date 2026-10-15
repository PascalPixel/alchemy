#include "airs_rock_inside.h"

extern u8 MsgCheckedThePsynergy[];
extern s32 Data_02006c10[];
extern s32 Data_02006d6c[];
extern s32 Data_02006e2c[];
extern s32 Data_02006eec[];
extern s32 Data_02006fa0[];
extern s32 Data_0200706c[];
extern s32 Data_020070fc[];
extern s32 Data_02007228[];
extern s32 Data_02007318[];
extern s32 Data_020073a8[];

void Func_02004bb4(void)
{
    const u8 *message;

    Func_02005e14();
    Func_02005f2c(0);
    message = MsgCheckedThePsynergy;
    UiText_ShowPositionedMessageAndWait(message, 1);
    Func_02005f94(126);
    Func_02005f34(999, 0);
    Battle_WaitMode0(10);
    GameFlag_ClearBit(322);
    UiText_ShowPositionedMessageAndWait(message + 1, 1);
    Func_02005e1c();
}

// The placement table for the current room.
s32 *Func_02004c00(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov664Room1)
        return Data_02006c10;
    if (scene == (s32)&SceneId_Ov664Room2)
        return Data_02006d6c;
    if (scene == (s32)&SceneId_Ov664Room3)
        return Data_02006e2c;
    if (scene == (s32)&SceneId_Ov664Room4)
        return Data_02006eec;
    if (scene == (s32)&SceneId_Ov664Room5)
        return Data_02006fa0;
    if (scene == (s32)&SceneId_Ov664Room6)
        return Data_0200706c;
    if (scene == (s32)&SceneId_Ov664Room7)
        return Data_020070fc;
    if (scene == (s32)&SceneId_Ov664Room8)
        return Data_02007228;
    if (scene == (s32)&SceneId_Ov664Room9)
        return Data_02007318;
    if (scene == (s32)&SceneId_Ov664Room10)
        return Data_020073a8;
    return Data_02006d6c;
}
