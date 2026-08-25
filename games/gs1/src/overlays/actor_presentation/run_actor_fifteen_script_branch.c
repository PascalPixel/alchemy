#include "types.h"

struct SceneActor { u8 reserved_00[100]; u16 presentation_flags; };

void Func_020016ee(s32);
s32 Func_0200271a(s32);
void Func_02002744(void);
struct SceneActor *Func_0200275e(s32);
void Func_0200277a(void);
struct SceneActor *Func_020027a0(s32);
void Func_020027ec(s32);
void Func_02002800(s32);

#define ActorPresentation_RunActorFifteenScriptBranch Func_02000d78

void ActorPresentation_RunActorFifteenScriptBranch(void)
{
    Func_0200275e(15)->presentation_flags |= 2;
    Func_02002744();
    if (Func_0200271a(0x855) == 0)
        Func_020027ec(0x123d);
    else
        Func_02002800(0x134b);
    Func_020016ee(15);
    Func_0200277a();
    Func_020027a0(15)->presentation_flags &= 1;
}
