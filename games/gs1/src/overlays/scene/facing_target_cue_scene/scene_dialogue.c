#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SceneDialogue_ShowLine1C13WithActor16Steps Func_02000ef8
#define SceneDialogue_RunActorThirteenDialogue Func_02000f38
#define SceneDialogue_RunActor16LineAndFlag81c Func_02000f64

void Func_02002808();
void Func_0200281a(s32);
void Func_0200283e();
void Func_020028aa(s32, s32, s32);
void Func_020028d0(s32);
void Func_020028e8(s32, s32);
void Func_0200290c(s32, s32, s32);
void Func_02002848(void);
void Func_02002906(s32);
s32 Func_0200291e(s32, s32);
void Func_02002844(s32);
void Func_02002868(void);
void Func_02002874(void);
void Func_02002932(s32);
s32 Func_0200294a(s32, s32);
void Func_02002870(s32);
void Func_02002894(void);

void SceneDialogue_ShowLine1C13WithActor16Steps(void) {
    Func_02002808();
    Func_020028aa(0x10, 0, 0xA);
    Func_020028d0(0x1C13);
    Func_020028e8(0x10, 0);
    Func_0200290c(0x10, 0xB000, 0xA);
    Func_0200281a(0x301);
    Func_0200283e();
}

void SceneDialogue_RunActorThirteenDialogue(void)
{
    Func_02002848();
    Func_02002906(0x1C1B);
    Func_0200291e(0xD, 0);
    Func_02002844(0x81C);
    Func_02002868();
}

void SceneDialogue_RunActor16LineAndFlag81c(void)
{
    Func_02002874();
    Func_02002932(0x1C1A);
    Func_0200294a(0x10, 0);
    Func_02002870(0x81C);
    Func_02002894();
}
