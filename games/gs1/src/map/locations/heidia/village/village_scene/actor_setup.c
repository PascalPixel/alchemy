#include "types.h"

#define ConfigureSceneActor9 Func_020012e0
#define ConfigureInteractionRegionA Func_0200226c
#define ConfigureInteractionRegionB Func_020022a0
#define ConfigureInteractionRegionC Func_020022d4
#define ConfigurePrimaryInteractionRegions Func_02002410
#define ConfigureSecondaryInteractionRegions Func_02002468
#define ConfigureSceneActor26 Func_02004b68
#define ConfigureSceneActor14 Func_02004b84
#define ConfigureSceneActor13 Func_02004ba4
#define ConfigureSceneActor12Variant Func_02004bc4
#define ConfigureSceneActor18 Func_02004be4
#define ConfigureActor13Interaction Func_02004d50
#define ConfigureActor13SceneResource Func_02004d88
extern u8 Value_00002440;
extern u8 Value_00002459;
extern u8 Value_0000256c;

void Func_02006914(void);
void Func_0200695a(s32);
void Func_02006984(s32, s32, s32);
void Func_020069a4(s32, s32);
void Func_020069d6(s32, s32, s32);
void Func_02006a2a(s32, s32, s32);
void Func_02002e3c(s32);
void Func_02006954(void);
void Func_02007852(s32, s32, s32, s32, s32, s32);
void Func_0200786c(s32, s32, s32, s32, s32, s32);
void Func_02007886(s32, s32, s32, s32, s32, s32);
void Func_020078a0(s32, s32, s32, s32, s32, s32);
void Func_020078ba(s32, s32, s32, s32, s32, s32);
void Func_020078d4(s32, s32, s32, s32, s32, s32);
void Func_020079f6(s32, s32, s32, s32, s32, s32);
void Func_02007a0a(s32, s32, s32, s32, s32, s32);
void Func_02007a24(s32, s32, s32, s32, s32, s32);
void Func_02007a36(s32, s32, s32, s32, s32, s32);
void Func_02007a4e(s32, s32, s32, s32, s32, s32);
void Func_02007a62(s32, s32, s32, s32, s32, s32);
void Func_02007a7c(s32, s32, s32, s32, s32, s32);
void Func_02007a8e(s32, s32, s32, s32, s32, s32);
void Func_0200a30a(s32, s32, s32);
void Func_0200a190(s32);
void Func_0200a254(s32, s32);
void Func_0200a27a(s32);
void Func_0200a292(s32, s32);
void Func_0200a274(s32, s32);
void Func_0200a29a(s32);
void Func_0200a2b2(s32, s32);
void Func_0200a294(s32, s32);
void Func_0200a2ba(s32);
void Func_0200a2d2(s32, s32);
void Func_0200a2d2_a(s32);
s32 Func_0200a2f2(s32, s32);
void Func_0200a440(s32);
void Func_0200a458(s32, s32);
s32 Func_0200a36e(s32);
void Func_0200a376(s32, s32);
void Func_0200a476(s32);
void Func_0200a48e(s32, s32);

void ConfigureSceneActor9(void)
{
    Func_02006914();
    Func_0200695a(9);
    Func_02006984(9, 0, 0);
    Func_020069a4(9, 0);
    Func_020069d6(9, 0, 0);
    Func_02006a2a(9, 256, 0);
    Func_02002e3c(10);
    Func_02006954();
}

void ConfigureInteractionRegionA(void)
{
    Func_02007852(2, 82, 1, 2, 21, 81);
    Func_0200786c(21, 32, 1, 1, 21, 34);
}

void ConfigureInteractionRegionB(void)
{
    Func_02007886(2, 84, 1, 2, 6, 55);
    Func_020078a0(5, 9, 1, 1, 6, 10);
}

void ConfigureInteractionRegionC(void)
{
    Func_020078ba(2, 86, 1, 2, 27, 62);
    Func_020078d4(26, 16, 1, 1, 27, 17);
}

void ConfigurePrimaryInteractionRegions(void)
{
    Func_020079f6(5, 77, 1, 2, 17, 82);
    Func_02007a0a(5, 77, 1, 2, 3, 55);
    Func_02007a24(15, 33, 1, 1, 17, 35);
    Func_02007a36(3, 8, 1, 1, 3, 10);
}

void ConfigureSecondaryInteractionRegions(void)
{
    Func_02007a4e(8, 77, 1, 2, 17, 82);
    Func_02007a62(8, 77, 1, 2, 3, 55);
    Func_02007a7c(18, 35, 1, 1, 17, 35);
    Func_02007a8e(2, 10, 1, 1, 3, 10);
}

void ConfigureSceneActor26(void)
{
    Func_0200a30a(26, 1, 5);
    Func_0200a190(0x94e);
}

void ConfigureSceneActor14(void)
{
    Func_0200a254(14, 2);
    Func_0200a27a(0x2441);
    Func_0200a292(14, 0);
}

void ConfigureSceneActor13(void)
{
    Func_0200a274(13, 2);
    Func_0200a29a((s32)&Value_00002440);
    Func_0200a2b2(13, 0);
}

void ConfigureSceneActor12Variant(void)
{
    Func_0200a294(12, 2);
    Func_0200a2ba(0x243f);
    Func_0200a2d2(12, 0);
}

void ConfigureSceneActor18(void)
{
    Func_0200a2d2_a((s32)&Value_00002459);
    Func_0200a2f2(18, 0);
}

void ConfigureActor13Interaction(void)
{
    u8 *interaction_resources = &Value_0000256c;

    Func_0200a440((s32)interaction_resources);
    Func_0200a458(0x800d, 0);
    if (Func_0200a36e(234) != -1) {
        Func_0200a376((s32)(interaction_resources + 2), 1);
    }
}

void ConfigureActor13SceneResource(void)
{
    Func_0200a476(0x256d);
    Func_0200a48e(13, 0);
}
