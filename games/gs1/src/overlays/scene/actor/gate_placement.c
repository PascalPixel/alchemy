/*
 * Overlay resource_388: in-image table getters and the scene entry that
 * places actor 8 once the gate is open.
 */

#include "types.h"

#define SceneData_GetPrimaryTable Func_02000030
#define SceneData_ReturnZero Func_02000038
#define SceneData_GetSecondaryTable Func_0200003c
#define SceneData_GetTertiaryTable Func_02000044
#define SceneData_GetQuaternaryTable Func_0200004c
#define FieldScene_PlaceActor8OnGate300 Func_02000054
#include "resource_388.h"
#include "resource_388_runtime.h"

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *SceneData_GetPrimaryTable(void)
{
    return RESOURCE388_PRIMARY_TABLE;   /* image offset 0x108 */
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetSecondaryTable(void)
{
    return RESOURCE388_SECONDARY_TABLE;   /* image offset 0x180 */
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetTertiaryTable(void)
{
    return RESOURCE388_TERTIARY_TABLE;   /* image offset 0x194 */
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetQuaternaryTable(void)
{
    return RESOURCE388_QUATERNARY_TABLE;   /* image offset 0x1c4 */
}

s32 FieldScene_PlaceActor8OnGate300(void)
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

    work = RESOURCE388_RUNTIME;
    work->setup_request_1c0 = 0x204;
    work->setup_value_1c8 = 0x18;
    if (Resource388_TestSetupGate(0x300) != 0) {
        Resource388_SetSlotPosition(8, pos_x, pos_z);
        Resource388_SetSlotMode(8, 2);
        Resource388_SetSlotOption(Resource388_LookupSlotHandle(8), 0);
        Resource388_GetSlotRecord(8)->unknown_23 = 2;
        state = 0;
        Resource388_GetSlotState(8)->unknown_59 = state;
        arg5 = 0xB;
        arg6 = 6;
        Resource388_QueueSlotCommand(0xB, 0x24, 5, 5, arg5, arg6);
    }
    return 0;
}
