#include "resource_388.h"
#include "resource_388_runtime.h"

#define Resource388_Run Func_02000054

s32 Resource388_Run(void) {
    struct Resource388Runtime *runtime;
    /* slot_state_byte, command_arg5 and command_arg6 look gratuitous but are
       load-bearing: they keep the values out of the immediate operands of the
       stores/call so the allocator gives them their own registers. */
    s8 slot_state_byte;
    s32 command_arg5;
    s32 command_arg6;
    /* Likewise position_x/position_z: initialising them here (outside the
       call's basic block) stops the call expander from pre-copying the two
       24-bit constants ahead of the r0 argument. */
    s32 position_x = 0xD80000;
    s32 position_z = 0x880000;

    runtime = RESOURCE388_RUNTIME;
    runtime->setup_request_1c0 = 0x204;
    runtime->setup_value_1c8 = 0x18;
    if (Resource388_TestSetupGate(0x300) != 0) {
        Resource388_SetSlotPosition(8, position_x, position_z);
        Resource388_SetSlotMode(8, 2);
        Resource388_SetSlotOption(Resource388_LookupSlotHandle(8), 0);
        Resource388_GetSlotRecord(8)->unknown_23 = 2;
        slot_state_byte = 0;
        Resource388_GetSlotState(8)->unknown_59 = slot_state_byte;
        command_arg5 = 0xB;
        command_arg6 = 6;
        Resource388_QueueSlotCommand(0xB, 0x24, 5, 5, command_arg5, command_arg6);
    }
    return 0;
}
