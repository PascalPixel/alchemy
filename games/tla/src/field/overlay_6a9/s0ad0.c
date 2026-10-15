#include "ov6a9.h"

const u8 *Func_02000ad0(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov6a9Room1)
        return Data_0200149c;
    if (scene == (s32)&SceneId_Ov6a9Room3)
        return Data_020014d8;
    if (scene == (s32)&SceneId_Ov6a9Room4)
        return Data_020014fc;
    return Data_02001430;
}

s32 Func_02000b20(void)
{
    s16 *scene;

    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 0x1ac) = 0x204;
    if (gPartyState.scene == (s32)&SceneId_Ov6a9Room1)
        Func_02000ba8();
    /* FAKEMATCH: a pointer to the scene field keeps the second test a plain ldrsh (plain gPartyState.scene or a party pointer: differ 16). */
    scene = &gPartyState.scene;
    if (*scene == (s32)&SceneId_Ov6a9Room4)
        Func_02000f98();
    return 0;
}
