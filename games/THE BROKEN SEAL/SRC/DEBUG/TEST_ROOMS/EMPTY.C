#include "TYPES.H"
#include "SCENE.H"

void BattleFx_RunPageEffectForSlot(s32 slot, s32 page, s32 effect);
void Engine_GameFlagClear(s32 flag);

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
    BattleFx_RunPageEffectForSlot(8, 0, 0);
    Engine_GameFlagClear(0x30);
}

void State_ApplyValues9And1And0Then44(void)
{
    BattleFx_RunPageEffectForSlot(9, 1, 0);
    Engine_GameFlagClear(0x44);
}

void State_ApplyValues10And2And0Then58(void)
{
    BattleFx_RunPageEffectForSlot(0xA, 2, 0);
    Engine_GameFlagClear(0x58);
}

void State_ApplyValues11And3And0Then6c(void)
{
    BattleFx_RunPageEffectForSlot(0xB, 3, 0);
    Engine_GameFlagClear(0x6C);
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
