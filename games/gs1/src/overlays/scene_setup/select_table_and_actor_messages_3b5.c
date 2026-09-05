/* Contiguous unnamed leaf-owner run for resource_3b5. */

#include "types.h"

extern u8 Data_02009a9c[];
extern u8 Data_020097a8[];
extern u8 Data_020094a8[];

extern s32 Func_020011fc(s32);
extern s32 Func_0200120a(s32);

#define SceneData_SelectTable94a8ByFlags Func_020004b0

u8 *SceneData_SelectTable94a8ByFlags(void)
{
    if (Func_020011fc(0x950) != 0) {
        return Data_02009a9c;
    }
    if (Func_0200120a(0x962) != 0) {
        return Data_020097a8;
    }
    return Data_020094a8;
}

#include "types.h"

extern void Func_02001248(void);
extern void Func_020012e6(s32);
extern s32 Func_0200130e(s32, s32);
extern void Func_02001262(void);

#define SceneDialogue_RunActor15Message1f92 Func_020004e8

void SceneDialogue_RunActor15Message1f92(void)
{
    Func_02001248();
    Func_020012e6(0x1F92);
    Func_0200130e(15, 0);
    Func_02001262();
}

#include "types.h"

#define SceneDialogue_RunActor24Message1f9d Func_02000508

extern void Func_02001268(void);
extern void Func_02001306(s32);
extern s32 Func_0200132e(s32, s32);
extern void Func_02001282(void);

void SceneDialogue_RunActor24Message1f9d(void)
{
    Func_02001268();
    Func_02001306(0x1F9D);
    Func_0200132e(24, 0);
    Func_02001282();
}
