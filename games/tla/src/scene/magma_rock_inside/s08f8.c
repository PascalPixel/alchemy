#include "local.h"

extern u8 Data_02005840[];
extern u8 Data_02005870[];
extern u8 Data_02005978[];
extern u8 Data_02005a50[];
extern u8 Data_02005c30[];
extern u8 Data_02005ca8[];
extern u8 Data_02005d38[];
extern u8 Data_02005ed0[];

extern s16 Data_020054fc[];
extern s16 Data_02005508[];
extern s16 Data_02005520[];
extern s16 Data_02005528[];
extern s16 Data_0200552e[];

void Func_0200075c(s32 value);
void Func_02004ee0(s16 *cells);

u8 *Func_020008f8(void)
{
    s16 scene = gPartyState.scene;

    if (scene != (s32)&SceneId_Ov692Room1) {
        if (scene == (s32)&SceneId_Ov692Room2)
            return Data_02005870;
        if (scene == (s32)&SceneId_Ov692Room3)
            return Data_02005978;
        if (scene == (s32)&SceneId_Ov692Room4)
            return Data_02005a50;
        if (scene == (s32)&SceneId_Ov692Room5)
            return Data_02005c30;
        if (scene == (s32)&SceneId_Ov692Room6)
            return Data_02005ca8;
        if (scene == (s32)&SceneId_Ov692Room7)
            return Data_02005d38;
        if (scene == (s32)&SceneId_Ov692Room8)
            return Data_02005ed0;
    }
    return Data_02005840;
}

void Func_0200099c(void)
{
    u8 *work;
    s16 scene;

    work = (u8 *)Ram_HeapSlots->eventWork;
    /* FAKEMATCH: the native loads the event work before it addresses gPartyState; without the barrier the scene offset is built first (differ 2). */
    __asm__("");
    scene = gPartyState.scene;
    if (scene == (s32)&SceneId_Ov692Room2)
        Func_02004ee0(Data_020054fc);
    else if (scene == (s32)&SceneId_Ov692Room3)
        Func_02004ee0(Data_02005508);
    else if (scene == (s32)&SceneId_Ov692Room4)
        Func_02004ee0(Data_02005520);
    else if (scene == (s32)&SceneId_Ov692Room6)
        Func_02004ee0(Data_02005528);
    else if (scene == (s32)&SceneId_Ov692Room7)
        Func_02004ee0(Data_0200552e);
    Func_0200075c(*(s32 *)(work + 0xcbc));
}
