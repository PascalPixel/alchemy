#include "RUNTIME_MEM.H"
/* Draft of LinkLobby_SendPartyRecords, resource_3cb at 0x02008580, for
 * MENU/LINK_LOBBY (beside RECEIVE_PARTY.C). Linking it needs the import
 * veneer to SerialRuntime_BeginTransferAFar labelled
 * SerialRuntime_BeginTransferA.
 * Remaining difference: score 3360 (42 register-only, 14 operand, 17
 * reordered). The frame, size variable and control flow match; the game
 * keeps the slot in r7 and the retries in r5 (mine swaps them), sets the
 * 600-frame timeout during the clear loop, walks the offer items with a
 * pointer beside the index and reads the count through the item base, and
 * tests the renumbered owner with lsls #24. Tried: s8 owner and place,
 * the store as the test, declaration order, and three minutes of permute
 * (2475 with dozens of forced rewrites). */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "LOBBY.H"
extern u8 gLinkStatus[];

s32 SerialRuntime_BeginTransferA(s32 data, s32 size);
s32 SerialRuntime_GetActiveTransfers(void);
u8 *Owner_GetState(s32 owner);
u8 *Trade_GetOfferState(s32 mode);
void Engine_TaskWait(s32 frames);

/* The offer block sent after the party: a list of items, each naming the
   party member that holds it. */
struct OfferItem {
    u8 unknown_0[2];
    u8 owner;
    u8 unknown_3;
};

struct Offer {
    u8 unknown_000[8];
    struct OfferItem items[64];
    s32 count;
    u8 unknown_10c[0x34];
};

/* Sends up to three party members' records (0x154-byte transfers, blank
 * ones filling the rest), then the offer block with each item's owner
 * renumbered to the member's place in the sent party and the items of
 * members left behind dropped. Each transfer waits at most 600 frames in
 * total and 24 unready polls. Returns 0, or -1 when the link fails. */
s32 LinkLobby_SendPartyRecords(void)
{
    u16 members[8];
    s8 place[8];
    u32 size;
    s32 heap;
    s32 result;
    s32 count;
    s32 timeout;
    s32 slot;
    s32 tries;
    s32 i;
    s32 n;
    s32 ret;
    struct OfferItem *from;
    struct Offer *offer;
    struct OfferItem *item;
    s8 owner;

    size = 0x154;
    heap = (s32)Runtime_BumpAllocateAlternatePool(size);
    result = 0;
    count = SceneData_CopyUpToThreeEntries(members);
    for (i = 7; i >= 0; i--)
        place[i] = 0;
    timeout = 600;
    for (slot = 0; slot < count; slot++) {
        Iwram_CopyWords((void *)heap, Owner_GetState(members[slot]), 0x154);
        ((u8 *)heap)[0x12a] = 2;
        place[members[slot]] = slot - 128;
        if ((ret = SerialRuntime_BeginTransferA(heap, 0x154)) == -1) {
            result = ret;
            goto done;
        }
        tries = 0;
        while (SerialRuntime_GetActiveTransfers() != 0) {
            Engine_TaskWait(1);
            if (--timeout < 0 || (*(volatile u16 *)gLinkStatus & 3) != 3) {
                if (++tries > 24)
                    goto failed;
            }
        }
        Engine_TaskWait(2);
    }
    goto next;
wait:
    Engine_TaskWait(1);
    if (--timeout < 0 || (*(volatile u16 *)gLinkStatus & 3) != 3) {
        if (++tries > 24) {
            result = -1;
            goto done;
        }
    }
test:
    if (SerialRuntime_GetActiveTransfers() != 0)
        goto wait;
    Engine_TaskWait(2);
    slot++;
next:
    if (slot <= 2) {
        ((u8 *)heap)[0x12a] = 0;
        tries = 0;
        if ((ret = SerialRuntime_BeginTransferA(heap, 0x154)) == -1) {
            result = ret;
            goto done;
        }
        goto test;
    }
    Runtime_BumpFree(heap);
    size = 0x140;
    heap = (s32)Runtime_BumpAllocateAlternatePool(size);
    Iwram_CopyWords((void *)heap, Trade_GetOfferState(0), size);
    offer = (struct Offer *)heap;
    tries = 0;
    timeout = 600;
    item = offer->items;
    for (i = 0; i < offer->count; i++) {
        item->owner = place[item->owner];
        if (item->owner == 0) {
            if (i < offer->count - 1) {
                n = offer->count - 1 - i;
                from = &offer->items[i];
                do {
                    from[0] = from[1];
                    from++;
                } while (--n != 0);
            }
            offer->count--;
            item--;
            i--;
        }
        item++;
    }
    if ((ret = SerialRuntime_BeginTransferA(heap, 0x140)) == -1) {
        result = ret;
        goto done;
    }
    while (SerialRuntime_GetActiveTransfers() != 0) {
        Engine_TaskWait(1);
        if (--timeout < 0 || (*(volatile u16 *)gLinkStatus & 3) != 3) {
            if (++tries > 24)
                goto failed;
        }
    }
    Engine_TaskWait(1);
    Engine_TaskWait(2);
    goto done;
failed:
    result = -1;
done:
    Runtime_BumpFree(heap);
    return result;
}
