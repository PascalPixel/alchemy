/* Near miss (12 of 384 instructions differ, all load scheduling): the
 * Broken Seal link exchange with its two nested transfers, ported with
 * TLA names; four loads sit one slot from the native. */
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
#include "mem.h"
#include "system.h"
#include "btlwork.h"
#include "battle_random.h"
#include "btlsys.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "serial_runtime.h"


s32 Func_0801680c(void *source, s32 size);
s32 SerialRuntime_GetActiveTransfers(void);
void MemFree(void *block);

extern volatile u16 gLinkStatus;

struct BattleLinkedActionState {
    s32 count;
    s32 check;
    s32 seed;
    u8 unknown_0c[28];
};

s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions, s32 count)
{
    struct BtlWork *battle = Ram_HeapSlots->battle_work;
    s32 result = 0;
    s32 allocation_size = (u32)(count * 16 + 19) / 20 * 20;
    struct BattleLinkedActionState *state = (struct BattleLinkedActionState *)Runtime_BumpAllocateAlternatePool(40);
    s32 index;

    s32 BattleLink_SendActions(void)
    {
        s32 timeout;
        s32 disconnected;
        s32 status = Func_0801680c(state, 20);

        disconnected = 0;
        timeout = 300;
        if (status == -1)
            return status;
        while (SerialRuntime_GetActiveTransfers()) {
            WaitFrames(1);
            if (--timeout < 0)
                return -1;
            if ((gLinkStatus & 3) != 3) {
                if (++disconnected > 24)
                    return -1;
            } else {
                disconnected = 0;
            }
        }
        if (allocation_size) {
            status = Func_0801680c(actions, allocation_size);
            if (status == -1)
                return status;
            while (SerialRuntime_GetActiveTransfers()) {
                WaitFrames(1);
                if (--timeout < 0)
                    return -1;
                if ((gLinkStatus & 3) != 3) {
                    if (++disconnected > 24)
                        return -1;
                } else {
                    disconnected = 0;
                }
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
        if (status == -1)
            return status;
        while (SerialRuntime_GetActiveTransfers()) {
            if (gSerialReceivedSize > 20)
                return -1;
            WaitFrames(1);
            if (--timeout < 0)
                return -1;
            if ((gLinkStatus & 3) != 3) {
                if (++disconnected > 24)
                    return -1;
            } else {
                disconnected = 0;
            }
        }
        if (gSerialReceivedSize != 20)
            return -1;
        result = state->count;
        if (state->count) {
            status = SerialRuntime_BeginTransferB(actions + count);
            if (status == -1)
                return status;
            while (SerialRuntime_GetActiveTransfers()) {
                if (gSerialReceivedSize > (u32)(result * 16 + 19) / 20 * 20)
                    return -1;
                WaitFrames(1);
                if (--timeout < 0)
                    return -1;
                if ((gLinkStatus & 3) != 3) {
                    if (++disconnected > 24)
                        return -1;
                } else {
                    disconnected = 0;
                }
            }
            if (gSerialReceivedSize != (u32)(result * 16 + 19) / 20 * 20)
                return -1;
        }
        return 0;
    }

    for (index = 0; index < count; index++) {
        struct BattleActionRecord *action = &actions[index];

        action->slot = battle->owner_slots[action->unit_id];

        if (battle->link_side == 0) {
            if (action->priority & 1)
                action->priority++;
        } else {
            action->priority |= 1;
        }
    }

    if (battle->link_paused == 0) {
        if (battle->link_side == 0) {
            volatile u16 *ime;
            u32 saved;

            state->count = count;
            state->check = BtlRndFar();
            /* FAKEMATCH: the one-pass IME scope keeps the seed copy between writes and reloads state after the first write. */
            do {
                ime = &REG_IME;
                saved = *ime;
                *ime = (u32)ime;
            } while (0);
            gBattleRandomSeed = state->seed = gRandomState;
            do { /* FAKEMATCH: the one-pass restore, see above */
                *ime = saved;
            } while (0);
            if (BattleLink_SendActions() < 0 || BattleLink_ReceiveActions() < 0)
                goto fail;
            result = state->count;
        } else {
            if (BattleLink_ReceiveActions() < 0)
                goto fail;
            result = state->count;
            state->count = count;
            if (BattleLink_SendActions() < 0)
                goto fail;
            if (BtlRndFar() != state->check)
                goto fail;
            gBattleRandomSeed = state->seed;
        }

        for (index = 0; index < result; index++) {
            struct BattleActionRecord *action = &actions[count + index];

            action->unit_id = action->slot;
            action->target = action->target ^ 0x80;
        }

        MemFree(state);
        return result;
    }

fail:
    BattleLink_ResetTransferState();
    SerialRuntime_RemoveIrqHandlers();
    MemFree(state);
    return -1;
}
