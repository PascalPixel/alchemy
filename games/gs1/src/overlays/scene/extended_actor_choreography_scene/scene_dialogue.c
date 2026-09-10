#include "types.h"

#define SceneDialogue_RunActor12Line Func_02000708
#define SceneDialogue_ShowLine1E19Or1D50 Func_020008a8
#define SceneDialogue_ShowLine1ECETo1ED0 Func_02001324
#define SceneDialogue_RunActor19TwoFlagLineA Func_02001378
#define SceneDialogue_RunActor20TwoFlagLine Func_020013cc
#define SceneDialogue_ShowLine1ED1Or1ED2 Func_02001420
#define SceneDialogue_RunActor22TwoFlagLine Func_0200145c
#define SceneDialogue_RunActor23BranchedDialogue Func_020014b0
#define SceneDialogue_RunActor24BranchedDialogue Func_02001504
#define SceneDialogue_RunActor25FlaggedLine Func_02001558
#define SceneDialogue_RunActor18TwoFlagLine Func_02001594
#define SceneDialogue_RunActor19TwoFlagLineB Func_020015e8
#define SceneDialogue_ShowLine1EDBTo1EDDActor20 Func_0200163c
#define SceneDialogue_RunActor21FlaggedLine Func_02001690
#define SceneDialogue_RunActor22BranchedDialogue Func_020016cc
#define SceneDialogue_ShowLine1EDBTo1EDDActor23 Func_02001720
#define SceneDialogue_ShowLine1EDBTo1EDDActor24 Func_02001774
#define SceneDialogue_ShowLine1EDEOr1EDF Func_020017c8
extern u8 Value_00001dd1;
extern u8 Value_00000925;
extern u8 Value_00001e19;
extern u8 Value_00001d50;

void Func_02006bd4(void);
void Func_02006cba(s32 n);
s32 Func_02006ce2(s32 a, s32 b);
void Func_02006bee(void);
void Func_02006d74(void);
s32 Func_02006d52(s32 n);
void Func_02006e64(s32 n);
void Func_02006e74(s32 n);
void Func_02006e8c(s32 a, s32 b);
void Func_02006e8c_a(s32 a, s32 b);
void Func_02006da8(void);
void Func_020077f0();
void Func_0200782e();
int Func_020077ce();
int Func_020077e0();
void Func_020078e0();
void Func_020078f2();
void Func_020078fa();
void Func_02007912();
void Func_02007844();
void Func_02007882();
int Func_02007822();
int Func_02007834();
void Func_02007934();
void Func_02007946();
void Func_0200794e();
void Func_02007966();
void Func_02007898();
void Func_020078d6();
int Func_02007876();
int Func_02007888();
void Func_02007988();
void Func_0200799a();
void Func_020079a2();
void Func_020079ba();
void Func_020078ec();
void Func_02007918();
int Func_020078ca();
void Func_020079dc();
void Func_020079e4();
void Func_020079fc();
void Func_02007928();
void Func_02007968();
int Func_02007908();
int Func_0200791a();
void Func_02007a1a();
void Func_02007a2c();
void Func_02007a34();
void Func_02007a4c();
void Func_0200797c();
void Func_020079ba_a();
int Func_0200795a();
int Func_0200796c();
void Func_02007a6c();
void Func_02007a7e();
void Func_02007a86();
void Func_02007a9e();
void Func_020079d0();
void Func_02007a0e();
int Func_020079ae();
int Func_020079c0();
void Func_02007ac0();
void Func_02007ad2();
void Func_02007ada();
void Func_02007af2();
void Func_02007a24();
void Func_02007a50();
int Func_02007a02();
void Func_02007b14();
void Func_02007b1c();
void Func_02007b34();
void Func_02007a60();
void Func_02007a9e_a();
int Func_02007a3e();
int Func_02007a50_a();
void Func_02007b50();
void Func_02007b62();
void Func_02007b6a();
void Func_02007b82();
void Func_02007ab4();
void Func_02007af2_a();
int Func_02007a92();
int Func_02007aa4();
void Func_02007ba4();
void Func_02007bb6();
void Func_02007bbe();
void Func_02007bd6();
void Func_02007b08();
void Func_02007b46();
int Func_02007ae6();
int Func_02007af8();
void Func_02007bf8();
void Func_02007c0a();
void Func_02007c12();
void Func_02007c2a();
void Func_02007b5c();
void Func_02007b88();
int Func_02007b3a();
void Func_02007c4c();
void Func_02007c54();
void Func_02007c6c();
void Func_02007b98();
void Func_02007bd8();
int Func_02007b78();
int Func_02007b8a();
void Func_02007c8a();
void Func_02007c9c();
void Func_02007ca4();
void Func_02007cbc();
void Func_02007bec();
void Func_02007c2a_a();
int Func_02007bca();
int Func_02007bdc();
void Func_02007cdc();
void Func_02007cee();
void Func_02007cf6();
void Func_02007d0e();
void Func_02007c40();
void Func_02007c7e();
int Func_02007c1e();
int Func_02007c30();
void Func_02007d30();
void Func_02007d42();
void Func_02007d4a();
void Func_02007d62();
void Func_02007c94();
void Func_02007cc0();
int Func_02007c72();
void Func_02007d84();
void Func_02007d8c();
void Func_02007da4();

void SceneDialogue_RunActor12Line(void)
{
    Func_02006bd4();
    Func_02006cba((s32)&Value_00001dd1);
    Func_02006ce2(12, 0);
    Func_02006bee();
}

void SceneDialogue_ShowLine1E19Or1D50(void)
{
    Func_02006d74();
    if (Func_02006d52((s32)&Value_00000925) != 0) {
        Func_02006e64((s32)&Value_00001e19);
        Func_02006e8c(10, 0);
    } else {
        Func_02006e74((s32)&Value_00001d50);
        Func_02006e8c_a(10, 0);
    }
    Func_02006da8();
}

/* Story selector owner at 0x02001324, 84 bytes; eight calls. Per-site call
 * veneers (raw asm confirms each callee slot uses a distinct local stub
 * even across the three near-identical "twin" owners at 0x1324/1378/13cc). */
void SceneDialogue_ShowLine1ECETo1ED0(void)
{
    Func_020077f0();
    if (Func_020077ce(0x92c)) Func_020078e0(0x1ece);
    else if (Func_020077e0(0x935)) Func_020078f2(0x1ecf);
    else Func_020078fa(0x1ed0);
    Func_02007912(0x12, 0); Func_0200782e();
}

/* Story selector owner at 0x02001378, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x13cc with distinct local stub addresses). */
void SceneDialogue_RunActor19TwoFlagLineA(void)
{
    Func_02007844();
    if (Func_02007822(0x92d)) Func_02007934(0x1ece);
    else if (Func_02007834(0x936)) Func_02007946(0x1ecf);
    else Func_0200794e(0x1ed0);
    Func_02007966(0x13, 0); Func_02007882();
}

/* Story selector owner at 0x020013cc, 84 bytes; eight calls. Per-site call
 * veneers (twin of 0x1324/0x1378 with distinct local stub addresses). */
void SceneDialogue_RunActor20TwoFlagLine(void)
{
    Func_02007898();
    if (Func_02007876(0x92e)) Func_02007988(0x1ece);
    else if (Func_02007888(0x937)) Func_0200799a(0x1ecf);
    else Func_020079a2(0x1ed0);
    Func_020079ba(0x14, 0); Func_020078d6();
}

/* Story selector owner at 0x02001420, 60 bytes; six calls. */
void SceneDialogue_ShowLine1ED1Or1ED2(void)
{
    Func_020078ec();
    if (Func_020078ca(0x92f)) Func_020079dc(0x1ed1);
    else Func_020079e4(0x1ed2);
    Func_020079fc(21, 0); Func_02007918();
}

/* Story selector owner at 0x0200145c, 84 bytes; eight calls. */
void SceneDialogue_RunActor22TwoFlagLine(void)
{
    Func_02007928();
    if (Func_02007908(0x930)) Func_02007a1a(0x1ece);
    else if (Func_0200791a(0x939)) Func_02007a2c(0x1ecf);
    else Func_02007a34(0x1ed0);
    Func_02007a4c(22, 0); Func_02007968();
}

/* Story selector owner at 0x020014b0, 84 bytes; eight calls. */
void SceneDialogue_RunActor23BranchedDialogue(void)
{
    Func_0200797c();
    if (Func_0200795a(0x931)) Func_02007a6c(0x1ece);
    else if (Func_0200796c(0x93a)) Func_02007a7e(0x1ecf);
    else Func_02007a86(0x1ed0);
    Func_02007a9e(23, 0); Func_020079ba_a();
}

/* Story selector owner at 0x02001504, 84 bytes; eight calls. */
void SceneDialogue_RunActor24BranchedDialogue(void)
{
    Func_020079d0();
    if (Func_020079ae(0x932)) Func_02007ac0(0x1ece);
    else if (Func_020079c0(0x93b)) Func_02007ad2(0x1ecf);
    else Func_02007ada(0x1ed0);
    Func_02007af2(24, 0); Func_02007a0e();
}

/* Story selector owner at 0x02001558, 60 bytes; six calls. */
void SceneDialogue_RunActor25FlaggedLine(void)
{
    Func_02007a24();
    if (Func_02007a02(0x933)) Func_02007b14(0x1ed1);
    else Func_02007b1c(0x1ed2);
    Func_02007b34(25, 0); Func_02007a50();
}

/* Second-phase story selector at 0x02001594, 84 bytes; eight calls. */
void SceneDialogue_RunActor18TwoFlagLine(void)
{
    Func_02007a60();
    if (Func_02007a3e(0x92c)) Func_02007b50(0x1edb);
    else if (Func_02007a50_a(0x935)) Func_02007b62(0x1edc);
    else Func_02007b6a(0x1edd);
    Func_02007b82(18, 0); Func_02007a9e_a();
}

/* Second-phase story selector at 0x020015e8, 84 bytes; eight calls. */
void SceneDialogue_RunActor19TwoFlagLineB(void)
{
    Func_02007ab4();
    if (Func_02007a92(0x92d)) Func_02007ba4(0x1edb);
    else if (Func_02007aa4(0x936)) Func_02007bb6(0x1edc);
    else Func_02007bbe(0x1edd);
    Func_02007bd6(19, 0); Func_02007af2_a();
}

/* Second-phase story selector at 0x0200163c, 84 bytes; eight calls. */
void SceneDialogue_ShowLine1EDBTo1EDDActor20(void)
{
    Func_02007b08();
    if (Func_02007ae6(0x92e)) Func_02007bf8(0x1edb);
    else if (Func_02007af8(0x937)) Func_02007c0a(0x1edc);
    else Func_02007c12(0x1edd);
    Func_02007c2a(20, 0); Func_02007b46();
}

/* Second-phase story selector at 0x02001690, 60 bytes; six calls. */
void SceneDialogue_RunActor21FlaggedLine(void)
{
    Func_02007b5c();
    if (Func_02007b3a(0x92f)) Func_02007c4c(0x1ede);
    else Func_02007c54(0x1edf);
    Func_02007c6c(21, 0); Func_02007b88();
}

/* Second-phase story selector at 0x020016cc, 84 bytes; eight calls. */
void SceneDialogue_RunActor22BranchedDialogue(void)
{
    Func_02007b98();
    if (Func_02007b78(0x930)) Func_02007c8a(0x1edb);
    else if (Func_02007b8a(0x939)) Func_02007c9c(0x1edc);
    else Func_02007ca4(0x1edd);
    Func_02007cbc(22, 0); Func_02007bd8();
}

/* Second-phase story selector at 0x02001720, 84 bytes; eight calls. */
void SceneDialogue_ShowLine1EDBTo1EDDActor23(void)
{
    Func_02007bec();
    if (Func_02007bca(0x931)) Func_02007cdc(0x1edb);
    else if (Func_02007bdc(0x93a)) Func_02007cee(0x1edc);
    else Func_02007cf6(0x1edd);
    Func_02007d0e(23, 0); Func_02007c2a_a();
}

/* Second-phase story selector at 0x02001774, 84 bytes; eight calls. */
void SceneDialogue_ShowLine1EDBTo1EDDActor24(void)
{
    Func_02007c40();
    if (Func_02007c1e(0x932)) Func_02007d30(0x1edb);
    else if (Func_02007c30(0x93b)) Func_02007d42(0x1edc);
    else Func_02007d4a(0x1edd);
    Func_02007d62(24, 0); Func_02007c7e();
}

/* Second-phase story selector at 0x020017c8, 60 bytes; six calls. */
void SceneDialogue_ShowLine1EDEOr1EDF(void)
{
    Func_02007c94();
    if (Func_02007c72(0x933)) Func_02007d84(0x1ede);
    else Func_02007d8c(0x1edf);
    Func_02007da4(25, 0); Func_02007cc0();
}
