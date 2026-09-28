/*
 * Overlay resource_388: in-image table getters and the scene entry that
 * places actor 8 once the gate is open.
 */

#include "TYPES.H"
#include "SCENE.H"
extern struct Resource388Runtime *gEventWork;

/* The scene's four tables, in the overlay's read-only data. */
extern u8 GomaIke_SceneTable0[];
extern u8 GomaIke_SceneTable1[];
extern u8 GomaIke_SceneTable2[];
extern u8 GomaIke_SceneTable3[];
#include "RESOURCE_388_RUNTIME.H"

extern void *Object_GetById(u32);

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *SceneData_GetPrimaryTable(void)
{
    return GomaIke_SceneTable0;
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetSecondaryTable(void)
{
    return GomaIke_SceneTable1;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTertiaryTable(void)
{
    return GomaIke_SceneTable2;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetQuaternaryTable(void)
{
    return GomaIke_SceneTable3;
}

s32 Scene_PlaceActor8OnGate300(void)
{
    struct Resource388Runtime *work;
    /*
     * state, arg5 and arg6 must stay locals: they keep the values out of the
     * immediate operands of the stores and the call, so each gets its own
     * register.
     */
    s8 state;
    s32 arg5;
    s32 arg6;
    /*
     * pos_x and pos_z likewise. Initialising them here, outside the call,
     * keeps the two wide constants from being copied ahead of the first
     * argument.
     */
    s32 pos_x = 0xD80000;
    s32 pos_z = 0x880000;

    work = gEventWork;
    work->setup_request_1c0 = 0x204;
    work->setup_value_1c8 = 0x18;
    if (Resource388_TestSetupGate(0x300) != 0) {
        Resource388_SetSlotPosition(8, pos_x, pos_z);
        Resource388_SetSlotMode(8, 2);
        Resource388_SetSlotOption((s32)Object_GetById(8), 0);
        ((struct Resource388SlotView *)Object_GetById(8))->unknown_23 = 2;
        state = 0;
        ((struct Resource388SlotView *)Object_GetById(8))->unknown_59 = state;
        arg5 = 0xB;
        arg6 = 6;
        Resource388_QueueSlotCommand(0xB, 0x24, 5, 5, arg5, arg6);
    }
    return 0;
}
