#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/data/empty.h"


#include "resource_3cc.h"

/*
 * Overlay resource_3cc: in-image table getters and the scene state helpers
 * that pair a value update with a follow-up counter write.
 */

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

void State_ApplyValues8And0And0Then30(void)
{
    SceneData_Place(8, 0, 0);
    SceneData_Do(0x30);
}

void State_ApplyValues9And1And0Then44(void)
{
    SceneData_Place2(9, 1, 0);
    SceneData_Do2(0x44);
}

void State_ApplyValues10And2And0Then58(void)
{
    SceneData_Place3(0xA, 2, 0);
    SceneData_Do3(0x58);
}

void State_ApplyValues11And3And0Then6c(void)
{
    SceneData_Place4(0xB, 3, 0);
    SceneData_Do4(0x6C);
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
s32 Scene_RunEmptyScene(void)
{
    return 0;
}
