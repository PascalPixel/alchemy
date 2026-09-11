#include "types.h"
#include "scene.h"

/* overlays/scene/dialogue/topic/run_scene_steps.c */
/* Contiguous unnamed leaf-owner run for resource_3b7. */

void Dialogue_RunMessage0e34(void)
{
    Talk_Run();
    Talk_Do(0xE34);
    Talk_Apply(-1, 0);
    Talk_unk2_4();
}

void Dialogue_RunMessage0e35(void)
{
    Talk_unk3_4();
    Talk_unk2_2(0xE35);
    Talk_Apply2(-1, 0);
    Talk_unk4_4();
}

void Scene_RunIndexedStep0(void)
{
    Talk_unk3_2(0);
}

extern s16 gCell[];
extern u8 Value_000000bd;
extern u8 gOv[];
extern u8 gOv2[];

s32 SceneData_SelectTable9e1cByState(void)
{
    if (gCell[224] == (s32)&Value_000000bd) {
        return (s32)gOv;
    }
    return (s32)gOv2;
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

extern s16 gCell[];
extern u8 Value_000000bd;
extern u8 gOv3[];
extern u8 gOv4[];

s32 SceneData_SelectTable9cfcByState(void)
{
    if (gCell[224] == (s32)&Value_000000bd) {
        return (s32)gOv3;
    }
    return (s32)gOv4;
}
