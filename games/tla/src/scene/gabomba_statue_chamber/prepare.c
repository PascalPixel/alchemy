#include "types.h"
#include "field_scene.h"
#include "gabomba_statue_chamber.h"

s32 Func_02001f00(void);
void Func_02001b14(void);
void Func_02001ba8(void);
void Func_02001c44(void);

s32 Func_02001aa4(void)
{
    struct GabombaStatueChamberEventWork *work;
    struct PartyState *party;
    s16 scene;

    work = Ram_HeapSlots->eventWork;
    work->screenEffect = 521;
    Object_GetById(Func_02001f00())->priorityFlags |= 0x20;
    party = &gPartyState;
    /* FAKEMATCH: ⚓️ loads the party state's address before it builds the scene's offset; read as a plain field the offset's first half is scheduled ahead of the load (differ 2). */
    asm volatile("" : "+l"(party));
    scene = party->scene;
    if (scene == (s32)&SceneId_Ov672Room1)
        Func_02001b14();
    else if (scene == (s32)&SceneId_Ov672Room2)
        Func_02001ba8();
    else if (scene == (s32)&SceneId_Ov672Room3)
        Func_02001c44();
    return 0;
}

s32 Scene_PrepareMap(void)
{
    return 0;
}
