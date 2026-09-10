#include "types.h"

#define SceneDialogue_ShowLine1B9C Func_02000738
#define SceneDialogue_RunActor9Line Func_02000844
#define SceneDialogue_ShowLine1BAA Func_02000864
#define SceneDialogue_ShowLine1BB0 Func_02000884
#define SceneDialogue_RunActor10Scene Func_020008a4
#define SceneDialogue_RunActorTwelveDialogue Func_020008cc
#define SceneDialogue_RunActorNineFlaggedDialogue Func_02000a90
extern u8 Data_00001bc0[];

void Func_0200211c(void);
void Func_020021e8(s32, s32, s32);
void Func_020021be(s32);
s32 Func_020021de(s32, s32);
void Func_02002142(void);
void Func_02002228(void);
void Func_020022be(s32);
s32 Func_020022de(s32, s32);
void Func_02002242(void);
void Func_02002248(void);
void Func_020022de_a(s32);
s32 Func_020022fe(s32, s32);
void Func_02002262(void);
void Func_02002268(void);
void Func_020022fe_a(s32);
s32 Func_0200231e(s32, s32);
void Func_02002282(void);
void Func_02002288(void);
void Func_02002308(s32, s32);
void Func_02002326(s32);
void Func_0200233e(s32, s32);
void Func_020022aa(void);
void Func_020022b0(void);
void Func_02002346(s32);
s32 Func_02002366(s32, s32);
void Func_020022ca(void);
s32 Func_02002456(s32);
void Func_02002510(s32);
void Func_02002518(s32);
void Func_02002530(s32, s32);

/* Returns a value: the reference sets r1 before r0 at this site, which
   only a value-returning callee does; the result is unused here. */

void SceneDialogue_ShowLine1B9C(void)
{
    Func_0200211c();
    Func_020021e8(17, 0x102, 60);
    Func_020021be(0x1b9c);
    Func_020021de(17, 0);
    Func_02002142();
}

void SceneDialogue_RunActor9Line(void) { Func_02002228(); Func_020022be(0x1ba6); Func_020022de(9, 0); Func_02002242(); }

void SceneDialogue_ShowLine1BAA(void) { Func_02002248(); Func_020022de_a(0x1baa); Func_020022fe(11, 0); Func_02002262(); }

void SceneDialogue_ShowLine1BB0(void) { Func_02002268(); Func_020022fe_a(0x1bb0); Func_0200231e(15, 0); Func_02002282(); }

void SceneDialogue_RunActor10Scene(void) { Func_02002288(); Func_02002308(10, 3); Func_02002326(0x24d1); Func_0200233e(10, 0); Func_020022aa(); }

void SceneDialogue_RunActorTwelveDialogue(void) { Func_020022b0(); Func_02002346(0x24d3); Func_02002366(12, 0); Func_020022ca(); }

void SceneDialogue_RunActorNineFlaggedDialogue(void)
{
    if (Func_02002456(0x941) != 0)
        Func_02002510(0x24e8);
    else
        Func_02002518((s32)Data_00001bc0);
    Func_02002530(9, 0);
}
