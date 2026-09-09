#include "types.h"

#define SceneDialogue_RunActor181Scene Func_02000a44
#define SceneDialogue_RunActorTenFlaggedDialogue Func_02000adc
#define SceneDialogue_RunActor21FlaggedLine Func_02000c60
#define SceneDialogue_RunActor10LineAndFlag81f Func_02000c9c
#define SceneDialogue_RunActorNineteenDialogue Func_02000d2c
void Func_02006a04(void);
void Func_02006a8e(s32, s32, s32);
void Func_020069fe(s32);
void Func_02006b7e(s32, s32);
s32 Func_02006a3e(s32, s32);
void Func_02006a32(void);
void Func_02006a9c(void);
s32 Func_02006a82(s32);
void Func_02006b74(s32);
void Func_02006b8c(s32, s32);
void Func_02006b84(s32);
void Func_02006b7e_a(s32, s32, s32);
void Func_02006bb6(s32, s32);
void Func_02006ada(void);
void Func_02006c20(void);
s32 Func_02006c06(s32);
void Func_02006cf8(s32);
void Func_02006d00(s32);
void Func_02006d18(s32, s32);
void Func_02006c4c(void);
void Func_02006c5c(void);
void Func_02006d1e(s32, s32, s32);
void Func_02006d34(s32);
s32 Func_02006d4c(s32, s32);
void Func_02006c62(s32);
void Func_02006c86(void);
void Func_02006cec(void);
s32 Func_02006d9c(s32, s32);
void Func_02006cf2(s32);
s32 Func_02006db4(s32, s32, s32);
void Func_02006dd2(s32);
void Func_02006dfa(s32, s32);
void Func_02006d00_a(s32);
void Func_02006d24(void);

void SceneDialogue_RunActor181Scene(void)
{
    Func_02006a04();
    Func_02006a8e(26, 0, 0);
    Func_020069fe(0xfd0);
    Func_02006b7e(0xb5, 3);
    Func_02006a3e(0xb5, 0);
    Func_02006a32();
}

void SceneDialogue_RunActorTenFlaggedDialogue(void)
{
    Func_02006a9c();
    if (Func_02006a82(0x815) != 0) {
        Func_02006b74(0x11cc);
        Func_02006b8c(10, 0);
    } else {
        Func_02006b84(0xf81);
        Func_02006b7e_a(10, 0, 4);
        Func_02006bb6(10, 0);
    }
    Func_02006ada();
}

void SceneDialogue_RunActor21FlaggedLine(void)
{
    Func_02006c20();
    if (Func_02006c06(0x202) != 0) {
        Func_02006cf8(0xf68);
    } else {
        Func_02006d00(0xf69);
    }
    Func_02006d18(21, 0);
    Func_02006c4c();
}

void SceneDialogue_RunActor10LineAndFlag81f(void)
{
    Func_02006c5c();
    Func_02006d1e(0, 10, 20);
    Func_02006d34(0x1c8d);
    Func_02006d4c(10, 0);
    Func_02006c62(0x81f);
    Func_02006c86();
}

void SceneDialogue_RunActorNineteenDialogue(void)
{
    Func_02006cec();
    Func_02006d9c(19, 2);
    Func_02006cf2(20);
    Func_02006db4(19, 0, 20);
    Func_02006dd2(0x1c9d);
    Func_02006dfa(19, 0);
    Func_02006d00_a(0x307);
    Func_02006d24();
}
