#include "types.h"

/*
 * resource_3b7 owner at 0x02000030: a leaf that loads its literal pool word
 * and returns it. The eight-byte owner includes that pool word at
 * 0x02000034, holding the address 0x02009a08 -- image offset 0x1a08 --
 * returned without being dereferenced. Many rows across the tree share this
 * body but each returns a different address.
 */



#include "types.h"



/* Contiguous unnamed leaf-owner run for resource_3b7. */

#include "types.h"

/*
 * resource_3b7 owner at 0x0200003c: a leaf that loads its literal pool word
 * and returns it. The eight-byte owner includes that pool word at
 * 0x02000040, holding the address 0x02009ac8 -- image offset 0x1ac8 --
 * returned without being dereferenced. Many rows across the tree share this
 * body but each returns a different address.
 */



#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000bd;
extern u8 Data_02009aec[];
extern u8 Data_02009cfc[];



#include "types.h"

extern u8 Value_00000e39;
extern void Func_020019e2(s32);
extern s32 Func_020019f2(s32, s32);
extern s32 Func_020019c2(s32, s32);
extern void Func_020019ac(s32);
extern void Func_02001a02(s32);
extern void Func_02001a0a_cue(s32);
extern void Func_02001a22(s32, s32);



#include "types.h"

extern u8 Value_00000e19;
extern void Func_02001a2a(s32);
extern s32 Func_02001a3a(s32, s32);
extern s32 Func_02001a0a_prompt(s32, s32);
extern void Func_020019f4(s32);
extern void Func_02001a4a(s32);
extern void Func_02001a52(s32);
extern void Func_02001a6a(s32, s32);



/* Contiguous unnamed leaf-owner run for resource_3b7. */

#include "types.h"

extern void Func_02001a74(void);
extern void Func_02001ac2(s32);
extern s32 Func_02001ad4(s32, s32);
extern void Func_02001a90(void);



#include "types.h"

extern void Func_02001a98(void);
extern void Func_02001ae6(s32);
extern s32 Func_02001af8(s32, s32);
extern void Func_02001ab4(void);



#include "types.h"

extern void Func_02000a9a(s32);



#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000bd;
extern u8 Data_02009f30[];
extern u8 Data_02009e1c[];



u8 *SceneData_GetTable9A08(void)
{
    return (u8 *)0x02009a08;
}

s32 Func_02000038(void)
{
    return 0;
}

u8 *SceneData_GetTable9ac8(void)
{
    return (u8 *)0x02009ac8;
}

s32 SceneData_SelectTable9cfcByState(void)
{
    if (Data_02000240[224] == (s32)&Value_000000bd) {
        return (s32)Data_02009aec;
    }
    return (s32)Data_02009cfc;
}

void FieldScene_RunActorCueBranch(s32 object)
{
    s32 cue = (s32)&Value_00000e39;
    Func_020019e2(cue);
    Func_020019f2(object, 0);
    if (Func_020019c2(0, 0) == 0) {
        Func_020019ac(10);
        Func_02001a02(cue + 1);
    } else {
        Func_02001a0a_cue(cue + 2);
    }
    Func_02001a22(object, 0);
}

void FieldScene_RunPromptDialogueE19(s32 object)
{
    s32 cue = (s32)&Value_00000e19;
    Func_02001a2a(cue);
    Func_02001a3a(object, 0);
    if (Func_02001a0a_prompt(0, 0) == 0) {
        Func_020019f4(10);
        Func_02001a4a(cue + 1);
    } else {
        Func_02001a52(cue + 2);
    }
    Func_02001a6a(object, 0);
}

void SceneDialogue_RunMessage0e34(void)
{
    Func_02001a74();
    Func_02001ac2(0xE34);
    Func_02001ad4(-1, 0);
    Func_02001a90();
}

void SceneDialogue_RunMessage0e35(void)
{
    Func_02001a98();
    Func_02001ae6(0xE35);
    Func_02001af8(-1, 0);
    Func_02001ab4();
}

void FieldScene_RunIndexedStep0(void)
{
    Func_02000a9a(0);
}

s32 SceneData_SelectTable9e1cByState(void)
{
    if (Data_02000240[224] == (s32)&Value_000000bd) {
        return (s32)Data_02009f30;
    }
    return (s32)Data_02009e1c;
}
