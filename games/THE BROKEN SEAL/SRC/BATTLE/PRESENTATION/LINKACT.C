#include "TYPES.H"
#include "SYSTEM.H"
#include "BATTLE_WORK.H"
#include "BATTLE_RANDOM.H"
#include "BATTLE_RUNTIME.H"

extern s32 Data_03001cb4;
extern volatile u16 gLinkStatus;
extern volatile u16 gSerialReceivedSize;

void *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_BumpFree(void *block);
s32 SerialRuntime_BeginTransferA(void *data, s32 size);
s32 SerialRuntime_BeginTransferB(void *data);
s32 SerialRuntime_GetActiveTransfers(void);
void SerialRuntime_RemoveIrqHandlers(void);
void BattleLink_ResetTransferState(void);

/* One queued action as the two sides of a linked battle exchange it. */
struct BattleLinkedAction {
    s16 unit_id;
    u16 source_id;
    u16 flags;
    u16 unknown_06;
    u16 unknown_08;
    u16 kind;
    u8 unknown_0c[4];
};

/* The header sent ahead of the actions: how many follow, a random check
 * value both sides must agree on, and the seed the battle continues from. */
struct BattleLinkedActionState {
    s32 count;
    s32 check;
    s32 seed;
    u8 unknown_0c[28];
};

/* Exchange this turn's actions with the other side of a linked battle.
 * The party's actions are tagged with their member slots and sent in
 * 20-byte blocks behind a header; the other side's are received behind
 * `actions` and renamed to this side's unit ids. The side that leads sends
 * first and gives the seed; the other receives first and checks it. Returns
 * the number of actions received, or -1 once the link fails. */
s32 BattlePresentation_AppendLinkedActions(struct BattleLinkedAction *actions, s32 count)
{
    struct BattleSession *battle = gBattleWork;
    s32 result = 0;
    s32 allocation_size = (u32)(count * 16 + 19) / 20 * 20;
    struct BattleLinkedActionState *state = Runtime_BumpAllocateAlternatePool(40);
    s32 index;

    /* Send the header, then the actions; give up after 300 frames or 25
     * frames without both link lines up. */
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

    /* Receive the header, then as many actions as it announces. */
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
        struct BattleLinkedAction *action = &actions[index];

        action->source_id = battle->owner_slots[action->unit_id];
        /* The side is read through a byte pointer each pass, as the
         * reference reads it. */
        if (*(u8 *)&battle->link_side == 0) {
            if (action->flags & 1)
                action->flags++;
        } else {
            action->flags |= 1;
        }
    }

    if (battle->link_paused == 0) {
        if (battle->link_side == 0) {
            volatile u16 *ime;
            u32 saved;

            state->count = count;
            state->check = BattleRandom16Far();
            /* FAKEMATCH: the interrupt master is saved and cleared inside
             * a one-pass loop, as QueueTransfer and the serial runtime do:
             * its notes keep the seed copy between the two writes and make
             * the state pointer reload after the first. */
            do {
                ime = (volatile u16 *)0x04000208;
                saved = *ime;
                *ime = (u16)(u32)ime;
            } while (0);
            gBattleRandomSeed = state->seed = Data_03001cb4;
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
            if (BattleRandom16Far() != state->check)
                goto fail;
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
