/* Contiguous unnamed leaf-owner run for resource_3b7. */

#include "types.h"

extern void Func_02001a74(void);
extern void Func_02001ac2(s32);
extern s32 Func_02001ad4(s32, s32);
extern void Func_02001a90(void);

#define SceneDialogue_RunMessage0e34 Func_02000154

void SceneDialogue_RunMessage0e34(void)
{
    Func_02001a74();
    Func_02001ac2(0xE34);
    Func_02001ad4(-1, 0);
    Func_02001a90();
}

#include "types.h"

extern void Func_02001a98(void);
extern void Func_02001ae6(s32);
extern s32 Func_02001af8(s32, s32);
extern void Func_02001ab4(void);

#define SceneDialogue_RunMessage0e35 Func_02000178

void SceneDialogue_RunMessage0e35(void)
{
    Func_02001a98();
    Func_02001ae6(0xE35);
    Func_02001af8(-1, 0);
    Func_02001ab4();
}

#include "types.h"

extern void Func_02000a9a(s32);

#define FieldScene_RunIndexedStep0 Func_0200019c

void FieldScene_RunIndexedStep0(void)
{
    Func_02000a9a(0);
}

#include "types.h"

#define SceneData_SelectTable9e1cByState Func_020001a8

extern s16 Data_02000240[];
extern u8 Value_000000bd;
extern u8 Data_02009f30[];
extern u8 Data_02009e1c[];

s32 SceneData_SelectTable9e1cByState(void) {
    if (Data_02000240[224] == (s32)&Value_000000bd) {
        return (s32)Data_02009f30;
    }
    return (s32)Data_02009e1c;
}
