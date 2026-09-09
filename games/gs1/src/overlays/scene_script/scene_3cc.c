/*
 * Overlay resource_3cc: in-image table getters and the scene state helpers
 * that pair a value update with a follow-up counter write.
 */

#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

#include "resource_3cc.h"

void Func_02000114(s32);
void Func_02000116(s32, s32, s32);
void Func_0200012c(s32);
void Func_0200012e(s32, s32, s32);
void Func_02000144(s32);
void Func_02000146(s32, s32, s32);
void Func_0200015c(s32);
void Func_0200015e(s32, s32, s32);

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *SceneData_GetTable81a8(void)
{
    return (u8 *)0x020081a8;
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTable81d8(void)
{
    return (u8 *)0x020081d8;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTable81ec(void)
{
    return (u8 *)0x020081ec;
}

void SceneState_ApplyValues8And0And0Then30(void)
{
    Func_02000116(8, 0, 0);
    Func_02000114(0x30);
}

void SceneState_ApplyValues9And1And0Then44(void)
{
    Func_0200012e(9, 1, 0);
    Func_0200012c(0x44);
}

void SceneState_ApplyValues10And2And0Then58(void)
{
    Func_02000146(0xA, 2, 0);
    Func_02000144(0x58);
}

void SceneState_ApplyValues11And3And0Then6c(void)
{
    Func_0200015e(0xB, 3, 0);
    Func_0200015c(0x6C);
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTable8264(void)
{
    return (u8 *)0x02008264;
}

/*
 * Overlay entry point, reached through the loader-relocated call word at
 * image offset 4. It sequences nothing and returns zero.
 */
s32 FieldScene_RunEmptyScene(void)
{
    return 0;
}
