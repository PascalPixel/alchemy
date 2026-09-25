/* NONMATCHING: 560 bytes, candidate 548, 262 differing halfwords
 * (2026-09-25). LinkLobby_SendPartyRecords, the sender twin of
 * LinkLobby_ReceivePartyRecords (MENU/LINK_LOBBY/RECEIVE_PARTY.C), meant for
 * MENU/LINK_LOBBY/SEND_PARTY.C as a single-overlay unit binding Engine_* and
 * Main_* at their import veneers; the IWRAM word copy at 0x03001388 is an
 * ordinary call through a function pointer (_call_via_r3 links). Remaining:
 * the reference keeps three separate 'str r0, [sp, #4]; b done' failure
 * blocks after each Main_08000380 test, where this spelling lets them merge
 * into one, and the heap lives in r8 (here r7) with the index in r7; the
 * second phase counts retries in sl, the relink list is walked through heap +
 * 8 with its count at +0x100. */
#include "TYPES.H"

u8 *Main_08000170(u32 size);
void Main_08000178(u8 *heap);
s32 SceneData_CopyUpToThreeEntries(u16 *owners);
u8 *Engine_OwnerGetState(s32 owner);
s32 Main_08000380(u8 *data, u32 size);
void Engine_TaskWait(s32 frames);
s32 Main_080003a8(void);
u8 *Main_08077000(s32 mode);

typedef void (*WordCopyFn)(void *destination, const void *source, u32 size);

#define LINK_STAT (*(volatile u16 *)0x03001f64)

struct LinkEntry {
    u8 a;
    u8 b;
    u8 owner;
    u8 c;
};

struct LinkList {
    struct LinkEntry entries[64];
    s32 count;
};

s32 Local_02000580(void)
{
    s32 result;
    u8 *table;
    u16 owners[8];
    u8 slots[8];
    s32 ret;
    u8 *heap;
    s32 count;
    s32 timeout;
    s32 tries;
    s32 i;
    s32 j;
    s32 k;
    struct LinkList *list;

    heap = Main_08000170(0x154);
    result = 0;
    count = SceneData_CopyUpToThreeEntries(owners);
    table = slots;
    timeout = 600;
    for (i = 7; i >= 0; i--) {
        table[i] = 0;
    }
    for (i = 0; i < count; i++) {
        ((WordCopyFn)0x03001388)(heap, Engine_OwnerGetState(owners[i]), 0x154);
        heap[0x12a] = 2;
        table[owners[i]] = i - 128;
        tries = 0;
        if ((ret = Main_08000380(heap, 0x154)) == -1) {
            result = ret;
            goto done;
        }
        while (Main_080003a8() != 0) {
            timeout--;
            Engine_TaskWait(1);
            if (timeout < 0 || (LINK_STAT & 3) != 3) {
                if (++tries > 24) {
                    result = -1;
                    goto done;
                }
            }
        }
        Engine_TaskWait(2);
    }
    goto next;
wait:
    timeout--;
    Engine_TaskWait(1);
    if (timeout < 0 || (LINK_STAT & 3) != 3) {
        if (++tries > 24) {
            result = -1;
            goto done;
        }
    }
test:
    if (Main_080003a8() != 0) {
        goto wait;
    }
    Engine_TaskWait(2);
    i++;
next:
    if (i <= 2) {
        heap[0x12a] = 0;
        tries = 0;
        if ((ret = Main_08000380(heap, 0x154)) != -1) {
            goto test;
        }
        result = ret;
        goto done;
    }
    Main_08000178(heap);
    heap = Main_08000170(0x140);
    ((WordCopyFn)0x03001388)(heap, Main_08077000(0), 0x140);
    list = (struct LinkList *)(heap + 8);
    tries = 0;
    timeout = 600;
    for (j = 0; j < list->count; j++) {
        list->entries[j].owner = table[list->entries[j].owner];
        if ((s8)list->entries[j].owner == 0) {
            for (k = j; k < list->count - 1; k++) {
                list->entries[k] = list->entries[k + 1];
            }
            list->count--;
            j--;
        }
    }
    if ((ret = Main_08000380(heap, 0x140)) == -1) {
        result = ret;
        goto done;
    }
    while (Main_080003a8() != 0) {
        timeout--;
        Engine_TaskWait(1);
        if (timeout < 0 || (LINK_STAT & 3) != 3) {
            if (++tries > 24) {
                result = -1;
                goto done;
            }
        }
    }
    Engine_TaskWait(1);
    Engine_TaskWait(2);
done:
    Main_08000178(heap);
    return result;
}
