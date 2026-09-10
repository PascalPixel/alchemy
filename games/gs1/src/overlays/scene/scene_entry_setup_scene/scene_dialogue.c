#include "types.h"

#define SceneDialogue_RunActorElevenDialogue Func_02000fbc
#define SceneDialogue_RunLine2682 Func_02001024
void Func_02005e54(void);
void Func_02005f0a(s32);
void Func_02005f24(s32, s32, s32);
void Func_02005eec(s32, s32);
void Func_02005f2c(s32, s32);
void Func_02005e80(void);
void Func_02005ebc(void);
void Func_02005f2c_a(s32, s32);
void Func_02005ea4(s32, s32);
void Func_02005ed8(void);

void SceneDialogue_RunActorElevenDialogue(void)
{
    Func_02005e54();
    Func_02005f0a(0x2670);
    Func_02005f24(11, 0, 20);
    Func_02005eec(11, 2);
    Func_02005f2c(11, 0);
    Func_02005e80();
}

void SceneDialogue_RunLine2682(void)
{
    Func_02005ebc();
    Func_02005f2c_a(0, 1);
    Func_02005ea4(0x2682, 1);
    Func_02005ed8();
}
