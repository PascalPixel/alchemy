#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene.h"
#include "field_event.h"

void BattleFx_RunPageEffectForSlot(s32 slot, s32 page, s32 effect);


u8 *TestRooms_GetEntrances(void)
{
    return TestRooms_Entrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *TestRooms_GetExits(void)
{
    return TestRooms_Exits;
}

u8 *TestRooms_GetPlacements(void)
{
    return TestRooms_Placements;
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

u8 *TestRooms_GetEvents(void)
{
    return TestRooms_Events;
}

s32 Scene_RunEmptyScene(void)
{
    return 0;
}
