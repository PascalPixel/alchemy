/*
 * Draft: BattlePresentation_AppendLinkedActions does not yet match; its raw assembly links in its place.
 * The nested functions it contains match and link as their compiler's own
 * assembly (recon/tbs/raw/080b9554.s).
 * 2026-09-29: the parent's allocation size divides with the operator
 * (the reference calls __udivsi3), the interrupt master is saved and
 * disabled by writing its own address as the link lobby does, and the
 * seed word is Data_03001cb4 copied to gBattleRandomSeed. 188 of 195
 * instructions: the reference keeps the battle work in r9 and the
 * captured count's frame address in r7 (&result r8, &state sl); here the
 * count is addressed through sp until a later copy in r8, the work takes
 * r6, and the seed copy is scheduled after the IME restore. Declaring the
 * work later, register and index order do not move it.
 */
#include "TYPES.H"
#include "SYSTEM.H"
extern void *gBattleWork[];
extern s32 gBattleRandomSeed;
extern s32 Data_03001cb4;
extern volatile u16 gLinkStatus;
extern volatile u16 gSerialReceivedSize;
u32 Math_DivU(u32, u32);
s32 SerialRuntime_BeginTransferA(void *, s32);
s32 SerialRuntime_BeginTransferB(void *);
s32 SerialRuntime_GetActiveTransfers(void);

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
    s32 check;
    s32 seed;
    u8 unknown_0c[28];
};

s32 BattlePresentation_AppendLinkedActions(
    struct BattleLinkedAction *actions, s32 count)
{
    u8 *battle = (u8 *)gBattleWork[0];
    s32 result = 0;
    s32 allocation_size = (u32)(count * 16 + 19) / 20 * 20;
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
            if ((gLinkStatus & 3) != 3) {
                if (++disconnected > 24) return -1;
            } else disconnected = 0;
        }
        if (allocation_size) {
            status = SerialRuntime_BeginTransferA(actions, allocation_size);
            if (status == -1) return status;
            while (SerialRuntime_GetActiveTransfers()) {
            WaitFrames(1);
            if (--timeout < 0) return -1;
            if ((gLinkStatus & 3) != 3) {
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
            if (gSerialReceivedSize > 20) return -1;
            WaitFrames(1);
            if (--timeout < 0) return -1;
            if ((gLinkStatus & 3) != 3) {
                if (++disconnected > 24) return -1;
            } else disconnected = 0;
        }
        if (gSerialReceivedSize != 20) return -1;
        result = state->count;
        if (state->count) {
            status = SerialRuntime_BeginTransferB(actions + count);
            if (status == -1) return status;
            while (SerialRuntime_GetActiveTransfers()) {
                if (gSerialReceivedSize > Math_DivU(result * 16 + 19, 20) * 20) return -1;
            WaitFrames(1);
            if (--timeout < 0) return -1;
            if ((gLinkStatus & 3) != 3) {
                if (++disconnected > 24) return -1;
            } else disconnected = 0;
            }
            if (gSerialReceivedSize != Math_DivU(result * 16 + 19, 20) * 20) return -1;
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
            volatile u16 *ime;
            u32 saved;

            state->count = count;
            state->check = BattleRandom16Far();
            ime = (volatile u16 *)0x04000208;
            saved = *ime;
            *ime = (u16)(u32)ime;
            gBattleRandomSeed = state->seed = Data_03001cb4;
            *ime = saved;
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
            if (BattleRandom16Far() != state->check) {
                goto fail;
            }
            gBattleRandomSeed = state->seed;
        }

        for (index = 0; index < result; index++) {
            struct BattleLinkedAction *action = &actions[count + index];

            action->unit_id = action->source_id;
            action->kind ^= 0x80;
        }

        Runtime_BumpFree(state);
        return result;
    }

fail:
    BattleLink_ResetTransferState();
    SerialRuntime_RemoveIrqHandlers();
    Runtime_BumpFree(state);
    return -1;
}
