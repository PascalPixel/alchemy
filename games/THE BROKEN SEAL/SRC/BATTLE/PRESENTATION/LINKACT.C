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
#include "MEM.H"
#include "SYSTEM.H"
#include "BTLWORK.H"
#include "BATTLE_RANDOM.H"
#include "BTLSYS.H"
#include "IO_REG.H"
#include "SERIAL_RUNTIME.H"

#define gRandomSeed (*(s32 *)&gRandomSeed)

extern volatile u16 gLinkStatus;

struct BattleLinkedActionState {
    s32 count;
    s32 check;
    s32 seed;
    u8 unknown_0c[28];
};

s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions, s32 count)
{
    struct BtlWork *battle = BtlWk;
    s32 result = 0;
    s32 allocation_size = (u32)(count * 16 + 19) / 20 * 20;
    struct BattleLinkedActionState *state = (struct BattleLinkedActionState *)Runtime_BumpAllocateAlternatePool(40);
    s32 index;

    s32 BattleLink_SendActions(void)
    {
        s32 timeout;
        s32 disconnected;
        s32 status = SerialRuntime_BeginTransferA(state, 20);

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
            status = SerialRuntime_BeginTransferA(actions, allocation_size);
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
            /* FAKEMATCH: needed to match: the one-pass IME scope keeps the seed copy between writes and reloads state after the first write */
            do {
                ime = &REG_IME;
                saved = *ime;
                *ime = (u32)ime;
            } while (0);
            gBattleRandomSeed = state->seed = gRandomSeed;
            do { /* FAKEMATCH: needed to match: the one-pass restore, see above */
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

        Runtime_BumpFree(state);
        return result;
    }

fail:
    BattleLink_ResetTransferState();
    SerialRuntime_RemoveIrqHandlers();
    Runtime_BumpFree(state);
    return -1;
}
