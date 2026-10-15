#include "scene.h"

void Func_0200316c(void);
void Func_020031b4(void);
void Func_020032f4(void);
void Func_02003364(void);
void Func_02003404(void);
void Func_020035b8(void);
void Func_020035d8(void);
void Func_020036d0(void);
void Func_0200371c(void);
void Func_020037d4(void);
void Func_020037e8(void);

// Clears the followed actor and runs the entered room's setup, as overlay
// 654's Func_02002ec4 does; Room7 has none.
s32 Func_02003094(void)
{
    s16 scene;

    Func_020021d4();
    /* FAKEMATCH: the one-pass block ends the scheduling block after the store, so the party state address is loaded before the scene field offset is built (differ 2 plainly; an s32 scene does not help, an empty asm barrier does the same). */
    do
    {
        gData_0200480c = 0;
    } while (0);
    scene = gPartyState.scene;
    if (scene == (s32)&SceneId_Ov661Room1)
        Func_0200316c();
    else if (scene == (s32)&SceneId_Ov661Room2)
        Func_020031b4();
    else if (scene == (s32)&SceneId_Ov661Room3)
        Func_020032f4();
    else if (scene == (s32)&SceneId_Ov661Room4)
        Func_02003364();
    else if (scene == (s32)&SceneId_Ov661Room5)
        Func_02003404();
    else if (scene == (s32)&SceneId_Ov661Room6)
        Func_020035b8();
    else if (scene == (s32)&SceneId_Ov661Room8)
        Func_020035d8();
    else if (scene == (s32)&SceneId_Ov661Room9)
        Func_020036d0();
    else if (scene == (s32)&SceneId_Ov661Room10)
        Func_0200371c();
    else if (scene == (s32)&SceneId_Ov661Room11)
        Func_020037d4();
    else if (scene == (s32)&SceneId_Ov661Room12)
        Func_020037e8();
    return 0;
}
