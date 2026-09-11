#include "types.h"

#define SceneDialogue_RunLine1637 Func_02000f14
#define SceneDialogue_RunActor3TimedLine Func_02002814
void Func_0200696c();
void Func_020069cc();
void Func_020069e0();
void Func_020082cc(void);
void Func_02008354(s32, s32);
void Func_020082d2(s32);
void Func_020083a8(s32);
void Func_020083c2(s32, s32, s32);
void Func_020082f6(void);

/* Complete scene-entry wrapper through return, alignment and its sole pool. */
void SceneDialogue_RunLine1637(void)
{
    Func_020069cc();
    Func_0200696c(0x1637, 1);
    Func_020069e0();
}

void SceneDialogue_RunActor3TimedLine(void)
{
    Func_020082cc();
    Func_02008354(3, 4);
    Func_020082d2(20);
    Func_020083a8(0x157d);
    Func_020083c2(3, 0, 20);
    Func_020082f6();
}
