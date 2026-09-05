#include "types.h"

#define SceneDialogue_RunActorFourteenTalk Func_02000b28

extern void Func_02006ae8(void);
extern char Value_00000806;
extern s32 Func_02006ace(s32);
extern void Func_02006bc0(s32);
extern void Func_02006bd8(s32, s32);
extern s32 Func_02006ae8_a(s32);
extern void Func_02006afa(s32);
extern void Func_02006be0(s32);
extern void Func_02006bda(s32, s32, s32);
extern void Func_02006c12(s32, s32);
extern void Func_02006bfa(s32);
extern void Func_02006bf4(s32, s32, s32);
extern void Func_02006c1c(s32, s32);
extern void Func_02006b50(void);

void SceneDialogue_RunActorFourteenTalk(void)
{
    s32 flag = 0x806;
    Func_02006ae8();
    if (Func_02006ace(0x815) != 0) {
        Func_02006bc0(0x11c9);
        Func_02006bd8(14, 0);
    } else if (Func_02006ae8_a(flag) == 0) {
        Func_02006afa(flag);
        Func_02006be0(0xf7c);
        Func_02006bda(14, 0, 4);
        Func_02006c12(14, 0);
    } else {
        Func_02006bfa(0xf7e);
        Func_02006bf4(14, 0, 4);
        Func_02006c1c(14, 0);
    }
    Func_02006b50();
}
