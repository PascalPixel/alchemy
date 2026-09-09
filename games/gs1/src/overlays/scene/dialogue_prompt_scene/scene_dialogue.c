#include "types.h"

#define SceneDialogue_ShowLine12BB Func_020002a4
#define SceneDialogue_RunActorElevenDialogue Func_02000400
#define SceneDialogue_RunActor9FlaggedLine Func_020004bc
#define SceneDialogue_RunActorTwelveFlaggedDialogue Func_020004f4
#define SceneDialogue_ShowLine128E Func_0200059c
#define SceneDialogue_RunActor10Line Func_020007cc
#define SceneDialogue_RunActor11Line Func_020007ec
#define SceneDialogue_RunActor14Line Func_0200080c
#define SceneDialogue_RunActorNineFlaggedDialogue Func_02000a14
#define SceneDialogue_RunActorElevenFlaggedDialogue Func_02000a4c
#define SceneDialogue_ShowLine124EOr135E Func_02000a84
#define SceneDialogue_RunActor16FlaggedLine Func_02000abc
#define SceneDialogue_RunActorEighteenBranchedDialogue Func_02000af4
void Func_0200513c(void);
void Func_020050c2(s32);
void Func_020050da(s32, s32);
void Func_02005148(void);
void Func_0200521e(s32);
void Func_020051e6(s32, s32);
void Func_020007d4(s32);
void Func_02005168(void);
void Func_02005204(void);
s32 Func_020051da(s32);
void Func_020052e4(s32);
void Func_020052ec(s32);
void Func_02000832(s32);
void Func_0200522e(void);
void Func_0200523c(void);
s32 Func_02005212(s32);
void Func_0200531c(s32);
void Func_02005324(s32);
void Func_0200086a(s32);
void Func_02005266(void);
void Func_020052e4_a(void);
void Func_020053ba(s32);
void Func_02000900(s32);
void Func_020052fc(void);
void Func_02005514(void);
void Func_020055ea(s32);
void Func_02000b98(s32);
void Func_0200552c(void);
void Func_02005534(void);
void Func_0200560a(s32);
void Func_02000b50(s32);
void Func_0200554c(void);
void Func_02005554(void);
void Func_0200562a(s32);
void Func_02000bd8(s32);
void Func_0200556c(void);
void Func_0200575c(void);
s32 Func_02005732(s32);
void Func_0200583c(s32);
void Func_02005844(s32);
void Func_02000dae(s32);
void Func_02005786(void);
void Func_02005794(void);
s32 Func_0200576a(s32);
void Func_02005874(s32);
void Func_0200587c(s32);
void Func_02000de6(s32);
void Func_020057be(void);
void Func_020057cc(void);
s32 Func_020057a2(s32);
void Func_020058ac(s32);
void Func_020058b4(s32);
void Func_02000e1e(s32);
void Func_020057f6(void);
void Func_02005804(void);
s32 Func_020057da(s32);
void Func_020058e4(s32);
void Func_020058ec(s32);
void Func_02000e56(s32);
void Func_0200582e(void);
void Func_0200583c_a(void);
s32 Func_02005812(s32);
void Func_0200591c(s32);
s32 Func_02005824(s32);
void Func_0200592e(s32);
void Func_02005936(s32);
void Func_02000ea0(s32);
void Func_02005878(void);

void SceneDialogue_ShowLine12BB(void)
{
    Func_0200513c();
    Func_020050c2(0x12bb);
    Func_020050da(1, 0);
}

void SceneDialogue_RunActorElevenDialogue(void)
{
    Func_02005148();
    Func_0200521e(0x1247);
    Func_020051e6(11, 1);
    Func_020007d4(11);
    Func_02005168();
}

void SceneDialogue_RunActor9FlaggedLine(void)
{
    Func_02005204();
    if (Func_020051da(0x855) == 0) {
        Func_020052e4(0x1243);
    } else {
        Func_020052ec(0x1353);
    }
    Func_02000832(9);
    Func_0200522e();
}

void SceneDialogue_RunActorTwelveFlaggedDialogue(void)
{
    Func_0200523c();
    if (Func_02005212(0x855) != 0) {
        Func_0200531c(0x135c);
    } else {
        Func_02005324(0x124c);
    }
    Func_0200086a(12);
    Func_02005266();
}

void SceneDialogue_ShowLine128E(void)
{
    Func_020052e4_a();
    Func_020053ba(0x128e);
    Func_02000900(18);
    Func_020052fc();
}

void SceneDialogue_RunActor10Line(void)
{
    Func_02005514();
    Func_020055ea(0x1356);
    Func_02000b98(10);
    Func_0200552c();
}

void SceneDialogue_RunActor11Line(void)
{
    Func_02005534();
    Func_0200560a(0x1359);
    Func_02000b50(11);
    Func_0200554c();
}

void SceneDialogue_RunActor14Line(void)
{
    Func_02005554();
    Func_0200562a(0x1368);
    Func_02000bd8(14);
    Func_0200556c();
}

void SceneDialogue_RunActorNineFlaggedDialogue(void)
{
    Func_0200575c();
    if (Func_02005732(0x855) == 0) {
        Func_0200583c(0x1245);
    } else {
        Func_02005844(0x1355);
    }
    Func_02000dae(9);
    Func_02005786();
}

void SceneDialogue_RunActorElevenFlaggedDialogue(void)
{
    Func_02005794();
    if (Func_0200576a(0x855) == 0) {
        Func_02005874(0x124b);
    } else {
        Func_0200587c(0x135b);
    }
    Func_02000de6(11);
    Func_020057be();
}

void SceneDialogue_ShowLine124EOr135E(void)
{
    Func_020057cc();
    if (Func_020057a2(0x855) == 0) {
        Func_020058ac(0x124e);
    } else {
        Func_020058b4(0x135e);
    }
    Func_02000e1e(12);
    Func_020057f6();
}

void SceneDialogue_RunActor16FlaggedLine(void)
{
    Func_02005804();
    if (Func_020057da(0x855) == 0) {
        Func_020058e4(0x127c);
    } else {
        Func_020058ec(0x136c);
    }
    Func_02000e56(16);
    Func_0200582e();
}

void SceneDialogue_RunActorEighteenBranchedDialogue(void)
{
    Func_0200583c_a();
    if (Func_02005812(0x855) == 0) {
        Func_0200591c(0x1294);
    } else if (Func_02005824(0x85b) == 0) {
        Func_0200592e(0x1382);
    } else {
        Func_02005936(0x1cf4);
    }
    Func_02000ea0(18);
    Func_02005878();
}
