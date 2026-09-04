#include "types.h"

#define SceneDialogue_RunActor3TimedLine Func_02002814

extern void Func_020082cc(void);
extern void Func_02008354(s32, s32);
extern void Func_020082d2(s32);
extern void Func_020083a8(s32);
extern void Func_020083c2(s32, s32, s32);
extern void Func_020082f6(void);

void SceneDialogue_RunActor3TimedLine(void)
{
    Func_020082cc();
    Func_02008354(3, 4);
    Func_020082d2(20);
    Func_020083a8(0x157d);
    Func_020083c2(3, 0, 20);
    Func_020082f6();
}
