#include "types.h"

#define SceneState_RunFlag200SetupAndPlaceActors16To20 Func_02000c2c

extern s16 Data_02000240;
extern u8 Value_00000097;

extern s32 Func_020048c0(s32);
extern void Func_02001770(void);
extern void Func_020049ba(s32, s32);
extern void Func_020049c2(s32, s32);
extern void Func_020049ca(s32, s32);
extern void Func_020049d2(s32, s32);
extern void Func_020049da(s32, s32);

void SceneState_RunFlag200SetupAndPlaceActors16To20(void)
{
    u8 *work = *(u8 **)0x03001f30;
    s16 *tbl;

    if (Func_020048c0(0x200) != 0) {
        Func_02001770();
        work[0x34] = 1;
    }
    tbl = &Data_02000240;
    if (tbl[0xe0] == (s32)&Value_00000097) {
        Func_020049ba(16, 6);
        Func_020049c2(17, 6);
        Func_020049ca(18, 6);
        Func_020049d2(19, 6);
        Func_020049da(20, 6);
    }
}
