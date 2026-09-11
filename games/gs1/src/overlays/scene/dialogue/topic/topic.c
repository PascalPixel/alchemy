#include "types.h"

/* overlays/scene/dialogue/topic/run_scene_steps.c */
/* Contiguous unnamed leaf-owner run for resource_3b7. */


extern void Func_02001a74(void);
extern void Func_02001ac2(s32);
extern s32 Func_02001ad4(s32, s32);
extern void Func_02001a90(void);

void Dialogue_RunMessage0e34(void)
{
    Func_02001a74();
    Func_02001ac2(0xE34);
    Func_02001ad4(-1, 0);
    Func_02001a90();
}


extern void Func_02001a98(void);
extern void Func_02001ae6(s32);
extern s32 Func_02001af8(s32, s32);
extern void Func_02001ab4(void);

void Dialogue_RunMessage0e35(void)
{
    Func_02001a98();
    Func_02001ae6(0xE35);
    Func_02001af8(-1, 0);
    Func_02001ab4();
}


extern void Func_02000a9a(s32);

void Scene_RunIndexedStep0(void)
{
    Func_02000a9a(0);
}


extern s16 Data_02000240[];
extern u8 Value_000000bd;
extern u8 Data_02009f30[];
extern u8 Data_02009e1c[];

s32 SceneData_SelectTable9e1cByState(void)
{
    if (Data_02000240[224] == (s32)&Value_000000bd) {
        return (s32)Data_02009f30;
    }
    return (s32)Data_02009e1c;
}

/* overlays/scene/dialogue/topic/table_accessors.c */
/* Contiguous unnamed leaf-owner run for resource_3b7. */


/*
 * resource_3b7 owner at 0x0200003c: a leaf that loads its literal pool word
 * and returns it. The eight-byte owner includes that pool word at
 * 0x02000040, holding the address 0x02009ac8 -- image offset 0x1ac8 --
 * returned without being dereferenced. Many rows across the tree share this
 * body but each returns a different address.
 */

u8 *SceneData_GetTable9ac8(void)
{
    return (u8 *)0x02009ac8;
}


extern s16 Data_02000240[];
extern u8 Value_000000bd;
extern u8 Data_02009aec[];
extern u8 Data_02009cfc[];

s32 SceneData_SelectTable9cfcByState(void)
{
    if (Data_02000240[224] == (s32)&Value_000000bd) {
        return (s32)Data_02009aec;
    }
    return (s32)Data_02009cfc;
}
