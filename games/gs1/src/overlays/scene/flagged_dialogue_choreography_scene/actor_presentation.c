#include "types.h"

#define SceneDialogue_RunActorEightFlagGatedDialogue Func_0200021c
#define ActorPresentation_ApplyTableA5ecToActorNine Func_020016ac
extern s32 Data_0200a5ec[];

void Func_02002450(void);
s32 Func_02002436(s32);
void Func_02002530(s32);
void Func_02002538(s32);
void Func_02002548(s32, s32);
void Func_0200247c(void);
void Func_0200390c(s32, s32);
void Func_020039c4(s32, s32);

void SceneDialogue_RunActorEightFlagGatedDialogue(void)
{
    Func_02002450();
    if (Func_02002436(3) != 0) {
        Func_02002530(0x1570);
    } else {
        Func_02002538(0x1529);
    }
    {
        s32 val = 0;
        s32 mode = 8;
        Func_02002548(mode, val);
    }
    Func_0200247c();
}

void ActorPresentation_ApplyTableA5ecToActorNine(void)
{
    Func_0200390c(9, (s32)Data_0200a5ec);
    Func_020039c4(9, 0);
}
