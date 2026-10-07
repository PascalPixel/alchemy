

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCENE.H"
extern struct Resource388Runtime *gEventWork;

#include "RESOURCE_388_RUNTIME.H"

extern void *Object_GetById(u32);

u8 *SceneData_GetPrimaryTable(void)
{
    return GomaIke_SceneTable0;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetSecondaryTable(void)
{
    return GomaIke_SceneTable1;
}

u8 *SceneData_GetTertiaryTable(void)
{
    return GomaIke_SceneTable2;
}

u8 *SceneData_GetQuaternaryTable(void)
{
    return GomaIke_SceneTable3;
}

s32 Scene_PlaceActor8OnGate300(void)
{
    struct Resource388Runtime *work;

    s8 state;
    s32 arg5;
    s32 arg6;
    s32 pos_x = 0xD80000;
    s32 pos_z = 0x880000;

    work = gEventWork;
    work->setup_request_1c0 = 516;
    work->setup_value_1c8 = 24;
    if (Resource388_TestSetupGate(768) != 0) {
        Resource388_SetSlotPosition(8, pos_x, pos_z);
        Resource388_SetSlotMode(8, 2);
        Resource388_SetSlotOption((s32)Object_GetById(8), 0);
        ((struct Resource388SlotView *)Object_GetById(8))->unknown_23 = 2;
        state = 0;
        ((struct Resource388SlotView *)Object_GetById(8))->unknown_59 = state;
        arg5 = 11;
        arg6 = 6;
        Resource388_QueueSlotCommand(11, 36, 5, 5, arg5, arg6);
    }
    return 0;
}
