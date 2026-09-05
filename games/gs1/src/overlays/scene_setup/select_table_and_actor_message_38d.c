/* Contiguous unnamed leaf-owner run for resource_38d. */

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000021;
extern u8 Data_0200aca8[];
extern u8 Data_0200ac9c[];

#define SceneData_SelectTableac9cByState Func_02000100

s32 SceneData_SelectTableac9cByState(void) {
    if (Data_02000240[224] == (s32)&Value_00000021) {
        return (s32)Data_0200aca8;
    }
    return (s32)Data_0200ac9c;
}

#include "types.h"

#define SceneDialogue_RunActor10Message1420 Func_02000130

extern u8 Value_00001420;

extern void Func_020025a4(void);
extern void Func_02002672(s32);
extern s32 Func_0200269a(s32, s32);
extern void Func_020025be(void);

void SceneDialogue_RunActor10Message1420(void)
{
    Func_020025a4();
    Func_02002672((s32)&Value_00001420);
    Func_0200269a(10, 0);
    Func_020025be();
}
