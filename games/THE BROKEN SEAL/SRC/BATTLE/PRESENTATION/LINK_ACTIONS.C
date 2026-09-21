#include "TYPES.H"
extern volatile u16 Data_03001f64;
extern volatile u16 Data_02002238;
u32 Func_080022f4(u32, u32);
#define Math_Mod Func_080022f4
s32 SerialRuntime_BeginTransferA(void *, s32);
s32 SerialRuntime_BeginTransferB(void *);
s32 SerialRuntime_GetActiveTransfers(void);
void WaitFrames(s32);


struct BattleLinkedAction {
    s16 unit_id;
    u16 source_id;
    u16 flags;
    u16 unknown_06;
    u16 unknown_08;
    u16 kind;
    u8 unknown_0c[4];
};

struct BattleLinkedActionState {
    s32 count;
    s32 marker;
    s32 display_table;
    u8 unknown_0c[28];
};

s32 BattlePresentation_AppendLinkedActions(
    struct BattleLinkedAction *actions, s32 count)
{
    u8 *battle = *(u8 **)0x03001e74;
    s32 result = 0;
    s32 allocation_size = Math_Mod(count * 16 + 19, 20) * 20;
    struct BattleLinkedActionState *state = Runtime_BumpAllocateAlternatePool(40);
    s32 index;


    s32 BattleLink_SendActions(void)
    {
        s32 timeout;
        s32 disconnected;
        s32 status = SerialRuntime_BeginTransferA(state, 20);
        disconnected = 0;
        timeout = 300;
        if (status == -1) return status;
        while (SerialRuntime_GetActiveTransfers()) {
            WaitFrames(1);
            if (--timeout < 0) return -1;
            if ((Data_03001f64 & 3) != 3) {
                if (++disconnected > 24) return -1;
            } else disconnected = 0;
        }
        if (allocation_size) {
            status = SerialRuntime_BeginTransferA(actions, allocation_size);
            if (status == -1) return status;
            while (SerialRuntime_GetActiveTransfers()) {
            WaitFrames(1);
            if (--timeout < 0) return -1;
            if ((Data_03001f64 & 3) != 3) {
                if (++disconnected > 24) return -1;
            } else disconnected = 0;
            }
        }
        return 0;
    }
    s32 BattleLink_ReceiveActions(void)
    {
        s32 timeout;
        s32 disconnected;
        s32 status = SerialRuntime_BeginTransferB(state);
        disconnected = 0;
        timeout = 300;
        if (status == -1) return status;
        while (SerialRuntime_GetActiveTransfers()) {
            if (Data_02002238 > 20) return -1;
            WaitFrames(1);
            if (--timeout < 0) return -1;
            if ((Data_03001f64 & 3) != 3) {
                if (++disconnected > 24) return -1;
            } else disconnected = 0;
        }
        if (Data_02002238 != 20) return -1;
        result = state->count;
        if (state->count) {
            status = SerialRuntime_BeginTransferB(actions + count);
            if (status == -1) return status;
            while (SerialRuntime_GetActiveTransfers()) {
                if (Data_02002238 > Math_Mod(result * 16 + 19, 20) * 20) return -1;
            WaitFrames(1);
            if (--timeout < 0) return -1;
            if ((Data_03001f64 & 3) != 3) {
                if (++disconnected > 24) return -1;
            } else disconnected = 0;
            }
            if (Data_02002238 != Math_Mod(result * 16 + 19, 20) * 20) return -1;
        }
        return 0;
    }

    for (index = 0; index < count; index++) {
        struct BattleLinkedAction *action = &actions[index];

        action->source_id = battle[action->unit_id + 72];
        if (battle[80] == 0) {
            if (action->flags & 1) {
                action->flags++;
            }
        } else {
            action->flags |= 1;
        }
    }

    if (battle[82] == 0) {
        if (battle[80] == 0) {
            u16 interrupt_enable;

            state->count = count;
            state->marker = Func_080771a0();
            interrupt_enable = *(u16 *)0x04000208;
            *(u16 *)0x04000208 = 0x0208;
            state->display_table = *(s32 *)0x03001cb4;
            *(s32 *)0x020023a8 = state->display_table;
            *(u16 *)0x04000208 = interrupt_enable;
            if (BattleLink_SendActions() < 0 || BattleLink_ReceiveActions() < 0) {
                goto fail;
            }
            result = state->count;
        } else {
            if (BattleLink_ReceiveActions() < 0) {
                goto fail;
            }
            result = state->count;
            state->count = count;
            if (BattleLink_SendActions() < 0) {
                goto fail;
            }
            if (Func_080771a0() != state->marker) {
                goto fail;
            }
            *(s32 *)0x020023a8 = state->display_table;
        }

        for (index = 0; index < result; index++) {
            struct BattleLinkedAction *action = &actions[count + index];

            action->unit_id = action->source_id;
            action->kind ^= 0x80;
        }

        Func_08002df0(state);
        return result;
    }

fail:
    BattleLink_ResetTransferState();
    SerialRuntime_RemoveIrqHandlers();
    Func_08002df0(state);
    return -1;
}
